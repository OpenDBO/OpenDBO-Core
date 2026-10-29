/******************************************************************************
* File			: EventRewardMainGui.h
* Abstract		: reward list of the event manager npc
*****************************************************************************
* Desc			: port of the tw CEventRewardMainGui (eventrewardmaingui.frm, dialog 0x8D)
*				  12 rewards per page, a click shows the details in CEventRewardSubGui
*****************************************************************************/

#ifndef __EVENTREWARD_MAIN_GUI_H__
#define __EVENTREWARD_MAIN_GUI_H__

#pragma once

#include <map>
#include <string>
#include <vector>

// core
#include "ceventhandler.h"

// shared
#include "NtlSharedType.h"

// presentation
#include "NtlPLGui.h"

// simulation
#include "NtlSLDef.h"

#define dEVENTREWARD_PAGE_COUNT			12

// adwSetting[0] of a reward event: who may receive it
#define dEVENTREWARD_RECEIVER_CHARACTER	1
#define dEVENTREWARD_RECEIVER_ACCOUNT	2

// tw: the text of a reward event is in the etc text table
#define dEVENTREWARD_TEXT_NAME(tblidx)	(((tblidx) + 1000000) * 10)
#define dEVENTREWARD_TEXT_DESC(tblidx)	(dEVENTREWARD_TEXT_NAME(tblidx) + 1)

struct sEVENT_SYSTEM_TBLDAT;

// the reward event of the event system table, NULL if the table or the event is missing
sEVENT_SYSTEM_TBLDAT*	Logic_FindEventRewardTbldat(TBLIDX eventTblidx);

class CEventRewardMainGui : public CNtlPLGui, public RWS::CEventHandler
{
	struct sREWARD
	{
		TBLIDX			eventTblidx;
		CHARACTERID		charId;			///< INVALID_CHARACTERID: a reward of the whole account
		RwBool			bReceived;
	};

public:
	CEventRewardMainGui(const RwChar* pName);
	~CEventRewardMainGui(VOID);

	RwBool		Create(VOID);
	VOID		Destroy(VOID);

	RwInt32		SwitchDialog(bool bOpen);
	VOID		HandleEvents(RWS::CMsg& msg);

private:
	VOID		ClearList(VOID);
	VOID		EnableList(bool bEnable);
	VOID		SetPage(RwInt32 iPage);
	VOID		UpdatePage(VOID);

	VOID		OnClickEvent(gui::CComponent* pComponent);
	VOID		OnClickPageLeft(gui::CComponent* pComponent);
	VOID		OnClickPageRight(gui::CComponent* pComponent);
	VOID		OnClickClose(gui::CComponent* pComponent);

	gui::CSlot			m_aslotClickEvent[dEVENTREWARD_PAGE_COUNT];
	gui::CSlot			m_slotClickPageLeft;
	gui::CSlot			m_slotClickPageRight;
	gui::CSlot			m_slotClickClose;

	gui::CStaticBox*	m_pstbTitle;
	gui::CButton*		m_apbtnEvent[dEVENTREWARD_PAGE_COUNT];
	gui::CButton*		m_pbtnPageLeft;
	gui::CStaticBox*	m_pstbPage;
	gui::CButton*		m_pbtnPageRight;
	gui::CButton*		m_pbtnClose;

	std::vector<sREWARD>					m_vecReward;
	std::map<CHARACTERID, std::wstring>		m_mapName;		///< receiver names of the character rewards

	SERIAL_HANDLE		m_hNPCSerial;
	RwInt32				m_iCurPage;		///< 1 based like tw
	RwInt32				m_iPageCount;
};

#endif//__EVENTREWARD_MAIN_GUI_H__
