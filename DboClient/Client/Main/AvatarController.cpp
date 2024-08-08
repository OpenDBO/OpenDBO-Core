#include "precomp_dboclient.h"
#include "AvatarController.h"

// shared
#include "NtlMovement.h"
#include "NtlResultCode.h"
#include "NtlDirection.h"

// core
#include "NtlSysEvent.h"
#include "NtlEventQueue.h"

// sound
#include "NtlSoundDefines.h"

// gui
#include "gui_renderer.h"

// presentation
#include "NtlPLSceneManager.h"
#include "NtlPLGuiManager.h"
#include "NtlDNController.h"
#include "NtlPostEffectCamera.h"

// simulation
#include "InputActionMap.h" 
#include "NtlSLEvent.h"
#include "NtlSLEventFunc.h"
#include "NtlSLGlobal.h"
#include "NtlSLLogic.h"
#include "NtlSobAvatar.h"
#include "NtlSobIcon.h"
#include "NtlSobManager.h"
#include "NtlSobProxy.h"
#include "NtlSobTriggerObject.h"
#include "NtlSkillContainer.h"
#include "NtlSLPacketGenerator.h"
#include "NtlSLVisualDeclear.h"
#include "NtlCameraManager.h"
#include "NtlSobHTBSkill.h"
#include "NtlSLApi.h"
#include "NtlWorldConcept.h"
#include "NtlDTCinematicManager.h"
#include "NtlFSMUtil.h"
#include "NtlFSMDef.h"
#include "NtlFSMCharActAgent.h"
#include "NtlFSMLayer.h"
#include "NtlNetSender.h"
#include "NtlSobVehicle.h"

// framework
#include "NtlApplication.h"

// dbo 
#include "DboDef.h"
#include "DisplayStringDef.h"
#include "DboGlobal.h"
#include "DboEvent.h"
#include "DumpCommand.h"
#include "DboLogic.h"
#include "CursorManager.h"
#include "AlarmManager.h"
#include "DialogManager.h"

#include "NtlPLObject.h"		//Test 추후 삭제 예정(by HoDong)
#include "NtlPLEventGenerator.h"//Test 추후 삭제 예정(by Peessi)
#include "NtlPLEvent.h"

#include "TBGui.h"
#include "TBTournamentGui.h"

#include "NtlPlWorldEntity.h"

const char* SOUND_SL_GROUND_PICKING[4] = {"Effect\\GENERAL_Picking_Ground.wav", 
"Effect\\GENERAL_Picking_Ground-01.wav",
"Effect\\GENERAL_Picking_Ground-02.wav",
"Effect\\GENERAL_Picking_Ground-03.wav"};


#define NTL_ZOOM_INOUT_SENSITIVITY		0.03f
#define NTL_MOUSE_SENSITIVITY_SCALE		0.5f
#define NTL_HEADING_UPDATE_TIME			5.0f
#define NTL_HEADING_DELTA_TIME			0.3f
#define NTL_HEADING_DELTA_ANGLE			10.0f

#define KEY_LOOTING_TIME				0.1f

#define PICK_FOCUS_TIME					0.1f

CAvatarController::CAvatarController()
{
	m_bLButtonMoveFlag				= FALSE;
	m_bRButtonMoveFlag				= FALSE;
	m_bChangeHeadingFlag			= FALSE;
	m_bSkipMouseWheel				= FALSE;
	m_bAutoRunKeyboardChangeHeading	= TRUE;
	m_fChangeHeadingUpdateTime		= 0.0f;
	m_fChangeHeadingDeltaTime		= 0.0f;

	m_fPickTime						= 0.0f;

	m_fChangeHeadingDeltaAngle		= 0.0f;

	m_hFocusSerialId				= INVALID_SERIAL_ID;

	m_iDeltaButtonMouseX			= 0;
	m_iDeltaButtonMouseY			= 0;

	m_fMoveMarkTime					= 0.0f;
	m_pMoveMark						= NULL;
	m_bAutoRun						= FALSE;
	
	m_eRotFlag						= CAMERA_ROTATE_FLAG_NONE;

	m_fKeyLootingTime				= 0.0f;
	m_bHTBRPStock					= FALSE;
}

CAvatarController::~CAvatarController()
{
	
}

RwBool CAvatarController::Create()
{
	m_handleMouseWheel = CInputHandler::GetInstance()->LinkMouseWheel(this, &CAvatarController::MouseWheelHandler);
	m_handleMouseMove  = CInputHandler::GetInstance()->LinkMouseMove(this, &CAvatarController::MouseMoveHandler);
	m_handleMouseDown  = CInputHandler::GetInstance()->LinkMouseDown(this, &CAvatarController::MouseDownHandler);
	m_handleMouseUp    = CInputHandler::GetInstance()->LinkMouseUp(this, &CAvatarController::MouseUpHandler);
	m_handleKeyDown    = CInputHandler::GetInstance()->LinkKeyDown(this, &CAvatarController::KeyboardDownHandler);
	m_handleKeyUp      = CInputHandler::GetInstance()->LinkKeyUp(this, &CAvatarController::KeyboardUpHandler);
	m_handleRestoreHandler = CNtlDeviceRestoreHandler::GetInstance()->LinkDeviceRestore(this, &CAvatarController::RestoreHandler);

	GetInputActionMap()->LinkKeyboardMove(this, &CAvatarController::ActionMapKeyboardMove);
	GetInputActionMap()->LinkKeyboardDashMove(this, &CAvatarController::ActionMapKeyboardDashMove);
    GetInputActionMap()->LinkAutoRun(this, &CAvatarController::ActionAutoRun);
	GetInputActionMap()->LinkAvatarSelect(this, &CAvatarController::ActionAvatarSelect);
	GetInputActionMap()->LinkJump(this, &CAvatarController::ActionJump);
	GetInputActionMap()->LinkCharging(this, &CAvatarController::ActionCharging);
	GetInputActionMap()->LinkBlocking(this, &CAvatarController::ActionBlocking);
	GetInputActionMap()->LinkSitAndStand(this, &CAvatarController::ActionSitAndStand);
	GetInputActionMap()->LinkLooting(this, &CAvatarController::ActionLooting);
	GetInputActionMap()->LinkAutoTarget(this, &CAvatarController::ActionAutoTarget);
	GetInputActionMap()->LinkAutoAttack(this, &CAvatarController::ActionAutoAttack);
	GetInputActionMap()->LinkAutoFollow(this, &CAvatarController::ActionAutoFollow);
	GetInputActionMap()->LinkPartySelect(this, &CAvatarController::ActionPartySelect);
    GetInputActionMap()->LinkScouterUse(this, &CAvatarController::ActionScouterUse);
    GetInputActionMap()->LinkTargetMarking(this, &CAvatarController::ActionPartyShareTargetMarking);
    GetInputActionMap()->LinkTargetSelecting(this, &CAvatarController::ActionPartyShareTargetSelect);
    GetInputActionMap()->LinkPartyAutoSelect(this, &CAvatarController::ActionPartyAutoSelect);
	
	LinkMsg(g_EventQueueProcessFilter);
	LinkMsg(g_EventCameraShake);
	LinkMsg(g_EventSobDelete);
	LinkMsg(g_EventSobPickWorldItem);
	LinkMsg(g_EventIconPopupResult);
	LinkMsg(g_EventWorldChange);
	LinkMsg(g_EventHTBRPStock);
	LinkMsg(g_EventDialog);	
    LinkMsg(g_EventActionFuncBasicAttack);
    LinkMsg(g_EventActionFuncAssist);
    LinkMsg(g_EventActionFuncGetItem);
    LinkMsg(g_EventActionFuncSitAndStand);
	
	//test
	LinkMsg(g_EventSCS);	
	
	m_uiCurMoveFlags	= 0;
	m_bAutoRun			= FALSE;

	// camera zoom min and max setting
	if(Logic_IsDevUser())
	{
		SDevMasterInfo *pDevMasterInfo = API_GetSLDevMasterInformation();
		GetNtlGameCameraManager()->SetMinDist(pDevMasterInfo->fCameraZoomMin);
		GetNtlGameCameraManager()->SetMaxDist(pDevMasterInfo->fCameraZoomMax);
		GetNtlGameCameraManager()->SetFov(pDevMasterInfo->fCameraFov);
		GetNtlDTCinematicManager()->SetFov(pDevMasterInfo->fCameraFov);
	}

	return TRUE;
}

void CAvatarController::Destroy()
{
	CInputHandler::GetInstance()->UnLinkMouseWheel(m_handleMouseWheel);
	CInputHandler::GetInstance()->UnLinkMouseMove(m_handleMouseMove);
	CInputHandler::GetInstance()->UnLinkMouseDown(m_handleMouseDown);
	CInputHandler::GetInstance()->UnLinkMouseUp(m_handleMouseUp);

	CInputHandler::GetInstance()->UnLinkKeyDown(m_handleKeyDown);
	CInputHandler::GetInstance()->UnLinkKeyUp(m_handleKeyUp);

	GetInputActionMap()->UnLinkKeyboardMove();
	GetInputActionMap()->UnLinkKeyboardDashMove();
	GetInputActionMap()->UnLinkAvatarSelect();
	GetInputActionMap()->UnLinkJump();
	GetInputActionMap()->UnLinkCharging();
	GetInputActionMap()->UnLinkBlocking();
	GetInputActionMap()->UnLinkSitAndStand();
	GetInputActionMap()->UnLinkLooting();
	GetInputActionMap()->UnLinkAutoTarget();
	GetInputActionMap()->UnLinkAutoAttack();
	GetInputActionMap()->UnLinkAutoFollow();
	GetInputActionMap()->UnLinkPartySelect();
    GetInputActionMap()->UnLinkScouterUse();
    GetInputActionMap()->UnLinkTargetMarking();
    GetInputActionMap()->UnLinkTargetSelecting();
    GetInputActionMap()->UnLinkPartyAutoSelect();
		
	CNtlDeviceRestoreHandler::GetInstance()->UnLinkDeviceRestore(m_handleRestoreHandler);

	UnLinkMsg(g_EventQueueProcessFilter);
	UnLinkMsg(g_EventCameraShake);
	UnLinkMsg(g_EventSobDelete);
	UnLinkMsg(g_EventSobPickWorldItem);
	UnLinkMsg(g_EventIconPopupResult);
	UnLinkMsg(g_EventWorldChange);
	UnLinkMsg(g_EventHTBRPStock);
	UnLinkMsg(g_EventDialog);
    UnLinkMsg(g_EventActionFuncBasicAttack);
    UnLinkMsg(g_EventActionFuncAssist);
    UnLinkMsg(g_EventActionFuncGetItem);
    UnLinkMsg(g_EventActionFuncSitAndStand);

	//test
	UnLinkMsg(g_EventSCS);
	
	// move mark를 삭제한다.
	DeleteMoveMark();
}

void CAvatarController::Update(RwReal fElapsed)
{
	// update move mark
	UpdateMoveMark(fElapsed);

	// update change heading
	UpdateChangeHeading(fElapsed);

	CalcCameraRotateToCharBack();
	UpdateAutoRun(fElapsed);
	UpdateCameraRotateToCharBack(fElapsed);
	GetNtlGameCameraManager()->Update(fElapsed);

	m_fKeyLootingTime += fElapsed;

	UpdatePickFocus(fElapsed);	

	Logic_UpdateSkillResTimeOut(fElapsed);
}

void CAvatarController::HandleEvents(RWS::CMsg &pMsg)
{
	if(pMsg.Id == g_EventQueueProcessFilter)
	{
		if(m_uiCurMoveFlags == NTL_MOVE_NONE)
		{
			EventQueueCurrentProcessFilterMsg(g_EventKeyboardMove);
		}
	}
	else if(pMsg.Id == g_EventSobDelete)
	{
		SNtlEventSobDelete* pData  = (SNtlEventSobDelete *)pMsg.pData;
		if(pData->hSerialId == GetDboGlobal()->GetTargetSerial())
		{
			GetDboGlobal()->SetTargetSerial(INVALID_SERIAL_ID);
		}
	}
	else if(pMsg.Id == g_EventSobPickWorldItem)
	{
		SNtlEventSobPickWorldItem* pData = (SNtlEventSobPickWorldItem*)pMsg.pData;
		
		Logic_WorldItemLooting( GetNtlSobManager()->GetSobObject( pData->hSerialId ) );
	}
	else if(pMsg.Id == g_EventIconPopupResult)
	{
		SDboEventIconPopupResult* pData = (SDboEventIconPopupResult*)pMsg.pData;

		if( pData->nWorkId == PMW_FOLLOW )
			ActionAutoFollow();
	}
	else if(pMsg.Id == g_EventWorldChange)
	{
		TeleportReset();
	}
	else if(pMsg.Id == g_EventHTBRPStock)
	{
		m_bHTBRPStock = TRUE;
	}
	else if(pMsg.Id == g_EventDialog)
	{
		SDboEventDialog* pEvent = reinterpret_cast<SDboEventDialog*>( pMsg.pData );

		if( pEvent->iType == DIALOGEVENT_SKIP_MOUSE_WHEEL )
		{
			if( pEvent->iSrcDialog == DIALOG_CHAT )
			{
				m_bSkipMouseWheel = TRUE;
			}
		}
	}
    else if(pMsg.Id == g_EventActionFuncBasicAttack)
    {
        CNtlSobActor* pTargetActor = (CNtlSobActor*)GetNtlSobManager()->GetSobObject(GetNtlSLGlobal()->GetSobAvatar()->GetTargetID());
        if(!pTargetActor || pTargetActor->GetClassID() != SLCLASS_MONSTER)  // 몬스터일때만 사용가능
            return;

        ActionAutoAttack();
    }
    else if(pMsg.Id == g_EventActionFuncAssist)
    {
        CNtlSobActor* pTargetActor = (CNtlSobActor*)GetNtlSobManager()->GetSobObject(GetNtlSLGlobal()->GetSobAvatar()->GetTargetID());
        if(!pTargetActor || pTargetActor->GetClassID() != SLCLASS_PLAYER)   // 플레이어일때만 사용가능
            return;

        ActionAutoAttack();
    }
    else if(pMsg.Id == g_EventActionFuncGetItem)
    {
        ActionLooting();
    }
    else if(pMsg.Id == g_EventActionFuncSitAndStand)
    {
        ActionSitAndStand();
    }
	else if(pMsg.Id == g_EventSCS )
	{
		GetAlarmManager()->AlarmMessage( L"오토 케어 시스템이라능!!" ,CAlarmManager::ALARM_ACTION_NOTIFY);	
	}
}

RwUInt8 CAvatarController::ConvertStraightMoveFlags(RwUInt8 uiMoveFlags)
{
	if(uiMoveFlags == NTL_MOVE_NONE)
		return NTL_MOVE_NONE;

	return NTL_MOVE_F;
}

RwReal CAvatarController::CalcAvatarStraightRotate(RwUInt32 uiMoveFlags)
{
	RwReal fYaw = GetNtlGameCameraManager()->GetYaw();

	if(uiMoveFlags == NTL_MOVE_B)
	{
		fYaw += 180.0f;
	}
	else if(uiMoveFlags == NTL_MOVE_TURN_L)
	{
		fYaw += 90.0f;
	}
	else if(uiMoveFlags == NTL_MOVE_TURN_R)
	{
		fYaw -= 90.0f;
	}
	else if(uiMoveFlags == NTL_MOVE_F_TURN_L)
	{
		fYaw += 45.0f;
	}
	else if(uiMoveFlags == NTL_MOVE_F_TURN_R)
	{
		fYaw -= 45.0f;
	}
	else if(uiMoveFlags == NTL_MOVE_B_TURN_L)
	{
		fYaw += 135.0f;
	}
	else if(uiMoveFlags == NTL_MOVE_B_TURN_R)
	{
		fYaw -= 135.0f;
	}

	return fYaw;
}


void CAvatarController::CalcCameraRotateSensitivity(int iPosX, int iPosY, int iMouseDeltaX, int iMouseDeltaY)
{
	RwReal fDeltaYaw = ((RwReal)iMouseDeltaX * NTL_MOUSE_SENSITIVITY_SCALE);
	RwReal fDeltaPitch = ((RwReal)iMouseDeltaY * NTL_MOUSE_SENSITIVITY_SCALE);
	
	GetNtlGameCameraManager()->AddYaw(fDeltaYaw);
	GetNtlGameCameraManager()->AddPitch(-fDeltaPitch);
}

void CAvatarController::TeleportReset(void)
{
	m_uiCurMoveFlags				= NTL_MOVE_NONE;
	m_bLButtonMoveFlag				= FALSE;
	m_bRButtonMoveFlag				= FALSE;
	m_bChangeHeadingFlag			= FALSE;
	m_bAutoRunKeyboardChangeHeading	= TRUE;
	m_fChangeHeadingUpdateTime		= 0.0f;
	m_fChangeHeadingDeltaTime		= 0.0f;
	m_fChangeHeadingDeltaAngle		= 0.0f;

	m_iDeltaButtonMouseX			= 0;
	m_iDeltaButtonMouseY			= 0;

	m_fMoveMarkTime					= 0.0f;
	m_bAutoRun						= FALSE;
	
	m_eRotFlag						= CAMERA_ROTATE_FLAG_NONE;

	m_fKeyLootingTime				= 0.0f;
}

void CAvatarController::PickPLWorld(SWorldPickInfo& sPickInfo)
{
	if(!GetNtlWorldConcept()->IsEnableAction(E_ACTION_PICK_WORLD))
		return;

	if(m_uiCurMoveFlags == NTL_MOVE_NONE)
	{
		// 사운드 출력.
		CNtlSobAvatar *pSobAvatar = GetNtlSLGlobal()->GetSobAvatar(); 

		CNtlSLEventGenerator::TerrainClick(sPickInfo.vPickPos);
		SetAutoRun(FALSE);

		SERIAL_HANDLE hTargetSerialId = GetDboGlobal()->GetTargetSerial();
		if(hTargetSerialId != INVALID_SERIAL_ID)
			CNtlSLEventGenerator::SobAttackMarkRelease(hTargetSerialId);

		if(pSobAvatar->CanMove())
		{
			//Mark를 찍을 때마다 생성을 한다. 퍼포먼스의 하락이 있을것으로 예상
			CreateMoveMark(&sPickInfo.vPickPos);
		}
	}
}

void CAvatarController::PickPLObject(SWorldPickInfo& sPickInfo, RwBool bAttack, RwUInt32 uiSerialID /* = INVALID_SERIAL_ID */)
{
	if(!GetNtlWorldConcept()->IsEnableAction(E_ACTION_PICK_OBJECT))
		return;

    if(Logic_IsDirectMode(GetNtlSLGlobal()->GetSobAvatar()))
        return;

	if( !Logic_CanMouseInput_in_Tutorial( ETL_MOUSE_INPUT_TYPE_TARGETING ) )
		return;

    if(Logic_GetActorStateId(GetNtlSLGlobal()->GetSobAvatar()) == NTL_FSMSID_FAINTING)
        return;


    RwUInt32 uiPickSerialId = INVALID_SERIAL_ID;
    if(uiSerialID != INVALID_SERIAL_ID)
    {
        uiPickSerialId = uiSerialID;
    }
    else
    {
        uiPickSerialId = sPickInfo.pPLEntity->GetSerialID();
        
        // pick serial id 존재하지 않을 경우.
        if(uiPickSerialId == INVALID_SERIAL_ID)
            return;
    }

	CNtlSobActor *pPickActor = reinterpret_cast<CNtlSobActor*>( GetNtlSobManager()->GetSobObject(uiPickSerialId) ); 
	if(pPickActor == NULL)
		return;

	if(!pPickActor->CanClick())
		return;

	CNtlSobAvatar* pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( !pSobAvatar )
		return;

	DeleteMoveMark();

	CNtlSob *pPickSobObj = GetNtlSobManager()->GetSobObject(uiPickSerialId);

	// 현재 지정된 target이 없는 경우.
	if(GetDboGlobal()->GetTargetSerial() == INVALID_SERIAL_ID)
	{
		GetDboGlobal()->SetTargetSerial(uiPickSerialId);

		API_GetSLPacketGenerator()->SendCharTargetSelect(uiPickSerialId);

		if( bAttack &&
			GetNtlWorldConcept()->IsEnableAction(E_ACTION_ATTACK) &&
			FALSE == Logic_IsVehicleDriver( pSobAvatar ) )
		{
			if( Logic_IsEnemyTargetFromAvatarActor(pPickSobObj->GetSerialID()) && pPickSobObj->CanHurted() &&
                !Logic_isEnemyTargetOcclusion(pPickSobObj->GetSerialID())) 
            {
                CNtlSLEventGenerator::SobAttackSelect(uiPickSerialId);                
            }
			else
            {
				GetAlarmManager()->AlarmMessage(DST_BATTLE_CAN_NOT_ATTACK_TARGET);
            }
		}
		else
		{
			RwBool bTSSuccess = CNtlSLEventGenerator::SobTargetSelect(uiPickSerialId);
			if(!bTSSuccess)
				return;

			CNtlSobActor *pTarActor = (CNtlSobActor*)pPickSobObj;
			CNtlSobAvatar *pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();
			if( Logic_IsCommunityActor(pTarActor) )
			{
				RwReal fCummuRange = Logic_GetCommunityRange(pTarActor);
				if(Logic_CanActorCommunity(pSobAvatar, pTarActor, fCummuRange))
				{
					Logic_BeginActorCommunity(pSobAvatar, pTarActor);
					return;
				}
			}
		}
	}
	// 현재 지정된 target이 있는 경우.
	else
	{
		RwUInt32 uiCurrTargetSerialId = GetDboGlobal()->GetTargetSerial();

		if((uiCurrTargetSerialId != uiPickSerialId) || (pPickSobObj && Logic_IsCommunityActor((CNtlSobActor*)pPickSobObj)))
		{
			if(uiCurrTargetSerialId != uiPickSerialId)
			{
				API_GetSLPacketGenerator()->SendCharTargetSelect(uiPickSerialId);
				CNtlSLEventGenerator::SobTargetSelectRelease(uiCurrTargetSerialId);
			}

			GetDboGlobal()->SetTargetSerial(uiPickSerialId);

			if( bAttack &&
				GetNtlWorldConcept()->IsEnableAction(E_ACTION_ATTACK) &&
				FALSE == Logic_IsVehicleDriver( pSobAvatar ) )
			{
				if( Logic_IsEnemyTargetFromAvatarActor(pPickSobObj->GetSerialID()) && pPickSobObj->CanHurted() &&
                    !Logic_isEnemyTargetOcclusion(pPickSobObj->GetSerialID())) 
					CNtlSLEventGenerator::SobAttackSelect(uiPickSerialId);
				else
					GetAlarmManager()->AlarmMessage(DST_BATTLE_CAN_NOT_ATTACK_TARGET);
			}
			else
			{
				RwBool bTSSuccess = CNtlSLEventGenerator::SobTargetSelect(uiPickSerialId);
				if(!bTSSuccess)
					return;

				// communication actor 이면?
				CNtlSobActor *pTarActor = (CNtlSobActor*)pPickSobObj;
				CNtlSobAvatar *pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();
				if( Logic_IsCommunityActor(pTarActor) )
				{
					RwReal fCummuRange = Logic_GetCommunityRange(pTarActor);
					if(Logic_CanActorCommunity(pSobAvatar, pTarActor, fCummuRange))
					{
						Logic_BeginActorCommunity(pSobAvatar, pTarActor);
						return;
					}
				}
			}
		}
		else
		{
			//Target이 Character일 경우

			if( bAttack &&
				GetNtlWorldConcept()->IsEnableAction(E_ACTION_ATTACK) &&
				FALSE == Logic_IsVehicleDriver( pSobAvatar ) )
			{
				if( Logic_IsEnemyTargetFromAvatarActor(pPickSobObj->GetSerialID()) && pPickSobObj->CanHurted() &&
                    !Logic_isEnemyTargetOcclusion(pPickSobObj->GetSerialID())) 
					CNtlSLEventGenerator::SobAttackSelect(uiPickSerialId);
				else
					GetAlarmManager()->AlarmMessage(DST_BATTLE_CAN_NOT_ATTACK_TARGET);
			}
		}
	}  
}


void CAvatarController::MouseUpLBtnHandler(const SMouseData *pData)
{
	GetNtlGameCameraManager()->UserCameraControlEnable(FALSE);

	m_bLButtonMoveFlag = FALSE;

	RwInt32 iDeltaButtonMouseX = m_iDeltaButtonMouseX;
	RwInt32 iDeltaButtonMouseY = m_iDeltaButtonMouseY;

	m_iDeltaButtonMouseX = 0;
	m_iDeltaButtonMouseY = 0;

	// mouse click 
	if(pData->uiDownDeltaTime <= 300 && iDeltaButtonMouseX < 20 && iDeltaButtonMouseY < 20)
	{
		// picking 검출.
		if(!GetNtlWorldConcept()->IsEnableAction(E_ACTION_PICK_WORLD) && !GetNtlWorldConcept()->IsEnableAction(E_ACTION_PICK_OBJECT))		// Picking이 되지 않는 상태라면, Picking을 실행시키지 않는다.
			return;

		if(Logic_IsClickDisable())
			return;

		m_sPickInfo.bTerrainExclusion	= FALSE;
		m_sPickInfo.bCharacterExclusion = FALSE;
		RwBool bPick = GetSceneManager()->PickWorld(pData->iPosX, pData->iPosY, m_sPickInfo, PICK_TERRAIN_LIMIT, PICK_OBJECT_LIMIT);

		if(bPick)
		{
			ENtlPLEntityType ePLType = m_sPickInfo.pPLEntity->GetClassType();
            
			if(!pData->bDblClick)
			{
				if(ePLType == PLENTITY_WORLD)
				{
					PickPLWorld(m_sPickInfo);
				}
				else if(ePLType == PLENTITY_OBJECT)
				{
					if(m_sPickInfo.pPLEntity->GetSerialID() == INVALID_SERIAL_ID)
						PickPLWorld(m_sPickInfo);
					else
					{
						CNtlSob *pSobObj = GetNtlSobManager()->GetSobObject(m_sPickInfo.pPLEntity->GetSerialID()); 
						if(pSobObj == NULL || pSobObj->GetFlags() & SLFLAG_NOT_TARGETED)
							PickPLWorld(m_sPickInfo);
						else
                        {
							PickPLObject(m_sPickInfo, FALSE);

                            if(pData->uiFlags & MK_CONTROL)
                            {
                                GetDboGlobal()->GetGamePacketGenerator()->SendPartyShareTargetReq((RwUInt8)Logic_GetShareTargetBlankSlot(), m_sPickInfo.pPLEntity->GetSerialID());
                            }
                        }
					}
				}
				else
				{
					PickPLObject(m_sPickInfo, FALSE);

                    if(pData->uiFlags & MK_CONTROL)
                    {
                        GetDboGlobal()->GetGamePacketGenerator()->SendPartyShareTargetReq((RwUInt8)Logic_GetShareTargetBlankSlot(), m_sPickInfo.pPLEntity->GetSerialID());
                    }
				}
			}
		}
	}
}

void CAvatarController::MouseUpRBtnHandler(const SMouseData *pData)
{
	GetNtlGameCameraManager()->UserCameraControlEnable(FALSE);

    if(Logic_IsDirectMode(GetNtlSLGlobal()->GetSobAvatar()))
        return;

	RwInt32 iDeltaMouseX = abs(pData->iPosX - m_iOldRButtonMouseX);
	RwInt32 iDeltaMouseY = abs(pData->iPosY - m_iOldRButtonMouseY);

	// 왼쪽 버튼이 눌려 있지 않으면?
	if(!m_bLButtonMoveFlag && !m_bAutoRun)
	{
		if(m_uiCurMoveFlags != NTL_MOVE_NONE && GetNtlWorldConcept()->IsEnableAction(E_ACTION_MOVE))
		{
			if(m_uiCurMoveFlags == NTL_MOVE_B && m_bRButtonMoveFlag)
			{
				CNtlSobAvatar *pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();
				RwV3d vDir = pSobAvatar->GetDirection();
				RwV3dScale(&vDir, &vDir, -1.0f);
			
				pSobAvatar->SetDirection(&vDir);
			}

			CNtlSLEventGenerator::KeyboardMove(m_uiCurMoveFlags, FALSE, 0.0f);
		}
	}

	// heading을 바꾼다.
	if(m_bChangeHeadingFlag)
	{
		CNtlSobAvatar *pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();
		RwV3d vPos = pSobAvatar->GetPosition();
		RwV3d vDir = pSobAvatar->GetDirection();
		API_GetSLPacketGenerator()->SendChangeHeading(vPos, vDir);
	}

	m_bRButtonMoveFlag			= FALSE;
	m_bChangeHeadingFlag		= FALSE;

	if(pData->uiDownDeltaTime <= 300)
	{
		if(!GetNtlWorldConcept()->IsEnableAction(E_ACTION_PICK_WORLD) && !GetNtlWorldConcept()->IsEnableAction(E_ACTION_PICK_OBJECT))		// Picking이 되지 않는 상태라면, Picking을 실행시키지 않는다.
			return;

		m_sPickInfo.bTerrainExclusion = FALSE;
		m_sPickInfo.bCharacterExclusion = FALSE;
		RwBool bPick = GetSceneManager()->PickWorld(pData->iPosX, pData->iPosY, m_sPickInfo, PICK_TERRAIN_LIMIT, PICK_OBJECT_LIMIT);
		if(bPick)
		{
			if(!pData->bDblClick && iDeltaMouseX < 10 && iDeltaMouseY < 10)
			{
				ENtlPLEntityType ePLType = m_sPickInfo.pPLEntity->GetClassType();
				if(ePLType == PLENTITY_WORLD)
				{
				}
				else if(ePLType == PLENTITY_OBJECT)
				{
					if(m_sPickInfo.pPLEntity->GetSerialID() != INVALID_SERIAL_ID)
						PickPLObject(m_sPickInfo, TRUE);
				}
				else
				{
					PickPLObject(m_sPickInfo, TRUE);
				}
			}
		}
	}
}

void CAvatarController::MouseMoveLBtnHandler(const SMouseData *pData)
{
	GetCursorManager()->ShowMouseCursor(FALSE);

	if(!GetNtlWorldConcept()->IsEnableAction(E_ACTION_CAMERA_CONTROL))
		return;

	RwInt32 iDeltaX = (m_iOldLButtonMouseX - pData->iPosX);
	RwInt32 iDeltaY = (m_iOldLButtonMouseY - pData->iPosY);

	m_iDeltaButtonMouseX += abs(iDeltaX);
	m_iDeltaButtonMouseY += abs(iDeltaY);

	if(!m_bLButtonMoveFlag)
	{
		m_iOldLButtonMouseX = pData->iPosX;
		m_iOldLButtonMouseY = pData->iPosY;
		m_bLButtonMoveFlag = TRUE;
		return;
	}

	CalcCameraRotateSensitivity(pData->iPosX, pData->iPosY, iDeltaX, iDeltaY);

	m_iOldLButtonMouseX = pData->iPosX;
	m_iOldLButtonMouseY = pData->iPosY;

	m_bLButtonMoveFlag = TRUE;

	GetNtlGameCameraManager()->UserCameraControlEnable(TRUE);
}

void CAvatarController::MouseMoveRBtnHandler(const SMouseData *pData)
{
	GetCursorManager()->ShowMouseCursor(FALSE);

	RwBool bActiveWorldConcept = GetNtlWorldConcept()->IsEnableAction(E_ACTION_CHANGE_HEADING);

	RwBool bChangeHeadingEnable = FALSE;

	if(m_bAutoRun)
	{
		if(!bActiveWorldConcept)
		{
			SetAutoRun(FALSE);
		}

		if(bActiveWorldConcept && !Logic_IsVehicleDriver(GetNtlSLGlobal()->GetSobAvatar()))
		{
			bChangeHeadingEnable = TRUE;
		}
	}
	else
	{
		if(bActiveWorldConcept && !m_bRButtonMoveFlag)
		{
			m_iOldRButtonMouseX = pData->iPosX;
			m_iOldRButtonMouseY = pData->iPosY;
			m_bRButtonMoveFlag = TRUE;

			if(m_uiCurMoveFlags != NTL_MOVE_NONE)
			{
				if ( !Logic_IsVehicleDriver( GetNtlSLGlobal()->GetSobAvatar() ) )
				{
					RwReal fAngleY = CalcAvatarStraightRotate(m_uiCurMoveFlags);
					RwUInt8 uiMoveFlags = ConvertStraightMoveFlags(m_uiCurMoveFlags);
					CNtlSLEventGenerator::KeyboardMove(uiMoveFlags, TRUE, fAngleY);
				}
				else
				{
					CNtlSLEventGenerator::KeyboardMove(m_uiCurMoveFlags, FALSE, 0.f);
				}
			}

			return;
		}

		if(bActiveWorldConcept && m_uiCurMoveFlags != NTL_MOVE_NONE)
			bChangeHeadingEnable = TRUE;
	}

	RwInt32 iDeltaX = ((RwInt32)m_iOldRButtonMouseX - pData->iPosX);
	RwInt32 iDeltaY = ((RwInt32)m_iOldRButtonMouseY - pData->iPosY);

	if(bChangeHeadingEnable)
	{
		if(!m_bChangeHeadingFlag)
		{
			m_bChangeHeadingFlag		= TRUE;
			m_fChangeHeadingUpdateTime	= 0.0f;
			m_fChangeHeadingDeltaTime	= 0.0f;
			m_fChangeHeadingDeltaAngle	= 0.0f;
		}

		m_fChangeHeadingDeltaAngle += iDeltaX *NTL_MOUSE_SENSITIVITY_SCALE;

		RwBool bSuccess = TRUE;
		CNtlSLEventGenerator::InputChagneHeading(FALSE, (float)iDeltaX *NTL_MOUSE_SENSITIVITY_SCALE, bSuccess);
		
	}

	CalcCameraRotateSensitivity(pData->iPosX, pData->iPosY, iDeltaX, iDeltaY);

	m_iOldRButtonMouseX = pData->iPosX;
	m_iOldRButtonMouseY = pData->iPosY;

	m_bRButtonMoveFlag = TRUE;

	GetNtlGameCameraManager()->UserCameraControlEnable(TRUE);
}

void CAvatarController::MouseMoveNotBtnHandler(const SMouseData *pData)
{
	GetCursorManager()->ShowMouseCursor(TRUE);
}

void CAvatarController::CalcCameraRotateToCharBack(void)
{
	if(!GetNtlWorldConcept()->IsEnableAction(E_ACTION_CAMERA_CONTROL))
		return;

	if(m_bAutoRun)
	{
		if(m_uiCurMoveFlags != NTL_MOVE_NONE || m_bLButtonMoveFlag || m_bRButtonMoveFlag)
		{
			m_eRotFlag = CAMERA_ROTATE_FLAG_NONE;
			return;
		}
	}
	else
	{
		if(m_uiCurMoveFlags == NTL_MOVE_NONE || m_bLButtonMoveFlag || m_bRButtonMoveFlag)
		{
			m_eRotFlag = CAMERA_ROTATE_FLAG_NONE;
			return;
		}
	}

	if(m_eRotFlag == CAMERA_ROTATE_FLAG_NONE)
	{
		m_eRotFlag = CAMERA_ROTATE_FLAG_ADJUST;
	}
	else if(m_eRotFlag == CAMERA_ROTATE_FLAG_ACCORD)
	{
		CNtlSobAvatar *pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();
		RwV3d vDir = pSobAvatar->GetDirection();
		RwReal fAngleY = CNtlMath::LineToAngleY(&vDir);

		GetNtlGameCameraManager()->SetYaw(fAngleY);
	}
}


void CAvatarController::UpdateCameraRotateToCharBack(RwReal fElapsed)
{
	if(m_eRotFlag != CAMERA_ROTATE_FLAG_ADJUST)
		return;

	if(!GetNtlWorldConcept()->IsEnableAction(E_ACTION_CAMERA_CONTROL))
		return;

    CNtlSobAvatar *pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();

    if(Logic_GetActorStateId(pSobAvatar) == NTL_FSMSID_SCOUTER)
        return;

	RwReal fDeltaAngleY = 180.0f * fElapsed;

	
	RwV3d vDir = pSobAvatar->GetDirection();
	RwReal fAngleY = CNtlMath::LineToAngleY(&vDir);
	RwReal fCovAngleY = GetNtlGameCameraManager()->GetYaw();

	if(fAngleY < 0.0f)
		fAngleY += 360.0f;
	if(fCovAngleY >= 360.0f)
		fCovAngleY -= 360.0f;

	//  회전은 시계 반대 방향 회전이다.
	//  카메라 angle 에서 character angle 뺀다.
	RwReal fSubAngleY = fCovAngleY - fAngleY;

	if(fSubAngleY > 0.0f)
		if(fSubAngleY > 180.0f) // 각도를 증가시킨다.
			fSubAngleY = 360.0f - fSubAngleY;
		else // 각도를 감소시킨다.
			fSubAngleY = -fSubAngleY; //(시계 반대 방향이므로 빼줘야 한다.)
	else
		if(fSubAngleY > -180.0f) // 각도를 증가시킨다.
			fSubAngleY = fabs(fSubAngleY); 
		else // 각도를 감소시킨다.
			fSubAngleY = -(360.0f - fabs(fSubAngleY)); // 시계 반대 방향이므로 빼줘야 한다.

	if(fabs(fSubAngleY) < fDeltaAngleY)
	{
		CNtlSobAvatar *pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();
		RwV3d vDir = pSobAvatar->GetDirection();
		RwReal fAngleY = CNtlMath::LineToAngleY(&vDir);

		GetNtlGameCameraManager()->SetYaw(fAngleY);

		m_eRotFlag = CAMERA_ROTATE_FLAG_ACCORD;
	}
	else
	{
		if(fSubAngleY > 0.0f)
			GetNtlGameCameraManager()->AddYaw(fDeltaAngleY);
		else
			GetNtlGameCameraManager()->AddYaw(-fDeltaAngleY);
	}
}

void CAvatarController::UpdateAutoRun(RwReal fElapsed)
{
	if(m_bAutoRun)
	{
		CNtlSobAvatar *pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();

		RwBool bMoveState = 0;

		RwV3d vPos;
		RwV3d vDir;
		RwReal fSpeed;


		if ( !Logic_IsVehicleDriver( pSobAvatar ) )
		{
			bMoveState = FSMUtil_IsMove(pSobAvatar->GetCurrentState());

			vPos	= pSobAvatar->GetPosition();
			vDir	= pSobAvatar->GetDirection();
			fSpeed	= Logic_GetFrontRunSpeed(pSobAvatar);
		}
		else
		{
			CNtlSobVehicle* pVehicleObj = (CNtlSobVehicle*)GetNtlSobManager()->GetSobObject( pSobAvatar->GetVehicleID() );

			if ( pVehicleObj )
			{
				bMoveState = FSMUtil_IsMove(pVehicleObj->GetCurrentState());

				vPos	= pVehicleObj->GetPosition();
				vDir	= pVehicleObj->GetDirection();
				fSpeed	= Logic_GetFrontRunSpeed(pVehicleObj);
			}
			else
			{
				bMoveState = FSMUtil_IsMove(pSobAvatar->GetCurrentState());

				vPos	= pSobAvatar->GetPosition();
				vDir	= pSobAvatar->GetDirection();
				fSpeed	= Logic_GetFrontRunSpeed(pSobAvatar);
			}
		}

		if(!bMoveState)
		{
			SetAutoRun(FALSE);
			return;
		}

		if ( !Logic_IsVehicleDriver(GetNtlSLGlobal()->GetSobAvatar()) )
		{
			//keyboard 이동을 처리한다
			CNtlVector vHeading, vDest;
			NtlGetDestination_Keyboard(vDir.x, vDir.z, fSpeed, vPos.x, vPos.y, vPos.z, m_uiCurMoveFlags, (DWORD)(fElapsed*1000.f), 1.0f, &vHeading, &vDest);

			RwV3d vNewDir;
			CNtlMath::MathRwV3dAssign(&vNewDir, vHeading.x, 0.0f, vHeading.z); 

			pSobAvatar->SetDirection(&vNewDir);

			if(m_uiCurMoveFlags == NTL_MOVE_TURN_L || m_uiCurMoveFlags == NTL_MOVE_TURN_R)
			{
				RwReal fAngleY = CNtlMath::LineToAngleY(&vNewDir);
				GetNtlGameCameraManager()->SetYaw(fAngleY);
//				m_eRotFlag = CAMERA_ROTATE_FLAG_ACCORD;
			}
		}
		else
		{
			CNtlSobVehicle* pSobVehicle = (CNtlSobVehicle*) GetNtlSobManager()->GetSobObject(pSobAvatar->GetVehicleID());

			if ( pSobVehicle )
			{
				RwUInt8 byMoveFlags = 0;

				if ( NTL_MOVE_TURN_L == m_uiCurMoveFlags )
				{
					byMoveFlags = NTL_MOVE_F_TURN_L;
				}
				else if ( NTL_MOVE_TURN_R == m_uiCurMoveFlags )
				{
					byMoveFlags = NTL_MOVE_F_TURN_R;
				}
				else
				{
					byMoveFlags = NTL_MOVE_F;
				}

				CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(GetNtlSLGlobal()->GetSobAvatar()->GetBehaviorData());
				SMoveStuff* pMoveStuff = pBeData->GetMoveStuff();
				if ( pMoveStuff->byMoveFlags != byMoveFlags )
				{
					CNtlSLEventGenerator::KeyboardMove(byMoveFlags, FALSE, 0.f);
				}

				CNtlVector vHeading, vDest;
				NtlGetDestination_Keyboard(vDir.x, vDir.z, fSpeed, vPos.x, vPos.y, vPos.z, byMoveFlags, (DWORD)(fElapsed*1000.f), 0.8f, &vHeading, &vDest);

				RwV3d vNewDir;
				CNtlMath::MathRwV3dAssign(&vNewDir, vHeading.x, 0.0f, vHeading.z); 

				if(m_uiCurMoveFlags == NTL_MOVE_TURN_L || m_uiCurMoveFlags == NTL_MOVE_TURN_R)
				{
					RwReal fAngleY = CNtlMath::LineToAngleY(&vNewDir);
					GetNtlGameCameraManager()->SetYaw(fAngleY);
//					m_eRotFlag = CAMERA_ROTATE_FLAG_ACCORD;
				}
			}
		}
	}
}

int	CAvatarController::MouseWheelHandler(unsigned int uiMouseData)
{
	if(!GetNtlWorldConcept()->IsEnableAction(E_ACTION_CAMERA_CONTROL))
		return TRUE;

	if( m_bSkipMouseWheel )
	{
		m_bSkipMouseWheel = FALSE;
		return TRUE;
	}

	SMouseData *pData = (SMouseData *)uiMouseData;
    //120.f (Wheel
    RwReal fDelta = -pData->zDelta * NTL_ZOOM_INOUT_SENSITIVITY;

    if(GetNtlGameCameraManager()->IsActiveFreeCamera())
    {
        // 프리카메라에서는 속도 조절
        RwReal fSpeed = GetNtlGameCameraManager()->FreeCameraGetMoveSpeed();
        fSpeed = pData->zDelta < 0? max(fSpeed - 0.02f, 0.01f): fSpeed + 0.02f;
        GetNtlGameCameraManager()->FreeCameraSetMoveSpeed(fSpeed);
    }
    else
    {
        GetNtlGameCameraManager()->ZoomInOut(fDelta);
    }

	return TRUE;
}

int CAvatarController::RestoreHandler(void)
{
	return TRUE;
}

int	CAvatarController::MouseMoveHandler(unsigned int uiMouseData)
{
	SMouseData *pData = (SMouseData *)uiMouseData;

	if( pData->uiFlags & MK_LBUTTON && pData->uiFlags & MK_RBUTTON)
		return TRUE;

	if( pData->uiFlags & MK_LBUTTON )
		MouseMoveLBtnHandler(pData);
	else if( pData->uiFlags & MK_RBUTTON)
		MouseMoveRBtnHandler(pData);
	else
		MouseMoveNotBtnHandler(pData);
	
	return TRUE;
}

int	CAvatarController::MouseDownHandler(unsigned int uiMouseData)
{
	GetCursorManager()->SaveCurrentPosition();

	SMouseData *pData = (SMouseData *)uiMouseData;

	if(!pData->bDblClick)
	{
		if(m_bAutoRun)
		{
			if(m_uiCurMoveFlags != NTL_MOVE_NONE)
			{
				SetAutoRun(m_bAutoRun);
			}
		}

		if(pData->chBtnType == MOUSE_LBTN)
		{
			m_iOldLButtonMouseX = pData->iPosX;
			m_iOldLButtonMouseY = pData->iPosY;

			m_iDeltaButtonMouseX = 0;
			m_iDeltaButtonMouseY = 0;

			m_bLButtonMoveFlag = TRUE;
		}
		else if(pData->chBtnType == MOUSE_RBTN)
		{
			m_iOldRButtonMouseX = pData->iPosX;
			m_iOldRButtonMouseY = pData->iPosY;

			m_bRButtonMoveFlag = TRUE;

			if(m_bAutoRun && !Logic_IsVehicleDriver(GetNtlSLGlobal()->GetSobAvatar()) )
			{
				RwReal fYaw = GetNtlGameCameraManager()->GetYaw();
				CNtlSobAvatar *pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();
				pSobAvatar->SetAngleY(fYaw);

				RwV3d vPos = pSobAvatar->GetPosition();
				RwV3d vDir = pSobAvatar->GetDirection();
				API_GetSLPacketGenerator()->SendChangeHeading(vPos, vDir);
			}

			if(m_uiCurMoveFlags != NTL_MOVE_NONE)
			{
				if(m_uiCurMoveFlags != NTL_MOVE_F)
				{
					CNtlSobAvatar *pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();
					if ( !Logic_IsVehicleDriver( pSobAvatar ) )
					{
						RwReal fAngleY = CalcAvatarStraightRotate(m_uiCurMoveFlags);
						RwUInt8 uiMoveFlags = ConvertStraightMoveFlags(m_uiCurMoveFlags);
						CNtlSLEventGenerator::KeyboardMove(uiMoveFlags, TRUE, fAngleY);
					}
					else
					{
						CNtlSLEventGenerator::KeyboardMove(m_uiCurMoveFlags, FALSE, 0.f);
					}
				}
			}
		}

		CalcCameraRotateToCharBack();
	}
	else
	{
		m_bRButtonMoveFlag			= FALSE;
		m_bChangeHeadingFlag		= FALSE;

		if(m_bAutoRun)
		{
			if(m_uiCurMoveFlags != NTL_MOVE_NONE)
				SetAutoRun(FALSE);
		}

		if(pData->chBtnType == MOUSE_RBTN)
		{
            if(Logic_IsDirectMode(GetNtlSLGlobal()->GetSobAvatar()))
                return TRUE;

			if(!GetNtlWorldConcept()->IsEnableAction(E_ACTION_PICK_WORLD) && !GetNtlWorldConcept()->IsEnableAction(E_ACTION_PICK_OBJECT))		// Picking이 되지 않는 상태라면, Picking을 실행시키지 않는다.
				return TRUE;

			m_sPickInfo.bTerrainExclusion	= FALSE;
			m_sPickInfo.bCharacterExclusion = FALSE;
			RwBool bPick = GetSceneManager()->PickWorld(pData->iPosX, pData->iPosY, m_sPickInfo, PICK_TERRAIN_LIMIT, PICK_OBJECT_LIMIT);
			if(bPick)
			{
				ENtlPLEntityType ePLType = m_sPickInfo.pPLEntity->GetClassType();

				if(ePLType == PLENTITY_WORLD || ePLType == PLENTITY_OBJECT)
				{
					if(Logic_IsPassiveDashUsePossible())
					{
						CNtlSLEventGenerator::MouseDashMove(m_sPickInfo.vPickPos, MOUSE_DASH_LIMIT);

						SERIAL_HANDLE hTargetSerialId = GetDboGlobal()->GetTargetSerial();
						if(hTargetSerialId != INVALID_SERIAL_ID)
							CNtlSLEventGenerator::SobAttackMarkRelease(hTargetSerialId);

						SetAutoRun(FALSE);
					}
				}
			}
		}
	}
	
	return TRUE;
}


int	CAvatarController::MouseUpHandler(unsigned int uiMouseData)
{
	NTL_FUNCTION("CAvatarController::MouseUpHandler");

	if(!GetCursorManager()->IsShowCursor())
	{
		GetCursorManager()->ShowMouseCursor(TRUE);
		GetCursorManager()->RestorePosition();
	}

	SMouseData *pData = (SMouseData *)uiMouseData;
	
	if(pData->chBtnType == MOUSE_LBTN)
	{
		MouseUpLBtnHandler(pData);
	}
	else if(pData->chBtnType == MOUSE_RBTN)
	{
		MouseUpRBtnHandler(pData);
	}

	// console debug module
    GetDumpCmdManager()->SetTargetSerial(GetDboGlobal()->GetTargetSerial()); 

	// camera auto rotate를 판정한다.
	CalcCameraRotateToCharBack();

	NTL_RETURN(1);
}

int	CAvatarController::KeyboardDownHandler(unsigned int uiKeyData)
{
	SKeyData* pData = (SKeyData*)uiKeyData;

	if(pData->uiChar & 0xFF)
	{
		if(m_bHTBRPStock)
		{
			RwUInt8 bySetRPStock = (RwUInt8)(pData->uiChar - 48);
			RwUInt32 uiRPStock = Logic_GetRpStock(GetNtlSLGlobal()->GetSobAvatar());
			if(bySetRPStock <= uiRPStock)
			{
				API_GetSLPacketGenerator()->SendHTBRPBallUseReq(bySetRPStock);
				m_bHTBRPStock = FALSE;
			}
		}
	}

	if(pData->uiChar == NTL_KEY_F2)
	{        

	}
	else if(pData->uiChar == NTL_KEY_F3 )
	{
	}
	else if(pData->uiChar == NTL_KEY_F5 )
	{
		if( Logic_IsDevUser() )
			GetDboGlobal()->ToggleDebugDisplay();
	}
#ifndef NDEBUG
	else if(pData->uiChar == NTL_KEY_F8 )
	{
		if(Logic_IsDevUser())
		{
			static RwBool bShowProfilerGui = TRUE;
			CNtlPLEventGenerator::ShowProfilerGui(bShowProfilerGui);
			bShowProfilerGui = !bShowProfilerGui;
		}
	}
#endif
    else if(pData->uiChar == NTL_KEY_F11 ) 
    {
#ifdef _UICONFIG
        GetDboGlobal()->GetDBOUIConfig()->Reflash();
#endif        
		// peessi: 천하제일무도회 대진표 테스트용.
		//if( !CTBGui::m_pInstance->m_pTournamentGui )
		//{
		//	CTBGui::m_pInstance->m_pTournamentGui = NTL_NEW CTBTournamentGui( "BudokaiTournamentGui" );
		//	if( !CTBGui::m_pInstance->m_pTournamentGui->Create() )
		//	{
		//		CTBGui::m_pInstance->m_pTournamentGui->Destroy();
		//		NTL_DELETE( CTBGui::m_pInstance->m_pTournamentGui );
		//	}
		//	else
		//	{
		//		CNtlPLGuiManager::GetInstance()->AddGui( CTBGui::m_pInstance->m_pTournamentGui );
		//		GetDialogManager()->RegistDialog( DIALOG_BUDOKAI_TOURNAMENT, CTBGui::m_pInstance->m_pTournamentGui, &CTBTournamentGui::SwitchDialog );
		//		CTBGui::m_pInstance->m_pTournamentGui->Show( true );
		//	}
		//}
		//else
		//{
		//	if( CTBGui::m_pInstance->m_pTournamentGui )
		//	{	
		//		CTBGui::m_pInstance->m_pTournamentGui->Show( false );
		//		CNtlPLGuiManager::GetInstance()->RemoveGui( CTBGui::m_pInstance->m_pTournamentGui );
		//		GetDialogManager()->UnRegistDialog( DIALOG_BUDOKAI_TOURNAMENT );
		//		CTBGui::m_pInstance->m_pTournamentGui->Destroy();
		//		NTL_DELETE( CTBGui::m_pInstance->m_pTournamentGui );
		//	}
		//}
    } 
	else if( pData->uiChar == NTL_KEY_NUMLOCK )
	{		
		if(Logic_IsDevUser())
		{
			static RwBool bDrawGUI = FALSE;
			CNtlPLGuiManager::GetInstance()->RenderEnable(bDrawGUI);
			bDrawGUI = !bDrawGUI;
		}
	}
	else if( pData->uiChar == NTL_KEY_F9 )
	{
		//Logic_SetActiveRpSkillBonus(DBO_RP_BONUS_TYPE_KNOCKDOWN);		
		//SEND_MSG(g_EventSCS, NULL);
		
	}
	else if( pData->uiChar == NTL_KEY_F10 )
	{		
//		Logic_SetActiveRpSkillBonus(DBO_RP_BONUS_TYPE_RESULT_ABSOLUTE);
	}
	else if( pData->uiChar == NTL_KEY_F11 )
	{
//		Logic_SetActiveRpSkillBonus(DBO_RP_BONUS_TYPE_RESULT_PERCENT);		
	}
	else if( pData->uiChar == NTL_KEY_F12 )
	{
//		Logic_SetActiveRpSkillBonus(DBO_RP_BONUS_TYPE_KEEP_TIME_PLUS);

		
	}
	
	return TRUE;
}

int CAvatarController::KeyboardUpHandler(unsigned int uiKeyData)
{
	return TRUE;
}


void CAvatarController::CreateMoveMark(const RwV3d *pPos)
{
//	DeleteMoveMark();

	if(m_pMoveMark == NULL)
	{
        m_pMoveMark = GetSceneManager()->CreateEntity(PLENTITY_EFFECT, NTL_VID_MARK_COMMON_MOVE);
	}

	m_pMoveMark->SetPosition(pPos);

	// 사운드 
	RwInt32 nSoundIndex = NtlRandomNumber(0, 3);
	Logic_PlayGUISound((char*)(SOUND_SL_GROUND_PICKING[nSoundIndex]));
}

void CAvatarController::DeleteMoveMark(void)
{
	if(m_pMoveMark)
	{
		GetSceneManager()->DeleteEntity(m_pMoveMark);
		m_pMoveMark = NULL;
		m_fMoveMarkTime = 0.0f;
	}
}

void CAvatarController::UpdateMoveMark(RwReal fElapsed)
{
	// move mark를 잠깐 띄웠다가 없앤다.
	if(m_pMoveMark)
	{
		m_fMoveMarkTime += fElapsed;
		if(m_fMoveMarkTime > 3.0f)
			DeleteMoveMark();
	}
}

void CAvatarController::UpdateChangeHeading(RwReal fElapsed)
{
	if(!GetNtlWorldConcept()->IsEnableAction(E_ACTION_CHANGE_HEADING))
		return;

	// change heading
	if(m_bChangeHeadingFlag)
	{
		CNtlSobAvatar *pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();

		m_fChangeHeadingDeltaTime += fElapsed;
		m_fChangeHeadingUpdateTime += fElapsed;

		if(m_fChangeHeadingDeltaTime >= NTL_HEADING_DELTA_TIME)
		{
			if((RwReal)fabs(m_fChangeHeadingDeltaAngle) > NTL_HEADING_DELTA_ANGLE)
			{
				RwV3d vPos = pSobAvatar->GetPosition();
				RwV3d vDir = pSobAvatar->GetDirection();
				API_GetSLPacketGenerator()->SendChangeHeading(vPos, vDir);

				m_fChangeHeadingDeltaAngle = 0.0f;
				m_fChangeHeadingUpdateTime = 0.0f;
			}

			m_fChangeHeadingDeltaTime = 0.0f;
		}

		if(m_fChangeHeadingUpdateTime >= NTL_HEADING_UPDATE_TIME)
		{
			RwV3d vPos = pSobAvatar->GetPosition();
			RwV3d vDir = pSobAvatar->GetDirection();
			API_GetSLPacketGenerator()->SendChangeHeading(vPos, vDir);

			m_fChangeHeadingDeltaAngle = 0.0f;
			m_fChangeHeadingUpdateTime = 0.0f;
		}
	}

	if(m_bAutoRun && m_bAutoRunKeyboardChangeHeading )
	{
		m_fChangeHeadingUpdateTime += fElapsed;

		if(m_fChangeHeadingUpdateTime >= NTL_HEADING_DELTA_TIME)
		{
			if ( !Logic_IsVehicleDriver( GetNtlSLGlobal()->GetSobAvatar() ) )
			{
				CNtlSobAvatar *pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();
				RwV3d vPos = pSobAvatar->GetPosition();
				RwV3d vDir = pSobAvatar->GetDirection();
				API_GetSLPacketGenerator()->SendChangeHeading(vPos, vDir);
			}

			m_fChangeHeadingUpdateTime = 0.0f;
		}
	}
}

void CAvatarController::UpdatePickFocus(RwReal fElapsed)
{
	m_fPickTime += fElapsed;	

	if( CNtlPLGuiManager::GetInstance()->IsGuiFocus() )
	{
		m_sPickInfo_for_Cursor.bWorldPick	= false;
		m_sPickInfo_for_Cursor.pSobObj		= NULL;

		GetCursorManager()->Update(fElapsed, &m_sPickInfo_for_Cursor);
	}
	else
	{
		if(m_fPickTime < PICK_FOCUS_TIME)
			return;

		if(!GetNtlWorldConcept()->IsEnableAction(E_ACTION_PICK_OBJECT))
			return;

		m_sPickInfo.bTerrainExclusion	= FALSE;
		m_sPickInfo.bCharacterExclusion = FALSE;

		CNtlSob* pSobObj = NULL;
		RwBool bWorldPick = GetSceneManager()->PickWorld(CMouse::m_nX, CMouse::m_nY, m_sPickInfo, PICK_TERRAIN_LIMIT, PICK_OBJECT_LIMIT);

		if( bWorldPick )
		{
			RwUInt32 uiSerialId = m_sPickInfo.pPLEntity->GetSerialID();
			pSobObj = GetNtlSobManager()->GetSobObject(uiSerialId); 

			if( pSobObj )
			{
				if(m_hFocusSerialId != INVALID_SERIAL_ID && m_hFocusSerialId != uiSerialId)
					CNtlSLEventGenerator::SobLostFocus(m_hFocusSerialId);

				if( !(pSobObj->GetFlags() & SLFLAG_NOT_HIGHLIGHTED) )
				{
					if(m_hFocusSerialId != uiSerialId)
					{
						m_hFocusSerialId = uiSerialId;
						CNtlSLEventGenerator::SobGotFocus(m_hFocusSerialId);
					}
				}
			}
			else
			{
				if(m_hFocusSerialId != INVALID_SERIAL_ID)
				{
					CNtlSLEventGenerator::SobLostFocus(m_hFocusSerialId);
                    m_hFocusSerialId = INVALID_SERIAL_ID;
				}
			}
		}
		else
		{
			if(m_hFocusSerialId != INVALID_SERIAL_ID)
			{
				CNtlSLEventGenerator::SobLostFocus(m_hFocusSerialId);
                m_hFocusSerialId = INVALID_SERIAL_ID;
			}
		}

		m_sPickInfo_for_Cursor.bWorldPick	= bWorldPick;
		m_sPickInfo_for_Cursor.pSobObj		= pSobObj;

		GetCursorManager()->Update(fElapsed, &m_sPickInfo_for_Cursor);
		m_fPickTime = 0;
	}		
}

int CAvatarController::ActionMapKeyboardMove(unsigned int uiMoveFlags)
{
	if(uiMoveFlags != 0)
	{ 
		if(!GetNtlWorldConcept()->IsEnableAction(E_ACTION_MOVE))
			return TRUE;
	}

    if(Logic_IsDirectMode(GetNtlSLGlobal()->GetSobAvatar()))
        return TRUE;

	if( !Logic_CanKeybaordInput_in_Tutorial( ETL_KEYBOARD_INPUT_TYPE_MOVE ) )
		return FALSE;

    // 프리 카메라가 활성화 되어 있는지 확인한다.
    if(GetNtlGameCameraManager()->IsActiveFreeCamera())
    {
        GetNtlGameCameraManager()->FreeCameraMove(uiMoveFlags);
        return TRUE;
    }

    // 대쉬중에는 사용 불가
    if(Logic_GetActorStateId(GetNtlSLGlobal()->GetSobAvatar()) == NTL_FSMSID_DASH)
    {
        m_uiCurMoveFlags = 0;
        CNtlSLEventGenerator::KeyboardMoveStop();
        return TRUE;
    }

	if(m_bAutoRun)
	{
		if(m_bLButtonMoveFlag || m_bRButtonMoveFlag)
		{
			SetAutoRun(FALSE);
		}
		else
		{
			if( !(uiMoveFlags == NTL_MOVE_NONE || uiMoveFlags == NTL_MOVE_TURN_L || uiMoveFlags == NTL_MOVE_TURN_R) )
			{
				SetAutoRun(FALSE);
			}
			else
			{
				if(uiMoveFlags == NTL_MOVE_NONE)
				{
					if ( !Logic_IsVehicleDriver( GetNtlSLGlobal()->GetSobAvatar() ) )
					{
						CNtlSobAvatar *pSobAvatar = GetNtlSLGlobal()->GetSobAvatar(); 
						RwV3d vPos = pSobAvatar->GetPosition();
						RwV3d vDir = pSobAvatar->GetDirection();
						API_GetSLPacketGenerator()->SendChangeHeading(vPos, vDir);
					}

					m_bAutoRunKeyboardChangeHeading = FALSE;
				}
				else
				{
					m_bAutoRunKeyboardChangeHeading = TRUE;
					m_fChangeHeadingUpdateTime	= 0.0f;
				}

				m_uiCurMoveFlags = (RwUInt8)uiMoveFlags;
				return TRUE;
			}
		}
	}

	RwUInt32 uiOldMoveFlags = m_uiCurMoveFlags;

	m_uiCurMoveFlags = (RwUInt8)uiMoveFlags;
	if(uiMoveFlags == 0)
	{
		CNtlSLEventGenerator::KeyboardMoveStop();
	}
	else 
	{
		RwBool bAvatarAngleChange = FALSE;
		RwReal fAngleY = 0.0f;
		if(m_bRButtonMoveFlag)
		{
			if ( !Logic_IsVehicleDriver( GetNtlSLGlobal()->GetSobAvatar() ) )
			{
				bAvatarAngleChange = TRUE;
				fAngleY = CalcAvatarStraightRotate(uiMoveFlags);
				uiMoveFlags = ConvertStraightMoveFlags((RwUInt8)uiMoveFlags);
			}
		}

		CNtlSLEventGenerator::KeyboardMove((RwUInt8)uiMoveFlags, bAvatarAngleChange, fAngleY);

		if(m_bRButtonMoveFlag)
		{
			CNtlSobAvatar *pSobAvatar = GetNtlSLGlobal()->GetSobAvatar(); 
			if ( !Logic_IsVehicleDriver( GetNtlSLGlobal()->GetSobAvatar() ) )
			{
				RwV3d vPos = pSobAvatar->GetPosition();
				RwV3d vDir = pSobAvatar->GetDirection();
				API_GetSLPacketGenerator()->SendChangeHeading(vPos, vDir);
			}
		}

		//SERIAL_HANDLE hTargetSerialId = GetDboGlobal()->GetTargetSerial();
		//if(hTargetSerialId != INVALID_SERIAL_ID)
		//	CNtlSLEventGenerator::SobAttackMarkRelease(hTargetSerialId);
		
		DeleteMoveMark();
	}
	
	// console debug module
	GetDumpCmdManager()->SetTargetSerial(GetDboGlobal()->GetTargetSerial());

	// camera auto rotate를 판정한다.
	if(!m_bRButtonMoveFlag)
	{
		if(uiOldMoveFlags != uiMoveFlags)
			CalcCameraRotateToCharBack();
	}
	
	return 1;
}

int CAvatarController::ActionAutoRun()
{
	if(!GetNtlWorldConcept()->IsEnableAction(E_ACTION_MOVE))
		return TRUE;

	if( !Logic_CanKeybaordInput_in_Tutorial( ETL_KEYBOARD_INPUT_TYPE_AUTORUN ) )
		return FALSE;

	CNtlSobAvatar *pSobAvatar = GetNtlSLGlobal()->GetSobAvatar(); 
	if(!pSobAvatar->CanMove())
		return TRUE;

	if(m_uiCurMoveFlags != NTL_MOVE_NONE)
		return TRUE;

    if(Logic_IsDirectMode(GetNtlSLGlobal()->GetSobAvatar()))
        return TRUE;

	RwReal fYaw = GetNtlGameCameraManager()->GetYaw();
	
	pSobAvatar->SetAngleY(fYaw);
	
	CNtlSLEventGenerator::KeyboardMove(NTL_MOVE_F, FALSE, 0.0f);

	SERIAL_HANDLE hTargetSerialId = GetDboGlobal()->GetTargetSerial();
	if(hTargetSerialId != INVALID_SERIAL_ID)
		CNtlSLEventGenerator::SobAttackMarkRelease(hTargetSerialId);
		
	DeleteMoveMark();
	
	// console debug module
	GetDumpCmdManager()->SetTargetSerial(GetDboGlobal()->GetTargetSerial());

	m_eRotFlag = CAMERA_ROTATE_FLAG_ADJUST;

	SetAutoRun(TRUE);

    return 1;
}

int CAvatarController::ActionMapKeyboardDashMove(unsigned int uiMoveFlags)
{
	if(!GetNtlWorldConcept()->IsEnableAction(E_ACTION_MOVE))
        return TRUE;

	if(!Logic_IsPassiveDashUsePossible())
        return TRUE;

    // 대쉬중에는 사용 불가
    if(Logic_GetActorStateId(GetNtlSLGlobal()->GetSobAvatar()) == NTL_FSMSID_DASH)
        return TRUE;

	if(m_bRButtonMoveFlag)
	{
		CNtlSLEventGenerator::KeyboardDashMove(NTL_MOVE_DASH_F);
	}
	else
	{
		CNtlSLEventGenerator::KeyboardDashMove(uiMoveFlags);
	}

	SERIAL_HANDLE hTargetSerialId = GetDboGlobal()->GetTargetSerial();
	if(hTargetSerialId != INVALID_SERIAL_ID)
		CNtlSLEventGenerator::SobAttackMarkRelease(hTargetSerialId);

	SetAutoRun(FALSE);

	return 1;
}

int CAvatarController::ActionAvatarSelect(void)
{
    if(!GetNtlWorldConcept()->IsEnableAction(E_ACTION_MOVE))
        return TRUE;

    if( !Logic_CanKeybaordInput_in_Tutorial( ETL_KEYBOARD_INPUT_TYPE_TARGETING ) )
        return FALSE;

    if(Logic_GetActorStateFlags(GetNtlSLGlobal()->GetSobAvatar()) & NTL_FSMSF_NOT_INPUT)
        return FALSE;

	CNtlSobAvatar *pSobAvatar = GetNtlSLGlobal()->GetSobAvatar(); 
	if(pSobAvatar)
	{
		SERIAL_HANDLE hSerialId = pSobAvatar->GetSerialID();
		if(hSerialId != GetDboGlobal()->GetTargetSerial())
		{
			if(GetDboGlobal()->GetTargetSerial() != INVALID_SERIAL_ID)
				CNtlSLEventGenerator::SobTargetSelectRelease(GetDboGlobal()->GetTargetSerial());

			GetDboGlobal()->SetTargetSerial(hSerialId);
			CNtlSLEventGenerator::SobTargetSelect(hSerialId);

			API_GetSLPacketGenerator()->SendCharTargetSelect(hSerialId);
		}
	}

	return 1;
}

int CAvatarController::ActionJump(unsigned int uiMoveFlags)
{
	if(!GetNtlWorldConcept()->IsEnableAction(E_ACTION_MOVE))
		return TRUE;

    // 거대화 변신상태에서는 점프 안됨
    CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
    if(Logic_IsTransformGreatNamek(pAvatar) || Logic_IsTransformSpinAttack(pAvatar))
        return TRUE;

	if( !Logic_CanKeybaordInput_in_Tutorial( ETL_KEYBOARD_INPUT_TYPE_MOVE ) )
		return FALSE;


	if ( Logic_IsVehicleDriver( pAvatar ) )
	{
		API_GetSLPacketGenerator()->SendVehicleStuntNfy();

		CNtlSLEventGenerator::SobVehicleStunt( pAvatar->GetSerialID() );
	}
	else
	{
		if(!m_bLButtonMoveFlag && m_bRButtonMoveFlag)
		{
			uiMoveFlags = ConvertStraightMoveFlags((RwUInt8)uiMoveFlags);
		}

		CNtlSLEventGenerator::ActionMapJump(uiMoveFlags);
	}

	SERIAL_HANDLE hTargetSerialId = GetDboGlobal()->GetTargetSerial();
	if(hTargetSerialId != INVALID_SERIAL_ID)
		CNtlSLEventGenerator::SobAttackMarkRelease(hTargetSerialId);

	return 1;
}

int CAvatarController::ActionCharging(unsigned int uiCharging)
{
	if(uiCharging)
	{
		if(!GetNtlWorldConcept()->IsEnableAction(E_ACTION_CHARGING))
			return TRUE;
	}

	if(!Logic_IsPassiveChargingUsePossible())
	{
		return 1;
	}

	if(uiCharging)
		CNtlSLEventGenerator::ActionMapCharging(TRUE);
	else
		CNtlSLEventGenerator::ActionMapCharging(FALSE);

	SERIAL_HANDLE hTargetSerialId = GetDboGlobal()->GetTargetSerial();
	if(hTargetSerialId != INVALID_SERIAL_ID)
		CNtlSLEventGenerator::SobAttackMarkRelease(hTargetSerialId);

	return 1;
}

int CAvatarController::ActionBlocking(unsigned int uiBlocking)
{
	if(uiBlocking)
	{
		if(!GetNtlWorldConcept()->IsEnableAction(E_ACTION_CHARGING))
			return TRUE;
	}

	if(!Logic_IsPassiveBlockingUsePossible())
	{
		return 1;
	}

	if(uiBlocking)
	{
		if(!Logic_IsPassiveBlockingUsePossibleCoolTime()) // 쿨타임체크.
		{
			GetAlarmManager()->AlarmMessage( DST_BLOCK_MODE_COOLTIME );
			return 1;
		}
	
		CNtlSLEventGenerator::ActionMapBlocking(TRUE);
	}
	else
		CNtlSLEventGenerator::ActionMapBlocking(FALSE);

	SERIAL_HANDLE hTargetSerialId = GetDboGlobal()->GetTargetSerial();
	if(hTargetSerialId != INVALID_SERIAL_ID)
		CNtlSLEventGenerator::SobAttackMarkRelease(hTargetSerialId);

	return 1;
}

int CAvatarController::ActionSitAndStand(void)
{
	if(!GetNtlWorldConcept()->IsEnableAction(E_ACTION_MOVE))
		return TRUE;

	if( !Logic_CanKeybaordInput_in_Tutorial( ETL_KEYBOARD_INPUT_TYPE_MOVE ) )
		return FALSE;

	CNtlSLEventGenerator::ActionMapSitAndStand();

	SERIAL_HANDLE hTargetSerialId = GetDboGlobal()->GetTargetSerial();
	if(hTargetSerialId != INVALID_SERIAL_ID)
		CNtlSLEventGenerator::SobAttackMarkRelease(hTargetSerialId);
	
	return 1;
}

int CAvatarController::ActionLooting(void)
{
	if(!GetNtlWorldConcept()->IsEnableAction(E_ACTION_LOOTING))
		return TRUE;

	if( m_fKeyLootingTime >= KEY_LOOTING_TIME )
	{
        RwV3d vPos = GetNtlSLGlobal()->GetSobAvatar()->GetPosition();
		CNtlSob* pSobObject = GetNtlSobManager()->GetSobNearWorldItem(vPos);

		Logic_WorldItemLooting( pSobObject );
		
		m_fKeyLootingTime = 0.0f;
	}

	return 1;
}

int CAvatarController::ActionAutoTarget(void)
{
	if(!GetNtlWorldConcept()->IsEnableAction(E_ACTION_TARGETTING))
		return TRUE;

	if( !Logic_CanKeybaordInput_in_Tutorial( ETL_KEYBOARD_INPUT_TYPE_TARGETING ) )
		return FALSE;

    if(Logic_GetActorStateFlags(GetNtlSLGlobal()->GetSobAvatar()) & NTL_FSMSF_NOT_INPUT)
        return FALSE;

	CNtlSobActor *pSobActor = Logic_FindNearEnemyFromAvatarActor();
	if(pSobActor)
	{
		SERIAL_HANDLE hTargetSerialId = GetDboGlobal()->GetTargetSerial();
		if(pSobActor->GetSerialID() != hTargetSerialId)
		{
			if(hTargetSerialId != INVALID_SERIAL_ID)
				CNtlSLEventGenerator::SobTargetSelectRelease(hTargetSerialId);
		}

		GetDboGlobal()->SetTargetSerial(pSobActor->GetSerialID());
		CNtlSLEventGenerator::SobTargetSelect(pSobActor->GetSerialID());

		API_GetSLPacketGenerator()->SendCharTargetSelect(pSobActor->GetSerialID());
	}

	return 1;
}

int CAvatarController::ActionAutoAttack(void)
{
	if(!GetNtlWorldConcept()->IsEnableAction(E_ACTION_ATTACK))
		return TRUE;

	if(m_uiCurMoveFlags != NTL_MOVE_NONE)
		return TRUE;

    if(Logic_IsDirectMode(GetNtlSLGlobal()->GetSobAvatar()))
        return TRUE;

	SERIAL_HANDLE hTargetSerialId = GetDboGlobal()->GetTargetSerial();
	if(hTargetSerialId != INVALID_SERIAL_ID)
	{
		CNtlSob *pTarSobObj = GetNtlSobManager()->GetSobObject(hTargetSerialId);
		if(pTarSobObj)
		{
			RwBool bEnemy = Logic_IsEnemyTargetFromAvatarActor(hTargetSerialId);

			if(bEnemy)
			{
				if ( pTarSobObj->CanHurted() &&
					 !Logic_isEnemyTargetOcclusion(hTargetSerialId) &&
					 FALSE == Logic_IsVehicleDriver( GetNtlSLGlobal()->GetSobAvatar() ) )
				{
					CNtlSLEventGenerator::SobAttackSelect(hTargetSerialId);
				}
			}
			else
			{
				API_GetSLPacketGenerator()->SendCrossFireReq(hTargetSerialId);
			}
		}
	}

	return 1;
}

int CAvatarController::ActionAutoFollow(void)
{
	// auto follow
	SERIAL_HANDLE hTargetSerialId = GetDboGlobal()->GetTargetSerial();
	if(hTargetSerialId == INVALID_SERIAL_ID)
	{
		return 1;
	}

	if(hTargetSerialId == GetNtlSLGlobal()->GetSobAvatar()->GetSerialID())
	{
		return 1;
	}

	CNtlSob *pSobObj = GetNtlSobManager()->GetSobObject(hTargetSerialId);

	if(pSobObj == NULL)
	{
		return 1;
	}

	if( !(pSobObj->GetFlags() & SLFLAG_CAN_AUTO_FOLLOW_TARGET) )
	{
		return 1;
	}

	CNtlSLEventGenerator::ActionMapAutoFollow(hTargetSerialId);

	return 1;
}

int CAvatarController::ActionPartySelect(unsigned int uiIdx)
{
	if( !Logic_CanKeybaordInput_in_Tutorial( ETL_KEYBOARD_INPUT_TYPE_TARGETING ) )
		return FALSE;

	if(GetNtlDTCinematicManager()->IsRunning())
		return TRUE;

	CNtlSobAvatar *pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if(pSobAvatar)
	{
		CNtlParty *pParty = pSobAvatar->GetParty();

		// 아바타 또한 파티 구조체에 파티 가입 순서에 따라 포함된다
		if( pParty->GetAvatarIndex() <= (RwInt32)uiIdx )
			++uiIdx;

		sPartyMember* pMember = reinterpret_cast<sPartyMember*>( pParty->GetMemberbyIndex(uiIdx) );
		if(pMember)
		{
			SERIAL_HANDLE hTargetSerialId = GetDboGlobal()->GetTargetSerial();
			if(pMember->hSerial != hTargetSerialId)
			{
				CNtlSob *pSobObj = GetNtlSobManager()->GetSobObject(pMember->hSerial);
				if(pSobObj)
				{
					if(hTargetSerialId != INVALID_SERIAL_ID)
						CNtlSLEventGenerator::SobTargetSelectRelease(hTargetSerialId);

					GetDboGlobal()->SetTargetSerial(pMember->hSerial);
					CNtlSLEventGenerator::SobTargetSelect(pMember->hSerial);

					API_GetSLPacketGenerator()->SendCharTargetSelect(pMember->hSerial);
				}
			}
		}
	}

	return 1;
}

int CAvatarController::ActionScouterUse( void ) 
{
	if( !Logic_CanKeybaordInput_in_Tutorial( ETL_KEYBOARD_INPUT_TYPE_SCOUTER_MODE ) )
		return FALSE;

    CNtlSLEventGenerator::ActionMapScouterUse();

    return TRUE;
}


void CAvatarController::SetAutoRun(RwBool bAutoRun)
{
	if(m_bAutoRun != bAutoRun)
	{
		Logic_SetAutoRun(bAutoRun);
	}

	m_bAutoRun = bAutoRun;
}

int CAvatarController::ActionPartyShareTargetMarking( unsigned int uiSlot ) 
{
    if(uiSlot < 0 || uiSlot >= NTL_MAX_SHARETARGET_COUNT)
        return FALSE;

    if(GetDboGlobal()->GetTargetSerial() == INVALID_SERIAL_ID)
        return FALSE;

	GetDboGlobal()->GetGamePacketGenerator()->SendPartyShareTargetReq( (RwUInt8)uiSlot, GetDboGlobal()->GetTargetSerial() );

    return TRUE;
}

int CAvatarController::ActionPartyShareTargetSelect( unsigned int uiSlot ) 
{	
    if(uiSlot < 0 || uiSlot >= NTL_MAX_SHARETARGET_COUNT)
        return FALSE;

    RwUInt32 uiSerialID = Logic_GetShareTargetSerial(uiSlot);
    if(uiSerialID == INVALID_SERIAL_ID)
        return FALSE;
    
    // 타겟의 선택에는 PickPLObject() 메소드를 이용한다.
    SWorldPickInfo info;
    PickPLObject(info, FALSE, uiSerialID);

    return TRUE;
}

/**
 * 순차적으로 파티멤버를 타겟팅한다. 
 */
int CAvatarController::ActionPartyAutoSelect() 
{
    if(!GetNtlWorldConcept()->IsEnableAction(E_ACTION_MOVE))
        return TRUE;

    if( !Logic_CanKeybaordInput_in_Tutorial( ETL_KEYBOARD_INPUT_TYPE_TARGETING ) )
        return FALSE;

    if(Logic_GetActorStateFlags(GetNtlSLGlobal()->GetSobAvatar()) & NTL_FSMSF_NOT_INPUT)
        return FALSE;

    static RwInt32 nSelectIndex = 0;
    CNtlParty* pParty = GetNtlSLGlobal()->GetSobAvatar()->GetParty();
    if(!pParty || pParty->GetMemberCount() == 0)
        return FALSE;

    nSelectIndex = nSelectIndex >= pParty->GetMemberCount() ? 0 : nSelectIndex;    
    SERIAL_HANDLE hNewTargetID = pParty->GetMemberbyIndex(nSelectIndex)->uiKey;         
    SERIAL_HANDLE hTargetSerialId = GetDboGlobal()->GetTargetSerial();    
    if(hNewTargetID != hTargetSerialId)
    {
        if(hTargetSerialId != INVALID_SERIAL_ID)
            CNtlSLEventGenerator::SobTargetSelectRelease(hTargetSerialId);

        if(GetNtlSobManager()->GetSobObject(hNewTargetID))
        {
            GetDboGlobal()->SetTargetSerial(hNewTargetID);
            CNtlSLEventGenerator::SobTargetSelect(hNewTargetID);
            API_GetSLPacketGenerator()->SendCharTargetSelect(hNewTargetID);
        }
        else    // 객체가 없을때는 커뮤니티 타겟으로 설정한다.
        {
            if(GetDboGlobal()->GetCommunityTarget() != INVALID_SERIAL_ID)
                CNtlSLEventGenerator::CommuTargetRelease(GetDboGlobal()->GetCommunityTarget());

            sPartyMember* pMember = (sPartyMember*)pParty->GetMemberbyIndex(nSelectIndex);
            CNtlSLEventGenerator::CommuTargetSelect(NULL, hNewTargetID, pMember->wszMemberName, COMMU_TARGET_PARTY);
        }

        ++nSelectIndex;
    }

    return TRUE;
}
