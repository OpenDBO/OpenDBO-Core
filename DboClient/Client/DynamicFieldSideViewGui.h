#pragma once

// core
#include "ceventhandler.h"

// presentation
#include "NtlPLGui.h"

// dbo
#include "Windowby3.h"
#include "SideIconGui.h"
#include "DboEvent.h"

/**
 * \ingroup Client
 * \brief Dynamic Field (World Boss event) side view
 *
 * Tooltip of CDynamicFieldSideIconGui. Shows the kill counter progress (or the remaining boss event time)
 * and whether the avatar already has the world boss quest.
 */
class CDynamicFieldSideViewGui : public CSideViewBase, public RWS::CEventHandler
{
public:
	CDynamicFieldSideViewGui(const RwChar* pName);
	virtual ~CDynamicFieldSideViewGui(void);

	RwBool			Create();
	VOID			Destroy();

	virtual VOID	Update(RwReal fElapsed);

	virtual VOID	OnPressESC();
	virtual VOID	OnSideViewOpen(const void* pData);
	virtual VOID	OnSideViewClose();
	virtual VOID	OnSideViewLocate(const CRectangle& rectSideIcon);

protected:
	virtual VOID	HandleEvents(RWS::CMsg& msg);

	VOID			LocateComponent();
	VOID			OnMove(RwInt32 iOldX, RwInt32 iOldY);
	VOID			OnPaint();

	gui::CStaticBox* CreateStaticBox(RwInt32 nY, RwUInt32 uiTextStyle, RwInt32 nFontHeight, COLORREF color, RwInt32 nX = 0);

	VOID			RefreshProgressText();
	VOID			RefreshQuestText();

	static bool		HasDynamicFieldQuest();

protected:
	CWindowby3			m_BackPanel;

	gui::CSlot			m_slotPaint;
	gui::CSlot			m_slotMove;

	gui::CStaticBox*	m_pstbTitle;
	gui::CStaticBox*	m_pstbProgress;		///< progress percent
	gui::CStaticBox*	m_pstbMapInfo;
	gui::CStaticBox*	m_pstbCount;		///< cur / max kills, or remaining time while bosses are out
	gui::CStaticBox*	m_pstbInfo1;
	gui::CStaticBox*	m_pstbInfo2;
	gui::CStaticBox*	m_pstbQuest;

	SDboEventDynamicFieldCount	m_sCount;			///< last status received from the server
	RwReal						m_fRemainEventTime;	///< seconds left of the boss event (client side countdown)
	RwInt32						m_nShownEventSecs;	///< last displayed countdown value
};
