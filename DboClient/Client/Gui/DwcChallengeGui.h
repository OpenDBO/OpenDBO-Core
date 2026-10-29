/******************************************************************************
* File			: DwcChallengeGui.h
* Abstract		: Dragon World Challenge card collection
*****************************************************************************
* Desc			: port of the tw CDwcChallengeGui (DWC_Main.frm, dialog 0x92)
*				  every grade of every mission is a card, filtered by scenario,
*				  grade and state. The progress comes from UG_DWC_SCENARIO_INFO_REQ
*****************************************************************************/

#ifndef __DWC_CHALLENGE_GUI_H__
#define __DWC_CHALLENGE_GUI_H__

#pragma once

// core
#include "ceventhandler.h"

// presentation
#include "NtlPLGui.h"

// shared
#include "NtlDWC.h"

struct sDWC_TBLDAT;
struct sDWCMISSION_TBLDAT;

class CDwcChallengeGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
	// tw card states, also the data of the state combo
	enum eCARD_STATE
	{
		CARD_STATE_LOCK,		///< a later grade, or the scenario is not open
		CARD_STATE_OPEN,		///< the next grade to win, shows the clear count
		CARD_STATE_PLAY,		///< the grade the avatar holds now
		CARD_STATE_CLEAR,		///< a lower grade of the held card

		NUM_CARD_STATE
	};

	// tw CDwcCard: a button with the card image, the number above it and a lock or clear stamp
	struct sCARD
	{
		TBLIDX				dwcTblidx;
		sDWCMISSION_TBLDAT*	pMissionTbldat;
		RwUInt8				byMissionIndex;
		RwUInt8				byGrade;
		RwUInt8				byState;			///< eCARD_STATE
		RwUInt16			wClearCount;
		RwUInt8				byRequireCount;

		gui::CButton*		pbtnCard;
		gui::CStaticBox*	pstbNumber;
		gui::CPanel*		ppnlStamp;			///< srfLock / srfClear
		gui::CPanel*		ppnlGrade;
		gui::CPanel*		ppnlGageBg;
		gui::CPanel*		ppnlGage;
		gui::CStaticBox*	pstbProgress;
		gui::CTexture*		pTexture;

		gui::CSlot			slotMouseEnter;
		gui::CSlot			slotMouseLeave;
		gui::CSlot			slotClicked;
		RwUInt32			uiNumber;			///< "No.%03d" of the shown card
	};

public:
	CDwcChallengeGui(const RwChar* pName);
	~CDwcChallengeGui(VOID);

	RwBool		Create(VOID);
	VOID		Destroy(VOID);

	RwInt32		SwitchDialog(bool bOpen);
	VOID		HandleEvents(RWS::CMsg& msg);

private:
	VOID		CreateCards(VOID);
	sCARD*		CreateCard(sDWC_TBLDAT* pDwcTbldat, RwUInt8 byMissionIndex, sDWCMISSION_TBLDAT* pMissionTbldat, RwUInt8 byGrade);
	VOID		DestroyCards(VOID);

	VOID		AddComboListSurface(gui::CComboBox* pComboBox, const char* pcPrefix);
	VOID		RefreshScenarioList(VOID);
	VOID		RefreshCardState(VOID);
	VOID		SetCardState(sCARD* pCard, RwUInt8 byState);
	VOID		SetCardImage(sCARD* pCard);
	VOID		LayoutCards(VOID);
	VOID		Refresh(VOID);

	VOID		OnClickClose(gui::CComponent* pComponent);
	VOID		OnSelectFilter(INT nIndex);
	VOID		OnSelectScenario(INT nIndex);
	VOID		OnClickCard(gui::CComponent* pComponent);
	VOID		OnScrollChanged(RwInt32 iValue);
	VOID		OnMouseWheel(RwInt32 nFlag, RwInt16 sDelta, CPos& pos);
	VOID		OnMouseEnterCard(gui::CComponent* pComponent);
	VOID		OnMouseLeaveCard(gui::CComponent* pComponent);

	gui::CSlot			m_slotClickClose;
	gui::CSlot			m_slotSelectScenario;
	gui::CSlot			m_slotSelectGrade;
	gui::CSlot			m_slotSelectState;
	gui::CSlot			m_slotScrollChanged;
	gui::CSlot			m_slotScrollMoved;
	gui::CSlot			m_slotMouseWheel;

	gui::CStaticBox*	m_pstbTitle;
	gui::CStaticBox*	m_pstbDwcTitle;
	gui::CStaticBox*	m_pstbText;
	gui::CStaticBox*	m_pstbDwcPointName;
	gui::CStaticBox*	m_pstbDwcPoint;
	gui::CButton*		m_pbtnClose;
	gui::CPanel*		m_ppnlCardList;
	gui::CScrollBar*	m_pscbScroll;
	gui::CComboBox*		m_pcbbScenario;
	gui::CComboBox*		m_pcbbGrade;
	gui::CComboBox*		m_pcbbState;

	std::vector<sCARD*>	m_vecCard;			///< every card, in table order
	RwInt32				m_iListTop;			///< y of pnlCardList without the scroll
	std::wstring		m_wstrTooltip;
};

#endif//__DWC_CHALLENGE_GUI_H__
