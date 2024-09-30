#include "precomp_dboclient.h"
#include "MascotSkillReMake.h"

// Core
#include "NtlDebug.h"


// Presetation
#include "NtlPLGuiManager.h"
#include "NtlPLEvent.h"

// Simulation
#include "NtlSLEvent.h"
#include "NtlSLLogic.h"
#include "NtlSLGlobal.h"
#include "NtlSLApi.h"
#include "NtlSLEventFunc.h"
#include "NtlCameraController.h"

// Dbo
#include "DboEvent.h"
#include "DboEventGenerator.h"
#include "DboGlobal.h"
#include "DboLogic.h"
#include "DisplayStringManager.h"
#include "InfoWndManager.h"
#include "DialogManager.h"

CMascotSkillReMake::CMascotSkillReMake(const RwChar* pName)
	:CNtlPLGui(pName)
{
	Init();
}

CMascotSkillReMake::~CMascotSkillReMake()
{

}

VOID CMascotSkillReMake::Init()
{
}

RwBool CMascotSkillReMake::Create()
{
	NTL_FUNCTION("CMascotSkillReMake::Create");

	if (!CNtlPLGui::Create("gui\\MascotEx.rsr", "gui\\MascotEx.srf", "gui\\MascotSkillReMake.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");

	m_Stb_Title = (gui::CStaticBox*)GetComponent("Stb_Title");
	m_Stb_OldSkillName = (gui::CStaticBox*)GetComponent("Stb_OldSkillName");
	m_Stb_NewSkillName = (gui::CStaticBox*)GetComponent("Stb_NewSkillName");
	m_Stb_TypeOld = (gui::CStaticBox*)GetComponent("Stb_TypeOld");
	m_Stb_TypeItem = (gui::CStaticBox*)GetComponent("Stb_TypeItem");
	m_Stb_TypeNew = (gui::CStaticBox*)GetComponent("Stb_TypeNew");
	m_Stb_TypeBefore = (gui::CStaticBox*)GetComponent("Stb_TypeBefore");
	m_Stb_TypeAfter = (gui::CStaticBox*)GetComponent("Stb_TypeAfter");

	m_Btn_Close = (gui::CButton*)GetComponent("Btn_Close");
	m_Btn_MascotSkillRemake = (gui::CButton*)GetComponent("Btn_MascotSkillRemake");

	m_Pgb_LeftGague = (gui::CProgressBar*)GetComponent("Pgb_LeftGague");
	m_Pgb_RightGague = (gui::CProgressBar*)GetComponent("Pgb_RightGague");

	m_Pgb_LeftGague->SetRange(0, 100);
	m_Pgb_RightGague->SetRange(0, 100);
	m_Pgb_LeftGague->SetPos(0);
	m_Pgb_RightGague->SetPos(0);

	m_HtmlBox_Explan = (gui::CHtmlBox*)GetComponent("HtmlBox_Explan");
	const WCHAR* wchHtmlMsg = GetDisplayStringManager()->GetString("DST_MASCOTEX_SKILL_CREATE_EXPALANATION");
	m_HtmlBox_Explan->SetHtmlFromMemory(wchHtmlMsg, wcslen(wchHtmlMsg));
	m_Stb_Title->SetText(GetDisplayStringManager()->GetString("DST_MASCOTEX_SKILL_CREATE_TITLE"));
	m_Stb_OldSkillName->SetText(GetDisplayStringManager()->GetString("DST_NOTTING"));
	m_Stb_NewSkillName->SetText(GetDisplayStringManager()->GetString("DST_NOTTING"));
	m_Stb_TypeOld->SetText(GetDisplayStringManager()->GetString("DST_MASCOTEX_ROLL_HAVESKILL"));
	m_Stb_TypeItem->SetText(GetDisplayStringManager()->GetString("DST_MASCOTEX_ROLL_ROLLITEM"));
	m_Stb_TypeNew->SetText(GetDisplayStringManager()->GetString("DST_MASCOTEX_ROLL_ROLLSKILL"));
	m_Stb_TypeBefore->SetText(GetDisplayStringManager()->GetString("DST_MASCOTEX_ROLL_HAVESKILL_TITLE"));
	m_Stb_TypeAfter->SetText(GetDisplayStringManager()->GetString("DST_MASCOTEX_ROLL_ROLLSKILL_TITLE"));
	m_Btn_MascotSkillRemake->SetText(GetDisplayStringManager()->GetString("DST_MASCOTEX_SKILL_CREATE_BUTTON"));

	m_slotCloseButton = m_Btn_Close->SigClicked().Connect(this, &CMascotSkillReMake::ClickedCloseButton);

	Show(false);

	NTL_RETURN(TRUE);
}

VOID CMascotSkillReMake::Destroy()
{
	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();
}

VOID CMascotSkillReMake::HandleEvents(RWS::CMsg& msg)
{

}

RwInt32 CMascotSkillReMake::SwitchDialog(bool bOpen)
{
	Show(bOpen);
	return 1;
}

VOID CMascotSkillReMake::ClickedCloseButton(gui::CComponent* pComponent)
{
	GetDialogManager()->CloseDialog(DIALOG_MASCOT_SKILL_REMAKE);
}