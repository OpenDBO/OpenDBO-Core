/******************************************************************************
* File			: ScouterGui.h
* Author		: Hong SungBock
* Copyright		: (占쏙옙)NTL
* Date			: 2008. 11. 17
* Abstract		: 
*****************************************************************************
* Desc			: GM 占쏙옙占쏙옙 占쏙옙占쏙옙 占쌨댐옙 占쏙옙占쏙옙占쏙옙占싱쏙옙
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

	gui::CButton*		m_pCharShowButton;				///< GM 캐占쏙옙占쏙옙 占쏙옙占싱깍옙 占쏙옙튼
	gui::CButton*		m_pCharHideButton;				///< GM 캐릭터 숨기기 버튼
	gui::CButton*		m_pTenkaichiButton;				///< 천占쏙옙占쏙옙占쏙옙 占쏙옙占쏙옙회 占쌨댐옙 占쏙옙占쏙옙 占쏙옙튼
	gui::CButton*		m_pPortalButton;				///< 占싣뱄옙타 占싱듸옙 占쏙옙튼
	gui::CButton*		m_pFreeCameraButton;			///< 占쏙옙占쏙옙 카占쌨띰옙 占쏙옙튼
	gui::CButton*		m_pNormalCameraButton;			///< 3占쏙옙칭 카占쌨띰옙 占쏙옙튼

	gui::CButton*		m_pTenkaichi_CameraLocButton;	///< 천占쏙옙占쏙옙占쏙옙 占쏙옙占쏙옙회 카占쌨띰옙 占쏙옙치 占쏙옙占쏙옙 占쏙옙튼
	gui::CButton*		m_pTenkaichi_CameraMoveButton;	///< 천占쏙옙占쏙옙占쏙옙 占쏙옙占쏙옙회 카占쌨띰옙 占싱듸옙 占쏙옙튼
	gui::CButton*		m_pTenkaichi_MenuShowButton;	///< 천占쏙옙占쏙옙占쏙옙 占쏙옙占쏙옙회 占쏙옙占쏙옙占쏙옙 占쌨댐옙 Show 占쏙옙튼
	gui::CButton*		m_pTenkaichi_MenuHideButton;	///< 천占쏙옙占쏙옙占쏙옙 占쏙옙占쏙옙회 占쏙옙占쏙옙占쏙옙 占쌨댐옙 Hide 占쏙옙튼
	gui::CButton*		m_pTenkaichi_PlayerButton;		///< 천占쏙옙占쏙옙占쏙옙 占쏙옙占쏙옙회 占쏙옙占쏙옙 占쏙옙占쏙옙 占쏙옙튼
	gui::CButton*		m_pTenkaichi_TournamentButton;	///< 천占쏙옙占쏙옙占쏙옙 占쏙옙占쏙옙회 占쏙옙占쏙옙표 占쏙옙占쏙옙 占쏙옙튼
	gui::CButton*		m_pReturnButton;				///< 占쏙옙占싣곤옙占쏙옙 占쏙옙튼

	CGMPopupGui*		m_pPopupGui;					///< GM Popup 占쌨댐옙

	eMenuGroup			m_eMenuGroup;
	RwBool				m_bAvatarShow;
	RwInt8				m_byPopupSlot;					///< 占싯억옙 占쌨댐옙占쏙옙 占쏙옙占쏙옙 占쏙옙占쏙옙占쏙옙 占싸듸옙占쏙옙

	MAP_POPUP			m_mapPlayerInfo;
};
