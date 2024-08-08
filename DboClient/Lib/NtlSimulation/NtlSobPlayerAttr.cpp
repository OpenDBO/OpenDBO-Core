#include "precomp_ntlsimulation.h"
#include "NtlSobPlayerAttr.h"

// shared
#include "NtlCharacter.h"
#include "PCTable.h"
#include "TableContainer.h"

// core
#include "NtlDebug.h"

// shared
#include "NtlBattle.h"

// simulation
#include "NtlSLGlobal.h"
#include "NtlSLEvent.h"
#include "NtlSLApi.h"
#include "NtlSLLogicDef.h"

DEFINITION_MEMORY_POOL(CNtlSobPlayerAttr)


CNtlSobPlayerAttr::CNtlSobPlayerAttr() 
{
	m_pPcTbl		= NULL;

	m_ucMainWeaponBattleAttr = BATTLE_ATTRIBUTE_NONE;
	m_ucSubWeaponBattleAttr = BATTLE_ATTRIBUTE_NONE;
	m_ucArmorBattleAttr = BATTLE_ATTRIBUTE_NONE;
}


void CNtlSobPlayerAttr::HandleEvents(RWS::CMsg &pMsg)
{
	if(pMsg.Id == g_EventSobCreate)
	{
		SNtlEventSobPlayerCreate *pSobPlayerCreate = (SNtlEventSobPlayerCreate*)pMsg.pData;
		CPCTable *pPcTbl = API_GetTableContainer()->GetPcTable();

		m_pPcTbl		= (sPC_TBLDAT*)pPcTbl->FindData(pSobPlayerCreate->pPcBrief->tblidx);
		SetLevel(pSobPlayerCreate->pPcBrief->byLevel);
		SetLp((RwUInt32)pSobPlayerCreate->pPcBrief->wCurLP);
		SetMaxLp((RwUInt32)pSobPlayerCreate->pPcBrief->wMaxLP);
		SetEp((RwUInt32)pSobPlayerCreate->pPcBrief->wCurEP);
		SetMaxEp((RwUInt32)pSobPlayerCreate->pPcBrief->wMaxEP);
		SetRadius(m_pPcTbl->fRadius);

        SetAdult(pSobPlayerCreate->pPcBrief->bIsAdult);
        SetModelName(pSobPlayerCreate->pPcBrief->bIsAdult ? m_pPcTbl->szModel_Adult : m_pPcTbl->szModel_Child);

		SetName(pSobPlayerCreate->pPcBrief->awchName);
		SetNameColor(NTL_PLAYER_NAME_COLOR);

		
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
		
        SetRunSpeed(pSobPlayerCreate->pPcBrief->fSpeed);
		if(pSobPlayerCreate->pPcBrief->wAttackSpeedRate > 0)
			SetAttackAnimSpeed((RwReal)1000/(RwReal)pSobPlayerCreate->pPcBrief->wAttackSpeedRate);

		SetAttackRange(m_pPcTbl->fAttack_Range);

		SetRace(m_pPcTbl->byRace);
		SetRaceFlag(m_pPcTbl->dwClass_Bit_Flag);
		SetClass(m_pPcTbl->byClass);
		SetGender(m_pPcTbl->byGender);
		SetFace(pSobPlayerCreate->pPcBrief->sPcShape.byFace);
		SetHair(pSobPlayerCreate->pPcBrief->sPcShape.byHair);
		SetHairColor(pSobPlayerCreate->pPcBrief->sPcShape.byHairColor);
		SetSkinColor(pSobPlayerCreate->pPcBrief->sPcShape.bySkinColor);

		SetMainWeaponAttr( pSobPlayerCreate->pPcBrief->sItemBrief[EQUIP_SLOT_TYPE_HAND].byBattleAttribute );
		SetSubWeaponAttr( pSobPlayerCreate->pPcBrief->sItemBrief[EQUIP_SLOT_TYPE_SUB_WEAPON].byBattleAttribute );
		SetArmorWeaponAttr( pSobPlayerCreate->pPcBrief->sItemBrief[EQUIP_SLOT_TYPE_JACKET].byBattleAttribute );
	}
	else if(pMsg.Id == g_EventSobConvertClass)
	{
		SNtlEventSobConvertClass *pSobConvertClass = (SNtlEventSobConvertClass*)pMsg.pData;
		
		CPCTable *pPcTbl = API_GetTableContainer()->GetPcTable();
		m_pPcTbl = (sPC_TBLDAT*)pPcTbl->GetPcTbldat(GetRace(), pSobConvertClass->byClass, GetGender());

		SetRace(m_pPcTbl->byRace);
		SetRaceFlag(m_pPcTbl->dwClass_Bit_Flag);
		SetClass(m_pPcTbl->byClass);
		SetGender(m_pPcTbl->byGender);
	}
	else if(pMsg.Id == g_EventSobEquipChange)
	{
		SNtlEventSobEquipChange* pSobEquipChange = reinterpret_cast<SNtlEventSobEquipChange*>( pMsg.pData );

		if( pSobEquipChange->bySlotIdx == EQUIP_SLOT_TYPE_HAND )
			SetMainWeaponAttr( pSobEquipChange->byBattleAttribute );
		else if( pSobEquipChange->bySlotIdx == EQUIP_SLOT_TYPE_SUB_WEAPON )
			SetSubWeaponAttr( pSobEquipChange->byBattleAttribute );
		else if( pSobEquipChange->bySlotIdx == EQUIP_SLOT_TYPE_JACKET )
			SetArmorWeaponAttr( pSobEquipChange->byBattleAttribute );
	}
    else if(pMsg.Id == g_EventSobChangeAdult)
    {
        SNtlEventSobChangeAdult* pChangeAdult = (SNtlEventSobChangeAdult*)pMsg.pData;
        CPCTable *pPcTbl = API_GetTableContainer()->GetPcTable();
        m_pPcTbl = (sPC_TBLDAT*)pPcTbl->GetPcTbldat(GetRace(), GetClass(), GetGender());

        SetModelName(pChangeAdult->bAdult ? m_pPcTbl->szModel_Adult : m_pPcTbl->szModel_Child);
        SetAdult(pChangeAdult->bAdult);

        if(pChangeAdult->bAdult)
        {
            SetRunAnimSpeed(m_pPcTbl->fAdult_Run_Speed / m_pPcTbl->fAdult_Run_Speed_Origin);            
        }
        else
        {
            SetRunAnimSpeed(m_pPcTbl->fChild_Run_Speed / m_pPcTbl->fChild_Run_Speed_Origin);            
        }
    }
}
