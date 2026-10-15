#include "precomp_dboclient.h"
#include "MascotRemoteSellGui.h"

// core
#include "NtlDebug.h"

// shared
#include "ItemTable.h"

// presentation
#include "NtlPLGuiManager.h"

// simulation
#include "NtlSLGlobal.h"
#include "NtlSobManager.h"
#include "NtlSobItem.h"
#include "NtlSobItemAttr.h"

// dbo
#include "DboGlobal.h"
#include "DboLogic.h"
#include "DboEvent.h"
#include "DialogManager.h"
#include "DisplayStringManager.h"
#include "IconMoveManager.h"
#include "InfoWndManager.h"
#include "AlarmManager.h"

namespace
{
	// sell column of TradeCart.frm
	const RwInt32 SELL_SLOT_X			= 18 + 97;
	const RwInt32 SELL_SLOT_Y			= 57;
	const RwInt32 SELL_SLOT_GAP_X		= 42;
	const RwInt32 SELL_SLOT_GAP_Y		= 57;

	// the server keeps 10% of a remote sale
	const RwReal REMOTE_SELL_RATE		= 0.9f;

	CNtlSobItem* GetSobItem(SERIAL_HANDLE hItem)
	{
		CNtlSob* pSob = GetNtlSobManager()->GetSobObject(hItem);
		if (pSob == NULL || pSob->GetClassID() != SLCLASS_SLOT_ITEM)
			return NULL;

		return reinterpret_cast<CNtlSobItem*>(pSob);
	}
}

CMascotRemoteSellGui::CMascotRemoteSellGui(const RwChar* pName)
: CNtlPLGui(pName)
{
	Init();
}

CMascotRemoteSellGui::~CMascotRemoteSellGui()
{
}

VOID CMascotRemoteSellGui::Init()
{
	m_pExitButton = NULL;
	m_pSellButton = NULL;
	m_pTotalSellMoney = NULL;
	m_iFocusSlot = -1;

	for (RwInt32 i = 0; i < NTL_MAX_SELL_SHOPPING_CART; ++i)
		m_ahItem[i] = INVALID_SERIAL_ID;
}

RwBool CMascotRemoteSellGui::Create()
{
	NTL_FUNCTION("CMascotRemoteSellGui::Create");

	if (!CNtlPLGui::Create("", "gui\\TradeCart.srf", "gui\\TradeCart.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(GetNtlGuiManager()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");
	m_pThis->SetPriority(dDIALOGPRIORITY_DEFAULT);

	m_pExitButton = (gui::CButton*)GetComponent("ExitButton");
	m_pSellButton = (gui::CButton*)GetComponent("SellButton");

	// only the sell half of the cart is used
	gui::CComponent* pBuyButton = GetComponent("BuyButton");
	if (pBuyButton)
		pBuyButton->Show(false);

	gui::CComponent* pZennyButton = GetComponent("BtnZenny");
	if (pZennyButton)
		pZennyButton->Show(false);

	m_pSellButton->SetText(GetDisplayStringManager()->GetString("DST_TRADECART_SELL"));

	CRectangle rect;
	rect.SetRectWH(114, 406, 61, 16);
	m_pTotalSellMoney = NTL_NEW gui::CStaticBox(rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_RIGHT);
	m_pTotalSellMoney->CreateFontStd(DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pTotalSellMoney->Enable(false);

	for (RwInt32 i = 0; i < NTL_MAX_SELL_SHOPPING_CART; ++i)
	{
		m_Slot[i].Create(m_pThis, DIALOG_MASCOT_REMOTE_SELL, REGULAR_SLOT_ITEM_SOB, SDS_COUNT | SFF_USE_ITEM_OPTION_TABLE);
		m_Slot[i].SetPosition_fromParent(SELL_SLOT_X + (i % 2) * SELL_SLOT_GAP_X, SELL_SLOT_Y + (i / 2) * SELL_SLOT_GAP_Y);
	}

	m_SellBar.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("TradeCart.srf", "srfSellbar"));
	m_SellBar.SetPositionfromParent(133, 28);

	m_surFocus.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("GameCommon.srf", "srfSlotFocusEffect"));
	m_surFocus.Show(FALSE);

	m_slotExit = m_pExitButton->SigClicked().Connect(this, &CMascotRemoteSellGui::OnClickedClose);
	m_slotSell = m_pSellButton->SigClicked().Connect(this, &CMascotRemoteSellGui::OnClickedSell);
	m_slotMouseUp = m_pThis->SigMouseUp().Connect(this, &CMascotRemoteSellGui::OnMouseUp);
	m_slotMouseMove = m_pThis->SigMouseMove().Connect(this, &CMascotRemoteSellGui::OnMouseMove);
	m_slotMouseLeave = m_pThis->SigMouseLeave().Connect(this, &CMascotRemoteSellGui::OnMouseLeave);
	m_slotPaint = m_pThis->SigPaint().Connect(this, &CMascotRemoteSellGui::OnPaint);
	m_slotMove = m_pThis->SigMove().Connect(this, &CMascotRemoteSellGui::OnMove);

	LinkMsg(g_EventMascotUnSummon);
	LinkMsg(g_EventDialog);

	RefreshTotal();
	Show(false);

	NTL_RETURN(TRUE);
}

VOID CMascotRemoteSellGui::Destroy()
{
	UnLinkMsg(g_EventMascotUnSummon);
	UnLinkMsg(g_EventDialog);

	for (RwInt32 i = 0; i < NTL_MAX_SELL_SHOPPING_CART; ++i)
		m_Slot[i].Destroy();

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();
}

RwInt32 CMascotRemoteSellGui::SwitchDialog(bool bOpen)
{
	if (bOpen)
	{
		CRectangle rtScreen = m_pThis->GetScreenRect();
		OnMove(rtScreen.left, rtScreen.top);
		GetDialogManager()->SwitchBag(TRUE);
	}
	else
	{
		ClearAll();

		if (GetInfoWndManager()->GetRequestGui() == DIALOG_MASCOT_REMOTE_SELL)
			GetInfoWndManager()->ShowInfoWindow(FALSE);
	}

	Show(bOpen);
	return 1;
}

VOID CMascotRemoteSellGui::HandleEvents(RWS::CMsg& msg)
{
	// the remote shop goes away with the mascot
	if (msg.Id == g_EventMascotUnSummon && IsShow())
		GetDialogManager()->CloseDialog(DIALOG_MASCOT_REMOTE_SELL);
	else if (msg.Id == g_EventDialog)
	{
		// it is an npc dialog, ESC and closing every window say goodbye instead of closing it
		SDboEventDialog* pEvent = reinterpret_cast<SDboEventDialog*>(msg.pData);
		if (pEvent->iDestDialog == DIALOG_MASCOT_REMOTE_SELL && pEvent->iType == DIALOGEVENT_NPC_BYEBYE && IsShow())
			GetDialogManager()->CloseDialog(DIALOG_MASCOT_REMOTE_SELL);
	}
}

RwBool CMascotRemoteSellGui::AddItem(SERIAL_HANDLE hItem)
{
	CNtlSobItem* pItem = GetSobItem(hItem);
	if (pItem == NULL)
		return FALSE;

	CNtlSobItemAttr* pItemAttr = reinterpret_cast<CNtlSobItemAttr*>(pItem->GetSobAttr());
	sITEM_TBLDAT* pItemTbl = pItemAttr->GetItemTbl();
	if (pItemTbl == NULL || pItemTbl->dwSell_Price == 0)
	{
		GetAlarmManager()->AlarmMessage("DST_ITEM_CAN_NOT_SELL");
		return FALSE;
	}

	RwInt32 iEmpty = -1;
	for (RwInt32 i = 0; i < NTL_MAX_SELL_SHOPPING_CART; ++i)
	{
		if (m_ahItem[i] == hItem)
			return FALSE;

		if (iEmpty < 0 && m_ahItem[i] == INVALID_SERIAL_ID)
			iEmpty = i;
	}

	if (iEmpty < 0)
		return FALSE;

	m_ahItem[iEmpty] = hItem;
	m_Slot[iEmpty].SetIcon(hItem, pItemAttr->GetStackNum());
	Logic_EnableBagItemIcon(hItem, FALSE);

	RefreshTotal();
	return TRUE;
}

VOID CMascotRemoteSellGui::RemoveItem(RwInt32 iSlot)
{
	if (iSlot < 0 || iSlot >= NTL_MAX_SELL_SHOPPING_CART || m_ahItem[iSlot] == INVALID_SERIAL_ID)
		return;

	Logic_EnableBagItemIcon(m_ahItem[iSlot], TRUE);
	m_ahItem[iSlot] = INVALID_SERIAL_ID;
	m_Slot[iSlot].Clear();

	RefreshTotal();
}

VOID CMascotRemoteSellGui::ClearAll(VOID)
{
	for (RwInt32 i = 0; i < NTL_MAX_SELL_SHOPPING_CART; ++i)
		RemoveItem(i);
}

VOID CMascotRemoteSellGui::RefreshTotal(VOID)
{
	DWORD dwTotal = 0;

	for (RwInt32 i = 0; i < NTL_MAX_SELL_SHOPPING_CART; ++i)
	{
		CNtlSobItem* pItem = GetSobItem(m_ahItem[i]);
		if (pItem == NULL)
			continue;

		CNtlSobItemAttr* pItemAttr = reinterpret_cast<CNtlSobItemAttr*>(pItem->GetSobAttr());
		if (pItemAttr->GetItemTbl())
			dwTotal += pItemAttr->GetItemTbl()->dwSell_Price * pItemAttr->GetStackNum();
	}

	m_pTotalSellMoney->SetText(Logic_FormatZeni((DWORD)(dwTotal * REMOTE_SELL_RATE)));
}

VOID CMascotRemoteSellGui::OnClickedClose(gui::CComponent* pComponent)
{
	GetAlarmManager()->AlarmMessage("DST_MASCOTEX_SKILL_REMOTE_SELL_FINISH");
	GetDialogManager()->CloseDialog(DIALOG_MASCOT_REMOTE_SELL);
}

VOID CMascotRemoteSellGui::OnClickedSell(gui::CComponent* pComponent)
{
	sSHOP_SELL_CART asCart[NTL_MAX_SELL_SHOPPING_CART];
	BYTE byCount = 0;

	for (RwInt32 i = 0; i < NTL_MAX_SELL_SHOPPING_CART; ++i)
	{
		CNtlSobItem* pItem = GetSobItem(m_ahItem[i]);
		if (pItem == NULL)
			continue;

		CNtlSobItemAttr* pItemAttr = reinterpret_cast<CNtlSobItemAttr*>(pItem->GetSobAttr());

		if (!Logic_GetItemContainerPosFromHandle(m_ahItem[i], asCart[byCount].byPlace, asCart[byCount].byPos))
			continue;

		asCart[byCount].byStack = pItemAttr->GetStackNum();
		++byCount;
	}

	if (byCount == 0)
		return;

	GetDboGlobal()->GetGamePacketGenerator()->SendMascotRemoteShopSellReq(byCount, asCart);
}

RwInt32 CMascotRemoteSellGui::PtInSlot(RwInt32 iX, RwInt32 iY)
{
	for (RwInt32 i = 0; i < NTL_MAX_SELL_SHOPPING_CART; ++i)
	{
		if (m_Slot[i].PtInRect(iX, iY))
			return i;
	}

	return -1;
}

VOID CMascotRemoteSellGui::OnMouseUp(const CKey& key)
{
	// items dragged from the bag land in the cart
	if (GetIconMoveManager()->IsActive())
	{
		RwBool bFromBag = GetIconMoveManager()->GetSrcPlace() == PLACE_BAG;
		SERIAL_HANDLE hItem = GetIconMoveManager()->GetSrcSerial();
		GetIconMoveManager()->IconMoveEnd();

		if (bFromBag)
			AddItem(hItem);

		return;
	}

	if (key.m_nID == UD_RIGHT_BUTTON)
		RemoveItem(PtInSlot((RwInt32)key.m_fX, (RwInt32)key.m_fY));
}

VOID CMascotRemoteSellGui::OnMouseMove(RwInt32 nFlags, RwInt32 nX, RwInt32 nY)
{
	RwInt32 iSlot = PtInSlot(nX, nY);
	if (iSlot == m_iFocusSlot)
		return;

	m_iFocusSlot = iSlot;
	m_surFocus.Show(FALSE);

	if (GetInfoWndManager()->GetRequestGui() == DIALOG_MASCOT_REMOTE_SELL)
		GetInfoWndManager()->ShowInfoWindow(FALSE);

	if (iSlot < 0 || m_ahItem[iSlot] == INVALID_SERIAL_ID)
		return;

	CRectangle rtScreen = m_pThis->GetScreenRect();
	RwInt32 iX = rtScreen.left + m_Slot[iSlot].GetX_fromParent();
	RwInt32 iY = rtScreen.top + m_Slot[iSlot].GetY_fromParent();

	m_surFocus.SetRectWH(iX, iY, 32, 32);
	m_surFocus.Show(TRUE);

	GetInfoWndManager()->ShowInfoWindow(TRUE, CInfoWndManager::INFOWND_ITEM, iX, iY, m_Slot[iSlot].GetSobItem(), DIALOG_MASCOT_REMOTE_SELL);
}

VOID CMascotRemoteSellGui::OnMouseLeave(gui::CComponent* pComponent)
{
	m_iFocusSlot = -1;
	m_surFocus.Show(FALSE);

	if (GetInfoWndManager()->GetRequestGui() == DIALOG_MASCOT_REMOTE_SELL)
		GetInfoWndManager()->ShowInfoWindow(FALSE);
}

VOID CMascotRemoteSellGui::OnPaint()
{
	m_SellBar.Render();

	for (RwInt32 i = 0; i < NTL_MAX_SELL_SHOPPING_CART; ++i)
		m_Slot[i].Paint();

	m_surFocus.Render();
}

VOID CMascotRemoteSellGui::OnMove(RwInt32 iOldX, RwInt32 iOldY)
{
	CRectangle rtScreen = m_pThis->GetScreenRect();

	for (RwInt32 i = 0; i < NTL_MAX_SELL_SHOPPING_CART; ++i)
		m_Slot[i].SetParentPosition(rtScreen.left, rtScreen.top);

	m_SellBar.SetPositionbyParent(rtScreen.left, rtScreen.top);
}
