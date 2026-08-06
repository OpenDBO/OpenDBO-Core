/*****************************************************************************
 *
 * File			: CharacterStage.h
 * Author		: HyungSuk, Jang
 * Copyright	: (占쏙옙)NTL
 * Date			: 2006. 11. 28	
 * Abstract		: Character stage class
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __CHARACTER_STAGE_H__
#define __CHARACTER_STAGE_H__

#include "NtlStage.h"
#include "ceventhandler.h"
#include "eventtimer.h"

#include "NtlPLWorldEntity.h"
#include "NtlPLObject.h"
#include "NtlPLSky.h"
#include "NtlPLCharacter.h"
#include "CharGuiGroup.h"
#include "NtlSobAvatar.h"

#include "DboEvent.h"
#include "InputHandler.h"


class CLobbyStage : public CNtlStage, public RWS::CEventHandler
{
#define dCAMERA_STEP_DISTANCE	0.1f
#define dCAMERA_MOVE_TIME		0.1f

	struct sCameraData
	{
		RwV3d		v3Pos;
	};

	enum eCharHeightType
	{
		CHT_CHILD,
		CHT_ADULT,

		NUM_CHT
	};

	enum eZoomLevel
	{
		ZL_ZOOM_OUT,
		ZL_ZOOM_IN,

		NUM_ZL
	};

	struct sCameraZoom
	{
		RwReal				fElapsed;
		RwV3d				v3CameraSrcPos;
		RwV3d				v3CameraDestPos;
		RwV3d				v3CameraSrcDir;
		RwV3d				v3CameraDestDir;

		sCameraZoom()
		:fElapsed(0.f)
		{}
	};

private:
	CNtlPLWorldEntity		*m_pWorldEntity;						// Main World
	
	RwV3d					m_vCameraPos;							// Camera Position
	RwV3d					m_vCameraPosAt;							// Camera占쏙옙 占쏙옙占쏙옙 占쏙옙치							
	RwV3d					m_vAvatarPos;							// Avatar占쏙옙 占쏙옙치
	RwV3d					m_vAvatarUpdatePos;

	RwReal					m_fAvatarAngleY;						// Avatar占쏙옙 Y占쏙옙 회占쏙옙 占쏙옙

	CNtlSobAvatar			*m_pAvatar;								// Avatar
	RwInt32					m_iAvatarRotState;						// Avatar Rotation State
	RwReal					m_fRotSpeedPerSec;						// Avatar Rotation Speed

	CCharGuiGroup			*m_pGuiGroup;							// Gui Group
	RwBool					m_bWorldAddCamera;						// World Add/Remove
		
	RwInt8					m_byRace;
	RwInt8					m_byCharHeightType;

	// ���콺 �� ��ũ�ѿ� ���� �ڿ������� ������ �ϱ� ���� ��� �� �޼��� ����
	std::list<sCameraZoom>	m_listZoomMode;
	RwReal					m_fReservatedZoomStep;
	RwReal					m_fTotalZoomStep;
	RwReal					m_fTotalDistance;
	RwReal					m_fTotalDirDistance;

	sCameraData				m_CameraData[RACE_COUNT + 1][NUM_CHT][NUM_ZL];
	RwV3d					m_v3Dir[RACE_COUNT + 1][NUM_CHT][NUM_ZL];

	RwBool					m_bOptionApply;							// CLobbayStage 占쏙옙체 占쏙옙占쏙옙占쏙옙 占싼뱄옙占쏙옙 FALSE 占쏙옙占쏙옙 占쏙옙占쏙옙占쏙옙.

private:
	INPUT_HANDLE m_hKeyDown;

private:
	void CheckDeletedCharacter();

	int	KeyboardDownHandler(uintptr_t uiKeyData);

	// Process network
	void RequestLobbyInfo();

	// event handler
	void EventProcUpdateTick(RwReal fElapsed);
	
	// Change Create
	void CreateDummyAvatar( BYTE byRace, 
		                    BYTE byClass, 
							BYTE byGender, 
							BYTE byFace, 
							BYTE byHair, 
							BYTE bySkinColor, 
							BYTE byHairColor);

	void CreateDownLoadAvatar( sPC_SUMMARY *pCharInfo);

	void SetupDefaultCamerabyRace(RwUInt8 byRace);							//Avatar占쏙옙 占쏙옙치, Camera占쏙옙 占쏙옙치占쏙옙 占십깍옙화
	void CreateBackGroundStage( RwUInt32 uiRace, RwBool bForce = false);	//World占쏙옙 占쏙옙占쏙옙(占싼뱄옙占쏙옙 占쏙옙占쏙옙占쏙옙 占싫댐옙.)
	void TeleportStage();													//SetUpDefaultCamerabyRace占쏙옙 占쏙옙占쌔쇽옙 占쏙옙占쏙옙占쏙옙 占쏙옙占쏙옙占쏙옙 Teleport
																			//Teleport占쏙옙 占싸듸옙占쏙옙 占싹어난占쏙옙.

	void DestroyBackGroundStage();
	void DestroyAvatar();

public:
	CLobbyStage(const char *pStageName);
	~CLobbyStage();

	virtual bool Create(void);
	virtual void Destroy(void);

	virtual void HandleEvents(RWS::CMsg &pMsg);

public:
	void OnTimer(void);
}; 

#endif
