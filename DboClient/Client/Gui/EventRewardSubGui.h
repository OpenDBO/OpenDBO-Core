/******************************************************************************
* File			: EventRewardSubGui.h
* Abstract		: details of the event reward picked in the event manager npc list
*****************************************************************************
* Desc			: port of the tw CEventRewardSubGui (eventrewardsubgui.frm, dialog 0x8E)
*				  shows the receiver, the description and the item, the button takes it
*****************************************************************************/

#ifndef __EVENTREWARD_SUB_GUI_H__
#define __EVENTREWARD_SUB_GUI_H__

#pragma once

// core
#include "ceventhandler.h"

// shared
#include "NtlSharedType.h"

// presentation
#include "NtlPLGui.h"

// simulation
#include "NtlSLDef.h"

// dbo
#include "SlotGui.h"

class CEventRewardSubGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
	CEventRewardSubGui(const RwChar* pName);
	~CEventRewardSubGui(VOID);

	RwBool		Create(VOID);
	VOID		Destroy(VOID);

	RwInt32		SwitchDialog(bool bOpen);
	VOID		HandleEvents(RWS::CMsg& msg);

private:
	VOID		SetReward(TBLIDX eventTblidx, CHARACTERID charId, const WCHAR* pwcName);
	VOID		ClearReward(VOID);

	VOID		OnPaint(VOID);
	VOID		OnMove(RwInt32 iOldX, RwInt32 iOldY);
	VOID		OnMouseMoveIcon(RwInt32 nFlags, RwInt32 nX, RwInt32 nY);
	VOID		OnMouseLeaveIcon(gui::CComponent* pComponent);
	VOID		OnClickGetReward(gui::CComponent* pComponent);
	VOID		OnClickClose(gui::CComponent* pComponent);

	gui::CSlot			m_slotPaint;
	gui::CSlot			m_slotMove;
	gui::CSlot			m_slotMouseMoveIcon;
	gui::CSlot			m_slotMouseLeaveIcon;
	gui::CSlot			m_slotClickGetReward;
	gui::CSlot			m_slotClickExit;
	gui::CSlot			m_slotClickClose;

	gui::CStaticBox*	m_pstbTitle;
	gui::CStaticBox*	m_pstbSenderTitle;
	gui::CStaticBox*	m_pstbSender;
	gui::CDialog*		m_pdlgReceiverTitle;
	gui::CStaticBox*	m_pstbReceiverTitle;
	gui::CDialog*		m_pdlgReceiver;
	gui::CStaticBox*	m_pstbReceiver;
	gui::CStaticBox*	m_pstbAccount;
	gui::CStaticBox*	m_pstbDesc;
	gui::CPanel*		m_ppnlItemIcon;
	gui::CStaticBox*	m_pstbItemName;
	gui::CButton*		m_pbtnGetReward;
	gui::CButton*		m_pbtnExit;
	gui::CButton*		m_pbtnClose;

	CRegularSlotGui		m_ItemSlot;

	SERIAL_HANDLE		m_hNPCSerial;
	TBLIDX				m_eventTblidx;
	CHARACTERID			m_charId;		///< receiver, INVALID_CHARACTERID: the character that takes it
	RwBool				m_bShowInfo;
};

#endif//__EVENTREWARD_SUB_GUI_H__
