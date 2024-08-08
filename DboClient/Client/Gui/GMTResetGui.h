/******************************************************************************
* File			: GMTResetGui.h
* Author		: Haesung, Cho
* Copyright		: (주)NTL
* Date			: 2009. 8. 20
* Abstract		: 
* Update		: 
*****************************************************************************
* Desc			: Game Mania Time 재설정 UI
*****************************************************************************/

#ifndef __GMT_RESET_GUI_H__
#define __GMT_RESET_GUI_H__

#pragma once

// core
#include "ceventhandler.h"

// presentation
#include "NtlPLGui.h"

#include "NtlCharacter.h"

class CGMTSetTime
{
public:
	CGMTSetTime();
	virtual ~CGMTSetTime();

	RwBool	Create( gui::CDialog* pParentGui, RwUInt32 idxGmt );
	RwBool	CreateHardCode(RwUInt8 byDurrationTime);
	void	Destroy();

	void	OnToggledBtnCheck( gui::CComponent* pComponent, bool bToggled );

	void	Init();

	RwBool	IsDone();
	RwUInt8 GetCountCheckNum();
	RwUInt8 GetEnableCheckNum();
	RwUInt32 GetTblIdx();

	void	GetCheckTimeIndex( std::vector<RwUInt8>& vecTimeIdx );

	void	Show( RwBool bShow );

protected:
	gui::CDialog*		m_pThis;

	gui::CStaticBox*	m_apStbTime2[12];
	gui::CStaticBox*	m_apStbTime3[8];
	gui::CStaticBox*	m_apStbTime4[6];
	gui::CStaticBox*	m_apStbTime6[4];

	gui::CStaticBox**	m_ppStbTime;		///< 이 포인터는 위의 m_apStbTime2 ~ 6 의 배열을 가리킨다.

	gui::CButton*		m_apBtnCheck2[12];
	gui::CButton*		m_apBtnCheck3[8];
	gui::CButton*		m_apBtnCheck4[6];
	gui::CButton*		m_apBtnCheck6[4];

	gui::CButton**		m_ppBtnCheck;				///< 이 포인터는 위의 m_apBtnCheck2 ~ 6 의 배열을 가리킨다.
	gui::CSlot			m_slotToggledBtnCheck[12];	///< Slot은 최대 갯수를 가지고 있는다.

	gui::CDialog*		m_pParentGui;

	RwUInt32			m_tblidx;
	RwUInt8				m_byTimeNum;				///< 시간대의 갯수
	RwUInt8				m_byEnableCheckNum;			///< 체크할 수 있는 갯수
	
};

class CGMTResetGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
	CGMTResetGui(const RwChar* pName);
	virtual ~CGMTResetGui();

	RwBool		Create();
	void		Destroy();

	void		Update(RwReal fElapsed);

	RwInt32		SwitchDialog(bool bOpen);
	void		HandleEvents(RWS::CMsg& msg);

	void		SelectGMT( RwUInt8 byIndex );

	void		SetPosPopup( RwInt32 nX, RwInt32 nY );
	void		OnSelectedGMT(RwInt32 nSelectIndex);
	void		OnListToggledGMT(BOOL bToggled, gui::CComponent* pComponent);

	void		OnClickedBtnOk(gui::CComponent* pComponent);
	void		OnClickedBtnCancel(gui::CComponent* pComponent);

protected:
	gui::CStaticBox*	m_pStbGMTResetTitle;
	gui::CStaticBox*	m_pStbExtGet;
	gui::CComboBox*		m_pCbbGMTList;
	gui::CSlot			m_slotListToggled;
	gui::CSlot			m_slotListSelected;
	gui::CButton*		m_pBtnClose;
	gui::CSlot			m_slotClickedBtnClose;
	gui::CStaticBox*	m_pStbRemainCount;
	gui::CStaticBox*	m_pStbSetTime;
	gui::CStaticBox*	m_pStbTimeNum;

	gui::CDialog*		m_pDlgSetTime;

	gui::CButton*		m_pBtnOK;
	gui::CSlot			m_slotClickedBtnOK;
	gui::CButton*		m_pBtnCancel;
	gui::CSlot			m_slotClickedBtnCancel;

	CGMTSetTime*		m_apGMTSetTime[GMT_MAX_EFFECT_LIST_SIZE];
};

#endif