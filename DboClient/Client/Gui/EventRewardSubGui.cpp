#include "precomp_dboclient.h"
#include "EventRewardSubGui.h"

// core
#include "NtlDebug.h"

// shared
#include "EventSystemTable.h"
#include "ItemTable.h"
#include "TableContainer.h"
#include "TextAllTable.h"

// presentation
#include "NtlPLGuiManager.h"

// simulation
#include "NtlSLApi.h"
#include "NtlSLGlobal.h"
#include "NtlSobAvatar.h"

// dbo
#include "AlarmManager.h"
#include "DboEvent.h"
#include "DboGlobal.h"
#include "DialogManager.h"
#include "DisplayStringManager.h"
#include "EventRewardMainGui.h"
#include "InfoWndManager.h"


CEventRewardSubGui::CEventRewardSubGui(const RwChar* pName)
: CNtlPLGui(pName)
{
	m_pstbTitle = NULL;
	m_pstbSenderTitle = NULL;
	m_pstbSender = NULL;
	m_pdlgReceiverTitle = NULL;
	m_pstbReceiverTitle = NULL;
	m_pdlgReceiver = NULL;
	m_pstbReceiver = NULL;
	m_pstbAccount = NULL;
	m_pstbDesc = NULL;
	m_ppnlItemIcon = NULL;
	m_pstbItemName = NULL;
	m_pbtnGetReward = NULL;
	m_pbtnExit = NULL;
	m_pbtnClose = NULL;

	m_hNPCSerial = INVALID_SERIAL_ID;
	m_eventTblidx = INVALID_TBLIDX;
	m_charId = INVALID_CHARACTERID;
	m_bShowInfo = FALSE;
}

CEventRewardSubGui::~CEventRewardSubGui(VOID)
{
}

RwBool CEventRewardSubGui::Create(VOID)
{
	NTL_FUNCTION("CEventRewardSubGui::Create");

	if (!CNtlPLGui::Create("gui\\EventReward.rsr", "gui\\EventReward.srf", "gui\\eventrewardsubgui.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(GetNtlGuiManager()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");
	m_pThis->SetPriority(dDIALOGPRIORITY_DEFAULT);

	m_pstbTitle = (gui::CStaticBox*)GetComponent("StTitle");
	m_pstbTitle->SetText(GetDisplayStringManager()->GetString("DST_REWARD_INFO"));

	m_pstbSenderTitle = (gui::CStaticBox*)GetComponent("StSenderTitle");
	m_pstbSenderTitle->SetText(GetDisplayStringManager()->GetString("DST_REWARD_SENDCHAR"));

	m_pstbSender = (gui::CStaticBox*)GetComponent("StSender");
	m_pstbSender->SetText(GetDisplayStringManager()->GetString("DST_REWARD_EVENT_AD"));

	m_pdlgReceiverTitle = (gui::CDialog*)GetComponent("dlgSubTitle");
	m_pstbReceiverTitle = (gui::CStaticBox*)GetComponent("StReceiverTitle");
	m_pstbReceiverTitle->SetText(GetDisplayStringManager()->GetString("DST_REWARD_CHAR"));

	m_pdlgReceiver = (gui::CDialog*)GetComponent("dlgSubName");
	m_pstbReceiver = (gui::CStaticBox*)GetComponent("StReceiver");

	// tw: an account reward shows this line instead of the receiver
	m_pstbAccount = (gui::CStaticBox*)GetComponent("StAccount");
	m_pstbAccount->SetText(GetDisplayStringManager()->GetString("DST_REWARD_ACCOUNT"));
	m_pstbAccount->Show(false);

	m_pstbDesc = (gui::CStaticBox*)GetComponent("StDesc");
	m_pstbItemName = (gui::CStaticBox*)GetComponent("StItemName");

	m_ppnlItemIcon = (gui::CPanel*)GetComponent("StItemIcon");
	m_slotMouseMoveIcon = m_ppnlItemIcon->SigMouseMove().Connect(this, &CEventRewardSubGui::OnMouseMoveIcon);
	m_slotMouseLeaveIcon = m_ppnlItemIcon->SigMouseLeave().Connect(this, &CEventRewardSubGui::OnMouseLeaveIcon);

	m_ItemSlot.Create(m_pThis, DIALOG_EVENTREWARD_SUB, REGULAR_SLOT_ITEM_TABLE);
	m_ItemSlot.SetPosition_fromParent(m_ppnlItemIcon->GetPosition().left, m_ppnlItemIcon->GetPosition().top);

	m_pbtnGetReward = (gui::CButton*)GetComponent("btnGetRewardBtn");
	m_pbtnGetReward->ClickEnable(FALSE);
	m_slotClickGetReward = m_pbtnGetReward->SigClicked().Connect(this, &CEventRewardSubGui::OnClickGetReward);

	m_pbtnExit = (gui::CButton*)GetComponent("btnExitBtn");
	m_pbtnExit->SetText(GetDisplayStringManager()->GetString("DST_REWARD_CLOSE"));
	m_slotClickExit = m_pbtnExit->SigClicked().Connect(this, &CEventRewardSubGui::OnClickClose);

	m_pbtnClose = (gui::CButton*)GetComponent("btnCloseBtn");
	m_slotClickClose = m_pbtnClose->SigClicked().Connect(this, &CEventRewardSubGui::OnClickClose);

	m_slotPaint = m_pThis->SigPaint().Connect(this, &CEventRewardSubGui::OnPaint);
	m_slotMove = m_pThis->SigMove().Connect(this, &CEventRewardSubGui::OnMove);

	LinkMsg(g_EventNPCDialogOpen);
	LinkMsg(g_EventDialog);
	LinkMsg(g_EventNpcReward);

	Show(false);

	NTL_RETURN(TRUE);
}

VOID CEventRewardSubGui::Destroy(VOID)
{
	NTL_FUNCTION("CEventRewardSubGui::Destroy");

	UnLinkMsg(g_EventNPCDialogOpen);
	UnLinkMsg(g_EventDialog);
	UnLinkMsg(g_EventNpcReward);

	m_ItemSlot.Destroy();

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();

	NTL_RETURNVOID();
}

RwInt32 CEventRewardSubGui::SwitchDialog(bool bOpen)
{
	if (bOpen)
	{
		CRectangle rtScreen = m_pThis->GetScreenRect();
		m_ItemSlot.SetParentPosition(rtScreen.left, rtScreen.top);
	}
	else
	{
		ClearReward();

		m_eventTblidx = INVALID_TBLIDX;
		m_charId = INVALID_CHARACTERID;
	}

	Show(bOpen);
	return TRUE;
}

VOID CEventRewardSubGui::HandleEvents(RWS::CMsg& msg)
{
	if (msg.Id == g_EventNPCDialogOpen)
	{
		SDboNpcDialogOpen* pData = reinterpret_cast<SDboNpcDialogOpen*>(msg.pData);
		if (pData->eDialog != DIALOG_EVENTREWARD_MAIN)
			return;

		m_hNPCSerial = pData->hSerialId;
	}
	else if (msg.Id == g_EventDialog)
	{
		SDboEventDialog* pData = reinterpret_cast<SDboEventDialog*>(msg.pData);

		if (pData->iType == DIALOGEVENT_NPC_BYEBYE && pData->iDestDialog == DIALOG_EVENTREWARD_MAIN)
			GetDialogManager()->CloseDialog(DIALOG_EVENTREWARD_SUB);
	}
	else if (msg.Id == g_EventNpcReward)
	{
		SDboEventNpcReward* pData = reinterpret_cast<SDboEventNpcReward*>(msg.pData);

		switch (pData->byType)
		{
			case NPC_REWARD_EVENT_DATA:
			{
				SetReward(pData->eventTblidx, pData->charId, pData->pwcName);
			}
			break;
			case NPC_REWARD_EVENT_SELECT:
			{
				// tw: the item leaves the window once it was received
				ClearReward();
			}
			break;
		}
	}
}

VOID CEventRewardSubGui::SetReward(TBLIDX eventTblidx, CHARACTERID charId, const WCHAR* pwcName)
{
	ClearReward();

	m_eventTblidx = eventTblidx;
	m_charId = charId;

	sEVENT_SYSTEM_TBLDAT* pEventTbldat = Logic_FindEventRewardTbldat(eventTblidx);
	if (pEventTbldat == NULL)
		return;

	// a reward of the whole account shows that line instead of the receiver
	if (charId == INVALID_CHARACTERID)
	{
		m_pstbAccount->Show(true);
		m_pdlgReceiverTitle->Show(false);
		m_pdlgReceiver->Show(false);
	}
	else
	{
		m_pstbAccount->Show(false);
		m_pdlgReceiverTitle->Show(true);
		m_pdlgReceiver->Show(true);
		m_pstbReceiver->SetText(pwcName ? pwcName : L"");
	}

	CTextAllTable* pTextAllTable = API_GetTableContainer()->GetTextAllTable();

	std::wstring wstrDesc;
	pTextAllTable->GetETCTbl()->GetText(dEVENTREWARD_TEXT_DESC(eventTblidx), &wstrDesc);
	m_pstbDesc->SetText(wstrDesc.c_str());

	sITEM_TBLDAT* pItemTbldat = static_cast<sITEM_TBLDAT*>(API_GetTableContainer()->GetItemTable()->FindData(pEventTbldat->aIndex));
	if (pItemTbldat)
	{
		std::wstring wstrItem;
		pTextAllTable->GetItemTbl()->GetText(pItemTbldat->Name, &wstrItem);
		m_pstbItemName->SetText(wstrItem.c_str());

		m_ItemSlot.SetIcon(pEventTbldat->aIndex, 1);
	}

	m_pbtnGetReward->ClickEnable(TRUE);
}

VOID CEventRewardSubGui::ClearReward(VOID)
{
	m_pstbItemName->SetText(L"");
	m_ItemSlot.Clear();
	m_pbtnGetReward->ClickEnable(FALSE);

	if (m_bShowInfo && GetInfoWndManager()->GetRequestGui() == DIALOG_EVENTREWARD_SUB)
		GetInfoWndManager()->ShowInfoWindow(FALSE);

	m_bShowInfo = FALSE;
}

VOID CEventRewardSubGui::OnPaint(VOID)
{
	m_ItemSlot.Paint();
}

VOID CEventRewardSubGui::OnMove(RwInt32 iOldX, RwInt32 iOldY)
{
	CRectangle rtScreen = m_pThis->GetScreenRect();
	m_ItemSlot.SetParentPosition(rtScreen.left, rtScreen.top);
}

VOID CEventRewardSubGui::OnMouseMoveIcon(RwInt32 nFlags, RwInt32 nX, RwInt32 nY)
{
	if (m_bShowInfo || m_ItemSlot.GetSerial() == INVALID_SERIAL_ID)
		return;

	CRectangle rtScreen = m_ppnlItemIcon->GetScreenRect();
	GetInfoWndManager()->ShowInfoWindow(TRUE, CInfoWndManager::INFOWND_TABLE_ITEM,
		rtScreen.left, rtScreen.top, m_ItemSlot.GetItemTable(), DIALOG_EVENTREWARD_SUB);

	m_bShowInfo = TRUE;
}

VOID CEventRewardSubGui::OnMouseLeaveIcon(gui::CComponent* pComponent)
{
	if (GetInfoWndManager()->GetRequestGui() == DIALOG_EVENTREWARD_SUB)
		GetInfoWndManager()->ShowInfoWindow(FALSE);

	m_bShowInfo = FALSE;
}

VOID CEventRewardSubGui::OnClickGetReward(gui::CComponent* pComponent)
{
	if (m_eventTblidx == INVALID_TBLIDX)
		return;

	CHARACTERID avatarCharId = GetNtlSLGlobal()->GetSobAvatar()->GetCharID();

	// tw: a reward of another character can only be looked at
	if (m_charId != INVALID_CHARACTERID && m_charId != avatarCharId)
	{
		GetAlarmManager()->AlarmMessage("DST_REWARD_NOT_REWARD");
		return;
	}

	if (m_charId == INVALID_CHARACTERID)
	{
		// tw: an account reward asks first, only the current character receives it
		sMsgBoxData sData;
		sData.sEventReward.hNpc = m_hNPCSerial;
		sData.sEventReward.eventTblidx = m_eventTblidx;

		GetAlarmManager()->AlarmMessage("DST_REWARD_SENT_CHARACTER", FALSE, &sData);
		return;
	}

	GetDboGlobal()->GetGamePacketGenerator()->SendEventRewardSelectReq(m_hNPCSerial, m_eventTblidx);
}

VOID CEventRewardSubGui::OnClickClose(gui::CComponent* pComponent)
{
	GetDialogManager()->CloseDialog(DIALOG_EVENTREWARD_SUB);
}
