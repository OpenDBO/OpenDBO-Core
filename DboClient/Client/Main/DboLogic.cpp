#include "precomp_dboclient.h"
#include "DboLogic.h"

// shared
#include "NtlResultCode.h"
#include "ObjectTable.h"
#include "ItemTable.h"
#include "QuestItemTable.h"
#include "ItemOptionTable.h"
#include "MobTable.h"
#include "SkillTable.h"
#include "HTBSetTable.h"
#include "NPCTable.h"
#include "TextAllTable.h"
#include "ItemRecipeTable.h"
#include "TableContainer.h"
#include "NtlBattle.h"
#include "ScriptLinkTable.h"
#include "DojoTable.h"

// core
#include "NtlDebug.h"
#include "NtlMath.h"
#include "NtlCoreUtil.h"

// sound
#include "NtlSoundEventGenerator.h"

// gui
#include "gui_htmlbox_item.h"

// presention
#include "NtlPLGuiManager.h"

// simulation
#include "NtlSLDef.h"
#include "NtlSLLogic.h"
#include "NtlSLGlobal.h"
#include "NtlSLEventFunc.h"
#include "NtlSLPacketGenerator.h"
#include "NtlSobAvatar.h"
#include "NtlSobAvatarAttr.h"
#include "NtlSobManager.h"
#include "NtlSobItemAttr.h"
#include "NtlSobQuestItemAttr.h"
#include "NtlInventory.h"
#include "NtlSobItemIcon.h"
#include "NtlSobSkillIcon.h"
#include "NtlSobSkillAttr.h"
#include "NtlSobNpcAttr.h"
#include "NtlSobPetAttr.h"
#include "NtlSobMonsterAttr.h"
#include "IconMoveManager.h"
#include "NtlSobWorldItemAttr.h"
#include "NtlSLLogic.h"
#include "NtlSLApi.h"
#include "NtlApplication.h"
#include "NtlSobCharProxy.h"
#include "InputHandler.h"
#include "NtlFSMDef.h"

// Simulation Storage
#include "NtlStorageDefine.h"
#include "NtlStorageManager.h"

// dbo
#include "DialogManager.h"
#include "DboGlobal.h"
#include "NPCShop.h"
#include "ChatGui.h"
#include "CharStageState.h"
#include "DboEventGenerator.h"
#include "DisplayStringManager.h"
#include "SubGaugeGui.h"
#include "DboApplication.h"
#include "CursorManager.h"
#include "LobbyManager.h"

#include "CommercialExtendGui.h"

RwReal		g_fElapsedWeightValue	= 1.0f;

RwBool Logic_ItemRepairProc( RwUInt8 ucPlace, RwUInt8 ucSrcSlotIdx )
{
	NTL_FUNCTION( "Logic_ItemRepairProc" );

	CNPCShop*		pNpcShop = reinterpret_cast<CNPCShop*>( GetDialogManager()->GetDialog( DIALOG_NPCSHOP ) );
	SERIAL_HANDLE	hNpcSerial = pNpcShop->GetNPCSerial();

	NTL_RETURN( GetDboGlobal()->GetGamePacketGenerator()->SendItemRepair( hNpcSerial, ucPlace, ucSrcSlotIdx ) );
}

RwBool Logic_ItemIdentificationProc( RwUInt8 byBagIndex, RwUInt8 ucSrcSlotIdx )
{
	NTL_FUNCTION( "Logic_ItemIdentificationProc" );

	CNtlInventory* pInventory = GetNtlSLGlobal()->GetSobAvatar()->GetInventory();
	SERIAL_HANDLE hBag = pInventory->GetBagItem(byBagIndex);
	CNtlSobItem* pBag = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( hBag ) );
	NTL_ASSERT(pBag, "Logic_ItemIdentificationProc, Not exist bag of handle : " << hBag << " of bag index : " << byBagIndex);
	CNtlSobItem* pChildItem = pBag->GetChildItem(ucSrcSlotIdx);
	NTL_ASSERT(pChildItem, "Logic_ItemIdentificationProc, Not exist child item of slot : " << ucSrcSlotIdx << " of bag index : " << byBagIndex);
	CNtlSobItemAttr* pChildItemAttr = reinterpret_cast<CNtlSobItemAttr*>( pChildItem->GetSobAttr() );
	NTL_ASSERT(pChildItemAttr, "Logic_ItemIdentificationProc, Not exist child item attribute of slot : " << ucSrcSlotIdx << " of bag index : " << byBagIndex);

	if( pChildItemAttr->IsNeedToIdentify() )
	{
		eCONTAINER_TYPE eSrcBagType = Logic_ConvertBagIdxToContainerType( byBagIndex );
		NTL_RETURN( GetDboGlobal()->GetGamePacketGenerator()->SendItemIdentification( (RwInt8)eSrcBagType, ucSrcSlotIdx ) );
	}		
	else
	{
		GetAlarmManager()->AlarmMessage( DST_ITEM_IS_IDENTIFIED_ALREADY );
		NTL_RETURN( FALSE );
	}
}

RwBool Logic_ItemIdentification_in_NPCShop_Proc( RwUInt8 byBagIndex, RwUInt8 ucSrcSlotIdx )
{
	NTL_FUNCTION( "Logic_ItemIdentification_in_NPCShop_Proc" );

	CNtlInventory* pInventory = GetNtlSLGlobal()->GetSobAvatar()->GetInventory();
	SERIAL_HANDLE hBag = pInventory->GetBagItem(byBagIndex);
	CNtlSobItem* pBag = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( hBag ) );
	NTL_ASSERT(pBag, "Logic_ItemIdentification_in_NPCShop_Proc, Not exist bag of handle : " << hBag << " of bag index : " << byBagIndex);
	CNtlSobItem* pChildItem = pBag->GetChildItem(ucSrcSlotIdx);
	NTL_ASSERT(pChildItem, "Logic_ItemIdentification_in_NPCShop_Proc, Not exist child item of slot : " << ucSrcSlotIdx << " of bag index : " << byBagIndex);
	CNtlSobItemAttr* pChildItemAttr = reinterpret_cast<CNtlSobItemAttr*>( pChildItem->GetSobAttr() );
	NTL_ASSERT(pChildItemAttr, "Logic_ItemIdentification_in_NPCShop_Proc, Not exist child item attribute of slot : " << ucSrcSlotIdx << " of bag index : " << byBagIndex);

	if( pChildItemAttr->IsNeedToIdentify() )
	{
		eCONTAINER_TYPE eSrcBagType = Logic_ConvertBagIdxToContainerType( byBagIndex );
		NTL_RETURN( GetDboGlobal()->GetGamePacketGenerator()->SendItemIdentifyReq(GetDboGlobal()->GetTargetSerial(), (RwInt8)eSrcBagType, ucSrcSlotIdx) );
	}	
	else
	{
		GetAlarmManager()->AlarmMessage( DST_ITEM_IS_IDENTIFIED_ALREADY );
		NTL_RETURN( FALSE );
	}
}

RwInt32 Logic_GetPCClassFlagFromPCClass( RwUInt8 ucClass )
{
	switch( ucClass )
	{
	case PC_CLASS_HUMAN_FIGHTER: return PC_CLASS_FLAG_HUMAN_FIGHTER; 
	case PC_CLASS_HUMAN_MYSTIC: return PC_CLASS_FLAG_HUMAN_MYSTIC; 
	case PC_CLASS_HUMAN_ENGINEER: return PC_CLASS_FLAG_HUMAN_ENGINEER; 
	case PC_CLASS_NAMEK_FIGHTER: return PC_CLASS_FLAG_NAMEK_FIGHTER; 
	case PC_CLASS_NAMEK_MYSTIC: return PC_CLASS_FLAG_NAMEK_MYSTIC; 
	case PC_CLASS_MIGHTY_MAJIN: return PC_CLASS_FLAG_MIGHTY_MAJIN; 
	case PC_CLASS_WONDER_MAJIN: return PC_CLASS_FLAG_WONDER_MAJIN; 
	case PC_CLASS_STREET_FIGHTER: return PC_CLASS_FLAG_STREET_FIGHTER; 
	case PC_CLASS_SWORD_MASTER: return PC_CLASS_FLAG_SWORD_MASTER; 
	case PC_CLASS_CRANE_ROSHI: return PC_CLASS_FLAG_CRANE_ROSHI; 
	case PC_CLASS_TURTLE_ROSHI: return PC_CLASS_FLAG_TURTLE_ROSHI; 
	case PC_CLASS_GUN_MANIA: return PC_CLASS_FLAG_GUN_MANIA;       
	case PC_CLASS_MECH_MANIA: return PC_CLASS_FLAG_MECH_MANIA;      
	case PC_CLASS_DARK_WARRIOR: return PC_CLASS_FLAG_DARK_WARRIOR;    
	case PC_CLASS_SHADOW_KNIGHT: return PC_CLASS_FLAG_SHADOW_KNIGHT;   
	case PC_CLASS_POCO_SUMMONER: return PC_CLASS_FLAG_POCO_SUMMONER;  
	case PC_CLASS_DENDEN_HEALER: return PC_CLASS_FLAG_DENDEN_HEALER;   
	case PC_CLASS_GRAND_MA: return PC_CLASS_FLAG_GRAND_MA;        
	case PC_CLASS_ULTI_MA: return PC_CLASS_FLAG_ULTI_MA;        
	case PC_CLASS_PLAS_MA: return PC_CLASS_FLAG_PLAS_MA;         
	case PC_CLASS_KAR_MA: return PC_CLASS_FLAG_KAR_MA;          
	}

	// Invalid
	return 0;
}

RwInt32 Logic_GetExplicitPCClassFlagFromPCClass( RwUInt8 ucClass )
{
	return 0x01 << ucClass;
}

eCONTAINER_TYPE Logic_ConvertBagIdxToContainerType( RwUInt8 ucBagIndex )
{
	switch( ucBagIndex )
	{
	case 0: return CONTAINER_TYPE_BAG1;
	case 1:	return CONTAINER_TYPE_BAG2;
	case 2:	return CONTAINER_TYPE_BAG3;
	case 3:	return CONTAINER_TYPE_BAG4;
	case 4:	return CONTAINER_TYPE_BAG5;
	case 5: return CONTAINER_TYPE_NETPYSTORE;
	default: NTL_ASSERT( NULL, "Invalid BagIndex" ); return CONTAINER_TYPE_NONE;
	}
}

RwInt8 Logic_ConvertContainerTypeToBagIdx( RwUInt8 ucContainerType )
{
	switch( ucContainerType )
	{
	case CONTAINER_TYPE_BAG1:	return 0;
	case CONTAINER_TYPE_BAG2:	return 1;
	case CONTAINER_TYPE_BAG3:	return 2;
	case CONTAINER_TYPE_BAG4:	return 3;
	case CONTAINER_TYPE_BAG5:	return 4;
	case CONTAINER_TYPE_NETPYSTORE: return 5;
	default: NTL_ASSERTFAIL("Invalid ContainerType" ); return INVALID_BYTE;
	}
}

eEQUIP_SLOT_TYPE Logic_ConvertEquipSlotIdxToType( RwUInt8 ucEquipSlotIdx )
{
	switch( ucEquipSlotIdx )
	{
	case 0: return EQUIP_SLOT_TYPE_HAND;
	case 1: return EQUIP_SLOT_TYPE_SUB_WEAPON;
	case 2: return EQUIP_SLOT_TYPE_JACKET;
	case 3: return EQUIP_SLOT_TYPE_PANTS;
	case 4: return EQUIP_SLOT_TYPE_BOOTS;
	case 5: return EQUIP_SLOT_TYPE_SCOUTER;
	case 6: return EQUIP_SLOT_TYPE_COSTUME;
	case 7: return EQUIP_SLOT_TYPE_NECKLACE; 
	case 8: return EQUIP_SLOT_TYPE_EARRING_1;
	case 9: return EQUIP_SLOT_TYPE_EARRING_2;
	case 10: return EQUIP_SLOT_TYPE_RING_1;
	case 11: return EQUIP_SLOT_TYPE_RING_2;			
	default: return EQUIP_SLOT_TYPE_UNKNOWN;
	}
};

RwUInt8 Logic_ConvertEquipTypeToSlotIdx( eEQUIP_SLOT_TYPE eEquipType )
{
	return (RwUInt8)( eEquipType - EQUIP_SLOT_TYPE_HAND );
}

RwBool Logic_EquipSlotTypeFlagCheck( RwUInt16 usTypeFlag1, RwUInt16 usTypeFlag2 )
{
	if( ( usTypeFlag1 & EQUIP_SLOT_FLAG_RING_1 || usTypeFlag1 & EQUIP_SLOT_FLAG_RING_2 ) &&
		( usTypeFlag2 & EQUIP_SLOT_FLAG_RING_1 || usTypeFlag2 & EQUIP_SLOT_FLAG_RING_2 ) )
		return TRUE;
	else if( ( usTypeFlag1 & EQUIP_SLOT_FLAG_EARRING_1 || usTypeFlag1 & EQUIP_SLOT_FLAG_EARRING_2 ) &&
		( usTypeFlag2 & EQUIP_SLOT_FLAG_EARRING_1 || usTypeFlag2 & EQUIP_SLOT_FLAG_EARRING_2 ) )
		return TRUE;
	else
		return ( usTypeFlag1 & usTypeFlag2 );
}

RwInt16 Logic_ConvertEquipSlotIdxToFlag( RwUInt8 ucEquipSlotIdx )
{
	eEQUIP_SLOT_TYPE eEquipSlotType = Logic_ConvertEquipSlotIdxToType( ucEquipSlotIdx );

	if( eEquipSlotType == EQUIP_SLOT_TYPE_UNKNOWN )
		return 0x00;
	else
		return (RwInt16)( 0x01 << eEquipSlotType );
}

SERIAL_HANDLE Logic_GetItemHandleFromContainerPos( RwUInt8 ucContainerType, RwUInt8 byPos )
{
	RwUInt8 byBagIdx = Logic_ConvertContainerTypeToBagIdx( ucContainerType );

	CNtlInventory* pInventory = GetNtlSLGlobal()->GetSobAvatar()->GetInventory();
	SERIAL_HANDLE hBag = pInventory->GetBagItem(byBagIdx);

	CNtlSobItem* pBag = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( hBag ) );

	return pBag->GetChildSerial( byPos );

}

RwUInt32 Logic_ConvertClassToClassFlag( RwUInt8 ucClass )
{
	return 0x01 << ucClass;
}

eCONTAINER_TYPE	Logic_ConverWarehouseIdxToContainderType(RwUInt8 byWarehouseIndex)
{
	switch( byWarehouseIndex )
	{
	case 0: return CONTAINER_TYPE_BANK1;
	case 1:	return CONTAINER_TYPE_BANK2;
	case 2:	return CONTAINER_TYPE_BANK3;
	case 3:	return CONTAINER_TYPE_BANK4;
	default: NTL_ASSERT( NULL, "Invalid Warehouse Index" ); return CONTAINER_TYPE_NONE;
	}
}

RwUInt8	Logic_ConverContainerTypeToWarehouseIdx	(eCONTAINER_TYPE eContainerType)
{
	switch( eContainerType )
	{
	case CONTAINER_TYPE_BANK1:	return 0;
	case CONTAINER_TYPE_BANK2:	return 1;
	case CONTAINER_TYPE_BANK3:	return 2;
	case CONTAINER_TYPE_BANK4:	return 3;
	default: NTL_ASSERT( NULL, "Invalid ContainerType" ); return INVALID_BYTE;
	}
}

eCONTAINER_TYPE	Logic_ConverGuildWarehouseIdxToContainderType(RwUInt8 byGuildwarehouseIndex)
{
	switch( byGuildwarehouseIndex )
	{
	case 0: return CONTAINER_TYPE_GUILD1;
	case 1:	return CONTAINER_TYPE_GUILD2;
	case 2:	return CONTAINER_TYPE_GUILD3;
	default: NTL_ASSERT( NULL, "Invalid guild warehouse Index" ); return CONTAINER_TYPE_NONE;
	}
}

RwUInt8	Logic_ConverContainerTypeToGuildWarehouseIdx(eCONTAINER_TYPE eContainerType)
{
	switch( eContainerType )
	{
	case CONTAINER_TYPE_GUILD1:	return 0;
	case CONTAINER_TYPE_GUILD2:	return 1;
	case CONTAINER_TYPE_GUILD3:	return 2;
	default: NTL_ASSERT( NULL, "Invalid ContainerType" ); return INVALID_BYTE;
	}
}

RwBool Logic_CanAcceptOthersAsk()
{
	if( GetDialogManager()->IsOpenDialog(DIALOG_QUESTPROPOSAL) ||
		GetDialogManager()->IsOpenDialog(DIALOG_QUESTREWARD) )
		return FALSE;

	if( GetNtlWorldConcept()->IsEnableAction(E_ACTION_CAN_COMMUNICATION) == FALSE )
		return FALSE;

	return TRUE;
}

RwBool Logic_IsStackMovable( SERIAL_HANDLE hSrcSerial, SERIAL_HANDLE hDestSerial, RwUInt32 nSplitCount )
{
	CNtlSobItem* pSrcItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( hSrcSerial ) );
	NTL_ASSERT( pSrcItem, "Logic_IsStackMove : SrcItem is null" );
	CNtlSobItemAttr* pSrcItemAttr = reinterpret_cast<CNtlSobItemAttr*>( pSrcItem->GetSobAttr() );
	if( pSrcItemAttr->IsNeedToIdentify() )
		return FALSE;

	if( nSplitCount == 0 )
		return FALSE;

	// 1. 목적슬롯이 비어있지 않다면
	if( hDestSerial != INVALID_SERIAL_ID )
	{
		CNtlSobItem* pDestItem= reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( hDestSerial) );
		NTL_ASSERT( pDestItem, "Logic_IsStackMove : DestItem is null" );
		CNtlSobItemAttr* pDestItemAttr = reinterpret_cast<CNtlSobItemAttr*>( pDestItem->GetSobAttr() );

		// 1-2. 미확인 아이템이 아니어야
		if( pDestItemAttr->IsNeedToIdentify() )
			return FALSE;

		// 2. 같은 종류의 아이콘이어야
		if( pSrcItemAttr->GetItemTbl()->tblidx != pDestItemAttr->GetItemTbl()->tblidx )
			return FALSE;

		// 3. 목적지의 아이템스택이 최대값보다 작아야
		if( pDestItemAttr->GetStackNum() >= pDestItemAttr->GetItemTbl()->byMax_Stack )
		{
			// 
			if( nSplitCount < pSrcItemAttr->GetStackNum() )
				return TRUE;	
			else
				return FALSE;
		}

		// 4. 근원지가 최대 아이템 갯수보다 적게 덜어야
		if( nSplitCount >= pSrcItemAttr->GetItemTbl()->byMax_Stack )
			return FALSE;
	}	
	else
	{
		// 1. 가진 만큼을 모두 들었다면 그냥 Move
		if( pSrcItemAttr->GetStackNum() <= nSplitCount )
			return FALSE;
	}

	return TRUE;
}

RwBool Logic_IsStackMovable_to_GuildWarehouse( sGuildWarehouseSlot* pGuildWarehouseItem, SERIAL_HANDLE hSrcSerial, RwUInt32 nSplitCount )
{
	CNtlSobItem* pSrcItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( hSrcSerial ) );
	NTL_ASSERT( pSrcItem, "Logic_IsStackMove : SrcItem is null" );
	CNtlSobItemAttr* pSrcItemAttr = reinterpret_cast<CNtlSobItemAttr*>( pSrcItem->GetSobAttr() );
	if( pSrcItemAttr->IsNeedToIdentify() )
		return FALSE;

	if( nSplitCount == 0 )
		return FALSE;

	NTL_ASSERT(pGuildWarehouseItem, "Logic_IsStackMovable_to_GuildWarehouse, Invalid pointer");
	if(pGuildWarehouseItem->hHandle != INVALID_SERIAL_ID)
	{
		// 0. 목적슬롯이 미확인 아이템이 아니어야
		if( pGuildWarehouseItem->bNeedToIdentify )
			return FALSE;

		// 1. 목적슬롯이 비어있지 않다면 같은 종류의 아이콘이어야
		if( pSrcItemAttr->GetItemTbl()->tblidx != pGuildWarehouseItem->pITEM_TBLDAT->tblidx )
			return FALSE;

		// 2. 목적지의 아이템스택이 최대값보다 작아야
		if( pGuildWarehouseItem->byStackcount >= pGuildWarehouseItem->pITEM_TBLDAT->byMax_Stack )
		{
			if( nSplitCount < pSrcItemAttr->GetStackNum() )
				return TRUE;	
			else
				return FALSE;
		}

		// 3. 근원지가 최대 아이템 갯수보다 적게 덜어야
		if( nSplitCount >= pSrcItemAttr->GetItemTbl()->byMax_Stack )
			return FALSE;
	}
	else
	{
		// 1. 가진 만큼을 모두 들었다면 그냥 Move
		if( pSrcItemAttr->GetStackNum() <= nSplitCount )
			return FALSE;
	}

	return TRUE;
}

RwBool Logic_IsStackMovable_from_GuildWarehouse( sGuildWarehouseSlot* pGuildWarehouseItem, SERIAL_HANDLE hDestSerial, RwUInt32 nSplitCount )
{
	if( pGuildWarehouseItem->bNeedToIdentify )
		return FALSE;

	if( nSplitCount == 0 )
		return FALSE;

	// 1. 목적슬롯이 비어있지 않다면
	if( hDestSerial != INVALID_SERIAL_ID )
	{
		CNtlSobItem* pDestItem= reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( hDestSerial) );
		NTL_ASSERT( pDestItem, "Logic_IsStackMovable_from_GuildWarehouse : DestItem is null" );
		CNtlSobItemAttr* pDestItemAttr = reinterpret_cast<CNtlSobItemAttr*>( pDestItem->GetSobAttr() );

		// 1-2. 목적지 아이콘이 미확인 아이템이 아니어야
		if( pDestItemAttr->IsNeedToIdentify() )
			return FALSE;

		// 2. 같은 종류의 아이콘이어야
		if( pGuildWarehouseItem->pITEM_TBLDAT->tblidx != pDestItemAttr->GetItemTbl()->tblidx )
			return FALSE;

		// 3. 목적지의 아이템스택이 최대값보다 작아야
		if( pDestItemAttr->GetStackNum() >= pDestItemAttr->GetItemTbl()->byMax_Stack )
		{
			// 
			if( nSplitCount < pGuildWarehouseItem->byStackcount )
				return TRUE;	
			else
				return FALSE;
		}

		// 4. 근원지가 최대 아이템 갯수보다 적게 덜어야
		if( nSplitCount >= pGuildWarehouseItem->pITEM_TBLDAT->byMax_Stack )
			return FALSE;
	}	
	else
	{
		// 1. 가진 만큼을 모두 들었다면 그냥 Move
		if( pGuildWarehouseItem->byStackcount <= nSplitCount )
			return FALSE;
	}

	return TRUE;
}

RwBool Logic_IsStackMovable_from_GuildWarehouse( sGuildWarehouseSlot* pGuildWarehouseSrcItem, sGuildWarehouseSlot* pGuildWarehouseDestItem, RwUInt32 nSplitCount )
{
	if( pGuildWarehouseSrcItem->bNeedToIdentify )
		return FALSE;

	if( pGuildWarehouseDestItem->bNeedToIdentify )
		return FALSE;

	if( nSplitCount == 0 )
		return FALSE;

	if( pGuildWarehouseDestItem->hHandle != INVALID_SERIAL_ID )
	{
		// 1. 목적슬롯이 비어있지 않다면 같은 종류의 아이콘이어야
		if( pGuildWarehouseSrcItem->pITEM_TBLDAT->tblidx != pGuildWarehouseDestItem->pITEM_TBLDAT->tblidx )
			return FALSE;

		// 2. 목적지의 아이템스택이 최대값보다 작아야
		if( pGuildWarehouseDestItem->byStackcount >= pGuildWarehouseDestItem->pITEM_TBLDAT->byMax_Stack )
		{
			if( nSplitCount < pGuildWarehouseDestItem->byStackcount )
				return TRUE;	
			else
				return FALSE;
		}

		// 3. 근원지가 최대 아이템 갯수보다 적게 덜어야
		if( nSplitCount >= pGuildWarehouseSrcItem->pITEM_TBLDAT->byMax_Stack )
			return FALSE;
	}
	else
	{
		// 1. 가진 만큼을 모두 들었다면 그냥 Move
		if( pGuildWarehouseSrcItem->byStackcount <= nSplitCount)
			return FALSE;
	}

	return TRUE;
}

RwBool Logic_ItemDeleteProc( sDboItemDeleteInfo* pInfo, RwBool* pPacketLock )
{
	NTL_FUNCTION( "Logic_ItemDeletePRoc" );

	if( pInfo->hSerial == INVALID_SERIAL_ID )
		NTL_RETURN( FALSE );

	if( pInfo->ePlace == PLACE_QUESTBAG )
		NTL_RETURN( GetDboGlobal()->GetGamePacketGenerator()->SendQuestItemDeleteReq((RwUInt8)pInfo->nSlotIdx, pPacketLock ) );

	CNtlSobItem* pItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( pInfo->hSerial ) );
	if( !pItem )
	{
		NTL_WARNING_MESSAGE( "Logic_ItemDeleteProc : Item Is Null ( " << pInfo->hSerial << " )" );
		NTL_RETURN( FALSE );
	}

	CNtlSobItemAttr* pItemAttr = reinterpret_cast<CNtlSobItemAttr*>( pItem->GetSobAttr() );
	if( !pItemAttr )
	{
		DBO_FAIL( "Not exist item attribute of sob item handle : " << pInfo->hSerial);
		NTL_RETURN( FALSE );
	}

	if( !pItemAttr->IsNeedToIdentify() &&
		GetNtlWorldConcept()->IsActivePlayConcept(WORLD_PLAY_DOJO_SCRAMBLE) )
	{
		CNtlSobAvatar*	pAvatar			= GetNtlSLGlobal()->GetSobAvatar();
		CNtlDojo*		pDojo			= pAvatar->GetDojo();
		sSCRAMBLE_INFO*	pSCRAMBLE_INFO	= pDojo->GetScramble();

		sDOJO_TBLDAT* pDOJO_TBLDAT = reinterpret_cast<sDOJO_TBLDAT*>( API_GetTableContainer()->GetDojoTable()->FindData(pSCRAMBLE_INFO->uiScrambleDojoTableIndex) );
		if( !pDOJO_TBLDAT )
		{
			DBO_FAIL("Not exist dojo table of index : " << pSCRAMBLE_INFO->uiScrambleDojoTableIndex);
			NTL_RETURN(FALSE);
		}

		sITEM_TBLDAT* pITEM_TBLDAT = pItemAttr->GetItemTbl();
		if( !pITEM_TBLDAT )
		{
			DBO_FAIL( "Not exist item table of sob item handle : " << pInfo->hSerial);
			NTL_RETURN( FALSE );
		}

		// 도장 쟁탈전 중에는 인장 제어기를 없앨 수 없다
		if( pDOJO_TBLDAT->controllerTblidx == pITEM_TBLDAT->tblidx )
		{
			GetAlarmManager()->AlarmMessage(DST_WORLD_CONCEPT_DOJO_SCRAMBLE);
			NTL_RETURN(FALSE);
		}
	}

	eCONTAINER_TYPE eSrcPlace;

	if( pInfo->ePlace == PLACE_BAG )
	{
		eSrcPlace = Logic_ConvertBagIdxToContainerType( (RwUInt8)pInfo->nPlaceIdx );
	}
	else if( pInfo->ePlace == PLACE_EQUIP )
	{
		eSrcPlace = CONTAINER_TYPE_EQUIP;
		if( !pItem->EmptyChild() )
		{
			if( pItem->IsScouterItem() )
			{
				CNtlSLEventGenerator::SysMsg( INVALID_SERIAL_ID, GAME_SCOUTER_BODY_IS_NOT_EMPTY );
			}
			else
			{
				CNtlSLEventGenerator::SysMsg( INVALID_SERIAL_ID, GAME_ITEM_BAG_IS_NOT_EMPTY );
			}

			NTL_RETURN( FALSE );
		}
	}
	else if( pInfo->ePlace == PLACE_BAGSLOT )
	{
		eSrcPlace = CONTAINER_TYPE_BAGSLOT;
		if( !pItem->EmptyChild() )
		{
			CNtlSLEventGenerator::SysMsg( INVALID_SERIAL_ID, GAME_ITEM_BAG_IS_NOT_EMPTY );
			NTL_RETURN( FALSE );
		}	
	}
	else if( pInfo->ePlace == PLACE_SCOUTER )
	{
		eSrcPlace = CONTAINER_TYPE_SCOUT;
		if( !pItem->EmptyChild() )
		{
			NTL_ASSERTE("Scouter part can't have child item!");
			NTL_RETURN( FALSE );
		}
	}
	else if( pInfo->ePlace == PLACE_SCOUTER_SLOT )
	{
		eSrcPlace = CONTAINER_TYPE_EQUIP;
		if( !pItem->EmptyChild() )
		{
			CNtlSLEventGenerator::SysMsg( INVALID_SERIAL_ID, GAME_SCOUTER_BODY_IS_NOT_EMPTY );
			NTL_RETURN( FALSE );
		}
	}
	else if( pInfo->ePlace == PLACE_WAREHOUSE )
	{
		eSrcPlace = Logic_ConverWarehouseIdxToContainderType((RwUInt8) pInfo->nPlaceIdx );

		if( !pItem->EmptyChild() )
		{
			CNtlSLEventGenerator::SysMsg( INVALID_SERIAL_ID, GAME_ITEM_BAG_IS_NOT_EMPTY );
			NTL_RETURN( FALSE );
		}

		NTL_RETURN( GetDboGlobal()->GetGamePacketGenerator()->SendBankDeleteItem( (RwUInt8)eSrcPlace, (RwUInt8)pItem->GetItemSlotIdx(), pPacketLock ) );
	}
	else
		NTL_RETURN( FALSE );

	if( pInfo->bRemoteSell )
		NTL_RETURN( GetDboGlobal()->GetGamePacketGenerator()->SendRemoteSellReq( (RwUInt8)eSrcPlace, (RwUInt8)pItem->GetItemSlotIdx(), pPacketLock ) );

	NTL_RETURN( GetDboGlobal()->GetGamePacketGenerator()->SendItemDeleteReq( (RwUInt8)eSrcPlace, (RwUInt8)pItem->GetItemSlotIdx(), pPacketLock ) );
}

RwBool Logic_ItemDirectEquipProc( SERIAL_HANDLE hSrcSerial, EPlace eSrcPlace, RwUInt8 ucSrcSlotIdx )
{
	CNtlSobItem* pItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( hSrcSerial ) );
	CNtlSobItemAttr* pItemAttr = reinterpret_cast<CNtlSobItemAttr*>( pItem->GetSobAttr() );

	if( pItem->IsBagItem() )
	{
		CNtlInventory* pInventory = GetNtlSLGlobal()->GetSobAvatar()->GetInventory();

		for( RwUInt8 i = 1 ; i < NTL_MAX_BAGSLOT_COUNT ; ++i )
		{
			if( pInventory->GetBagItem( i ) == INVALID_SERIAL_ID )
			{
				if( !Logic_IsNetStore( i ) )
					return Logic_ItemMoveProc( hSrcSerial, eSrcPlace, ucSrcSlotIdx, PLACE_BAGSLOT, INVALID_SERIAL_ID, i, 0 );
			}
		}

		GetAlarmManager()->AlarmMessage( DST_ITEM_CAN_NOT_MOVE_TO_BAGSLOT_FULLBAGSLOT );
	}
	else
	{
		if( pItem->IsScouterPartItem() )
		{
			CNtlInventory* pInventory = GetNtlSLGlobal()->GetSobAvatar()->GetInventory();
			RwUInt32 uiScouterSerial = pInventory->GetEquipItem(EQUIP_SLOT_TYPE_SCOUTER);

			// 스카우터가 장착이 된 상태
			if( uiScouterSerial != INVALID_SERIAL_ID )
			{
				// 스카우터 파츠
				RwUInt8 byEmptySlot = Logic_GetFirstEmptyScouterSlot();
				if( byEmptySlot != INVALID_BYTE )
				{
					return Logic_ItemMoveProc( hSrcSerial, eSrcPlace, ucSrcSlotIdx, PLACE_SCOUTER, uiScouterSerial, byEmptySlot, 1 );
				}
				else
				{
					// 스카우터에 파츠가 가득 찼습니다
					GetAlarmManager()->AlarmMessage(DST_SCOUTER_FULL);
				}
			}			
			else
			{
				// 스카우터가 필요합니다.
				GetAlarmManager()->AlarmMessage(DST_SCOUTER_NEED_SCOUTER);
			}
		}
		else if( pItem->IsEquipItem() )
		{	
			// 스카우터의 경우는 한가지 더 체크 peessi: 할 필요가 없어졌음. 내부로직에서 처리한다. 
			//if( pItem->IsScouterItem() )
			//{
			//	// 스카우터를 장착하려고 할 때 이미 스카우터가 장착되어 있는 것이 비어있지 않다면
			//	CNtlInventory* pInventory = GetNtlSLGlobal()->GetSobAvatar()->GetInventory();
			//	RwUInt32 uiScouterSerial = pInventory->GetEquipItem(EQUIP_SLOT_TYPE_SCOUTER);

			//	if( INVALID_SERIAL_ID != uiScouterSerial )
			//	{
			//		CNtlSobItem* pScouterItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( uiScouterSerial ) );
			//		if( pScouterItem->ExistChild() )
			//		{
			//			GetAlarmManager()->AlarmMessage(DST_SCOUTER_MUST_CLEAR_SCOUTER);
			//			return FALSE;
			//		}
			//	}				
			//}

			// 일반 장착 아이템
			for( RwUInt8 i = 0 ; i < NTL_MAX_EQUIP_ITEM_SLOT ; ++i )
			{
				RwUInt16 wSlotFlag = Logic_ConvertEquipSlotIdxToFlag( i );

				if( Logic_EquipSlotTypeFlagCheck( wSlotFlag, pItemAttr->GetItemTbl()->wEquip_Slot_Type_Bit_Flag ) )
				{
					if( i == EQUIP_SLOT_TYPE_EARRING_1 || i == EQUIP_SLOT_TYPE_RING_1 )
					{
						CNtlInventory* pInventory = GetNtlSLGlobal()->GetSobAvatar()->GetInventory();
						if( pInventory->GetEquipItem( i ) != INVALID_SERIAL_ID )
							continue;
					}

					return Logic_ItemMoveProc( hSrcSerial, eSrcPlace, ucSrcSlotIdx, PLACE_EQUIP, INVALID_SERIAL_ID, i, 0 );
				}
			}

			GetAlarmManager()->AlarmMessage( DST_ITEM_CAN_NOT_MOVE_NOT_EQUIAL_EQUIPTYPE );
		}		
	}

	return FALSE;
}

RwBool Logic_UseProc( SERIAL_HANDLE hSrcSerial, RwUInt32 param1 /* = 0xFF*/ )
{
	if( INVALID_SERIAL_ID == hSrcSerial )
		return FALSE;

	if(!GetNtlWorldConcept()->IsEnableAction(E_ACTION_CAN_COMMUNICATION))
	{
		Logic_WarnningWorldConceptActionMessage( WORLD_CONCEPT_SECOND_GRADE );
		return FALSE;
	}

	if( Logic_IsVehicleDriver( Logic_GetAvatarActor() ) )
	{
		GetAlarmManager()->AlarmMessage(GAME_COMMON_CANT_DO_THAT_IN_RIDEON_STATE, TRUE);
		return FALSE;
	}


	SERIAL_HANDLE hTarget = GetDboGlobal()->GetTargetSerial();

	CNtlSob* pSobObject = GetNtlSobManager()->GetSobObject( hSrcSerial );
	if( !pSobObject )
	{
		DBO_FAIL("Not exist sob object of handle : " << hSrcSerial);
		return FALSE;
	}

	CNtlSobIcon* pIcon = reinterpret_cast<CNtlSobIcon*>( pSobObject->GetIcon() );

	if( pIcon->IsUsePossible() )
	{
		return pIcon->Use( hTarget, param1 );		
	}
    else
    {
        CNtlSLEventGenerator::SysMsg(pIcon->GetOwnerID(), pIcon->GetErrorMsg());
    }

	return FALSE;
}

/**
* \brief Rp Bonus Skill 사용
* \param hSrcSerial	
* \retrun Up 에서 사용처리 여부 (TRUE : Up을 무시, FALSE : Up을 처리)
*/
RwBool Logic_UseProcRpBonusSkill( SERIAL_HANDLE hSrcSerial ) 
{
	SERIAL_HANDLE hTarget = GetDboGlobal()->GetTargetSerial();

	CNtlSob* pSobObject = GetNtlSobManager()->GetSobObject( hSrcSerial );
	RwUInt32 uiClassID = pSobObject->GetClassID();
	CNtlSobSkillIcon* pSkillIcon = reinterpret_cast<CNtlSobSkillIcon*>( pSobObject->GetIcon() );
	CNtlSobSkillAttr* pSkillAttr = (CNtlSobSkillAttr*)pSobObject->GetSobAttr();

	// uiClassID가 Skill일 경우에만 TRUE를 넣을 수 있다.
	if( uiClassID == SLCLASS_SKILL )
	{
		// 죽은 경우라면 Rp Bonus Skill을 사용하지 못한다.
		if( GetNtlSLGlobal()->GetSobAvatar()->IsDie() )
			return TRUE;

		// 자동 모드이면 이미 선택되어 있는 값을 보내줌.
		// 수동 모드라면 선택하는 UI를 띄우는 이벤트를 보낸다.
		if( pSkillAttr->m_bRPBonusAutoMode )
		{
			if( pSkillAttr->m_byRPBonusType != DBO_RP_BONUS_TYPE_INVALID )
			{
				if( pSkillIcon->Use( hTarget, pSkillAttr->m_byRPBonusType ) )
					CDboEventGenerator::RpBonusAuto( pSkillAttr->m_byRPBonusType );
			}
			else
			{
				return FALSE;
			}
		}
		else
		{
			CDboEventGenerator::RpBonusSelect( reinterpret_cast<void*>(pSkillIcon) );
		}

		return TRUE;
	}

	return FALSE;
}


RwBool Logic_ItemBagClearProc( SERIAL_HANDLE hSrcSerial, RwUInt8 ucSrcSlotIdx )
{
	return TRUE;
}

RwBool Logic_IsBagClearable( SERIAL_HANDLE hSrcSerial, RwUInt8 ucSrcSlotIdx )
{
	return TRUE;
}

RwInt32 Logic_ItemGetGUI_EXTEND_MODEByCommonPointType(RwUInt8 byCommonPointType)
{
	// item의 모드 분류 Common_Point_Type
	CCommercialExtendGui::GUI_EXTEND_MODE eMode = CCommercialExtendGui::ZENNY_EXTEND;
	switch( byCommonPointType )
	{
	case 1:	/// Netpy
		eMode = CCommercialExtendGui::NETPY_EXTEND;
		break;
	case 2:	/// Cash
		eMode = CCommercialExtendGui::CASH_EXTEND;
		break;
		//default:/// Zenny
		//	break;
	}
	
	return	(RwInt32) eMode;
}

VOID Logic_ItemDropConfirmProc(RwBool bRemoteSell /* = FALSE */)
{
	if( !Logic_CanMouseInput_in_Tutorial( ETL_MOUSE_INPUT_TYPE_CPASULE_KIT_WND_TRASH ) )		
	{
		GetAlarmManager()->AlarmMessage( DST_ITEM_CAN_NOT_DISCARD_NOT_STATE );
		return;
	}

	if( !GetIconMoveManager()->IsActive() )
	{
		GetAlarmManager()->AlarmMessage( DST_ITEM_CAN_NOT_DISCARD_NOT_PICKUP );
		return;
	}

	if( GetIconMoveManager()->GetSrcPlace() == PLACE_GUILD_WAREHOUSE )
	{
		GetAlarmManager()->AlarmMessage( DST_GUILD_WAREHOUSE_CAN_NOT_DISCARD );
		return;
	}

	if( GetIconMoveManager()->GetSrcPlace() != PLACE_BAG &&
		GetIconMoveManager()->GetSrcPlace() != PLACE_EQUIP &&
		GetIconMoveManager()->GetSrcPlace() != PLACE_QUESTBAG &&
		GetIconMoveManager()->GetSrcPlace() != PLACE_BAGSLOT &&
		GetIconMoveManager()->GetSrcPlace() != PLACE_WAREHOUSE &&
		GetIconMoveManager()->GetSrcPlace() != PLACE_SCOUTER &&
		GetIconMoveManager()->GetSrcPlace() != PLACE_SCOUTER_SLOT )
	{
		GetAlarmManager()->AlarmMessage( DST_ITEM_CAN_NOT_MOVE_THERE );
		return;
	}


	CNtlSob* pPickedObj = GetNtlSobManager()->GetSobObject( GetIconMoveManager()->GetSrcSerial() );
	NTL_ASSERT( pPickedObj, "Logic_ItemDropConfirmProc : Invalid PickupIcon Serial" );
	if( !pPickedObj )
	{
		GetAlarmManager()->AlarmMessage( DST_ITEM_CAN_NOT_DISCARD_NOT_PICKUP );
		return;
	}

	if( pPickedObj->GetClassID() == SLCLASS_SLOT_ITEM ||
		pPickedObj->GetClassID() == SLCLASS_SLOT_QUESTITEM )
	{
		RwInt32 nPlaceSlotIndex = -1;

		if( GetIconMoveManager()->GetSrcPlace() == PLACE_BAG || GetIconMoveManager()->GetSrcPlace() == PLACE_WAREHOUSE )
		{
			CNtlSobItem* pPickedItemObj = reinterpret_cast<CNtlSobItem*>( pPickedObj );
			nPlaceSlotIndex = pPickedItemObj->GetParentItemSlotIdx();
		}		

		sMsgBoxData data;
		data.sItemDeleteInfo.hSerial		= GetIconMoveManager()->GetSrcSerial();
		data.sItemDeleteInfo.ePlace			= (EPlace)GetIconMoveManager()->GetSrcPlace();
		data.sItemDeleteInfo.nPlaceIdx		= nPlaceSlotIndex;
		data.sItemDeleteInfo.nSlotIdx		= GetIconMoveManager()->GetSrcSlotIdx();
		data.sItemDeleteInfo.bRemoteSell	= bRemoteSell;

		CDboEventGenerator::EnableItemIcon( FALSE, data.sItemDeleteInfo.ePlace, data.sItemDeleteInfo.nSlotIdx, data.sItemDeleteInfo.nPlaceIdx );

		if( pPickedObj->GetClassID() == SLCLASS_SLOT_ITEM )
		{
			CNtlSobItemAttr* pItemAttr = reinterpret_cast<CNtlSobItemAttr*>( pPickedObj->GetSobAttr() );
			CTextTable* pItemTextTable = API_GetTableContainer()->GetTextAllTable()->GetItemTbl();

			if( pItemAttr->IsNeedToIdentify() )
			{
				if( bRemoteSell )
				{
					GetAlarmManager()->FormattedAlarmMessage( DST_ITEM_REMOTE_SELL, FALSE, &data, GetDisplayStringManager()->GetString( DST_MAILSYSTEM_NOT_IDENTIFICATION_ITEM ) );
				}
				else
				{
					GetAlarmManager()->FormattedAlarmMessage( DST_ITEM_CONFIRM_DROP, FALSE, &data, GetDisplayStringManager()->GetString( DST_MAILSYSTEM_NOT_IDENTIFICATION_ITEM ) );
				}
			}
			else
			{
				if( pItemAttr->IsExpired() )
				{
					GetAlarmManager()->FormattedAlarmMessage( DST_COMMERCIAL_MB_DELETE_TERM_LIMETED_ITEM_MSG, FALSE, &data, pItemTextTable->GetText( pItemAttr->GetItemTbl()->Name ).c_str() );
				}
				else
				{
					if( bRemoteSell )
					{
						if( pItemAttr->GetItemTbl()->byMax_Stack > 1 )
							GetAlarmManager()->FormattedAlarmMessage( DST_STACKITEM_REMOTE_SELL, FALSE, &data, pItemTextTable->GetText( pItemAttr->GetItemTbl()->Name ).c_str(), pItemAttr->GetStackNum() );
						else
							GetAlarmManager()->FormattedAlarmMessage( DST_ITEM_REMOTE_SELL, FALSE, &data, pItemTextTable->GetText( pItemAttr->GetItemTbl()->Name ).c_str() );
					}
					else
					{
						if( pItemAttr->GetItemTbl()->byMax_Stack > 1 )
							GetAlarmManager()->FormattedAlarmMessage( DST_STACKITEM_CONFIRM_DROP, FALSE, &data, pItemTextTable->GetText( pItemAttr->GetItemTbl()->Name ).c_str(), pItemAttr->GetStackNum() );
						else
							GetAlarmManager()->FormattedAlarmMessage( DST_ITEM_CONFIRM_DROP, FALSE, &data, pItemTextTable->GetText( pItemAttr->GetItemTbl()->Name ).c_str() );
					}

				}
			}			
		}
		else
		{
			CNtlSobQuestItemAttr* pQuestItemAttr = reinterpret_cast<CNtlSobQuestItemAttr*>( pPickedObj->GetSobAttr() );
			CTextTable* pQuestItemTextTable = API_GetTableContainer()->GetTextAllTable()->GetQuestItemTbl();

			GetAlarmManager()->FormattedAlarmMessage( DST_STACKITEM_CONFIRM_DROP, FALSE, &data, pQuestItemTextTable->GetText( pQuestItemAttr->GetQuestItemTbl()->ItemName ).c_str(), pQuestItemAttr->GetStackNum() );			
		}

		GetIconMoveManager()->IconMoveEnd();	
	}
	else
	{
		GetAlarmManager()->AlarmMessage( DST_ITEM_CAN_NOT_DISCARD_NOT_PICKUP );
		return;
	}
}

RwBool Logic_IsMovableEquipedScouter()
{
	if( GetDialogManager()->IsOpenDialog(DIALOG_SCOUTER_BACKGROUND) )
		return FALSE;

	CNtlInventory* pInventory = GetNtlSLGlobal()->GetSobAvatar()->GetInventory();
	SERIAL_HANDLE hHandle = pInventory->GetEquipItem(EQUIP_SLOT_TYPE_SCOUTER);

	if( hHandle != INVALID_SERIAL_ID )
	{
		CNtlSobItem* pSobScouter = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( hHandle ) );

		if( pSobScouter->ExistChild() )
			return FALSE;
	}

	return TRUE;
}

RwBool Logic_IsMovableEquipedScouterParts()
{
	if( GetDialogManager()->IsOpenDialog(DIALOG_SCOUTER_BACKGROUND) )
		return FALSE;

	return TRUE;
}

RwBool Logic_IsUsingScouter(VOID)
{
	if( GetDialogManager()->IsOpenDialog(DIALOG_SCOUTER_BACKGROUND) )
		return TRUE;

	return FALSE;
}

RwBool Logic_IsScouterHasParts(VOID)
{
	CNtlInventory* pInventory = GetNtlSLGlobal()->GetSobAvatar()->GetInventory();
	SERIAL_HANDLE hHandle = pInventory->GetEquipItem( EQUIP_SLOT_TYPE_SCOUTER );

	if( hHandle != INVALID_SERIAL_ID )
	{
		CNtlSobItem* pSobScouter = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( hHandle ) );

		if( pSobScouter->ExistChild() )
			return TRUE;
	}

	return FALSE;
}

RwBool Logic_FindEmptyItemSlot( RwInt32* pBagIndex, RwInt32* pSlotIndex )
{
	CNtlInventory* pInventory = GetNtlSLGlobal()->GetSobAvatar()->GetInventory();

	for( RwInt32 i = 0 ; i < NTL_MAX_BAGSLOT_COUNT ; ++i )
	{
		if( pInventory->GetBagItem( i ) == INVALID_SERIAL_ID )
			continue;

		if( i == BAGSLOT_POSITION_BAGSLOT_POSITION_NETPYSTORE &&
			!GetNtlSLGlobal()->IsEnableUseNetpyStore() )
			continue;

		CNtlSobItem* pBag = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( pInventory->GetBagItem( i ) ) );

		for( RwInt32 j = 0 ; j < pBag->GetChildNum() ; ++j )
		{
			if( pBag->GetChildSerial( j ) == INVALID_SERIAL_ID )
			{
				*pBagIndex = i;
				*pSlotIndex = j;
				return TRUE;
			}
		}
	}

	return FALSE;
}

RwBool Logic_FindEmptyItemSlot()
{
	CNtlInventory* pInventory = GetNtlSLGlobal()->GetSobAvatar()->GetInventory();

	for( RwInt32 i = 0 ; i < NTL_MAX_BAGSLOT_COUNT ; ++i )
	{
		if( pInventory->GetBagItem( i ) == INVALID_SERIAL_ID )
			continue;

		if( i == BAGSLOT_POSITION_BAGSLOT_POSITION_NETPYSTORE &&
			!GetNtlSLGlobal()->IsEnableUseNetpyStore() )
			continue;

		CNtlSobItem* pBag = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( pInventory->GetBagItem( i ) ) );

		for( RwInt32 j = 0 ; j < pBag->GetChildNum() ; ++j )
		{
			if( pBag->GetChildSerial( j ) == INVALID_SERIAL_ID )
			{
				return TRUE;
			}
		}
	}

	return FALSE;
}

RwBool Logic_WorldItemLooting( CNtlSob* pWorldItem )
{
	if( !Logic_CanKeybaordInput_in_Tutorial( ETL_KEYBOARD_INPUT_TYPE_LOOTING ) )
		return FALSE;

	if( pWorldItem )
	{
		CNtlSobWorldItemAttr* pAttr = reinterpret_cast<CNtlSobWorldItemAttr*>( pWorldItem->GetSobAttr() );

		if( pAttr->IsItem() )
		{
			GetDboGlobal()->GetGamePacketGenerator()->SendPickUpWorldItemReq( pWorldItem->GetSerialID() );
		}
		else
		{
			GetDboGlobal()->GetGamePacketGenerator()->SendPickUpWorldZennyReq( pWorldItem->GetSerialID() );
		}

		return TRUE;
	}

	return FALSE;
}

RwInt32 Logic_WhichBagHasItem( SERIAL_HANDLE hItemSerial )
{
	CNtlSobItem* pItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( hItemSerial ) );
	CNtlInventory* pInventory = GetNtlSLGlobal()->GetSobAvatar()->GetInventory();

	return Logic_ConvertBagIdxToContainerType( (RwUInt8)pInventory->FindBagSlot( pItem->GetParentItemSerial() ) );
}

CNtlSobItem* Logic_FindInventoryItemByTableId( RwUInt32 tblidx )
{
	CNtlInventory* pInventory = GetNtlSLGlobal()->GetSobAvatar()->GetInventory();

	for( RwInt32 i = 0 ; i < NTL_MAX_BAGSLOT_COUNT ; ++i )
	{
		SERIAL_HANDLE hBagSerial = pInventory->GetBagItem( i );

		if( hBagSerial != INVALID_SERIAL_ID )
		{
			CNtlSobItem* pBagItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( hBagSerial ) );
			CNtlSobItemAttr* pBagItemAttr = reinterpret_cast<CNtlSobItemAttr*>( pBagItem->GetSobAttr() );
			if( tblidx == pBagItemAttr->GetTblIdx() )
				return pBagItem;

			for( RwInt32 j = 0 ; j < pBagItem->GetChildNum() ; ++j )
			{
				CNtlSobItem* pItem = pBagItem->GetChildItem( j );

				if( pItem )
				{
					CNtlSobItemAttr* pItemAttr = reinterpret_cast<CNtlSobItemAttr*>( pItem->GetSobAttr() );

					if( tblidx == pItemAttr->GetTblIdx() )
						return pItem;
				}
			}
		}		
	}

	return NULL;
}

RwUInt32 Logic_FindInventoryItemCountByTableId( RwUInt32 tblidx )
{
	CNtlInventory* pInventory = GetNtlSLGlobal()->GetSobAvatar()->GetInventory();

	RwUInt32 nCount = 0;

	for( RwInt32 i = 0 ; i < NTL_MAX_BAGSLOT_COUNT ; ++i )
	{
		SERIAL_HANDLE hBagSerial = pInventory->GetBagItem( i );

		if( hBagSerial != INVALID_SERIAL_ID )
		{
			CNtlSobItem* pBagItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( hBagSerial ) );
			CNtlSobItemAttr* pBagItemAttr = reinterpret_cast<CNtlSobItemAttr*>( pBagItem->GetSobAttr() );
			if( tblidx == pBagItemAttr->GetTblIdx() )
			{
				nCount += pBagItemAttr->GetStackNum();
			}

			for( RwInt32 j = 0 ; j < pBagItem->GetChildNum() ; ++j )
			{
				CNtlSobItem* pItem = pBagItem->GetChildItem( j );

				if( pItem )
				{
					CNtlSobItemAttr* pItemAttr = reinterpret_cast<CNtlSobItemAttr*>( pItem->GetSobAttr() );

					if( tblidx == pItemAttr->GetTblIdx() )
					{
						nCount += pItemAttr->GetStackNum();
					}
				}
			}
		}		
	}

	return nCount;
}

CNtlSobItem* Logic_FindEquipItemByTableId( RwUInt32 tblidx )
{
	CNtlInventory* pInventory = GetNtlSLGlobal()->GetSobAvatar()->GetInventory();

	for( RwInt32 i = 0 ; i < NTL_MAX_EQUIP_ITEM_SLOT; ++i )
	{
		SERIAL_HANDLE hEquipItemSerial = pInventory->GetEquipItem( i );

		if( hEquipItemSerial != INVALID_SERIAL_ID )
		{
			CNtlSobItem* pEquipItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( hEquipItemSerial ) );
			CNtlSobItemAttr* pEquipItemAttr = reinterpret_cast<CNtlSobItemAttr*>( pEquipItem->GetSobAttr() );
			if( tblidx == pEquipItemAttr->GetTblIdx() )
				return pEquipItem;

			if( i == EQUIP_SLOT_TYPE_SCOUTER )
			{
				for( RwInt32 j = 0 ; j < pEquipItem->GetChildNum() ; ++j )
				{
					CNtlSobItem* pItem = pEquipItem->GetChildItem( j );

					if( pItem )
					{
						CNtlSobItemAttr* pItemAttr = reinterpret_cast<CNtlSobItemAttr*>( pItem->GetSobAttr() );

						if( tblidx == pItemAttr->GetTblIdx() )
							return pItem;
					}
				}
			}
		}				
	}

	return NULL;
}

CNtlSobItem* Logic_FindInventoryItemMinDurByDurationGroup(RwUInt32 uiDurationGroup)
{
	CNtlInventory* pInventory = GetNtlSLGlobal()->GetSobAvatar()->GetInventory();

	CNtlSobItem* pRetNtlSobItem = NULL;
	RwUInt32 uiRemaintime = 0xFFFFFFFF;

	/// Bag 검색
	for( RwInt32 i = 0 ; i < NTL_MAX_BAGSLOT_COUNT ; ++i )
	{
		SERIAL_HANDLE hBagSerial = pInventory->GetBagItem( i );

		if( hBagSerial != INVALID_SERIAL_ID )
		{
			CNtlSobItem* pBagItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( hBagSerial ) );

			CNtlSobItemAttr* pBagItemAttr = reinterpret_cast<CNtlSobItemAttr*>( pBagItem->GetSobAttr() );
			sITEM_TBLDAT* pITEM_TBLDAT = pBagItemAttr->GetItemTbl();

			if( uiDurationGroup == pITEM_TBLDAT->dwDurationGroup && uiRemaintime > pBagItemAttr->GetRemainTime() )
			{
				pRetNtlSobItem = pBagItem;
				uiRemaintime = (RwUInt32)pBagItemAttr->GetRemainTime();
			}

			for( RwInt32 j = 0 ; j < pBagItem->GetChildNum() ; ++j )
			{
				CNtlSobItem* pItem = pBagItem->GetChildItem( j );

				if( pItem )
				{
					CNtlSobItemAttr* pItemAttr = reinterpret_cast<CNtlSobItemAttr*>( pItem->GetSobAttr() );
					sITEM_TBLDAT* pITEM_TBLDAT = pItemAttr->GetItemTbl();

					if( uiDurationGroup == pITEM_TBLDAT->dwDurationGroup && uiRemaintime > pItemAttr->GetRemainTime() )
					{
						pRetNtlSobItem = pItem;
						uiRemaintime = (RwUInt32)pItemAttr->GetRemainTime();
					}
				}
			}
		}		
	}

	/// Eqip 검색 (scouter 포함)
	for( RwInt32 i = 0 ; i < NTL_MAX_EQUIP_ITEM_SLOT; ++i )
	{
		SERIAL_HANDLE hEquipItemSerial = pInventory->GetEquipItem( i );

		if( hEquipItemSerial != INVALID_SERIAL_ID )
		{
			CNtlSobItem* pEquipItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( hEquipItemSerial ) );
			CNtlSobItemAttr* pEquipItemAttr = reinterpret_cast<CNtlSobItemAttr*>( pEquipItem->GetSobAttr() );
			sITEM_TBLDAT* pITEM_TBLDAT = pEquipItemAttr->GetItemTbl();

			if( uiDurationGroup == pITEM_TBLDAT->dwDurationGroup && uiRemaintime > pEquipItemAttr->GetRemainTime() )
			{
				pRetNtlSobItem = pEquipItem;
				uiRemaintime = (RwUInt32)pEquipItemAttr->GetRemainTime();
			}

			if( i == EQUIP_SLOT_TYPE_SCOUTER )
			{
				for( RwInt32 j = 0 ; j < pEquipItem->GetChildNum() ; ++j )
				{
					CNtlSobItem* pItem = pEquipItem->GetChildItem( j );

					if( pItem )
					{
						CNtlSobItemAttr* pItemAttr = reinterpret_cast<CNtlSobItemAttr*>( pItem->GetSobAttr() );
						sITEM_TBLDAT* pITEM_TBLDAT = pEquipItemAttr->GetItemTbl();

						if( uiDurationGroup == pITEM_TBLDAT->dwDurationGroup && uiRemaintime > pItemAttr->GetRemainTime() )
						{
							pRetNtlSobItem = pItem;
							uiRemaintime = (RwUInt32)pItemAttr->GetRemainTime();
						}
					}
				}
			}
		}				
	}

	return pRetNtlSobItem;
}

VOID Logic_CaculDayHourMinSecond( RwUInt32 uiInSecond, CRetCaculDayHourMinSecond* pOut)
{
	pOut->uiSec = uiInSecond;
	pOut->uiMin = uiInSecond / 60;		/// s -> m
	pOut->uiSec -= pOut->uiMin * 60;
	pOut->uiHour = pOut->uiMin / 60;	/// m -> h					/// m -> h
	pOut->uiMin -= pOut->uiHour * 60;
	pOut->uiDay = pOut->uiHour / 24;	/// h -> d	
	pOut->uiHour -= pOut->uiDay * 24;
}

RwBool Logic_IsMailRemainTimeOverDay( DBOTIME tmTime )
{
	static WCHAR awcBuffer[256] = L"";

	DBOTIME serverTime = GetNtlSLGlobal()->GetServerTime();

	tm tmServerTime;

	errno_t errid = _localtime64_s( &tmServerTime, (__time64_t*)&serverTime );
	if( 0 != errid )
	{
		DBO_FAIL( "Logic_IsMailRemainTimeOverDay - "<< serverTime );
		return FALSE;
	}

	DBOTIME remainTime = tmTime - serverTime;
	if( remainTime <= 0 )
	{
		swprintf_s( awcBuffer, 256, GetDisplayStringManager()->GetString( DST_BUDOKAI_TIME_MINUTE ), 0. );
		return FALSE;
	}; 

	tm tmRemainTime;

	errid = _localtime64_s( &tmRemainTime, (__time64_t*)&remainTime );
	if( 0 != errid )
	{
		DBO_FAIL( "Logic_IsMailRemainTimeOverDay - "<< remainTime );
		return FALSE;
	}

	if( tmRemainTime.tm_yday > 0 )
	{
		return TRUE;
	}

	return FALSE;
}

VOID Logic_EnableIcon( RwBool bEnable, RwUInt8 byPlace, RwUInt8 uiSlotIdx )
{
	if( byPlace >= CONTAINER_TYPE_BAG_FIRST && byPlace <= CONTAINER_TYPE_BAG_LAST )
	{
		CDboEventGenerator::EnableItemIcon( bEnable, PLACE_BAG, uiSlotIdx, byPlace - CONTAINER_TYPE_BAG1 );		
	}	
	else if( byPlace == CONTAINER_TYPE_EQUIP )
	{// ScouterSlot은 Equip에 포함.
		CDboEventGenerator::EnableItemIcon( bEnable, PLACE_EQUIP, uiSlotIdx );
	}
	else if( byPlace == CONTAINER_TYPE_BAGSLOT )
	{
		CDboEventGenerator::EnableItemIcon( bEnable, PLACE_BAGSLOT, uiSlotIdx );
	}
	else if( byPlace >= CONTAINER_TYPE_BANK1 && byPlace <= CONTAINER_TYPE_BANK4 )
	{
		CDboEventGenerator::EnableItemIcon( bEnable, PLACE_WAREHOUSE, uiSlotIdx, byPlace - CONTAINER_TYPE_BANK1 );
	}
	else if( byPlace == CONTAINER_TYPE_SCOUT )
	{
		CDboEventGenerator::EnableItemIcon( bEnable, PLACE_SCOUTER, uiSlotIdx );
	}

	// QuestIcon은 다른 패킷에서 이벤트 호출.
}

RwBool Logic_IsUpdateType(RwInt32 iType, void* pData)
{
	// NtlSLEvent.h파일안의 EEventAvatarInfoUpdateType형의 타입만 검사

	SNtlEventSobInfoUpdate* pPacket = reinterpret_cast<SNtlEventSobInfoUpdate*>( pData );

	if( pPacket->uiUpdateType & iType )
	{
		return TRUE;
	}

	return FALSE;
}

gui::CTexture* Logic_CreateTexture( RwChar* pFilename, eTEXTURETYPE eTexType /*= TEXTYPE_ICON*/, RwChar* pcPathName /* = NULL */)
{
	std::string buf;

	switch( eTexType )
	{
	case TEXTYPE_ICON:		buf += "icon\\";	; break;
	case TEXTYPE_ILLUST:	buf += "illust\\";	; break;		
	case TEXTYPE_MAP:		buf += pcPathName;	; break;
	case TEXTYPE_LANDMARK:	buf += "landmark\\"	; break;
	case TEXTTYPE_WARFOG:	buf += "warfog\\"	; break;
	case TEXTTYPE_EMBLEM:	buf += "emblem\\"	; break;
	}

	buf += pFilename;
	gui::CTexture* pImage = NULL;
	pImage = g_TextureMgr.CreateTexture( buf.c_str() );

	if(!pImage)
	{
		if( eTexType == TEXTYPE_ICON )
			pImage = g_TextureMgr.CreateTexture( "icon\\DEF_NTL_ICO.png" );
	}

	return pImage;
}

VOID Logic_DeleteTexture( gui::CTexture* pTexture )
{
	if( pTexture )
	{
		g_TextureMgr.DeleteTexture( pTexture );		
	}
}

std::string Logic_GetSmallIconName(const char* pcIconName)
{
	if( !pcIconName )
		return NULL;

	char acBuffer[128] = "";
	sprintf_s(acBuffer, 128, "%s", pcIconName);

	char* pChangeString = strstr(acBuffer, ".");
	strncpy(pChangeString, "_S.PNG", 6);

	return acBuffer;
}

/**
* \brief PC의 Class Icon Surface 얻기
* 외곽선이 있는 아이콘은 가로/세로 25px 의 크기이며, 외곽선이 없는 아이콘은
* 가로/세로 19px의 크기이다.
* \param byClass		NtlCharacter.h 에 선언되어 있는 PC 클래스 상수
* \param bOutLine		외곽선의 유/무 ( 디폴트 : TRUE )
* \return 찾아낸 서페이스
*/
gui::CSurface& Logic_GetPCClassIconSurface(RwUInt8 byClass, RwBool bOutLine /* TRUE */ )
{
	static gui::CSurface surface;

	std::string strSrfName;

	switch(byClass)
	{
	case PC_CLASS_HUMAN_FIGHTER:	strSrfName = "srfHuman_Fighter";	break;  //(무도가)
	case PC_CLASS_HUMAN_MYSTIC:		strSrfName = "srfHuman_Mystic";		break;	//(기공사)
	case PC_CLASS_HUMAN_ENGINEER:	strSrfName = "srfHuman_Engineer";	break;	//(엔지니어)
	case PC_CLASS_NAMEK_FIGHTER:	strSrfName = "srfNamek_Fighter";	break;	//(전사)
	case PC_CLASS_NAMEK_MYSTIC:		strSrfName = "srfNamek_Mystic";		break;	//(용족)
	case PC_CLASS_MIGHTY_MAJIN:		strSrfName = "srfMighty_Majin";		break;	//(대마인)
	case PC_CLASS_WONDER_MAJIN:		strSrfName = "srfWonder_Majin";		break;	//(의마인)
	case PC_CLASS_STREET_FIGHTER:	strSrfName = "srfStreet_Fighter";	break;	//(격투가)
	case PC_CLASS_SWORD_MASTER:		strSrfName = "srfSword_Master";		break;	//(검술가)
	case PC_CLASS_CRANE_ROSHI:		strSrfName = "srfCrane_Roshi";		break;	//(학선사)
	case PC_CLASS_TURTLE_ROSHI:		strSrfName = "srfTurtle_Roshi";		break;	//(거북선사)
	case PC_CLASS_GUN_MANIA:		strSrfName = "srfGun_Mania";		break;	//(건매니아)
	case PC_CLASS_MECH_MANIA:		strSrfName = "srfMech_Mania";		break;	//(메카매니아)
	case PC_CLASS_DARK_WARRIOR:		strSrfName = "srfDark_Warrior";		break;	//(마계전사)
	case PC_CLASS_SHADOW_KNIGHT:	strSrfName = "srfShadow_Knight";	break;	//(마도전사)
	case PC_CLASS_DENDEN_HEALER:	strSrfName = "srfDenden_Healer";	break;	//(덴덴도사)
	case PC_CLASS_POCO_SUMMONER:	strSrfName = "srfPoco_Summoner";	break;	//(포코도사)
	case PC_CLASS_GRAND_MA:			strSrfName = "srfGrand_Ma";			break;	//(그랜마)
	case PC_CLASS_ULTI_MA:			strSrfName = "srfUlti_Ma";			break;	//(얼티마)
	case PC_CLASS_PLAS_MA:			strSrfName = "srfPlas_Ma";			break;	//(플라즈마)
	case PC_CLASS_KAR_MA:			strSrfName = "srfKar_Ma";			break;	//(카르마)
	default:
		NTL_ASSERT( false, "Not exist class" );
	}

	// OutLine 이 있는 아이콘이라면 _Out이라는 단어를 붙여준다.
	if( bOutLine )
		strSrfName += "_Out";

	// 서페이스를 가져와서 리턴
	surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", strSrfName );

	return surface;
}

/**
* \brief NPC의 Class Icon Surface 얻기
* 외곽선이 있는 아이콘은 가로/세로 25px 의 크기이며, 외곽선이 없는 아이콘은
* 가로/세로 14px의 크기이다.
* \param byClass		NtlCharacter.h 에 선언되어 있는 NPC 클래스 상수
* \param bOutLine		외곽선의 유/무 ( 디폴트 : TRUE )
* \return 찾아낸 서페이스
*/
gui::CSurface& Logic_GetNPCClassIconSurface(RwUInt8 byClass, RwBool bOutLine /* TRUE */ )
{
	static gui::CSurface surface;

	std::string strSrfName;

	switch(byClass)
	{
	case NPC_JOB_WEAPON_MERCHANT:			strSrfName = "srfWeapon_Merchant";	break;	// 무기상인
	case NPC_JOB_ARMOR_MERCHANT:			strSrfName = "srfArmor_Merchant";	break;	// 의복상인
	case NPC_JOB_GOODS_MERCHANT:			strSrfName = "srfGoods_Merchant";	break;	// 잡화상인
	case NPC_JOB_SCOUTER_MERCHANT:			strSrfName = "srfScouter_Merchant";	break;	// 스카우터상인
	case NPC_JOB_GUARD:						strSrfName = "srfGuard";			break;	// 경비
	case NPC_JOB_SKILL_TRAINER_HFI:			// 무도가 교관
	case NPC_JOB_SKILL_TRAINER_HMY:			// 기공사 교관
	case NPC_JOB_SKILL_TRAINER_HEN:			// 엔지니어 교관
	case NPC_JOB_SKILL_TRAINER_NFI:			// 전사 교관
	case NPC_JOB_SKILL_TRAINER_NMY:			// 용족 교관
	case NPC_JOB_SKILL_TRAINER_MMI:			// 대마인 교관
	case NPC_JOB_SKILL_TRAINER_MWO:			strSrfName = "srfSkill_Trainer";	break;	// 의마인 교관
	case NPC_JOB_BANKER:					strSrfName = "srfBanker";			break;	// 창고지기
	case NPC_JOB_TALKER:					strSrfName = "srfTalker";			break;	// 이야기꾼
	case NPC_JOB_GUILD_MANAGER:				strSrfName = "srfGuild_Manager";	break;	// 길드매니저
	case NPC_JOB_SUMMON_PET:				strSrfName = "srfSummon_Pet";		break;	// 소환수
	case NPC_JOB_DOGI_MERCHANT:				strSrfName = "srfDogi_Merchant";	break;	// 도복 상인
	case NPC_JOB_SPECIAL_WEAPON_MERCHANT:	strSrfName = "srfSpecial_Weapon_Merchant";	break;	// 고급 무기 상인
	case NPC_JOB_SPECIAL_ARMOR_MERCHANT:	strSrfName = "srfSpecial_Armor_Merchant";	break;	// 고급 의복 상인
	case NPC_JOB_SPECIAL_GOODS_MERCHANT:	strSrfName = "srfSpecial_Goods_Merchant";	break;	// 고급 잡화 상인
	case NPC_JOB_SPECIAL_FOODS_MERCHANT:	strSrfName = "srfSpecial_Foods_Merchant";	break;	// 고급 식료품 상인
	case NPC_JOB_SPECIAL_SCOUTER_MERCHANT:	strSrfName = "srfSpecial_Scouter_Merchant";	break;	// 고급 스카우터 상인
	case NPC_JOB_GRAND_SKILL_TRAINER_HFI:	// 무도가 그랜드 스킬마스터
	case NPC_JOB_GRAND_SKILL_TRAINER_HMY:	// 기공사 그랜드 스킬마스터
	case NPC_JOB_GRAND_SKILL_TRAINER_HEN:	// 엔지니어 그랜드 스킬마스터
	case NPC_JOB_GRAND_SKILL_TRAINER_NFI:	// 전사 그랜드 스킬마스터
	case NPC_JOB_GRAND_SKILL_TRAINER_NMY:	// 용족 그랜드 스킬마스터
	case NPC_JOB_GRAND_SKILL_TRAINER_MMI:	// 대마인 그랜드 스킬마스터
	case NPC_JOB_GRAND_SKILL_TRAINER_MWO:	strSrfName = "srfGrand_Skill_Trainer";		break;	// 의마인 그랜드 스킬마스터
	case NPC_JOB_SUB_WEAPON_MERCHANT:		strSrfName = "srfSub_Weapon_Merchant";		break;// 보조 무기 상인
	case NPC_JOB_GATE_KEEPER:				strSrfName = "srfGate_Keeper";				break;	// 문지기
	case NPC_JOB_VENDING_MACHINE:			strSrfName = "srfVending_Machine";			break;	// 자판기
	case NPC_JOB_TIMEMACHINE_MERCHANT:		strSrfName = "srfTimeMachine_Merchant";		break;	// 타임머신진입 NPC
	case NPC_JOB_PORTAL_MAN:				strSrfName = "srfPortal_Man";				break;// 순간 이동 서비스맨		
	case NPC_JOB_BUS:						strSrfName = "srfBus";						break;// 버스
	case NPC_JOB_RECEPTION:					strSrfName = "srfReception";				break;// 천하제일 무도회 접수인
	case NPC_JOB_BUDOHSI_MERCHANT:			strSrfName = "srfBudo_Merchant";			break;// 무도사 상인
	case NPC_JOB_REFEREE:					strSrfName = "srfRefree";					break;// 심판
	case NPC_JOB_GAMBLE_MERCHANT:			strSrfName = "srfGamble_Merchant";			break;// 뽑기 상인
	case NPC_JOB_CHAMPION_MERCHANT:			strSrfName = "srfChampion_Merchant";		break;// 챔피언 상인
	case NPC_JOB_DOJO_MANAGER:				strSrfName = "srfDojoNPC";					break;// 도장 관리인
	case NPC_JOB_DOJO_MERCHANT:				strSrfName = "srfDojoNPC";					break;// 도장 상인
	case NPC_JOB_DOJO_SEAL:					strSrfName = "srfDojoNPC";					break;// 도장 인장
	case NPC_JOB_DOJO_BANKER:				strSrfName = "srfDojoNPC";					break;// 도장 창고
	case NPC_JOB_MIX_MASTER:				strSrfName = "srfHoipoiNPC";				break;// 호이포이 믹스 마스터
	default:
		NTL_ASSERT( false, "Not exist class" );
	}

	// OutLine 이 있는 아이콘이라면 _Out이라는 단어를 붙여준다.
	if( bOutLine )
		strSrfName += "_Out";

	// 서페이스를 가져와서 리턴
	surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", strSrfName );

	return surface;
}

gui::CSurface& Logic_GetSmallRPTypeMark( RwInt32 eRpType )
{
	static gui::CSurface surface;

	switch( eRpType )
	{
	case DBO_RP_BONUS_TYPE_KNOCKDOWN :
		surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "SkillMasterWnd.srf", "srfRPKnockDown" );		
		break;
	case DBO_RP_BONUS_TYPE_RESULT_PLUS:
		surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "SkillMasterWnd.srf", "srfRPPower" );		
		break;
	case DBO_RP_BONUS_TYPE_KEEP_TIME_PLUS:
		surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "SkillMasterWnd.srf", "srfRPKeepTime" );		
		break;
	case DBO_RP_BONUS_TYPE_CASTING_TIME_MINUS:
		surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "SkillMasterWnd.srf", "srfRPCastTime" );		
		break;
	case DBO_RP_BONUS_TYPE_COOL_TIME_MINUS:
		surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "SkillMasterWnd.srf", "srfRPCoolTime" );		
		break;
	case DBO_RP_BONUS_TYPE_EP_MINUS:
		surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "SkillMasterWnd.srf", "srfRPEP" );		
		break;
	case DBO_RP_BONUS_TYPE_GUARD_CRASH:
		surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "SkillMasterWnd.srf", "srfGuardCrush" );		
		break;
	default:
		surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "SkillMasterWnd.srf", "srfRPNoUse" );		
		break;
	}

	return surface;
}

gui::CSurface& Logic_GetBattleAttributeIconSurface( RwUInt8 byAttribute, RwBool bOutLine /* = TRUE  */ )
{
	static gui::CSurface surface;

	std::string filename;

	switch( byAttribute )
	{
	case BATTLE_ATTRIBUTE_NONE:			filename = "srfNone"; break;
	case BATTLE_ATTRIBUTE_HONEST:		filename = "srfHonest"; break; 
	case BATTLE_ATTRIBUTE_STRANGE:		filename = "srfStrange"; break;		
	case BATTLE_ATTRIBUTE_WILD:			filename = "srfWild"; break;
	case BATTLE_ATTRIBUTE_ELEGANCE:		filename = "srfElegance"; break;		
	case BATTLE_ATTRIBUTE_FUNNY:		filename = "srfFunny"; break;
	default : break;/* return surface;*/
	}

	if( !bOutLine )
		filename += "Small";

	surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", filename );

	return surface;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Passive skill 사용 가능 유무 검사
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

RwBool Logic_IsPassiveDashUsePossible(void)
{
	CNtlSobAvatar *pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();
	CNtlAvatarSkillContainer *pSkillContainer = reinterpret_cast<CNtlAvatarSkillContainer*>( pSobAvatar->GetSkillContainer() );

	if(Logic_IsDirectMode(pSobAvatar))
		return FALSE;

	// 거대화 변신상태, 순수 마인 상태에서는 대시 안됨    
	if(Logic_IsTransformGreatNamek(pSobAvatar) || 
		Logic_IsTransformSpinAttack(pSobAvatar) ||
		pSobAvatar->GetAspectState()->sAspectStateBase.byAspectStateId == ASPECTSTATE_PURE_MAJIN)
		return FALSE;


	RwUInt32 uiCurrLp = Logic_GetLp(pSobAvatar);
	RwUInt32 uiCurrEp = Logic_GetEp(pSobAvatar);
	RwUInt32 uiCurrRp = Logic_GetRp(pSobAvatar);

	RwUInt16 wReqLp, wReqEp, wReqRp;

	RwBool bCanDash = pSkillContainer->CanDash(wReqLp, wReqEp, wReqRp);

	if(bCanDash)
	{
		if(wReqLp > uiCurrLp)
		{
			CNtlSLEventGenerator::SysMsg(INVALID_SERIAL_ID, GAME_SKILL_NOT_ENOUGH_LP);
			return FALSE;
		}

		if(wReqEp > uiCurrEp)
		{
			CNtlSLEventGenerator::SysMsg(INVALID_SERIAL_ID, GAME_SKILL_NOT_ENOUGH_EP);
			return FALSE;
		}

		if(uiCurrRp > uiCurrRp)
		{
			CNtlSLEventGenerator::SysMsg(INVALID_SERIAL_ID, GAME_SKILL_NOT_ENOUGH_RP_BALL);
			return FALSE;
		}
	}

	return bCanDash;
}

RwBool Logic_IsPassiveChargingUsePossible(void)
{
	CNtlSobAvatar *pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();
	CNtlAvatarSkillContainer *pSkillContainer = reinterpret_cast<CNtlAvatarSkillContainer*>( pSobAvatar->GetSkillContainer() );
	return pSkillContainer->CanCharging();
}

RwBool Logic_IsPassiveBlockingUsePossible(void)
{
	CNtlSobAvatar *pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();
	CNtlAvatarSkillContainer *pSkillContainer = reinterpret_cast<CNtlAvatarSkillContainer*>( pSobAvatar->GetSkillContainer() );
	return pSkillContainer->CanBlocking();	
}

RwBool Logic_IsPassiveBlockingUsePossibleCoolTime(void)
{
	return (CSubGaugeGui::m_eGuardGaugeState == CSubGaugeGui::STATE_NONE ) ? TRUE : FALSE;	
}

RwBool IsV2LessLength(const RwV2d* vPos, const RwV2d* vPos2, RwReal fLength)
{
	RwV2d v2Sub;
	RwV2dSub(&v2Sub, vPos, vPos2);

	RwReal fSquaredLength = CNtlMath::MathRwV2dSquaredLength(&v2Sub);

	if( (fLength*fLength) > fSquaredLength )
		return TRUE;

	return FALSE;
}

RwBool IsV2LessorEqualLength(const RwV2d* vPos, const RwV2d* vPos2, RwReal fLength)
{
	RwV2d v2Sub;
	RwV2dSub(&v2Sub, vPos, vPos2);

	RwReal fSquaredLength = CNtlMath::MathRwV2dSquaredLength(&v2Sub);

	if( (fLength*fLength) >= fSquaredLength )
		return TRUE;

	return FALSE;
}

RwBool IsV2MoreLength(const RwV2d* vPos, const RwV2d* vPos2, RwReal fLength)
{
	RwV2d v2Sub;
	RwV2dSub(&v2Sub, vPos, vPos2);

	RwReal fSquaredLength = CNtlMath::MathRwV2dSquaredLength(&v2Sub);

	if( (fLength*fLength) < fSquaredLength )
		return TRUE;

	return FALSE;
}

RwBool IsV2MoreorEqualLength(const RwV2d* vPos, const RwV2d* vPos2, RwReal fLength)
{
	RwV2d v2Sub;
	RwV2dSub(&v2Sub, vPos, vPos2);

	RwReal fSquaredLength = CNtlMath::MathRwV2dSquaredLength(&v2Sub);

	if( (fLength*fLength) <= fSquaredLength )
		return TRUE;

	return FALSE;
}

RwBool IsV2EqualLength(const RwV2d* vPos, const RwV2d* vPos2, RwReal fLength)
{
	RwV2d v2Sub;
	RwV2dSub(&v2Sub, vPos, vPos2);

	RwReal fSquaredLength = CNtlMath::MathRwV2dSquaredLength(&v2Sub);

	if( (fLength*fLength) == fSquaredLength )
		return TRUE;

	return FALSE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	user option load/save
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


const WCHAR* Logic_GetActiveServerName(void)
{
	SERVER_HANDLE hServer = GetLobbyManager()->GetSelectedServerHandle();
	if( INVALID_SERVERFARMID == hServer )
		return L"DBO_SERVER";

	CLobby* pLobby = GetLobbyManager()->GetLobby(hServer);
	if( !pLobby )
	{
		DBO_FAIL("Not exist lobby infomation of server handle : " << hServer);
		return L"DBO_SERVER";
	}

	return pLobby->GetServerName();
}

RwBool Logic_MakeLoadGameOptFileName(std::string& strFileName, const WCHAR *pCharName)
{
	RwChar chBuffer[128];
	SUserData *pUserData = GetDboGlobal()->GetUserData();

	// user
	std::string str = USEROPT_SERIALIZE_FOLDERNAME;

	// 계정
	::WideCharToMultiByte(GetACP(), 0, pUserData->wchUserID, -1, chBuffer, 128, NULL, NULL);
	str += "\\";
	str += chBuffer;

	// server
	std::wstring wstr = Logic_GetActiveServerName();
	WideCharToMultiByte(GetACP(), 0, wstr.c_str(), -1, chBuffer, 128, NULL, NULL);
	str += "\\";
	str += chBuffer;

	// character name
	::WideCharToMultiByte(GetACP(), 0, pCharName, -1, chBuffer, 128, NULL, NULL);
	str += "\\";
	str += chBuffer;

	WIN32_FIND_DATA finddata;
	HANDLE hFile = ::FindFirstFile(str.c_str(), &finddata);

	if(hFile == INVALID_HANDLE_VALUE || finddata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
	{
		::FindClose( hFile );
		return FALSE;
	}

	::FindClose( hFile );

	strFileName = str;

	return TRUE;
}


RwBool Logic_MakeSaveGameOptFileName(std::string& strFileName, const WCHAR *pCharName)
{
	RwChar chBuffer[128];	
	SUserData *pUserData = GetDboGlobal()->GetUserData();

	HANDLE hFile;
	WIN32_FIND_DATA finddata;

	// user
	std::string str = USEROPT_SERIALIZE_FOLDERNAME;
	hFile = ::FindFirstFile(str.c_str(), &finddata);
	if(hFile == INVALID_HANDLE_VALUE || !(finddata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		::CreateDirectory(str.c_str(), NULL);		

	::FindClose( hFile );

	// 계정.
	::WideCharToMultiByte(GetACP(), 0, pUserData->wchUserID, -1, chBuffer, 128, NULL, NULL);
	str += "\\";
	str += chBuffer;
	hFile = ::FindFirstFile(str.c_str(), &finddata);
	if(hFile == INVALID_HANDLE_VALUE || !(finddata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		::CreateDirectory(str.c_str(), NULL);		

	::FindClose( hFile );

	// server
	std::wstring wstr = Logic_GetActiveServerName();
	WideCharToMultiByte(GetACP(), 0, wstr.c_str(), -1, chBuffer, 128, NULL, NULL);
	str += "\\";
	str += chBuffer;
	hFile = ::FindFirstFile(str.c_str(), &finddata);
	if(hFile == INVALID_HANDLE_VALUE || !(finddata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		::CreateDirectory(str.c_str(), NULL);		

	::FindClose( hFile );

	// 캐릭터 이름.
	::WideCharToMultiByte(GetACP(), 0, pCharName, -1, chBuffer, 128, NULL, NULL);
	str += "\\";
	str += chBuffer;
	hFile = ::FindFirstFile(str.c_str(), &finddata);
	if(hFile == INVALID_HANDLE_VALUE || !(finddata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		::CreateDirectory(str.c_str(), NULL);		

	::FindClose( hFile );

	strFileName = str;

	return TRUE;
}
/**
* \brief eNTL_STORAGE_GROUP_ACCOUNT 관련 옵션들을 로드한다.
*/
void Logic_LoadAccountOption( void )
{
	RwChar chBuffer[128];
	SUserData *pUserData = GetDboGlobal()->GetUserData();

	// user
	std::string str = USEROPT_SERIALIZE_FOLDERNAME;

	// 계정
	::WideCharToMultiByte(GetACP(), 0, pUserData->wchUserID, -1, chBuffer, 128, NULL, NULL);
	str += "\\";
	str += chBuffer;

	// file name
	str += "\\";
	str += ACCOUNTOPT_SERIALIZE_FILENAME;

	WIN32_FIND_DATA finddata;
	HANDLE hFile = ::FindFirstFile(str.c_str(), &finddata);

	if(hFile == INVALID_HANDLE_VALUE || finddata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
	{
		/*NtlLogFilePrint("Logic_LoadAccountOption");*/
		::FindClose( hFile );
		return;
	}

	::FindClose( hFile );

	GetNtlStorageManager()->Load( eNTL_STORAGE_GROUP_ACCOUNT, str.c_str() );
}

/**
* \brief eNTL_STORAGE_GROUP_ACCOUNT 관련 옵션들을 저장한다.
*/
void Logic_SaveAccountOption( void )
{
	RwChar chBuffer[128];	
	SUserData *pUserData = GetDboGlobal()->GetUserData();

	HANDLE hFile;
	WIN32_FIND_DATA finddata;

	// user
	std::string str = USEROPT_SERIALIZE_FOLDERNAME;
	hFile = ::FindFirstFile(str.c_str(), &finddata);
	if(hFile == INVALID_HANDLE_VALUE || !(finddata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		::CreateDirectory(str.c_str(), NULL);		

	::FindClose( hFile );

	// 계정.
	::WideCharToMultiByte(GetACP(), 0, pUserData->wchUserID, -1, chBuffer, 128, NULL, NULL);
	str += "\\";
	str += chBuffer;
	hFile = ::FindFirstFile(str.c_str(), &finddata);
	if(hFile == INVALID_HANDLE_VALUE || !(finddata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		::CreateDirectory(str.c_str(), NULL);	

	::FindClose( hFile );

	// file name
	str += "\\";
	str += ACCOUNTOPT_SERIALIZE_FILENAME;

	GetNtlStorageManager()->Save( eNTL_STORAGE_GROUP_ACCOUNT, str.c_str() );
}

/**
* \brief eNTL_STORAGE_GROUP_SYSTEM 관련 옵션들을 로드한다.
*/
void Logic_LoadSystemOption( void )
{

	// user
	std::string str = USEROPT_SERIALIZE_FOLDERNAME;

	// file name
	str += "\\";
	str += SYSTEMENV_SERIALIZE_FILENAME;

	WIN32_FIND_DATA finddata;
	HANDLE hFile = ::FindFirstFile(str.c_str(), &finddata);

	if(hFile == INVALID_HANDLE_VALUE || finddata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
	{
		/*NtlLogFilePrint("Logic_LoadSystemOption");*/
		::FindClose( hFile );
		return;
	}

	::FindClose( hFile );

	GetNtlStorageManager()->Load( eNTL_STORAGE_GROUP_SYSTEM, str.c_str() );
}

/**
* \brief eNTL_STORAGE_GROUP_SYSTEM 관련 옵션들을 저장한다.
*/
void Logic_SaveSystemOption( void )
{

	HANDLE hFile;
	WIN32_FIND_DATA finddata;

	// user
	std::string str = USEROPT_SERIALIZE_FOLDERNAME;
	hFile = ::FindFirstFile(str.c_str(), &finddata);
	if(hFile == INVALID_HANDLE_VALUE || !(finddata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		::CreateDirectory(str.c_str(), NULL);		

	::FindClose( hFile );

	// file name
	str += "\\";
	str += SYSTEMENV_SERIALIZE_FILENAME;

	GetNtlStorageManager()->Save( eNTL_STORAGE_GROUP_SYSTEM, str.c_str() );
}

/**
* \brief eNTL_STORAGE_GROUP_GAMEINFO 관련 옵션들을 로드한다.
*/
void Logic_LoadGameOption(void)
{
	RwChar chBuffer[128];
	SUserData *pUserData = GetDboGlobal()->GetUserData();

	// user
	std::string str = USEROPT_SERIALIZE_FOLDERNAME;

	// 계정
	::WideCharToMultiByte(GetACP(), 0, pUserData->wchUserID, -1, chBuffer, 128, NULL, NULL);
	str += "\\";
	str += chBuffer;

	// file name
	str += "\\";
	str += GAMEENV_SERIALIZE_FILENAME;

	WIN32_FIND_DATA finddata;
	HANDLE hFile = ::FindFirstFile(str.c_str(), &finddata);

	if(hFile == INVALID_HANDLE_VALUE || finddata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
	{
		/*NtlLogFilePrint("Logic_LoadGameOption");*/
		::FindClose( hFile );
		return;
	}

	::FindClose( hFile );

	GetNtlStorageManager()->Load( eNTL_STORAGE_GROUP_GAMEINFO, str.c_str() );
}

/**
* \brief eNTL_STORAGE_GROUP_GAMEINFO 관련 옵션들을 저장한다.
*/
void Logic_SaveGameOption(void)
{
	RwChar chBuffer[128];	
	SUserData *pUserData = GetDboGlobal()->GetUserData();

	HANDLE hFile;
	WIN32_FIND_DATA finddata;

	// user
	std::string str = USEROPT_SERIALIZE_FOLDERNAME;
	hFile = ::FindFirstFile(str.c_str(), &finddata);
	if(hFile == INVALID_HANDLE_VALUE || !(finddata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		::CreateDirectory(str.c_str(), NULL);		

	::FindClose( hFile );

	// 계정.
	::WideCharToMultiByte(GetACP(), 0, pUserData->wchUserID, -1, chBuffer, 128, NULL, NULL);
	str += "\\";
	str += chBuffer;
	hFile = ::FindFirstFile(str.c_str(), &finddata);
	if(hFile == INVALID_HANDLE_VALUE || !(finddata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		::CreateDirectory(str.c_str(), NULL);	

	::FindClose( hFile );

	// file name
	str += "\\";
	str += GAMEENV_SERIALIZE_FILENAME;

	GetNtlStorageManager()->Save( eNTL_STORAGE_GROUP_GAMEINFO, str.c_str() );
}

void Logic_LoadCharacterOption( void )
{
	RwChar chBuffer[128];
	SUserData *pUserData = GetDboGlobal()->GetUserData();

	// user
	std::string str = USEROPT_SERIALIZE_FOLDERNAME;

	// 계정
	::WideCharToMultiByte(GetACP(), 0, pUserData->wchUserID, -1, chBuffer, 128, NULL, NULL);
	str += "\\";
	str += chBuffer;

	SERVER_HANDLE	hServer	= GetLobbyManager()->GetSelectedServerHandle();
	CLobby*			pLobby	= GetLobbyManager()->GetLobby(hServer);
	if( !pLobby )
	{
		DBO_FAIL("Not exist lobby infomation of server handle : " << hServer);
		return;
	}

	// 서버 이름.
	::WideCharToMultiByte(GetACP(), 0, pLobby->GetServerName(), -1, chBuffer, 128, NULL, NULL);
	str += "\\";
	str += chBuffer;

	// 캐릭터 이름.
	RwUInt8 bySelChar = pLobby->GetSelectedCharacterIndex();
	sLOBBY_CHARACTER* pLOBBY_CHARACTER = pLobby->GetCharacter( bySelChar );
	if( !pLOBBY_CHARACTER )
	{
		DBO_FAIL("Not exist character of index : " << pLobby->GetSelectedCharacterIndex());
		NTL_RETURNVOID();
	}

	::WideCharToMultiByte(GetACP(), 0, pLOBBY_CHARACTER->tSummary.awchName, -1, chBuffer, 128, NULL, NULL);
	str += "\\";
	str += chBuffer;

	// file name
	str += "\\";
	str += CHARACTERENV_SERIALIZE_FILENAME;

	WIN32_FIND_DATA finddata;
	HANDLE hFile = ::FindFirstFile(str.c_str(), &finddata);

	if(hFile == INVALID_HANDLE_VALUE || finddata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
	{
		GetNtlStorageManager()->Reset( eNTL_STORAGE_GROUP_CHARACTER );
		::FindClose( hFile );
		return;
	}

	::FindClose( hFile );

	GetNtlStorageManager()->Load( eNTL_STORAGE_GROUP_CHARACTER, str.c_str() );
}

void Logic_SaveCharacterOption( void )
{
	RwChar chBuffer[128];	
	SUserData *pUserData = GetDboGlobal()->GetUserData();

	HANDLE hFile;
	WIN32_FIND_DATA finddata;

	// user
	std::string str = USEROPT_SERIALIZE_FOLDERNAME;
	hFile = ::FindFirstFile(str.c_str(), &finddata);
	if(hFile == INVALID_HANDLE_VALUE || !(finddata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		::CreateDirectory(str.c_str(), NULL);	

	::FindClose( hFile );

	// 계정.
	::WideCharToMultiByte(GetACP(), 0, pUserData->wchUserID, -1, chBuffer, 128, NULL, NULL);
	str += "\\";
	str += chBuffer;
	hFile = ::FindFirstFile(str.c_str(), &finddata);
	if(hFile == INVALID_HANDLE_VALUE || !(finddata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		::CreateDirectory(str.c_str(), NULL);	

	::FindClose( hFile );

	SERVER_HANDLE	hServer	= GetLobbyManager()->GetSelectedServerHandle();
	CLobby*			pLobby	= GetLobbyManager()->GetLobby(hServer);
	if( !pLobby )
	{
		DBO_FAIL("Not exist lobby infomation of server handle : " << hServer);
		return;
	}

	// 서버 이름.
	::WideCharToMultiByte(GetACP(), 0, pLobby->GetServerName(), -1, chBuffer, 128, NULL, NULL);
	str += "\\";
	str += chBuffer;
	hFile = ::FindFirstFile(str.c_str(), &finddata);
	if(hFile == INVALID_HANDLE_VALUE || !(finddata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		::CreateDirectory(str.c_str(), NULL);	

	::FindClose( hFile );

	// 캐릭터 이름.
	RwUInt8 bySelChar = pLobby->GetSelectedCharacterIndex();
	sLOBBY_CHARACTER* pLOBBY_CHARACTER = pLobby->GetCharacter( bySelChar );
	if( !pLOBBY_CHARACTER )
	{
		DBO_FAIL("Not exist character of index : " << pLobby->GetSelectedCharacterIndex());
		NTL_RETURNVOID();
	}

	::WideCharToMultiByte(GetACP(), 0, pLOBBY_CHARACTER->tSummary.awchName, -1, chBuffer, 128, NULL, NULL);
	str += "\\";
	str += chBuffer;
	hFile = ::FindFirstFile(str.c_str(), &finddata);
	if(hFile == INVALID_HANDLE_VALUE || !(finddata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		::CreateDirectory(str.c_str(), NULL);	

	::FindClose( hFile );

	// file name
	str += "\\";
	str += CHARACTERENV_SERIALIZE_FILENAME;

	GetNtlStorageManager()->Save( eNTL_STORAGE_GROUP_CHARACTER, str.c_str() );
}

/**
* \brief eNTL_STORAGE_GROUP_SCOUTER 관련 옵션들을 로드한다.
*/
void Logic_LoadScouterOption(void)
{
	RwChar chBuffer[128];
	SUserData *pUserData = GetDboGlobal()->GetUserData();

	// user
	std::string str = USEROPT_SERIALIZE_FOLDERNAME;

	// 계정
	::WideCharToMultiByte(GetACP(), 0, pUserData->wchUserID, -1, chBuffer, 128, NULL, NULL);
	str += "\\";
	str += chBuffer;

	SERVER_HANDLE	hServer	= GetLobbyManager()->GetSelectedServerHandle();
	CLobby*			pLobby	= GetLobbyManager()->GetLobby(hServer);
	if( !pLobby )
	{
		DBO_FAIL("Not exist lobby infomation of server handle : " << hServer);
		return;
	}

	// 서버 이름.
	::WideCharToMultiByte(GetACP(), 0, pLobby->GetServerName(), -1, chBuffer, 128, NULL, NULL);
	str += "\\";
	str += chBuffer;

	// 캐릭터 이름.
	RwUInt8 bySelChar = pLobby->GetSelectedCharacterIndex();
	sLOBBY_CHARACTER* pLOBBY_CHARACTER = pLobby->GetCharacter( bySelChar );
	if( !pLOBBY_CHARACTER )
	{
		DBO_FAIL("Not exist character of index : " << pLobby->GetSelectedCharacterIndex());
		NTL_RETURNVOID();
	}

	::WideCharToMultiByte(GetACP(), 0, pLOBBY_CHARACTER->tSummary.awchName, -1, chBuffer, 128, NULL, NULL);
	str += "\\";
	str += chBuffer;

	// file name
	str += "\\";
	str += SCOUTEROPT_SERILIZE_FILENAME;

	WIN32_FIND_DATA finddata;
	HANDLE hFile = ::FindFirstFile(str.c_str(), &finddata);

	if(hFile == INVALID_HANDLE_VALUE || finddata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
	{
		::FindClose( hFile );
		return;
	}

	::FindClose( hFile );

	GetNtlStorageManager()->Load( eNTL_STORAGE_GROUP_SCOUTER, str.c_str() );
}

/**
* \brief eNTL_STORAGE_GROUP_SCOUTER 관련 옵션들을 저장한다.
*/
void Logic_SaveScouterOption(void)
{	
	RwChar chBuffer[128];	
	SUserData *pUserData = GetDboGlobal()->GetUserData();

	HANDLE hFile;
	WIN32_FIND_DATA finddata;

	// user
	std::string str = USEROPT_SERIALIZE_FOLDERNAME;
	hFile = ::FindFirstFile(str.c_str(), &finddata);
	if(hFile == INVALID_HANDLE_VALUE || !(finddata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		::CreateDirectory(str.c_str(), NULL);	

	::FindClose( hFile );

	// 계정.
	::WideCharToMultiByte(GetACP(), 0, pUserData->wchUserID, -1, chBuffer, 128, NULL, NULL);
	str += "\\";
	str += chBuffer;
	hFile = ::FindFirstFile(str.c_str(), &finddata);
	if(hFile == INVALID_HANDLE_VALUE || !(finddata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		::CreateDirectory(str.c_str(), NULL);	

	::FindClose( hFile );

	SERVER_HANDLE	hServer	= GetLobbyManager()->GetSelectedServerHandle();
	CLobby*			pLobby	= GetLobbyManager()->GetLobby(hServer);
	if( !pLobby )
	{
		DBO_FAIL("Not exist lobby infomation of server handle : " << hServer);
		return;
	}

	// 서버 이름.
	::WideCharToMultiByte(GetACP(), 0, pLobby->GetServerName(), -1, chBuffer, 128, NULL, NULL);
	str += "\\";
	str += chBuffer;
	hFile = ::FindFirstFile(str.c_str(), &finddata);
	if(hFile == INVALID_HANDLE_VALUE || !(finddata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		::CreateDirectory(str.c_str(), NULL);	

	::FindClose( hFile );

	// 캐릭터 이름.
	RwUInt8 bySelChar = pLobby->GetSelectedCharacterIndex();
	sLOBBY_CHARACTER* pLOBBY_CHARACTER = pLobby->GetCharacter( bySelChar );
	if( !pLOBBY_CHARACTER )
	{
		DBO_FAIL("Not exist character of index : " << pLobby->GetSelectedCharacterIndex());
		NTL_RETURNVOID();
	}

	::WideCharToMultiByte(GetACP(), 0, pLOBBY_CHARACTER->tSummary.awchName, -1, chBuffer, 128, NULL, NULL);
	str += "\\";
	str += chBuffer;
	hFile = ::FindFirstFile(str.c_str(), &finddata);
	if(hFile == INVALID_HANDLE_VALUE || !(finddata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		::CreateDirectory(str.c_str(), NULL);	

	::FindClose( hFile );

	// file name
	str += "\\";
	str += SCOUTEROPT_SERILIZE_FILENAME;

	GetNtlStorageManager()->Save( eNTL_STORAGE_GROUP_SCOUTER, str.c_str() );
}

/**
* \brief eNTL_STORAGE_GROUP_QUEST 관련 옵션들을 로드한다.
*/
void Logic_LoadQuestOption(void)
{
	RwChar chBuffer[128];
	SUserData *pUserData = GetDboGlobal()->GetUserData();

	// user
	std::string str = USEROPT_SERIALIZE_FOLDERNAME;

	// 계정
	::WideCharToMultiByte(GetACP(), 0, pUserData->wchUserID, -1, chBuffer, 128, NULL, NULL);
	str += "\\";
	str += chBuffer;

	SERVER_HANDLE	hServer	= GetLobbyManager()->GetSelectedServerHandle();
	CLobby*			pLobby	= GetLobbyManager()->GetLobby(hServer);
	if( !pLobby )
	{
		DBO_FAIL("Not exist lobby infomation of server handle : " << hServer);
		return;
	}

	// 서버 이름.
	::WideCharToMultiByte(GetACP(), 0, pLobby->GetServerName(), -1, chBuffer, 128, NULL, NULL);
	str += "\\";
	str += chBuffer;

	// 캐릭터 이름.
	RwUInt8 bySelChar = pLobby->GetSelectedCharacterIndex();
	sLOBBY_CHARACTER* pLOBBY_CHARACTER = pLobby->GetCharacter( bySelChar );
	if( !pLOBBY_CHARACTER )
	{
		DBO_FAIL("Not exist character of index : " << pLobby->GetSelectedCharacterIndex());
		NTL_RETURNVOID();
	}

	::WideCharToMultiByte(GetACP(), 0, pLOBBY_CHARACTER->tSummary.awchName, -1, chBuffer, 128, NULL, NULL);
	str += "\\";
	str += chBuffer;

	// file name
	str += "\\";
	str += QUESTOPT_SERIALIZE_FILENAME;

	WIN32_FIND_DATA finddata;
	HANDLE hFile = ::FindFirstFile(str.c_str(), &finddata);

	if(hFile == INVALID_HANDLE_VALUE || finddata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
	{
		::FindClose( hFile );
		return;
	}

	::FindClose( hFile );

	GetNtlStorageManager()->Load( eNTL_STORAGE_GROUP_QUEST, str.c_str() );
}

/**
* \brief eNTL_STORAGE_GROUP_QUEST 관련 옵션들을 저장한다.
*/
void Logic_SaveQuestOption(void)
{
	// Perfomence


	RwChar chBuffer[128];	
	SUserData *pUserData = GetDboGlobal()->GetUserData();

	HANDLE hFile;
	WIN32_FIND_DATA finddata;

	// user
	std::string str = USEROPT_SERIALIZE_FOLDERNAME;
	hFile = ::FindFirstFile(str.c_str(), &finddata);
	if(hFile == INVALID_HANDLE_VALUE || !(finddata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		::CreateDirectory(str.c_str(), NULL);	

	::FindClose( hFile );

	// 계정.
	::WideCharToMultiByte(GetACP(), 0, pUserData->wchUserID, -1, chBuffer, 128, NULL, NULL);
	str += "\\";
	str += chBuffer;
	hFile = ::FindFirstFile(str.c_str(), &finddata);
	if(hFile == INVALID_HANDLE_VALUE || !(finddata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		::CreateDirectory(str.c_str(), NULL);	

	::FindClose( hFile );

	SERVER_HANDLE	hServer	= GetLobbyManager()->GetSelectedServerHandle();
	CLobby*			pLobby	= GetLobbyManager()->GetLobby(hServer);
	if( !pLobby )
	{
		DBO_FAIL("Not exist lobby infomation of server handle : " << hServer);
		return;
	}

	// 서버 이름.
	::WideCharToMultiByte(GetACP(), 0, pLobby->GetServerName(), -1, chBuffer, 128, NULL, NULL);
	str += "\\";
	str += chBuffer;
	hFile = ::FindFirstFile(str.c_str(), &finddata);
	if(hFile == INVALID_HANDLE_VALUE || !(finddata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		::CreateDirectory(str.c_str(), NULL);	

	::FindClose( hFile );

	// 캐릭터 이름.
	RwUInt8 bySelChar = pLobby->GetSelectedCharacterIndex();
	sLOBBY_CHARACTER* pLOBBY_CHARACTER = pLobby->GetCharacter( bySelChar );
	if( !pLOBBY_CHARACTER )
	{
		DBO_FAIL("Not exist character of index : " << pLobby->GetSelectedCharacterIndex());
		NTL_RETURNVOID();
	}

	::WideCharToMultiByte(GetACP(), 0, pLOBBY_CHARACTER->tSummary.awchName, -1, chBuffer, 128, NULL, NULL);
	str += "\\";
	str += chBuffer;
	hFile = ::FindFirstFile(str.c_str(), &finddata);
	if(hFile == INVALID_HANDLE_VALUE || !(finddata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		::CreateDirectory(str.c_str(), NULL);	

	::FindClose( hFile );

	// file name
	str += "\\";
	str += QUESTOPT_SERIALIZE_FILENAME;

	GetNtlStorageManager()->Save( eNTL_STORAGE_GROUP_QUEST, str.c_str() );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	game의 각종 callback 함수.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Logic_CallbackHtmlUserTag(const WCHAR *pTag, RwUInt32 uiId, std::wstring& strOut)
{
	std::wstring strTag = pTag;
	if(strTag == L"npc")
	{
		/*sNPC_TBLDAT *pNpcTblData = reinterpret_cast<sNPC_TBLDAT*>( API_GetTableContainer()->GetNpcTable()->FindData(uiId) ); */
		CTextTable* pNPCText = API_GetTableContainer()->GetTextAllTable()->GetNPCTbl();
		if(pNPCText)
			strOut = pNPCText->GetText( uiId );
	}
	else if(strTag == L"object")
	{
		CTextTable* pObjText = API_GetTableContainer()->GetTextAllTable()->GetObjectTbl();
		strOut = pObjText->GetText( uiId );
	}
	else if(strTag == L"mob")
	{
		/*sMOB_TBLDAT *pMobTblData = reinterpret_cast<sMOB_TBLDAT*>( API_GetTableContainer()->GetMobTable()->FindData(uiId) ); */
		CTextTable* pMobText = API_GetTableContainer()->GetTextAllTable()->GetMobTbl();
		if(pMobText)
			strOut = pMobText->GetText( uiId );
	}
	else if(strTag == L"etc")
	{
		CTextTable* pETCText = API_GetTableContainer()->GetTextAllTable()->GetETCTbl();
		strOut = pETCText->GetText( uiId );		
	}
	else if(strTag == L"user")
	{
		strOut = Logic_GetName(GetNtlSLGlobal()->GetSobAvatar());
	}
	else if(strTag == L"class")
	{
		CNtlSobAvatar* pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();
		CNtlSobAvatarAttr* pSobAvatarAttr = reinterpret_cast<CNtlSobAvatarAttr*>(pSobAvatar->GetSobAttr());

		strOut = Logic_GetClassName( pSobAvatarAttr->GetClass() );
	}
	else if(strTag == L"race")
	{
		if( uiId < 0 || uiId > DST_MAJIN - DST_HUMAN )
			strOut = L"!!race value error!!";
		else
			strOut = GetDisplayStringManager()->GetString( DST_HUMAN + uiId );
	}
	else if(strTag == L"place")	// 2008.1.3 추가 comment by Kell
	{
		CMapNameTextTable* pPlaceText = API_GetTableContainer()->GetTextAllTable()->GetMapNameTbl();
		strOut = pPlaceText->GetText( uiId );
	}
	else if(strTag == L"item")
	{
		CTextTable* pItemText = API_GetTableContainer()->GetTextAllTable()->GetItemTbl();
		strOut = pItemText->GetText( uiId );
	}
	else if(strTag == L"questitem")
	{
		CTextTable* pQuestItemText = API_GetTableContainer()->GetTextAllTable()->GetQuestItemTbl();
		strOut = pQuestItemText->GetText( uiId );
	}
	else if(strTag == L"skill")
	{
		CTextTable* pSkillText = API_GetTableContainer()->GetTextAllTable()->GetSkillTbl();
		strOut = pSkillText->GetText( uiId );
	}
}

void Logic_CallbackHtmlLinkTag( gui::CComponent* pComponent, RwUInt8 byType, RwUInt32 uiIndex, std::wstring& strFilename )
{
	// File
	if( byType == gui::eHTML_LINK_FILE )
	{
		gui::CHtmlBox* pHtmlBox = reinterpret_cast<gui::CHtmlBox*>( pComponent );
		pHtmlBox->SetHtmlFromFile( strFilename.c_str() );
	}
	// Interface ( Table )
	else if( byType == gui::eHTML_LINK_INTERFACE )
	{
		sSCRIPT_LINK_TBLDAT *pScriptTblData = reinterpret_cast<sSCRIPT_LINK_TBLDAT*>( API_GetTableContainer()->GetScriptLinkTable()->FindData( uiIndex ) );
		if( pScriptTblData )
			CDboEventGenerator::ScriptLinkNotify( pScriptTblData->byType, pScriptTblData->byAction, pScriptTblData->wszValue );
	}
}

RwUInt32 Logic_CalcPrice_for_Sell(sITEM_TBLDAT* pITEM_TBLDAT, sITEM_OPTION_TBLDAT* pITEM_OPTION_TBLDAT, RwUInt32 uiCount)
{
	RwUInt32 uiSellPrice;

	if(pITEM_OPTION_TBLDAT)
	{
		uiSellPrice = (RwUInt32)NTL_SELL_PRICE(pITEM_TBLDAT->bySell_Price, pITEM_TBLDAT->dwCost, pITEM_OPTION_TBLDAT->dwCost);
	}
	else
	{
		uiSellPrice = (RwUInt32)NTL_SELL_PRICE(pITEM_TBLDAT->bySell_Price, pITEM_TBLDAT->dwCost, 0);
	}

	// avooo's comment : 기획에 의해 판매가격은 최소한 1이상이다
	if( uiSellPrice == 0 )
		uiSellPrice = 1;

	return uiSellPrice * uiCount;
}

RwUInt32 Logic_AllRepairCost()
{
	RwUInt32 uiRepairCost = 0;	
	CNtlInventory* pInventory = GetNtlSLGlobal()->GetSobAvatar()->GetInventory();	
	CNtlSobItem* pSobItem = NULL;
	CNtlSobItemAttr* pSobItemAttr = NULL;
	sITEM_TBLDAT* pITEM_TBLDAT = NULL;
	SERIAL_HANDLE hItem = INVALID_SERIAL_ID;

	// 장착 아이템의 수리 가격
	for( RwUInt8 i = 0 ; i < EQUIP_SLOT_TYPE_COUNT ; ++i )
	{
		hItem = pInventory->GetEquipItem(i);
		if( hItem == INVALID_SERIAL_ID )
			continue;		

		pSobItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject(hItem) );
		NTL_ASSERT(pSobItem, "Logic_AllRepairCost, Not exist sob equip item of handle : " << hItem);

		pSobItemAttr = reinterpret_cast<CNtlSobItemAttr*>( pSobItem->GetSobAttr() );
		NTL_ASSERT(pSobItemAttr, "Logic_AllRepairCost, Not exist sob equip item attr of handle : " << hItem);

		pITEM_TBLDAT = pSobItemAttr->GetItemTbl();
		NTL_ASSERT(pITEM_TBLDAT, "Logic_AllRepairCost, Not exist equip item table of sob item of handle : " << hItem);

		uiRepairCost += Dbo_GetRepairPay(pITEM_TBLDAT->dwCost, pSobItemAttr->GetMaxDur(), pSobItemAttr->GetDur());
	}

	// avooo's comment : 와우처럼 가방의 아이템도 한꺼번에 고치기 위한 값이 필요한 줄 알고 코딩했다
	//					 기획의 변경이 있을지 모르니 지우지 말고 주석 처리해두자
	// 가방안의 아이템
	/*
	for( RwUInt8 i = 0 ; i < NTL_MAX_BAGSLOT_COUNT ; ++i )
	{
	hItem = pInventory->GetBagItem(i);
	if( hItem == INVALID_SERIAL_ID )
	continue;

	pSobItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject(hItem) );
	NTL_ASSERT(pSobItem, "Logic_AllRepairCost, Not exist sob equip item of handle : " << hItem);

	pITEM_TBLDAT = Logic_GetItemDataFromSob(hItem);
	NTL_ASSERT(pITEM_TBLDAT, "Logic_AllRepairCost, Not exist sob bag item table of item handle : " << hItem);

	for( RwUInt8 k = 0 ; k < pITEM_TBLDAT->byBag_Size ; ++k )
	{
	SERIAL_HANDLE hChildItem = pSobItem->GetChildSerial(k);
	if( hChildItem == INVALID_SERIAL_ID )
	continue;

	CNtlSobItem* pSobChildItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject(hChildItem) );
	NTL_ASSERT(pSobChildItem, "Logic_AllRepairCost, Not exist sob bag child item of handle : " << hChildItem);

	CNtlSobItemAttr* pSobChildItemAttr = reinterpret_cast<CNtlSobItemAttr*>( pSobChildItem->GetSobAttr() );
	NTL_ASSERT(pSobChildItemAttr, "Logic_AllRepairCost, Not exist sob bag child item attr of handle : " << hChildItem);

	if( pSobChildItemAttr->IsNeedToIdentify() )
	continue;

	sITEM_TBLDAT* pITEM_TBLDAT_Child = pSobChildItemAttr->GetItemTbl();
	NTL_ASSERT(pITEM_TBLDAT_Child, "Logic_AllRepairCost, Not exist bag child item table of sob item of handle : " << hChildItem);

	uiRepairCost += Dbo_GetRepairPay(pITEM_TBLDAT_Child->dwCost, pSobChildItemAttr->GetMaxDur(), pSobChildItemAttr->GetDur());
	}
	}*/

	return uiRepairCost;
}

void Logic_WorldItemPick(SERIAL_HANDLE hPickSerial)
{
	if(!GetNtlWorldConcept()->IsEnableAction(E_ACTION_PICK_OBJECT))
		return;

	if(Logic_IsDirectMode(GetNtlSLGlobal()->GetSobAvatar()))
		return;

	if( !Logic_CanMouseInput_in_Tutorial( ETL_MOUSE_INPUT_TYPE_TARGETING ) )
		return;

	CNtlSobActor *pPickActor = reinterpret_cast<CNtlSobActor*>( GetNtlSobManager()->GetSobObject(hPickSerial) ); 
	if(pPickActor == NULL)
		return;

	if(!pPickActor->CanClick())
		return;

	// 이부분을 어떻게 처리할 것인가?
	//	DeleteMoveMark();

	CNtlSob *pPickSobObj = GetNtlSobManager()->GetSobObject(hPickSerial);

	// 현재 지정된 target이 없는 경우.
	if(GetDboGlobal()->GetTargetSerial() == INVALID_SERIAL_ID)
	{
		GetDboGlobal()->SetTargetSerial(hPickSerial);

		API_GetSLPacketGenerator()->SendCharTargetSelect(hPickSerial);

		RwBool bTSSuccess = CNtlSLEventGenerator::SobTargetSelect(hPickSerial);
		if(!bTSSuccess)
			return;

		CNtlSobActor *pTarActor = (CNtlSobActor*)pPickSobObj;
		CNtlSobAvatar *pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();
		if( Logic_IsCommunityActor(pTarActor) )
		{
			RwReal fCummuRange = Logic_GetCommunityRange(pTarActor);
			if(Logic_InFollowRange(pSobAvatar, pTarActor, fCummuRange))
			{
				Logic_BeginActorCommunity(pSobAvatar, pTarActor);
				return;
			}
		}
	}
	// 현재 지정된 target이 있는 경우.
	else
	{
		RwUInt32 uiCurrTargetSerialId = GetDboGlobal()->GetTargetSerial();

		if((uiCurrTargetSerialId != hPickSerial) || (pPickSobObj && Logic_IsCommunityActor((CNtlSobActor*)pPickSobObj)))
		{
			if(uiCurrTargetSerialId != hPickSerial)
			{
				API_GetSLPacketGenerator()->SendCharTargetSelect(hPickSerial);
				CNtlSLEventGenerator::SobTargetSelectRelease(uiCurrTargetSerialId);
			}

			GetDboGlobal()->SetTargetSerial(hPickSerial);

			RwBool bTSSuccess = CNtlSLEventGenerator::SobTargetSelect(hPickSerial);
			if(!bTSSuccess)
				return;

			// communication actor 이면?
			CNtlSobActor *pTarActor = (CNtlSobActor*)pPickSobObj;
			CNtlSobAvatar *pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();
			if( Logic_IsCommunityActor(pTarActor) )
			{
				RwReal fCummuRange = Logic_GetCommunityRange(pTarActor);
				if(Logic_InFollowRange(pSobAvatar, pTarActor, fCummuRange))
				{
					Logic_BeginActorCommunity(pSobAvatar, pTarActor);
					return;
				}
			}
		}
	}  
}

void Logic_SobTarget( RwUInt32 hTargetSerial )
{
	RwUInt32 hOldTargetSerial = GetDboGlobal()->GetTargetSerial();

	if( hOldTargetSerial != INVALID_SERIAL_ID )
		CNtlSLEventGenerator::SobTargetSelectRelease( hOldTargetSerial );

	if( hTargetSerial != hOldTargetSerial )
		API_GetSLPacketGenerator()->SendCharTargetSelect( hTargetSerial );

	GetDboGlobal()->SetTargetSerial( hTargetSerial );
	CNtlSLEventGenerator::SobTargetSelect( hTargetSerial );
}

void Logic_SobTargetRelease(void)
{
	RwUInt32 hTargetSerial = GetDboGlobal()->GetTargetSerial();

	if( hTargetSerial == INVALID_SERIAL_ID )
		return;

	API_GetSLPacketGenerator()->SendCharTargetSelect( INVALID_SERIAL_ID );

	GetDboGlobal()->SetTargetSerial( INVALID_SERIAL_ID );
	CNtlSLEventGenerator::SobTargetSelectRelease( hTargetSerial );
}

void Logic_AvatarTarget(void)
{
	if(!GetNtlWorldConcept()->IsEnableAction(E_ACTION_MOVE))
		return;// TRUE;

	if( !Logic_CanKeybaordInput_in_Tutorial( ETL_KEYBOARD_INPUT_TYPE_TARGETING ) )
		return;// FALSE;

	CNtlSobAvatar *pSobAvatar = GetNtlSLGlobal()->GetSobAvatar(); 
	if(pSobAvatar)
	{
		SERIAL_HANDLE hSerialId = pSobAvatar->GetSerialID();
		if(hSerialId != GetDboGlobal()->GetTargetSerial())
		{
			if(GetDboGlobal()->GetTargetSerial() != INVALID_SERIAL_ID)
				CNtlSLEventGenerator::SobTargetSelectRelease(GetDboGlobal()->GetTargetSerial());

			GetDboGlobal()->SetTargetSerial(hSerialId);
			CNtlSLEventGenerator::SobTargetSelect(hSerialId);

			API_GetSLPacketGenerator()->SendCharTargetSelect(hSerialId);
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  스킬 관련
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
RwReal Logic_GetHTBTotalPower( sHTB_SET_TBLDAT* pHTBData )
{
	CSkillTable* pSkillTable = API_GetTableContainer()->GetSkillTable();
	sSKILL_TBLDAT* pSkillData = NULL;
	RwReal fDamageTotal = 0;

	for( RwInt32 i = 0 ; i < NTL_HTB_MAX_SKILL_COUNT_IN_SET ; ++i )
	{
		if( pHTBData->aHTBAction[i].bySkillType != 0 || pHTBData->aHTBAction[i].skillTblidx == INVALID_TBLIDX )
			continue;

		pSkillData = reinterpret_cast<sSKILL_TBLDAT*>( pSkillTable->FindData( pHTBData->aHTBAction[i].skillTblidx ) );

		if( pSkillData )
		{
			if( pSkillData->skill_Effect[0] != INVALID_TBLIDX )
				fDamageTotal += pSkillData->fSkill_Effect_Value[0];
			if( pSkillData->skill_Effect[1] != INVALID_TBLIDX )
				fDamageTotal += pSkillData->fSkill_Effect_Value[1];
		}
	}

	return fDamageTotal;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	배틀 속성 아이콘 및 툴팁 설정
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Logic_SetBattleAttributeMark( gui::CPanel* pComponent, RwUInt8 byBattleAttribute, RwBool bOutLine /* = TRUE  */ )
{
	gui::CSurface surface = Logic_GetBattleAttributeIconSurface( byBattleAttribute, bOutLine );

	// ToolTip은 InfoWindow로 대체한다.

	pComponent->Show( true );
	pComponent->GetSurface()->clear();
	pComponent->AddSurface( surface );
}


RwUInt32 Logic_GetBattleAttributeColor( RwUInt8 byBattleAttribute )
{
	switch( byBattleAttribute )
	{
	case BATTLE_ATTRIBUTE_NONE:		return INFOCOLOR_0;
	case BATTLE_ATTRIBUTE_HONEST:	return INFOCOLOR_3;
	case BATTLE_ATTRIBUTE_STRANGE:	return INFOCOLOR_8;
	case BATTLE_ATTRIBUTE_WILD:		return INFOCOLOR_5;
	case BATTLE_ATTRIBUTE_ELEGANCE: return INFOCOLOR_2;
	case BATTLE_ATTRIBUTE_FUNNY:	return INFOCOLOR_7;
	default:						return INFOCOLOR_9;
	}
}

RwUInt8 Logic_GetCounterpartGradeType(CNtlSobAttr* pOriginalSobAttr, CNtlSobAttr* pCounterpartSobAttr)
{
	if( !pOriginalSobAttr || !pCounterpartSobAttr )
		return CGT_SIMILAR;

	CNtlSobAttr* pSob[2];
	RwUInt8 byLevel[2];

	pSob[0] = pOriginalSobAttr;
	pSob[1] = pCounterpartSobAttr;

	for( RwUInt8 i = 0 ; i < 2 ; ++i )
	{
		RwUInt8 byClassID = (RwUInt8)pSob[i]->GetClassID();

		switch( byClassID )
		{
		case SLCLASS_AVATAR:
			{
				CNtlSobAvatarAttr* pAvatarAttr = reinterpret_cast<CNtlSobAvatarAttr*>( pSob[i] );
				byLevel[i] = pAvatarAttr->GetLevel();
				break;
			}
		case SLCLASS_PLAYER:
			{
				CNtlSobPlayerAttr* pPlayerAttr = reinterpret_cast<CNtlSobPlayerAttr*>( pSob[i] );
				byLevel[i] = pPlayerAttr->GetLevel();
				break;
			}
		case SLCLASS_NPC:
			{
				CNtlSobNpcAttr* pNPCAttr = reinterpret_cast<CNtlSobNpcAttr*>( pSob[i] );
				byLevel[i] = pNPCAttr->GetLevel();
				break;
			}
		case SLCLASS_MONSTER:
			{
				CNtlSobMonsterAttr* pMonsterAttr = reinterpret_cast<CNtlSobMonsterAttr*>( pSob[i] );
				byLevel[i] = pMonsterAttr->GetLevel();
				break;
			}
		case SLCLASS_PET:
			{
				CNtlSobPetAttr* pPetAttr = reinterpret_cast<CNtlSobPetAttr*>( pSob[i] );
				byLevel[i] = pPetAttr->GetLevel();
				break;
			}
		default:
			return CGT_SIMILAR;
		}		
	}


	if( byLevel[0] + 3 < byLevel[1] )
	{
		return CGT_STRONG_1;
	}
	else if( byLevel[0] > byLevel[1] )
	{
		return CGT_WEAK_1;
	}

	return CGT_SIMILAR;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	다이얼로그 관련
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Logic_LocateDialog_in_CleintRect(CNtlPLGui* pGui, RwBool bForce /* = FALSE */)
{
	if( !bForce && GetDialogManager()->IsDialogMovable() == FALSE )
		return;

	RwInt32 iNewX, iNewY;
	gui::CDialog* pDialog = pGui->GetDialog();
	CRectangle rtScreen = pDialog->GetScreenRect();
	RwInt32 iCleintWidth = GetDboGlobal()->GetScreenWidth();
	RwInt32 iCleintHeight = GetDboGlobal()->GetScreenHeight();

	if( (rtScreen.right - 70) >= iCleintWidth )
	{
		iNewX = NtlRandomNumber(iCleintWidth - 300, iCleintWidth - 200);
	}
	else
	{
		iNewX = rtScreen.left;
	}

	if( (rtScreen.bottom - 70) >= iCleintHeight )
	{
		iNewY = NtlRandomNumber(iCleintHeight - 300, iCleintHeight - 200);
	}
	else
	{
		iNewY = rtScreen.top;
	}

	if( iNewX != rtScreen.left || iNewY != rtScreen.top )
		pGui->SetPosition(iNewX, iNewY);
}

//////////////////////////////////////////////////////////////////////////
// 호이포이 믹스
//////////////////////////////////////////////////////////////////////////

/**
* \brief 호이포이 믹스 캡슐 시스템의 레시피가 제조가 가능한지 여부를 반환
* \param idxRecipeTable	(TBLIDX) 레시피 테이블 인덱스
* \returns 가능여부
*/
RwBool Logic_IsEnableCreateRecipe( TBLIDX idxRecipeTable )
{
	// 레시피 테이블 데이타 읽어오기
	sITEM_RECIPE_TBLDAT* pRecipeTblDat = (sITEM_RECIPE_TBLDAT*)API_GetTableContainer()->GetItemRecipeTable()->FindData( idxRecipeTable );
	if( pRecipeTblDat == NULL )
	{
		NTL_ASSERTFAIL( "CRecipeDlgItem::Create - Out of item recipe table index" );
		return FALSE;
	}

	// 현재의 재료를 아바타가 가지고 있는지 확인
	for( RwInt32 i=0; i < DBO_MAX_COUNT_RECIPE_MATERIAL_ITEM; ++i )
	{
		if( pRecipeTblDat->asMaterial[i].materialTblidx == INVALID_TBLIDX )
			continue;

		// 재료가 다 있는지 확인하고 하나라도 없으면 불가능
		if( pRecipeTblDat->asMaterial[i].byMaterialCount > Logic_FindInventoryItemCountByTableId( pRecipeTblDat->asMaterial[i].materialTblidx ) )
			return FALSE;
	}

	return FALSE;
}

/**
* \brief 호이포이 믹스 캡슐 시스템의 레시피가 현재 몇개까지 제조가 가능한지 여부를 판단
* \param idxRecipeTable (TBLIDX) 레시피 테이블의 인덱스
* \returns 갯수
*/
RwInt32 Logic_AmountEnableCreateRecipe( TBLIDX idxRecipeTable )
{
	// 레시피 테이블 데이타 읽어오기
	sITEM_RECIPE_TBLDAT* pRecipeTblDat = (sITEM_RECIPE_TBLDAT*)API_GetTableContainer()->GetItemRecipeTable()->FindData( idxRecipeTable );
	if( pRecipeTblDat == NULL )
	{
		NTL_ASSERTFAIL( "CRecipeDlgItem::Create - Out of item recipe table index" );
		return FALSE;
	}

	// 현재의 재료를 아바타가 가지고 있는지 확인

	// 가진 갯수 / 필요갯수 = 만들 수 있는 갯수
	// 그 중에 가장 작은 작은 갯수가 만들 수 있는 갯수임

	RwReal fMin = 999.0f;
	for( RwInt32 i=0; i < DBO_MAX_COUNT_RECIPE_MATERIAL_ITEM; ++i )
	{
		if( pRecipeTblDat->asMaterial[i].materialTblidx == INVALID_TBLIDX )
			continue;

		RwReal fRequired = (RwReal)pRecipeTblDat->asMaterial[i].byMaterialCount;
		RwReal fHaved = (RwReal)Logic_FindInventoryItemCountByTableId( pRecipeTblDat->asMaterial[i].materialTblidx );

		// 0 으로 나누지 못한다.
		if( fRequired == 0.f || fHaved == 0.f )
			return 0;

		RwReal fEnableAmount = fHaved / fRequired;

		if( fMin > fEnableAmount )
			fMin = fEnableAmount;
	}

	// 999보다 더 클 수는 없다.
	if( fMin >= 999.f )
		return 0;

	return (RwInt32)fMin;

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 부활메시지 박스 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Logic_ShowRegenBox(void)
{
	CNtlWorldConceptController* pWorldConcept = GetNtlWorldConcept()->FindGradeExtController(WORLD_CONCEPT_FIRST_GRADE);
	if( pWorldConcept )
	{
		EWorldPlayConcept eWorldConceptType = pWorldConcept->GetConceptType();

		switch( eWorldConceptType )
		{
		case WORLD_PLAY_TLQ:
			GetAlarmManager()->AlarmMessage( DST_REGEN_PLAYER_TLQ );
			break;
		case WORLD_PLAY_TIME_MACHINE:
			GetAlarmManager()->AlarmMessage( DST_REGEN_PLAYER_TMQ );
			break;
		case WORLD_PLAY_PARTY_DUNGEON:
			GetAlarmManager()->AlarmMessage( DST_REGEN_PLAYER_PARTY_DUNGEON );
			break;
		case WORLD_PLAY_DOJO_SCRAMBLE:
			GetAlarmManager()->AlarmMessage( DST_REGEN_PLAYER_DOJO );
			break;
		}
	}	
	else
	{
		GetAlarmManager()->AlarmMessage( DST_REGEN_PLAYER_MAINWORLD );
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 동영상 관련
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Logic_SetOptionMoviePlay()
{
	CNtlSoundEventGenerator::SendEventMusic(EVENT_MUSIC_VOLUME_DOWN_BECAUSE_MOVIE_PLAY);
	CInputHandler::GetInstance()->SetActive(FALSE);
	GetCursorManager()->ShowMouseCursor(FALSE);

	// 동영상을 플레이 할 때는 PostEffectCamera의 Filter를 NONE으로 한다.
	// 코드 순서에 주의한다.
	CNtlPostEffectCamera::SetPostEffectFilters(POST_EFFECT_FILTER_NONE);
	CDboApplication::GetInstance()->SetRenderEnable(FALSE);
}

void Logic_SetOptionMovieStop()
{
	CNtlSoundEventGenerator::SendEventMusic(EVENT_MUSIC_VOLUME_UP_BECAUSE_MOVIE_PLAY);
	CInputHandler::GetInstance()->SetActive(TRUE);
	GetCursorManager()->ShowMouseCursor(TRUE);

	// HDR Option을 읽어와서 복원한다.
	// 코드 순서에 주의한다
	if( GetNtlStorageManager()->GetBoolData(dSTORAGE_GRAPHIC_SHADER_HDR) )
		CNtlPostEffectCamera::SetPostEffectFilters(POST_EFFECT_FILTER_HDR);
	else
		CNtlPostEffectCamera::SetPostEffectFilters(POST_EFFECT_FILTER_NONE);
	CDboApplication::GetInstance()->SetRenderEnable(TRUE);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 색상 관련
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
RwUInt32 Logic_GetItemRankColor( RwUInt8 byRank )
{
	RwUInt32 uiColor = INFOCOLOR_0;

	switch( byRank )
	{
	case ITEM_RANK_NORMAL:		uiColor = INFOCOLOR_0; break;
	case ITEM_RANK_SUPERIOR:	uiColor = INFOCOLOR_1; break;
	case ITEM_RANK_EXCELLENT:	uiColor = INFOCOLOR_7; break;
	case ITEM_RANK_RARE:		uiColor = INFOCOLOR_5; break;
	case ITEM_RANK_LEGENDARY:	uiColor = INFOCOLOR_8; break;	
	}

	return uiColor;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 보안 관련
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Logic_ProcessSecurityResultCode( RwUInt32 uiNPRetCode, sSEC_RET_DATA* pRetData )
{
	switch ( uiNPRetCode )
	{
	case 110/*NPGAMEMON_ERROR_EXIST*/:				{ GetAlarmManager()->FormattedAlarmMessage( DST_NPGAMEMON_ERROR_EXIST, FALSE, NULL ); return; }
	case 115/*NPGAMEMON_ERROR_GAME_EXIST*/:			{ GetAlarmManager()->FormattedAlarmMessage( DST_NPGAMEMON_ERROR_GAME_EXIST, FALSE, NULL ); return; }
	case 114/*NPGAMEMON_ERROR_INIT*/:				{ GetAlarmManager()->FormattedAlarmMessage( DST_NPGAMEMON_ERROR_INIT, FALSE, NULL ); return; }
	case 124/*NPGAMEMON_ERROR_AUTH_GAMEGUARD*/:		{ GetAlarmManager()->FormattedAlarmMessage( DST_NPGAMEMON_ERROR_NO_CONFIG_FILE, FALSE, NULL ); return; }
	case 153/*NPGAMEMON_ERROR_NFOUND_GG*/:			{ GetAlarmManager()->FormattedAlarmMessage( DST_NPGAMEMON_ERROR_NO_CONFIG_FILE, FALSE, NULL ); return; }
	case 120/*NPGAMEMON_ERROR_AUTH_INI*/:			{ GetAlarmManager()->FormattedAlarmMessage( DST_NPGAMEMON_ERROR_NO_CONFIG_FILE, FALSE, NULL ); return; }
	case 150/*NPGAMEMON_ERROR_NFOUND_INI*/:			{ GetAlarmManager()->FormattedAlarmMessage( DST_NPGAMEMON_ERROR_NO_CONFIG_FILE, FALSE, NULL ); return; }
	case 155/*NPGAMEMON_ERROR_CRYPTOAPI*/:			{ GetAlarmManager()->FormattedAlarmMessage( DST_NPGAMEMON_ERROR_CRYPTOAPI, FALSE, NULL ); return; }
	case 170/*NPGAMEMON_ERROR_EXECUTE*/:			{ GetAlarmManager()->FormattedAlarmMessage( DST_NPGAMEMON_ERROR_EXECUTE, FALSE, NULL ); return; }
	case 200/*NPGAMEMON_ERROR_ILLEGAL_PRG*/:		{ GetAlarmManager()->FormattedAlarmMessage( DST_NPGAMEMON_ERROR_ILLEGAL_PRG, FALSE, NULL ); return; }
	case 350/*NPGMUP_ERROR_ABORT*/:					{ GetAlarmManager()->FormattedAlarmMessage( DST_NPGMUP_ERROR_ABORT, FALSE, NULL ); return; }
	case 380/*NPGMUP_ERROR_CONNECT*/:				{ GetAlarmManager()->FormattedAlarmMessage( DST_NPGMUP_ERROR_CONNECT, FALSE, NULL ); return; }
	case 230/*NPGAMEMON_ERROR_GAMEGUARD*/:			{ GetAlarmManager()->FormattedAlarmMessage( DST_NPGAMEMON_ERROR_GAMEGUARD, FALSE, NULL ); return; }
	case 320/*NPGMUP_ERROR_PARAM*/:					{ GetAlarmManager()->FormattedAlarmMessage( DST_NPGMUP_ERROR_PARAM, FALSE, NULL ); return; }
	case 330/*NPGMUP_ERROR_INIT*/:					{ GetAlarmManager()->FormattedAlarmMessage( DST_NPGMUP_ERROR_INIT, FALSE, NULL ); return; }
	case 340/*NPGMUP_ERROR_DOWNCFG*/:				{ GetAlarmManager()->FormattedAlarmMessage( DST_NPGMUP_ERROR_DOWNCFG, FALSE, NULL ); return; }
	case 360/*NPGMUP_ERROR_AUTH*/:					{ GetAlarmManager()->FormattedAlarmMessage( DST_NPGMUP_ERROR_AUTH, FALSE, NULL ); return; }
	case 112/*NPGAMEMON_ERROR_NPSCAN*/:				{ GetAlarmManager()->FormattedAlarmMessage( DST_NPGAMEMON_ERROR_NPSCAN, FALSE, NULL ); return; }
	case 1001/*NPGAMEMON_COMM_ERROR*/:				{ GetAlarmManager()->FormattedAlarmMessage( DST_NPGAMEMON_COMM_ERROR, FALSE, NULL ); return; }
	case 1002/*NPGAMEMON_COMM_CLOSE*/:				{ GetAlarmManager()->FormattedAlarmMessage( DST_NPGAMEMON_COMM_CLOSE, FALSE, NULL ); return; }
	case 1014/*NPGAMEMON_INIT_ERROR*/:				{ GetAlarmManager()->FormattedAlarmMessage( DST_NPGAMEMON_INIT_ERROR, FALSE, NULL, (pRetData->pData && pRetData->dwDataSize == sizeof(DWORD)) ? *(DWORD*)pRetData->pData : 0 ); return; }
	case 1011/*NPGAMEMON_SPEEDHACK*/:				{ GetAlarmManager()->FormattedAlarmMessage( DST_NPGAMEMON_SPEEDHACK, FALSE, NULL ); return; }
	case 1012/*NPGAMEMON_GAMEHACK_KILLED*/:			{ GetAlarmManager()->FormattedAlarmMessage( DST_NPGAMEMON_GAMEHACK_KILLED, FALSE, NULL ); return; }
	case 1013/*NPGAMEMON_GAMEHACK_DETECT*/:			{ GetAlarmManager()->FormattedAlarmMessage( DST_NPGAMEMON_GAMEHACK_DETECT, FALSE, NULL ); return; }
	case 1015/*NPGAMEMON_GAMEHACK_DOUBT*/:			{ GetAlarmManager()->FormattedAlarmMessage( DST_NPGAMEMON_GAMEHACK_DOUBT, FALSE, NULL ); return; }

	case 1018/*NPGAMEMON_GAMEHACK_REPORT*/:
		{
			if ( pRetData && pRetData->dwDataSize > 0 && pRetData->pData )
			{
				GetDboGlobal()->GetGamePacketGenerator()->SendGameGuardAuthAnswerRes( (RwUInt8)pRetData->dwDataSize, (BYTE*)pRetData->pData );
			}

			return;
		}

	case 1017/*NPGAMEMON_CHECK_CSAUTH2*/:
		{
			if ( pRetData && pRetData->dwDataSize > 0 && pRetData->pData )
			{
				GetDboGlobal()->GetGamePacketGenerator()->SendGameGuardAuthAnswerRes((RwUInt8) pRetData->dwDataSize, (BYTE*)pRetData->pData );
			}

			return;
		}

	default:										{ GetAlarmManager()->FormattedAlarmMessage( DST_NPGAMEGUARD_ERROR_ETC, FALSE, NULL ); return; }
	}
}