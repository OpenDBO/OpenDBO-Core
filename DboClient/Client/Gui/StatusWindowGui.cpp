#include "precomp_dboclient.h"

// shared
#include "NtlItem.h"
#include "NtlResultCode.h"
#include "MobTable.h"

// core
#include "ceventhandler.h"
#include "NtlDebug.h"

// presentation
#include "NtlPLDef.h"
#include "NtlPLGui.h"
#include "NtlPLGuiManager.h"
#include "gui_renderer.h"
#include "NtlPLCharacter.h"
#include "NtlPLEvent.h"

// Table
#include "ItemTable.h"

// simulation
#include "NtlSLGlobal.h"
#include "NtlSLDef.h"
#include "NtlSobManager.h"
#include "NtlSobAvatar.h"
#include "NtlSobAvatarAttr.h"
#include "NtlSobItem.h"
#include "NtlSobItemAttr.h"
#include "NtlSobIcon.h"
#include "NtlSLEvent.h"
#include "NtlSobCharProxy.h"
#include "NtlSLEventFunc.h"
#include "NtlSobMonster.h"
#include "NtlSobMonsterAttr.h"

#include "NtlStorageManager.h"
#include "NtlStorageGroupScouter.h"

// DBO
#include "DboGlobal.h"
#include "DboEvent.h"
#include "DboEventGenerator.h"
#include "DboLogic.h"
#include "SurfaceGui.h"
#include "IconMoveManager.h"
#include "PopupManager.h"
#include "ChatGui.h"
#include "DialogManager.h"
#include "DisplayStringDef.h"
#include "DisplayStringManager.h"
#include "AlarmManager.h"
#include "GuiFunctor.h"

// Self
#include "StatusWindowGui.h"

////////////////////////////////////////////////////////////////////////////////
// ! Define
#define MAX_STACK_NUM_WIDTH		20
#define MAX_STACK_NUM_HEIGHT	20

#define YELLOW_TEXT_COLOR	RGB( 0xFF, 0xD1, 0x44 )
#define BLUE_TEXT_COLOR		RGB( 0x9B, 0xE2, 0xFF )

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ! AvatarTab
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// ! Construction & Destruction

CStatusAvatarTab::CStatusAvatarTab(VOID)
{
	m_pSelf = NULL;

	m_nShowPickedUp = -1;

	m_nLSelectedSlotIdx = -1;
	m_nRSelectedSlotIdx = -1;
	m_nMouseOnIndex = -1;
	m_nPushDownIndex = -1;	
	m_nDragRotX = 0;

	memset( m_arrFocusEffect, 0, sizeof( m_arrFocusEffect ) );
	
	m_sBattleAttr.eBattleAttrInfoType = stINFOWND_BATTLEATTR::TYPE_ATTR_INVALID;
	m_sBattleAttr.bySourceWeaponAttr = INVALID_BYTE;
	m_sBattleAttr.bySourceArmorAttr = INVALID_BYTE;
	m_sBattleAttr.byTargetWeaponAttr = INVALID_BYTE;
	m_sBattleAttr.byTargetArmorAttr = INVALID_BYTE;
}


CStatusAvatarTab::~CStatusAvatarTab(VOID)
{

}

////////////////////////////////////////////////////////////////////////////////
// ! Operation
RwBool CStatusAvatarTab::IsShow(VOID)
{
	if( !m_pSelf->IsVisible() )
		return FALSE;

	if( !m_pSelf->GetParent()->IsVisible() )
		return FALSE;

	return TRUE;
}

VOID CStatusHonorTab::Init()
{
	m_bExpandHonor = FALSE;
	m_bExpandRankBattle = FALSE;
}

RwBool CStatusAvatarTab::Create( CNtlPLGui* pParent )
{
	m_pSelf = reinterpret_cast<gui::CDialog*>( pParent->GetComponent( "dlgAvatar" ) );

	m_pstbBattleAttrTitle = reinterpret_cast<gui::CStaticBox*>( pParent->GetComponent( "sttAttrTitle" ) );

	for( RwInt32 i = 0 ; i < NUM_BASIC_STAT ; ++i )
	{
		const char* szFilename[] = { "sttSTR", "sttDEX", "sttCON", "sttENG", "sttSOL", "sttFOC" };
		char buf[256]; 

		sprintf_s( buf, 256, "%s%s", szFilename[i], "Title" );
		
		m_pstbBasicStat[i] = reinterpret_cast<gui::CStaticBox*>( pParent->GetComponent( szFilename[i] ) );
		m_pstbBasicStatTitle[i] = reinterpret_cast<gui::CStaticBox*>( pParent->GetComponent( buf ) );
	}

	for( RwInt32 i = 0 ; i < NUM_PCINFO ; ++i )
	{
		const char* szFilename[] = { "sttName", "sttClass", "sttLevel", "sttAP" };
		char buf[256]; 

		sprintf_s( buf, 256, "%s%s", szFilename[i], "Title" );

		m_pstbPCInfo[i] = reinterpret_cast<gui::CStaticBox*>( pParent->GetComponent( szFilename[i] ) );
		m_pstbPCInfoTitle[i] = reinterpret_cast<gui::CStaticBox*>( pParent->GetComponent( buf ) );
	}

	for( RwInt32 i = 0 ; i < NUM_STAT ; ++i )
	{
		const char* szFilename[] = { "sttLP", "sttEP", "sttEXP_Cur", "sttEXP_Max" };
		
		m_pstbStat[i] = reinterpret_cast<gui::CStaticBox*>( pParent->GetComponent( szFilename[i] ) );
	}

	for( RwInt32 i = 0 ; i < NUM_COMBAT_STAT ; ++i ) 
	{
		const char* szFilename[] = { "stt%sAttack", "stt%sDefence", "stt%sCritical" };
		char buf[256]; 
		char buftitle[256];

		sprintf_s( buf, 256, szFilename[i], "Physical" );
		sprintf_s( buftitle, 256, "%s%s", buf, "Title" );

		m_pstbPhysicalCombatStat[i] = reinterpret_cast<gui::CStaticBox*>( pParent->GetComponent( buf ) );
		m_pstbPhysicalCombatStatTitle[i] = reinterpret_cast<gui::CStaticBox*>( pParent->GetComponent( buftitle ) );

		sprintf_s( buf, 256, szFilename[i], "Energy" );
		sprintf_s( buftitle, 256, "%s%s", buf, "Title" );

		m_pstbEnergyCombatStat[i] = reinterpret_cast<gui::CStaticBox*>( pParent->GetComponent( buf ) );
		m_pstbEnergyCombatStatTitle[i] = reinterpret_cast<gui::CStaticBox*>( pParent->GetComponent( buftitle ) );
	}

	for( RwInt32 i = 0 ; i < NUM_ETC_STAT ; ++i )
	{
		const char* szFilename[] = { "sttAttackRate", "sttDodgeRate" };
		char buf[256];

		sprintf_s( buf, 256, "%s%s", szFilename[i], "Title" );

		m_pstbETCStat[i] = reinterpret_cast<gui::CStaticBox*>( pParent->GetComponent( szFilename[i] ) );
		m_pstbETCStatTitle[i] = reinterpret_cast<gui::CStaticBox*>( pParent->GetComponent( buf ) );
	}

	m_pbtnLeftRot = reinterpret_cast<gui::CButton*>( pParent->GetComponent( "btnLeftSpin" ) );
	m_pbtnRightRot = reinterpret_cast<gui::CButton*>( pParent->GetComponent( "btnRightSpin" ) );
	m_pbtnDragRot = reinterpret_cast<gui::CButton*>( pParent->GetComponent( "btnCharDrag" ) );

	m_ppnlBattleAttribute[TYPE_ARMOR] = reinterpret_cast<gui::CPanel*>( pParent->GetComponent( "pnlDefenceAttr" ) );
	m_ppnlBattleAttribute[TYPE_WEAPON] = reinterpret_cast<gui::CPanel*>( pParent->GetComponent( "pnlOffenceAttr" ) );
	
	m_slotMouseDown = m_pSelf->SigMouseDown().Connect( this, &CStatusAvatarTab::OnMouseDown );
	m_slotMouseUp = m_pSelf->SigMouseUp().Connect( this, &CStatusAvatarTab::OnMouseUp );
	m_slotMouseMove = m_pSelf->SigMouseMove().Connect( this, &CStatusAvatarTab::OnMouseMove );
	m_slotMove = m_pSelf->SigMove().Connect( this, &CStatusAvatarTab::OnMove );
	m_slotMouseLeave = m_pSelf->SigMouseLeave().Connect( this, &CStatusAvatarTab::OnMouseLeave );
	m_slotPaint = m_pSelf->SigPaint().Connect( this, &CStatusAvatarTab::OnPaint );
	m_slotSlotPaint = m_pstbBattleAttrTitle->SigPaint().Connect( this, &CStatusAvatarTab::OnSlotPaint );

	m_slotLeftRotBtnPress = m_pbtnLeftRot->SigPressed().Connect( this, &CStatusAvatarTab::OnLeftRotBtnPress );
	m_slotLeftRotBtnRelease = m_pbtnLeftRot->SigReleased().Connect( this, &CStatusAvatarTab::OnLeftRotBtnRelease );
	m_slotRightRotBtnPress = m_pbtnRightRot->SigPressed().Connect( this, &CStatusAvatarTab::OnRightRotBtnPress );
	m_slotRightRotBtnRelease = m_pbtnRightRot->SigReleased().Connect( this, &CStatusAvatarTab::OnRightRotBtnRelease );
	m_slotDragRotBtnPress = m_pbtnDragRot->SigPressed().Connect( this, &CStatusAvatarTab::OnDragRotBtnPress );
	m_slotDragRotBtnRelease = m_pbtnDragRot->SigReleased().Connect( this, &CStatusAvatarTab::OnDragRotBtnRelease );
	m_slotDragRotMouseMove = m_pbtnDragRot->SigMouseMove().Connect( this, &CStatusAvatarTab::OnDragRotMouseMove );

	m_slotWeaponMouseEnter = m_ppnlBattleAttribute[TYPE_WEAPON]->SigMouseEnter().Connect( this, &CStatusAvatarTab::OnMouseWeaponAttrEnter );
	m_slotWeaponMouseLeave = m_ppnlBattleAttribute[TYPE_WEAPON]->SigMouseLeave().Connect( this, &CStatusAvatarTab::OnMouseWeaponAttrLeave );
	m_slotArmorMouseEnter = m_ppnlBattleAttribute[TYPE_ARMOR]->SigMouseEnter().Connect( this, &CStatusAvatarTab::OnMouseArmorAttrEnter );
	m_slotArmorMouseLeave = m_ppnlBattleAttribute[TYPE_ARMOR]->SigMouseLeave().Connect( this, &CStatusAvatarTab::OnMouseArmorAttrLeave );

	// Equip Slot CoordSet
	SetSlotRectHardCode();
	SetTextHardCode();

	// Tooltip
	SetAttributeToolTip();

	return TRUE;
}

VOID CStatusAvatarTab::Destroy(VOID)
{

}

VOID CStatusAvatarTab::UpdateBeforeCamera( RwReal fElapsed )
{
	if( !IsShow() )
		return;

	CNtlSobCharProxy* pCharProxy = reinterpret_cast<CNtlSobCharProxy*>( GetNtlSLGlobal()->GetSobAvatar()->GetSobProxy() );

	m_texCharacter.Load( pCharProxy->UIPcStatusWndRender() );
	m_surCharacter.SetTexture( &m_texCharacter );
}

VOID CStatusAvatarTab::HandleEvents( RWS::CMsg& msg )
{
	// Equip 정보
	if( msg.Id == g_EventSobInfoUpdate )
	{
		if( !IsShow() )
			return;

		SNtlEventSobInfoUpdate* pUpdate = reinterpret_cast<SNtlEventSobInfoUpdate*>( msg.pData );

		if( pUpdate->hSerialId != GetNtlSLGlobal()->GetSobAvatar()->GetSerialID() )
			return;
				
		if( pUpdate->uiUpdateType & EVENT_AIUT_ITEM )
		{
			UpdateEquipData();
		}
		if( pUpdate->uiUpdateType & EVENT_AIUT_ATTR )
		{
			UpdateStatData();
			
			// 아바타의 Attribute ( Battle Attribute bonus ) 가 업데이트 되면 다시 계산해준다.
			CalcBattleAttribute();
			OnBattleAttributeRefresh();
		}
	}
	else if( msg.Id == g_EventSobConvertClass )
	{// 클래스 변경은 이것밖에 안날라온다.
		SNtlEventSobConvertClass* pData = reinterpret_cast<SNtlEventSobConvertClass*>( msg.pData );

		if( pData->hSerialId == GetNtlSLGlobal()->GetSobAvatar()->GetSerialID() )
		{
			m_pstbPCInfo[CLASS]->SetText( Logic_GetClassName( pData->byClass ) );
		}
	}
	else if( msg.Id == g_EventIconMoveClick )
	{
		ShowIconDestination();
	}
	//else if( msg.Id == g_EventIconPopupResult )
	//{
	//	SDboEventIconPopupResult* pData = reinterpret_cast<SDboEventIconPopupResult*>( msg.pData );

	//	if( pData->nSrcPlace != PLACE_EQUIP )
	//		return;

	//	if( pData->nWorkId == PMW_VIEW )
	//	{
	//		GetDialogManager()->OpenDialog(DIALOG_SCOUTER_INFO);
	//		GetDialogManager()->OpenDialog(DIALOG_SCOUTER_BODY);
	//	}
	//	else if( pData->nWorkId == PMW_PICKUP )
	//	{
	//		if( IsEnableEquipPickUp( pData->nSrcSlotIdx ) )
	//		{
	//			CNtlSobItem* pItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( pData->uiSerial ) );
	//			CNtlSobItemAttr* pItemAttr = reinterpret_cast<CNtlSobItemAttr*>( pItem->GetSobAttr() );
	//			GetIconMoveManager()->IconMovePickUp( pData->uiSerial, PLACE_EQUIP, pData->nSrcSlotIdx, pItemAttr->GetStackNum(), pItem->GetIcon()->GetImage() );
	//		}
	//	}		
	//}
	else if( msg.Id == g_EventPickedUpHide )
	{
		RwInt32 nSlotIdx = (RwInt32)msg.pData;
		if( nSlotIdx != PLACE_EQUIP )
			return;

		ShowPickedUp( FALSE );
	}
	else if( msg.Id == g_EventEnableItemIcon )
	{
		SDboEventEnableItemIcon* pData = reinterpret_cast<SDboEventEnableItemIcon*>( msg.pData );

		if( pData->ePlace == PLACE_EQUIP )
			ShowDisableSlot( !pData->bEnable, pData->uiSlotIdx );
		else if( pData->ePlace == PLACE_SCOUTER_SLOT )
			ShowDisableSlot( !pData->bEnable, EQUIP_SLOT_TYPE_SCOUTER );
	}
	else if(msg.Id == g_EventSubWeaponActive )
	{
		SNtlEventSubWeaponActive* pActiveData = reinterpret_cast<SNtlEventSubWeaponActive*>( msg.pData );

		if( GetNtlSLGlobal()->GetSobAvatar()->GetSerialID() == pActiveData->uiSerialId ||
			GetDboGlobal()->GetTargetSerial() == pActiveData->uiSerialId )
		{
			CalcBattleAttribute();
			OnBattleAttributeRefresh();
		}
	}
	else if(msg.Id == g_EventSubWeaponDeActive )
	{
		SNtlEventSubWeaponDeActive* pActiveData = reinterpret_cast<SNtlEventSubWeaponDeActive*>( msg.pData );

		if( GetNtlSLGlobal()->GetSobAvatar()->GetSerialID() == pActiveData->uiSerialId ||
			GetDboGlobal()->GetTargetSerial() == pActiveData->uiSerialId )
		{
			CalcBattleAttribute();
			OnBattleAttributeRefresh();
		}
	}
	else if(msg.Id == g_EventSobEquipChange )
	{
		SNtlEventSobEquipChange* pEquipChange = reinterpret_cast<SNtlEventSobEquipChange*>( msg.pData );

		if( GetNtlSLGlobal()->GetSobAvatar()->GetSerialID() == pEquipChange->hSerialId ||
			GetDboGlobal()->GetTargetSerial() == pEquipChange->hSerialId )
		{
			CalcBattleAttribute();
			OnBattleAttributeRefresh();
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
// ! Implements

VOID CStatusAvatarTab::UpdateEquipData(VOID)
{
	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();

	if( !pAvatar )
		return;				// 초기 아바타 생성전에 들어오는 경우.

	CNtlInventory* pInventory = pAvatar->GetInventory();

	for( RwInt32 i = 0 ; i < NTL_MAX_EQUIP_ITEM_SLOT ; ++i )
	{
		SERIAL_HANDLE hEquipSerial = pInventory->GetEquipItem( i );

		if( hEquipSerial == INVALID_SERIAL_ID )
		{
			if( i == EQUIP_SLOT_TYPE_SCOUTER )
			{
				if( m_surIcon[i].GetTexture() )
					Logic_SendTutorialCondition(ETL_CONDITION_TYPE_STATUS_SCOUTER_SLOT_UNREG);
			}

			ShowDisableSlot( FALSE, i );
			m_surIcon[i].SetTexture( (gui::CTexture*)NULL );
			FritzEffect( FRITZ_NONE, i );			
		}
		else
		{
			if( i == EQUIP_SLOT_TYPE_SCOUTER )
			{
				if( !m_surIcon[i].GetTexture() )
					Logic_SendTutorialCondition(ETL_CONDITION_TYPE_STATUS_SCOUTER_SLOT_REG);
			}

			CNtlSobItem* pItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( hEquipSerial ) );
			CNtlSobItemAttr* pItemAttr = reinterpret_cast<CNtlSobItemAttr*>( pItem->GetSobAttr() );
			
			m_surIcon[i].SetTexture( (gui::CTexture*)pItem->GetIcon()->GetImage() );

			if( pItemAttr->IsExpired() || pItemAttr->GetDur() == 0 )
				FritzEffect( FRITZ_ABSOLUTE, i );
			else if( (RwReal)pItemAttr->GetDur() / (RwReal)pItemAttr->GetMaxDur() <= ITEM_DURATION_WARNING )
				FritzEffect( FRITZ_HALF, i );
			else
				FritzEffect( FRITZ_NONE, i );			
		}
	}

	if( m_nMouseOnIndex >= 0 && GetInfoWndManager()->GetRequestGui() == DIALOG_STATUS )
	{
		CRectangle rtScreen = m_pSelf->GetScreenRect();		

		if( pInventory->GetEquipItem( m_nMouseOnIndex ) == INVALID_SERIAL_ID )
		{
			GetInfoWndManager()->ShowInfoWindow( TRUE, CInfoWndManager::INFOWND_JUST_WTEXT, m_rtEquipSlot[m_nMouseOnIndex].left + rtScreen.left, m_rtEquipSlot[m_nMouseOnIndex].top + rtScreen.top, (VOID*)GetEquipSlotText( m_nMouseOnIndex ), DIALOG_STATUS );						
		}
		else
		{
			CNtlSobItem* pItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( pInventory->GetEquipItem( m_nMouseOnIndex ) ) );
			GetInfoWndManager()->ShowInfoWindow( TRUE, CInfoWndManager::INFOWND_ITEM, m_rtEquipSlot[m_nMouseOnIndex].left + rtScreen.left, m_rtEquipSlot[m_nMouseOnIndex].top + rtScreen.top, pItem, DIALOG_STATUS );
		}
	}
}

VOID CStatusAvatarTab::UpdateStatData(VOID)
{
	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();

	if( !pAvatar )
		return;				// 초기 아바타 생성전에 들어오는 경우.

	CNtlSobAvatarAttr* pAvatarAttr = reinterpret_cast<CNtlSobAvatarAttr*>( pAvatar->GetSobAttr() );
	RwUInt32 uiColor;

	// BasicStat
	uiColor = GetGuiFuntor()->ColorDecision( pAvatarAttr->m_byBaseStr, pAvatarAttr->m_byLastStr );
	m_pstbBasicStat[STR]->SetTextColor( uiColor );
	m_pstbBasicStat[STR]->SetText( pAvatarAttr->m_byLastStr );
	uiColor = GetGuiFuntor()->ColorDecision( pAvatarAttr->m_byBaseDex, pAvatarAttr->m_byLastDex );
	m_pstbBasicStat[DEX]->SetTextColor( uiColor );
	m_pstbBasicStat[DEX]->SetText( pAvatarAttr->m_byLastDex );
	uiColor = GetGuiFuntor()->ColorDecision( pAvatarAttr->m_byBaseCon, pAvatarAttr->m_byLastCon );
	m_pstbBasicStat[CON]->SetTextColor( uiColor );
	m_pstbBasicStat[CON]->SetText( pAvatarAttr->m_byLastCon );
	uiColor = GetGuiFuntor()->ColorDecision( pAvatarAttr->m_byBaseEng, pAvatarAttr->m_byLastEng );
	m_pstbBasicStat[ENG]->SetTextColor( uiColor );
	m_pstbBasicStat[ENG]->SetText( pAvatarAttr->m_byLastEng );
	uiColor = GetGuiFuntor()->ColorDecision( pAvatarAttr->m_byBaseSol, pAvatarAttr->m_byLastSol );
	m_pstbBasicStat[SOL]->SetTextColor( uiColor );
	m_pstbBasicStat[SOL]->SetText( pAvatarAttr->m_byLastSol );
	uiColor = GetGuiFuntor()->ColorDecision( pAvatarAttr->m_byBaseFoc, pAvatarAttr->m_byLastFoc );
	m_pstbBasicStat[FOC]->SetTextColor( uiColor );
	m_pstbBasicStat[FOC]->SetText( pAvatarAttr->m_byLastFoc );
	
	// Physical Stat
	uiColor = GetGuiFuntor()->ColorDecision( pAvatarAttr->m_wBasePhyshicalOffence, pAvatarAttr->m_wLastPhyshicalOffence );
	m_pstbPhysicalCombatStat[OFFENCE]->SetTextColor( uiColor );
	m_pstbPhysicalCombatStat[OFFENCE]->SetText( pAvatarAttr->m_wLastPhyshicalOffence );
	uiColor = GetGuiFuntor()->ColorDecision( pAvatarAttr->m_wBasePhyshicalDefence, pAvatarAttr->m_wLastPhyshicalDefence );
	m_pstbPhysicalCombatStat[DEFENCE]->SetTextColor( uiColor );
	m_pstbPhysicalCombatStat[DEFENCE]->SetText( pAvatarAttr->m_wLastPhyshicalDefence );
	uiColor = GetGuiFuntor()->ColorDecision( pAvatarAttr->m_wBasePhysicalCriticalRate, pAvatarAttr->m_wLastPhysicalCriticalRate );
	m_pstbPhysicalCombatStat[CRITICAL]->SetTextColor( uiColor );
	m_pstbPhysicalCombatStat[CRITICAL]->SetText( pAvatarAttr->m_wLastPhysicalCriticalRate );
	
	// Energy Stat
	uiColor = GetGuiFuntor()->ColorDecision( pAvatarAttr->m_wBaseEnergyOffence, pAvatarAttr->m_wLastEnergyOffence );
	m_pstbEnergyCombatStat[OFFENCE]->SetTextColor( uiColor );
	m_pstbEnergyCombatStat[OFFENCE]->SetText( pAvatarAttr->m_wLastEnergyOffence );
	uiColor = GetGuiFuntor()->ColorDecision( pAvatarAttr->m_wBaseEnergyDefence, pAvatarAttr->m_wLastEnergyDefence );
	m_pstbEnergyCombatStat[DEFENCE]->SetTextColor( uiColor );
	m_pstbEnergyCombatStat[DEFENCE]->SetText( pAvatarAttr->m_wLastEnergyDefence );
	uiColor = GetGuiFuntor()->ColorDecision( pAvatarAttr->m_wBaseEnergyCriticalRate, pAvatarAttr->m_wLastEnergyCriticalRate );
	m_pstbEnergyCombatStat[CRITICAL]->SetTextColor( uiColor );
	m_pstbEnergyCombatStat[CRITICAL]->SetText( pAvatarAttr->m_wLastEnergyCriticalRate );
	
	// Etc Stat
	uiColor = GetGuiFuntor()->ColorDecision( pAvatarAttr->m_wBaseAttackRate, pAvatarAttr->m_wLastAttackRate );
	m_pstbETCStat[ATTACKRATE]->SetTextColor( uiColor );
	m_pstbETCStat[ATTACKRATE]->SetText( pAvatarAttr->m_wLastAttackRate );
	uiColor = GetGuiFuntor()->ColorDecision( pAvatarAttr->m_wBaseDodgeRate, pAvatarAttr->m_wLastDodgeRate );
	m_pstbETCStat[DODGE]->SetTextColor( uiColor );
	m_pstbETCStat[DODGE]->SetText( pAvatarAttr->m_wLastDodgeRate );
	
	// Name
	m_pstbPCInfo[NAME]->SetText( pAvatarAttr->GetName() );
	m_pstbPCInfo[LEVEL]->SetText( pAvatarAttr->GetLevel() );
	m_pstbPCInfo[CLASS]->SetText( Logic_GetClassName( pAvatarAttr->GetClass() ) );
	m_pstbPCInfo[AP]->SetText( pAvatarAttr->GetAP() );

	m_pstbStat[LP]->Format( "%d / %d", pAvatarAttr->GetLp(), pAvatarAttr->GetMaxLp() );
	m_pstbStat[EP]->Format( "%d / %d", pAvatarAttr->GetEp(), pAvatarAttr->GetMaxEp() );
	
	if( pAvatarAttr->m_uiMaxExp )
	{
		m_pstbStat[EXP_CUR]->Format( L"%s : %d", GetDisplayStringManager()->GetString( DST_STATUS_CURRENT_EXP ), pAvatarAttr->m_uiExp );
		m_pstbStat[EXP_MAX]->Format( L"%s : %d", GetDisplayStringManager()->GetString( DST_STATUS_GOAL_EXP ), pAvatarAttr->m_uiMaxExp );

		if( m_pstbStat[EXP_CUR]->GetFitWidthToString() > m_pstbStat[EXP_CUR]->GetWidth() )
			m_pstbStat[EXP_CUR]->SetText( pAvatarAttr->m_uiExp );
		if( m_pstbStat[EXP_MAX]->GetFitWidthToString() > m_pstbStat[EXP_MAX]->GetWidth() )
			m_pstbStat[EXP_MAX]->SetText( pAvatarAttr->m_uiMaxExp );
	}	
	else
	{
		m_pstbStat[EXP_CUR]->SetText( "MAX EXP" );
		m_pstbStat[EXP_MAX]->Clear();
	}
}

RwInt32 CStatusAvatarTab::GetEquipSlotIdx( RwInt32 nX, RwInt32 nY )
{
	for( RwInt32 i = 0 ; i < NTL_MAX_EQUIP_ITEM_SLOT ; ++i )
	{
		if( m_rtEquipSlot[i].PtInRect( CPos( nX, nY ) ) )
			return i;
	}

	return -1;
}

CRectangle CStatusAvatarTab::GetEquipSlotRect( RwInt32 nSlotIdx )
{
	if( nSlotIdx < 0 || nSlotIdx >= NTL_MAX_EQUIP_ITEM_SLOT )
		return CRectangle( 0, 0, 0, 0 );

	return m_rtEquipSlot[nSlotIdx];
}

const WCHAR* CStatusAvatarTab::GetEquipSlotText( RwInt32 nSlotIdx )
{
	if( nSlotIdx < EQUIP_SLOT_TYPE_FIRST || nSlotIdx > EQUIP_SLOT_TYPE_LAST )
		return NULL;

	RwInt32 nTextID = 0;

	switch( nSlotIdx )
	{
	case EQUIP_SLOT_TYPE_HAND: nTextID = DST_EQUIP_SLOT_TYPE_HAND; break; 
	case EQUIP_SLOT_TYPE_SUB_WEAPON: nTextID = DST_EQUIP_SLOT_TYPE_SUB_WEAPON ; break;
	case EQUIP_SLOT_TYPE_JACKET: nTextID = DST_EQUIP_SLOT_TYPE_JACKET; break;
	case EQUIP_SLOT_TYPE_PANTS: nTextID = DST_EQUIP_SLOT_TYPE_PANTS	; break;
	case EQUIP_SLOT_TYPE_BOOTS: nTextID = DST_EQUIP_SLOT_TYPE_BOOTS	; break;
	case EQUIP_SLOT_TYPE_SCOUTER: nTextID = DST_EQUIP_SLOT_TYPE_SCOUTER; break;
	case EQUIP_SLOT_TYPE_COSTUME: nTextID = DST_EQUIP_SLOT_TYPE_DOGI	; break;
	case EQUIP_SLOT_TYPE_NECKLACE: nTextID = DST_EQUIP_SLOT_TYPE_NECKLACE; break;
	case EQUIP_SLOT_TYPE_EARRING_1: 
	case EQUIP_SLOT_TYPE_EARRING_2: nTextID = DST_EQUIP_SLOT_TYPE_EARRING; break;
	case EQUIP_SLOT_TYPE_RING_1:  
	case EQUIP_SLOT_TYPE_RING_2: nTextID = DST_EQUIP_SLOT_TYPE_RING	; break;
	}

	return GetDisplayStringManager()->GetString( nTextID );
}

RwBool CStatusAvatarTab::IsEnableEquipPickUp( RwInt32 nSlotIdx )
{
	if( nSlotIdx < 0 )
		return FALSE;

	if( IsDisableSlot( nSlotIdx ) )
		return FALSE;

	if( !GetIconMoveManager()->IsEnable() )
		return FALSE;

	CNtlInventory* pInventory = GetNtlSLGlobal()->GetSobAvatar()->GetInventory();
	SERIAL_HANDLE hSlotSerial = pInventory->GetEquipItem( nSlotIdx );
	CNtlSobItem* pSobItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( hSlotSerial ) );

	if( !pSobItem )
		return FALSE;

	// 자식 아이템이 있다면 옮길 수 없다.(가방이나 스카우터 같은 것)
	if( pSobItem->ExistChild() )
	{
		if( pSobItem->IsScouterItem() )
		{
			// 스카우터인 경우
			CNtlSLEventGenerator::SysMsg( INVALID_SERIAL_ID, GAME_SCOUTER_BODY_IS_NOT_EMPTY );
		}

		return FALSE;
	}

	CNtlSobIcon* pIcon = pSobItem->GetIcon();
	if( !pIcon->IsDragAndDropPossible() )
		return FALSE;

	return TRUE;
}

RwBool CStatusAvatarTab::IsEnableEquipPutDown( RwInt32 nSlotIdx )
{
	if( nSlotIdx < 0 )
		return FALSE;

	if( IsDisableSlot( nSlotIdx ) )
		return FALSE;

	// 나머지 비교는 Logic에 맡긴다. 

	return TRUE;
}

RwBool CStatusAvatarTab::IsEnableUnequipItem( RwInt32 nSlotIdx )
{
	if( nSlotIdx < 0 )
		return FALSE;

	if( IsDisableSlot( nSlotIdx ) )
		return FALSE;

	CNtlInventory* pInventory = GetNtlSLGlobal()->GetSobAvatar()->GetInventory();
	if( pInventory->GetEquipItem( nSlotIdx ) == INVALID_SERIAL_ID )
		return FALSE;
	
	return TRUE;
}

//RwBool CStatusAvatarTab::IsEnableEquipPopup( RwInt32 nSlotIdx )
//{
//	if( nSlotIdx < 0 )
//		return FALSE;
//
//	CNtlInventory* pInventory = GetNtlSLGlobal()->GetSobAvatar()->GetInventory();
//	SERIAL_HANDLE hSlotSerial = pInventory->GetEquipItem( nSlotIdx );
//
//	if( hSlotSerial == INVALID_SERIAL_ID )
//		return FALSE;
//
//	return TRUE;
//}

VOID CStatusAvatarTab::ShowIconDestination(VOID)
{
	if( GetIconMoveManager()->IsActive() )
	{
		if( GetIconMoveManager()->GetSrcPlace() != PLACE_BAG )
			return;

		CNtlSobItem* pSobItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( GetIconMoveManager()->GetSrcSerial() ) );
		if( !pSobItem )
			return;

		CNtlSobItemAttr* pSobAttr = reinterpret_cast<CNtlSobItemAttr*>( pSobItem->GetSobAttr() );

		if( !pSobItem->IsEquipItem() )
			return;


		for( RwUInt8 i = 0 ; i < NTL_MAX_EQUIP_ITEM_SLOT ; ++i )
		{
			if( Logic_ConvertEquipSlotIdxToFlag( i ) & pSobAttr->GetItemTbl()->wEquip_Slot_Type_Bit_Flag )
			{
				m_arrFocusEffect[i] = m_arrFocusEffect[i] | SLOT_FOCUS_CAN_MOVE;
			}
			else
			{
				m_arrFocusEffect[i] = m_arrFocusEffect[i] & ~SLOT_FOCUS_CAN_MOVE;
			}
		}
	}
	else
	{
		for( RwInt32 i = 0 ; i < NTL_MAX_EQUIP_ITEM_SLOT ; ++i )
		{
			m_arrFocusEffect[i] = m_arrFocusEffect[i] & ~SLOT_FOCUS_CAN_MOVE;
		}
	}
}

VOID CStatusAvatarTab::SetSlotRectHardCode(VOID)
{
	CRectangle rtScreen = m_pSelf->GetScreenRect();

	m_rtEquipSlot[EQUIP_SLOT_TYPE_HAND].SetRectWH( 165, 231, NTL_ITEM_ICON_SIZE , NTL_ITEM_ICON_SIZE );
	m_rtEquipSlot[EQUIP_SLOT_TYPE_SUB_WEAPON].SetRectWH( 165, 273, NTL_ITEM_ICON_SIZE , NTL_ITEM_ICON_SIZE );
	m_rtEquipSlot[EQUIP_SLOT_TYPE_JACKET].SetRectWH( 165, 105, NTL_ITEM_ICON_SIZE , NTL_ITEM_ICON_SIZE );
	m_rtEquipSlot[EQUIP_SLOT_TYPE_PANTS].SetRectWH( 165, 147, NTL_ITEM_ICON_SIZE , NTL_ITEM_ICON_SIZE );
	m_rtEquipSlot[EQUIP_SLOT_TYPE_BOOTS].SetRectWH( 165, 189, NTL_ITEM_ICON_SIZE , NTL_ITEM_ICON_SIZE );
	m_rtEquipSlot[EQUIP_SLOT_TYPE_SCOUTER].SetRectWH( 66, 273, NTL_ITEM_ICON_SIZE , NTL_ITEM_ICON_SIZE );
	m_rtEquipSlot[EQUIP_SLOT_TYPE_COSTUME].SetRectWH( 118, 273, NTL_ITEM_ICON_SIZE, NTL_ITEM_ICON_SIZE );
	m_rtEquipSlot[EQUIP_SLOT_TYPE_NECKLACE].SetRectWH( 20, 105, NTL_ITEM_ICON_SIZE , NTL_ITEM_ICON_SIZE );
	m_rtEquipSlot[EQUIP_SLOT_TYPE_EARRING_1].SetRectWH( 20, 147, NTL_ITEM_ICON_SIZE , NTL_ITEM_ICON_SIZE );
	m_rtEquipSlot[EQUIP_SLOT_TYPE_EARRING_2].SetRectWH( 20, 189, NTL_ITEM_ICON_SIZE , NTL_ITEM_ICON_SIZE );
	m_rtEquipSlot[EQUIP_SLOT_TYPE_RING_1].SetRectWH( 20, 231, NTL_ITEM_ICON_SIZE , NTL_ITEM_ICON_SIZE );
	m_rtEquipSlot[EQUIP_SLOT_TYPE_RING_2].SetRectWH( 20, 273, NTL_ITEM_ICON_SIZE , NTL_ITEM_ICON_SIZE );	

	for( RwInt32 i = 0 ; i < NTL_MAX_EQUIP_ITEM_SLOT ; ++i )
	{
		m_surIcon[i].SetRect( rtScreen.left + m_rtEquipSlot[i].left, rtScreen.top + m_rtEquipSlot[i].top,
							  rtScreen.left + m_rtEquipSlot[i].right, rtScreen.top + m_rtEquipSlot[i].bottom );
		m_surFocusSlot[i].SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfSlotFocusEffect" ) );
		m_surFocusSlot[i].SetRect( rtScreen.left + m_rtEquipSlot[i].left, rtScreen.top + m_rtEquipSlot[i].top,
								   rtScreen.left + m_rtEquipSlot[i].right, rtScreen.top + m_rtEquipSlot[i].bottom );
		m_surDisableSlot[i].SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfSlotDisableEffect" ) );
		m_surDisableSlot[i].SetRect( rtScreen.left + m_rtEquipSlot[i].left, rtScreen.top + m_rtEquipSlot[i].top,
									 rtScreen.left + m_rtEquipSlot[i].right, rtScreen.top + m_rtEquipSlot[i].bottom );
		m_surDisableSlot[i].Show( false );
		
		// Default
		m_surFritzSlot[i].Show( false );
	}

	m_surPickedUp.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfSlotGrayedEffect" ) );

	m_surCharacter.SetRectWH( rtScreen.left + 43, rtScreen.top + 96, 132, 176 );
}

VOID CStatusAvatarTab::SetTextHardCode(VOID)
{
	for( RwInt32 i = 0 ; i < NUM_BASIC_STAT ; ++i )
	{
		m_pstbBasicStatTitle[i]->SetText( GetDisplayStringManager()->GetString( DST_STATUS_STAT_STR + i ) );
	}

	for( RwInt32 i = 0 ; i < NUM_COMBAT_STAT ; ++i )
	{
		m_pstbPhysicalCombatStatTitle[i]->SetText( GetDisplayStringManager()->GetString( DST_STATUS_PHYSICAL_ATTACK + i ) );
		m_pstbEnergyCombatStatTitle[i]->SetText( GetDisplayStringManager()->GetString( DST_STATUS_ENERGY_ATTACK + i ) );
	}

	for( RwInt32 i = 0 ; i < NUM_ETC_STAT ; ++i )
	{
		m_pstbETCStatTitle[i]->SetText( GetDisplayStringManager()->GetString( DST_STATUS_ATTACK_RATE + i ) );
	}

	for( RwInt32 i = 0 ; i < NUM_PCINFO ; ++i )
	{
		m_pstbPCInfoTitle[i]->SetText( GetDisplayStringManager()->GetString( DST_STATUS_NAME + i ) );
	}

	m_pstbBattleAttrTitle->SetText( GetDisplayStringManager()->GetString( DST_STATUS_ATTR )	);	
}

VOID CStatusAvatarTab::CalcBattleAttribute(VOID)
{
	CNtlSobAvatar* pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( pSobAvatar == NULL )
		return;

	CNtlSobAvatarAttr* pSobAvatarAttr = reinterpret_cast<CNtlSobAvatarAttr*>( pSobAvatar->GetSobAttr() );
	CNtlSobCharProxy* pAvatarProxy = reinterpret_cast<CNtlSobCharProxy*>( pSobAvatar->GetSobProxy() );

	// 아바타가 서브웨폰 사용중이라면 서브웨폰의 Attribute를 메인 속성으로 삼고 서브웨폰이 없다면
	// 메인웨폰의 Attribute를 메인 속성으로 삼는다.
	RwUInt8 byWeaponAttr = INVALID_BYTE;
	if( pAvatarProxy->GetActiveSubWeapon() )
	{
		byWeaponAttr = pSobAvatarAttr->GetSubWeaponAttr();
		SetBattleAttribute( TYPE_WEAPON, pSobAvatarAttr->GetSubWeaponAttr() );		
	}
	else
	{
		byWeaponAttr = pSobAvatarAttr->GetMainWeaponAttr();
		SetBattleAttribute( TYPE_WEAPON, pSobAvatarAttr->GetMainWeaponAttr() );
	}

	SetBattleAttribute( TYPE_ARMOR, pSobAvatarAttr->GetArmorWeaponAttr() );
	SetSourceAttr( byWeaponAttr, pSobAvatarAttr->GetArmorWeaponAttr() );
	
	// Source의 Bonus는 여기서 세팅. Target의 Bonus는 패킷이 날라올 때 변경한다.
	m_sBattleAttr.afSourceOffenceBonus[BATTLE_ATTRIBUTE_HONEST] = pSobAvatarAttr->m_fHonestOffence;
	m_sBattleAttr.afSourceOffenceBonus[BATTLE_ATTRIBUTE_STRANGE] = pSobAvatarAttr->m_fStrangeOffence;
	m_sBattleAttr.afSourceOffenceBonus[BATTLE_ATTRIBUTE_WILD] = pSobAvatarAttr->m_fWildOffence;
	m_sBattleAttr.afSourceOffenceBonus[BATTLE_ATTRIBUTE_ELEGANCE] = pSobAvatarAttr->m_fStrangeOffence;
	m_sBattleAttr.afSourceOffenceBonus[BATTLE_ATTRIBUTE_FUNNY] = pSobAvatarAttr->m_fFunnyOffence;
	m_sBattleAttr.afSourceDefenceBonus[BATTLE_ATTRIBUTE_HONEST] = pSobAvatarAttr->m_fHonestDefence;
	m_sBattleAttr.afSourceDefenceBonus[BATTLE_ATTRIBUTE_STRANGE] = pSobAvatarAttr->m_fStrangeDefence;
	m_sBattleAttr.afSourceDefenceBonus[BATTLE_ATTRIBUTE_WILD] = pSobAvatarAttr->m_fWildDefence;
	m_sBattleAttr.afSourceDefenceBonus[BATTLE_ATTRIBUTE_ELEGANCE] = pSobAvatarAttr->m_fStrangeDefence;
	m_sBattleAttr.afSourceDefenceBonus[BATTLE_ATTRIBUTE_FUNNY] = pSobAvatarAttr->m_fFunnyDefence;
}

VOID CStatusAvatarTab::SetBattleAttribute( eTYPE_BATTLEATTR eType, RwUInt8 byBattleAttribute )
{
	Logic_SetBattleAttributeMark( m_ppnlBattleAttribute[eType], byBattleAttribute, FALSE );	
}

VOID CStatusAvatarTab::ClickEffect( RwBool bPush, RwInt32 nSlotIdx /* = -1  */ )
{
	CRectangle rtScreen = m_pSelf->GetScreenRect();

	if( bPush )
	{
		m_surIcon[nSlotIdx].SetRect( rtScreen.left + m_rtEquipSlot[nSlotIdx].left + ICONPUSH_SIZEDIFF, rtScreen.top + m_rtEquipSlot[nSlotIdx].top + ICONPUSH_SIZEDIFF,
									 rtScreen.left + m_rtEquipSlot[nSlotIdx].right - ICONPUSH_SIZEDIFF, rtScreen.top + m_rtEquipSlot[nSlotIdx].bottom - ICONPUSH_SIZEDIFF );
	}
	else if( m_nPushDownIndex >= 0 )
	{
		m_surIcon[m_nPushDownIndex].SetRect( rtScreen.left + m_rtEquipSlot[m_nPushDownIndex].left, rtScreen.top + m_rtEquipSlot[m_nPushDownIndex].top,
											 rtScreen.left + m_rtEquipSlot[m_nPushDownIndex].right, rtScreen.top + m_rtEquipSlot[m_nPushDownIndex].bottom );
	}	

	m_nPushDownIndex = nSlotIdx;
}

VOID CStatusAvatarTab::FritzEffect( eFRITZEFFECT eFritzState, RwInt32 nSlotIdx )
{
	if( nSlotIdx < 0 || nSlotIdx >= NTL_MAX_EQUIP_ITEM_SLOT )
		return;

	switch( eFritzState )
	{
	case FRITZ_NONE:
		m_surFritzSlot[nSlotIdx].Show( FALSE );
		return;
	case FRITZ_HALF:
		m_surFritzSlot[nSlotIdx].SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfHalfFritzSlotEffect" ) );
		break;
	case FRITZ_ABSOLUTE:
		m_surFritzSlot[nSlotIdx].SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfFritzSlotEffect" ) );
		break;
	}

	CRectangle rtScreen = m_pSelf->GetScreenRect();

	m_surFritzSlot[nSlotIdx].SetRect( rtScreen.left + m_rtEquipSlot[nSlotIdx].left, rtScreen.top + m_rtEquipSlot[nSlotIdx].top,
									  rtScreen.left + m_rtEquipSlot[nSlotIdx].right, rtScreen.top + m_rtEquipSlot[nSlotIdx].bottom );

	m_surFritzSlot[nSlotIdx].Show( TRUE );
}

VOID CStatusAvatarTab::ShowPickedUp( RwBool bShow, RwInt32 nSlotIdx /* = 0  */ )
{
	if( bShow )
	{
		CRectangle rtScreen = m_pSelf->GetScreenRect();

		m_nShowPickedUp = nSlotIdx;
		m_surPickedUp.SetPosition( m_rtEquipSlot[nSlotIdx].left + rtScreen.left, m_rtEquipSlot[nSlotIdx].top + rtScreen.top );
	}
	else
	{
		m_nShowPickedUp = -1;
	}
}

VOID CStatusAvatarTab::ShowDisableSlot( RwBool bShow, RwInt32 nSlotIdx )
{
	m_surDisableSlot[nSlotIdx].Show( bShow );
}

RwBool CStatusAvatarTab::IsDisableSlot( RwInt32 nSlotIdx )
{
	if( nSlotIdx < 0 && nSlotIdx >= NTL_MAX_EQUIP_ITEM_SLOT )
		return TRUE;

	return m_surDisableSlot[nSlotIdx].IsShow() ? TRUE : FALSE;
}

////////////////////////////////////////////////////////////////////////////////
// ! Callback

VOID CStatusAvatarTab::OnMouseDown( const CKey& key )
{
	RwInt32 nClickIdx = GetEquipSlotIdx( (RwInt32)key.m_fX, (RwInt32)key.m_fY );

	if( EQUIP_SLOT_TYPE_SCOUTER == nClickIdx )
	{
		if( !Logic_CanMouseInput_in_Tutorial( ETL_MOUSE_INPUT_TYPE_STATUS_WND_SCOUTER_SLOT ) )
		{
			m_nLSelectedSlotIdx = -1;
			m_nRSelectedSlotIdx = -1;
			ClickEffect( FALSE );
			m_pSelf->ReleaseMouse();
			return;
		}
	}	

	if( nClickIdx >= 0 && !GetIconMoveManager()->IsActive() )
		ClickEffect( TRUE, nClickIdx );

	if( key.m_nID == UD_LEFT_BUTTON )
	{
		// 1. Src선택인가 Dest선택인가.
		if( GetIconMoveManager()->IsActive() )
		{
			if( IsEnableEquipPutDown( nClickIdx ) )
			{
				m_nLSelectedSlotIdx = nClickIdx;
			}
		}
		else
		{
			eDialogMode eMode = GetDialogManager()->GetMode();

			if( eMode == DIALOGMODE_ITEM_REPAIR ||
				eMode == DIALOGMODE_ITEM_IDENTIFICATION ||
				eMode == DIALOGMODE_NPCSHOP_ITEM_IDENTIFICATION )
			{
				m_nLSelectedSlotIdx = nClickIdx;
			}
			// 2. DragDrop이 가능한 상황인가.
			else if( IsEnableEquipPickUp( nClickIdx ) )
			{
				m_nLSelectedSlotIdx = nClickIdx;
			}			
		}		
	}
	else if( key.m_nID == UD_RIGHT_BUTTON )
	{
		if( GetDialogManager()->GetMode() == DIALOGMODE_UNKNOWN )
		{
			if( !GetIconMoveManager()->IsActive() )
			{
				if( IsEnableUnequipItem( nClickIdx ) )
				{
					m_nRSelectedSlotIdx = nClickIdx;
				}
			}			
		}
		
		// 1. 아이콘 유효성 검사
		//if( IsEnableEquipPopup( nClickIdx ) )
		//{
		//	m_nRSelectedSlotIdx = nClickIdx;
		//}
	}

	m_pSelf->CaptureMouse();

	if( m_nLSelectedSlotIdx >= 0 && m_nRSelectedSlotIdx >= 0 )
	{
		m_nLSelectedSlotIdx = -1;
		m_nRSelectedSlotIdx = -1;
		ClickEffect( FALSE );
		m_pSelf->ReleaseMouse();
	}
}

VOID CStatusAvatarTab::OnMouseUp( const CKey& key )
{
	// 1. Left버튼인가 Right버튼인가
	// 2. Src선택인가 Dest선택인가.

	RwInt32 nSlotIdx = GetEquipSlotIdx( (RwInt32)key.m_fX, (RwInt32)key.m_fY );
	ClickEffect( FALSE );
	m_pSelf->ReleaseMouse();

	if( nSlotIdx < 0 || !m_pSelf->IsVisibleTruly() )
	{
		m_nLSelectedSlotIdx = -1;
		m_nRSelectedSlotIdx = -1;
		return;
	}

	if( key.m_nID == UD_LEFT_BUTTON )
	{
		if( nSlotIdx == m_nLSelectedSlotIdx )
		{
			if( GetIconMoveManager()->IsActive() )
			{
				if( GetDialogManager()->GetMode() == DIALOGMODE_UNKNOWN )
				{
					GetIconMoveManager()->IconMovePutDown( PLACE_EQUIP, INVALID_SERIAL_ID, nSlotIdx );
				}				
			}
			else
			{
				if( GetDialogManager()->IsMode( DIALOGMODE_ITEM_REPAIR ) )
				{
					Logic_ItemRepairProc( CONTAINER_TYPE_EQUIP, (RwUInt8)nSlotIdx );					
				}
				else
				{
					if( GetDialogManager()->GetMode() == DIALOGMODE_UNKNOWN )
					{
						CNtlInventory* pInventory = GetNtlSLGlobal()->GetSobAvatar()->GetInventory();
						SERIAL_HANDLE hSrcSerial = pInventory->GetEquipItem( nSlotIdx );
						CNtlSobItem* pItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( hSrcSerial ) );
						if( pItem )
						{
							CNtlSobItemAttr* pItemAttr = reinterpret_cast<CNtlSobItemAttr*>( pItem->GetSobAttr() );
							if( GetIconMoveManager()->IconMovePickUp( hSrcSerial, PLACE_EQUIP, nSlotIdx, pItemAttr->GetStackNum(), pItem->GetIcon()->GetImage() ) )
								ShowPickedUp( TRUE, nSlotIdx );
						}
					}					
				}
			}

		}

		m_nLSelectedSlotIdx = -1;
	}
	else if( key.m_nID == UD_RIGHT_BUTTON )
	{
		if( GetDialogManager()->GetMode() == DIALOGMODE_UNKNOWN )
		{
			if( nSlotIdx == m_nRSelectedSlotIdx )
			{
				if( nSlotIdx == EQUIP_SLOT_TYPE_SCOUTER )
				{
					GetDialogManager()->SwitchDialog( DIALOG_SCOUTER_BODY );
				}
				else
				{
					RwInt32 nDestBagSlotIdx = 0, nDestSlotIdx = 0;
					if( Logic_FindEmptyItemSlot( &nDestBagSlotIdx, &nDestSlotIdx ) )
					{
						CNtlInventory* pInventory = GetNtlSLGlobal()->GetSobAvatar()->GetInventory();
						SERIAL_HANDLE hSrcSerial = pInventory->GetEquipItem( nSlotIdx );
						CNtlSobItem* pItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( hSrcSerial ) );
						if( pItem )
						{
							CNtlSobItemAttr* pItemAttr = reinterpret_cast<CNtlSobItemAttr*>( pItem->GetSobAttr() );
							Logic_ItemMoveProc( hSrcSerial, PLACE_EQUIP, (RwUInt8)nSlotIdx, PLACE_BAG, pInventory->GetBagItem( nDestBagSlotIdx ), (RwUInt8)nDestSlotIdx, pItemAttr->GetStackNum() );
						}					
					}
					else
					{
						GetAlarmManager()->AlarmMessage( DST_ITEM_CAN_NOT_UNEQUIP_BAG_FULL );
					}

					//CRectangle rtScreen = m_pSelf->GetScreenRect();
					//SERIAL_HANDLE hSrcSerial = GetNtlSLGlobal()->GetSobAvatar()->GetInventory()->GetEquipItem( nSlotIdx );
					//CDboEventGenerator::IconPopupShow( TRUE, hSrcSerial, PLACE_EQUIP, nSlotIdx, m_rtEquipSlot[nSlotIdx].left + rtScreen.left, m_rtEquipSlot[nSlotIdx].top + rtScreen.top );
				}			
			}
		}
		
		m_nRSelectedSlotIdx = -1;
	}
}

VOID CStatusAvatarTab::OnMouseMove( RwInt32 nFlags, RwInt32 nX, RwInt32 nY )
{
	RwBool bFlag = FALSE;

	for( RwInt32 i = 0 ; i < NTL_MAX_EQUIP_ITEM_SLOT ; ++i )
	{
		if( m_rtEquipSlot[i].PtInRect( nX, nY ) )
		{
			m_arrFocusEffect[i] = m_arrFocusEffect[i] | SLOT_FOCUS;

			if( m_nMouseOnIndex != i )
			{
				SERIAL_HANDLE hSerial = GetNtlSLGlobal()->GetSobAvatar()->GetInventory()->GetEquipItem( i );
				CNtlSobItem* pItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( hSerial ) );
				CRectangle rtScreen = m_pSelf->GetScreenRect();

				if( pItem )
				{
					GetInfoWndManager()->ShowInfoWindow( TRUE, CInfoWndManager::INFOWND_ITEM, m_rtEquipSlot[i].left + rtScreen.left, m_rtEquipSlot[i].top + rtScreen.top, pItem, DIALOG_STATUS );					
				}
				else
				{
					GetInfoWndManager()->ShowInfoWindow( TRUE, CInfoWndManager::INFOWND_JUST_WTEXT, m_rtEquipSlot[i].left + rtScreen.left, m_rtEquipSlot[i].top + rtScreen.top, (VOID*)GetEquipSlotText( i ), DIALOG_STATUS );										
				}

				if( i == m_nPushDownIndex )
					ClickEffect( TRUE, i );
				else if( m_nPushDownIndex >= 0 )
					ClickEffect( FALSE, m_nPushDownIndex );

				m_nMouseOnIndex = i;
			}

			bFlag = TRUE;
		}
		else
		{
			m_arrFocusEffect[i] = m_arrFocusEffect[i] & ~SLOT_FOCUS;
		}
	}

	if( !bFlag && m_nMouseOnIndex != -1 )
	{
		GetInfoWndManager()->ShowInfoWindow( FALSE );
		if( m_nPushDownIndex >= 0 )
			ClickEffect( FALSE, m_nPushDownIndex );
		m_nMouseOnIndex = -1;
	}
}

VOID CStatusAvatarTab::OnMouseLeave( gui::CComponent* pComponent )
{
	if( m_nMouseOnIndex != -1 )
	{
		m_arrFocusEffect[m_nMouseOnIndex] = m_arrFocusEffect[m_nMouseOnIndex] & ~SLOT_FOCUS;
		GetInfoWndManager()->ShowInfoWindow( FALSE );
		if( m_nPushDownIndex >= 0 )
			ClickEffect( FALSE, m_nPushDownIndex );
		m_nMouseOnIndex = -1;
	}
}

VOID CStatusAvatarTab::OnPaint(VOID)
{
	// Avatar출력
	m_surCharacter.Render();
}

VOID CStatusAvatarTab::OnSlotPaint(VOID)
{
	// Icon 출력
	for( RwInt32 i = 0 ; i < NTL_MAX_EQUIP_ITEM_SLOT ; ++i )
	{
		if( m_surIcon[i].GetTexture() )
			m_surIcon[i].Render();
	}

	if( m_nShowPickedUp >= 0 )
		m_surPickedUp.Render();

	for( RwInt32 i = 0 ; i < NTL_MAX_EQUIP_ITEM_SLOT ; ++i )
	{
		m_surFritzSlot[i].Render();
		m_surDisableSlot[i].Render();

		if( m_arrFocusEffect[i] )
			m_surFocusSlot[i].Render();
	}
}

VOID CStatusAvatarTab::OnMove( RwInt32 nX, RwInt32 nY )
{
	CRectangle rtScreen = m_pSelf->GetScreenRect();

	for( RwInt32 i = 0 ; i < NTL_MAX_EQUIP_ITEM_SLOT ; ++i )
	{
		m_surIcon[i].SetPosition( rtScreen.left + m_rtEquipSlot[i].left, rtScreen.top + m_rtEquipSlot[i].top );
		m_surFocusSlot[i].SetPosition( rtScreen.left + m_rtEquipSlot[i].left, rtScreen.top + m_rtEquipSlot[i].top );
		m_surFritzSlot[i].SetPosition( rtScreen.left + m_rtEquipSlot[i].left, rtScreen.top + m_rtEquipSlot[i].top );
		m_surDisableSlot[i].SetPosition( rtScreen.left + m_rtEquipSlot[i].left, rtScreen.top + m_rtEquipSlot[i].top );
	}

	if( m_nShowPickedUp >= 0 )
		m_surPickedUp.SetPosition( rtScreen.left + m_rtEquipSlot[m_nShowPickedUp].left, rtScreen.top + m_rtEquipSlot[m_nShowPickedUp].top );

	m_surCharacter.SetPosition( rtScreen.left + 46, rtScreen.top + 96 );
}

VOID CStatusAvatarTab::OnLeftRotBtnPress( gui::CComponent* pComponent )
{
	CNtlSobCharProxy* pCharProxy = reinterpret_cast<CNtlSobCharProxy*>( GetNtlSLGlobal()->GetSobAvatar()->GetSobProxy() );
	if( pCharProxy )
		pCharProxy->PcStatusRotateLeft();
}

VOID CStatusAvatarTab::OnLeftRotBtnRelease( gui::CComponent* pComponent )
{
	CNtlSobCharProxy* pCharProxy = reinterpret_cast<CNtlSobCharProxy*>( GetNtlSLGlobal()->GetSobAvatar()->GetSobProxy() );
	if( pCharProxy )
		pCharProxy->PcStatusRotateStop();
}

VOID CStatusAvatarTab::OnRightRotBtnPress( gui::CComponent* pComponent )
{
	CNtlSobCharProxy* pCharProxy = reinterpret_cast<CNtlSobCharProxy*>( GetNtlSLGlobal()->GetSobAvatar()->GetSobProxy() );
	if( pCharProxy )
		pCharProxy->PcStatusRotateRight();
}

VOID CStatusAvatarTab::OnRightRotBtnRelease( gui::CComponent* pComponent )
{
	CNtlSobCharProxy* pCharProxy = reinterpret_cast<CNtlSobCharProxy*>( GetNtlSLGlobal()->GetSobAvatar()->GetSobProxy() );
	if( pCharProxy )
		pCharProxy->PcStatusRotateStop();	
}

VOID CStatusAvatarTab::OnDragRotBtnPress( gui::CComponent* pComponent )
{
	CRectangle rtScreen = pComponent->GetScreenRect();
	m_nDragRotX = CMouse::GetX() - rtScreen.left;
}

VOID CStatusAvatarTab::OnDragRotBtnRelease( gui::CComponent* pComponent )
{

}

VOID CStatusAvatarTab::OnDragRotMouseMove( RwInt32 nFlags, RwInt32 nX, RwInt32 nY )
{
	if( nFlags & UD_MK_LBUTTON )
	{
		CNtlSobCharProxy* pCharProxy = reinterpret_cast<CNtlSobCharProxy*>( GetNtlSLGlobal()->GetSobAvatar()->GetSobProxy() );

		RwReal fDeltaAngle = (RwReal)( nX - m_nDragRotX ) * 1.5f;
		pCharProxy->PcStatusRotate( fDeltaAngle );

		m_nDragRotX = nX;
	}
}

VOID CStatusAvatarTab::OnMouseWeaponAttrEnter( gui::CComponent* pComponent )
{
	CRectangle rtScreen = pComponent->GetScreenRect();

	m_sBattleAttr.eBattleAttrInfoType = stINFOWND_BATTLEATTR::TYPE_ATTR_WEAPON_INFO;

	GetInfoWndManager()->ShowInfoWindow( TRUE, CInfoWndManager::INFOWND_BATTLEATTRIBUTE,
		rtScreen.left, rtScreen.top,
		reinterpret_cast<void*>( &m_sBattleAttr ),
		DIALOG_STATUS );
}

VOID CStatusAvatarTab::OnMouseWeaponAttrLeave( gui::CComponent* pComponent )
{
	if( DIALOG_STATUS == GetInfoWndManager()->GetRequestGui() &&
		CInfoWndManager::INFOWND_BATTLEATTRIBUTE == GetInfoWndManager()->GetInfoWndState() )
		GetInfoWndManager()->ShowInfoWindow( FALSE );
}

VOID CStatusAvatarTab::OnMouseArmorAttrEnter( gui::CComponent* pComponent )
{
	CRectangle rtScreen = pComponent->GetScreenRect();

	m_sBattleAttr.eBattleAttrInfoType = stINFOWND_BATTLEATTR::TYPE_ATTR_ARMOR_INFO;

	GetInfoWndManager()->ShowInfoWindow( TRUE, CInfoWndManager::INFOWND_BATTLEATTRIBUTE,
		rtScreen.left, rtScreen.top,
		reinterpret_cast<void*>( &m_sBattleAttr ),
		DIALOG_STATUS );
}

VOID CStatusAvatarTab::OnMouseArmorAttrLeave( gui::CComponent* pComponent )
{
	if( DIALOG_STATUS == GetInfoWndManager()->GetRequestGui() &&
		CInfoWndManager::INFOWND_BATTLEATTRIBUTE == GetInfoWndManager()->GetInfoWndState() )
		GetInfoWndManager()->ShowInfoWindow( FALSE );
}

VOID CStatusAvatarTab::SetSourceAttr( RwUInt8 bySourceWeaponAttr, RwUInt8 bySourceArmorAttr )
{
	m_sBattleAttr.bySourceWeaponAttr = bySourceWeaponAttr;
	m_sBattleAttr.bySourceArmorAttr = bySourceArmorAttr;
}

VOID CStatusAvatarTab::SetTargetAttr( RwUInt8 byTargetWeaponAttr, RwUInt8 byTargetArmorAttr )
{
	m_sBattleAttr.byTargetWeaponAttr = byTargetWeaponAttr;
	m_sBattleAttr.byTargetArmorAttr = byTargetArmorAttr;
}

VOID CStatusAvatarTab::OnBattleAttributeRefresh()
{
	// 현재 마우스의 위치가 무기 전투 속성 패널에 있는지 방어구 전투 속성 패널에 있는지 판단
	eTYPE_BATTLEATTR eType = TYPECOUNT;
	for( RwInt32 i = 0; i< TYPECOUNT; ++i )
	{
		if( m_ppnlBattleAttribute[i]->GetScreenRect().PtInRect( CMouse::GetX(), CMouse::GetY() ) )
			eType = (eTYPE_BATTLEATTR)i;
	}

	// 마우스의 두 개의 패널이 아니라면 InfoWindow를 갱신할 필요가 없다.
	if( eType == TYPECOUNT )
		return;
	else if( eType == TYPE_WEAPON )
		m_sBattleAttr.eBattleAttrInfoType = stINFOWND_BATTLEATTR::TYPE_ATTR_WEAPON_INFO;
	else
		m_sBattleAttr.eBattleAttrInfoType = stINFOWND_BATTLEATTR::TYPE_ATTR_ARMOR_INFO;

	// 현재 HpGui에서 요청한 InfoWindow인지 확인하고 전투 속성 관련 InfoWindow를 띄우는 것까지 확인
	// 하여 맞다면 현재 띄워진 InfoWindow를 닫고 새로 띄워준다.
	if( DIALOG_STATUS == GetInfoWndManager()->GetRequestGui() &&
		CInfoWndManager::INFOWND_BATTLEATTRIBUTE == GetInfoWndManager()->GetInfoWndState() )
	{
		GetInfoWndManager()->ShowInfoWindow( FALSE );

		CRectangle rtScreen = m_ppnlBattleAttribute[eType]->GetScreenRect();

		GetInfoWndManager()->ShowInfoWindow( TRUE, CInfoWndManager::INFOWND_BATTLEATTRIBUTE,
			rtScreen.left, rtScreen.top,
			reinterpret_cast<void*>( &m_sBattleAttr ),
			DIALOG_STATUS );
	}
}

VOID CStatusAvatarTab::SetAttributeToolTip( VOID )
{
	m_pstbBattleAttrTitle->SetToolTip( GetDisplayStringManager()->GetString( DST_STATUS_TOOLTIP_ATTR ) );
	
	m_pstbBasicStatTitle[STR]->SetToolTip( GetDisplayStringManager()->GetString( DST_STATUS_TOOLTIP_STR ) );
	m_pstbBasicStatTitle[DEX]->SetToolTip( GetDisplayStringManager()->GetString( DST_STATUS_TOOLTIP_DEX ) );
	m_pstbBasicStatTitle[CON]->SetToolTip( GetDisplayStringManager()->GetString( DST_STATUS_TOOLTIP_CON ) );
	m_pstbBasicStatTitle[ENG]->SetToolTip( GetDisplayStringManager()->GetString( DST_STATUS_TOOLTIP_ENG ) );
	m_pstbBasicStatTitle[SOL]->SetToolTip( GetDisplayStringManager()->GetString( DST_STATUS_TOOLTIP_SOL ) );
	m_pstbBasicStatTitle[FOC]->SetToolTip( GetDisplayStringManager()->GetString( DST_STATUS_TOOLTIP_FOC ) );

	m_pstbPhysicalCombatStatTitle[OFFENCE]->SetToolTip( GetDisplayStringManager()->GetString( DST_STATUS_TOOLTIP_PHYSICAL_ATTACK ) );
	m_pstbPhysicalCombatStatTitle[DEFENCE]->SetToolTip( GetDisplayStringManager()->GetString( DST_STATUS_TOOLTIP_PHYSICAL_DEFENCE ) );
	m_pstbPhysicalCombatStatTitle[CRITICAL]->SetToolTip( GetDisplayStringManager()->GetString( DST_STATUS_TOOLTIP_PHYSICAL_CRITICAL ) );

	m_pstbEnergyCombatStatTitle[OFFENCE]->SetToolTip( GetDisplayStringManager()->GetString( DST_STATUS_TOOLTIP_ENERGY_ATTACK ) );
	m_pstbEnergyCombatStatTitle[DEFENCE]->SetToolTip( GetDisplayStringManager()->GetString( DST_STATUS_TOOLTIP_ENERGY_DEFENCE ) );
	m_pstbEnergyCombatStatTitle[CRITICAL]->SetToolTip( GetDisplayStringManager()->GetString( DST_STATUS_TOOLTIP_ENERGY_CRITICAL ) );

	m_pstbETCStatTitle[ATTACKRATE]->SetToolTip( GetDisplayStringManager()->GetString( DST_STATUS_TOOLTIP_ATTACK_RATING ) );
	m_pstbETCStatTitle[DODGE]->SetToolTip( GetDisplayStringManager()->GetString( DST_STATUS_TOOLTIP_DODGE_RATING ) );
}
//VOID CStatusWindowGui::SetStackNum( RwInt32 nSlot, RwInt32 nStackNum )
//{
//	if( nSlot >= NTL_MAX_EQUIP_ITEM_SLOT || nSlot < 0 )
//		return;
//
//	if( m_pGuiStackNum[nSlot] )
//	{
//		m_pGuiStackNum[nSlot]->SetText( nStackNum );
//	}
//	else
//	{
//		CRectangle rtStackNum( m_rtEquipSlot[nSlot].right - MAX_STACK_NUM_WIDTH,
//							   m_rtEquipSlot[nSlot].bottom - MAX_STACK_NUM_HEIGHT,
//							   MAX_STACK_NUM_WIDTH, MAX_STACK_NUM_HEIGHT );
//		m_pGuiStackNum[nSlot] = NTL_NEW gui::CStaticBox( rtStackNum, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_RIGHT );
//		m_pGuiStackNum[nSlot]->CreateFontStd( "Arial", 70, EZ_ATTR_BOLD );
//	}
//}
//
//VOID CStatusWindowGui::DeleteStackNum( RwInt32 nSlot )
//{
//	if( nSlot >= NTL_MAX_EQUIP_ITEM_SLOT || nSlot < 0 )
//		return;
//
//	if( m_pGuiStackNum[nSlot] )
//		NTL_DELETE( m_pGuiStackNum[nSlot] );
//}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// ! CStatusHonorList
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//CStatusHonorList::CStatusHonorList()
//{
//
//}
//
//CStatusHonorList::~CStatusHonorList()
//{
//
//}
//
//RwBool CStatusHonorList::Create( CRectangle& rect, gui::CComponent* pParent, RwInt32 nLineHeight, RwInt32 nLineMargin, RwInt32 nChildXMargin, RwInt32 nScrollBarWidth )
//{
//	if( !CGuiLineTree::Create( rect, pParent, nLineHeight, nLineMargin, nChildXMargin, nScrollBarWidth ))
//		return FALSE;
//
//	m_pScroll->AddSurfaceLayout( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfScrollLayout" ) );
//	m_pScroll->AddSurfaceSlider( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfScrollMedium" ) );
//	m_pScroll->GetDecreaseButton()->AddSurfaceUp( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfScrollPrevBtnUp" ) );
//	m_pScroll->GetDecreaseButton()->AddSurfaceDown( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfScrollPrevBtnDown" ) );
//	m_pScroll->GetDecreaseButton()->AddSurfaceFocus( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfScrollPrevBtnFoc" ) );
//	m_pScroll->GetIncreaseButton()->AddSurfaceUp( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfScrollNextBtnUp" ) );
//	m_pScroll->GetIncreaseButton()->AddSurfaceDown( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfScrollNextBtnDown" ) );
//	m_pScroll->GetIncreaseButton()->AddSurfaceFocus( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfScrollNextBtnUp" ) );
//	m_pScroll->SetButtonSize(dSTATUSWINDOW_LIST_SCROLL_WIDTH, dSTATUSWINDOW_LIST_SCROLL_HEIGHT);
//	m_pScroll->SetSliderSize(dSTATUSWINDOW_LIST_SLIDER_WIDTH, dSTATUSWINDOW_LIST_SLIDER_HEIGHT);
//
//	return TRUE;
//}
//
//void CStatusHonorList::CreateTree()
//{
//
//	// 명예 점수 카테고리
//	CGuiLineTreeNode* pNode = NTL_NEW CStatusHonorCategoryNode(this,
//		L"명예점수", -2 );
//	CGuiLineTree::AddNode( pNode, GUILINETREE_ROOTNODE_ID );
//	pNode->Expand( true );
//
//	pNode = NTL_NEW CStatusHonorContent( this, L"명예점수 : 9999999999", 10 );
//	CGuiLineTree::AddNode( pNode, -2 );
//
//	pNode = NTL_NEW CStatusHonorCategoryNode( this,
//		L"랭크배틀", -3 );
//	CGuiLineTree::AddNode( pNode, GUILINETREE_ROOTNODE_ID );
//	pNode->Expand( true );
//
//	pNode = NTL_NEW CStatusHonorContent( this, L"랭크포인트 : 99999999", 11 );
//	CGuiLineTree::AddNode( pNode, -3 );
//
//}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// ! CStatusHonorCategoryNode
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//CStatusHonorCategoryNode::CStatusHonorCategoryNode( CGuiLineTree* pMgr, std::wstring strTitle, RwInt32 nID )
//: CGuiLineTreeNode( pMgr, nID )
//, m_pBtnExpand(NULL)
//, m_pBtnReduce(NULL)
//{
//	CRectangle rect;
//	rect.SetRectWH( dSTATUSWINDOW_CATEGORY_BUTTON_X, dSTATUSWINDOW_CATEGORY_BUTTON_Y,
//		dSTATUSWINDOW_CATEGORY_BUTTON_WIDTH, dSTATUSWINDOW_CATEGORY_BUTTON_HEIGHT );
//	m_pBtnExpand = NTL_NEW gui::CButton( rect, std::string(),pMgr->GetParentGui(), GetNtlGuiManager()->GetSurfaceManager());
//	m_pBtnExpand->AddSurfaceUp(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("QuestList.srf", "srfExpandBtnUp"));
//	m_pBtnExpand->AddSurfaceFocus(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("QuestList.srf", "srfExpandBtnFoc"));
//	m_pBtnExpand->AddSurfaceDown(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("QuestList.srf", "srfExpandBtnDown"));
//	m_pBtnExpand->SetText( strTitle.c_str() );
//
//	// -버튼
//	m_pBtnReduce = NTL_NEW gui::CButton(rect, std::string(),pMgr->GetParentGui(), GetNtlGuiManager()->GetSurfaceManager());
//	m_pBtnReduce->AddSurfaceUp(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("QuestList.srf", "srfReduceBtnUp"));
//	m_pBtnReduce->AddSurfaceFocus(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("QuestList.srf", "srfReduceBtnFoc"));
//	m_pBtnReduce->AddSurfaceDown(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("QuestList.srf", "srfReduceBtnDown"));
//	m_pBtnReduce->SetText( strTitle.c_str() );
//
//	// Button의 Signal 연결
//	m_slotClickedBtnExpand = m_pBtnExpand->SigClicked().Connect(this, &CStatusHonorCategoryNode::OnClickBtnExpand);
//	m_slotClickedBtnReduce = m_pBtnReduce->SigClicked().Connect(this, &CStatusHonorCategoryNode::OnClickBtnReduce);
//}
//
//CStatusHonorCategoryNode::~CStatusHonorCategoryNode()
//{
//	NTL_DELETE( m_pBtnExpand );
//	NTL_DELETE( m_pBtnReduce );
//}
//
//void CStatusHonorCategoryNode::ShowProc()
//{
//	CGuiLineTreeNode::ShowProc();
//
//	if (IsExpand())
//	{
//		m_pBtnReduce->Show(true);		
//		m_pBtnExpand->Show(false);
//	}
//	else
//	{
//		m_pBtnExpand->Show(true);
//		m_pBtnReduce->Show(false);		
//	}
//
//	// 카테고리 노드들의 위치를 재 계산
//	m_pBtnExpand->SetPosition(m_nPosX + dSTATUSWINDOW_CATEGORY_BUTTON_X, m_nPosY + dSTATUSWINDOW_CATEGORY_BUTTON_Y);
//	m_pBtnReduce->SetPosition(m_nPosX + dSTATUSWINDOW_CATEGORY_BUTTON_X, m_nPosY + dSTATUSWINDOW_CATEGORY_BUTTON_Y);
//}
//
//void CStatusHonorCategoryNode::HideProc()
//{
//	CGuiLineTreeNode::HideProc();
//
//	m_pBtnExpand->Show(false);
//	m_pBtnReduce->Show(false);
//}
//
//void CStatusHonorCategoryNode::OnClickBtnExpand( gui::CComponent* pComponent )
//{
//	Expand(TRUE);
//}
//
//void CStatusHonorCategoryNode::OnClickBtnReduce( gui::CComponent* pComponent )
//{
//	Expand(FALSE);
//}
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// ! CStatusHonorContent
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//CStatusHonorContent::CStatusHonorContent( CGuiLineTree* pMgr, const WCHAR* pwcContent, RwInt32 nID )
//: CGuiLineTreeNode( pMgr, nID )
//, m_pHtmlContent(NULL)
//{
//	CRectangle rect;
//	rect.SetRectWH( dSTATUSWINDOW_CONTENTS_X, dSTATUSWINDOW_CONTENTS_Y,
//		dSTATUSWINDOW_CONTENTS_WIDTH, dSTATUSWINDOW_CONTENTS_HEIGHT );
//	
//	m_pHtmlContent = NTL_NEW gui::CHtmlBox( rect, pMgr->GetParentGui(), GetNtlGuiManager()->GetSurfaceManager() );
//	m_pHtmlContent->SetHtmlFromMemory( pwcContent, wcslen( pwcContent ) );
//}
//
//CStatusHonorContent::~CStatusHonorContent()
//{
//	NTL_DELETE( m_pHtmlContent );
//}
//
//void CStatusHonorContent::ShowProc()
//{
//	CGuiLineTreeNode::ShowProc();
//
//	m_pHtmlContent->Show(true);
//
//	m_pHtmlContent->SetPosition( m_nPosX, m_nPosY );
//}
//
//void CStatusHonorContent::HideProc()
//{
//	CGuiLineTreeNode::HideProc();
//
//	m_pHtmlContent->Show(false);
//}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ! CStatusHonorTab
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CStatusHonorTab::CStatusHonorTab()
{
	m_pSelf = NULL;
}

CStatusHonorTab::~CStatusHonorTab()
{
	
}

RwBool CStatusHonorTab::IsShow( VOID )
{
	if( !m_pSelf->IsVisible() )
		return FALSE;

	if( !m_pSelf->GetParent()->IsVisible() )
		return FALSE;

	return TRUE;
}

RwBool CStatusHonorTab::Create( CNtlPLGui* pParent )
{
	NTL_FUNCTION( "CStatusHonorTab::Create" );
	m_pSelf = reinterpret_cast<gui::CDialog*>( pParent->GetComponent( "dlgHonor" ) );

	m_pBtnExpandHonor = (gui::CButton*)pParent->GetComponent("btnHonorExpand");
	m_pBtnReduceHonor = (gui::CButton*)pParent->GetComponent("btnHonorReduce");
	m_pBtnExpandRankBattle = (gui::CButton*)pParent->GetComponent("btnRankBattleExpand");
	m_pBtnReduceRankBattle = (gui::CButton*)pParent->GetComponent("btnRankBattleReduce");

	m_pHtmlHonor = (gui::CHtmlBox*)pParent->GetComponent("htmlHonor");
	m_pHtmlRankBattle = (gui::CHtmlBox*)pParent->GetComponent("htmlRankBattle");

	m_slotClickedBtnExpandHonor = m_pBtnExpandHonor->SigClicked().Connect( this, &CStatusHonorTab::OnClickedBtnExpandHonor );
	m_slotClickedBtnReduceHonor = m_pBtnReduceHonor->SigClicked().Connect( this, &CStatusHonorTab::OnClickedBtnReduceHonor );
	m_slotClickedBtnExpandRankBattle = m_pBtnExpandRankBattle->SigClicked().Connect( this, &CStatusHonorTab::OnClickedBtnExpandRankBattle );
	m_slotClickedBtnReduceRankBattle = m_pBtnReduceRankBattle->SigClicked().Connect( this, &CStatusHonorTab::OnClickedBtnReduceRankBattle );

	m_pBtnExpandHonor->SetText( GetDisplayStringManager()->GetString( DST_STATUS_HONOR_TITLE_HONOR ) );
	m_pBtnReduceHonor->SetText( GetDisplayStringManager()->GetString( DST_STATUS_HONOR_TITLE_HONOR ) );
	m_pBtnExpandRankBattle->SetText( GetDisplayStringManager()->GetString( DST_STATUS_HONOR_TITLE_RANKBATTLE ) );
	m_pBtnReduceRankBattle->SetText( GetDisplayStringManager()->GetString( DST_STATUS_HONOR_TITLE_RANKBATTLE ) );

	UpdateUI();
	Refresh();

	NTL_RETURN( TRUE );
}

VOID CStatusHonorTab::Destroy( VOID )
{
	NTL_FUNCTION("CStatusHonorTab::Destroy");

	NTL_RETURNVOID();
}

VOID CStatusHonorTab::UpdateUI()
{
	m_pBtnExpandRankBattle->Show( !m_bExpandRankBattle );
	m_pBtnReduceRankBattle->Show( B2b(m_bExpandRankBattle) );

	m_pBtnExpandRankBattle->SetPosition( 20, m_pBtnExpandHonor->GetPosition().top ); 
	m_pBtnReduceRankBattle->SetPosition( 20, m_pBtnExpandHonor->GetPosition().top ); 

	if( m_bExpandRankBattle )
	{
		m_pHtmlRankBattle->SetPosition( 50, m_pBtnExpandRankBattle->GetPosition().bottom + 5 );
		m_pHtmlRankBattle->Show( true );
	}
	else
	{
		m_pHtmlRankBattle->Show( false );
	}
}

VOID CStatusHonorTab::Refresh( VOID )
{
	CNtlSobAvatar* pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();

	if( pSobAvatar )
	{
		CNtlRankBattle* pRankBattle = pSobAvatar->GetRankBattle();

		if( pRankBattle )
		{
			sRANKBATTLE_SCORE_INFO* pScore = pRankBattle->GetRankBattleScore();

			RwUInt32 uiTotal = pScore->dwWin
				+ pScore->dwDraw + pScore->dwLose;

			WCHAR awcData[256];
			swprintf_s( awcData, 256, GetDisplayStringManager()->GetString( DST_STATUS_HONOR_PRIVATE_SCORE ),
				uiTotal, 
				pScore->dwWin,
				pScore->dwLose );

			WCHAR awcRankPoint[256];
			swprintf_s( awcRankPoint, 256,
				GetDisplayStringManager()->GetString( DST_STATUS_HONOR_RANK_SCORE ),
				(RwInt32)pScore->fPoint );

			WCHAR awcMudosaPoint[256];
			swprintf_s( awcMudosaPoint, 
				256, 
				GetDisplayStringManager()->GetString( DST_STATUS_HONOR_MUDOSA_SCORE ),
				Logic_GetMudosaPoint() );

			WCHAR awcRankBattle[1024];
			swprintf_s( awcRankBattle, 1024, 
				GetDisplayStringManager()->GetString( DST_STATUS_HONOR_HTMLTAG_RANKBATTLE ), 
				awcData, 
				awcRankPoint, 
				awcMudosaPoint );

			m_pHtmlRankBattle->SetHtmlFromMemory( awcRankBattle, 1024*sizeof(WCHAR) );
		}
	}
	else
	{
		SAvatarInfo* pInfo = GetNtlSLGlobal()->GetAvatarInfo();

		if( pInfo )
		{
			RwUInt32 uiTotal = pInfo->sRankBattleScoreInfo.dwWin
				+ pInfo->sRankBattleScoreInfo.dwDraw + pInfo->sRankBattleScoreInfo.dwLose;

			WCHAR awcData[256];
			swprintf_s( awcData, 256, GetDisplayStringManager()->GetString( DST_STATUS_HONOR_PRIVATE_SCORE ),
				uiTotal, 
				pInfo->sRankBattleScoreInfo.dwWin,
				pInfo->sRankBattleScoreInfo.dwLose );

			WCHAR awcRankPoint[256];
			swprintf_s( awcRankPoint, 256,
				GetDisplayStringManager()->GetString( DST_STATUS_HONOR_RANK_SCORE ),
				(RwInt32)pInfo->sRankBattleScoreInfo.fPoint );

			WCHAR awcMudosaPoint[256];
			swprintf_s( awcMudosaPoint, 
				256, 
				GetDisplayStringManager()->GetString( DST_STATUS_HONOR_MUDOSA_SCORE ),
				Logic_GetMudosaPoint() );

			WCHAR awcRankBattle[1024];
			swprintf_s( awcRankBattle, 1024, 
				GetDisplayStringManager()->GetString( DST_STATUS_HONOR_HTMLTAG_RANKBATTLE ), 
				awcData, 
				awcRankPoint, 
				awcMudosaPoint );

			m_pHtmlRankBattle->SetHtmlFromMemory( awcRankBattle, 1024*sizeof(WCHAR) );
		}
	}
}

VOID CStatusHonorTab::HandleEvents( RWS::CMsg& msg )
{
	if( msg.Id == g_EventRBTotalScoreUpdate ||
		msg.Id == g_EventCharRankPointResetNfy )
	{
		Refresh();
	}
	else if( msg.Id == g_EventSobInfoUpdate )
	{
		if( !IsShow() )
			return;

		SNtlEventSobInfoUpdate* pUpdate = reinterpret_cast<SNtlEventSobInfoUpdate*>( msg.pData );

		if( pUpdate->hSerialId != GetNtlSLGlobal()->GetSobAvatar()->GetSerialID() )
			return;

		if( pUpdate->uiUpdateType & EVENT_AIUT_POINT_MUDOSA ||
			pUpdate->uiUpdateType & EVENT_AIUT_POINT_HONOR )
		{
			Refresh();
		}
	}
}

VOID CStatusHonorTab::OnClickedBtnExpandHonor( gui::CComponent* pComponent )
{
	m_bExpandHonor = TRUE;
	UpdateUI();
}

VOID CStatusHonorTab::OnClickedBtnReduceHonor( gui::CComponent* pComponent )
{
	m_bExpandHonor = FALSE;
	UpdateUI();
}

VOID CStatusHonorTab::OnClickedBtnExpandRankBattle( gui::CComponent* pComponent )
{
	m_bExpandRankBattle = TRUE;
	UpdateUI();
}

VOID CStatusHonorTab::OnClickedBtnReduceRankBattle( gui::CComponent* pComponent )
{
	m_bExpandRankBattle = FALSE;
	UpdateUI();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ! StatusTechnicTab
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CStatusTechnicTab::CStatusTechnicTab()
{

}

CStatusTechnicTab::~CStatusTechnicTab()
{

}

RwBool CStatusTechnicTab::IsShow( VOID )
{
	return m_pSelf->IsVisible();
}

VOID CStatusTechnicTab::Init()
{
	m_bHoipoiMixExpand = true;
}

RwBool CStatusTechnicTab::Create( CNtlPLGui* pParent )
{
	m_pSelf = reinterpret_cast<gui::CDialog*>( pParent->GetComponent( "dlgTechnic" ) );

	m_pBtnExpandHoipoiMix = reinterpret_cast<gui::CButton*>( pParent->GetComponent( "btnHoipoiExpand") );
	m_pBtnExpandHoipoiMix->SetText( GetDisplayStringManager()->GetString( DST_STATUS_TECHNIC_HOIPOIMIX_TITLE ) );
	m_pBtnReduceHoipoiMix = reinterpret_cast<gui::CButton*>( pParent->GetComponent( "btnHoipoiReduce") );
	m_pBtnReduceHoipoiMix->SetText( GetDisplayStringManager()->GetString( DST_STATUS_TECHNIC_HOIPOIMIX_TITLE ) );

	m_slotClickedBtnExpandHoipoiMix = m_pBtnExpandHoipoiMix->SigClicked().Connect( this, &CStatusTechnicTab::OnClickedBtnExpandHoipoiMix );
	m_slotClickedBtnReduceHoipoiMix = m_pBtnReduceHoipoiMix->SigClicked().Connect( this, &CStatusTechnicTab::OnClickedBtnReduceHoipoiMix );

	m_pHoipoiMixDlg = reinterpret_cast<gui::CDialog*>( pParent->GetComponent( "dlgHoipoiMix" ) );
	
	m_pStbMixLevel = reinterpret_cast<gui::CStaticBox*>( pParent->GetComponent( "stbMixlevel" ) );
	m_pStbMixLevel->SetText( GetDisplayStringManager()->GetString( DST_STATUS_TECHNIC_HOIPOIMIX_MIXLEVEL ) );
	m_pStbMixLevelData = reinterpret_cast<gui::CStaticBox*>( pParent->GetComponent( "stbMixLevelData" ) );
	m_pStbMixExp = reinterpret_cast<gui::CStaticBox*>( pParent->GetComponent( "stbMixExp" ) );
	m_pStbMixExp->SetText( GetDisplayStringManager()->GetString( DST_STATUS_TECHNIC_HOIPOIMIX_MIXEXPDATA ) );
	m_pStbMixExpData = reinterpret_cast<gui::CStaticBox*>( pParent->GetComponent( "stbMixExpData" ) );
	m_pPgbExpData = reinterpret_cast<gui::CProgressBar*>( pParent->GetComponent( "pgbMixExpData" ) );
	m_pPgbExpData->SetRange( 0, 100 );

	m_pStbNormalMix = reinterpret_cast<gui::CStaticBox*>( pParent->GetComponent( "stbNormalMix" ) );
	m_pStbNormalMix->SetText( GetDisplayStringManager()->GetString( DST_STATUS_TECHNIC_HOIPOIMIX_NORMAL_SKILL ) );
	m_pStbNormalRequireGuide = reinterpret_cast<gui::CStaticBox*>( pParent->GetComponent( "stbNormalRequireGuide" ) );
	m_pStbNormalRequireGuide->SetText( GetDisplayStringManager()->GetString( DST_STATUS_TECHNIC_HOIPOIMIX_NORMAL_SKILL_GUIDE ) );
	m_pHtmlNormalGuide = reinterpret_cast<gui::CHtmlBox*>( pParent->GetComponent( "htmlNormalGuide" ) );
	m_pHtmlNormalGuide->SetHtmlFromMemory( GetDisplayStringManager()->GetString( DST_STATUS_TECHNIC_HOIPOIMIX_NORMAL_SKILL_GUIDE_HTML ),
		wcslen( GetDisplayStringManager()->GetString( DST_STATUS_TECHNIC_HOIPOIMIX_NORMAL_SKILL_GUIDE_HTML ) ) );

	m_pStbSpecialMix = reinterpret_cast<gui::CStaticBox*>( pParent->GetComponent( "stbSpecialMix" ) );
	m_pStbSpecialMix->SetText( GetDisplayStringManager()->GetString( DST_STATUS_TECHNIC_HOIPOIMIX_SPECIAL_SKILL ) );
	m_pStbSpecialRequireGuide = reinterpret_cast<gui::CStaticBox*>( pParent->GetComponent( "stbSpecialRequireGuide" ) );
	m_pStbSpecialRequireGuide->SetText( GetDisplayStringManager()->GetString( DST_STATUS_TECHNIC_HOIPOIMIX_SPECIAL_SKILL_GUIDE ) );
	m_pHtmlSpecialGuide = reinterpret_cast<gui::CHtmlBox*>( pParent->GetComponent( "htmlSpecialGuide" ) );
	m_pHtmlSpecialGuide->SetHtmlFromMemory( GetDisplayStringManager()->GetString( DST_STATUS_TECHNIC_HOIPOIMIX_SPECIAL_SKILL_GUIDE_HTML ),
		wcslen( GetDisplayStringManager()->GetString( DST_STATUS_TECHNIC_HOIPOIMIX_SPECIAL_SKILL_GUIDE_HTML ) ) );

	UpdateUI();
	Refresh();

	return TRUE;
}

VOID CStatusTechnicTab::Destroy( VOID )
{

}

VOID CStatusTechnicTab::HandleEvents( RWS::CMsg& msg )
{
	if( msg.Id == g_EventHoipoiMixItemMakeExpNfy ||
		msg.Id == g_EventHoipoiMixJobSetRes ||
		msg.Id == g_EventHoipoiMixJobResetRes )
	{
		Refresh();
	}
}

VOID CStatusTechnicTab::UpdateUI( VOID )
{
	m_pBtnExpandHoipoiMix->Show( !m_bHoipoiMixExpand );
	m_pBtnReduceHoipoiMix->Show( m_bHoipoiMixExpand );
	m_pHoipoiMixDlg->Show( m_bHoipoiMixExpand );
}

VOID CStatusTechnicTab::Refresh( VOID )
{
	CNtlSobAvatar* pSobAvatar =  GetNtlSLGlobal()->GetSobAvatar();
	if( pSobAvatar )
	{
		CNtlSLHoipoiMix* pHoipoiMix = pSobAvatar->GetHoipoiMix();
		if( pHoipoiMix )
		{
			RwUInt8 byLevel = pHoipoiMix->GetMixLevel();

			// 레벨
			m_pStbMixLevelData->SetText( byLevel );

			// 경험치
			RwUInt32 uiExp = pHoipoiMix->GetMixExp();
			RwUInt32 uiMaxExp = Dbo_GetHoipoiMixNeedLevelExp( byLevel );

			WCHAR awcBuffer[128];
			swprintf_s( awcBuffer, 128, L"%u / %u", uiExp, uiMaxExp );

			m_pStbMixExpData->SetText( awcBuffer );
			RwInt32 nPercent = 0;

			if( uiExp != 0 )
			{
				nPercent = (RwInt32)(( (RwReal)uiExp / (RwReal)uiMaxExp ) * 100.f );
				if( nPercent > 100 )
					nPercent = 100;
			}
		
			m_pPgbExpData->SetPos( nPercent );

			if( pHoipoiMix->IsNormalStart() )
			{
				m_pHtmlNormalGuide->SetHtmlFromMemory( GetDisplayStringManager()->GetString( DST_STATUS_TECHNIC_HOIPOIMIX_SKILL_FOOD_HTML ),
					wcslen( GetDisplayStringManager()->GetString( DST_STATUS_TECHNIC_HOIPOIMIX_SKILL_FOOD_HTML ) ) );
			}
			else
			{
				m_pHtmlNormalGuide->SetHtmlFromMemory( GetDisplayStringManager()->GetString( DST_STATUS_TECHNIC_HOIPOIMIX_NORMAL_SKILL_GUIDE_HTML ),
					wcslen( GetDisplayStringManager()->GetString( DST_STATUS_TECHNIC_HOIPOIMIX_NORMAL_SKILL_GUIDE_HTML ) ) );
			}

			if( pHoipoiMix->IsSpecialStart() )
			{
				switch( pHoipoiMix->GetSpecialType() )
				{
				case ITEM_RECIPE_TYPE_MAIN_WEAPON:
					m_pHtmlSpecialGuide->SetHtmlFromMemory( GetDisplayStringManager()->GetString( DST_STATUS_TECHNIC_HOIPOIMIX_SKILL_MAIN_HTML ),
						wcslen( GetDisplayStringManager()->GetString( DST_STATUS_TECHNIC_HOIPOIMIX_SKILL_MAIN_HTML ) ) );
					break;
				case ITEM_RECIPE_TYPE_ARMOR:
					m_pHtmlSpecialGuide->SetHtmlFromMemory( GetDisplayStringManager()->GetString( DST_STATUS_TECHNIC_HOIPOIMIX_SKILL_ARMOR_HTML ),
						wcslen( GetDisplayStringManager()->GetString( DST_STATUS_TECHNIC_HOIPOIMIX_SKILL_ARMOR_HTML ) ) );
				case ITEM_RECIPE_TYPE_ACCESSORY:
					m_pHtmlSpecialGuide->SetHtmlFromMemory( GetDisplayStringManager()->GetString( DST_STATUS_TECHNIC_HOIPOIMIX_SKILL_ACCESSORY_HTML ),
						wcslen( GetDisplayStringManager()->GetString( DST_STATUS_TECHNIC_HOIPOIMIX_SKILL_ACCESSORY_HTML ) ) );
					break;
				case ITEM_RECIPE_TYPE_SCOUTER:
					m_pHtmlSpecialGuide->SetHtmlFromMemory( GetDisplayStringManager()->GetString( DST_STATUS_TECHNIC_HOIPOIMIX_SKILL_SCOUTER_HTML ),
						wcslen( GetDisplayStringManager()->GetString( DST_STATUS_TECHNIC_HOIPOIMIX_SKILL_SCOUTER_HTML ) ) );
					break;
				case ITEM_RECIPE_TYPE_SUB_WEAPON:
					m_pHtmlSpecialGuide->SetHtmlFromMemory( GetDisplayStringManager()->GetString( DST_STATUS_TECHNIC_HOIPOIMIX_SKILL_SUB_WEAPON_HTML ),
						wcslen( GetDisplayStringManager()->GetString( DST_STATUS_TECHNIC_HOIPOIMIX_SKILL_SUB_WEAPON_HTML ) ) );
					break;
				default:
					m_pHtmlSpecialGuide->SetHtmlFromMemory( GetDisplayStringManager()->GetString( DST_STATUS_TECHNIC_HOIPOIMIX_NORMAL_SKILL_GUIDE_HTML ),
						wcslen( GetDisplayStringManager()->GetString( DST_STATUS_TECHNIC_HOIPOIMIX_NORMAL_SKILL_GUIDE_HTML ) ) );
					break;
				}
			}
			else
			{
				m_pHtmlSpecialGuide->SetHtmlFromMemory( GetDisplayStringManager()->GetString( DST_STATUS_TECHNIC_HOIPOIMIX_NORMAL_SKILL_GUIDE_HTML ),
					wcslen( GetDisplayStringManager()->GetString( DST_STATUS_TECHNIC_HOIPOIMIX_NORMAL_SKILL_GUIDE_HTML ) ) );
			}
		}
	}
}

VOID CStatusTechnicTab::OnClickedBtnExpandHoipoiMix( gui::CComponent* pComponent )
{
	m_bHoipoiMixExpand = true;
	UpdateUI();
}

VOID CStatusTechnicTab::OnClickedBtnReduceHoipoiMix( gui::CComponent* pComponent )
{
	m_bHoipoiMixExpand = false;
	UpdateUI();
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ! StatusWnd
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// ! Construction & Destruction

CStatusWindowGui::CStatusWindowGui(VOID)
{
	m_pstbTitle = NULL;
	m_ptabStatus = NULL;
	m_pbtnExit = NULL;

	m_pAvatarTab = NULL;
	m_pHonorTab = NULL;
	m_pTechnicTab = NULL;
}

CStatusWindowGui::CStatusWindowGui( const RwChar* pName )
: CNtlPLGui( pName )
{
	m_pstbTitle = NULL;
	m_ptabStatus = NULL;
	m_pbtnExit = NULL;

	m_pAvatarTab = NULL;
	m_pHonorTab = NULL;
	m_pTechnicTab = NULL;

	m_nCurrentTab = -1;
}

CStatusWindowGui::~CStatusWindowGui(VOID)
{
	NTL_DELETE( m_pAvatarTab );
	NTL_DELETE( m_pHonorTab );
	NTL_DELETE( m_pTechnicTab );
}

////////////////////////////////////////////////////////////////////////////////
// ! Operation
RwBool CStatusWindowGui::Create(VOID)
{
	NTL_FUNCTION( "CStatusWindowGui::Create" );

	if( !CNtlPLGui::Create( "", "gui\\StatusWnd.srf", "gui\\StatusWnd.frm" ) )
		NTL_RETURN( FALSE );

	CNtlPLGui::CreateComponents( GetNtlGuiManager()->GetGuiManager() );

	m_pThis = (gui::CDialog*)GetComponent( "dlgMain" );

	m_pstbTitle = reinterpret_cast<gui::CStaticBox*>( GetComponent( "stbTitle" ) );
	m_ptabStatus = reinterpret_cast<gui::CTabButton*>( GetComponent( "tabStatus" ) );
	m_pbtnExit = reinterpret_cast<gui::CButton*>( GetComponent( "btnClose" ) );
	m_pBtnHelp = reinterpret_cast<gui::CButton*>( GetComponent( "btnHelp" ) );

	m_slotHelp = m_pBtnHelp->SigClicked().Connect( this, &CStatusWindowGui::OnClickedHelp );

	m_slotSelectTab = m_ptabStatus->SigSelectChanged().Connect( this, &CStatusWindowGui::OnSelectedTab );
	m_slotExit = m_pbtnExit->SigClicked().Connect( this, &CStatusWindowGui::OnClickedExit );
	m_slotCaptureMouseDown = GetNtlGuiManager()->GetGuiManager()->SigCaptureMouseDown().Connect( this, &CStatusWindowGui::OnAbsolulteClick );
	
	m_pAvatarTab = NTL_NEW CStatusAvatarTab;
	NTL_ASSERT( m_pAvatarTab, "CStatusWindowGui::Create() : Alloc Fail" );
	if( !m_pAvatarTab->Create( this ) )
	{
		m_pAvatarTab->Destroy();

		NTL_DELETE( m_pAvatarTab );

		NTL_RETURN( FALSE );
	}

	m_pHonorTab = NTL_NEW CStatusHonorTab;
	NTL_ASSERT( m_pHonorTab, "CStatusWindowGui::Create() : Alloc Failed" );
	if( !m_pHonorTab->Create( this ) )
	{
		m_pHonorTab->Destroy();

		NTL_DELETE( m_pHonorTab );

		NTL_RETURN( FALSE );
	}

	m_pTechnicTab = NTL_NEW CStatusTechnicTab;
	NTL_ASSERT( m_pTechnicTab, "CStatusWindowGui::Create() : Alloc Failed" );
	if( !m_pTechnicTab->Create( this ) )
	{
		m_pTechnicTab->Destroy();

		NTL_DELETE( m_pTechnicTab );

		NTL_RETURN( FALSE );
	}

	// Initial Setting
	SetBasicUISetting();
	m_pThis->Show( false );

	// serialize setting
	EnableSerialize(TRUE);

	// Update 연결
	GetNtlGuiManager()->AddUpdateBeforeFunc( this );
	
	// Event Link
	LinkMsg( g_EventSobInfoUpdate, 0 );
	LinkMsg( g_EventIconMoveClick, 0 );
	LinkMsg( g_EventPickedUpHide, 0 );
	LinkMsg( g_EventEnableItemIcon, 0 );
	LinkMsg(g_EventSubWeaponActive, (const char*)0, 0x7000 );
	LinkMsg(g_EventSubWeaponDeActive, (const char*)0, 0x7000 );
	LinkMsg(g_EventSobEquipChange, (const char*)0, 0x7000);
	LinkMsg(g_EventTLNotifyLockUnlock, 0);
	LinkMsg(g_EventChangeWorldConceptState, 0);
	LinkMsg(g_EventSobConvertClass);
	LinkMsg(g_EventChangeClassAuthorityChangedNfy);
	LinkMsg(g_EventRBTotalScoreUpdate);
	LinkMsg(g_EventCharRankPointResetNfy);
	LinkMsg(g_EventHoipoiMixItemMakeExpNfy);
	LinkMsg(g_EventHoipoiMixJobSetRes);
	LinkMsg(g_EventHoipoiMixJobResetRes);

	NTL_RETURN( TRUE );
}

VOID CStatusWindowGui::Destroy(VOID)
{
	NTL_FUNCTION( "CStatusWindowGui::Destroy" );

	// Event Unlink
	UnLinkMsg( g_EventIconMoveClick );
	UnLinkMsg( g_EventSobInfoUpdate );
	UnLinkMsg( g_EventPickedUpHide );
	UnLinkMsg( g_EventEnableItemIcon );
	UnLinkMsg(g_EventSubWeaponActive);
	UnLinkMsg(g_EventSubWeaponDeActive);
	UnLinkMsg(g_EventSobEquipChange);
	UnLinkMsg(g_EventTLNotifyLockUnlock);
	UnLinkMsg(g_EventChangeWorldConceptState);
	UnLinkMsg(g_EventSobConvertClass);
	UnLinkMsg(g_EventChangeClassAuthorityChangedNfy);
	UnLinkMsg(g_EventRBTotalScoreUpdate);
	UnLinkMsg(g_EventCharRankPointResetNfy);
	UnLinkMsg(g_EventHoipoiMixItemMakeExpNfy);
	UnLinkMsg(g_EventHoipoiMixJobSetRes);
	UnLinkMsg(g_EventHoipoiMixJobResetRes);

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();

	// Update 해제
	GetNtlGuiManager()->RemoveUpdateBeforeFunc( this );

	NTL_RETURNVOID();
}

VOID CStatusWindowGui::UpdateBeforeCamera(RwReal fElapsed)
{
	if( m_nCurrentTab == TAB_AVATAR )
		m_pAvatarTab->UpdateBeforeCamera( fElapsed );	
}

VOID CStatusWindowGui::HandleEvents( RWS::CMsg& msg )
{
	if( m_nCurrentTab == TAB_AVATAR )
		m_pAvatarTab->HandleEvents( msg );
	else if( m_nCurrentTab == TAB_HONOR )
		m_pHonorTab->HandleEvents( msg );
	else if( m_nCurrentTab == TAB_TECHNIC )
		m_pTechnicTab->HandleEvents( msg );

	if( msg.Id == g_EventTLNotifyLockUnlock )
	{
		SNtlEventTLLockUnlcok* pEvent = reinterpret_cast<SNtlEventTLLockUnlcok*>( msg.pData );

		if( pEvent->byLockType != TLLOCK_MOUSE )
			return;

		if( pEvent->byIndex != ETL_MOUSE_INPUT_TYPE_STATUS_WND_TAB && 
			pEvent->byIndex != ETL_MOUSE_INPUT_TYPE_ALL )
			return;

		for( RwUInt8 i = 0 ; i < m_ptabStatus->GetButtonCount() ; ++i )
			m_ptabStatus->EnableTab(i, !pEvent->bLock);
	}
	else if( msg.Id == g_EventChangeWorldConceptState )
	{
		SNtlEventWorldConceptState* pEvent = reinterpret_cast<SNtlEventWorldConceptState*>( msg.pData );

		if( pEvent->eWorldConcept == WORLD_PLAY_TUTORIAL )
		{
			if( pEvent->uiState == WORLD_STATE_EXIT )
			{
				for( RwUInt8 i = 0 ; i < m_ptabStatus->GetButtonCount() ; ++i )
					m_ptabStatus->EnableTab(i, true);
			}
		}
	}
	else if( msg.Id == g_EventChangeClassAuthorityChangedNfy )
	{
		SNtlEventChangeClassAuthorityChanged* pData = reinterpret_cast<SNtlEventChangeClassAuthorityChanged*>( msg.pData );

		if( pData->bCanChangeClass )
			GetAlarmManager()->AlarmMessage( DST_CHAR_CAN_CHANGE_CLASS );
	}
}

RwInt32 CStatusWindowGui::SwitchDialog( bool bOpen )
{
	if( bOpen )
	{
		RaiseTop();
		Show( true );
		m_ptabStatus->SelectTab( (RwInt32)TAB_AVATAR );
		SelectTab( TAB_AVATAR );
		Logic_SendTutorialCondition(ETL_CONDITION_TYPE_STATUS_OPEN);
	}
	else
	{
		if( GetInfoWndManager()->GetRequestGui() == DIALOG_STATUS )
			GetInfoWndManager()->ShowInfoWindow( FALSE );

		Show( false );		
		Logic_SendTutorialCondition(ETL_CONDITION_TYPE_STATUS_CLOSE);
	}

	return 1;
}

////////////////////////////////////////////////////////////////////////////////
// ! Implementation

VOID CStatusWindowGui::SetBasicUISetting(VOID)
{
	m_pstbTitle->SetText( GetDisplayStringManager()->GetString( DST_STATUS_WINDOW_TITLE ) );

	m_ptabStatus->AddTab( std::wstring( GetDisplayStringManager()->GetString( DST_STATUS_TAB_AVATAR ) ) );
	m_ptabStatus->AddTab( std::wstring( GetDisplayStringManager()->GetString( DST_STATUS_TAB_FAMILY ) ) );
	m_ptabStatus->AddTab( std::wstring( GetDisplayStringManager()->GetString( DST_STATUS_TECHNIC_TITLE ) ) );
	m_ptabStatus->SelectTab( (RwInt32)TAB_AVATAR );		
	SelectTab( TAB_AVATAR );	
}

VOID CStatusWindowGui::SelectTab( RwInt32 nIndex )
{
	switch( nIndex )
	{
	case TAB_AVATAR:
		m_pAvatarTab->Show( true );
		m_pHonorTab->Show( false );
		m_pAvatarTab->Refresh();
		m_pTechnicTab->Show( false );
		break;
	case TAB_HONOR:
		m_pAvatarTab->Show( false );
		m_pHonorTab->Show( true );
		m_pHonorTab->Refresh();
		m_pTechnicTab->Show( false );
		break;
	case TAB_TECHNIC:
		m_pAvatarTab->Show( false );
		m_pHonorTab->Show( false );
		m_pTechnicTab->Show( true );
		m_pTechnicTab->Refresh();
		break;
	}

	m_nCurrentTab = nIndex;
}

////////////////////////////////////////////////////////////////////////////////
// ! Callbacks
VOID CStatusWindowGui::OnClickedExit( gui::CComponent* pComponent )
{
	if( !Logic_CanMouseInput_in_Tutorial( ETL_MOUSE_INPUT_TYPE_STATUS_WND_CLOSE_BTN ) )
		return;

	GetDialogManager()->SwitchDialog( DIALOG_STATUS );
}

VOID CStatusWindowGui::OnSelectedTab( RwInt32 nSelectIndex, RwInt32 nPreviousIndex )
{
	SelectTab( nSelectIndex );
}

VOID CStatusWindowGui::OnAbsolulteClick( const CKey& key )
{
	CAPTURE_MOUSEDOWN_RAISE( DIALOG_STATUS, key.m_fX, key.m_fY );
}

VOID CStatusWindowGui::OnClickedHelp( gui::CComponent* pComponent )
{
	CDboEventGenerator::OpenHelpContent( DIALOG_STATUS );
}