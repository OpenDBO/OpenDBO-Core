#include "precomp_dboclient.h"
#include "PartyCharmGui.h"
// core
#include "NtlDebug.h"

// shared
#include "ItemTable.h"
#include "TextAllTable.h"
#include "TableContainer.h"
#include "CharmTable.h"

// presentation
#include "NtlPLDef.h"
#include "NtlPLGuiManager.h"

// simulation
#include "NtlSLGlobal.h"
#include "NtlSLLogic.h"
#include "NtlSobAvatar.h"
#include "NtlSLApi.h"
#include "NtlSobItem.h"
#include "NtlSobItemAttr.h"
#include "NtlSobManager.h"

// dbo
#include "DboEvent.h"
#include "DboEventGenerator.h"
#include "DboGlobal.h"
#include "DialogManager.h"
#include "SideDialogManager.h"
#include "IconMoveManager.h"
#include "DisplayStringManager.h"
#include "InfoWndManager.h"
#include "ChatGui.h"
#include "DboLogic.h"
#include "DumpCommand.h"
#include "DiceManager.h"


#define dINVALID_CHARM_PAGE		0xff
#define dCHARMSLOT_INDEX		0

CPartyCharm* CPartyCharm::m_pInstance = NULL;

CPartyCharm::CPartyCharm(const RwChar* pName)
:CNtlPLGui(pName)
,m_bSelect(FALSE)
,m_bFocus(FALSE)
,m_bLeftMouseDown(FALSE)
,m_bRightMouseDown(FALSE)
,m_bZennySlotDestination(FALSE)
,m_pCHARM_TBLDAT(NULL)
,m_pMoneyIconTexture(NULL)
{

}

CPartyCharm::~CPartyCharm()
{
}

RwBool CPartyCharm::Create()
{
	NTL_FUNCTION( "CPartyCharm::Create" );

	if(!CNtlPLGui::Create("gui\\PartyInven.rsr", "gui\\PartyCharm.srf", "gui\\PartyCharm.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");

	CRectangle rect;

	m_pMoneyIconTexture = Logic_CreateTexture( MONEYICON_NAME );

	// 닫기 버튼
	m_pExitButton = (gui::CButton*)GetComponent( "btnClose" );
	m_slotCloseButton = m_pExitButton->SigClicked().Connect(this, &CPartyCharm::OnClicked_CloseButton);

	// 다이얼로그의 이름 스태틱
	m_pDialogName		= (gui::CStaticBox*)GetComponent( "stbDialogName" );
	m_pDialogName->CreateFontStd(DEFAULT_FONT, 110, DEFAULT_FONT_ATTR);
	m_pDialogName->SetText( GetDisplayStringManager()->GetString(DST_PARTYGUI_CHARM) );

	// "목표금액"
	m_pDestZenny		= (gui::CStaticBox*)GetComponent( "stbZennyTitle" );	
	m_pDestZenny->SetText( GetDisplayStringManager()->GetString(DST_PARTYGUI_DEST_ZENNY) );

	// 목표 Zenny
	m_pDestZennyPoint	= (gui::CStaticBox*)GetComponent( "stbZennyPoint" );
	m_pDestZennyPoint->SetText(L"0");
	m_pDestZennyPoint->Show(false);

	// 아바타 이름
	m_pAvatarName		= (gui::CStaticBox*)GetComponent( "stbAvatarName" );
	m_pAvatarName->SetText( Logic_GetAvatarName() );

	// 현재 자신이 투자한 Zenny
	m_pCurZenny			= (gui::CStaticBox*)GetComponent( "stbCurZenny" );
	m_pCurZenny->SetText(L"0");

	// 전체 Zenny 게이지
	m_pZennyGauge		= (gui::CProgressBar*)GetComponent( "gaugeZenny" );

	// Zenny 투자 버튼
	m_pZennyButton		= (gui::CButton*)GetComponent( "ZennyButton" );

	// 부적 발동 버튼
	m_pActivateButton	= (gui::CButton*)GetComponent( "ActivateButton" );

	m_slotBtnZenny		= m_pZennyButton->SigClicked().Connect(this, &CPartyCharm::OnClicked_ZennyButton);
	m_slotActivateButton = m_pActivateButton->SigClicked().Connect(this, &CPartyCharm::OnClicked_ActivateButton);

	m_pActivateButton->SetText( GetDisplayStringManager()->GetString(DST_PARTYGUI_CHARM_USE) );

	// 파티원 이름과 투자한 제니 게이지
	char acBuffer[128] = "";
	RwInt32 iMemberGaugeBackY = 175;
	for( RwInt8 i = 0 ; i < NTL_MAX_MEMBER_IN_PARTY ; ++i )
	{
		sprintf_s(acBuffer, 128, "stbMemberName%d", i);
		m_MemberVictim[i].pName			= (gui::CStaticBox*)GetComponent(acBuffer);
		m_MemberVictim[i].pName->Show(false);

		sprintf_s(acBuffer, 128, "gaugeMember%dZennyGauge", i);
		m_MemberVictim[i].pZennyGauge	= (gui::CProgressBar*)GetComponent(acBuffer);
		m_MemberVictim[i].pZennyGauge->Show(false);

		m_MemberVictim[i].srfGaugeBack.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "PartyCharm.srf", "srfMemberGaugeBack" ) );
		m_MemberVictim[i].srfGaugeBack.SetPositionfromParent(110, iMemberGaugeBackY);

		iMemberGaugeBackY += 21;
	}

	// 슬롯 포커스 이펙트
	m_FocusEffect.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfSlotFocusEffect" ) );
	m_FocusEffect.SetPositionfromParent(16, 46);

	// 슬롯 셀렉트 이펙트
	m_SelectEffect.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfSlotFocusEffect" ) );
	m_SelectEffect.SetPositionfromParent(16, 46);

	// 부적슬롯이 있는 공간의 배경
	m_srfCharmPanel.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "PartyCharm.srf", "srfCharmPanel" ) );
	m_srfCharmPanel.SetPositionfromParent(7, 32);	

	// 부적 슬롯 GUI
	m_CharmSlot.Create(m_pThis, SDIALOG_PARTY_CHRAM, REGULAR_SLOT_ITEM_TABLE);
	m_CharmSlot.SetPosition_fromParent(18, 48);

	// 제니 슬롯 Destination 이미지
	m_srfZennySlotDestination.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "PartyCharm.srf", "srfBtnOctionZennyDestination" ) );
	m_srfZennySlotDestination.SetPositionfromParent(11, 126);

	// 제니 아이콘
	m_srfZennyIcon.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "PartyCharm.srf", "srfZennyIcon" ) );
	m_srfZennyIcon.SetPositionfromParent(140, 68);

	// 부적 슬롯 배경
	m_srfCharmSlot.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "PartyCharm.srf", "srfCharmBack" ) );
	m_srfCharmSlot.SetPositionfromParent(16, 46);

	// 전체 제니 게이지 배경
	m_srfZennyGaugeBack.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "PartyCharm.srf", "srfZennyGaugeBack" ) );
	m_srfZennyGaugeBack.SetPositionfromParent(61, 83);

	// sig
	m_slotMouseDown		= m_pThis->SigMouseDown().Connect( this, &CPartyCharm::OnMouseDown );
	m_slotMouseUp		= m_pThis->SigMouseUp().Connect( this, &CPartyCharm::OnMouseUp );
	m_slotMove			= m_pThis->SigMove().Connect( this, &CPartyCharm::OnMove);
	m_slotMouseMove		= m_pThis->SigMouseMove().Connect( this, &CPartyCharm::OnMouseMove );
	m_slotMouseLeave	= m_pThis->SigMouseLeave().Connect( this, &CPartyCharm::OnMouseLeave );
	m_slotPaint			= m_pThis->SigPaint().Connect( this, &CPartyCharm::OnPaint );
	m_slotPostPaint		= m_pZennyButton->SigPaint().Connect( this, &CPartyCharm::OnPostPaint );

	LinkMsg(g_EventCharm);
	LinkMsg(g_EventCharmVictim);	
	LinkMsg(g_EventCalcPopupResult);
	LinkMsg(g_EventPartyUpdateValue);
	LinkMsg(g_EventIconMoveClick);
	LinkMsg(g_EventDirectMoveIcon);

	Show(false);

	NTL_RETURN(TRUE);
}

VOID CPartyCharm::Destroy()
{
	NTL_FUNCTION("CPartyCharm::Destroy");

	Clear();

	GetDiceManager()->Cancel(DICE_WORKD_ID_PARTY_CHARM_DICE);

	m_CharmSlot.Destroy();

	Logic_DeleteTexture(m_pMoneyIconTexture);
	m_pMoneyIconTexture = NULL;

	CheckInfoWindow();
	CheckIconMove();

	UnLinkMsg(g_EventCharm);
	UnLinkMsg(g_EventCharmVictim);
	UnLinkMsg(g_EventCalcPopupResult);
	UnLinkMsg(g_EventPartyUpdateValue);
	UnLinkMsg(g_EventIconMoveClick);
	UnLinkMsg(g_EventDirectMoveIcon);

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy(); 

	NTL_RETURNVOID();
}

VOID CPartyCharm::CreateInstance()
{
	if( m_pInstance )
		return;

	m_pInstance = NTL_NEW CPartyCharm("PartyCharmGui");
	if( !m_pInstance->Create() )
		NTL_DELETE(m_pInstance);

	CNtlPLGuiManager::GetInstance()->AddGui(m_pInstance);	
	GetSideDialogManager()->RegistDialog(SDIALOG_PARTY_CHRAM, m_pInstance, &CPartyCharm::SwitchDialog);
}

VOID CPartyCharm::DestroyInstance()
{
	if( !m_pInstance )
		return;

	GetSideDialogManager()->UnRegistDialog(SDIALOG_PARTY_CHRAM);
	GetNtlGuiManager()->RemoveGui(m_pInstance);
	m_pInstance->Destroy();
	NTL_DELETE(m_pInstance);
}

VOID CPartyCharm::ClearBagLock()
{
	if( !m_pInstance )
		return;

	if( m_pInstance->m_BagLockInfo.hHandle != INVALID_SERIAL_ID )
		CDboEventGenerator::DialogEvent(DIALOGEVENT_END_TRADING_ITEM_IN_BAG, PLACE_PARTYCHARM, PLACE_BAG,
										m_pInstance->m_BagLockInfo.byPlace,
										m_pInstance->m_BagLockInfo.byPos,
										m_pInstance->m_BagLockInfo.hHandle);
}

VOID CPartyCharm::Clear()
{
	GetNtlSLGlobal()->GetAdjustZennyInfo()->SetAdjustZenny( SAdjustZennyInfo::PARTY_CHARM, 0 );

	m_pDestZennyPoint->Show(false);
	m_pCurZenny->SetText(L"0");	
	m_pZennyGauge->SetPos(0);

	for( RwUInt8 i = 0 ; i < NTL_MAX_MEMBER_IN_PARTY ; ++i )
	{
		m_MemberVictim[i].pZennyGauge->SetPos(0);
		m_MemberVictim[i].pZennyGauge->DeleteToolTip();
		m_MemberVictim[i].uiZenny	= 0;
	}

	m_CharmSlot.Clear();

	m_bSelect			= FALSE;
	m_bFocus			= FALSE;
	m_bLeftMouseDown	= FALSE;
	m_bRightMouseDown	= FALSE;	

	// 락 정보 지우기
	m_BagLockInfo.byPlace	= INVALID_BYTE;
	m_BagLockInfo.byPos		= INVALID_BYTE;
	m_BagLockInfo.hHandle	= INVALID_SERIAL_ID;

	m_pCHARM_TBLDAT		= NULL;

	CheckInfoWindow();	
}

VOID CPartyCharm::CheckInfoWindow()
{
	if( GetInfoWndManager()->GetInfoWndState() == CInfoWndManager::INFOWND_TABLE_ITEM &&
		GetInfoWndManager()->GetRequestGui() == SDIALOG_PARTY_CHRAM )
	{
		FocusEffect(FALSE);
		GetInfoWndManager()->ShowInfoWindow( FALSE );
	}
}

VOID CPartyCharm::CheckIconMove()
{
	if( GetIconMoveManager()->GetSrcPlace() == PLACE_SUB_PARTYCHARM_ZENNY )
		GetIconMoveManager()->IconMoveEnd();
}

VOID CPartyCharm::AddMember(sPartyMember* pMember)
{
	for( RwUInt8 i = 0 ; i < NTL_MAX_MEMBER_IN_PARTY ; ++i )
	{
		if( m_MemberVictim[i].hMember == INVALID_SERIAL_ID )
		{
			m_MemberVictim[i].pName->SetText( pMember->wszMemberName );
			m_MemberVictim[i].pName->Show(true);
			m_MemberVictim[i].pZennyGauge->Show(true);
			m_MemberVictim[i].pZennyGauge->DeleteToolTip();
			m_MemberVictim[i].hMember	= pMember->hSerial;
			m_MemberVictim[i].uiZenny	= 0;
			break;
		}
	}
}

VOID CPartyCharm::DelMember(SERIAL_HANDLE hHandle)
{
	RwBool bDeleted = FALSE;

	for( RwUInt8 i = 0 ; i < NTL_MAX_MEMBER_IN_PARTY ; ++i )
	{
		if( m_MemberVictim[i].hMember == hHandle )
		{
			for( RwUInt8 k = i ; k < NTL_MAX_MEMBER_IN_PARTY ; ++k )
			{
				if( k < NTL_MAX_MEMBER_IN_PARTY-1 &&
					m_MemberVictim[k+1].hMember != INVALID_SERIAL_ID )
				{
					WCHAR awcBuffer[128] = L"";
					swprintf_s(awcBuffer, 128, GetDisplayStringManager()->GetString(DST_PARTY_TOOLTIP_ZENNY), m_MemberVictim[k+1].uiZenny);

					m_MemberVictim[k].pName->SetText( m_MemberVictim[k+1].pName->GetText().c_str() );
					m_MemberVictim[k].pZennyGauge->SetPos( m_MemberVictim[k+1].pZennyGauge->GetPos() );
					m_MemberVictim[k].pZennyGauge->SetToolTip(awcBuffer);

					m_MemberVictim[k].hMember	= m_MemberVictim[k+1].hMember;
					m_MemberVictim[k].uiZenny	= m_MemberVictim[k+1].uiZenny;
				}
				else
				{
					m_MemberVictim[k].pName->Clear();
					m_MemberVictim[k].pZennyGauge->SetPos(0);
					m_MemberVictim[k].pZennyGauge->DeleteToolTip();

					m_MemberVictim[i].pName->Show(false);
					m_MemberVictim[i].pZennyGauge->Show(false);

					m_MemberVictim[k].hMember	= INVALID_SERIAL_ID;
					m_MemberVictim[k].uiZenny	= 0;
				}
			}			

			break;
		}
	}
}

CPartyCharm::sMemberVictim* CPartyCharm::GetMemberVictim(SERIAL_HANDLE hHandle)
{
	for( RwUInt8 i = 0 ; i < NTL_MAX_MEMBER_IN_PARTY ; ++i )
	{
		if( m_MemberVictim[i].hMember == hHandle )
			return &(m_MemberVictim[i]);
	}

	return NULL;
}

RwUInt32 CPartyCharm::GetTotalCharmZenny()
{
	RwUInt32 uiPoint = 0;

	for( RwUInt8 i = 0 ; i < NTL_MAX_MEMBER_IN_PARTY ; ++i )
		uiPoint += m_MemberVictim[i].uiZenny;

	return uiPoint;
}

VOID CPartyCharm::RefreshTotalZenny()
{
	RwUInt32 uiPoint = 0;

	for( RwUInt8 i = 0 ; i < NTL_MAX_MEMBER_IN_PARTY ; ++i )
		uiPoint += m_MemberVictim[i].uiZenny;

	RwInt32 uiPercent = (RwInt32)((RwReal)uiPoint/(RwReal)m_pCHARM_TBLDAT->dwNeed_Zenny*100.f);

	if( uiPercent < 0 )
		uiPercent = 0;

	m_pZennyGauge->SetPos(uiPercent);
}

RwBool CPartyCharm::PtinSlot(RwInt32 iX, RwInt32 iY)
{
	return m_CharmSlot.PtInRect(iX, iY);
}

VOID CPartyCharm::OnClicked_CloseButton(gui::CComponent* pComponent)
{
	GetDboGlobal()->GetGamePacketGenerator()->SendPartyCloseCharmSlot();
}

VOID CPartyCharm::OnClicked_ZennyButton(gui::CComponent* pComponent)
{
	if( !m_pCHARM_TBLDAT )
		return;

	if( m_pCHARM_TBLDAT->dwNeed_Zenny == 0 )
		return;

	if( GetIconMoveManager()->IsActive() )
	{
		// 제니 등록
		GetIconMoveManager()->IconMovePutDown(PLACE_SUB_PARTYCHARM_ZENNY, INVALID_SERIAL_ID, INVALID_INDEX);
	}		
	else
	{
		SERIAL_HANDLE hAvatar = Logic_GetAvatarHandle();
		sMemberVictim* pMemberVictim = GetMemberVictim(hAvatar);
		NTL_ASSERT(pMemberVictim, "CPartyCharm::OnClicked_ZennyButton, Not exist avatar charm gui info");

		if( pMemberVictim )
		{
			if( pMemberVictim->uiZenny > 0 )
			{
				// 내가 등록한 제니가 있다면...다시 계산
				CRectangle rtZenny = m_pZennyButton->GetScreenRect();
				CDboEventGenerator::CalcPopupShow(TRUE, INVALID_SERIAL_ID, PLACE_SUB_PARTYCHARM_ZENNY, 
					rtZenny.right, rtZenny.bottom, pMemberVictim->uiZenny);
			}
		}
	}
}

VOID CPartyCharm::OnClicked_ActivateButton(gui::CComponent* pComponent)
{
	if( Logic_I_am_PartyLeader() )
	{
		// 부적 아이템이 등록되어 있지 않다
		if( m_pCHARM_TBLDAT == NULL )
			return;

		if( GetTotalCharmZenny() < m_pCHARM_TBLDAT->dwNeed_Zenny )
		{
			GetAlarmManager()->FormattedAlarmMessage(DST_PARTY_NEED_ZENNY, FALSE, NULL, m_pCHARM_TBLDAT->dwNeed_Zenny);
			return;
		}

		GetDboGlobal()->GetGamePacketGenerator()->SendPartyAvtivateCharm();
	}
}

VOID CPartyCharm::OnMouseDown(const CKey& key)
{
	if( PtinSlot((RwInt32)key.m_fX, (RwInt32)key.m_fY) )
	{
		if( key.m_nID == UD_LEFT_BUTTON )
		{
			m_bLeftMouseDown = TRUE;
			m_pThis->CaptureMouse();			
		}
		else if( key.m_nID == UD_RIGHT_BUTTON )
		{
			m_pThis->CaptureMouse();
			m_bRightMouseDown = TRUE;
		}

		return;
	}
}

VOID CPartyCharm::OnMouseUp(const CKey& key)
{
	m_pThis->ReleaseMouse();

	if( !IsShow() )
	{
		m_bLeftMouseDown	= FALSE;
		m_bRightMouseDown	= FALSE;
		return;
	}

	if( PtinSlot((RwInt32)key.m_fX, (RwInt32)key.m_fY) )
	{
		if( key.m_nID == UD_LEFT_BUTTON )
		{
			if( m_bLeftMouseDown )
			{
				// 제물 아이템을 등록한다
				if( GetIconMoveManager()->IsActive() )
				{
					GetIconMoveManager()->IconMovePutDown(PLACE_PARTYCHARM, INVALID_SERIAL_ID, dCHARMSLOT_INDEX);
					SelectEffect(FALSE);
				}
			}
		}
		else if( key.m_nID == UD_RIGHT_BUTTON )
		{
			if( m_bRightMouseDown )
			{
				// 등록된 제물 아이템을 해제한다.					
				GetDboGlobal()->GetGamePacketGenerator()->SendPartyUnRegVictimItem(dCHARMSLOT_INDEX);
				SelectEffect(FALSE);
			}
		}

		return;
	}

	m_bLeftMouseDown	= FALSE;
	m_bRightMouseDown	= FALSE;
}

VOID CPartyCharm::OnMove(RwInt32 iOldX, RwInt32 iOldY)
{
	CRectangle rtScreen = m_pThis->GetScreenRect();

	for( RwInt8 i = 0 ; i < NTL_MAX_MEMBER_IN_PARTY ; ++i )
		m_MemberVictim[i].srfGaugeBack.SetPositionbyParent(rtScreen.left, rtScreen.top);

	m_CharmSlot.SetParentPosition(rtScreen.left, rtScreen.top);

	m_FocusEffect.SetPositionbyParent(rtScreen.left, rtScreen.top);
	m_SelectEffect.SetPositionbyParent(rtScreen.left, rtScreen.top);
	m_srfCharmPanel.SetPositionbyParent(rtScreen.left, rtScreen.top);
	m_srfZennySlotDestination.SetPositionbyParent(rtScreen.left, rtScreen.top);
	m_srfZennyIcon.SetPositionbyParent(rtScreen.left, rtScreen.top);
	m_srfCharmSlot.SetPositionbyParent(rtScreen.left, rtScreen.top);
	m_srfZennyGaugeBack.SetPositionbyParent(rtScreen.left, rtScreen.top);

	CheckInfoWindow();
}

VOID CPartyCharm::OnMouseMove(RwInt32 nFlags, RwInt32 nX, RwInt32 nY)
{
	if( PtinSlot(nX, nY) )
	{
		FocusEffect(TRUE);

		if( m_CharmSlot.GetSerial() != INVALID_SERIAL_ID )
		{
			CRectangle rtScreen = m_pThis->GetScreenRect();
			GetInfoWndManager()->ShowInfoWindow( TRUE, CInfoWndManager::INFOWND_TABLE_ITEM,
				rtScreen.left + m_CharmSlot.GetX_fromParent(),
				rtScreen.top + m_CharmSlot.GetY_fromParent(),
				m_CharmSlot.GetItemTable(), SDIALOG_PARTY_CHRAM );
			return;
		}
	}

	FocusEffect(FALSE);
	GetInfoWndManager()->ShowInfoWindow( FALSE );
}

VOID CPartyCharm::OnMouseLeave(gui::CComponent* pComponent)
{
	FocusEffect(FALSE);
	GetInfoWndManager()->ShowInfoWindow( FALSE );
}

VOID CPartyCharm::FocusEffect(RwBool bPush)
{
	m_bFocus = bPush;
}

VOID CPartyCharm::SelectEffect(RwBool bPush)
{
	m_bSelect = bPush;
}

VOID CPartyCharm::OnPaint()
{
	for( RwInt8 i = 0 ; i < NTL_MAX_MEMBER_IN_PARTY ; ++i )
	{
		if( m_MemberVictim[i].hMember == INVALID_SERIAL_ID )
			continue;

		m_MemberVictim[i].srfGaugeBack.Render();
	}

	m_srfCharmPanel		.Render();
	m_srfZennyIcon		.Render();
	m_srfCharmSlot		.Render();
	m_srfZennyGaugeBack	.Render();

	m_CharmSlot.Paint();	

	if( m_bSelect )
		m_SelectEffect.Render();

	if( m_bFocus )
		m_FocusEffect.Render();
}

VOID CPartyCharm::OnPostPaint()
{
	if( FALSE == m_bZennySlotDestination )
		return;

	if( m_CharmSlot.GetSerial() == INVALID_SERIAL_ID )
		return;

	m_srfZennySlotDestination.Render();
}

RwInt32 CPartyCharm::SwitchDialog(bool bOpen)
{
	if( bOpen )
	{
		CNtlParty* pParty = GetNtlSLGlobal()->GetSobAvatar()->GetParty();
		if( pParty->IsOpenCharm() == FALSE )
			return -1;

		Show(true);
	}
	else
	{
		CheckIconMove();
		CheckInfoWindow();	

		Show(false);
	}

	return 1;
}

VOID CPartyCharm::HandleEvents( RWS::CMsg &msg )
{
	NTL_FUNCTION("CPartyCharm::HandleEvents");

	if(msg.Id == g_EventPartyUpdateValue)
	{
		SNtlEventPartyUpdate* pEvent = reinterpret_cast<SNtlEventPartyUpdate*>(msg.pData);

		switch(pEvent->iMessage)
		{
		case PMT_PARTY_LEADER_CHANGE:
			{
				if( pEvent->hSerialId == Logic_GetAvatarHandle() )
					m_pExitButton->Show(true);
				else
					m_pExitButton->Show(false);

				break;
			}
		case PMT_PARTY_MEMBER_ADD:
			{
				CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
				sPartyMember* pMember = reinterpret_cast<sPartyMember*>( pAvatar->GetParty()->GetMemberbyKey(pEvent->hSerialId) );
				AddMember(pMember);
				break;
			}
		case PMT_PARTY_MEMBER_DEL:
			{
				DelMember(pEvent->hSerialId);
				break;
			}
		}
	}
	else if(msg.Id == g_EventCharm)
	{
		SDboEventPartyCharm* pEvent = reinterpret_cast<SDboEventPartyCharm*>(msg.pData);

		switch(pEvent->iType)
		{
		case PCET_OPEN:
			{
				// 파티장만이 열고 닫을 수 있다.
				if( Logic_I_am_PartyLeader() )
					m_pExitButton->Show(true);
				else
					m_pExitButton->Show(false);

				GetSideDialogManager()->OpenDialog(SDIALOG_PARTY_CHRAM);

				break;
			}
		case PCET_CLOSE:
			{
				// 창을 닫으면 등록된 내용 전부를 지운다.
				GetSideDialogManager()->CloseDialog(SDIALOG_PARTY_CHRAM);
				CPartyCharm::ClearBagLock();
				Clear();
				break;
			}
		case PCET_ACTIVATE:
			{
				GetSideDialogManager()->CloseDialog(SDIALOG_PARTY_CHRAM);

				// PartyMenu쪽에 이벤트로 버프를 표시하도록 한다
				CDboEventGenerator::PartyCharm(PCET_DISPLAY_CHARM_BUFF, pEvent->iValue, pEvent->uiValue2);
				CPartyCharm::ClearBagLock();
				Clear();
				break;
			}
		}
	}
	else if( msg.Id == g_EventCharmVictim )
	{
		SDboEventPartyVictim* pEvent = reinterpret_cast<SDboEventPartyVictim*>( msg.pData );

		switch(pEvent->iType)
		{
		case PVI_REG_CHARM:
			{
				RwUInt32 uiItemIndex = pEvent->uiValue;
				sITEM_TBLDAT* pITEM_TBLDAT = Logic_GetItemDataFromTable(uiItemIndex);
				NTL_ASSERT(pITEM_TBLDAT, "CPartyCharm::HandleEvents, PVI_REG_CHARM, Not exist item table data of sob item of handle : " << uiItemIndex);

				m_CharmSlot.SetIcon(pITEM_TBLDAT->tblidx);

				m_pCHARM_TBLDAT = Logic_GetCharmDataFromTable(pITEM_TBLDAT->Charm_Tblidx);
				NTL_ASSERT(m_pCHARM_TBLDAT, "CPartyCharm::HandleEvents, PVI_REG_CHARM, Not exist charm table data of index : " << pITEM_TBLDAT->tblidx);

				m_pDestZennyPoint->Show(true);
				m_pDestZennyPoint->SetText((RwUInt32)m_pCHARM_TBLDAT->dwNeed_Zenny);				

				if( pEvent->bByMyself )
				{
					RwUInt32 uiBag = pEvent->uiValue3;
					RwUInt32 uiSlot = pEvent->uiValue4;
					CNtlInventory* pInventory = GetNtlSLGlobal()->GetSobAvatar()->GetInventory();
					SERIAL_HANDLE hBagHandle = pInventory->GetBagItem(uiBag);
					CNtlSobItem* pItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject(hBagHandle) );
					NTL_ASSERT(pItem, "CPartyCharm::HandleEvents, PVI_REG_ITEM, Can not find bag item of handle : " << hBagHandle);
					CNtlSobItem* pChildItem = pItem->GetChildItem(uiSlot);
					NTL_ASSERT(pChildItem, "CPartyCharm::HandleEvents, PVI_REG_ITEM, Can not find bag child item of handle : " << pItem->GetSerialID());

					// 락 정보 보관
					m_BagLockInfo.hHandle	= pChildItem->GetSerialID();
					m_BagLockInfo.byPlace	= (RwUInt8)uiBag;
					m_BagLockInfo.byPos		= (RwUInt8)uiSlot;

					// 락 걸기				
					CDboEventGenerator::DialogEvent(DIALOGEVENT_BEGIN_TRADING_ITEM_IN_BAG, PLACE_PARTYCHARM, PLACE_BAG,
						uiBag, uiSlot, m_BagLockInfo.hHandle );

					GetIconMoveManager()->IconMoveEnd();
				}

				break;
			}
		case PVI_UNREG_CHARM:
			{
				CPartyCharm::ClearBagLock();
				Clear();
				break;
			}
		case PVI_ZENNY:
			{
				sMemberVictim* pMemberVictim = GetMemberVictim(pEvent->uiValue);
				if(pMemberVictim)
				{
					WCHAR awcBuffer[128] = L"";
					swprintf_s(awcBuffer, 128, GetDisplayStringManager()->GetString(DST_PARTY_TOOLTIP_ZENNY), pEvent->uiValue2);
					pMemberVictim->uiZenny = pEvent->uiValue2;
					pMemberVictim->pZennyGauge->SetPos((RwInt32)((RwReal)pMemberVictim->uiZenny/(RwReal)m_pCHARM_TBLDAT->dwNeed_Zenny*100));
					pMemberVictim->pZennyGauge->SetToolTip(awcBuffer);

					RefreshTotalZenny();

					if( pMemberVictim->hMember == Logic_GetAvatarHandle() )
					{
						m_pCurZenny->SetText(pMemberVictim->uiZenny);
						GetNtlSLGlobal()->GetAdjustZennyInfo()->SetAdjustZenny( SAdjustZennyInfo::PARTY_CHARM, -1 * pMemberVictim->uiZenny);
						GetIconMoveManager()->IconMoveEnd();
					}
				}

				break;
			}
		case PVI_START_CHARM_DICE:
			{
				// 파티 부적 주사위를 연다
				CDboEventGenerator::DiceStart(DICE_WORKD_ID_PARTY_CHARM_DICE, m_CharmSlot.GetItemTable());
				break;
			}
		}
	}
	else if( msg.Id == g_EventCalcPopupResult )
	{
		SDboEventCalcPopupResult* pData = reinterpret_cast<SDboEventCalcPopupResult*>( msg.pData );

		if( pData->nSrcPlace == PLACE_SUB_PARTYCHARM_ZENNY )
		{
			GetIconMoveManager()->IconMovePickUp(INVALID_SERIAL_ID, PLACE_SUB_PARTYCHARM_ZENNY, INVALID_INDEX, pData->uiValue, m_pMoneyIconTexture);
		}
	}
	else if( msg.Id == g_EventPickedUpHide )
	{
		RwInt32 nSlotIdx = (RwInt32)msg.pData;
		if( nSlotIdx != PLACE_PARTYCHARM )
			NTL_RETURNVOID();

		SelectEffect(FALSE);
	}
	else if( msg.Id == g_EventIconMoveClick )
	{
		RwBool bPickUp = (RwBool)msg.pData;
		if( bPickUp )
		{
			if( GetIconMoveManager()->GetSrcPlace() == PLACE_BAG )
			{
				RwUInt32 uiTableIndex = GetIconMoveManager()->GetSrcSerial();
				sITEM_TBLDAT* pITEM_TBLDAT = Logic_GetItemDataFromSob( uiTableIndex );				

				// 미확인 아이템일수도 있다
				if( pITEM_TBLDAT )
				{
					// 부적 아이템을 들고 있는 상태라면
					if( pITEM_TBLDAT->byItem_Type == ITEM_TYPE_CHARM )
					{
						SelectEffect(TRUE);
					}
				}
			}
			else if( GetIconMoveManager()->GetSrcPlace() == PLACE_SUB_BAG_ZENNY )
			{
				m_bZennySlotDestination = TRUE;
			}			
		}
		else
		{
			SelectEffect(FALSE);
			m_bZennySlotDestination = FALSE;
		}		
	}
	else if( msg.Id == g_EventDirectMoveIcon )
	{
		SDboEventDirectMoveIcon* pEvent = reinterpret_cast<SDboEventDirectMoveIcon*>( msg.pData );

		if( PLACE_PARTYCHARM != pEvent->eDestPlace )
			NTL_RETURNVOID();

		if( m_CharmSlot.GetSerial() == INVALID_SERIAL_ID )
		{
			// 가방에서 이벤트로 파티참 다이얼로그로 부적을 등록한다
			Logic_ItemMoveSubProcBagChildToPartyCharmSlot(pEvent->uiSrcSerial, pEvent->uiSrcSlotIdx, dCHARMSLOT_INDEX);
			SelectEffect(FALSE);
		}
		else
		{
			// 부적 슬롯이 비어있지 않습니다
			GetAlarmManager()->AlarmMessage(DST_PARTY_CHARMSLOT_NOT_EMPTY);
		}
	}

	NTL_RETURNVOID();
}