/******************************************************************************
* File			: DwcPrologGui.h
* Abstract		: Dragon World Challenge prologue of the dwc the avatar plays
*****************************************************************************
* Desc			: port of the tw CDwcPrologGui (DWC_Prolog.frm, dialog 0x96)
*				  opened by the prologue button of the dwc status, the text of
*				  the dwc prologue in pages
*****************************************************************************/

#ifndef __DWC_PROLOG_GUI_H__
#define __DWC_PROLOG_GUI_H__

#pragma once

// core
#include "ceventhandler.h"

// presentation
#include "NtlPLGui.h"

class CDwcPrologGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
	CDwcPrologGui(const RwChar* pName);
	~CDwcPrologGui(VOID);

	RwBool		Create(VOID);
	VOID		Destroy(VOID);

	VOID		Update(RwReal fElapsed);

	RwInt32		SwitchDialog(bool bOpen);
	VOID		HandleEvents(RWS::CMsg& msg);

private:
	VOID		SetPageInfo(VOID);

	VOID		OnClickClose(gui::CComponent* pComponent);
	VOID		OnClickPrev(gui::CComponent* pComponent);
	VOID		OnClickNext(gui::CComponent* pComponent);

	gui::CSlot			m_slotClickClose;
	gui::CSlot			m_slotClickPrev;
	gui::CSlot			m_slotClickNext;

	gui::CStaticBox*	m_pstbTitle;
	gui::CStaticBox*	m_pstbPrologTitle;
	gui::CStaticBox*	m_pstbPageInfo;
	gui::CHtmlBox*		m_phbxMessage;
	gui::CButton*		m_pbtnClose;
	gui::CButton*		m_pbtnPrev;
	gui::CButton*		m_pbtnNext;
};

#endif//__DWC_PROLOG_GUI_H__
