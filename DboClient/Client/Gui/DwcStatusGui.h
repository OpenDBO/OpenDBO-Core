/******************************************************************************
* File			: DwcStatusGui.h
* Abstract		: Dragon World Challenge objectives side dialog
*****************************************************************************
* Desc			: port of the tw CDwcStatusGui (DWC_MissionInfo.frm)
*				  lists the event status the server script registers and owns the
*				  countdown of the script (GU_SERVERSCRIPT_COUNTDOWN_NFY)
*****************************************************************************/

#ifndef __DWC_STATUS_GUI_H__
#define __DWC_STATUS_GUI_H__

#pragma once

// core
#include "ceventhandler.h"

// presentation
#include "NtlPLGui.h"

class CCounterGui;

class CDwcStatusGui : public CNtlPLGui, public RWS::CEventHandler
{
	// tw: an objective is a text and, for the count objectives, a "%d/%d" on the right
	struct sOBJECTIVE
	{
		gui::CStaticBox*	pstbText;
		gui::CStaticBox*	pstbCount;
	};

public:
	CDwcStatusGui(const RwChar* pName);
	~CDwcStatusGui(VOID);

	static VOID		CreateInstance(VOID);
	static VOID		DestroyInstance(VOID);

	RwBool			Create(VOID);
	VOID			Destroy(VOID);

	RwInt32			SwitchDialog(bool bOpen);
	VOID			HandleEvents(RWS::CMsg& msg);

private:
	VOID			RefreshObjectives(VOID);
	VOID			ClearObjectives(VOID);

	VOID			StartCountdown(RwReal fSecond);
	VOID			StopCountdown(VOID);
	VOID			LocateCountdown(VOID);

	VOID			OnClickedBtnMinimize(gui::CComponent* pComponent);
	VOID			OnClickedBtnPrologInfo(gui::CComponent* pComponent);
	VOID			OnClickedBtnScenarioInfo(gui::CComponent* pComponent);
	VOID			OnScrollChanged(RwInt32 iValue);
	VOID			OnMouseWheel(RwInt32 nFlag, RwInt16 sDelta, CPos& pos);

	static CDwcStatusGui*	m_pInstance;

	gui::CSlot			m_slotClickedBtnMinimize;
	gui::CSlot			m_slotClickedBtnPrologInfo;
	gui::CSlot			m_slotClickedBtnScenarioInfo;
	gui::CSlot			m_slotScrollChanged;
	gui::CSlot			m_slotScrollMoved;
	gui::CSlot			m_slotMouseWheel;

	gui::CStaticBox*	m_pstbDialogName;
	gui::CStaticBox*	m_pstbManual;
	gui::CStaticBox*	m_pstbStep;
	gui::CButton*		m_pbtnMinimize;
	gui::CButton*		m_pbtnPrologInfo;
	gui::CButton*		m_pbtnScenarioInfo;
	gui::CPanel*		m_ppnlClippingFrame;
	gui::CPanel*		m_ppnlInfoTextView;
	gui::CScrollBar*	m_pscbScroll;

	std::vector<sOBJECTIVE>	m_vecObjective;
	RwInt32				m_iViewTop;			///< y of pnlInfoTextView without the scroll

	CCounterGui*		m_pCounterGui;
};

#endif//__DWC_STATUS_GUI_H__
