#include "precomp_dboclient.h"
#include "DwcChallengeGui.h"

// core
#include "NtlDebug.h"

// shared
#include "DwcMissionTable.h"
#include "DwcTable.h"
#include "TableContainer.h"
#include "TextAllTable.h"

// presentation
#include "NtlPLGuiManager.h"

// simulation
#include "NtlSLApi.h"
#include "NtlSLGlobal.h"

// dbo
#include "DboEvent.h"
#include "DboEventGenerator.h"
#include "DboGlobal.h"
#include "DboLogic.h"
#include "DialogManager.h"
#include "DisplayStringManager.h"
#include "DwcHeroInfoGui.h"
#include "DwcScenarioInfoGui.h"
#include "InfoWndManager.h"

// tw card grid inside pnlCardList
#define dDWC_CARD_COLUMN				7
#define dDWC_CARD_VISIBLE_ROW			3
#define dDWC_CARD_MARGIN_X				18
#define dDWC_CARD_GAP_X					85
#define dDWC_CARD_GAP_Y					139
#define dDWC_CARD_WHEEL_STEP			18

// tw CDwcCard layout
#define dDWC_CARD_BUTTON_WIDTH			81
#define dDWC_CARD_BUTTON_HEIGHT			106
#define dDWC_CARD_IMAGE_INSET			3
#define dDWC_CARD_NUMBER_WIDTH			75
#define dDWC_CARD_NUMBER_HEIGHT			15
#define dDWC_CARD_BUTTON_OFFSET_X		-3		///< the button from the number box
#define dDWC_CARD_STAMP_OFFSET_X		-8		///< the lock / clear stamp from the number box
#define dDWC_CARD_STAMP_WIDTH			80
#define dDWC_CARD_STAMP_HEIGHT			44
#define dDWC_CARD_GRADE_X				4
#define dDWC_CARD_GRADE_Y				85
#define dDWC_CARD_GRADE_WIDTH			73
#define dDWC_CARD_GRADE_HEIGHT			13
#define dDWC_CARD_GAGE_BG_X				8
#define dDWC_CARD_GAGE_BG_Y				85
#define dDWC_CARD_GAGE_BG_WIDTH			65
#define dDWC_CARD_GAGE_BG_HEIGHT		11
#define dDWC_CARD_GAGE_X				2
#define dDWC_CARD_GAGE_Y				1
#define dDWC_CARD_GAGE_WIDTH			61
#define dDWC_CARD_GAGE_HEIGHT			9

#define dDWC_CARD_NUMBER_COLOR			RGB(210, 210, 210)
#define dDWC_SCENARIO_LOCK_COLOR		RGB(183, 191, 195)

static const char* s_apcSmallGrade[MAX_DWC_REWARD_GRADE_COUNT] = { "srfSmallNormal", "srfSmallSuperior", "srfSmallExcellent", "srfSmallRare", "srfSmallLegendary" };


CDwcChallengeGui::CDwcChallengeGui(const RwChar* pName)
: CNtlPLGui(pName)
{
	m_pstbTitle = NULL;
	m_pstbDwcTitle = NULL;
	m_pstbText = NULL;
	m_pstbDwcPointName = NULL;
	m_pstbDwcPoint = NULL;
	m_pbtnClose = NULL;
	m_ppnlCardList = NULL;
	m_pscbScroll = NULL;
	m_pcbbScenario = NULL;
	m_pcbbGrade = NULL;
	m_pcbbState = NULL;
	m_iListTop = 0;
}

CDwcChallengeGui::~CDwcChallengeGui(VOID)
{
}

RwBool CDwcChallengeGui::Create(VOID)
{
	NTL_FUNCTION("CDwcChallengeGui::Create");

	if (!CNtlPLGui::Create("gui\\DWC.rsr", "gui\\DWC.srf", "gui\\DWC_Main.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(GetNtlGuiManager()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");
	m_pThis->SetPriority(dDIALOGPRIORITY_DEFAULT);

	m_pstbTitle = (gui::CStaticBox*)GetComponent("stbTitle");
	m_pstbTitle->SetText(GetDisplayStringManager()->GetString("DST_DWC_MAIN_TITLE"));

	m_pbtnClose = (gui::CButton*)GetComponent("btnClose");
	m_slotClickClose = m_pbtnClose->SigClicked().Connect(this, &CDwcChallengeGui::OnClickClose);

	m_pstbDwcTitle = (gui::CStaticBox*)GetComponent("stbDwcTitle");
	m_pstbText = (gui::CStaticBox*)GetComponent("stbText");

	m_pstbDwcPointName = (gui::CStaticBox*)GetComponent("stbDwcPointName");
	m_pstbDwcPointName->SetText(GetDisplayStringManager()->GetString("DST_DWC_CARDCOUNT"));
	m_pstbDwcPointName->SetToolTip(std::wstring(GetDisplayStringManager()->GetString("DST_DWC_CLEAR_CARD_TOOLTIP")));
	m_pstbDwcPoint = (gui::CStaticBox*)GetComponent("stbDwcPoint");
	m_pstbDwcPoint->SetToolTip(std::wstring(GetDisplayStringManager()->GetString("DST_DWC_CLEAR_CARD_TOOLTIP")));

	m_ppnlCardList = (gui::CPanel*)GetComponent("pnlCardList");
	m_iListTop = m_ppnlCardList->GetPosition().top;

	// tw: the cards are cut to pnlCardClipping, every card component clips as well
	m_ppnlCardList->SetClippingMode(true);

	m_pscbScroll = (gui::CScrollBar*)GetComponent("scbScroll");
	m_slotScrollChanged = m_pscbScroll->SigValueChanged().Connect(this, &CDwcChallengeGui::OnScrollChanged);
	m_slotScrollMoved = m_pscbScroll->SigSliderMoved().Connect(this, &CDwcChallengeGui::OnScrollChanged);

	// tw: the frm only names the top left piece of the drop down lists
	m_pcbbScenario = (gui::CComboBox*)GetComponent("cbbDwcScenario");
	AddComboListSurface(m_pcbbScenario, "srfScenarioCombo");
	m_pcbbScenario->AddItem(GetDisplayStringManager()->GetString("DST_DWC_SCENARIO_LIST"), INVALID_TBLIDX);
	m_pcbbScenario->SelectItem(0);
	m_pcbbScenario->SetToolTip(std::wstring(GetDisplayStringManager()->GetString("DST_DWC_SCENARIO_TOOLTIP")));
	m_pcbbScenario->GetButton()->SetToolTip(std::wstring(GetDisplayStringManager()->GetString("DST_DWC_SCENARIO_TOOLTIP")));
	m_slotSelectScenario = m_pcbbScenario->SigSelected().Connect(this, &CDwcChallengeGui::OnSelectScenario);

	m_pcbbGrade = (gui::CComboBox*)GetComponent("cbbDwcGrade");
	AddComboListSurface(m_pcbbGrade, "srfGradeCombo");
	m_pcbbGrade->AddItem(GetDisplayStringManager()->GetString("DST_DWC_CARD_GRADE_00"), INVALID_DWC_REWARD_GRADE);
	m_pcbbGrade->AddItem(GetDisplayStringManager()->GetString("DST_DWC_CARD_GRADE_01"), DWC_REWARD_GRADE_NORMAL);
	m_pcbbGrade->AddItem(GetDisplayStringManager()->GetString("DST_DWC_CARD_GRADE_02"), DWC_REWARD_GRADE_SUPERIOR);
	m_pcbbGrade->AddItem(GetDisplayStringManager()->GetString("DST_DWC_CARD_GRADE_03"), DWC_REWARD_GRADE_EXCELLENT);
	m_pcbbGrade->AddItem(GetDisplayStringManager()->GetString("DST_DWC_CARD_GRADE_04"), DWC_REWARD_GRADE_RARE);
	m_pcbbGrade->AddItem(GetDisplayStringManager()->GetString("DST_DWC_CARD_GRADE_05"), DWC_REWARD_GRADE_LEGENDARY);
	m_pcbbGrade->SelectItem(0);
	m_pcbbGrade->SetToolTip(std::wstring(GetDisplayStringManager()->GetString("DST_DWC_GRADE_TOOLTIP")));
	m_pcbbGrade->GetButton()->SetToolTip(std::wstring(GetDisplayStringManager()->GetString("DST_DWC_GRADE_TOOLTIP")));
	m_slotSelectGrade = m_pcbbGrade->SigSelected().Connect(this, &CDwcChallengeGui::OnSelectFilter);

	m_pcbbState = (gui::CComboBox*)GetComponent("cbbDwcState");
	AddComboListSurface(m_pcbbState, "srfGradeCombo");
	m_pcbbState->AddItem(GetDisplayStringManager()->GetString("DST_LOBBY_STATE"), NUM_CARD_STATE);
	m_pcbbState->AddItem(GetDisplayStringManager()->GetString("DST_DWC_STATE_LOCK"), CARD_STATE_LOCK);
	m_pcbbState->AddItem(GetDisplayStringManager()->GetString("DST_DWC_STATE_OPEN"), CARD_STATE_OPEN);
	m_pcbbState->AddItem(GetDisplayStringManager()->GetString("DST_DWC_STATE_PLAY"), CARD_STATE_PLAY);
	m_pcbbState->AddItem(GetDisplayStringManager()->GetString("DST_DWC_STATE_CLEAR"), CARD_STATE_CLEAR);
	m_pcbbState->SelectItem(0);
	m_pcbbState->SetToolTip(std::wstring(GetDisplayStringManager()->GetString("DST_DWC_STATE_TOOLTIP")));
	m_pcbbState->GetButton()->SetToolTip(std::wstring(GetDisplayStringManager()->GetString("DST_DWC_STATE_TOOLTIP")));
	m_slotSelectState = m_pcbbState->SigSelected().Connect(this, &CDwcChallengeGui::OnSelectFilter);

	m_slotMouseWheel = GetNtlGuiManager()->GetGuiManager()->SigCaptureWheelMove().Connect(this, &CDwcChallengeGui::OnMouseWheel);

	CreateCards();

	LinkMsg(g_EventDwcNotify);

	Show(false);

	NTL_RETURN(TRUE);
}

VOID CDwcChallengeGui::Destroy(VOID)
{
	NTL_FUNCTION("CDwcChallengeGui::Destroy");

	UnLinkMsg(g_EventDwcNotify);

	DestroyCards();

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();

	NTL_RETURNVOID();
}

RwInt32 CDwcChallengeGui::SwitchDialog(bool bOpen)
{
	if (bOpen)
	{
		// tw: the window asks for the card progress every time it opens
		GetDboGlobal()->GetGamePacketGenerator()->SendDwcScenarioInfoReq();

		RefreshScenarioList();
		Refresh();

		// tw: the challenge open, the side icon has done its job
		CDboEventGenerator::DwcNotify(DWC_EVENT_CHALLENGE_OPEN);
	}
	else
	{
		// tw: the details close with the challenge
		if (GetDialogManager()->IsOpenDialog(DIALOG_DWC_HEROINFO))
			GetDialogManager()->CloseDialog(DIALOG_DWC_HEROINFO);

		if (GetDialogManager()->IsOpenDialog(DIALOG_DWC_SCENARIO_INFO))
			GetDialogManager()->CloseDialog(DIALOG_DWC_SCENARIO_INFO);

		if (GetInfoWndManager()->GetRequestGui() == DIALOG_DWC_CHALLENGE)
			GetInfoWndManager()->ShowInfoWindow(FALSE);
	}

	Show(bOpen);
	return TRUE;
}

VOID CDwcChallengeGui::HandleEvents(RWS::CMsg& msg)
{
	if (msg.Id == g_EventDwcNotify)
	{
		SDboEventDwcNotify* pData = reinterpret_cast<SDboEventDwcNotify*>(msg.pData);
		if (pData->byType != DWC_EVENT_SCENARIO_INFO && pData->byType != DWC_EVENT_ACHIEVEMENT)
			return;

		if (!IsShow())
			return;

		RefreshScenarioList();
		Refresh();
	}
}

VOID CDwcChallengeGui::CreateCards(VOID)
{
	CDwcTable* pDwcTable = API_GetTableContainer()->GetDwcTable();
	CDwcMissionTable* pMissionTable = API_GetTableContainer()->GetDwcMissionTable();

	for (CTable::TABLEIT it = pDwcTable->Begin(); it != pDwcTable->End(); ++it)
	{
		sDWC_TBLDAT* pDwcTbldat = reinterpret_cast<sDWC_TBLDAT*>(it->second);
		if (pDwcTbldat->tblNameIndex == INVALID_TBLIDX)
			continue;

		for (RwUInt8 i = 0; i < MAX_DWC_MISSION_COUNT_PER_SCENARIO; ++i)
		{
			sDWCMISSION_TBLDAT* pMissionTbldat = reinterpret_cast<sDWCMISSION_TBLDAT*>(pMissionTable->FindData(pDwcTbldat->aMissionTblidx[i]));
			if (pMissionTbldat == NULL)
				continue;

			for (RwUInt8 byGrade = 0; byGrade < MAX_DWC_REWARD_GRADE_COUNT; ++byGrade)
			{
				if (pMissionTbldat->asReward[byGrade].cardNameTblidx == INVALID_TBLIDX)
					continue;

				m_vecCard.push_back(CreateCard(pDwcTbldat, i, pMissionTbldat, byGrade));
			}
		}
	}
}

CDwcChallengeGui::sCARD* CDwcChallengeGui::CreateCard(sDWC_TBLDAT* pDwcTbldat, RwUInt8 byMissionIndex, sDWCMISSION_TBLDAT* pMissionTbldat, RwUInt8 byGrade)
{
	gui::CSurfaceManager* pSurfaceManager = GetNtlGuiManager()->GetSurfaceManager();

	sCARD* pCard = NTL_NEW sCARD;
	pCard->dwcTblidx = pDwcTbldat->tblidx;
	pCard->pMissionTbldat = pMissionTbldat;
	pCard->byMissionIndex = byMissionIndex;
	pCard->byGrade = byGrade;
	pCard->byState = NUM_CARD_STATE;
	pCard->wClearCount = 0;
	pCard->byRequireCount = pMissionTbldat->asReward[byGrade].byRequireCount;
	pCard->pTexture = NULL;

	CRectangle rect;

	rect.SetRectWH(0, 0, dDWC_CARD_BUTTON_WIDTH, dDWC_CARD_BUTTON_HEIGHT);
	pCard->pbtnCard = NTL_NEW gui::CButton(rect, m_ppnlCardList, pSurfaceManager);
	pCard->slotMouseEnter = pCard->pbtnCard->SigMouseEnter().Connect(this, &CDwcChallengeGui::OnMouseEnterCard);
	pCard->slotMouseLeave = pCard->pbtnCard->SigMouseLeave().Connect(this, &CDwcChallengeGui::OnMouseLeaveCard);
	pCard->slotClicked = pCard->pbtnCard->SigClicked().Connect(this, &CDwcChallengeGui::OnClickCard);
	pCard->uiNumber = 0;

	rect.SetRectWH(0, 0, dDWC_CARD_NUMBER_WIDTH, dDWC_CARD_NUMBER_HEIGHT);
	pCard->pstbNumber = NTL_NEW gui::CStaticBox(rect, m_ppnlCardList, pSurfaceManager, COMP_TEXT_CENTER);
	pCard->pstbNumber->AddSurface(pSurfaceManager->GetSurface("DWC.srf", "srfNumberBg"));
	pCard->pstbNumber->CreateFontStd(DEFAULT_FONT, 80, DEFAULT_FONT_ATTR);
	pCard->pstbNumber->SetTextColor(dDWC_CARD_NUMBER_COLOR);
	pCard->pstbNumber->Enable(false);

	rect.SetRectWH(0, 0, dDWC_CARD_STAMP_WIDTH, dDWC_CARD_STAMP_HEIGHT);
	pCard->ppnlStamp = NTL_NEW gui::CPanel(rect, m_ppnlCardList, pSurfaceManager);
	pCard->ppnlStamp->Enable(false);

	rect.SetRectWH(dDWC_CARD_GAGE_BG_X, dDWC_CARD_GAGE_BG_Y, dDWC_CARD_GAGE_BG_WIDTH, dDWC_CARD_GAGE_BG_HEIGHT);
	pCard->ppnlGageBg = NTL_NEW gui::CPanel(rect, pCard->pbtnCard, pSurfaceManager, pSurfaceManager->GetSurface("DWC.srf", "srfCardGageBg"));
	pCard->ppnlGageBg->Enable(false);

	rect.SetRectWH(dDWC_CARD_GAGE_X, dDWC_CARD_GAGE_Y, dDWC_CARD_GAGE_WIDTH, dDWC_CARD_GAGE_HEIGHT);
	pCard->ppnlGage = NTL_NEW gui::CPanel(rect, pCard->ppnlGageBg, pSurfaceManager, pSurfaceManager->GetSurface("DWC.srf", "srfCardGage"));
	pCard->ppnlGage->Enable(false);

	pCard->pstbProgress = NTL_NEW gui::CStaticBox(rect, pCard->ppnlGageBg, pSurfaceManager, COMP_TEXT_CENTER | COMP_TEXT_VERTICAL_CENTER);
	pCard->pstbProgress->CreateFontStd("detail", 75, DEFAULT_FONT_ATTR);
	pCard->pstbProgress->SetTextColor(RGB(255, 255, 255));
	pCard->pstbProgress->Enable(false);

	rect.SetRectWH(dDWC_CARD_GRADE_X, dDWC_CARD_GRADE_Y, dDWC_CARD_GRADE_WIDTH, dDWC_CARD_GRADE_HEIGHT);
	pCard->ppnlGrade = NTL_NEW gui::CPanel(rect, pCard->pbtnCard, pSurfaceManager);
	pCard->ppnlGrade->Enable(false);

	// cut to the visible part of the list
	pCard->pbtnCard->SetClippingMode(true);
	pCard->pstbNumber->SetClippingMode(true);
	pCard->ppnlStamp->SetClippingMode(true);
	pCard->ppnlGageBg->SetClippingMode(true);
	pCard->ppnlGage->SetClippingMode(true);
	pCard->pstbProgress->SetClippingMode(true);
	pCard->ppnlGrade->SetClippingMode(true);

	SetCardState(pCard, CARD_STATE_LOCK);

	return pCard;
}

VOID CDwcChallengeGui::DestroyCards(VOID)
{
	for (std::vector<sCARD*>::iterator it = m_vecCard.begin(); it != m_vecCard.end(); ++it)
	{
		sCARD* pCard = *it;

		if (pCard->pTexture)
			Logic_DeleteTexture(pCard->pTexture);

		NTL_DELETE(pCard->pstbProgress);
		NTL_DELETE(pCard->ppnlGage);
		NTL_DELETE(pCard->ppnlGageBg);
		NTL_DELETE(pCard->ppnlGrade);
		NTL_DELETE(pCard->pbtnCard);
		NTL_DELETE(pCard->pstbNumber);
		NTL_DELETE(pCard->ppnlStamp);
		NTL_DELETE(pCard);
	}

	m_vecCard.clear();
}

VOID CDwcChallengeGui::AddComboListSurface(gui::CComboBox* pComboBox, const char* pcPrefix)
{
	// the 9 pieces of the list background, the frm gives the top left one
	static const char* s_apcPiece[] = { "CT", "RT", "LC", "CC", "RC", "LB", "CB", "RB" };

	for (RwInt32 i = 0; i < sizeof(s_apcPiece) / sizeof(s_apcPiece[0]); ++i)
	{
		std::string strSurface = pcPrefix;
		strSurface += s_apcPiece[i];

		pComboBox->GetListBox()->AddSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("DWC.srf", strSurface));
	}
}

VOID CDwcChallengeGui::RefreshScenarioList(VOID)
{
	INT nSelected = m_pcbbScenario->GetSelectedItemIndex();
	DWORD dwSelected = nSelected >= 0 ? m_pcbbScenario->GetSelectedItemData() : INVALID_TBLIDX;

	m_pcbbScenario->ClearAllItems();
	m_pcbbScenario->AddItem(GetDisplayStringManager()->GetString("DST_DWC_SCENARIO_LIST"), INVALID_TBLIDX);

	CDwcTable* pDwcTable = API_GetTableContainer()->GetDwcTable();
	CTextTable* pTextTable = API_GetTableContainer()->GetTextAllTable()->GetDWCTbl();

	INT nSelect = 0;

	for (CTable::TABLEIT it = pDwcTable->Begin(); it != pDwcTable->End(); ++it)
	{
		sDWC_TBLDAT* pDwcTbldat = reinterpret_cast<sDWC_TBLDAT*>(it->second);
		if (pDwcTbldat->tblNameIndex == INVALID_TBLIDX || pDwcTbldat->aMissionTblidx[0] == INVALID_TBLIDX)
			continue;

		// tw: an open scenario gets DST_DWC_SCENARIO_CLEAR, a closed one DST_DWC_SCENARIO_LOCK
		std::wstring wstrName = pTextTable ? pTextTable->GetText(pDwcTbldat->tblNameIndex) : L"";
		wstrName += GetDisplayStringManager()->GetString(Logic_IsDwcEnterable(pDwcTbldat) ? "DST_DWC_SCENARIO_CLEAR" : "DST_DWC_SCENARIO_LOCK");

		INT nIndex = m_pcbbScenario->AddItem(wstrName.c_str(), pDwcTbldat->tblidx);
		if (pDwcTbldat->tblidx == dwSelected)
			nSelect = nIndex;
	}

	m_pcbbScenario->SelectItem(nSelect);
}

VOID CDwcChallengeGui::RefreshCardState(VOID)
{
	CDwcTable* pDwcTable = API_GetTableContainer()->GetDwcTable();
	SDwcInfo* pDwcInfo = GetNtlSLGlobal()->GetDwcInfo();

	for (std::vector<sCARD*>::iterator it = m_vecCard.begin(); it != m_vecCard.end(); ++it)
	{
		sCARD* pCard = *it;

		sDWC_SCENARIO_INFO* pScenario = pDwcInfo->FindScenario(pCard->dwcTblidx);
		RwUInt8 byHeldGrade = pScenario ? pScenario->asMission[pCard->byMissionIndex].byRewardGrade : INVALID_DWC_REWARD_GRADE;
		pCard->wClearCount = pScenario ? pScenario->asMission[pCard->byMissionIndex].wClearCount : 0;

		RwUInt8 byState = CARD_STATE_LOCK;

		if (byHeldGrade != INVALID_DWC_REWARD_GRADE)
		{
			// tw SetMission: the lower grades are cleared, the held one is played (no clear stamp), the next one is open
			if (pCard->byGrade < byHeldGrade)
				byState = CARD_STATE_CLEAR;
			else if (pCard->byGrade == byHeldGrade)
				byState = CARD_STATE_PLAY;
			else if (pCard->byGrade == byHeldGrade + 1)
				byState = CARD_STATE_OPEN;
		}
		else if (pCard->byGrade == DWC_REWARD_GRADE_NORMAL)
		{
			// tw 0xD69370: the normal card of a mission without a card is open (gauge, no lock stamp)
			// once the scenario can be entered, level and admission conditions
			if (Logic_IsDwcEnterable(reinterpret_cast<sDWC_TBLDAT*>(pDwcTable->FindData(pCard->dwcTblidx))))
				byState = CARD_STATE_OPEN;
		}

		SetCardState(pCard, byState);
	}
}

VOID CDwcChallengeGui::SetCardState(sCARD* pCard, RwUInt8 byState)
{
	gui::CSurfaceManager* pSurfaceManager = GetNtlGuiManager()->GetSurfaceManager();

	RwBool bImageChange = (pCard->byState <= CARD_STATE_OPEN) != (byState <= CARD_STATE_OPEN) || pCard->byState == NUM_CARD_STATE;
	pCard->byState = byState;

	// tw: lock stamp, gauge, grade label and clear stamp per state
	pCard->ppnlStamp->ClearSurface();
	pCard->ppnlGrade->ClearSurface();

	switch (byState)
	{
		case CARD_STATE_LOCK:
		{
			pCard->ppnlStamp->AddSurface(pSurfaceManager->GetSurface("DWC.srf", "srfLock"));
			pCard->ppnlGageBg->Show(false);
		}
		break;
		case CARD_STATE_OPEN:
		{
			pCard->ppnlGageBg->Show(true);

			RwInt32 iRequire = pCard->byRequireCount == INVALID_BYTE ? 0 : pCard->byRequireCount;
			RwInt32 iClear = pCard->wClearCount;

			WCHAR awcProgress[32];
			swprintf_s(awcProgress, 32, L"%03d / %03d", iClear, iRequire);
			pCard->pstbProgress->SetText(awcProgress);

			// the gauge surface is cut to the clear rate, a panel does not scale its surfaces
			RwReal fRate = iRequire > 0 ? (RwReal)(iClear < iRequire ? iClear : iRequire) / (RwReal)iRequire : 0.f;

			gui::CSurface gage = pSurfaceManager->GetSurface("DWC.srf", "srfCardGage");
			RwInt32 iWidth = (RwInt32)(gage.m_Original.rtRect.GetWidth() * fRate);

			gage.m_Original.rtRect.right = gage.m_Original.rtRect.left + iWidth;
			gage.m_Original.UVs[2] = gage.m_Original.UVs[0] + (gage.m_Original.UVs[2] - gage.m_Original.UVs[0]) * fRate;
			gage.m_SnapShot = gage.m_Original;

			pCard->ppnlGage->ClearSurface();
			if (iWidth > 0)
				pCard->ppnlGage->AddSurface(gage);
		}
		break;
		case CARD_STATE_PLAY:
		case CARD_STATE_CLEAR:
		{
			pCard->ppnlGageBg->Show(false);
			pCard->ppnlGrade->AddSurface(pSurfaceManager->GetSurface("DWC.srf", s_apcSmallGrade[pCard->byGrade]));

			if (byState == CARD_STATE_CLEAR)
				pCard->ppnlStamp->AddSurface(pSurfaceManager->GetSurface("DWC.srf", "srfClear"));
		}
		break;
	}

	if (bImageChange)
		SetCardImage(pCard);
}

VOID CDwcChallengeGui::SetCardImage(sCARD* pCard)
{
	gui::CSurfaceManager* pSurfaceManager = GetNtlGuiManager()->GetSurfaceManager();
	gui::CButton* pButton = pCard->pbtnCard;

	pButton->GetSurfaceUp()->clear();
	pButton->GetSurfaceDown()->clear();
	pButton->GetSurfaceFocus()->clear();
	pButton->GetSurfaceDisable()->clear();

	if (pCard->pTexture)
	{
		Logic_DeleteTexture(pCard->pTexture);
		pCard->pTexture = NULL;
	}

	CRectangle rtImage(dDWC_CARD_IMAGE_INSET, dDWC_CARD_IMAGE_INSET, dDWC_CARD_BUTTON_WIDTH - dDWC_CARD_IMAGE_INSET, dDWC_CARD_BUTTON_HEIGHT - dDWC_CARD_IMAGE_INSET);

	// tw: a card not won yet is the grey silhouette, a won one the card texture
	gui::CSurface surface;
	if (pCard->byState <= CARD_STATE_OPEN)
	{
		surface = pSurfaceManager->GetSurface("DWC.srf", "srfSmallCard_D");
	}
	else
	{
		std::string strTexture = Logic_GetDwcCardTexture(pCard->pMissionTbldat, pCard->byGrade);
		pCard->pTexture = Logic_CreateTexture((RwChar*)strTexture.c_str(), TEXTYPE_NONE);
		surface.m_pTexture = pCard->pTexture;
	}

	surface.m_Original.rtRect = rtImage;
	surface.m_SnapShot.rtRect = rtImage;

	pButton->AddSurfaceUp(surface);
	pButton->AddSurfaceDown(surface);
	pButton->AddSurfaceDisabled(surface);
	pButton->AddSurfaceFocus(surface);

	// the legendary card has its own frame
	if (pCard->byState > CARD_STATE_OPEN && pCard->byGrade == DWC_REWARD_GRADE_LEGENDARY)
	{
		gui::CSurface outline = pSurfaceManager->GetSurface("DWC.srf", "srfSmallOutLineLegendary");
		outline.m_Original.rtRect = rtImage;
		outline.m_SnapShot.rtRect = rtImage;

		pButton->AddSurfaceUp(outline);
		pButton->AddSurfaceDown(outline);
		pButton->AddSurfaceDisabled(outline);
		pButton->AddSurfaceFocus(outline);
	}

	// the focus frame covers the whole button
	pButton->AddSurfaceFocus(pSurfaceManager->GetSurface("DWC.srf", "srfCarSelectImage"));
}

VOID CDwcChallengeGui::LayoutCards(VOID)
{
	INT nScenario = m_pcbbScenario->GetSelectedItemIndex();
	INT nGrade = m_pcbbGrade->GetSelectedItemIndex();
	INT nState = m_pcbbState->GetSelectedItemIndex();

	DWORD dwScenario = nScenario >= 0 ? m_pcbbScenario->GetSelectedItemData() : INVALID_TBLIDX;
	DWORD dwGrade = nGrade >= 0 ? m_pcbbGrade->GetSelectedItemData() : INVALID_DWC_REWARD_GRADE;
	DWORD dwState = nState >= 0 ? m_pcbbState->GetSelectedItemData() : NUM_CARD_STATE;

	RwInt32 iShown = 0;
	RwInt32 iObtained = 0;

	for (std::vector<sCARD*>::iterator it = m_vecCard.begin(); it != m_vecCard.end(); ++it)
	{
		sCARD* pCard = *it;

		if (pCard->byState == CARD_STATE_PLAY || pCard->byState == CARD_STATE_CLEAR)
			++iObtained;

		RwBool bShow = (dwScenario == INVALID_TBLIDX || dwScenario == pCard->dwcTblidx)
					&& (dwGrade == INVALID_DWC_REWARD_GRADE || dwGrade == pCard->byGrade)
					&& (dwState == NUM_CARD_STATE || dwState == pCard->byState);

		pCard->pbtnCard->Show(bShow ? true : false);
		pCard->pstbNumber->Show(bShow ? true : false);
		pCard->ppnlStamp->Show(bShow ? true : false);

		if (!bShow)
			continue;

		// tw: 7 cards a row, the number box on top, the button and the stamp below it
		RwInt32 iX = dDWC_CARD_MARGIN_X + (iShown % dDWC_CARD_COLUMN) * dDWC_CARD_GAP_X;
		RwInt32 iY = (iShown / dDWC_CARD_COLUMN) * dDWC_CARD_GAP_Y;

		pCard->pstbNumber->SetPosition(iX, iY);
		pCard->pbtnCard->SetPosition(iX + dDWC_CARD_BUTTON_OFFSET_X, iY + dDWC_CARD_NUMBER_HEIGHT);
		pCard->ppnlStamp->SetPosition(iX + dDWC_CARD_STAMP_OFFSET_X, iY + dDWC_CARD_NUMBER_HEIGHT);

		++iShown;
		pCard->pstbNumber->Format(L"No.%03d", iShown);
		pCard->uiNumber = iShown;
	}

	// tw: the list scrolls by pixel past 3 rows
	RwInt32 iRow = (iShown + dDWC_CARD_COLUMN - 1) / dDWC_CARD_COLUMN;
	m_ppnlCardList->SetHeight(iRow * dDWC_CARD_GAP_Y);
	// SetHeight keeps the old clipping rect, a move recalculates it for the panel and every child
	m_ppnlCardList->SetPosition(m_ppnlCardList->GetPosition().left, m_iListTop + 1);
	m_ppnlCardList->SetPosition(m_ppnlCardList->GetPosition().left, m_iListTop);

	if (iRow > dDWC_CARD_VISIBLE_ROW)
	{
		m_pscbScroll->SetRange(0, (iRow - dDWC_CARD_VISIBLE_ROW) * dDWC_CARD_GAP_Y);
		m_pscbScroll->Show(true);
	}
	else
	{
		m_pscbScroll->SetRange(0, 0);
		m_pscbScroll->Show(false);
	}

	m_pscbScroll->SetValue(0, FALSE);

	// tw: the selected scenario, the cards shown and the cards won of all
	m_pstbDwcTitle->SetText(m_pcbbScenario->GetSelectedItemText().c_str());
	m_pstbText->SetText(iShown);
	m_pstbDwcPoint->Format(L" %d / %d", iObtained, (RwInt32)m_vecCard.size());
}

VOID CDwcChallengeGui::Refresh(VOID)
{
	RefreshCardState();
	LayoutCards();
}

VOID CDwcChallengeGui::OnClickClose(gui::CComponent* pComponent)
{
	GetDialogManager()->CloseDialog(DIALOG_DWC_CHALLENGE);
}

VOID CDwcChallengeGui::OnSelectFilter(INT nIndex)
{
	LayoutCards();
}

VOID CDwcChallengeGui::OnScrollChanged(RwInt32 iValue)
{
	m_ppnlCardList->SetPosition(m_ppnlCardList->GetPosition().left, m_iListTop - iValue);
}

VOID CDwcChallengeGui::OnMouseWheel(RwInt32 nFlag, RwInt16 sDelta, CPos& pos)
{
	if (!IsShow() || !m_pscbScroll->IsVisible())
		return;

	if (m_pThis->GetParent()->GetChildComponentReverseAt(pos.x, pos.y) != m_pThis)
		return;

	if (m_pThis->PosInRect(pos.x, pos.y) != gui::CComponent::INRECT)
		return;

	// tw: 18 pixels per wheel notch
	RwInt32 iValue = m_pscbScroll->GetValue() - sDelta * dDWC_CARD_WHEEL_STEP / GUI_MOUSE_WHEEL_DELTA;
	if (iValue < 0)
		iValue = 0;
	else if (iValue > m_pscbScroll->GetMaxValue())
		iValue = m_pscbScroll->GetMaxValue();

	m_pscbScroll->SetValue(iValue);
}

VOID CDwcChallengeGui::OnMouseEnterCard(gui::CComponent* pComponent)
{
	for (std::vector<sCARD*>::iterator it = m_vecCard.begin(); it != m_vecCard.end(); ++it)
	{
		sCARD* pCard = *it;
		if (pCard->pbtnCard != pComponent)
			continue;

		// card name, grade and state
		m_wstrTooltip.clear();

		if (CTextTable* pTextTable = API_GetTableContainer()->GetTextAllTable()->GetDWCTbl())
			m_wstrTooltip = pTextTable->GetText(pCard->pMissionTbldat->asReward[pCard->byGrade].cardNameTblidx);

		static const char* s_apcGradeName[MAX_DWC_REWARD_GRADE_COUNT] = { "DST_DWC_CARD_GRADE_01", "DST_DWC_CARD_GRADE_02", "DST_DWC_CARD_GRADE_03", "DST_DWC_CARD_GRADE_04", "DST_DWC_CARD_GRADE_05" };
		static const char* s_apcStateName[NUM_CARD_STATE] = { "DST_DWC_STATE_LOCK", "DST_DWC_STATE_OPEN", "DST_DWC_STATE_PLAY", "DST_DWC_STATE_CLEAR" };

		m_wstrTooltip += L"\n";
		m_wstrTooltip += GetDisplayStringManager()->GetString(s_apcGradeName[pCard->byGrade]);
		m_wstrTooltip += L" / ";
		m_wstrTooltip += GetDisplayStringManager()->GetString(s_apcStateName[pCard->byState]);

		CRectangle rtScreen = pCard->pbtnCard->GetScreenRect();
		GetInfoWndManager()->ShowInfoWindow(TRUE, CInfoWndManager::INFOWND_JUST_WTEXT, rtScreen.right, rtScreen.top, (VOID*)m_wstrTooltip.c_str(), DIALOG_DWC_CHALLENGE);
		return;
	}
}

VOID CDwcChallengeGui::OnMouseLeaveCard(gui::CComponent* pComponent)
{
	if (GetInfoWndManager()->GetRequestGui() == DIALOG_DWC_CHALLENGE)
		GetInfoWndManager()->ShowInfoWindow(FALSE);
}

VOID CDwcChallengeGui::OnSelectScenario(INT nIndex)
{
	LayoutCards();

	// tw: a scenario opens its details, the card details close
	DWORD dwScenario = m_pcbbScenario->GetSelectedItemData();
	if (nIndex < 0 || dwScenario == INVALID_TBLIDX)
		return;

	if (GetDialogManager()->IsOpenDialog(DIALOG_DWC_HEROINFO))
		GetDialogManager()->CloseDialog(DIALOG_DWC_HEROINFO);

	CDwcScenarioInfoGui* pScenarioInfo = static_cast<CDwcScenarioInfoGui*>(GetDialogManager()->GetDialog(DIALOG_DWC_SCENARIO_INFO));
	if (pScenarioInfo == NULL)
		return;

	pScenarioInfo->SetScenario(dwScenario);

	if (!GetDialogManager()->IsOpenDialog(DIALOG_DWC_SCENARIO_INFO))
		GetDialogManager()->OpenDialog(DIALOG_DWC_SCENARIO_INFO);

	// tw: the details stand on the right of the challenge
	CRectangle rtChallenge = GetPosition();
	pScenarioInfo->SetPosition(rtChallenge.right, rtChallenge.top);
}

VOID CDwcChallengeGui::OnClickCard(gui::CComponent* pComponent)
{
	for (std::vector<sCARD*>::iterator it = m_vecCard.begin(); it != m_vecCard.end(); ++it)
	{
		sCARD* pCard = *it;
		if (pCard->pbtnCard != pComponent)
			continue;

		// tw: a card opens its details, the scenario details close
		if (GetDialogManager()->IsOpenDialog(DIALOG_DWC_SCENARIO_INFO))
			GetDialogManager()->CloseDialog(DIALOG_DWC_SCENARIO_INFO);

		CDwcHeroInfoGui* pHeroInfo = static_cast<CDwcHeroInfoGui*>(GetDialogManager()->GetDialog(DIALOG_DWC_HEROINFO));
		if (pHeroInfo == NULL)
			return;

		sDWC_SCENARIO_INFO* pScenario = GetNtlSLGlobal()->GetDwcInfo()->FindScenario(pCard->dwcTblidx);
		RwUInt8 byHeldGrade = pScenario ? pScenario->asMission[pCard->byMissionIndex].byRewardGrade : INVALID_DWC_REWARD_GRADE;
		RwBool bWon = pCard->byState == CARD_STATE_PLAY || pCard->byState == CARD_STATE_CLEAR;

		pHeroInfo->SetCard(pCard->dwcTblidx, pCard->pMissionTbldat, pCard->byGrade, byHeldGrade, bWon, pCard->uiNumber);

		if (!GetDialogManager()->IsOpenDialog(DIALOG_DWC_HEROINFO))
			GetDialogManager()->OpenDialog(DIALOG_DWC_HEROINFO);

		// tw: the details stand on the right of the challenge
		CRectangle rtChallenge = GetPosition();
		pHeroInfo->SetPosition(rtChallenge.right, rtChallenge.top);

		return;
	}
}
