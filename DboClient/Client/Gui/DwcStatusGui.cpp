#include "precomp_dboclient.h"
#include "DwcStatusGui.h"

// core
#include "NtlDebug.h"

// shared
#include "TableContainer.h"
#include "TextAllTable.h"

// presentation
#include "NtlPLGuiManager.h"
#include "NtlPLEvent.h"

// simulation
#include "NtlSLApi.h"
#include "NtlSLGlobal.h"
#include "NtlWorldConcept.h"

// dbo
#include "CounterGui.h"
#include "DboEvent.h"
#include "DboEventGenerator.h"
#include "DboGlobal.h"
#include "DialogManager.h"
#include "DisplayStringManager.h"
#include "SideDialogManager.h"

// same corner as the ccbd counter, above the quick slots
#define dDWC_COUNTER_X				5
#define dDWC_COUNTER_BOTTOM_GAP		125

// tw layout of the objective rows inside pnlInfoTextView
#define dDWC_OBJECTIVE_X			3
#define dDWC_OBJECTIVE_GAP			3
#define dDWC_OBJECTIVE_WIDTH		135
#define dDWC_OBJECTIVE_HEIGHT		15
#define dDWC_OBJECTIVE_FONT_SIZE	90
#define dDWC_OBJECTIVE_VIEW_HEIGHT	244
#define dDWC_OBJECTIVE_WHEEL_STEP	18

// tw: a running objective is white, a done one grey
#define dDWC_OBJECTIVE_COLOR		RGB(255, 255, 255)
#define dDWC_OBJECTIVE_DONE_COLOR	RGB(183, 191, 195)


CDwcStatusGui* CDwcStatusGui::m_pInstance = NULL;


CDwcStatusGui::CDwcStatusGui(const RwChar* pName)
: CNtlPLGui(pName)
{
	m_pstbDialogName = NULL;
	m_pstbManual = NULL;
	m_pstbStep = NULL;
	m_pbtnMinimize = NULL;
	m_pbtnPrologInfo = NULL;
	m_pbtnScenarioInfo = NULL;
	m_ppnlClippingFrame = NULL;
	m_ppnlInfoTextView = NULL;
	m_pscbScroll = NULL;
	m_iViewTop = 0;
	m_pCounterGui = NULL;
}

CDwcStatusGui::~CDwcStatusGui(VOID)
{
}

VOID CDwcStatusGui::CreateInstance(VOID)
{
	if (m_pInstance)
		return;

	m_pInstance = NTL_NEW CDwcStatusGui("DwcStatusGui");
	if (!m_pInstance->Create())
	{
		m_pInstance->Destroy();
		NTL_DELETE(m_pInstance);
		return;
	}

	CNtlPLGuiManager::GetInstance()->AddGui(m_pInstance);
	GetSideDialogManager()->RegistDialog(SDIALOG_DWC_STATUS, m_pInstance, &CDwcStatusGui::SwitchDialog);
}

VOID CDwcStatusGui::DestroyInstance(VOID)
{
	if (!m_pInstance)
		return;

	GetSideDialogManager()->UnRegistDialog(SDIALOG_DWC_STATUS);
	GetNtlGuiManager()->RemoveGui(m_pInstance);
	m_pInstance->Destroy();
	NTL_DELETE(m_pInstance);
}

RwBool CDwcStatusGui::Create(VOID)
{
	NTL_FUNCTION("CDwcStatusGui::Create");

	if (!CNtlPLGui::Create("gui\\DWC.rsr", "gui\\DWC.srf", "gui\\DWC_MissionInfo.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(GetNtlGuiManager()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");
	m_pThis->SetPriority(dDIALOGPRIORITY_SIDE_DIALOG);

	m_pbtnMinimize = (gui::CButton*)GetComponent("btnMinimize");
	m_slotClickedBtnMinimize = m_pbtnMinimize->SigClicked().Connect(this, &CDwcStatusGui::OnClickedBtnMinimize);

	// tw: the prologue and the objectives of the dwc the avatar plays
	m_pbtnPrologInfo = (gui::CButton*)GetComponent("btnPrologInfo");
	m_pbtnPrologInfo->SetToolTip(GetDisplayStringManager()->GetString("DST_DWC_PROLOG_TITLE"));
	m_slotClickedBtnPrologInfo = m_pbtnPrologInfo->SigClicked().Connect(this, &CDwcStatusGui::OnClickedBtnPrologInfo);

	m_pbtnScenarioInfo = (gui::CButton*)GetComponent("btnScenarioInfo");
	m_pbtnScenarioInfo->SetToolTip(GetDisplayStringManager()->GetString("DST_DWC_TARGET_TITLE"));
	m_slotClickedBtnScenarioInfo = m_pbtnScenarioInfo->SigClicked().Connect(this, &CDwcStatusGui::OnClickedBtnScenarioInfo);

	// tw texts of the side dialog
	m_pstbDialogName = (gui::CStaticBox*)GetComponent("stbDialogName");
	m_pstbDialogName->SetText(GetDisplayStringManager()->GetString("DST_DWC_STATUS"));

	m_pstbManual = (gui::CStaticBox*)GetComponent("stbManual");
	m_pstbManual->SetText(GetDisplayStringManager()->GetString("DST_TMQ_STATUS_INFO"));

	m_pstbStep = (gui::CStaticBox*)GetComponent("stbStep");
	m_pstbStep->SetText(GetDisplayStringManager()->GetString("DST_TMQ_STATUS_STATUS_INFO"));

	m_ppnlClippingFrame = (gui::CPanel*)GetComponent("pnlClippingFrame");
	m_ppnlInfoTextView = (gui::CPanel*)GetComponent("pnlInfoTextView");
	m_iViewTop = m_ppnlInfoTextView->GetPosition().top;
	m_ppnlInfoTextView->SetClippingMode(true);

	m_pscbScroll = (gui::CScrollBar*)GetComponent("scbScroll");
	m_pscbScroll->Show(false);
	m_slotScrollChanged = m_pscbScroll->SigValueChanged().Connect(this, &CDwcStatusGui::OnScrollChanged);
	m_slotScrollMoved = m_pscbScroll->SigSliderMoved().Connect(this, &CDwcStatusGui::OnScrollChanged);

	m_slotMouseWheel = GetNtlGuiManager()->GetGuiManager()->SigCaptureWheelMove().Connect(this, &CDwcStatusGui::OnMouseWheel);

	LinkMsg(g_EventCharReady);
	LinkMsg(g_EventDwcNotify);
	LinkMsg(g_EventResize);

	Show(false);

	NTL_RETURN(TRUE);
}

VOID CDwcStatusGui::Destroy(VOID)
{
	NTL_FUNCTION("CDwcStatusGui::Destroy");

	StopCountdown();

	UnLinkMsg(g_EventCharReady);
	UnLinkMsg(g_EventDwcNotify);
	UnLinkMsg(g_EventResize);

	ClearObjectives();

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();

	NTL_RETURNVOID();
}

RwInt32 CDwcStatusGui::SwitchDialog(bool bOpen)
{
	if (bOpen)
	{
		if (GetNtlWorldConcept()->IsActivePlayConcept(WORLD_PLAY_DWC) == FALSE)
			return -1;

		RefreshObjectives();
	}

	Show(bOpen);
	return TRUE;
}

VOID CDwcStatusGui::HandleEvents(RWS::CMsg& msg)
{
	if (msg.Id == g_EventCharReady)
	{
		if (GetNtlWorldConcept()->IsActivePlayConcept(WORLD_PLAY_DWC) == FALSE)
			return;

		if (IsShow() == FALSE)
			GetSideDialogManager()->OpenDialog(SDIALOG_DWC_STATUS);
	}
	else if (msg.Id == g_EventDwcNotify)
	{
		SDboEventDwcNotify* pData = reinterpret_cast<SDboEventDwcNotify*>(msg.pData);

		if (pData->byType == DWC_EVENT_SCRIPT_STATUS)
		{
			RefreshObjectives();
		}
		else if (pData->byType == DWC_EVENT_COUNTDOWN)
		{
			if (pData->bValue)
				StartCountdown((RwReal)pData->uiValue);
			else
				StopCountdown();
		}
	}
	else if (msg.Id == g_EventResize)
	{
		LocateCountdown();
	}
}

VOID CDwcStatusGui::RefreshObjectives(VOID)
{
	ClearObjectives();

	CTextTable* pTextTable = API_GetTableContainer()->GetTextAllTable()->GetCSTextTble();
	LIST_SERVERSCRIPT_EVENTSTATUS* pList = GetNtlSLGlobal()->GetServerScriptEventStatus();
	gui::CSurfaceManager* pSurfaceManager = GetNtlGuiManager()->GetSurfaceManager();

	RwInt32 iY = dDWC_OBJECTIVE_GAP;
	WCHAR awcCount[32];

	for (LIST_SERVERSCRIPT_EVENTSTATUS::iterator it = pList->begin(); it != pList->end(); ++it)
	{
		COLORREF color = it->bSelected ? dDWC_OBJECTIVE_DONE_COLOR : dDWC_OBJECTIVE_COLOR;

		CRectangle rect;
		rect.SetRectWH(dDWC_OBJECTIVE_X, iY, dDWC_OBJECTIVE_WIDTH, dDWC_OBJECTIVE_HEIGHT);

		sOBJECTIVE sObjective;

		sObjective.pstbText = NTL_NEW gui::CStaticBox(rect, m_ppnlInfoTextView, pSurfaceManager);
		sObjective.pstbText->CreateFontStd(DEFAULT_FONT, dDWC_OBJECTIVE_FONT_SIZE, DEFAULT_FONT_ATTR);
		sObjective.pstbText->SetTextStyle(COMP_TEXT_LEFT);
		sObjective.pstbText->SetTextColor(color);

		sObjective.pstbCount = NTL_NEW gui::CStaticBox(rect, m_ppnlInfoTextView, pSurfaceManager);
		sObjective.pstbCount->CreateFontStd(DEFAULT_FONT, dDWC_OBJECTIVE_FONT_SIZE, DEFAULT_FONT_ATTR);
		sObjective.pstbCount->SetTextStyle(COMP_TEXT_RIGHT);
		sObjective.pstbCount->SetTextColor(color);
		sObjective.pstbText->SetClippingMode(true);
		sObjective.pstbCount->SetClippingMode(true);

		// tw: the full text is in the tooltip, a long one does not fit the row
		if (pTextTable)
		{
			const std::wstring& wstrText = pTextTable->GetText(it->textTblidx);
			sObjective.pstbText->SetText(wstrText.c_str());
			sObjective.pstbText->SetToolTip(wstrText);
		}

		// tw: only the mob kill count shows the "%d/%d"
		if (it->byType == SERVERSCRIPT_EVENTSTATUS_MOBKILL_COUNT && it->wMaxCount > 0)
		{
			swprintf_s(awcCount, 32, L"%d/%d", it->wCount, it->wMaxCount);
			sObjective.pstbCount->SetText(awcCount);
			sObjective.pstbCount->SetToolTip(std::wstring(awcCount));
		}

		m_vecObjective.push_back(sObjective);

		iY += dDWC_OBJECTIVE_HEIGHT + dDWC_OBJECTIVE_GAP;
	}

	// tw: the view grows with the rows, the scroll bar shows once they pass the frame
	m_ppnlInfoTextView->SetHeight(iY);
	// SetHeight keeps the old clipping rect, a move recalculates it for the panel and every child
	m_ppnlInfoTextView->SetPosition(m_ppnlInfoTextView->GetPosition().left, m_iViewTop + 1);
	m_ppnlInfoTextView->SetPosition(m_ppnlInfoTextView->GetPosition().left, m_iViewTop);

	RwInt32 iMax = iY - dDWC_OBJECTIVE_VIEW_HEIGHT;
	if (iMax > 0)
	{
		m_pscbScroll->SetRange(0, iMax);
		m_pscbScroll->Show(true);
	}
	else
	{
		m_pscbScroll->SetRange(0, 0);
		m_pscbScroll->Show(false);
	}

	m_pscbScroll->SetValue(0, FALSE);
}

VOID CDwcStatusGui::ClearObjectives(VOID)
{
	for (std::vector<sOBJECTIVE>::iterator it = m_vecObjective.begin(); it != m_vecObjective.end(); ++it)
	{
		NTL_DELETE(it->pstbText);
		NTL_DELETE(it->pstbCount);
	}

	m_vecObjective.clear();
}

VOID CDwcStatusGui::StartCountdown(RwReal fSecond)
{
	if (m_pCounterGui)
	{
		m_pCounterGui->SetTime(fSecond);
		return;
	}

	m_pCounterGui = NTL_NEW CCounterGui("DwcCounterGui");
	if (m_pCounterGui->Create(COUNTER_DISPLAY_2, fSecond, COUNTER_QUADRANT_4) == FALSE)
	{
		m_pCounterGui->Destroy();
		NTL_DELETE(m_pCounterGui);
		return;
	}

	CNtlPLGuiManager::GetInstance()->AddGui(m_pCounterGui);
	m_pCounterGui->AlarmSound(TRUE);

	LocateCountdown();
}

VOID CDwcStatusGui::StopCountdown(VOID)
{
	if (m_pCounterGui == NULL)
		return;

	CNtlPLGuiManager::GetInstance()->RemoveGui(m_pCounterGui);
	m_pCounterGui->Destroy();
	NTL_DELETE(m_pCounterGui);
}

VOID CDwcStatusGui::LocateCountdown(VOID)
{
	if (m_pCounterGui)
		m_pCounterGui->SetPosition(dDWC_COUNTER_X, GetDboGlobal()->GetScreenHeight() - dDWC_COUNTER_BOTTOM_GAP);
}

VOID CDwcStatusGui::OnClickedBtnMinimize(gui::CComponent* pComponent)
{
	CDboEventGenerator::SideDialogEvent(SDIALOG_MESSAGE_MINIMIZE, SDIALOG_DWC_STATUS, DIALOG_SIDEDIALOG_CONTROLLER);
}

VOID CDwcStatusGui::OnClickedBtnPrologInfo(gui::CComponent* pComponent)
{
	GetDialogManager()->SwitchDialog(DIALOG_DWC_PROLOG);
}

VOID CDwcStatusGui::OnClickedBtnScenarioInfo(gui::CComponent* pComponent)
{
	GetDialogManager()->SwitchDialog(DIALOG_DWC_SCENARIO_MAIN);
}

VOID CDwcStatusGui::OnScrollChanged(RwInt32 iValue)
{
	m_ppnlInfoTextView->SetPosition(m_ppnlInfoTextView->GetPosition().left, m_iViewTop - iValue);
}

VOID CDwcStatusGui::OnMouseWheel(RwInt32 nFlag, RwInt16 sDelta, CPos& pos)
{
	if (!IsShow() || !m_pscbScroll->IsVisible())
		return;

	if (m_pThis->GetParent()->GetChildComponentReverseAt(pos.x, pos.y) != m_pThis)
		return;

	if (m_pThis->PosInRect(pos.x, pos.y) != gui::CComponent::INRECT)
		return;

	// tw: 18 pixels per wheel notch
	RwInt32 iValue = m_pscbScroll->GetValue() - sDelta * dDWC_OBJECTIVE_WHEEL_STEP / GUI_MOUSE_WHEEL_DELTA;
	if (iValue < 0)
		iValue = 0;
	else if (iValue > m_pscbScroll->GetMaxValue())
		iValue = m_pscbScroll->GetMaxValue();

	m_pscbScroll->SetValue(iValue);
}
