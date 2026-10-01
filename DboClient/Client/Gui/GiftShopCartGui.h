/******************************************************************************
* File			: GiftShopCartGui.h
* Abstract		:
*****************************************************************************
* Desc			: Gift Shop (Wagu Point Shop) 의 Cart GUI 및 기능을 정의하는 Class
*****************************************************************************/

#ifndef __GIFTSHOP_CART_GUI_H__
#define __GIFTSHOP_CART_GUI_H__

#pragma once

// Core
#include "ceventhandler.h"

// Shared
#include "NtlPacketUG.h"

// Gui
#include "eventtimer.h"
#include "gui_slot.h"
#include "gui_button.h"
#include "gui_staticbox.h"

// Presentation
#include "NtlPLGui.h"

// Simulation
#include "NtlSLDef.h"

// Dbo
#include "SlotGui.h"
#include "SurfaceGui.h"

struct sSHOP_BUY_CART;
struct SDboEventGiftShopEvent;

#define dGIFTSHOP_CART_INVALID_INDEX		(-1)

class CGiftShopCartGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
	CGiftShopCartGui(const RwChar* pName);
	virtual ~CGiftShopCartGui();

	enum eGiftShopCartDefinedValue
	{
		MAX_SLOT				= 12,	///< 카트가 담을 수 있는 최대 슬롯 갯수 (NTL_MAX_BUY_SHOPPING_CART와 동일)
	};

	struct BuySlotInfo
	{
		sSHOP_BUY_CART		GiftShopBuyInfo;	///< 서버로 넘기기 위한 정보
		CRegularSlotGui		slot;
	};

	RwBool			Create();
	VOID			Destroy();

	RwInt32			SwitchDialog(bool bOpen);

	VOID			AddItem(RwInt32 iSlotY, RwInt32 iCount);
	VOID			SubItem(RwInt32 iSlotY, RwInt32 iCount);

protected:
	CGiftShopCartGui() {}
	virtual VOID	HandleEvents( RWS::CMsg &pMsg );

	VOID			Clear();
	VOID			ClearSlot(RwInt32 iSlot);

	VOID			AddItemCount(RwInt32 iSlot, RwInt32 iCount);

	VOID			CalcTotalBuyPrice();

	VOID			RegBuyItemByDrag(RwInt32 iSlot);
	VOID			RegBuyItemByEvent(RwInt32 iSlot, SDboEventGiftShopEvent& BuyInfo);

	RwInt32			FindEmptySlot();
	RwInt32			PtinSlot(RwInt32 iX, RwInt32 iY);

	VOID			CheckInfoWindow();

	VOID			FocusEffect( RwBool bPush, RwInt32 iSlotIdx = -1 );

	VOID			OnPaint();

	VOID			ClickedBuyButton(gui::CComponent* pComponent);

	VOID			ClickUpButton(gui::CComponent* pComponent);
	VOID			ClickDownButton(gui::CComponent* pComponent);

	VOID			OnMouseDown(const CKey& key);
	VOID			OnMouseUp(const CKey& key);
	VOID			OnMove( RwInt32 iOldX, RwInt32 iOldY );
	VOID			OnMouseMove(RwInt32 nFlags, RwInt32 nX, RwInt32 nY);
	VOID			OnMouseLeave(gui::CComponent* pComponent);
	VOID			OnCaptureWheelMove(RwInt32 iFlag, RwInt16 sDelta, CPos& pos);
	VOID			OnCaptureMouseDown(const CKey& key);

protected:
	gui::CSlot			m_slotMouseDown;
	gui::CSlot			m_slotMouseUp;
	gui::CSlot			m_slotMove;
	gui::CSlot			m_slotMouseMove;
	gui::CSlot			m_slotMouseLeave;
	gui::CSlot			m_slotCaptureWheelMove;
	gui::CSlot			m_slotPaint;
	gui::CSlot			m_slotCaptureMouseDown;
	gui::CSlot			m_slotClickedBuy;
	gui::CSlot			m_slotUpButton[MAX_SLOT];
	gui::CSlot			m_slotDownButton[MAX_SLOT];

	RwInt8				m_byInfoWindowIndex;
	RwInt32				m_iMouseDownSlot;

	gui::CButton*		m_pBuyButton;

	gui::CButton*		m_pUpButton[MAX_SLOT];
	gui::CButton*		m_pDownButton[MAX_SLOT];

	gui::CStaticBox*	m_pDialogName;
	gui::CStaticBox*	m_pTotalBuyMoney;

	CSurfaceGui			m_FocusEffect;

	RwBool				m_bFocus;

	BuySlotInfo			m_BuySlotInfo[MAX_SLOT];

	RwUInt32			m_uiTotalBuyPrice;
};

#endif
