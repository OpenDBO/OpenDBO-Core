/******************************************************************************
* File			: ScouterBodyGui.h
* Author		: Hong SungBock
* Copyright		: (주)NTL
* Date			: 2006. 8. 18
* Abstract		: 
*****************************************************************************
* Desc			: ScouterBodyGui
*****************************************************************************/

#pragma once

// core
#include "ceventhandler.h"

// presentation
#include "NtlPLGui.h"

#include "NtlSLDef.h"

// dbo
#include "SlotGui.h"
#include "Windowby3.h"


struct sITEM_TBLDAT;

class CScouterBodyGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
	CScouterBodyGui(const RwChar* pName);
	virtual ~CScouterBodyGui();

	RwBool		Create();
	VOID		Destroy();

	RwInt32		SwitchDialog(bool bOpen);		///< DialogManager에서의 Open/Close
	
	RwInt32		GetCurPower();						///< 스카우터의 현재 사용용량

protected:
	CScouterBodyGui() {}
	virtual VOID	HandleEvents( RWS::CMsg &msg );

	RwBool			SetScouterBody(SERIAL_HANDLE hSerial);	///< 스카우터 바디에 맞는 정보와 UI를 설정한다
	VOID			UnSetScouterBody();					///< 스카우터 바디를 해제한다.	

	RwInt32			GetPartsCount();
	VOID			UpdateParts();						///< 스카우터 슬롯에 파츠 정보를 업데이트 한다.

	VOID			SetPower(RwInt32 iPower, RwInt32 iMaxPower);

	RwUInt8			PtinSlot(RwInt32 iX, RwInt32 iY);	///< 마우스가 눌린 슬롯의 찾는다.
	
	VOID			ClickReleasePartsButton(gui::CComponent* pComponent);	///< 스카우터 파츠 모두 해제 버튼
	
	VOID			FocusEffect( RwBool bPush, RwUInt8 bySlot = INVALID_BYTE );
	VOID			SelectEffect( RwBool bPush, RwUInt8 bySlot = INVALID_BYTE );

	VOID			LocateComponent();
	VOID			ShowDisableSlot(RwBool bShow, RwUInt8 bySlot);
	VOID			CheckInfoWindow();

	VOID			OnPaint();

	VOID			OnClicked_OnMenuButton(gui::CComponent* pComponent);
	VOID			OnClicked_OffMenuButton(gui::CComponent* pComponent);
	VOID			OnClicked_RemoveAllPartsButton(gui::CComponent* pComponent);

	VOID			OnMouseDown(const CKey& key);
	VOID			OnMouseUp(const CKey& key);
	VOID			OnMove(RwInt32 iOldX, RwInt32 iOldY);
	VOID			OnResize(RwInt32 iOldW, RwInt32 iOldH);
	VOID			OnMouseMove(RwInt32 nFlags, RwInt32 nX, RwInt32 nY);
	VOID			OnMouseLeave(gui::CComponent* pComponent);
	VOID			OnCaptureMouseDown(const CKey& key);

protected:	
	gui::CSlot			m_slotOnMenuButton;
	gui::CSlot			m_slotOffMenuButton;
	gui::CSlot			m_slotRemoveAllPartsButton;
	gui::CSlot			m_slotMouseDown;
	gui::CSlot			m_slotMouseUp;
	gui::CSlot			m_slotMove;
	gui::CSlot			m_slotResize;
	gui::CSlot			m_slotMouseMove;
	gui::CSlot			m_slotMouseLeave;
	gui::CSlot			m_slotPaint;
	gui::CSlot			m_slotCaptureMouseDown;

	RwUInt8				m_byMouseDownSlot;			///< 마우스로 눌린 슬롯의 인덱스
	RwUInt8				m_bySelectedSlot;			///< 셀렉트 이펙트 슬롯
	RwUInt8				m_byClickEffectedSlot;		///< 현재 클릭 이펙트가 생긴 슬롯
	
	gui::CButton*		m_pOnMenuButton;			///< 스카우터 메뉴 키기
	gui::CButton*		m_pOffMenuButton;			///< 스카우터 메뉴 끄기
	gui::CButton*		m_pRemoveAllParts;			///< 모든 파츠 제거 버튼

	gui::CStaticBox*	m_pScouterName;				///< 스카우터 이름
	gui::CStaticBox*	m_pPowerStatic;				///< 스카우터 전략량
	gui::CProgressBar*	m_pPowerGauge;				///< 스카우터 전력 게이지

	CWindowby3			m_BackPanel;				///< 스카우터 모양
	CSurfaceGui			m_FocusEffect;				///< 슬롯 포커스 이펙트
	CSurfaceGui			m_SelectEffect;				///< 슬롯 셀렉트 이펙트
	CSurfaceGui			m_srfSlot[4];				///< 슬롯 서페이스

	RwUInt8				m_byInfoWindowIndex;
	RwBool				m_bFocus;					///< 포커스를 얻었다.

	CRegularSlotGui			m_ScouterSlot;				///< 정보를 알아볼 스카우터의 아이콘 슬롯
	RwInt32				m_iScouterSlotCount;		///< 현재 스카우터의 슬롯 개수
	CRegularSlotGui			m_Slot[NTL_MAX_SCOUTER_ITEM_SLOT];	///< 스카우터의 슬롯	

	RwInt32				m_iCurPower;
};