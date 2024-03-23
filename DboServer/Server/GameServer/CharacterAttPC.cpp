#include "stdafx.h"
#include "CharacterAttPC.h"
#include "CPlayer.h"
#include "item.h"
#include "VehicleTable.h"
#include "ItemOptionTable.h"
#include "TableContainerManager.h"
#include "CharTitleTable.h"
#include "calcs.h"
#include "StatusTransformTable.h"
#include "CommonConfigTable.h"
#include "FormulaTable.h"
#include "GameServer.h"


CCharacterAttPC::CCharacterAttPC()
{
}

CCharacterAttPC::~CCharacterAttPC()
{
}


bool CCharacterAttPC::Create(CPlayer* pPlayer)
{
	if (CCharacterAtt::Create(pPlayer))
	{
		m_pPlayerRef = pPlayer;
		return true;
	}

	return false;
}


void CCharacterAttPC::CalculateAtt()
{
	sCHAR_TBLDAT* charTbldat = m_pPlayerRef->GetTbldat();
	sPC_TBLDAT* pcTbldat = m_pPlayerRef->GetTbldat();
	float byLevel = (float)m_pPlayerRef->GetLevel();

	m_pAttribute.dwBaseMaxLP = m_pAttribute.dwLastMaxLP = (int)charTbldat->dwBasic_LP;
	m_pAttribute.wBaseMaxEP = m_pAttribute.wLastMaxEP = charTbldat->wBasic_EP;
	m_pAttribute.wBasePhysicalDefense = m_pAttribute.wLastPhysicalDefense = charTbldat->wBasic_Physical_Defence;
	m_pAttribute.wBaseEnergyDefense = m_pAttribute.wLastEnergyDefense = charTbldat->wBasic_Energy_Defence;
	m_pAttribute.wBaseAttackSpeedRate = m_pAttribute.wLastAttackSpeedRate = charTbldat->wAttack_Speed_Rate;
	m_pAttribute.fBaseAttackRange = m_pAttribute.fLastAttackRange = charTbldat->fAttack_Range;
	m_pAttribute.wBaseHitRate = m_pAttribute.wLastHitRate = charTbldat->wAttack_Rate;
	m_pAttribute.wBaseDodgeRate = m_pAttribute.wLastDodgeRate = charTbldat->wDodge_Rate;
	m_pAttribute.wBaseBlockRate = m_pAttribute.wLastBlockRate = charTbldat->wBlock_Rate;
	m_pAttribute.wBaseCurseSuccessRate = m_pAttribute.wLastCurseSuccessRate = charTbldat->wCurse_Success_Rate;
	m_pAttribute.wBaseCurseToleranceRate = m_pAttribute.wLastCurseToleranceRate = charTbldat->wCurse_Tolerance_Rate;

	m_pAttribute.wBaseStr = m_pAttribute.wLastStr = (WORD)(charTbldat->wBasicStr + pcTbldat->fLevel_Up_Str * byLevel);
	m_pAttribute.wBaseCon = m_pAttribute.wLastCon = (WORD)(charTbldat->wBasicCon + pcTbldat->fLevel_Up_Con * byLevel);
	m_pAttribute.wBaseFoc = m_pAttribute.wLastFoc = (WORD)(charTbldat->wBasicFoc + pcTbldat->fLevel_Up_Foc * byLevel);
	m_pAttribute.wBaseDex = m_pAttribute.wLastDex = (WORD)(charTbldat->wBasicDex + pcTbldat->fLevel_Up_Dex * byLevel);
	m_pAttribute.wBaseSol = m_pAttribute.wLastSol = (WORD)(charTbldat->wBasicSol + pcTbldat->fLevel_Up_Sol * byLevel);
	m_pAttribute.wBaseEng = m_pAttribute.wLastEng = (WORD)(charTbldat->wBasicEng + pcTbldat->fLevel_Up_Eng * byLevel);

	m_pAttribute.fBaseSkillAnimationSpeed = m_pAttribute.fLastSkillAnimationSpeed = 100.0f;
	m_pAttribute.dwBaseWeightMax = m_pAttribute.dwLastWeightMax = pcTbldat->dwWeightLimit;
	m_pAttribute.dwBaseMaxAp = m_pAttribute.dwLastMaxAp = 450000;
	m_pAttribute.wBaseMaxRP = m_pAttribute.wLastMaxRP = WORD(CFormulaTable::m_afRate[2300][1] + byLevel * CFormulaTable::m_afRate[2300][2]);
	m_pAttribute.wBaseRpRegen = m_pAttribute.wLastRpRegen = 25;
	m_pAttribute.wLastRpDiminutionRate = 6;
	m_pAttribute.wBaseApRegen = m_pAttribute.wLastApRegen = 5000;
	m_pAttribute.wBaseApSitdownRegen = m_pAttribute.wLastApSitdownRegen = 10000;
	m_pAttribute.wAirAP_Degen = 5000;

	if (m_pPlayerRef->IsAdult())
	{
		m_pAttribute.fBaseRunSpeed = m_pAttribute.fLastRunSpeed = pcTbldat->fAdult_Run_Speed;
		m_pAttribute.fBaseAirSpeed = m_pAttribute.fLastAirSpeed = pcTbldat->fAdult_Fly_Speed;
		m_pAttribute.fBaseAirDashSpeed = m_pAttribute.fLastAirDashSpeed = pcTbldat->fAdult_Dash_Speed;
		m_pAttribute.fBaseAirDashAccelSpeed = m_pAttribute.fLastAirDashAccelSpeed = pcTbldat->fAdult_Accel_Speed;
	}
	else
	{
		m_pAttribute.fBaseRunSpeed = m_pAttribute.fLastRunSpeed = pcTbldat->fChild_Run_Speed;
		m_pAttribute.fBaseAirSpeed = m_pAttribute.fLastAirSpeed = pcTbldat->fChild_Fly_Speed;
		m_pAttribute.fBaseAirDashSpeed = m_pAttribute.fLastAirDashSpeed = pcTbldat->fChild_Dash_Speed;
		m_pAttribute.fBaseAirDashAccelSpeed = m_pAttribute.fLastAirDashAccelSpeed = pcTbldat->fChild_Accel_Speed;
	}

	m_pAttribute.dwBaseMaxLP = m_pAttribute.dwLastMaxLP += int(CFormulaTable::m_afRate[200 + pcTbldat->byClass + 1][1] + m_pAttribute.wBaseCon * CFormulaTable::m_afRate[200 + pcTbldat->byClass + 1][2]);
	m_pAttribute.wBaseMaxEP = m_pAttribute.wLastMaxEP += WORD(CFormulaTable::m_afRate[1300 + pcTbldat->byClass + 1][1] + m_pAttribute.wBaseEng * CFormulaTable::m_afRate[1300 + pcTbldat->byClass + 1][2]);
	m_pAttribute.wBaseHitRate = m_pAttribute.wLastHitRate += WORD(m_pAttribute.wBaseFoc / CFormulaTable::m_afRate[600][1] * CFormulaTable::m_afRate[600][2]);
	m_pAttribute.wBaseDodgeRate = m_pAttribute.wLastDodgeRate += WORD(m_pAttribute.wBaseDex / CFormulaTable::m_afRate[800][1] * CFormulaTable::m_afRate[800][2]);

	m_pAttribute.wBasePhysicalOffense = m_pAttribute.wLastPhysicalOffense += WORD(byLevel * CFormulaTable::m_afRate[100 + pcTbldat->byClass + 1][1] + m_pAttribute.wBaseStr * CFormulaTable::m_afRate[100 + pcTbldat->byClass + 1][2]);
	m_pAttribute.wBasePhysicalOffense = m_pAttribute.wLastPhysicalOffense += WORD(m_pAttribute.wBaseDex * CFormulaTable::m_afRate[100 + pcTbldat->byClass + 1][3]);

	m_pAttribute.wBaseBlockRate = m_pAttribute.wLastBlockRate += WORD(CFormulaTable::m_afRate[900][1] + m_pAttribute.wBaseDex * CFormulaTable::m_afRate[900][2] + m_pAttribute.wBaseCon * CFormulaTable::m_afRate[900][2]);
	m_pAttribute.wGuardRateBase = m_pAttribute.wGuardRateLast += WORD(CFormulaTable::m_afRate[1000][1] + m_pAttribute.wBaseDex * CFormulaTable::m_afRate[1000][2]);


	m_pAttribute.wBaseEnergyOffense = m_pAttribute.wLastEnergyOffense += WORD(byLevel * CFormulaTable::m_afRate[1100 + pcTbldat->byClass + 1][1] + m_pAttribute.wBaseSol * CFormulaTable::m_afRate[1100 + pcTbldat->byClass + 1][2]);
	m_pAttribute.wBaseEnergyOffense = m_pAttribute.wLastEnergyOffense += WORD(m_pAttribute.wBaseFoc * CFormulaTable::m_afRate[1100 + pcTbldat->byClass + 1][3]);

	m_pAttribute.wBaseLpRegen = m_pAttribute.wLastLpRegen += WORD(CFormulaTable::m_afRate[300][1] + m_pAttribute.wBaseCon * CFormulaTable::m_afRate[300][2]);
	m_pAttribute.wBaseLpSitdownRegen = m_pAttribute.wLastLpSitdownRegen += WORD(m_pAttribute.wBaseLpRegen * CFormulaTable::m_afRate[400][1]);
	m_pAttribute.wBaseLpBattleRegen = m_pAttribute.wLastLpBattleRegen = WORD(m_pAttribute.wBaseLpRegen * CFormulaTable::m_afRate[500][1]);
	m_pAttribute.wBaseEpRegen = m_pAttribute.wLastEpRegen += WORD(CFormulaTable::m_afRate[1400][1] + m_pAttribute.wBaseEng * CFormulaTable::m_afRate[1400][2]);
	m_pAttribute.wBaseEpSitdownRegen = m_pAttribute.wLastEpSitdownRegen += WORD(m_pAttribute.wBaseEpRegen * CFormulaTable::m_afRate[1500][1]);
	m_pAttribute.wBaseEpBattleRegen = m_pAttribute.wLastEpBattleRegen = WORD(m_pAttribute.wBaseEpRegen * CFormulaTable::m_afRate[1600][1]);

	m_pAttribute.wBasePhysicalCriticalRate = m_pAttribute.wLastPhysicalCriticalRate += WORD(CFormulaTable::m_afRate[1900 + pcTbldat->byClass + 1][1] + m_pAttribute.wBaseDex / CFormulaTable::m_afRate[1900 + pcTbldat->byClass + 1][2]);
	m_pAttribute.wBaseEnergyCriticalRate = m_pAttribute.wLastEnergyCriticalRate += WORD(CFormulaTable::m_afRate[2100 + pcTbldat->byClass + 1][1] + m_pAttribute.wBaseFoc / CFormulaTable::m_afRate[2100 + pcTbldat->byClass + 1][2]);
	m_pAttribute.wBaseCurseSuccessRate = m_pAttribute.wLastCurseSuccessRate += WORD(m_pAttribute.wBaseFoc / CFormulaTable::m_afRate[700][1] * CFormulaTable::m_afRate[700][2]);
	m_pAttribute.wBaseCurseToleranceRate = m_pAttribute.wLastCurseToleranceRate += WORD(m_pAttribute.wBaseDex / CFormulaTable::m_afRate[1200][1] * CFormulaTable::m_afRate[1200][2]);

	m_pAttribute.fBasePhysicalCritDmgRate = m_pAttribute.fLastPhysicalCritDmgRate += CFormulaTable::m_afRate[2000 + pcTbldat->byClass + 1][1] + m_pAttribute.wBaseDex / CFormulaTable::m_afRate[2000 + pcTbldat->byClass + 1][2];
	m_pAttribute.fBaseEnergyCritDmgRate = m_pAttribute.fLastEnergyCritDmgRate += CFormulaTable::m_afRate[2200 + pcTbldat->byClass + 1][1] + m_pAttribute.wBaseFoc / CFormulaTable::m_afRate[2200 + pcTbldat->byClass + 1][2];

	//default equipment stats added to base
	m_pPlayerRef->GetPlayerItemContainer()->CopyBaseItemAttributesTo(m_pAttribute);

	//apply passive skills attributes
	m_pPlayerRef->GetSkillManager()->CopyPassiveAttributesTo(this);


	m_pPlayerRef->GetPlayerItemContainer()->CopyItemAttributesTo(this);

	//set char title attribute
	if (m_pPlayerRef->GetCharTitleID() != INVALID_TBLIDX)
	{
		sCHARTITLE_TBLDAT* charTitleTbldat = (sCHARTITLE_TBLDAT*)g_pTableContainer->GetCharTitleTable()->FindData(m_pPlayerRef->GetCharTitleID());
		if (charTitleTbldat)
		{
			for (BYTE i = 0; i < NTL_MAX_CHAR_TITLE_EFFECT; i++)
			{
				eSYSTEM_EFFECT_CODE effectcode = g_pTableContainer->GetSystemEffectTable()->GetEffectCodeWithTblidx(charTitleTbldat->atblSystem_Effect_Index[i]);
				if (effectcode != INVALID_SYSTEM_EFFECT_CODE)
				{
					//printf("effectcode %d \n", effectcode);
					Dbo_SetAvatarAttributeValue(this, effectcode, (float)charTitleTbldat->abySystem_Effect_Value[i], charTitleTbldat->abySystem_Effect_Type[i]);
				}
			}
		}
	}

	//buff attributes
	m_pPlayerRef->GetBuffManager()->CopyBuffAttributesTo(this);

	//transformation attributes
	if (m_pPlayerRef->GetTransformationTbldat())
	{
		sSTATUS_TRANSFORM_TBLDAT* pTransformationTbldat = m_pPlayerRef->GetTransformationTbldat();

		m_pAttribute.dwLastMaxLP = (int)(m_pAttribute.dwLastMaxLP * pTransformationTbldat->fLP_Transform);
		m_pAttribute.wLastMaxEP = (WORD)(m_pAttribute.wLastMaxEP * pTransformationTbldat->fEP_Transform);
		m_pAttribute.wLastPhysicalOffense = (WORD)(m_pAttribute.wLastPhysicalOffense * pTransformationTbldat->fPhysical_Offence_Transform);
		m_pAttribute.wLastEnergyOffense = (WORD)(m_pAttribute.wLastEnergyOffense * pTransformationTbldat->fEnergy_Offence_Transform);
		m_pAttribute.wLastPhysicalDefense = (WORD)(m_pAttribute.wLastPhysicalDefense * pTransformationTbldat->fPhysical_Defence_Transform);
		m_pAttribute.wLastEnergyDefense = (WORD)(m_pAttribute.wLastEnergyDefense * pTransformationTbldat->fEnergy_Defence_Transform);
		m_pAttribute.fLastRunSpeed = m_pAttribute.fLastRunSpeed * pTransformationTbldat->fRun_Speed_Transform;

		if (m_pPlayerRef->GetAspectStateId() == ASPECTSTATE_KAIOKEN)
		{
			float fSpeedBonus = 3.0f;

			if (m_pPlayerRef->GetTransformGrade() == 2)
				fSpeedBonus = 5.0f;
			else if (m_pPlayerRef->GetTransformGrade() >= 3)
				fSpeedBonus = 7.0f;

			CalculateAttackSpeedRate(fSpeedBonus, SYSTEM_EFFECT_APPLY_TYPE_PERCENT, false);
		}
		else
		{
			if (pTransformationTbldat->fAttack_Speed_Transform < 1.0f)
				CalculateAttackSpeedRate(100.f - pTransformationTbldat->fAttack_Speed_Transform * 100.f, SYSTEM_EFFECT_APPLY_TYPE_PERCENT, false); // * 100 because the value is like 0.75 = 0.75 * 100 = 75% and then - 100 to get the real %
			else
				CalculateAttackSpeedRate(pTransformationTbldat->fAttack_Speed_Transform * 100.f - 100.f, SYSTEM_EFFECT_APPLY_TYPE_PERCENT, true);
		}

		if (m_pPlayerRef->GetAspectStateId() == ASPECTSTATE_SUPER_SAIYAN)
			m_pAttribute.fLastSkillAnimationSpeed = m_pAttribute.fLastSkillAnimationSpeed += 20.f; // make skill animation faster


		m_pAttribute.wLastHitRate = (WORD)(m_pAttribute.wLastHitRate * pTransformationTbldat->fAttack_Rate_Transform);
		m_pAttribute.wLastDodgeRate = (WORD)(m_pAttribute.wLastDodgeRate * pTransformationTbldat->fDodge_Rate_Transform);
		m_pAttribute.wLastBlockRate = (WORD)(m_pAttribute.wLastBlockRate * pTransformationTbldat->fBlock_Rate_Transform);
		m_pAttribute.wLastCurseSuccessRate = (WORD)(m_pAttribute.wLastCurseSuccessRate * pTransformationTbldat->fCurse_Success_Transform);
		m_pAttribute.wLastCurseToleranceRate = (WORD)(m_pAttribute.wLastCurseToleranceRate * pTransformationTbldat->fCurse_Tolerance_Transform);
		m_pAttribute.fLastAttackRange = pTransformationTbldat->fAttack_Range_Change;

		if (m_pPlayerRef->GetTransformGrade() > 1)
		{
		m_pAttribute.wLastPhysicalCriticalRate += WORD((float)m_pAttribute.wBaseDex * (float)m_pPlayerRef->GetTransformGrade() / 100.0f);
		m_pAttribute.wLastEnergyCriticalRate += WORD((float)m_pAttribute.wBaseFoc * (float)m_pPlayerRef->GetTransformGrade() / 100.0f);
		}

	}

	//add percent values
	CalculatePercentValues();

	// -- prop stats
	// offence
	if (GetBattleAttributeOffence() != BATTLE_ATTRIBUTE_NONE)
	{
		Dbo_SetAvatarAttributeValue(this, GetBattleAttributeEffectCodeOfence(GetBattleAttributeOffence()), 5.0f, GetBattleAttributeEffectApplyType(GetBattleAttributeOffence()));
	}
	// Defence
	if (GetBattleAttributeDefence() != BATTLE_ATTRIBUTE_NONE)
	{
		Dbo_SetAvatarAttributeValue(this, GetBattleAttributeEffectCodeDefence(GetBattleAttributeDefence()), 5.0f, GetBattleAttributeEffectApplyType(GetBattleAttributeDefence()));
	}
	//vehicle -- MUST BE THE LAST
	if (m_pPlayerRef->GetVehicleTblidx() != INVALID_TBLIDX)
	{
		sVEHICLE_TBLDAT* pVehicleTbl = (sVEHICLE_TBLDAT*)g_pTableContainer->GetVehicleTable()->FindData(m_pPlayerRef->GetVehicleTblidx());
		if (pVehicleTbl)
		{
			m_pAttribute.fBaseRunSpeed = m_pAttribute.fLastRunSpeed = (float)pVehicleTbl->bySpeed;

			if (m_pPlayerRef->GetVehicleFuelId() != INVALID_HOBJECT)
			{
				CItem* pFuel = m_pPlayerRef->GetPlayerItemContainer()->GetItem(m_pPlayerRef->GetVehicleFuelId());
				if (pFuel && pFuel->IsExpired() == false)
				{
					sITEM_TBLDAT* fuelTbldat = pFuel->GetTbldat();
					if (fuelTbldat)
					{
						if (fuelTbldat->Item_Option_Tblidx != INVALID_TBLIDX)
						{
							sITEM_OPTION_TBLDAT* optionTbldat = (sITEM_OPTION_TBLDAT*)g_pTableContainer->GetItemOptionTable()->FindData(fuelTbldat->Item_Option_Tblidx);
							if (optionTbldat)
							{
								m_pAttribute.fBaseRunSpeed = m_pAttribute.fLastRunSpeed += m_pAttribute.fBaseRunSpeed * (float)optionTbldat->nValue[0] / 100.f;
							}
						}
					}
				}
			}
		}
	}

	CGameServer* app = (CGameServer*)g_pApp;

	if (m_pPlayerRef->GetDragonballScramble() && app->GetGsChannel() == 0)
		m_pAttribute.dwBaseMaxLP = m_pAttribute.dwLastMaxLP += 5000;

	CalcSecondWeaponOffence();

	//m_pAttribute.fCoolTimeChangePercent = -100.f; // TEMP

	CCharacterAtt::CalculateAtt();
}


float CCharacterAttPC::GetWalkSpeed()
{
	return GetRunSpeed();	//player walk and run speed is the same..
}


void CCharacterAttPC::CalculateStr(float fValue, BYTE byApplyType, bool bIsPlus)
{
	WORD wCurStr = m_pAttribute.wLastStr;

	if (bIsPlus == true)
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wLastStr = UnsignedSafeIncrease<WORD>(m_pAttribute.wLastStr, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fSTR += fValue;
		}

		m_pAttribute.wLastPhysicalOffense = UnsignedSafeIncrease<WORD>(m_pAttribute.wLastPhysicalOffense, WORD((m_pAttribute.wLastStr - wCurStr) * CFormulaTable::m_afRate[100 + m_pPlayerRef->GetClass() + 1][2]));
	}
	else
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wLastStr = UnsignedSafeDecrease<WORD>(m_pAttribute.wLastStr, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fSTRNegative -= fValue;
		}

		m_pAttribute.wLastPhysicalOffense = UnsignedSafeDecrease<WORD>(m_pAttribute.wLastPhysicalOffense, WORD((wCurStr - m_pAttribute.wLastStr) * CFormulaTable::m_afRate[100 + m_pPlayerRef->GetClass() + 1][2]));
	}
}

void CCharacterAttPC::CalculateCon(float fValue, BYTE byApplyType, bool bIsPlus)
{
	WORD wCurCon = m_pAttribute.wLastCon;
	WORD wCurLpRegen = m_pAttribute.wLastLpRegen;

	if (bIsPlus == true)
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wLastCon = UnsignedSafeIncrease<WORD>(m_pAttribute.wLastCon, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fCON += fValue;
		}

		m_pAttribute.dwLastMaxLP += int(float(m_pAttribute.wLastCon - wCurCon) * CFormulaTable::m_afRate[200 + m_pPlayerRef->GetClass() + 1][2]);
		m_pAttribute.wLastLpRegen += WORD(float(m_pAttribute.wLastCon - wCurCon) * CFormulaTable::m_afRate[300][2]);
		m_pAttribute.wLastLpSitdownRegen += WORD(float(m_pAttribute.wLastLpRegen - wCurLpRegen) * CFormulaTable::m_afRate[400][1]);

		m_pAttribute.wLastBlockRate += WORD(float(m_pAttribute.wLastCon - wCurCon) * CFormulaTable::m_afRate[900][2]);
	}
	else
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wLastCon = UnsignedSafeDecrease<WORD>(m_pAttribute.wLastCon, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fCONNegative -= fValue;
		}

		m_pAttribute.dwLastMaxLP = UnsignedSafeDecrease<WORD>(m_pAttribute.dwLastMaxLP, WORD(float(wCurCon - m_pAttribute.wLastCon) * CFormulaTable::m_afRate[200 + m_pPlayerRef->GetClass() + 1][2]));
		m_pAttribute.wLastLpRegen = UnsignedSafeDecrease<WORD>(m_pAttribute.wLastLpRegen, WORD(float(wCurCon - m_pAttribute.wLastCon) * CFormulaTable::m_afRate[300][2]));
		m_pAttribute.wLastLpSitdownRegen = UnsignedSafeDecrease<WORD>(m_pAttribute.wLastLpSitdownRegen, WORD(float(wCurLpRegen - m_pAttribute.wBaseLpRegen) * CFormulaTable::m_afRate[400][1]));

		m_pAttribute.wLastBlockRate = UnsignedSafeDecrease<WORD>(m_pAttribute.wLastBlockRate, WORD(float(wCurCon - m_pAttribute.wLastCon) * CFormulaTable::m_afRate[900][2]));
	}
}

void CCharacterAttPC::CalculateFoc(float fValue, BYTE byApplyType, bool bIsPlus)
{
	WORD wCurFoc = m_pAttribute.wLastFoc;

	if (bIsPlus == true)
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wLastFoc = UnsignedSafeIncrease<WORD>(m_pAttribute.wLastFoc, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fFOC += fValue;
		}

		m_pAttribute.wLastHitRate += WORD(float(m_pAttribute.wLastFoc - wCurFoc) / CFormulaTable::m_afRate[600][1] * CFormulaTable::m_afRate[600][2]);
		m_pAttribute.wLastCurseSuccessRate += WORD(float(m_pAttribute.wLastFoc - wCurFoc) / CFormulaTable::m_afRate[700][1] * CFormulaTable::m_afRate[700][2]);
		m_pAttribute.wLastEnergyCriticalRate += WORD(float(m_pAttribute.wLastFoc - wCurFoc) / CFormulaTable::m_afRate[2100 + m_pPlayerRef->GetClass() + 1][2]);
		m_pAttribute.fLastEnergyCritDmgRate += float(m_pAttribute.wLastFoc - wCurFoc) / CFormulaTable::m_afRate[2200 + m_pPlayerRef->GetClass() + 1][2];
		m_pAttribute.wLastEnergyOffense += WORD(float(m_pAttribute.wLastFoc - wCurFoc) * CFormulaTable::m_afRate[1100 + m_pPlayerRef->GetClass() + 1][3]);
	}
	else
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wLastFoc = UnsignedSafeDecrease<WORD>(m_pAttribute.wLastFoc, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fFOCNegative -= fValue;
		}

		m_pAttribute.wLastHitRate = UnsignedSafeDecrease<WORD>(m_pAttribute.wLastHitRate, WORD(float(wCurFoc - m_pAttribute.wLastFoc) / CFormulaTable::m_afRate[600][1] * CFormulaTable::m_afRate[600][2]));
		m_pAttribute.wLastCurseSuccessRate = UnsignedSafeDecrease<WORD>(m_pAttribute.wLastCurseSuccessRate, WORD(float(wCurFoc - m_pAttribute.wLastFoc) / CFormulaTable::m_afRate[700][1] * CFormulaTable::m_afRate[700][2]));
		m_pAttribute.wLastEnergyCriticalRate = UnsignedSafeDecrease<WORD>(m_pAttribute.wLastEnergyCriticalRate, WORD(float(wCurFoc - m_pAttribute.wLastFoc) / CFormulaTable::m_afRate[2100 + m_pPlayerRef->GetClass() + 1][2]));
		m_pAttribute.fLastEnergyCritDmgRate = UnsignedSafeDecrease<float>(m_pAttribute.fLastEnergyCritDmgRate, float(wCurFoc - m_pAttribute.wLastFoc) / CFormulaTable::m_afRate[2200 + m_pPlayerRef->GetClass() + 1][2]);
		m_pAttribute.wLastEnergyOffense = UnsignedSafeDecrease<WORD>(m_pAttribute.wLastEnergyOffense, WORD(float(wCurFoc - m_pAttribute.wLastFoc) * CFormulaTable::m_afRate[1100 + m_pPlayerRef->GetClass() + 1][3]));
	}
}

void CCharacterAttPC::CalculateDex(float fValue, BYTE byApplyType, bool bIsPlus)
{
	WORD wCurDex = m_pAttribute.wLastDex;

	if (bIsPlus == true)
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wLastDex = UnsignedSafeIncrease<WORD>(m_pAttribute.wLastDex, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fDEX += fValue;
		}

		m_pAttribute.wLastDodgeRate += WORD(float(m_pAttribute.wLastDex - wCurDex) / CFormulaTable::m_afRate[800][1] * CFormulaTable::m_afRate[800][2]);
		m_pAttribute.wLastCurseToleranceRate += WORD(float(m_pAttribute.wLastDex - wCurDex) / CFormulaTable::m_afRate[1200][1] * CFormulaTable::m_afRate[1200][2]);
		m_pAttribute.wLastPhysicalCriticalRate += WORD(float(m_pAttribute.wLastDex - wCurDex) / CFormulaTable::m_afRate[1900 + m_pPlayerRef->GetClass() + 1][2]);
		m_pAttribute.fLastPhysicalCritDmgRate += float(m_pAttribute.wLastDex - wCurDex) / CFormulaTable::m_afRate[2000 + m_pPlayerRef->GetClass() + 1][2];
		m_pAttribute.wLastPhysicalOffense += WORD(float(m_pAttribute.wLastDex - wCurDex) * CFormulaTable::m_afRate[100 + m_pPlayerRef->GetClass() + 1][3]);
		m_pAttribute.wLastBlockRate += WORD(float(m_pAttribute.wLastDex - wCurDex) * CFormulaTable::m_afRate[900][2]);
		m_pAttribute.wGuardRateLast += WORD(float(m_pAttribute.wLastDex - wCurDex) * CFormulaTable::m_afRate[1000][2]);
	}
	else
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wLastDex = UnsignedSafeDecrease<WORD>(m_pAttribute.wLastDex, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fDEXNegative -= fValue;
		}

		m_pAttribute.wLastDodgeRate = UnsignedSafeDecrease<WORD>(m_pAttribute.wLastDodgeRate, WORD(float(wCurDex - m_pAttribute.wLastDex) / CFormulaTable::m_afRate[800][1] * CFormulaTable::m_afRate[800][2]));
		m_pAttribute.wLastCurseToleranceRate = UnsignedSafeDecrease<WORD>(m_pAttribute.wLastCurseToleranceRate, WORD(float(wCurDex - m_pAttribute.wLastDex) / CFormulaTable::m_afRate[1200][1] * CFormulaTable::m_afRate[1200][2]));
		m_pAttribute.wLastPhysicalCriticalRate = UnsignedSafeDecrease<WORD>(m_pAttribute.wLastPhysicalCriticalRate, WORD(float(wCurDex - m_pAttribute.wLastDex) / CFormulaTable::m_afRate[1900 + m_pPlayerRef->GetClass() + 1][2]));
		m_pAttribute.fLastPhysicalCritDmgRate = UnsignedSafeDecrease<float>(m_pAttribute.fLastPhysicalCritDmgRate, float(wCurDex - m_pAttribute.wLastDex) / CFormulaTable::m_afRate[2000 + m_pPlayerRef->GetClass() + 1][2]);
		m_pAttribute.wLastPhysicalOffense = UnsignedSafeDecrease<WORD>(m_pAttribute.wLastPhysicalOffense, WORD(float(wCurDex - m_pAttribute.wLastDex) / CFormulaTable::m_afRate[100 + m_pPlayerRef->GetClass() + 1][3]));
		m_pAttribute.wLastBlockRate = UnsignedSafeDecrease<WORD>(m_pAttribute.wLastBlockRate, WORD(float(wCurDex - m_pAttribute.wLastDex) * CFormulaTable::m_afRate[900][2]));
		m_pAttribute.wGuardRateLast = UnsignedSafeDecrease<WORD>(m_pAttribute.wGuardRateLast, WORD(float(wCurDex - m_pAttribute.wLastDex) * CFormulaTable::m_afRate[1000][2]));
	}
}

void CCharacterAttPC::CalculateSol(float fValue, BYTE byApplyType, bool bIsPlus)
{
	WORD wCurSol = m_pAttribute.wLastSol;

	if (bIsPlus == true)
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wLastSol = UnsignedSafeIncrease<WORD>(m_pAttribute.wLastSol, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fSOL += fValue;
		}

		m_pAttribute.wLastEnergyOffense += WORD(float(m_pAttribute.wLastSol - wCurSol) * CFormulaTable::m_afRate[1100 + m_pPlayerRef->GetClass() + 1][2]);
	}
	else
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wLastSol = UnsignedSafeDecrease<WORD>(m_pAttribute.wLastSol, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fSOLNegative -= fValue;
		}

		m_pAttribute.wLastEnergyOffense = UnsignedSafeDecrease<WORD>(m_pAttribute.wLastEnergyOffense, WORD(float(wCurSol - m_pAttribute.wLastSol) * CFormulaTable::m_afRate[1100 + m_pPlayerRef->GetClass() + 1][2]));
	}
}

void CCharacterAttPC::CalculateEng(float fValue, BYTE byApplyType, bool bIsPlus)
{
	WORD wCurEng = m_pAttribute.wLastEng;
	WORD wCurEpRegen = m_pAttribute.wLastEpRegen;

	if (bIsPlus == true)
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wLastEng = UnsignedSafeIncrease<WORD>(m_pAttribute.wLastEng, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fENG += fValue;
		}

		m_pAttribute.wLastMaxEP += (WORD)((float)(m_pAttribute.wLastEng - wCurEng) * CFormulaTable::m_afRate[1300 + m_pPlayerRef->GetClass() + 1][2]);
		m_pAttribute.wLastEpRegen += (WORD)((float)(m_pAttribute.wLastEng - wCurEng) * CFormulaTable::m_afRate[1400][2]);
		m_pAttribute.wLastEpSitdownRegen += (WORD)(float(m_pAttribute.wLastEpRegen - wCurEpRegen) * CFormulaTable::m_afRate[1500][1]);
		m_pAttribute.wLastEpBattleRegen += (WORD)((float)(m_pAttribute.wLastEpRegen - wCurEpRegen) * CFormulaTable::m_afRate[1600][1]);
	}
	else
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wLastEng = UnsignedSafeDecrease<WORD>(m_pAttribute.wLastEng, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fENGNegative -= fValue;
		}

		m_pAttribute.wLastMaxEP = UnsignedSafeDecrease<WORD>(m_pAttribute.wLastMaxEP, WORD(float(wCurEng - m_pAttribute.wLastEng) * CFormulaTable::m_afRate[1300 + m_pPlayerRef->GetClass() + 1][2]));
		m_pAttribute.wLastEpRegen = UnsignedSafeDecrease<WORD>(m_pAttribute.wLastEpRegen, WORD(float(wCurEng - m_pAttribute.wLastEng) * CFormulaTable::m_afRate[1400][2]));
		m_pAttribute.wLastEpSitdownRegen = UnsignedSafeDecrease<WORD>(m_pAttribute.wLastEpSitdownRegen, WORD(float(wCurEpRegen - m_pAttribute.wLastEpRegen) * CFormulaTable::m_afRate[1500][1]));
		m_pAttribute.wLastEpBattleRegen = UnsignedSafeDecrease<WORD>(m_pAttribute.wLastEpBattleRegen, WORD(float(wCurEpRegen - m_pAttribute.wLastEpRegen) * CFormulaTable::m_afRate[1600][1]));
	}
}

void CCharacterAttPC::CalculateBattleAttribute(float fValue, BYTE byApplyType, bool bIsPlus)
{
	CCharacterAtt::CalculateBattleAttribute(fValue, byApplyType, bIsPlus);
}


void CCharacterAttPC::CalcSecondWeaponOffence()
{
	m_wSubWeaponPhysicalOffence = m_pAttribute.wBasePhysicalOffense;
	m_wSubWeaponEnergyOffence = m_pAttribute.wBaseEnergyOffense;

	//remove main weapon offence
	CItem* pMainWeapon = m_pPlayerRef->GetPlayerItemContainer()->GetItem(CONTAINER_TYPE_EQUIP, EQUIP_SLOT_TYPE_HAND);
	if (pMainWeapon)
	{
		m_wSubWeaponPhysicalOffence = UnsignedSafeDecrease<WORD>(m_wSubWeaponPhysicalOffence, Dbo_GetFinalOffence(pMainWeapon->GetTbldat()->wPhysical_Offence, pMainWeapon->GetGrade()));
		m_wSubWeaponEnergyOffence = UnsignedSafeDecrease<WORD>(m_wSubWeaponEnergyOffence, Dbo_GetFinalOffence(pMainWeapon->GetTbldat()->wEnergy_Offence, pMainWeapon->GetGrade()));
	}

	//add sub weapon offence
	CItem* pWeapon = m_pPlayerRef->GetPlayerItemContainer()->GetItem(CONTAINER_TYPE_EQUIP, EQUIP_SLOT_TYPE_SUB_WEAPON);
	if (pWeapon)
	{
		m_wSubWeaponPhysicalOffence = UnsignedSafeIncrease<WORD>(m_wSubWeaponPhysicalOffence, Dbo_GetFinalOffence(pWeapon->GetTbldat()->wPhysical_Offence, pWeapon->GetGrade()));
		m_wSubWeaponEnergyOffence = UnsignedSafeIncrease<WORD>(m_wSubWeaponEnergyOffence, Dbo_GetFinalOffence(pWeapon->GetTbldat()->wEnergy_Offence, pWeapon->GetGrade()));
	}
}
