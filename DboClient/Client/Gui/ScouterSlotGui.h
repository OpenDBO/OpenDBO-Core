/******************************************************************************
* File			: ScouterGui.h
* Author		: Hong SungBock
* Copyright		: (주)NTL
* Date			: 2006. 8. 28
* Abstract		: 
*****************************************************************************
* Desc			: 화면 하단에 스카우터가 들어갈 슬롯
*				  단순히 스카우터 바디의 장착과 해제 스카우터 가방열기 정도의
*			      기능만이 있다
*				  스카우터의 슬롯은 두 군데에서 표현된다
*				  하나는 스테이터스 윈도우이고 하나는 화면하단의 5개의 가방
*				  슬롯옆이다. 한쪽의 이벤트를 다른 쪽에도 알려주어야 하기에
*				  g_EventScouter 이벤트를 통해서 스카우터 바디가 바뀐 여부를
*				  알 수 있게 하였다.
*****************************************************************************/

#pragma once

// core
#include "ceventhandler.h"

// presetation
#include "NtlPLGui.h"

// simulation
#include "NtlSLDef.h"

// dbo
#include "SlotGui.h"


class CScouterSlotGui : public CNtlPLGui, public RWS::CEventHandler
{
public:	
	CScouterSlotGui(const RwChar* pName);
	virtual ~CScouterSlotGui();

	RwBool		Create();
	VOID		Destroy();

	VOID		SetScouterBody(SERIAL_HANDLE hHandle);	///< 스카우터의 바디 정보가 변경되면 처리
	VOID		UnSetScouterBody();						///< 스카우터의 바디 정보를 지운다.

	RwInt32		SwitchDialog(bool bOpen);			///< DialogManager에서의 Open/Close

protected:
	CScouterSlotGui() {}
	virtual VOID	HandleEvents( RWS::CMsg &msg );	

	VOID			ShowIconDestination();				
	///< 가방안에서 스카우터 파츠를 집었다면 스카우터 슬롯에 셀렉트 이펙트를 보여준다.

	VOID			ShowDisableSlot(RwBool bShow);
	VOID			CheckInfoWindow();

	VOID			OnPaint();

	VOID			OnMouseDown(const CKey& key);
	VOID			OnMouseUp(const CKey& key);
	VOID			OnMove(RwInt32 iOldX, RwInt32 iOldY);
	VOID			OnMouseMove(RwInt32 nFlags, RwInt32 nX, RwInt32 nY);
	VOID			OnMouseLeave(gui::CComponent* pComponent);

protected:	
	gui::CSlot			m_slotMouseDown;
	gui::CSlot			m_slotMouseUp;
	gui::CSlot			m_slotMove;
	gui::CSlot			m_slotMouseMove;
	gui::CSlot			m_slotMouseLeave;
	gui::CSlot			m_slotPaint;

	RwInt32				m_iMouseDownSlot;			///< 마우스로 눌린 슬롯의 인덱스

	CRegularSlotGui		m_slot;						///< 슬롯

	CSurfaceGui			m_FocusEffect;				///< 슬롯 포커스 이펙트
	CSurfaceGui			m_SelectEffect;				///< 슬롯 셀렉트 이펙트

	RwBool				m_bFocus;					///< 포커스를 얻었다.
	RwBool				m_bIconMoveClickEvent;		///< 가방에서 스카우터 바디를 집었을 때 이벤트
	RwBool				m_bSelected;				///< 선택이 되었다.
	RwBool				m_bClickEffect;				///< 슬롯 클릭 모드
};