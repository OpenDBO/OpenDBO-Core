#include "precomp_ntlsimulation.h"
#include "NtlSobAvatarAttr.h"

// shared
#include "PCTable.h"
#include "TableContainer.h"

// core
#include "NtlDebug.h"

// simulation
#include "NtlSLGlobal.h"
#include "NtlSLEvent.h"
#include "NtlSLApi.h"
#include "NtlSLLogicDef.h"

DEFINITION_MEMORY_POOL(CNtlSobAvatarAttr)

CNtlSobAvatarAttr::CNtlSobAvatarAttr() 
{
	m_uiExp = 1;
	m_uiMaxExp = 1;
	m_byBaseStr = 1;		
	m_byLastStr = 1;
	m_byBaseCon = 1;		
	m_byLastCon = 1;		
	m_byBaseFoc = 1;		
	m_byLastFoc = 1;		
	m_byBaseDex = 1;		
	m_byLastDex = 1;		
	m_byBaseSol = 1;		
	m_byLastSol = 1;		
	m_byBaseEng = 1;		
	m_byLastEng = 1;		

	m_wLastRpRegen = 0;
	m_wLastRpDimimutionRate = 0;

	m_wBasePhyshicalOffence = 1;
	m_wLastPhyshicalOffence = 1;
	m_wBasePhyshicalDefence = 1;
	m_wLastPhyshicalDefence = 1;
	
	m_wBaseEnergyOffence = 1;
	m_wLastEnergyOffence = 1;
	m_wBaseEnergyDefence = 1;
	m_wLastEnergyDefence = 1;
	
	m_wBaseAttackRate = 1;
	m_wLastAttackRate = 1;
	m_wBaseDodgeRate = 1;
	m_wLastDodgeRate = 1;
	m_wBaseBlockRate = 1;
	m_wLastBlockRate = 1;

	m_wBasePhysicalCriticalRate = 1;
	m_wLastPhysicalCriticalRate = 1;
	m_wBaseEnergyCriticalRate = 1;
	m_wLastEnergyCriticalRate = 1;

	m_uiZenny = 0;
	m_uiSp = 0;

	m_uiHonorPoint = 0;
	m_uiMudosaPoint = 0;			///< 무도사 포인트

    // PC방 관련
    m_dwNetPy = 0;

}

void CNtlSobAvatarAttr::HandleEvents(RWS::CMsg &pMsg)
{
	if(pMsg.Id == g_EventSobCreate)
	{
		SAvatarInfo *pAvatarInfo	= GetNtlSLGlobal()->GetAvatarInfo();
		CPCTable *pPcTbl			= API_GetTableContainer()->GetPcTable();
		m_pPcTbl					= (sPC_TBLDAT*)pPcTbl->FindData(pAvatarInfo->sCharPf.tblidx);

		SetLevel(pAvatarInfo->sCharPf.byLevel);
		SetLp((RwInt32)pAvatarInfo->sCharPf.wCurLP);
		SetMaxLp((RwInt32)pAvatarInfo->sCharPf.avatarAttribute.wLastMaxLP);
		SetEp((RwInt32)pAvatarInfo->sCharPf.wCurEP);
		SetMaxEp((RwInt32)pAvatarInfo->sCharPf.avatarAttribute.wLastMaxEP);
		SetRp((RwInt32)pAvatarInfo->sCharPf.wCurRP);
		SetMaxRp((RwInt32)pAvatarInfo->sCharPf.avatarAttribute.wLastMaxRP);
		SetMaxRpStock((RwInt32)pAvatarInfo->byMaxRpStock);
		SetRadius(m_pPcTbl->fRadius);

        SetAdult(pAvatarInfo->sCharPf.bIsAdult);
        SetModelName(pAvatarInfo->sCharPf.bIsAdult ? m_pPcTbl->szModel_Adult : m_pPcTbl->szModel_Child);

		SetName(pAvatarInfo->sCharPf.awchName);
		SetNameColor(NTL_PLAYER_NAME_COLOR);
		
		SetAttackRange(m_pPcTbl->fAttack_Range);

		SetRace(m_pPcTbl->byRace);
		SetRaceFlag(m_pPcTbl->dwClass_Bit_Flag);
		SetClass(m_pPcTbl->byClass);
		SetGender(m_pPcTbl->byGender);
		SetFace(pAvatarInfo->sCharPf.sPcShape.byFace);
		SetHair(pAvatarInfo->sCharPf.sPcShape.byHair);
		SetHairColor(pAvatarInfo->sCharPf.sPcShape.byHairColor);
		SetSkinColor(pAvatarInfo->sCharPf.sPcShape.bySkinColor);

		SetReputation(pAvatarInfo->sCharPf.dwReputation);
				
		m_uiExp			= pAvatarInfo->sCharPf.dwCurExp; 
		m_uiMaxExp		= pAvatarInfo->sCharPf.dwMaxExpInThisLevel;
		m_wLastRpRegen	= pAvatarInfo->sCharPf.avatarAttribute.wLastRpRegen;
		m_wLastRpDimimutionRate = pAvatarInfo->sCharPf.avatarAttribute.wLastRpDimimutionRate;
	
		m_byBaseStr = pAvatarInfo->sCharPf.avatarAttribute.byBaseStr; 
		m_byLastStr = pAvatarInfo->sCharPf.avatarAttribute.byLastStr; 
		m_byBaseCon = pAvatarInfo->sCharPf.avatarAttribute.byBaseCon;
		m_byLastCon = pAvatarInfo->sCharPf.avatarAttribute.byLastCon; 
		m_byBaseFoc = pAvatarInfo->sCharPf.avatarAttribute.byBaseFoc; 
		m_byLastFoc = pAvatarInfo->sCharPf.avatarAttribute.byLastFoc;
		m_byBaseSol = pAvatarInfo->sCharPf.avatarAttribute.byBaseSol; 
		m_byLastSol = pAvatarInfo->sCharPf.avatarAttribute.byLastSol;
		m_byBaseEng = pAvatarInfo->sCharPf.avatarAttribute.byBaseEng; 
		m_byLastEng = pAvatarInfo->sCharPf.avatarAttribute.byLastEng;
		m_byBaseDex = pAvatarInfo->sCharPf.avatarAttribute.byBaseDex; 
		m_byLastDex = pAvatarInfo->sCharPf.avatarAttribute.byLastDex;

		m_wBasePhyshicalOffence = pAvatarInfo->sCharPf.avatarAttribute.wBasePhysicalOffence; 
		m_wLastPhyshicalOffence = pAvatarInfo->sCharPf.avatarAttribute.wLastPhysicalOffence; 
		m_wBasePhyshicalDefence = pAvatarInfo->sCharPf.avatarAttribute.wBasePhysicalDefence; 
		m_wLastPhyshicalDefence = pAvatarInfo->sCharPf.avatarAttribute.wLastPhysicalDefence; 
		
		m_wBaseEnergyOffence = pAvatarInfo->sCharPf.avatarAttribute.wBaseEnergyOffence;
		m_wLastEnergyOffence = pAvatarInfo->sCharPf.avatarAttribute.wLastEnergyOffence;
		m_wBaseEnergyDefence = pAvatarInfo->sCharPf.avatarAttribute.wBaseEnergyDefence; 
		m_wLastEnergyDefence = pAvatarInfo->sCharPf.avatarAttribute.wLastEnergyDefence;
		
		m_wBaseAttackRate = pAvatarInfo->sCharPf.avatarAttribute.wBaseAttackRate; 
		m_wLastAttackRate = pAvatarInfo->sCharPf.avatarAttribute.wLastAttackRate; 
		m_wBaseDodgeRate = pAvatarInfo->sCharPf.avatarAttribute.wBaseDodgeRate; 
		m_wLastDodgeRate = pAvatarInfo->sCharPf.avatarAttribute.wLastDodgeRate;
		m_wBaseBlockRate = pAvatarInfo->sCharPf.avatarAttribute.wBaseBlockRate; 
		m_wLastBlockRate = pAvatarInfo->sCharPf.avatarAttribute.wLastBlockRate;
		m_wBaseAttackSpeedRate = pAvatarInfo->sCharPf.avatarAttribute.wBaseAttackSpeedRate;
		m_wLastAttackSpeedRate = pAvatarInfo->sCharPf.avatarAttribute.wLastAttackSpeedRate;

		m_wBaseCurseSuccessRate = pAvatarInfo->sCharPf.avatarAttribute.wBaseCurseSuccessRate;
		m_wLastCurseSuccessRate = pAvatarInfo->sCharPf.avatarAttribute.wLastCurseSuccessRate;
		m_wBaseCurseToleranceRate = pAvatarInfo->sCharPf.avatarAttribute.wBaseCurseToleranceRate;
		m_wLastCurseToleranceRate = pAvatarInfo->sCharPf.avatarAttribute.wLastCurseToleranceRate;

		m_wBasePhysicalCriticalRate = pAvatarInfo->sCharPf.avatarAttribute.wBasePhysicalCriticalRate;
		m_wLastPhysicalCriticalRate = pAvatarInfo->sCharPf.avatarAttribute.wLastPhysicalCriticalRate;
		m_wBaseEnergyCriticalRate = pAvatarInfo->sCharPf.avatarAttribute.wBaseEnergyCriticalRate;
		m_wLastEnergyCriticalRate = pAvatarInfo->sCharPf.avatarAttribute.wLastEnergyCriticalRate;

		SetBaseAttackRange(pAvatarInfo->sCharPf.avatarAttribute.fBaseAttackRange);
		SetAttackRange(pAvatarInfo->sCharPf.avatarAttribute.fLastAttackRange);
		SetCastingTimeModifier(pAvatarInfo->sCharPf.avatarAttribute.fCastingTimeChangePercent);
		SetCoolingTimeModifier(pAvatarInfo->sCharPf.avatarAttribute.fCoolTimeChangePercent);
		SetKeepingTimeModifier(pAvatarInfo->sCharPf.avatarAttribute.fKeepTimeChangePercent);
		SetDOTValueModifier(pAvatarInfo->sCharPf.avatarAttribute.fDotValueChangePercent);
		SetDOTTimeModifier(pAvatarInfo->sCharPf.avatarAttribute.fDotTimeChangeAbsolute);
		SetRequiredEPModifier(pAvatarInfo->sCharPf.avatarAttribute.fRequiredEpChangePercent);

		SetWalkSpeed(pAvatarInfo->sCharPf.avatarAttribute.fLastRunSpeed);
		SetRunSpeed(pAvatarInfo->sCharPf.avatarAttribute.fLastRunSpeed);
		if(m_wLastAttackSpeedRate > 0)
			SetAttackAnimSpeed((RwReal)1000/(RwReal)m_wLastAttackSpeedRate);

        if(IsAdult())
        {
            SetDefaultRunSpeed(m_pPcTbl->fAdult_Run_Speed);            
            SetRunAnimSpeed(m_pPcTbl->fAdult_Run_Speed / m_pPcTbl->fAdult_Run_Speed_Origin);           
        }
        else
        {
            SetDefaultRunSpeed(m_pPcTbl->fChild_Run_Speed);            
            SetRunAnimSpeed(m_pPcTbl->fChild_Run_Speed / m_pPcTbl->fChild_Run_Speed_Origin);
        }


		m_uiZenny = pAvatarInfo->sCharPf.dwZenny;
		m_uiSp = pAvatarInfo->sCharPf.dwSpPoint;
		m_uiMudosaPoint = pAvatarInfo->sCharPf.dwMudosaPoint;

		m_bCanChangeClass = pAvatarInfo->sCharPf.bChangeClass;

        // PC방 관련
        m_dwNetPy   = pAvatarInfo->sCharPf.sLocalize.pcProfileCJKor.netP;

		// Battle Attribute
		m_fHonestOffence = pAvatarInfo->sCharPf.avatarAttribute.fHonestOffence;
		m_fHonestDefence = pAvatarInfo->sCharPf.avatarAttribute.fHonestDefence;
		m_fStrangeOffence = pAvatarInfo->sCharPf.avatarAttribute.fStrangeOffence;
		m_fStrangeDefence = pAvatarInfo->sCharPf.avatarAttribute.fStrangeDefence;
		m_fWildOffence = pAvatarInfo->sCharPf.avatarAttribute.fWildOffence;
		m_fWildDefence = pAvatarInfo->sCharPf.avatarAttribute.fWildDefence;			
		m_fEleganceOffence = pAvatarInfo->sCharPf.avatarAttribute.fEleganceOffence;
		m_fEleganceDefence = pAvatarInfo->sCharPf.avatarAttribute.fEleganceDefence;
		m_fFunnyOffence = pAvatarInfo->sCharPf.avatarAttribute.fFunnyOffence;
		m_fFunnyDefence = pAvatarInfo->sCharPf.avatarAttribute.fFunnyDefence;

		return;
	}

	CNtlSobPlayerAttr::HandleEvents(pMsg);
}

RwUInt32 CNtlSobAvatarAttr::GetAP(void)
{
	return Dbo_CalculatePowerLevel(	m_byLastStr, m_byLastCon, m_byLastFoc, m_byLastDex, m_byLastSol, m_byLastEng,
									m_wLastPhyshicalOffence, m_wLastPhyshicalDefence, m_wLastEnergyOffence, m_wLastEnergyDefence,
									m_wLastAttackRate, m_wLastDodgeRate, m_wLastCurseSuccessRate, m_wLastCurseToleranceRate,
									m_wLastPhysicalCriticalRate, m_wLastEnergyCriticalRate, m_wLastAttackSpeedRate,
									(WORD)GetMaxLp(), (WORD)GetMaxEp(), (WORD)GetLp(), (WORD)GetEp(), 
                                    (BYTE)GetRpStock(), GetLevel(), 0 );
}

