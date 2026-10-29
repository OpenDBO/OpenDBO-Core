#include "precomp_dboclient.h"
#include "DwcScenarioInfoGui.h"

// core
#include "NtlDebug.h"

// shared
#include "DwcMissionTable.h"
#include "DwcTable.h"
#include "TableContainer.h"

// presentation
#include "NtlPLGuiManager.h"

// simulation
#include "NtlSLApi.h"
#include "NtlSLGlobal.h"
#include "NtlSLLogic.h"

// dbo
#include "DboEvent.h"
#include "DboGlobal.h"
#include "DboLogic.h"
#include "DialogManager.h"
#include "DisplayStringManager.h"

// tw layout of the admission conditions inside pnlConditionScroll
#define dDWC_CONDITION_TEXT_X			30
#define dDWC_CONDITION_CHECK_X			10
#define dDWC_CONDITION_FIRST_Y			35
#define dDWC_CONDITION_GAP				12
#define dDWC_CONDITION_TEXT_WIDTH		190
#define dDWC_CONDITION_TEXT_HEIGHT		16
#define dDWC_CONDITION_CHECK_WIDTH		13
#define dDWC_CONDITION_CHECK_HEIGHT		14
#define dDWC_CONDITION_FONT_SIZE		90
#define dDWC_CONDITION_VIEW_HEIGHT		234
#define dDWC_CONDITION_WHEEL_STEP		18

// the md static box does not wrap, the lines are cut at about this many letters
#define dDWC_CONDITION_LINE_LETTERS		30

#define dDWC_CONDITION_COLOR			RGB(255, 255, 255)

// DWC_DATA text of an admission condition achievement
#define dDWC_CONDITION_TEXT_BASE		100000

static const char* s_apcGradePanel[MAX_DWC_REWARD_GRADE_COUNT] = { "pnlNormalCard%d", "pnlSuperiorCard%d", "pnlExcellentCard%d", "pnlRareCard%d", "pnlLegendaryCard%d" };


CDwcScenarioInfoGui::CDwcScenarioInfoGui(const RwChar* pName)
: CNtlPLGui(pName)
{
	m_dwcTblidx = INVALID_TBLIDX;
	m_iViewTop = 0;
}

CDwcScenarioInfoGui::~CDwcScenarioInfoGui(VOID)
{
}

RwBool CDwcScenarioInfoGui::Create(VOID)
{
	NTL_FUNCTION("CDwcScenarioInfoGui::Create");

	if (!CNtlPLGui::Create("gui\\DWC.rsr", "gui\\DWC.srf", "gui\\DWC_ScenarioInfo.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(GetNtlGuiManager()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");
	m_pThis->SetPriority(dDIALOGPRIORITY_DEFAULT);

	m_pstbTitle = (gui::CStaticBox*)GetComponent("stbTitle");
	m_pstbScenarioTitle = (gui::CStaticBox*)GetComponent("stbScenarioTitle");
	m_pstbPlayerCountText = (gui::CStaticBox*)GetComponent("stbPlayerCountText");
	m_pstbPlayerCountValue = (gui::CStaticBox*)GetComponent("stbPlayerCountValue");
	m_pstbTargetText = (gui::CStaticBox*)GetComponent("stbTargetText");
	m_pstbCardGrade = (gui::CStaticBox*)GetComponent("stbCardGrade");
	m_pstbEnterCondition = (gui::CStaticBox*)GetComponent("stbEnterCondition");
	m_pstbLevel = (gui::CStaticBox*)GetComponent("stbLevel");
	m_ppnlCheckLevel = (gui::CPanel*)GetComponent("pnlCheckLevel");
	m_ppnlConditionScroll = (gui::CPanel*)GetComponent("pnlConditionScroll");
	m_pscbScroll = (gui::CScrollBar*)GetComponent("scbScroll");
	m_pbtnClose = (gui::CButton*)GetComponent("btnClose");

	char acName[64];
	for (RwInt32 i = 0; i < MAX_DWC_MISSION_COUNT_PER_SCENARIO; ++i)
	{
		sprintf_s(acName, 64, "stbTarget%02d", i + 1);
		m_apstbTarget[i] = (gui::CStaticBox*)GetComponent(acName);

		for (RwInt32 j = 0; j < MAX_DWC_REWARD_GRADE_COUNT; ++j)
		{
			sprintf_s(acName, 64, s_apcGradePanel[j], i + 1);
			m_appnlGrade[i][j] = (gui::CPanel*)GetComponent(acName);
		}
	}

	m_pstbTitle->SetText(GetDisplayStringManager()->GetString("DST_DWC_SCENARIOINFO_TITLE"));
	m_pstbPlayerCountText->SetText(GetDisplayStringManager()->GetString("DST_DWC_PLAYER_COUNT"));
	m_pstbTargetText->SetText(GetDisplayStringManager()->GetString("DST_DWC_TARGET"));
	m_pstbCardGrade->SetText(GetDisplayStringManager()->GetString("DST_DWC_CARD_GRADE"));
	m_pstbEnterCondition->SetText(GetDisplayStringManager()->GetString("DST_DWC_ENTER_CONDITION"));

	// the conditions scroll inside pnlEnterConditionFrame
	m_iViewTop = m_ppnlConditionScroll->GetPosition().top;
	m_ppnlConditionScroll->SetClippingMode(true);
	m_pstbLevel->SetClippingMode(true);
	m_ppnlCheckLevel->SetClippingMode(true);

	// tw: 10 condition rows, a text and the check of a met condition
	gui::CSurfaceManager* pSurfaceManager = GetNtlGuiManager()->GetSurfaceManager();

	for (RwInt32 i = 0; i < MAX_DWC_ADMISSION_CONDITION_COUNT; ++i)
	{
		CRectangle rect;

		rect.SetRectWH(dDWC_CONDITION_TEXT_X, 0, dDWC_CONDITION_TEXT_WIDTH, dDWC_CONDITION_TEXT_HEIGHT);
		m_asCondition[i].pmdbText = NTL_NEW gui::CMDStaticBox(rect, m_ppnlConditionScroll, pSurfaceManager);
		m_asCondition[i].pmdbText->AddFont(DEFAULT_FONT, dDWC_CONDITION_FONT_SIZE, DEFAULT_FONT_ATTR);
		m_asCondition[i].pmdbText->SetClippingMode(true);
		m_asCondition[i].pmdbText->Enable(false);
		m_asCondition[i].pmdbText->Show(false);

		rect.SetRectWH(dDWC_CONDITION_CHECK_X, 0, dDWC_CONDITION_CHECK_WIDTH, dDWC_CONDITION_CHECK_HEIGHT);
		m_asCondition[i].ppnlCheck = NTL_NEW gui::CPanel(rect, m_ppnlConditionScroll, pSurfaceManager, pSurfaceManager->GetSurface("DWC.srf", "srfConditionCheck"));
		m_asCondition[i].ppnlCheck->SetClippingMode(true);
		m_asCondition[i].ppnlCheck->Enable(false);
		m_asCondition[i].ppnlCheck->Show(false);
	}

	m_slotClickClose = m_pbtnClose->SigClicked().Connect(this, &CDwcScenarioInfoGui::OnClickClose);
	m_slotScrollChanged = m_pscbScroll->SigValueChanged().Connect(this, &CDwcScenarioInfoGui::OnScrollChanged);
	m_slotScrollMoved = m_pscbScroll->SigSliderMoved().Connect(this, &CDwcScenarioInfoGui::OnScrollChanged);
	m_slotMouseWheel = GetNtlGuiManager()->GetGuiManager()->SigCaptureWheelMove().Connect(this, &CDwcScenarioInfoGui::OnMouseWheel);

	LinkMsg(g_EventDwcNotify);

	Show(false);

	NTL_RETURN(TRUE);
}

VOID CDwcScenarioInfoGui::Destroy(VOID)
{
	NTL_FUNCTION("CDwcScenarioInfoGui::Destroy");

	UnLinkMsg(g_EventDwcNotify);

	for (RwInt32 i = 0; i < MAX_DWC_ADMISSION_CONDITION_COUNT; ++i)
	{
		NTL_DELETE(m_asCondition[i].pmdbText);
		NTL_DELETE(m_asCondition[i].ppnlCheck);
	}

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();

	NTL_RETURNVOID();
}

RwInt32 CDwcScenarioInfoGui::SwitchDialog(bool bOpen)
{
	if (bOpen)
		Refresh();

	Show(bOpen);
	return TRUE;
}

VOID CDwcScenarioInfoGui::HandleEvents(RWS::CMsg& msg)
{
	if (msg.Id == g_EventDwcNotify)
	{
		SDboEventDwcNotify* pData = reinterpret_cast<SDboEventDwcNotify*>(msg.pData);

		// tw: a new achievement checks its condition
		if (pData->byType == DWC_EVENT_ACHIEVEMENT && IsShow())
			Refresh();
	}
}

VOID CDwcScenarioInfoGui::SetScenario(TBLIDX dwcTblidx)
{
	m_dwcTblidx = dwcTblidx;

	if (IsShow())
		Refresh();
}

VOID CDwcScenarioInfoGui::Refresh(VOID)
{
	sDWC_TBLDAT* pDwcTbldat = reinterpret_cast<sDWC_TBLDAT*>(API_GetTableContainer()->GetDwcTable()->FindData(m_dwcTblidx));
	if (pDwcTbldat == NULL)
		return;

	CDwcMissionTable* pMissionTable = API_GetTableContainer()->GetDwcMissionTable();
	SDwcInfo* pDwcInfo = GetNtlSLGlobal()->GetDwcInfo();

	m_pstbScenarioTitle->SetText(Logic_GetDwcText(pDwcTbldat->tblNameIndex).c_str());

	WCHAR awcBuffer[256];
	swprintf_s(awcBuffer, 256, GetDisplayStringManager()->GetString("DST_PLAYER_COUNT"), pDwcTbldat->byAdmission_Num_Min);
	m_pstbPlayerCountValue->SetText(awcBuffer);

	// tw: every mission with the grades it has
	for (RwInt32 i = 0; i < MAX_DWC_MISSION_COUNT_PER_SCENARIO; ++i)
	{
		sDWCMISSION_TBLDAT* pMissionTbldat = reinterpret_cast<sDWCMISSION_TBLDAT*>(pMissionTable->FindData(pDwcTbldat->aMissionTblidx[i]));

		if (pMissionTbldat == NULL)
		{
			m_apstbTarget[i]->Show(false);
			for (RwInt32 j = 0; j < MAX_DWC_REWARD_GRADE_COUNT; ++j)
				m_appnlGrade[i][j]->Show(false);

			continue;
		}

		m_apstbTarget[i]->Show(true);
		m_apstbTarget[i]->SetText(Logic_GetDwcText(pMissionTbldat->asReward[DWC_REWARD_GRADE_NORMAL].cardNameTblidx).c_str());

		for (RwInt32 j = 0; j < MAX_DWC_REWARD_GRADE_COUNT; ++j)
			m_appnlGrade[i][j]->Show(pMissionTbldat->asReward[j].cardNameTblidx != INVALID_TBLIDX);
	}

	// tw: the level, then the achievements, a check on the met ones
	swprintf_s(awcBuffer, 256, GetDisplayStringManager()->GetString("DST_DWC_PLAYLEVEL"), pDwcTbldat->byLevel_Min);
	m_pstbLevel->SetText(awcBuffer);
	m_ppnlCheckLevel->Show(Logic_GetLevel(GetNtlSLGlobal()->GetSobAvatar()) >= pDwcTbldat->byLevel_Min);

	RwInt32 iY = dDWC_CONDITION_FIRST_Y;
	RwInt32 iBottom = m_pstbLevel->GetPosition().bottom;

	for (RwInt32 i = 0; i < MAX_DWC_ADMISSION_CONDITION_COUNT; ++i)
	{
		sCONDITION_ROW& rRow = m_asCondition[i];
		TBLIDX conditionTblidx = pDwcTbldat->aConditionTblidx[i];

		if (conditionTblidx == INVALID_TBLIDX)
		{
			rRow.pmdbText->Show(false);
			rRow.ppnlCheck->Show(false);
			continue;
		}

		// the condition text is DWC_DATA 100000 + achievement
		std::wstring wstrText = Logic_GetDwcText(dDWC_CONDITION_TEXT_BASE + conditionTblidx);

		rRow.pmdbText->Clear();

		std::wstring wstrRest = wstrText;
		RwInt32 iLine = 0;
		char acName[32];

		while (!wstrRest.empty())
		{
			std::wstring wstrLine;

			if ((RwInt32)wstrRest.size() <= dDWC_CONDITION_LINE_LETTERS)
			{
				wstrLine = wstrRest;
				wstrRest.clear();
			}
			else
			{
				std::wstring::size_type pos = wstrRest.rfind(L' ', dDWC_CONDITION_LINE_LETTERS);
				if (pos == std::wstring::npos || pos == 0)
					pos = dDWC_CONDITION_LINE_LETTERS;

				wstrLine = wstrRest.substr(0, pos);
				wstrRest = wstrRest.substr(pos);

				while (!wstrRest.empty() && wstrRest[0] == L' ')
					wstrRest.erase(0, 1);
			}

			sprintf_s(acName, 32, "line%d", ++iLine);
			rRow.pmdbText->SetItem(wstrLine.c_str(), acName, 0, COMP_TEXT_LEFT, dDWC_CONDITION_COLOR);
		}

		rRow.pmdbText->DrawItem();
		rRow.pmdbText->SetPosition(dDWC_CONDITION_TEXT_X, iY);
		rRow.pmdbText->Show(true);

		rRow.ppnlCheck->SetPosition(dDWC_CONDITION_CHECK_X, iY);
		rRow.ppnlCheck->Show(pDwcInfo->IsAchievementComplete(conditionTblidx) ? true : false);

		iBottom = rRow.pmdbText->GetPosition().bottom;
		iY = iBottom + dDWC_CONDITION_GAP;
	}

	// tw: the list scrolls once it passes the frame
	m_ppnlConditionScroll->SetHeight(iBottom);

	RwInt32 iMax = iBottom - dDWC_CONDITION_VIEW_HEIGHT;
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

	// SetHeight keeps the old clipping rect, a move recalculates it for the panel and every child
	m_ppnlConditionScroll->SetPosition(m_ppnlConditionScroll->GetPosition().left, m_iViewTop + 1);
	m_ppnlConditionScroll->SetPosition(m_ppnlConditionScroll->GetPosition().left, m_iViewTop);
}

VOID CDwcScenarioInfoGui::OnClickClose(gui::CComponent* pComponent)
{
	GetDialogManager()->CloseDialog(DIALOG_DWC_SCENARIO_INFO);
}

VOID CDwcScenarioInfoGui::OnScrollChanged(RwInt32 iValue)
{
	m_ppnlConditionScroll->SetPosition(m_ppnlConditionScroll->GetPosition().left, m_iViewTop - iValue);
}

VOID CDwcScenarioInfoGui::OnMouseWheel(RwInt32 nFlag, RwInt16 sDelta, CPos& pos)
{
	if (!IsShow() || !m_pscbScroll->IsVisible())
		return;

	if (m_pThis->GetParent()->GetChildComponentReverseAt(pos.x, pos.y) != m_pThis)
		return;

	if (m_pThis->PosInRect(pos.x, pos.y) != gui::CComponent::INRECT)
		return;

	RwInt32 iValue = m_pscbScroll->GetValue() - sDelta * dDWC_CONDITION_WHEEL_STEP / GUI_MOUSE_WHEEL_DELTA;
	if (iValue < 0)
		iValue = 0;
	else if (iValue > m_pscbScroll->GetMaxValue())
		iValue = m_pscbScroll->GetMaxValue();

	m_pscbScroll->SetValue(iValue);
}
