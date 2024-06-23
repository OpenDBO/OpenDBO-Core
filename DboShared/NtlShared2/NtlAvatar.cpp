#include "stdafx.h"
#include "NtlBitFlag.h"
#include "NtlAvatar.h"

#include "NtlBitFlagManager.h"

CNtlAvatar::CNtlAvatar(void)
{
	Init();
}

CNtlAvatar::~CNtlAvatar(void)
{
}

void CNtlAvatar::Init()
{
	InitializeAttributeLink();
}

void CNtlAvatar::InitializeAttributeLink()
{
}

CNtlAvatar* CNtlAvatar::GetInstance()
{
	static CNtlAvatar avatar;
	return &avatar;
}

bool CNtlAvatar::UpdateAvatarAttribute(BYTE byAttributeTotalCount, void* pvRawData, sAVATAR_ATTRIBUTE* pAttributeData)
{
	BYTE* pbyCurrentPosition = (BYTE*)pvRawData;
	BYTE* pbyAttributeData = (BYTE*)pAttributeData;

	CNtlBitFlagManager changedFlag;
	if (false == changedFlag.Create(pvRawData, byAttributeTotalCount))
	{
		return false;
	}

	pbyCurrentPosition += changedFlag.GetBytesUsed();

	for (BYTE byIndex = ATTRIBUTE_TO_UPDATE_FIRST; byIndex <= ATTRIBUTE_TO_UPDATE_LAST; byIndex++)
	{
		if (false != changedFlag.IsSet(byIndex))
		{
			DWORD dwDataSize = m_attributeLogic[byIndex].pCopyAttributeFunction(pbyCurrentPosition, pbyAttributeData + m_attributeLogic[byIndex].dwFieldOffset);
			if (0 == dwDataSize)
			{
				return false;
			}
			else
			{
				pbyCurrentPosition += dwDataSize;
			}
		}
	}

	return true;
}

bool CNtlAvatar::SaveAvatarAttribute(CNtlBitFlagManager* pChangedFlag, sAVATAR_ATTRIBUTE_LINK* pAttributeDataLink, void* pvBuffer, DWORD* pwdDataSize)
{
	BYTE* pbyBuffer = (BYTE*)pvBuffer;
	BYTE* pbyAttributeDataLink = (BYTE*)pAttributeDataLink;

	*pwdDataSize = 0;

	::CopyMemory(pbyBuffer, pChangedFlag->GetRawData(), pChangedFlag->GetBytesUsed());

	pbyBuffer += pChangedFlag->GetBytesUsed();
	*pwdDataSize += pChangedFlag->GetBytesUsed();

	for (BYTE byIndex = ATTRIBUTE_TO_UPDATE_FIRST; byIndex <= ATTRIBUTE_TO_UPDATE_LAST; byIndex++)
	{
		if (false != pChangedFlag->IsSet(byIndex))
		{
			void* pvAttributeFieldLink = *((void**)(pbyAttributeDataLink + byIndex * sizeof(void*)));
			if (NULL == pvAttributeFieldLink)
			{
				continue;
			}

			DWORD dwDataSize = m_attributeLogic[byIndex].pCopyAttributeFunction(pvAttributeFieldLink, pbyBuffer);
			if (0 == dwDataSize)
			{
				return false;
			}
			else
			{
				pbyBuffer += dwDataSize;
				*pwdDataSize += dwDataSize;
			}
		}
	}

	return true;
}

bool CNtlAvatar::SaveAvatarAttribute(CNtlBitFlagManager* pChangedFlag, sAVATAR_ATTRIBUTE* pAttributeData, void* pvBuffer, DWORD* pwdDataSize)
{
	BYTE* pbyBuffer = (BYTE*)pvBuffer;
	BYTE* pbyAttributeData = (BYTE*)pAttributeData;

	*pwdDataSize = 0;

	::CopyMemory(pbyBuffer, pChangedFlag->GetRawData(), pChangedFlag->GetBytesUsed());

	pbyBuffer += pChangedFlag->GetBytesUsed();
	*pwdDataSize += pChangedFlag->GetBytesUsed();

	for (BYTE byIndex = ATTRIBUTE_TO_UPDATE_FIRST; byIndex <= ATTRIBUTE_TO_UPDATE_LAST; byIndex++)
	{
		if (false != pChangedFlag->IsSet(byIndex))
		{
			void* pvAttributeFieldLink = *((void**)(pbyAttributeData + byIndex * sizeof(void*)));
			if (NULL == pvAttributeFieldLink)
			{
				continue;
			}

			DWORD dwDataSize = m_attributeLogic[byIndex].pCopyAttributeFunction(pvAttributeFieldLink, pbyBuffer);
			if (0 == dwDataSize)
			{
				return false;
			}
			else
			{
				pbyBuffer += dwDataSize;
				*pwdDataSize += dwDataSize;
			}
		}
	}

	return true;
}

bool CNtlAvatar::FillAvatarAttribute(sAVATAR_ATTRIBUTE_LINK* pAttributeDataLink, sAVATAR_ATTRIBUTE* pAttributeData)
{
	BYTE* pbyAttributeDataLink = (BYTE*)pAttributeDataLink;
	BYTE* pbyAttributeData = (BYTE*)pAttributeData;

	for (BYTE byIndex = ATTRIBUTE_TO_UPDATE_FIRST; byIndex <= ATTRIBUTE_TO_UPDATE_LAST; byIndex++)
	{
		void* pvAttributeFieldLink = *((void**)(pbyAttributeDataLink + byIndex * sizeof(void*)));
		if (NULL == pvAttributeFieldLink)
		{
			continue;
		}

		DWORD dwDataSize = m_attributeLogic[byIndex].pCopyAttributeFunction(
			pvAttributeFieldLink,
			pbyAttributeData + m_attributeLogic[byIndex].dwFieldOffset);
		if (0 == dwDataSize)
		{
			return false;
		}
	}

	return true;
}

#ifndef ATTRIBUTE_LOGIC_DEFINE
#define ATTRIBUTE_LOGIC_DEFINE(field_name, type)					\
	{																\
		PtrToUlong(&(((sAVATAR_ATTRIBUTE*)NULL)->field_name)),		\
		CopyValueByType_##type										\
	}
#endif

CNtlAvatar::sATTRIBUTE_LOGIC CNtlAvatar::m_attributeLogic[AVATAR_ATTRIBUTE_COUNT] =
{
		ATTRIBUTE_LOGIC_DEFINE(wBaseStr, WORD),
		ATTRIBUTE_LOGIC_DEFINE(wLastStr, WORD),
		ATTRIBUTE_LOGIC_DEFINE(wBaseCon, WORD),
		ATTRIBUTE_LOGIC_DEFINE(wLastCon, WORD),
		ATTRIBUTE_LOGIC_DEFINE(wBaseFoc, WORD),
		ATTRIBUTE_LOGIC_DEFINE(wLastFoc, WORD),
		ATTRIBUTE_LOGIC_DEFINE(wBaseDex, WORD),
		ATTRIBUTE_LOGIC_DEFINE(wLastDex, WORD),
		ATTRIBUTE_LOGIC_DEFINE(wBaseSol, WORD),
		ATTRIBUTE_LOGIC_DEFINE(wLastSol, WORD),
		ATTRIBUTE_LOGIC_DEFINE(wBaseEng, WORD),
		ATTRIBUTE_LOGIC_DEFINE(wLastEng, WORD),

		ATTRIBUTE_LOGIC_DEFINE(dwBaseMaxLP, DWORD),
		ATTRIBUTE_LOGIC_DEFINE(dwLastMaxLP, DWORD),
		ATTRIBUTE_LOGIC_DEFINE(wBaseMaxEP, WORD),
		ATTRIBUTE_LOGIC_DEFINE(wLastMaxEP, WORD),
		ATTRIBUTE_LOGIC_DEFINE(dwBaseMaxAp, DWORD),
		ATTRIBUTE_LOGIC_DEFINE(dwLastMaxAp, DWORD),
		ATTRIBUTE_LOGIC_DEFINE(wBaseMaxRP, WORD),
		ATTRIBUTE_LOGIC_DEFINE(wLastMaxRP, WORD),

		ATTRIBUTE_LOGIC_DEFINE(wBaseLpRegen, WORD),
		ATTRIBUTE_LOGIC_DEFINE(wLastLpRegen, WORD),
		ATTRIBUTE_LOGIC_DEFINE(wBaseLpSitdownRegen, WORD),
		ATTRIBUTE_LOGIC_DEFINE(wLastLpSitdownRegen, WORD),
		ATTRIBUTE_LOGIC_DEFINE(wBaseLpBattleRegen, WORD),
		ATTRIBUTE_LOGIC_DEFINE(wLastLpBattleRegen, WORD),

		ATTRIBUTE_LOGIC_DEFINE(wBaseEpRegen, WORD),
		ATTRIBUTE_LOGIC_DEFINE(wLastEpRegen, WORD),
		ATTRIBUTE_LOGIC_DEFINE(wBaseEpSitdownRegen, WORD),
		ATTRIBUTE_LOGIC_DEFINE(wLastEpSitdownRegen, WORD),
		ATTRIBUTE_LOGIC_DEFINE(wBaseEpBattleRegen, WORD),
		ATTRIBUTE_LOGIC_DEFINE(wLastEpBattleRegen, WORD),

		ATTRIBUTE_LOGIC_DEFINE(wBaseApRegen, WORD),
		ATTRIBUTE_LOGIC_DEFINE(wLastApRegen, WORD),
		ATTRIBUTE_LOGIC_DEFINE(wBaseApSitdownRegen, WORD),
		ATTRIBUTE_LOGIC_DEFINE(wLastApSitdownRegen, WORD),
		ATTRIBUTE_LOGIC_DEFINE(wBaseApBattleRegen, WORD),
		ATTRIBUTE_LOGIC_DEFINE(wLastApBattleRegen, WORD),

		ATTRIBUTE_LOGIC_DEFINE(wAirAP_Degen, WORD),
		ATTRIBUTE_LOGIC_DEFINE(wAirAP_DegenFly, WORD),
		ATTRIBUTE_LOGIC_DEFINE(wAirAP_DegenDash, WORD),
		ATTRIBUTE_LOGIC_DEFINE(wAirAP_DegenAccel, WORD),

		ATTRIBUTE_LOGIC_DEFINE(wBaseRpRegen, WORD),
		ATTRIBUTE_LOGIC_DEFINE(wLastRpRegen, WORD),
		ATTRIBUTE_LOGIC_DEFINE(wLastRpDiminutionRate, WORD),

		ATTRIBUTE_LOGIC_DEFINE(wBasePhysicalOffense, WORD),
		ATTRIBUTE_LOGIC_DEFINE(wLastPhysicalOffense, WORD),
		ATTRIBUTE_LOGIC_DEFINE(wBasePhysicalDefense, WORD),
		ATTRIBUTE_LOGIC_DEFINE(wLastPhysicalDefense, WORD),

		ATTRIBUTE_LOGIC_DEFINE(wBaseEnergyOffense, WORD),
		ATTRIBUTE_LOGIC_DEFINE(wLastEnergyOffense, WORD),
		ATTRIBUTE_LOGIC_DEFINE(wBaseEnergyDefense, WORD),
		ATTRIBUTE_LOGIC_DEFINE(wLastEnergyDefense, WORD),

		ATTRIBUTE_LOGIC_DEFINE(wBaseHitRate, WORD),
		ATTRIBUTE_LOGIC_DEFINE(wLastHitRate, WORD),
		ATTRIBUTE_LOGIC_DEFINE(wBaseDodgeRate, WORD),
		ATTRIBUTE_LOGIC_DEFINE(wLastDodgeRate, WORD),
		ATTRIBUTE_LOGIC_DEFINE(wBaseBlockRate, WORD),
		ATTRIBUTE_LOGIC_DEFINE(wLastBlockRate, WORD),

		ATTRIBUTE_LOGIC_DEFINE(wBaseBlockDMG, WORD),
		ATTRIBUTE_LOGIC_DEFINE(wLastBlockDMG, WORD),

		ATTRIBUTE_LOGIC_DEFINE(wBaseCurseSuccessRate, WORD),
		ATTRIBUTE_LOGIC_DEFINE(wLastCurseSuccessRate, WORD),
		ATTRIBUTE_LOGIC_DEFINE(wBaseCurseToleranceRate, WORD),
		ATTRIBUTE_LOGIC_DEFINE(wLastCurseToleranceRate, WORD),

		ATTRIBUTE_LOGIC_DEFINE(wBasePhysicalCriticalRate, WORD),
		ATTRIBUTE_LOGIC_DEFINE(wLastPhysicalCriticalRate, WORD),
		ATTRIBUTE_LOGIC_DEFINE(wBaseEnergyCriticalRate, WORD),
		ATTRIBUTE_LOGIC_DEFINE(wLastEnergyCriticalRate, WORD),

		ATTRIBUTE_LOGIC_DEFINE(fBasePhysicalCritDmgRate, FLOAT),
		ATTRIBUTE_LOGIC_DEFINE(fLastPhysicalCritDmgRate, FLOAT),
		ATTRIBUTE_LOGIC_DEFINE(fBaseEnergyCritDmgRate, FLOAT),
		ATTRIBUTE_LOGIC_DEFINE(fLastEnergyCritDmgRate, FLOAT),

		ATTRIBUTE_LOGIC_DEFINE(fBaseRunSpeed, FLOAT),
		ATTRIBUTE_LOGIC_DEFINE(fLastRunSpeed, FLOAT),
		ATTRIBUTE_LOGIC_DEFINE(fBaseAirSpeed, FLOAT),
		ATTRIBUTE_LOGIC_DEFINE(fLastAirSpeed, FLOAT),
		ATTRIBUTE_LOGIC_DEFINE(fBaseAirDashSpeed, FLOAT),
		ATTRIBUTE_LOGIC_DEFINE(fLastAirDashSpeed, FLOAT),
		ATTRIBUTE_LOGIC_DEFINE(fBaseAirDashAccelSpeed, FLOAT),
		ATTRIBUTE_LOGIC_DEFINE(fLastAirDashAccelSpeed, FLOAT),

		ATTRIBUTE_LOGIC_DEFINE(wBaseAttackSpeedRate, WORD),
		ATTRIBUTE_LOGIC_DEFINE(wLastAttackSpeedRate, WORD),
		ATTRIBUTE_LOGIC_DEFINE(fBaseAttackRange, FLOAT),
		ATTRIBUTE_LOGIC_DEFINE(fLastAttackRange, FLOAT),

		ATTRIBUTE_LOGIC_DEFINE(fCastingTimeChangePercent, FLOAT),
		ATTRIBUTE_LOGIC_DEFINE(fCoolTimeChangePercent, FLOAT),
		ATTRIBUTE_LOGIC_DEFINE(fKeepTimeChangePercent, FLOAT),
		ATTRIBUTE_LOGIC_DEFINE(fDotKeepTimeChangeInSeconds, FLOAT),
		ATTRIBUTE_LOGIC_DEFINE(fRequiredEpChangePercent, FLOAT),

		ATTRIBUTE_LOGIC_DEFINE(fHonestOffense, FLOAT),
		ATTRIBUTE_LOGIC_DEFINE(fHonestDefense, FLOAT),
		ATTRIBUTE_LOGIC_DEFINE(fStrangeOffense, FLOAT),
		ATTRIBUTE_LOGIC_DEFINE(fStrangeDefense, FLOAT),
		ATTRIBUTE_LOGIC_DEFINE(fWildOffense, FLOAT),
		ATTRIBUTE_LOGIC_DEFINE(fWildDefense, FLOAT),
		ATTRIBUTE_LOGIC_DEFINE(fEleganceOffense, FLOAT),
		ATTRIBUTE_LOGIC_DEFINE(fEleganceDefense, FLOAT),
		ATTRIBUTE_LOGIC_DEFINE(fFunnyOffense, FLOAT),
		ATTRIBUTE_LOGIC_DEFINE(fFunnyDefense, FLOAT),

		ATTRIBUTE_LOGIC_DEFINE(fPhysicalReflect, FLOAT),
		ATTRIBUTE_LOGIC_DEFINE(fEnergyReflect, FLOAT),

		ATTRIBUTE_LOGIC_DEFINE(wParalyzeToleranceRate, WORD),
		ATTRIBUTE_LOGIC_DEFINE(wTerrorToleranceRate, WORD),
		ATTRIBUTE_LOGIC_DEFINE(wConfuseToleranceRate, WORD),
		ATTRIBUTE_LOGIC_DEFINE(wStoneToleranceRate, WORD),
		ATTRIBUTE_LOGIC_DEFINE(wCandyToleranceRate, WORD),

		ATTRIBUTE_LOGIC_DEFINE(fParalyzeKeepTimeDown, FLOAT),
		ATTRIBUTE_LOGIC_DEFINE(fTerrorKeepTimeDown, FLOAT),
		ATTRIBUTE_LOGIC_DEFINE(fConfuseKeepTimeDown, FLOAT),
		ATTRIBUTE_LOGIC_DEFINE(fStoneKeepTimeDown, FLOAT),
		ATTRIBUTE_LOGIC_DEFINE(fCandyKeepTimeDown, FLOAT),
		ATTRIBUTE_LOGIC_DEFINE(fBleedingKeepTimeDown, FLOAT),
		ATTRIBUTE_LOGIC_DEFINE(fPoisonKeepTimeDown, FLOAT),
		ATTRIBUTE_LOGIC_DEFINE(fStomachacheKeepTimeDown, FLOAT),

		ATTRIBUTE_LOGIC_DEFINE(fCriticalBlockSuccessRate, FLOAT),

		ATTRIBUTE_LOGIC_DEFINE(dwLpRecoveryWhenHit, DWORD),
		ATTRIBUTE_LOGIC_DEFINE(fLpRecoveryWhenHitInPercent, FLOAT),
		ATTRIBUTE_LOGIC_DEFINE(dwEpRecoveryWhenHit, DWORD),
		ATTRIBUTE_LOGIC_DEFINE(fEpRecoveryWhenHitInPercent, FLOAT),

		ATTRIBUTE_LOGIC_DEFINE(wBaseAbdominalPainDefense, WORD),
		ATTRIBUTE_LOGIC_DEFINE(wLastAbdominalPainDefense, WORD),
		ATTRIBUTE_LOGIC_DEFINE(wBasePoisonDefense, WORD),
		ATTRIBUTE_LOGIC_DEFINE(wLastPoisonDefense, WORD),
		ATTRIBUTE_LOGIC_DEFINE(wBaseBleedingDefense, WORD),
		ATTRIBUTE_LOGIC_DEFINE(wLastBleedingDefense, WORD),
		ATTRIBUTE_LOGIC_DEFINE(wBaseBurnDefense, WORD),
		ATTRIBUTE_LOGIC_DEFINE(wLastBurnDefense, WORD),

		ATTRIBUTE_LOGIC_DEFINE(fBaseMindCurseImmunity, FLOAT),
		ATTRIBUTE_LOGIC_DEFINE(fLastMindCurseImmunity, FLOAT),
		ATTRIBUTE_LOGIC_DEFINE(fBaseBodyCurseImmunity, FLOAT),
		ATTRIBUTE_LOGIC_DEFINE(fLastBodyCurseImmunity, FLOAT),
		ATTRIBUTE_LOGIC_DEFINE(fBaseChangeCurseImmunity, FLOAT),
		ATTRIBUTE_LOGIC_DEFINE(fLastChangeCurseImmunity, FLOAT),
		ATTRIBUTE_LOGIC_DEFINE(fBaseSkillAnimationSpeed, FLOAT),
		ATTRIBUTE_LOGIC_DEFINE(fLastSkillAnimationSpeed, FLOAT),
		ATTRIBUTE_LOGIC_DEFINE(dwBaseWeightMax, DWORD),
		ATTRIBUTE_LOGIC_DEFINE(dwLastWeightMax, DWORD),

		ATTRIBUTE_LOGIC_DEFINE(fSkillAggroBonus, FLOAT),
		ATTRIBUTE_LOGIC_DEFINE(fSkillAggroBonusInPercent, FLOAT),
		ATTRIBUTE_LOGIC_DEFINE(fDirectHealPowerBonus, FLOAT),
		ATTRIBUTE_LOGIC_DEFINE(fDirectHealPowerBonusInPercent, FLOAT),
		ATTRIBUTE_LOGIC_DEFINE(fHotPowerBonus, FLOAT),
		ATTRIBUTE_LOGIC_DEFINE(fHotPowerBonusInPercent, FLOAT),
		ATTRIBUTE_LOGIC_DEFINE(fDotValueChangePercent, FLOAT),

		ATTRIBUTE_LOGIC_DEFINE(fPhysicalCriticalDefenseRate, DWORD),
		ATTRIBUTE_LOGIC_DEFINE(fEnergyCriticalDefenseRate, DWORD),
		ATTRIBUTE_LOGIC_DEFINE(wGuardRateBase, WORD),
		ATTRIBUTE_LOGIC_DEFINE(wGuardRateLast, WORD),

		ATTRIBUTE_LOGIC_DEFINE(fSkillDamageBlockModeSuccessRate, FLOAT),
		ATTRIBUTE_LOGIC_DEFINE(fCurseBlockModeSuccessRate, FLOAT),
		ATTRIBUTE_LOGIC_DEFINE(fKnockdownBlockModeSuccessRate, FLOAT),
		ATTRIBUTE_LOGIC_DEFINE(fHtbBlockModeSuccessRate, FLOAT),
		ATTRIBUTE_LOGIC_DEFINE(fItemUpgradeBonusRate, FLOAT),
		ATTRIBUTE_LOGIC_DEFINE(fItemUpgradeBreakBonusRate, FLOAT),

		ATTRIBUTE_LOGIC_DEFINE(byExpBooster, BYTE),
		ATTRIBUTE_LOGIC_DEFINE(byQuestDropRate, BYTE),

		//ATTRIBUTE_LOGIC_DEFINE(unkTest, BYTE),
};

#undef ATTRIBUTE_LOGIC_DEFINE

DWORD CNtlAvatar::CopyValueByType_BYTE(void* pvValue, void* pvBuffer)
{
	BYTE* pbyBuffer = (BYTE*)pvBuffer;
	*pbyBuffer = *((BYTE*)pvValue);
	return sizeof(BYTE);
}
DWORD CNtlAvatar::CopyValueByType_WORD(void* pvValue, void* pvBuffer)
{
	WORD* pbyBuffer = (WORD*)pvBuffer;
	*pbyBuffer = *((WORD*)pvValue);
	return sizeof(WORD);
}
DWORD CNtlAvatar::CopyValueByType_float(void* pvValue, void* pvBuffer)
{
	float* pbyBuffer = (float*)pvBuffer;
	*pbyBuffer = *((float*)pvValue);
	return sizeof(float);
}

DWORD CNtlAvatar::CopyValueByType_FLOAT(void* pvValue, void* pvBuffer) {
	FLOAT* pbyBuffer = (FLOAT*)pvBuffer;
	*pbyBuffer = *((FLOAT*)pvValue);
	return sizeof(FLOAT);
}

DWORD CNtlAvatar::CopyValueByType_DWORD(void* pvValue, void* pvBuffer)
{
	DWORD* pbyBuffer = (DWORD*)pvBuffer;
	*pbyBuffer = *((DWORD*)pvValue);
	return sizeof(DWORD);
}
DWORD CNtlAvatar::CopyValueByType_int(void* pvValue, void* pvBuffer)
{
	int* pbyBuffer = (int*)pvBuffer;
	*pbyBuffer = *((int*)pvValue);
	return sizeof(int);
}



bool CNtlAvatar::FillAvatarAttributeNew(sAVATAR_ATTRIBUTE* pAttributeData, sAVATAR_ATTRIBUTE_LINK* pAttributeDataLink)
{
	BYTE* pbyAttributeDataLink = (BYTE*)pAttributeDataLink;
	BYTE* pbyAttributeData = (BYTE*)pAttributeData;

	for (BYTE byIndex = ATTRIBUTE_TO_UPDATE_FIRST; byIndex <= ATTRIBUTE_TO_UPDATE_LAST; byIndex++)
	{
		void* pvAttributeFieldLink = ((void*)(pbyAttributeData + byIndex * sizeof(void*)));
		if (NULL == pvAttributeFieldLink)
		{
			continue;
		}

		DWORD dwDataSize = m_attributeLogicNew[byIndex].pCopyAttributeFunction(
			pvAttributeFieldLink,
			pbyAttributeDataLink + m_attributeLogic[byIndex].dwFieldOffset);
		if (0 == dwDataSize)
		{
			return false;
		}
	}

	return true;
}



#ifndef ATTRIBUTE_LOGIC_DEFINE_NEW
#define ATTRIBUTE_LOGIC_DEFINE_NEW(field_name, type)						\
	{																		\
		PtrToUlong(&(((sAVATAR_ATTRIBUTE_LINK*)NULL)->field_name)),			\
		CopyValueByTypeNew_##type											\
	}
#endif

CNtlAvatar::sATTRIBUTE_LOGIC CNtlAvatar::m_attributeLogicNew[AVATAR_ATTRIBUTE_COUNT] =
{
		ATTRIBUTE_LOGIC_DEFINE_NEW(pwBaseStr, WORD),
		ATTRIBUTE_LOGIC_DEFINE_NEW(pwLastStr, WORD),
		ATTRIBUTE_LOGIC_DEFINE_NEW(pwBaseCon, WORD),
		ATTRIBUTE_LOGIC_DEFINE_NEW(pwLastCon, WORD),
		ATTRIBUTE_LOGIC_DEFINE_NEW(pwBaseFoc, WORD),
		ATTRIBUTE_LOGIC_DEFINE_NEW(pwLastFoc, WORD),
		ATTRIBUTE_LOGIC_DEFINE_NEW(pwBaseDex, WORD),
		ATTRIBUTE_LOGIC_DEFINE_NEW(pwLastDex, WORD),
		ATTRIBUTE_LOGIC_DEFINE_NEW(pwBaseSol, WORD),
		ATTRIBUTE_LOGIC_DEFINE_NEW(pwLastSol, WORD),
		ATTRIBUTE_LOGIC_DEFINE_NEW(pwBaseEng, WORD),
		ATTRIBUTE_LOGIC_DEFINE_NEW(pwLastEng, WORD),

		ATTRIBUTE_LOGIC_DEFINE_NEW(pdwBaseMaxLP, DWORD),
		ATTRIBUTE_LOGIC_DEFINE_NEW(pdwLastMaxLP, DWORD),
		ATTRIBUTE_LOGIC_DEFINE_NEW(pwBaseMaxEP, WORD),
		ATTRIBUTE_LOGIC_DEFINE_NEW(pwLastMaxEP, WORD),
		ATTRIBUTE_LOGIC_DEFINE_NEW(pdwBaseMaxAp, DWORD),
		ATTRIBUTE_LOGIC_DEFINE_NEW(pdwLastMaxAp, DWORD),
		ATTRIBUTE_LOGIC_DEFINE_NEW(pwBaseMaxRP, WORD),
		ATTRIBUTE_LOGIC_DEFINE_NEW(pwLastMaxRP, WORD),

		ATTRIBUTE_LOGIC_DEFINE_NEW(pwBaseLpRegen, WORD),
		ATTRIBUTE_LOGIC_DEFINE_NEW(pwLastLpRegen, WORD),
		ATTRIBUTE_LOGIC_DEFINE_NEW(pwBaseLpSitdownRegen, WORD),
		ATTRIBUTE_LOGIC_DEFINE_NEW(pwLastLpSitdownRegen, WORD),
		ATTRIBUTE_LOGIC_DEFINE_NEW(pwBaseLpBattleRegen, WORD),
		ATTRIBUTE_LOGIC_DEFINE_NEW(pwLastLpBattleRegen, WORD),

		ATTRIBUTE_LOGIC_DEFINE_NEW(pwBaseEpRegen, WORD),
		ATTRIBUTE_LOGIC_DEFINE_NEW(pwLastEpRegen, WORD),
		ATTRIBUTE_LOGIC_DEFINE_NEW(pwBaseEpSitdownRegen, WORD),
		ATTRIBUTE_LOGIC_DEFINE_NEW(pwLastEpSitdownRegen, WORD),
		ATTRIBUTE_LOGIC_DEFINE_NEW(pwBaseEpBattleRegen, WORD),
		ATTRIBUTE_LOGIC_DEFINE_NEW(pwLastEpBattleRegen, WORD),

		ATTRIBUTE_LOGIC_DEFINE_NEW(pwBaseApRegen, WORD),
		ATTRIBUTE_LOGIC_DEFINE_NEW(pwLastApRegen, WORD),
		ATTRIBUTE_LOGIC_DEFINE_NEW(pwBaseApSitdownRegen, WORD),
		ATTRIBUTE_LOGIC_DEFINE_NEW(pwLastApSitdownRegen, WORD),
		ATTRIBUTE_LOGIC_DEFINE_NEW(pwBaseApBattleRegen, WORD),
		ATTRIBUTE_LOGIC_DEFINE_NEW(pwLastApBattleRegen, WORD),

		ATTRIBUTE_LOGIC_DEFINE_NEW(pwAirAPDegen, WORD),
		ATTRIBUTE_LOGIC_DEFINE_NEW(pwAirAPFlyDegen, WORD),
		ATTRIBUTE_LOGIC_DEFINE_NEW(pwAirAPFLyDashDegen, WORD),
		ATTRIBUTE_LOGIC_DEFINE_NEW(pwAirAPFlyDashBoostDegen, WORD),

		ATTRIBUTE_LOGIC_DEFINE_NEW(pwBaseRpRegen, WORD),
		ATTRIBUTE_LOGIC_DEFINE_NEW(pwLastRpRegen, WORD),
		ATTRIBUTE_LOGIC_DEFINE_NEW(pwLastRpDiminutionRate, WORD),

		ATTRIBUTE_LOGIC_DEFINE_NEW(pwBasePhysicalOffense, WORD),
		ATTRIBUTE_LOGIC_DEFINE_NEW(pwLastPhysicalOffense, WORD),
		ATTRIBUTE_LOGIC_DEFINE_NEW(pwBasePhysicalDefense, WORD),
		ATTRIBUTE_LOGIC_DEFINE_NEW(pwLastPhysicalDefense, WORD),

		ATTRIBUTE_LOGIC_DEFINE_NEW(pwBaseEnergyOffense, WORD),
		ATTRIBUTE_LOGIC_DEFINE_NEW(pwLastEnergyOffense, WORD),
		ATTRIBUTE_LOGIC_DEFINE_NEW(pwBaseEnergyDefense, WORD),
		ATTRIBUTE_LOGIC_DEFINE_NEW(pwLastEnergyDefense, WORD),

		ATTRIBUTE_LOGIC_DEFINE_NEW(pwBaseHitRate, WORD),
		ATTRIBUTE_LOGIC_DEFINE_NEW(pwLastHitRate, WORD),
		ATTRIBUTE_LOGIC_DEFINE_NEW(pwBaseDodgeRate, WORD),
		ATTRIBUTE_LOGIC_DEFINE_NEW(pwLastDodgeRate, WORD),
		ATTRIBUTE_LOGIC_DEFINE_NEW(pwBaseBlockRate, WORD),
		ATTRIBUTE_LOGIC_DEFINE_NEW(pwLastBlockRate, WORD),

		ATTRIBUTE_LOGIC_DEFINE_NEW(pwBaseBlockDMG, WORD),
		ATTRIBUTE_LOGIC_DEFINE_NEW(pwLastBlockDMG, WORD),

		ATTRIBUTE_LOGIC_DEFINE_NEW(pwBaseCurseSuccessRate, WORD),
		ATTRIBUTE_LOGIC_DEFINE_NEW(pwLastCurseSuccessRate, WORD),
		ATTRIBUTE_LOGIC_DEFINE_NEW(pwBaseCurseToleranceRate, WORD),
		ATTRIBUTE_LOGIC_DEFINE_NEW(pwLastCurseToleranceRate, WORD),

		ATTRIBUTE_LOGIC_DEFINE_NEW(pwBasePhysicalCriticalRate, WORD),
		ATTRIBUTE_LOGIC_DEFINE_NEW(pwLastPhysicalCriticalRate, WORD),
		ATTRIBUTE_LOGIC_DEFINE_NEW(pwBaseEnergyCriticalRate, WORD),
		ATTRIBUTE_LOGIC_DEFINE_NEW(pwLastEnergyCriticalRate, WORD),

		ATTRIBUTE_LOGIC_DEFINE_NEW(pfBasePhysicalCritDmgRate, FLOAT),
		ATTRIBUTE_LOGIC_DEFINE_NEW(pfLastPhysicalCritDmgRate, FLOAT),
		ATTRIBUTE_LOGIC_DEFINE_NEW(pfBaseEnergyCritDmgRate, FLOAT),
		ATTRIBUTE_LOGIC_DEFINE_NEW(pfLastEnergyCritDmgRate, FLOAT),

		ATTRIBUTE_LOGIC_DEFINE_NEW(pfBaseRunSpeed, FLOAT),
		ATTRIBUTE_LOGIC_DEFINE_NEW(pfLastRunSpeed, FLOAT),
		ATTRIBUTE_LOGIC_DEFINE_NEW(pfBaseAirSpeed, FLOAT),
		ATTRIBUTE_LOGIC_DEFINE_NEW(pfLastAirSpeed, FLOAT),
		ATTRIBUTE_LOGIC_DEFINE_NEW(pfBaseAirDashSpeed, FLOAT),
		ATTRIBUTE_LOGIC_DEFINE_NEW(pfLastAirDashSpeed, FLOAT),
		ATTRIBUTE_LOGIC_DEFINE_NEW(pfBaseAirDashAccelSpeed, FLOAT),
		ATTRIBUTE_LOGIC_DEFINE_NEW(pfLastAirDashAccelSpeed, FLOAT),

		ATTRIBUTE_LOGIC_DEFINE_NEW(pwBaseAttackSpeedRate, WORD),
		ATTRIBUTE_LOGIC_DEFINE_NEW(pwLastAttackSpeedRate, WORD),
		ATTRIBUTE_LOGIC_DEFINE_NEW(pfBaseAttackRange, FLOAT),
		ATTRIBUTE_LOGIC_DEFINE_NEW(pfLastAttackRange, FLOAT),

		ATTRIBUTE_LOGIC_DEFINE_NEW(pfCastingTimeChangePercent, FLOAT),
		ATTRIBUTE_LOGIC_DEFINE_NEW(pfCoolTimeChangePercent, FLOAT),
		ATTRIBUTE_LOGIC_DEFINE_NEW(pfKeepTimeChangePercent, FLOAT),
		ATTRIBUTE_LOGIC_DEFINE_NEW(pfDotKeepTimeChangeInSeconds, FLOAT),
		ATTRIBUTE_LOGIC_DEFINE_NEW(pfRequiredEpChangePercent, FLOAT),

		ATTRIBUTE_LOGIC_DEFINE_NEW(pfHonestOffense, FLOAT),
		ATTRIBUTE_LOGIC_DEFINE_NEW(pfHonestDefense, FLOAT),
		ATTRIBUTE_LOGIC_DEFINE_NEW(pfStrangeOffense, FLOAT),
		ATTRIBUTE_LOGIC_DEFINE_NEW(pfStrangeDefense, FLOAT),
		ATTRIBUTE_LOGIC_DEFINE_NEW(pfWildOffense, FLOAT),
		ATTRIBUTE_LOGIC_DEFINE_NEW(pfWildDefense, FLOAT),
		ATTRIBUTE_LOGIC_DEFINE_NEW(pfEleganceOffense, FLOAT),
		ATTRIBUTE_LOGIC_DEFINE_NEW(pfEleganceDefense, FLOAT),
		ATTRIBUTE_LOGIC_DEFINE_NEW(pfFunnyOffense, FLOAT),
		ATTRIBUTE_LOGIC_DEFINE_NEW(pfFunnyDefense, FLOAT),

		ATTRIBUTE_LOGIC_DEFINE_NEW(pfPhysicalReflect, FLOAT),
		ATTRIBUTE_LOGIC_DEFINE_NEW(pfEnergyReflect, FLOAT),

		ATTRIBUTE_LOGIC_DEFINE_NEW(pwParalyzeToleranceRate, WORD),
		ATTRIBUTE_LOGIC_DEFINE_NEW(pwTerrorToleranceRate, WORD),
		ATTRIBUTE_LOGIC_DEFINE_NEW(pwConfuseToleranceRate, WORD),
		ATTRIBUTE_LOGIC_DEFINE_NEW(pwStoneToleranceRate, WORD),
		ATTRIBUTE_LOGIC_DEFINE_NEW(pwCandyToleranceRate, WORD),

		ATTRIBUTE_LOGIC_DEFINE_NEW(pfParalyzeKeepTimeDown, FLOAT),
		ATTRIBUTE_LOGIC_DEFINE_NEW(pfTerrorKeepTimeDown, FLOAT),
		ATTRIBUTE_LOGIC_DEFINE_NEW(pfConfuseKeepTimeDown, FLOAT),
		ATTRIBUTE_LOGIC_DEFINE_NEW(pfStoneKeepTimeDown, FLOAT),
		ATTRIBUTE_LOGIC_DEFINE_NEW(pfCandyKeepTimeDown, FLOAT),
		ATTRIBUTE_LOGIC_DEFINE_NEW(pfBleedingKeepTimeDown, FLOAT),
		ATTRIBUTE_LOGIC_DEFINE_NEW(pfPoisonKeepTimeDown, FLOAT),
		ATTRIBUTE_LOGIC_DEFINE_NEW(pfStomachacheKeepTimeDown, FLOAT),

		ATTRIBUTE_LOGIC_DEFINE_NEW(pfCriticalBlockSuccessRate, FLOAT),

		ATTRIBUTE_LOGIC_DEFINE_NEW(pdwLpRecoveryWhenHit, DWORD),
		ATTRIBUTE_LOGIC_DEFINE_NEW(pfLpRecoveryWhenHitInPercent, FLOAT),
		ATTRIBUTE_LOGIC_DEFINE_NEW(pdwEpRecoveryWhenHit, DWORD),
		ATTRIBUTE_LOGIC_DEFINE_NEW(pfEpRecoveryWhenHitInPercent, FLOAT),

		ATTRIBUTE_LOGIC_DEFINE_NEW(pwBaseAbdominalPainDefense, WORD),
		ATTRIBUTE_LOGIC_DEFINE_NEW(pwLastAbdominalPainDefense, WORD),
		ATTRIBUTE_LOGIC_DEFINE_NEW(pwBasePoisonDefense, WORD),
		ATTRIBUTE_LOGIC_DEFINE_NEW(pwLastPoisonDefense, WORD),
		ATTRIBUTE_LOGIC_DEFINE_NEW(pwBaseBleedingDefense, WORD),
		ATTRIBUTE_LOGIC_DEFINE_NEW(pwLastBleedingDefense, WORD),
		ATTRIBUTE_LOGIC_DEFINE_NEW(pwBaseBurnDefense, WORD),
		ATTRIBUTE_LOGIC_DEFINE_NEW(pwLastBurnDefense, WORD),

		ATTRIBUTE_LOGIC_DEFINE_NEW(pfBaseMindCurseImmunity, FLOAT),
		ATTRIBUTE_LOGIC_DEFINE_NEW(pfLastMindCurseImmunity, FLOAT),
		ATTRIBUTE_LOGIC_DEFINE_NEW(pfBaseBodyCurseImmunity, FLOAT),
		ATTRIBUTE_LOGIC_DEFINE_NEW(pfLastBodyCurseImmunity, FLOAT),
		ATTRIBUTE_LOGIC_DEFINE_NEW(pfBaseChangeCurseImmunity, FLOAT),
		ATTRIBUTE_LOGIC_DEFINE_NEW(pfLastChangeCurseImmunity, FLOAT),
		ATTRIBUTE_LOGIC_DEFINE_NEW(pfBaseSkillAnimationSpeed, FLOAT),
		ATTRIBUTE_LOGIC_DEFINE_NEW(pfLastSkillAnimationSpeed, FLOAT),
		ATTRIBUTE_LOGIC_DEFINE_NEW(pdwBaseWeightMax, DWORD),
		ATTRIBUTE_LOGIC_DEFINE_NEW(pdwLastWeightMax, DWORD),

		ATTRIBUTE_LOGIC_DEFINE_NEW(pfSkillAggroBonus, FLOAT),
		ATTRIBUTE_LOGIC_DEFINE_NEW(pfSkillAggroBonusInPercent, FLOAT),
		ATTRIBUTE_LOGIC_DEFINE_NEW(pfDirectHealPowerBonus, FLOAT),
		ATTRIBUTE_LOGIC_DEFINE_NEW(pfDirectHealPowerBonusInPercent, FLOAT),
		ATTRIBUTE_LOGIC_DEFINE_NEW(pfHotPowerBonus, FLOAT),
		ATTRIBUTE_LOGIC_DEFINE_NEW(pfHotPowerBonusInPercent, FLOAT),
		ATTRIBUTE_LOGIC_DEFINE_NEW(pfDotValueChangePercent, FLOAT),

		ATTRIBUTE_LOGIC_DEFINE_NEW(pfPhysicalCriticalDefenseRate, DWORD),
		ATTRIBUTE_LOGIC_DEFINE_NEW(pfEnergyCriticalDefenseRate, DWORD),
		ATTRIBUTE_LOGIC_DEFINE_NEW(pwGuardRateBase, WORD),
		ATTRIBUTE_LOGIC_DEFINE_NEW(pwGuardRateLast, WORD),

		ATTRIBUTE_LOGIC_DEFINE_NEW(pfSkillDamageBlockModeSuccessRate, FLOAT),
		ATTRIBUTE_LOGIC_DEFINE_NEW(pfCurseBlockModeSuccessRate, FLOAT),
		ATTRIBUTE_LOGIC_DEFINE_NEW(pfKnockdownBlockModeSuccessRate, FLOAT),
		ATTRIBUTE_LOGIC_DEFINE_NEW(pfHtbBlockModeSuccessRate, FLOAT),
		ATTRIBUTE_LOGIC_DEFINE_NEW(pfItemUpgradeBonusRate, FLOAT),
		ATTRIBUTE_LOGIC_DEFINE_NEW(pfItemUpgradeBreakBonusRate, FLOAT),

		ATTRIBUTE_LOGIC_DEFINE_NEW(pbyExpBooster, BYTE),
		ATTRIBUTE_LOGIC_DEFINE_NEW(pbyQuestDropRate, BYTE),

		//ATTRIBUTE_LOGIC_DEFINE_NEW(punkTest, BYTE),
};

#undef ATTRIBUTE_LOGIC_DEFINE_NEW



DWORD CNtlAvatar::CopyValueByTypeNew_BYTE(void* pvValue, void* pvBuffer)
{
	BYTE* pbyBuffer = (BYTE*)pvBuffer;
	*pbyBuffer = *((BYTE*)pvValue);
	return sizeof(pvValue);
}
DWORD CNtlAvatar::CopyValueByTypeNew_WORD(void* pvValue, void* pvBuffer)
{
	WORD* pbyBuffer = (WORD*)pvBuffer;
	pbyBuffer = (WORD*)pvValue;
	return sizeof(WORD*);
}
DWORD CNtlAvatar::CopyValueByTypeNew_float(void* pvValue, void* pvBuffer)
{
	float* pbyBuffer = (float*)pvBuffer;
	pbyBuffer = (float*)pvValue;
	return sizeof(float*);
}

DWORD CNtlAvatar::CopyValueByTypeNew_FLOAT(void* pvValue, void* pvBuffer) {
	FLOAT* pbyBuffer = (FLOAT*)pvBuffer;
	pbyBuffer = (FLOAT*)pvValue;
	return sizeof(FLOAT*);
}

DWORD CNtlAvatar::CopyValueByTypeNew_DWORD(void* pvValue, void* pvBuffer)
{
	DWORD* pbyBuffer = (DWORD*)pvBuffer;
	*pbyBuffer = *((DWORD*)pvValue);
	return sizeof(DWORD);
}
DWORD CNtlAvatar::CopyValueByTypeNew_int(void* pvValue, void* pvBuffer)
{
	int* pbyBuffer = (int*)pvBuffer;
	*pbyBuffer = *((int*)pvValue);
	return sizeof(int);
}

sAVATAR_ATTRIBUTE_LINK CNtlAvatar::ConvertAVATAR_ATTRIBUTE(sAVATAR_ATTRIBUTE* avt)
{
	sAVATAR_ATTRIBUTE_LINK newavt;


	newavt.pwBaseStr = &avt->wBaseStr;
	newavt.pwLastStr = &avt->wLastStr;
	newavt.pwBaseCon = &avt->wBaseCon;
	newavt.pwLastCon = &avt->wLastCon;
	newavt.pwBaseFoc = &avt->wBaseFoc;
	newavt.pwLastFoc = &avt->wLastFoc;
	newavt.pwBaseDex = &avt->wBaseDex;
	newavt.pwLastDex = &avt->wLastDex;
	newavt.pwBaseSol = &avt->wBaseSol;
	newavt.pwLastSol = &avt->wLastSol;
	newavt.pwBaseEng = &avt->wBaseEng;
	newavt.pwLastEng = &avt->wLastEng;

	newavt.pdwBaseMaxLP = &avt->dwBaseMaxLP;
	newavt.pdwLastMaxLP = &avt->dwLastMaxLP;
	newavt.pwBaseMaxEP = &avt->wBaseMaxEP;
	newavt.pwLastMaxEP = &avt->wLastMaxEP;
	newavt.pdwBaseMaxAp = &avt->dwBaseMaxAp;
	newavt.pdwLastMaxAp = &avt->dwLastMaxAp;
	newavt.pwBaseMaxRP = &avt->wBaseMaxRP;
	newavt.pwLastMaxRP = &avt->wLastMaxRP;

	newavt.pwBaseLpRegen = &avt->wBaseLpRegen;
	newavt.pwLastLpRegen = &avt->wLastLpRegen;
	newavt.pwBaseLpSitdownRegen = &avt->wBaseLpSitdownRegen;
	newavt.pwLastLpSitdownRegen = &avt->wLastLpSitdownRegen;
	newavt.pwBaseLpBattleRegen = &avt->wBaseLpBattleRegen;
	newavt.pwLastLpBattleRegen = &avt->wLastLpBattleRegen;

	newavt.pwBaseEpRegen = &avt->wBaseEpRegen;
	newavt.pwLastEpRegen = &avt->wLastEpRegen;
	newavt.pwBaseEpSitdownRegen = &avt->wBaseEpSitdownRegen;
	newavt.pwLastEpSitdownRegen = &avt->wLastEpSitdownRegen;
	newavt.pwBaseEpBattleRegen = &avt->wBaseEpBattleRegen;
	newavt.pwLastEpBattleRegen = &avt->wLastEpBattleRegen;

	newavt.pwBaseApRegen = &avt->wBaseApRegen;
	newavt.pwLastApRegen = &avt->wLastApRegen;
	newavt.pwBaseApSitdownRegen = &avt->wBaseApSitdownRegen;
	newavt.pwLastApSitdownRegen = &avt->wLastApSitdownRegen;
	newavt.pwBaseApBattleRegen = &avt->wBaseApBattleRegen;
	newavt.pwLastApBattleRegen = &avt->wLastApBattleRegen;

	newavt.pwAirAPDegen = &avt->wAirAP_Degen;
	newavt.pwAirAPFlyDegen = &avt->wAirAP_DegenFly;
	newavt.pwAirAPFLyDashDegen = &avt->wAirAP_DegenDash;
	newavt.pwAirAPFlyDashBoostDegen = &avt->wAirAP_DegenAccel;

	newavt.pwBaseRpRegen = &avt->wBaseRpRegen;
	newavt.pwLastRpRegen = &avt->wLastRpRegen;
	newavt.pwLastRpDiminutionRate = &avt->wLastRpDiminutionRate;

	newavt.pwBasePhysicalOffense = &avt->wBasePhysicalOffense;
	newavt.pwLastPhysicalOffense = &avt->wLastPhysicalOffense;
	newavt.pwBasePhysicalDefense = &avt->wBasePhysicalDefense;
	newavt.pwLastPhysicalDefense = &avt->wLastPhysicalDefense;

	newavt.pwBaseEnergyOffense = &avt->wBaseEnergyOffense;
	newavt.pwLastEnergyOffense = &avt->wLastEnergyOffense;
	newavt.pwBaseEnergyDefense = &avt->wBaseEnergyDefense;
	newavt.pwLastEnergyDefense = &avt->wLastEnergyDefense;

	newavt.pwBaseHitRate = &avt->wBaseHitRate;
	newavt.pwLastHitRate = &avt->wLastHitRate;
	newavt.pwBaseDodgeRate = &avt->wBaseDodgeRate;
	newavt.pwLastDodgeRate = &avt->wLastDodgeRate;
	newavt.pwBaseBlockRate = &avt->wBaseBlockRate;
	newavt.pwLastBlockRate = &avt->wLastBlockRate;

	newavt.pwBaseBlockDMG = &avt->wBaseBlockDMG;
	newavt.pwLastBlockDMG = &avt->wLastBlockDMG;

	newavt.pwBaseCurseSuccessRate = &avt->wBaseCurseSuccessRate;
	newavt.pwLastCurseSuccessRate = &avt->wLastCurseSuccessRate;
	newavt.pwBaseCurseToleranceRate = &avt->wBaseCurseToleranceRate;
	newavt.pwLastCurseToleranceRate = &avt->wLastCurseToleranceRate;

	newavt.pwBasePhysicalCriticalRate = &avt->wBasePhysicalCriticalRate;
	newavt.pwLastPhysicalCriticalRate = &avt->wLastPhysicalCriticalRate;
	newavt.pwBaseEnergyCriticalRate = &avt->wBaseEnergyCriticalRate;
	newavt.pwLastEnergyCriticalRate = &avt->wLastEnergyCriticalRate;

	newavt.pfBasePhysicalCritDmgRate = &avt->fBasePhysicalCritDmgRate;
	newavt.pfLastPhysicalCritDmgRate = &avt->fLastPhysicalCritDmgRate;
	newavt.pfBaseEnergyCritDmgRate = &avt->fBaseEnergyCritDmgRate;
	newavt.pfLastEnergyCritDmgRate = &avt->fLastEnergyCritDmgRate;

	newavt.pfBaseRunSpeed = &avt->fBaseRunSpeed;
	newavt.pfLastRunSpeed = &avt->fLastRunSpeed;
	newavt.pfBaseAirSpeed = &avt->fBaseAirSpeed;
	newavt.pfLastAirSpeed = &avt->fLastAirSpeed;
	newavt.pfBaseAirDashSpeed = &avt->fBaseAirDashSpeed;
	newavt.pfLastAirDashSpeed = &avt->fLastAirDashSpeed;
	newavt.pfBaseAirDashAccelSpeed = &avt->fBaseAirDashAccelSpeed;
	newavt.pfLastAirDashAccelSpeed = &avt->fLastAirDashAccelSpeed;

	newavt.pwBaseAttackSpeedRate = &avt->wBaseAttackSpeedRate;
	newavt.pwLastAttackSpeedRate = &avt->wLastAttackSpeedRate;
	newavt.pfBaseAttackRange = &avt->fBaseAttackRange;
	newavt.pfLastAttackRange = &avt->fLastAttackRange;

	newavt.pfCastingTimeChangePercent = &avt->fCastingTimeChangePercent;
	newavt.pfCoolTimeChangePercent = &avt->fCoolTimeChangePercent;
	newavt.pfKeepTimeChangePercent = &avt->fKeepTimeChangePercent;
	newavt.pfDotKeepTimeChangeInSeconds = &avt->fDotKeepTimeChangeInSeconds;
	newavt.pfRequiredEpChangePercent = &avt->fRequiredEpChangePercent;

	newavt.pfHonestOffense = &avt->fHonestOffense;
	newavt.pfHonestDefense = &avt->fHonestDefense;
	newavt.pfStrangeOffense = &avt->fStrangeOffense;
	newavt.pfStrangeDefense = &avt->fStrangeDefense;
	newavt.pfWildOffense = &avt->fWildOffense;
	newavt.pfWildDefense = &avt->fWildDefense;
	newavt.pfEleganceOffense = &avt->fEleganceOffense;
	newavt.pfEleganceDefense = &avt->fEleganceDefense;
	newavt.pfFunnyOffense = &avt->fFunnyOffense;
	newavt.pfFunnyDefense = &avt->fFunnyDefense;

	newavt.pfPhysicalReflect = &avt->fPhysicalReflect;
	newavt.pfEnergyReflect = &avt->fEnergyReflect;

	newavt.pwParalyzeToleranceRate = &avt->wParalyzeToleranceRate;
	newavt.pwTerrorToleranceRate = &avt->wTerrorToleranceRate;
	newavt.pwConfuseToleranceRate = &avt->wConfuseToleranceRate;
	newavt.pwStoneToleranceRate = &avt->wStoneToleranceRate;
	newavt.pwCandyToleranceRate = &avt->wCandyToleranceRate;

	newavt.pfParalyzeKeepTimeDown = &avt->fParalyzeKeepTimeDown;
	newavt.pfTerrorKeepTimeDown = &avt->fTerrorKeepTimeDown;
	newavt.pfConfuseKeepTimeDown = &avt->fConfuseKeepTimeDown;
	newavt.pfStoneKeepTimeDown = &avt->fStoneKeepTimeDown;
	newavt.pfCandyKeepTimeDown = &avt->fCandyKeepTimeDown;
	newavt.pfBleedingKeepTimeDown = &avt->fBleedingKeepTimeDown;
	newavt.pfPoisonKeepTimeDown = &avt->fPoisonKeepTimeDown;
	newavt.pfStomachacheKeepTimeDown = &avt->fStomachacheKeepTimeDown;

	newavt.pfCriticalBlockSuccessRate = &avt->fCriticalBlockSuccessRate;

	newavt.pdwLpRecoveryWhenHit = &avt->dwLpRecoveryWhenHit;
	newavt.pfLpRecoveryWhenHitInPercent = &avt->fLpRecoveryWhenHitInPercent;
	newavt.pdwEpRecoveryWhenHit = &avt->dwEpRecoveryWhenHit;
	newavt.pfEpRecoveryWhenHitInPercent = &avt->fEpRecoveryWhenHitInPercent;

	newavt.pwBaseAbdominalPainDefense = &avt->wBaseAbdominalPainDefense;
	newavt.pwLastAbdominalPainDefense = &avt->wLastAbdominalPainDefense;
	newavt.pwBasePoisonDefense = &avt->wBasePoisonDefense;
	newavt.pwLastPoisonDefense = &avt->wLastPoisonDefense;
	newavt.pwBaseBleedingDefense = &avt->wBaseBleedingDefense;
	newavt.pwLastBleedingDefense = &avt->wLastBleedingDefense;
	newavt.pwBaseBurnDefense = &avt->wBaseBurnDefense;
	newavt.pwLastBurnDefense = &avt->wLastBurnDefense;

	newavt.pfBaseMindCurseImmunity = &avt->fBaseMindCurseImmunity;
	newavt.pfLastMindCurseImmunity = &avt->fLastMindCurseImmunity;
	newavt.pfBaseBodyCurseImmunity = &avt->fBaseBodyCurseImmunity;
	newavt.pfLastBodyCurseImmunity = &avt->fLastBodyCurseImmunity;
	newavt.pfBaseChangeCurseImmunity = &avt->fBaseChangeCurseImmunity;
	newavt.pfLastChangeCurseImmunity = &avt->fLastChangeCurseImmunity;
	newavt.pfBaseSkillAnimationSpeed = &avt->fBaseSkillAnimationSpeed;
	newavt.pfLastSkillAnimationSpeed = &avt->fLastSkillAnimationSpeed;
	newavt.pdwBaseWeightMax = &avt->dwBaseWeightMax;
	newavt.pdwLastWeightMax = &avt->dwLastWeightMax;

	newavt.pfSkillAggroBonus = &avt->fSkillAggroBonus;
	newavt.pfSkillAggroBonusInPercent = &avt->fSkillAggroBonusInPercent;
	newavt.pfDirectHealPowerBonus = &avt->fDirectHealPowerBonus;
	newavt.pfDirectHealPowerBonusInPercent = &avt->fDirectHealPowerBonusInPercent;
	newavt.pfHotPowerBonus = &avt->fHotPowerBonus;
	newavt.pfHotPowerBonusInPercent = &avt->fHotPowerBonusInPercent;
	newavt.pfDotValueChangePercent = &avt->fDotValueChangePercent;

	newavt.pfPhysicalCriticalDefenseRate = &avt->fPhysicalCriticalDefenseRate;
	newavt.pfEnergyCriticalDefenseRate = &avt->fEnergyCriticalDefenseRate;
	newavt.pwGuardRateBase = &avt->wGuardRateBase;
	newavt.pwGuardRateLast = &avt->wGuardRateLast;

	newavt.pfSkillDamageBlockModeSuccessRate = &avt->fSkillDamageBlockModeSuccessRate;
	newavt.pfCurseBlockModeSuccessRate = &avt->fCurseBlockModeSuccessRate;
	newavt.pfKnockdownBlockModeSuccessRate = &avt->fKnockdownBlockModeSuccessRate;
	newavt.pfHtbBlockModeSuccessRate = &avt->fHtbBlockModeSuccessRate;
	newavt.pfItemUpgradeBonusRate = &avt->fItemUpgradeBonusRate;
	newavt.pfItemUpgradeBreakBonusRate = &avt->fItemUpgradeBreakBonusRate;

	newavt.pbyExpBooster = &avt->byExpBooster;
	newavt.pbyQuestDropRate = &avt->byQuestDropRate;

	//newavt.punkTest = &avt->unkTest;

	return newavt;
}


