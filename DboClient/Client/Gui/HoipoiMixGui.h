/******************************************************************************
* File			: HoipoiMix.h
* Author		: Haesung Cho
* Copyright		: (¡÷)NTL
* Date			: 2009. 1. 8
* Abstract		: 
*****************************************************************************
* Desc			: »£¿Ã∆˜¿Ã πÕΩ∫ ƒ∏Ω∂ æ∆¿Ã≈€ GUI
*****************************************************************************/

#ifndef _HOIPOIMIX_H_
#define _HOIPOIMIX_H_

#pragma once

#include "ceventhandler.h"
#include "NtlPLGui.h"
#include "NtlSLDef.h"

class CRecipeDlgItem;
class CHoipoiMixRecipe;
class CHoipoiMixCreate;

class CHoipoiMixCapsuleGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
	enum eTAB
	{
		INVALID_TAB = -1,
		RECIPE_TAB,			/// ∑πΩ√«« ∏ÆΩ∫∆Æ ≈«
		CREATE_TAB,			/// ¡¶¡∂ ≈«

		NUM_TAB
	};

	CHoipoiMixCapsuleGui(const RwChar* pName);
	virtual ~CHoipoiMixCapsuleGui();

	RwBool		Create();
	VOID		Destroy();

	RwInt32		SwitchDialog( bool bOpen );
	VOID		HandleEvents( RWS::CMsg& msg );

	SERIAL_HANDLE GetObject();

	VOID		Update( RwReal fElapsed );

	VOID		SelectTab( eTAB eTabIndex );
	VOID		SelectRecipe( CRecipeDlgItem* pSelectItem );

protected:
	VOID		OnClickedClose( gui::CComponent* pComponent );
	VOID		OnSelectTab( RwInt32 nSelectIdx, RwInt32 nPreviousIndex );

	
protected:
	gui::CTabButton*	m_pTabButton;
	eTAB				m_eCurTab;

	gui::CStaticBox*	m_pStbTitle;
	gui::CButton*		m_pBtnClose;

	gui::CSlot			m_slotSelectTab;
	gui::CSlot			m_slotClickedClose;

	CHoipoiMixRecipe*	m_pRecipeTab;
	CHoipoiMixCreate*	m_pCreateTab;

	SERIAL_HANDLE		m_hObject;
};

#endif