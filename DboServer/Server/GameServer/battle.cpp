#include "stdafx.h"
#include "battle.h"
#include "calcs.h"
#include "Npc.h"
#include "CPlayer.h"
#include "FormulaTable.h"


bool BattleIsCrit(CCharacterAtt* pAttackerAtt, CCharacterAtt* pTargetAtt, bool bIsPhysical)
{
	float fRate = 0.0f;
	float fVar;
	float fVar2;
	float fCritRate;

	if (bIsPhysical == false)
	{
		fCritRate = (float)pAttackerAtt->GetEnergyCriticalRate();

		if (pAttackerAtt->GetFoc() < 170)
			fVar = 37.f;
		else
			fVar = 21.f;
			

		fVar2 = (float)pAttackerAtt->GetFoc();
	}
	else
	{
		fCritRate = (float)pAttackerAtt->GetPhysicalCriticalRate();

		if (pAttackerAtt->GetDex() < 220)
			fVar = 40.f;
		else
			fVar = 28.f;

		fVar2 = (float)pAttackerAtt->GetDex();
	}

	fRate = fCritRate - (fCritRate * (pTargetAtt->GetCriticalBlockSuccessRate() / 100.f)) + fVar2 / fVar;

	//if (0.0f >= fCritBlockRate)
	//{
	//	fRate = fCritRate;
	//}
	//else
	//{
	//	fRate = fCritRate - (fCritRate * (fCritBlockRate / 100.f));
	//}

	//NTL_PRINT(PRINT_APP, "fRate = %f, fVar = %f, fVar2 = %f, fCritRate = %f, CritBlockRate = %f", fRate, fVar, fVar2, fCritRate, pTargetAtt->GetCriticalBlockSuccessRate());

	if (fRate > 90.0f)
		fRate = 90.0f;

	return Dbo_CheckProbabilityF(fRate);
}

//--------------------------------------------------------------------------------------//
//		IS DODGED?
//--------------------------------------------------------------------------------------//
bool BattleIsDodge(bool bTargetPC, WORD hitrate, WORD dodge, BYTE byAttackerLv, BYTE byTargetLv)
{
	float fRate = 100.0f - (CFormulaTable::m_afRate[3700][1] * float((float)hitrate / (float)MAX(hitrate + dodge, 1)) * float(float(byAttackerLv + 1) / float(byAttackerLv + byTargetLv)) * 100.0f);

	if (fRate > 90.f)
		fRate = 90.0f;

	//if(bTargetPC)
	//	NTL_PRINT(PRINT_APP, "BattleIsDodge: dodge percent %f, hitrate %u, dodge %u, byAttackerLv %u, byTargetLv %u \n", fRate, hitrate, dodge, byAttackerLv, byTargetLv);

	return Dbo_CheckProbabilityF(fRate);
}


//--------------------------------------------------------------------------------------//
//		IS RESIST CURSE?
//--------------------------------------------------------------------------------------//
bool BattleIsResist(WORD wSuccessRate, WORD wResistRate, BYTE byAttackerLv, BYTE byTargetLv)
{
	float fRate = 100.0f - (CFormulaTable::m_afRate[3900][1] * float((float)wSuccessRate / (float)MAX(wSuccessRate + wResistRate, 1)) * float(float(byAttackerLv + 1) / float(byAttackerLv + byTargetLv)) * 100.0f);

	if (fRate > 90.f)
		fRate = 90.f;

	//NTL_PRINT(PRINT_APP, "BattleIsResist: curse resist percent %f, wSuccessRate %u, wResistRate %u, byAttackerLv %u, byTargetLv %u \n", fRate, wSuccessRate, wResistRate, byAttackerLv, byTargetLv);

	return Dbo_CheckProbabilityF(fRate);
}

bool BattleIsBlock(WORD wDefenceRate, BYTE byAttackerLv, BYTE byTargetLv)
{
	//float fRate = ((float)wDefenceRate - ((float)byTargetLv / ((float)byAttackerLv * 0.13f)) + 0.02f) / 3.5f;
	float fRate = ((float)wDefenceRate * 2.f + (byTargetLv - byAttackerLv)) / 200.f;
	if (fRate > 20.f)
		fRate = 20.f;

	//NTL_PRINT(PRINT_APP, "fRate:%f, wDefenceRate:%u, byAttackerLv:%u, byTargetLv:%u", fRate, wDefenceRate, byAttackerLv, byTargetLv);

	return Dbo_CheckProbabilityF(fRate);
}


//-----------------------------------------------------------------------------------------------------------//
//											CALCULATE SKILL DAMAGE
//-----------------------------------------------------------------------------------------------------------//

//--------------------------------------------------------------------------------------//
//		
//--------------------------------------------------------------------------------------//
void CalcSkillDamage(CCharacterObject* pCaster, CCharacterObject* victim, sSKILL_TBLDAT* skilltbl, BYTE byEffectNr, float fBaseSkillDmg, float& resultvalue, BYTE& rAttackResult, int& rfReflectDmg, sDBO_LP_EP_RECOVERED* pLpEpRecover, bool bIncreaseDmg/* = false*/, bool bAttackFromBehindBonus/* = false*/)
{
	CCharacterAtt* cCasterAtt = pCaster->GetCharAtt();
	CCharacterAtt* cVictimAtt = victim->GetCharAtt();

	float fFinalDamage = 0.0f;
	float min_damage = 0.0f;
	float max_damage = 0.0f;
	float fAttackerPower = 0.0f;
	float fTargetDefensePower = 0.0f;
	float fCritDefRate = 0.0f;

	bool bIsApplyTypeValue = skilltbl->bySkill_Effect_Type[byEffectNr] == SYSTEM_EFFECT_APPLY_TYPE_VALUE;
	bool bIsApplyTypePercent = skilltbl->bySkill_Effect_Type[byEffectNr] == SYSTEM_EFFECT_APPLY_TYPE_PERCENT;
	bool bIsSubWeapon = pCaster->IsPC() && skilltbl->byRequire_Epuip_Slot_Type == EQUIP_SLOT_TYPE_SUB_WEAPON;

	bool bIsSkillPhysical = skilltbl->bySkill_Type == NTL_SKILL_TYPE_PHYSICAL;
	bool bIsSkillEnergy = skilltbl->bySkill_Type == NTL_SKILL_TYPE_ENERGY;
	bool bIsSkillState = skilltbl->bySkill_Type == NTL_SKILL_TYPE_STATE;

	float fWeaponPhysicalOffence = bIsSubWeapon ? cCasterAtt->GetSubWeaponPhysicalOffence() : cCasterAtt->GetPhysicalOffence();
	float fWeaponEnergyOffence = bIsSubWeapon ? cCasterAtt->GetSubWeaponEnergyOffence() : cCasterAtt->GetEnergyOffence();

	/* INFO:
		- At "value" damage, we do not add weapon offence. See https://youtu.be/cj5E1dOIYfk?t=155 as proof. With weapon offence added we deal much more dmg. Without we deal exact the same damage. State needs to be figured out.
		- State calculation is / 1.5 instead of 2.0. Watch https://youtu.be/JlBDzAmoNTk?t=317 for more info.
	*/

	if (bIsApplyTypeValue || bIsApplyTypePercent)
	{
		if (bIsSkillPhysical)
		{
			fAttackerPower = bIsApplyTypeValue ? fBaseSkillDmg : fWeaponPhysicalOffence + ((fWeaponPhysicalOffence * fBaseSkillDmg) / 100.f);
			fTargetDefensePower = CalcDefensePower(cVictimAtt->GetPhysicalDefence(), cCasterAtt->GetPhysicalArmorPenRate());
			fCritDefRate = cVictimAtt->GetPhysicalCriticalDefenceRate();
		}
		else if (bIsSkillEnergy)
		{
			fAttackerPower = bIsApplyTypeValue ? fBaseSkillDmg : fWeaponEnergyOffence + ((fWeaponEnergyOffence * fBaseSkillDmg) / 100.f);
			fTargetDefensePower = CalcDefensePower(cVictimAtt->GetEnergyDefence(), cCasterAtt->GetEnergyArmorPenRate());
			fCritDefRate = cVictimAtt->GetEnergyCriticalDefenceRate();
		}
		else if (bIsSkillState)
		{
			float fDefense = (cVictimAtt->GetPhysicalDefence() + cVictimAtt->GetEnergyOffence()) / 1.5f;
			float fPenRate = (cCasterAtt->GetPhysicalArmorPenRate() + cCasterAtt->GetEnergyArmorPenRate()) / 2.f;
			float fWeaponStateOffence = (fWeaponPhysicalOffence + fWeaponEnergyOffence) / 1.5f;

			fAttackerPower = bIsApplyTypeValue ? fBaseSkillDmg + fWeaponStateOffence : (fWeaponStateOffence * fBaseSkillDmg) / 100.f;
			fTargetDefensePower = CalcDefensePower(fDefense, fPenRate);
			fCritDefRate = (cVictimAtt->GetPhysicalCriticalDefenceRate() + cVictimAtt->GetEnergyCriticalDefenceRate()) / 2.f;
		}
	}

	fAttackerPower = CalcPropBonus(pCaster, victim, fAttackerPower);

	float fDmg0 = fAttackerPower * (1.f - (fTargetDefensePower / (fTargetDefensePower + (float)pCaster->GetLevel() * CFormulaTable::m_afRate[3100][1])));
	min_damage = CalcMinDamage(fDmg0, pCaster->GetLevel());
	max_damage = CalcMaxDamage(fDmg0, pCaster->GetLevel());

	fFinalDamage = RandomRangeF(min_damage, max_damage);


	resultvalue = (fFinalDamage <= 1.f) ? 1.f : fFinalDamage;

	resultvalue = CalcCritBonus(resultvalue, cCasterAtt, skilltbl->bySkill_Type, rAttackResult, fCritDefRate, bIncreaseDmg);

	// Reflect Damage
	rfReflectDmg += (int)GetSkillReflectDamage(resultvalue, skilltbl->bySkill_Type, victim->GetCharAtt());

	//---------------//
	if (pLpEpRecover)
	{
		// LP Recover
		pLpEpRecover->targetLpRecoveredWhenHit = (int)(cVictimAtt->GetLpRecoveryWhenHit() + (resultvalue * cVictimAtt->GetLpRecoveryWhenHitInPercent() / 100.0f));
		pLpEpRecover->bIsLpRecoveredWhenHit = pLpEpRecover->targetLpRecoveredWhenHit > 0;

		// EP Recover
		pLpEpRecover->dwTargetEpRecoveredWhenHit = (DWORD)(cVictimAtt->GetEpRecoveryWhenHit() + (resultvalue * cVictimAtt->GetEpRecoveryWhenHitInPercent() / 100.0f));
		pLpEpRecover->bIsEpRecoveredWhenHit = pLpEpRecover->dwTargetEpRecoveredWhenHit > 0;
	}
}


//--------------------------------------------------------------------------------------//
//		
//--------------------------------------------------------------------------------------//
void CalcSpecialSkillDamage(CCharacterObject* pCaster, CCharacterObject* victim, sSKILL_TBLDAT* skilltbl, BYTE byEffectNr, float fBaseSkillDmg, float& resultvalue, BYTE& rAttackResult, int& rfReflectDmg, sDBO_LP_EP_RECOVERED& rLpEpRecover)
{
	CCharacterAtt* cCasterAtt = pCaster->GetCharAtt();
	CCharacterAtt* cVictimAtt = victim->GetCharAtt();

	float fFinalDamage = 0.0f;
	float min_damage = 0.0f;
	float max_damage = 0.0f;
	float fAttackerPower = 0.0f;
	float fTargetDefensePower = 0.0f;
	float fCritDefRate = 0.0f;
	float fCritDmgRate = 0.0f;

	bool bIsSubWeapon = pCaster->IsPC() && skilltbl->byRequire_Epuip_Slot_Type == EQUIP_SLOT_TYPE_SUB_WEAPON;
	bool bIsSkillPhysical = skilltbl->bySkill_Type == NTL_SKILL_TYPE_PHYSICAL;
	bool bIsSkillEnergy = skilltbl->bySkill_Type == NTL_SKILL_TYPE_ENERGY;
	bool bIsSkillState = skilltbl->bySkill_Type == NTL_SKILL_TYPE_STATE;

	float fWeaponPhysicalOffence = bIsSubWeapon ? cCasterAtt->GetSubWeaponPhysicalOffence() : cCasterAtt->GetPhysicalOffence();
	float fWeaponEnergyOffence = bIsSubWeapon ? cCasterAtt->GetSubWeaponEnergyOffence() : cCasterAtt->GetEnergyOffence();

	if (bIsSkillPhysical)
	{
		fAttackerPower = (fWeaponPhysicalOffence * (fBaseSkillDmg / 2.f)) / 100.f;
		fTargetDefensePower = CalcDefensePower(cVictimAtt->GetPhysicalDefence(), cCasterAtt->GetPhysicalArmorPenRate());
		fCritDefRate = cVictimAtt->GetPhysicalCriticalDefenceRate();
	}
	else if (bIsSkillEnergy)
	{
		fAttackerPower = (fWeaponEnergyOffence * (fBaseSkillDmg / 2.f)) / 100.f;
		fTargetDefensePower = CalcDefensePower(cVictimAtt->GetEnergyDefence(), cCasterAtt->GetEnergyArmorPenRate());
		fCritDefRate = cVictimAtt->GetEnergyCriticalDefenceRate();
	}
	else if (bIsSkillState)
	{
		float fDefense = (cVictimAtt->GetPhysicalDefence() + cVictimAtt->GetEnergyOffence()) / 2.f;
		float fPenRate = (cCasterAtt->GetPhysicalArmorPenRate() + cCasterAtt->GetEnergyArmorPenRate()) / 2.f;
		float fWeaponStateOffence = (fWeaponPhysicalOffence + fWeaponEnergyOffence) / 2.f;

		fAttackerPower = (fWeaponStateOffence * fBaseSkillDmg) / 100.f;
		fTargetDefensePower = CalcDefensePower(fDefense, fPenRate);
		fCritDefRate = (cVictimAtt->GetPhysicalCriticalDefenceRate() + cVictimAtt->GetEnergyCriticalDefenceRate()) / 2.f;
	}

	float fDmg0 = fAttackerPower * (1.f - (fTargetDefensePower / (fTargetDefensePower + (float)pCaster->GetLevel() * 15.f)));

	min_damage = CalcMinDamage(fDmg0, pCaster->GetLevel());
	max_damage = CalcMaxDamage(fDmg0, pCaster->GetLevel());

	fFinalDamage = RandomRangeF(min_damage, max_damage);

	resultvalue = (fFinalDamage <= 1.f) ? 1.f : fFinalDamage;

	resultvalue = CalcCritBonus(resultvalue, cCasterAtt, skilltbl->bySkill_Type, rAttackResult, fCritDefRate);

	// Reflect Damage
	rfReflectDmg += (int)GetSkillReflectDamage(resultvalue, skilltbl->bySkill_Type, cVictimAtt);

	//---------------//
	// LP Recover
	rLpEpRecover.targetLpRecoveredWhenHit += (int)(cVictimAtt->GetLpRecoveryWhenHit() + (resultvalue * cVictimAtt->GetLpRecoveryWhenHitInPercent() / 100.0f));
	rLpEpRecover.bIsLpRecoveredWhenHit = rLpEpRecover.targetLpRecoveredWhenHit > 0;

	// EP Recover
	rLpEpRecover.dwTargetEpRecoveredWhenHit += (DWORD)(cVictimAtt->GetEpRecoveryWhenHit() + (resultvalue * cVictimAtt->GetEpRecoveryWhenHitInPercent() / 100.0f));
	rLpEpRecover.bIsEpRecoveredWhenHit = rLpEpRecover.dwTargetEpRecoveredWhenHit > 0;
}


void CalcSkillDotDamage(CCharacterObject* pCaster, CCharacterObject* victim, sSKILL_TBLDAT* skilltbl, BYTE byEffectNr, WORD wDefence, float fBaseSkillDmg, float fBonusDmg, float& resultvalue, BYTE rAttackResult, BYTE byEffectCode)
{
	CCharacterAtt* cCasterAtt = pCaster->GetCharAtt();
	CCharacterAtt* cVictimAtt = victim->GetCharAtt();

	float  fFinalDamage = 0.0f;
	float min_damage = 0.0f;
	float max_damage = 0.0f;
	float fAttackerPower = 0.0f;
	float fCritDefRate = 0.0f;
	float fTargetDefensePower = wDefence;

	bool bIsApplyTypeValue = skilltbl->bySkill_Effect_Type[byEffectNr] == SYSTEM_EFFECT_APPLY_TYPE_VALUE;
	bool bIsApplyTypePercent = skilltbl->bySkill_Effect_Type[byEffectNr] == SYSTEM_EFFECT_APPLY_TYPE_PERCENT;
	bool bIsSubWeapon = pCaster->IsPC() && skilltbl->byRequire_Epuip_Slot_Type == EQUIP_SLOT_TYPE_SUB_WEAPON;

	bool bIsSkillPhysical = skilltbl->bySkill_Type == NTL_SKILL_TYPE_PHYSICAL;
	bool bIsSkillEnergy = skilltbl->bySkill_Type == NTL_SKILL_TYPE_ENERGY;
	bool bIsSkillState = skilltbl->bySkill_Type == NTL_SKILL_TYPE_STATE;

	float fWeaponPhysicalOffence = bIsSubWeapon ? cCasterAtt->GetSubWeaponPhysicalOffence() : cCasterAtt->GetPhysicalOffence();
	float fWeaponEnergyOffence = bIsSubWeapon ? cCasterAtt->GetSubWeaponEnergyOffence() : cCasterAtt->GetEnergyOffence();

	if (bIsApplyTypeValue)
	{
		fAttackerPower = fBaseSkillDmg;
		fFinalDamage = fAttackerPower * (1.f - (fTargetDefensePower / (fTargetDefensePower + (float)pCaster->GetLevel() * 25.f)));
	}
	else if (bIsApplyTypePercent)
	{
		if (bIsSkillPhysical)
		{
			fAttackerPower = (fWeaponPhysicalOffence * fBaseSkillDmg) / 100.f;

			fTargetDefensePower += cVictimAtt->GetPhysicalDefence();
			fTargetDefensePower = CalcDefensePower(fTargetDefensePower, cCasterAtt->GetPhysicalArmorPenRate());

			// critical dmg def
			fCritDefRate = cVictimAtt->GetPhysicalCriticalDefenceRate();
		}
		else if (bIsSkillEnergy)
		{
			fAttackerPower = (fWeaponEnergyOffence * fBaseSkillDmg) / 100.f;

			fTargetDefensePower += cVictimAtt->GetEnergyDefence();
			fTargetDefensePower = CalcDefensePower(fTargetDefensePower, cCasterAtt->GetEnergyArmorPenRate());

			// critical dmg def
			fCritDefRate = cVictimAtt->GetEnergyCriticalDefenceRate();
		}
		else if (bIsSkillState)
		{
			float fStateOffence;

			float fDefense = (cVictimAtt->GetPhysicalDefence() + cVictimAtt->GetEnergyOffence()) / 2.f;
			float fPenRate = (cCasterAtt->GetPhysicalArmorPenRate() + cCasterAtt->GetEnergyArmorPenRate()) / 2.f;
			float fWeaponStateOffence = (fWeaponPhysicalOffence + fWeaponEnergyOffence) / 2.f;

			fAttackerPower = (fWeaponStateOffence * fBaseSkillDmg) / 100.f;
			fTargetDefensePower += fDefense;
			fTargetDefensePower = CalcDefensePower(fTargetDefensePower, fPenRate);

			// critical dmg def
			fCritDefRate = (cVictimAtt->GetPhysicalCriticalDefenceRate() + cVictimAtt->GetEnergyCriticalDefenceRate()) / 2.f;
		}

		fFinalDamage = fAttackerPower * (1.f - (fTargetDefensePower / (fTargetDefensePower + (float)pCaster->GetLevel() * 35.f)));
	}

	fFinalDamage += fBonusDmg;

	if (victim->IsPC())
	{
		if (byEffectCode == ACTIVE_BLEED || byEffectCode == ACTIVE_BURN || wDefence < 1)
		{
			fFinalDamage -= (float)wDefence;
		}
		else
		{
			fFinalDamage -= (float)wDefence / 2.0f;
		}
	}

	resultvalue = (fFinalDamage <= 1.f) ? 1.f : fFinalDamage;

	resultvalue = CalcCritBonus(resultvalue, pCaster->GetCharAtt(), skilltbl->bySkill_Type, rAttackResult, fCritDefRate);
}


void CalcLifeStealDamage(CCharacterObject* pCaster, CCharacterObject* victim, sSKILL_TBLDAT* skilltbl, BYTE byEffectNr, float fBaseSkillDmg, float& resultvalue)
{
	CCharacterAtt* cCasterAtt = pCaster->GetCharAtt();
	CCharacterAtt* cVictimAtt = victim->GetCharAtt();

	float fFinalDamage = 0.0f;
	float fAttackerPower = 0.0f;
	float fTargetDefensePower = 0.0f;

	bool bIsSubWeapon = pCaster->IsPC() && skilltbl->byRequire_Epuip_Slot_Type == EQUIP_SLOT_TYPE_SUB_WEAPON;
	bool bIsSkillEnergy = skilltbl->bySkill_Type == NTL_SKILL_TYPE_ENERGY;

	float fWeaponPhysicalOffence = bIsSubWeapon ? cCasterAtt->GetSubWeaponPhysicalOffence() : cCasterAtt->GetPhysicalOffence();
	float fWeaponEnergyOffence = bIsSubWeapon ? cCasterAtt->GetSubWeaponEnergyOffence() : cCasterAtt->GetEnergyOffence();
	float fCasterOffence = bIsSkillEnergy ? fWeaponEnergyOffence : fWeaponPhysicalOffence;
	float fCasterArmorPenRate = bIsSkillEnergy ? cCasterAtt->GetEnergyArmorPenRate() : cCasterAtt->GetPhysicalArmorPenRate();
	float fVictimDefence = bIsSkillEnergy ? cVictimAtt->GetEnergyDefence() : cVictimAtt->GetPhysicalDefence();

	fAttackerPower = fBaseSkillDmg + fCasterOffence;
	fTargetDefensePower = CalcDefensePower(fVictimDefence, fCasterArmorPenRate);

	fFinalDamage = fAttackerPower * (1.f - (fTargetDefensePower / (fTargetDefensePower + (float)pCaster->GetLevel() * 35.f)));

	resultvalue = (fFinalDamage <= 1.f) ? 1.f : fFinalDamage;
}

//--------------------------------------------------------------------------------------//
//		CALCULATE NORMAL ATTACK DAMAGE
//--------------------------------------------------------------------------------------//
float CalcMeleeDamage(CCharacter* pkAttacker, CCharacter* pkVictim)
{
	CCharacterAtt* cCasterAtt = pkAttacker->GetCharAtt();
	CCharacterAtt* cVictimAtt = pkVictim->GetCharAtt();

	float fFinalDamage = 0.0f;
	float min_damage = 0.0f;
	float max_damage = 0.0f;
	float fAttackerPower = 0.0f;
	float fTargetDefensePower = 0.0f;

	bool bIsAttackEnergy = pkAttacker->GetAttackType() == BATTLE_ATTACK_TYPE_ENERGY;

	float fCasterOffence = bIsAttackEnergy ? cCasterAtt->GetEnergyOffence() : cCasterAtt->GetPhysicalOffence();
	float fCasterArmorPenRate = bIsAttackEnergy ? cCasterAtt->GetEnergyArmorPenRate() : cCasterAtt->GetPhysicalArmorPenRate();
	float fVictimDefence = bIsAttackEnergy ? cVictimAtt->GetEnergyDefence() : cVictimAtt->GetPhysicalDefence();

	fAttackerPower = fCasterOffence;
	fTargetDefensePower = CalcDefensePower(fVictimDefence, fCasterArmorPenRate);

	fAttackerPower = CalcPropBonus(pkAttacker, pkVictim, fAttackerPower);

	float fDmg0 = fAttackerPower * (1.f - (fTargetDefensePower / (fTargetDefensePower + (float)pkAttacker->GetLevel() * CFormulaTable::m_afRate[3100][1])));

	min_damage = CalcMinDamage(fDmg0, pkAttacker->GetLevel());
	max_damage = CalcMaxDamage(fDmg0, pkAttacker->GetLevel());

	fFinalDamage = RandomRangeF(min_damage, max_damage);

	return (fFinalDamage <= 1.f) ? 1.f : fFinalDamage;
}

void CalcDirectHeal(CCharacterObject * pCaster, sSKILL_TBLDAT * skilltbl, BYTE byEffectNr, float & resultvalue)
{
	resultvalue = (float)skilltbl->aSkill_Effect_Value[byEffectNr];

	if (pCaster->IsPC() && skilltbl->byRequire_Epuip_Slot_Type == EQUIP_SLOT_TYPE_SUB_WEAPON)
	{
		//add weapon offence to healing power
		resultvalue += (float)pCaster->GetCharAtt()->GetSubWeaponEnergyOffence();

		// Add % heal bonus (offence * %)
		resultvalue += (float)pCaster->GetCharAtt()->GetSubWeaponEnergyOffence() * pCaster->GetCharAtt()->GetDirectHealPowerBonusInPercent() / 100.f;
	}
	else
	{
		//add weapon offence to healing power
		resultvalue += (float)pCaster->GetCharAtt()->GetEnergyOffence();

		// Add % heal bonus (offence * %)
		resultvalue += (float)pCaster->GetCharAtt()->GetEnergyOffence() * pCaster->GetCharAtt()->GetDirectHealPowerBonusInPercent() / 100.f;
	}

	// Add Static Bonus
	resultvalue += pCaster->GetCharAtt()->GetDirectHealPowerBonus();

	//NTL_PRINT(PRINT_APP,"resultvalue %f, GetSubWeaponEnergyOffence %u, GetEnergyOffence %u, GetDirectHealPowerBonusInPercent %f, GetDirectHealPowerBonus %f \n", 
	//	resultvalue, pCaster->GetCharAtt()->GetSubWeaponEnergyOffence(), pCaster->GetCharAtt()->GetEnergyOffence(), pCaster->GetCharAtt()->GetDirectHealPowerBonusInPercent(), pCaster->GetCharAtt()->GetDirectHealPowerBonus());
}


void CalcHealOverTime(CCharacterObject * pCaster, sSKILL_TBLDAT * skilltbl, BYTE byEffectNr, float & resultvalue)
{
	resultvalue = (float)skilltbl->aSkill_Effect_Value[byEffectNr];

	if (pCaster->IsPC() && skilltbl->byRequire_Epuip_Slot_Type == EQUIP_SLOT_TYPE_SUB_WEAPON)
	{
		// Add % heal bonus (offence * %)
		resultvalue += (float)pCaster->GetCharAtt()->GetSubWeaponEnergyOffence() * pCaster->GetCharAtt()->GetHotPowerBonusInPercent() / 100.f;
	}
	else
	{
		// Add % heal bonus (offence * %)
		resultvalue += (float)pCaster->GetCharAtt()->GetEnergyOffence() * pCaster->GetCharAtt()->GetHotPowerBonusInPercent() / 100.f;
	}

	// Add Static Bonus
	resultvalue += pCaster->GetCharAtt()->GetHotPowerBonus();
}


//--------------------------------------------------------------------------------------//
//		INCREASES THE AGGRO FROM MONSTER WHICH ARE ATTACKING pTARGET (USED WHEN PCASTER HEAL PTARGET)
//--------------------------------------------------------------------------------------//
void IncreaseTargetEnemyAggro(CCharacter * pCaster, CCharacter * pTarget, DWORD dwDefaultAggro)
{
	int nAgro = (int)(dwDefaultAggro + pCaster->GetCharAtt()->GetSkillAggroBonus());
	nAgro += (int)((float)nAgro * pCaster->GetCharAtt()->GetSkillAggroBonusInPercent() / 100.f);

	auto it = pTarget->GetTargetListManager()->AggroBegin();
	auto itEnd = pTarget->GetTargetListManager()->AggroEnd();

	int nLoopCount = 0;

	while (it != itEnd)
	{
		++nLoopCount;
		if (nLoopCount > 5000)
		{
			ERR_LOG(LOG_GENERAL, "INFINITE LOOP FOUND");
		}

		CCharacter* pAttacker = g_pObjectManager->GetChar(it->first);
		if (pAttacker && pAttacker->IsInitialized())
		{
			if (pAttacker->IsNPC() || pAttacker->IsMonster())
			{
				pAttacker->ChangeAggro(pCaster->GetID(), DBO_AGGRO_CHANGE_TYPE_INCREASE, (DWORD)nAgro);
			}
		}

		++it;
	}
}

float GetReflectDamage(float fDmg, BYTE byAttackType, CCharacterAtt* pVictimAtt)
{
	/*
		We need to sum + 1 to convert enum types

		NTL_SKILL_TYPE_PHYSICAL = BATTLE_ATTACK_TYPE_PHYSICAL + 1
		NTL_SKILL_TYPE_ENERGY = BATTLE_ATTACK_TYPE_ENERGY + 1
	*/

	return GetSkillReflectDamage(fDmg, byAttackType + 1, pVictimAtt);
}

float GetSkillReflectDamage(float fDmg, BYTE bySkillType, CCharacterAtt* pVictimAtt)
{
	bool bIsSkillPhysical = bySkillType == NTL_SKILL_TYPE_PHYSICAL;
	bool bIsSkillEnergy = bySkillType == NTL_SKILL_TYPE_ENERGY;
	float fReflectedValue = 0.0f;

	if (bIsSkillPhysical || bIsSkillEnergy)
	{
		float fReflection = bIsSkillPhysical ? pVictimAtt->GetPhysicalReflection() : pVictimAtt->GetEnergyReflection();

		fReflectedValue = fDmg * fReflection / 100.0f;
	}

	return fReflectedValue;
}


float GetAttributeBonusRate(bool bIsPc, bool bSubWeapon, BYTE byOffence, BYTE byDefence, BYTE bySubOffence, sAVATAR_ATTRIBUTE& sOffenceAttribute, sAVATAR_ATTRIBUTE& sDefenceAttribute)
{
	float fAttributeBonusRate = 0.0f;
	BYTE byAttrOffence = byOffence;

	fAttributeBonusRate = NtlGetBattleAttributeBonusRate(byOffence, byDefence);

	if (IsValidAttribute(byOffence))
	{
		fAttributeBonusRate += GetOffenceAttribute(byOffence, sOffenceAttribute) - GetDefenceAttribute(byOffence, sDefenceAttribute);
	}
	else if (IsValidAttribute(byDefence))
	{
		fAttributeBonusRate -= GetDefenceAttribute(byDefence, sDefenceAttribute);
	}

	// Increase cap if attacker is a mob
	if (!bIsPc && fAttributeBonusRate <= -70.0f) {
		fAttributeBonusRate = -70.0f;
	}

	return fAttributeBonusRate;
}

////////////////////////////////////////////////
//  Helper Methods
/////////////////////////////////////////////////

float CalcPropBonus(CCharacterObject* pCaster, CCharacterObject* pVictim, float fAttackerPower)
{
	CCharacterAtt* cCasterAtt = pCaster->GetCharAtt();
	CCharacterAtt* cVictimAtt = pVictim->GetCharAtt();

	float fFinalProp = GetAttributeBonusRate(pCaster->IsPC(), 0, cCasterAtt->GetBattleAttributeOffence(),
		cVictimAtt->GetBattleAttributeDefence(), 0,
		cCasterAtt->GetAvatarAttribute(), cVictimAtt->GetAvatarAttribute());
	float fPropsValue = 0;

	if (fFinalProp == 0)
	{
		return fAttackerPower;
	}

	fPropsValue = (fAttackerPower * fFinalProp / 100.f);

	return fAttackerPower + fPropsValue;
}

float CalcDefensePower(float fDefence, float fArmorPenRate)
{
	return fDefence - (fArmorPenRate * fDefence / 100.0f);
}

float CalcCritBonus(float fResultValue, CCharacterAtt* pCasterAtt, BYTE bySkill_Type, BYTE byAttackResult, float fCritDefRate, bool bIncreaseDmg)
{
	bool bIsSkillPhysical = bySkill_Type == NTL_SKILL_TYPE_PHYSICAL;
	bool bIsSkillEnergy = bySkill_Type == NTL_SKILL_TYPE_ENERGY;
	bool bIsSkillState = bySkill_Type == NTL_SKILL_TYPE_STATE;
	float fCritDmgRate = 0.0f;
	float fCritDmgBonus = 0.0f;

	if (byAttackResult == BATTLE_ATTACK_RESULT_CRITICAL_HIT)
	{
		if (bIsSkillPhysical)
		{
			fCritDmgRate = pCasterAtt->GetPhysicalCriticalDamageRate();
		}
		else if (bIsSkillEnergy)
		{
			fCritDmgRate = pCasterAtt->GetEnergyCriticalDamageRate();
		}
		else if (bIsSkillState)
		{
			fCritDmgRate = (pCasterAtt->GetPhysicalCriticalDamageRate() + pCasterAtt->GetEnergyCriticalDamageRate()) / 2.f;
		}

		fCritDmgBonus = ((fResultValue * fCritDmgRate) / 100.f);
		fCritDmgBonus = bIncreaseDmg ? fCritDmgBonus * (DBO_BATTLE_OFFENCE_BONUS_RATE_BY_CRITICAL / 100.f) : fCritDmgBonus;
		fCritDmgBonus -= fCritDmgBonus * fCritDefRate / 100.f;
	}

	return fResultValue + fCritDmgBonus;
}

float CalcMinDamage(float fDamage, float fLevel)
{
	return fDamage * (CFormulaTable::m_afRate[3500][1] + (fLevel * CFormulaTable::m_afRate[3500][2]));
}

float CalcMaxDamage(float fDamage, float fLevel)
{
	return fDamage * (CFormulaTable::m_afRate[3500][3] - (fLevel * CFormulaTable::m_afRate[3500][4]));
}

bool IsValidAttribute(BYTE byAttr)
{
	return byAttr == BATTLE_ATTRIBUTE_HONEST ||
		byAttr == BATTLE_ATTRIBUTE_STRANGE ||
		byAttr == BATTLE_ATTRIBUTE_WILD ||
		byAttr == BATTLE_ATTRIBUTE_ELEGANCE ||
		byAttr == BATTLE_ATTRIBUTE_FUNNY;
}

float GetOffenceAttribute(BYTE byAttr, sAVATAR_ATTRIBUTE& sOffenceAttribute)
{
	switch (byAttr) {
	case BATTLE_ATTRIBUTE_HONEST:
		return sOffenceAttribute.fHonestOffense;
	case BATTLE_ATTRIBUTE_STRANGE:
		return sOffenceAttribute.fStrangeOffense;
	case BATTLE_ATTRIBUTE_WILD:
		return sOffenceAttribute.fWildOffense;
	case BATTLE_ATTRIBUTE_ELEGANCE:
		return sOffenceAttribute.fEleganceOffense;
	case BATTLE_ATTRIBUTE_FUNNY:
		return sOffenceAttribute.fFunnyOffense;
	default:
		return 0.0f;
	}
}

float GetDefenceAttribute(BYTE byAttr, sAVATAR_ATTRIBUTE& sDefenceAttribute)
{
	switch (byAttr) {
	case BATTLE_ATTRIBUTE_HONEST:
		return sDefenceAttribute.fHonestDefense;
	case BATTLE_ATTRIBUTE_STRANGE:
		return sDefenceAttribute.fStrangeDefense;
	case BATTLE_ATTRIBUTE_WILD:
		return sDefenceAttribute.fWildDefense;
	case BATTLE_ATTRIBUTE_ELEGANCE:
		return sDefenceAttribute.fEleganceDefense;
	case BATTLE_ATTRIBUTE_FUNNY:
		return sDefenceAttribute.fFunnyDefense;
	default:
		return 0.0f;
	}
}