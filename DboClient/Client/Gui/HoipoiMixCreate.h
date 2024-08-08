/******************************************************************************
* File			: HoipoiCreate.h
* Author		: Haesung Cho
* Copyright		: (¡÷)NTL
* Date			: 2009. 1. 8
* Abstract		: 
*****************************************************************************
* Desc			: »£¿Ã∆˜¿Ã πÕΩ∫ ƒ∏Ω∂ æ∆¿Ã≈€ ¡¶¡∂ TAB
*****************************************************************************/

#ifndef _HOIPOIMIX_CREATE_H_
#define _HOIPOIMIX_CREATE_H_

#pragma once

#include "ceventhandler.h"
#include "NtlPLGui.h"
#include "SurfaceGui.h"
#include "NtlSharedType.h"
#include "SlotGui.h"
#include "NtlResultCode.h"

#define dHOIPOIMIX_CREATE_MATERIAL_MAX_NUM	5
#define dHOIPOIMIX_CREATE_ITEM_MAX_NUM		4
#define dHOIPOIMIX_ARROW_MAX_NUM			3
#define dHOIPOIMIX_PROGRESS_BAR_WIDTH		253
#define dHOIPOIMIX_PROGRESS_BAR_WIDTH_F		253.f

class CHoipoiMixCapsuleGui;

class CHoipoiMixCreate
{
public:
	CHoipoiMixCreate();
	virtual ~CHoipoiMixCreate();

	RwBool			Create( CHoipoiMixCapsuleGui* pParentGui );
	VOID			Destroy();

	VOID			Show( bool bShow );
	RwBool			IsShow();

	VOID			InitFlash();
	VOID			InitGui();

	VOID			Update( RwReal fElapsed );
	VOID			UpdateMaterialCheck();

	VOID			SetRecipe( TBLIDX idxRecipeTbl );

	VOID			HandleEvents( RWS::CMsg& msg );

protected:
	VOID			OnPaint();
	VOID			OnMove(int nOldX, int nOldY);
	
	VOID			StartCreate( RwInt32 nAmount );
	VOID			Step( RwBool bStep );
	VOID			StopCreate();

	VOID			PlayResultMovie( RwUInt8 byResultType );

	VOID			SetCreating( RwBool bCreate );

	VOID			SetOutputLog( const WCHAR* pLog, COLORREF color );

	VOID			CreateArrow();
	VOID			ClearArrow();

	VOID			OnClickedBtnMax(gui::CComponent* pComponent);
	VOID			OnClickedBtnCalc(gui::CComponent* pComponent);
	VOID			OnClickedBtnCreate(gui::CComponent* pComponent);
	VOID			OnClickedBtnStop(gui::CComponent* pComponent);

	VOID			OnKeyDownFromConcent(const CKey& key);
	VOID			OnMouseMove( RwInt32 nKey, RwInt32 nXPos, RwInt32 nYPos );
	VOID			OnCaptureKeyDown( gui::CComponent* pComponent, CInputDevice* pDevice, const CKey& key );
	VOID			OnMovieEnd( gui::CComponent* pComponent );

protected:
	gui::CDialog*		m_pThis;
	gui::CStaticBox*	m_pStbTitle;
	gui::CStaticBox*	m_pStbSettingCategory;
	gui::CStaticBox*	m_pStbZenny;
	gui::CButton*		m_pBtnMax;
	gui::CButton*		m_pBtnCalc;
	gui::CStaticBox*	m_pStbPreViewCategory;
	gui::CButton*		m_pBtnCreate;
	gui::CButton*		m_pBtnStop;
	gui::CStaticBox*	m_pStbControlCategory;
	gui::CProgressBar*	m_pPgbConcent;
	gui::CStaticBox*	m_pStbConcentText;
	gui::CPanel*		m_pPnlNotify;
	gui::COutputBox*	m_pOpbLog;
	gui::CParticleBox*	m_pPbBox;
	gui::CFlash*		m_pFlaResult[ITEM_MIX_RESULT_TYPE_NUMS];
	
	gui::CSlot			m_slotClickedBtnMax;
	gui::CSlot			m_slotClickedBtnCalc;
	gui::CSlot			m_slotClickedBtnCreate;
	gui::CSlot			m_slotClickedBtnStop;
	gui::CSlot			m_slotCaptureKeyDown;
	gui::CSlot			m_slotMovieEnd[ITEM_MIX_RESULT_TYPE_NUMS];

	TBLIDX				m_idxEnableRecipe;

	gui::CPanel*		m_pPnlMaterialSlot[dHOIPOIMIX_CREATE_MATERIAL_MAX_NUM];
	gui::CPanel*		m_pPnlPreViewSlot[dHOIPOIMIX_CREATE_ITEM_MAX_NUM];

	CRegularSlotGui			m_SlotRecipe;

	CRegularSlotGui			m_SlotMaterial[dHOIPOIMIX_CREATE_MATERIAL_MAX_NUM];
	CRegularSlotGui			m_SlotPreview[dHOIPOIMIX_CREATE_ITEM_MAX_NUM];
	gui::CStaticBox*	m_apStbStackMaterial[dHOIPOIMIX_CREATE_MATERIAL_MAX_NUM];
	gui::CStaticBox*	m_apStbStackPreview[dHOIPOIMIX_CREATE_ITEM_MAX_NUM];

	gui::CSlot			m_slotPaint;
	gui::CSlot			m_slotMove;
	gui::CSlot			m_slotMouseMove;

	RwBool				m_bCreating;
	RwInt32				m_nLeftCreate;
	RwBool				m_bStep;

	RwReal				m_fElapsedTime;
	RwReal				m_fNotifyTime;

	RwInt32				m_anArrowPosX[dHOIPOIMIX_ARROW_MAX_NUM];
	RwBool				m_abArrowHit[dHOIPOIMIX_ARROW_MAX_NUM];
	gui::CPanel*		m_pPnlEpArrow[dHOIPOIMIX_ARROW_MAX_NUM];
	gui::CPanel*		m_pPnlHitLine[dHOIPOIMIX_ARROW_MAX_NUM];
	RwInt32				m_nArrowCount;
	RwInt32				m_nHitCount;
	RwUInt32			m_uiOpbLogCount;

	CHoipoiMixCapsuleGui*	m_pHoipoiMix;
};

#endif