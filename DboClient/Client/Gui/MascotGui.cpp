#include "precomp_dboclient.h"

// core
#include "NtlDebug.h"
#include "CEventHandler.h"

// gui
#include "gui_define.h"
#include "NtlPLGuiManager.h"

// presentation
#include "NtlPLGui.h"
#include "NtlPLGuiManager.h"

// framework
#include "NtlApplication.h"

// simulation
#include "NtlSLGlobal.h"
#include "NtlSobPet.h"
#include "NtlSLApi.h"
#include "NtlSLLogic.h"
#include "NtlSLEvent.h"
#include "NtlSobCharProxy.h"
#include "NtlSobAvatar.h"
#include "NtlSobManager.h"
#include "NPCTable.h"
#include "NtlOtherParam.h"
#include "NtlMascot.h"
#include "NtlSLPacketGenerator.h"

// table
#include "MascotTable.h"
#include "MascotGradeTable.h"
#include "MascotStatusTable.h"
#include "TextAllTable.h"
#include "TableContainer.h"

//sound
#include "GUISoundDefine.h"

// dbo
#include "DboGlobal.h"
#include "DboLogic.h"
#include "BuffDispObject.h"
#include "DboDef.h"
#include "DboEvent.h"
#include "DboEventGenerator.h"
#include "DialogManager.h"
#include "DisplayStringManager.h"
#include "MascotGui.h"
#include <NtlSobPetAttr.h>
#include <InfoWndManager.h>

#define MASCOT_LIST_X 21
#define MASCOT_LIST_Y 97
#define MASCOT_LIST_MARGIN 4
#define MASCOT_ITEM_WITH 274
#define MASCOT_ITEM_HEIGHT 19
#define MASCOT_ICON_X 15
#define MASCOT_ICON_Y 97

#define SUR_CHAR_LEFT_OFFSET 13
#define SUR_CHAR_TOP_OFFSET  17


CMascotGui::CMascotGui(void)
{
}

CMascotGui::CMascotGui(const RwChar* pName)
	:CNtlPLGui(pName)
{
	Init();
}

CMascotGui::~CMascotGui()
{
}

void CMascotGui::Init()
{
	//StaticBox
	m_sttbTitle = NULL;
	m_psttb_Info = NULL;
	m_psttb_SkillInfo = NULL;
	m_psttb_Summoned = NULL;
	m_psttb_Name = NULL;
	m_psttb_ClassType = NULL;
	m_psttb_Vp = NULL;
	m_psttb_Cp = NULL;

	//Btn
	m_pbtn_Summon = NULL;
	m_pbtn_SummonEnd = NULL;
	m_pbtn_MascotDel = NULL;
	m_pbtn_MascotFusion = NULL;
	m_pbtn_MascotRename = NULL;
	m_pbtn_Close = NULL;

	//ScrollBar
	m_pscb_MascotScroll = NULL;

	//Other
	m_eListMode = MASCOT_LIST;
}

RwBool CMascotGui::Create()
{
	if (!CNtlPLGui::Create("gui\\MascotEx.rsr", "gui\\MascotEx.srf", "gui\\MascotEx.frm"))
		return FALSE;


	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());
	m_pThis = (gui::CDialog*)GetComponent("dlgMain");
	m_pThis->SetPriority(dDIALOGPRIORITY_DEFAULT);

	//StaticBox
	m_sttbTitle = (gui::CStaticBox*)GetComponent("Stb_Title");
	m_psttb_Info = (gui::CStaticBox*)GetComponent("Stb_Info");
	m_psttb_SkillInfo = (gui::CStaticBox*)GetComponent("Stb_SkillInfo");
	m_psttb_Summoned = (gui::CStaticBox*)GetComponent("Stb_Summoned");
	m_psttb_Name = (gui::CStaticBox*)GetComponent("Stb_Name");
	m_psttb_ClassType = (gui::CStaticBox*)GetComponent("Stb_ClassType");
	m_psttb_Vp = (gui::CStaticBox*)GetComponent("Stb_Vp");
	m_psttb_Cp = (gui::CStaticBox*)GetComponent("Stb_Cp");

	//Btn
	m_pbtn_Summon = (gui::CButton*)GetComponent("Btn_Summon");
	m_pbtn_SummonEnd = (gui::CButton*)GetComponent("Btn_SummonEnd");
	m_pbtn_MascotDel = (gui::CButton*)GetComponent("Btn_MascotDel");
	m_pbtn_MascotFusion = (gui::CButton*)GetComponent("Btn_MascotFusion");
	m_pbtn_MascotRename = (gui::CButton*)GetComponent("Btn_MascotRename");
	m_pbtn_Close = (gui::CButton*)GetComponent("Btn_Close");

	//ProgressBar
	m_ppgb_Vp = (gui::CProgressBar*)GetComponent("Pgb_Vp");
	m_ppgb_Xp = (gui::CProgressBar*)GetComponent("Pgb_Xp");

	//Panel
	m_pPn_IvnBack = (gui::CPanel*)GetComponent("Pn_IvnBack");
	m_pPn_Buff0 = (gui::CPanel*)GetComponent("Pn_Buff0");
	m_pPn_Buff1 = (gui::CPanel*)GetComponent("Pn_Buff1");
	m_pPn_Buff2 = (gui::CPanel*)GetComponent("Pn_Buff2");
	m_pPn_Buff3 = (gui::CPanel*)GetComponent("Pn_Buff3");
	m_pPn_IvnIcon0 = (gui::CPanel*)GetComponent("Pn_IvnIcon0");
	m_pPn_IvnIcon1 = (gui::CPanel*)GetComponent("Pn_IvnIcon1");
	m_pPn_IvnIcon2 = (gui::CPanel*)GetComponent("Pn_IvnIcon2");
	m_pPn_IvnIcon3 = (gui::CPanel*)GetComponent("Pn_IvnIcon3");
	m_pPn_IvnIcon4 = (gui::CPanel*)GetComponent("Pn_IvnIcon4");
	m_pPn_IvnIcon5 = (gui::CPanel*)GetComponent("Pn_IvnIcon5");
	m_pPn_IvnIcon6 = (gui::CPanel*)GetComponent("Pn_IvnIcon6");
	m_pPn_IvnIcon7 = (gui::CPanel*)GetComponent("Pn_IvnIcon7");
	m_pPn_SkillIcon0 = (gui::CPanel*)GetComponent("Pn_SkillIcon0");
	m_pPn_SkillIcon1 = (gui::CPanel*)GetComponent("Pn_SkillIcon1");
	m_pPn_SkillIcon2 = (gui::CPanel*)GetComponent("Pn_SkillIcon2");
	m_pPn_SkillIcon3 = (gui::CPanel*)GetComponent("Pn_SkillIcon3");

	//ScrollBar
	m_pscb_MascotScroll = (gui::CScrollBar*)GetComponent("Scb_MascotScroll");

	//From local_data.dat
	m_pbtn_Summon->SetToolTip(GetDisplayStringManager()->GetString("DST_RECALL_START_MASCOT_MB_CONFIRM"));
	m_pbtn_SummonEnd->SetToolTip(GetDisplayStringManager()->GetString("DST_RECALL_STOP_MASCOT_MB_CONFIRM"));
	m_pbtn_MascotDel->SetToolTip(GetDisplayStringManager()->GetString("DST_MASCOT_DELETE_MB_CONFIRM"));
	m_pbtn_MascotFusion->SetToolTip(GetDisplayStringManager()->GetString("DST_MASCOTEX_FUSIONBUTTON"));
	m_pbtn_MascotRename->SetToolTip(GetDisplayStringManager()->GetString("DST_MASCOT_RENAME_MB_CONFIRM"));

	//Events
	m_slotMouseLeave = m_pThis->SigMouseLeave().Connect(this, &CMascotGui::OnMouseLeave);
	m_slotClose = m_pbtn_Close->SigClicked().Connect(this, &CMascotGui::OnCloseBtnClicked);
	m_slotBtnSummon = m_pbtn_Summon->SigClicked().Connect(this, &CMascotGui::OnSummonBtnClicked);
	m_slotBtnSummonEnd = m_pbtn_SummonEnd->SigClicked().Connect(this, &CMascotGui::OnSummonEndBtnClicked);
	m_slotBtnMascotDel = m_pbtn_MascotDel->SigClicked().Connect(this, &CMascotGui::OnMascotDelBtnClicked);
	m_slotBtnMascotFusion = m_pbtn_MascotFusion->SigClicked().Connect(this, &CMascotGui::OnMascotFusionBtnClicked);
	m_slotBtnMascotRename = m_pbtn_MascotRename->SigClicked().Connect(this, &CMascotGui::OnMascotRenameBtnClicked);
	m_slotMascotScroll = m_pscb_MascotScroll->SigValueChanged().Connect(this, &CMascotGui::OnScrollChanged);
	m_slotMascotScrollMove = m_pscb_MascotScroll->SigSliderMoved().Connect(this, &CMascotGui::OnScrollChanged);
	m_slotCaptureWheelMove = m_pThis->SigMouseWheel().Connect(this, &CMascotGui::OnCaptureWheelMove);
	m_slotCaptureMouseDown = GetNtlGuiManager()->GetGuiManager()->SigCaptureMouseDown().Connect(this, &CMascotGui::OnCaptureMouseDown); // required to put mascot gui above other UI's

	//Window Name
	m_sttbTitle->SetPosition(DBOGUI_DIALOG_TITLE_X, DBOGUI_DIALOG_TITLE_Y);
	m_sttbTitle->SetText(GetDisplayStringManager()->GetString("DST_MASCOT"));


	//Set Name of the Buttons
	m_pbtn_Summon->SetText(GetDisplayStringManager()->GetString("DST_RECALL"));
	m_pbtn_SummonEnd->SetText(GetDisplayStringManager()->GetString("DST_RELEASE"));
	m_pbtn_MascotDel->SetText(GetDisplayStringManager()->GetString("DST_DELETE"));
	m_pbtn_MascotFusion->SetText(GetDisplayStringManager()->GetString("DST_MASCOTEX_FUSIONBUTTON"));
	m_pbtn_MascotRename->SetText(GetDisplayStringManager()->GetString("DST_MASCOTEX_RENAMEBUTTON"));

	Show(false);

	return TRUE;
}

void CMascotGui::Destroy()
{
	NTL_FUNCTION("CMascotGui::Destroy");
	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();
	gui::CResourceManager* pResMgr = CNtlPLGuiManager::GetInstance()->GetReourceManager();
	GetNtlGuiManager()->GetReourceManager()->RemovePage("gui\\mascotex.rsr");
}

RwInt32 CMascotGui::SwitchDialog(bool bOpen)
{
	Show(bOpen);
	return TRUE;
}

void CMascotGui::HandleEvents(RWS::CMsg& pMsg)
{
	return VOID();
}


VOID CMascotGui::OnMouseMove(RwInt32 nFlags, RwInt32 nX, RwInt32 nY)
{
	// Displays the tooltip when the mouse hovers over the button.

	nX = CMouse::GetX();
	nY = CMouse::GetY();

	return VOID();
}

VOID CMascotGui::OnMouseLeave(gui::CComponent* pComponent)
{
	if (GetInfoWndManager()->GetRequestGui() == DIALOG_MASCOT)
	{
		GetInfoWndManager()->ShowInfoWindow(FALSE);
	}
}

VOID CMascotGui::OnToggled(gui::CComponent* pComponent, bool bToggled)
{
	return VOID();
}

VOID CMascotGui::OnCaptureWheelMove(RwInt32 nFlag, RwInt16 sDelta, CPos& pos)
{
	return VOID();
}

VOID CMascotGui::OnCaptureMouseDown(const CKey& key)
{
	CAPTURE_MOUSEDOWN_RAISE(DIALOG_MASCOT, key.m_fX, key.m_fY);
}

VOID CMascotGui::OnCloseBtnClicked(gui::CComponent* pComponent)
{
	if (!Logic_CanMouseInput_in_Tutorial(ETL_MOUSE_INPUT_TYPE_MAINMENU_MASCOT_CLOSE))
		return;

	GetDialogManager()->CloseDialog(DIALOG_MASCOT);
}

VOID CMascotGui::OnPetClicked(gui::CComponent* pComponent)
{
	return VOID();
}

VOID CMascotGui::OnSummonBtnClicked(gui::CComponent* pComponent)
{
	return VOID();
}

VOID CMascotGui::OnSummonEndBtnClicked(gui::CComponent* pComponent)
{
	return VOID();
}

VOID CMascotGui::OnMascotDelBtnClicked(gui::CComponent* pComponent)
{
	return VOID();
}

VOID CMascotGui::OnMascotFusionBtnClicked(gui::CComponent* pComponent)
{
	return VOID();
}

VOID CMascotGui::OnMascotRenameBtnClicked(gui::CComponent* pComponent)
{
	return VOID();
}

VOID CMascotGui::OnScrollChanged(RwInt32 nNewOffset)
{
	return VOID();
}
