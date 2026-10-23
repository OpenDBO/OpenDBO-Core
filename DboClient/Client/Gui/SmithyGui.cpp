#include "precomp_dboclient.h"
#include "SmithyGui.h"

// shared
#include "ItemTable.h"
#include "TextAllTable.h"
#include "TableContainer.h"

// sound
#include "GUISoundDefine.h"

// presentation
#include "NtlPLGuiManager.h"

// simulation
#include "NtlSLGlobal.h"
#include "NtlSLApi.h"
#include "NtlSLEvent.h"
#include "NtlSLLogic.h"
#include "NtlSobAvatar.h"
#include "NtlSobItem.h"
#include "NtlSobItemAttr.h"
#include "NtlSobManager.h"
#include "NtlWorldConceptNPCCommu.h"

// dbo
#include "DboEvent.h"
#include "DboGlobal.h"
#include "DboLogic.h"
#include "DialogManager.h"
#include "DisplayStringManager.h"
#include "InfoWndManager.h"

#define SMITHY_DISASSEMBLE_FLASHFILE	"Item_decomposition_Effect.swf"

// tw: the tab pages sit 58 px under the top of the smithy
#define SMITHY_TAB_OFFSET_Y				58

// tw layout of the frames drawn behind the components
#define SMITHY_BACK_X					9
#define SMITHY_BACK_Y					50
#define SMITHY_BACK_WIDTH				303
#define SMITHY_BACK_HEIGHT				379

#define DISASSEMBLE_BACK_X				15
#define DISASSEMBLE_BACK_WIDTH			288
#define DISASSEMBLE_TOP_BACK_Y			0
#define DISASSEMBLE_TOP_BACK_HEIGHT		98
#define DISASSEMBLE_BOTTOM_BACK_Y		101
#define DISASSEMBLE_BOTTOM_BACK_HEIGHT	262

#define DISASSEMBLE_RESULT_SLOT_X		45
#define DISASSEMBLE_RESULT_SLOT_Y		150
#define DISASSEMBLE_RESULT_SLOT_GAP_Y	70


////////////////////////////////////////////////////////////////////////////////
// CItemDisassembleTabGui (tw CItemDisassembleTabState)

CItemDisassembleTabGui::CItemDisassembleTabGui(const RwChar* pName)
: CNtlPLGui(pName)
{
	m_pstbInfo = NULL;
	m_pstbName = NULL;
	m_pstbResultName = NULL;
	m_pbtnDisassemble = NULL;
	m_pbtnCancel = NULL;
	m_pflsDisassemble = NULL;
	m_iFocusSlot = -1;
	m_bUpgradePages = FALSE;

	for (RwInt32 i = 0; i < ITEM_DISASSEMBLE_MAX_RESULT; ++i)
	{
		m_apstbResultTitle[i] = NULL;
		m_apstbResultInfo[i] = NULL;
	}
}

CItemDisassembleTabGui::~CItemDisassembleTabGui(VOID)
{
}

RwBool CItemDisassembleTabGui::Create(gui::CComponent* pParent)
{
	NTL_FUNCTION("CItemDisassembleTabGui::Create");

	if (!CNtlPLGui::Create("gui\\BaseTabStateGui.rsr", "gui\\BaseTabStateGui.srf", "gui\\ItemDisassembleTabGui.frm"))
		NTL_RETURN(FALSE);

	// the cancel button uses the item upgrade surfaces
	GetNtlGuiManager()->GetReourceManager()->AddPage("gui\\ItemUpgrade.rsr");
	GetNtlGuiManager()->GetSurfaceManager()->AddPage("gui\\ItemUpgradeEx.srf");
	m_bUpgradePages = TRUE;

	CNtlPLGui::CreateComponents(pParent);

	m_pThis = (gui::CDialog*)GetComponent("dlgBase");

	m_pstbInfo = (gui::CStaticBox*)GetComponent("ItemDisassembleInfoStatic");
	m_pstbName = (gui::CStaticBox*)GetComponent("ItemDisassembleNameStatic");
	m_pstbResultName = (gui::CStaticBox*)GetComponent("ItemDisassembleResultName");
	m_pbtnDisassemble = (gui::CButton*)GetComponent("ItemDisassembleButton");
	m_pbtnCancel = (gui::CButton*)GetComponent("ItemDisassembleCancel");
	m_pflsDisassemble = (gui::CFlash*)GetComponent("FlashDisassemble");

	char acName[64];
	for (RwInt32 i = 0; i < ITEM_DISASSEMBLE_MAX_RESULT; ++i)
	{
		sprintf_s(acName, 64, "ItemDisassembleResultTitle%d", i + 1);
		m_apstbResultTitle[i] = (gui::CStaticBox*)GetComponent(acName);
		m_apstbResultTitle[i]->SetText(GetDisplayStringManager()->GetString("DST_ITEMDISASSEMBLE_RESULT_TITLE"));

		sprintf_s(acName, 64, "ItemDisassembleResultInfo%d", i + 1);
		m_apstbResultInfo[i] = (gui::CStaticBox*)GetComponent(acName);

		m_ResultSlot[i].Create(m_pThis, DIALOG_SMITHY, REGULAR_SLOT_ITEM_SOB);
		m_ResultSlot[i].SetPosition_fromParent(DISASSEMBLE_RESULT_SLOT_X, DISASSEMBLE_RESULT_SLOT_Y + i * DISASSEMBLE_RESULT_SLOT_GAP_Y);
	}

	m_pstbInfo->SetText(GetDisplayStringManager()->GetString("DST_ITEMDISASSEMBLE_INFO"));
	m_pstbName->SetText(GetDisplayStringManager()->GetString("DST_ITEMDISASSEMBLETABNAME"));
	m_pstbResultName->SetText(GetDisplayStringManager()->GetString("DST_ITEMDISASSEMBLE_RESULT_NAME"));
	m_pbtnCancel->SetText(GetDisplayStringManager()->GetString("DST_ITEMDISASSEMBLE_CLOSE"));

	m_TopBack.SetType(CWindowby3::WT_HORIZONTAL);
	m_TopBack.SetSurface(0, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("BaseTabStateGui.srf", "srfBackLineTop"));
	m_TopBack.SetSurface(1, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("BaseTabStateGui.srf", "srfBackLineCenter"));
	m_TopBack.SetSurface(2, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("BaseTabStateGui.srf", "srfBackLineBottom"));
	m_TopBack.SetSize(DISASSEMBLE_BACK_WIDTH, DISASSEMBLE_TOP_BACK_HEIGHT);
	m_TopBack.SetPositionfromParent(DISASSEMBLE_BACK_X, DISASSEMBLE_TOP_BACK_Y);

	m_BottomBack.SetType(CWindowby3::WT_HORIZONTAL);
	m_BottomBack.SetSurface(0, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("BaseTabStateGui.srf", "srfBackLineTop"));
	m_BottomBack.SetSurface(1, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("BaseTabStateGui.srf", "srfBackLineCenter"));
	m_BottomBack.SetSurface(2, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("BaseTabStateGui.srf", "srfBackLineBottom"));
	m_BottomBack.SetSize(DISASSEMBLE_BACK_WIDTH, DISASSEMBLE_BOTTOM_BACK_HEIGHT);
	m_BottomBack.SetPositionfromParent(DISASSEMBLE_BACK_X, DISASSEMBLE_BOTTOM_BACK_Y);

	// the effect covers the whole smithy, above the result frame
	m_pflsDisassemble->Raise();
	m_pflsDisassemble->Show(false);

	m_slotClickDisassemble = m_pbtnDisassemble->SigClicked().Connect(this, &CItemDisassembleTabGui::OnClickDisassemble);
	m_slotClickCancel = m_pbtnCancel->SigClicked().Connect(this, &CItemDisassembleTabGui::OnClickCancel);
	m_slotMouseMove = m_pThis->SigMouseMove().Connect(this, &CItemDisassembleTabGui::OnMouseMove);
	m_slotMouseLeave = m_pThis->SigMouseLeave().Connect(this, &CItemDisassembleTabGui::OnMouseLeave);
	m_slotPaintBack = m_pThis->SigPaint().Connect(this, &CItemDisassembleTabGui::OnPaintBack);
	m_slotEffectEnd = m_pflsDisassemble->SigMovieEnd().Connect(this, &CItemDisassembleTabGui::OnEffectEnd);

	// painted from the last frame child, the result frame would cover the icons otherwise
	m_slotPaint = GetComponent("OperateBackGround")->SigPaint().Connect(this, &CItemDisassembleTabGui::OnPaint);

	Reset();
	Show(false);

	NTL_RETURN(TRUE);
}

VOID CItemDisassembleTabGui::Destroy(VOID)
{
	for (RwInt32 i = 0; i < ITEM_DISASSEMBLE_MAX_RESULT; ++i)
		m_ResultSlot[i].Destroy();

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();

	// the pages are shared with the item upgrade gui, only drop the ref this tab took
	if (m_bUpgradePages)
	{
		GetNtlGuiManager()->GetSurfaceManager()->RemovePage("gui\\ItemUpgradeEx.srf");
		GetNtlGuiManager()->GetReourceManager()->RemovePage("gui\\ItemUpgrade.rsr");
		m_bUpgradePages = FALSE;
	}
}

VOID CItemDisassembleTabGui::Update(RwReal fElapsed)
{
	if (m_pflsDisassemble->IsVisible())
		m_pflsDisassemble->Update(fElapsed);
}

VOID CItemDisassembleTabGui::Activate(RwBool bActive)
{
	Show(bActive ? true : false);

	if (bActive)
	{
		if (!m_pflsDisassemble->Load(SMITHY_DISASSEMBLE_FLASHFILE, TRUE))
			DBO_WARNING_MESSAGE("CItemDisassembleTabGui::Activate, can not load " << SMITHY_DISASSEMBLE_FLASHFILE);

		m_pflsDisassemble->SetPosition(0, -SMITHY_TAB_OFFSET_Y);
		m_pflsDisassemble->AutoRewind(FALSE);
		m_pflsDisassemble->SetFrameCallback(TRUE);	// SigMovieEnd only fires with it

		Reset();
		OnParentMove();
	}
	else
	{
		if (GetDialogManager()->IsMode(DIALOGMODE_ITEM_DISASSEMBLE))
			GetDialogManager()->OffMode();

		if (GetInfoWndManager()->GetRequestGui() == DIALOG_SMITHY)
			GetInfoWndManager()->ShowInfoWindow(FALSE);

		Reset();
		m_pflsDisassemble->Unload();
	}
}

VOID CItemDisassembleTabGui::Reset(VOID)
{
	m_pflsDisassemble->PlayMovie(FALSE);
	m_pflsDisassemble->Show(false);

	m_iFocusSlot = -1;

	for (RwInt32 i = 0; i < ITEM_DISASSEMBLE_MAX_RESULT; ++i)
	{
		m_apstbResultTitle[i]->Show(false);
		m_apstbResultInfo[i]->Show(false);
		m_ResultSlot[i].Clear();
	}
}

VOID CItemDisassembleTabGui::SetResult(const RwUInt32 (&ahResultItem)[ITEM_DISASSEMBLE_MAX_RESULT])
{
	Reset();

	m_pflsDisassemble->Show(true);
	m_pflsDisassemble->RestartMovie();
	m_pflsDisassemble->PlayMovie(TRUE);

	Logic_PlayGUISound(GSD_SYSTEM_DRAGOBALL_REGISTRATION);

	for (RwInt32 i = 0; i < ITEM_DISASSEMBLE_MAX_RESULT; ++i)
	{
		if (ahResultItem[i] == INVALID_SERIAL_ID)
			continue;

		CNtlSobItem* pItem = static_cast<CNtlSobItem*>(GetNtlSobManager()->GetSobObject(ahResultItem[i]));
		if (!pItem)
			continue;

		CNtlSobItemAttr* pItemAttr = static_cast<CNtlSobItemAttr*>(pItem->GetSobAttr());
		sITEM_TBLDAT* pITEM_TBLDAT = pItemAttr ? pItemAttr->GetItemTbl() : NULL;
		if (!pITEM_TBLDAT)
			continue;

		m_ResultSlot[i].SetIcon(ahResultItem[i]);

		m_apstbResultInfo[i]->SetText(API_GetTableContainer()->GetTextAllTable()->GetItemTbl()->GetText(pITEM_TBLDAT->Name).c_str());
		m_apstbResultTitle[i]->Show(true);
		m_apstbResultInfo[i]->Show(true);
	}
}

VOID CItemDisassembleTabGui::RemoveResultItem(RwUInt32 hItem)
{
	for (RwInt32 i = 0; i < ITEM_DISASSEMBLE_MAX_RESULT; ++i)
	{
		if (m_ResultSlot[i].GetSerial() != hItem)
			continue;

		if (m_iFocusSlot == i && GetInfoWndManager()->GetRequestGui() == DIALOG_SMITHY)
			GetInfoWndManager()->ShowInfoWindow(FALSE);

		m_ResultSlot[i].Clear();
		m_apstbResultTitle[i]->Show(false);
		m_apstbResultInfo[i]->Show(false);
	}
}

VOID CItemDisassembleTabGui::OnParentMove(VOID)
{
	CRectangle rtScreen = m_pThis->GetScreenRect();

	for (RwInt32 i = 0; i < ITEM_DISASSEMBLE_MAX_RESULT; ++i)
		m_ResultSlot[i].SetParentPosition(rtScreen.left, rtScreen.top);

	m_TopBack.SetPositionbyParent(rtScreen.left, rtScreen.top);
	m_BottomBack.SetPositionbyParent(rtScreen.left, rtScreen.top);
}

VOID CItemDisassembleTabGui::OnClickDisassemble(gui::CComponent* pComponent)
{
	// tw: the button only arms the disassemble cursor, the item is picked in the bag
	if (GetDialogManager()->GetMode() != DIALOGMODE_UNKNOWN)
		return;

	GetDialogManager()->OnMode(DIALOGMODE_ITEM_DISASSEMBLE);
}

VOID CItemDisassembleTabGui::OnClickCancel(gui::CComponent* pComponent)
{
	GetDialogManager()->CloseDialog(DIALOG_SMITHY);
}

VOID CItemDisassembleTabGui::OnMouseMove(RwInt32 nFlags, RwInt32 nX, RwInt32 nY)
{
	RwInt32 iSlot = -1;
	for (RwInt32 i = 0; i < ITEM_DISASSEMBLE_MAX_RESULT; ++i)
	{
		if (m_ResultSlot[i].GetSerial() != INVALID_SERIAL_ID && m_ResultSlot[i].PtInRect(nX, nY))
		{
			iSlot = i;
			break;
		}
	}

	if (iSlot == m_iFocusSlot)
		return;

	m_iFocusSlot = iSlot;

	if (GetInfoWndManager()->GetRequestGui() == DIALOG_SMITHY)
		GetInfoWndManager()->ShowInfoWindow(FALSE);

	if (iSlot < 0)
		return;

	CRectangle rtScreen = m_pThis->GetScreenRect();
	GetInfoWndManager()->ShowInfoWindow(TRUE, CInfoWndManager::INFOWND_ITEM,
		rtScreen.left + m_ResultSlot[iSlot].GetX_fromParent(), rtScreen.top + m_ResultSlot[iSlot].GetY_fromParent(),
		m_ResultSlot[iSlot].GetSobItem(), DIALOG_SMITHY);
}

VOID CItemDisassembleTabGui::OnMouseLeave(gui::CComponent* pComponent)
{
	m_iFocusSlot = -1;

	if (GetInfoWndManager()->GetRequestGui() == DIALOG_SMITHY)
		GetInfoWndManager()->ShowInfoWindow(FALSE);
}

VOID CItemDisassembleTabGui::OnPaint(VOID)
{
	for (RwInt32 i = 0; i < ITEM_DISASSEMBLE_MAX_RESULT; ++i)
		m_ResultSlot[i].Paint();
}

VOID CItemDisassembleTabGui::OnPaintBack(VOID)
{
	m_TopBack.Render();
	m_BottomBack.Render();
}

VOID CItemDisassembleTabGui::OnEffectEnd(gui::CComponent* pComponent)
{
	m_pflsDisassemble->PlayMovie(FALSE);
	m_pflsDisassemble->Show(false);
}


////////////////////////////////////////////////////////////////////////////////
// CSmithyGui

CSmithyGui::CSmithyGui(const RwChar* pName)
: CNtlPLGui(pName)
, m_DisassembleTab("ItemDisassembleTab")
{
	m_pbtnExit = NULL;
	m_pstbTitle = NULL;
	m_ptabSmithy = NULL;
	m_hNPCSerial = INVALID_SERIAL_ID;
}

CSmithyGui::~CSmithyGui(VOID)
{
}

RwBool CSmithyGui::Create(VOID)
{
	NTL_FUNCTION("CSmithyGui::Create");

	if (!CNtlPLGui::Create("", "", "gui\\SmithyGui.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(GetNtlGuiManager()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");

	m_pbtnExit = (gui::CButton*)GetComponent("ExitButton");
	m_pstbTitle = (gui::CStaticBox*)GetComponent("TitleNameStaticBox");
	m_ptabSmithy = (gui::CTabButton*)GetComponent("SmithyTabButton");

	// tw: "Item " + "Upgrade" / "Disassemble" for the tabs that are on
	std::wstring wstrTitle = GetDisplayStringManager()->GetString("DST_ITEM");
	wstrTitle += L" ";
	wstrTitle += GetDisplayStringManager()->GetString("DST_MULTIDIALOG_MENU_SMITH_DISASSEMBLE");
	m_pstbTitle->SetText(wstrTitle.c_str());

	m_BackLine.SetType(CWindowby3::WT_HORIZONTAL);
	m_BackLine.SetSurface(0, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("GameCommon.srf", "srfBackLineTop"));
	m_BackLine.SetSurface(1, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("GameCommon.srf", "srfBackLineCenter"));
	m_BackLine.SetSurface(2, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("GameCommon.srf", "srfBackLineBottom"));
	m_BackLine.SetSize(SMITHY_BACK_WIDTH, SMITHY_BACK_HEIGHT);
	m_BackLine.SetPositionfromParent(SMITHY_BACK_X, SMITHY_BACK_Y);

	if (!m_DisassembleTab.Create(m_pThis))
		NTL_RETURN(FALSE);	// AddDialog calls Destroy

	m_ptabSmithy->AddTab(std::wstring(GetDisplayStringManager()->GetString("DST_ITEMDISASSEMBLETABNAME")));
	m_ptabSmithy->SelectTab(0);

	m_slotClickExit = m_pbtnExit->SigClicked().Connect(this, &CSmithyGui::OnClickExit);
	m_slotPaint = m_pThis->SigPaint().Connect(this, &CSmithyGui::OnPaint);
	m_slotMove = m_pThis->SigMove().Connect(this, &CSmithyGui::OnMove);

	Show(false);

	LinkMsg(g_EventNPCDialogOpen, 0);
	LinkMsg(g_EventDialog, 0);
	LinkMsg(g_EventItemDisassembleResult, 0);
	LinkMsg(g_EventSobItemDelete, 0, 0x9000);	// the result slots keep the item pointer
	LinkMsg(g_EventGameServerChangeOut);

	m_pThis->SetPriority(dDIALOGPRIORITY_DEFAULT);

	NTL_RETURN(TRUE);
}

VOID CSmithyGui::Destroy(VOID)
{
	UnLinkMsg(g_EventNPCDialogOpen);
	UnLinkMsg(g_EventDialog);
	UnLinkMsg(g_EventItemDisassembleResult);
	UnLinkMsg(g_EventSobItemDelete);
	UnLinkMsg(g_EventGameServerChangeOut);

	m_DisassembleTab.Destroy();

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();
}

VOID CSmithyGui::Update(RwReal fElapsed)
{
	m_DisassembleTab.Update(fElapsed);
}

RwInt32 CSmithyGui::SwitchDialog(bool bOpen)
{
	if (bOpen)
	{
		CNtlWorldConceptNPCCOMMU* pWorldConceptController = reinterpret_cast<CNtlWorldConceptNPCCOMMU*>(GetNtlWorldConcept()->GetWorldConceptController(WORLD_PLAY_NPC_COMMU));
		pWorldConceptController->ChangeState(WORLD_NPC_ITEM_UPGRADE);

		RaiseTop();
		PositionAlign(GetDboGlobal()->GetScreenWidth(), GetDboGlobal()->GetScreenHeight());
		Show(true);

		m_DisassembleTab.Activate(TRUE);

		GetNtlGuiManager()->AddUpdateFunc(this);
	}
	else
	{
		m_DisassembleTab.Activate(FALSE);

		GetNtlWorldConcept()->RemoveWorldPlayConcept(WORLD_PLAY_NPC_COMMU);
		GetDialogManager()->SwitchBag(FALSE);

		Show(false);

		Logic_CancelNpcFacing();

		GetNtlGuiManager()->RemoveUpdateFunc(this);

		m_hNPCSerial = INVALID_SERIAL_ID;
	}

	return 1;
}

VOID CSmithyGui::HandleEvents(RWS::CMsg& msg)
{
	if (msg.Id == g_EventNPCDialogOpen)
	{
		SDboNpcDialogOpen* pData = reinterpret_cast<SDboNpcDialogOpen*>(msg.pData);

		if (pData->eDialog != DIALOG_SMITHY)
			return;

		m_hNPCSerial = pData->hSerialId;
		GetDialogManager()->OpenDialog(DIALOG_SMITHY, GetNtlSLGlobal()->GetSobAvatar()->GetSerialID());
		GetDialogManager()->SwitchBag(TRUE);
	}
	else if (msg.Id == g_EventDialog)
	{
		SDboEventDialog* pData = reinterpret_cast<SDboEventDialog*>(msg.pData);
		if (pData->iType == DIALOGEVENT_NPC_BYEBYE && pData->iDestDialog == DIALOG_SMITHY)
			GetDialogManager()->CloseDialog(DIALOG_SMITHY);
	}
	else if (msg.Id == g_EventItemDisassembleResult)
	{
		if (!IsShow())
			return;

		SDboItemDisassembleResult* pData = reinterpret_cast<SDboItemDisassembleResult*>(msg.pData);
		m_DisassembleTab.SetResult(pData->ahResultItem);
	}
	else if (msg.Id == g_EventSobItemDelete)
	{
		SNtlEventSobItemDelete* pData = reinterpret_cast<SNtlEventSobItemDelete*>(msg.pData);
		m_DisassembleTab.RemoveResultItem(pData->hItemSerialId);
	}
	else if (msg.Id == g_EventGameServerChangeOut)
	{
		GetDialogManager()->CloseDialog(DIALOG_SMITHY);
	}
}

VOID CSmithyGui::PositionAlign(RwInt32 nScreenWidth, RwInt32 nScreenHeight)
{
	m_pThis->SetPosition((nScreenWidth - m_pThis->GetWidth()) / 2, (nScreenHeight - m_pThis->GetHeight()) / 2);
}

VOID CSmithyGui::OnClickExit(gui::CComponent* pComponent)
{
	GetDialogManager()->CloseDialog(DIALOG_SMITHY);
}

VOID CSmithyGui::OnPaint(VOID)
{
	m_BackLine.Render();
}

VOID CSmithyGui::OnMove(RwInt32 nOldX, RwInt32 nOldY)
{
	CRectangle rtScreen = m_pThis->GetScreenRect();
	m_BackLine.SetPositionbyParent(rtScreen.left, rtScreen.top);

	m_DisassembleTab.OnParentMove();
}
