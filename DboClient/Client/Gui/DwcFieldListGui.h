/******************************************************************************
* File			: DwcFieldListGui.h
* Abstract		: Dragon World Challenge entry list of the dwc npc
*****************************************************************************
* Desc			: port of the tw CDwcFieldListGui (DWC_FieldList.frm, dialog 0x97)
*				  one button per dwc the server lists, the meter shows how busy it is
*****************************************************************************/

#ifndef __DWC_FIELDLIST_GUI_H__
#define __DWC_FIELDLIST_GUI_H__

#pragma once

// core
#include "ceventhandler.h"

// presentation
#include "NtlPLGui.h"

// shared
#include "NtlDWC.h"

#define dDWC_FIELDLIST_METER_COUNT		5

class CDwcFieldListGui : public CNtlPLGui, public RWS::CEventHandler
{
	struct sFIELD_ITEM
	{
		TBLIDX			dwcTblidx;
		gui::CButton*	pButton;
		gui::CPanel*	apHalfMeter[dDWC_FIELDLIST_METER_COUNT];	///< tw: srfFieldStateLevel1
		gui::CPanel*	apFullMeter[dDWC_FIELDLIST_METER_COUNT];	///< tw: srfFieldStateLevel2
		gui::CSlot		slotClicked;
		RwUInt32		uiLastClickTime;
	};

public:
	CDwcFieldListGui(const RwChar* pName);
	~CDwcFieldListGui(VOID);

	RwBool		Create(VOID);
	VOID		Destroy(VOID);

	RwInt32		SwitchDialog(bool bOpen);
	VOID		HandleEvents(RWS::CMsg& msg);

private:
	VOID		SetFieldList(RwUInt8 byWorldCount, const sDWC_WORLD_COUNT_INFO* pInfo);
	VOID		ClearFieldList(VOID);
	VOID		SetMeter(sFIELD_ITEM* pItem, RwUInt16 wUseCount);
	VOID		PositionAlign(VOID);

	VOID		OnClickItem(gui::CComponent* pComponent);
	VOID		OnClickExit(gui::CComponent* pComponent);
	VOID		OnScrollChanged(RwInt32 iValue);
	VOID		OnMouseWheel(RwInt32 nFlag, RwInt16 sDelta, CPos& pos);

	gui::CSlot			m_slotClickExit;
	gui::CSlot			m_slotScrollChanged;
	gui::CSlot			m_slotScrollMoved;
	gui::CSlot			m_slotMouseWheel;

	gui::CStaticBox*	m_pstbTitle;
	gui::CPanel*		m_ppnlFieldList;
	gui::CScrollBar*	m_pscbScroll;
	gui::CButton*		m_pbtnExit;

	std::vector<sFIELD_ITEM*>	m_vecItem;

	RwUInt32			m_hNPCSerial;
	RwUInt8				m_byRemainCount;
	RwInt32				m_iListTop;			///< y of pnlFieldList without the scroll
};

#endif//__DWC_FIELDLIST_GUI_H__
