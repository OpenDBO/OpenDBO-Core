#ifndef __MASCOT_STATUSBAR_GUI_H__
#define __MASCOT_STATUSBAR_GUI_H__

#pragma once

// Core
#include "ceventhandler.h"

// Presentation
#include "NtlPLGui.h"

// Simulation
#include "NtlSLDef.h"

// Gui
#include "eventtimer.h"

// Client
#include "SurfaceGui.h"
#include "SlotGui.h"
#include "Windowby3.h"

class CMascotStatusBarGui : public CNtlPLGui, public RWS::CEventHandler
{

public:

	CMascotStatusBarGui(const RwChar* pName);
	virtual ~CMascotStatusBarGui();

	virtual RwInt32			SwitchDialog(bool bOpen);

	virtual VOID			Init();
	virtual RwBool			Create();
	virtual VOID			Destroy();

	static CMascotStatusBarGui* GetInstance();
	static void				 DeleteInstance();

	void					SetMascotID(BYTE index);

protected:

	virtual VOID	HandleEvents(RWS::CMsg& msg);

	VOID			OnMove(RwInt32 iOldX, RwInt32 iOldY);
	VOID			OnPaint(VOID);

protected:

	static CMascotStatusBarGui* m_pInstance;

	gui::CSlot		m_slotMove;
	gui::CSlot		m_slotPaint;

	gui::CStaticBox* m_sttNickName;
	gui::CStaticBox* m_sttSP;
	gui::CProgressBar* m_pgbSP;

	CSurfaceGui			m_surMascot;

};

#endif