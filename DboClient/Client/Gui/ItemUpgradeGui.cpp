#include "precomp_dboclient.h"
#include "ItemUpgradeGui.h"

// core
#include "NtlDebug.h"
#include "NtlMath.h"

// shared
#include "NtlItem.h"
#include "NtlCharacter.h"
#include "ItemTable.h"
#include "ItemOptionTable.h"
#include "SystemEffectTable.h"
#include "NPCTable.h"
#include "TextAllTable.h"
#include "TableContainer.h"
#include "GraphicDataTable.h"

// presentation
#include "NtlPLGuiManager.h"
#include "NtlPLEvent.h"

// simulation
#include "NtlSLDef.h"
#include "NtlSLEvent.h"
#include "NtlSLEventFunc.h"
#include "NtlSLLogic.h"
#include "NtlSobManager.h"
#include "NtlSobItem.h"
#include "NtlSobItemAttr.h"
#include "NtlSobItemIcon.h"
#include "NtlSobNpc.h"
#include "NtlSobNpcAttr.h"
#include "NtlSLApi.h"
#include "NtlWorldConcept.h"
#include "NtlCameraController.h"
#include "NtlWorldConceptNPCCommu.h"
#include "NtlSLGlobal.h"
#include "NtlSobAvatar.h"

// Dbo
#include "DboDef.h"
#include "DboEvent.h"
#include "DboEventGenerator.h"
#include "DboGlobal.h"
#include "InfoWndManager.h"
#include "IconMoveManager.h"
#include "DialogManager.h"
#include "DboLogic.h"
#include "DisplayStringManager.h"
#include "GUISoundDefine.h"
#include "AlarmManager.h"

#define ITEMUPGRADEGUI_SUCCESS_FLASHFILE		"ItemUpgrade_Success.swf"
#define ITEMUPGRADEGUI_SUCCESS_FLASHFILE_2		"ItemUpgrade_Success2.swf"
#define ITEMUPGRADEGUI_SUCCESS_FLASHFILE_3		"ItemUpgrade_Success3.swf"

#define ITEMUPGRADEGUI_DOWNGRADE_SUCCESS_FLASHFILE "ItemDowngrade_Success.swf"
#define ITEMUPGRADEGUI_FAIL_FLASHFILE			"ItemUpgrade_Fail.swf"
#define ITEMUPGRADEGUI_PROCESS_FLASHFILE		"ItemUpgrade_Effect.swf"
#define ITEMUPGRADEGUI_PURE_PROCESS_FLASHFILE	"ItemUpgrade_Effect_Pure.swf"
#define ITEMUPGRADEGUI_BLACK_PROCESS_FLASHFILE	"ItemUpgrade_Effect_Black.swf"

#define ITEMUPGRADEGUI_MAX_PURE_GRADE			3

#define CONV_TO_POSNUM(y)						(y+1)
#define ITEMUPGRADE_MAX_GRADE					4
#define ITEMUPGRADE_HOIPOISTONE30				19951
#define ITEMUPGRADE_TEXTBUFFER					4096

CItemUpgradeGui::CItemUpgradeGui(VOID)
{
	Init();
}

CItemUpgradeGui::CItemUpgradeGui( const RwChar* pName )
: CNtlPLGui( pName )
{
	Init();
}

CItemUpgradeGui::~CItemUpgradeGui(VOID)
{

}

VOID CItemUpgradeGui::Init(VOID)
{
	m_pUpgradeItem = NULL;
	m_pHoipoiStone = NULL;
	m_nStoneSlotIdx = -1;
	m_pstbStackNum = NULL;
	m_nStoneStackNum = 0;
	m_fDestroyProbability = 0.0f;
	m_fScouterBonus = 0.0f;

	m_nLSelectedSlotIdx = -1;
	m_nRSelectedSlotIdx = -1;
	m_nMouseOnIdx = -1;
	m_nPushDownIndex = -1;

	for( RwInt32 i = 0 ; i < NUM_SLOT; ++i )
	{
		m_anFocusEffect[i] = SLOT_FOCUS_NONE;				
	}

	m_eState = STATE_NONE;

	m_fCurrentTime = 0.0f;

	m_hNPCSerial = INVALID_SERIAL_ID;
}

RwBool CItemUpgradeGui::Create(VOID)
{
	NTL_FUNCTION( "CItemUpgradeGui::Create" );

	if( !CNtlPLGui::Create( "", "gui\\ItemUpgrade.srf", "gui\\ItemUpgrade.frm" ) )
		NTL_RETURN( FALSE );

	CNtlPLGui::CreateComponents( GetNtlGuiManager()->GetGuiManager() );

	m_pThis = (gui::CDialog*)GetComponent( "dlgMain" );

	m_pstbTitle = (gui::CStaticBox*)GetComponent( "stbTitle" );
	m_pstbInfoTitle = (gui::CStaticBox*)GetComponent( "stbInfoTitle" );

	m_pflsUpgradeEffect = (gui::CFlash*)GetComponent( "flsUpgradeEffect" );
	m_phtmlInfoText = (gui::CHtmlBox*)GetComponent( "htmlUpgradeInfo" );

	m_ppnlPredict = (gui::CPanel*)GetComponent( "pnlPredict" );
	m_pstbPredictText = (gui::CStaticBox*)GetComponent( "stbPredictText" );
	m_pstbPredictResult = (gui::CStaticBox*)GetComponent( "stbPredictResult" );
	m_pstbDestroyPredictText = (gui::CStaticBox*)GetComponent( "stbDestroyPredictText" );
	m_pstbDestroyPredictResult = (gui::CStaticBox*)GetComponent( "stbDestroyPredictResult" );

	m_pbtnExit = (gui::CButton*)GetComponent( "btnExit" );
	m_pbtnCompound = (gui::CButton*)GetComponent( "btnCompound" );

	m_pdlgResult = (gui::CDialog*)GetComponent( "dlgResult" );
	m_pflsResult = (gui::CFlash*)GetComponent( "flsResult" );
	m_pstbResult = (gui::CStaticBox*)GetComponent( "stbResult" );
	m_ppnlUpgradeItemIcon = (gui::CPanel*)GetComponent( "pnlUpgradedItemIcon" );
	m_pbtnCloseResult = (gui::CButton*)GetComponent( "btnClose" );
	m_pstbUpgradeItemName = (gui::CStaticBox*)GetComponent( "stbUpgradedItemName" );
	m_pstbResultInfo = (gui::CStaticBox*)GetComponent( "stbUpgradedInfo" );

	m_pstbStackNum = NTL_NEW gui::CStaticBox( CRectangle( 0, 0,DBOGUI_STACKNUM_WIDTH, DBOGUI_STACKNUM_HEIGHT ), m_pThis, GetNtlGuiManager()->GetSurfaceManager(), DBOGUI_STACKNUM_ALIGN );
	m_pstbStackNum->CreateFontStd( DBOGUI_STACKNUM_FONT, DBOGUI_STACKNUM_FONTHEIGHT, DBOGUI_STACKNUM_FONTATTR );
	m_pstbStackNum->SetEffectMode( DBOGUI_STACKNUM_FONTEFFECTMODE );
	m_pstbStackNum->Enable( false );
	m_pstbStackNum->Show( false );

	m_slotMouseDown		= m_pThis->SigMouseDown().Connect( this, &CItemUpgradeGui::OnMouseDown );
	m_slotMouseUp		= m_pThis->SigMouseUp().Connect( this, &CItemUpgradeGui::OnMouseUp );
	m_slotMouseMove		= m_pThis->SigMouseMove().Connect( this, &CItemUpgradeGui::OnMouseMove );
	m_slotMouseOut		= m_pThis->SigMouseLeave().Connect( this, &CItemUpgradeGui::OnMouseOut );
	m_slotPaint			= m_pbtnExit->SigPaint().Connect( this, &CItemUpgradeGui::OnPaint );
	m_slotMove			= m_pThis->SigMove().Connect( this, &CItemUpgradeGui::OnMove );
	m_slotClickExitBtn  = m_pbtnExit->SigClicked().Connect( this, &CItemUpgradeGui::OnClickExitBtn );
	m_slotClickCompoundBtn = m_pbtnCompound->SigClicked().Connect( this, &CItemUpgradeGui::OnClickCompoundBtn );
	m_slotClickCloseResult = m_pbtnCloseResult->SigClicked().Connect( this, &CItemUpgradeGui::OnClickCloseResult );
	m_slotCaptureMouseDown = GetNtlGuiManager()->GetGuiManager()->SigCaptureMouseDown().Connect( this, &CItemUpgradeGui::OnCaptureMouseDown );
	m_slotCaptureMouseWheel= m_phtmlInfoText->SigMouseWheel().Connect( this, &CItemUpgradeGui::OnCaptureMouseWheel );
	m_slotResultItemIconPaint = m_ppnlUpgradeItemIcon->SigPaint().Connect( this, &CItemUpgradeGui::OnResultItemIconPaint );
	m_slotResultItemIconMove = m_ppnlUpgradeItemIcon->SigMove().Connect( this, &CItemUpgradeGui::OnResultItemIconMove );
	m_slotResultItemIconMouseEnter = m_ppnlUpgradeItemIcon->SigMouseEnter().Connect( this, &CItemUpgradeGui::OnResultItemIconMouseEnter );
	m_slotResultItemIconMouseLeave = m_ppnlUpgradeItemIcon->SigMouseLeave().Connect( this, &CItemUpgradeGui::OnResultItemIconMouseLeave );
	m_slotUpgradeEffectEnd = m_pflsUpgradeEffect->SigMovieEnd().Connect( this, &CItemUpgradeGui::OnUpgradeEffectEnd );

	// 스택넘버 보다 위로. 
	m_pdlgResult->Raise();

	SetSlotRectHardCode();
	SetBasicUISetting();
	Show( false );

	m_pdlgResult->Show( false );
	m_ppnlPredict->Show( false );

	LinkMsg( g_EventItemUpgradeResult, 0 );
	LinkMsg( g_EventNPCDialogOpen, 0 );
	LinkMsg( g_EventDialog, 0 );
	LinkMsg( g_EventSobInfoUpdate, 0 );
	LinkMsg( g_EventIconMoveClick, 0 );
	LinkMsg( g_EventSobItemDelete, 0, 0x9000 );		// 포인터를 쓰기 때문에 객체가 지워지기 전에 들어와야 한다.
	LinkMsg( g_EventRegisterItemUpgrade, 0 );
	LinkMsg( g_EventGameServerChangeOut );
	LinkMsg(g_EventAnimEnd);

	NTL_RETURN( TRUE );
}

VOID CItemUpgradeGui::Destroy(VOID)
{
	UnLinkMsg( g_EventItemUpgradeResult );
	UnLinkMsg( g_EventNPCDialogOpen );
	UnLinkMsg( g_EventDialog );
	UnLinkMsg( g_EventSobInfoUpdate );
	UnLinkMsg( g_EventIconMoveClick );
	UnLinkMsg( g_EventSobItemDelete );
	UnLinkMsg( g_EventRegisterItemUpgrade );
	UnLinkMsg( g_EventGameServerChangeOut );
	UnLinkMsg(g_EventAnimEnd);

	NTL_DELETE( m_pstbStackNum );	

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();
}

VOID CItemUpgradeGui::DestroyResultDialog(VOID)
{
	m_surUpgradeItemIcon.UnsetTexture();
	m_pdlgResult->Show( false );
	m_pflsResult->PlayMovie( FALSE );
}

VOID CItemUpgradeGui::Update( RwReal fElapsed )
{
	if( m_eState == STATE_RESULT )
	{
		m_pflsResult->Update( fElapsed );
	}
	else if( m_eState == STATE_UPGRADE_PROCESS )
	{
		m_pflsUpgradeEffect->Update( fElapsed );
	}

	for( RwInt32 i = 0 ; i < NUM_SLOT ; ++i )
	{
		m_aSlotEffect[i].Update( fElapsed );
	}

	m_TempItemSlot.Update( fElapsed );
	m_BackFocus.Update( fElapsed );
}

RwInt32 CItemUpgradeGui::SwitchDialog( bool bOpen )
{
	if( bOpen )
	{
		CNtlWorldConceptNPCCOMMU* pWorldConceptController = reinterpret_cast<CNtlWorldConceptNPCCOMMU*>( GetNtlWorldConcept()->GetWorldConceptController(WORLD_PLAY_NPC_COMMU) );
		pWorldConceptController->ChangeState(WORLD_NPC_ITEM_UPGRADE);

		RaiseTop();			

		Show( true );
		DeleteHoipoiStone();
		DeleteUpgradeItem();
		SetState( STATE_NONE );

		for( RwInt32 i = 0 ; i < NUM_SLOT ; ++i )
			m_aSlotEffect[i].SetState( stSLOTEFFECT::SLOT_EFFECT_NONE );

		m_TempItemSlot.SetState( stSLOTEFFECT::SLOT_EFFECT_NONE );
		m_BackFocus.SetState( stSLOTEFFECT::SLOT_EFFECT_NONE );

		GetNtlGuiManager()->AddUpdateFunc( this );		
		m_pflsUpgradeEffect->Show( false );
	}
	else
	{
		if( m_eState == STATE_PACKETWAIT )
			return -1;

		// 애니메이션을 IDLE로 변경        
		Logic_SetActorAnimation(m_hNPCSerial, NML_IDLE_LOOP, TRUE);

		GetNtlWorldConcept()->RemoveWorldPlayConcept( WORLD_PLAY_NPC_COMMU );
		GetDialogManager()->SwitchBag( FALSE );

		if( GetIconMoveManager()->GetSrcPlace() == PLACE_ITEMUPGRADE )
			GetIconMoveManager()->IconMoveEnd();

		if( m_pdlgResult->IsVisible() )
			DestroyResultDialog();

		Show( false );
		DeleteHoipoiStone();
		DeleteUpgradeItem();

		for( RwInt32 i = 0 ; i < NUM_SLOT ; ++i )
			m_aSlotEffect[i].SetState( stSLOTEFFECT::SLOT_EFFECT_NONE );

		m_TempItemSlot.SetState( stSLOTEFFECT::SLOT_EFFECT_NONE );
		m_BackFocus.SetState( stSLOTEFFECT::SLOT_EFFECT_NONE );

		CNtlSLEventGenerator::CameraControlDelete( CAMERA_CONTROL_NPC );
		GetNtlWorldConcept()->RemoveWorldPlayConcept( WORLD_PLAY_NPC_COMMU );

		GetNtlGuiManager()->RemoveUpdateFunc( this );
		m_pflsUpgradeEffect->Unload();
	}

	return 1;
}

VOID CItemUpgradeGui::HandleEvents( RWS::CMsg& msg )
{
	if( msg.Id == g_EventItemUpgradeResult )
	{
		SDboItemUpgradeResult* pData = reinterpret_cast<SDboItemUpgradeResult*>( msg.pData );

		SetResult( pData->eResult, pData->byBattleAttribute, pData->nGrade );
	}
	// peessitemp: 현재 없어져야 하는 이벤트 및 패킷.
	//else if( msg.Id == g_EventScouterUpgradePredict )
	//{
	//	SDboScouterUpgradePredict* pData = reinterpret_cast<SDboScouterUpgradePredict*>( msg.pData );

	//	ShowPredictMessage( pData->byPosNum, pData->byRetRate );
	//}
	else if( msg.Id == g_EventNPCDialogOpen )
	{
		SDboNpcDialogOpen* pData = reinterpret_cast<SDboNpcDialogOpen*>( msg.pData );

		if( pData->eDialog != DIALOG_ITEMUPGRADE )
			return;

		GetDialogManager()->OpenDialog( DIALOG_ITEMUPGRADE, GetNtlSLGlobal()->GetSobAvatar()->GetSerialID() );
		GetDialogManager()->SwitchBag( TRUE );
		m_hNPCSerial = pData->hSerialId;
	}
	else if( msg.Id == g_EventDialog )
	{
		SDboEventDialog* pData = reinterpret_cast<SDboEventDialog*>( msg.pData );
		if( pData->iType == DIALOGEVENT_NPC_BYEBYE && pData->iDestDialog == DIALOG_ITEMUPGRADE )
		{
			GetDialogManager()->CloseDialog( DIALOG_ITEMUPGRADE );
		}
	}
	else if( msg.Id == g_EventSobInfoUpdate )
	{
		SNtlEventSobInfoUpdate* pData = reinterpret_cast<SNtlEventSobInfoUpdate*>( msg.pData );

		if( pData->hSerialId != GetNtlSLGlobal()->GetSobAvatar()->GetSerialID() )
			return;

		if( pData->uiUpdateType & EVENT_AIUT_ITEM )
		{
			UpdateSlotIcon();			
		}
	}
	else if( msg.Id == g_EventIconMoveClick )
	{
		RwBool bPickUpState = (RwBool)msg.pData;
		ShowIconDestination( bPickUpState );
	}
	else if( msg.Id == g_EventSobItemDelete )
	{
		SNtlEventSobItemDelete* pData = reinterpret_cast<SNtlEventSobItemDelete*>( msg.pData );

		if( m_pUpgradeItem )
		{
			if( pData->hItemSerialId == m_pUpgradeItem->GetSerialID() )
				DeleteUpgradeItem();						
		}

		if( m_pHoipoiStone )
		{
			if( pData->hItemSerialId == m_pHoipoiStone->GetSerialID() )
				DeleteHoipoiStone();			
		}
	}
	else if( msg.Id == g_EventRegisterItemUpgrade )
	{
		SDboRegisterItemUpgrade* pData = reinterpret_cast<SDboRegisterItemUpgrade*>( msg.pData );

		CNtlSobItem* pItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( pData->hSrcSerial ) );
		if( pItem )
		{
			CNtlSobItemAttr* pItemAttr = reinterpret_cast<CNtlSobItemAttr*>( pItem->GetSobAttr() );

			if( IsUpgradableItem( pItemAttr ) )
			{
				SetItemSlot( pData->hSrcSerial, pData->eSrcPlace, pData->uiSrcSlotIdx, SLOT_ITEM, FALSE );
				RaiseTop();			
			}
			else if( IsHoipoiStone( pItemAttr ) )
			{
				SetItemSlot( pData->hSrcSerial, pData->eSrcPlace, pData->uiSrcSlotIdx, SLOT_HONEST, FALSE );				
				RaiseTop();			
			}
			else
				GetAlarmManager()->AlarmMessage( DST_ITEMUPGRADE_NOTIFY_CAN_INSERT_ONLY_UPGRADEITEM_OR_HOIPOISTONE );
		}		
	}
	else if( msg.Id == g_EventGameServerChangeOut )
	{
		GetDialogManager()->CloseDialog( DIALOG_ITEMUPGRADE );
	}
	else if(msg.Id == g_EventAnimEnd)
	{
		// 아이템 업그레이드 애니메이션은 한번 재생한후 Loop로 변경한다.
		SNtlEventAnimEnd* pData = (SNtlEventAnimEnd*)msg.pData;
		if(pData->uiSerialId == m_hNPCSerial && (pData->uiBaseAnimKey == SOC_NO || pData->uiBaseAnimKey == SOC_BYE))
		{
			Logic_SetActorAnimation(m_hNPCSerial, NML_IDLE_LOOP, TRUE);            
		}
	}
}

VOID CItemUpgradeGui::SetSlotRectHardCode(VOID)
{
	CRectangle rtScreen = m_pThis->GetScreenRect();

	m_rtSlot[SLOT_ITEM].SetRectWH( 144, 267, DBOGUI_ICON_SIZE, DBOGUI_ICON_SIZE );
	m_rtSlot[SLOT_HONEST].SetRectWH( 144, 201, DBOGUI_ICON_SIZE, DBOGUI_ICON_SIZE );
	m_rtSlot[SLOT_STRANGE].SetRectWH( 179, 327, DBOGUI_ICON_SIZE, DBOGUI_ICON_SIZE );
	m_rtSlot[SLOT_WILD].SetRectWH( 211, 263, DBOGUI_ICON_SIZE, DBOGUI_ICON_SIZE );
	m_rtSlot[SLOT_ELEGANT].SetRectWH( 109, 327, DBOGUI_ICON_SIZE, DBOGUI_ICON_SIZE );
	m_rtSlot[SLOT_FUNNY].SetRectWH( 76, 263, DBOGUI_ICON_SIZE, DBOGUI_ICON_SIZE );

	m_aSlotEffect[SLOT_ITEM].m_rtEffectSlot.SetRectWH( 110, 236, 101, 92 );
	m_aSlotEffect[SLOT_HONEST].m_rtEffectSlot.SetRectWH( 126, 171, 64, 72 );
	m_aSlotEffect[SLOT_STRANGE].m_rtEffectSlot.SetRectWH( 168, 317, 93, 76 );
	m_aSlotEffect[SLOT_WILD].m_rtEffectSlot.SetRectWH( 200, 244, 81, 62 );
	m_aSlotEffect[SLOT_ELEGANT].m_rtEffectSlot.SetRectWH( 61, 317, 90, 81 );
	m_aSlotEffect[SLOT_FUNNY].m_rtEffectSlot.SetRectWH( 36, 239, 84, 66 );
	m_TempItemSlot.m_rtEffectSlot = m_aSlotEffect[SLOT_ITEM].m_rtEffectSlot;

	m_aSlotEffect[SLOT_HONEST].m_surSlotEffect.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "ItemUpgrade.srf", "srfEffectHonest" ) );
	m_aSlotEffect[SLOT_STRANGE].m_surSlotEffect.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "ItemUpgrade.srf", "srfEffectStrange" ) );
	m_aSlotEffect[SLOT_WILD].m_surSlotEffect.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "ItemUpgrade.srf", "srfEffectWild" ) );
	m_aSlotEffect[SLOT_ELEGANT].m_surSlotEffect.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "ItemUpgrade.srf", "srfEffectElegant" ) );
	m_aSlotEffect[SLOT_FUNNY].m_surSlotEffect.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "ItemUpgrade.srf", "srfEffectFunny" ) );

	m_BackFocus.m_rtEffectSlot.SetRectWH( 15, 152, 290, 276 );
	m_BackFocus.m_surSlotEffect.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "ItemUpgrade.srf", "srfBackFocus" ) );

	for( RwInt32 i = 0 ; i < NUM_SLOT ; ++i )
	{
		m_surSlot[i].SetRectWH( rtScreen.left + m_rtSlot[i].left, rtScreen.top + m_rtSlot[i].top, DBOGUI_ICON_SIZE, DBOGUI_ICON_SIZE );
		m_asurFocus[i].SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfSlotFocusEffect" ) );
		m_asurFocus[i].SetRectWH( 0, 0, DBOGUI_ICON_SIZE, DBOGUI_ICON_SIZE );		
		m_asurDisableSlot[i].SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfSlotDisableEffect" ) );
		m_asurDisableSlot[i].SetRectWH( rtScreen.left + m_rtSlot[i].left, rtScreen.top + m_rtSlot[i].top, DBOGUI_ICON_SIZE, DBOGUI_ICON_SIZE );				
		m_aSlotEffect[i].m_surSlotEffect.Show( FALSE );
	}

	m_surUpgradeItemIcon.SetRectWH( 0, 0, DBOGUI_ICON_SIZE, DBOGUI_ICON_SIZE );
}	

VOID CItemUpgradeGui::SetBasicUISetting(VOID)
{
	m_pstbTitle->SetText( GetDisplayStringManager()->GetString( DST_ITEMUPGRADE_DLGTITLE ) );
	m_pstbInfoTitle->SetText( GetDisplayStringManager()->GetString( DST_ITEMUPGRADE_INFOTITLE ) );
	m_pstbPredictText->SetText( GetDisplayStringManager()->GetString( DST_ITEMUPGRADE_PREDICT ) );
	m_pstbDestroyPredictText->SetText( GetDisplayStringManager()->GetString( DST_ITEMUPGRADE_DESTROY_PREDICT ) );	
	m_pbtnCompound->SetText( GetDisplayStringManager()->GetString( DST_ITEMUPGRADE_BTN_COMPOUND ) );	

	m_pstbDestroyPredictResult->SetTextColor( INFOCOLOR_5 );
}

VOID CItemUpgradeGui::CreateStackNumber( RwInt32 nSlotIdx, RwInt32 nValue )
{
	m_pstbStackNum->SetPosition( m_rtSlot[nSlotIdx].left, m_rtSlot[nSlotIdx].bottom - DBOGUI_STACKNUM_HEIGHT );
	m_pstbStackNum->SetText( nValue );
	m_pstbStackNum->Show();	
}

VOID CItemUpgradeGui::DeleteStackNumber(VOID)
{
	m_pstbStackNum->Show( false );
}

RwInt32 CItemUpgradeGui::GetChildSlotIdx( RwInt32 nX, RwInt32 nY )
{
	for( RwInt32 i = 0 ; i < NUM_SLOT ; ++i )
	{
		if( m_rtSlot[i].PtInRect( nX, nY ) )
			return i;
	}

	return -1;
}

// Error Notify는 여기서 처리.
VOID CItemUpgradeGui::SetItemSlot( RwUInt32 hSerialID, RwUInt32 eSrcplace, RwUInt32 uiSrcSlotIdx, RwUInt32 uiDestSlotIdx, RwBool bNeedToIconMoveEnd )
{
	if( m_eState == STATE_UPGRADE_PROCESS ||
		m_eState == STATE_PACKETWAIT ||
		m_eState == STATE_RESULT )
	{
		GetAlarmManager()->AlarmMessage( DST_ITEMUPGRADE_NOTIFY_CANNOT_CONTROL_STATE );
		return;
	}

	CNtlSobItem* pItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( hSerialID ) );
	NTL_ASSERT( pItem, "CItemUpgradeGui::SetITemSlot : pItem is must be present!" );

	CNtlSobItemAttr* pItemAttr = reinterpret_cast<CNtlSobItemAttr*>( pItem->GetSobAttr() );
	RwInt32 nSrcPlace = eSrcplace;

	if( nSrcPlace == PLACE_ITEMUPGRADE )
	{
		RwInt32 nSrcSlotIdx = uiSrcSlotIdx;

		if( uiSrcSlotIdx == uiDestSlotIdx )
		{
			if( bNeedToIconMoveEnd )
				GetIconMoveManager()->IconMoveEnd();

			return;
		}

		if( uiDestSlotIdx == SLOT_ITEM )
		{
			GetAlarmManager()->AlarmMessage( DST_ITEMUPGRADE_NOTIFY_CAN_INSERT_ONLY_UPGRADEBLE_ITEM );
			return;
		}
		else
		{
			if( !IsHoipoiStone( pItemAttr ) )
			{
				GetAlarmManager()->AlarmMessage( DST_ITEMUPGRADE_NOTIFY_CAN_INSERT_ONLY_HOIPOISTONE );
				return;
			}

			SetHoipoiStone( uiDestSlotIdx, pItemAttr->GetStackNum(), pItem );

			if( m_eState == STATE_UPGRADE_READY )
				SetState( STATE_UPGRADE_READY );		// UpgradeInfo 갱신.
		}
	}
	else if( nSrcPlace == PLACE_BAG )
	{
		if( m_eState == STATE_NONE )
		{
			if( uiDestSlotIdx == SLOT_ITEM )
			{
				if( !IsUpgradableItem( pItemAttr ) )
				{
					GetAlarmManager()->AlarmMessage( DST_ITEMUPGRADE_NOTIFY_CAN_INSERT_ONLY_UPGRADEBLE_ITEM );
					return;
				}

				SetUpgradeItem( pItem );
				SetState( STATE_ITEM_IN );								
			}
			else 
			{
				if( !IsHoipoiStone( pItemAttr ) )
				{
					GetAlarmManager()->AlarmMessage( DST_ITEMUPGRADE_NOTIFY_CAN_INSERT_ONLY_HOIPOISTONE );
					return;
				}		

				SetHoipoiStone( uiDestSlotIdx, pItemAttr->GetStackNum(), pItem );
				SetState( STATE_HOIPOI_IN );
			}			
		}
		else if( m_eState == STATE_ITEM_IN )
		{
			if( uiDestSlotIdx == SLOT_ITEM )
			{
				if( IsUpgradableItem( pItemAttr ) )
				{
					GetAlarmManager()->AlarmMessage( DST_ITEMUPGRADE_NOTIFY_CANNOT_INSERT_DUPLEX );
					return;
				}
				else
				{
					GetAlarmManager()->AlarmMessage( DST_ITEMUPGRADE_NOTIFY_CAN_INSERT_ONLY_UPGRADEBLE_ITEM );
					return;
				}
			}
			else
			{
				if( !IsHoipoiStone( pItemAttr ) )
				{
					GetAlarmManager()->AlarmMessage( DST_ITEMUPGRADE_NOTIFY_CAN_INSERT_ONLY_HOIPOISTONE );
					return;
				}

				CNtlSobItemAttr* pUpgradeItemAttr = reinterpret_cast<CNtlSobItemAttr*>( m_pUpgradeItem->GetSobAttr() );

				SetHoipoiStone( uiDestSlotIdx, pItemAttr->GetStackNum(), pItem );

				if( IsValidHoipoiStone( pItemAttr ) )
				{
					SetState( STATE_UPGRADE_READY );
				}
				else
				{
					SetState( STATE_UPGRADE_READYFAIL_NOT_MATCHED_HOIPOISTONE );
				}				
			}				
		}
		else if( m_eState == STATE_HOIPOI_IN )
		{
			if( uiDestSlotIdx == SLOT_ITEM )
			{
				if( !IsUpgradableItem( pItemAttr ) )
				{
					GetAlarmManager()->AlarmMessage( DST_ITEMUPGRADE_NOTIFY_CAN_INSERT_ONLY_UPGRADEBLE_ITEM );
					return;
				}

				SetUpgradeItem( pItem );

				CNtlSobItemAttr* pHoipoiAttr = reinterpret_cast<CNtlSobItemAttr*>( m_pHoipoiStone->GetSobAttr() );

				if( IsValidHoipoiStone( pHoipoiAttr ) )
				{
					SetState( STATE_UPGRADE_READY );
				}
				else
				{
					SetState( STATE_UPGRADE_READYFAIL_NOT_MATCHED_HOIPOISTONE );
				}				
			}
			else
			{
				if( IsHoipoiStone( pItemAttr ) )
				{
					if( uiDestSlotIdx == m_nStoneSlotIdx )
					{
						GetAlarmManager()->AlarmMessage( DST_ITEMUPGRADE_NOTIFY_CANNOT_INSERT_DUPLEX );
						return;	
					}
					else
					{
						GetAlarmManager()->AlarmMessage( DST_ITEMUPGRADE_NOTIFY_CANNOT_INSERT_HOIPOISTONE );
						return;
					}				
				}
				else
				{
					GetAlarmManager()->AlarmMessage( DST_ITEMUPGRADE_NOTIFY_CAN_INSERT_ONLY_HOIPOISTONE );
					return;
				}				
			}
		}
		// 이 스테이트 들은 아이템 및 호이포이스톤이 둘다 들어있지만 업그레이드에 만족하지 않는 상태이다. 
		else if( m_eState == STATE_UPGRADE_READYFAIL_NOMOREUPGRADE ||
			m_eState == STATE_UPGRADE_READYFAIL_NOMOREDOWNGRADE ||
			m_eState == STATE_UPGRADE_READYFAIL_NOT_ENOUGH_HOIPOISTONE ||
			m_eState == STATE_UPGRADE_READYFAIL_NOT_MATCHED_HOIPOISTONE ) 
		{
			if( uiDestSlotIdx == SLOT_ITEM )
			{
				if( IsUpgradableItem( pItemAttr ) )
				{
					GetAlarmManager()->AlarmMessage( DST_ITEMUPGRADE_NOTIFY_CANNOT_INSERT_DUPLEX );
					return;
				}
				else
				{
					GetAlarmManager()->AlarmMessage( DST_ITEMUPGRADE_NOTIFY_CAN_INSERT_ONLY_UPGRADEBLE_ITEM );
					return;
				}
			}
			else
			{
				if( IsHoipoiStone( pItemAttr ) )
				{
					if( uiDestSlotIdx == m_nStoneSlotIdx )
					{
						GetAlarmManager()->AlarmMessage( DST_ITEMUPGRADE_NOTIFY_CANNOT_INSERT_DUPLEX );
						return;	
					}
					else
					{
						if( m_pHoipoiStone )
						{
							GetAlarmManager()->AlarmMessage( DST_ITEMUPGRADE_NOTIFY_CANNOT_INSERT_HOIPOISTONE );
							return;
						}
						// peessi : 이 밑으로 들어올리가 없다. -_-;;;;
						else
						{
							SetHoipoiStone( uiDestSlotIdx, pItemAttr->GetStackNum(), pItem );

							if( m_eState != STATE_UPGRADE_READYFAIL_NOMOREUPGRADE )
							{
								if( IsValidHoipoiStone( pItemAttr ) )
								{
									SetState( STATE_UPGRADE_READY );
								}
								else
								{
									SetState( STATE_UPGRADE_READYFAIL_NOT_MATCHED_HOIPOISTONE );
								}				
							}							
						}						
					}				
				}
				else
				{
					GetAlarmManager()->AlarmMessage( DST_ITEMUPGRADE_NOTIFY_CAN_INSERT_ONLY_HOIPOISTONE );
					return;
				}					
			}
		}
		else // Upgrade Ready
		{
			if( uiDestSlotIdx == SLOT_ITEM )
			{
				GetAlarmManager()->AlarmMessage( DST_ITEMUPGRADE_NOTIFY_CANNOT_INSERT_DUPLEX );
			}
			else
			{
				GetAlarmManager()->AlarmMessage( DST_ITEMUPGRADE_NOTIFY_CANNOT_INSERT_HOIPOISTONE );
			}

			return;
		}	

		CDboEventGenerator::DialogEvent( DIALOGEVENT_BEGIN_UPGRADE_ITEM_IN_BAG, PLACE_ITEMUPGRADE, PLACE_BAG, pItem->GetParentItemSlotIdx(), pItem->GetItemSlotIdx() );
	}
	else
	{
		return;
	}	

	// 
	UpdateSlotIcon();

	// 아이콘 무브 완료
	if( bNeedToIconMoveEnd )
		GetIconMoveManager()->IconMoveEnd();
}

VOID CItemUpgradeGui::UpdateSlotIcon(VOID)
{
	// StackCount 변경
	if( m_pHoipoiStone )
	{
		CNtlSobItemAttr* pAttr = reinterpret_cast<CNtlSobItemAttr*>( m_pHoipoiStone->GetSobAttr() );
		CreateStackNumber( m_nStoneSlotIdx, pAttr->GetStackNum() );
	}

	// Info Wnd 변경
	if( m_nMouseOnIdx >= 0 && GetInfoWndManager()->GetRequestGui() == DIALOG_ITEMUPGRADE )
	{
		ShowInfoWnd( TRUE, m_nMouseOnIdx );					
	}

	// UpgradeReady상태시 스카우터 파츠 관련해서 바뀌었으면 다시 세팅
	if( m_eState == STATE_UPGRADE_READY && m_fScouterBonus != CalcBonusFromScouterParts() )
	{
		SetState( STATE_UPGRADE_READY );
	}
}

VOID CItemUpgradeGui::SetUpgradeItem( CNtlSobItem* pUpgradeItem )
{
	if( !pUpgradeItem )
	{
		NTL_ASSERTFAIL( "CItemUpgradeGui::SetUpgrade : Slot Fail or Invalid Icon");
		return;
	}

	m_pUpgradeItem = pUpgradeItem;
	m_surSlot[SLOT_ITEM].SetTexture( (gui::CTexture*)pUpgradeItem->GetIcon()->GetImage() );	

	SetAttributeSlotEffect( SLOT_ITEM, TRUE );
}

VOID CItemUpgradeGui::SetHoipoiStone( RwInt32 nSlotIdx, RwInt32 nStackCount, CNtlSobItem* pHoipoiStone )
{
	if( !pHoipoiStone || ! IsValidIdx( nSlotIdx ) )
	{
		NTL_ASSERTFAIL( "CItemUpgradeGui::SetHoipoiStone : Slot Fail or Invalid Icon");
		return;
	}

	// 아이템 업그레이드 내부에서 움직일때 이전 이펙트를 지운다. DeleteHoipoiStone은 업그레이드 창에서 사라질때만 호출된다. 
	if( m_pHoipoiStone )
	{
		m_surSlot[nSlotIdx].UnsetTexture();
		SetAttributeSlotEffect( m_nStoneSlotIdx, FALSE );		
	}

	m_pHoipoiStone = pHoipoiStone;
	m_nStoneSlotIdx = nSlotIdx;
	m_nStoneStackNum = nStackCount;
	m_surSlot[nSlotIdx].SetTexture( (gui::CTexture*)pHoipoiStone->GetIcon()->GetImage() );
	CreateStackNumber( nSlotIdx, nStackCount );		

	SetAttributeSlotEffect( nSlotIdx, TRUE );

	// 호이포이스톤의 위치 변화에 따라 달라짐.
	if( m_eState == STATE_UPGRADE_READY )
		ShowPredictMessage();	

	// 블랙호이포이 스톤이라면 텍스트를 다운그레이드로
	CNtlSobItemAttr* pHoipoiAttr = reinterpret_cast<CNtlSobItemAttr*>( m_pHoipoiStone->GetSobAttr() );
	if( pHoipoiAttr->GetItemTbl()->byItem_Type == ITEM_TYPE_BLACK_STONE )
		m_pbtnCompound->SetText( GetDisplayStringManager()->GetString( DST_ITEMUPGRADE_BTN_COMPOUND_DOWN ) );
	else
		m_pbtnCompound->SetText( GetDisplayStringManager()->GetString( DST_ITEMUPGRADE_BTN_COMPOUND ) );

	Logic_PlayGUISound( GSD_SYSTEM_ITEM_LIGHT_ON );
}

VOID CItemUpgradeGui::DeleteUpgradeItem(VOID)
{
	if( m_pUpgradeItem )
	{
		CDboEventGenerator::DialogEvent( DIALOGEVENT_END_UPGRADE_ITEM_IN_BAG, PLACE_ITEMUPGRADE, PLACE_BAG, 
			m_pUpgradeItem->GetParentItemSlotIdx(), m_pUpgradeItem->GetItemSlotIdx() );

		m_surSlot[SLOT_ITEM].SetTexture( (gui::CTexture*)NULL );
		m_pUpgradeItem = NULL;	

		SetAttributeSlotEffect( SLOT_ITEM, FALSE );
	}		

	if( m_eState != STATE_RESULT )
	{
		if( m_pHoipoiStone )
			SetState( STATE_HOIPOI_IN );
		else
			SetState( STATE_NONE );
	}	
}

VOID CItemUpgradeGui::DeleteHoipoiStone(VOID)
{
	if( m_pHoipoiStone )
	{
		CDboEventGenerator::DialogEvent( DIALOGEVENT_END_UPGRADE_ITEM_IN_BAG, PLACE_ITEMUPGRADE, PLACE_BAG, 
			m_pHoipoiStone->GetParentItemSlotIdx(), m_pHoipoiStone->GetItemSlotIdx() );

		// 순서 매우 중요. m_pHoipoiStone 의 NULL값으로 SetAttribute에서 완전히 호이포이스톤이 사라진걸 판별함.
		m_pHoipoiStone = NULL;
		DeleteStackNumber();		
		SetAttributeSlotEffect( m_nStoneSlotIdx, FALSE );
		m_surSlot[m_nStoneSlotIdx].SetTexture( (gui::CTexture*)NULL );		
		m_nStoneSlotIdx = -1;				
	}	

	// Result 상태에서는 닫기 버튼 누를때 알아서 처리한다.
	if( m_eState != STATE_RESULT )
	{
		if( m_pUpgradeItem )
		{
			//CNtlSobItemAttr* pUpgradeItemAttr = reinterpret_cast<CNtlSobItemAttr*>( m_pUpgradeItem->GetSobAttr() );

			// 이미 등록되어 있는 경우에 업그레이드 가능하지 않은건 더이상 그레이드를 올릴 수 없는 경우 뿐.
			//if( IsUpgradableItem( pUpgradeItemAttr ) )
			SetState( STATE_ITEM_IN );
			//else
			//	SetState( STATE_UPGRADE_READYFAIL_NOMOREUPGRADE );
		}
		else
			SetState( STATE_NONE );
	}	

	// 혹시 다운그레이드라고 되어 있는 경우 원상태로.
	m_pbtnCompound->SetText( GetDisplayStringManager()->GetString( DST_ITEMUPGRADE_BTN_COMPOUND ) );
}

// peessiupgrade : 사라지는 경우도 받아서 처리해야 한다. 
VOID CItemUpgradeGui::SetResult( RwInt32 eResult, RwUInt8 byBattleAttribute, RwInt32 nGrade )
{
	NTL_ASSERT( m_pUpgradeItem, "CItemUpgradeGui::HandleEvents : UpgradeItem Must be Present!" );
	CNtlSobItemAttr* pAttr = reinterpret_cast<CNtlSobItemAttr*>( m_pUpgradeItem->GetSobAttr() );

	// execption.. --;; 에러메시지는 패킷핸들러에서 처리된다.
	if( eResult == SDboItemUpgradeResult::SYSTEM_FAIL )
	{
		SetState( STATE_UPGRADE_READY );
		return;
	}

	// 아이템 이름
	RwUInt32 uiColor = Logic_GetItemRankColor( pAttr->GetRank() );
	std::wstring strBuf = API_GetTableContainer()->GetTextAllTable()->GetItemTbl()->GetText( pAttr->GetItemTbl()->Name );
	if( nGrade > 0 )
	{
		WCHAR buf[16];
		swprintf_s( buf, 16, L" + %d", nGrade );
		strBuf += buf;
	}
	m_pstbUpgradeItemName->SetTextColor( uiColor );
	m_pstbUpgradeItemName->SetText( strBuf.c_str() );

	// 아이템 아이콘
	if( eResult == SDboItemUpgradeResult::FAIL_DESTROY_ITEM )
		m_surUpgradeItemIcon.SetTexture( Logic_CreateTexture( pAttr->GetItemTbl()->szIcon_Name ), TRUE );
	else
		m_surUpgradeItemIcon.SetTexture( reinterpret_cast<gui::CTexture*>( m_pUpgradeItem->GetIcon()->GetImage() ) );

	if( eResult == SDboItemUpgradeResult::SUCCESS )
	{
		if( pAttr->GetGrade() < nGrade )
		{
			if( nGrade - pAttr->GetGrade() == 1 )
				m_pflsResult->Load( ITEMUPGRADEGUI_SUCCESS_FLASHFILE );
			else if( nGrade - pAttr->GetGrade() == 2 )
				m_pflsResult->Load( ITEMUPGRADEGUI_SUCCESS_FLASHFILE_2 );
			else if( nGrade - pAttr->GetGrade() == 3 )
				m_pflsResult->Load( ITEMUPGRADEGUI_SUCCESS_FLASHFILE_3 );
			else
				m_pflsResult->Load( ITEMUPGRADEGUI_SUCCESS_FLASHFILE );

			m_pstbResult->SetText( GetDisplayStringManager()->GetString( DST_ITEMUPGRADE_RESULT_MSG_SUCCESS ) );
		}
		else
		{
			m_pflsResult->Load( ITEMUPGRADEGUI_DOWNGRADE_SUCCESS_FLASHFILE );
			m_pstbResult->SetText( GetDisplayStringManager()->GetString( DST_ITEMUPGRADE_RESULT_MSG_DOWNGRADE_SUCCESS ) );
		}

		m_pflsResult->PlayMovie( TRUE );

		// NPC 애니메이션
		Logic_SetActorAnimation(m_hNPCSerial, SOC_NO, FALSE);      
		// 아바타 액션은 브로드캐스팅해야한다.
		CNtlSLEventGenerator::SobAnimPlay( GetNtlSLGlobal()->GetSobAvatar()->GetSerialID(), SOC_HAPPY, FALSE, 0.0f );
	}
	else if( eResult == SDboItemUpgradeResult::FAIL ||
		eResult == SDboItemUpgradeResult::FAIL_DESTROY_ITEM )
	{	
		m_pflsResult->Load( ITEMUPGRADEGUI_FAIL_FLASHFILE );
		m_pflsResult->PlayMovie( TRUE );
		m_pstbResult->SetText( GetDisplayStringManager()->GetString( DST_ITEMUPGRADE_RESULT_MSG_FAIL ) );

		// NPC 애니메이션
		Logic_SetActorAnimation(m_hNPCSerial, SOC_BYE, FALSE);        
		// 아바타 액션은 브로드캐스팅해야한다.
		CNtlSLEventGenerator::SobAnimPlay( GetNtlSLGlobal()->GetSobAvatar()->GetSerialID(), SOC_SAD, FALSE, 0.0f );
	}		

	WCHAR wBuf[256];
	std::wstring wstrInfo;

	if( eResult == SDboItemUpgradeResult::FAIL_DESTROY_ITEM )
	{
		wstrInfo = GetDisplayStringManager()->GetString( DST_ITEMUPGRADE_RESULT_REPORT_ITEM_DESTROY );
		m_pstbResultInfo->SetTextColor( INFOCOLOR_5 );
	}
	else
	{
		// Grade
		if( pAttr->GetGrade() < nGrade )
		{
			swprintf_s( wBuf, 256, GetDisplayStringManager()->GetString( DST_ITEMUPGRADE_RESULT_REPORT_UPGRADE ), pAttr->GetGrade(), nGrade );		
			wstrInfo = wBuf;
			m_pstbResultInfo->SetTextColor( INFOCOLOR_1 );
		}
		else if( pAttr->GetGrade() > nGrade )
		{
			swprintf_s( wBuf, 256, GetDisplayStringManager()->GetString( DST_ITEMUPGRADE_RESTLT_REPORT_DOWNGRADE ), pAttr->GetGrade(), nGrade );
			wstrInfo = wBuf;
			m_pstbResultInfo->SetTextColor( INFOCOLOR_6 );
		}

		// Value
		RwUInt16 wPhysicalOffence = 0;
		RwUInt16 wEnergyOffence = 0;
		RwUInt16 wPhysicalDefence = 0;
		RwUInt16 wEnergyDefence = 0;
		RwUInt16 wBeforePhysicalOffence = 0;
		RwUInt16 wBeforeEnergyOffence = 0;
		RwUInt16 wBeforePhysicalDefence = 0;
		RwUInt16 wBeforeEnergyDefence = 0;

		if( EQUIP_TYPE_MAIN_WEAPON == pAttr->GetItemTbl()->byEquip_Type ||
			EQUIP_TYPE_SUB_WEAPON == pAttr->GetItemTbl()->byEquip_Type )			
		{
			if( pAttr->GetItemTbl()->wPhysical_Offence != INVALID_WORD )
			{
				wPhysicalOffence = Logic_GetFinalItemValue( pAttr->GetItemTbl()->wPhysical_Offence, nGrade, pAttr->GetItemTbl()->dwPhysical_OffenceUpgrade );
				wBeforePhysicalOffence = Logic_GetFinalItemValue( pAttr->GetItemTbl()->wPhysical_Offence, pAttr->GetGrade(), pAttr->GetItemTbl()->dwPhysical_OffenceUpgrade );

				wstrInfo += L"\\n";
				if( wBeforePhysicalOffence < wPhysicalOffence )
				{
					swprintf_s( wBuf, 256, GetDisplayStringManager()->GetString( DST_ITEMUPGRADE_RESULT_REPORT_STATUS_PHYSICAL_OFFENCE_UP ), wBeforePhysicalOffence, wPhysicalOffence );
					wstrInfo += wBuf;
				}
				else if( wBeforePhysicalOffence > wPhysicalOffence )
				{
					swprintf_s( wBuf, 256, GetDisplayStringManager()->GetString( DST_ITEMUPGRADE_RESULT_REPORT_STATUS_PHYSICAL_OFFENCE_DOWN ), wBeforePhysicalOffence, wPhysicalOffence );
					wstrInfo += wBuf;
				}
				else
				{
					wstrInfo += GetDisplayStringManager()->GetString( DST_ITEMUPGRADE_RESULT_REPORT_NO_PHYSICAL_OFFENCE_CHANGE );
				}
			}

			if( pAttr->GetItemTbl()->wEnergy_Offence != INVALID_WORD )
			{
				wEnergyOffence = Logic_GetFinalItemValue( pAttr->GetItemTbl()->wEnergy_Offence, nGrade, pAttr->GetItemTbl()->dwEnergy_OffenceUpgrade );
				wBeforeEnergyOffence = Logic_GetFinalItemValue( pAttr->GetItemTbl()->wEnergy_Offence, pAttr->GetGrade(), pAttr->GetItemTbl()->dwEnergy_OffenceUpgrade );

				wstrInfo += L"\\n";
				if( wBeforeEnergyOffence < wEnergyOffence )
				{
					swprintf_s( wBuf, 256, GetDisplayStringManager()->GetString( DST_ITEMUPGRADE_RESULT_REPORT_STATUS_ENERGY_OFFENCE_UP ), wBeforeEnergyOffence, wEnergyOffence );
					wstrInfo += wBuf;
				}
				else if( wBeforeEnergyOffence > wEnergyOffence )
				{
					swprintf_s( wBuf, 256, GetDisplayStringManager()->GetString( DST_ITEMUPGRADE_RESULT_REPORT_STATUS_ENERGY_OFFENCE_DOWN ), wBeforeEnergyOffence, wEnergyOffence );
					wstrInfo += wBuf;
				}
				else
				{
					wstrInfo += GetDisplayStringManager()->GetString( DST_ITEMUPGRADE_RESULT_REPORT_NO_ENERGY_OFFENCE_CHANGE );
				}
			}
		}
		else
		{
			if( pAttr->GetItemTbl()->wPhysical_Defence != INVALID_WORD )
			{
				wPhysicalDefence = Logic_GetFinalItemValue( pAttr->GetItemTbl()->wPhysical_Defence, nGrade, pAttr->GetItemTbl()->dwPhysical_DefenceUpgrade );
				wBeforePhysicalDefence = Logic_GetFinalItemValue( pAttr->GetItemTbl()->wPhysical_Defence, pAttr->GetGrade(), pAttr->GetItemTbl()->dwPhysical_DefenceUpgrade );

				wstrInfo += L"\\n";
				if( wBeforePhysicalDefence < wPhysicalDefence )
				{
					swprintf_s( wBuf, 256, GetDisplayStringManager()->GetString( DST_ITEMUPGRADE_RESULT_REPORT_STATUS_PHYSICAL_DEFENCE_UP ), wBeforePhysicalDefence, wPhysicalDefence );
					wstrInfo += wBuf;
				}
				else if( wBeforePhysicalDefence > wPhysicalDefence )
				{
					swprintf_s( wBuf, 256, GetDisplayStringManager()->GetString( DST_ITEMUPGRADE_RESULT_REPORT_STATUS_PHYSICAL_DEFENCE_DOWN ), wBeforePhysicalDefence, wPhysicalDefence );
					wstrInfo += wBuf;
				}
				else
				{
					wstrInfo += GetDisplayStringManager()->GetString( DST_ITEMUPGRADE_RESULT_REPORT_NO_PHYSICAL_DEFENCE_CHANGE );
				}
			}

			if( pAttr->GetItemTbl()->wEnergy_Defence != INVALID_WORD )
			{
				wEnergyDefence = Logic_GetFinalItemValue( pAttr->GetItemTbl()->wEnergy_Defence, nGrade, pAttr->GetItemTbl()->dwEnergy_DefenceUpgrade );
				wBeforeEnergyDefence = Logic_GetFinalItemValue( pAttr->GetItemTbl()->wEnergy_Defence, pAttr->GetGrade(), pAttr->GetItemTbl()->dwEnergy_DefenceUpgrade );

				wstrInfo += L"\\n";
				if( wBeforeEnergyDefence < wEnergyDefence )
				{
					swprintf_s( wBuf, 256, GetDisplayStringManager()->GetString( DST_ITEMUPGRADE_RESULT_REPORT_STATUS_ENERGY_DEFENCE_UP ), wBeforeEnergyDefence, wEnergyDefence );
					wstrInfo += wBuf;
				}
				else if( wBeforeEnergyDefence > wEnergyDefence )
				{
					swprintf_s( wBuf, 256, GetDisplayStringManager()->GetString( DST_ITEMUPGRADE_RESULT_REPORT_STATUS_ENERGY_DEFENCE_DOWN ), wBeforeEnergyDefence, wEnergyDefence );
					wstrInfo += wBuf;
				}
				else
				{
					wstrInfo += GetDisplayStringManager()->GetString( DST_ITEMUPGRADE_RESULT_REPORT_NO_ENERGY_DEFENCE_CHANGE );
				}
			}		
		}

		wstrInfo += L"\\n";
		// Attribute
		if( pAttr->GetBattleAttribute() != byBattleAttribute )
		{
			swprintf_s( wBuf, 256, GetDisplayStringManager()->GetString( DST_ITEMUPGRADE_RESULT_REPORT_ATTRIBUTE ), 
				GetDisplayStringManager()->GetString( DST_ITEM_ATTR_HONEST + pAttr->GetBattleAttribute() - 1 ),
				GetDisplayStringManager()->GetString( DST_ITEM_ATTR_HONEST + byBattleAttribute - 1 ) );
			wstrInfo += wBuf;
		}
		else
		{
			wstrInfo += GetDisplayStringManager()->GetString( DST_ITEMUPGRADE_RESULT_REPORT_NO_ATTRIBUTE_CHANGE );
		}
	}	

	m_pstbResultInfo->SetText( wstrInfo.c_str() );
	SetState( STATE_RESULT );
}

VOID CItemUpgradeGui::SetState( STATE eState )
{
	m_pbtnExit->ClickEnable( TRUE );
	m_pbtnCompound->ClickEnable( FALSE );
	m_ppnlPredict->Show( false );
	DisableSlot( FALSE );

	const WCHAR* pInfo;
	WCHAR wBuf[ITEMUPGRADE_TEXTBUFFER];

	switch( eState )
	{
	case STATE_NONE:
		{
			// Info
			pInfo = GetDisplayStringManager()->GetString( DST_ITEMUPGRADE_MSG_SLOT_IS_EMPTY );
			m_phtmlInfoText->SetHtmlFromMemory( pInfo, wcslen( pInfo ) );

			if( GetIconMoveManager()->IsActive() )
				ShowIconDestination( TRUE );			

			if( m_BackFocus.m_eSlotEffect != stSLOTEFFECT::SLOT_EFFECT_NONE )
				m_BackFocus.SetState( stSLOTEFFECT::SLOT_EFFECT_OFF );
		}
		break;
	case STATE_ITEM_IN:
		{
			// Info
			CNtlSobItemAttr* pItemAttr = reinterpret_cast<CNtlSobItemAttr*>( m_pUpgradeItem->GetSobAttr() );
			std::wstring wstrItemName = API_GetTableContainer()->GetTextAllTable()->GetItemTbl()->GetText( pItemAttr->GetItemTbl()->Name );

			if( pItemAttr->GetGrade() == 0 )
			{
				pInfo = GetDisplayStringManager()->GetString( DST_ITEMUPGRADE_MSG_MINLEVEL_ITEM_IN );
				m_phtmlInfoText->SetHtmlFromMemory( pInfo, wcslen( pInfo ) );			
			}
			else if( pItemAttr->GetGrade() == NTL_ITEM_MAX_GRADE )
			{
				pInfo = GetDisplayStringManager()->GetString( DST_ITEMUPGRADE_MSG_MAXLEVEL_ITEM_IN );
				m_phtmlInfoText->SetHtmlFromMemory( pInfo, wcslen( pInfo ) );			
			}
			else
			{
				pInfo = GetDisplayStringManager()->GetString( DST_ITEMUPGRADE_MSG_ITEM_IN );
				swprintf_s( wBuf, ITEMUPGRADE_TEXTBUFFER, pInfo, Dbo_GetHoipoiStoneCount( ITEM_TYPE_STONE, pItemAttr->GetItemTbl()->byItem_Type, pItemAttr->GetGrade() ),
					Dbo_GetHoipoiStoneCount( ITEM_TYPE_PURE_STONE, pItemAttr->GetItemTbl()->byItem_Type, pItemAttr->GetGrade() ),
					Dbo_GetHoipoiStoneCount(ITEM_TYPE_BLACK_STONE, pItemAttr->GetItemTbl()->byItem_Type, pItemAttr->GetGrade() ) );
				m_phtmlInfoText->SetHtmlFromMemory( wBuf, wcslen( wBuf ) );			
			}			

			if( m_BackFocus.m_eSlotEffect != stSLOTEFFECT::SLOT_EFFECT_NONE )
				m_BackFocus.SetState( stSLOTEFFECT::SLOT_EFFECT_OFF );
		}		
		break;
	case STATE_HOIPOI_IN:
		{
			// Info
			CNtlSobItemAttr* pHoipoiAttr = reinterpret_cast<CNtlSobItemAttr*>( m_pHoipoiStone->GetSobAttr() );
			RwUInt8 byHoipoiStoneType = pHoipoiAttr->GetItemTbl()->byItem_Type;

			if( byHoipoiStoneType == ITEM_TYPE_BLACK_STONE )
				pInfo = GetDisplayStringManager()->GetString( DST_ITEMUPGRADE_MSG_BLACK_HOIPOISTONE_IN );
			else if( byHoipoiStoneType == ITEM_TYPE_PURE_STONE )
				pInfo = GetDisplayStringManager()->GetString( DST_ITEMUPGRADE_MSG_PURE_HOIPOISTONE_IN );
			else
				pInfo = GetDisplayStringManager()->GetString( DST_ITEMUPGRADE_MSG_HOIPOISTONE_IN );

			m_phtmlInfoText->SetHtmlFromMemory( pInfo, wcslen( pInfo ) );

			if( m_BackFocus.m_eSlotEffect != stSLOTEFFECT::SLOT_EFFECT_NONE )
				m_BackFocus.SetState( stSLOTEFFECT::SLOT_EFFECT_OFF );
		}		
		break;
	case STATE_UPGRADE_READYFAIL_NOT_ENOUGH_HOIPOISTONE:
		{
			// Info
			CNtlSobItemAttr* pItemAttr = reinterpret_cast<CNtlSobItemAttr*>( m_pUpgradeItem->GetSobAttr() );
			CNtlSobItemAttr* pHoipoiAttr = reinterpret_cast<CNtlSobItemAttr*>( m_pHoipoiStone->GetSobAttr() );
			std::wstring wstrItemName = API_GetTableContainer()->GetTextAllTable()->GetItemTbl()->GetText( pItemAttr->GetItemTbl()->Name );
			std::wstring wstrHoipoiStoneName = API_GetTableContainer()->GetTextAllTable()->GetItemTbl()->GetText( pHoipoiAttr->GetItemTbl()->Name );				
			RwUInt8 byHoipoiStoneType = pHoipoiAttr->GetItemTbl()->byItem_Type;

			if( byHoipoiStoneType == ITEM_TYPE_BLACK_STONE )
				pInfo = GetDisplayStringManager()->GetString( DST_ITEMUPGRADE_MSG_BLACKHOIPOISTONE_IN_FAIL_QUANTITY );
			else if( byHoipoiStoneType == ITEM_TYPE_PURE_STONE )
				pInfo = GetDisplayStringManager()->GetString( DST_ITEMUPGRADE_MSG_PUREHOIPOISTONE_IN_FAIL_QUANTITY );
			else
				pInfo = GetDisplayStringManager()->GetString( DST_ITEMUPGRADE_MSG_HOIPOISTONE_IN_FAIL_QUANTITY );

			swprintf_s( wBuf, 1024, pInfo, wstrHoipoiStoneName.c_str(), wstrHoipoiStoneName.c_str(), Dbo_GetHoipoiStoneCount( pHoipoiAttr->GetItemTbl()->byItem_Type, pItemAttr->GetItemTbl()->byItem_Type, pItemAttr->GetGrade() ) ); 
			m_phtmlInfoText->SetHtmlFromMemory( wBuf, wcslen( wBuf ) );			

			if( m_BackFocus.m_eSlotEffect != stSLOTEFFECT::SLOT_EFFECT_NONE )
				m_BackFocus.SetState( stSLOTEFFECT::SLOT_EFFECT_OFF );
		}	
		break;
	case STATE_UPGRADE_READYFAIL_NOT_MATCHED_HOIPOISTONE:
		{
			// Info
			pInfo = GetDisplayStringManager()->GetString( DST_ITEMUPGRADE_MSG_HOIPOISTONE_IN_FAIL_CLASS );
			m_phtmlInfoText->SetHtmlFromMemory( pInfo, wcslen( pInfo ) );			

			if( m_BackFocus.m_eSlotEffect != stSLOTEFFECT::SLOT_EFFECT_NONE )
				m_BackFocus.SetState( stSLOTEFFECT::SLOT_EFFECT_OFF );
		}
		break;
	case STATE_UPGRADE_READYFAIL_NOMOREUPGRADE:
		{
			// Info			
			pInfo = GetDisplayStringManager()->GetString( DST_ITEMUPGRADE_MSG_UNUPGRADABLE_ITEM );
			m_phtmlInfoText->SetHtmlFromMemory( pInfo, wcslen( pInfo ) );

			if( m_BackFocus.m_eSlotEffect != stSLOTEFFECT::SLOT_EFFECT_NONE )
				m_BackFocus.SetState( stSLOTEFFECT::SLOT_EFFECT_OFF );
		}
		break;
	case STATE_UPGRADE_READYFAIL_NOMOREDOWNGRADE:
		{
			pInfo = GetDisplayStringManager()->GetString( DST_ITEMUPGRADE_MSG_UNDOWNGRADABLE_ITEM );
			m_phtmlInfoText->SetHtmlFromMemory( pInfo, wcslen( pInfo ) );

			if( m_BackFocus.m_eSlotEffect != stSLOTEFFECT::SLOT_EFFECT_NONE )
				m_BackFocus.SetState( stSLOTEFFECT::SLOT_EFFECT_OFF );
		}
		break;
	case STATE_UPGRADE_READY:
		{
			// Info
			CNtlSobItemAttr* pItemAttr = reinterpret_cast<CNtlSobItemAttr*>( m_pUpgradeItem->GetSobAttr() );
			CNtlSobItemAttr* pHoipoiAttr = reinterpret_cast<CNtlSobItemAttr*>( m_pHoipoiStone->GetSobAttr() );
			
			// HoipoiStone에 따라서 다운그레이드 가능여부, 업그레이드 가능여부를 따져서 상태를 바꾼다.
			if( pItemAttr->GetGrade() == NTL_ITEM_MAX_GRADE && 
				( pHoipoiAttr->GetItemTbl()->byItem_Type == ITEM_TYPE_STONE || pHoipoiAttr->GetItemTbl()->byItem_Type == ITEM_TYPE_PURE_STONE ) )
			{
				SetState( STATE_UPGRADE_READYFAIL_NOMOREUPGRADE );
				return;
			}
			else if( pItemAttr->GetGrade() == 0 && pHoipoiAttr->GetItemTbl()->byItem_Type == ITEM_TYPE_BLACK_STONE )
			{
				SetState( STATE_UPGRADE_READYFAIL_NOMOREDOWNGRADE );
				return;
			}

			// 인덱스 오류로 Invalid가 날라올 확률은 없다. 
			// 아이템이 0그레이드, 블랙호이포이스톤을 넣었을때 INVALID_INDEX가 날라올테지만, 위에서 미리 처리됨.
			if( Dbo_GetHoipoiStoneCount( pHoipoiAttr->GetItemTbl()->byItem_Type, pItemAttr->GetItemTbl()->byItem_Type, pItemAttr->GetGrade() ) > pHoipoiAttr->GetStackNum() )
			{
				SetState( STATE_UPGRADE_READYFAIL_NOT_ENOUGH_HOIPOISTONE );
				return;
			}			

			if( pHoipoiAttr->GetItemTbl()->byItem_Type == ITEM_TYPE_BLACK_STONE )
				pInfo = GetDisplayStringManager()->GetString( DST_ITEMUPGRADE_MSG_DOWNGRADEREADY );
			else
				pInfo = GetDisplayStringManager()->GetString( DST_ITEMUPGRADE_MSG_UPGRADEREADY );

			m_fScouterBonus = CalcBonusFromScouterParts();
			m_fDestroyProbability = Dbo_ItemDestroyRateByUpgradeFail( pItemAttr->GetRank(), pItemAttr->GetBattleAttribute(), CONV_TO_POSNUM( m_nStoneSlotIdx ), m_fScouterBonus ) + 0.5f; 

			RwUInt16 wPhysicalValue = 0;
			RwUInt16 wEnergyValue = 0;
			RwUInt16 wNextPhysicalValue = 0;
			RwUInt16 wNextEnergyValue = 0;
			RwUInt16 wFailPhysicalValue = 0;
			RwUInt16 wFailEnergyValue = 0;
			RwUInt16 wPureMaxValue = 0;
			RwUInt8	 byGrade = 0;
			WCHAR* pString;
			WCHAR wParameter[ITEMUPGRADE_TEXTBUFFER] = { 0, };
			std::wstring wstrSuccessParameter;
			std::wstring wstrFailParameter;
			std::wstring wstrAttr;
			RwBool bSameSuccessParameter = TRUE;
			RwBool bSameFailParameter = TRUE;
			RwUInt8 byNextGrade = pItemAttr->GetGrade();
			RwUInt8 byPureMaxGrade = pItemAttr->GetGrade();

			if( byNextGrade + 1 <= NTL_ITEM_MAX_GRADE )
				++byNextGrade;

			if( byPureMaxGrade + ITEMUPGRADEGUI_MAX_PURE_GRADE <= NTL_ITEM_MAX_GRADE )
				byPureMaxGrade += ITEMUPGRADEGUI_MAX_PURE_GRADE;
			else
				byPureMaxGrade = NTL_ITEM_MAX_GRADE;

			if( EQUIP_TYPE_MAIN_WEAPON == pItemAttr->GetItemTbl()->byEquip_Type ||
				EQUIP_TYPE_SUB_WEAPON == pItemAttr->GetItemTbl()->byEquip_Type )				
			{
				pString = const_cast<WCHAR*>( GetDisplayStringManager()->GetString( DST_ITEMUPGRADE_INFO_OFFENCE ) );

				if( pItemAttr->GetItemTbl()->wPhysical_Offence != INVALID_WORD )
				{
					byGrade = pItemAttr->GetGrade();

					wPhysicalValue = Logic_GetFinalItemValue( pItemAttr->GetItemTbl()->wPhysical_Offence, byGrade, pItemAttr->GetItemTbl()->dwPhysical_OffenceUpgrade );
					wNextPhysicalValue = Logic_GetFinalItemValue( pItemAttr->GetItemTbl()->wPhysical_Offence, byNextGrade, pItemAttr->GetItemTbl()->dwPhysical_OffenceUpgrade );
					wPureMaxValue = Logic_GetFinalItemValue( pItemAttr->GetItemTbl()->wPhysical_Offence, byPureMaxGrade, pItemAttr->GetItemTbl()->dwPhysical_OffenceUpgrade );

					if( pHoipoiAttr->GetItemTbl()->byItem_Type == ITEM_TYPE_PURE_STONE && wNextPhysicalValue != wPureMaxValue )
					{
						swprintf_s( wParameter, ITEMUPGRADE_TEXTBUFFER, const_cast<WCHAR*>( GetDisplayStringManager()->GetString( DST_ITEMUPGRADE_MSG_PURE_PHYSICAL_UP ) ),
							pString, wPhysicalValue, wNextPhysicalValue, wPureMaxValue );

						if( wPhysicalValue != wNextPhysicalValue || wPhysicalValue != wPureMaxValue )
							bSameSuccessParameter = FALSE;
					}
					else
					{
						swprintf_s( wParameter, ITEMUPGRADE_TEXTBUFFER, const_cast<WCHAR*>( GetDisplayStringManager()->GetString( DST_ITEMUPGRADE_MSG_PHYSICAL_UP ) ),
							pString, wPhysicalValue, wNextPhysicalValue );

						if( wPhysicalValue != wNextPhysicalValue )
							bSameSuccessParameter = FALSE;
					}

					if( wstrSuccessParameter.empty() )
						wstrSuccessParameter = wParameter;
					else
						wstrSuccessParameter += wParameter;

					// 블랙호이포이스톤의 경우 성공시 실패텍스트를 이용해 출력. 한등급아래로 떨어진다. 
					if( pHoipoiAttr->GetItemTbl()->byItem_Type == ITEM_TYPE_BLACK_STONE )
					{
						--byGrade;
						if( byGrade < 0 )
							byGrade = 0;
					}
					else 
						byGrade = 0;

					wFailPhysicalValue = Logic_GetFinalItemValue( pItemAttr->GetItemTbl()->wPhysical_Offence, byGrade, pItemAttr->GetItemTbl()->dwPhysical_OffenceUpgrade );

					swprintf_s( wParameter, ITEMUPGRADE_TEXTBUFFER, const_cast<WCHAR*>( GetDisplayStringManager()->GetString( DST_ITEMUPGRADE_MSG_PHYSICAL_DOWN ) ),
						pString, wPhysicalValue, wFailPhysicalValue );

					if( wstrFailParameter.empty() )
						wstrFailParameter = wParameter;						
					else
						wstrFailParameter += wParameter;												

					if( wPhysicalValue != wFailPhysicalValue )
						bSameFailParameter = FALSE;
				}

				if( pItemAttr->GetItemTbl()->wEnergy_Offence != INVALID_WORD )
				{
					byGrade = pItemAttr->GetGrade();

					wEnergyValue = Logic_GetFinalItemValue( pItemAttr->GetItemTbl()->wEnergy_Offence, byGrade, pItemAttr->GetItemTbl()->dwEnergy_OffenceUpgrade );
					wNextEnergyValue = Logic_GetFinalItemValue( pItemAttr->GetItemTbl()->wEnergy_Offence, byNextGrade, pItemAttr->GetItemTbl()->dwEnergy_OffenceUpgrade );
					wPureMaxValue = Logic_GetFinalItemValue( pItemAttr->GetItemTbl()->wEnergy_Offence, byPureMaxGrade, pItemAttr->GetItemTbl()->dwEnergy_OffenceUpgrade );

					if( pHoipoiAttr->GetItemTbl()->byItem_Type == ITEM_TYPE_PURE_STONE && wNextEnergyValue != wPureMaxValue )
					{
						swprintf_s( wParameter, ITEMUPGRADE_TEXTBUFFER, const_cast<WCHAR*>( GetDisplayStringManager()->GetString( DST_ITEMUPGRADE_MSG_PURE_ENERGY_UP ) ),
							pString, wEnergyValue, wNextEnergyValue, wPureMaxValue );

						if( wPhysicalValue != wNextPhysicalValue || wPhysicalValue != wPureMaxValue )
							bSameSuccessParameter = FALSE;
					}
					else
					{
						swprintf_s( wParameter, ITEMUPGRADE_TEXTBUFFER, const_cast<WCHAR*>( GetDisplayStringManager()->GetString( DST_ITEMUPGRADE_MSG_ENERGY_UP ) ),
							pString, wEnergyValue, wNextEnergyValue );

						if( wPhysicalValue != wNextPhysicalValue )
							bSameSuccessParameter = FALSE;
					}

					if( wstrSuccessParameter.empty() )
						wstrSuccessParameter = wParameter;
					else
						wstrSuccessParameter += wParameter;

					// 블랙호이포이스톤의 경우 성공시 실패텍스트를 이용해 출력. 한등급아래로 떨어진다. 
					if( pHoipoiAttr->GetItemTbl()->byItem_Type == ITEM_TYPE_BLACK_STONE )
					{
						--byGrade;
						if( byGrade < 0 )
							byGrade = 0;
					}
					else
						byGrade = 0;

					wFailEnergyValue = Logic_GetFinalItemValue( pItemAttr->GetItemTbl()->wEnergy_Offence, byGrade, pItemAttr->GetItemTbl()->dwEnergy_OffenceUpgrade );

					swprintf_s( wParameter, ITEMUPGRADE_TEXTBUFFER, const_cast<WCHAR*>( GetDisplayStringManager()->GetString( DST_ITEMUPGRADE_MSG_ENERGY_DOWN ) ),
						pString, wEnergyValue, wFailEnergyValue );

					if( wstrFailParameter.empty() )
						wstrFailParameter = wParameter;						
					else
						wstrFailParameter += wParameter;			

					if( wPhysicalValue != wFailPhysicalValue )
						bSameFailParameter = FALSE;
				}				
			}
			else
			{
				pString = const_cast<WCHAR*>( GetDisplayStringManager()->GetString( DST_ITEMUPGRADE_INFO_DEFENCE ) );

				if( pItemAttr->GetItemTbl()->wPhysical_Defence != INVALID_WORD )
				{
					byGrade = pItemAttr->GetGrade();

					wPhysicalValue = Logic_GetFinalItemValue( pItemAttr->GetItemTbl()->wPhysical_Defence, byGrade, pItemAttr->GetItemTbl()->dwPhysical_DefenceUpgrade );
					wNextPhysicalValue = Logic_GetFinalItemValue( pItemAttr->GetItemTbl()->wPhysical_Defence, byNextGrade, pItemAttr->GetItemTbl()->dwPhysical_DefenceUpgrade );
					wPureMaxValue = Logic_GetFinalItemValue( pItemAttr->GetItemTbl()->wPhysical_Defence, byPureMaxGrade, pItemAttr->GetItemTbl()->dwPhysical_DefenceUpgrade );

					if( pHoipoiAttr->GetItemTbl()->byItem_Type == ITEM_TYPE_PURE_STONE && wNextPhysicalValue != wPureMaxValue )
					{
						swprintf_s( wParameter, ITEMUPGRADE_TEXTBUFFER, const_cast<WCHAR*>( GetDisplayStringManager()->GetString( DST_ITEMUPGRADE_MSG_PURE_PHYSICAL_UP ) ),
							pString, wPhysicalValue, wNextPhysicalValue, wPureMaxValue );

						if( wPhysicalValue != wNextPhysicalValue || wPhysicalValue != wPureMaxValue )
							bSameSuccessParameter = FALSE;
					}
					else
					{
						swprintf_s( wParameter, ITEMUPGRADE_TEXTBUFFER, const_cast<WCHAR*>( GetDisplayStringManager()->GetString( DST_ITEMUPGRADE_MSG_PHYSICAL_UP ) ),
							pString, wPhysicalValue, wNextPhysicalValue );

						if( wPhysicalValue != wNextPhysicalValue )
							bSameSuccessParameter = FALSE;
					}

					if( wstrSuccessParameter.empty() )
						wstrSuccessParameter = wParameter;
					else
						wstrSuccessParameter += wParameter;

					// 블랙호이포이스톤의 경우 성공시 실패텍스트를 이용해 출력. 한등급아래로 떨어진다. 
					if( pHoipoiAttr->GetItemTbl()->byItem_Type == ITEM_TYPE_BLACK_STONE )
					{
						--byGrade;
						if( byGrade < 0 )
							byGrade = 0;
					}
					else 
						byGrade = 0;

					wFailPhysicalValue = Logic_GetFinalItemValue( pItemAttr->GetItemTbl()->wPhysical_Defence, byGrade, pItemAttr->GetItemTbl()->dwPhysical_DefenceUpgrade );

					swprintf_s( wParameter, ITEMUPGRADE_TEXTBUFFER, const_cast<WCHAR*>( GetDisplayStringManager()->GetString( DST_ITEMUPGRADE_MSG_PHYSICAL_DOWN ) ),
						pString, wPhysicalValue, wFailPhysicalValue );

					if( wstrFailParameter.empty() )
						wstrFailParameter = wParameter;						
					else
						wstrFailParameter += wParameter;	

					if( wPhysicalValue != wFailPhysicalValue )
						bSameFailParameter = FALSE;
				}

				if( pItemAttr->GetItemTbl()->wEnergy_Defence != INVALID_WORD )
				{
					byGrade = pItemAttr->GetGrade();

					wEnergyValue = Logic_GetFinalItemValue( pItemAttr->GetItemTbl()->wEnergy_Defence, byGrade, pItemAttr->GetItemTbl()->dwEnergy_DefenceUpgrade );
					wNextEnergyValue = Logic_GetFinalItemValue( pItemAttr->GetItemTbl()->wEnergy_Defence, byNextGrade, pItemAttr->GetItemTbl()->dwEnergy_DefenceUpgrade );
					wPureMaxValue = Logic_GetFinalItemValue( pItemAttr->GetItemTbl()->wEnergy_Defence, byPureMaxGrade, pItemAttr->GetItemTbl()->dwEnergy_DefenceUpgrade );

					if( pHoipoiAttr->GetItemTbl()->byItem_Type == ITEM_TYPE_PURE_STONE && wNextEnergyValue != wPureMaxValue )
					{
						swprintf_s( wParameter, ITEMUPGRADE_TEXTBUFFER, const_cast<WCHAR*>( GetDisplayStringManager()->GetString( DST_ITEMUPGRADE_MSG_PURE_ENERGY_UP ) ),
							pString, wEnergyValue, wNextEnergyValue, wPureMaxValue );

						if( wPhysicalValue != wNextPhysicalValue || wPhysicalValue != wPureMaxValue )
							bSameSuccessParameter = FALSE;
					}
					else
					{
						swprintf_s( wParameter, ITEMUPGRADE_TEXTBUFFER, const_cast<WCHAR*>( GetDisplayStringManager()->GetString( DST_ITEMUPGRADE_MSG_ENERGY_UP ) ),
							pString, wEnergyValue, wNextEnergyValue );

						if( wPhysicalValue != wNextPhysicalValue )
							bSameSuccessParameter = FALSE;
					}

					if( wstrSuccessParameter.empty() )
						wstrSuccessParameter = wParameter;
					else
						wstrSuccessParameter += wParameter;

					// 블랙호이포이스톤의 경우 성공시 실패텍스트를 이용해 출력. 한등급아래로 떨어진다. 
					if( pHoipoiAttr->GetItemTbl()->byItem_Type == ITEM_TYPE_BLACK_STONE )
					{
						--byGrade;
						if( byGrade < 0 )
							byGrade = 0;
					}
					else
						byGrade = 0;

					wFailEnergyValue = Logic_GetFinalItemValue( pItemAttr->GetItemTbl()->wEnergy_Defence, byGrade, pItemAttr->GetItemTbl()->dwEnergy_DefenceUpgrade );

					swprintf_s( wParameter, ITEMUPGRADE_TEXTBUFFER, const_cast<WCHAR*>( GetDisplayStringManager()->GetString( DST_ITEMUPGRADE_MSG_ENERGY_DOWN ) ),
						pString, wEnergyValue, wFailEnergyValue );

					if( wstrFailParameter.empty() )
						wstrFailParameter = wParameter;						
					else
						wstrFailParameter += wParameter;					

					if( wPhysicalValue != wFailPhysicalValue )
						bSameFailParameter = FALSE;
				}				
			}

			const WCHAR* pCurAttribute = GetDisplayStringManager()->GetString( DST_ITEM_ATTR_NONE + pItemAttr->GetBattleAttribute() );
			WCHAR* pUpAttribute;
			WCHAR* pDownAttribute; 
			WCHAR bufUpAttr[ITEMUPGRADE_TEXTBUFFER];
			WCHAR bufDownAttr[ITEMUPGRADE_TEXTBUFFER];
			if( m_nStoneSlotIdx + 1 == pItemAttr->GetBattleAttribute() )
			{
				pUpAttribute = const_cast<WCHAR*>( GetDisplayStringManager()->GetString( DST_ITEM_ATTR_NONE + pItemAttr->GetBattleAttribute() ) );
				pDownAttribute = const_cast<WCHAR*>( GetDisplayStringManager()->GetString( DST_ITEM_ATTR_NONE + pItemAttr->GetBattleAttribute() ) );
			}
			else
			{
				pUpAttribute = const_cast<WCHAR*>( GetDisplayStringManager()->GetString( DST_ITEM_ATTR_HONEST + m_nStoneSlotIdx ) );
				pDownAttribute = const_cast<WCHAR*>( GetDisplayStringManager()->GetString( DST_ITEM_ATTR_NONE ) );
			}

			if( wcscmp( pCurAttribute, pUpAttribute ) == 0 )
				swprintf_s( bufUpAttr, ITEMUPGRADE_TEXTBUFFER, GetDisplayStringManager()->GetString( DST_ITEMUPGRADE_MSG_NO_CHANGE_ATTR ) );
			else
				swprintf_s( bufUpAttr, ITEMUPGRADE_TEXTBUFFER, GetDisplayStringManager()->GetString( DST_ITEMUPGRADE_MSG_CHANGE_ATTR ), pCurAttribute, pUpAttribute );

			if( wcscmp( pCurAttribute, pDownAttribute ) == 0 )
				swprintf_s( bufDownAttr, ITEMUPGRADE_TEXTBUFFER, GetDisplayStringManager()->GetString( DST_ITEMUPGRADE_MSG_NO_CHANGE_ATTR ) );
			else
				swprintf_s( bufDownAttr, ITEMUPGRADE_TEXTBUFFER, GetDisplayStringManager()->GetString( DST_ITEMUPGRADE_MSG_CHANGE_ATTR ), pCurAttribute, pDownAttribute );

			if( bSameSuccessParameter )
			{
				wstrSuccessParameter = GetDisplayStringManager()->GetString( DST_ITEMUPGRADE_MSG_NO_CHANGE_PARAMETER );
			}

			if( bSameFailParameter )
			{
				wstrFailParameter = GetDisplayStringManager()->GetString( DST_ITEMUPGRADE_MSG_NO_CHANGE_PARAMETER );
			}			

			// peessi : Brushup 추가. 
			WCHAR bufUseHoipoiStoneCount[ITEMUPGRADE_TEXTBUFFER];
			std::wstring wstrHoipoiStoneName = API_GetTableContainer()->GetTextAllTable()->GetItemTbl()->GetText( pHoipoiAttr->GetItemTbl()->Name );
			swprintf_s( bufUseHoipoiStoneCount, ITEMUPGRADE_TEXTBUFFER, GetDisplayStringManager()->GetString( DST_ITEMUPGRADE_MSG_USE_HOIPOISTONECOUNT ), wstrHoipoiStoneName.c_str(), Dbo_GetHoipoiStoneCount( pHoipoiAttr->GetItemTbl()->byItem_Type, pItemAttr->GetItemTbl()->byItem_Type, pItemAttr->GetGrade() ) );

			if( pHoipoiAttr->GetItemTbl()->byItem_Type == ITEM_TYPE_BLACK_STONE )
			{
				swprintf_s( wBuf, ITEMUPGRADE_TEXTBUFFER, pInfo, bufUseHoipoiStoneCount, pString, wstrFailParameter.c_str(), bufUpAttr );
			}
			else
			{
				swprintf_s( wBuf, ITEMUPGRADE_TEXTBUFFER, pInfo, bufUseHoipoiStoneCount, pString, wstrSuccessParameter.c_str(), bufUpAttr,
					(RwInt32)m_fDestroyProbability, pString, wstrFailParameter.c_str(), bufDownAttr );
			}

			m_phtmlInfoText->SetHtmlFromMemory( wBuf, wcslen( wBuf ) );

			// UpgradeButton 활성화
			m_pbtnCompound->ClickEnable( TRUE );

			// 예측 UI 활성화
			ShowPredictMessage();

			// Effect
			if( STATE_NONE == m_eState || 
				STATE_ITEM_IN == m_eState ||
				STATE_HOIPOI_IN == m_eState ||
				STATE_UPGRADE_READYFAIL_NOT_ENOUGH_HOIPOISTONE == m_eState ||
				STATE_UPGRADE_READYFAIL_NOT_MATCHED_HOIPOISTONE == m_eState ||
				STATE_UPGRADE_READYFAIL_NOMOREUPGRADE == m_eState ||
				STATE_UPGRADE_READYFAIL_NOMOREDOWNGRADE == m_eState )
				m_BackFocus.SetState( stSLOTEFFECT::SLOT_EFFECT_ON );
		}
		break;
	case STATE_UPGRADE_PROCESS:
		{
			DisableSlot( TRUE );

			CNtlSobItemAttr* pHoipoiAttr = reinterpret_cast<CNtlSobItemAttr*>( m_pHoipoiStone->GetSobAttr() );
			if( pHoipoiAttr->GetItemTbl()->byItem_Type == ITEM_TYPE_BLACK_STONE )
				m_pflsUpgradeEffect->Load( ITEMUPGRADEGUI_BLACK_PROCESS_FLASHFILE );
			else if( pHoipoiAttr->GetItemTbl()->byItem_Type == ITEM_TYPE_PURE_STONE )
				m_pflsUpgradeEffect->Load( ITEMUPGRADEGUI_PURE_PROCESS_FLASHFILE );
			else
				m_pflsUpgradeEffect->Load( ITEMUPGRADEGUI_PROCESS_FLASHFILE );

			m_pflsUpgradeEffect->RestartMovie();			
			m_pflsUpgradeEffect->Show( TRUE );			
		}
		break;
	case STATE_PACKETWAIT:
		{
			DisableSlot( TRUE );

			// 취소버튼 비활성화!!!
			m_pbtnExit->ClickEnable( FALSE );
		}
		break;
	case STATE_RESULT:
		{
			DisableSlot( TRUE );
			m_pdlgResult->Show( true );			
		}
		break;
	}

	m_eState = eState;
}

RwReal CItemUpgradeGui::CalcBonusFromScouterParts(VOID)
{
	RwReal fTotalBonusValue = 0.0f;
	CNtlInventory* pInventory = GetNtlSLGlobal()->GetSobAvatar()->GetInventory();
	SERIAL_HANDLE hScouterSerial = pInventory->GetEquipItem( EQUIP_SLOT_TYPE_SCOUTER );

	if( hScouterSerial != INVALID_SERIAL_ID )
	{
		CNtlSobItem* pScouterItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( hScouterSerial ) );
		CNtlSobItemAttr* pScouterItemAttr = reinterpret_cast<CNtlSobItemAttr*>( pScouterItem->GetSobAttr() );
		sITEM_TBLDAT* pITEM_TBLDAT = pScouterItemAttr->GetItemTbl();			

		CNtlSobItem* pPartsItem;
		CNtlSobItemAttr* pPartsItemAttr;
		sITEM_TBLDAT* pPartsTBLDAT;

		for(RwInt32 i = 0 ; i < pITEM_TBLDAT->byBag_Size ; ++i )
		{
			pPartsItem = pScouterItem->GetChildItem( i );
			if( !pPartsItem )
				continue;

			pPartsItemAttr = reinterpret_cast<CNtlSobItemAttr*>( pPartsItem->GetSobAttr() );
			pPartsTBLDAT = pPartsItemAttr->GetItemTbl();
			if( pPartsTBLDAT->byScouter_Parts_Type == SCOUTER_PARTS_UPGRADE_PREDICTION )
			{
				if( pPartsTBLDAT->Item_Option_Tblidx != INVALID_TBLIDX )
				{
					sITEM_OPTION_TBLDAT* pOptionData = reinterpret_cast<sITEM_OPTION_TBLDAT*>( API_GetTableContainer()->GetItemOptionTable()->FindData( pPartsTBLDAT->Item_Option_Tblidx ) );

					for( RwInt32 j = 0 ; j < NTL_MAX_SYSTEM_EFFECT_COUNT ; ++j )
					{
						if( pOptionData->system_Effect[j] == INVALID_TBLIDX )
							continue;

						sSYSTEM_EFFECT_TBLDAT* pSystemEffectData = reinterpret_cast<sSYSTEM_EFFECT_TBLDAT*>( API_GetTableContainer()->GetSystemEffectTable()->FindData( pOptionData->system_Effect[j] ) );

						if( pSystemEffectData->effectCode == ACTIVE_ITEM_BREAK_RATE_DOWN )
						{
							fTotalBonusValue += (RwReal)pOptionData->nValue[j];
						}
					}
				}				
			}
		}
	}

	return fTotalBonusValue;
}

VOID CItemUpgradeGui::ShowPredictMessage(VOID)
{
	// STATE_UPGRADE_READY에서만 호출.
	CNtlSobItemAttr* pItemAttr = reinterpret_cast<CNtlSobItemAttr*>( m_pUpgradeItem->GetSobAttr() );
	CNtlSobItemAttr* pHoipoiAttr = reinterpret_cast<CNtlSobItemAttr*>( m_pHoipoiStone->GetSobAttr() );

	RwReal fFinalValue = Dbo_UpgradeSuccessRate( pItemAttr->GetGrade(), pHoipoiAttr->GetItemTbl()->byItem_Type );		
	RwUInt32 uiTextColor = 0;
	const WCHAR* pString = NULL;

	if( pHoipoiAttr->GetItemTbl()->byItem_Type == ITEM_TYPE_BLACK_STONE )
		return;
	else
	{
		if( fFinalValue >= 80.0f )
		{
			pString = GetDisplayStringManager()->GetString( DST_ITEMUPGRADE_PREDICT_VERYGOOD );
			uiTextColor = INFOCOLOR_1;
		}
		else if( fFinalValue >= 60.0f )
		{
			pString = GetDisplayStringManager()->GetString( DST_ITEMUPGRADE_PREDICT_GOOD );
			uiTextColor = INFOCOLOR_1;
		}
		else if( fFinalValue >= 40.0f )
		{
			pString = GetDisplayStringManager()->GetString( DST_ITEMUPGRADE_PREDICT_NORMAL );
			uiTextColor = INFOCOLOR_3;
		}
		else if( fFinalValue >= 20.0f )
		{
			pString = GetDisplayStringManager()->GetString( DST_ITEMUPGRADE_PREDICT_BAD );
			uiTextColor = INFOCOLOR_7;
		}
		else if( fFinalValue >= 10.0f )
		{
			pString = GetDisplayStringManager()->GetString( DST_ITEMUPGRADE_PREDICT_POOR );
			uiTextColor = INFOCOLOR_6;
		}
		else if( fFinalValue >= 6.0f )
		{
			pString = GetDisplayStringManager()->GetString( DST_ITEMUPGRADE_PREDICT_VERYPOOR );
			uiTextColor = INFOCOLOR_5;
		}
		else
		{
			pString = GetDisplayStringManager()->GetString( DST_ITEMUPGRADE_PREDICT_HOPELESS );
			uiTextColor = INFOCOLOR_5;
		}
	}	

	m_pstbPredictResult->SetTextColor( uiTextColor );
	m_pstbPredictResult->SetText( pString );

	m_pstbDestroyPredictResult->Format( L"%u%%", (RwUInt32)m_fDestroyProbability );

	m_ppnlPredict->Show( true );
}

// peessiupgrade : 인포윈도우 체크
VOID CItemUpgradeGui::ShowInfoWnd( RwBool bShow, RwInt32 nMouseOnIdx /* = -1  */ )
{
	if(nMouseOnIdx >= NUM_SLOT)
		return;

	if( bShow )
	{
		CRectangle rtScreen = m_pThis->GetScreenRect();

		if( nMouseOnIdx == SLOT_ITEM )
		{
			if( m_pUpgradeItem )
			{
				GetInfoWndManager()->ShowInfoWindow( TRUE, CInfoWndManager::INFOWND_ITEM,
					m_rtSlot[nMouseOnIdx].left + rtScreen.left, m_rtSlot[nMouseOnIdx].top + rtScreen.top,
					(VOID*)GetSlotObject( nMouseOnIdx ), DIALOG_ITEMUPGRADE );				
			}
			else
			{
				GetInfoWndManager()->ShowInfoWindow( TRUE, CInfoWndManager::INFOWND_JUST_WTEXT,
					m_rtSlot[nMouseOnIdx].left + rtScreen.left, m_rtSlot[nMouseOnIdx].top + rtScreen.top,
					(VOID*)GetDisplayStringManager()->GetString( DST_ITEMUPGRADE_INFO_ITEM_SLOT ), DIALOG_ITEMUPGRADE );
			}			
		}
		else
		{
			if( m_eState == STATE_UPGRADE_READY )
			{
				CNtlSobItemAttr* pAttr = reinterpret_cast<CNtlSobItemAttr*>( m_pUpgradeItem->GetSobAttr() );
				CNtlSobItemAttr* pHoipoiAttr = reinterpret_cast<CNtlSobItemAttr*>( m_pHoipoiStone->GetSobAttr() );
				stINFOWND_UPGRADE InfoData;
				WCHAR wBuf[512];

				if( pHoipoiAttr->GetItemTbl()->byItem_Type == ITEM_TYPE_BLACK_STONE )
				{
					if( CONV_TO_POSNUM( nMouseOnIdx ) == pAttr->GetBattleAttribute() )
					{// 무속성은 여기로 들어올 수 없다. 
						InfoData.wszText[0] = const_cast<WCHAR*>( GetDisplayStringManager()->GetString( DST_ITEMUPGRADE_INFO_HOIPOI_SLOT_IN_1_NO_BLACKCHANGE ) );
						InfoData.uiColor[0] = INFOCOLOR_2;
					}
					else
					{
						swprintf_s( wBuf, 512, GetDisplayStringManager()->GetString( DST_ITEMUPGRADE_INFO_HOIPOI_SLOT_IN_1_BLACKCHANGE ), GetDisplayStringManager()->GetString( DST_ITEM_ATTR_HONEST + nMouseOnIdx ) );
						InfoData.wszText[0] = wBuf;
						InfoData.uiColor[0] = INFOCOLOR_2;
					}
				}
				else
				{
					if( CONV_TO_POSNUM( nMouseOnIdx ) == pAttr->GetBattleAttribute() )
					{// 무속성은 여기로 들어올 수 없다. 
						InfoData.wszText[0] = const_cast<WCHAR*>( GetDisplayStringManager()->GetString( DST_ITEMUPGRADE_INFO_HOIPOI_SLOT_IN_1_NO_CHANGE ) );
						InfoData.wszText[1] = const_cast<WCHAR*>( GetDisplayStringManager()->GetString( DST_ITEMUPGRADE_INFO_HOIPOI_SLOT_IN_2_COMMON ) );
						InfoData.wszText[2] = const_cast<WCHAR*>( GetDisplayStringManager()->GetString( DST_ITEMUPGRADE_INFO_HOIPOI_SLOT_IN_2_BEST ) );
						InfoData.wszText[3] = const_cast<WCHAR*>( GetDisplayStringManager()->GetString( DST_ITEMUPGRADE_INFO_HOIPOI_SLOT_IN_3_NO_CHANGE ) ); 					

						InfoData.uiColor[0] = INFOCOLOR_2;
						InfoData.uiColor[1] = INFOCOLOR_0;
						InfoData.uiColor[2] = INFOCOLOR_1;
						InfoData.uiColor[3] = INFOCOLOR_7;					
					}
					else
					{
						RwUInt8 byRate = Dbo_GetUpgradeRate( pAttr->GetBattleAttribute(), CONV_TO_POSNUM( nMouseOnIdx ) );

						swprintf_s( wBuf, 512, GetDisplayStringManager()->GetString( DST_ITEMUPGRADE_INFO_HOIPOI_SLOT_IN_1_CHANGE ), GetDisplayStringManager()->GetString( DST_ITEM_ATTR_HONEST + nMouseOnIdx ) );
						InfoData.wszText[0] = wBuf;
						InfoData.uiColor[0] = INFOCOLOR_2;
						InfoData.wszText[1] = const_cast<WCHAR*>( GetDisplayStringManager()->GetString( DST_ITEMUPGRADE_INFO_HOIPOI_SLOT_IN_2_COMMON ) ); 
						InfoData.uiColor[1] = INFOCOLOR_0;

						if( byRate == ITEM_UPGRADE_RATE_4 )
						{
							InfoData.wszText[2] = const_cast<WCHAR*>( GetDisplayStringManager()->GetString( DST_ITEMUPGRADE_INFO_HOIPOI_SLOT_IN_2_GOOD ) ); 
							InfoData.uiColor[2] = INFOCOLOR_1;
						}
						else if( byRate == ITEM_UPGRADE_RATE_3 )
						{
							InfoData.wszText[2] = const_cast<WCHAR*>( GetDisplayStringManager()->GetString( DST_ITEMUPGRADE_INFO_HOIPOI_SLOT_IN_2_LITTLE_GOOD ) );
							InfoData.uiColor[2] = INFOCOLOR_3;
						}
						else if( byRate == ITEM_UPGRADE_RATE_2 )
						{
							InfoData.wszText[2] = const_cast<WCHAR*>( GetDisplayStringManager()->GetString( DST_ITEMUPGRADE_INFO_HOIPOI_SLOT_IN_2_LITTLE_GOOD ) );
							InfoData.uiColor[2] = INFOCOLOR_7;
						}
						else if( byRate == ITEM_UPGRADE_RATE_1 )
						{
							InfoData.wszText[2] = const_cast<WCHAR*>( GetDisplayStringManager()->GetString( DST_ITEMUPGRADE_INFO_HOIPOI_SLOT_IN_2_LITTLE_BAD ) );
							InfoData.uiColor[2] = INFOCOLOR_6;
						}
						else if( byRate == ITEM_UPGRADE_RATE_0 )
						{
							InfoData.wszText[2] = const_cast<WCHAR*>( GetDisplayStringManager()->GetString( DST_ITEMUPGRADE_INFO_HOIPOI_SLOT_IN_2_BAD ) );
							InfoData.uiColor[2] = INFOCOLOR_5;
						}

						if( pAttr->GetBattleAttribute() )
							InfoData.wszText[3] = const_cast<WCHAR*>( GetDisplayStringManager()->GetString( DST_ITEMUPGRADE_INFO_HOIPOI_SLOT_IN_3 ) ); 					
						else
							InfoData.wszText[3] = const_cast<WCHAR*>( GetDisplayStringManager()->GetString( DST_ITEMUPGRADE_INFO_HOIPOI_SLOT_IN_3_NO_CHANGE ) ); 					

						InfoData.uiColor[3] = INFOCOLOR_7;
					}				
				}				

				GetInfoWndManager()->ShowInfoWindow( TRUE, CInfoWndManager::INFOWND_UPGRADE, 
					m_rtSlot[nMouseOnIdx].left + rtScreen.left, m_rtSlot[nMouseOnIdx].top + rtScreen.top, &InfoData, DIALOG_ITEMUPGRADE );
			}
			else if( m_eState == STATE_UPGRADE_READYFAIL_NOT_ENOUGH_HOIPOISTONE )
			{
				CNtlSobItemAttr* pItemAttr = reinterpret_cast<CNtlSobItemAttr*>( m_pUpgradeItem->GetSobAttr() );
				CNtlSobItemAttr* pHoipoiAttr = reinterpret_cast<CNtlSobItemAttr*>( m_pHoipoiStone->GetSobAttr() );
				std::wstring wstrItemName = API_GetTableContainer()->GetTextAllTable()->GetItemTbl()->GetText( pItemAttr->GetItemTbl()->Name );
				std::wstring wstrHoipoiStoneName = API_GetTableContainer()->GetTextAllTable()->GetItemTbl()->GetText( pHoipoiAttr->GetItemTbl()->Name );				
				WCHAR wBuf[ITEMUPGRADE_TEXTBUFFER];
				swprintf_s( wBuf, ITEMUPGRADE_TEXTBUFFER, GetDisplayStringManager()->GetString( DST_ITEMUPGRADE_MSG_HOIPOISTONE_IN_FAIL_QUANTITY ), 
					wstrItemName.c_str(), wstrHoipoiStoneName.c_str(), pHoipoiAttr->GetStackNum(), Dbo_GetHoipoiStoneCount( pHoipoiAttr->GetItemTbl()->byItem_Type, pItemAttr->GetItemTbl()->byItem_Type, pItemAttr->GetGrade() ) ); 

				std::wstring wstrInfo = gui::GetHtmlFromMemoryString( wBuf, wcslen( wBuf ), TRUE );

				GetInfoWndManager()->ShowInfoWindow( TRUE, CInfoWndManager::INFOWND_JUST_WTEXT,	m_rtSlot[nMouseOnIdx].left + rtScreen.left, m_rtSlot[nMouseOnIdx].top + rtScreen.top, (VOID*)wstrInfo.c_str(), DIALOG_ITEMUPGRADE );
			}
			else if( m_eState == STATE_UPGRADE_READYFAIL_NOT_MATCHED_HOIPOISTONE )
			{
				const WCHAR* pInfo = GetDisplayStringManager()->GetString( DST_ITEMUPGRADE_MSG_HOIPOISTONE_IN_FAIL_CLASS );
				std::wstring wstrInfo = gui::GetHtmlFromMemoryString( pInfo, wcslen( pInfo ), TRUE );

				GetInfoWndManager()->ShowInfoWindow( TRUE, CInfoWndManager::INFOWND_JUST_WTEXT,	m_rtSlot[nMouseOnIdx].left + rtScreen.left, m_rtSlot[nMouseOnIdx].top + rtScreen.top, (VOID*)wstrInfo.c_str(), DIALOG_ITEMUPGRADE );
			}
			else if( m_eState == STATE_UPGRADE_READYFAIL_NOMOREUPGRADE )
			{
				const WCHAR* pInfo = GetDisplayStringManager()->GetString( DST_ITEMUPGRADE_MSG_UNUPGRADABLE_ITEM );
				std::wstring wstrInfo = gui::GetHtmlFromMemoryString( pInfo, wcslen( pInfo ), TRUE );

				GetInfoWndManager()->ShowInfoWindow( TRUE, CInfoWndManager::INFOWND_JUST_WTEXT,	m_rtSlot[nMouseOnIdx].left + rtScreen.left, m_rtSlot[nMouseOnIdx].top + rtScreen.top, (VOID*)wstrInfo.c_str(), DIALOG_ITEMUPGRADE );
			}
			else if( m_eState == STATE_UPGRADE_READYFAIL_NOMOREDOWNGRADE )
			{
				const WCHAR* pInfo = GetDisplayStringManager()->GetString( DST_ITEMUPGRADE_MSG_UNDOWNGRADABLE_ITEM );
				std::wstring wstrInfo = gui::GetHtmlFromMemoryString( pInfo, wcslen( pInfo ), TRUE );

				GetInfoWndManager()->ShowInfoWindow( TRUE, CInfoWndManager::INFOWND_JUST_WTEXT,	m_rtSlot[nMouseOnIdx].left + rtScreen.left, m_rtSlot[nMouseOnIdx].top + rtScreen.top, (VOID*)wstrInfo.c_str(), DIALOG_ITEMUPGRADE );
			}
			else if( m_eState != STATE_UPGRADE_PROCESS && m_eState != STATE_PACKETWAIT && m_eState != STATE_RESULT )
			{
				stINFOWND_BATTLEATTR sBattleAttr;				
				sBattleAttr.eBattleAttrInfoType = stINFOWND_BATTLEATTR::TYPE_ATTR_UPGRADE;
								
				switch( nMouseOnIdx )
				{
				case SLOT_HONEST: sBattleAttr.bySourceWeaponAttr = BATTLE_ATTRIBUTE_HONEST;	break;
				case SLOT_STRANGE: sBattleAttr.bySourceWeaponAttr = BATTLE_ATTRIBUTE_STRANGE; break;					
				case SLOT_WILD: sBattleAttr.bySourceWeaponAttr = BATTLE_ATTRIBUTE_WILD; break;					
				case SLOT_ELEGANT: sBattleAttr.bySourceWeaponAttr = BATTLE_ATTRIBUTE_ELEGANCE; break;					
				case SLOT_FUNNY: sBattleAttr.bySourceWeaponAttr = BATTLE_ATTRIBUTE_FUNNY; break;					
				default: return;
				}				

				GetInfoWndManager()->ShowInfoWindow( TRUE, CInfoWndManager::INFOWND_BATTLEATTRIBUTE, m_rtSlot[nMouseOnIdx].left + rtScreen.left, m_rtSlot[nMouseOnIdx].top + rtScreen.top, (VOID*)&sBattleAttr, DIALOG_ITEMUPGRADE );

				//WCHAR wBuf[ITEMUPGRADE_TEXTBUFFER];
				//swprintf_s( wBuf, ITEMUPGRADE_TEXTBUFFER, GetDisplayStringManager()->GetString( DST_ITEMUPGRADE_INFO_HOIPOI_SLOT_EMPTY ), GetDisplayStringManager()->GetString( DST_ITEM_ATTR_HONEST + nMouseOnIdx ) );
				//GetInfoWndManager()->ShowInfoWindow( TRUE, CInfoWndManager::INFOWND_JUST_WTEXT,
				//	m_rtSlot[nMouseOnIdx].left + rtScreen.left, m_rtSlot[nMouseOnIdx].top + rtScreen.top, (VOID*)wBuf, DIALOG_ITEMUPGRADE );
			}
		}
	}
	else
	{
		GetInfoWndManager()->ShowInfoWindow( FALSE );		
	}
}

VOID CItemUpgradeGui::SendUpgradePacket(VOID)
{
	RwUInt8 byItemPlace = m_pUpgradeItem->GetParentItemSlotIdx() + CONTAINER_TYPE_BAG1;
	RwUInt8 byItemPos = m_pUpgradeItem->GetItemSlotIdx();

	RwUInt8 byStonePlace = m_pHoipoiStone->GetParentItemSlotIdx() + CONTAINER_TYPE_BAG1;
	RwUInt8 byStonePos = m_pHoipoiStone->GetItemSlotIdx();

	GetDboGlobal()->GetGamePacketGenerator()->SendItemUpgradeReq( m_hNPCSerial, byItemPlace, byItemPos, byStonePlace, byStonePos, CONV_TO_POSNUM( m_nStoneSlotIdx ) );
	SetState( STATE_PACKETWAIT );
}

VOID CItemUpgradeGui::ShowIconDestination( RwBool isPick )
{
	if( isPick )
	{
		if( GetIconMoveManager()->GetSrcPlace() == PLACE_BAG )
		{
			if( m_eState == STATE_UPGRADE_PROCESS || m_eState == STATE_PACKETWAIT || m_eState == STATE_RESULT )
				return;
			else
			{
				CNtlSobItem* pSobItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( GetIconMoveManager()->GetSrcSerial() ) );
				NTL_ASSERT( pSobItem, "CItemUpgradeGui::ShowIconDestination : pSobItem is must be present!" );			

				CNtlSobItemAttr* pSobItemAttr = reinterpret_cast<CNtlSobItemAttr*>( pSobItem->GetSobAttr() );
				NTL_ASSERT( pSobItemAttr, "CItemUpgradeGui::ShowIconDestination : pSobItemAttr is must be present!" );			

				if( IsUpgradableItem( pSobItemAttr ) )
				{
					m_anFocusEffect[SLOT_ITEM] |= SLOT_FOCUS_CAN_MOVE;
				}
				else if( IsValidHoipoiStone( pSobItemAttr ) )
				{
					for( RwInt32 i = 0 ; i < NUM_SLOTEFFECT ; ++i )
					{
						m_anFocusEffect[i] = m_anFocusEffect[i] | SLOT_FOCUS_CAN_MOVE;
					}	
				}				
			}
		}
		else if( GetIconMoveManager()->GetSrcPlace() == PLACE_ITEMUPGRADE )
		{
			if( m_eState == STATE_UPGRADE_PROCESS || m_eState == STATE_PACKETWAIT || m_eState == STATE_RESULT )
				return;
			else
			{
				// Item 은 장착된후에는 Pickup이 되지 않는다.
				for( RwInt32 i = 0 ; i < NUM_SLOTEFFECT ; ++i )
				{
					m_anFocusEffect[i] = m_anFocusEffect[i] | SLOT_FOCUS_CAN_MOVE;
				}
			}			
		}
	}
	else
	{
		for( RwInt32 i = 0 ; i < NUM_SLOT ; ++i )
		{
			m_anFocusEffect[i] = m_anFocusEffect[i] & ~SLOT_FOCUS_CAN_MOVE;
		}
	}
}

VOID CItemUpgradeGui::DisableSlot( RwBool bDisable )
{
	for( RwInt32 i = 0 ; i < NUM_SLOT ; ++i )
	{
		m_asurDisableSlot[i].Show( bDisable );
	}
}

VOID CItemUpgradeGui::ClickEffect( RwBool bPush, RwInt32 nSlotIdx /* = -1  */ )
{
	CRectangle rtScreen = m_pThis->GetScreenRect();

	if( bPush )
	{
		m_surSlot[nSlotIdx].SetRect( rtScreen.left + m_rtSlot[nSlotIdx].left + ICONPUSH_SIZEDIFF, rtScreen.top + m_rtSlot[nSlotIdx].top + ICONPUSH_SIZEDIFF,
			rtScreen.left + m_rtSlot[nSlotIdx].right - ICONPUSH_SIZEDIFF, rtScreen.top + m_rtSlot[nSlotIdx].bottom - ICONPUSH_SIZEDIFF );
	}
	else if( m_nPushDownIndex >= 0 )
	{
		m_surSlot[m_nPushDownIndex].SetRect( rtScreen.left + m_rtSlot[m_nPushDownIndex].left, rtScreen.top + m_rtSlot[m_nPushDownIndex].top,
			rtScreen.left + m_rtSlot[m_nPushDownIndex].right, rtScreen.top + m_rtSlot[m_nPushDownIndex].bottom );
	}	

	m_nPushDownIndex = nSlotIdx;
}

VOID CItemUpgradeGui::SetAttributeSlotEffect( RwInt32 nSlotIdx, RwBool bInSlot )
{
	if(nSlotIdx >= NUM_SLOT)
		return;

	if( bInSlot )
	{
		if( nSlotIdx == SLOT_ITEM )
		{
			if( !m_pHoipoiStone )
			{
				m_aSlotEffect[SLOT_ITEM].m_surSlotEffect.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "ItemUpgrade.srf", "srfItemEffectNone" ) );
			}
			else
			{
				switch( m_nStoneSlotIdx )
				{
				case SLOT_HONEST: m_aSlotEffect[SLOT_ITEM].m_surSlotEffect.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "ItemUpgrade.srf", "srfItemEffectHonest" ) ); break;
				case SLOT_STRANGE: m_aSlotEffect[SLOT_ITEM].m_surSlotEffect.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "ItemUpgrade.srf", "srfItemEffectStrange" ) ); break;
				case SLOT_WILD: m_aSlotEffect[SLOT_ITEM].m_surSlotEffect.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "ItemUpgrade.srf", "srfItemEffectWild" ) ); break;
				case SLOT_ELEGANT: m_aSlotEffect[SLOT_ITEM].m_surSlotEffect.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "ItemUpgrade.srf", "srfItemEffectElegant" ) ); break;
				case SLOT_FUNNY: m_aSlotEffect[SLOT_ITEM].m_surSlotEffect.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "ItemUpgrade.srf", "srfItemEffectFunny" ) ); break;
				}
			}

			CRectangle rtScreen = m_pThis->GetScreenRect();
			m_aSlotEffect[SLOT_ITEM].m_surSlotEffect.SetPosition( rtScreen.left + m_aSlotEffect[SLOT_ITEM].m_rtEffectSlot.left, rtScreen.top + m_aSlotEffect[SLOT_ITEM].m_rtEffectSlot.top );
			m_aSlotEffect[SLOT_ITEM].SetState( stSLOTEFFECT::SLOT_EFFECT_ON );
		}
		else // hoipoi stone slot
		{
			if( m_pUpgradeItem )
			{
				m_TempItemSlot.m_surSlotEffect.SetSurface( *m_aSlotEffect[SLOT_ITEM].m_surSlotEffect.GetSurface() );

				switch( m_nStoneSlotIdx )
				{
				case SLOT_HONEST: m_aSlotEffect[SLOT_ITEM].m_surSlotEffect.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "ItemUpgrade.srf", "srfItemEffectHonest" ) ); break;
				case SLOT_STRANGE: m_aSlotEffect[SLOT_ITEM].m_surSlotEffect.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "ItemUpgrade.srf", "srfItemEffectStrange" ) ); break;
				case SLOT_WILD: m_aSlotEffect[SLOT_ITEM].m_surSlotEffect.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "ItemUpgrade.srf", "srfItemEffectWild" ) ); break;
				case SLOT_ELEGANT: m_aSlotEffect[SLOT_ITEM].m_surSlotEffect.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "ItemUpgrade.srf", "srfItemEffectElegant" ) ); break;
				case SLOT_FUNNY: m_aSlotEffect[SLOT_ITEM].m_surSlotEffect.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "ItemUpgrade.srf", "srfItemEffectFunny" ) ); break;
				}

				CRectangle rtScreen = m_pThis->GetScreenRect();
				m_aSlotEffect[SLOT_ITEM].m_surSlotEffect.SetPosition( rtScreen.left + m_aSlotEffect[SLOT_ITEM].m_rtEffectSlot.left, rtScreen.top + m_aSlotEffect[SLOT_ITEM].m_rtEffectSlot.top );
				m_aSlotEffect[SLOT_ITEM].SetState( stSLOTEFFECT::SLOT_EFFECT_ON );

				m_TempItemSlot.m_surSlotEffect.SetPosition( rtScreen.left + m_TempItemSlot.m_rtEffectSlot.left, rtScreen.top + m_TempItemSlot.m_rtEffectSlot.top );
				m_TempItemSlot.SetState( stSLOTEFFECT::SLOT_EFFECT_OFF );
			}

			m_aSlotEffect[nSlotIdx].SetState( stSLOTEFFECT::SLOT_EFFECT_ON );
		}
	}	
	else
	{
		if( nSlotIdx == SLOT_ITEM )
		{
			m_aSlotEffect[SLOT_ITEM].SetState( stSLOTEFFECT::SLOT_EFFECT_OFF );
		}
		else
		{
			// 호이포이스톤이 완전히 없어졌을때만 원래 슬롯을 무속성이펙트로 돌린다. 
			if( m_pUpgradeItem && !m_pHoipoiStone )
			{
				m_TempItemSlot.m_surSlotEffect.SetSurface( *m_aSlotEffect[SLOT_ITEM].m_surSlotEffect.GetSurface() );
				m_aSlotEffect[SLOT_ITEM].m_surSlotEffect.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "ItemUpgrade.srf", "srfItemEffectNone" ) );

				CRectangle rtScreen = m_pThis->GetScreenRect();
				m_aSlotEffect[SLOT_ITEM].m_surSlotEffect.SetPosition( rtScreen.left + m_aSlotEffect[SLOT_ITEM].m_rtEffectSlot.left, rtScreen.top + m_aSlotEffect[SLOT_ITEM].m_rtEffectSlot.top );
				m_aSlotEffect[SLOT_ITEM].SetState( stSLOTEFFECT::SLOT_EFFECT_ON );

				m_TempItemSlot.m_surSlotEffect.SetPosition( rtScreen.left + m_TempItemSlot.m_rtEffectSlot.left, rtScreen.top + m_TempItemSlot.m_rtEffectSlot.top );
				m_TempItemSlot.SetState( stSLOTEFFECT::SLOT_EFFECT_OFF );
			}

			m_aSlotEffect[nSlotIdx].SetState( stSLOTEFFECT::SLOT_EFFECT_OFF );
		}
	}
}

RwBool CItemUpgradeGui::IsUpgradableItem( CNtlSobItemAttr* pItemAttr )
{
	if( pItemAttr->IsNeedToIdentify() )
		return FALSE;

	sITEM_TBLDAT* pData = pItemAttr->GetItemTbl();

	if( !pData )
		return FALSE;

	//if( pItemAttr->GetGrade() >= NTL_ITEM_MAX_GRADE )
	//	return FALSE;

	if( EQUIP_TYPE_MAIN_WEAPON == pData->byEquip_Type ||
		EQUIP_TYPE_SUB_WEAPON == pData->byEquip_Type ||
		EQUIP_TYPE_ARMOR == pData->byEquip_Type )
	{
		return TRUE;
	}

	return FALSE;
}

RwBool CItemUpgradeGui::IsHoipoiStone( CNtlSobItemAttr* pItemAttr )
{
	if( pItemAttr->IsNeedToIdentify() )
		return FALSE;

	sITEM_TBLDAT* pData = pItemAttr->GetItemTbl();

	if( !pData )
		return FALSE;

	if( pData->byItem_Type == ITEM_TYPE_STONE ||
		pData->byItem_Type == ITEM_TYPE_BLACK_STONE ||
		pData->byItem_Type == ITEM_TYPE_PURE_STONE )
		return TRUE;

	return FALSE;
}

RwBool CItemUpgradeGui::IsValidHoipoiStone( CNtlSobItemAttr* pItemAttr )
{
	sITEM_TBLDAT* pData = pItemAttr->GetItemTbl();

	if( !IsHoipoiStone( pItemAttr ) )
		return FALSE;

	if( !m_pUpgradeItem )
		return TRUE;

	CNtlSobItemAttr* pUpgradeItemAttr = reinterpret_cast<CNtlSobItemAttr*>( m_pUpgradeItem->GetSobAttr() );
	sITEM_TBLDAT* pItemData = pUpgradeItemAttr->GetItemTbl();
	NTL_ASSERT( pItemData, "CItemUpgradeGui::IsValidHoipoiStone : Item Must have ItemTable" );

	if( pData->byNeed_Level >= pItemData->byNeed_Level )
		return TRUE;

	return FALSE;
}

// 업그레이드 진행창에서 쓰일 호이포이스톤 이름. Html형식
//const WCHAR* CItemUpgradeGui::NeededHoipoiStoneName( CNtlSobItemAttr* pItemAttr )
//{
//	sITEM_TBLDAT* pItemData = pItemAttr->GetItemTbl();
//	CTextAllTable* pTextAllTable = API_GetTableContainer()->GetTextAllTable();
//	CTextTable* pItemTextTable = pTextAllTable->GetItemTbl();
//	CItemTable* pItemTable = API_GetTableContainer()->GetItemTable();
//	sITEM_TBLDAT* pHoipoiData = NULL;
//	
//	if( pItemData->byNeed_Level > 0 && pItemData->byNeed_Level <= 15 )
//	{
//		pHoipoiData = reinterpret_cast<sITEM_TBLDAT*>( pItemTable->FindData( ITEMUPGRADE_HOIPOISTONE30 ) );		
//	}
//	else if( pItemData->byNeed_Level > 30 && pItemData->byNeed_Level <= 40 )
//	{
//		pHoipoiData = reinterpret_cast<sITEM_TBLDAT*>( pItemTable->FindData( ITEMUPGRADE_HOIPOISTONE30 + 1) );				
//	}
//	else if( pItemData->byNeed_Level > 40 && pItemData->byNeed_Level <= 50 )
//	{
//		pHoipoiData = reinterpret_cast<sITEM_TBLDAT*>( pItemTable->FindData( ITEMUPGRADE_HOIPOISTONE30 + 2 ) );				
//	}
//	else if( pItemData->byNeed_Level > 50 && pItemData->byNeed_Level <= 60 )
//	{
//		pHoipoiData = reinterpret_cast<sITEM_TBLDAT*>( pItemTable->FindData( ITEMUPGRADE_HOIPOISTONE30 + 3 ) );				
//	}
//	else if( pItemData->byNeed_Level > 70 )
//	{
//		pHoipoiData = reinterpret_cast<sITEM_TBLDAT*>( pItemTable->FindData( ITEMUPGRADE_HOIPOISTONE30 + 4 ) );				
//	}
//
//	return pItemTextTable->GetText( pHoipoiData->Name ).c_str();	
//}


RwBool CItemUpgradeGui::IsValidIconPickup( RwInt32 nClickIdx )
{
	if( m_eState == STATE_UPGRADE_PROCESS || 
		m_eState == STATE_PACKETWAIT ||
		m_eState == STATE_RESULT )
	{
		GetAlarmManager()->AlarmMessage( DST_ITEMUPGRADE_NOTIFY_CANNOT_CONTROL_STATE );
		return FALSE;
	}

	if( IsEmptySlot( nClickIdx ) )
	{
		return FALSE;
	}
	else if( IsUpgradeItemSlot( nClickIdx ) )
	{
		GetAlarmManager()->AlarmMessage( DST_ITEMUPGRADE_NOTIFY_CANNOT_PICKUP_UPGRADE_ITEM );
		return FALSE;
	}

	return TRUE;
}

RwBool CItemUpgradeGui::IsValidRightBtnProc( RwInt32 nClickIdx )
{
	if( m_eState == STATE_UPGRADE_PROCESS || 
		m_eState == STATE_PACKETWAIT ||
		m_eState == STATE_RESULT )
	{
		GetAlarmManager()->AlarmMessage( DST_ITEMUPGRADE_NOTIFY_CANNOT_CONTROL_STATE );
		return FALSE;
	}

	if( IsEmptySlot( nClickIdx ) )
	{
		return FALSE;
	}

	return TRUE;
}

RwBool CItemUpgradeGui::IsValidIdx( RwInt32 nIdx )
{
	if( nIdx >= 0 && nIdx < NUM_SLOT )
		return TRUE;

	return FALSE;
}

RwBool CItemUpgradeGui::IsEmptySlot( RwInt32 nSlotIdx )
{
	if( nSlotIdx == SLOT_ITEM )
	{
		if( m_pUpgradeItem )
			return FALSE;
		else
			return TRUE;
	}
	else
	{
		if( m_nStoneSlotIdx == nSlotIdx )
			return FALSE;
		else 
			return TRUE;
	}	
}

RwBool CItemUpgradeGui::IsUpgradeItemSlot( RwInt32 nSlotIdx )
{
	return ( SLOT_ITEM == nSlotIdx ) ? TRUE : FALSE ;
}

RwBool CItemUpgradeGui::IsHoipoiStoneSlot( RwInt32 nSlotIdx )
{
	return ( m_nStoneSlotIdx == nSlotIdx ) ? TRUE : FALSE ;
}

CNtlSobItem* CItemUpgradeGui::GetSlotObject( RwInt32 nSlotIdx )
{
	if( IsUpgradeItemSlot( nSlotIdx ) )
		return m_pUpgradeItem;
	else if( IsHoipoiStoneSlot( nSlotIdx ) )
		return m_pHoipoiStone;

	return NULL;
}

VOID CItemUpgradeGui::OnMouseDown( const CKey& key )
{
	RwInt32 nClickIdx = GetChildSlotIdx( (RwInt32)key.m_fX, (RwInt32)key.m_fY );

	if( nClickIdx < 0 )
		return;

	if( !GetIconMoveManager()->IsActive() )
		ClickEffect( TRUE, nClickIdx );

	if( m_asurDisableSlot[nClickIdx].IsShow() )
		return;

	if( key.m_nID == UD_LEFT_BUTTON )
	{
		if( GetIconMoveManager()->IsActive() )
		{
			m_nLSelectedSlotIdx = nClickIdx;
		}
		else
		{
			if( IsValidIconPickup( nClickIdx ) )
				m_nLSelectedSlotIdx = nClickIdx;			
		}
	}
	else if( key.m_nID == UD_RIGHT_BUTTON )
	{
		if( !GetIconMoveManager()->IsActive() )
		{
			if( IsValidRightBtnProc( nClickIdx ) )
			{
				if( nClickIdx == SLOT_ITEM )
				{
					if( m_pUpgradeItem )
						m_nRSelectedSlotIdx = nClickIdx;
				}
				else
				{
					if( nClickIdx == m_nStoneSlotIdx )
						m_nRSelectedSlotIdx = nClickIdx;
				}				
			}					
		}
	}

	m_pThis->CaptureMouse();

	if( m_nLSelectedSlotIdx >= 0 && m_nRSelectedSlotIdx >= 0 )
	{
		m_nLSelectedSlotIdx = -1;
		m_nRSelectedSlotIdx = -1;
		m_pThis->ReleaseMouse();
		ClickEffect( FALSE );
	}
}

VOID CItemUpgradeGui::OnMouseUp( const CKey& key )
{
	RwInt32 nClickIdx = GetChildSlotIdx( (RwInt32)key.m_fX, (RwInt32)key.m_fY );
	ClickEffect( FALSE );

	m_pThis->ReleaseMouse();

	if( nClickIdx < 0 || !IsShow() )
	{
		m_nLSelectedSlotIdx = -1;
		m_nRSelectedSlotIdx = -1;
		return;
	}

	if( key.m_nID == UD_LEFT_BUTTON )
	{
		if( nClickIdx == m_nLSelectedSlotIdx )
		{
			if( GetIconMoveManager()->IsActive() )
			{
				if( GetIconMoveManager()->GetSrcSerial() != INVALID_SERIAL_ID )
				{
					SetItemSlot( GetIconMoveManager()->GetSrcSerial(), 
						GetIconMoveManager()->GetSrcPlace(),
						GetIconMoveManager()->GetSrcSlotIdx(),
						nClickIdx, TRUE );
				}
			}
			else
			{
				if( IsHoipoiStoneSlot( nClickIdx ) )
				{
					GetIconMoveManager()->IconMovePickUp( m_pHoipoiStone->GetSerialID(), PLACE_ITEMUPGRADE,
						nClickIdx, m_nStoneStackNum, m_pHoipoiStone->GetIcon()->GetImage() );
				}								
			}
		}

		m_nLSelectedSlotIdx = -1;
	}
	else if( key.m_nID == UD_RIGHT_BUTTON )
	{
		if( nClickIdx == m_nRSelectedSlotIdx )
		{
			if( !GetIconMoveManager()->IsActive() )
			{
				if( nClickIdx == SLOT_ITEM )
					DeleteUpgradeItem();					
				else
					DeleteHoipoiStone();

				// Info Wnd 변경
				if( m_nMouseOnIdx >= 0 && GetInfoWndManager()->GetRequestGui() == DIALOG_ITEMUPGRADE )
				{
					ShowInfoWnd( TRUE, m_nMouseOnIdx );					
				}
			}
		}		

		m_nRSelectedSlotIdx = -1;
	}
}

VOID CItemUpgradeGui::OnMouseMove( RwInt32 nKey, RwInt32 nXPos, RwInt32 nYPos )
{
	RwInt32 nMouseOnIdx = GetChildSlotIdx( nXPos, nYPos );

	if( nMouseOnIdx >= 0 )
	{
		m_anFocusEffect[nMouseOnIdx] = m_anFocusEffect[nMouseOnIdx] | SLOT_FOCUS;		

		if( m_nMouseOnIdx != nMouseOnIdx )
		{
			m_anFocusEffect[m_nMouseOnIdx] = m_anFocusEffect[m_nMouseOnIdx] & ~SLOT_FOCUS;
			m_nMouseOnIdx = nMouseOnIdx;

			ShowInfoWnd( TRUE, m_nMouseOnIdx );							

			if( nMouseOnIdx == m_nPushDownIndex )
				ClickEffect( TRUE, nMouseOnIdx );
			else if( m_nPushDownIndex >= 0 )
				ClickEffect( FALSE, m_nPushDownIndex );
		}
	}
	else
	{
		if( m_nMouseOnIdx >= 0 )
		{
			m_anFocusEffect[m_nMouseOnIdx] = m_anFocusEffect[m_nMouseOnIdx] & ~SLOT_FOCUS;
			m_nMouseOnIdx = -1;
			if( m_nPushDownIndex >= 0 )
				ClickEffect( FALSE, m_nPushDownIndex );
			ShowInfoWnd( FALSE );
		}
	}
}

VOID CItemUpgradeGui::OnMouseOut( gui::CComponent* pComponent )
{
	if( m_nMouseOnIdx >= 0 )
	{
		m_anFocusEffect[m_nMouseOnIdx] = m_anFocusEffect[m_nMouseOnIdx] & ~SLOT_FOCUS;
		m_nMouseOnIdx = -1;
		if( m_nPushDownIndex >= 0 )
			ClickEffect( FALSE, m_nPushDownIndex );
		ShowInfoWnd( FALSE );
	}
}

VOID CItemUpgradeGui::OnPaint(VOID)
{
	m_BackFocus.m_surSlotEffect.Render();

	if( m_pUpgradeItem )
		m_surSlot[SLOT_ITEM].Render();

	if( m_nStoneSlotIdx >= 0 )
		m_surSlot[m_nStoneSlotIdx].Render();

	for( RwInt32 i = 0 ; i < NUM_SLOT ; ++i )
	{
		if( m_anFocusEffect[i] )
			m_asurFocus[i].Render();

		m_asurDisableSlot[i].Render();
		m_aSlotEffect[i].m_surSlotEffect.Render();				
	}	

	m_TempItemSlot.m_surSlotEffect.Render();
}

VOID CItemUpgradeGui::OnMove( RwInt32 nXPos, RwInt32 nYPos )
{
	CRectangle rtScreen = m_pThis->GetScreenRect();
	CRectangle rect;

	for( RwInt32 i = 0 ; i < NUM_SLOT ; ++i )
	{
		m_surSlot[i].SetPosition( rtScreen.left + m_rtSlot[i].left, rtScreen.top + m_rtSlot[i].top );
		m_asurFocus[i].SetPosition( rtScreen.left + m_rtSlot[i].left, rtScreen.top + m_rtSlot[i].top );
		m_asurDisableSlot[i].SetPosition( rtScreen.left + m_rtSlot[i].left, rtScreen.top + m_rtSlot[i].top );
		m_aSlotEffect[i].m_surSlotEffect.SetPosition( rtScreen.left + m_aSlotEffect[i].m_rtEffectSlot.left, rtScreen.top + m_aSlotEffect[i].m_rtEffectSlot.top );
	}	

	m_TempItemSlot.m_surSlotEffect.SetPosition( rtScreen.left + m_TempItemSlot.m_rtEffectSlot.left, rtScreen.top + m_TempItemSlot.m_rtEffectSlot.top );
	m_BackFocus.m_surSlotEffect.SetPosition( rtScreen.left + m_BackFocus.m_rtEffectSlot.left, rtScreen.top + m_BackFocus.m_rtEffectSlot.top );
}

VOID CItemUpgradeGui::OnClickExitBtn( gui::CComponent* pComponent )
{
	if( m_eState == STATE_PACKETWAIT )
		return;

	GetDialogManager()->CloseDialog( DIALOG_ITEMUPGRADE );
}

VOID CItemUpgradeGui::OnClickCompoundBtn( gui::CComponent* pComponent )
{
	if( m_eState != STATE_UPGRADE_READY )
		return;

	SetState( STATE_UPGRADE_PROCESS );

	if( GetIconMoveManager()->GetSrcPlace() == PLACE_ITEMUPGRADE )
		GetIconMoveManager()->IconMoveEnd();	
}

VOID CItemUpgradeGui::OnClickCloseResult( gui::CComponent* pComponent )
{
	if( m_pHoipoiStone && m_pUpgradeItem )
	{
		CNtlSobItemAttr* pHoipoiAttr = reinterpret_cast<CNtlSobItemAttr*>( m_pHoipoiStone->GetSobAttr() );
		CNtlSobItemAttr* pItemAttr = reinterpret_cast<CNtlSobItemAttr*>( m_pUpgradeItem->GetSobAttr() );

		/*if( !IsUpgradableItem( pItemAttr ) )
		{
		SetState( STATE_UPGRADE_READYFAIL_NOMOREUPGRADE );
		}
		else */
		if( IsValidHoipoiStone( pHoipoiAttr ) )
		{
			SetState( STATE_UPGRADE_READY );
		}
		else
		{
			SetState( STATE_UPGRADE_READYFAIL_NOT_MATCHED_HOIPOISTONE );
		}						
	}
	else if( m_pUpgradeItem )
	{
		SetState( STATE_ITEM_IN );
	}
	else if( m_pHoipoiStone )
	{
		SetState( STATE_HOIPOI_IN );
	}
	else
	{
		SetState( STATE_NONE );
	}

	DestroyResultDialog();
}

VOID CItemUpgradeGui::OnCaptureMouseDown( const CKey& key )
{
	CAPTURE_MOUSEDOWN_RAISE( DIALOG_ITEMUPGRADE, key.m_fX, key.m_fY );
}

VOID CItemUpgradeGui::OnCaptureMouseWheel( RwInt32 nFlag, RwInt16 sDelta, CPos& pos )
{
	gui::CScrollBar* pScrollBar = m_phtmlInfoText->GetScrollBar();

	RwInt32 nValue = pScrollBar->GetValue();
	RwInt32 nMaxValue = pScrollBar->GetMaxValue();
	RwInt32 nDelta = nValue - ( sDelta * 18 ) / GUI_MOUSE_WHEEL_DELTA;

	if( nDelta < 0 )
		nDelta = 0;
	else if( nDelta > nMaxValue )
		nDelta = nMaxValue;

	pScrollBar->SetValue( nDelta );	
}

VOID CItemUpgradeGui::OnResultItemIconPaint(VOID)
{
	m_surUpgradeItemIcon.Render();
}

VOID CItemUpgradeGui::OnResultItemIconMove( RwInt32 nOldX, RwInt32 nOldY )
{
	CRectangle rtScreen = m_ppnlUpgradeItemIcon->GetScreenRect();

	m_surUpgradeItemIcon.SetPosition( rtScreen.left, rtScreen.top );
}

VOID CItemUpgradeGui::OnResultItemIconMouseEnter( gui::CComponent* pComponent )
{
	CRectangle rtScreen = m_ppnlUpgradeItemIcon->GetScreenRect();

	if( m_pUpgradeItem )
	{
		GetInfoWndManager()->ShowInfoWindow( TRUE, CInfoWndManager::INFOWND_ITEM, rtScreen.left, rtScreen.top,
			(VOID*)GetSlotObject( SLOT_ITEM ), DIALOG_ITEMUPGRADE );		
	}
	else
	{
		GetInfoWndManager()->ShowInfoWindow( TRUE, CInfoWndManager::INFOWND_JUST_WTEXT, rtScreen.left, rtScreen.top, 
			(VOID*)GetDisplayStringManager()->GetString( DST_ITEMUPGRADE_RESULT_REPORT_ITEM_DESTROY ), DIALOG_ITEMUPGRADE );
	}	
}

VOID CItemUpgradeGui::OnResultItemIconMouseLeave( gui::CComponent* pComponent )
{
	if( GetInfoWndManager()->GetRequestGui() == DIALOG_ITEMUPGRADE )
		GetInfoWndManager()->ShowInfoWindow( FALSE );
}	

VOID CItemUpgradeGui::OnUpgradeEffectEnd( gui::CComponent* pComponent )
{
	m_pflsUpgradeEffect->Show( false );
	SendUpgradePacket();	
}