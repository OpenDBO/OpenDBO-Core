/******************************************************************************
* File			: DwcScenarioInfoGui.h
* Abstract		: Dragon World Challenge scenario details
*****************************************************************************
* Desc			: port of the tw CDwcScenarioGui (DWC_ScenarioInfo.frm, dialog 0x94)
*				  opened by the scenario combo of the challenge: the players,
*				  the missions with their card grades and the admission conditions
*****************************************************************************/

#ifndef __DWC_SCENARIOINFO_GUI_H__
#define __DWC_SCENARIOINFO_GUI_H__

#pragma once

// core
#include "ceventhandler.h"

// presentation
#include "NtlPLGui.h"

// shared
#include "NtlDWC.h"

class CDwcScenarioInfoGui : public CNtlPLGui, public RWS::CEventHandler
{
	struct sCONDITION_ROW
	{
		gui::CMDStaticBox*	pmdbText;
		gui::CPanel*		ppnlCheck;
	};

public:
	CDwcScenarioInfoGui(const RwChar* pName);
	~CDwcScenarioInfoGui(VOID);

	RwBool		Create(VOID);
	VOID		Destroy(VOID);

	RwInt32		SwitchDialog(bool bOpen);
	VOID		HandleEvents(RWS::CMsg& msg);

	VOID		SetScenario(TBLIDX dwcTblidx);
	TBLIDX		GetScenario(VOID) { return m_dwcTblidx; }

private:
	VOID		Refresh(VOID);

	VOID		OnClickClose(gui::CComponent* pComponent);
	VOID		OnScrollChanged(RwInt32 iValue);
	VOID		OnMouseWheel(RwInt32 nFlag, RwInt16 sDelta, CPos& pos);

	gui::CSlot			m_slotClickClose;
	gui::CSlot			m_slotScrollChanged;
	gui::CSlot			m_slotScrollMoved;
	gui::CSlot			m_slotMouseWheel;

	gui::CStaticBox*	m_pstbTitle;
	gui::CStaticBox*	m_pstbScenarioTitle;
	gui::CStaticBox*	m_pstbPlayerCountText;
	gui::CStaticBox*	m_pstbPlayerCountValue;
	gui::CStaticBox*	m_pstbTargetText;
	gui::CStaticBox*	m_pstbCardGrade;
	gui::CStaticBox*	m_pstbEnterCondition;
	gui::CStaticBox*	m_apstbTarget[MAX_DWC_MISSION_COUNT_PER_SCENARIO];
	gui::CPanel*		m_appnlGrade[MAX_DWC_MISSION_COUNT_PER_SCENARIO][MAX_DWC_REWARD_GRADE_COUNT];
	gui::CStaticBox*	m_pstbLevel;
	gui::CPanel*		m_ppnlCheckLevel;
	gui::CPanel*		m_ppnlConditionScroll;
	gui::CScrollBar*	m_pscbScroll;
	gui::CButton*		m_pbtnClose;

	sCONDITION_ROW		m_asCondition[MAX_DWC_ADMISSION_CONDITION_COUNT];

	TBLIDX				m_dwcTblidx;
	RwInt32				m_iViewTop;
};

#endif//__DWC_SCENARIOINFO_GUI_H__
