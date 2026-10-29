/******************************************************************************
* File			: DwcHeroInfoGui.h
* Abstract		: Dragon World Challenge card details
*****************************************************************************
* Desc			: port of the tw CDwcHeroInfoGui (DWC_HeroInfo.frm, dialog 0x93)
*				  opened by a card of the challenge: the large card, the mission,
*				  the clear condition and the rewards of the card grade
*****************************************************************************/

#ifndef __DWC_HEROINFO_GUI_H__
#define __DWC_HEROINFO_GUI_H__

#pragma once

// core
#include "ceventhandler.h"

// presentation
#include "NtlPLGui.h"

// shared
#include "NtlDWC.h"

struct sDWCMISSION_TBLDAT;
struct sDWCMISSION_REWARD_ENTITY;

class CDwcHeroInfoGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
	CDwcHeroInfoGui(const RwChar* pName);
	~CDwcHeroInfoGui(VOID);

	RwBool		Create(VOID);
	VOID		Destroy(VOID);

	RwInt32		SwitchDialog(bool bOpen);
	VOID		HandleEvents(RWS::CMsg& msg);

	// bWon: the avatar holds the card (or a higher grade of it)
	VOID		SetCard(TBLIDX dwcTblidx, sDWCMISSION_TBLDAT* pMissionTbldat, RwUInt8 byGrade, RwUInt8 byHeldGrade, RwBool bWon, RwUInt32 uiNumber);

private:
	VOID		SetFrameHeight(gui::CPanel* pPanel, RwInt32 iHeight);
	VOID		SetMultiLine(gui::CMDStaticBox* pBox, const std::wstring& wstrText, COLORREF color);
	VOID		AddWrappedLines(gui::CMDStaticBox* pBox, const std::wstring& wstrText, COLORREF color, RwInt32& iLine);
	VOID		SetRewardList(gui::CMDStaticBox* pBox, sDWCMISSION_REWARD_ENTITY* pEntity);
	VOID		ExpandCondition(RwBool bExpand);
	VOID		ExpandReward(RwBool bExpand);
	VOID		RelocateSections(VOID);

	VOID		OnClickClose(gui::CComponent* pComponent);
	VOID		OnClickConditionExpand(gui::CComponent* pComponent);
	VOID		OnClickConditionReduce(gui::CComponent* pComponent);
	VOID		OnClickRewardExpand(gui::CComponent* pComponent);
	VOID		OnClickRewardReduce(gui::CComponent* pComponent);
	VOID		OnScrollChanged(RwInt32 iValue);
	VOID		OnMouseWheel(RwInt32 nFlag, RwInt16 sDelta, CPos& pos);

	gui::CSlot			m_slotClickClose;
	gui::CSlot			m_slotClickConditionExpand;
	gui::CSlot			m_slotClickConditionReduce;
	gui::CSlot			m_slotClickRewardExpand;
	gui::CSlot			m_slotClickRewardReduce;
	gui::CSlot			m_slotScrollChanged;
	gui::CSlot			m_slotScrollMoved;
	gui::CSlot			m_slotMouseWheel;

	gui::CStaticBox*	m_pstbTitle;
	gui::CStaticBox*	m_pstbScenarioTitle;
	gui::CStaticBox*	m_pstbHeroName;
	gui::CStaticBox*	m_pstbPlayerCountText;
	gui::CStaticBox*	m_pstbPlayerCountValue;
	gui::CStaticBox*	m_pstbTargetNameText;
	gui::CStaticBox*	m_pstbTargetNameValue;
	gui::CStaticBox*	m_pstbPlayLevelText;
	gui::CStaticBox*	m_pstbPlayLevelValue;
	gui::CStaticBox*	m_pstbGradeText;
	gui::CStaticBox*	m_pstbBasicRewardText;
	gui::CStaticBox*	m_pstbRepeatRewardText;
	gui::CPanel*		m_ppnlScrollView;
	gui::CPanel*		m_ppnlCard;
	gui::CPanel*		m_ppnlLevel;
	gui::CPanel*		m_apnlGrade[MAX_DWC_REWARD_GRADE_COUNT];
	gui::CPanel*		m_ppnlCondition;
	gui::CPanel*		m_ppnlReward;
	gui::CMDStaticBox*	m_pmdbCondition;
	gui::CMDStaticBox*	m_pmdbBasicReward;
	gui::CMDStaticBox*	m_pmdbRepeatReward;
	gui::CButton*		m_pbtnClose;
	gui::CButton*		m_pbtnConditionExpand;
	gui::CButton*		m_pbtnConditionReduce;
	gui::CButton*		m_pbtnRewardExpand;
	gui::CButton*		m_pbtnRewardReduce;
	gui::CScrollBar*	m_pscbScroll;

	gui::CTexture*		m_pCardTexture;
	RwInt32				m_iViewTop;
	RwBool				m_bConditionExpand;
	RwBool				m_bRewardExpand;
};

#endif//__DWC_HEROINFO_GUI_H__
