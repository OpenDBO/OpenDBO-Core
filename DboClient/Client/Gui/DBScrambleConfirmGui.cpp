#include "precomp_dboclient.h"
#include "DBScrambleConfirmGui.h"

// core
#include "NtlDebug.h"

// presentation
#include "NtlPLGuiManager.h"

// dbo
#include "DialogManager.h"
#include "AlarmManager.h"
#include "DisplayStringManager.h"


CDBScrambleConfirmGui::CDBScrambleConfirmGui(const RwChar* pName)
	: CNtlPLGui(pName)
{
}

CDBScrambleConfirmGui::~CDBScrambleConfirmGui()
{
}

RwBool CDBScrambleConfirmGui::Create()
{
	NTL_FUNCTION("CDBScrambleConfirmGui::Create");

	if (!CNtlPLGui::Create("gui\\DragonBallScrambleConfirm.rsr", "gui\\DragonBallScrambleConfirm.srf", "gui\\DragonBallScrambleConfirm.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");

	SetText("stbTitle", "DST_DRAGONBALL_SCRAMBLE_CONFIRM_TITLE");
	SetText("stbDBSGetGuideTitle", "DST_DRAGONBALL_SCRAMBLE_CONFIRM_GET_TITLE");
	SetText("stbDBSGetGuide_1", "DST_DRAGONBALL_SCRAMBLE_CONFIRM_GET_1");
	SetText("stbDBSGetGuide_2", "DST_DRAGONBALL_SCRAMBLE_CONFIRM_GET_2");
	SetText("stbDBSGetGuide_3", "DST_DRAGONBALL_SCRAMBLE_CONFIRM_GET_3");
	SetText("stbDBSDropGuideTitle", "DST_DRAGONBALL_SCRAMBLE_CONFIRM_DROP_TITLE");
	SetText("stbDBSDropGuide_1", "DST_DRAGONBALL_SCRAMBLE_CONFIRM_DROP_1");
	SetText("stbDBSDropGuide_2", "DST_DRAGONBALL_SCRAMBLE_CONFIRM_DROP_2");
	SetText("stbDBSDropGuide_3", "DST_DRAGONBALL_SCRAMBLE_CONFIRM_DROP_3");
	SetText("stbDBSTotalGuideTitle", "DST_DRAGONBALL_SCRAMBLE_CONFIRM_WARNING_TITLE");
	SetText("stbDBSTotalGuide", "DST_DRAGONBALL_SCRAMBLE_CONFIRM_WARNINR");

	gui::CButton* pBtnOk = (gui::CButton*)GetComponent("btnOk");
	gui::CButton* pBtnCancel = (gui::CButton*)GetComponent("btnCancel");
	gui::CButton* pBtnClose = (gui::CButton*)GetComponent("btnClose");
	if (!pBtnOk || !pBtnCancel || !pBtnClose)
		NTL_RETURN(FALSE);

	pBtnOk->SetText(GetDisplayStringManager()->GetString("DST_DRAGONBALL_SCRAMBLE_CONFIRM_OK"));
	pBtnCancel->SetText(GetDisplayStringManager()->GetString("DST_DRAGONBALL_SCRAMBLE_CONFIRM_CANCEL"));

	m_slotOk = pBtnOk->SigClicked().Connect(this, &CDBScrambleConfirmGui::OnOkButtonClicked);
	m_slotCancel = pBtnCancel->SigClicked().Connect(this, &CDBScrambleConfirmGui::OnCloseButtonClicked);
	m_slotClose = pBtnClose->SigClicked().Connect(this, &CDBScrambleConfirmGui::OnCloseButtonClicked);

	Show(false);

	NTL_RETURN(TRUE);
}

VOID CDBScrambleConfirmGui::Destroy()
{
	NTL_FUNCTION("CDBScrambleConfirmGui::Destroy");

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();

	NTL_RETURNVOID();
}

RwInt32 CDBScrambleConfirmGui::SwitchDialog(bool bOpen)
{
	Show(bOpen);

	return 1;
}

void CDBScrambleConfirmGui::SetText(const RwChar* pcComponent, const RwChar* pcStringKey)
{
	gui::CStaticBox* pStatic = (gui::CStaticBox*)GetComponent(pcComponent);
	if (pStatic)
		pStatic->SetText(GetDisplayStringManager()->GetString(pcStringKey));
}

void CDBScrambleConfirmGui::OnOkButtonClicked(gui::CComponent* pComponent)
{
	GetDialogManager()->CloseDialog(DIALOG_DBSCRAMBLE_CONFIRM);

	// last confirmation. The join request is sent from CMsgBoxManager
	GetAlarmManager()->AlarmMessage("DST_DRAGONBALL_SCRAMBLE_JOIN_CONFIRM_MSGBOX");
}

void CDBScrambleConfirmGui::OnCloseButtonClicked(gui::CComponent* pComponent)
{
	GetDialogManager()->CloseDialog(DIALOG_DBSCRAMBLE_CONFIRM);
}
