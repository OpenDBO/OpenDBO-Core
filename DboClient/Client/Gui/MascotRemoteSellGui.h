/*****************************************************************************
*
* File			: MascotRemoteSellGui.h
* Abstract		: Sell cart opened by the mascot remote shop skill
*****************************************************************************
* Desc         : Inventory items dropped on the cart are sold in one request,
*				 the server allows a single sale per skill use.
*****************************************************************************/

#pragma once

// Core
#include "ceventhandler.h"

// Presentation
#include "NtlPLGui.h"

// Simulation
#include "NtlSLDef.h"

// Shared
#include "NtlItem.h"

// Client
#include "SurfaceGui.h"
#include "SlotGui.h"

class CMascotRemoteSellGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
	CMascotRemoteSellGui(const RwChar* pName);
	virtual ~CMascotRemoteSellGui();

	VOID			Init();
	RwBool			Create();
	VOID			Destroy();
	RwInt32			SwitchDialog(bool bOpen);

	/// adds an inventory item to the cart, the whole stack
	RwBool			AddItem(SERIAL_HANDLE hItem);

protected:
	virtual VOID	HandleEvents(RWS::CMsg& msg);

	VOID			OnClickedClose(gui::CComponent* pComponent);
	VOID			OnClickedSell(gui::CComponent* pComponent);
	VOID			OnMouseUp(const CKey& key);
	VOID			OnMouseMove(RwInt32 nFlags, RwInt32 nX, RwInt32 nY);
	VOID			OnMouseLeave(gui::CComponent* pComponent);
	VOID			OnPaint();
	VOID			OnMove(RwInt32 iOldX, RwInt32 iOldY);

	RwInt32			PtInSlot(RwInt32 iX, RwInt32 iY);
	VOID			RemoveItem(RwInt32 iSlot);
	VOID			ClearAll(VOID);
	VOID			RefreshTotal(VOID);

protected:
	gui::CButton*		m_pExitButton;
	gui::CButton*		m_pSellButton;
	gui::CStaticBox*	m_pTotalSellMoney;

	gui::CSlot			m_slotExit;
	gui::CSlot			m_slotSell;
	gui::CSlot			m_slotMouseUp;
	gui::CSlot			m_slotMouseMove;
	gui::CSlot			m_slotMouseLeave;
	gui::CSlot			m_slotPaint;
	gui::CSlot			m_slotMove;

	CRegularSlotGui		m_Slot[NTL_MAX_SELL_SHOPPING_CART];
	SERIAL_HANDLE		m_ahItem[NTL_MAX_SELL_SHOPPING_CART];
	CSurfaceGui			m_SellBar;
	CSurfaceGui			m_surFocus;
	RwInt32				m_iFocusSlot;
};
