#include "precomp_dboclient.h"
#include "DwcScenarioMainGui.h"

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

// dbo
#include "DboLogic.h"
#include "DialogManager.h"
#include "DisplayStringManager.h"

// tw: the line colors of the dwc, the required mission and the optional missions
#define dDWC_SCENARIO_TARGET_LINE		L"- [font size = \"10\" color = \"f4d762\"]"
#define dDWC_SCENARIO_ESSENTIAL_LINE	L"- [font size = \"10\" color = \"f36a7c\"]"
#define dDWC_SCENARIO_CHOOSE_LINE		L"- [font size = \"10\" color = \"ffff00\"]"
#define dDWC_SCENARIO_LINE_END			L"[/font]"
#define dDWC_SCENARIO_LINE_BREAK		L"[/font][br]"
#define dDWC_SCENARIO_LINE_SPACE		3


CDwcScenarioMainGui::CDwcScenarioMainGui(const RwChar* pName)
: CNtlPLGui(pName)
{
}

CDwcScenarioMainGui::~CDwcScenarioMainGui(VOID)
{
}

RwBool CDwcScenarioMainGui::Create(VOID)
{
	NTL_FUNCTION("CDwcScenarioMainGui::Create");

	if (!CNtlPLGui::Create("gui\\DWC.rsr", "gui\\DWC.srf", "gui\\DWC_ScenarioMain.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(GetNtlGuiManager()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");
	m_pThis->SetPriority(dDIALOGPRIORITY_DEFAULT);

	m_pstbTitle = (gui::CStaticBox*)GetComponent("stbTitle");
	m_pstbTargetTitle = (gui::CStaticBox*)GetComponent("stbTargetTitle");
	m_pstbEssentialTitle = (gui::CStaticBox*)GetComponent("stbEssentialTitle");
	m_pstbChooseTitle = (gui::CStaticBox*)GetComponent("stbChooseTitle");
	m_phbxTargetText = (gui::CHtmlBox*)GetComponent("hbxTargetText");
	m_phbxEssentialText = (gui::CHtmlBox*)GetComponent("hbxEssentialText");
	m_phbxChooseText = (gui::CHtmlBox*)GetComponent("hbxChooseText");
	m_pbtnClose = (gui::CButton*)GetComponent("btnClose");

	m_pstbTitle->SetText(GetDisplayStringManager()->GetString("DST_DWC_TITLE"));
	m_pstbTargetTitle->SetText(GetDisplayStringManager()->GetString("DST_DWC_TARGET_TITLE"));
	m_pstbEssentialTitle->SetText(GetDisplayStringManager()->GetString("DST_DWC_ESSENTIAL_TITLE"));
	m_pstbChooseTitle->SetText(GetDisplayStringManager()->GetString("DST_DWC_CHOOSE_TITLE"));

	m_phbxTargetText->SetLineSpace(dDWC_SCENARIO_LINE_SPACE);
	m_phbxEssentialText->SetLineSpace(dDWC_SCENARIO_LINE_SPACE);
	m_phbxChooseText->SetLineSpace(dDWC_SCENARIO_LINE_SPACE);

	m_slotClickClose = m_pbtnClose->SigClicked().Connect(this, &CDwcScenarioMainGui::OnClickClose);

	Show(false);

	NTL_RETURN(TRUE);
}

VOID CDwcScenarioMainGui::Destroy(VOID)
{
	NTL_FUNCTION("CDwcScenarioMainGui::Destroy");

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();

	NTL_RETURNVOID();
}

RwInt32 CDwcScenarioMainGui::SwitchDialog(bool bOpen)
{
	if (bOpen)
	{
		// only the dwc the avatar is in has objectives to show
		if (Logic_GetAvatarDwc() == NULL)
			return -1;

		Refresh();
	}

	Show(bOpen);
	return TRUE;
}

VOID CDwcScenarioMainGui::HandleEvents(RWS::CMsg& msg)
{
}

VOID CDwcScenarioMainGui::Refresh(VOID)
{
	sDWC_TBLDAT* pDwcTbldat = Logic_GetAvatarDwc();
	if (pDwcTbldat == NULL)
		return;

	CDwcMissionTable* pMissionTable = API_GetTableContainer()->GetDwcMissionTable();

	// tw: the dwc name
	std::wstring wstrHtml = dDWC_SCENARIO_TARGET_LINE;
	wstrHtml += Logic_GetDwcText(pDwcTbldat->tblNameIndex);
	wstrHtml += dDWC_SCENARIO_LINE_END;
	SetHtml(m_phbxTargetText, wstrHtml);

	// tw: the first mission has to be cleared
	wstrHtml.clear();
	sDWCMISSION_TBLDAT* pMissionTbldat = reinterpret_cast<sDWCMISSION_TBLDAT*>(pMissionTable->FindData(pDwcTbldat->aMissionTblidx[0]));
	if (pMissionTbldat)
	{
		wstrHtml = dDWC_SCENARIO_ESSENTIAL_LINE;
		wstrHtml += Logic_GetDwcText(pMissionTbldat->clearObjectTextTblidx);
		wstrHtml += dDWC_SCENARIO_LINE_END;
	}
	SetHtml(m_phbxEssentialText, wstrHtml);

	// tw: the other missions are optional
	wstrHtml.clear();
	for (RwInt32 i = 1; i < MAX_DWC_MISSION_COUNT_PER_SCENARIO; ++i)
	{
		pMissionTbldat = reinterpret_cast<sDWCMISSION_TBLDAT*>(pMissionTable->FindData(pDwcTbldat->aMissionTblidx[i]));
		if (pMissionTbldat == NULL)
			continue;

		wstrHtml += dDWC_SCENARIO_CHOOSE_LINE;
		wstrHtml += Logic_GetDwcText(pMissionTbldat->clearObjectTextTblidx);
		wstrHtml += dDWC_SCENARIO_LINE_BREAK;
	}
	SetHtml(m_phbxChooseText, wstrHtml);
}

VOID CDwcScenarioMainGui::SetHtml(gui::CHtmlBox* pHtmlBox, const std::wstring& wstrHtml)
{
	pHtmlBox->Clear();

	if (!wstrHtml.empty())
		pHtmlBox->SetHtmlFromMemory(wstrHtml.c_str(), (INT)wstrHtml.size());
}

VOID CDwcScenarioMainGui::OnClickClose(gui::CComponent* pComponent)
{
	GetDialogManager()->CloseDialog(DIALOG_DWC_SCENARIO_MAIN);
}
