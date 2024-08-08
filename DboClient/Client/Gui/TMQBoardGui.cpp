#include "precomp_dboclient.h"
#include "TMQBoardGui.h"

// core
#include "NtlDebug.h"

// share
#include "TextAllTable.h"
#include "TimeQuestTable.h"
#include "TableContainer.h"
#include "QuestTextDataTable.h"

// presentation
#include "NtlPLGuiManager.h"

// simulation
#include "NtlSLEvent.h"
#include "NtlSLEventFunc.h"
#include "NtlSLApi.h"
#include "ntlCameraController.h"
#include "NtlSLTMQ.h"
#include "NtlSobAvatar.h"
#include "NtlGameRuleConcept.h"
#include "NtlSLGlobal.h"

// dbo
#include "DboEvent.h"
#include "DboEventGenerator.h"
#include "DialogManager.h"
#include "IconMoveManager.h"
#include "InfoWndManager.h"
#include "DboGlobal.h"
#include "DisplayStringManager.h"

namespace
{
	#define dREGISTER_COLOR			RGB(255, 255, 255)
	#define dUNREGISTER_COLOR		RGB(255, 0, 0)
};

CTMQBoardGui::CTMQBoardGui(const RwChar* pName)
:CNtlPLGui(pName)
,m_hNPCSerial(INVALID_SERIAL_ID)
,m_pTIMEQUEST_TBLDAT(NULL)
{
}

CTMQBoardGui::~CTMQBoardGui()
{

}

RwBool CTMQBoardGui::Create()
{
	NTL_FUNCTION( "CTMQBoardGui::Create" );

	if(!CNtlPLGui::Create("", "gui\\TMQBoard.srf", "gui\\TMQBoard.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");

	CRectangle rect;	

	// 닫기 버튼
	m_pExitButton = (gui::CButton*)GetComponent( "btnExit" );
	m_slotCloseButton = m_pExitButton->SigClicked().Connect(this, &CTMQBoardGui::ClickedCloseButton);

	// '제한레벨'
	rect.SetRectWH(32, 55, 80, 14);
	m_pLimitedLevel = NTL_NEW gui::CStaticBox( rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_LEFT );
	m_pLimitedLevel->CreateFontStd( DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pLimitedLevel->SetTextColor( RGB(255, 190, 77) );
	m_pLimitedLevel->SetText(GetDisplayStringManager()->GetString(DST_TMQ_BOARD_LIMITE_LEVEL));
	m_pLimitedLevel->Enable(false);	

	// 제한레벨 표시
	rect.SetRectWH(100, 45, 150, 40);
	m_pLimitedLevelText = NTL_NEW gui::CStaticBox( rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_CENTER );
	m_pLimitedLevelText->CreateFontStd( DEFAULT_FONT, 130, DEFAULT_FONT_ATTR);
	m_pLimitedLevelText->Enable(false);	

	// TMQ 제목
	rect.SetRectWH(68, 94, 210, 20);
	m_pTitle = NTL_NEW gui::CStaticBox( rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_CENTER );
	m_pTitle->CreateFontStd( DEFAULT_FONT, 110, DEFAULT_FONT_ATTR);
	m_pTitle->Enable(false);

	// '난이도'
	rect.SetRectWH(32, 130, 100, 14);
	m_pDifficult = NTL_NEW gui::CStaticBox( rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_LEFT );
	m_pDifficult->CreateFontStd( "detail", DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pDifficult->SetTextColor( RGB(255, 190, 77) );
	m_pDifficult->SetText( "난이도" );
	m_pDifficult->Enable(false);

	// '출발시간'
	rect.SetRectWH(32, 152, 100, 14);
	m_pStartTime = NTL_NEW gui::CStaticBox( rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_LEFT );
	m_pStartTime->CreateFontStd( "detail", DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pStartTime->SetTextColor( RGB(255, 190, 77) );
	m_pStartTime->SetText(GetDisplayStringManager()->GetString(DST_TMQ_BALLOON_START_TIME));
	m_pStartTime->Enable(false);

	// '제한시간'
	rect.SetRectWH(32, 174, 100, 14);
	m_pLimitedTime = NTL_NEW gui::CStaticBox( rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_LEFT );
	m_pLimitedTime->CreateFontStd( "detail", DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pLimitedTime->SetTextColor( RGB(255, 190, 77) );
	m_pLimitedTime->SetText(GetDisplayStringManager()->GetString(DST_TMQ_BOARD_LIMITE_TIME));
	m_pLimitedTime->Enable(false);

	// 난이도 표시
	rect.SetRectWH(120, 130, 100, 14);
	m_pDifficultText = NTL_NEW gui::CStaticBox( rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_LEFT );
	m_pDifficultText->CreateFontStd( "detail", DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pDifficultText->Enable(false);

	// 출발시간 표시
	rect.SetRectWH(120, 152, 100, 14);
	m_pStartTimeText = NTL_NEW gui::CStaticBox( rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_LEFT );
	m_pStartTimeText->CreateFontStd( "detail", DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pStartTimeText->Enable(false);

	// 제한시간 표시
	rect.SetRectWH(120, 174, 100, 14);
	m_pLimitedTimeText = NTL_NEW gui::CStaticBox( rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_LEFT );
	m_pLimitedTimeText->CreateFontStd( "detail", DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pLimitedTimeText->Enable(false);

	// 현재 예약한 인원
	rect.SetRectWH(16, 221, 314, 14);
	m_pReservationPeople = NTL_NEW gui::CStaticBox( rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_RIGHT );
	m_pReservationPeople->CreateFontStd( "detail", DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pReservationPeople->Enable(false);

	// '다음 타임머신 퀘스트'
	rect.SetRectWH(20, 253, 250, 14);
	m_pNextTitle = NTL_NEW gui::CStaticBox( rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_LEFT );
	m_pNextTitle->CreateFontStd( DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pNextTitle->SetTextColor( RGB(255, 190, 77) );
	m_pNextTitle->SetText(GetDisplayStringManager()->GetString(DST_TMQ_BOARD_NEXT_TMQ));
	m_pNextTitle->Enable(false);

	// ' 다음 퀘스트 1'
	rect.SetRectWH(32, 277, 306, 14);
	m_pNextTitle_Senario_1 = NTL_NEW gui::CStaticBox( rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_LEFT );
	m_pNextTitle_Senario_1->CreateFontStd( "detail", DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pNextTitle_Senario_1->Enable(false);

	// '다음 퀘스트 2'
	rect.SetRectWH(32, 299, 306, 14);
	m_pNextTitle_Senario_2 = NTL_NEW gui::CStaticBox( rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_LEFT );
	m_pNextTitle_Senario_2->CreateFontStd( "detail", DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pNextTitle_Senario_2->Enable(false);

	// '다음 퀘스트 3'
	rect.SetRectWH(32, 321, 306, 14);
	m_pNextTitle_Senario_3 = NTL_NEW gui::CStaticBox( rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_LEFT );
	m_pNextTitle_Senario_3->CreateFontStd( "detail", DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pNextTitle_Senario_3->Enable(false);

	// 개인 예약 버튼
	m_pPrivateReservationButton = (gui::CButton*)GetComponent( "btnPrivateReservation" );
	m_pPrivateReservationButton->SetText(GetDisplayStringManager()->GetString(DST_TMQ_BOARD_RESERVATION_PRIVATE));
	m_slotPrivateReservationButton = m_pPrivateReservationButton->SigClicked().Connect(this, &CTMQBoardGui::ClickedPrivateReservation);

	// 파티 예약 버튼
	m_pPartyReservationButton = (gui::CButton*)GetComponent( "btnPartyReservation" );
	m_pPartyReservationButton->SetText(GetDisplayStringManager()->GetString(DST_TMQ_BOARD_RESERVATION_PARTY));
	m_slotPartyReservationButton = m_pPartyReservationButton->SigClicked().Connect(this, &CTMQBoardGui::ClickedPartyReservation);

	// 예약 취소 버튼
	m_pCancelReservationButton = (gui::CButton*)GetComponent( "btnCancelReservation" );
	m_pCancelReservationButton->SetText(GetDisplayStringManager()->GetString(DST_TMQ_BALLOON_CENCLE_RESERVATION));
	m_slotCancelReservationButton = m_pCancelReservationButton->SigClicked().Connect(this, &CTMQBoardGui::ClickedCancelReservation);

	// 아이템 슬롯
	m_NeedItem.Create(m_pThis, DIALOG_TMQBOARD, REGULAR_SLOT_ITEM_TABLE);
	m_NeedItem.SetPosition_fromParent(265, 160);


	// sig	
	m_slotPaint			= m_pThis->SigPaint().Connect( this, &CTMQBoardGui::OnPaint );
	m_slotMove			= m_pThis->SigMove().Connect( this, &CTMQBoardGui::OnMove );
	m_slotMouseMove		= m_pThis->SigMouseMove().Connect( this, &CTMQBoardGui::OnMouseMove );
	m_slotMouseLeave	= m_pThis->SigMouseLeave().Connect( this, &CTMQBoardGui::OnMouseLeave );

	Show(false);

	GetNtlGuiManager()->AddUpdateFunc( this );

	LinkMsg(g_EventTMQBoardNotify);
	LinkMsg(g_EventNPCDialogOpen);
	LinkMsg(g_EventCharObjDelete);
	LinkMsg(g_EventDialog);

	NTL_RETURN(TRUE);
}

VOID CTMQBoardGui::Destroy()
{
	NTL_FUNCTION("CTMQBoardGui::Destroy");

	CheckInfoWindow();
	GetNtlGuiManager()->RemoveUpdateFunc( this );

	m_NeedItem.Destroy();
	
	UnLinkMsg(g_EventTMQBoardNotify);
	UnLinkMsg(g_EventNPCDialogOpen);
	UnLinkMsg(g_EventCharObjDelete);
	UnLinkMsg(g_EventDialog);

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy(); 

	NTL_RETURNVOID();
}

VOID CTMQBoardGui::Clear()
{
}

VOID CTMQBoardGui::Update(RwReal fElapsed)
{
	if( IsShow() == FALSE )
		return;
}

VOID CTMQBoardGui::SetRoomState(RwUInt8 byState)
{
	m_byRoomState = byState;
}

SERIAL_HANDLE CTMQBoardGui::GetNPCHandle()
{
	return m_hNPCSerial;
}

VOID CTMQBoardGui::CheckInfoWindow()
{
	if( GetInfoWndManager()->GetInfoWndState() == CInfoWndManager::INFOWND_TABLE_ITEM &&
		GetInfoWndManager()->GetRequestGui() == DIALOG_TMQBOARD )
		GetInfoWndManager()->ShowInfoWindow( FALSE );
}

VOID CTMQBoardGui::ClickedCloseButton(gui::CComponent* pComponent)
{
	GetDialogManager()->CloseDialog(DIALOG_TMQBOARD);
}

VOID CTMQBoardGui::ClickedPrivateReservation(gui::CComponent* pComponent)
{
	if( CNtlGameRuleConcept::GetInstance()->CanTransition(GAME_RULE_TYPE_MAIN_TMQ_WAIT_LOTTERY) == FALSE )
		return;

	// 개인 예약
	GetDboGlobal()->GetGamePacketGenerator()->SendTMQ_Room_Join_Req(m_hNPCSerial, TIMEQUEST_MODE_INDIVIDUAL,FALSE);
}

VOID CTMQBoardGui::ClickedPartyReservation(gui::CComponent* pComponent)
{
	if( CNtlGameRuleConcept::GetInstance()->CanTransition(GAME_RULE_TYPE_MAIN_TMQ_WAIT_LOTTERY) == FALSE )
		return;

	// 파티 예약
	GetDboGlobal()->GetGamePacketGenerator()->SendTMQ_Room_Join_Req(m_hNPCSerial, TIMEQUEST_MODE_PARTY, FALSE);
}

VOID CTMQBoardGui::ClickedCancelReservation(gui::CComponent* pComponent)
{
	// TMQ 예약을 취소한다
	GetDboGlobal()->GetGamePacketGenerator()->SendTMQ_Room_Leave_Req();
}

VOID CTMQBoardGui::OnMove(RwInt32 iOldX, RwInt32 iOldY)
{
	CRectangle rtScreen = m_pThis->GetScreenRect();

	m_NeedItem.SetParentPosition(rtScreen.left, rtScreen.top);

	CheckInfoWindow();
}

VOID CTMQBoardGui::OnMouseMove(RwInt32 nFlags, RwInt32 nX, RwInt32 nY)
{
	if( m_NeedItem.PtInRect(nX, nY) && m_NeedItem.GetSerial() != INVALID_SERIAL_ID )
	{
		if( GetInfoWndManager()->GetInfoWndState() != CInfoWndManager::INFOWND_TABLE_ITEM )
		{
			CRectangle rtScreen = m_pThis->GetScreenRect();
			GetInfoWndManager()->ShowInfoWindow( TRUE, CInfoWndManager::INFOWND_TABLE_ITEM,
												rtScreen.left + m_NeedItem.GetX_fromParent(),
												rtScreen.top + m_NeedItem.GetY_fromParent(),
												m_NeedItem.GetItemTable(), DIALOG_TMQBOARD );
		}

		return;
	}

	GetInfoWndManager()->ShowInfoWindow( FALSE );
}

VOID CTMQBoardGui::OnMouseLeave(gui::CComponent* pComponent)
{
	GetInfoWndManager()->ShowInfoWindow( FALSE );
}

VOID CTMQBoardGui::OnPaint()
{
	if( m_NeedItem.GetSerial() != INVALID_SERIAL_ID )
		m_NeedItem.Paint();
}

RwInt32 CTMQBoardGui::SwitchDialog(bool bOpen)
{	
	if( bOpen )
	{
		Show(true);
	}
	else
	{
		Show(false);		
		GetNtlWorldConcept()->RemoveWorldPlayConcept( WORLD_PLAY_NPC_COMMU );
		CNtlSLEventGenerator::CameraControlDelete( CAMERA_CONTROL_NPC );
		Clear();

		m_hNPCSerial = INVALID_SERIAL_ID;
		m_pTIMEQUEST_TBLDAT = NULL;
		CheckInfoWindow();
	}
	
	return 1;
}

VOID CTMQBoardGui::HandleEvents( RWS::CMsg &msg )
{
	NTL_FUNCTION("CTMQBoardGui::HandleEvents");

	if( msg.Id == g_EventNPCDialogOpen )
	{
		SDboNpcDialogOpen* pData = reinterpret_cast<SDboNpcDialogOpen*>( msg.pData );

		if( pData->eDialog != DIALOG_TMQBOARD )
			return;

		// 같은 NPC
		if( m_hNPCSerial == pData->hSerialId )
			return;

		// 다른 NPC와 대화를 하는 경우이다
		if( m_hNPCSerial != INVALID_SERIAL_ID )
		{
			GetDialogManager()->CloseDialog(DIALOG_TMQBOARD);
			return;
		}


		RwBool	bPacketLock		= FALSE;
		bool	bSendSuccess	= GetDboGlobal()->GetGamePacketGenerator()->SendTMQ_Room_list_Req(pData->hSerialId, &bPacketLock);

		if( bPacketLock && !bSendSuccess )
		{
			GetNtlWorldConcept()->RemoveWorldPlayConcept( WORLD_PLAY_NPC_COMMU );
			CNtlSLEventGenerator::CameraControlDelete( CAMERA_CONTROL_NPC );
			return;
		}


		CNtlTMQ* pTMQ = GetNtlSLGlobal()->GetSobAvatar()->GetTMQ();

		if( pTMQ->GetNPCHandle() == INVALID_SERIAL_ID )
			pTMQ->SetNPCHandle(pData->hSerialId);

		m_hNPCSerial = pData->hSerialId;
	}
	else if( msg.Id == g_EventTMQBoardNotify )
	{
		SNtlEventTMQBoardNotify* pEvent = reinterpret_cast<SNtlEventTMQBoardNotify*>( msg.pData );

		switch(pEvent->uiEventType)
		{
		case TBT_ROOM_LIST:
			{
				sTIMEQUEST_ROOM_TMQ_INFO* pListInfo = reinterpret_cast<sTIMEQUEST_ROOM_TMQ_INFO*>(pEvent->pExData);
				m_pTIMEQUEST_TBLDAT = reinterpret_cast<sTIMEQUEST_TBLDAT*>( API_GetTableContainer()->GetTimeQuestTable()->FindData(pListInfo->tmqTblidx) );
				if( m_pTIMEQUEST_TBLDAT )
				{	
					sTIMEQUEST_DATASET* pTIMEQUEST_DATASET = &m_pTIMEQUEST_TBLDAT->sTimeQuestDataset[pListInfo->byDifficult];
					CQuestTextDataTable* pQuestTextTable = API_GetTableContainer()->GetQuestTextDataTable();
					sQUEST_TEXT_DATA_TBLDAT* pQUEST_TEXT_DATA_TBLDAT = reinterpret_cast<sQUEST_TEXT_DATA_TBLDAT*>( pQuestTextTable->FindData( pTIMEQUEST_DATASET->nameTblidx ) );
					if( pQUEST_TEXT_DATA_TBLDAT )
					{
						RwBool bFittingLevel = pEvent->uiValue;
						COLORREF DifficultColor;
						std::wstring& wstrText = gui::GetHtmlFromMemoryString(pQUEST_TEXT_DATA_TBLDAT->wstrText.c_str(), pQUEST_TEXT_DATA_TBLDAT->wstrText.size());


						// 아바타의 레벨과 맞는 TMQ가 있는지 여부
						if( bFittingLevel )
						{
							DifficultColor = dREGISTER_COLOR;

							m_pPrivateReservationButton->ClickEnable(true);
							m_pPartyReservationButton->ClickEnable(true);
							m_pCancelReservationButton->ClickEnable(true);
						}
						else
						{
							DifficultColor = dUNREGISTER_COLOR;

							m_pPrivateReservationButton->ClickEnable(false);
							m_pPartyReservationButton->ClickEnable(false);
							m_pCancelReservationButton->ClickEnable(false);
						}


						// TMQ 제목
						m_pTitle->SetTextColor(DifficultColor, TRUE);
						m_pTitle->SetText( wstrText.c_str() );

						// TMQ 난이도
						m_pDifficultText->SetTextColor(DifficultColor, TRUE);
						switch(pListInfo->byDifficult)
						{
						case TIMEQUEST_DIFFICULTY_EASY:
							{
								// 쉬움
								m_pDifficultText->SetText(GetDisplayStringManager()->GetString(DST_TMQ_DEGREE_OF_DIFFICULTY_1));
								break;
							}
						case TIMEQUEST_DIFFICULTY_NORMAL:
							{
								// 보통
								m_pDifficultText->SetText(GetDisplayStringManager()->GetString(DST_TMQ_DEGREE_OF_DIFFICULTY_2));
								break;
							}
						case TIMEQUEST_DIFFICULTY_HARD:
							{
								// 어려움
								m_pDifficultText->SetText(GetDisplayStringManager()->GetString(DST_TMQ_DEGREE_OF_DIFFICULTY_3));
								break;
							}
						}

						// 출발시간
						m_pStartTimeText->SetTextColor(DifficultColor, TRUE);
						m_pStartTimeText->Format(L"%02d:%02d", pListInfo->sNextEnterTime.byHour, pListInfo->sNextEnterTime.byMinute);

						// 제한시간
						m_pLimitedTimeText->SetTextColor(DifficultColor, TRUE);
						m_pLimitedTimeText->Format(L"%u%s", (RwInt32)((RwReal)pTIMEQUEST_DATASET->dwLimitTime/60.f),
							GetDisplayStringManager()->GetString(DST_TIME_MINUTE));

						// 제한레벨
						m_pLimitedLevelText->SetTextColor(DifficultColor, TRUE);
						m_pLimitedLevelText->Format(L"%d Lv ~ %d Lv", pTIMEQUEST_DATASET->byMinMemberLevel,
							pTIMEQUEST_DATASET->byMaxMemberLevel);

						// 필요 아이템
						m_NeedItem.SetIcon(pTIMEQUEST_DATASET->needItemTblidx);

						// 현재 예약한 인원
						m_pReservationPeople->SetTextColor(DifficultColor, TRUE);
						m_pReservationPeople->Format(L"%s %d%s, %s %d%s %s",
							GetDisplayStringManager()->GetString(DST_TAB_PARTY),
							pListInfo->wWaitPartyCount,
							GetDisplayStringManager()->GetString(DST_TMQ_BOARD_TEAM),
							GetDisplayStringManager()->GetString(DST_TMQ_BOARD_PRIVATE),
							pListInfo->wWaitIndividualCount,
							GetDisplayStringManager()->GetString(DST_ETC_PERSONS),
							GetDisplayStringManager()->GetString(DST_TMQ_BOARD_RESERVATING));


						// 다음 퀘스트 1
						sTIMEQUEST_TIME nextTimequest = pListInfo->sNextEnterTime;
						nextTimequest.NextBySec((RwUInt8)m_pTIMEQUEST_TBLDAT->dwStartTime);
						m_pNextTitle_Senario_1->Format(L"%02d:%02d %s", nextTimequest.byHour,
																		nextTimequest.byMinute,
																		wstrText.c_str());

						// 다음 퀘스트 2
						nextTimequest.NextBySec((RwUInt8)m_pTIMEQUEST_TBLDAT->dwStartTime);
						m_pNextTitle_Senario_2->Format(L"%02d:%02d %s", nextTimequest.byHour,
																		nextTimequest.byMinute,
																		wstrText.c_str());

						// 다음 퀘스트 3
						nextTimequest.NextBySec((RwUInt8)m_pTIMEQUEST_TBLDAT->dwStartTime);
						m_pNextTitle_Senario_3->Format(L"%02d:%02d %s", nextTimequest.byHour,
																		nextTimequest.byMinute,
																		wstrText.c_str());						

						CNtlTMQ* pTMQ = GetNtlSLGlobal()->GetSobAvatar()->GetTMQ();
						sTIMEQUEST_TBLDAT* pTIMEQUEST_TBLDAT = pTMQ->GetTMQTable_for_RegRoom();

						// 현재 보고 있는 게시판이 등록된 게시판인지를 판별
						if( pTIMEQUEST_TBLDAT && pTIMEQUEST_TBLDAT->tblidx == m_pTIMEQUEST_TBLDAT->tblidx )
							SetRoomState(TIMEQUEST_ROOM_STATE_WAITENTRY);
						else
							SetRoomState(INVALID_TIMEQUEST_ROOM_STATE);
					}
					else
					{
						NTL_ASSERT(false, "CTMQBoardGui::HandleEvents, g_EventTMQBoardNotify, Not exist QUEST TEXT DATA TABLE of index : " << pTIMEQUEST_DATASET->nameTblidx);
					}
				}
				else
				{
					NTL_ASSERT(m_pTIMEQUEST_TBLDAT, "CTMQBoardGui::HandleEvents, Not exist tmq table of index : " << pListInfo->tmqTblidx);
				}

				GetDialogManager()->OpenDialog(DIALOG_TMQBOARD);
				break;
			}
		case TBT_ROOM_RESERVATION:
		case TBT_ROOM_RESERVATION_NFY:
			{
				sTIMEQUEST_ROOM_JOIN_INFO* pTIMEQUEST_ROOM_JOIN_INFO = reinterpret_cast<sTIMEQUEST_ROOM_JOIN_INFO*>(msg.pData);

				if( pTIMEQUEST_ROOM_JOIN_INFO->byRoomState == TIMEQUEST_ROOM_STATE_REGISTRAITION )
				{
					CNtlTMQ* pTMQ = GetNtlSLGlobal()->GetSobAvatar()->GetTMQ();
					sTIMEQUEST_TBLDAT* pTIMEQUEST_TBLDAT = pTMQ->GetTMQTable_for_RegRoom();

					// 현재 보고 있는 게시판이 등록된 게시판인지를 판별
					if( m_pTIMEQUEST_TBLDAT && pTIMEQUEST_TBLDAT->tblidx == m_pTIMEQUEST_TBLDAT->tblidx )
						SetRoomState(TIMEQUEST_ROOM_STATE_REGISTRAITION);
					else
						SetRoomState(INVALID_TIMEQUEST_ROOM_STATE);
				}
				else if( pTIMEQUEST_ROOM_JOIN_INFO->byRoomState == TIMEQUEST_ROOM_STATE_WAITENTRY )
				{
					SetRoomState(INVALID_TIMEQUEST_ROOM_STATE);
				}

				GetDialogManager()->CloseDialog(DIALOG_TMQBOARD);

				break;
			}
		case TBT_ROOM_SUCCESS_SELECTION:
			{
				CNtlTMQ* pTMQ = GetNtlSLGlobal()->GetSobAvatar()->GetTMQ();
				sTIMEQUEST_ROOM_JOIN_INFO* pTIMEQUEST_ROOM_JOIN_INFO = pTMQ->GetRoom();

				SetRoomState(pTIMEQUEST_ROOM_JOIN_INFO->byRoomState);

				break;
			}
		case TBT_ROOM_LEAVE:
		case TBT_ROOM_ASK_RETRY:
		case TBT_ROOM_LAUNCHED_TM:
		case TBT_ROOM_SERVER_EXCEPTION:
			{
				SetRoomState(INVALID_TIMEQUEST_ROOM_STATE);
				break;
			}
		}
	}
	else if( msg.Id == g_EventCharObjDelete )
	{		
		RwUInt32* pDeleteSerial = reinterpret_cast<RwUInt32*>( msg.pData );

		if( m_hNPCSerial == *pDeleteSerial )
		{
			// NPC 서버가 다운되거나 하여 갑자기 NPC가 사라지는 경우
			GetDialogManager()->CloseDialog(DIALOG_TMQBOARD);
		}
	}
	else if( msg.Id == g_EventDialog )
	{		
		SDboEventDialog* pPacket = (SDboEventDialog*)msg.pData;
		if( pPacket->iType == DIALOGEVENT_OPEN_FAIL_NPC_DIALOG &&
			pPacket->iDestDialog == DIALOG_TMQBOARD )
		{
			// TMQ 게시판을 여는데 서버로 부터 실패 resultcode 가 왔다
			GetNtlWorldConcept()->RemoveWorldPlayConcept( WORLD_PLAY_NPC_COMMU );
			CNtlSLEventGenerator::CameraControlDelete( CAMERA_CONTROL_NPC );

			m_hNPCSerial = INVALID_SERIAL_ID;
		}
	}

	NTL_RETURNVOID();
}