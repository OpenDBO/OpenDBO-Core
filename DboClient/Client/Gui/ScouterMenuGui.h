/******************************************************************************
* File			: ScouterMenuGui.h
* Author		: Hong SungBock
* Copyright		: (주)NTL
* Date			: 2006. 10. 30
* Update		: 2008. 3. 4
* Abstract		: 
*****************************************************************************
* Desc			: ScouterMenuGui
*****************************************************************************/

#pragma once

// core
#include "ceventhandler.h"

// Shared
#include "NtlItem.h"

// presentation
#include "NtlPLGui.h"

// simulation
#include "InputHandler.h"

// dbo
#include "SlotGui.h"
		
enum eScouterMenuType
{
	SCOUTERMENU_MEASURE_POWER,					///< 전투력 측정
	SCOUTERMENU_IDENTIFICATION_ITEM,			///< 아이템 감정
	SCOUTERMENU_VIEW_PC_EQUIP_ITEM,				///< 다른 사람 장비 보기
	SCOUTERMENU_FIND_HIDDEN_OBJECT,				///< 숨겨진 오브젝트 찾기
	SCOUTERMENU_QUEST_SEARCH,					///< 주변 퀘스트 찾기

	NUM_SCOUTERMENU,

	INVALID_SCOUTERMENU
};

class CScouterMenuGui;

class CScouterMenuItem
{
public:
	CScouterMenuItem(CScouterMenuGui* pScounterMenuGui)
	:m_pFlashButton(NULL)
	,m_byMenuType(INVALID_SCOUTERMENU)
	,m_pScounterMenuGui(pScounterMenuGui)
	{}

	virtual ~CScouterMenuItem() {}

	RwBool			Create(gui::CComponent* pParent, RwUInt8 byMenuType, CRectangle& rt);
	VOID			Destroy();

	VOID			Update(RwReal fElapsed);

	VOID			Show(bool bOpen);

	RwUInt8			GetMenuType();

	VOID			SetPosition(CRectangle& rt);
	CRectangle		GetPosition();

	RwBool			IsShow();

protected:
	VOID			OnFSCallback(const char* pcParam, const char* pcParam2);

protected:
	gui::CSlot			m_slotFSCallback;

	gui::CFlash*		m_pFlashButton;
	RwUInt8				m_byMenuType;
	CScouterMenuGui*	m_pScounterMenuGui;
};

class CScouterMenuGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
	typedef std::list<CScouterMenuItem*>				LIST_MENUITEM;
	typedef std::list<CScouterMenuItem*>::iterator		LIST_MENUITEM_ITER;

	CScouterMenuGui(const RwChar* pName);
	virtual ~CScouterMenuGui();

	RwBool		Create();
	VOID		Destroy();

	VOID		Update(RwReal fElapsed);
	VOID		Clear();

	VOID		OnClick_MenuItem(RwUInt8 byMenuType);

	RwInt32		SwitchDialog(bool bOpen);

protected:
	CScouterMenuGui() {}
	virtual VOID	HandleEvents( RWS::CMsg &msg );

	VOID			Menu_Update();						///< 메뉴 정보를 업데이트 한다
	RwBool			Menu_Add(RwUInt8 byMenuType);		///< 기존에 메뉴가 없다면 해당 타입의 메뉴를 추가한다
	VOID			Menu_Del(RwUInt8 byMenuType);		///< 메뉴를 지운다
	VOID			Menu_JugdeVisiblePreNextButton();

	VOID			ResizeDilaog();
	RwUInt8			PartsTypeToMenuIndex(RwUInt8 byType);				///< 아이템 테이블의 파츠타입을 메뉴 인덱스로 컨버젼

	VOID			ArrangeMenuButtons();

	RwInt32			KeyboardUpHandler(unsigned int uiKeyData);

	VOID			OnClicked_CloseButton(const char* pcParam, const char* pcParam2);
	VOID			OnClicked_PrePageButton(const char* pcParam, const char* pcParam2);
	VOID			OnClicked_NextButton(const char* pcParam, const char* pcParam2);

	VOID			OnMove(RwInt32 iOldX, RwInt32 iOldY);

protected:		
	gui::CSlot			m_slotCloseButton;
	gui::CSlot			m_slotPrePageButton;
	gui::CSlot			m_slotNextPageButton;
	gui::CSlot			m_slotMove;

	INPUT_HANDLE		m_handleKeyUp;

	gui::CFlash*		m_pFlashExitButton;
	gui::CFlash*		m_pFlashPrePageButton;			///< 이전 페이지로 넘김 버튼
	gui::CFlash*		m_pFlashNextPageButton;			///< 다음 페이지로 넘김 버튼

	RwUInt8				m_byVisibleMenuIndex;

	LIST_MENUITEM		m_listMenuItem;
};