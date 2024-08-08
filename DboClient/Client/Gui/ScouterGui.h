/******************************************************************************
* File			: ScouterGui.h
* Author		: Hong SungBock
* Copyright		: (주)NTL
* Date			: 2006. 8. 14
* Abstract		: 스카우터 파츠의 기능을 표시한다
*****************************************************************************
* Desc			: Item_Option_Table의 Scouter_Info 항목 순서되로 표시된다
*
*				스카우터 파츠의 기능을 표시하는 방법은 복잡하다
*				새로운 기능의 파츠가 추가될 때마다 다음의 작업 순서를 꼭 지키자
*
*				1. Item_Option_Table의 Scouter_Info의 값을 참고하여 NtlItem.h
*				   에 eSCOUTER_INFO에 타입을 추가한다
*
*				2. DisplayStringDef.h에 표시될 글귀를 추가한다
*
*				3. 파츠에 기본 수치가 변하게 하는 것이 있다면
*				   CScouterGui::GetDefaultValue()에 eSCOUTER_INFO에 추가된
*				   타입을 추가한다
*
*				알림) 일부 파츠(eSCOUTER_INFO의 SCOUTER_INFO_BEGIN_ONLY_ALLPY_MAX에서
*				   SCOUTER_INFO_BEGIN_SPECIAL_FUNCTION사이는 가장 좋은 아이템 하나만을 표시한다
*****************************************************************************/

#pragma once

// core
#include "ceventhandler.h"

// share
#include "NtlItem.h"

// presetation
#include "NtlPLGui.h"

// simulation
#include "NtlSLDef.h"

// dbo
#include "SlotGui.h"



class CScouterGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
	// 같은 파츠의 내용을 걸러내고, 같은 능력을 올려주는 것들은 수치를 하나로 표시하기 위해
	// first  : sITEM_OPTION_TBLDAT의 byScouterInfo
	// second : sITEM_OPTION_TBLDAT의 nValue[0] or sITEM_TBLDAT의 byScouter_Parts_Value
	typedef std::map<RwUInt32, RwUInt32>				MAP_PARTS_EXPLAIN;
	typedef std::map<RwUInt32, RwUInt32>::iterator		MAP_PARTS_EXPLAIN_ITER;
	
	CScouterGui(const RwChar* pName);
	virtual ~CScouterGui();

	RwBool		Create();
	VOID		Destroy();

	RwInt32		SwitchDialog(bool bOpen);			///< DialogManager에서의 Open/Close

	VOID		AddScouterParts();

protected:
	CScouterGui() {}
	virtual VOID	HandleEvents( RWS::CMsg &msg );

	VOID			SetScouterInfo(RwUInt32 uiSerial);	///< 스카우터 정보 표시
	VOID			UnSetScouterInfo();					///< 스카우터 정보 초기화

	VOID			UpdatePartsInfo();					///< 스카우터 파츠 정보 업데이트

	VOID			ReadyPartsExplain(RwUInt32 uiPartsValue, RwUInt32 uiItemOptionTableIndex);	///< 파츠 정보를 가공한다
	VOID			DisplayPartsInfo();											///< 파츠 정보를 보여준다

	RwInt32			GetDefaultValue(RwUInt32 uiType);							///< 파츠 적용 수치별 디폴트 값을 얻어온다
	RwUInt32		ConvertPartsInfotoStringTableIndex(RwUInt32 iType);			///< 스트링 테이블 인덱스 찾기

	VOID			OnCaptureWheelMove( RwInt32 nFlag, RwInt16 sDelta, CPos& pos );
	VOID			OnCaptureMouseDown(const CKey& key);
	VOID			OnClicked_ExitButton(gui::CComponent* pComponent);

protected:
	gui::CSlot			m_slotCaptureWheelMove;
	gui::CSlot			m_slotCaptureMouseDown;
	gui::CSlot			m_slotExitButton;

	gui::CButton*		m_pExitButton;
	gui::CListBox*		m_pPartsListBox;			///< 파츠 옵션 설명 리스트

	SERIAL_HANDLE		m_hScouterHandle;
	MAP_PARTS_EXPLAIN	m_mapPartsExplain;			///< 파츠 설명을 위한 임시 저장소
};