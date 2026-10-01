#pragma once

// core
#include "ceventhandler.h"
#include "NtlSLEvent.h"

// presentation
#include "NtlPLGui.h"

// dbo
#include "SideIconGui.h"

/**
 * \ingroup Client
 * \brief Gift Shop (Wagu Point Shop) side icon
 */
class CGiftSideIconGui : public CSideIconBase, public RWS::CEventHandler
{
public:
	CGiftSideIconGui(const RwChar* pName);
	virtual ~CGiftSideIconGui(void);

	RwBool			Create();
	VOID			Destroy();

	virtual VOID	OnIconButtonClicked(gui::CComponent* pComponent);
	virtual VOID	OnSideViewClosed();
	virtual void	Show(bool bShow);

protected:
	virtual VOID	HandleEvents(RWS::CMsg &msg);
	void            OnMouseEnter(gui::CComponent* pComponent);
	void            OnMouseLeave(gui::CComponent* pComponent);

protected:
	gui::CSlot      m_slotGiftBtn;
	gui::CSlot      m_slotGiftMouseEnter;
	gui::CSlot      m_slotGiftMouseLeave;
	gui::CButton*   m_pBtnGift;
};
