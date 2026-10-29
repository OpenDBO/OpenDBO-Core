/******************************************************************************
* File			: DwcBackGroundGui.h
* Abstract		: Dragon World Challenge curtain
*****************************************************************************
* Desc			: port of the tw CDwcBackGroundGui (Backboard.frm, dialog 0x4A)
*				  the curtain flash over the screen when the avatar arrives in a
*				  dwc world, it takes no input
*****************************************************************************/

#ifndef __DWC_BACKGROUND_GUI_H__
#define __DWC_BACKGROUND_GUI_H__

#pragma once

// core
#include "ceventhandler.h"

// presentation
#include "NtlPLGui.h"

class CDwcBackGroundGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
	CDwcBackGroundGui(const RwChar* pName);
	~CDwcBackGroundGui(VOID);

	RwBool		Create(VOID);
	VOID		Destroy(VOID);

	VOID		Update(RwReal fElapsed);

	RwInt32		SwitchDialog(bool bOpen);
	VOID		HandleEvents(RWS::CMsg& msg);

private:
	VOID		LocateComponent(VOID);
	VOID		OnMovieEnd(gui::CComponent* pComponent);

	gui::CSlot			m_slotMovieEnd;

	gui::CFlash*		m_pflaCurtain;
};

#endif//__DWC_BACKGROUND_GUI_H__
