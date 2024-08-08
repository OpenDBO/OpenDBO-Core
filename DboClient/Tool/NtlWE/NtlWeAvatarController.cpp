#include "StdAfx.h"

#include "NtlWeAvatarController.h"

#include "NtlCameraManager.h"
#include "NtlSLApi.h"
#include "TableContainer.h"
#include "NtlSLEventFunc.h"
#include "InputActionMap.h"

#include "NtlSLGlobal.h"
#include "NtlSobAvatar.h"
#include "NtlSobProxy.h"
#include "NtlCoreApi.h"
#include "PCTable.h"

#include "NtlMovement.h"
#include "NtlSLApi.h"
#include "NtlSLPacketGenerator.h"
#include "NtlSobAvatarAttr.h"

#include "NtlPLWorldEntity.h"

#define NTL_ZOOM_INOUT_SENSITIVITY		0.03f
#define NTL_MOUSE_SENSITIVITY_SCALE		0.5f
#define NTL_HEADING_UPDATE_TIME			1.0f

#define KEY_LOOTING_TIME				0.5f

CNtlWeAvatarController::CNtlWeAvatarController(void) : m_pVisualManager(NULL),
													   m_pWeAvatar(NULL),
													   m_bFirstCreate(FALSE),
													   m_bAvatarCreate(FALSE),
													   m_pCamera(NULL),
													   m_fZoomDist(20.f)
{
	memset(&m_sRotCharBack, 0, sizeof(SCameraAutoRotate));
	m_bLButtonMoveFlag = FALSE;
	m_bRButtonMoveFlag = FALSE;
	m_bChangeHeadingFlag = FALSE;
	m_fChangeHeadingUpdateTime = 0.0f;
	
	m_sRotCharBack.bActive = FALSE;
	m_sRotCharBack.fCurr = 0.0f;
	m_sRotCharBack.fDelta = 0.0f;
}

CNtlWeAvatarController::~CNtlWeAvatarController(void)
{
}

void CNtlWeAvatarController::CreateControl(CNtlCamera *pCamera, CNtlPLVisualManager *pVisualManager)
{
	m_pCamera = pCamera;
	m_pVisualManager = pVisualManager;

	if(m_bFirstCreate == FALSE)
	{
		GetInputActionMap()->LinkKeyboardMove(this, &CNtlWeAvatarController::ActionMapKeyboardMove);
		GetInputActionMap()->SetActive( TRUE );
		GetInputActionMap()->LinkJump(this, &CNtlWeAvatarController::ActionJump);
		m_bFirstCreate = TRUE;
	}
	
	if(m_bAvatarCreate == FALSE)
	{
		CreateWeAvatar();

		GetNtlGameCameraManager()->SetActiveCamera(m_pCamera);
		GetNtlGameCameraManager()->SetActiveActor(m_pWeAvatar);
		GetNtlSLGlobal()->SetSobAvatar(m_pWeAvatar);

		m_handleMouseWheel = CInputHandler::GetInstance()->LinkMouseWheel(this, &CNtlWeAvatarController::MouseWheelHandler);
		m_handleMouseMove  = CInputHandler::GetInstance()->LinkMouseMove(this, &CNtlWeAvatarController::MouseMoveHandler);
		m_handleMouseDown  = CInputHandler::GetInstance()->LinkMouseDown(this, &CNtlWeAvatarController::MouseDownHandler);
		m_handleMouseUp    = CInputHandler::GetInstance()->LinkMouseUp(this, &CNtlWeAvatarController::MouseUpHandler);

		m_bAvatarCreate = TRUE;
		m_pCamera = pCamera;

		SetZoomDist(m_fZoomDist);
	}
	m_uiCurMoveFlags = 0;
}

void CNtlWeAvatarController::UpdateControl(float fElapsed)
{
	CLAMP(fElapsed, 0.0f, 1.0f);

	if(m_bAvatarCreate)
	{
		// dGET_WORLD_PARAM()->WorldPolyCnt = 0;

		API_CoreUpdate(fElapsed);
		GetNtlGameCameraManager()->Update(fElapsed);
		
		if(m_pVisualManager)
			m_pVisualManager->UpdateBeforeCamera(fElapsed);

		GetSceneManager()->GetWorld()->SetPlayerPosition(m_pWeAvatar->GetPosition());
	}
}

void CNtlWeAvatarController::DestroyControl()
{
	if(m_pWeAvatar != NULL)
	{
		SAvatarInfo *pAvatarInfo = GetNtlSLGlobal()->GetAvatarInfo();
		CNtlSLEventGenerator::SobDelete(pAvatarInfo->uiSerialId);
		
		m_pWeAvatar = NULL;
		m_pCamera = NULL;

		CInputHandler::GetInstance()->UnLinkMouseWheel(m_handleMouseWheel);
		CInputHandler::GetInstance()->UnLinkMouseMove(m_handleMouseMove);
		CInputHandler::GetInstance()->UnLinkMouseDown(m_handleMouseDown);
		CInputHandler::GetInstance()->UnLinkMouseUp(m_handleMouseUp);

		m_bAvatarCreate = FALSE;
		GetNtlSLGlobal()->SetSobAvatar(NULL);

		GetInputActionMap()->SetActive( FALSE );
	}
}

void CNtlWeAvatarController::CreateWeAvatar()
{
	SAvatarInfo *pAvatarInfo = GetNtlSLGlobal()->GetAvatarInfo();
	pAvatarInfo->uiSerialId = 1;		

	//sPC_TBLDAT *pTblData = (sPC_TBLDAT*)API_GetTableContainer()->GetPcTable()->GetPcTbldat(RACE_NAMEK, PC_CLASS_SHADOW_KNIGHT, GENDER_ONE_SEX);   
	sPC_TBLDAT *pTblData = (sPC_TBLDAT*)API_GetTableContainer()->GetPcTable()->GetPcTbldat(RACE_HUMAN, PC_CLASS_STREET_FIGHTER, GENDER_FEMALE);  
	//sPC_TBLDAT *pTblData = (sPC_TBLDAT*)API_GetTableContainer()->GetPcTable()->GetPcTbldat(RACE_MAJIN, PC_CLASS_WONDER_MAJIN, GENDER_MALE);  
	pAvatarInfo->sCharPf.tblidx = pTblData->tblidx;	
	
	wchar_t awcUserName[NTL_MAX_SIZE_CHAR_NAME_UNICODE];
	DWORD	dwSize = NTL_MAX_SIZE_CHAR_NAME_UNICODE;
	if (GetUserNameW(awcUserName, &dwSize))
	{
		wcscpy_s(pAvatarInfo->sCharPf.awchName, NTL_MAX_SIZE_CHAR_NAME_UNICODE, awcUserName);
	}
	else
	{
		wcscpy_s(pAvatarInfo->sCharPf.awchName, NTL_MAX_SIZE_CHAR_NAME_UNICODE, L"Saber 1.0");
	}

	pAvatarInfo->sCharPf.sPcShape.byFace = 1;
	pAvatarInfo->sCharPf.sPcShape.byHair = 1;
	pAvatarInfo->sCharPf.sPcShape.byHairColor = 1;
	pAvatarInfo->sCharPf.sPcShape.bySkinColor = 1;
	
	pAvatarInfo->sCharPf.avatarAttribute.byBaseStr = 1;
	pAvatarInfo->sCharPf.avatarAttribute.byLastStr = 1;
	pAvatarInfo->sCharPf.avatarAttribute.byBaseCon = 1;
	pAvatarInfo->sCharPf.avatarAttribute.byLastCon = 1;
	pAvatarInfo->sCharPf.avatarAttribute.byBaseFoc = 1;
	pAvatarInfo->sCharPf.avatarAttribute.byLastFoc = 1;
	pAvatarInfo->sCharPf.avatarAttribute.byBaseDex = 1;
	pAvatarInfo->sCharPf.avatarAttribute.byLastDex = 1;
	pAvatarInfo->sCharPf.avatarAttribute.byBaseSol = 1;
	pAvatarInfo->sCharPf.avatarAttribute.byLastSol = 1;
	pAvatarInfo->sCharPf.avatarAttribute.byBaseEng = 1;
	pAvatarInfo->sCharPf.avatarAttribute.byLastEng = 1;

	pAvatarInfo->sCharPf.wCurLP = 100;
	pAvatarInfo->sCharPf.avatarAttribute.wBaseMaxLP = 100;
	pAvatarInfo->sCharPf.avatarAttribute.wLastMaxLP = 100;
	pAvatarInfo->sCharPf.wCurEP = 100;
	pAvatarInfo->sCharPf.avatarAttribute.wBaseMaxEP = 100;
	pAvatarInfo->sCharPf.avatarAttribute.wLastMaxEP = 100;
	pAvatarInfo->sCharPf.wCurRP = 100;
	pAvatarInfo->sCharPf.avatarAttribute.wBaseMaxRP = 100;
	pAvatarInfo->sCharPf.avatarAttribute.wLastMaxRP = 100;

	pAvatarInfo->sCharPf.avatarAttribute.wBasePhysicalOffence = 100;
	pAvatarInfo->sCharPf.avatarAttribute.wLastPhysicalOffence = 100;
	pAvatarInfo->sCharPf.avatarAttribute.wBasePhysicalDefence = 100;
	pAvatarInfo->sCharPf.avatarAttribute.wLastPhysicalDefence = 100;
	pAvatarInfo->sCharPf.avatarAttribute.wBaseEnergyOffence = 100;
	pAvatarInfo->sCharPf.avatarAttribute.wLastEnergyOffence = 100;
	pAvatarInfo->sCharPf.avatarAttribute.wBaseEnergyDefence = 100;
	pAvatarInfo->sCharPf.avatarAttribute.wLastEnergyDefence = 100;
		
	pAvatarInfo->sCharPf.avatarAttribute.wBaseAttackRate = 100;
	pAvatarInfo->sCharPf.avatarAttribute.wLastAttackRate = 100;
	pAvatarInfo->sCharPf.avatarAttribute.wBaseDodgeRate = 100;
	pAvatarInfo->sCharPf.avatarAttribute.wLastDodgeRate = 100;
	pAvatarInfo->sCharPf.avatarAttribute.wBaseBlockRate = 100;
	pAvatarInfo->sCharPf.avatarAttribute.wLastBlockRate = 100;
	pAvatarInfo->sCharPf.avatarAttribute.wLastPhysicalCriticalRate = 100;
	pAvatarInfo->sCharPf.avatarAttribute.wLastEnergyCriticalRate = 100;
		
	pAvatarInfo->sCharPf.avatarAttribute.fLastRunSpeed = 8.0f;
	pAvatarInfo->sCharPf.byLevel = 1;
	pAvatarInfo->sCharPf.dwCurExp = 1;

	pAvatarInfo->sCharState.sCharStateBase.vCurLoc.x = 0.0f;
	pAvatarInfo->sCharState.sCharStateBase.vCurLoc.y = 0.0f;
	pAvatarInfo->sCharState.sCharStateBase.vCurLoc.z = 0.0f;

	pAvatarInfo->sCharState.sCharStateBase.vCurDir.x = 0.0f;  
	pAvatarInfo->sCharState.sCharStateBase.vCurDir.y = 0.0f;  
	pAvatarInfo->sCharState.sCharStateBase.vCurDir.z = 1.0f;  

	pAvatarInfo->byItemCount = 0;
	pAvatarInfo->bySkillCount = 0;
	pAvatarInfo->byHTBCount = 0;
/*
	pAvatarInfo->byItemCount = 5;

	pAvatarInfo->sItemPf[0].handle			= 20000;
	pAvatarInfo->sItemPf[0].tblidx			= 13044;
	pAvatarInfo->sItemPf[0].byPlace			= CONTAINER_TYPE_EQUIP;	
	pAvatarInfo->sItemPf[0].byPos			= EQUIP_SLOT_TYPE_JACKET;
	pAvatarInfo->sItemPf[0].byStackcount	= 1;
	pAvatarInfo->sItemPf[0].byRank			= 1;
	pAvatarInfo->sItemPf[0].byCurDur		= 1;		// 내구도
	pAvatarInfo->sItemPf[0].Option			= 0xffffffff;
	pAvatarInfo->sItemPf[0].bNeedToIdentify = false;
	pAvatarInfo->sItemPf[0].byGrade			= 1;

	pAvatarInfo->sItemPf[1].handle			= 20001;
	pAvatarInfo->sItemPf[1].tblidx			= 13012;
	pAvatarInfo->sItemPf[1].byPlace			= CONTAINER_TYPE_EQUIP;	
	pAvatarInfo->sItemPf[1].byPos			= EQUIP_SLOT_TYPE_PANTS;
	pAvatarInfo->sItemPf[1].byStackcount	= 1;
	pAvatarInfo->sItemPf[1].byRank			= 1;
	pAvatarInfo->sItemPf[1].byCurDur		= 1;		// 내구도
	pAvatarInfo->sItemPf[1].Option			= 0xffffffff;
	pAvatarInfo->sItemPf[1].bNeedToIdentify = false;
	pAvatarInfo->sItemPf[1].byGrade			= 1;

	pAvatarInfo->sItemPf[2].handle			= 20002;
	pAvatarInfo->sItemPf[2].tblidx			= 13013;
	pAvatarInfo->sItemPf[2].byPlace			= CONTAINER_TYPE_EQUIP;	
	pAvatarInfo->sItemPf[2].byPos			= EQUIP_SLOT_TYPE_BOOTS;
	pAvatarInfo->sItemPf[2].byStackcount	= 1;
	pAvatarInfo->sItemPf[2].byRank			= 1;
	pAvatarInfo->sItemPf[2].byCurDur		= 1;		// 내구도
	pAvatarInfo->sItemPf[2].Option			= 0xffffffff;
	pAvatarInfo->sItemPf[2].bNeedToIdentify = false;
	pAvatarInfo->sItemPf[2].byGrade			= 1;

	pAvatarInfo->sItemPf[3].handle			= 20003;
	pAvatarInfo->sItemPf[3].tblidx			= 17051;
	pAvatarInfo->sItemPf[3].byPlace			= CONTAINER_TYPE_EQUIP;	
	pAvatarInfo->sItemPf[3].byPos			= EQUIP_SLOT_TYPE_SCOUTER;
	pAvatarInfo->sItemPf[3].byStackcount	= 1;
	pAvatarInfo->sItemPf[3].byRank			= 1;
	pAvatarInfo->sItemPf[3].byCurDur		= 1;		// 내구도
	pAvatarInfo->sItemPf[3].Option			= 0xffffffff;
	pAvatarInfo->sItemPf[3].bNeedToIdentify = false;
	pAvatarInfo->sItemPf[3].byGrade			= 1;

	pAvatarInfo->sItemPf[4].handle			= 20004;
	pAvatarInfo->sItemPf[4].tblidx			= 10401;
	pAvatarInfo->sItemPf[4].byPlace			= CONTAINER_TYPE_EQUIP;	
	pAvatarInfo->sItemPf[4].byPos			= EQUIP_SLOT_TYPE_SUB_WEAPON;
	pAvatarInfo->sItemPf[4].byStackcount	= 1;
	pAvatarInfo->sItemPf[4].byRank			= 1;
	pAvatarInfo->sItemPf[4].byCurDur		= 1;		// 내구도
	pAvatarInfo->sItemPf[4].Option			= 0xffffffff;
	pAvatarInfo->sItemPf[4].bNeedToIdentify = false;
	pAvatarInfo->sItemPf[4].byGrade			= 1;
*/
	pAvatarInfo->sCharState.sCharStateBase.vCurLoc.x = 0.0f;
	pAvatarInfo->sCharState.sCharStateBase.vCurLoc.y = 0.0f;
	pAvatarInfo->sCharState.sCharStateBase.vCurLoc.z = 0.0f;

	pAvatarInfo->sCharState.sCharStateBase.vCurDir.x = 0.0f;  
	pAvatarInfo->sCharState.sCharStateBase.vCurDir.y = 0.0f;  
	pAvatarInfo->sCharState.sCharStateBase.vCurDir.z = 1.0f;  

	pAvatarInfo->sWorldInfo.tblidx = 0;
	pAvatarInfo->sWorldInfo.worldID = 0;

	pAvatarInfo = GetNtlSLGlobal()->GetAvatarInfo();

	if(m_pWeAvatar == NULL)
		m_pWeAvatar = reinterpret_cast<CNtlSobAvatar*>(CNtlSLEventGenerator::SobAvatarCreate(SLCLASS_AVATAR, pAvatarInfo->uiSerialId, &pAvatarInfo->sCharState));
}

void CNtlWeAvatarController::SetVisibleWeAvatar(bool bVisible)
{
	NTL_ASSERTE(m_pWeAvatar != NULL);
	m_pWeAvatar->EnableVisible(bVisible);

	if(bVisible == true)
		GetNtlGameCameraManager()->SetActiveActor(m_pWeAvatar);
	else
		GetNtlGameCameraManager()->SetActiveActor(NULL);
}

int CNtlWeAvatarController::ActionMapKeyboardMove(unsigned int uiMoveFlags)
{
	if(m_bActive)
	{
		if(uiMoveFlags == 0)
		{
			CNtlSLEventGenerator::KeyboardMoveStop();
		}
		else 
		{
			CNtlSLEventGenerator::KeyboardMove(uiMoveFlags, FALSE, 0.0f);
		}
	}

	return 1;
}

int CNtlWeAvatarController::ActionJump(unsigned int uiMoveFlags)
{
	if(m_bActive)
		CNtlSLEventGenerator::ActionMapJump(uiMoveFlags);
	
	return 1;
}

int	CNtlWeAvatarController::MouseWheelHandler(unsigned int uiMouseData)
{
	SMouseData *pData = (SMouseData *)uiMouseData;

	//120.f (Wheel
	//RwReal fDelta = -pData->zDelta * NTL_ZOOM_INOUT_SENSITIVITY;
	//GetNtlGameCameraManager()->ZoomInOut(fDelta);

	return TRUE;
}

int	CNtlWeAvatarController::MouseMoveHandler(unsigned int uiMouseData)
{
	SMouseData *pData = (SMouseData *)uiMouseData;
	if( pData->uiFlags & MK_RBUTTON)
		MouseMoveRBtnHandler(pData);
	
	return TRUE;
}

int	CNtlWeAvatarController::MouseDownHandler(unsigned int uiMouseData)
{
	SMouseData *pData = (SMouseData *)uiMouseData;
	if(pData->chBtnType == MOUSE_RBTN)
	{
		m_iOldRButtonMouseX = pData->iPosX;
		m_iOldRButtonMouseY = pData->iPosY;
	}
	CalcCameraRotateToCharBack();
	
	return TRUE;
}


int	CNtlWeAvatarController::MouseUpHandler(unsigned int uiMouseData)
{
	NTL_FUNCTION("CAvatarController::MouseUpHandler");

	SMouseData *pData = (SMouseData *)uiMouseData;
	if(pData->chBtnType == MOUSE_RBTN)
	{
		MouseUpRBtnHandler(pData);
	}
	// camera auto rotate를 판정한다.
	CalcCameraRotateToCharBack();
	NTL_RETURN(1);
}

CNtlWeAvatarController &CNtlWeAvatarController::GetInstance()
{
	static CNtlWeAvatarController	s_WeAvatarController;
	return s_WeAvatarController;
}

void CNtlWeAvatarController::SetActive(bool bFlag)
{
	CNtlWeController::SetActive(bFlag);

	if(m_bActive)
	{
		CInputActionMap::GetInstance()->SetActive(TRUE);
		CNtlWeAvatarController::GetInstance().SetVisibleWeAvatar(TRUE);

		if(m_pCamera && m_pWeAvatar)
		{
			RwCamera *pCamera = m_pCamera->GetCamera();
			RwFrame *pFrame = RwCameraGetFrame(pCamera);
			m_pWeAvatar->SetPosition(&pFrame->ltm.pos);
		}
	}
	else
	{
		CInputActionMap::GetInstance()->SetActive(FALSE);
		CNtlWeAvatarController::GetInstance().SetVisibleWeAvatar(FALSE);
	}
}

void CNtlWeAvatarController::CalcCameraRotateSensitivity(int iPosX, int iPosY, int iMouseDeltaX, int iMouseDeltaY)
{
	RwReal fDeltaYaw = ((RwReal)iMouseDeltaX * NTL_MOUSE_SENSITIVITY_SCALE);
	RwReal fDeltaPitch = ((RwReal)iMouseDeltaY * NTL_MOUSE_SENSITIVITY_SCALE);
	
	GetNtlGameCameraManager()->AddYaw(fDeltaYaw);
	GetNtlGameCameraManager()->AddPitch(-fDeltaPitch);
}

void CNtlWeAvatarController::CalcCameraRotateToCharBack(void)
{
	if(m_uiCurMoveFlags == NTL_MOVE_NONE || m_bLButtonMoveFlag || m_bRButtonMoveFlag)
	{
		m_sRotCharBack.bActive = FALSE;
	}
	else
	{
		if(!m_sRotCharBack.bActive)
		{
			CNtlSobAvatar *pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();
			RwV3d vDir = pSobAvatar->GetDirection();
			RwReal fAngleY = CNtlMath::LineToAngleY(&vDir);
			if(fAngleY < 0.0f)
				fAngleY += 360.0f;

			RwReal fCovAngleY = GetNtlGameCameraManager()->GetYaw();
			if(fCovAngleY >= 360.0f)
				fCovAngleY -= 360.0f;

			//  회전은 시계 반대 방향 회전이다.
			//  카메라 angle 에서 character angle 뺀다.
			RwReal fSubAngleY = fCovAngleY - fAngleY;

			if(fSubAngleY > 0.0f)
				if(fSubAngleY > 180.0f) // 각도를 증가시킨다.
					m_sRotCharBack.fDelta = 360.0f - fSubAngleY;
				else // 각도를 감소시킨다.
					m_sRotCharBack.fDelta = -fSubAngleY; //(시계 반대 방향이므로 빼줘야 한다.)
			else
				if(fSubAngleY > -180.0f) // 각도를 증가시킨다.
					m_sRotCharBack.fDelta = fabs(fSubAngleY); 
				else // 각도를 감소시킨다.
					m_sRotCharBack.fDelta = -(360.0f - fabs(fSubAngleY)); // 시계 반대 방향이므로 빼줘야 한다.
			
			m_sRotCharBack.fCurr = 0.0f;
			m_sRotCharBack.bActive = TRUE;
		}
	}
}

void CNtlWeAvatarController::UpdateCameraRotateToCharBack(RwReal fElapsed)
{
	if(!m_sRotCharBack.bActive)
		return;

	RwReal fDeltaAngleY = 180.0f * fElapsed;

	if(m_sRotCharBack.fDelta > 0.0f)
	{
		if(m_sRotCharBack.fCurr + fDeltaAngleY >= m_sRotCharBack.fDelta)
		{
			fDeltaAngleY = m_sRotCharBack.fDelta - m_sRotCharBack.fCurr;
			m_sRotCharBack.bActive = FALSE;
		}
		else
			m_sRotCharBack.fCurr += fDeltaAngleY;

		GetNtlGameCameraManager()->AddYaw(fDeltaAngleY);
	}
	else
	{
		if(m_sRotCharBack.fCurr - fDeltaAngleY <= m_sRotCharBack.fDelta)
		{
			fDeltaAngleY = fabs(m_sRotCharBack.fCurr - m_sRotCharBack.fCurr);
			m_sRotCharBack.bActive = FALSE;
		}
		else
			m_sRotCharBack.fCurr -= fDeltaAngleY;

		GetNtlGameCameraManager()->AddYaw(-fDeltaAngleY);
	}
}

void CNtlWeAvatarController::MouseUpRBtnHandler(const SMouseData *pData)
{
	m_bRButtonMoveFlag = FALSE;
}

void CNtlWeAvatarController::MouseMoveRBtnHandler(const SMouseData *pData)
{
	RwInt32 iDeltaX = (m_iOldRButtonMouseX - pData->iPosX);
	RwInt32 iDeltaY = (m_iOldRButtonMouseY - pData->iPosY);

	CalcCameraRotateSensitivity(pData->iPosX, pData->iPosY, iDeltaX, iDeltaY);

	m_iOldRButtonMouseX = pData->iPosX;
	m_iOldRButtonMouseY = pData->iPosY;

	m_bRButtonMoveFlag = TRUE;
}

void CNtlWeAvatarController::SetFov(float fFov)
{
	if(m_pCamera)
		m_pCamera->SetCalcFov(fFov);
}

void CNtlWeAvatarController::SetSpeed(float fSpeed)
{
	if(m_pWeAvatar)
	{
		CNtlSobAvatarAttr* pAvatarAttr = reinterpret_cast<CNtlSobAvatarAttr*>( m_pWeAvatar->GetSobAttr() );
		if(pAvatarAttr)
		{
			//pAvatarAttr->m_fRunSpeed = fSpeed;
			pAvatarAttr->SetRunSpeed(fSpeed);
		}
	}

	dGET_WORLD_PARAM()->WorldCamSpeedPerSec	= fSpeed;
}

void CNtlWeAvatarController::SetPosition(RwV3d *pPos)
{
	if(m_pWeAvatar)
	{
		m_pWeAvatar->SetPosition(pPos);
	}
}

void CNtlWeAvatarController::SetZoomDist(float fZoom)
{
	float fDist	 = GetNtlGameCameraManager()->GetDist();

	if(fZoom < 0.1f)
		fZoom = 0.1f;

	m_fZoomDist = fZoom;
	float fDelta = fZoom - fDist;
	
	GetNtlGameCameraManager()->SetMaxDist(m_fZoomDist); 
	GetNtlGameCameraManager()->ZoomInOut(fDelta);
}

VOID CNtlWeAvatarController::Init()
{	
	GetSceneManager()->SetSoundListener(m_pWeAvatar->GetSobProxy()->GetPLMainEntity());
}