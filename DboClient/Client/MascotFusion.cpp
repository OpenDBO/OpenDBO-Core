#include "precomp_dboclient.h"
#include "MascotFusion.h"

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

CMascotFusion::CMascotFusion(const RwChar* pName)
	:CNtlPLGui(pName)
{
	Init();
}

CMascotFusion::~CMascotFusion()
{

}

VOID CMascotFusion::Init()
{
}

RwBool CMascotFusion::Create()
{
	NTL_FUNCTION("CMascotFusion::Create");

	if (!CNtlPLGui::Create("gui\\MascotEx.rsr", "gui\\MascotEx.srf", "gui\\MascotFusion.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");

	m_Stb_Title = (gui::CStaticBox*)GetComponent("Stb_Title");
	m_Stb_Title->SetText(GetDisplayStringManager()->GetString("DST_MASCOTEX_FUSIONBUTTON"));

	m_HtmlBox_Explan = (gui::CHtmlBox*)GetComponent("HtmlBox_Explan");
	const WCHAR* wchHtmlMsg = GetDisplayStringManager()->GetString("DST_MASCOTEX_FUSION_EXPLANATION");
	m_HtmlBox_Explan->SetHtmlFromMemory(wchHtmlMsg, wcslen(wchHtmlMsg));

	m_Btn_MascotFusion = (gui::CButton*)GetComponent("Btn_MascotFusion");
	m_Btn_MascotFusion->SetText(GetDisplayStringManager()->GetString("DST_MASCOTEX_FUSIONBUTTON"));

	m_Btn_Close = (gui::CButton*)GetComponent("Btn_Close");

	m_Pgb_LGague = (gui::CProgressBar*)GetComponent("Pgb_LGague");
	m_Pgb_RGague = (gui::CProgressBar*)GetComponent("Pgb_RGague");

	// Progress bar range
	m_Pgb_LGague->SetRange(0, 100);
	m_Pgb_LGague->SetPos(0);
	m_Pgb_RGague->SetRange(0, 100);
	m_Pgb_RGague->SetPos(0);

	m_slotCloseButton = m_Btn_Close->SigClicked().Connect(this, &CMascotFusion::ClickedCloseButton);

	Show(false);

	NTL_RETURN(TRUE);
}

VOID CMascotFusion::Destroy()
{

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();
}

VOID CMascotFusion::HandleEvents(RWS::CMsg& msg)
{

}

RwInt32 CMascotFusion::SwitchDialog(bool bOpen)
{
	Show(bOpen);
	return 1;
}

VOID CMascotFusion::ClickedCloseButton(gui::CComponent* pComponent)
{
	GetDialogManager()->CloseDialog(DIALOG_MASCOT_FUSION);
}