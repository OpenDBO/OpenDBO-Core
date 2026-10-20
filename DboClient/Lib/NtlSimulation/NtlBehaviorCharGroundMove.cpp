#include "precomp_ntlsimulation.h"
#include "NtlBehaviorCharGroundMove.h"

// shared 
#include "NtlMovement.h"

// inventory
#include "NtlInventory.h"

// sound
#include "NtlSoundDefines.h"
#include "NtlSoundManager.h"

// framework
#include "NtlCamera.h"

// presentation
#include "NtlPLEvent.h"
#include "NtlPLPropertyContainer.h"

// simulation
#include "NtlSobActor.h"
#include "NtlSLLogic.h"
#include "NtlSobCharProxy.h"
#include "NtlSLVisualDeclear.h"
#include "NtlFSMDef.h"
#include "NtlSobManager.h"
#include "NtlCameraManager.h"
#include "NtlSLEventFunc.h"
#include "NtlSlApi.h"
#include "NtlSLPacketGenerator.h"
#include "NtlCameraController.h"

#define FRAME_2 (2.0f / 30.0f)

// tw dead reckoning / collision report values
#define MOVE_COLLISION_CHECK_TIME		0.5f	// the avatar repeats its collision report at most this often
#define MOVE_GHOST_SNAP_TIME			1.0f	// held back by object collision this long, the prediction takes the free move
#define MOVE_PREDICT_SEED_RANGE			30.0f	// a server position further away than this is not trusted to seed the prediction
#define MOVE_SMOOTH_MIN_DIST			0.1f	// the shown position is pulled to the prediction above this distance
#define MOVE_SMOOTH_RATE				0.8f
#define MOVE_SMOOTH_RATE_BACK			0.4f	// pulling against the move direction
#define MOVE_SMOOTH_COLLISION_DIST		0.3f
#define MOVE_SMOOTH_COLLISION_MIN_RATE	0.2f
#define MOVE_LOCATION_SYNC_TIME			0.5f	// the moving avatar sends its position this often (not in tw)
#define MOVE_LOCATION_SYNC_TURN_TIME	0.1f	// sooner while it turns
#define MOVE_LOCATION_SYNC_TURN_COS		0.9986f	// cos(3 degrees)
#define MOVE_LOCATION_SYNC_STOPTURN_TIME	0.05f	// not turned this long after a turn, where the turn ended is sent
#define MOVE_TURNING_COS				0.99999f	// turned less than this in a frame, the avatar is not turning
#define MOVE_YAW_SAMPLE_MIN_TIME		0.08f	// headings closer than this do not measure the turn speed
#define MOVE_YAW_SAMPLE_MAX_TIME		1.0f	// further apart, the character was not turning
#define MOVE_YAW_MIN_DELTA				0.035f	// (2 degrees) a smaller change between headings is no turn
#define MOVE_YAW_EXTRAPOLATE_TIME		0.2f	// the turn goes on at the measured speed this long after a heading
#define MOVE_YAW_MAX_EXTRAPOLATE		0.7f	// (40 degrees) but never more than this
#define MOVE_TURN_SMOOTH_RATE			15.0f	// the shown facing follows the moving one (per second)


DEFINITION_MEMORY_POOL(CNtlBehaviorCharGroundMove)

CNtlBehaviorCharGroundMove::CNtlBehaviorCharGroundMove()
{
    SetBehaviorId(NTL_BEID_CHAR_GROUNDMOVE);
    SetBehaviorName(NTL_BENAME_CHAR_GROUNDMOVE);

    m_bSlowMove				= FALSE;
    m_fSlowMoveTime			= 0.0f;
    m_byDashState			= DASH_STATE_MOVE;
    m_pDashEffect			= NULL;
    m_pDashLine				= NULL;
    m_pDashEffectSmoke		= NULL;
    m_fEffectSmokeTime		= 0.0f;
    m_bDashCameraEnable		= FALSE;
    m_fPosChangeTime		= 0.0f;
    m_byPosChangeCnt		= 0;
    m_byCollMoveImpossCnt	= 0;

    m_vPredictPos			= ZeroAxis;
    m_vPredictDir			= ZeroAxis;
    m_vGhostPos				= ZeroAxis;
    m_fGhostTime			= 0.0f;
    m_bMoveCollision		= FALSE;
    m_fForcedSpeed			= 1.0f;
    m_vForcedDir			= ZeroAxis;

    m_bCollisionSent		= FALSE;
    m_fLastCollisionRate	= RwRealMAXVAL;
    m_fCollisionCheckTime	= MOVE_COLLISION_CHECK_TIME;

    m_fLocationSyncTime		= 0.0f;
    m_vLocationSyncDir		= ZeroAxis;
    m_vLastFrameDir			= ZeroAxis;
    m_fNoTurnTime			= 0.0f;
    m_bTurnSyncPending		= FALSE;

    m_fYawRate				= 0.0f;
    m_fYawBudget			= 0.0f;
    m_fYawExtrapolateTime	= 0.0f;
    m_vYawSampleDir			= ZeroAxis;
    m_fYawSampleTime		= 0.0f;

    m_fInterAnimTime        = 0.0f;
}

CNtlBehaviorCharGroundMove::~CNtlBehaviorCharGroundMove()
{
    DestroyEndDashCamera();
    DestroyDashEffect();
    DestroyDashEffectSmoke();
}


void CNtlBehaviorCharGroundMove::Enter(void)
{
    CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData());     
    memcpy(&m_MoveStuff, pBeData->GetMoveStuff(), sizeof(SMoveStuff));

    if(IsInMoveRound())
    {
        Finish();
        return;
    }

    SetPathNode();
    SetTransform();	
    CreateStartDashCamera();

    m_fDefaultWalkSpeed = Logic_GetDefaultFrontWalkSpeed(m_pActor);
    m_fDefaultRunSpeed	= Logic_GetDefaultFrontRunSpeed(m_pActor);
    m_fDefaultBackSpeed = Logic_GetDefaultBackSpeed(m_pActor);

    ResetDeadReckoning(TRUE);

    SetAnim(m_MoveStuff.byMoveFlags);

    // Idle -> Run 보간 애니메이션
    SetIdle2RunAnim();

    // Later, call base class enter.
    CNtlBehaviorBase::Enter(); 
}

void CNtlBehaviorCharGroundMove::SetIdle2RunAnim( void ) 
{
    if(m_pActor->GetSobProxy()->GetDisableAniChange() || Logic_IsTransformSpinAttack(m_pActor) || Logic_IsTransformRollingAttack(m_pActor))
        return;

    // 占쏙옙占신시울옙占쏙옙 占쏙옙占쏙옙 占쌍니몌옙占싱쇽옙占쏙옙 占쏙옙占싼댐옙.
    if(Logic_GetPlayerRace(m_pActor) == RACE_NAMEK && Logic_IsTransform(m_pActor))
        return;

    // Idle 占쌍니몌옙占싱쇽옙 占쏙옙占싹띰옙占쏙옙 占쏙옙환占싼댐옙.
    if(!IsIdleAnimationPlaying())
        return;

    // Idle -> Run Interpolation animation
    if(m_MoveStuff.byMoveFlags == NTL_MOVE_F || m_MoveStuff.byType == NTL_MOVETARGET_LOC || m_MoveStuff.byType == NTL_MOVETARGET_ACTOR)
    {
        if(m_pActor->GetClassID() == SLCLASS_AVATAR)
        {
            CNtlSobCharProxy *pSobProxy = reinterpret_cast<CNtlSobCharProxy*>(m_pActor->GetSobProxy());            
            sITEM_TBLDAT *pItemTblData = Logic_GetEquipedWeaponItemTableData(m_pActor);
            if(pItemTblData && Logic_IsEquipedStaffWeapon(pItemTblData))    // 스태프를 들고 있을때
            {
                pSobProxy->SetBaseAnimation(NML_STAFF_IDLE_RUN_FRONT, FALSE);
            }
            else    // �ƹ��͵� ��� ���� ������
            {
                pSobProxy->SetBaseAnimation(IDLE_RUN_FRONT, FALSE);   
            }
            m_bInterAnimEnd = FALSE;
        }
    }
}

void CNtlBehaviorCharGroundMove::Exit(void)
{
    DestroyEndDashCamera();
    DestroyDashEffect();
    DestroyDashEffectSmoke();

    EndCollisionReport();

    if(m_pActor->GetClassID() != SLCLASS_AVATAR)
    {
        CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData()); 
        if(pBeData)
        {
            SMoveSyncStuff* pMoveSyncStuff = pBeData->GetMoveSyncStuff();
            pMoveSyncStuff->Clear();
        }        
    }
    

    // 占쏙옙占쌩울옙 base class enter占쏙옙 호占쏙옙占싼댐옙.
    CNtlBehaviorBase::Exit(); 
}

void CNtlBehaviorCharGroundMove::Update(RwReal fElapsed)
{
    fElapsed = min(fElapsed, 0.2f);

    if(IsFinish() && !m_pActor->GetSobProxy()->GetDisableAniChange()) // 애니메이션 변경안되는 상태에서는 끝내지 않는다. (회전공격을 위해)
        return;

    // 占쏙옙占쏙옙 占쌍니몌옙占싱쇽옙 처占쏙옙 (2Frame 占식븝옙占쏙옙 占싱듸옙占싼댐옙)
    if(!m_bInterAnimEnd)
    {
        if(m_fInterAnimTime < FRAME_2)
        {
            m_fInterAnimTime += fElapsed;
            return;
        }        
        else
        {
            m_bInterAnimEnd = TRUE;
            m_fInterAnimTime = 0.0f;            
        }
    }

    RwBool bFinish = FALSE;
    switch(m_MoveStuff.byType)
    {
    case NTL_MOVETARGET_DIR:                
        bFinish = UpdateDirectionMove(fElapsed);        
        break;
    case NTL_MOVETARGET_LOC:
        bFinish = UpdateLocationMove(fElapsed);
        break;
    case NTL_MOVETARGET_ACTOR:
        if(!Logic_IsTransformSpinAttack(m_pActor) && !Logic_IsTransformRollingAttack(m_pActor))  // When spin attack, do not move target.
        {
            bFinish = UpdateTargetMove(fElapsed);        
        }        
        break;
    case NTL_MOVETARGET_DASH:
        UpdateDashMove(fElapsed);
        break;
    case NTL_MOVETARGET_DASH_ACTOR:
        bFinish = UpdateDashTargetMove(fElapsed);
        break;
    default:
        bFinish = TRUE;
    }

    if(!bFinish)
        UpdateLocationSync(fElapsed);

    UpdateAnimSpeed();

    if(bFinish && (!Logic_IsTransformSpinAttack(m_pActor) && !Logic_IsTransformRollingAttack(m_pActor)))
    {
        Finish();        
    }
}

RwUInt32 CNtlBehaviorCharGroundMove::HandleEvents(RWS::CMsg &pMsg)
{
    NTL_FUNCTION("CNtlBehaviorCharGroundMove::HandleEvents");

    if(pMsg.Id == g_EventAnimFootStep)
    {
        if(!m_sHStuff.bWaterAttr || m_sHStuff.fWorldHeight > m_sHStuff.fWaterHeight)
        {
            FootStepMaterialProc(pMsg);
        }
        else
        {
            RwV3d vPos = m_pActor->GetPosition();
            CNtlSobProxy *pSobProxy = m_pActor->GetSobProxy();
            vPos.y = m_sHStuff.fWaterHeight;
            pSobProxy->CreatePLEffect(NTL_VID_WATER_FOOT, &vPos);
        }
    }

    else if(pMsg.Id == g_EventSobEquipChange)
    {
        RwUInt8 byMoveType = m_MoveStuff.byType;
        if(byMoveType == NTL_MOVETARGET_DIR || byMoveType == NTL_MOVETARGET_LOC ||
            byMoveType == NTL_MOVETARGET_ACTOR)
        {
            SetAnim(m_MoveStuff.byMoveFlags);
        }
    }
    else if(pMsg.Id == g_EventAnimEnd)
    {
        SNtlEventAnimEnd *pAnimEnd = reinterpret_cast<SNtlEventAnimEnd*>( pMsg.pData );
        RwUInt32 uiAnimKey = pAnimEnd->uiBaseAnimKey;

        if(uiAnimKey == IDLE_RUN_FRONT || uiAnimKey == NML_STAFF_IDLE_RUN_FRONT) // Completion processing of interpolation animation
        {
            SetAnim(m_MoveStuff.byMoveFlags);                
            m_bInterAnimEnd = TRUE; 
        }        

        if(m_byDashState == DASH_STATE_LANDING || m_byDashState == DASH_STATE_END)
        {
			if( uiAnimKey == NML_DASH_FRONT || uiAnimKey == NML_DASH_BACK || uiAnimKey == NML_DASH_LEFT || uiAnimKey == NML_DASH_RIGHT)
            {
                Finish();
            }
        }
    }
    else if(pMsg.Id == g_EventSobMoveCollision)
    {
        OnMoveCollision(reinterpret_cast<SNtlEventSobMoveCollision*>( pMsg.pData ));
    }
    else if(pMsg.Id == g_EventSobMoveCollisionEnd)
    {
        OnMoveCollisionEnd(reinterpret_cast<SNtlEventSobMoveCollisionEnd*>( pMsg.pData ));
    }
    else if(pMsg.Id == g_EventSobChangeHeading)
    {
        OnChangeHeading(reinterpret_cast<SNtlEventSobChangeHeading*>( pMsg.pData )->vDir);
    }
    else if(pMsg.Id == g_EventSobMoveSync)
    {
        SNtlEventSobMoveSync* pMoveSync = reinterpret_cast<SNtlEventSobMoveSync*>( pMsg.pData );
        OnMoveSync(pMoveSync->vLoc, pMoveSync->vDir);

        // applied right away, the old move sync queue of the base class is not used by the ground move
        NTL_RETURN(NTL_FSM_EVENTRES_PASS);
    }
    else if(pMsg.Id == g_EventTransform)
    {
        SNtlEventTransform* pEvent = (SNtlEventTransform*)pMsg.pData;
        if(pEvent->sApsectState.sAspectStateBase.byAspectStateId == ASPECTSTATE_INVALID)     // 占쏙옙占쏙옙 占쏙옙占쏙옙
        {
            SetAnim(m_MoveStuff.byMoveFlags);
        }
    }

    NTL_RETURN(CNtlBehaviorCharMove::HandleEvents(pMsg));
}

void CNtlBehaviorCharGroundMove::UpdateData(void)
{
    CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData()); 
    SMoveStuff *pMoveStuff = pBeData->GetMoveStuff();
    memcpy(&m_MoveStuff, pMoveStuff, sizeof(SMoveStuff));

    SetAnim(m_MoveStuff.byMoveFlags);

    // tw: every move packet of another client restarts the prediction from the server position
    ResetDeadReckoning();

    // the move packet just sent the avatar position
    m_fLocationSyncTime = 0.0f;
    m_vLocationSyncDir = m_pActor->GetDirection();
}

void CNtlBehaviorCharGroundMove::FootStepMaterialProc(RWS::CMsg &pMsg)
{
    RwV3d			vPos		= m_pActor->GetPosition();

    if(m_pActor->GetClassID() == SLCLASS_AVATAR || m_pActor->GetClassID() == SLCLASS_MONSTER)
    {
        SNtlEventFootStep*			pData		= reinterpret_cast<SNtlEventFootStep*>(pMsg.pData);
        SEventFootStep*             pEvent      = reinterpret_cast<SEventFootStep*>(pData->pData);
        CNtlPLEntity*				pEntity;
        RwChar						cSoundResoureName[64];
        RwChar						cEffectResoureName[64];

        BYTE						byMaterial	= 0;
        RwReal						fHeight		= 0.0f;

        // Cz : CNtlPLGlobal::m_pWHEntity ���强�� ������ GetWorldHeight�� ȣ���մϴ�.
        //		Performance占쏙옙 占쏙옙占쏙옙占쏙옙 占쏙옙 占쏙옙占쏙옙占쏙옙 占쏙옙占쏙옙占쏙옙 占쏙옙占쏙옙占실뤄옙 占쏙옙占쏙옙歐占쏙옙 占싹울옙占쏙옙占싹댐옙.
        //		문제의 요지가 있을 경우 진성이를 호출해 주세요.
        if (!(GetSceneManager()->GetWorldHeight(&vPos, fHeight, NULL) && CNtlPLGlobal::m_pWHEntity))
        {
            byMaterial = GetSceneManager()->GetWorldMaterialAttribute(vPos);
        }

        RwInt32 iChannelGroup = (m_pActor->GetClassID() == SLCLASS_AVATAR)
            ? CHANNEL_GROUP_AVATAR_EFFECT_SOUND
            : CHANNEL_GROUP_EFFECT_SOUND;

        if(pEvent->eFootStepMobType == FOOT_TYPE_LARGE || (m_pActor->GetFlags() & SLFLAG_LARGE_FOOT_SOUND)) // 占쏙옙占쏙옙 占쏙옙 占쏙옙占쌘깍옙 占쌀몌옙
        {
            sprintf_s(cSoundResoureName, 64, "%s_L_%u.wav", NAMING_SOUND_FOOTSTEP, NtlRandomNumber(0, 3));

            sNtlSoundPlayParameta tSoundParam;
            tSoundParam.iChannelGroup	= iChannelGroup;
            tSoundParam.pcFileName		= cSoundResoureName;
            tSoundParam.bLoop			= FALSE;
            tSoundParam.fVolume			= NtlRandomNumber(0.7f, 1.0f);
            tSoundParam.fPitch			= NtlRandomNumber(0.97f, 1.03f);
            tSoundParam.fXPos			= vPos.x;
            tSoundParam.fYPos			= vPos.y;
            tSoundParam.fZPos			= vPos.z;
            tSoundParam.fMinDistance	= 7.0f;
            tSoundParam.fMaxDistance	= 25.0f;

			GetSoundManager()->Play(&tSoundParam);
        }
        else    // 占싹뱄옙 占쏙옙 占쏙옙占쌘깍옙 占쌀몌옙
        {
            sprintf_s(cSoundResoureName, 64, "%s_%u_%u.wav", NAMING_SOUND_FOOTSTEP, byMaterial, NtlRandomNumber(0, 3));

            sNtlSoundPlayParameta tSoundParam;
            tSoundParam.iChannelGroup	= iChannelGroup;
            tSoundParam.pcFileName		= cSoundResoureName;
            tSoundParam.bLoop			= FALSE;
            tSoundParam.fVolume			= NtlRandomNumber(0.7f, 1.0f);
            tSoundParam.fPitch			= NtlRandomNumber(0.97f, 1.03f);
            tSoundParam.fXPos			= vPos.x;
            tSoundParam.fYPos			= vPos.y;
            tSoundParam.fZPos			= vPos.z;
            tSoundParam.fMinDistance	= 2.0f;
            tSoundParam.fMaxDistance	= 10.0f;

			GetSoundManager()->Play(&tSoundParam);
        }

        sprintf_s(cEffectResoureName, 64, "%s_%u_%c_%u", NAMING_EFFECT_FOOTSTEP, byMaterial, (pEvent->eFootStepType ? 'R' : 'L'), NtlRandomNumber(0, 1)); 	 	
        if (CNtlPLPropertyContainer::GetInstance()->GetProperty(PLENTITY_EFFECT, cEffectResoureName))
        {
            pEntity	= m_pActor->GetSobProxy()->CreatePLEffect(cEffectResoureName, &vPos);
            if (pEntity)
            {
                pEntity->SetRotate(0.0f, CNtlMath::LineToAngleY(&m_pActor->GetDirection()), 0.0f);
            }
        }		
    }
}

RwBool CNtlBehaviorCharGroundMove::IsInDestMoveRound(void)
{
    RwV3d vPos = m_pActor->GetPosition(); 
    RwV3d vDir;
    RwV3dSubMacro(&vDir, &m_MoveStuff.vDest, &vPos);
    vDir.y = 0.0f;
    RwReal fLen = RwV3dLength(&vDir);
    if(fLen < 0.05f)
        return TRUE;

    return FALSE;
}

RwBool CNtlBehaviorCharGroundMove::IsInFollowMoveRound(void)
{
    CNtlSobActor *pTargetActor = reinterpret_cast<CNtlSobActor*>(GetNtlSobManager()->GetSobObject(m_MoveStuff.hFollowSerialId));
    if(pTargetActor == NULL)
        return TRUE;

    RwV3d vPos = m_pActor->GetPosition();
    RwV3d vDestPos = pTargetActor->GetPosition(); 
    // An object that floats on top of the terrain and drifts.
    if(pTargetActor->GetFlags() & SLFLAG_CAN_GROUND_FLY)
        vDestPos.y -= Logic_GetGroundFlyHeight(pTargetActor);

    RwV3d vDir;
    RwV3dSubMacro(&vDir, &vDestPos, &vPos); 

    // Obtain the current distance.
    RwReal fCurrLen = RwV3dLength(&vDir);

    RwReal fInRange = m_MoveStuff.fFollowRange/**FOLLOW_ERROR_RANGE*/;// disable by daneos

    if(fCurrLen <= fInRange)
        return TRUE;

    // 2d Check the distance.
    vDir.y = 0.0f;
    fCurrLen = RwV3dLength(&vDir);
    if(fCurrLen <= fInRange / 2.0f)
        return TRUE;

    return FALSE;
}

RwBool CNtlBehaviorCharGroundMove::IsInMoveRound(void)
{
    if(m_MoveStuff.byType == NTL_MOVETARGET_LOC)
        return IsInDestMoveRound();
    else if(m_MoveStuff.byType == NTL_MOVETARGET_ACTOR || m_MoveStuff.byType == NTL_MOVETARGET_DASH_ACTOR)
        return IsInFollowMoveRound();

    return FALSE;
}

void CNtlBehaviorCharGroundMove::LimitPositionChangeCheck(RwReal fElapsed)
{
    m_fPosChangeTime += fElapsed;
    m_byPosChangeCnt++;
    if(m_fPosChangeTime >= MOVE_POSITION_CHANGE_CHECK_TIME)
    {
        if(m_byPosChangeCnt >= 3)
        {
            RwV3d vPos = m_pActor->GetPosition();
            RwV3d vSub;
            RwV3dSubMacro(&vSub, &vPos, &m_vChangeOldPos);
            if(RwV3dLength(&vSub) <= MOVE_POSITION_CHANGE_LIMIT_RANGE)
                Finish();
            else
                RwV3dAssignMacro(&m_vChangeOldPos, &vPos);
        }

        m_fPosChangeTime -= MOVE_POSITION_CHANGE_CHECK_TIME;
        m_byPosChangeCnt = 0;
    }
}

void CNtlBehaviorCharGroundMove::SetPathNode(void)
{
    if(m_MoveStuff.byPathNodeNum)
    {
        for(RwInt32 i = 0; i < m_MoveStuff.byPathNodeNum; i++)
            AddPathNode(m_MoveStuff.arrPathNode[i]);

        RwV3dAssignMacro(&m_MoveStuff.vDest, &m_MoveStuff.arrPathNode[0]);
    }
}

void CNtlBehaviorCharGroundMove::SetTransform(void)
{
    RwV3d vPos = m_pActor->GetPosition(); 
    RwV3dAssignMacro(&m_vChangeOldPos, &vPos);

    if(m_MoveStuff.byType == NTL_MOVETARGET_LOC)
    {
        RwV3d vDir;
        RwV3dSubMacro(&vDir, &m_MoveStuff.vDest, &vPos);
        vDir.y = 0.0f;
        if(RwV3dLength(&vDir) > 0.001f)
            RwV3dNormalize(&vDir, &vDir); 
        else
            RwV3dAssignMacro(&vDir, &m_pActor->GetDirection());

        m_pActor->SetDirection(&vDir); 
    }

    // it is determined whether it is a slow move.
    else if(m_MoveStuff.byType == NTL_MOVETARGET_ACTOR)
    {
        CNtlSobActor *pTargetActor = reinterpret_cast<CNtlSobActor*>(GetNtlSobManager()->GetSobObject(m_MoveStuff.hFollowSerialId));
        if(pTargetActor)
        {	
            RwV3d vDestPos = pTargetActor->GetPosition(); 

            // ���� ���� ���� ���� ����ٴϴ� ��ü�� ���.
            if(pTargetActor->GetFlags() & SLFLAG_CAN_GROUND_FLY)
                vDestPos.y -= Logic_GetGroundFlyHeight(pTargetActor);

            NTL_ASSERT(m_pActor != pTargetActor, "CNtlBehaviorCharGroundMove::SetTransform");

            RwV3d vDir;
            RwV3dSubMacro(&vDir, &vDestPos, &vPos); 

            // Obtain the current distance.
            vDir.y = 0.0f;
            RwReal fCurrLen = RwV3dLength(&vDir);
            if(fCurrLen - m_MoveStuff.fFollowRange < SLOW_MOVE_RANGE)
            {
             //   m_bSlowMove = TRUE; // by daneos
            }

            RwV3dNormalize(&vDir, &vDir); 
            m_pActor->SetDirection(&vDir); 
        }
    }

    else if(m_MoveStuff.byType == NTL_MOVETARGET_DASH)
    {
        m_fDashLadingDist = NTL_DASH_SPEED*DASH_LANDING_TIME;

        Logic_GetWorldHeight(&m_MoveStuff.vDest, m_sHStuff);
        m_MoveStuff.vDest.y = m_sHStuff.fFinialHeight;
        m_fDashDestPosY	= m_MoveStuff.vDest.y;
    }
}

void CNtlBehaviorCharGroundMove::SetAnim(RwUInt8 byMoveFlags)
{
    if(m_pActor->GetSobProxy()->GetDisableAniChange())
        return;

    CNtlSobCharProxy *pSobProxy = reinterpret_cast<CNtlSobCharProxy*>(m_pActor->GetSobProxy());
    RwUInt32 uiCurrAnimKey = pSobProxy->GetBaseAnimationKey();
    RwUInt32 uiNextAnimKey = 0;

    if(m_MoveStuff.byType == NTL_MOVETARGET_DIR)
    {
		sITEM_TBLDAT *pItemTblData = Logic_GetEquipedWeaponItemTableData(m_pActor);

        if( byMoveFlags == NTL_MOVE_F || byMoveFlags == NTL_MOVE_F_TURN_L || byMoveFlags == NTL_MOVE_F_TURN_R )
        {
            if(Logic_IsTransformGreatNamek(m_pActor))  // Transformation
            {
                uiNextAnimKey = TRANS_RUN_FRONT;
            }
            else if(Logic_IsTransformSpinAttack(m_pActor))
            {
                uiNextAnimKey = TRANS_IDLE;
            }
			else if (Logic_IsTransformRollingAttack(m_pActor))
			{
				uiNextAnimKey = TRANS_IDLE;
			}
            else if(pItemTblData && Logic_IsEquipedStaffWeapon(pItemTblData))
            {
                uiNextAnimKey = NML_STAFF_FRONT_RUN;
            }
            else
            {
                uiNextAnimKey = RUN_FRONT;
            }
        }
        else if( byMoveFlags == NTL_MOVE_B || byMoveFlags == NTL_MOVE_B_TURN_L || byMoveFlags == NTL_MOVE_B_TURN_R )
        {
            if(Logic_IsTransformGreatNamek(m_pActor)) // Transformation
            {
                uiNextAnimKey = TRANS_RUN_BACK;
            }
            else if(Logic_IsTransformSpinAttack(m_pActor))
            {
                uiNextAnimKey = TRANS_IDLE;
            }
			else if (Logic_IsTransformRollingAttack(m_pActor))
			{
				uiNextAnimKey = TRANS_IDLE;
			}
			else if (pItemTblData && Logic_IsEquipedStaffWeapon(pItemTblData))
			{
				uiNextAnimKey = NML_STAFF_BACK_RUN;
			}
            else
            {
                uiNextAnimKey = RUN_BACK;
            }
        }
		else if (byMoveFlags == NTL_MOVE_L || byMoveFlags == NTL_MOVE_L_TURN_L || byMoveFlags == NTL_MOVE_L_TURN_R)
		{
			sITEM_TBLDAT *pItemTblData = Logic_GetEquipedWeaponItemTableData(m_pActor);
			if (Logic_IsTransformSpinAttack(m_pActor))
			{
				uiNextAnimKey = TRANS_IDLE;
			}
			else if (Logic_IsTransformRollingAttack(m_pActor))
			{
				uiNextAnimKey = TRANS_IDLE;
			}
			else
			{
				uiNextAnimKey = NML_MOVE_LEFT;
			}
		}
		else if (byMoveFlags == NTL_MOVE_R || byMoveFlags == NTL_MOVE_R_TURN_L || byMoveFlags == NTL_MOVE_R_TURN_R)
		{
			sITEM_TBLDAT *pItemTblData = Logic_GetEquipedWeaponItemTableData(m_pActor);
			if (Logic_IsTransformSpinAttack(m_pActor))
			{
				uiNextAnimKey = TRANS_IDLE;
			}
			else if (Logic_IsTransformRollingAttack(m_pActor))
			{
				uiNextAnimKey = TRANS_IDLE;
			}
			else
			{
				uiNextAnimKey = NML_MOVE_RIGHT;
			}
		}
        else if(byMoveFlags == NTL_MOVE_TURN_L)
        {
			if (pItemTblData && Logic_IsEquipedStaffWeapon(pItemTblData))
			{
				uiNextAnimKey = NML_STAFF_LEFT_TURN;
			}
			else
			{
				uiNextAnimKey = TURN_LEFT;
			}
        }
        else if(byMoveFlags == NTL_MOVE_TURN_R)
        {
			if (pItemTblData && Logic_IsEquipedStaffWeapon(pItemTblData))
			{
				uiNextAnimKey = NML_STAFF_RIGHT_TURN;
			}
			else
			{
				uiNextAnimKey = TURN_RIGHT;
			}
        }
    }
    else if(m_MoveStuff.byType == NTL_MOVETARGET_LOC || m_MoveStuff.byType == NTL_MOVETARGET_ACTOR)
    {
        RwBool bWalkMove = Logic_IsActorWalkMove(m_pActor, m_MoveStuff.byFormFlag);
        sITEM_TBLDAT *pItemTblData = Logic_GetEquipedWeaponItemTableData(m_pActor);

        if(bWalkMove)
        {
            uiNextAnimKey = WALK_FRONT;
        }
        else
        {
            if(Logic_IsTransformGreatNamek(m_pActor))  // 占쏙옙占신쏙옙
            {
                uiNextAnimKey = TRANS_RUN_FRONT;
            }
            else if(Logic_IsTransformSpinAttack(m_pActor))
            {
                uiNextAnimKey = TRANS_IDLE;
            }
			else if (Logic_IsTransformRollingAttack(m_pActor))
			{
				uiNextAnimKey = TRANS_IDLE;
			}
            else if(pItemTblData && Logic_IsEquipedStaffWeapon(pItemTblData))
            {
                uiNextAnimKey = NML_STAFF_FRONT_RUN;
            }
            else
            {
                uiNextAnimKey = RUN_FRONT;
            }
        }
    }
    else if(m_MoveStuff.byType == NTL_MOVETARGET_DASH)
    {
        if(byMoveFlags == NTL_MOVE_F)
            uiNextAnimKey = NML_DASH_FRONT;
        else if(byMoveFlags == NTL_MOVE_B)
            uiNextAnimKey = NML_DASH_BACK;
        else if(byMoveFlags == NTL_MOVE_L)
            uiNextAnimKey = NML_DASH_LEFT;
        else
            uiNextAnimKey = NML_DASH_RIGHT;

        RwUInt8 bootsGrade = Logic_GetBootsGrade(m_pActor);
        switch (bootsGrade) 
        {
            case (0): 
            {
                CreateDashEffect(byMoveFlags, NTL_VID_DASH_ACTIVE_01, NTL_VID_DASH_ACTIVE_02);
                break;
            }
            case (1):
            {
                CreateDashEffect(byMoveFlags, NTL_VID_DASH_ACTIVE_BLUE_01, NTL_VID_DASH_ACTIVE_BLUE_02);
                break;
            }
            case (2):
            {
                CreateDashEffect(byMoveFlags, NTL_VID_DASH_ACTIVE_BLUE_01, NTL_VID_DASH_ACTIVE_BLUE_02);
                break;
            }
            case (3):
            {
                CreateDashEffect(byMoveFlags, NTL_VID_DASH_ACTIVE_BLUE_01, NTL_VID_DASH_ACTIVE_BLUE_02);
                break;
            }
            case (4):
            {
                CreateDashEffect(byMoveFlags, NTL_VID_DASH_ACTIVE_BLUE_01, NTL_VID_DASH_ACTIVE_BLUE_02);
                break;
            }
            case (5):
            {
                CreateDashEffect(byMoveFlags, NTL_VID_DASH_ACTIVE_GREEN_01, NTL_VID_DASH_ACTIVE_GREEN_02);
                break;
            }
            case (6):
            {
                CreateDashEffect(byMoveFlags, NTL_VID_DASH_ACTIVE_GREEN_01, NTL_VID_DASH_ACTIVE_GREEN_02);
                break;
            }
            case (7):
            {
                CreateDashEffect(byMoveFlags, NTL_VID_DASH_ACTIVE_GREENT_01, NTL_VID_DASH_ACTIVE_GREEN_02);
                break;
            }
            case (8):
            {
                CreateDashEffect(byMoveFlags, NTL_VID_DASH_ACTIVE_GREENT_01, NTL_VID_DASH_ACTIVE_GREEN_02);
                break;
            }
            case (9):
            {
                CreateDashEffect(byMoveFlags, NTL_VID_DASH_ACTIVE_YELLOW_01, NTL_VID_DASH_ACTIVE_YELLOW_02);
                break;
            }
            case (10):
            {
                CreateDashEffect(byMoveFlags, NTL_VID_DASH_ACTIVE_YELLOW_01, NTL_VID_DASH_ACTIVE_YELLOW_02);
                break;
            }
            case (11):
            {
                CreateDashEffect(byMoveFlags, NTL_VID_DASH_ACTIVE_YELLOW_01, NTL_VID_DASH_ACTIVE_YELLOW_02);
                break;
            }
            case (12):
            {
                CreateDashEffect(byMoveFlags, NTL_VID_DASH_ACTIVE_YELLOW_01, NTL_VID_DASH_ACTIVE_YELLOW_02);
                break;
            }
            case (13):
            {
                CreateDashEffect(byMoveFlags, NTL_VID_DASH_ACTIVE_RED_01, NTL_VID_DASH_ACTIVE_RED_02);
                break;
            }
            case (14):
            {
                CreateDashEffect(byMoveFlags, NTL_VID_DASH_ACTIVE_REDT_01, NTL_VID_DASH_ACTIVE_REDT_02);
                break;
            }
            case (15):
            {
                CreateDashEffect(byMoveFlags, NTL_VID_DASH_ACTIVE_PURPLE_01, NTL_VID_DASH_ACTIVE_PURPLE_02);
                break;
            }
        }

        UpdateDashEffect(byMoveFlags, m_MoveStuff.vDir);
    }
    else if(m_MoveStuff.byType == NTL_MOVETARGET_DASH_ACTOR)
    {
        uiNextAnimKey = NML_DASH_FRONT;

        RwV3d vDir = m_pActor->GetDirection();

        RwUInt8 bootsGrade = Logic_GetBootsGrade(m_pActor);
        switch (bootsGrade)
        {
        case (0):
        {
            CreateDashEffect(byMoveFlags, NTL_VID_DASH_ACTIVE_01, NTL_VID_DASH_ACTIVE_02);
            break;
        }
        case (1):
        {
            CreateDashEffect(byMoveFlags, NTL_VID_DASH_ACTIVE_BLUE_01, NTL_VID_DASH_ACTIVE_BLUE_02);
            break;
        }
        case (2):
        {
            CreateDashEffect(byMoveFlags, NTL_VID_DASH_ACTIVE_BLUE_01, NTL_VID_DASH_ACTIVE_BLUE_02);
            break;
        }
        case (3):
        {
            CreateDashEffect(byMoveFlags, NTL_VID_DASH_ACTIVE_BLUE_01, NTL_VID_DASH_ACTIVE_BLUE_02);
            break;
        }
        case (4):
        {
            CreateDashEffect(byMoveFlags, NTL_VID_DASH_ACTIVE_BLUE_01, NTL_VID_DASH_ACTIVE_BLUE_02);
            break;
        }
        case (5):
        {
            CreateDashEffect(byMoveFlags, NTL_VID_DASH_ACTIVE_GREEN_01, NTL_VID_DASH_ACTIVE_GREEN_02);
            break;
        }
        case (6):
        {
            CreateDashEffect(byMoveFlags, NTL_VID_DASH_ACTIVE_GREEN_01, NTL_VID_DASH_ACTIVE_GREEN_02);
            break;
        }
        case (7):
        {
            CreateDashEffect(byMoveFlags, NTL_VID_DASH_ACTIVE_GREENT_01, NTL_VID_DASH_ACTIVE_GREEN_02);
            break;
        }
        case (8):
        {
            CreateDashEffect(byMoveFlags, NTL_VID_DASH_ACTIVE_GREENT_01, NTL_VID_DASH_ACTIVE_GREEN_02);
            break;
        }
        case (9):
        {
            CreateDashEffect(byMoveFlags, NTL_VID_DASH_ACTIVE_YELLOW_01, NTL_VID_DASH_ACTIVE_YELLOW_02);
            break;
        }
        case (10):
        {
            CreateDashEffect(byMoveFlags, NTL_VID_DASH_ACTIVE_YELLOW_01, NTL_VID_DASH_ACTIVE_YELLOW_02);
            break;
        }
        case (11):
        {
            CreateDashEffect(byMoveFlags, NTL_VID_DASH_ACTIVE_YELLOW_01, NTL_VID_DASH_ACTIVE_YELLOW_02);
            break;
        }
        case (12):
        {
            CreateDashEffect(byMoveFlags, NTL_VID_DASH_ACTIVE_YELLOW_01, NTL_VID_DASH_ACTIVE_YELLOW_02);
            break;
        }
        case (13):
        {
            CreateDashEffect(byMoveFlags, NTL_VID_DASH_ACTIVE_RED_01, NTL_VID_DASH_ACTIVE_RED_02);
            break;
        }
        case (14):
        {
            CreateDashEffect(byMoveFlags, NTL_VID_DASH_ACTIVE_REDT_01, NTL_VID_DASH_ACTIVE_REDT_02);
            break;
        }
        case (15):
        {
            CreateDashEffect(byMoveFlags, NTL_VID_DASH_ACTIVE_PURPLE_01, NTL_VID_DASH_ACTIVE_PURPLE_02);
            break;
        }
        }

        UpdateDashEffect(NTL_MOVE_F, vDir);
    }

    if(uiCurrAnimKey != uiNextAnimKey)
        pSobProxy->SetBaseAnimation(uiNextAnimKey);
}

void CNtlBehaviorCharGroundMove::CreateStartDashCamera(void)
{
    if(m_MoveStuff.byType != NTL_MOVETARGET_DASH)
        return;

    if(m_pActor->GetClassID() != SLCLASS_AVATAR)
        return;

    // camera screen motion blur
    GetNtlGameCameraManager()->GetCamera()->SetEnableBlur(TRUE, 0.9f);

    CNtlSLEventGenerator::CameraDash(m_pActor->GetSerialID(), Logic_GetDashSpeed(m_pActor), m_MoveStuff.vDest);

    m_bDashCameraEnable = TRUE;
}

void CNtlBehaviorCharGroundMove::DestroyEndDashCamera(void)
{
    if(m_pActor->GetClassID() != SLCLASS_AVATAR)
        return;

    if(m_MoveStuff.byType != NTL_MOVETARGET_DASH)
        return;

    if(!m_bDashCameraEnable)
        return;

    // camera screen motion blur
    GetNtlGameCameraManager()->GetCamera()->SetEnableBlur(FALSE);

    CNtlSLEventGenerator::CameraControlDelete(CAMERA_CONTROL_DASH); 
    m_bDashCameraEnable = FALSE;
}

void CNtlBehaviorCharGroundMove::SetDashAnimEnd(void)
{
    if(m_MoveStuff.byType == NTL_MOVETARGET_DASH)
    {
		RwUInt32 uiNextAnimKey;

		/*if(m_MoveStuff.byMoveFlags == NTL_MOVE_F)
            uiNextAnimKey = FLYING_MOVE_FRONT;
        else if(m_MoveStuff.byMoveFlags == NTL_MOVE_B)
            uiNextAnimKey = FLYING_MOVE_BACK;
        else if(m_MoveStuff.byMoveFlags == NTL_MOVE_L)
            uiNextAnimKey = NML_MOVE_LEFT;
        else
            uiNextAnimKey = NML_MOVE_RIGHT;*/

		CNtlSobCharProxy *pSobProxy = reinterpret_cast<CNtlSobCharProxy*>(m_pActor->GetSobProxy());
		pSobProxy->SetBaseAnimation(pSobProxy->GetBaseAnimationKey(), FALSE);
    }
}

//////////////////////////////////////////////////////////////////////////
// tw dead reckoning
//
// A character moved by another client is not simulated from where it is shown. The behavior
// keeps its own prediction that every server move restarts from the server position, and the
// shown position follows the prediction and is pulled onto it, so a correction slides instead
// of teleporting. When an object stops the prediction on this client but its owner reported
// nothing, the same move without object collision (the ghost) takes over after a second.
//////////////////////////////////////////////////////////////////////////

RwBool CNtlBehaviorCharGroundMove::IsDeadReckoning(void)
{
    // the avatar moves itself, everything else follows the server
    return m_pActor->GetClassID() != SLCLASS_AVATAR;
}

void CNtlBehaviorCharGroundMove::ResetDeadReckoning(RwBool bSnapDir)
{
    if(!IsDeadReckoning())
        return;

    RwV3d vActorPos = m_pActor->GetPosition();
    RwV3d vSeedPos = m_MoveStuff.vCurrLoc;

    // some move events leave the current location empty
    RwV3d vSub = vSeedPos - vActorPos;
    if((vSeedPos.x == 0.0f && vSeedPos.z == 0.0f) || RwV3dLength(&vSub) > MOVE_PREDICT_SEED_RANGE)
        vSeedPos = vActorPos;

    m_vPredictPos	= vSeedPos;
    m_vGhostPos		= vSeedPos;
    m_fGhostTime	= 0.0f;

    RwV3d vDir = m_MoveStuff.vDir;
    vDir.y = 0.0f;
    if(RwV3dLength(&vDir) > 0.001f)
        RwV3dNormalize(&m_vPredictDir, &vDir);
    else
        m_vPredictDir = m_pActor->GetDirection();

    // a new move measures the turn from its heading again
    m_fYawRate				= 0.0f;
    m_fYawBudget			= 0.0f;
    m_fYawExtrapolateTime	= 0.0f;
    m_vYawSampleDir			= m_vPredictDir;
    m_fYawSampleTime		= 0.0f;

    // a move that starts faces its way at once, a changed move key turns the shown facing smoothly
    if(bSnapDir && m_MoveStuff.byType == NTL_MOVETARGET_DIR)
        m_pActor->SetDirection(&m_vPredictDir);
}

RwBool CNtlBehaviorCharGroundMove::BeginDeadReckoning(OUT RwV3d& vPos, OUT RwV3d& vDir)
{
    RwBool bMoveGhost = FALSE;

    if(m_fGhostTime > MOVE_GHOST_SNAP_TIME)
    {
        // an object held the prediction back for a while, its owner went on without it
        if(!m_bMoveCollision)
            m_vPredictPos = m_vGhostPos;

        m_fGhostTime = 0.0f;
    }
    else if(m_fGhostTime >= 0.0001f)
    {
        bMoveGhost = TRUE;
    }

    // the prediction lost track (far from where the character is shown), start over from there
    RwV3d vShown = m_pActor->GetPosition();
    RwV3d vSub = m_vPredictPos - vShown;
    if(RwV3dLength(&vSub) > MOVE_PREDICT_SEED_RANGE)
    {
        m_vPredictPos = vShown;
        m_vGhostPos = vShown;
        bMoveGhost = FALSE;
    }

    vPos = m_vPredictPos;
    vDir = m_vPredictDir;

    return bMoveGhost;
}

void CNtlBehaviorCharGroundMove::MoveGhost(const RwV3d& vDir, RwReal fSpeed, RwReal fElapsed)
{
    CNtlVector vHeading, vDest;
    NtlGetDestination_Keyboard(vDir.x, vDir.y, vDir.z, fSpeed, m_vGhostPos.x, m_vGhostPos.y, m_vGhostPos.z, m_MoveStuff.byMoveFlags, fElapsed * 1000.f, 1.0f, &vHeading, &vDest);

    m_vGhostPos.x = vDest.x;
    m_vGhostPos.z = vDest.z;

    SWorldHeightStuff sHStuff;
    Logic_GetWorldHeight(m_pActor, &m_vGhostPos, sHStuff);
    m_vGhostPos.y = sHStuff.fFinialHeight;
}

void CNtlBehaviorCharGroundMove::EndDeadReckoning(const RwV3d& vOldPos, const RwV3d& vNewPos, const RwV3d& vNewDir, RwReal fSpeed, RwReal fElapsed, RwBool bBlocked, RwBool bMoveGhost)
{
    // count how long an object holds the prediction back (not while its owner reports the same)
    if(bBlocked)
        m_fGhostTime = m_bMoveCollision ? 0.0f : m_fGhostTime + fElapsed;

    // the shown position makes the same step as the prediction, towards it
    RwV3d vShown = m_pActor->GetPosition();
    RwV3d vStep = vNewPos - vOldPos;
    RwReal fStep = RwV3dLength(&vStep);

    RwV3d vFollowDir = vNewPos - vShown;
    RwV3d vDisplay = vShown;
    if(RwV3dLength(&vFollowDir) > 0.0001f)
    {
        RwV3dNormalize(&vFollowDir, &vFollowDir);
        vDisplay = vShown + vFollowDir * fStep;
    }
    else
    {
        vFollowDir = ZeroAxis;
    }

    // and is pulled onto it
    RwV3d vRemainDir = vNewPos - vDisplay;
    RwReal fRemain = RwV3dLength(&vRemainDir);
    if(fRemain > MOVE_SMOOTH_MIN_DIST)
    {
        RwV3dNormalize(&vRemainDir, &vRemainDir);

        RwReal fRate = RwV3dDotProduct(&vFollowDir, &vRemainDir) < 0.0f ? MOVE_SMOOTH_RATE_BACK : MOVE_SMOOTH_RATE;

        RwReal fPullSpeed = fSpeed;
        if(m_MoveStuff.byType == NTL_MOVETARGET_DIR && m_bMoveCollision && fRemain < MOVE_SMOOTH_COLLISION_DIST)
            fPullSpeed = fSpeed * max(m_fForcedSpeed, MOVE_SMOOTH_COLLISION_MIN_RATE);

        RwReal fPull = fPullSpeed * fRate * fElapsed * fRemain;
        if(fPull > fRemain)
            fPull = fRemain;

        vDisplay = vDisplay + vRemainDir * fPull;
    }

    SWorldHeightStuff sHStuff;
    Logic_GetWorldHeight(m_pActor, &vDisplay, sHStuff);
    if(sHStuff.fFinialHeight > -500.f)
        vDisplay.y = sHStuff.fFinialHeight;

    m_pActor->SetPosition(&vDisplay);
    SetShownDirection(vNewDir, fElapsed);

    m_vPredictPos = vNewPos;
    m_vPredictDir = vNewDir;

    if(!bMoveGhost)
        m_vGhostPos = vNewPos;
}

void CNtlBehaviorCharGroundMove::OnMoveCollision(const SNtlEventSobMoveCollision* pCollision)
{
    // the avatar is the one reporting it
    if(!IsDeadReckoning())
        return;

    RwV3d vDir = pCollision->vDir;
    vDir.y = 0.0f;
    if(RwV3dLength(&vDir) > 0.001f)
        RwV3dNormalize(&vDir, &vDir);
    else
        vDir = m_vPredictDir;

    RwV3d vDirForced = pCollision->vDirForced;
    vDirForced.y = 0.0f;
    if(RwV3dLength(&vDirForced) > 0.001f)
        RwV3dNormalize(&vDirForced, &vDirForced);
    else
        vDirForced = vDir;

    m_MoveStuff.vDir	= vDir;
    m_vPredictDir		= vDir;
    m_vPredictPos		= pCollision->vLoc;
    m_vGhostPos			= pCollision->vLoc;
    m_pActor->SetDirection(&m_vPredictDir);

    m_bMoveCollision	= TRUE;
    m_fForcedSpeed		= pCollision->fForcedSpeed;
    m_vForcedDir		= vDirForced;
}

void CNtlBehaviorCharGroundMove::OnMoveCollisionEnd(const SNtlEventSobMoveCollisionEnd* pCollisionEnd)
{
    if(!IsDeadReckoning())
        return;

    m_vPredictPos		= pCollisionEnd->vLoc;
    m_vGhostPos			= pCollisionEnd->vLoc;
    m_bMoveCollision	= FALSE;
    m_fForcedSpeed		= 1.0f;
}

// tw (0x60AF20): GU_CHAR_CHANGE_HEADING turns the move as well as the actor
void CNtlBehaviorCharGroundMove::OnChangeHeading(const RwV3d& vDir)
{
    RwV3d vHeading = vDir;
    vHeading.y = 0.0f;
    if(RwV3dLength(&vHeading) <= 0.001f)
        return;

    RwV3dNormalize(&vHeading, &vHeading);

    m_MoveStuff.vDir = vHeading;

    if(IsDeadReckoning())
    {
        // the shown facing eases after the prediction
        m_vPredictDir = vHeading;
        OnHeadingSample(vHeading);
        return;
    }

    m_pActor->SetDirection(&vHeading);
}

// Position the moving player sent a moment ago (relayed by the server). Not in tw: without it
// a player steering with the mouse drifts away from where the others see it, every turn reaches
// them a little late and nothing corrects it until the next move key changes. The prediction is
// moved there, the shown position slides after it.
void CNtlBehaviorCharGroundMove::OnMoveSync(const RwV3d& vLoc, const RwV3d& vDir)
{
    if(!IsDeadReckoning())
        return;

    m_vPredictPos	= vLoc;
    m_vGhostPos		= vLoc;
    m_fGhostTime	= 0.0f;

    if(m_MoveStuff.byType == NTL_MOVETARGET_DIR)
    {
        RwV3d vHeading = vDir;
        vHeading.y = 0.0f;
        if(RwV3dLength(&vHeading) > 0.001f)
        {
            RwV3dNormalize(&vHeading, &vHeading);
            m_MoveStuff.vDir	= vHeading;
            m_vPredictDir		= vHeading;
            OnHeadingSample(vHeading);
        }
    }
}

static RwReal Move_GetYaw(const RwV3d& vDir)
{
    return atan2f(vDir.x, vDir.z);
}

static RwReal Move_WrapAngle(RwReal fAngle)
{
    while(fAngle > rwPI)
        fAngle -= 2.0f * rwPI;
    while(fAngle < -rwPI)
        fAngle += 2.0f * rwPI;

    return fAngle;
}

static RwV3d Move_GetYawDir(RwReal fYaw)
{
    RwV3d vDir;
    vDir.x = sinf(fYaw);
    vDir.y = 0.0f;
    vDir.z = cosf(fYaw);

    return vDir;
}

static RwBool Move_IsKeyboardTurn(RwUInt8 byMoveFlags)
{
    return byMoveFlags >= NTL_MOVE_TURN_L && byMoveFlags <= NTL_MOVE_B_TURN_R_JUMP;
}

// A heading of another client's character arrived (change heading or position sync). The turn
// speed is measured between headings far enough apart, the prediction then keeps turning at it
// for a moment: the headings come every 0.1 - 0.3 seconds while the mouse turns the character,
// followed one by one the character ran a polygon and was pulled back onto the curve each time.
void CNtlBehaviorCharGroundMove::OnHeadingSample(const RwV3d& vDir)
{
    if(m_fYawSampleTime >= MOVE_YAW_SAMPLE_MIN_TIME)
    {
        RwReal fRate = 0.0f;
        if(m_fYawSampleTime <= MOVE_YAW_SAMPLE_MAX_TIME && RwV3dLength(&m_vYawSampleDir) > 0.001f)
        {
            RwReal fDelta = Move_WrapAngle(Move_GetYaw(vDir) - Move_GetYaw(m_vYawSampleDir));
            if(fabs(fDelta) > MOVE_YAW_MIN_DELTA)
                fRate = fDelta / m_fYawSampleTime;
        }

        m_fYawRate			= fRate;
        m_vYawSampleDir		= vDir;
        m_fYawSampleTime	= 0.0f;
    }

    m_fYawExtrapolateTime	= 0.0f;
    m_fYawBudget			= min((RwReal)fabs(m_fYawRate) * MOVE_YAW_EXTRAPOLATE_TIME, MOVE_YAW_MAX_EXTRAPOLATE);
}

void CNtlBehaviorCharGroundMove::ExtrapolateHeading(RwV3d& vDir, RwReal fElapsed)
{
    m_fYawSampleTime += fElapsed;

    // a keyboard turn is in the move flags already, an object in the way sets the way itself
    if(Move_IsKeyboardTurn(m_MoveStuff.byMoveFlags) || m_bMoveCollision)
        return;

    if(m_fYawRate == 0.0f || m_fYawBudget <= 0.0f || m_fYawExtrapolateTime >= MOVE_YAW_EXTRAPOLATE_TIME)
        return;

    RwReal fStep = m_fYawRate * fElapsed;
    if(fabs(fStep) > m_fYawBudget)
        fStep = fStep > 0.0f ? m_fYawBudget : -m_fYawBudget;

    m_fYawBudget			-= (RwReal)fabs(fStep);
    m_fYawExtrapolateTime	+= fElapsed;

    vDir = Move_GetYawDir(Move_GetYaw(vDir) + fStep);
    m_vPredictDir = vDir;
}

// the shown facing turns after the predicted one instead of jumping to every heading
void CNtlBehaviorCharGroundMove::SetShownDirection(const RwV3d& vDir, RwReal fElapsed)
{
    RwV3d vTarget = vDir;
    vTarget.y = 0.0f;
    RwV3d vCurr = m_pActor->GetDirection();
    vCurr.y = 0.0f;

    if(RwV3dLength(&vTarget) <= 0.001f)
        return;

    if(RwV3dLength(&vCurr) <= 0.001f)
    {
        m_pActor->SetDirection(&vTarget);
        return;
    }

    RwReal fCurrYaw = Move_GetYaw(vCurr);
    RwReal fDelta = Move_WrapAngle(Move_GetYaw(vTarget) - fCurrYaw);
    if(fabs(fDelta) < 0.01f)
    {
        RwV3dNormalize(&vTarget, &vTarget);
        m_pActor->SetDirection(&vTarget);
        return;
    }

    RwReal fRate = 1.0f - expf(-MOVE_TURN_SMOOTH_RATE * fElapsed);
    RwV3d vShown = Move_GetYawDir(fCurrYaw + fDelta * fRate);
    m_pActor->SetDirection(&vShown);
}

void CNtlBehaviorCharGroundMove::UpdateLocationSync(RwReal fElapsed)
{
    if(m_pActor->GetClassID() != SLCLASS_AVATAR)
        return;

    if(m_MoveStuff.byType != NTL_MOVETARGET_DIR && m_MoveStuff.byType != NTL_MOVETARGET_LOC)
        return;

    m_fLocationSyncTime += fElapsed;

    RwV3d vDir = m_pActor->GetDirection();

    // a curve run with the mouse sends its heading only every 10 degrees, the others fall behind
    // on the inside of it; follow it closer
    RwV3d vFlatDir = vDir;
    RwV3d vFlatSent = m_vLocationSyncDir;
    vFlatDir.y = 0.0f;
    vFlatSent.y = 0.0f;
    RwBool bTurned = FALSE;
    RwBool bSentDir = FALSE;
    RwBool bTurning = FALSE;
    if(RwV3dLength(&vFlatDir) > 0.001f)
    {
        RwV3dNormalize(&vFlatDir, &vFlatDir);

        if(RwV3dLength(&vFlatSent) > 0.001f)
        {
            RwV3dNormalize(&vFlatSent, &vFlatSent);
            RwReal fSentDot = RwV3dDotProduct(&vFlatDir, &vFlatSent);
            bTurned = fSentDot < MOVE_LOCATION_SYNC_TURN_COS;
            bSentDir = fSentDot >= MOVE_TURNING_COS;
        }

        // still turning this frame?
        if(RwV3dLength(&m_vLastFrameDir) > 0.001f)
            bTurning = RwV3dDotProduct(&vFlatDir, &m_vLastFrameDir) < MOVE_TURNING_COS;

        m_vLastFrameDir = vFlatDir;
    }

    m_fNoTurnTime = bTurning ? 0.0f : m_fNoTurnTime + fElapsed;

    // the turn stopped: where it ended is sent at once, the others stop turning the character
    RwBool bTurnEnd = FALSE;
    if(m_bTurnSyncPending && m_fNoTurnTime >= MOVE_LOCATION_SYNC_STOPTURN_TIME)
    {
        if(bSentDir)
            m_bTurnSyncPending = FALSE;
        else
            bTurnEnd = TRUE;
    }

    if(m_fLocationSyncTime < MOVE_LOCATION_SYNC_TIME && !(bTurned && m_fLocationSyncTime >= MOVE_LOCATION_SYNC_TURN_TIME) && !bTurnEnd)
        return;

    m_fLocationSyncTime = 0.0f;
    m_vLocationSyncDir = vDir;
    m_bTurnSyncPending = bTurning || (bTurned && !bTurnEnd);

    API_GetSLPacketGenerator()->SendChangePosition(m_pActor->GetPosition(), vDir);
}

//////////////////////////////////////////////////////////////////////////
// tw collision report: the avatar compares the step it should make with the step the object
// collision left it. While they differ it reports the position and how much of the step is
// left (immediately when it stops completely, else every MOVE_COLLISION_CHECK_TIME), the
// other clients move it the same way. When the steps match again the collision ends.
//////////////////////////////////////////////////////////////////////////

void CNtlBehaviorCharGroundMove::UpdateCollisionReport(const RwV3d& vOldPos, const RwV3d& vMovePos, const RwV3d& vFinalPos, const RwV3d& vDir, RwReal fElapsed)
{
    if(m_pActor->GetClassID() != SLCLASS_AVATAR)
        return;

    m_fCollisionCheckTime += fElapsed;
    if(m_fCollisionCheckTime > MOVE_COLLISION_CHECK_TIME)
        m_fCollisionCheckTime = MOVE_COLLISION_CHECK_TIME;

    // compare on the ground plane, walking onto an object changes the height of the step only
    RwV3d vMove = vMovePos - vOldPos;
    RwV3d vMoved = vFinalPos - vOldPos;
    vMove.y = 0.0f;
    vMoved.y = 0.0f;
    RwReal fMove = RwV3dLength(&vMove);
    RwReal fMoved = RwV3dLength(&vMoved);

    if(fabs(fMove - fMoved) <= 0.001f)
    {
        EndCollisionReport();
        return;
    }

    RwReal fRate = fMove >= 0.0001f ? fMoved / fMove : 0.0f;

    if((fRate < 0.0001f && m_fLastCollisionRate >= 0.0001f) || m_fCollisionCheckTime >= MOVE_COLLISION_CHECK_TIME)
    {
        API_GetSLPacketGenerator()->SendCharMoveCollision(vFinalPos, vDir, fRate);

        m_fLastCollisionRate	= fRate;
        m_bCollisionSent		= TRUE;
        m_fCollisionCheckTime	= 0.0f;
    }
}

void CNtlBehaviorCharGroundMove::EndCollisionReport(void)
{
    if(!m_bCollisionSent)
        return;

    API_GetSLPacketGenerator()->SendCharMoveCollisionEnd(m_pActor->GetPosition(), m_pActor->GetDirection());

    m_bCollisionSent		= FALSE;
    m_fCollisionCheckTime	= MOVE_COLLISION_CHECK_TIME;
    m_fLastCollisionRate	= RwRealMAXVAL;
}

RwBool CNtlBehaviorCharGroundMove::UpdateDirectionMove(RwReal fElapsed)
{
    RwBool bDeadReckoning = IsDeadReckoning();
    RwBool bMoveGhost = FALSE;

    RwV3d vPos, vDir;
    if(bDeadReckoning)
    {
        bMoveGhost = BeginDeadReckoning(vPos, vDir);
        ExtrapolateHeading(vDir, fElapsed);
    }
    else
    {
        vPos = m_pActor->GetPosition();
        vDir = m_pActor->GetDirection();
    }

    RwV3d vOldPos = vPos;
    RwReal fOldActorHeight = vPos.y;
    RwV3d vNewPos, vNewDir;

    RwReal fSpeed	= Logic_GetFrontRunSpeed(m_pActor);
    m_fAnimSpeed	= 1.0f;

	RwBool bTurn = FALSE;

    if(m_MoveStuff.byMoveFlags == NTL_MOVE_TURN_L ||  m_MoveStuff.byMoveFlags == NTL_MOVE_TURN_R)
    {
        bTurn = TRUE;
    }
    else
    {
        m_fAnimSpeed	= fSpeed / m_fDefaultRunSpeed;
    }

    // tw: while its client reports an object in the way, the character moves the reported way, slower
    RwV3d vMoveDir = vDir;
    RwReal fMoveSpeed = fSpeed;
    if(bDeadReckoning && m_bMoveCollision)
    {
        vMoveDir = m_vForcedDir;
        fMoveSpeed = fSpeed * m_fForcedSpeed;
    }

    //Handles keyboard movement
    CNtlVector vHeading, vDest;
    NtlGetDestination_Keyboard(vMoveDir.x, vMoveDir.y, vMoveDir.z, fMoveSpeed, vPos.x, vPos.y, vPos.z, m_MoveStuff.byMoveFlags, fElapsed * 1000.f, 1.0f, &vHeading, &vDest);

    CNtlMath::MathRwV3dAssign(&vNewPos, vDest.x, vPos.y, vDest.z);
    CNtlMath::MathRwV3dAssign(&vNewDir, vHeading.x, 0.0f, vHeading.z);

    if(bMoveGhost)
        MoveGhost(vMoveDir, fMoveSpeed, fElapsed);

    Logic_GetWorldHeight(m_pActor, &vNewPos, m_sHStuff);
    vNewPos.y = m_sHStuff.fFinialHeight;

    RwV3d vMovePos = vNewPos;
    RwUInt8 byColliResult = NTL_CHARACTER_COLLI_NONE;
    RwBool bBlocked = FALSE;
    if(!bTurn)
    {
		if (ObjectCollision(vNewPos, fOldActorHeight, fSpeed, fElapsed, &byColliResult))
		{
			CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData());
			SMoveStuff *pMoveStuff = pBeData->GetMoveStuff();

			// falling ends the move, so does an object stopping the avatar
			if(!bDeadReckoning || (pMoveStuff->byMoveResult & NTL_MOVE_RESULT_FALLING))
			{
				if (byColliResult != NTL_CHARACTER_COLLI_NONE)
					pMoveStuff->byMoveResult |= NTL_MOVE_RESULT_COLLISION;

				return TRUE;
			}

			// an object stops the character on this client only, its server move goes on
			pMoveStuff->byMoveResult &= ~NTL_MOVE_RESULT_COLLISION;
			vNewPos = vOldPos;
			bBlocked = TRUE;
		}
		else if(byColliResult != NTL_CHARACTER_COLLI_NONE)
		{
			bBlocked = TRUE;
		}
    }

    if(bDeadReckoning)
    {
        EndDeadReckoning(vOldPos, vNewPos, vNewDir, fSpeed, fElapsed, bBlocked, bMoveGhost);
        return FALSE;
    }

    m_pActor->SetPosition(&vNewPos);
    m_pActor->SetDirection(&vNewDir);

    if(!bTurn)
        UpdateCollisionReport(vOldPos, vMovePos, vNewPos, vNewDir, fElapsed);

    // 일정 시간 동안 이동한 거리가 limit 거리 안에 있으면? 멈춘다.
    if( !bTurn && byColliResult != NTL_CHARACTER_COLLI_WORLD_ATTR_TERRAIN_SLANT)
    {
        LimitPositionChangeCheck(fElapsed);
    }

    return FALSE;
}

RwBool CNtlBehaviorCharGroundMove::UpdateLocationMove(RwReal fElapsed)
{
    //-------------------------------------
    // speed 占쏙옙 anim speed 占쏙옙占쏙옙.
    RwBool bWalkMove = Logic_IsActorWalkMove(m_pActor, m_MoveStuff.byFormFlag);
    RwReal fSpeed; 
    if(bWalkMove)
    {
        fSpeed			= Logic_GetFrontWalkSpeed(m_pActor);
        m_fAnimSpeed	= fSpeed / m_fDefaultWalkSpeed;
    }
    else
    {
        fSpeed			= Logic_GetFrontRunSpeed(m_pActor);
        m_fAnimSpeed	= fSpeed / m_fDefaultRunSpeed;
    }

    //-------------------------------------
    // Obtain the current distance.

    RwBool bDeadReckoning = IsDeadReckoning();

    RwV3d vPos;
    if(bDeadReckoning)
    {
        RwV3d vPredictDir;
        BeginDeadReckoning(vPos, vPredictDir);
    }
    else
    {
        vPos = m_pActor->GetPosition();
    }

    RwV3d vOldPos = vPos;
    RwReal fOldActorHeight = vPos.y;
    RwV3d vDestPos = m_MoveStuff.vDest;
    RwV3d vDir = vDestPos - vPos;
    vDir.y = 0.0f;
    RwReal fCurrLen = RwV3dLength(&vDir);
    if(fCurrLen > 0.001f)
        RwV3dNormalize(&vDir, &vDir);
    else
        RwV3dAssignMacro(&vDir, &m_pActor->GetDirection()); // Keep current direction if at destination
    RwV3d vDelta = vDir * fElapsed * fSpeed;

    //-------------------------------------
    // The current remaining distance is less than the next distance to travel.
    if(fCurrLen <= RwV3dLength(&vDelta))
    {
        RwUInt8 bColliResult = NTL_CHARACTER_COLLI_NONE;
        if(fElapsed >= 0.5f)
        {
            if(m_pActor->GetFlags() & SLFLAG_OBJECT_COLLISION)
            {
                RwV3dAssignMacro(&vPos, &vDestPos);
                RwBool bCollMoveImPossible;
                bColliResult = Logic_CharacterCollisionEx(m_pActor, &vPos, fSpeed, vPos, bCollMoveImPossible, fElapsed);

                if(bColliResult == NTL_CHARACTER_COLLI_OBJECT)
                {
                    if(bCollMoveImPossible)
                    {
                        ++m_byCollMoveImpossCnt;
                    }
                }
            }
        }

        Logic_GetWorldHeight(m_pActor, &vPos, m_sHStuff);
        // Only apply terrain height if valid (not the -999 sentinel when sector is unloaded)
        if(m_sHStuff.fFinialHeight > -500.f)
            vPos.y = m_sHStuff.fFinialHeight;

        if(bDeadReckoning)
            EndDeadReckoning(vOldPos, vPos, vDir, fSpeed, fElapsed, FALSE, FALSE);
        else
            m_pActor->SetPosition(&vPos);

        if(bColliResult == NTL_CHARACTER_COLLI_OBJECT)
        {
            // If there is a collision but neither movement is possible, it stops.
            if(m_byCollMoveImpossCnt >= COLLISION_MOVE_IMPOSSIBLE_COUNT)
            {
                CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData()); 
                pBeData->GetMoveStuff()->byMoveResult |= NTL_MOVE_RESULT_COLLISION;                


                return TRUE;
            }

            LimitPositionChangeCheck(fElapsed);

            return FALSE;
        }

        if(PopPathNode(m_MoveStuff.vDest))
            return FALSE;

        // the move ends, the shown position stops where the server put the character
        if(bDeadReckoning)
            m_pActor->SetPosition(&vPos);

        return TRUE;
    }
    else // The current remaining distance is not less than the next travel distance.
    {
        vPos += vDelta;

        Logic_GetWorldHeight(m_pActor, &vPos, m_sHStuff);
        // Only apply terrain height if valid (not the -999 sentinel when sector is unloaded)
        if(m_sHStuff.fFinialHeight > -500.f)
            vPos.y = m_sHStuff.fFinialHeight;

        RwV3d vMovePos = vPos;

        // object 충돌 처리.
        RwUInt8 byColliResult = NTL_CHARACTER_COLLI_NONE;
        if (ObjectCollision(vPos, fOldActorHeight, fSpeed, fElapsed, &byColliResult))
		{
			if (byColliResult != NTL_CHARACTER_COLLI_NONE)
			{
				CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData());

				SMoveStuff *pMoveStuff = pBeData->GetMoveStuff();
				pMoveStuff->byMoveResult |= NTL_MOVE_RESULT_COLLISION;// NTL_MOVE_RESULT_FALLING; // BY DANEOS

				// by daneos
				/*SCtrlStuff *pCtrlStuff = pBeData->GetCtrlStuff();

				pCtrlStuff->uExtra.sFalling.vFallingDir = m_pActor->GetDirection();

				pCtrlStuff->uExtra.sFalling.byMoveFlags = m_MoveStuff.byMoveFlags;
				pCtrlStuff->uExtra.sFalling.fSpeed = (m_MoveStuff.byMoveFlags == NTL_MOVE_B) ? fSpeed * NTL_BACKWARD_MOVEMENT_SPEED_RATE : fSpeed;
				pCtrlStuff->uExtra.sFalling.fSearchHeight = m_sHStuff.fFinialHeight;*/
			}

			return TRUE;
		}

        if(bDeadReckoning)
        {
            EndDeadReckoning(vOldPos, vPos, vDir, fSpeed, fElapsed, FALSE, FALSE);
        }
        else
        {
            m_pActor->SetDirection(&vDir);
            m_pActor->SetPosition(&vPos);

            UpdateCollisionReport(vOldPos, vMovePos, vPos, vDir, fElapsed);
        }

        if(byColliResult == NTL_CHARACTER_COLLI_NONE)
            return FALSE;

        // If the distance traveled within a certain time is within the limit distance? It stops.
        if(byColliResult != NTL_CHARACTER_COLLI_WORLD_ATTR_TERRAIN_SLANT)
        {
            LimitPositionChangeCheck(fElapsed);
        }

        return FALSE;
    }
}

RwBool CNtlBehaviorCharGroundMove::UpdateTargetMove(RwReal fElapsed)
{
    CNtlSobActor *pTargetActor = reinterpret_cast<CNtlSobActor*>(GetNtlSobManager()->GetSobObject(m_MoveStuff.hFollowSerialId));
    if(pTargetActor == NULL)
        return TRUE;

    //RwReal fFollowRange = m_MoveStuff.fFollowRange * FOLLOW_ERROR_RANGE; // I do not know why I multiply by 0.9. This causes problems with skill activation distance. (by agebreak 2009.08.5)
    RwReal fFollowRange = m_MoveStuff.fFollowRange;

    RwV3d vPos = m_pActor->GetPosition();
    RwV3d vDestPos = pTargetActor->GetPosition(); 
    RwReal fOldActorHeight = vPos.y;

    // ���� ���� ���� ���� ����ٴϴ� ��ü�� ���.
    if(pTargetActor->GetFlags() & SLFLAG_CAN_GROUND_FLY)
        vDestPos.y -= Logic_GetGroundFlyHeight(pTargetActor);

    // If the target is an actor to swim, ignore y
	SWorldHeightStuff hStuff;
	if(Logic_IsSwimmingActor(pTargetActor, &pTargetActor->GetPosition(), hStuff))
	{
		vDestPos.y = vPos.y = 0.0f;
	}

    RwV3d vDir = vDestPos - vPos;	    

    // Obtain the current distance.
    RwReal fCurrLen = RwV3dLength(&vDir);
    vDir.y = 0.0f;
    RwV3dNormalize(&vDir, &vDir);

    m_pActor->SetDirection(&vDir);

    if(fCurrLen < fFollowRange)
    {
        vPos.x = vDestPos.x - vDir.x*fFollowRange;
        vPos.z = vDestPos.z - vDir.z*fFollowRange;

        Logic_GetWorldHeight(m_pActor, &vPos, m_sHStuff);
        vPos.y = m_sHStuff.fFinialHeight;

        m_pActor->SetPosition(&vPos);

        return TRUE;
    }

    RwReal fSpeed = SLOW_MOVE_SPEED;
    if(!m_bSlowMove)
    {
        RwBool bWalkMove = Logic_IsActorWalkMove(m_pActor, m_MoveStuff.byFormFlag);
        if(bWalkMove)
        {
            fSpeed			= Logic_GetFrontWalkSpeed(m_pActor);
            m_fAnimSpeed	= fSpeed / m_fDefaultWalkSpeed;
        }
        else
        {
            fSpeed			= Logic_GetFrontRunSpeed(m_pActor);
            m_fAnimSpeed	= fSpeed / m_fDefaultRunSpeed;
        }
    }

    RwV3d vDelta = vDir * fSpeed * fElapsed;	

    // The current remaining distance is less than the moving delta distance.
    RwReal fDeltaLen = RwV3dLength(&vDelta);
    if(fCurrLen < fDeltaLen + fFollowRange)
    {
        vPos.x = vDestPos.x - vDir.x*fFollowRange;
        vPos.z = vDestPos.z - vDir.z*fFollowRange;

        Logic_GetWorldHeight(m_pActor, &vPos, m_sHStuff);
        vPos.y = m_sHStuff.fFinialHeight;

        m_pActor->SetPosition(&vPos);
        return TRUE;
    }

    // 占쏙옙표 update
    vPos += vDelta;    

    Logic_GetWorldHeight(m_pActor, &vPos, m_sHStuff);
    vPos.y = m_sHStuff.fFinialHeight;

    // slow move일 경우.
    if(m_bSlowMove)
    {
        m_fSlowMoveTime += fElapsed;
        if(m_fSlowMoveTime > SLOW_MOVE_TIME)
            return TRUE;
    }

    // �浹 üũ.
    RwUInt8 byColliResult = NTL_CHARACTER_COLLI_NONE;

    if(m_pActor->GetFlags() & SLFLAG_OBJECT_COLLISION)
    {
        RwBool bCollMoveImPossible;
        byColliResult = Logic_CharacterCollisionEx(m_pActor, &vPos, fSpeed, vPos, bCollMoveImPossible, fElapsed);

        if(byColliResult == NTL_CHARACTER_COLLI_OBJECT)
        {
            if(bCollMoveImPossible)
                m_byCollMoveImpossCnt++;
            else
                m_byCollMoveImpossCnt = 0;

            vPos.y = m_sHStuff.fFinialHeight;
        }
        else if(byColliResult == NTL_CHARACTER_COLLI_WORLD_ATTR_TERRAIN_SLANT)
        {
            if(m_byDashState == DASH_STATE_MOVE)
            {
                SetDashAnimEnd();
                m_byDashState = DASH_STATE_END;
                return FALSE;
            }
            else
            {
                return TRUE;
            }
        }

        // If there is a collision but neither movement is possible, it stops.
        if(m_byCollMoveImpossCnt >= COLLISION_MOVE_IMPOSSIBLE_COUNT)
        {
            CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData()); 
            SMoveStuff *pMoveStuff = pBeData->GetMoveStuff();
            pMoveStuff->byMoveResult |= NTL_MOVE_RESULT_COLLISION;

            return TRUE;
        }
    }

    // 占쏙옙占쏙옙 체크
    if(CheckFalling(fOldActorHeight, m_sHStuff.fFinialHeight, fSpeed, NTL_MOVE_F))
        return TRUE;

    m_pActor->SetPosition(&vPos);

    if(byColliResult == NTL_CHARACTER_COLLI_NONE)
        return FALSE;

    // ���� �ð� ���� �̵��� �Ÿ��� limit �Ÿ� �ȿ� ������? �����.
    if(byColliResult != NTL_CHARACTER_COLLI_WORLD_ATTR_TERRAIN_SLANT)
    {
        LimitPositionChangeCheck(fElapsed);
    }

    return FALSE;
}

RwBool CNtlBehaviorCharGroundMove::UpdateDashMove(RwReal fElapsed)
{
    if(m_byDashState == DASH_STATE_END)
    {
        m_fEffectSmokeTime += fElapsed;
        if(m_fEffectSmokeTime > 0.1f)
        {
            DestroyDashEffect();
            DestroyDashEffectSmoke();
        }

        return TRUE;
    }

    RwV3d vPos = m_pActor->GetPosition();
    CNtlVector vNewPos;

    RwV3d vDestPos = m_MoveStuff.vDest;    

    
    RwV3d vDir = vDestPos - vPos;
    RwV3d v2dSub = vDir;
    v2dSub.y = 0.0f;    

    RwReal f2dCurrLen = RwV3dLength(&v2dSub);
    RwReal fCurrLen = RwV3dLength(&vDir);

    if(m_byDashState == DASH_STATE_MOVE)
    {
        if(fCurrLen <= m_fDashLadingDist)
        {
			m_byDashState = DASH_STATE_LANDING;
            SetDashAnimEnd();
        }
    }

	float fDashSpeed = Logic_GetDashSpeed(m_pActor);

    if(f2dCurrLen <= NTL_EPSILON || fCurrLen <= fDashSpeed * fElapsed)
    {
		// The defense code when a lot of elapsed time comes because of loading.
		// Defending code for height
		// The height is determined by the current coordinates of character + height.

        RwBool bSuccess = FALSE;
        RwUInt8 byColliResult = NTL_CHARACTER_COLLI_NONE;
        if(fElapsed >= 0.5f)
        {
            if(m_pActor->GetFlags() & SLFLAG_OBJECT_COLLISION)
            {
                RwV3dAssignMacro(&vPos, &vDestPos);
                RwBool bCollMoveImPossible;

                byColliResult = Logic_CharacterCollisionEx(m_pActor, &vPos, fDashSpeed, vPos, bCollMoveImPossible, fElapsed);

                if(byColliResult == NTL_CHARACTER_COLLI_OBJECT)
                {
                    if(bCollMoveImPossible)
                        m_byCollMoveImpossCnt++;
                }
            }
        }

        Logic_GetWorldHeight(m_pActor, &vPos, m_sHStuff);
        vPos.y = m_sHStuff.fFinialHeight;
        m_MoveStuff.vDest.y = m_sHStuff.fFinialHeight;
        vDir.y = 0.0f;

        m_pActor->SetPosition(&vPos);

        if(byColliResult == NTL_CHARACTER_COLLI_OBJECT)
        {
            // If there is a collision but neither movement is possible, it stops.
            if(m_byCollMoveImpossCnt >= COLLISION_MOVE_IMPOSSIBLE_COUNT)
            {
                CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData()); 
                SMoveStuff *pMoveStuff = pBeData->GetMoveStuff();
                pMoveStuff->byMoveResult |= NTL_MOVE_RESULT_COLLISION;

                bSuccess = TRUE;
            }
            else
            {
                LimitPositionChangeCheck(fElapsed);
                bSuccess = FALSE;
            }

            return FALSE;
        }
        else
        {
            bSuccess = TRUE;
        }

        if(bSuccess)
        {
            if(m_byDashState == DASH_STATE_MOVE)
            {
                SetDashAnimEnd();
            }

            DestroyEndDashCamera();

            m_byDashState = DASH_STATE_END;
        }
    }

    RwV3dNormalize(&vDir, &vDir); 
    RwReal fSpeed = fDashSpeed * fElapsed;

    // data�� �ε��Ͽ� ���ڱ� elapsed time�� ũ�� ���� ��� �̷� ��찡 �����
    if(fSpeed >= fCurrLen)
    {
        fSpeed = fCurrLen;
        vPos += vDir * fSpeed;
        vPos.y = max(m_MoveStuff.vDest.y, vPos.y);        
    }
    else
    {
        vPos += vDir * fSpeed;        
        RwReal fCurrPosY = vPos.y;

        Logic_GetWorldHeight(m_pActor, &vPos, m_sHStuff);
        vPos.y = max(m_sHStuff.fFinialHeight, vPos.y);        

        if(fCurrPosY < vPos.y && m_MoveStuff.vDest.y < vPos.y)
        {
            m_MoveStuff.vDest.y = vPos.y + fabs(fCurrPosY - vPos.y);
        }
        else
        {
            m_MoveStuff.vDest.y = m_fDashDestPosY;
        }
    }

    RwUInt8 byColliResult = NTL_CHARACTER_COLLI_NONE;
    if(m_pActor->GetFlags() & SLFLAG_OBJECT_COLLISION)
    {
        RwV2d vOld2DPos;
        vOld2DPos.x = vPos.x;
        vOld2DPos.y = vPos.z;

        RwBool bCollMoveImPossible;
        RwV3d vNewPosV3d;
        byColliResult = Logic_CharacterCollisionEx(m_pActor, &vPos, fDashSpeed, vNewPosV3d, bCollMoveImPossible, fElapsed);

        if(byColliResult == NTL_CHARACTER_COLLI_OBJECT)
        {
            if(bCollMoveImPossible)
                m_byCollMoveImpossCnt++;
            else
                m_byCollMoveImpossCnt = 0;

            vPos.x = vNewPosV3d.x;
            vPos.z = vNewPosV3d.z;

            RwV2d vNew2DPos;
            vNew2DPos.x = vPos.x;
            vNew2DPos.y = vPos.z;
            RwV2dSubMacro(&vNew2DPos, &vNew2DPos, &vOld2DPos);

            RwReal fCollDeltaLen = RwV2dLength(&vNew2DPos);
            vNewPos.y += fCollDeltaLen*4;

            Logic_GetWorldHeight(m_pActor, &vNewPosV3d, m_sHStuff);
            if(vPos.y <= m_sHStuff.fFinialHeight )
                vPos.y = m_sHStuff.fFinialHeight;

            if(m_MoveStuff.vDest.y < vPos.y)
                m_MoveStuff.vDest.y = vPos.y + 0.01f;
        }
        else if(byColliResult == NTL_CHARACTER_COLLI_WORLD_ATTR_TERRAIN_SLANT)
        {
            if(m_byDashState == DASH_STATE_MOVE)
            {
                SetDashAnimEnd();
                m_byDashState = DASH_STATE_END;
                return FALSE;
            }
            else
            {
                return TRUE;
            }
        }

        // If there is a collision but neither movement is possible, it stops.
        if(m_byCollMoveImpossCnt >= COLLISION_MOVE_IMPOSSIBLE_COUNT)
        {
            CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData()); 
            SMoveStuff *pMoveStuff = pBeData->GetMoveStuff();
            pMoveStuff->byMoveResult |= NTL_MOVE_RESULT_COLLISION;

            if(m_byDashState == DASH_STATE_MOVE)
                SetDashAnimEnd();

            m_byDashState = DASH_STATE_END;
            return TRUE;
        }
    }

    m_pActor->SetPosition(&vPos);

    // ���� �ð� ���� �̵��� �Ÿ��� limit �Ÿ� �ȿ� ������? �����.
    if(byColliResult != NTL_CHARACTER_COLLI_WORLD_ATTR_TERRAIN_SLANT)
    {
        LimitPositionChangeCheck(fElapsed);
    }

    // effect position update
    if(m_pDashEffect)
    {
        vPos = vPos + m_vDashEffOffset;
        m_pDashEffect->SetPosition(&vPos);
    }

    UpdateDashLineEffect(vPos, vDir);

    if(m_pDashEffectSmoke)
        m_pDashEffectSmoke->SetPosition(&vPos);

    return FALSE;
}


RwBool CNtlBehaviorCharGroundMove::UpdateDashTargetMove(RwReal fElapsed)
{
    CNtlSobActor *pTargetActor = reinterpret_cast<CNtlSobActor*>(GetNtlSobManager()->GetSobObject(m_MoveStuff.hFollowSerialId));
    if(pTargetActor == NULL)
        return TRUE;

    RwReal fFollowRange = m_MoveStuff.fFollowRange/**FOLLOW_ERROR_RANGE*/; // disable by daneos

    RwV3d vPos = m_pActor->GetPosition();
    RwV3d vDestPos = pTargetActor->GetPosition(); 

    // ���� ���� ���� ���� ����ٴϴ� ��ü�� ���.
    if(pTargetActor->GetFlags() & SLFLAG_CAN_GROUND_FLY)
        vDestPos.y -= Logic_GetGroundFlyHeight(pTargetActor);

    RwV3d vDir;
    RwV3dSubMacro(&vDir, &vDestPos, &vPos); 

    // 占쏙옙占쏙옙 占쏙옙占쏙옙 占신몌옙占쏙옙 占쏙옙占싼댐옙.
    RwReal fCurrLen = RwV3dLength(&vDir);
    vDir.y = 0.0f;
    RwV3dNormalize(&vDir, &vDir);

    m_pActor->SetDirection(&vDir);

    if(fCurrLen < fFollowRange)
    {
        vPos.x = vDestPos.x - vDir.x*fFollowRange;
        vPos.z = vDestPos.z - vDir.z*fFollowRange;

        Logic_GetWorldHeight(m_pActor, &vPos, m_sHStuff);
        vPos.y = m_sHStuff.fFinialHeight;

        m_pActor->SetPosition(&vPos);

        return TRUE;
    }

	RwReal fSpeed = Logic_GetDashSpeed(m_pActor);
	if(m_MoveStuff.byType == NTL_MOVETARGET_DASH_ACTOR) // if we dash with skill
		fSpeed = NTL_ACTIVE_DASH_SPEED;

    RwV3d vDelta;
    CNtlMath::MathRwV3dAssign(&vDelta, vDir.x * fElapsed * fSpeed, 0.0f, vDir.z*fElapsed*fSpeed);

    // The current remaining distance is less than the moving delta distance.
    RwReal fDeltaLen = RwV3dLength(&vDelta);
    if(fCurrLen < fDeltaLen + fFollowRange)
    {
        vPos.x = vDestPos.x - vDir.x*fFollowRange;
        vPos.z = vDestPos.z - vDir.z*fFollowRange;

        Logic_GetWorldHeight(m_pActor, &vPos, m_sHStuff);
        vPos.y = m_sHStuff.fFinialHeight;

        m_pActor->SetPosition(&vPos);
        return TRUE;
    }

    // position update
    RwV3dAdd(&vPos, &vPos, &vDelta);  

    Logic_GetWorldHeight(m_pActor, &vPos, m_sHStuff);
    vPos.y = m_sHStuff.fFinialHeight;

    RwUInt8 byColliResult = NTL_CHARACTER_COLLI_NONE;
    if(m_pActor->GetFlags() & SLFLAG_OBJECT_COLLISION)
    {
        RwBool bCollMoveImPossible;
        byColliResult = Logic_CharacterCollisionEx(m_pActor, &vPos, fSpeed, vPos, bCollMoveImPossible, fElapsed);

        if(byColliResult == NTL_CHARACTER_COLLI_OBJECT)
        {
            if(bCollMoveImPossible)
                m_byCollMoveImpossCnt++;
            else
                m_byCollMoveImpossCnt = 0;

            Logic_GetWorldHeight(m_pActor, &vPos, m_sHStuff);
            vPos.y = m_sHStuff.fFinialHeight;
        }
        else if(byColliResult == NTL_CHARACTER_COLLI_WORLD_ATTR_TERRAIN_SLANT)
        {
            if(m_byDashState == DASH_STATE_MOVE)
            {
                SetDashAnimEnd();
                m_byDashState = DASH_STATE_END;
                return FALSE;
            }
            else
            {
                return TRUE;
            }
        }

        // If there is a collision but neither movement is possible, it stops.
        if(m_byCollMoveImpossCnt >= COLLISION_MOVE_IMPOSSIBLE_COUNT)
        {
            CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData()); 
            SMoveStuff *pMoveStuff = pBeData->GetMoveStuff();
            pMoveStuff->byMoveResult |= NTL_MOVE_RESULT_COLLISION;

            return TRUE;
        }
    }

    // 占쏙옙占싸울옙 占쏙옙표 setting
    m_pActor->SetPosition(&vPos);

    // ���� �ð� ���� �̵��� �Ÿ��� limit �Ÿ� �ȿ� ������? �����.
    if(byColliResult != NTL_CHARACTER_COLLI_WORLD_ATTR_TERRAIN_SLANT)
    {
        LimitPositionChangeCheck(fElapsed);
    }

    // effect update
    // effect position update
    UpdateDashEffect(NTL_MOVE_F, vDir);

    return FALSE;
}


void CNtlBehaviorCharGroundMove::CreateDashEffect(RwUInt32 uiMoveDirFlags, RwChar *pKey1, RwChar *pKey2)
{
    if(!m_pActor)
        return;

    DestroyDashEffect();
    DestroyDashEffectSmoke();

    CNtlPLAttach* pPLChar = (CNtlPLAttach*)m_pActor->GetSobProxy()->GetPLMainEntity();

    m_pDashEffect = GetSceneManager()->CreateEntity(PLENTITY_EFFECT, pKey1);
    pPLChar->Attach((CNtlPLAttach*)m_pDashEffect);

    m_pDashEffectSmoke = GetSceneManager()->CreateEntity(PLENTITY_EFFECT, pKey2);
    pPLChar->Attach((CNtlPLAttach*)m_pDashEffectSmoke);    

    if(m_pActor->GetClassID() == SLCLASS_AVATAR)
        m_pDashLine = GetSceneManager()->CreateEntity(PLENTITY_EFFECT, NTL_VID_DASH_LINE);
}

void CNtlBehaviorCharGroundMove::DestroyDashEffect(void)
{
    if(m_pDashEffect)
    {
        m_pDashEffect->Finish();
        m_pDashEffect = NULL;
    }

    if(m_pDashLine)
    {
        m_pDashLine->Finish();
        m_pDashLine = NULL;
    }
}

void CNtlBehaviorCharGroundMove::DestroyDashEffectSmoke(void)
{
    if(m_pDashEffectSmoke)
    {
        //		GetSceneManager()->DeleteEntity(m_pDashEffectSmoke);
        m_pDashEffectSmoke->Finish();
        m_pDashEffectSmoke = NULL;
    }
}

void CNtlBehaviorCharGroundMove::UpdateDashEffect(RwUInt32 uiMoveDirFlags, RwV3d& vDir)
{
    RwV3d vPos = m_pActor->GetPosition();

    // 占쌘울옙 占쏙옙占쏙옙 占쌕댐옙 effect
    RwMatrix mat;
    RwMatrixSetIdentity(&mat);

    RwV3dAssignMacro(&mat.at, &vDir);
    RwV3dCrossProduct(&mat.right, &mat.at, &CNtlPLGlobal::m_vYAxisV3);  
    RwV3dNormalize(&mat.right, &mat.right);
    RwV3dCrossProduct(&mat.up, &mat.at, &mat.right);  
    RwV3dNormalize(&mat.up, &mat.up);
    RwV3dAssignMacro(&mat.up, &CNtlPLGlobal::m_vYAxisV3);
    RwMatrixUpdate(&mat);

    // effect 占쏙옙占쏙옙 setting.
    if(uiMoveDirFlags == NTL_MOVE_F)
        CNtlMath::MathRwV3dAssign(&m_vDashEffOffset, vDir.x*0.6f, 0.86f, vDir.z*0.6f);
    else if(uiMoveDirFlags == NTL_MOVE_B)
        CNtlMath::MathRwV3dAssign(&m_vDashEffOffset, vDir.x*0.45f, 0.86f, vDir.z*0.45f);
    else if(uiMoveDirFlags == NTL_MOVE_L)
        CNtlMath::MathRwV3dAssign(&m_vDashEffOffset, vDir.x*0.6f, 0.86f, vDir.z*0.6f);
    else
        CNtlMath::MathRwV3dAssign(&m_vDashEffOffset, vDir.x*0.6f, 0.86f, vDir.z*0.6f);

    if(m_pDashEffectSmoke)
        m_pDashEffectSmoke->SetPosition(&vPos);

    if(m_pDashLine)
        m_pDashLine->SetMatrix(mat);

    RwV3dAdd(&vPos, &vPos, &m_vDashEffOffset);
    RwMatrixTranslate(&mat, &vPos, rwCOMBINEPOSTCONCAT);

    if(m_pDashEffect)
        m_pDashEffect->SetMatrix(mat);
}

void CNtlBehaviorCharGroundMove::UpdateDashLineEffect(RwV3d& vPos, RwV3d& vDir)
{
    if(m_pDashLine)
    {
        m_pDashLine->SetPosition(&vPos);

        // 카占쌨띰옙 占쏙옙占쏙옙 占쏙옙占쏙옙占쏙옙
        RwMatrix *pMatrix = RwFrameGetMatrix( RwCameraGetFrame( CNtlPLGlobal::m_RwCamera ) );
        RwV3d *pCamDir = RwMatrixGetAt( pMatrix );

        RwReal fValue = RwV3dDotProduct(&vDir, pCamDir);
        if(fValue < -0.9f)
            m_pDashLine->SetAlpha(80);
        else
            m_pDashLine->SetAlpha(255);
    }

}

RwBool CNtlBehaviorCharGroundMove::CheckFalling(RwReal fOldActorHeight, RwReal fFinalHeight, RwReal fSpeed, RwUInt8 byMoveFlag, RwV3d* pNewPos /* = NULL */)
{
	if( m_pActor->GetFlags() & SLFLAG_FALLING_ENABLE && fOldActorHeight - fFinalHeight > FALLING_CHECK_LEN)
	{
        CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData()); 

        SMoveStuff *pMoveStuff = pBeData->GetMoveStuff();
        pMoveStuff->byMoveResult |= NTL_MOVE_RESULT_FALLING;

        SCtrlStuff *pCtrlStuff = pBeData->GetCtrlStuff();

        if(pNewPos)
        {
            RwV3d vSub = *pNewPos - m_pActor->GetPosition();        
            vSub.y = 0.0f;
            RwV3dNormalize(&pCtrlStuff->uExtra.sFalling.vFallingDir, &vSub);
        }
        else
        {
            pCtrlStuff->uExtra.sFalling.vFallingDir = m_pActor->GetDirection();
        }

        pCtrlStuff->uExtra.sFalling.byMoveDirection = byMoveFlag;
        pCtrlStuff->uExtra.sFalling.fSpeed = (byMoveFlag == NTL_MOVE_B)? fSpeed * NTL_BACKWARD_MOVEMENT_SPEED_RATE: fSpeed;
        pCtrlStuff->uExtra.sFalling.fSearchHeight = m_sHStuff.fFinialHeight;

        return TRUE;
    }

    return FALSE;
}

RwBool CNtlBehaviorCharGroundMove::ObjectCollision(OUT RwV3d &vPos, RwReal fOldActorHeight, RwReal fSpeed, RwReal fElapsed, OUT RwUInt8* pColliResult)
{
    RwUInt8 byColliResult = NTL_CHARACTER_COLLI_NONE;

    if(m_pActor->GetFlags() & SLFLAG_OBJECT_COLLISION)
    {
        //RwV2d vOld2DPos;
        //vOld2DPos.x = vPos.x;
        //vOld2DPos.y = vPos.z;

        RwBool bCollMoveImPossible;
        byColliResult = Logic_CharacterCollisionEx(m_pActor, &vPos, fSpeed, vPos, bCollMoveImPossible, fElapsed);
        *pColliResult = byColliResult;

        // If there is a collision due to the slope of the terrain or the property, it stops in place.
        if(byColliResult == NTL_CHARACTER_COLLI_WORLD_ATTR_TERRAIN_SLANT)
        {
            return TRUE;
        }

        if(byColliResult == NTL_CHARACTER_COLLI_OBJECT)
        {
            m_byCollMoveImpossCnt = bCollMoveImPossible ? ++m_byCollMoveImpossCnt : 0;

            // I do not know why this code is ?? (by agebreak 09.08.13)

			// - lleo52 09.08.18
			// The reason for entering the following code is to prevent it from falling into an object.
			// Test results after comment processing show no problem
			// First of all, you should comment out the code below and watch the situation.

			//RwV2d vNew2DPos;
			//vNew2DPos.x = vPos.x;
			//vNew2DPos.y = vPos.z;
			//RwReal fOffset = CNtlMath::GetLength(vNew2DPos, vOld2DPos) * 2;
			//vPos.y += fOffset;

            Logic_GetWorldHeight(m_pActor, &vPos, m_sHStuff);
            vPos.y = m_sHStuff.fFinialHeight;
            //////////////////////////////////////////////////////////////////////////
        }
        else if(byColliResult == NTL_CHARACTER_COLLI_WORLD_ATTR_TERRAIN_SLANT && Logic_IsDirectMode(m_pActor))
        {
            CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData());                 
            pBeData->GetMoveStuff()->byMoveResult |= NTL_MOVE_RESULT_COLLISION;   

            return TRUE;                
        }            
    }
    
    // If there is a collision but neither movement is possible, it stops.
    if(m_byCollMoveImpossCnt >= COLLISION_MOVE_IMPOSSIBLE_COUNT)
    {
		CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData());
		pBeData->GetMoveStuff()->byMoveResult |= NTL_MOVE_RESULT_COLLISION;

        return TRUE;
    }

    // falling state check
    if (CheckFalling(fOldActorHeight, m_sHStuff.fFinialHeight, fSpeed, m_MoveStuff.byMoveFlags, &vPos))
    {
        // If falling, reset height to the old one but update X and Z to account for delta sync offsets.
        vPos.y = fOldActorHeight;
        m_pActor->SetPosition(&vPos);

        return TRUE;
    }

    return FALSE;
}

RwBool CNtlBehaviorCharGroundMove::IsIdleAnimationPlaying()
{
    CNtlSobCharProxy *pSobProxy = reinterpret_cast<CNtlSobCharProxy*>(m_pActor->GetSobProxy());
    RwUInt32 uiCurrAnimKey = pSobProxy->GetBaseAnimationKey();

    if(uiCurrAnimKey == TRANS_IDLE || uiCurrAnimKey == NML_IDLE_01 || 
       uiCurrAnimKey == NML_IDLE_LOOP || uiCurrAnimKey == NML_IDLE_02 ||
       uiCurrAnimKey == NML_STAFF_IDLE_LOOP || uiCurrAnimKey == NML_STAFF_IDLE_01 ||
       uiCurrAnimKey == NML_STAFF_IDLE_02 || uiCurrAnimKey == NML_DGUN_IDLE_LOOP || 
       uiCurrAnimKey == NML_DGUN_IDLE_01 || uiCurrAnimKey == NML_DGUN_IDLE_02)
    {
        return TRUE;
    }

    return FALSE;
}
