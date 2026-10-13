#pragma once

// core
#include "ceventhandler.h"

// presentation
#include "NtlPLGui.h"

// dbo
#include "SideIconGui.h"

/**
 * \ingroup Client
 * \brief Dragonball scramble side icon
 *
 * Visible while the scramble runs on the current channel. Clicking it opens the join confirmation,
 * the tooltip tells whether the avatar takes part.
 */
class CDBScrambleSideIconGui : public CSideIconBase, public RWS::CEventHandler
{
public:
	CDBScrambleSideIconGui(const RwChar* pName);
	virtual ~CDBScrambleSideIconGui(void);

	RwBool			Create();
	VOID			Destroy();

	virtual VOID	OnIconButtonClicked(gui::CComponent* pComponent);
	virtual VOID	OnSideViewClosed();
	virtual void	Show(bool bShow);

protected:
	virtual VOID	HandleEvents(RWS::CMsg& msg);

	void			UpdateToolTip();

protected:
	gui::CSlot      m_slotClicked;
	gui::CButton*   m_pBtnIcon;
};
