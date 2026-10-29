/******************************************************************************
* File			: DwcRewardGui.h
* Abstract		: Dragon World Challenge mission result
*****************************************************************************
* Desc			: port of the tw CDwcReward (DWC_Reward.frm, dialog 0x49)
*				  shows the card of the cleared mission, the title it gave and the
*				  reward items, the items come out one by one after the card dropped
*****************************************************************************/

#ifndef __DWC_REWARD_GUI_H__
#define __DWC_REWARD_GUI_H__

#pragma once

// core
#include "ceventhandler.h"

// presentation
#include "NtlPLGui.h"

// dbo
#include "SlotGui.h"
#include "SurfaceGui.h"

// tw: 4 basic and 4 repeat reward slots
#define dDWC_REWARD_SLOT_COUNT		8

class CDwcRewardGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
	CDwcRewardGui(const RwChar* pName);
	~CDwcRewardGui(VOID);

	RwBool		Create(VOID);
	VOID		Destroy(VOID);

	VOID		Update(RwReal fElapsed);

	RwInt32		SwitchDialog(bool bOpen);
	VOID		HandleEvents(RWS::CMsg& msg);

private:
	VOID		SetReward(RwUInt32 dwcMissionTblidx, RwBool bIsBasicReward, RwUInt8 byRewardGrade);
	VOID		ClearReward(VOID);
	VOID		LoadFlash(gui::CFlash* pFlash, const char* pcFileName);
	VOID		LocateComponent(VOID);
	VOID		RevealNextSlot(VOID);

	VOID		OnClickConfirm(gui::CComponent* pComponent);
	VOID		OnPaintSlot(VOID);
	VOID		OnMouseMove(RwInt32 nFlags, RwInt32 nX, RwInt32 nY);
	VOID		OnMouseLeave(gui::CComponent* pComponent);
	VOID		OnMovieEnd(gui::CComponent* pComponent);

	gui::CSlot			m_slotClickConfirm;
	gui::CSlot			m_slotPaintSlot;
	gui::CSlot			m_slotMouseMove;
	gui::CSlot			m_slotMouseLeave;
	gui::CSlot			m_slotCardEnd;
	gui::CSlot			m_slotCapsuleEnd;

	gui::CFlash*		m_pflaResultBg;
	gui::CFlash*		m_pflaResult;
	gui::CFlash*		m_pflaCard;
	gui::CFlash*		m_pflaCapsule;
	gui::CPanel*		m_ppnlCard;
	gui::CPanel*		m_ppnlCardGrade;
	gui::CStaticBox*	m_pstbMission;
	gui::CStaticBox*	m_pstbCharTitle;
	gui::CButton*		m_pbtnConfirm;

	gui::CTexture*		m_pCardTexture;

	CRegularSlotGui		m_aSlot[dDWC_REWARD_SLOT_COUNT];
	CSurfaceGui			m_asrfSlotBack[dDWC_REWARD_SLOT_COUNT];
	RwInt32				m_iRevealCount;		///< tw: the slots below it are shown
	RwInt32				m_iFocusSlot;
};

#endif//__DWC_REWARD_GUI_H__
