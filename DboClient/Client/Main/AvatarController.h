/*****************************************************************************
 *
 * File			: AvatarController.h
 * Author		: HongHoDong
 * Copyright	: (주)NTL
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
* Keyboard 및 Mouse의 입력으로 Camera를 갱신 하며, Camera의 이동 및 회전에
* 에 대해서 Avatar에게 Event를 보내는 기능을 한다. 그리고 Keyboard와 Mouse가
* 동시에 눌렸을 때의 처리 또한 하며 ActionMap에 대한 Event도 이곳에서 보낸다
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
	* mouse and keyboard hander 함수.	
	*/

	int	MouseWheelHandler(unsigned int uiMouseData);
	int	MouseMoveHandler(unsigned int uiMouseData);

	int	MouseDownHandler(unsigned int uiMouseData);
	int	MouseUpHandler(unsigned int uiMouseData);

	int	KeyboardDownHandler(unsigned int uiKeyData);
	int KeyboardUpHandler(unsigned int uiKeyData);
	
	int RestoreHandler(void);		///< Device Restore 처리 함수
		
	/**
	*	input action map 처리 함수.
	*/ 
	int ActionMapKeyboardMove(unsigned int uiMoveFlags);
	int ActionMapKeyboardDashMove(unsigned int uiMoveFlags);
	int ActionAvatarSelect(void);
	int ActionJump(unsigned int uiMoveFlags);
	int ActionCharging(unsigned int uiCharging);
	int	ActionBlocking(unsigned int uiBlocking);
	int ActionSitAndStand(void);
	int ActionLooting(void);
	int ActionAutoTarget(void);
	int ActionAutoAttack(void);
	int ActionAutoFollow(void);
	int ActionPartySelect(unsigned int uiIdx);
    int ActionAutoRun();
    int ActionScouterUse(void);
    int ActionPartyShareTargetMarking(unsigned int uiSlot);
    int ActionPartyShareTargetSelect(unsigned int uiSlot);
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
	void PickPLObject(SWorldPickInfo& sPickInfo, RwBool bAttack, RwUInt32 uiSerialID = INVALID_SERIAL_ID);

	/**
	* avatar를 카메라 방향을 참조하여 바로 방향을 바꿀 필요가 있는데, 기본 move flag를 변한한다.
	*/
	RwUInt8 ConvertStraightMoveFlags(RwUInt8 uiMoveFlags);

	/**
	* avatar를 카메라 방향을 참조하여(현재 move flag와 조합하여) 바로 회전한다.
	*/
	RwReal CalcAvatarStraightRotate(RwUInt32 uiMoveFlags);

	/**
	* 카메라 회전 감도를 구한다.
	*/
	void CalcCameraRotateSensitivity(int iPosX, int iPosY, int iMouseDeltaX, int iMouseDeltaY);

	/**
	* 마우스 버튼이 아무것도 눌리지 않고,  keyboard로 캐릭터가 front and back 으로 움직일 경우
	* 캐릭터 뒷편으로 카메라를 회전시킬 delta angle을 계산한다.
	*/
	void CalcCameraRotateToCharBack(void);

	/**
	* 마우스 버튼이 아무것도 눌리지 않고,  keyboard로 캐릭터가 front and back 으로 움직일 경우
	* 캐릭터 뒷편으로 카메라를 회전시켜준다.
	*/
	void UpdateCameraRotateToCharBack(RwReal fElapsed);

	/**
	* auto run일 경우.
	*/
	void UpdateAutoRun(RwReal fElapsed);

	/**
	* move mark effect create 함수.
	*/
	void CreateMoveMark(const RwV3d *pPos);

	/**
	* move mark effect delete 함수.
	*/
	void DeleteMoveMark(void);

	/**
	* move mark effect update 함수.
	*/
	void UpdateMoveMark(RwReal fElapsed);
	
	/**
	* move mark effect update 함수.
	*/
	void UpdateChangeHeading(RwReal fElapsed);

	/**
	* picking focus update 함수.
	*/
	void UpdatePickFocus(RwReal fElapsed);

	/**
	* mouse left button up 일때 처리 함수.
	*/
	void MouseUpLBtnHandler(const SMouseData *pData);

	/**
	* mouse right button up 일때 처리 함수.
	*/
	void MouseUpRBtnHandler(const SMouseData *pData);

	/**
	* mouse move left button down 일때 처리 함수.
	*/
	void MouseMoveLBtnHandler(const SMouseData *pData);

	/**
	* mouse move right button down 일때 처리 함수.
	*/
	void MouseMoveRBtnHandler(const SMouseData *pData);

	/**
	* mouse move not button down 일때 처리 함수.
	*/
	void MouseMoveNotBtnHandler(const SMouseData *pData);

private:

	// picking
	SWorldPickInfo	m_sPickInfo;
	sWorldPickInfo_for_Cursor	m_sPickInfo_for_Cursor;

	RwReal			m_fPickTime;

	// Effect
	RwReal			m_fMoveMarkTime;
	CNtlPLEntity	*m_pMoveMark;
	RwUInt8	    	m_uiCurMoveFlags;	
	RwBool			m_bAutoRun;
	RwBool			m_bHTBRPStock;

	// Looting
	RwReal			m_fKeyLootingTime;

	ECameraRotateFlag m_eRotFlag;

	RwBool			m_bLButtonMoveFlag;
	RwInt32			m_iOldLButtonMouseX;
	RwInt32			m_iOldLButtonMouseY;

	RwInt32			m_iDeltaButtonMouseX;
	RwInt32			m_iDeltaButtonMouseY;

	RwBool			m_bRButtonMoveFlag;
	RwInt32			m_iOldRButtonMouseX;
	RwInt32			m_iOldRButtonMouseY;

	RwBool			m_bChangeHeadingFlag;
	RwBool			m_bAutoRunKeyboardChangeHeading;
	RwBool			m_bSkipMouseWheel;
	RwReal			m_fChangeHeadingUpdateTime;
	RwReal			m_fChangeHeadingDeltaTime;
	RwReal			m_fChangeHeadingDeltaAngle;

	SERIAL_HANDLE   m_hFocusSerialId;

	INPUT_HANDLE	m_handleMouseWheel;
	INPUT_HANDLE	m_handleMouseMove;
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
