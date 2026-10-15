#ifndef __MASCOT_SKILL_REMAKE_GUI_H__
#define __MASCOT_SKILL_REMAKE_GUI_H__

#pragma once

// Core
#include "ceventhandler.h"

// Presentation
#include "NtlPLGui.h"

// Simulation
#include "NtlSLDef.h"

// Client
#include "SurfaceGui.h"
#include "SlotGui.h"

/**
* \brief Learns a random skill in an empty slot of the summoned mascot, or re-rolls
*		 a learned one with a skill change item.
*/
class CMascotSkillReMake : public CNtlPLGui, public RWS::CEventHandler
{
public:
	enum eREMAKE_SLOT
	{
		REMAKE_SLOT_OLD,
		REMAKE_SLOT_ITEM,
		REMAKE_SLOT_NEW,

		REMAKE_SLOT_COUNT
	};

	CMascotSkillReMake(const RwChar* pName);
	virtual ~CMascotSkillReMake();

	RwInt32			SwitchDialog(bool bOpen);

	VOID			Init();
	RwBool			Create();
	VOID			Destroy();

	/// opens on bySkillSlot of the mascot, keeping an item already placed
	VOID			OpenSlot(BYTE byMascotIndex, BYTE bySkillSlot);

	/// opens with a skill change item used from the inventory, or places it if already open
	VOID			OpenWithItem(SERIAL_HANDLE hItem);

	BYTE			GetSkillSlot(void) const { return m_bySkillSlot; }

	/// a learned or re-rolled skill is shown once the gauges finish filling
	RwBool			IsRevealPending(BYTE byMascotIndex, BYTE bySkillSlot) const;

	virtual VOID	Update(RwReal fElapsed);

protected:
	virtual VOID	HandleEvents(RWS::CMsg& msg);

	VOID			ClickedCloseButton(gui::CComponent* pComponent);
	VOID			ClickedRemakeButton(gui::CComponent* pComponent);

	VOID			OnMouseEnter(gui::CComponent* pComponent);
	VOID			OnMouseLeave(gui::CComponent* pComponent);
	VOID			OnMouseUp(const CKey& key);
	VOID			OnPaint();
	VOID			OnMove(RwInt32 iOldX, RwInt32 iOldY);

	VOID			SetItem(SERIAL_HANDLE hItem);
	VOID			ClearItem(VOID);
	VOID			Refresh(VOID);
	RwBool			IsCreateMode(VOID);

	VOID			StartFill(VOID);
	VOID			StopFill(VOID);
	VOID			RevealResult(VOID);

protected:
	gui::CStaticBox*	m_Stb_Title;
	gui::CStaticBox*	m_Stb_OldSkillName;
	gui::CStaticBox*	m_Stb_NewSkillName;
	gui::CStaticBox*	m_Stb_TypeOld;
	gui::CStaticBox*	m_Stb_TypeItem;
	gui::CStaticBox*	m_Stb_TypeNew;
	gui::CStaticBox*	m_Stb_TypeBefore;
	gui::CStaticBox*	m_Stb_TypeAfter;
	gui::CButton*		m_Btn_Close;
	gui::CButton*		m_Btn_MascotSkillRemake;
	gui::CProgressBar*	m_Pgb_LeftGague;
	gui::CProgressBar*	m_Pgb_RightGague;
	gui::CHtmlBox*		m_HtmlBox_Explan;
	gui::CPanel*		m_pPanel[REMAKE_SLOT_COUNT];

	gui::CSlot			m_slotCloseButton;
	gui::CSlot			m_slotRemakeButton;
	gui::CSlot			m_slotPaint;
	gui::CSlot			m_slotMove;
	gui::CSlot			m_slotEnter[REMAKE_SLOT_COUNT];
	gui::CSlot			m_slotLeave[REMAKE_SLOT_COUNT];
	gui::CSlot			m_slotUp[REMAKE_SLOT_COUNT];

	CRegularSlotGui		m_Slot[REMAKE_SLOT_COUNT];
	CSurfaceGui			m_surFocus;

	BYTE				m_byMascotIndex;
	BYTE				m_bySkillSlot;
	TBLIDX				m_oldSkillTblidx;		///< skill the slot had when it was opened, shown on the left
	TBLIDX				m_newSkillTblidx;		///< result of the last learn or re-roll, shown on the right
	SERIAL_HANDLE		m_hItem;
	RwInt32				m_iFocusSlot;

	RwBool				m_bFilling;			///< gauges are filling after a request
	RwReal				m_fFillTime;
	RwBool				m_bResultArrived;
};

#endif
