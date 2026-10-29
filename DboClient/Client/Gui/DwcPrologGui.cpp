#include "precomp_dboclient.h"
#include "DwcPrologGui.h"

// core
#include "NtlDebug.h"

// shared
#include "DwcTable.h"

// presentation
#include "NtlPLGuiManager.h"

// dbo
#include "DboLogic.h"
#include "DialogManager.h"
#include "DisplayStringManager.h"

// the prologue is shown at once, the pages are turned by the buttons
#define dDWC_PROLOG_TEXT_INTERVAL		0.01f
#define dDWC_PROLOG_LINE_SPACE			5


CDwcPrologGui::CDwcPrologGui(const RwChar* pName)
: CNtlPLGui(pName)
{
}

CDwcPrologGui::~CDwcPrologGui(VOID)
{
}

RwBool CDwcPrologGui::Create(VOID)
{
	NTL_FUNCTION("CDwcPrologGui::Create");

	if (!CNtlPLGui::Create("gui\\DWC.rsr", "gui\\DWC.srf", "gui\\DWC_Prolog.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(GetNtlGuiManager()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");
	m_pThis->SetPriority(dDIALOGPRIORITY_DEFAULT);

	m_pstbTitle = (gui::CStaticBox*)GetComponent("stbTitle");
	m_pstbPrologTitle = (gui::CStaticBox*)GetComponent("stbPrologTitle");
	m_pstbPageInfo = (gui::CStaticBox*)GetComponent("stbPageInfo");
	m_phbxMessage = (gui::CHtmlBox*)GetComponent("hbxMessage");
	m_pbtnClose = (gui::CButton*)GetComponent("btnClose");
	m_pbtnPrev = (gui::CButton*)GetComponent("btnPrev");
	m_pbtnNext = (gui::CButton*)GetComponent("btnNext");

	m_pstbTitle->SetText(GetDisplayStringManager()->GetString("DST_DWC_MAIN_TITLE"));
	m_pstbPrologTitle->SetText(GetDisplayStringManager()->GetString("DST_DWC_PROLOG_TITLE"));

	// tw: the html is split in pages
	m_phbxMessage->SetIntervalTextEnable(dDWC_PROLOG_TEXT_INTERVAL);
	m_phbxMessage->SetLineSpace(dDWC_PROLOG_LINE_SPACE);

	m_slotClickClose = m_pbtnClose->SigClicked().Connect(this, &CDwcPrologGui::OnClickClose);
	m_slotClickPrev = m_pbtnPrev->SigClicked().Connect(this, &CDwcPrologGui::OnClickPrev);
	m_slotClickNext = m_pbtnNext->SigClicked().Connect(this, &CDwcPrologGui::OnClickNext);

	Show(false);

	NTL_RETURN(TRUE);
}

VOID CDwcPrologGui::Destroy(VOID)
{
	NTL_FUNCTION("CDwcPrologGui::Destroy");

	GetNtlGuiManager()->RemoveUpdateFuncWithoutRefCheck(this);

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();

	NTL_RETURNVOID();
}

VOID CDwcPrologGui::Update(RwReal fElapsed)
{
	if (IsShow())
		m_phbxMessage->Update(fElapsed);
}

RwInt32 CDwcPrologGui::SwitchDialog(bool bOpen)
{
	if (bOpen)
	{
		// tw: the prologue of the dwc the avatar is in
		sDWC_TBLDAT* pDwcTbldat = Logic_GetAvatarDwc();
		if (pDwcTbldat == NULL)
			return -1;

		std::wstring wstrHtml = Logic_GetDwcText(pDwcTbldat->prologueTblidx);

		m_phbxMessage->Clear();
		if (!wstrHtml.empty())
			m_phbxMessage->SetHtmlFromMemory(wstrHtml.c_str(), (INT)wstrHtml.size());

		m_phbxMessage->SetImmediate(TRUE);
		SetPageInfo();

		GetNtlGuiManager()->AddUpdateFunc(this);
	}
	else
	{
		GetNtlGuiManager()->RemoveUpdateFunc(this);
	}

	Show(bOpen);
	return TRUE;
}

VOID CDwcPrologGui::HandleEvents(RWS::CMsg& msg)
{
}

VOID CDwcPrologGui::SetPageInfo(VOID)
{
	RwInt32 iPage = m_phbxMessage->GetCurrentPage();
	RwInt32 iPageCount = m_phbxMessage->GetIntervalPageNums();
	if (iPageCount < 1)
		iPageCount = 1;

	m_pstbPageInfo->Format(L"%d / %d", iPage + 1, iPageCount);

	m_pbtnPrev->ClickEnable(iPage > 0);
	m_pbtnNext->ClickEnable(iPage + 1 < iPageCount);
}

VOID CDwcPrologGui::OnClickClose(gui::CComponent* pComponent)
{
	GetDialogManager()->CloseDialog(DIALOG_DWC_PROLOG);
}

VOID CDwcPrologGui::OnClickPrev(gui::CComponent* pComponent)
{
	m_phbxMessage->SetImmediate(TRUE);

	if (m_phbxMessage->SetPrevPage())
		SetPageInfo();
}

VOID CDwcPrologGui::OnClickNext(gui::CComponent* pComponent)
{
	m_phbxMessage->SetImmediate(TRUE);

	if (m_phbxMessage->SetNextPage())
		SetPageInfo();
}
