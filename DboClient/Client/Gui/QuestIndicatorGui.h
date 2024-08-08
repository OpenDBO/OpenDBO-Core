////////////////////////////////////////////////////////////////////////////////
// File: QuestIndicatorGui.h
// Desc: QuestIndicator
//
// 2006.11.22 Peessi@hitel.net   
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __QUEST_INDICATOR_GUI_H__
#define __QUEST_INDICATOR_GUI_H__

#include "NtlPLGui.h"
#include "ceventhandler.h"
#include "SurfaceGui.h"
#include "NtlSLEvent.h"
#include "QuestGuiDef.h"

#define QUEST_INDICATOR_QUESTTITLE_HEIGHT	23
#define QUEST_INDICATOR_MAX_HEIGHT			287
#define QUEST_INDICATOR_QUESTTITLE_MARGIN	5
#define QUEST_MAX_INDICATOR_COUNT ( QUEST_INDICATOR_MAX_HEIGHT / ( QUEST_INDICATOR_QUESTTITLE_HEIGHT + QUEST_INDICATOR_QUESTTITLE_MARGIN ) )

class CQuestGui;

struct stINDICATOR_ITEM
{
	gui::CFrame*		m_pFrame;
	gui::CButton*		m_pTitle;
	gui::CPanel*		m_pState;
	gui::CStaticBox*	m_pGoalInfo[QUEST_MAX_GOAL_COUNT];
	gui::CStaticBox*	m_pGoalResult[QUEST_MAX_GOAL_COUNT];	
	gui::CStaticBox*	m_pTime;

	RwInt32				m_nFrameHeight;	
	sTS_KEY				m_TSKey;			

	RwUInt32			m_uiQuestTitle;
	RwUInt32			m_uiQuestGoal;

	gui::CSlot			m_slotClickTitle;
	gui::CSlot			m_slotMouseEnter;
	gui::CSlot			m_slotMouseLeave;

	VOID	OnClickTitle( gui::CComponent* pComponent );
	VOID	OnMouseEnterTitle( gui::CComponent* pComponent );
	VOID	OnMouseLeaveTitle( gui::CComponent* pComponent );
};

class CQuestIndicatorGui : public CNtlPLGui
{	
public:
	//! Constructor & Destructor
	CQuestIndicatorGui(VOID);
	CQuestIndicatorGui( const RwChar* pName );
	~CQuestIndicatorGui(VOID);

	//! Operation
	RwBool	Create( CQuestGui* pQuestGui );
	VOID	Destroy(VOID);

	RwInt32 SwitchDialog( bool bOpen );

	RwBool	IsIndicatedQuest( RwUInt32 nID );
	
	//! Event
	VOID	HandleEvents( RWS::CMsg& msg );

	typedef std::list<SNtlEventShowIndicator_Nfy*> LIST_DATA;

private:
	//! Implementation
	VOID	Init(VOID);
	VOID	Refresh(VOID);						
	VOID	DrawData( RwInt32 nIdx, SNtlEventShowIndicator_Nfy* pIndicatorData );
	VOID	UpdateData( SNtlEventUpdateQuestProgressInfo_Nfy* pData );
	VOID	UpdateState( sTS_KEY& sTSKey, RwUInt32 uiQuestState );
	VOID	UpdateResultItem( const WCHAR* pInfo, RwInt32 nCurVal, RwInt32 nTargetVal, RwInt32 i, RwInt32 j );
	VOID	UpdateResultItem( const WCHAR* pInfo, RwBool bComplete, RwInt32 i, RwInt32 j );
	VOID	CalculatePosition(VOID);	
	RwBool	IsAvaliableHeight( SNtlEventShowIndicator_Nfy* pData );
	RwBool	IsAvaliableHeight(VOID);

	RwInt32 FindData( RwUInt32 nID, SNtlEventShowIndicator_Nfy** ppData );	// index ∏Æ≈œ.
	VOID	CreateData( SNtlEventShowIndicator_Nfy* pData );
	VOID	DeleteData( RwUInt32 nID );
	VOID	DeleteAllData(VOID);		

	//! Callback
	VOID	OnClickMinimize( gui::CComponent* pComponent );

	//! Variables
	gui::CSlot			m_slotClickMinimize;

	gui::CButton*		m_pbtnMinimize;
	gui::CStaticBox*	m_pstbTitle;

	stINDICATOR_ITEM	m_IndicatorItem[QUEST_MAX_INDICATOR_COUNT];
	RwInt32				m_nTotalItemHeight;
	
	LIST_DATA			m_listData;	

	CQuestGui*			m_pQuestGui;	// for Refrerence
};

#endif