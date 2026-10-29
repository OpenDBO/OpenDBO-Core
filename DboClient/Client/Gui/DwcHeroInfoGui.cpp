#include "precomp_dboclient.h"
#include "DwcHeroInfoGui.h"

// core
#include "NtlDebug.h"

// shared
#include "CharTitleTable.h"
#include "DwcMissionTable.h"
#include "DwcTable.h"
#include "ItemTable.h"
#include "TableContainer.h"
#include "TextAllTable.h"

// presentation
#include "NtlPLGuiManager.h"

// simulation
#include "NtlSLApi.h"

// dbo
#include "DboGlobal.h"
#include "DboLogic.h"
#include "DialogManager.h"
#include "DisplayStringManager.h"

// tw: the sections close to their 20 pixel title, the view scrolls past 436 pixels
#define dDWC_HERO_SECTION_HEIGHT		20
#define dDWC_HERO_SECTION_GAP			2
#define dDWC_HERO_SECTION_BOTTOM		6
#define dDWC_HERO_VIEW_HEIGHT			436
#define dDWC_HERO_WHEEL_STEP			18
#define dDWC_HERO_REWARD_GAP			5

// the 3 pieces of srfMissionWindow
#define dDWC_HERO_FRAME_TOP				5
#define dDWC_HERO_FRAME_BOTTOM			5

// the md static boxes do not wrap, the lines are cut at about this many letters
#define dDWC_HERO_LINE_LETTERS			34

#define dDWC_HERO_TEXT_COLOR			RGB(255, 255, 255)
#define dDWC_HERO_REWARD_COLOR			RGB(255, 219, 71)

static const char* s_apcLargeGrade[MAX_DWC_REWARD_GRADE_COUNT] = { "srfLargeNormal", "srfLargeSuperior", "srfLargeExcellent", "srfLargeRare", "srfLargeLegendary" };
static const char* s_apcOutLine[MAX_DWC_REWARD_GRADE_COUNT] = { "srfOutLineNormal", "srfOutLineSuperior", "srfOutLineExcellent", "srfOutLineRare", "srfOutLineLegendary" };
static const char* s_apcGradeEnable[MAX_DWC_REWARD_GRADE_COUNT] = { "srfLvNormalEnable", "srfLvSuperiorEnable", "srfLvExcellentEnable", "srfLvRareEnable", "srfLvLegendaryEnable" };
static const char* s_apcGradeDisable[MAX_DWC_REWARD_GRADE_COUNT] = { "srfLvNormalDisable", "srfLvSuperiorDisable", "srfLvExcellentDisable", "srfLvRareDisable", "srfLvLegendaryDisable" };


CDwcHeroInfoGui::CDwcHeroInfoGui(const RwChar* pName)
: CNtlPLGui(pName)
{
	m_pCardTexture = NULL;
	m_iViewTop = 0;
	m_bConditionExpand = FALSE;
	m_bRewardExpand = FALSE;
}

CDwcHeroInfoGui::~CDwcHeroInfoGui(VOID)
{
}

RwBool CDwcHeroInfoGui::Create(VOID)
{
	NTL_FUNCTION("CDwcHeroInfoGui::Create");

	if (!CNtlPLGui::Create("gui\\DWC.rsr", "gui\\DWC.srf", "gui\\DWC_HeroInfo.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(GetNtlGuiManager()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");
	m_pThis->SetPriority(dDIALOGPRIORITY_DEFAULT);

	m_pstbTitle = (gui::CStaticBox*)GetComponent("stbTitle");
	m_pstbScenarioTitle = (gui::CStaticBox*)GetComponent("stbScenarioTitle");
	m_pstbHeroName = (gui::CStaticBox*)GetComponent("stbHeroName");
	m_pstbPlayerCountText = (gui::CStaticBox*)GetComponent("stbPlayerCountText");
	m_pstbTargetNameText = (gui::CStaticBox*)GetComponent("stbTargetNameText");
	m_pstbPlayLevelText = (gui::CStaticBox*)GetComponent("stbPlayLevelText");
	m_pstbGradeText = (gui::CStaticBox*)GetComponent("stbGradeText");
	m_pstbPlayerCountValue = (gui::CStaticBox*)GetComponent("stbPlayerCountValue");
	m_pstbTargetNameValue = (gui::CStaticBox*)GetComponent("stbTargetNameValue");
	m_pstbPlayLevelValue = (gui::CStaticBox*)GetComponent("stbPlayLevelValue");
	m_pstbBasicRewardText = (gui::CStaticBox*)GetComponent("stbBasicRewardText");
	m_pstbRepeatRewardText = (gui::CStaticBox*)GetComponent("stbRepeatRewardText");

	m_apnlGrade[DWC_REWARD_GRADE_NORMAL] = (gui::CPanel*)GetComponent("pnlNormalCard");
	m_apnlGrade[DWC_REWARD_GRADE_SUPERIOR] = (gui::CPanel*)GetComponent("pnlSuperiorCard");
	m_apnlGrade[DWC_REWARD_GRADE_EXCELLENT] = (gui::CPanel*)GetComponent("pnlExcellentCard");
	m_apnlGrade[DWC_REWARD_GRADE_RARE] = (gui::CPanel*)GetComponent("pnlRareCard");
	m_apnlGrade[DWC_REWARD_GRADE_LEGENDARY] = (gui::CPanel*)GetComponent("pnlLegendaryCard");

	m_ppnlCard = (gui::CPanel*)GetComponent("pnlCard");
	m_ppnlLevel = (gui::CPanel*)GetComponent("pnlLevel");
	m_ppnlCondition = (gui::CPanel*)GetComponent("pnlCondition");
	m_ppnlReward = (gui::CPanel*)GetComponent("pnlReward");
	m_ppnlScrollView = (gui::CPanel*)GetComponent("pnlScrollView");
	m_iViewTop = m_ppnlScrollView->GetPosition().top;

	m_pmdbCondition = (gui::CMDStaticBox*)GetComponent("mdbCondition");
	m_pmdbBasicReward = (gui::CMDStaticBox*)GetComponent("mdbBasicReward");
	m_pmdbRepeatReward = (gui::CMDStaticBox*)GetComponent("mdbRepeatReward");

	m_pbtnClose = (gui::CButton*)GetComponent("btnClose");
	m_pbtnConditionExpand = (gui::CButton*)GetComponent("btnConditionExpand");
	m_pbtnConditionReduce = (gui::CButton*)GetComponent("btnConditionReduce");
	m_pbtnRewardExpand = (gui::CButton*)GetComponent("btnRewardExpand");
	m_pbtnRewardReduce = (gui::CButton*)GetComponent("btnRewardReduce");
	m_pscbScroll = (gui::CScrollBar*)GetComponent("scbScroll");

	// tw texts
	m_pstbTitle->SetText(GetDisplayStringManager()->GetString("DST_DWC_HERO_TITLE"));
	m_pstbPlayerCountText->SetText(GetDisplayStringManager()->GetString("DST_DWC_PLAYER_COUNT"));
	m_pstbTargetNameText->SetText(GetDisplayStringManager()->GetString("DST_DWC_TARGET"));
	m_pstbPlayLevelText->SetText(GetDisplayStringManager()->GetString("DST_DWC_PLAY_LEVEL"));
	m_pstbGradeText->SetText(GetDisplayStringManager()->GetString("DST_DWC_GRADE"));
	m_pstbBasicRewardText->SetText(GetDisplayStringManager()->GetString("DST_QUESTREWARD_TEXT_BASIC"));
	m_pstbRepeatRewardText->SetText(GetDisplayStringManager()->GetString("DST_DWC_REPEAT_REWARD"));

	m_pbtnConditionExpand->SetText(GetDisplayStringManager()->GetString("DST_DWC_CONDITION"));
	m_pbtnConditionReduce->SetText(GetDisplayStringManager()->GetString("DST_DWC_CONDITION"));
	m_pbtnRewardExpand->SetText(GetDisplayStringManager()->GetString("DST_DWC_REWARD"));
	m_pbtnRewardReduce->SetText(GetDisplayStringManager()->GetString("DST_DWC_REWARD"));

	m_slotClickClose = m_pbtnClose->SigClicked().Connect(this, &CDwcHeroInfoGui::OnClickClose);
	m_slotClickConditionExpand = m_pbtnConditionExpand->SigClicked().Connect(this, &CDwcHeroInfoGui::OnClickConditionExpand);
	m_slotClickConditionReduce = m_pbtnConditionReduce->SigClicked().Connect(this, &CDwcHeroInfoGui::OnClickConditionReduce);
	m_slotClickRewardExpand = m_pbtnRewardExpand->SigClicked().Connect(this, &CDwcHeroInfoGui::OnClickRewardExpand);
	m_slotClickRewardReduce = m_pbtnRewardReduce->SigClicked().Connect(this, &CDwcHeroInfoGui::OnClickRewardReduce);
	m_slotScrollChanged = m_pscbScroll->SigValueChanged().Connect(this, &CDwcHeroInfoGui::OnScrollChanged);
	m_slotScrollMoved = m_pscbScroll->SigSliderMoved().Connect(this, &CDwcHeroInfoGui::OnScrollChanged);
	m_slotMouseWheel = GetNtlGuiManager()->GetGuiManager()->SigCaptureWheelMove().Connect(this, &CDwcHeroInfoGui::OnMouseWheel);

	ExpandCondition(FALSE);
	ExpandReward(FALSE);

	Show(false);

	NTL_RETURN(TRUE);
}

VOID CDwcHeroInfoGui::Destroy(VOID)
{
	NTL_FUNCTION("CDwcHeroInfoGui::Destroy");

	m_ppnlCard->ClearSurface();
	if (m_pCardTexture)
	{
		Logic_DeleteTexture(m_pCardTexture);
		m_pCardTexture = NULL;
	}

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();

	NTL_RETURNVOID();
}

RwInt32 CDwcHeroInfoGui::SwitchDialog(bool bOpen)
{
	Show(bOpen);
	return TRUE;
}

VOID CDwcHeroInfoGui::HandleEvents(RWS::CMsg& msg)
{
}

VOID CDwcHeroInfoGui::SetCard(TBLIDX dwcTblidx, sDWCMISSION_TBLDAT* pMissionTbldat, RwUInt8 byGrade, RwUInt8 byHeldGrade, RwBool bWon, RwUInt32 uiNumber)
{
	if (pMissionTbldat == NULL || byGrade >= MAX_DWC_REWARD_GRADE_COUNT)
		return;

	gui::CSurfaceManager* pSurfaceManager = GetNtlGuiManager()->GetSurfaceManager();
	sDWC_TBLDAT* pDwcTbldat = reinterpret_cast<sDWC_TBLDAT*>(API_GetTableContainer()->GetDwcTable()->FindData(dwcTblidx));
	sDWCMISSION_REWARD& rReward = pMissionTbldat->asReward[byGrade];

	// tw: a section is closed again for every card
	ExpandCondition(FALSE);
	ExpandReward(FALSE);

	// the large card, its frame and the grade name below it
	m_ppnlCard->ClearSurface();
	m_ppnlLevel->ClearSurface();

	if (m_pCardTexture)
	{
		Logic_DeleteTexture(m_pCardTexture);
		m_pCardTexture = NULL;
	}

	CRectangle rtCard(0, 0, m_ppnlCard->GetWidth(), m_ppnlCard->GetHeight());

	if (bWon)
	{
		std::string strTexture = Logic_GetDwcCardTexture(pMissionTbldat, byGrade);
		m_pCardTexture = Logic_CreateTexture((RwChar*)strTexture.c_str(), TEXTYPE_NONE);

		gui::CSurface surface;
		surface.m_pTexture = m_pCardTexture;
		surface.m_Original.rtRect = rtCard;
		surface.m_SnapShot.rtRect = rtCard;
		m_ppnlCard->AddSurface(surface);

		// the frame is 2 pixels larger than the card
		gui::CSurface outline = pSurfaceManager->GetSurface("DWC.srf", s_apcOutLine[byGrade]);
		CRectangle rtOutLine(-1, -1, rtCard.right + 1, rtCard.bottom + 1);
		outline.m_Original.rtRect = rtOutLine;
		outline.m_SnapShot.rtRect = rtOutLine;
		m_ppnlCard->AddSurface(outline);

		m_ppnlLevel->AddSurface(pSurfaceManager->GetSurface("DWC.srf", s_apcLargeGrade[byGrade]));
	}
	else
	{
		gui::CSurface surface = pSurfaceManager->GetSurface("DWC.srf", "srfLargeCard_D");
		surface.m_Original.rtRect = rtCard;
		surface.m_SnapShot.rtRect = rtCard;
		m_ppnlCard->AddSurface(surface);
	}

	WCHAR awcBuffer[256];

	swprintf_s(awcBuffer, 256, L"No.%03d", uiNumber);
	m_pstbScenarioTitle->SetText(awcBuffer);

	m_pstbHeroName->SetText(Logic_GetDwcText(rReward.cardNameTblidx).c_str());

	if (pDwcTbldat)
	{
		swprintf_s(awcBuffer, 256, GetDisplayStringManager()->GetString("DST_PLAYER_COUNT"), pDwcTbldat->byAdmission_Num_Min);
		m_pstbPlayerCountValue->SetText(awcBuffer);
	}

	m_pstbTargetNameValue->SetText(Logic_GetDwcText(pMissionTbldat->clearObjectTextTblidx).c_str());
	m_pstbTargetNameValue->SetToolTip(Logic_GetDwcText(pMissionTbldat->clearObjectTextTblidx));

	// tw: the difficulty of the mission
	static const char* s_apcDifficulty[] = { "DST_TMQ_DEGREE_OF_DIFFICULTY_1", "DST_TMQ_DEGREE_OF_DIFFICULTY_2", "DST_TMQ_DEGREE_OF_DIFFICULTY_3" };
	RwUInt8 byDifficulty = pMissionTbldat->byDifficulty < 3 ? pMissionTbldat->byDifficulty : 2;
	m_pstbPlayLevelValue->SetText(GetDisplayStringManager()->GetString(s_apcDifficulty[byDifficulty]));

	// the grades the avatar holds are lit
	for (RwInt32 i = 0; i < MAX_DWC_REWARD_GRADE_COUNT; ++i)
	{
		RwBool bHeld = byHeldGrade != INVALID_DWC_REWARD_GRADE && i <= byHeldGrade;

		m_apnlGrade[i]->ClearSurface();
		m_apnlGrade[i]->AddSurface(pSurfaceManager->GetSurface("DWC.srf", bHeld ? s_apcGradeEnable[i] : s_apcGradeDisable[i]));
		m_apnlGrade[i]->Show(pMissionTbldat->asReward[i].cardNameTblidx != INVALID_TBLIDX);
	}

	SetMultiLine(m_pmdbCondition, Logic_GetDwcText(pMissionTbldat->clearConditionTextTblidx), dDWC_HERO_TEXT_COLOR);

	SetRewardList(m_pmdbBasicReward, rReward.asBasicReward);
	SetRewardList(m_pmdbRepeatReward, rReward.asRepeatReward);

	RelocateSections();
}

VOID CDwcHeroInfoGui::SetFrameHeight(gui::CPanel* pPanel, RwInt32 iHeight)
{
	// srfMissionWindow is a top, a stretched middle and a bottom piece
	gui::CSurfaceManager* pSurfaceManager = GetNtlGuiManager()->GetSurfaceManager();

	gui::CSurface top = pSurfaceManager->GetSurface("DWC.srf", "srfMissionWindow01");
	gui::CSurface middle = pSurfaceManager->GetSurface("DWC.srf", "srfMissionWindow02");
	gui::CSurface bottom = pSurfaceManager->GetSurface("DWC.srf", "srfMissionWindow03");

	RwInt32 iWidth = pPanel->GetWidth();

	top.m_Original.rtRect.SetRect(0, 0, iWidth, dDWC_HERO_FRAME_TOP);
	middle.m_Original.rtRect.SetRect(0, dDWC_HERO_FRAME_TOP, iWidth, iHeight - dDWC_HERO_FRAME_BOTTOM);
	bottom.m_Original.rtRect.SetRect(0, iHeight - dDWC_HERO_FRAME_BOTTOM, iWidth, iHeight);

	top.m_SnapShot.rtRect = top.m_Original.rtRect;
	middle.m_SnapShot.rtRect = middle.m_Original.rtRect;
	bottom.m_SnapShot.rtRect = bottom.m_Original.rtRect;

	pPanel->SetHeight(iHeight);
	pPanel->ClearSurface();
	pPanel->AddSurface(top);
	pPanel->AddSurface(middle);
	pPanel->AddSurface(bottom);
}

VOID CDwcHeroInfoGui::SetMultiLine(gui::CMDStaticBox* pBox, const std::wstring& wstrText, COLORREF color)
{
	pBox->Clear();

	RwInt32 iLine = 0;
	AddWrappedLines(pBox, wstrText, color, iLine);

	pBox->DrawItem();
}

VOID CDwcHeroInfoGui::AddWrappedLines(gui::CMDStaticBox* pBox, const std::wstring& wstrText, COLORREF color, RwInt32& iLine)
{
	// cut at a blank before the letter limit
	std::wstring wstrRest = wstrText;
	char acName[32];

	while (!wstrRest.empty())
	{
		std::wstring wstrLine;

		if ((RwInt32)wstrRest.size() <= dDWC_HERO_LINE_LETTERS)
		{
			wstrLine = wstrRest;
			wstrRest.clear();
		}
		else
		{
			std::wstring::size_type pos = wstrRest.rfind(L' ', dDWC_HERO_LINE_LETTERS);
			if (pos == std::wstring::npos || pos == 0)
				pos = dDWC_HERO_LINE_LETTERS;

			wstrLine = wstrRest.substr(0, pos);
			wstrRest = wstrRest.substr(pos);

			while (!wstrRest.empty() && wstrRest[0] == L' ')
				wstrRest.erase(0, 1);
		}

		sprintf_s(acName, 32, "name%d", ++iLine);
		pBox->SetItem(wstrLine.c_str(), acName, 0, COMP_TEXT_LEFT, color);
	}
}

VOID CDwcHeroInfoGui::SetRewardList(gui::CMDStaticBox* pBox, sDWCMISSION_REWARD_ENTITY* pEntity)
{
	pBox->Clear();

	CItemTable* pItemTable = API_GetTableContainer()->GetItemTable();
	CCharTitleTable* pTitleTable = API_GetTableContainer()->GetCharTitleTable();
	CTextAllTable* pTextAll = API_GetTableContainer()->GetTextAllTable();

	RwInt32 iLine = 0;

	for (RwInt32 i = 0; i < MAX_DWC_REWARD_COUNT; ++i)
	{
		if (pEntity[i].tblidx == INVALID_TBLIDX)
			continue;

		std::wstring wstrLine;

		// tw: "Item <name> <count> items" or "Title <name>"
		if (pEntity[i].byType == DWC_REWARD_ITEM)
		{
			wstrLine = GetDisplayStringManager()->GetString("DST_ITEM");
			wstrLine += L" ";

			sITEM_TBLDAT* pItemTbldat = reinterpret_cast<sITEM_TBLDAT*>(pItemTable->FindData(pEntity[i].tblidx));
			if (pItemTbldat)
				wstrLine += pTextAll->GetItemTbl()->GetText(pItemTbldat->Name);

			WCHAR awcCount[64];
			swprintf_s(awcCount, 64, GetDisplayStringManager()->GetString("DST_ITEM_COUNT"), pEntity[i].byValue);
			wstrLine += awcCount;
		}
		else if (pEntity[i].byType == DWC_REWARD_TITLE)
		{
			wstrLine = GetDisplayStringManager()->GetString("DST_CHAR_TITLE");
			wstrLine += L" ";

			sCHARTITLE_TBLDAT* pTitleTbldat = reinterpret_cast<sCHARTITLE_TBLDAT*>(pTitleTable->FindData(pEntity[i].tblidx));
			if (pTitleTbldat)
				wstrLine += pTextAll->GetCharTitleTbl()->GetText(pTitleTbldat->tblNameIndex);
		}
		else
		{
			continue;
		}

		// a long item name is wrapped like the condition text
		AddWrappedLines(pBox, wstrLine, dDWC_HERO_REWARD_COLOR, iLine);
	}

	pBox->DrawItem();
}

VOID CDwcHeroInfoGui::ExpandCondition(RwBool bExpand)
{
	m_bConditionExpand = bExpand;

	m_pbtnConditionExpand->Show(!bExpand);
	m_pbtnConditionReduce->Show(bExpand ? true : false);
	m_pmdbCondition->Show(bExpand ? true : false);

	RelocateSections();
}

VOID CDwcHeroInfoGui::ExpandReward(RwBool bExpand)
{
	m_bRewardExpand = bExpand;

	m_pbtnRewardExpand->Show(!bExpand);
	m_pbtnRewardReduce->Show(bExpand ? true : false);
	m_pstbBasicRewardText->Show(bExpand ? true : false);
	m_pmdbBasicReward->Show(bExpand ? true : false);
	m_pstbRepeatRewardText->Show(bExpand ? true : false);
	m_pmdbRepeatReward->Show(bExpand ? true : false);

	RelocateSections();
}

VOID CDwcHeroInfoGui::RelocateSections(VOID)
{
	// condition: the title, or the title and the condition text
	RwInt32 iConditionHeight = dDWC_HERO_SECTION_HEIGHT;
	if (m_bConditionExpand)
		iConditionHeight = m_pmdbCondition->GetPosition().bottom + dDWC_HERO_SECTION_BOTTOM;

	SetFrameHeight(m_ppnlCondition, iConditionHeight);

	// tw: the reward follows the condition
	CRectangle rtCondition = m_ppnlCondition->GetPosition();
	m_ppnlReward->SetPosition(rtCondition.left, rtCondition.bottom + dDWC_HERO_SECTION_GAP);

	// reward: the basic list, then the repeat list under it
	RwInt32 iRewardHeight = dDWC_HERO_SECTION_HEIGHT;
	if (m_bRewardExpand)
	{
		CRectangle rtBasic = m_pmdbBasicReward->GetPosition();
		m_pstbRepeatRewardText->SetPosition(m_pstbRepeatRewardText->GetPosition().left, rtBasic.bottom + dDWC_HERO_REWARD_GAP);

		CRectangle rtRepeatText = m_pstbRepeatRewardText->GetPosition();
		m_pmdbRepeatReward->SetPosition(m_pmdbRepeatReward->GetPosition().left, rtRepeatText.bottom + 1);

		iRewardHeight = m_pmdbRepeatReward->GetPosition().bottom + dDWC_HERO_REWARD_GAP;
	}

	SetFrameHeight(m_ppnlReward, iRewardHeight);

	// the view is as tall as its content, it scrolls once that passes the frame
	RwInt32 iViewHeight = m_ppnlReward->GetPosition().bottom;
	m_ppnlScrollView->SetHeight(iViewHeight);

	RwInt32 iMax = iViewHeight - dDWC_HERO_VIEW_HEIGHT;
	m_pscbScroll->SetRange(0, iMax > 0 ? iMax : 0);
	m_pscbScroll->SetValue(0, FALSE);

	// SetHeight keeps the old clipping rect, a move recalculates it for the view and every child
	m_ppnlScrollView->SetPosition(m_ppnlScrollView->GetPosition().left, m_iViewTop + 1);
	m_ppnlScrollView->SetPosition(m_ppnlScrollView->GetPosition().left, m_iViewTop);
}

VOID CDwcHeroInfoGui::OnClickClose(gui::CComponent* pComponent)
{
	GetDialogManager()->CloseDialog(DIALOG_DWC_HEROINFO);
}

VOID CDwcHeroInfoGui::OnClickConditionExpand(gui::CComponent* pComponent)
{
	ExpandCondition(TRUE);
}

VOID CDwcHeroInfoGui::OnClickConditionReduce(gui::CComponent* pComponent)
{
	ExpandCondition(FALSE);
}

VOID CDwcHeroInfoGui::OnClickRewardExpand(gui::CComponent* pComponent)
{
	ExpandReward(TRUE);
}

VOID CDwcHeroInfoGui::OnClickRewardReduce(gui::CComponent* pComponent)
{
	ExpandReward(FALSE);
}

VOID CDwcHeroInfoGui::OnScrollChanged(RwInt32 iValue)
{
	m_ppnlScrollView->SetPosition(m_ppnlScrollView->GetPosition().left, m_iViewTop - iValue);
}

VOID CDwcHeroInfoGui::OnMouseWheel(RwInt32 nFlag, RwInt16 sDelta, CPos& pos)
{
	if (!IsShow() || m_pscbScroll->GetMaxValue() <= 0)
		return;

	if (m_pThis->GetParent()->GetChildComponentReverseAt(pos.x, pos.y) != m_pThis)
		return;

	if (m_pThis->PosInRect(pos.x, pos.y) != gui::CComponent::INRECT)
		return;

	RwInt32 iValue = m_pscbScroll->GetValue() - sDelta * dDWC_HERO_WHEEL_STEP / GUI_MOUSE_WHEEL_DELTA;
	if (iValue < 0)
		iValue = 0;
	else if (iValue > m_pscbScroll->GetMaxValue())
		iValue = m_pscbScroll->GetMaxValue();

	m_pscbScroll->SetValue(iValue);
}
