#ifndef __MASCOT_FUSION_GUI_H__
#define __MASCOT_FUSION_GUI_H__

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
#include "DboLogic.h"

/**
* \brief Fuses two mascots: the middle one ranks up, the left one is sacrificed. An optional fusion
*		 item on the right makes the rank up certain. The evolved mascot appears in the middle once
*		 both gauges have filled.
*/
class CMascotFusion : public CNtlPLGui, public RWS::CEventHandler
{
public:
	enum eFUSION_SLOT
	{
		FUSION_SLOT_OFFERING,	// left, the mascot that is sacrificed
		FUSION_SLOT_MAIN,		// middle, the mascot that ranks up, shows the evolved one once fused
		FUSION_SLOT_ITEM,		// right, the optional fusion item

		FUSION_SLOT_COUNT
	};

	CMascotFusion(const RwChar* pName);
	virtual ~CMascotFusion();

	RwInt32			SwitchDialog(bool bOpen);

	VOID			Init();
	RwBool			Create();
	VOID			Destroy();

	VOID			SetMainMascot(BYTE byIndex);
	VOID			SetOfferingMascot(BYTE byIndex);
	BYTE			GetMainMascot(void) const { return m_byMainIndex; }
	VOID			SetItem(SERIAL_HANDLE hItem);

	RwBool			IsFusing(void) const { return m_bFilling; }
	/// the server answer, the mascot list is updated once the animation has finished
	VOID			SetFusionResult(const SMascotFusionResult& sResult);

	virtual VOID	Update(RwReal fElapsed);

protected:
	virtual VOID	HandleEvents(RWS::CMsg& msg);

	VOID			ClickedCloseButton(gui::CComponent* pComponent);
	VOID			ClickedFusionButton(gui::CComponent* pComponent);

	VOID			OnMouseEnter(gui::CComponent* pComponent);
	VOID			OnMouseLeave(gui::CComponent* pComponent);
	VOID			OnMouseUp(const CKey& key);
	VOID			OnPaint();
	VOID			OnMove(RwInt32 iOldX, RwInt32 iOldY);

	VOID			DropItem(VOID);
	VOID			ClearItem(VOID);
	VOID			Refresh(VOID);
	VOID			StartFill(VOID);
	VOID			StopFill(VOID);
	VOID			RevealResult(VOID);
	VOID			ShowResultMessages(RwBool bSuccess);
	VOID			SetFused(RwBool bFused);

protected:
	gui::CStaticBox*	m_Stb_Title;
	gui::CHtmlBox*		m_HtmlBox_Explan;
	gui::CButton*		m_Btn_MascotFusion;
	gui::CButton*		m_Btn_Close;
	gui::CProgressBar*	m_Pgb_LGague;
	gui::CProgressBar*	m_Pgb_RGague;
	gui::CPanel*		m_pPanel[FUSION_SLOT_COUNT];
	gui::CPanel*		m_pCircle;

	gui::CSlot			m_slotCloseButton;
	gui::CSlot			m_slotFusionButton;
	gui::CSlot			m_slotPaint;
	gui::CSlot			m_slotMove;
	gui::CSlot			m_slotEnter[FUSION_SLOT_COUNT];
	gui::CSlot			m_slotLeave[FUSION_SLOT_COUNT];
	gui::CSlot			m_slotUp[FUSION_SLOT_COUNT];

	CRegularSlotGui		m_Slot[FUSION_SLOT_COUNT];
	CSurfaceGui			m_surFocus;

	BYTE				m_byMainIndex;
	BYTE				m_byOfferingIndex;
	SERIAL_HANDLE		m_hItem;
	RwInt32				m_iFocusSlot;
	RwBool				m_bFused;				///< gauges and circle stay lit after a successful fusion

	RwBool				m_bFilling;
	RwReal				m_fFillTime;
	RwBool				m_bRequestPending;
	RwBool				m_bResultArrived;
	RwBool				m_bResultSuccess;
	SMascotFusionResult	m_sResult;
};

#endif
