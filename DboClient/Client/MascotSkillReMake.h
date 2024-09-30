#ifndef __MASCOT_SKILL_REMAKE_GUI_H__
#define __MASCOT_SKILL_REMAKE_GUI_H__

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

class CMascotSkillReMake : public CNtlPLGui, public RWS::CEventHandler
{

public:

	CMascotSkillReMake(const RwChar* pName);
	virtual ~CMascotSkillReMake();

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
	gui::CStaticBox* m_Stb_OldSkillName;
	gui::CStaticBox* m_Stb_NewSkillName;
	gui::CStaticBox* m_Stb_TypeOld;
	gui::CStaticBox* m_Stb_TypeItem;
	gui::CStaticBox* m_Stb_TypeNew;
	gui::CStaticBox* m_Stb_TypeBefore;
	gui::CStaticBox* m_Stb_TypeAfter;
	gui::CButton* m_Btn_Close;
	gui::CButton* m_Btn_MascotSkillRemake;
	gui::CProgressBar* m_Pgb_LeftGague;
	gui::CProgressBar* m_Pgb_RightGague;
	gui::CHtmlBox* m_HtmlBox_Explan;
};

#endif