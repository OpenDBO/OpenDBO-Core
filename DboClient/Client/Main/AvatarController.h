/*****************************************************************************
 *
 * File			: AvatarController.h
 * Author		: HongHoDong
 * Copyright	: (占쏙옙)NTL
 * Date			: 2005. 11. 13	
 * Abstract		: NTl CAvatarController
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __NTL_AVATAR_CONTROLLER__
#define __NTL_AVATAR_CONTROLLER__

#include "NtlCamera.h"
#include "NtlGameCamera.h"
#include "InputHandler.h"
#include "ceventhandler.h"

#include "NtlSLEvent.h"
#include "NtlSobActor.h"
#include "NtlSLEventHandler.h"
#include "NtlDebug.h"
#include "NtlDeviceRestoreHandler.h"
#include "NtlPLPicking.h"
#include "DboDef.h"


enum ECameraRotateFlag
{
	CAMERA_ROTATE_FLAG_NONE,	
	CAMERA_ROTATE_FLAG_ADJUST,	
	CAMERA_ROTATE_FLAG_ACCORD,	
};

/**
* \ingroup Client
* Keyboard 占쏙옙 Mouse占쏙옙 占쌉뤄옙占쏙옙占쏙옙 Camera占쏙옙 占쏙옙占쏙옙 占싹몌옙, Camera占쏙옙 占싱듸옙 占쏙옙 회占쏙옙占쏙옙
* 에 대해서 Avatar에게 Event를 보내는 기능을 한다. 그리고 Keyboard와 Mouse가
* 占쏙옙占시울옙 占쏙옙占쏙옙占쏙옙 占쏙옙占쏙옙 처占쏙옙 占쏙옙占쏙옙 占싹몌옙 ActionMap占쏙옙 占쏙옙占쏙옙 Event占쏙옙 占싱곤옙占쏙옙占쏙옙 占쏙옙占쏙옙占쏙옙
* 
*/
class CAvatarController : public RWS::CEventHandler
{
public:

	CAvatarController();
	~CAvatarController();

	RwBool	Create();
	void	Destroy();

	void Update(RwReal fElapsed);
	void HandleEvents(RWS::CMsg &pMsg);
	
	/**
	* mouse and keyboard hander 占쌉쇽옙.	
	*/

	int	MouseWheelHandler(uintptr_t uiMouseData);

	int	MouseDownHandler(uintptr_t uiMouseData);
	int	MouseUpHandler(uintptr_t uiMouseData);

	int	KeyboardDownHandler(uintptr_t uiKeyData);
	int KeyboardUpHandler(uintptr_t uiKeyData);
	
	int RestoreHandler(void);		///< Device Restore 처占쏙옙 占쌉쇽옙
		
	/**
	*	input action map 처占쏙옙 占쌉쇽옙.
	*/ 
	int ActionMapKeyboardMove(uintptr_t uiMoveFlags);
	int ActionMapKeyboardDashMove(uintptr_t uiMoveFlags);
	int ActionAvatarSelect(void);
	int ActionJump(uintptr_t uiMoveFlags);
	int ActionCharging(uintptr_t uiCharging);
	int	ActionBlocking(uintptr_t uiBlocking);
	int ActionSitAndStand(uintptr_t bKeyDown);
	int ActionLooting(void);
	int ActionAutoTarget(void);
	int ActionAutoAttack(void);
	int ActionAutoFollow(void);
	int ActionPartySelect(uintptr_t uiIdx);
    int ActionAutoRun();
    int ActionScouterUse(void);
    int ActionPartyShareTargetMarking(uintptr_t uiSlot);
    int ActionPartyShareTargetSelect(uintptr_t uiSlot);
    int ActionPartyAutoSelect();

private:

	void TeleportReset(void);

	/**
	* picking pl world entity
	*/
	void PickPLWorld(SWorldPickInfo& sPickInfo);

	/**
	* picking pl character entity
	*/
	void PickPLObject(SWorldPickInfo& sPickInfo, int someEnum, RwInt32 nXPos, RwInt32 nYPos);
	void PickPLObject(SWorldPickInfo& sPickInfo);

	/**
	* avatar�� ī�޶� ������ �����Ͽ� �ٷ� ������ �ٲ� �ʿ䰡 �ִµ�, �⺻ move flag�� �����Ѵ�.
	*/
	RwUInt8 ConvertStraightMoveFlags(RwUInt8 uiMoveFlags);

	/**
	* avatar占쏙옙 카占쌨띰옙 占쏙옙占쏙옙占쏙옙 占쏙옙占쏙옙占싹울옙(占쏙옙占쏙옙 move flag占쏙옙 占쏙옙占쏙옙占싹울옙) 占쌕뤄옙 회占쏙옙占싼댐옙.
	*/
	RwReal CalcAvatarStraightRotate(RwUInt32 uiMoveFlags);

	/**
	* 카占쌨띰옙 회占쏙옙 占쏙옙占쏙옙占쏙옙 占쏙옙占싼댐옙.
	*/
	void CalcCameraRotateSensitivity(int iPosX, int iPosY, int iMouseDeltaX, int iMouseDeltaY);

	/**
	* ���콺 ��ư�� �ƹ��͵� ������ �ʰ�,  keyboard�� ĳ���Ͱ� front and back ���� ������ ���
	* 캐릭터 뒷편으로 카메라를 회전시킬 delta angle을 계산한다.
	*/
	void CalcCameraRotateToCharBack(void);

	/**
	* ���콺 ��ư�� �ƹ��͵� ������ �ʰ�,  keyboard�� ĳ���Ͱ� front and back ���� ������ ���
	* 캐占쏙옙占쏙옙 占쏙옙占쏙옙占쏙옙占쏙옙 카占쌨띰옙 회占쏙옙占쏙옙占쏙옙占쌔댐옙.
	*/
	void UpdateCameraRotateToCharBack(RwReal fElapsed);

	/**
	* auto run일 경우.
	*/
	void UpdateAutoRun(RwReal fElapsed);

	/**
	* move mark effect create 占쌉쇽옙.
	*/
	void CreateMoveMark(const RwV3d *pPos);

	/**
	* move mark effect delete 占쌉쇽옙.
	*/
	void DeleteMoveMark(void);

	/**
	* move mark effect update 占쌉쇽옙.
	*/
	void UpdateMoveMark(RwReal fElapsed);

	/*
	*/
	void UpdateMouse(RwReal fElapsed);
	
	/**
	* move mark effect update 占쌉쇽옙.
	*/
	void UpdateChangeHeading(RwReal fElapsed);

	/*
		* change x angle while flying
	*/
	void UpdateFlightAngle(float fElapsed);

	/**
	* picking focus update 占쌉쇽옙.
	*/
	void UpdatePickFocus(RwReal fElapsed);

	/**
	* mouse left button up 占싹띰옙 처占쏙옙 占쌉쇽옙.
	*/
	void MouseUpLBtnHandler(const SMouseData *pData);

	/**
	* mouse right button up 占싹띰옙 처占쏙옙 占쌉쇽옙.
	*/
	void MouseUpRBtnHandler(const SMouseData *pData);

	/**
	* mouse move left button down 占싹띰옙 처占쏙옙 占쌉쇽옙.
	*/
	void MouseMoveLBtnHandler(int iPosX, int iPosY);

	/**
	* mouse move right button down 占싹띰옙 처占쏙옙 占쌉쇽옙.
	*/
	void MouseMoveRBtnHandler(int iPosX, int iPosY);


	/*
	/ put mouse cursor to the middle
	*/
	void			ResetMousePosition();

private:

	bool			m_bHideMouse;

	// picking
	SWorldPickInfo	m_sPickInfo;
	sWorldPickInfo_for_Cursor	m_sPickInfo_for_Cursor;

	RwReal			m_fPickTime;

	// Effect
	RwReal			m_fMoveMarkTime;
	CNtlPLEntity	*m_pMoveMark;
	RwUInt8	    	m_uiCurMoveFlags;	 // not used when we use autorun
	RwBool			m_bAutoRun;
	RwBool			m_bHTBRPStock;

	// Looting
	RwReal			m_fKeyLootingTime;

	ECameraRotateFlag m_eRotFlag;

	bool			m_bMouseAutoRun;

	RwBool			m_bLButtonMoveFlag;
	RwInt32			m_iOldLButtonMouseX;
	RwInt32			m_iOldLButtonMouseY;

	bool			m_bLButtonDownFlag;
	bool			m_bRButtonDownFlag;

	RwInt32			m_iDeltaLButtonMouseX;
	RwInt32			m_iDeltaLButtonMouseY;
	RwInt32			m_iDeltaRButtonMouseX;
	RwInt32			m_iDeltaRButtonMouseY;

	RwBool			m_bRButtonMoveFlag;
	RwInt32			m_iOldRButtonMouseX;
	RwInt32			m_iOldRButtonMouseY;

	RwBool			m_bChangeHeadingFlag;
	RwBool			m_bAutoRunKeyboardChangeHeading;
	RwBool			m_bSkipMouseWheel;
	RwReal			m_fChangeHeadingUpdateTime;
	RwReal			m_fChangeHeadingDeltaTime;
	RwReal			m_fChangeHeadingDeltaAngleY;
	RwReal			m_fChangeHeadingDeltaAngleX;

	// changle angleX with keyboard
	bool			m_bChangeAngleX;
	bool			m_bChangeAngleXUp;

	SERIAL_HANDLE   m_hFocusSerialId;

	INPUT_HANDLE	m_handleMouseWheel;
	INPUT_HANDLE	m_handleMouseDown;
	INPUT_HANDLE	m_handleMouseUp;
	INPUT_HANDLE	m_handleKeyDown;
	INPUT_HANDLE	m_handleKeyUp;

	// device restore
	DEVICE_RESTORE_HANDLE m_handleRestoreHandler;

private:

	void SetAutoRun(RwBool bAutoRun);
};

#endif
