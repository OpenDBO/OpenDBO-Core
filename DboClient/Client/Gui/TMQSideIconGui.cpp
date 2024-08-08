#include "precomp_dboclient.h"
#include "TMQSideIconGui.h"


// core
#include "NtlDebug.h"

// share
#include "TextAllTable.h"
#include "ItemTable.h"
#include "TimeQuestTable.h"
#include "TableContainer.h"
#include "QuestTextDataTable.h"
#include "NtlTimeQuest.h"

// sound
#include "GUISoundDefine.h"

// presentation
#include "NtlPLGuiManager.h"

// simulation
#include "NtlSLEvent.h"
#include "NtlSLTMQ.h"
#include "NtlSobAvatar.h"
#include "NtlSLGlobal.h"
#include "NtlSLApi.h"
#include "NtlSLLogic.h"
#include "NtlSLEventFunc.h"

// dbo
#include "DboEvent.h"
#include "DboGlobal.h"
#include "DisplayStringManager.h"
#include "DboEventGenerator.h"
#include "InfoWndManager.h"
#include "TMQBoardGui.h"
#include "DialogManager.h"
#include "DboLogic.h"
#include "CounterGui.h"

namespace
{
	// 추첨전
#define dTSC_VOTING					10.f			///< 실제 추첨하기전 추첨을 알리는 시간

	// 추첨후
#define dTSC_RESULT_LOTTERY			5.f				///< 추첨 결과를 보여주는 시간
#define dTSC_ASK_RETRY				300.f			///< 재신청할지를 물어보는 시간

#define dTSC_TIME_DISABLE_TELEPORT	20.f			///< TMQ 진입직전 TMQ 텔레포트를 할 수 없는 시간

	// TMQ View
#define	dVIEWBOTTOM_GAP				20
#define dVIEWEDGE_GAP				10

#define dSTRING_HEIGHT_GAP			10
#define dMESSAGE_X					20

	// color
#define dREGISTER_COLOR			RGB(255, 192, 0)
#define dUNREGISTER_COLOR		RGB(255, 0, 0)

	enum eTMQViewMode
	{
		VM_NONE,
		VM_LOTTERY_WAITING,					///< 추첨 대기중
		VM_HONGING_A_LOTTERY,				///< 추첨중
		VM_SUCCESS_LOTTERY,					///< 추첨 성공
		VM_FAIL_LOTTERY,					///< 추첨 실패
		VM_RETRY_LOTTERY,					///< 추첨 재신청
		VM_REMAIN_TIME,						///< TMQ 진입까지 남은 시간
	};

	RwUInt8		byViewMode = VM_NONE;	
	RwReal		fTMQIconElapsed;
	sTIMEQUEST_TIME nextTimequest;			///< TMQ 추첨실패시 다음 TMQ 신청여부를 물을 때 쓸 다음 TMQ 출발시간
}


//////////////////////////////////////////////////////////////////////////
// CTMQSideIconGui
//////////////////////////////////////////////////////////////////////////

CTMQSideIconGui::CTMQSideIconGui(const RwChar* pName)
:CSideIconBase(pName)
,m_eIconImageType(IIT_NORMAL)
,m_pCounterGui(NULL)
{
}

CTMQSideIconGui::~CTMQSideIconGui()
{

}

RwBool CTMQSideIconGui::Create()
{
	NTL_FUNCTION( "CTMQSideIconGui::Create" );

	if(!CNtlPLGui::Create("", "gui\\TMQSideIcon.srf", "gui\\TMQSideIcon.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");

	// 일반 아이콘 버튼
	m_pIconButton = (gui::CButton*)GetComponent("btnIcon");
	m_slotIconButton = m_pIconButton->SigClicked().Connect(this, &CTMQSideIconGui::OnIconButtonClicked);

	// 이벤트 아이콘 버튼
	m_pEventIconButton = (gui::CButton*)GetComponent("btnEventIcon");
	m_slotEventIconButton = m_pEventIconButton->SigClicked().Connect(this, &CTMQSideIconGui::OnIconButtonClicked);

	// sig	
	m_slotMove			= m_pThis->SigMove().Connect( this, &CTMQSideIconGui::OnMove );	
	m_slotPaint			= m_pThis->SigPaint().Connect( this, &CTMQSideIconGui::OnPaint );

	SetIconImageType(IIT_EVENT);
	Show(false);

	LinkMsg(g_EventTMQBoardNotify);

	NTL_RETURN(TRUE);
}

VOID CTMQSideIconGui::Destroy()
{
	NTL_FUNCTION("CTMQSideIconGui::Destroy");

	GetNtlGuiManager()->RemoveUpdateFunc( this );

	UnLinkMsg(g_EventTMQBoardNotify);

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy(); 

	NTL_RETURNVOID();
}

VOID CTMQSideIconGui::Clear()
{
	Show(false);
	byViewMode = VM_NONE;
	GetNtlGuiManager()->RemoveUpdateFunc( this );

	if( m_pCounterGui )
	{
		m_pCounterGui->Destroy();
		NTL_DELETE(m_pCounterGui);
	}
}

VOID  CTMQSideIconGui::Update(RwReal fElapsed)
{
	RwBool bOpenView = FALSE;

	if( byViewMode == VM_LOTTERY_WAITING )
	{
		CNtlTMQ* pTMQ = GetNtlSLGlobal()->GetSobAvatar()->GetTMQ();
		RwReal fViewElapsed = pTMQ->GetRemainTime_in_Room();

		if( fViewElapsed <= dTSC_VOTING )
		{
			byViewMode = VM_HONGING_A_LOTTERY;
			bOpenView = TRUE;
		}
	}
	else if( byViewMode == VM_SUCCESS_LOTTERY )
	{
		fTMQIconElapsed += fElapsed;

		if( fTMQIconElapsed >= dTSC_RESULT_LOTTERY )
		{
			byViewMode = VM_REMAIN_TIME;
			bOpenView = TRUE;
		}
	}
	else if( byViewMode == VM_FAIL_LOTTERY )
	{
		fTMQIconElapsed += fElapsed;

		if( fTMQIconElapsed >= dTSC_RESULT_LOTTERY )
		{
			byViewMode = VM_RETRY_LOTTERY;
			bOpenView = TRUE;
		}
	}
	else if( byViewMode == VM_RETRY_LOTTERY )
	{
		fTMQIconElapsed += fElapsed;

		if( fTMQIconElapsed >= dTSC_ASK_RETRY )
		{
			Show(false);
			return;
		}
	}
	else if( byViewMode == VM_REMAIN_TIME )
	{
		if( !m_pCounterGui )
		{
			CNtlTMQ* pTMQ = GetNtlSLGlobal()->GetSobAvatar()->GetTMQ();
			RwReal fViewElapsed = pTMQ->GetRemainTime_in_Room();

			m_pCounterGui = NTL_NEW CCounterGui("TMQ Counter");
			if( !m_pCounterGui->Create(COUNTER_DISPLAY_1, fViewElapsed, COUNTER_QUADRANT_4) )
			{
				m_pCounterGui->Destroy();
				NTL_DELETE(m_pCounterGui);
				return;
			}

			m_pCounterGui->AlarmSound(TRUE, 10.f);
			m_pCounterGui->SetDisplayFlag(dCOUNTER_DISPLAY_FLAG_BIGGER);			
			
			OnMove(0, 0);
		}

		if( m_pCounterGui )
		{
			if( CSideIconGui::GetInstance()->GetPresentView() == INVALID_SIDEVIEW )
				m_pCounterGui->Show(true);
			else
				m_pCounterGui->Show(false);
		}
	}


	if( bOpenView )
	{
		fTMQIconElapsed = 0.f;
		SetIconImageType(IIT_EVENT);
		CSideIconGui::GetInstance()->OpenSideView(this, SIDEVIEW_TMQ, NULL);
	}
}

void CTMQSideIconGui::OnSideViewClosed()
{
}

void CTMQSideIconGui::OnIconButtonClicked(gui::CComponent* pComponent)
{	
	if( byViewMode == VM_LOTTERY_WAITING )
	{
		// 해당 상태에서 View 창을 열 때마다 정보를 요청한다
		GetDboGlobal()->GetGamePacketGenerator()->SendTMQ_Room_Info_Req();
	}

	SetIconImageType(IIT_NORMAL);
	CSideIconGui::GetInstance()->OpenSideView(this, SIDEVIEW_TMQ, NULL);
}

VOID CTMQSideIconGui::SetIconImageType(eIconImageType eType)
{
	m_eIconImageType = eType;

	switch(eType)
	{
	case IIT_NORMAL:	m_pIconButton->Show(true);	m_pEventIconButton->Show(false);	break;
	case IIT_EVENT:		m_pIconButton->Show(false);	m_pEventIconButton->Show(true);		break;
	default:			DBO_FAIL("rong type : " << eType);
	}
}

VOID CTMQSideIconGui::OnMove(RwInt32 iOldX, RwInt32 iOldY)
{
	CRectangle rtScreen = m_pThis->GetScreenRect();

	if( m_pCounterGui )
	{
		m_pCounterGui->SetPosition(rtScreen.left - 45, rtScreen.top - 70);
	}
}

VOID CTMQSideIconGui::OnPaint()
{
}

VOID CTMQSideIconGui::HandleEvents( RWS::CMsg &msg )
{
	NTL_FUNCTION("CTMQSideIconGui::HandleEvents");

	if( msg.Id == g_EventTMQBoardNotify )
	{
		SNtlEventTMQBoardNotify* pEvent = reinterpret_cast<SNtlEventTMQBoardNotify*>( msg.pData );

		switch(pEvent->uiEventType)
		{
		case TBT_ROOM_RESERVATION:
		case TBT_ROOM_RESERVATION_NFY:
		case TBT_ROOM_SUCCESS_SELECTION:
		case TBT_ROOM_ASK_RETRY:
			{
				if( pEvent->uiEventType == TBT_ROOM_RESERVATION ||
					pEvent->uiEventType == TBT_ROOM_RESERVATION_NFY )
				{
					GetNtlGuiManager()->AddUpdateFunc( this );
				}

				SetIconImageType(IIT_EVENT);
				fTMQIconElapsed = 0.f;
				Show(true);
				CSideIconGui::GetInstance()->OpenSideView(this, SIDEVIEW_TMQ, NULL);

				Logic_PlayGUISound(GSD_SYSTEM_WINDOW_ALREAT);
				break;
			}
		case TBT_ROOM_LEAVE:
		case TBT_ROOM_LAUNCHED_TM:
		case TBT_ROOM_SERVER_EXCEPTION:
			{				
				Clear();				
				break;
			}
		}
	}

	NTL_RETURNVOID();
}


//////////////////////////////////////////////////////////////////////////
// CTMQSideViewGui
//////////////////////////////////////////////////////////////////////////

CTMQSideViewGui::CTMQSideViewGui(const RwChar* pName)
:CSideViewBase(pName)
{
}

CTMQSideViewGui::~CTMQSideViewGui()
{

}

RwBool CTMQSideViewGui::Create()
{
	NTL_FUNCTION( "CTMQSideViewGui::Create" );

	if(!CNtlPLGui::Create("", "gui\\TMQSideView.srf", "gui\\TMQSideView.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	CRectangle rect;

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");

	// 닫기 버튼
	m_pExitButton = (gui::CButton*)GetComponent("btnExit");
	m_slotExitButton = m_pExitButton->SigClicked().Connect(this, &CTMQSideViewGui::OnExitClicked);

	// 배경
	m_BackPanel.SetType(CWindowby3::WT_HORIZONTAL);
	m_BackPanel.SetSurface(0, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("TMQSideView.srf", "srfBackUp"));
	m_BackPanel.SetSurface(1, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("TMQSideView.srf", "srfBackCenter"));
	m_BackPanel.SetSurface(2, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("TMQSideView.srf", "srfBackDown"));

	// 알림 주제
	rect.SetRectWH(dMESSAGE_X, dVIEWEDGE_GAP, 250, 14);
	m_pNotifyTitle = NTL_NEW gui::CStaticBox(rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_LEFT, TRUE );
	m_pNotifyTitle->CreateFontStd( DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pNotifyTitle->SetTextColor(RGB(0, 255, 255));
	m_pNotifyTitle->Enable(false);

	// 메세지 1
	rect.SetRectWH(dMESSAGE_X, 10, 240, 50);
	m_pMessage_1 = NTL_NEW gui::CStaticBox(rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_LEFT, TRUE );
	m_pMessage_1->CreateFontStd( DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pMessage_1->Enable(false);

	// 메세지 1 내용
	rect.SetRectWH(dMESSAGE_X, 10, 240, 14);
	m_pMessage_1_Content = NTL_NEW gui::CStaticBox(rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_LEFT, TRUE );
	m_pMessage_1_Content->CreateFontStd( DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pMessage_1_Content->SetTextColor(RGB(255, 192, 0));
	m_pMessage_1_Content->Enable(false);

	// 메세지 2
	rect.SetRectWH(dMESSAGE_X, 10, 240, 14);
	m_pMessage_2 = NTL_NEW gui::CStaticBox(rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_LEFT, TRUE );
	m_pMessage_2->CreateFontStd( DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pMessage_2->Enable(false);

	// 메세지 2 내용
	rect.SetRectWH(dMESSAGE_X, 10, 240, 14);
	m_pMessage_2_Content = NTL_NEW gui::CStaticBox(rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_LEFT, TRUE );
	m_pMessage_2_Content->CreateFontStd( DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pMessage_2_Content->SetTextColor(RGB(255, 192, 0));
	m_pMessage_2_Content->Enable(false);

	// 메세지 3
	rect.SetRectWH(dMESSAGE_X, 10, 240, 14);
	m_pMessage_3 = NTL_NEW gui::CStaticBox(rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_LEFT, TRUE );
	m_pMessage_3->CreateFontStd( DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pMessage_3->Enable(false);

	// 메세지 3 내용
	rect.SetRectWH(dMESSAGE_X, 10, 240, 14);
	m_pMessage_3_Content = NTL_NEW gui::CStaticBox(rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_LEFT, TRUE );
	m_pMessage_3_Content->CreateFontStd( DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pMessage_3_Content->SetTextColor(RGB(255, 192, 0));
	m_pMessage_3_Content->Enable(false);

	// 메세지 4
	rect.SetRectWH(dMESSAGE_X, 10, 240, 14);
	m_pMessage_4 = NTL_NEW gui::CStaticBox(rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_LEFT, TRUE );
	m_pMessage_4->CreateFontStd( DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pMessage_4->Enable(false);

	// 메세지 4 내용
	rect.SetRectWH(dMESSAGE_X, 10, 240, 14);
	m_pMessage_4_Content = NTL_NEW gui::CStaticBox(rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_LEFT, TRUE );
	m_pMessage_4_Content->CreateFontStd( DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pMessage_4_Content->SetTextColor(RGB(255, 192, 0));
	m_pMessage_4_Content->Enable(false);

	// Need Item
	m_NeedItem.Create(m_pThis, DIALOG_SIDEICON, REGULAR_SLOT_ITEM_TABLE);


	// 확인 버튼
	m_pOKButton = (gui::CButton*)GetComponent("btnOK");
	m_pOKButton->SetTextUpColor(RGB(0, 255, 255));
	m_slotOKButton = m_pOKButton->SigClicked().Connect(this, &CTMQSideViewGui::OnOKClicked);

	// 취소 버튼
	m_pCancelButton = (gui::CButton*)GetComponent("btnCancel");
	m_pCancelButton->SetTextUpColor(RGB(0, 255, 255));
	m_slotCancelButton = m_pCancelButton->SigClicked().Connect(this, &CTMQSideViewGui::OnCancelClicked);

	// sig	
	m_slotMove			= m_pThis->SigMove().Connect( this, &CTMQSideViewGui::OnMove );	
	m_slotMouseMove		= m_pThis->SigMouseMove().Connect( this, &CTMQSideViewGui::OnMouseMove );
	m_slotMouseLeave	= m_pThis->SigMouseLeave().Connect( this, &CTMQSideViewGui::OnMouseLeave );
	m_slotPaint			= m_pThis->SigPaint().Connect( this, &CTMQSideViewGui::OnPaint );

	Show(false);

	LinkMsg(g_EventTMQBoardNotify);
	LinkMsg(g_EventPartyUpdateValue);
	LinkMsg(g_EventSobInfoUpdate);

	NTL_RETURN(TRUE);
}

VOID CTMQSideViewGui::Destroy()
{
	NTL_FUNCTION("CTMQSideViewGui::Destroy");

	CheckInfoWindow();
	m_NeedItem.Destroy();

	UnLinkMsg(g_EventTMQBoardNotify);
	UnLinkMsg(g_EventPartyUpdateValue);
	UnLinkMsg(g_EventSobInfoUpdate);

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy(); 

	NTL_RETURNVOID();
}

void CTMQSideViewGui::OnPressESC()
{

}

void CTMQSideViewGui::OnSideViewOpen(const void* pData)
{
	Show(true);	
	SetViewContent(byViewMode);
}

void CTMQSideViewGui::SetViewContent(RwUInt8 byMode)
{
	CRectangle rect;

	byViewMode = byMode;

	switch( byViewMode )
	{
	case VM_LOTTERY_WAITING:
		{
			CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
			if( !pAvatar )
			{
				DBO_FAIL("Not exist avatar instance");
				NTL_RETURNVOID();
			}

			CNtlTMQ*			pTMQ				= pAvatar->GetTMQ();
			sTIMEQUEST_TBLDAT*	pTIMEQUEST_TBLDAT	= pTMQ->GetTMQTable_for_RegRoom();
			if( !pTIMEQUEST_TBLDAT )
			{
				DBO_FAIL("VM_LOTTERY_WAITING, Not exist TMQ table");
				NTL_RETURNVOID();
			}

			eTIMEQUEST_DIFFICULTY eDuffucultType = pTMQ->GetDifficult_for_RegRoom();
			if( eDuffucultType >= MAX_TIMEQUEST_DIFFICULTY )
			{
				DBO_FAIL("VM_LOTTERY_WAITING, Invalid TMQ dificult type");
				NTL_RETURNVOID();
			}
			
			sTIMEQUEST_DATASET* pTIMEQUEST_DATASET = &pTIMEQUEST_TBLDAT->sTimeQuestDataset[eDuffucultType];
			if( !pTIMEQUEST_DATASET )
			{
				DBO_FAIL("VM_LOTTERY_WAITING, Not exist TMQ data set of TMQ table index : " << pTIMEQUEST_TBLDAT->tblidx);
				NTL_RETURNVOID();
			}

			// 타임머신 퀘스트 추첨 대기중
			m_pNotifyTitle->SetText(GetDisplayStringManager()->GetString(DST_TMQ_BALLOON_LOTTERY_WAITING));
			rect = m_pNotifyTitle->GetPosition();

			// 제   목 : 
			m_pMessage_1->Format(L"%s : ", GetDisplayStringManager()->GetString(DST_TMQ_BALLOON_TITLE));
			m_pMessage_1->SetPosition(dMESSAGE_X, rect.bottom + dSTRING_HEIGHT_GAP);
			rect = m_pMessage_1->GetPosition();

			CQuestTextDataTable* pQuestTextTable = API_GetTableContainer()->GetQuestTextDataTable();
			sQUEST_TEXT_DATA_TBLDAT* pQUEST_TEXT_DATA_TBLDAT = reinterpret_cast<sQUEST_TEXT_DATA_TBLDAT*>( pQuestTextTable->FindData( pTIMEQUEST_DATASET->nameTblidx ) );
			if( pQUEST_TEXT_DATA_TBLDAT )
			{
				std::wstring& wstrText = gui::GetHtmlFromMemoryString(pQUEST_TEXT_DATA_TBLDAT->wstrText.c_str(), pQUEST_TEXT_DATA_TBLDAT->wstrText.size());
				m_pMessage_1_Content->SetText( wstrText.c_str() );
				m_pMessage_1_Content->SetPosition(rect.right, rect.top);
			}
			else
			{
				DBO_FAIL("VM_LOTTERY_WAITING, Not exist QUEST TEXT DATA TABLE of index : " << pTIMEQUEST_DATASET->nameTblidx);
			}
			

			// 현재 신청한 파티/개인 : 
			m_pMessage_2->SetText(GetDisplayStringManager()->GetString(DST_TMQ_BALLOON_CURRENT_PEOPLE));
			m_pMessage_2->SetPosition(dMESSAGE_X, rect.bottom + dSTRING_HEIGHT_GAP);
			rect = m_pMessage_2->GetPosition();

			switch( pTMQ->GetWaitMode() )
			{
			case TIMEQUEST_MODE_INDIVIDUAL:
				{
					// 개인 등록
					m_pMessage_2_Content->SetText(GetDisplayStringManager()->GetString(DST_TMQ_BALLOON_PRIVATE_REG));
					break;
				}
			case TIMEQUEST_MODE_PARTY:
				{
					// 파티 등록
					m_pMessage_2_Content->SetText(GetDisplayStringManager()->GetString(DST_TMQ_BALLOON_PARTY_REG));
					break;
				}
			}				
			m_pMessage_2_Content->SetPosition(rect.right, rect.top);

			// 추첨까지 남은 시간 : 
			m_pMessage_3->SetText(GetDisplayStringManager()->GetString(DST_TMQ_BALLOON_TIME_BY_LOTTERY));
			m_pMessage_3->SetPosition(dMESSAGE_X, rect.bottom + dSTRING_HEIGHT_GAP);
			rect = m_pMessage_3->GetPosition();

			m_pMessage_3_Content->SetPosition(rect.right, rect.top);		

			// 예약취소
			m_pCancelButton->SetText(GetDisplayStringManager()->GetString(DST_TMQ_BALLOON_CENCLE_RESERVATION));

			m_iViewHeight = rect.bottom + dSTRING_HEIGHT_GAP + m_pCancelButton->GetHeight() + dVIEWBOTTOM_GAP;


			m_pMessage_1->Show(true);
			m_pMessage_2->Show(true);
			m_pMessage_3->Show(true);
			m_pMessage_4->Show(false);
			m_pMessage_1_Content->Show(true);
			m_pMessage_2_Content->Show(true);
			m_pMessage_3_Content->Show(true);
			m_pMessage_4_Content->Show(false);			

			m_pOKButton->Show(false);

			if( pTMQ->GetWaitMode() == TIMEQUEST_MODE_PARTY )
			{
				if( Logic_I_am_PartyLeader() )
					m_pCancelButton->Show(true);
				else
					m_pCancelButton->Show(false);
			}
			else
				m_pCancelButton->Show(true);

			break;
		}
	case VM_HONGING_A_LOTTERY:
		{
			CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
			if( !pAvatar )
			{
				DBO_FAIL("Not exist avatar instance");
				NTL_RETURNVOID();
			}

			CNtlTMQ*				pTMQ				= pAvatar->GetTMQ();
			CQuestTextDataTable*	pQuestTextTable		= API_GetTableContainer()->GetQuestTextDataTable();
			sTIMEQUEST_TBLDAT*		pTIMEQUEST_TBLDAT	= pTMQ->GetTMQTable_for_RegRoom();
			if( !pTIMEQUEST_TBLDAT )
			{
				DBO_FAIL("VM_HONGING_A_LOTTERY, Not exist TMQ table");
				NTL_RETURNVOID();
			}

			eTIMEQUEST_DIFFICULTY eDuffucultType = pTMQ->GetDifficult_for_RegRoom();
			if( eDuffucultType >= MAX_TIMEQUEST_DIFFICULTY )
			{
				DBO_FAIL("VM_HONGING_A_LOTTERY, Invalid TMQ dificult type");
				NTL_RETURNVOID();
			}

			sTIMEQUEST_DATASET* pTIMEQUEST_DATASET = &pTIMEQUEST_TBLDAT->sTimeQuestDataset[eDuffucultType];
			if( !pTIMEQUEST_DATASET )
			{
				DBO_FAIL("VM_HONGING_A_LOTTERY, Not exist TMQ data set of TMQ table index : " << pTIMEQUEST_TBLDAT->tblidx);
				NTL_RETURNVOID();
			}

			sQUEST_TEXT_DATA_TBLDAT* pQUEST_TEXT_DATA_TBLDAT = reinterpret_cast<sQUEST_TEXT_DATA_TBLDAT*>( pQuestTextTable->FindData( pTIMEQUEST_DATASET->nameTblidx ) );
			if( NULL == pQUEST_TEXT_DATA_TBLDAT )
			{
				DBO_FAIL("VM_HONGING_A_LOTTERY, Not exist QUEST TEXT DATA TABLE of index : " << pTIMEQUEST_DATASET->nameTblidx);
			}			

			// 타임머신 퀘스트 추첨중입니다
			m_pNotifyTitle->SetText(GetDisplayStringManager()->GetString(DST_TMQ_BALLOON_HOLDING_A_LOTTERY));
			rect = m_pNotifyTitle->GetPosition();

			// 제   목 : 
			m_pMessage_1->Format(L"%s : ", GetDisplayStringManager()->GetString(DST_TMQ_BALLOON_TITLE));
			m_pMessage_1->SetPosition(dMESSAGE_X, rect.bottom + dSTRING_HEIGHT_GAP);
			rect = m_pMessage_1->GetPosition();

			if( pQUEST_TEXT_DATA_TBLDAT )
			{
				std::wstring& wstrText = gui::GetHtmlFromMemoryString(pQUEST_TEXT_DATA_TBLDAT->wstrText.c_str(), pQUEST_TEXT_DATA_TBLDAT->wstrText.size());
				m_pMessage_1_Content->SetText( wstrText.c_str() );
			}

			m_pMessage_1_Content->SetPosition(rect.right, rect.top);

			// 추첨까지 남은 시간 : 
			m_pMessage_2->SetText(GetDisplayStringManager()->GetString(DST_TMQ_BALLOON_TIME_BY_LOTTERY));
			m_pMessage_2->SetPosition(dMESSAGE_X, rect.bottom + dSTRING_HEIGHT_GAP);
			rect = m_pMessage_2->GetPosition();

			m_pMessage_2_Content->SetPosition(rect.right, rect.top);		

			// 잠시만 기다려 주십시요
			m_pMessage_3->SetText(GetDisplayStringManager()->GetString(DST_TMQ_BALLOON_PLEASE_WAIT));
			m_pMessage_3->SetPosition(dMESSAGE_X, rect.bottom + dSTRING_HEIGHT_GAP);
			rect = m_pMessage_3->GetPosition();

			m_iViewHeight = rect.bottom + dVIEWBOTTOM_GAP;


			m_pMessage_1->Show(true);
			m_pMessage_2->Show(true);
			m_pMessage_3->Show(true);
			m_pMessage_4->Show(false);
			m_pMessage_1_Content->Show(true);
			m_pMessage_2_Content->Show(true);
			m_pMessage_3_Content->Show(false);
			m_pMessage_4_Content->Show(false);			

			m_pOKButton->Show(false);
			m_pCancelButton->Show(false);

			break;
		}
	case VM_SUCCESS_LOTTERY:
	case VM_FAIL_LOTTERY:
		break;
	case VM_RETRY_LOTTERY :
		{
			CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
			if( !pAvatar )
			{
				DBO_FAIL("Not exist avatar instance");
				NTL_RETURNVOID();
			}

			CNtlTMQ*				pTMQ				= pAvatar->GetTMQ();
			CQuestTextDataTable*	pQuestTextTable		= API_GetTableContainer()->GetQuestTextDataTable();
			sTIMEQUEST_TBLDAT*		pTIMEQUEST_TBLDAT	= pTMQ->GetTMQTable_for_RegRoom();
			if( !pTIMEQUEST_TBLDAT )
			{
				DBO_FAIL("VM_RETRY_LOTTERY, Not exist TMQ table");
				NTL_RETURNVOID();
			}

			eTIMEQUEST_DIFFICULTY eDuffucultType = pTMQ->GetDifficult_for_RegRoom();
			if( eDuffucultType >= MAX_TIMEQUEST_DIFFICULTY )
			{
				DBO_FAIL("VM_RETRY_LOTTERY, Invalid TMQ dificult type");
				NTL_RETURNVOID();
			}

			sTIMEQUEST_DATASET* pTIMEQUEST_DATASET = &pTIMEQUEST_TBLDAT->sTimeQuestDataset[eDuffucultType];
			if( !pTIMEQUEST_DATASET )
			{
				DBO_FAIL("VM_RETRY_LOTTERY, Not exist TMQ data set of TMQ table index : " << pTIMEQUEST_TBLDAT->tblidx);
				NTL_RETURNVOID();
			}

			sQUEST_TEXT_DATA_TBLDAT* pQUEST_TEXT_DATA_TBLDAT = reinterpret_cast<sQUEST_TEXT_DATA_TBLDAT*>( pQuestTextTable->FindData( pTIMEQUEST_DATASET->nameTblidx ) );
			if( NULL == pQUEST_TEXT_DATA_TBLDAT )
			{
				DBO_FAIL("VM_RETRY_LOTTERY, Not exist QUEST TEXT DATA TABLE of index : " << pTIMEQUEST_DATASET->nameTblidx);
			}
			

			// 타임머신 퀘스트 재신청
			m_pNotifyTitle->SetText(GetDisplayStringManager()->GetString(DST_TMQ_BALLOON_RETRY_LOTTERY));
			rect = m_pNotifyTitle->GetPosition();

			// 다음 타임머신 퀘스트를 예약하시겠습니까?
			m_pMessage_1->SetText(GetDisplayStringManager()->GetString(DST_TMQ_BALLOON_ASK_RETRY_LOTTERY));
			m_pMessage_1->SetPosition(dMESSAGE_X, rect.bottom + dSTRING_HEIGHT_GAP);
			rect = m_pMessage_1->GetPosition();

			// 제   목 : 
			m_pMessage_2->Format(L"%s : ", GetDisplayStringManager()->GetString(DST_TMQ_BALLOON_TITLE));
			m_pMessage_2->SetPosition(dMESSAGE_X, rect.bottom + dSTRING_HEIGHT_GAP);
			rect = m_pMessage_2->GetPosition();

			if( pQUEST_TEXT_DATA_TBLDAT )
			{
				std::wstring& wstrText = gui::GetHtmlFromMemoryString(pQUEST_TEXT_DATA_TBLDAT->wstrText.c_str(), pQUEST_TEXT_DATA_TBLDAT->wstrText.size());
				m_pMessage_2_Content->SetText( wstrText.c_str() );
			}
			
			m_pMessage_2_Content->SetPosition(rect.right, rect.top);

			// 출발시간 : 
			m_pMessage_3->Format(L"%s : ", GetDisplayStringManager()->GetString(DST_TMQ_BALLOON_START_TIME));
			m_pMessage_3->SetPosition(dMESSAGE_X, rect.bottom + dSTRING_HEIGHT_GAP);
			rect = m_pMessage_3->GetPosition();

			m_pMessage_3_Content->Format(L"%02d:%02d", nextTimequest.byHour, nextTimequest.byMinute);
			m_pMessage_3_Content->SetPosition(rect.right, rect.top);

			// 재신청까지 남은 시간 : 
			m_pMessage_4->SetText(GetDisplayStringManager()->GetString(DST_TMQ_BALLOON_REMAIN_TIME_BY_RETRY));
			m_pMessage_4->SetPosition(dMESSAGE_X, rect.bottom + dSTRING_HEIGHT_GAP);
			rect = m_pMessage_4->GetPosition();

			m_pMessage_4_Content->SetPosition(rect.right, rect.top);

			// 재신청, 취소
			m_pOKButton->SetText(GetDisplayStringManager()->GetString(DST_TMQ_BALLOON_RETRY));
			m_pCancelButton->SetText(GetDisplayStringManager()->GetString(DST_MSG_BTN_CANCEL));

			m_iViewHeight = rect.bottom + dSTRING_HEIGHT_GAP + m_pCancelButton->GetHeight() + dVIEWBOTTOM_GAP;


			m_pMessage_1->Show(true);
			m_pMessage_2->Show(true);
			m_pMessage_3->Show(true);
			m_pMessage_4->Show(false);
			m_pMessage_1_Content->Show(false);
			m_pMessage_2_Content->Show(true);
			m_pMessage_3_Content->Show(true);
			m_pMessage_4_Content->Show(false);			

			if( pTMQ->GetWaitMode() == TIMEQUEST_MODE_PARTY )
			{
				if( Logic_I_am_PartyLeader() )
				{
					m_pOKButton->Show(true);
					m_pCancelButton->Show(true);
				}
				else
				{
					m_pOKButton->Show(false);
					m_pCancelButton->Show(false);
				}
			}
			else
			{
				m_pOKButton->Show(true);
				m_pCancelButton->Show(true);
			}				

			break;
		}
	case VM_REMAIN_TIME:
		{
			CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
			if( !pAvatar )
			{
				DBO_FAIL("Not exist avatar instance");
				NTL_RETURNVOID();
			}

			CNtlTMQ*				pTMQ				= pAvatar->GetTMQ();
			CQuestTextDataTable*	pQuestTextTable		= API_GetTableContainer()->GetQuestTextDataTable();
			sTIMEQUEST_TBLDAT*		pTIMEQUEST_TBLDAT	= pTMQ->GetTMQTable_for_RegRoom();
			if( !pTIMEQUEST_TBLDAT )
			{
				DBO_FAIL("VM_REMAIN_TIME, Not exist TMQ table");
				NTL_RETURNVOID();
			}

			eTIMEQUEST_DIFFICULTY eDuffucultType = pTMQ->GetDifficult_for_RegRoom();
			if( eDuffucultType >= MAX_TIMEQUEST_DIFFICULTY )
			{
				DBO_FAIL("VM_REMAIN_TIME, Invalid TMQ dificult type");
				NTL_RETURNVOID();
			}

			sTIMEQUEST_DATASET* pTIMEQUEST_DATASET = &pTIMEQUEST_TBLDAT->sTimeQuestDataset[eDuffucultType];
			if( !pTIMEQUEST_DATASET )
			{
				DBO_FAIL("VM_REMAIN_TIME, Not exist TMQ data set of TMQ table index : " << pTIMEQUEST_TBLDAT->tblidx);
				NTL_RETURNVOID();
			}

			sQUEST_TEXT_DATA_TBLDAT* pQUEST_TEXT_DATA_TBLDAT = reinterpret_cast<sQUEST_TEXT_DATA_TBLDAT*>( pQuestTextTable->FindData( pTIMEQUEST_DATASET->nameTblidx ) );
			if( NULL == pQUEST_TEXT_DATA_TBLDAT )
			{
				DBO_FAIL("VM_REMAIN_TIME, Not exist QUEST TEXT DATA TABLE of index : " << pTIMEQUEST_DATASET->nameTblidx);
			}

			CRectangle rtScreen = m_pThis->GetScreenRect();
			

			sITEM_TBLDAT* pITEM_TBLDAT;
			CTextTable* pItemTextAllTable = API_GetTableContainer()->GetTextAllTable()->GetItemTbl();
			WCHAR awcBuffer[256] = L"";

			// 타임머신 퀘스트 준비를 해주십시요
			m_pNotifyTitle->SetText(GetDisplayStringManager()->GetString(DST_TMQ_BALLOON_READY_FOR_TMQ));
			rect = m_pNotifyTitle->GetPosition();

			// 제   목 : 
			m_pMessage_1->Format(L"%s : ", GetDisplayStringManager()->GetString(DST_TMQ_BALLOON_TITLE));
			m_pMessage_1->SetPosition(dMESSAGE_X, rect.bottom + dSTRING_HEIGHT_GAP);
			rect = m_pMessage_1->GetPosition();

			if( pQUEST_TEXT_DATA_TBLDAT )
			{
				std::wstring& wstrText = gui::GetHtmlFromMemoryString(pQUEST_TEXT_DATA_TBLDAT->wstrText.c_str(), pQUEST_TEXT_DATA_TBLDAT->wstrText.size());
				m_pMessage_1_Content->SetText( wstrText.c_str() );
			}

			m_pMessage_1_Content->SetPosition(rect.right, rect.top);

			// 등록상태 : 
			m_pMessage_2->SetText(GetDisplayStringManager()->GetString(DST_TMQ_BALLOON_REG_STATUS));
			m_pMessage_2->SetPosition(dMESSAGE_X, rect.bottom + dSTRING_HEIGHT_GAP);
			rect = m_pMessage_2->GetPosition();

			switch( pTMQ->GetWaitMode() )
			{
			case TIMEQUEST_MODE_INDIVIDUAL:
				{
					// 개인 등록
					m_pMessage_2_Content->SetText(GetDisplayStringManager()->GetString(DST_TMQ_BALLOON_PRIVATE_REG));
					break;
				}
			case TIMEQUEST_MODE_PARTY:
				{
					// 파티 등록
					m_pMessage_2_Content->SetText(GetDisplayStringManager()->GetString(DST_TMQ_BALLOON_PARTY_REG));
					break;
				}
			}		
			m_pMessage_2_Content->SetPosition(rect.right, rect.top);

			// 남은시간 : 
			m_pMessage_3->SetText(GetDisplayStringManager()->GetString(DST_TMQ_BALLOON_REMAIN_TIME));
			m_pMessage_3->SetPosition(dMESSAGE_X, rect.bottom + dSTRING_HEIGHT_GAP);
			rect = m_pMessage_3->GetPosition();

			m_pMessage_3_Content->SetPosition(rect.right, rect.top);

			SetNeedItemState();

			// 필요아이템 : 
			m_pMessage_4->SetText(GetDisplayStringManager()->GetString(DST_TMQ_BALLOON_NEED_ITEM));
			m_pMessage_4->SetPosition(dMESSAGE_X, rect.bottom + dSTRING_HEIGHT_GAP);
			rect = m_pMessage_4->GetPosition();

			if( pTIMEQUEST_DATASET->needItemTblidx != INVALID_SERIAL_ID )
			{
				std::wstring wstrText = L"";
				pITEM_TBLDAT = reinterpret_cast<sITEM_TBLDAT*>(API_GetTableContainer()->GetItemTable()->FindData(pTIMEQUEST_DATASET->needItemTblidx));
				NTL_ASSERT(pITEM_TBLDAT, "CTMQSideViewGui::SetViewContent, Not exist table of index : " << pTIMEQUEST_DATASET->needItemTblidx);				

				if( pITEM_TBLDAT )
					pItemTextAllTable->GetText( pITEM_TBLDAT->Name, &wstrText );

				m_pMessage_4_Content->SetText( wstrText.c_str() );
			}
			else
			{
				// 없음
				m_pMessage_4_Content->SetText(GetDisplayStringManager()->GetString(DST_TMQ_BALLOON_NOT_THING));
			}

			m_pMessage_4_Content->SetPosition(rect.right, rect.top);

			m_NeedItem.SetIcon(pTMQ->GetTMQTable_for_RegRoom()->sTimeQuestDataset[pTMQ->GetDifficult_for_RegRoom()].needItemTblidx);
			m_NeedItem.SetPosition_fromParent((rtScreen.GetWidth() - m_NeedItem.GetWidth())/2, rect.bottom + dSTRING_HEIGHT_GAP);
			m_NeedItem.SetParentPosition(rtScreen.left, rtScreen.top);
			rect.bottom = rect.bottom + dSTRING_HEIGHT_GAP + m_NeedItem.GetHeight() + dSTRING_HEIGHT_GAP;

			// 이동하기
			// 타임머신 퀘스트 진입 %d초 전까지 등록한 로보트 근처로 단 한번 텔레포트 할 수 있습니다
			swprintf_s(awcBuffer, 256, GetDisplayStringManager()->GetString(DST_TMQ_BALLOON_CAN_TELEPORT), (RwInt32)dTSC_TIME_DISABLE_TELEPORT);
			m_pOKButton->SetToolTip(awcBuffer);
			m_pOKButton->SetText(GetDisplayStringManager()->GetString(DST_TMQ_BALLOON_MOVE));

			// 예약취소
			m_pCancelButton->SetText(GetDisplayStringManager()->GetString(DST_TMQ_BALLOON_CENCLE_RESERVATION));

			m_iViewHeight = rect.bottom + m_pCancelButton->GetHeight() + dVIEWBOTTOM_GAP;


			m_pMessage_1->Show(true);
			m_pMessage_2->Show(true);
			m_pMessage_3->Show(true);
			m_pMessage_4->Show(true);
			m_pMessage_1_Content->Show(true);
			m_pMessage_2_Content->Show(true);
			m_pMessage_3_Content->Show(true);
			m_pMessage_4_Content->Show(true);			


			if( pTMQ->CanTMQTeleport() )
				m_pOKButton->Show(true);
			else
				m_pOKButton->Show(false);

			if( pTMQ->GetWaitMode() == TIMEQUEST_MODE_PARTY )
			{
				if( Logic_I_am_PartyLeader() )
					m_pCancelButton->Show(true);
				else
					m_pCancelButton->Show(false);
			}
			else
				m_pCancelButton->Show(true);

			break;
		}
	}
}

void CTMQSideViewGui::SetNeedItemState()
{
	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( !pAvatar )
		return;

	CNtlTMQ*					pTMQ						= pAvatar->GetTMQ();
	sTIMEQUEST_ROOM_JOIN_INFO*	pTIMEQUEST_ROOM_JOIN_INFO	= pTMQ->GetRoom();
	if( !pTIMEQUEST_ROOM_JOIN_INFO )
	{
		DBO_FAIL("Not exist TMQ joined room infomation");
		return;
	}

	if( pTIMEQUEST_ROOM_JOIN_INFO->byRoomState != TIMEQUEST_ROOM_STATE_WAITENTRY )
		return;


	sTIMEQUEST_TBLDAT* pTIMEQUEST_TBLDAT = pTMQ->GetTMQTable_for_RegRoom();
	if( pTIMEQUEST_TBLDAT )
	{
		TBLIDX tblidx = pTIMEQUEST_TBLDAT->sTimeQuestDataset[pTMQ->GetDifficult_for_RegRoom()].needItemTblidx;
		if( Logic_FindInventoryItemByTableId(tblidx) )
		{
			// TMQ 진입에 필요한 아이템을 가방에 넣고 있다
			m_pMessage_4->SetTextColor(RGB(255, 255, 255), TRUE);
			m_pMessage_4_Content->SetTextColor(dREGISTER_COLOR, TRUE);
			return;
		}
	}	

	m_pMessage_4->SetTextColor(dUNREGISTER_COLOR, TRUE);
	m_pMessage_4_Content->SetTextColor(dUNREGISTER_COLOR, TRUE);
}

void CTMQSideViewGui::CheckInfoWindow()
{
	if( GetInfoWndManager()->GetInfoWndState() == CInfoWndManager::INFOWND_TABLE_ITEM &&
		GetInfoWndManager()->GetRequestGui() == DIALOG_SIDEICON )
		GetInfoWndManager()->ShowInfoWindow( FALSE );
}

void CTMQSideViewGui::OnSideViewClose()
{
	Show(false);
	CheckInfoWindow();
}

void CTMQSideViewGui::OnSideViewLocate(const CRectangle& rectSideIcon)
{
	CRectangle rect;
	CRectangle rtScreen;

	// TMQ View 영역
	rtScreen.SetRectWH(rectSideIcon.left - m_BackPanel.GetWidth() + rectSideIcon.GetWidth(),
		rectSideIcon.top - m_iViewHeight,
		m_BackPanel.GetWidth(),
		m_iViewHeight);	

	m_pThis->SetPosition(rtScreen);
	m_BackPanel.SetRect(rtScreen);	

	rect = m_pOKButton->GetPosition();
	m_pOKButton->SetPosition(15, rtScreen.GetHeight() - dVIEWBOTTOM_GAP - rect.GetHeight() );
	m_pCancelButton->SetPosition(147, rtScreen.GetHeight() - dVIEWBOTTOM_GAP - rect.GetHeight() );	
}

VOID CTMQSideViewGui::OnExitClicked(gui::CComponent* pComponent)
{
	Show(false);

	CTMQSideIconGui* pTMQIcon = reinterpret_cast<CTMQSideIconGui*>( CSideIconGui::GetInstance()->GetSideIcon(SIDEICON_TMQ) );
	pTMQIcon->SetIconImageType(CTMQSideIconGui::IIT_NORMAL);
}

VOID CTMQSideViewGui::OnOKClicked(gui::CComponent* pComponent)
{
	if( byViewMode == VM_RETRY_LOTTERY )
	{
		// TMQ 추첨을 신청한다
		CNtlTMQ* pTMQ = GetNtlSLGlobal()->GetSobAvatar()->GetTMQ();
		GetDboGlobal()->GetGamePacketGenerator()->SendTMQ_Room_Join_Req(pTMQ->GetNPCHandle(),
																		(RwUInt8)pTMQ->GetWaitMode(),
																		true);

		// 일단 TMQ 정보를 지우고 서버로 부터 패킷이 오면 다시 채워 놓는다
		CNtlSLEventGenerator::SLTMQBoard(TBT_ROOM_LEAVE);
	}
	else if( byViewMode == VM_REMAIN_TIME )
	{
		// TMQ에서 제공하는 텔레포트를 한다
		GetDboGlobal()->GetGamePacketGenerator()->SendTMQ_Room_Teleport_Req();
	}
}

VOID CTMQSideViewGui::OnCancelClicked(gui::CComponent* pComponent)
{
	if( byViewMode == VM_LOTTERY_WAITING ||
		byViewMode == VM_REMAIN_TIME )
	{
		// TMQ 예약을 취소한다
		GetDboGlobal()->GetGamePacketGenerator()->SendTMQ_Room_Leave_Req();
	}
	else if( byViewMode == VM_RETRY_LOTTERY )
	{
		CNtlSLEventGenerator::SLTMQBoard(TBT_ROOM_LEAVE);
	}
}

VOID CTMQSideViewGui::OnMove(RwInt32 iOldX, RwInt32 iOldY)
{
	CRectangle rtScreen = m_pThis->GetScreenRect();

	m_NeedItem.SetParentPosition(rtScreen.left, rtScreen.top);

	CheckInfoWindow();
}

VOID CTMQSideViewGui::OnMouseMove(RwInt32 nFlags, RwInt32 nX, RwInt32 nY)
{
	if( m_NeedItem.PtInRect(nX, nY) && m_NeedItem.GetSerial() != INVALID_SERIAL_ID )
	{
		if( GetInfoWndManager()->GetRequestGui() != DIALOG_SIDEICON )
		{
			CRectangle rtScreen = m_pThis->GetScreenRect();
			GetInfoWndManager()->ShowInfoWindow( TRUE, CInfoWndManager::INFOWND_TABLE_ITEM,
				rtScreen.left + m_NeedItem.GetX_fromParent(), rtScreen.top + m_NeedItem.GetY_fromParent(),
				m_NeedItem.GetItemTable(), DIALOG_SIDEICON );
		}
	}
	else
	{
		GetInfoWndManager()->ShowInfoWindow( FALSE );
	}
}

VOID CTMQSideViewGui::OnMouseLeave(gui::CComponent* pComponent)
{
	GetInfoWndManager()->ShowInfoWindow( FALSE );
}

VOID CTMQSideViewGui::OnPaint()
{
	m_BackPanel.Render();

	// avooo's comment : 분단위로 나타낼 때 1을 더하는 이유는 1분이 표시되고 있을 때
	// 실제로는 1분 ~ 1분 59초가 남아있을 때이기에....
	if( byViewMode == VM_LOTTERY_WAITING )
	{
		CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
		if( !pAvatar )
			return;

		CNtlTMQ*	pTMQ		= pAvatar->GetTMQ();
		RwReal		fRemainTime	= pTMQ->GetRemainTime_in_Room();

		if( fRemainTime >= 60.f )
			m_pMessage_3_Content->Format(L"%d%s", 1 + (RwInt32)(fRemainTime/60.f), GetDisplayStringManager()->GetString(DST_TIME_MINUTE));
		else
			m_pMessage_3_Content->Format(L"%d%s", (RwInt32)fRemainTime, GetDisplayStringManager()->GetString(DST_TIME_SECOND));
	}
	else if( byViewMode == VM_HONGING_A_LOTTERY )
	{
		CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
		if( !pAvatar )
			return;

		CNtlTMQ*	pTMQ		= pAvatar->GetTMQ();
		RwReal		fRemainTime	= pTMQ->GetRemainTime_in_Room();

		if( fRemainTime >= 60.f )
			m_pMessage_2_Content->Format(L"%d%s", 1 + (RwInt32)(fRemainTime/60.f), GetDisplayStringManager()->GetString(DST_TIME_MINUTE));
		else
			m_pMessage_2_Content->Format(L"%d%s", (RwInt32)fRemainTime, GetDisplayStringManager()->GetString(DST_TIME_SECOND));
	}
	else if( byViewMode == VM_RETRY_LOTTERY )
	{
		if( fTMQIconElapsed >= 60.f )
			m_pMessage_4->Format(L"%d%s", 1 + (RwInt32)(fTMQIconElapsed/60.f), GetDisplayStringManager()->GetString(DST_TIME_MINUTE));
		else
			m_pMessage_4->Format(L"%d%s", (RwInt32)fTMQIconElapsed, GetDisplayStringManager()->GetString(DST_TIME_SECOND));
	}
	else if( byViewMode == VM_REMAIN_TIME )
	{
		CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
		if( !pAvatar )
			return;

		CNtlTMQ*	pTMQ		= pAvatar->GetTMQ();
		RwReal		fRemainTime	= pTMQ->GetRemainTime_in_Room();

		if( fRemainTime >= 60.f )
			m_pMessage_3_Content->Format(L"%d%s", 1 + (RwInt32)(fRemainTime/60.f), GetDisplayStringManager()->GetString(DST_TIME_MINUTE));
		else
		{
			m_pMessage_3_Content->Format(L"%d%s", (RwInt32)fRemainTime, GetDisplayStringManager()->GetString(DST_TIME_SECOND));

			if( fRemainTime <= dTSC_TIME_DISABLE_TELEPORT 
				&& m_pOKButton->IsClickEnable() )
			{
				// 타임머신 퀘스트 진입 직전입니다. 텔레포트 하실 수 없습니다
				m_pOKButton->SetToolTip(GetDisplayStringManager()->GetString(DST_TMQ_BALLOON_CANNOT_TELEPORT));
				m_pOKButton->ClickEnable(false);
			}
		}	

		m_NeedItem.Paint();
	}
}

VOID CTMQSideViewGui::HandleEvents( RWS::CMsg &msg )
{
	NTL_FUNCTION("CTMQSideViewGui::HandleEvents");

	if( msg.Id == g_EventTMQBoardNotify )
	{
		SNtlEventTMQBoardNotify* pEvent = reinterpret_cast<SNtlEventTMQBoardNotify*>( msg.pData );

		switch(pEvent->uiEventType)
		{
		case TBT_ROOM_INFO:
			{
				CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
				if( !pAvatar )
				{
					DBO_FAIL("Not exist avatar instance");
					NTL_RETURNVOID();
				}

				CNtlTMQ*					pTMQ	= pAvatar->GetTMQ();
				sTIMEQUEST_ROOM_JOIN_INFO*	pRoom	= pTMQ->GetRoom();
				if( !pRoom )
				{
					DBO_FAIL("Not exist TMQ joined room infomation");
					NTL_RETURNVOID();
				}

				uTIMEQUEST_ROOM_ICON_INFO* pTIMEQUEST_ROOM_ICON_INFO = reinterpret_cast<uTIMEQUEST_ROOM_ICON_INFO*>(pEvent->pExData);

				if( pRoom->byRoomState == TIMEQUEST_ROOM_STATE_WAITENTRY )
				{
					if( byViewMode == VM_LOTTERY_WAITING )
					{
						// 파티 / 명
						m_pMessage_2_Content->Format(L"%d%s / %d%s", pTIMEQUEST_ROOM_ICON_INFO->sRegistrationInfo.wWaitPartyCount,
							GetDisplayStringManager()->GetString(DST_TAB_PARTY),
							pTIMEQUEST_ROOM_ICON_INFO->sRegistrationInfo.wWaitIndividualCount,
							GetDisplayStringManager()->GetString(DST_ETC_PERSONS));
					}
				}

				break;
			}
		case TBT_ROOM_RESERVATION:
			{
				SetViewContent(VM_LOTTERY_WAITING);				
				break;
			}
		case TBT_ROOM_SUCCESS_SELECTION:
			{
				CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
				if( !pAvatar )
				{
					DBO_FAIL("Not exist avatar instance");
					NTL_RETURNVOID();
				}

				CNtlTMQ*			pTMQ				= pAvatar->GetTMQ();
				sTIMEQUEST_TBLDAT*	pTIMEQUEST_TBLDAT	= pTMQ->GetTMQTable_for_RegRoom();
				if( !pTIMEQUEST_TBLDAT )
				{
					DBO_FAIL("TBT_ROOM_SUCCESS_SELECTION, Not exist TMQ table");
					NTL_RETURNVOID();
				}

				eTIMEQUEST_DIFFICULTY eDuffucultType = pTMQ->GetDifficult_for_RegRoom();
				if( eDuffucultType >= MAX_TIMEQUEST_DIFFICULTY )
				{
					DBO_FAIL("TBT_ROOM_SUCCESS_SELECTION, Invalid TMQ dificult type");
					NTL_RETURNVOID();
				}

				sTIMEQUEST_DATASET* pTIMEQUEST_DATASET = &pTIMEQUEST_TBLDAT->sTimeQuestDataset[eDuffucultType];
				if( !pTIMEQUEST_DATASET )
				{
					DBO_FAIL("TBT_ROOM_SUCCESS_SELECTION, Not exist TMQ data set of TMQ table index : " << pTIMEQUEST_TBLDAT->tblidx);
					NTL_RETURNVOID();
				}

				CRectangle rect;
				CQuestTextDataTable*			pQuestTextTable			= API_GetTableContainer()->GetQuestTextDataTable();
				sQUEST_TEXT_DATA_TBLDAT*		pQUEST_TEXT_DATA_TBLDAT = reinterpret_cast<sQUEST_TEXT_DATA_TBLDAT*>( pQuestTextTable->FindData( pTIMEQUEST_DATASET->nameTblidx ) );
				if( NULL == pQUEST_TEXT_DATA_TBLDAT )
				{
					DBO_FAIL("TBT_ROOM_SUCCESS_SELECTION, Not exist QUEST TEXT DATA TABLE of index : " << pTIMEQUEST_DATASET->nameTblidx);
				}


				// View 모드 변경
				byViewMode = VM_SUCCESS_LOTTERY;

				// 축하합니다
				m_pNotifyTitle->SetText(GetDisplayStringManager()->GetString(DST_TMQ_BALLOON_CONGRATULATION));
				rect = m_pNotifyTitle->GetPosition();

				// 신청하신 타임머신 퀘스트에 추첨되었습니다
				m_pMessage_1->SetText(GetDisplayStringManager()->GetString(DST_TMQ_BALLOON_SUCCESS_LOTTERY));
				m_pMessage_1->SetPosition(dMESSAGE_X, rect.bottom + dSTRING_HEIGHT_GAP);
				rect = m_pMessage_1->GetPosition();

				// 제   목 : 
				m_pMessage_2->Format(L"%s : ", GetDisplayStringManager()->GetString(DST_TMQ_BALLOON_TITLE));
				m_pMessage_2->SetPosition(dMESSAGE_X, rect.bottom + dSTRING_HEIGHT_GAP);
				rect = m_pMessage_2->GetPosition();

				if( pQUEST_TEXT_DATA_TBLDAT )
				{
					std::wstring& wstrText = gui::GetHtmlFromMemoryString(pQUEST_TEXT_DATA_TBLDAT->wstrText.c_str(), pQUEST_TEXT_DATA_TBLDAT->wstrText.size());
					m_pMessage_2_Content->SetText( wstrText.c_str() );
				}

				m_pMessage_2_Content->SetPosition(rect.right, rect.top);

				m_iViewHeight = rect.bottom + dVIEWBOTTOM_GAP;


				m_pMessage_1->Show(true);
				m_pMessage_2->Show(true);
				m_pMessage_3->Show(false);
				m_pMessage_4->Show(false);
				m_pMessage_1_Content->Show(false);
				m_pMessage_2_Content->Show(true);
				m_pMessage_3_Content->Show(false);
				m_pMessage_4_Content->Show(false);			

				m_pOKButton->Show(false);
				m_pCancelButton->Show(false);


				CDboEventGenerator::NotifyMessage(SDboEventNotify::ACTION, GetDisplayStringManager()->GetString(DST_TMQ_BALLOON_SUCCESS_LOTTERY));

				break;
			}
		case TBT_ROOM_ASK_RETRY:
			{
				CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
				if( !pAvatar )
				{
					DBO_FAIL("Not exist avatar instance");
					NTL_RETURNVOID();
				}

				CNtlTMQ*			pTMQ				= pAvatar->GetTMQ();
				sTIMEQUEST_TBLDAT*	pTIMEQUEST_TBLDAT	= pTMQ->GetTMQTable_for_RegRoom();
				if( !pTIMEQUEST_TBLDAT )
				{
					DBO_FAIL("TBT_ROOM_ASK_RETRY, Not exist TMQ table");
					NTL_RETURNVOID();
				}

				eTIMEQUEST_DIFFICULTY eDuffucultType = pTMQ->GetDifficult_for_RegRoom();
				if( eDuffucultType >= MAX_TIMEQUEST_DIFFICULTY )
				{
					DBO_FAIL("TBT_ROOM_ASK_RETRY, Invalid TMQ dificult type");
					NTL_RETURNVOID();
				}

				sTIMEQUEST_DATASET* pTIMEQUEST_DATASET = &pTIMEQUEST_TBLDAT->sTimeQuestDataset[eDuffucultType];
				if( !pTIMEQUEST_DATASET )
				{
					DBO_FAIL("TBT_ROOM_ASK_RETRY, Not exist TMQ data set of TMQ table index : " << pTIMEQUEST_TBLDAT->tblidx);
					NTL_RETURNVOID();
				}

				CRectangle rect;
				CQuestTextDataTable* pQuestTextTable = API_GetTableContainer()->GetQuestTextDataTable();
				uTIMEQUEST_ROOM_SELECTION_INFO*	pTIMEQUEST_ROOM_SELECTION_INFO	= reinterpret_cast<uTIMEQUEST_ROOM_SELECTION_INFO*>(pEvent->pExData);				
				sQUEST_TEXT_DATA_TBLDAT*		pQUEST_TEXT_DATA_TBLDAT			= reinterpret_cast<sQUEST_TEXT_DATA_TBLDAT*>( pQuestTextTable->FindData( pTIMEQUEST_DATASET->nameTblidx ) );
				if( pQUEST_TEXT_DATA_TBLDAT )
				{
					DBO_FAIL("TBT_ROOM_ASK_RETRY, Not exist QUEST TEXT DATA TABLE of index : " << pTIMEQUEST_DATASET->nameTblidx);
				}
				
				

				// View 모드 변경
				byViewMode = VM_FAIL_LOTTERY;

				// 죄송합니다
				m_pNotifyTitle->SetText(GetDisplayStringManager()->GetString(DST_TMQ_BALLOON_I_AM_SORRY));
				rect = m_pNotifyTitle->GetPosition();

				// 신청하신 타임머신 퀘스트에 당첨되지 않았습니다
				m_pMessage_1->SetText(GetDisplayStringManager()->GetString(DST_TMQ_BALLOON_FAIL_LOTTERY));
				m_pMessage_1->SetPosition(dMESSAGE_X, rect.bottom + dSTRING_HEIGHT_GAP);
				rect = m_pMessage_1->GetPosition();

				// 제   목 : 
				m_pMessage_2->Format(L"%s : ", GetDisplayStringManager()->GetString(DST_TMQ_BALLOON_TITLE));
				m_pMessage_2->SetPosition(dMESSAGE_X, rect.bottom + dSTRING_HEIGHT_GAP);
				rect = m_pMessage_2->GetPosition();

				if( pQUEST_TEXT_DATA_TBLDAT )
				{
					std::wstring& wstrText = gui::GetHtmlFromMemoryString(pQUEST_TEXT_DATA_TBLDAT->wstrText.c_str(), pQUEST_TEXT_DATA_TBLDAT->wstrText.size());
					m_pMessage_2_Content->SetText( wstrText.c_str() );
				}

				m_pMessage_2_Content->SetPosition(rect.right, rect.top);
				rect = m_pMessage_2->GetPosition();

				// 출발시간 : 
				m_pMessage_3->Format(L"%s : ", GetDisplayStringManager()->GetString(DST_TMQ_BALLOON_START_TIME));
				m_pMessage_3->SetPosition(dMESSAGE_X, rect.bottom + dSTRING_HEIGHT_GAP);
				rect = m_pMessage_3->GetPosition();

				m_pMessage_3_Content->Format(L"%02d:%02d", pTIMEQUEST_ROOM_SELECTION_INFO->sNextTmqInfo.sNextTMQTime.byHour,
					pTIMEQUEST_ROOM_SELECTION_INFO->sNextTmqInfo.sNextTMQTime.byMinute);
				m_pMessage_3_Content->SetPosition(rect.right, rect.top);

				m_iViewHeight = rect.bottom + dVIEWBOTTOM_GAP;


				// 등록 후 추첨 실패시 다음 TMQ의 시간을 계산하기 위해 저장
				nextTimequest = pTIMEQUEST_ROOM_SELECTION_INFO->sNextTmqInfo.sNextTMQTime;


				m_pMessage_1->Show(true);
				m_pMessage_2->Show(true);
				m_pMessage_3->Show(true);
				m_pMessage_4->Show(false);
				m_pMessage_1_Content->Show(false);
				m_pMessage_2_Content->Show(true);
				m_pMessage_3_Content->Show(true);
				m_pMessage_4_Content->Show(false);			

				m_pOKButton->Show(false);
				m_pCancelButton->Show(false);	


				CDboEventGenerator::NotifyMessage(SDboEventNotify::ACTION, GetDisplayStringManager()->GetString(DST_TMQ_BALLOON_FAIL_LOTTERY));

				break;
			}
		case TBT_ROOM_RESERVATION_NFY:
			{
				sTIMEQUEST_ROOM_JOIN_INFO* pTIMEQUEST_ROOM_JOIN_INFO = reinterpret_cast<sTIMEQUEST_ROOM_JOIN_INFO*>(pEvent->pExData);

				if( pTIMEQUEST_ROOM_JOIN_INFO->byRoomState == TIMEQUEST_ROOM_STATE_REGISTRAITION )
				{
					SetViewContent(VM_LOTTERY_WAITING);
				}
				else if( pTIMEQUEST_ROOM_JOIN_INFO->byRoomState == TIMEQUEST_ROOM_STATE_WAITENTRY )
				{
					SetViewContent(VM_REMAIN_TIME);
				}

				break;
			}
		case TBT_ROOM_TELEPORT:
			{
				m_pOKButton->Show(false);
				break;
			}
		case TBT_ROOM_LEAVE:
		case TBT_ROOM_LAUNCHED_TM:
		case TBT_ROOM_SERVER_EXCEPTION:
			{
				m_NeedItem.Clear();
				Show(false);
				m_pOKButton->ClickEnable(true);
				m_pOKButton->SetToolTip(L"");
				break;
			}
		}
	}
	else if(msg.Id == g_EventPartyUpdateValue)
	{
		SNtlEventPartyUpdate* pPacket = reinterpret_cast<SNtlEventPartyUpdate*>(msg.pData);

		if( byViewMode == VM_NONE )
			NTL_RETURNVOID();

		if( pPacket->iMessage == PMT_PARTY_LEADER_CHANGE )
		{
			SetViewContent(byViewMode);
		}
	}
	else if( msg.Id == g_EventSobInfoUpdate )
	{
		if( Logic_IsUpdateType(EVENT_AIUT_ITEM, msg.pData) == false )
			NTL_RETURNVOID();

		SNtlEventSobInfoUpdate* pEvent = reinterpret_cast<SNtlEventSobInfoUpdate*>( msg.pData );
		if( Logic_GetAvatarHandle() != pEvent->hSerialId )
			NTL_RETURNVOID();

		SetNeedItemState();
	}

	NTL_RETURNVOID();
}