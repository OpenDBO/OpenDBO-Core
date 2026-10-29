/******************************************************************************
* File			: DwcScenarioMainGui.h
* Abstract		: Dragon World Challenge objectives of the dwc the avatar plays
*****************************************************************************
* Desc			: port of the tw CDwcScenarioInfoGui (DWC_ScenarioMain.frm, dialog 0x95)
*				  opened by the scenario button of the dwc status
*****************************************************************************/

#ifndef __DWC_SCENARIOMAIN_GUI_H__
#define __DWC_SCENARIOMAIN_GUI_H__

#pragma once

// core
#include "ceventhandler.h"

// presentation
#include "NtlPLGui.h"

class CDwcScenarioMainGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
	CDwcScenarioMainGui(const RwChar* pName);
	~CDwcScenarioMainGui(VOID);

	RwBool		Create(VOID);
	VOID		Destroy(VOID);

	RwInt32		SwitchDialog(bool bOpen);
	VOID		HandleEvents(RWS::CMsg& msg);

private:
	VOID		Refresh(VOID);
	VOID		SetHtml(gui::CHtmlBox* pHtmlBox, const std::wstring& wstrHtml);

	VOID		OnClickClose(gui::CComponent* pComponent);

	gui::CSlot			m_slotClickClose;

	gui::CStaticBox*	m_pstbTitle;
	gui::CStaticBox*	m_pstbTargetTitle;
	gui::CStaticBox*	m_pstbEssentialTitle;
	gui::CStaticBox*	m_pstbChooseTitle;
	gui::CHtmlBox*		m_phbxTargetText;
	gui::CHtmlBox*		m_phbxEssentialText;
	gui::CHtmlBox*		m_phbxChooseText;
	gui::CButton*		m_pbtnClose;
};

#endif//__DWC_SCENARIOMAIN_GUI_H__
