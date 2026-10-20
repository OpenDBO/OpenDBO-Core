#pragma once

#include "NtlBehaviorCharMove.h"

struct SNtlEventSobMoveCollision;
struct SNtlEventSobMoveCollisionEnd;

/**
 * \ingroup NtlSimulation
 * \brief 일반적인 지형 이동을 처리하는 행동 클래스
 *
 * \date 2009-04-15
 * \author agebreak
 */
class CNtlBehaviorCharGroundMove : public CNtlBehaviorCharMove
{
    DECLEAR_MEMORY_POOL(CNtlBehaviorCharGroundMove, NTL_DEFAULT_MEMORY_POOL)
    DECLEAR_BEHAVIOR_FACTROY(CNtlBehaviorCharGroundMove)

    enum 
    {
        DASH_STATE_NONE,
        DASH_STATE_MOVE,
        DASH_STATE_LANDING,
        DASH_STATE_END
    };

public:
    CNtlBehaviorCharGroundMove();
    ~CNtlBehaviorCharGroundMove();

    virtual void Enter(void);
    virtual void Exit(void);
    virtual void Update(RwReal fElapsed);
    virtual RwUInt32 HandleEvents(RWS::CMsg &pMsg);
    virtual void UpdateData(void);

    RwUInt32 GetDashState(void) { return m_byDashState; }


protected:
    RwBool	IsInDestMoveRound(void);
    RwBool	IsInFollowMoveRound(void);
    RwBool	IsInMoveRound(void);

    void	SetPathNode(void);
    void	SetTransform(void);
    void	SetAnim(RwUInt8 byMoveFlags);
    void	SetDashAnimEnd(void);
    void    SetIdle2RunAnim(void);                 ///< Idle -> Run 보간 애니메이션을 설정한다.
    RwBool  IsIdleAnimationPlaying();              ///< Idle 애니메이션 플레이중인지 확인한다.

    // tw: characters moved by other clients are dead reckoned from the server position
    RwBool  IsDeadReckoning(void);
    void    ResetDeadReckoning(RwBool bSnapDir = FALSE);
    RwBool  BeginDeadReckoning(OUT RwV3d& vPos, OUT RwV3d& vDir);
    void    MoveGhost(const RwV3d& vDir, RwReal fSpeed, RwReal fElapsed);
    void    EndDeadReckoning(const RwV3d& vOldPos, const RwV3d& vNewPos, const RwV3d& vNewDir, RwReal fSpeed, RwReal fElapsed, RwBool bBlocked, RwBool bMoveGhost);
    void    OnMoveCollision(const SNtlEventSobMoveCollision* pCollision);
    void    OnMoveCollisionEnd(const SNtlEventSobMoveCollisionEnd* pCollisionEnd);
    void    OnChangeHeading(const RwV3d& vDir);
    void    OnMoveSync(const RwV3d& vLoc, const RwV3d& vDir);

    // a character of another client turned with the mouse: its heading reaches this client in steps,
    // the turn is measured and continued between them, the shown facing eases after it
    void    OnHeadingSample(const RwV3d& vDir);
    void    ExtrapolateHeading(RwV3d& vDir, RwReal fElapsed);
    void    SetShownDirection(const RwV3d& vDir, RwReal fElapsed);

    // the moving avatar sends its position now and then, the others correct their prediction with it
    void    UpdateLocationSync(RwReal fElapsed);

    // tw: the avatar tells the server when an object blocks its move
    void    UpdateCollisionReport(const RwV3d& vOldPos, const RwV3d& vMovePos, const RwV3d& vFinalPos, const RwV3d& vDir, RwReal fElapsed);
    void    EndCollisionReport(void);


    RwBool	UpdateDirectionMove(RwReal fElapsed);
    RwBool	UpdateLocationMove(RwReal fElapsed);

    RwBool  ObjectCollision(OUT RwV3d &vPos, RwReal fOldActorHeight, RwReal fSpeed, RwReal fElapsed, OUT RwUInt8* pColliResult);
    RwBool	UpdateTargetMove(RwReal fElapsed);
    RwBool	UpdateDashMove(RwReal fElapsed);
    RwBool	UpdateDashTargetMove(RwReal fElapsed);

    void	CreateStartDashCamera(void);
    void	DestroyEndDashCamera(void);

    void	CreateDashEffect(RwUInt32 uiMoveDirFlags, RwChar *pKey1, RwChar *pKey2);
    void	DestroyDashEffect(void);
    void	DestroyDashEffectSmoke(void);
    void	UpdateDashEffect(RwUInt32 uiMoveDirFlags, RwV3d& vDir);
    void	UpdateDashLineEffect(RwV3d& vPos, RwV3d& vDir);

    void	FootStepMaterialProc(RWS::CMsg &pMsg);

    // position 변화 check
    void	LimitPositionChangeCheck(RwReal fElapsed);
    RwBool  CheckFalling(RwReal fOldActorHeight, RwReal fFinalHeight, RwReal fSpeed, RwUInt8 byMoveFlag, RwV3d* pNewPos = NULL);              ///< 폴링 상태 체크 

protected:

    // move stuff
    SMoveStuff			m_MoveStuff;
    SWorldHeightStuff	m_sHStuff;				/** world height stuff */
    RwBool				m_bSlowMove;			/** If the target is near to the target when the movement is first started ... */
    RwReal				m_fSlowMoveTime;		/** slow move일 경우 누적 시간을 위해 사용 */

    // speed
    RwReal				m_fDefaultWalkSpeed;
    RwReal				m_fDefaultRunSpeed;
    RwReal				m_fDefaultBackSpeed;	


    // dash
    RwUInt8				m_byDashState;
    RwReal				m_fDashDestPosY;
    RwReal				m_fDashLadingDist;

    // dash effect
    CNtlPLEntity		*m_pDashEffect;
    CNtlPLEntity		*m_pDashLine;
    CNtlPLEntity		*m_pDashEffectSmoke;
    RwReal				m_fEffectSmokeTime;
    RwV3d				m_vDashEffOffset;

    // camera
    RwBool				m_bDashCameraEnable;

    // possible 변화 check
    RwReal				m_fPosChangeTime;
    RwUInt8				m_byPosChangeCnt;
    RwV3d				m_vChangeOldPos;

    // collision
    RwUInt8				m_byCollMoveImpossCnt;		// collision move impossible count

    // tw dead reckoning of a character moved by another client. The prediction runs from the
    // position the server gave, the actor is only shown sliding towards it.
    RwV3d				m_vPredictPos;
    RwV3d				m_vPredictDir;
    RwV3d				m_vGhostPos;			///< the same move without object collision
    RwReal				m_fGhostTime;			///< time the prediction is held back by object collision
    RwBool				m_bMoveCollision;		///< its client reported an object blocking the move
    RwReal				m_fForcedSpeed;			///< rate of the move speed while blocked
    RwV3d				m_vForcedDir;

    // tw collision report of the avatar
    RwBool				m_bCollisionSent;
    RwReal				m_fLastCollisionRate;
    RwReal				m_fCollisionCheckTime;

    RwReal				m_fLocationSyncTime;
    RwV3d				m_vLocationSyncDir;		///< heading sent with the last position
    RwV3d				m_vLastFrameDir;		///< heading of the avatar the frame before
    RwReal				m_fNoTurnTime;			///< time the avatar has not turned
    RwBool				m_bTurnSyncPending;		///< a position was sent during a turn, send where it ends

    // turn of another client's character
    RwReal				m_fYawRate;				///< measured turn speed (radians per second)
    RwReal				m_fYawBudget;			///< how much more it may turn without a new heading
    RwReal				m_fYawExtrapolateTime;	///< time turned since the last heading
    RwV3d				m_vYawSampleDir;		///< heading the turn speed is measured from
    RwReal				m_fYawSampleTime;

    // 보간 애니메이션을 위한 변수들
    RwBool              m_bInterAnimEnd;
    RwReal              m_fInterAnimTime;   
};
