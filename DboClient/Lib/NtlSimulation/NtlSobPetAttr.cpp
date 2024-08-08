#include "precomp_ntlsimulation.h"
#include "NtlSobPetAttr.h"

// shared
#include "NPCTable.h"
#include "TextAllTable.h"
#include "TableContainer.h"

// core
#include "NtlDebug.h"

// simulation
#include "NtlSLEvent.h"
#include "NtlSLGlobal.h"
#include "NtlSLApi.h"
#include "NtlSLLogicDef.h"


DEFINITION_MEMORY_POOL(CNtlSobPetAttr)

CNtlSobPetAttr::CNtlSobPetAttr()
{
	m_bySummonSourceType	= 0;
	m_uiSourceTblId			= 0;
	m_pNpcTbl				= NULL;

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
}

void CNtlSobPetAttr::HandleEvents(RWS::CMsg &pMsg)
{
	if(pMsg.Id == g_EventSobCreate)
	{
		SNtlEventSobPetCreate *pSobPetCreate = (SNtlEventSobPetCreate*)pMsg.pData;

		CNPCTable *pNpcTbl		= API_GetTableContainer()->GetNpcTable();
		CTextTable *pTextTable	= API_GetTableContainer()->GetTextAllTable()->GetNPCTbl();

		if(pSobPetCreate->bOwnerAvatar)
		{
			sSUMMON_PET_PROFILE *pPetProfile = pSobPetCreate->uPetBrief.pPetProfile;
			sAVATAR_ATTRIBUTE *pAttrData = &pPetProfile->attribute;

			m_pNpcTbl = (sNPC_TBLDAT*)pNpcTbl->FindData(pPetProfile->npcTblidx);
			
			SetLevel(m_pNpcTbl->byLevel);
			SetFlyHeight(m_pNpcTbl->fFly_Height);
			SetScale(m_pNpcTbl->fScale);
			SetAttackRange(m_pNpcTbl->fAttack_Range);
			SetRadius(m_pNpcTbl->fRadius);

			SetLp((RwInt32)pPetProfile->wCurLP);
			SetEp(pPetProfile->wCurEP);
			SetMaxLp((RwInt32)pAttrData->wLastMaxLP);
			SetMaxEp((RwInt32)pAttrData->wLastMaxEP);
			SetMaxRp((RwInt32)pAttrData->wLastMaxRP);

			m_byBaseStr = pAttrData->byBaseStr; 
			m_byLastStr = pAttrData->byLastStr; 
			m_byBaseCon = pAttrData->byBaseCon;
			m_byLastCon = pAttrData->byLastCon; 
			m_byBaseFoc = pAttrData->byBaseFoc; 
			m_byLastFoc = pAttrData->byLastFoc;
			m_byBaseSol = pAttrData->byBaseSol; 
			m_byLastSol = pAttrData->byLastSol;
			m_byBaseEng = pAttrData->byBaseEng; 
			m_byLastEng = pAttrData->byLastEng;
			m_byBaseDex = pAttrData->byBaseDex; 
			m_byLastDex = pAttrData->byLastDex;

			m_wBasePhyshicalOffence = pAttrData->wBasePhysicalOffence; 
			m_wLastPhyshicalOffence = pAttrData->wLastPhysicalOffence; 
			m_wBasePhyshicalDefence = pAttrData->wBasePhysicalDefence; 
			m_wLastPhyshicalDefence = pAttrData->wLastPhysicalDefence; 
			
			m_wBaseEnergyOffence = pAttrData->wBaseEnergyOffence;
			m_wLastEnergyOffence = pAttrData->wLastEnergyOffence;
			m_wBaseEnergyDefence = pAttrData->wBaseEnergyDefence; 
			m_wLastEnergyDefence = pAttrData->wLastEnergyDefence;
			
			m_wBaseAttackRate = pAttrData->wBaseAttackRate; 
			m_wLastAttackRate = pAttrData->wLastAttackRate; 
			m_wBaseDodgeRate = pAttrData->wBaseDodgeRate; 
			m_wLastDodgeRate = pAttrData->wLastDodgeRate;
			m_wBaseBlockRate = pAttrData->wBaseBlockRate; 
			m_wLastBlockRate = pAttrData->wLastBlockRate;
			m_wBaseAttackSpeedRate = pAttrData->wBaseAttackSpeedRate;
			m_wLastAttackSpeedRate = pAttrData->wLastAttackSpeedRate;

			m_wBaseCurseSuccessRate = pAttrData->wBaseCurseSuccessRate;
			m_wLastCurseSuccessRate = pAttrData->wLastCurseSuccessRate;
			m_wBaseCurseToleranceRate = pAttrData->wBaseCurseToleranceRate;
			m_wLastCurseToleranceRate = pAttrData->wLastCurseToleranceRate;

			m_wBasePhysicalCriticalRate = pAttrData->wBasePhysicalCriticalRate;
			m_wLastPhysicalCriticalRate = pAttrData->wLastPhysicalCriticalRate;
			m_wBaseEnergyCriticalRate = pAttrData->wBaseEnergyCriticalRate;
			m_wLastEnergyCriticalRate = pAttrData->wLastEnergyCriticalRate;

			SetDefaultWalkSpeed(m_pNpcTbl->fWalk_Speed);
			SetDefaultRunSpeed(m_pNpcTbl->fRun_Speed);
			SetWalkSpeed(pAttrData->fLastRunSpeed);
			SetRunSpeed(pAttrData->fLastRunSpeed);
			
			if(m_wLastAttackSpeedRate > 0)
				SetAttackAnimSpeed((RwReal)1000/(RwReal)m_wLastAttackSpeedRate);

			m_bySummonSourceType	= pPetProfile->bySourceType;
			m_uiSourceTblId			= pPetProfile->sourceTblidx;
		}
		else
		{
			sSUMMON_PET_BRIEF *pPetBrief = pSobPetCreate->uPetBrief.pPetBrief;

			m_pNpcTbl	= (sNPC_TBLDAT*)pNpcTbl->FindData(pPetBrief->npcTblidx);
			SetLevel(m_pNpcTbl->byLevel);
			SetLp((RwInt32)pPetBrief->wCurLP);
			SetMaxLp((RwInt32)pPetBrief->wMaxLP);
			SetEp(pPetBrief->wCurEP);
			SetMaxEp(pPetBrief->wMaxEP);
			SetRadius(m_pNpcTbl->fRadius);

			SetDefaultWalkSpeed(m_pNpcTbl->fWalk_Speed);
			SetDefaultRunSpeed(m_pNpcTbl->fRun_Speed);
			SetRunSpeed(pPetBrief->fLastRunningSpeed);
			if(pPetBrief->wAttackSpeedRate > 0)
				SetAttackAnimSpeed((RwReal)1000/(RwReal)pPetBrief->wAttackSpeedRate);
			SetFlyHeight(m_pNpcTbl->fFly_Height);
			SetScale(m_pNpcTbl->fScale);
			SetAttackRange(m_pNpcTbl->fAttack_Range);

			m_bySummonSourceType	= pPetBrief->bySourceType;
			m_uiSourceTblId			= pPetBrief->sourceTblidx;
		}

		
		SetModelName(m_pNpcTbl->szModel);

		sTEXT_TBLDAT* pTextTblData = reinterpret_cast<sTEXT_TBLDAT*>( pTextTable->FindData( m_pNpcTbl->Name ) );
		if(pTextTblData)
			SetName(pTextTblData->wstrText.c_str());

		SetNameColor(NTL_NPC_NAME_COLOR);
	}
}


sNPC_TBLDAT* CNtlSobPetAttr::GetNpcTbl(void) const
{
	return m_pNpcTbl;
}
