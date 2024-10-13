#include "stdafx.h"
#include "CPlayer.h"
#include "battle.h"
#include "NtlPacketGU.h"



//-------------------------------------------------------------------//
// UPDATE PVP ZONE
//-------------------------------------------------------------------//
void CPlayer::UpdatePvpZone(bool bStatus)
{
	SetPvpZone(bStatus);

	if(bStatus == true)
	{
		CNtlPacket packet(sizeof(sGU_WORLD_FREE_PVP_ZONE_ENTERED_NFY));
		sGU_WORLD_FREE_PVP_ZONE_ENTERED_NFY * res = (sGU_WORLD_FREE_PVP_ZONE_ENTERED_NFY *)packet.GetPacketData();
		res->wOpCode = GU_WORLD_FREE_PVP_ZONE_ENTERED_NFY;
		res->hSubject = this->GetID();
		packet.SetPacketLen( sizeof(sGU_WORLD_FREE_PVP_ZONE_ENTERED_NFY) );
		this->Broadcast(&packet);
	}
	else
	{
		CNtlPacket packet(sizeof(sGU_WORLD_FREE_PVP_ZONE_LEFT_NFY));
		sGU_WORLD_FREE_PVP_ZONE_LEFT_NFY * res = (sGU_WORLD_FREE_PVP_ZONE_LEFT_NFY *)packet.GetPacketData();
		res->wOpCode = GU_WORLD_FREE_PVP_ZONE_LEFT_NFY;
		res->hSubject = this->GetID();
		packet.SetPacketLen( sizeof(sGU_WORLD_FREE_PVP_ZONE_LEFT_NFY) );
		this->Broadcast(&packet);
	}
}


//-------------------------------------------------------------------//
// send attack action packet
//-------------------------------------------------------------------//
void CCharacter::AttackAction(CCharacter* pVictim)
{
	float fDmg = 0.0f;
	BYTE byAttackType = GetAttackType();
	BYTE byBlockedAction = DBO_GUARD_TYPE_INVALID;
	BYTE byAttackResult = BATTLE_ATTACK_RESULT_HIT;
	float fReflectedDamage = 0.0f;
	float fTargetLpRecoveredWhenHit = 0.0f;
	float fTargetEpRecoveredWhenHit = 0.0f;

	CNtlVector vShift(pVictim->GetCurLoc() - GetCurLoc());
	vShift.y = 0.0f;
	vShift.SafeNormalize();
	
	// Player characters will chain all attacks from 1 until max anim id (depending on level, max 6), then reset back to 1.
	if (IsPC()) {
		if (m_byChainSequence < GetMaxChainAttack())
			++m_byChainSequence;
		else
			m_byChainSequence = GetStartChainIndex();
	}
	// Mobs will simply select either main or secondary attack randomly.
	else
		m_byChainSequence = rand() % GetMaxChainAttack();

	if (pVictim->GetStateManager()->IsCharCondition(CHARCOND_INVINCIBLE))
	{
		goto SEND_PACKET;
	}

	//check guard.
	if (pVictim->GetCharStateID() == CHARSTATE_GUARD)
	{
		bool bCanCounterAttack = true;
		if (((CPlayer*)pVictim)->IsGuardSuccess(DBO_GUARD_TYPE_DEFENCE, bCanCounterAttack))
		{
			byAttackResult = BATTLE_ATTACK_RESULT_BLOCK;
			byBlockedAction = DBO_GUARD_TYPE_DEFENCE;
			goto SEND_PACKET;
		}
	}

	//check dodge
	if (BattleIsDodge(pVictim->IsPC(), GetCharAtt()->GetAttackRate(), pVictim->GetCharAtt()->GetDodgeRate(), GetLevel(), pVictim->GetLevel()) == true)
	{
		byAttackResult = BATTLE_ATTACK_RESULT_DODGE;
		goto SEND_PACKET;
	}

	//if not guarded or dodged then calculate damage
	fDmg = CalcMeleeDamage(this, pVictim);

	//add chain bonus dmg
	float fChainBonusRate = NtlGetBattleChainAttackBounsRate(m_byChainSequence);
	fDmg += fDmg * fChainBonusRate / 100;

	//bool bIsComboFinisher = IsPC() && m_byChainSequence == NTL_BATTLE_MAX_CHAIN_ATTACK_COUNT_PLAYER;
	if (BattleIsBlock(pVictim->GetCharAtt()->GetBlockRate(), GetLevel(), pVictim->GetLevel()))
	{
		// Commented out code because this sliding behavior didn't seem to exist in latest retail TW version.
		// TODO: Below code is commented out because the sliding (aka a knockdown that's being blocked) mechanic
		//  seems to not work properly. Unsure if client issue or server issue (most likely).
		/*if (bIsComboFinisher)
		{
			byAttackResult = BATTLE_ATTACK_RESULT_SLIDING;
			byBlockedAction = DBO_GUARD_TYPE_KNOCKDOWN;
			vShift *= +NTL_BATTLE_SLIDING_DISTANCE; // should it be push distance instead?
		}
		else*/
		{
			byAttackResult = BATTLE_ATTACK_RESULT_BLOCK;
		}
		//fDmg *= (1.0f - NTL_BATTLE_BLOCK_DAMAGE_REDUCE_RATE);
		fDmg /= 2.0f;
	}
	// Commented out code because this knockdown behavior didn't seem to exist in latest retail TW version.
	//else if (bIsComboFinisher)
	//{
	//	byAttackResult = BATTLE_ATTACK_RESULT_KNOCKDOWN;
	//	vShift *= +NTL_BATTLE_KNOCKDOWN_DISTANCE;
	//}
	else
	{
		// check crit
		float fCritDmgRate = 0.0f, fCritDmgBonus = 0.0f, fCritDefRate = 0.0f;
		bool bIsCrit = false;

		if (byAttackType == BATTLE_ATTACK_TYPE_PHYSICAL)
		{
			if (BattleIsCrit(GetCharAtt(), pVictim->GetCharAtt(), true))
			{
				bIsCrit = true;
				fCritDmgRate = GetCharAtt()->GetPhysicalCriticalDamageRate();

				// critical dmg def
				fCritDefRate = pVictim->GetCharAtt()->GetPhysicalCriticalDefenceRate();
			}
		}
		else if (byAttackType == BATTLE_ATTACK_TYPE_ENERGY)
		{
			if (BattleIsCrit(GetCharAtt(), pVictim->GetCharAtt(), false))
			{
				bIsCrit = true;
				fCritDmgRate = GetCharAtt()->GetEnergyCriticalDamageRate();

				// critical dmg def
				fCritDefRate = pVictim->GetCharAtt()->GetEnergyCriticalDefenceRate();
			}
		}

		if (bIsCrit)
		{
			byAttackResult = BATTLE_ATTACK_RESULT_CRITICAL_HIT;
			fCritDmgBonus = ((fDmg * fCritDmgRate) / 100.f);
			fCritDmgBonus -= fCritDmgBonus * fCritDefRate / 100.f;

			fDmg += fCritDmgBonus;
		}
	}

	//set victim heal-on-hit
	fTargetLpRecoveredWhenHit = (float)pVictim->GetCharAtt()->GetLpRecoveryWhenHit() + (fDmg * pVictim->GetCharAtt()->GetLpRecoveryWhenHitInPercent() / 100.0f);
	fTargetEpRecoveredWhenHit = (float)pVictim->GetCharAtt()->GetEpRecoveryWhenHit() + (fDmg * pVictim->GetCharAtt()->GetEpRecoveryWhenHitInPercent() / 100.0f);

	//if(fTargetLpRecoveredWhenHit)
	//printf("fTargetLpRecoveredWhenHit %f, GetLastLpRecoveryWhenHit %u, GetLastLpRecoveryWhenHitInPercent %f fDmg %f\n", 
	//	fTargetLpRecoveredWhenHit, pVictim->GetCharAtt()->GetLastLpRecoveryWhenHit(), pVictim->GetCharAtt()->GetLastLpRecoveryWhenHitInPercent(), fDmg);

	//check reflect
	fReflectedDamage = GetReflectDamage(fDmg, byAttackType, pVictim->GetCharAtt()->GetPhysicalReflection(), pVictim->GetCharAtt()->GetEnergyReflection());

//---SEND PACKET START----------------------------------------------------------
SEND_PACKET:
	CNtlPacket packet(sizeof(sGU_CHAR_ACTION_ATTACK));
	sGU_CHAR_ACTION_ATTACK * res = (sGU_CHAR_ACTION_ATTACK *)packet.GetPacketData();
	res->wOpCode = GU_CHAR_ACTION_ATTACK;
	res->hSubject = GetID();
	res->hTarget = pVictim->GetID();
	res->dwLpEpEventId = pVictim->AcquireLpEpEventID();
	res->bChainAttack = IsPC(); // Only players perform chain attacks.
	res->byAttackResult = byAttackResult;
	res->attackResultValue = (int)fDmg;
	res->byAttackSequence = m_byChainSequence;
	res->fReflectedDamage = fReflectedDamage;
	res->byBlockedAction = byBlockedAction;
	vShift.CopyTo(res->vShift);

	res->lpEpRecovered.targetLpRecoveredWhenHit = (int)fTargetLpRecoveredWhenHit;
	if (res->lpEpRecovered.targetLpRecoveredWhenHit >= 1)
		res->lpEpRecovered.bIsLpRecoveredWhenHit = true;

	res->lpEpRecovered.dwTargetEpRecoveredWhenHit = (DWORD)fTargetEpRecoveredWhenHit;
	if (res->lpEpRecovered.dwTargetEpRecoveredWhenHit >= 1)
		res->lpEpRecovered.bIsEpRecoveredWhenHit = true;

	packet.SetPacketLen( sizeof(sGU_CHAR_ACTION_ATTACK) );
	Broadcast(&packet);

//---SEND PACKET END----------------------------------------------------------

	//<attacker>deal reflect damage
	if (fReflectedDamage > 1.0f)
	{
		OnAttackAction(pVictim, (int)fReflectedDamage, BATTLE_ATTACK_RESULT_REFLECTED_DAMAGE);
		//printf("fPhysicalReflection %f fEnergyReflection %f | fReflectedDamage %f \n", pVictim->GetCharAtt()->GetLastPhysicalReflection(), pVictim->GetCharAtt()->GetLastEnergyReflection(), fReflectedDamage);
	}


	//<victim>Deal damage on victim
	if (pVictim->OnAttackAction(this, (int)fDmg, byAttackResult) == false) //return false if not dead
	{
		//printf("fTargetLpRecoveredWhenHit %f, fTargetEpRecoveredWhenHit %f targetLpRecoveredWhenHit %u dwTargetEpRecoveredWhenHit %u\n", fTargetLpRecoveredWhenHit, fTargetEpRecoveredWhenHit, res->lpEpRecovered.targetLpRecoveredWhenHit, res->lpEpRecovered.dwTargetEpRecoveredWhenHit);
		//<victim> heal on hit
		if (res->lpEpRecovered.targetLpRecoveredWhenHit >= 1)
			pVictim->UpdateCurLP((int)fTargetLpRecoveredWhenHit, true, false);
		if (res->lpEpRecovered.dwTargetEpRecoveredWhenHit >= 1)
			pVictim->UpdateCurEP((WORD)fTargetEpRecoveredWhenHit, true, false);
	}

	// Commented out code because this knockdown/sliding behavior didn't seem to exist in latest retail TW version.
	// TODO: Should we send this before the sGU_CHAR_ACTION_ATTACK packet? After? Should we actually send the sGU_UPDATE_CHAR_STATE
	//  packet too? Or should we simply update the values internally but not broadcast?
	//if (byAttackResult == BATTLE_ATTACK_RESULT_KNOCKDOWN)
	//{
	//	pVictim->SendCharStateKnockdown(res->vShift);
	//}
	//else if (byAttackResult == BATTLE_ATTACK_RESULT_SLIDING)
	//{
	//	pVictim->SendCharStateSliding(res->vShift);
	//}
}

void CCharacter::SpecialAttackAction(CCharacter * pVictim, BYTE byAttackType, BYTE bySourceType, TBLIDX sourceTblidx, CSkill* pSkill)
{
	float fDmg = 0.0f;
	BYTE byAttackResult = BATTLE_ATTACK_RESULT_HIT;
	int nReflectedDamage = 0;
	sDBO_LP_EP_RECOVERED lpEpRecovered;
	lpEpRecovered.Reset();

	//printf("sourceTblidx %u, GetSkillId %u, GetSkillIndex %u, aSkill_Effect_Value[0] %f \n", 
	//	sourceTblidx, pSkill->GetSkillId(), pSkill->GetSkillIndex(), (float)pSkill->GetOriginalTableData()->aSkill_Effect_Value[0]);

	//check dodge
	if (BattleIsDodge(pVictim->IsPC(), GetCharAtt()->GetAttackRate(), pVictim->GetCharAtt()->GetDodgeRate(), GetLevel(), pVictim->GetLevel()) == true)
	{
		byAttackResult = BATTLE_ATTACK_RESULT_DODGE;
		goto SEND_PACKET;
	}

	CalcSpecialSkillDamage(this, pVictim, pSkill->GetOriginalTableData(), 0, (float)pSkill->GetOriginalTableData()->aSkill_Effect_Value[0], fDmg, byAttackResult, nReflectedDamage, lpEpRecovered);


	//---SEND PACKET START----------------------------------------------------------
SEND_PACKET:

	CNtlPacket packet(sizeof(sGU_CHAR_SPECIAL_ATTACK_NFY));
	sGU_CHAR_SPECIAL_ATTACK_NFY * res = (sGU_CHAR_SPECIAL_ATTACK_NFY *)packet.GetPacketData();
	res->wOpCode = GU_CHAR_SPECIAL_ATTACK_NFY;
	res->hAttacker = GetID();
	res->hTarget = pVictim->GetID();

	res->byAttackType = byAttackType;
	res->bySourceType = bySourceType;
	res->sourceTblidx = sourceTblidx;
	
	res->byAttackResult = byAttackResult;
	res->attackResultValue = (int)fDmg;
	
	NtlLocationCompress(&res->vDestLoc, pVictim->GetCurLoc().x, pVictim->GetCurLoc().y, pVictim->GetCurLoc().z);

	packet.SetPacketLen(sizeof(sGU_CHAR_SPECIAL_ATTACK_NFY));
	Broadcast(&packet);

	//---SEND PACKET END----------------------------------------------------------

	//<attacker>deal reflect damage
	if (nReflectedDamage > 1)
	{
		OnAttackAction(pVictim, nReflectedDamage, BATTLE_ATTACK_RESULT_REFLECTED_DAMAGE);
	}

	//<victim>Deal damage on victim
	if(pVictim->OnAttackAction(this, (int)fDmg, byAttackResult) == false)
	{
		//<victim> heal on hit
		if (lpEpRecovered.bIsLpRecoveredWhenHit || lpEpRecovered.bIsEpRecoveredWhenHit)
		{
			CNtlPacket packet(sizeof(sGU_CHAR_LP_EP_RECOVERED));
			sGU_CHAR_LP_EP_RECOVERED * res = (sGU_CHAR_LP_EP_RECOVERED *)packet.GetPacketData();
			res->wOpCode = GU_CHAR_LP_EP_RECOVERED;
			res->hCaster = GetID();
			res->hSubject = pVictim->GetID();
			res->lpEpRecovered = lpEpRecovered;
			pVictim->Broadcast(&packet);

			//update LP
			if (res->lpEpRecovered.bIsLpRecoveredWhenHit)
				pVictim->UpdateCurLP(res->lpEpRecovered.targetLpRecoveredWhenHit, true, false);

			//update EP
			if (res->lpEpRecovered.bIsEpRecoveredWhenHit)
				pVictim->UpdateCurEP((WORD)res->lpEpRecovered.dwTargetEpRecoveredWhenHit, true, false);
		}
	}
}




