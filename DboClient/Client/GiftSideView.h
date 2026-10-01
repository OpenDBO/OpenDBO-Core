#pragma once

// core
#include "ceventhandler.h"

// presentation
#include "NtlPLGui.h"

// dbo
#include "SideIconGui.h"
#include "DBOEvent.h"
#include "Windowby3.h"

/**
 * \ingroup Client
 * \brief Gift Shop(Wagu Point) 잔여 포인트를 표시하는 Side View
 */
class CGiftSideViewGui : public CSideViewBase, public RWS::CEventHandler
{
public:
	CGiftSideViewGui(const RwChar* pName);
	virtual ~CGiftSideViewGui(void);

	RwBool		Create();
	VOID		Destroy();

	virtual VOID	OnPressESC();
	virtual VOID	OnSideViewOpen(const void* pData);
	virtual VOID	OnSideViewClose();
	virtual VOID	OnSideViewLocate(const CRectangle& rectSideIcon);

protected:
	virtual VOID	HandleEvents( RWS::CMsg &msg );
	VOID			RefreshText();
	VOID			LocateComponent();
	VOID			OnMove(RwInt32 iOldX, RwInt32 iOldY);
	VOID			OnPaint();

protected:
	gui::CSlot			m_slotPaint;
	gui::CSlot			m_slotMove;

	CWindowby3			m_BackPanel;
	gui::CStaticBox*    m_pstbViewName;
};
