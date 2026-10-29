#pragma once

// core
#include "ceventhandler.h"

// presentation
#include "NtlPLGui.h"

// dbo
#include "SideIconGui.h"
#include "Windowby3.h"

/**
 * \ingroup Client
 * \brief Dragon World Challenge side icon (tw CDwcSideIconGui, DWC_SideIcon.frm)
 *        tells about a new card or a new admission condition, opens the challenge
 */
class CDwcSideIconGui : public CSideIconBase, public RWS::CEventHandler
{
public:
	CDwcSideIconGui(const RwChar* pName);
	virtual ~CDwcSideIconGui(void);

	RwBool			Create();
	VOID			Destroy();

	virtual VOID	OnIconButtonClicked(gui::CComponent* pComponent);
	virtual VOID	OnSideViewClosed();

protected:
	virtual VOID	HandleEvents(RWS::CMsg &msg);
	void			OnMouseEnter(gui::CComponent* pComponent);
	void			OnMouseLeave(gui::CComponent* pComponent);

protected:
	gui::CSlot		m_slotIconButton;
	gui::CSlot		m_slotMouseEnter;
	gui::CSlot		m_slotMouseLeave;
	gui::CButton*	m_pbtnIcon;
};

/**
 * \ingroup Client
 * \brief Dragon World Challenge side view (tw CDwcSideViewGui, DWCsideView.frm)
 */
class CDwcSideViewGui : public CSideViewBase
{
public:
	CDwcSideViewGui(const RwChar* pName);
	virtual ~CDwcSideViewGui(void);

	RwBool			Create();
	VOID			Destroy();

	virtual VOID	OnPressESC();
	virtual VOID	OnSideViewOpen(const void* pData);
	virtual VOID	OnSideViewClose();
	virtual VOID	OnSideViewLocate(const CRectangle& rectSideIcon);

protected:
	VOID			OnMove(RwInt32 iOldX, RwInt32 iOldY);
	VOID			OnPaint();

protected:
	gui::CSlot			m_slotPaint;
	gui::CSlot			m_slotMove;

	CWindowby3			m_BackPanel;
	gui::CStaticBox*	m_pstbViewName;
};
