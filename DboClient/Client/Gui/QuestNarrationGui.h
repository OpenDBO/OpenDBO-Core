////////////////////////////////////////////////////////////////////////////////
// File: QuestNarrationGui.h
// Desc: QuestNarrationGui
//		 Narration ����
//
// 2006.12.5 Peessi@hitel.net   
//
// To Be Desired:
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __QUEST_NARRATION_GUI_H__
#define __QUEST_NARRATION_GUI_H__

#include "NtlSLEvent.h"
#include "gui_precomp.h"
#include "NtlPLCinematicObject.h"

class CQuestGui;

class CQuestNarrationGui
{
public:
	enum STATE { STATE_NONE, STATE_BEGIN, STATE_ING, STATE_IDLE, STATE_END, STATE_WAIT_RES };
	enum OWNERTYPE { NONE = 0, NPCMOB = 0x01, PC = 0x02, PC_SELECT = 0x04, ALL = 0x07 };

	//! Constructor & Destructor
	CQuestNarrationGui(VOID);
	~CQuestNarrationGui(VOID);

	//! Operation
	RwBool	Create( CQuestGui* pQuestGui );
	VOID	Destroy(VOID);	

	INT		StateUpdate( uintptr_t uiElapsedTime );

	//! Event
	VOID	HandleEvents( RWS::CMsg& msg );		
private:
	//! Implement
	VOID	Init(VOID);
	VOID	InitQuestData(VOID);
	VOID	SetUIData( SNtlEventQuestNarrationDialog_Req* pData );
	VOID	SetUIData( SNtlEventQuestNarrationUserSelectDialog_Req* pData );
	VOID	SendResultToTS( bool bResult, RwUInt8 bySelectID = 0xff );

	VOID	SetState( STATE eState, OWNERTYPE eOwnerType );
		
	INT		BeginStateDirection(VOID);
	INT		IngStateDirection(VOID);
	INT		EndStateDirection(VOID);

	RwBool	CheckTSState( RwInt32 eTSState, OWNERTYPE eOwnerType );

	//! Callback
	VOID	OnMouseUp( const CKey& key );
	VOID	OnHtmlPageDone( RwInt32 nPage );
	VOID	OnHtmlPCPageDone( RwInt32 nPage );
	VOID	OnPCCSelect1( gui::CComponent* pComponent );
	VOID	OnPCCSelect2( gui::CComponent* pComponent );
	VOID	OnPCCSelect3( gui::CComponent* pComponent );
	VOID	OnPCCSelect4( gui::CComponent* pComponent );
	VOID	OnFocusSelect1( gui::CComponent* pComponent );
	VOID	OnFocusSelect2( gui::CComponent* pComponent );
	VOID	OnFocusSelect3( gui::CComponent* pComponent );
	VOID	OnFocusSelect4( gui::CComponent* pComponent );

	gui::CSlot			m_slotPCFrameUp;
	gui::CSlot			m_slotMouseUp;
	gui::CSlot			m_slotHtmlPageDone;
	gui::CSlot			m_slotHtmlPCPageDone;
	gui::CSlot			m_slotPCSelect1;
	gui::CSlot			m_slotPCSelect2;
	gui::CSlot			m_slotPCSelect3;
	gui::CSlot			m_slotPCSelect4;
	gui::CSlot			m_slotFocusSelect1;
	gui::CSlot			m_slotFocusSelect2;
	gui::CSlot			m_slotFocusSelect3;
	gui::CSlot			m_slotFocusSelect4;

	//! Variable
	CNtlTCUnit*			m_pTCUnit;
	sTS_KEY				m_TSKey;
	
	STATE				m_eUIState;					// UI��ü�� State
	RwInt32				m_eTSState;					// TS�� ������ State
	RwInt32				m_eTextOutType;				// Text ��� ���
	RwUInt32			m_uiMaxLifeTime;			// �� �ð��� ������ ������ true�� �Ѱ��ش�.
	RwUInt32			m_uiCurrentTime;			//
	RwUInt32			m_flagOwnerType;			// OWNERTYPE Flag, ������ � Ÿ���� ����Ǿ���. Begin ~ End
	OWNERTYPE			m_eCurrentType;				// ������ Ÿ�Ը�.

	RwUInt8				m_byPCSelectID[CINEMATIC_MAX_PC_SELECT];
	
	bool				m_bResult;					// ���� �����ΰ� �������� �ΰ�.
	RwUInt8				m_bySelectID;				// User Select Value;
	//RwBool				m_bShowPCMessageOnPCSelect;	// PCSelect�� PCMessage�� ǥ������ ����. Begin���� ���ö� Ing���� �Ǻ��ϱ� ����.

	CQuestGui*			m_pQuestGui;

	CNtlPLCinematicObject*	m_pCinematicObject;
};

#endif//__QUEST_NARRATION_GUI_H__