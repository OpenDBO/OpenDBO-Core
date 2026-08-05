#ifndef __BROAD_CAST_SYSTEM_H__
#define __BROAD_CAST_SYSTEM_H__

#pragma once

// core
#include "ceventhandler.h"

// presentation
#include "NtlPLGui.h"

// Client UI Config
#include "DBOUIConfig.h"

// BroadCast
//#include "NtlPLAccelController.h"

// Client gui
#include "SurfaceGui.h"

#include <vector>

class CBroadCastSystem : public CNtlPLGui, public RWS::CEventHandler
{
public:

	CBroadCastSystem(const RwChar* pName);
	virtual ~CBroadCastSystem();

	RwBool		Create();
	VOID		Destroy();

	// HandleEvents
	VOID		HandleEvents(RWS::CMsg& msg);

	VOID		ResizeEventHandler(RWS::CMsg& msg);

	RwInt32		SwitchDialog(bool bOpen);

	void		Update(RwReal fElapsed);
	void		InitStringBoxFirst();
	void		InitStringBoxSecond();

	VOID		OnMouseEnter(gui::CComponent* pComponent);
	VOID		OnMouseLeave(gui::CComponent* pComponent);

protected:

	VOID		LocateComponent(RwInt32 iWidth, RwInt32 iHeight);

private:

	void		InitStringBox(gui::CStaticBox* pStringBox, RwBool& bIsOver, RwReal& fBoxPresentPosX);

	gui::CSlot			m_slotMouseEnter;
	gui::CSlot			m_slotMouseLeave;

	RwBool				m_bIsPaused;

	/*gui::CPanel*		m_pnlBackGround;
	gui::CStaticBox*	m_paStbInfoBox;*/

	gui::CStaticBox*	m_stBackGround;
	gui::CStaticBox*	m_stString;
	gui::CStaticBox*	m_stSecondString;

protected:

	int					m_iTextValues;

	RwBool				m_bIsOneTextOver;
	RwBool				m_bIsTwoTextOver;

	RwReal				m_fBoxPresentPosX;
	RwReal				m_fBoxPresentPosXTwo;
	std::vector<std::wstring>		m_sTextOne;
	std::vector<std::wstring>		m_sTextTwo;
};
#endif