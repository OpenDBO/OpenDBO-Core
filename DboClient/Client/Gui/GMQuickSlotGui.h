/******************************************************************************
* File			: ScouterGui.h
* Author		: Hong SungBock
* Copyright		: (주)NTL
* Date			: 2008. 11. 17
* Abstract		: 
*****************************************************************************
* Desc			: GM 게임 단축 메뉴 인터페이스
*****************************************************************************/

#pragma once

class CGMPopupGui;
class CPlayerInfoGui;

// core
#include "ceventhandler.h"

// presetation
#include "NtlPLGui.h"

// simulation
#include "NtlSLDef.h"


class CGMQuickSlotGui : public CNtlPLGui, public RWS::CEventHandler
{
public:	
	enum eMenuGroup
	{
		MENU_GROUP_MAIN,
		MENU_GROUP_TENKAICHI,
	};

	typedef std::map<SERIAL_HANDLE, CPlayerInfoGui*>					MAP_POPUP;


	CGMQuickSlotGui(const RwChar* pName);
	virtual ~CGMQuickSlotGui();

	RwBool		Create();
	VOID		Destroy();

	CGMPopupGui*	GetPopupGui() { return m_pPopupGui; }
	RwInt32		SwitchDialog(bool bOpen);

protected:
	CGMQuickSlotGui() {}
	virtual VOID	HandleEvents( RWS::CMsg &msg );

	VOID			SetMenuGroup(eMenuGroup eGroup);
	VOID			ShowMainButtons(bool bShow);
	VOID			ShowTenkaichiButtons(bool bShow);

	VOID			AddPlayerInfo(SERIAL_HANDLE hPlayer);
	VOID			RemovePlayerInfo(SERIAL_HANDLE hPlayer);

	VOID			ShowPopup(RwBool bShow, RwUInt8 bySlot);

	VOID			OnClicked_CharShowHide( gui::CComponent* pComponent );
	VOID			OnClicked_Tenkaichi( gui::CComponent* pComponent );
	VOID			OnClicked_Portal( gui::CComponent* pComponent );
	VOID			OnClicked_FreeCamera( gui::CComponent* pComponent );

	VOID			OnClicked_Tenkaichi_CameraLoc( gui::CComponent* pComponent );
	VOID			OnClicked_Tenkaichi_CameraMove( gui::CComponent* pComponent );
	VOID			OnClicked_Tenkaichi_MenuShowHide( gui::CComponent* pComponent );
	VOID			OnClicked_Tenkaichi_Player( gui::CComponent* pComponent );
	VOID			OnClicked_Tenkaichi_Tournament( gui::CComponent* pComponent );
	VOID			OnClicked_Return( gui::CComponent* pComponent );

	VOID			OnMove(RwInt32 iOldX, RwInt32 iOldY);

	RwInt32 ActionMapQuickSlotDown( uintptr_t uiKey );
	RwInt32 ActionMapQuickSlotUp( uintptr_t uiKey );

protected:
	gui::CSlot			m_slotCharShowButton;
	gui::CSlot			m_slotCharHideButton;
	gui::CSlot			m_slotTenkaichiButton;
	gui::CSlot			m_slotPortalButton;
	gui::CSlot			m_slotFreeCameraButton;
	gui::CSlot			m_slotNormalCameraButton;

	gui::CSlot			m_slotTenkaichi_CameraLoc;
	gui::CSlot			m_slotTenkaichi_CameraMove;
	gui::CSlot			m_slotTenkaichi_MenuShow;
	gui::CSlot			m_slotTenkaichi_MenuHide;
	gui::CSlot			m_slotTenkaichi_Player;
	gui::CSlot			m_slotTenkaichi_Tournament;
	gui::CSlot			m_slotReturn;
	gui::CSlot			m_slotMove;

	gui::CButton*		m_pCharShowButton;				///< GM ĳ���� ���̱� ��ư
	gui::CButton*		m_pCharHideButton;				///< GM ĳ���� ����� ��ư
	gui::CButton*		m_pTenkaichiButton;				///< õ������ ����ȸ �޴� ���� ��ư
	gui::CButton*		m_pPortalButton;				///< �ƹ�Ÿ �̵� ��ư
	gui::CButton*		m_pFreeCameraButton;			///< ���� ī�޶� ��ư
	gui::CButton*		m_pNormalCameraButton;			///< 3��Ī ī�޶� ��ư

	gui::CButton*		m_pTenkaichi_CameraLocButton;	///< õ������ ����ȸ ī�޶� ��ġ ���� ��ư
	gui::CButton*		m_pTenkaichi_CameraMoveButton;	///< õ������ ����ȸ ī�޶� �̵� ��ư
	gui::CButton*		m_pTenkaichi_MenuShowButton;	///< õ������ ����ȸ ������ �޴� Show ��ư
	gui::CButton*		m_pTenkaichi_MenuHideButton;	///< õ������ ����ȸ ������ �޴� Hide ��ư
	gui::CButton*		m_pTenkaichi_PlayerButton;		///< õ������ ����ȸ ���� ���� ��ư
	gui::CButton*		m_pTenkaichi_TournamentButton;	///< õ������ ����ȸ ����ǥ ���� ��ư
	gui::CButton*		m_pReturnButton;				///< ���ư��� ��ư

	CGMPopupGui*		m_pPopupGui;					///< GM Popup �޴�

	eMenuGroup			m_eMenuGroup;
	RwBool				m_bAvatarShow;
	RwInt8				m_byPopupSlot;					///< �˾� �޴��� ���� ������ �ε���

	MAP_POPUP			m_mapPlayerInfo;
};