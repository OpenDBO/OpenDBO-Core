/******************************************************************************
* File			: PartynGui.h
* Author		: Hong SungBock
* Copyright		: (주)NTL
* Date			: 2008. 10. 17
* Abstract		: 
*****************************************************************************
* Desc			: 파티의 버프 GUI
*****************************************************************************/

#pragma once

// presetation
#include "NtlPLGui.h"

// dbo
#include "SlotGui.h"

class CPartyBuffGui : public CNtlPLGui
{
public:
	CPartyBuffGui(const RwChar* pName);
	virtual ~CPartyBuffGui();

	RwBool		Create(RwUInt8 byBuffIndex, RwUInt32 uiItemIndex);
	VOID		Destroy();

	VOID		Update( RwReal fElapsed );
	VOID		Switch(bool bOpen);	

	RwUInt8		GetBuffIndex();

protected:
	CPartyBuffGui() {}

	RwBool			SetBuffItemIndex(RwUInt8 byBuffIndex, RwUInt32 uiIndex);
	VOID			CheckInfoWindow();

	VOID			OnMouseDown(const CKey& key);
	VOID			OnMouseUp(const CKey& key);
	VOID			OnMove(RwInt32 iOldX, RwInt32 iOldY);
	VOID			OnMouseEnter(gui::CComponent* pComponent);
	VOID			OnMouseLeave(gui::CComponent* pComponent);
	VOID			OnPaint();

protected:
	gui::CSlot			m_slotMouseDown;
	gui::CSlot			m_slotMouseUp;
	gui::CSlot			m_slotMove;
	gui::CSlot			m_slotMouseMove;
	gui::CSlot			m_slotMouseLeave;
	gui::CSlot			m_slotPaint;

	gui::CRadarEffect	m_CoolTimeEffect;

	CRegularSlotGui			m_slot;

	RwReal				m_fElapsed;
	RwReal				m_fRemainTime;

	RwUInt8				m_byBuffIndex;
	RwBool				m_bRightMouseDown;
	RwBool				m_bMouseEnter;
};