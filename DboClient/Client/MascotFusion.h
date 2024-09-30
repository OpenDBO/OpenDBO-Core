#ifndef __MASCOT_FUSION_GUI_H__
#define __MASCOT_FUSION_GUI_H__

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

class CMascotFusion : public CNtlPLGui, public RWS::CEventHandler
{

public:

	CMascotFusion(const RwChar* pName);
	virtual ~CMascotFusion();

	RwInt32			SwitchDialog(bool bOpen);

	VOID			Init();
	RwBool			Create();
	VOID			Destroy();


protected:

	virtual VOID	HandleEvents(RWS::CMsg& msg);

	VOID			ClickedCloseButton(gui::CComponent* pComponent);

protected:

	gui::CSlot				m_slotCloseButton;

	gui::CStaticBox* m_Stb_Title;
	gui::CHtmlBox* m_HtmlBox_Explan;
	gui::CButton* m_Btn_MascotFusion;
	gui::CButton* m_Btn_Close;
	gui::CProgressBar* m_Pgb_LGague;
	gui::CProgressBar* m_Pgb_RGague;
};


#endif