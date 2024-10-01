////////////////////////////////////////////////////////////////////////////////
// File: QuestIndicatorGui.cpp
// Desc: QuestIndicator
//
// 2006.11.22 Peessi@hitel.net
// 2020 Extended by Erinnyes
//
////////////////////////////////////////////////////////////////////////////////

#include "precomp_dboclient.h"
#include "QuestIndicatorGui.h"

// presentation
#include "NtlPLGuiManager.h"
#include "NtlPLEvent.h"

// gui
#include "GuiUtil.h"

// shared
#include "QuestTextDataTable.h"
#include "MobTable.h"
#include "QuestItemTable.h"
#include "TextAllTable.h"
#include "TableContainer.h"

// sound
#include "GUISoundDefine.h"

// simulation
#include "NtlSLEvent.h"
#include "NtlSLGlobal.h"
#include "NtlSLLogic.h"
#include "NtlSobQuestItem.h"
#include "NtlSobQuestItemAttr.h"
#include "NtlSobIcon.h"
#include "NtlSLApi.h"
#include "NtlWorldConcept.h"

#include "NtlStorageManager.h"
#include "NtlStorageGroupQuest.h"

// Dbo
#include "DboDef.h"
#include "DboGlobal.h"
#include "InfoWndManager.h"
#include "QuestGui.h"
#include "QuestListGui.h"
#include "SideDialogManager.h"
#include "DialogManager.h"

//test
#include "NtlSobNpc.h";
#include <math.h>       /* atan2 */

#define QUEST_INDICATOR_Y					300

#define QUEST_INDICATOR_ITEM_MARGIN			0

#define QUEST_INDICATOR_FRAME_WIDTH			258//155

#define QUEST_INDICATOR_QUESTSTATE_WIDTH	15
#define QUEST_INDICATOR_QUESTSTATE_HEIGHT	15
#define QUEST_INDICATOR_QUESTTITLE_WIDTH	QUEST_INDICATOR_FRAME_WIDTH
#define QUEST_INDICATOR_QUESTTITLE_TEXTPOSX	20//( QUEST_INDICATOR_QUESTSTATE_WIDTH )
#define QUEST_INDICATOR_QUESTSTATE_POSY		( QUEST_INDICATOR_QUESTTITLE_HEIGHT - QUEST_INDICATOR_QUESTSTATE_HEIGHT ) / 2 + 5

#define QUEST_INDICATOR_QUESTINFO_POSX		100//20
#define QUEST_INDICATOR_QUESTINFO_WIDTH		242
#define QUEST_INDICATOR_QUESTINFO_HEIGHT	16

#define QUEST_INDICATOR_QUESTRESULT_WIDTH	33
#define QUEST_INDICATOR_QUESTRESULT_HEIGHT	QUEST_INDICATOR_QUESTINFO_HEIGHT

#define QUEST_INDICATOR_QUESTTIME_POSX		40
#define QUEST_INDICATOR_QUESTTIME_WIDTH		( QUEST_INDICATOR_QUESTINFO_WIDTH - QUEST_INDICATOR_QUESTTIME_POSX )
#define QUEST_INDICATOR_QUESTTIME_HEIGHT	QUEST_INDICATOR_QUESTINFO_HEIGHT

#define QUEST_INDICATOR_COLOR_GRAY			RGB( 168, 168, 168 )
#define QUEST_INDICATOR_COLOR_WHITE			RGB( 254, 254, 254 )
#define QUEST_INDICATOR_COLOR_RED			RGB( 254, 100, 100 )

#define QUEST_INDICATOR_SHADOW_COLOR		RGB( 0, 0, 0 )
#define QUEST_INDICATOR_SHADOW_OFFSET		16

#define QUEST_INDICATOR_QUESTTITLE_FONTSIZE	95//90

VOID stINDICATOR_ITEM::OnClickTitle(gui::CComponent* pComponent)
{
	CNtlSLEventGenerator::ShowQuestWindow(m_TSKey);
}

VOID stINDICATOR_ITEM::OnMouseEnterTitle(gui::CComponent* pComponent)
{
	stINFOWND_QUESTINDICATOR stData;
	stData.uiQuestTitle = m_uiQuestTitle;
	stData.uiQuestGoal = m_uiQuestGoal;

	CRectangle rect = m_pTitle->GetScreenRect();
	GetInfoWndManager()->ShowInfoWindow(TRUE, CInfoWndManager::INFOWND_QUEST_INDICATOR, rect.left, rect.top, &stData, SDIALOG_QUEST_INDICATOR);
}

VOID stINDICATOR_ITEM::OnMouseLeaveTitle(gui::CComponent* pComponent)
{
	if (GetInfoWndManager()->GetRequestGui() == SDIALOG_QUEST_INDICATOR)
		GetInfoWndManager()->ShowInfoWindow(FALSE);
}


void stINDICATOR_ITEM::OnClickExpend(gui::CComponent* pComponent)
{
	if (m_bQuestInfoShown)
	{
		m_bQuestInfoShown = false;
	}
	else
	{
		m_bQuestInfoShown = true;
	}
}

CQuestIndicatorGui::CQuestIndicatorGui(VOID)
{
	Init();
}

CQuestIndicatorGui::CQuestIndicatorGui(const RwChar* pName)
	: CNtlPLGui(pName)
{
	Init();
}

CQuestIndicatorGui::~CQuestIndicatorGui(VOID)
{

}

VOID CQuestIndicatorGui::Init(VOID)
{
	m_nTotalItemHeight = 0;

	m_pQuestGui = NULL;

	m_nScrollPos = 0;
}

RwBool CQuestIndicatorGui::Create(CQuestGui* pQuestGui)
{
	NTL_FUNCTION("CQuestIndicatorGui::Create");

	if (!CNtlPLGui::Create("", "gui\\QuestIndicator.srf", "gui\\QuestIndicator.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(GetNtlGuiManager()->GetGuiManager());

	GetNtlGuiManager()->AddUpdateFunc(this);

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");

	m_pbtnMinimize = (gui::CButton*)GetComponent("btnMinimize");
	m_pbtnMinimize->SetToolTip(GetDisplayStringManager()->GetString("DST_QUESTINDICATOR_TRANSFORM_MINI"));
	m_slotClickMinimize = m_pbtnMinimize->SigClicked().Connect(this, &CQuestIndicatorGui::OnClickMinimize);

	m_pbtnAllExtendOn = (gui::CButton*)GetComponent("btnAllExtendOn");
	m_pbtnAllExtendOn->SetToolTip(GetDisplayStringManager()->GetString("DST_QUESTINDICATOR_ALLEXTEND_ON"));
	m_slotClickAllExtendOn = m_pbtnAllExtendOn->SigClicked().Connect(this, &CQuestIndicatorGui::OnClickAllExtendOn);

	m_pbtnAllExtendOff = (gui::CButton*)GetComponent("btnAllExtendOff");
	m_pbtnAllExtendOff->SetToolTip(GetDisplayStringManager()->GetString("DST_QUESTINDICATOR_ALLEXTEND_OFF"));
	m_pbtnAllExtendOff->Show(false);
	m_slotClickAllExtendOff = m_pbtnAllExtendOff->SigClicked().Connect(this, &CQuestIndicatorGui::OnClickAllExtendOff);

	m_pstbTitle = (gui::CStaticBox*)GetComponent("stbTitle");
	m_pstbTitle->SetText(GetDisplayStringManager()->GetString("DST_SIDEDLG_TITLE_QUESTINDICATOR"));

	m_ppnClippingArea = (gui::CPanel*)GetComponent("pnClippingArea");
	m_ppnClippingArea->SetClippingMode(TRUE);

	m_ppnIndicatorList = (gui::CPanel*)GetComponent("pnIndicatorList");
	m_ppnIndicatorList->SetHeight(QUEST_INDICATOR_MAX_HEIGHT);
	m_ppnIndicatorList->SetClippingMode(TRUE);

	m_pscrBar = (gui::CScrollBar*)GetComponent("scrBar");
	m_pscrBar->Show(false);
	m_slotScrollChanged = m_pscrBar->SigValueChanged().Connect(this, &CQuestIndicatorGui::OnScrollChanged);
	m_slotScrollSliderMoved = m_pscrBar->SigSliderMoved().Connect(this, &CQuestIndicatorGui::OnScrollChanged);

	m_ppnTitlePanel = (gui::CPanel*)GetComponent("pnTitlePanel");

	m_pbtnQuestListOpen = (gui::CButton*)GetComponent("btnQuestListOpen");
	m_pbtnQuestListOpen->SetToolTip(GetDisplayStringManager()->GetString("DST_QUESTINDICATOR_QUESTLIST_BTN"));
	m_slotClickQuestListOpen = m_pbtnQuestListOpen->SigClicked().Connect(this, &CQuestIndicatorGui::OnClickQuestListOpen);

	gui::CSurfaceManager* pSurfaceManager = GetNtlGuiManager()->GetSurfaceManager();
	CRectangle rect;

	for (int i = 0; i < eMAX_CAN_PROGRESS_QUEST_NUM; i++)
	{
		rect.SetRectWH(0, 0, QUEST_INDICATOR_FRAME_WIDTH, 0);
		m_IndicatorItem[i].m_pFrame = NTL_NEW gui::CDialog(rect, m_ppnIndicatorList, pSurfaceManager);
		m_IndicatorItem[i].m_pFrame->SetClippingMode(TRUE);

		m_IndicatorItem[i].m_Background.SetType(CWindowby3::WT_HORIZONTAL);
		m_IndicatorItem[i].m_Background.SetSurface(0, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("QuestIndicator.srf", "srfItemBackTop"));
		m_IndicatorItem[i].m_Background.SetSurface(1, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("QuestIndicator.srf", "srfItemBackCenter"));
		m_IndicatorItem[i].m_Background.SetSurface(2, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("QuestIndicator.srf", "srfItemBackBottom"));
		m_IndicatorItem[i].m_Background.SetAlpha(150);

		rect.SetRect(30, 6, 200, 29);
		m_IndicatorItem[i].m_pTitle = NTL_NEW gui::CButton(rect, m_IndicatorItem[i].m_pFrame, pSurfaceManager);
		m_IndicatorItem[i].m_pTitle->SetTextFont("detail", QUEST_INDICATOR_QUESTTITLE_FONTSIZE, 0);
		m_IndicatorItem[i].m_pTitle->SetTextDownCoordDiff(0, 1);
		m_IndicatorItem[i].m_pTitle->SetTextStyle(COMP_TEXT_LEFT);
		m_IndicatorItem[i].m_pTitle->SetTextCoord(QUEST_INDICATOR_QUESTTITLE_TEXTPOSX, 0);
		m_IndicatorItem[i].m_pTitle->AddSurfaceFocus(pSurfaceManager->GetSurface("QuestIndicator.srf", "srfbtnTitleFocus"));

		m_IndicatorItem[i].m_pTitle->SetTextEffectMode(TE_OUTLINE);
		m_IndicatorItem[i].m_pTitle->SetTextUpColor(SIDE_DIALOG_FONT_YELLOW_COLOR);
		m_IndicatorItem[i].m_pTitle->SetTextDownColor(SIDE_DIALOG_FONT_YELLOW_COLOR);
		m_IndicatorItem[i].m_pTitle->SetTextFocusColor(SIDE_DIALOG_FONT_YELLOW_COLOR);
		m_IndicatorItem[i].m_pTitle->ApplyText();
		m_IndicatorItem[i].m_pTitle->SetClippingMode(TRUE);

		rect.SetRect(239, 1, 257, 19);
		m_IndicatorItem[i].m_pbtnExpend = NTL_NEW gui::CButton(rect, m_IndicatorItem[i].m_pFrame, pSurfaceManager);
		m_IndicatorItem[i].m_pbtnExpend->AddSurfaceFocus(pSurfaceManager->GetSurface("QuestIndicator.srf", "srfExpendBtnFoc"));
		m_IndicatorItem[i].m_pbtnExpend->AddSurfaceUp(pSurfaceManager->GetSurface("QuestIndicator.srf", "srfExpendBtnUp"));
		m_IndicatorItem[i].m_pbtnExpend->AddSurfaceDown(pSurfaceManager->GetSurface("QuestIndicator.srf", "srfExpendBtnDown"));
		m_IndicatorItem[i].m_pbtnExpend->SetToolTip(GetDisplayStringManager()->GetString("DST_QUESTINDICATOR_EXTEND"));
		m_IndicatorItem[i].m_pbtnExpend->SetClippingMode(TRUE);

		rect.SetRectWH(10, QUEST_INDICATOR_QUESTSTATE_POSY, 50, 50);
		m_IndicatorItem[i].m_pStateSquare = NTL_NEW gui::CPanel(rect, m_IndicatorItem[i].m_pFrame, GetNtlGuiManager()->GetSurfaceManager());
		m_IndicatorItem[i].m_pState = NTL_NEW gui::CPanel(rect, m_IndicatorItem[i].m_pFrame, GetNtlGuiManager()->GetSurfaceManager());
		m_IndicatorItem[i].m_pArrow = NTL_NEW gui::CPanel(rect, m_IndicatorItem[i].m_pFrame, GetNtlGuiManager()->GetSurfaceManager());
		m_IndicatorItem[i].m_eGrade = NTL_NEW gui::CPanel(rect, m_IndicatorItem[i].m_pFrame, GetNtlGuiManager()->GetSurfaceManager());
		m_IndicatorItem[i].m_eGrade->SetClippingMode(TRUE);
		m_IndicatorItem[i].m_pState->SetClippingMode(TRUE);
		m_IndicatorItem[i].m_pStateSquare->SetClippingMode(TRUE);
		m_IndicatorItem[i].m_pArrow->SetClippingMode(TRUE);

		m_IndicatorItem[i].m_pDistanceLabel = NTL_NEW gui::CStaticBox(rect, m_IndicatorItem[i].m_pFrame, pSurfaceManager, COMP_TEXT_LEFT);
		m_IndicatorItem[i].m_pDistanceLabel->CreateFontStd("detail", DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
		m_IndicatorItem[i].m_pDistanceLabel->Enable(true);
		m_IndicatorItem[i].m_pDistanceLabel->SetClippingMode(TRUE);

		rect.SetRectWH(QUEST_INDICATOR_QUESTTIME_POSX, 0, QUEST_INDICATOR_QUESTTIME_WIDTH, QUEST_INDICATOR_QUESTTIME_HEIGHT);
		m_IndicatorItem[i].m_pTime = NTL_NEW gui::CStaticBox(rect, m_IndicatorItem[i].m_pFrame, pSurfaceManager, COMP_TEXT_LEFT);
		m_IndicatorItem[i].m_pTime->CreateFontStd("detail", DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
		m_IndicatorItem[i].m_pTime->Enable(false);
		m_IndicatorItem[i].m_pTime->SetClippingMode(TRUE);

		for (int j = 0; j < QUEST_MAX_GOAL_COUNT; j++)
		{
			rect.SetRectWH(QUEST_INDICATOR_QUESTINFO_POSX, 0, QUEST_INDICATOR_QUESTINFO_WIDTH, QUEST_INDICATOR_QUESTINFO_HEIGHT);
			m_IndicatorItem[i].m_pGoalInfo[j] = NTL_NEW gui::CStaticBox(rect, m_IndicatorItem[i].m_pFrame, pSurfaceManager, COMP_TEXT_LEFT);
			m_IndicatorItem[i].m_pGoalInfo[j]->CreateFontStd("detail", DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
			m_IndicatorItem[i].m_pGoalInfo[j]->Enable(false);
			m_IndicatorItem[i].m_pGoalInfo[j]->SetClippingMode(TRUE);

			rect.SetRectWH(0, 0, QUEST_INDICATOR_QUESTRESULT_WIDTH, QUEST_INDICATOR_QUESTRESULT_HEIGHT);
			m_IndicatorItem[i].m_pGoalResult[j] = NTL_NEW gui::CStaticBox(rect, m_IndicatorItem[i].m_pFrame, pSurfaceManager, COMP_TEXT_LEFT);
			m_IndicatorItem[i].m_pGoalResult[j]->CreateFontStd("detail", DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
			m_IndicatorItem[i].m_pGoalResult[j]->Enable(false);
			m_IndicatorItem[i].m_pGoalResult[j]->SetClippingMode(TRUE);
		}

		m_IndicatorItem[i].m_pFrame->Show(false);

		m_IndicatorItem[i].m_nFrameHeight = 0;

		m_IndicatorItem[i].m_slotClickTitle = m_IndicatorItem[i].m_pTitle->SigClicked().Connect(&m_IndicatorItem[i], &stINDICATOR_ITEM::OnClickTitle);
		m_IndicatorItem[i].m_slotMouseEnter = m_IndicatorItem[i].m_pTitle->SigMouseEnter().Connect(&m_IndicatorItem[i], &stINDICATOR_ITEM::OnMouseEnterTitle);
		m_IndicatorItem[i].m_slotMouseLeave = m_IndicatorItem[i].m_pTitle->SigMouseLeave().Connect(&m_IndicatorItem[i], &stINDICATOR_ITEM::OnMouseLeaveTitle);
		m_IndicatorItem[i].m_slotClickExpend = m_IndicatorItem[i].m_pbtnExpend->SigClicked().Connect(&m_IndicatorItem[i], &stINDICATOR_ITEM::OnClickExpend);
		m_IndicatorItem[i].m_slotReleaseExpend = m_IndicatorItem[i].m_pbtnExpend->SigReleased().Connect(this, &CQuestIndicatorGui::OnReleaseExpend);

		m_IndicatorItem[i].m_bQuestInfoShown = true;
	}

	m_slotPaint = m_ppnIndicatorList->SigPaint().Connect(this, &CQuestIndicatorGui::OnPaint);
	m_slotMouseWheel = GetNtlGuiManager()->GetGuiManager()->SigCaptureWheelMove().Connect(this, &CQuestIndicatorGui::OnMouseWheel);

	Show(false);
	m_pQuestGui = pQuestGui;

	NTL_RETURN(TRUE);
}

VOID CQuestIndicatorGui::Destroy(VOID)
{
	DeleteAllData();

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();

	GetNtlGuiManager()->RemoveUpdateFunc(this);

}

RwInt32 CQuestIndicatorGui::SwitchDialog(bool bOpen)
{
	if (bOpen)
	{
		CNtlWorldConcept* pWorldConcept = GetNtlWorldConcept();

		if (m_listData.size() > 0)
		{
			if (pWorldConcept->IsActivePlayConcept(WORLD_PLAY_TIME_MACHINE))
			{
				if (pWorldConcept->GetWorldConceptController(WORLD_PLAY_TIME_MACHINE)->GetState() == WORLD_STATE_EXIT)
					Show(true);
				else
					return -1;
			}
			else
			{
				Show(true);
			}
		}
		else
			return -1;
	}
	else
	{
		Show(false);
	}

	return 1;
}

RwBool CQuestIndicatorGui::IsIndicatedQuest(RwUInt32 nID)
{
	LIST_DATA::iterator it;

	for (it = m_listData.begin(); it != m_listData.end(); ++it)
	{
		if ((*it)->sTSKey.tID == nID)
			return TRUE;
	}

	return FALSE;
}

VOID CQuestIndicatorGui::HandleEvents(RWS::CMsg& msg)
{

	if (msg.Id == g_EventUpdateQuestProgressInfo_Nfy)
	{
		SNtlEventUpdateQuestProgressInfo_Nfy* pData = reinterpret_cast<SNtlEventUpdateQuestProgressInfo_Nfy*>(msg.pData);
		UpdateData(pData);
		Refresh();

		// this hides the quest list whenever a quest progressed
		//int nSize = m_listData.size();
		//for (int i = 0; i < nSize; i++)
		//{
		//	SNtlEventShowIndicator_Nfy* pBackData = NULL;
		//	pBackData = m_listData.back();
		//	m_pQuestGui->GetQuestListGui()->CancelIndicator( pBackData->sTSKey, TRUE );	// ¸Þ½ÃÁö¸¦ ¶ç¿ìÁö ¾Ê±â À§ÇØ AutoÇÃ·¡±× ÀÓÀÇ TRUE¼¼ÆÃ.
		//}
	}
	else if (msg.Id == g_EventUpdateQuestState_Nfy)
	{
		SNtlEventUpdateQuestState_Nfy* pData = reinterpret_cast<SNtlEventUpdateQuestState_Nfy*>(msg.pData);

		UpdateState(pData->sTSKey, pData->uiQuestState, pData->indicatorTargetPos, pData->indicatorTargetWorldID, pData->indicatorNpcTblidx);
	}
	else if (msg.Id == g_EventShowIndicator_Nfy)
	{
		SNtlEventShowIndicator_Nfy* pData = reinterpret_cast<SNtlEventShowIndicator_Nfy*>(msg.pData);

		// disabled by daneos
		//if( IsAvaliableHeight( pData ) )
		{
			SNtlEventShowIndicator_Nfy* pIndicatorData;
			FindData(pData->sTSKey.tID, &pIndicatorData);

			if (pIndicatorData)
				memcpy(pIndicatorData, pData, sizeof(SNtlEventShowIndicator_Nfy));
			else
				CreateData(pData);

			GetSideDialogManager()->OpenDialog(SDIALOG_QUEST_INDICATOR, TRUE);
			Refresh();
		}
		//else
		//{
		//	m_pQuestGui->GetQuestListGui()->CancelIndicator( pData->sTSKey, pData->bAuto );
		//}
	}
	else if (msg.Id == g_EventHideIndicator)
	{
		SNtlEventHideIndicator* pData = reinterpret_cast<SNtlEventHideIndicator*>(msg.pData);

		DeleteData(pData->sTSKey.tID);
		Refresh();

		if (m_listData.size() == 0)
			GetSideDialogManager()->CloseDialog(SDIALOG_QUEST_INDICATOR, TRUE);
	}
	else if (msg.Id == g_EventUnregQuest_Nfy)
	{
		SNtlEventUnregQuest_Nfy* pData = reinterpret_cast<SNtlEventUnregQuest_Nfy*>(msg.pData);

		DeleteData(pData->sTSKey.tID);
		Refresh();

		if (m_listData.size() == 0)
			GetSideDialogManager()->CloseDialog(SDIALOG_QUEST_INDICATOR, TRUE);
	}
	else if (msg.Id == g_EventChangeWorldConceptState)
	{
		SNtlEventWorldConceptState* pData = reinterpret_cast<SNtlEventWorldConceptState*>(msg.pData);

		if (pData->eWorldConcept == WORLD_PLAY_TIME_MACHINE)
		{
			if (pData->uiState == WORLD_STATE_EXIT)
				GetSideDialogManager()->OpenDialog(SDIALOG_QUEST_INDICATOR, FALSE);
		}
	}
	/*else if (msg.Id == g_EventWorldChange) {
		SNtlEventWorldChange* pWorldChange = reinterpret_cast<SNtlEventWorldChange*>(msg.pData);
		pWorldChange->bWorldChange;
	}*/
	//else if( msg.Id == g_EventRBMatch )
	//{
	//	SNtlEventRBMatch* pData = reinterpret_cast<SNtlEventRBMatch*>( msg.pData );

	//	if( pData->type == SNtlEventRBMatch::MATCH_START )
	//		GetSideDialogManager()->CloseDialog( SDIALOG_QUEST_INDICATOR, FALSE );
	//}
	else if (msg.Id == g_EventTSRemovingTMQQuest_Nfy)
	{
		SNtlEventTSRemovingTMQQuest_Nfy* pData = reinterpret_cast<SNtlEventTSRemovingTMQQuest_Nfy*>(msg.pData);
		DeleteData(pData->tRmvTSId);
		Refresh();
	}
	else if (msg.Id == g_EventResize)
	{
		CRectangle rect = GetNtlGuiManager()->GetGuiManager()->GetScreenRect();
		m_pThis->SetPosition(rect.right - (QUEST_INDICATOR_FRAME_WIDTH + QUEST_INDICATOR_QUESTSTATE_WIDTH), m_pThis->GetPosition().top);
		Refresh();
	}
	CalculatePosition(m_nScrollPos);
}

VOID CQuestIndicatorGui::Refresh(VOID)
{
	RwInt32 i;
	LIST_DATA::iterator it;

	for (i = 0; i < eMAX_CAN_PROGRESS_QUEST_NUM; i++)
	{
		m_IndicatorItem[i].m_pFrame->Show(false);
	}

	for (i = 0, it = m_listData.begin(); it != m_listData.end(); ++it, ++i)
	{
		SNtlEventShowIndicator_Nfy* pIndicatorData = (*it);
		DrawData(i, pIndicatorData);
	}

	CalculatePosition(m_nScrollPos);
	CalculatePosition(m_nScrollPos);
}

VOID CQuestIndicatorGui::DrawData(RwInt32 nIdx, SNtlEventShowIndicator_Nfy* pIndicatorData)
{
	RwInt32 j;
	RwUInt32 color;

	m_IndicatorItem[nIdx].m_nFrameHeight = 0;
	m_IndicatorItem[nIdx].m_pFrame->Show(true);

	m_IndicatorItem[nIdx].m_pTitle->GetTextComponent()->Clear();
	m_IndicatorItem[nIdx].m_pTime->Clear();

	for (j = 0; j < QUEST_MAX_GOAL_COUNT; ++j)
	{
		m_IndicatorItem[nIdx].m_pGoalInfo[j]->Clear();
		m_IndicatorItem[nIdx].m_pGoalResult[j]->Clear();
		m_IndicatorItem[nIdx].m_pGoalInfo[j]->Show(false);
		m_IndicatorItem[nIdx].m_pGoalResult[j]->Show(false);
	}

	// DataÀÔ·Â
	m_IndicatorItem[nIdx].m_TSKey = pIndicatorData->sTSKey;
	m_IndicatorItem[nIdx].m_uiQuestTitle = pIndicatorData->uiQuestTitle;
	m_IndicatorItem[nIdx].m_uiQuestGoal = pIndicatorData->uiQuestGoal;


	m_IndicatorItem[nIdx].m_indicatorTargetPos = pIndicatorData->indicatorTargetPos;
	m_IndicatorItem[nIdx].m_indicatorTargetWorldID = pIndicatorData->indicatorTargetWorldID;
	m_IndicatorItem[nIdx].m_indicatorNpcTblidx = pIndicatorData->indicatorNpcTblidx;

	//set grade
	m_IndicatorItem[nIdx].m_eGrade->GetSurface()->clear();
	switch (pIndicatorData->m_eGradeType) {
	case eGRADE_TYPE_VERY_EASY:
	case eGRADE_TYPE_EASY:
		m_IndicatorItem[nIdx].m_eGrade->AddSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("QuestIndicator.srf", "srfQuestGradeEasy"));
		break;
	case eGRADE_TYPE_NORMAL:
		m_IndicatorItem[nIdx].m_eGrade->AddSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("QuestIndicator.srf", "srfQuestGradeNormal"));
		break;
	case eGRADE_TYPE_HARD:
	case eGRADE_TYPE_VERY_HARD:
	case eGRADE_TYPE_IMPOSSIBLE:
		m_IndicatorItem[nIdx].m_eGrade->AddSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("QuestIndicator.srf", "srfQuestGradeHard"));
		break;
	}
	m_IndicatorItem[nIdx].m_eGrade->SetPosition(2, 7);


	// Á¦¸ñ
	CQuestTextDataTable* pTable = API_GetTableContainer()->GetQuestTextDataTable();
	sQUEST_TEXT_DATA_TBLDAT* pData = reinterpret_cast<sQUEST_TEXT_DATA_TBLDAT*>(pTable->FindData(pIndicatorData->uiQuestTitle));
	std::wstring wstrTitle;

	if (pData)
		wstrTitle = gui::GetHtmlFromMemoryString(pData->wstrText.c_str(), pData->wstrText.size());
	else
	{
		if (pIndicatorData->uiQuestTitle != INVALID_TBLIDX)
		{
			RwChar buf[256];
			sprintf_s(buf, 256, "%u QuestTextData is Missing", pIndicatorData->uiQuestTitle);
		}
	}

	m_IndicatorItem[nIdx].m_pTitle->SetText(wstrTitle.c_str());
	m_IndicatorItem[nIdx].m_pTitle->SetPosition(30, 5);
	m_IndicatorItem[nIdx].m_nFrameHeight += m_IndicatorItem[nIdx].m_pTitle->GetHeight();

	bool infoExists = false;

	UpdateState(pIndicatorData->sTSKey, pIndicatorData->uiQState, pIndicatorData->indicatorTargetPos, pIndicatorData->indicatorTargetWorldID, pIndicatorData->indicatorNpcTblidx);

	if (pIndicatorData->eEvtInfoType == eSTOC_EVT_DATA_TYPE_MOB_KILL_CNT)
	{
		CMobTable* pMobTable = API_GetTableContainer()->GetMobTable();
		CTextTable* pMobText = API_GetTableContainer()->GetTextAllTable()->GetMobTbl();

		for (j = 0; j < uSTOC_EVT_DATA::MAX_MOB_KILL; ++j)
		{
			RwUInt32 nTblIdx = pMobTable->FindTblidxByGroup(pIndicatorData->uEvtInfoData.sMobKillCnt[j].uiMobIdx);
			sMOB_TBLDAT* pMobData = reinterpret_cast<sMOB_TBLDAT*>(pMobTable->FindData(nTblIdx));
			if (!pMobData)
				continue;

			UpdateResultItem(pMobText->GetText(pMobData->Name).c_str(), pIndicatorData->uEvtInfoData.sMobKillCnt[j].nCurMobCnt, pIndicatorData->uEvtInfoData.sMobKillCnt[j].nMobCnt, nIdx, j);
			infoExists = true;
		}
	}
	else if (pIndicatorData->eEvtInfoType == eSTOC_EVT_DATA_TYPE_MOB_KILL_ITEM_CNT)
	{
		CQuestItemTable* pItemTable = API_GetTableContainer()->GetQuestItemTable();
		CTextTable* pQuestItemText = API_GetTableContainer()->GetTextAllTable()->GetQuestItemTbl();

		for (j = 0; j < uSTOC_EVT_DATA::MAX_MOB_KILL_ITEM; ++j)
		{
			sQUESTITEM_TBLDAT* pItemData = reinterpret_cast<sQUESTITEM_TBLDAT*>(pItemTable->FindData(pIndicatorData->uEvtInfoData.sMobKillItemCnt[j].uiMobLIIdx));

			if (!pItemData)
				continue;

			UpdateResultItem(pQuestItemText->GetText(pItemData->ItemName).c_str(), pIndicatorData->uEvtInfoData.sMobKillItemCnt[j].nCurMobLICnt, pIndicatorData->uEvtInfoData.sMobKillItemCnt[j].nMobLICnt, nIdx, j);
			infoExists = true;
		}
	}
	else if (pIndicatorData->eEvtInfoType == eSTOC_EVT_DATA_TYPE_DELIVERY_ITEM)
	{
		CQuestItemTable* pItemTable = API_GetTableContainer()->GetQuestItemTable();
		CTextTable* pQuestItemText = API_GetTableContainer()->GetTextAllTable()->GetQuestItemTbl();

		for (j = 0; j < uSTOC_EVT_DATA::MAX_DELIVERY_ITEM; ++j)
		{
			sQUESTITEM_TBLDAT* pItemData = reinterpret_cast<sQUESTITEM_TBLDAT*>(pItemTable->FindData(pIndicatorData->uEvtInfoData.sDeliveryItemCnt[j].uiItemIdx));

			if (!pItemData)
				continue;

			UpdateResultItem(pQuestItemText->GetText(pItemData->ItemName).c_str(), pIndicatorData->uEvtInfoData.sDeliveryItemCnt[j].nCurItemCnt, pIndicatorData->uEvtInfoData.sDeliveryItemCnt[j].nItemCnt, nIdx, j);
			infoExists = true;
		}
	}
	else if (pIndicatorData->eEvtInfoType == eSTOC_EVT_DATA_TYPE_OBJECT_ITEM)
	{
		CQuestItemTable* pItemTable = API_GetTableContainer()->GetQuestItemTable();
		CTextTable* pQuestItemText = API_GetTableContainer()->GetTextAllTable()->GetQuestItemTbl();

		for (j = 0; j < uSTOC_EVT_DATA::MAX_OBJECT_ITEM; ++j)
		{
			sQUESTITEM_TBLDAT* pItemData = reinterpret_cast<sQUESTITEM_TBLDAT*>(pItemTable->FindData(pIndicatorData->uEvtInfoData.sObjectItemCnt[j].uiItemIdx));

			if (!pItemData)
				continue;

			UpdateResultItem(pQuestItemText->GetText(pItemData->ItemName).c_str(), pIndicatorData->uEvtInfoData.sObjectItemCnt[j].nCurItemCnt, pIndicatorData->uEvtInfoData.sObjectItemCnt[j].nItemCnt, nIdx, j);
			infoExists = true;
		}
	}
	else if (pIndicatorData->eEvtInfoType == eSTOC_EVT_DATA_TYPE_PUBLIC_MOB_ITEM_CNT)
	{
		CQuestItemTable* pItemTable = API_GetTableContainer()->GetQuestItemTable();
		CTextTable* pQuestItemText = API_GetTableContainer()->GetTextAllTable()->GetQuestItemTbl();

		for (j = 0; j < uSTOC_EVT_DATA::MAX_PUBLIC_MOB_ITEM; ++j)
		{
			sQUESTITEM_TBLDAT* pItemData = reinterpret_cast<sQUESTITEM_TBLDAT*>(pItemTable->FindData(pIndicatorData->uEvtInfoData.sPublicMobItemCnt[j].uiItemIdx));

			if (!pItemData)
				continue;

			UpdateResultItem(pQuestItemText->GetText(pItemData->ItemName).c_str(), pIndicatorData->uEvtInfoData.sPublicMobItemCnt[j].nCurItemCnt, pIndicatorData->uEvtInfoData.sPublicMobItemCnt[j].nItemCnt, nIdx, j);
			infoExists = true;
		}
	}
	else if (pIndicatorData->eEvtInfoType == eSTOC_EVT_DATA_TYPE_CUSTOM_EVT_CNT)
	{
		CQuestTextDataTable* pQuestTextTable = API_GetTableContainer()->GetQuestTextDataTable();

		for (RwInt32 i = 0; i < uSTOC_EVT_DATA::MAX_CUSTOM_EVT_CNT; ++i)
		{
			sQUEST_TEXT_DATA_TBLDAT* pQuestTextData = reinterpret_cast<sQUEST_TEXT_DATA_TBLDAT*>(pQuestTextTable->FindData(pIndicatorData->uEvtInfoData.sCustomEvtCnt[i].uiQTextTblIdx));

			if (!pQuestTextData)
				continue;

			std::wstring strQuestText = gui::GetHtmlFromMemoryString(pQuestTextData->wstrText.c_str(), pQuestTextData->wstrText.size());
			UpdateResultItem(strQuestText.c_str(), pIndicatorData->uEvtInfoData.sCustomEvtCnt[i].nCurCnt, pIndicatorData->uEvtInfoData.sCustomEvtCnt[i].nMaxCnt, nIdx, i);
			infoExists = true;
		}
	}
	else if (pIndicatorData->eEvtInfoType == eSTOC_EVT_DATA_TYPE_VISIT)
	{
		CQuestTextDataTable* pQuestTextTable = API_GetTableContainer()->GetQuestTextDataTable();

		for (RwInt32 i = 0; i < uSTOC_EVT_DATA::MAX_VISIT_EVT; ++i)
		{
			sQUEST_TEXT_DATA_TBLDAT* pQuestTextData = reinterpret_cast<sQUEST_TEXT_DATA_TBLDAT*>(pQuestTextTable->FindData(pIndicatorData->uEvtInfoData.sVisitEvt[i].uiIndicatorQText));

			if (!pQuestTextData)
				continue;

			std::wstring wstrText = gui::GetHtmlFromMemoryString(pQuestTextData->wstrText.c_str(), pQuestTextData->wstrText.size());
			UpdateResultItem(wstrText.c_str(), pIndicatorData->uEvtInfoData.sVisitEvt[i].bCompleted, nIdx, i);
			infoExists = true;
		}
	}

	if (!(infoExists && m_IndicatorItem[nIdx].m_bQuestInfoShown)) {
		m_IndicatorItem[nIdx].m_pTitle->SetPosition(30, 15);
		if (!infoExists) {
			m_IndicatorItem[nIdx].m_nFrameHeight += m_IndicatorItem[nIdx].m_pTitle->GetHeight();
		}
	}

	// ½Ã°£.
	if (pIndicatorData->uiTimeLimit != INVALID_DWORD)
	{
		RwUInt32 uiTime;
		RwUInt32 uiMinute;
		RwUInt32 uiSecond;

		if (pIndicatorData->uiTimeLimit <= 0)
		{
			uiMinute = 0;
			uiSecond = 0;
			color = QUEST_INDICATOR_COLOR_GRAY;
		}
		else
		{
			uiTime = pIndicatorData->uiTimeLimit / 1000;
			uiMinute = uiTime / 60;
			uiSecond = uiTime % 60;

			if (pIndicatorData->uiTimeLimit <= 10)
				color = QUEST_INDICATOR_COLOR_RED;
			else
				color = QUEST_INDICATOR_COLOR_WHITE;
		}

		m_IndicatorItem[nIdx].m_pTime->SetTextColor(color);
		m_IndicatorItem[nIdx].m_pTime->Format(GetDisplayStringManager()->GetString("DST_QUEST_REMAIN_TIME"), uiMinute, uiSecond);
		m_IndicatorItem[nIdx].m_pTime->SetPosition(m_IndicatorItem[nIdx].m_pTime->GetPosition().left + 20, m_IndicatorItem[nIdx].m_nFrameHeight);
		m_IndicatorItem[nIdx].m_nFrameHeight += m_IndicatorItem[nIdx].m_pTime->GetHeight();
	}

	m_IndicatorItem[nIdx].m_nFrameHeight += m_IndicatorItem[nIdx].m_pDistanceLabel->GetHeight();
}

VOID CQuestIndicatorGui::UpdateData(SNtlEventUpdateQuestProgressInfo_Nfy* pData)
{
	SNtlEventShowIndicator_Nfy* pIndicatorData;
	RwInt32 nIdx = FindData(pData->sTSKey.tID, &pIndicatorData);

	if (pIndicatorData)
	{
		pIndicatorData->eEvtInfoType = pData->eEvtInfoType;
		pIndicatorData->sTSKey = pData->sTSKey;
		pIndicatorData->uEvtInfoData = pData->uEvtInfoData;
		pIndicatorData->uiTimeLimit = pData->uiTimeLimit;
		DrawData(nIdx, pIndicatorData);
	}
}

VOID CQuestIndicatorGui::UpdateState(sTS_KEY& sTSKey, RwUInt32 uiQuestState, RwV3d m_indicatorTargetPos, RwUInt32 indicatorTargetWorldID, RwUInt32 indicatorNpcTblidx)
{
	SNtlEventShowIndicator_Nfy* pIndicatorData;
	RwInt32 nIdx = FindData(sTSKey.tID, &pIndicatorData);
	RwInt32 activeWorldID = Logic_GetActiveWorldId();

	if (pIndicatorData)
	{
		pIndicatorData->uiQState = uiQuestState;
		m_IndicatorItem[nIdx].m_uiQuestState = uiQuestState;

		m_IndicatorItem[nIdx].m_pStateSquare->GetSurface()->clear();
		m_IndicatorItem[nIdx].m_pState->GetSurface()->clear();
		m_IndicatorItem[nIdx].m_pState->SetPosition(20, 20);

		m_IndicatorItem[nIdx].m_pArrow->GetSurface()->clear();
		m_IndicatorItem[nIdx].m_pArrow->AddSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("QuestIndicator.srf", "srfQuestDirArrow"));
		m_IndicatorItem[nIdx].m_pArrow->SetPosition(18, 18);

		m_IndicatorItem[nIdx].m_pDistanceLabel->SetPosition(15, 40);
		m_IndicatorItem[nIdx].m_pDistanceLabel->SetSize(45, 15);

		CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();


		m_IndicatorItem[nIdx].m_indicatorTargetPos = m_indicatorTargetPos;
		m_IndicatorItem[nIdx].m_indicatorNpcTblidx = indicatorNpcTblidx;
		m_IndicatorItem[nIdx].m_indicatorTargetWorldID = indicatorTargetWorldID;

		if (uiQuestState & eTS_PROG_STATE_PROGRESS_FAILED)
		{
			m_IndicatorItem[nIdx].m_pStateSquare->AddSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("QuestIndicator.srf", "srfQuestProgressStateBG"));
			m_IndicatorItem[nIdx].m_pStateSquare->SetToolTip(GetDisplayStringManager()->GetString("DST_QUESTLIST_TIP_FAILD_QUEST"));

			m_IndicatorItem[nIdx].m_pState->AddSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("QuestList.srf", "srfQuestStateFail"));
			m_IndicatorItem[nIdx].m_pState->SetToolTip(GetDisplayStringManager()->GetString("DST_QUESTLIST_TIP_FAILD_QUEST"));
			m_IndicatorItem[nIdx].m_pArrow->Show(false);
			m_IndicatorItem[nIdx].m_pDistanceLabel->Show(false);

			m_IndicatorItem[nIdx].m_pState->SetPosition(18, 18);

			m_IndicatorItem[nIdx].m_pState->Show(true);
			m_IndicatorItem[nIdx].m_indicatorTargetWorldID = 0xffffffff;
			m_IndicatorItem[nIdx].m_indicatorNpcTblidx = 0xffffffff;
		}
		else if (uiQuestState & eTS_PROG_STATE_ERROR_BLOCK)
		{
			m_IndicatorItem[nIdx].m_pStateSquare->AddSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("QuestIndicator.srf", "srfQuestProgressStateBG"));
			m_IndicatorItem[nIdx].m_pStateSquare->SetToolTip(GetDisplayStringManager()->GetString("DST_QUESTLIST_TIP_ERROR_BLOCK"));

			m_IndicatorItem[nIdx].m_pState->AddSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("QuestList.srf", "srfQuestStateFail"));
			m_IndicatorItem[nIdx].m_pState->SetToolTip(GetDisplayStringManager()->GetString("DST_QUESTLIST_TIP_ERROR_BLOCK"));
			m_IndicatorItem[nIdx].m_pArrow->Show(false);
			m_IndicatorItem[nIdx].m_pDistanceLabel->Show(false);

			m_IndicatorItem[nIdx].m_pState->Show(true);
			m_IndicatorItem[nIdx].m_indicatorTargetWorldID = 0xffffffff;
			m_IndicatorItem[nIdx].m_indicatorNpcTblidx = 0xffffffff;
		}
		else if (uiQuestState & eTS_PROG_STATE_CLEARED)
		{
			m_IndicatorItem[nIdx].m_pStateSquare->AddSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("QuestIndicator.srf", "srfQuestRewardStateBG"));
			m_IndicatorItem[nIdx].m_pStateSquare->SetToolTip(GetDisplayStringManager()->GetString("DST_QUESTLIST_TIP_CLEARD_QUEST"));

			m_IndicatorItem[nIdx].m_pState->AddSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("QuestList.srf", "srfQuestStateReward"));
			m_IndicatorItem[nIdx].m_pState->SetToolTip(GetDisplayStringManager()->GetString("DST_QUESTLIST_TIP_CLEARD_QUEST"));


			if (m_IndicatorItem[nIdx].m_indicatorTargetWorldID != 0xffffffff) {
				CalcDistance(m_IndicatorItem[nIdx].m_pDistanceLabel, pAvatar, m_indicatorTargetPos);
				UpdateDirection(m_IndicatorItem[nIdx].m_pArrow, pAvatar, m_indicatorTargetPos);
				m_IndicatorItem[nIdx].m_pState->Show(false);
			}
			else {
				m_IndicatorItem[nIdx].m_pArrow->Show(false);
				m_IndicatorItem[nIdx].m_pDistanceLabel->Show(false);
				m_IndicatorItem[nIdx].m_pState->Show(true);
			}
		}
		else
		{
			m_IndicatorItem[nIdx].m_pStateSquare->AddSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("QuestIndicator.srf", "srfQuestProgressStateBG"));
			m_IndicatorItem[nIdx].m_pStateSquare->SetToolTip(GetDisplayStringManager()->GetString("DST_QUESTLIST_TIP_PROGRESS_QUEST"));

			m_IndicatorItem[nIdx].m_pState->AddSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("QuestList.srf", "srfQuestStateProgress"));
			m_IndicatorItem[nIdx].m_pState->SetToolTip(GetDisplayStringManager()->GetString("DST_QUESTLIST_TIP_PROGRESS_QUEST"));

			if (m_IndicatorItem[nIdx].m_indicatorTargetWorldID != 0xffffffff) {
				CalcDistance(m_IndicatorItem[nIdx].m_pDistanceLabel, pAvatar, m_indicatorTargetPos);
				UpdateDirection(m_IndicatorItem[nIdx].m_pArrow, pAvatar, m_indicatorTargetPos);
				m_IndicatorItem[nIdx].m_pState->Show(false);
			}
			else {
				m_IndicatorItem[nIdx].m_pArrow->Show(false);
				m_IndicatorItem[nIdx].m_pDistanceLabel->Show(false);
				m_IndicatorItem[nIdx].m_pState->Show(true);
			}
		}
	}
}

VOID CQuestIndicatorGui::Update(RwReal fElapsedTime)
{
	for (int i = 0; i < eMAX_CAN_PROGRESS_QUEST_NUM; i++)
	{
		if (!m_IndicatorItem[i].m_pFrame->IsVisible())
			break;

		CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
		RwInt32 activeWorldID = Logic_GetActiveWorldId();

		if (m_IndicatorItem[i].m_indicatorNpcTblidx != 0xffffffff && m_IndicatorItem[i].m_indicatorTargetWorldID == 0xffffffff) {
			RwV3d* npcPos = GetTSCMain()->FindNPCPosition(activeWorldID, m_IndicatorItem[i].m_indicatorNpcTblidx);
			if (npcPos) {
				m_IndicatorItem[i].m_indicatorTargetPos.x = npcPos->x;
				m_IndicatorItem[i].m_indicatorTargetPos.y = npcPos->y;
				m_IndicatorItem[i].m_indicatorTargetPos.z = npcPos->z;
				m_IndicatorItem[i].m_indicatorTargetWorldID = activeWorldID;
			}
		}
		//only show arrow/distance if the target is in the same world.
		if (activeWorldID == m_IndicatorItem[i].m_indicatorTargetWorldID) {

			m_IndicatorItem[i].m_pState->Show(false);

			m_IndicatorItem[i].m_pArrow->Show(true);
			UpdateDirection(m_IndicatorItem[i].m_pArrow, pAvatar, m_IndicatorItem[i].m_indicatorTargetPos);

			m_IndicatorItem[i].m_pDistanceLabel->Show(true);
			CalcDistance(m_IndicatorItem[i].m_pDistanceLabel, pAvatar, m_IndicatorItem[i].m_indicatorTargetPos);
		}
		else {
			m_IndicatorItem[i].m_pArrow->Show(false);
			m_IndicatorItem[i].m_pDistanceLabel->Show(false);
			m_IndicatorItem[i].m_pState->Show(true);
		}
	}

}

// Ÿ�ٰ��� �Ÿ��� �����Ÿ����� ��������� ����Ʈ�� �����Ѵ�.
VOID CQuestIndicatorGui::CalcDistance(gui::CStaticBox* m_pDistanceLabel, CNtlSobAvatar* pAvatar, RwV3d m_indicatorTargetPos)
{

	RwV2d vDist;

	RwV2d targetPos;
	targetPos.x = m_indicatorTargetPos.x;
	targetPos.y = m_indicatorTargetPos.z;

	RwV2d avatarPos;
	avatarPos.x = pAvatar->GetPosition().x;
	avatarPos.y = pAvatar->GetPosition().z;

	RwV2dSubMacro(&vDist, &targetPos, &avatarPos);
	float dist = RwV2dLength(&vDist);

	bool isMeters = true;
	if (dist > 1000) {
		dist /= 1000;
		isMeters = false;
	}
	char buffer[10];
	snprintf(buffer, sizeof buffer, isMeters ? "%.0f" : "%.1f", dist);

	std::strcat(buffer, isMeters ? "m" : "km");

	m_pDistanceLabel->SetText(buffer);
}

VOID CQuestIndicatorGui::UpdateDirection(gui::CPanel* pArrow, CNtlSobAvatar* pAvatar, RwV3d m_indicatorTargetPos)
{
	RwV3d avatarPosition = pAvatar->GetPosition();
	RwV2d avPos;
	avPos.x = avatarPosition.x;
	avPos.y = avatarPosition.z;

	RwV2d dirPos;
	dirPos.x = pAvatar->GetDirection().x;
	dirPos.y = pAvatar->GetDirection().z;


	RwV2d npcPos;
	npcPos.x = m_indicatorTargetPos.x;
	npcPos.y = m_indicatorTargetPos.z;

	RwV2d v2Dir;
	RwV2dSub(&v2Dir, &npcPos, &avPos);
	float angleNPC = GetAngle(v2Dir, NULL);
	float angleDIR = GetAngle(dirPos, NULL);
	pArrow->SetAngle(angleNPC - angleDIR);
}

RwReal CQuestIndicatorGui::GetAngle(RwV2d v2Dir, RwV2d* pv3OutSqrtDir /* = NULL */)
{
	static RwV2d v2North = { 0.f, 1.f };
	RwV2d v2Temp = v2Dir;
	float fSQRT = sqrt(v2Temp.x * v2Temp.x + v2Temp.y * v2Temp.y);

	v2Temp.x /= fSQRT;
	v2Temp.y /= fSQRT;

	if (pv3OutSqrtDir)
		*pv3OutSqrtDir = v2Temp;

	RwReal fTheta = acos(RwV2dDotProduct(&v2North, &v2Temp));

	if (v2Temp.x >= 0.0f)
		return NTL_RAD2DEG(fTheta);

	return NTL_RAD2DEG(-fTheta);
}




VOID CQuestIndicatorGui::UpdateResultItem(const WCHAR* pInfo, RwInt32 nCurVal, RwInt32 nTargetVal, RwInt32 i, RwInt32 j)
{
	if (nCurVal >= nTargetVal)
	{
		m_IndicatorItem[i].m_pGoalInfo[j]->SetTextColor(QUEST_INDICATOR_COLOR_GRAY);
		m_IndicatorItem[i].m_pGoalResult[j]->SetTextColor(QUEST_INDICATOR_COLOR_GRAY);
	}
	else
	{
		m_IndicatorItem[i].m_pGoalInfo[j]->SetTextColor(QUEST_INDICATOR_COLOR_WHITE);
		m_IndicatorItem[i].m_pGoalResult[j]->SetTextColor(QUEST_INDICATOR_COLOR_WHITE);
	}

	m_IndicatorItem[i].m_pGoalInfo[j]->SetText(pInfo);
	m_IndicatorItem[i].m_pGoalResult[j]->Format(L" : %d / %d", nCurVal, nTargetVal);

	RwInt32 nResultFitWidth = m_IndicatorItem[i].m_pGoalResult[j]->GetFitWidthToString();
	m_IndicatorItem[i].m_pGoalResult[j]->SetWidth(nResultFitWidth);

	RwInt32 nWidth = m_IndicatorItem[i].m_pGoalInfo[j]->GetFitWidthToString();
	if (nWidth > QUEST_INDICATOR_QUESTINFO_WIDTH - nResultFitWidth)
		nWidth = QUEST_INDICATOR_QUESTINFO_WIDTH - nResultFitWidth;
	m_IndicatorItem[i].m_pGoalInfo[j]->SetWidth(nWidth);

	m_IndicatorItem[i].m_pGoalInfo[j]->SetPosition(50, m_IndicatorItem[i].m_nFrameHeight);
	m_IndicatorItem[i].m_pGoalResult[j]->SetPosition(m_IndicatorItem[i].m_pGoalInfo[j]->GetPosition().right, m_IndicatorItem[i].m_nFrameHeight);
	m_IndicatorItem[i].m_nFrameHeight += m_IndicatorItem[i].m_pGoalInfo[j]->GetHeight();

	if (m_IndicatorItem[i].m_bQuestInfoShown) {
		m_IndicatorItem[i].m_pGoalInfo[j]->Show(true);
		m_IndicatorItem[i].m_pGoalResult[j]->Show(true);
	}
	else {
		m_IndicatorItem[i].m_pGoalInfo[j]->Show(false);
		m_IndicatorItem[i].m_pGoalResult[j]->Show(false);
	}
}

VOID CQuestIndicatorGui::UpdateResultItem(const WCHAR* pInfo, RwBool bComplete, RwInt32 i, RwInt32 j)
{

	if (bComplete)
	{
		m_IndicatorItem[i].m_pGoalInfo[j]->SetTextColor(QUEST_INDICATOR_COLOR_GRAY);
		m_IndicatorItem[i].m_pGoalResult[j]->SetTextColor(QUEST_INDICATOR_COLOR_GRAY);
	}
	else
	{
		m_IndicatorItem[i].m_pGoalInfo[j]->SetTextColor(QUEST_INDICATOR_COLOR_WHITE);
		m_IndicatorItem[i].m_pGoalResult[j]->SetTextColor(QUEST_INDICATOR_COLOR_WHITE);
	}

	m_IndicatorItem[i].m_pGoalInfo[j]->SetText(pInfo);

	RwInt32 nResultFitWidth = m_IndicatorItem[i].m_pGoalResult[j]->GetFitWidthToString();
	m_IndicatorItem[i].m_pGoalResult[j]->SetWidth(nResultFitWidth);

	RwInt32 nWidth = m_IndicatorItem[i].m_pGoalInfo[j]->GetFitWidthToString();
	if (nWidth > QUEST_INDICATOR_QUESTINFO_WIDTH - nResultFitWidth)
		nWidth = QUEST_INDICATOR_QUESTINFO_WIDTH - nResultFitWidth;
	m_IndicatorItem[i].m_pGoalInfo[j]->SetWidth(nWidth);

	m_IndicatorItem[i].m_pGoalInfo[j]->SetPosition(m_IndicatorItem[i].m_pGoalInfo[j]->GetPosition().left + 50, m_IndicatorItem[i].m_nFrameHeight);
	m_IndicatorItem[i].m_pGoalResult[j]->SetPosition(m_IndicatorItem[i].m_pGoalInfo[j]->GetPosition().right + 50, m_IndicatorItem[i].m_nFrameHeight);
	m_IndicatorItem[i].m_nFrameHeight += m_IndicatorItem[i].m_pGoalInfo[j]->GetHeight();

	m_IndicatorItem[i].m_pGoalInfo[j]->Show(true);
	m_IndicatorItem[i].m_pGoalResult[j]->Show(true);
}

VOID CQuestIndicatorGui::CalculatePosition(int nOffset)
{
	CRectangle listRect = GetPosition();
	if (listRect.top == 0) {
		listRect.top = 203;
	}
	int nYPos = 1;

	for (int i = 0; i < eMAX_CAN_PROGRESS_QUEST_NUM; i++)
	{
		if (!m_IndicatorItem[i].m_pFrame->IsVisible())
			break;

		m_IndicatorItem[i].m_pFrame->SetHeight(m_IndicatorItem[i].m_nFrameHeight);
		m_IndicatorItem[i].m_Background.SetSize(QUEST_INDICATOR_FRAME_WIDTH, m_IndicatorItem[i].m_nFrameHeight);

		m_IndicatorItem[i].m_pFrame->SetPosition(0, nYPos);

		CRectangle rectPos = m_IndicatorItem[i].m_pFrame->GetPosition();
		CRectangle m_ppnIndicatorListPos = m_ppnIndicatorList->GetPosition();
		m_IndicatorItem[i].m_Background.SetPosition(listRect.left + rectPos.left, listRect.top + rectPos.top + 25 + m_ppnIndicatorListPos.top);
		nYPos += rectPos.GetHeight() + QUEST_INDICATOR_ITEM_MARGIN;
		m_IndicatorItem[i].m_Background.SetClippingRect(m_ppnIndicatorList->GetClippingRect());
	}

	m_nTotalItemHeight = nYPos;


	if (m_nTotalItemHeight > m_ppnClippingArea->GetHeight())
	{
		m_ppnIndicatorList->SetHeight(QUEST_INDICATOR_MAX_HEIGHT);
		m_pscrBar->SetRange(0, (m_nTotalItemHeight - m_ppnClippingArea->GetHeight()));
		m_pscrBar->SetValue(m_nScrollPos);
		m_pscrBar->Show(true);
	}
	else
	{
		m_pscrBar->Show(false);
		m_nScrollPos = 0;
		CRectangle rtPos = m_ppnIndicatorList->GetPosition();
		m_ppnIndicatorList->SetPosition(rtPos.left, -m_nScrollPos);
	}
}

RwBool	CQuestIndicatorGui::IsAvaliableHeight(SNtlEventShowIndicator_Nfy* pData)
{
	RwInt32 nItemHeight = QUEST_INDICATOR_QUESTTITLE_HEIGHT + QUEST_INDICATOR_ITEM_MARGIN;

	if (pData->eEvtInfoType == eSTOC_EVT_DATA_TYPE_MOB_KILL_CNT)
	{
		for (RwInt32 i = 0; i < uSTOC_EVT_DATA::MAX_MOB_KILL; ++i)
		{
			if (pData->uEvtInfoData.sMobKillCnt[i].uiMobIdx == INVALID_TBLIDX)
				break;

			nItemHeight += QUEST_INDICATOR_QUESTINFO_HEIGHT;
		}
	}
	else if (pData->eEvtInfoType == eSTOC_EVT_DATA_TYPE_MOB_KILL_ITEM_CNT)
	{
		for (RwInt32 i = 0; i < uSTOC_EVT_DATA::MAX_MOB_KILL_ITEM; ++i)
		{
			if (pData->uEvtInfoData.sMobKillItemCnt[i].uiMobLIIdx == INVALID_TBLIDX)
				break;

			nItemHeight += QUEST_INDICATOR_QUESTINFO_HEIGHT;
		}
	}
	else if (pData->eEvtInfoType == eSTOC_EVT_DATA_TYPE_DELIVERY_ITEM)
	{
		for (RwInt32 i = 0; i < uSTOC_EVT_DATA::MAX_DELIVERY_ITEM; ++i)
		{
			if (pData->uEvtInfoData.sDeliveryItemCnt[i].uiItemIdx == INVALID_TBLIDX)
				break;

			nItemHeight += QUEST_INDICATOR_QUESTINFO_HEIGHT;
		}
	}
	else if (pData->eEvtInfoType == eSTOC_EVT_DATA_TYPE_OBJECT_ITEM)
	{
		for (RwInt32 i = 0; i < uSTOC_EVT_DATA::MAX_OBJECT_ITEM; ++i)
		{
			if (pData->uEvtInfoData.sObjectItemCnt[i].uiItemIdx == INVALID_TBLIDX)
				break;

			nItemHeight += QUEST_INDICATOR_QUESTINFO_HEIGHT;
		}
	}
	else if (pData->eEvtInfoType == eSTOC_EVT_DATA_TYPE_PUBLIC_MOB_ITEM_CNT)
	{
		for (RwInt32 i = 0; i < uSTOC_EVT_DATA::MAX_PUBLIC_MOB_ITEM; ++i)
		{
			if (pData->uEvtInfoData.sPublicMobItemCnt[i].uiItemIdx == INVALID_TBLIDX)
				break;

			nItemHeight += QUEST_INDICATOR_QUESTINFO_HEIGHT;
		}
	}
	else if (pData->eEvtInfoType == eSTOC_EVT_DATA_TYPE_CUSTOM_EVT_CNT)
	{
		for (RwInt32 i = 0; i < uSTOC_EVT_DATA::MAX_CUSTOM_EVT_CNT; ++i)
		{
			if (pData->uEvtInfoData.sCustomEvtCnt[i].uiQTextTblIdx == INVALID_TBLIDX)
				break;

			nItemHeight += QUEST_INDICATOR_QUESTINFO_HEIGHT;
		}
	}
	else if (pData->eEvtInfoType == eSTOC_EVT_DATA_TYPE_VISIT)
	{
		for (RwInt32 i = 0; i < uSTOC_EVT_DATA::MAX_VISIT_EVT; ++i)
		{
			if (pData->uEvtInfoData.sVisitEvt[i].uiIndicatorQText == INVALID_TBLIDX)
				break;

			nItemHeight += QUEST_INDICATOR_QUESTINFO_HEIGHT;
		}
	}

	if (pData->uiTimeLimit != INVALID_DWORD)
		nItemHeight += QUEST_INDICATOR_QUESTINFO_HEIGHT;

	//
	if (QUEST_INDICATOR_MAX_HEIGHT - m_nTotalItemHeight >= nItemHeight)
		return TRUE;
	else
		return FALSE;
}

RwBool CQuestIndicatorGui::IsAvaliableHeight(VOID)
{
	if (QUEST_INDICATOR_MAX_HEIGHT - m_nTotalItemHeight >= 0)
		return TRUE;
	else
		return FALSE;
}

RwInt32 CQuestIndicatorGui::FindData(RwUInt32 nID, SNtlEventShowIndicator_Nfy** ppData)
{
	RwInt32 i;
	LIST_DATA::iterator it;

	for (i = 0, it = m_listData.begin(); it != m_listData.end(); ++it, ++i)
	{
		if ((*it)->sTSKey.tID == nID)
		{
			*ppData = (*it);
			return i;
		}
	}

	*ppData = NULL;
	return -1;
}

VOID CQuestIndicatorGui::CreateData(SNtlEventShowIndicator_Nfy* pData)
{
	SNtlEventShowIndicator_Nfy* pIndiData = NTL_NEW SNtlEventShowIndicator_Nfy;
	memcpy(pIndiData, pData, sizeof(SNtlEventShowIndicator_Nfy));

	m_listData.push_back(pIndiData);

	CNtlStorageGroupQuest* pGroup = (CNtlStorageGroupQuest*)GetNtlStorageManager()->GetStorageGroup(eNTL_STORAGE_GROUP_QUEST);
	pGroup->IndicateQuest(pData->sTSKey.tID, TRUE);
}

VOID CQuestIndicatorGui::DeleteData(RwUInt32 nID)
{
	LIST_DATA::iterator it;

	CNtlStorageGroupQuest* pGroupQuest = (CNtlStorageGroupQuest*)GetNtlStorageManager()->GetStorageGroup(eNTL_STORAGE_GROUP_QUEST);

	for (it = m_listData.begin(); it != m_listData.end(); ++it)
	{
		if ((*it)->sTSKey.tID == nID)
		{
			pGroupQuest->IndicateQuest((*it)->sTSKey.tID, FALSE);
			NTL_DELETE((*it));
			m_listData.erase(it);
			break;
		}
	}
}

VOID CQuestIndicatorGui::DeleteAllData(VOID)
{
	LIST_DATA::iterator it;

	for (it = m_listData.begin(); it != m_listData.end(); ++it)
	{
		NTL_DELETE((*it));
	}

	m_listData.clear();
}

void CQuestIndicatorGui::SetListItem(int nOffset)
{
	m_nScrollPos = nOffset;

	CRectangle rtPos = m_ppnIndicatorList->GetPosition();
	m_ppnIndicatorList->SetPosition(rtPos.left, -m_nScrollPos);

	CalculatePosition(m_nScrollPos);
}

void CQuestIndicatorGui::OnPaint()
{
	for (int i = 0; i < eMAX_CAN_PROGRESS_QUEST_NUM; i++)
	{
		if (m_IndicatorItem[i].m_pFrame->IsVisible() == false)
			break;

		m_IndicatorItem[i].m_Background.Render();
	}
}

void CQuestIndicatorGui::OnClickMinimize(gui::CComponent* pComponent)
{
	CDboEventGenerator::SideDialogEvent(SDIALOG_MESSAGE_MINIMIZE, SDIALOG_QUEST_INDICATOR, DIALOG_SIDEDIALOG_CONTROLLER);
}

void CQuestIndicatorGui::OnClickAllExtendOn(gui::CComponent* pComponent)
{

	m_pbtnAllExtendOff->Show(true);
	m_pbtnAllExtendOn->Show(false);

	for (int i = 0; i < eMAX_CAN_PROGRESS_QUEST_NUM; i++)
	{
		m_IndicatorItem[i].m_bQuestInfoShown = false;
	}
	Refresh();

}

void CQuestIndicatorGui::OnClickAllExtendOff(gui::CComponent* pComponent)
{
	m_pbtnAllExtendOff->Show(false);
	m_pbtnAllExtendOn->Show(true);

	for (int i = 0; i < eMAX_CAN_PROGRESS_QUEST_NUM; i++)
	{
		m_IndicatorItem[i].m_bQuestInfoShown = true;
	}
	Refresh();

}


void CQuestIndicatorGui::OnReleaseExpend(gui::CComponent* pComponent)
{
	Refresh();
}

void CQuestIndicatorGui::OnClickQuestListOpen(gui::CComponent* pComponent)
{
	if (GetDialogManager()->IsOpenDialog(DIALOG_QUESTLIST))
		GetDialogManager()->CloseDialog(DIALOG_QUESTLIST);
	else
		GetDialogManager()->OpenDialog(DIALOG_QUESTLIST);
}

void CQuestIndicatorGui::OnScrollChanged(int nNewOffset)
{
	SetListItem(nNewOffset);
}

void CQuestIndicatorGui::OnMouseWheel(int nFlag, RwInt16 sDelta, CPos& pos)
{
	if (m_pscrBar->IsVisible() == false)
		return;

	int nOffset = (int)((float)m_pscrBar->GetMaxValue() * 0.5f) * sDelta;
	CAPTURE_MOUSEWHEELMOVE_SCROLLBAR(IsShow(), m_pThis, m_pThis, m_pscrBar, nOffset, pos);
}
