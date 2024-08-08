/******************************************************************************
* File			: TMQBoardGui.h
* Author		: Hong SungBock
* Copyright		: (주)NTL
* Date			: 2007. 7. 3
* Update		: 2007. 10.23
* Abstract		: 
*****************************************************************************
* Desc			: 진행 예정인 타임머신 퀘스트의 리스트를 보여준다
*****************************************************************************/

#pragma once

#include <list>

// core
#include "ceventhandler.h"

// share
#include "NtlTimeQuest.h"

// presentation
#include "NtlPLGui.h"

// simulation
#include "NtlSLDef.h"

// dbo
#include "SurfaceGui.h"
#include "SlotGui.h"

struct sTIMEQUEST_TBLDAT;

class CTMQBoardGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
	struct sTMQ_Senario
	{
		RwUInt32			uiTblIdx;
	};

	enum eDefinedValue
	{
		DV_SENARIO_NUM		= 4
	};

	CTMQBoardGui(const RwChar* pName);
	virtual ~CTMQBoardGui();

	RwBool		Create();
	VOID		Update(RwReal fElapsed);
	VOID		Destroy();

	VOID		Clear();

	SERIAL_HANDLE	GetNPCHandle();

	RwInt32		SwitchDialog(bool bOpen);			///< DialogManager에서의 Open/Close

protected:
	CTMQBoardGui() {}
	virtual VOID	HandleEvents( RWS::CMsg &msg );

	VOID			UpdateList();
	VOID			SetRoomState(RwUInt8 byState);
	VOID			CheckInfoWindow();

	VOID			ClickedCloseButton(gui::CComponent* pComponent);
	VOID			ClickedPrivateReservation(gui::CComponent* pComponent);
	VOID			ClickedPartyReservation(gui::CComponent* pComponent);
	VOID			ClickedCancelReservation(gui::CComponent* pComponent);

	VOID			OnMove(RwInt32 iOldX, RwInt32 iOldY);
	VOID			OnMouseMove(RwInt32 nFlags, RwInt32 nX, RwInt32 nY);
	VOID			OnMouseLeave(gui::CComponent* pComponent);
	VOID			OnPaint();

protected:		
	gui::CSlot			m_slotPaint;
	gui::CSlot			m_slotMove;
	gui::CSlot			m_slotMouseMove;
	gui::CSlot			m_slotMouseLeave;
	gui::CSlot			m_slotCloseButton;	
	gui::CSlot			m_slotPrivateReservationButton;
	gui::CSlot			m_slotPartyReservationButton;
	gui::CSlot			m_slotCancelReservationButton;

	gui::CStaticBox*	m_pLimitedLevel;			///< '제한레벨'
	gui::CStaticBox*	m_pLimitedLevelText;		///< 제한레벨 표시	
	gui::CStaticBox*	m_pTitle;					///< TMQ 제목

	gui::CStaticBox*	m_pDifficult;				///< '난이도'
	gui::CStaticBox*	m_pStartTime;				///< '출발시간'	
	gui::CStaticBox*	m_pLimitedTime;				///< '제한시간'	
	gui::CStaticBox*	m_pDifficultText;			///< 난이도 표시
	gui::CStaticBox*	m_pStartTimeText;			///< 출발시간 표시
	gui::CStaticBox*	m_pLimitedTimeText;			///< 제한시간 표시	

	gui::CStaticBox*	m_pReservationPeople;		///< 현재 예약한 인원

	gui::CStaticBox*	m_pNextTitle;				///< '다음 타임머신 퀘스트'
	gui::CStaticBox*	m_pNextTitle_Senario_1;		///< 다음 퀘스트 1
	gui::CStaticBox*	m_pNextTitle_Senario_2;		///< 다음 퀘스트 2
	gui::CStaticBox*	m_pNextTitle_Senario_3;		///< 다음 퀘스트 3

	gui::CButton*		m_pExitButton;
	gui::CButton*		m_pPrivateReservationButton;///< 개인 예약 버튼
	gui::CButton*		m_pPartyReservationButton;	///< 파티 예약 버튼
	gui::CButton*		m_pCancelReservationButton;	///< 예약취소 버튼

	CRegularSlotGui			m_NeedItem;

	SERIAL_HANDLE		m_hNPCSerial;
	sTMQ_Senario		m_Senario[DV_SENARIO_NUM];
	RwUInt8				m_byRoomState;

	sTIMEQUEST_TBLDAT*	m_pTIMEQUEST_TBLDAT;		///< 현재 보고 있는 게시판의 TMQ 테이블
};