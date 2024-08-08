/******************************************************************************
* File			: OptionGMTTab.h
* Author		: Hae sung, Cho
* Copyright		: (주)NTL
* Date			: 2009. 8. 17
* Abstract		: 
*****************************************************************************
* Desc			: Tab of GameManiaTime(GMT)
*****************************************************************************/

#ifndef _OPTION_GMT_TAB_H_
#define _OPTION_GMT_TAB_H_

#pragma once

#include "OptionBase.h"

struct sGameManiaTimeGuiInfo
{
	RwUInt32 dwTblIdx;
	RwUInt8 byEffectDurrationTime;
	RwUInt8 byEffectType;
	RwUInt8 byEffectValue;
	RwUInt8 byEffectTimeNum;
	RwUInt8 abySetTime[GMT_MAX_TIME_SLOT_SIZE];
	RwUInt32 dwSetTime;
};

/**
* \ingroup client
*/
class COptionGMT : public COptionBase
{
public:
	enum eGMT_PRESENT_LIST
	{
		GPL_1 = 0,
		GPL_2, GPL_3, GPL_4, GPL_5,
		GPL_NUMS
	};

	enum eGMT_PRESENT_TIME
	{
		GPT_1 = 0,
		GPT_2, GPT_3,
		GPT_NUMS
	};

	enum eGMT_EFFECT_COUNT
	{
		GEC_1 = 0,
		GEC_2, GEC_3, GEC_4,

		GEC_NUMS
	};

	// UI 모드
	enum eGMT_MODE
	{
		GMT_UI_NORMAL = 0,		// 변경되지 않은 상태
		GMT_UI_CHANGE,			// 변경될 상태

		GMT_UI_NUMS
	};

	enum eEFFECT_TYPE
	{
		ET_EXP = 0,		///> 경험치
		ET_ZENNY,		///< 제니

		ET_NUMS
	};

	enum eGMT_TBLIDX
	{
		GMT_TBLIDX_1 = 0,
		GMT_TBLIDX_2, GMT_TBLIDX_3, GMT_TBLIDX_4,

		GMT_TBLIDX_NUMS
	};

public:
	COptionGMT();
	virtual ~COptionGMT();

	virtual RwBool		Create(COptionWindowGui* pOptionWindow);
	virtual void		Destroy();

	virtual void		Show();
	virtual void		Hide();

	virtual void		OnInit();
	virtual void		OnReset();
	virtual void		OnOk();
	virtual void		OnCancel();
	virtual void		OnHandleEvents(RWS::CMsg &pMsg);

public:
	void				InitGMTList();
	void				UpdateUIMode( eGMT_MODE eUIMode );
	void				UpdateResetCount();
	void				SetDownGMT( eGMT_PRESENT_LIST eList );
	void				OnToggledGMTButton( gui::CComponent* pComponent, bool bToggled );
	void				OnClickedSetting( gui::CComponent* pComponent );

protected:
	gui::CDialog*		m_pThis;			///< 현재 Dialog

	gui::CStaticBox*	m_pStbGameManiaTime;
	gui::CStaticBox*	m_pStbGameManiaTimeProfile;
	gui::CStaticBox*	m_pStbExGet;
	
	// 보여지는 GMT 항목
	gui::CButton*			m_apBtnGPL[GPL_NUMS];
	gui::CSlot				m_slotToggledGPL[GPL_NUMS];
	RwUInt32				m_adwGmtPresentList[GPL_NUMS];
	sGameManiaTimeGuiInfo	m_asGmtGuiInfo[GPL_NUMS];
	sGameManiaTimeGuiInfo	m_asGmtNextInfo[GPL_NUMS];
	eGMT_PRESENT_LIST		m_ePresentIndex;

	// 남은 재설정 횟수
	gui::CStaticBox*	m_pStbRemainCount;
	RwUInt32			m_uiResetCount;
	
	// 재설정
	gui::CButton*		m_pBtnSetting;
	gui::CSlot			m_slotClickedBtnSetting;

	// 설정되어 있는 시간 출력
	gui::CStaticBox*	m_pStbManiaCurrentTime;
	gui::CStaticBox*	m_pStbManiaChangeTime;

	gui::CStaticBox*	m_apStbCurrentTime[GPT_NUMS];
	gui::CStaticBox*	m_apStbTo[GPT_NUMS];
	gui::CStaticBox*	m_apStbChangeTime[GPT_NUMS];

	eGMT_MODE			m_eUIMode;
};


#endif