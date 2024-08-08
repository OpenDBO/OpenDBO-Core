#include "precomp_dboclient.h"
#include "DojoAccountGui.h"


// core
#include "NtlDebug.h"

// presentation
#include "NtlPLGuiManager.h"

// simulation
#include "NtlWorldConcept.h"
#include "NtlSLEventFunc.h"
#include "NtlCameraController.h"

// cleint
#include "DboEventGenerator.h"
#include "DialogManager.h"
#include "DisplayStringManager.h"
#include "DboGlobal.h"


CDojoAccountGui::CDojoAccountGui(const RwChar* pName)
:CNtlPLGui(pName)
,m_hNPCSerial(INVALID_SERIAL_ID)
,m_iVisibleStart(0)
,m_bAscentDay(FALSE)
,m_bAscentSummary(FALSE)
,m_bAscentZenny(FALSE)
,m_uiDojoTableIndex(INVALID_TBLIDX)
{

}

CDojoAccountGui::~CDojoAccountGui()
{
	Destroy();
}

RwBool CDojoAccountGui::Create()
{
	NTL_FUNCTION( "CDojoAccountGui::Create" );

	if(!CNtlPLGui::Create("gui\\DojoAccount.rsr", "gui\\DojoAccount.srf", "gui\\DojoAccount.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis				= (gui::CDialog*)GetComponent("dlgMain");

	m_pTabButton		= (gui::CTabButton*)GetComponent( "TabButton" );	

	m_pHelpButton		= (gui::CButton*)GetComponent( "btnHelp" );
	m_pExitButton		= (gui::CButton*)GetComponent( "btnExit" );
	m_pDayButton		= (gui::CButton*)GetComponent( "btnDay" );
	m_pSummaryButton	= (gui::CButton*)GetComponent( "btnSummary" );
	m_pAmountButton		= (gui::CButton*)GetComponent( "btnAmount" );

	m_pScrollbar		= (gui::CScrollBar*)GetComponent("scbScroll");

	m_pDialogName		= (gui::CStaticBox*)GetComponent( "stbTitle" );
	m_pCreditStatic		= (gui::CStaticBox*)GetComponent( "stbCreditStatic" );
	m_pCredit			= (gui::CStaticBox*)GetComponent( "stbCredit" );


	RwInt32 iY = 100;
	CRectangle rect;

	for( RwUInt8 i = 0 ; i < dDISPLAY_ACCOUNT_NUM ; ++i )
	{
		rect.SetRectWH(16, iY, 91, 14);
		m_aAccountItemGui[i].pDay = NTL_NEW gui::CStaticBox( rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_CENTER );
		m_aAccountItemGui[i].pDay->CreateFontStd( DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
		m_aAccountItemGui[i].pDay->Enable(false);

		rect.SetRectWH(108, iY, 91, 14);
		m_aAccountItemGui[i].pSummary = NTL_NEW gui::CStaticBox( rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_CENTER );
		m_aAccountItemGui[i].pSummary->CreateFontStd( DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
		m_aAccountItemGui[i].pSummary->Enable(false);

		rect.SetRectWH(200, iY, 91, 14);
		m_aAccountItemGui[i].pZenny = NTL_NEW gui::CStaticBox( rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_CENTER );
		m_aAccountItemGui[i].pZenny->CreateFontStd( DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
		m_aAccountItemGui[i].pZenny->Enable(false);

		iY += 23;
	}

	m_srfCreditBackground.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfZennyBtnUp" ) );
	m_srfCreditBackground.SetPositionfromParent(145, 429);

	// 백라인
	m_srf3BackLine.SetType(CWindowby3::WT_HORIZONTAL);
	m_srf3BackLine.SetSurface( 0, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfBackLineTop" ) );
	m_srf3BackLine.SetSurface( 1, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfBackLineCenter" ) );
	m_srf3BackLine.SetSurface( 2, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfBackLineBottom" ) );
	m_srf3BackLine.SetSize(303, 414);
	m_srf3BackLine.SetPositionfromParent(9, 50);

	// 배경
	m_srf3Background.SetType(CWindowby3::WT_VERTICAL);
	m_srf3Background.SetSurface( 0, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "DojoAccount.srf", "srfBackgroundLeft" ) );
	m_srf3Background.SetSurface( 1, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "DojoAccount.srf", "srfBackgroundCenter" ) );
	m_srf3Background.SetSurface( 2, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "DojoAccount.srf", "srfBackgroundRight" ) );	
	m_srf3Background.SetSize(273, 318);
	m_srf3Background.SetPositionfromParent(18, 97);


	// 스트링	
	m_pDialogName->SetText(GetDisplayStringManager()->GetString(DST_DOJO_ACCOUNT_WAREHOUSE));

	std::wstring wstrText = GetDisplayStringManager()->GetString(DST_DOJO_ACCOUNT_EXPENDITURE);
	m_pTabButton->AddTab(wstrText);
	m_pTabButton->SelectTab(0);

	m_pDayButton->SetText(GetDisplayStringManager()->GetString(DST_A_DAY));
	m_pSummaryButton->SetText(GetDisplayStringManager()->GetString(DST_DOJO_ACCOUNT_ITEM));
	m_pAmountButton->SetText(GetDisplayStringManager()->GetString(DST_DOJO_ACCOUNT_AMOUNT));	

	m_pCreditStatic->SetText(GetDisplayStringManager()->GetString(DST_DOJO_ACCOUNT_BALANCE));
	m_pCredit->SetText("0");	


	// default value
	m_pDialogName->SetPosition(DBOGUI_DIALOG_TITLE_X, DBOGUI_DIALOG_TITLE_Y);

	// 데이터를 받기 전에 스크롤이 보여지는 것을 방지하기 위해	
	m_pScrollbar->SetMaxValue(0);
	m_pScrollbar->SetValue(0);


	// sig
	m_slotTabChanged				= m_pTabButton->SigSelectChanged().Connect( this, &CDojoAccountGui::OnTabChanged );
	m_slotHelpButton				= m_pHelpButton->SigClicked().Connect(this, &CDojoAccountGui::OnClicked_HelpButton);
	m_slotExitButton				= m_pExitButton->SigClicked().Connect(this, &CDojoAccountGui::OnClicked_ExitButton);
	m_slotDayButton					= m_pDayButton->SigClicked().Connect(this, &CDojoAccountGui::OnClicked_DayButton);
	m_slotSummaryButton				= m_pSummaryButton->SigClicked().Connect(this, &CDojoAccountGui::OnClicked_SummaryButton);
	m_slotAmountButton				= m_pAmountButton->SigClicked().Connect(this, &CDojoAccountGui::OnClicked_AmountButton);

	m_slotServerScrollChanged		= m_pScrollbar->SigValueChanged().Connect( this, &CDojoAccountGui::OnScrollChanged );
	m_slotServerScrollSliderMoved	= m_pScrollbar->SigSliderMoved().Connect( this, &CDojoAccountGui::OnScrollChanged );	
	m_slotMouseDown					= m_pThis->SigMouseDown().Connect( this, &CDojoAccountGui::OnMouseDown );
	m_slotMouseUp					= m_pThis->SigMouseUp().Connect( this, &CDojoAccountGui::OnMouseUp );	
	m_slotMove						= m_pThis->SigMove().Connect( this, &CDojoAccountGui::OnMove );	
	m_slotMouseMove					= m_pThis->SigMouseMove().Connect( this, &CDojoAccountGui::OnMouseMove );
	m_slotMouseLeave				= m_pThis->SigMouseLeave().Connect( this, &CDojoAccountGui::OnMouseLeave );
	m_slotPaint						= m_pThis->SigPaint().Connect( this, &CDojoAccountGui::OnPaint );		
	m_slotCaptureWheelMove			= GetNtlGuiManager()->GetGuiManager()->SigCaptureWheelMove().Connect( this, &CDojoAccountGui::OnCaptureWheelMove );
	m_slotCaptureMouseDown			= GetNtlGuiManager()->GetGuiManager()->SigCaptureMouseDown().Connect( this, &CDojoAccountGui::OnCaptureMouseDown );

	// OnMove를 통해 위치가 결정되는 컴포넌트를 위해서
	OnMove(100, 100);

	LinkMsg(g_EventNPCDialogOpen);
	LinkMsg(g_EventCharObjDelete);
	LinkMsg(g_EventDialog);
	LinkMsg(g_EventDojoNotify);
	
	Show(false);

	NTL_RETURN(TRUE);
}

VOID CDojoAccountGui::Destroy()
{
	NTL_FUNCTION("CDojoAccountGui::Destroy");

	ClearList();

	UnLinkMsg(g_EventNPCDialogOpen);
	UnLinkMsg(g_EventCharObjDelete);
	UnLinkMsg(g_EventDialog);
	UnLinkMsg(g_EventDojoNotify);	

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy(); 

	NTL_RETURNVOID();
}

VOID CDojoAccountGui::Clear()
{
	m_hNPCSerial		= INVALID_SERIAL_ID;
	m_bAscentDay		= FALSE;
	m_bAscentSummary	= FALSE;
	m_bAscentZenny		= FALSE;
	m_uiDojoTableIndex	= INVALID_TBLIDX;

	m_pCredit->Clear();

	ClearList();
}

VOID CDojoAccountGui::SetAccount(RwUInt32 uiBalance, RwUInt8 byCount, sDBO_DOJO_BANK_HISTORY_INFO* pDBO_DOJO_BANK_HISTORY_INFO)
{
	for( RwUInt8 i = 0 ; i < byCount ; ++i )
	{
		sACCOUNT_ITEM* pACCOUNT_ITEM = NTL_NEW sACCOUNT_ITEM;

		pACCOUNT_ITEM->byIndex		= byCount;
		pACCOUNT_ITEM->tTime		= pDBO_DOJO_BANK_HISTORY_INFO[i].sTime;
		pACCOUNT_ITEM->uiZenny		= pDBO_DOJO_BANK_HISTORY_INFO[i].dwAmount;
		pACCOUNT_ITEM->bSave		= pDBO_DOJO_BANK_HISTORY_INFO[i].bIsSave;

		switch( pDBO_DOJO_BANK_HISTORY_INFO[i].byType )
		{
		case DBO_GUILD_ZENNY_UPDATE_TYPE_LEADER_INCOME:
			{
				pACCOUNT_ITEM->wstrSummary = GetDisplayStringManager()->GetString(DST_DOJO_ACCOUNT_TYPE_LEADER_INCOME);
				break;
			}
		case DBO_GUILD_ZENNY_UPDATE_TYPE_LEADER_OUTCOME:
			{
				pACCOUNT_ITEM->wstrSummary = GetDisplayStringManager()->GetString(DST_DOJO_ACCOUNT_TYPE_LEADER_OUTCOME);
				break;
			}
		case DBO_GUILD_ZENNY_UPDATE_TYPE_VILLAGE_INCOME:
			{
				pACCOUNT_ITEM->wstrSummary = GetDisplayStringManager()->GetString(DST_DOJO_ACCOUNT_TYPE_VILLAGE_INCOME);
				break;
			}
		case DBO_GUILD_ZENNY_UPDATE_TYPE_BATTLE_OUTCOME:
			{
				pACCOUNT_ITEM->wstrSummary = GetDisplayStringManager()->GetString(DST_DOJO_ACCOUNT_TYPE_BATTLE_OUTCOME);
				break;
			}
		case DBO_GUILD_ZENNY_UPDATE_TYPE_BATTLE_INCOME:
			{
				pACCOUNT_ITEM->wstrSummary = GetDisplayStringManager()->GetString(DST_DOJO_ACCOUNT_TYPE_BATTLE_INCOME);
				break;
			}
		default:
			{
				DBO_FAIL("Invalid eDBO_GUILD_ZENNY_UPDATE_TYPE type : " << pDBO_DOJO_BANK_HISTORY_INFO[i].byType);
				break;
			}
		}

		m_listAccount.push_back(pACCOUNT_ITEM);
	}
	
	m_pCredit->SetText(uiBalance);
	RefreshList(0);
}

VOID CDojoAccountGui::RefreshList(RwInt32 iOffSet)
{
	for( RwUInt8 i = 0 ; i < dDISPLAY_ACCOUNT_NUM ; ++i )
	{
		m_aAccountItemGui[i].pDay		->Clear();
		m_aAccountItemGui[i].pSummary	->Clear();
		m_aAccountItemGui[i].pZenny		->Clear();
	}


	RwUInt32 uiCount		= 0;
	RwUInt32 uiListCount	= 0;
	LIST_ACCOUNT::iterator it = m_listAccount.begin();
	LIST_ACCOUNT::iterator it_end = m_listAccount.end();

	m_iVisibleStart = iOffSet;

	for( ; it != it_end ; ++it )
	{
		if( (RwUInt8)m_iVisibleStart <= uiCount && uiCount < (RwUInt32)m_iVisibleStart + dDISPLAY_ACCOUNT_NUM )
		{
			sACCOUNT_ITEM* pACCOUNT_ITEM = *it;

			WCHAR awcBuffer[256];
			swprintf_s(awcBuffer, 256, L"%d%s %d%s", pACCOUNT_ITEM->tTime.month,
													 GetDisplayStringManager()->GetString(DST_TIME_MONTH),
													 pACCOUNT_ITEM->tTime.day,
													 GetDisplayStringManager()->GetString(DST_TIME_DAY));

			m_aAccountItemGui[uiListCount].pDay		->SetText( awcBuffer );
			m_aAccountItemGui[uiListCount].pSummary	->SetText( pACCOUNT_ITEM->wstrSummary.c_str() );

			if( pACCOUNT_ITEM->bSave )
				m_aAccountItemGui[uiListCount].pZenny->SetTextColor(RGB(255,255,255));
			else
				m_aAccountItemGui[uiListCount].pZenny->SetTextColor(RGB(243,106,124));

			m_aAccountItemGui[uiListCount].pZenny->SetText( pACCOUNT_ITEM->uiZenny );


			++uiListCount;
		}

		++uiCount;
	}

	m_pScrollbar->SetMaxValue( m_listAccount.size() - dDISPLAY_ACCOUNT_NUM );
}

VOID CDojoAccountGui::ClearList()
{
	LIST_ACCOUNT::iterator it = m_listAccount.begin();
	for( ; it != m_listAccount.end() ; ++it )
	{
		sACCOUNT_ITEM* pACCOUNT_ITEM = *it;
		NTL_DELETE(pACCOUNT_ITEM);
	}
	m_listAccount.clear();
}

VOID CDojoAccountGui::OnClicked_HelpButton(gui::CComponent* pComponent)
{
	CDboEventGenerator::OpenHelpContent( DIALOG_DOJO_ACCOUNT );
}

VOID CDojoAccountGui::OnClicked_ExitButton(gui::CComponent* pComponent)
{
	GetDialogManager()->CloseDialog(DIALOG_DOJO_ACCOUNT);
}

VOID CDojoAccountGui::OnClicked_DayButton(gui::CComponent* pComponent)
{
	m_bAscentDay = !m_bAscentDay;

	if( m_bAscentDay )
		std::stable_sort(m_listAccount.begin(), m_listAccount.end(), CompareAscentDay);
	else
		std::stable_sort(m_listAccount.begin(), m_listAccount.end(), CompareDecentDay);

	RefreshList(0);
}

VOID CDojoAccountGui::OnClicked_SummaryButton(gui::CComponent* pComponent)
{
	m_bAscentSummary = !m_bAscentSummary;

	if( m_bAscentSummary )
		std::stable_sort(m_listAccount.begin(), m_listAccount.end(), CompareAscentSummary);
	else
		std::stable_sort(m_listAccount.begin(), m_listAccount.end(), CompareDecentSummary);

	RefreshList(0);
}

VOID CDojoAccountGui::OnClicked_AmountButton(gui::CComponent* pComponent)
{
	m_bAscentZenny = !m_bAscentZenny;

	if( m_bAscentZenny )
		std::stable_sort(m_listAccount.begin(), m_listAccount.end(), CompareAscentZenny);
	else
		std::stable_sort(m_listAccount.begin(), m_listAccount.end(), CompareDecentZenny);

	RefreshList(0);
}

VOID CDojoAccountGui::OnTabChanged(RwInt32 nIndex, RwInt32 nOldIndex)
{
}

VOID CDojoAccountGui::OnScrollChanged(RwInt32 iOffset)
{
	RefreshList(iOffset);
}

VOID CDojoAccountGui::OnMouseDown(const CKey& key)
{

}

VOID CDojoAccountGui::OnMouseUp(const CKey& key)
{

}

VOID CDojoAccountGui::OnMove(RwInt32 iOldX, RwInt32 iOldY)
{
	CRectangle rtScreen = m_pThis->GetScreenRect();

	m_srfCreditBackground	.SetPositionbyParent(rtScreen.left, rtScreen.top);
	m_srf3BackLine			.SetPositionbyParent(rtScreen.left, rtScreen.top);
	m_srf3Background		.SetPositionbyParent(rtScreen.left, rtScreen.top);
}

VOID CDojoAccountGui::OnMouseMove(RwInt32 nFlags, RwInt32 nX, RwInt32 nY)
{

}

VOID CDojoAccountGui::OnMouseLeave(gui::CComponent* pComponent)
{

}

VOID CDojoAccountGui::OnPaint()
{
	m_srfCreditBackground.Render();
	m_srf3BackLine.Render();
	m_srf3Background.Render();
}

VOID CDojoAccountGui::OnCaptureWheelMove(RwInt32 iFlag, RwInt16 sDelta, CPos& pos)
{
	if( !IsShow() )
		return;

	if( GetDialog()->GetParent()->GetChildComponentReverseAt( pos.x, pos.y ) != GetDialog() )
		return;

	if( GetDialog()->PosInRect( pos.x, pos.y ) != gui::CComponent::INRECT )
		return;

	RwInt32 iValue = m_pScrollbar->GetValue();
	RwInt32 iMaxValue = m_pScrollbar->GetMaxValue();

	if( sDelta < 0 )
		++iValue;
	else
		--iValue;

	if( iValue < 0 )
		iValue = 0;
	else if( iValue > iMaxValue )
		iValue = iMaxValue;

	m_pScrollbar->SetValue( iValue );

	RefreshList(iValue);
}

VOID CDojoAccountGui::OnCaptureMouseDown(const CKey& key)
{
	CAPTURE_MOUSEDOWN_RAISE(DIALOG_DOJO_ACCOUNT, key.m_fX, key.m_fY);
}

RwInt32 CDojoAccountGui::SwitchDialog(bool bOpen)
{
	if( bOpen )
	{
		CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
		if( !pAvatar )
		{
			DBO_FAIL("Not exist avatar instance");
			return -1;
		}

		CNtlGuild*	pGuild		= pAvatar->GetGuild();
		CNtlDojo*	pDojo		= pAvatar->GetDojo();
		sDOJO_INFO*	pDOJO_INFO	= pDojo->GetDojoInfo_fromGuildID( pGuild->GetGuildID() );

		if( !pDOJO_INFO )
		{
			DBO_FAIL("Invalid dojo info pointer");

			GetNtlWorldConcept()->RemoveWorldPlayConcept( WORLD_PLAY_NPC_COMMU );
			CNtlSLEventGenerator::CameraControlDelete( CAMERA_CONTROL_NPC );
			return -1;
		}

		m_uiDojoTableIndex = pDOJO_INFO->uiDojoTableIndex;
	}
	else
	{
		Clear();		

		GetNtlWorldConcept()->RemoveWorldPlayConcept( WORLD_PLAY_NPC_COMMU );
		CNtlSLEventGenerator::CameraControlDelete( CAMERA_CONTROL_NPC );
	}

	Show(bOpen);
	return 1;
}

VOID CDojoAccountGui::HandleEvents( RWS::CMsg &msg )
{
	NTL_FUNCTION("CDojoAccountGui::HandleEvents");

	if( msg.Id == g_EventNPCDialogOpen )
	{
		SDboNpcDialogOpen* pEvent = reinterpret_cast<SDboNpcDialogOpen*>( msg.pData );

		if( pEvent->eDialog != DIALOG_DOJO_ACCOUNT )
			return;

		// 같은 NPC
		if( m_hNPCSerial == pEvent->hSerialId )
			return;

		// 다른 NPC와 대화를 하는 경우이다
		if( m_hNPCSerial != INVALID_SERIAL_ID )
		{
			GetDialogManager()->CloseDialog(DIALOG_DOJO_ACCOUNT);
			return;
		}

		m_hNPCSerial = pEvent->hSerialId;

		if( FALSE == GetDialogManager()->OpenDialog(DIALOG_DOJO_ACCOUNT, m_hNPCSerial) )
		{
			m_hNPCSerial = INVALID_SERIAL_ID;

			GetNtlWorldConcept()->RemoveWorldPlayConcept( WORLD_PLAY_NPC_COMMU );
			CNtlSLEventGenerator::CameraControlDelete( CAMERA_CONTROL_NPC );
			return;
		}

		RwBool	bPacketLock		= FALSE;
		bool	bSendSuccess	= GetDboGlobal()->GetGamePacketGenerator()->SendDojo_BackHistoryReq(m_hNPCSerial, &bPacketLock);

		if( bPacketLock && !bSendSuccess )
		{
			GetDialogManager()->CloseDialog(DIALOG_DOJO_ACCOUNT);

			GetNtlWorldConcept()->RemoveWorldPlayConcept( WORLD_PLAY_NPC_COMMU );
			CNtlSLEventGenerator::CameraControlDelete( CAMERA_CONTROL_NPC );
			return;
		}
	}
	else if( msg.Id == g_EventCharObjDelete )
	{		
		RwUInt32* pDeleteSerial = reinterpret_cast<RwUInt32*>( msg.pData );

		if( m_hNPCSerial == *pDeleteSerial )
		{
			// NPC 서버가 다운되거나 하여 갑자기 NPC가 사라지는 경우
			GetDialogManager()->CloseDialog(DIALOG_DOJO_ACCOUNT);
		}
	}
	else if( msg.Id == g_EventDialog )
	{		
		SDboEventDialog* pEvent = (SDboEventDialog*)msg.pData;

		if( pEvent->iDestDialog == DIALOG_DOJO_ACCOUNT )
		{
			if( pEvent->iType == DIALOGEVENT_NPC_BYEBYE ||
				pEvent->iType == DIALOGEVENT_OPEN_FAIL_NPC_DIALOG )
			{
				GetDialogManager()->CloseDialog(DIALOG_DOJO_ACCOUNT);
			}
		}
	}
	else if( msg.Id == g_EventDojoNotify )
	{
		// GUI를 열 때마다 정보를 갱신하기에 닫혀있는 상태에서는 갱신하지 않는다
		if( FALSE == GetDialogManager()->IsOpenDialog(DIALOG_DOJO_ACCOUNT) )
			return;


		SNtlEventDojo* pEvent = reinterpret_cast<SNtlEventDojo*>( msg.pData );

		if( DOJO_EVENT_BANK_HISTORY == pEvent->byDojoEvent )
		{
			RwUInt8 byCount = *(RwUInt8*)pEvent->pExData;
			sDBO_DOJO_BANK_HISTORY_INFO* pDBO_DOJO_BANK_HISTORY_INFO = (sDBO_DOJO_BANK_HISTORY_INFO*)pEvent->pExData2;
			SetAccount(pEvent->uiParam, byCount, pDBO_DOJO_BANK_HISTORY_INFO);
		}
		else if( DOJO_EVENT_LOST_DOJO == pEvent->byDojoEvent )
		{
			if( pEvent->uiParam != m_uiDojoTableIndex )
				NTL_RETURNVOID();

			Clear();			
		}
	}

	NTL_RETURNVOID();
}