#pragma once

// core
#include "ceventhandler.h"

// presentation
#include "NtlPLGui.h"

// tw: plays a tutorial flash while the avatar has a buff from one of the flight practice scrolls
// (the flight quests give them), the buff ending or the movie ending closes it
class CHelpWindowGuiByEvent : public CNtlPLGui, public RWS::CEventHandler
{
public:
	CHelpWindowGuiByEvent(const RwChar* pName);
	virtual ~CHelpWindowGuiByEvent();

	RwBool		Create();
	VOID		Destroy();
	VOID		Update(RwReal fElapsed);

protected:
	CHelpWindowGuiByEvent() {}
	virtual VOID	HandleEvents(RWS::CMsg& msg);

	RwBool			LoadFlash(const char* pcFileName);
	VOID			LocateComponent();
	VOID			CloseFlash();

	VOID			OnMovieEnd(gui::CComponent* pComponent);

protected:
	gui::CSlot			m_slotMovieEnd;

	gui::CFlash*		m_pFlash;

	RwUInt8				m_byBuffIndex;		///< buff the flash belongs to, our buff drop event has no table index
};
