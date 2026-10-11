#pragma once

// core
#include "ceventhandler.h"
#include "NtlSLEvent.h"

// presentation
#include "NtlPLGui.h"

// dbo
#include "SideIconGui.h"
#include "DboEvent.h"

/**
 * \ingroup Client
 * \brief Dynamic Field (World Boss event) side icon
 *
 * Shows the progress of the server wide monster kill counter in 3 stages. Once the counter is full the
 * field bosses spawn and the icon switches to the "boss" stage. Hovering shows the progress side view and
 * clicking it starts the world boss quest matching the avatar level.
 */
class CDynamicFieldSideIconGui : public CSideIconBase, public RWS::CEventHandler
{
public:
	enum eSTAGE
	{
		STAGE_ONE,				// 0 ~ 33%
		STAGE_TWO,				// 33 ~ 66%
		STAGE_THREE,			// 66 ~ 100%
		STAGE_BOSS,				// bosses spawned

		STAGE_COUNT,
		STAGE_INVALID = 0xFF
	};

public:
	CDynamicFieldSideIconGui(const RwChar* pName);
	virtual ~CDynamicFieldSideIconGui(void);

	RwBool			Create();
	VOID			Destroy();

	virtual VOID	OnIconButtonClicked(gui::CComponent* pComponent);
	virtual VOID	OnSideViewClosed();
	virtual void	Show(bool bShow);

	/// Lowest avatar level which can take part in the event (from the side icon quests' level check)
	static RwUInt8	GetMinParticipationLevel();

protected:
	virtual VOID	HandleEvents(RWS::CMsg& msg);
	void            OnMouseEnter(gui::CComponent* pComponent);
	void            OnMouseLeave(gui::CComponent* pComponent);

	void			SetStage(eSTAGE eStage);
	void			UpdateVisibility();

	static eSTAGE	CalcStage(const SDboEventDynamicFieldCount& rCount);

protected:
	gui::CButton*	m_apBtnStage[STAGE_COUNT];

	gui::CSlot      m_aSlotClicked[STAGE_COUNT];
	gui::CSlot      m_aSlotMouseEnter[STAGE_COUNT];
	gui::CSlot      m_aSlotMouseLeave[STAGE_COUNT];

	eSTAGE			m_eStage;
	bool			m_bReceivedCount;	///< server sent the event status at least once
	bool			m_bCanParticipate;	///< avatar level is high enough
};
