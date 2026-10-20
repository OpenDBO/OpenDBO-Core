#include "precomp_ntlsimulation.h"
#include "NtlBehaviorCharAdjustMove.h"

// shared 
#include "NtlMovement.h"

// simulation
#include "NtlSobActor.h"
#include "NtlSLLogic.h"
#include "NtlSobCharProxy.h"
#include "NtlFSMLayer.h"
#include "NtlFSMCharActAgent.h"


DEFINITION_MEMORY_POOL(CNtlBehaviorCharGroundAdjustMove)

CNtlBehaviorCharGroundAdjustMove::CNtlBehaviorCharGroundAdjustMove()
{
    SetBehaviorId(NTL_BEID_CHAR_GROUNDADJUSTMOVE);
    SetBehaviorName(NTL_BENAME_CHAR_GROUNDADJUSTMOVE);

    m_bSlidingAdjust = FALSE;
}

CNtlBehaviorCharGroundAdjustMove::~CNtlBehaviorCharGroundAdjustMove()
{
}

void CNtlBehaviorCharGroundAdjustMove::Enter(void)
{
    CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData()); 
    SMoveStuff *pMoveStuff = pBeData->GetMoveStuff();    
    if(IsNotAdjustMove())
    {
        Finish();
        return;
    }
        
    memcpy(&m_MoveStuff, pMoveStuff, sizeof(SMoveStuff));    

    m_vAdjustDir = m_MoveStuff.vDest - m_pActor->GetPosition();    
    m_vAdjustDir.y = 0.0f;
    RwReal fLen = RwV3dLength(&m_vAdjustDir);

    RwReal fSpeed	= Logic_GetFrontRunSpeed(m_pActor);

    // Interpolates only when the difference is smaller than twice the run speed, and jumps when the difference is exceeded.
    if(fLen < fSpeed  * 2.0f)
    {
        RwV3dNormalize(&m_vAdjustDir, &m_vAdjustDir);
        m_pActor->SetDirection(&m_MoveStuff.vDir);
        m_bSlidingAdjust = TRUE;
    }

    SetAnim();

    // 나중에 base class enter를 호출한다.
    CNtlBehaviorBase::Enter(); 

}

void CNtlBehaviorCharGroundAdjustMove::Exit(void)
{
    if(!IsFinish())
    {
        //Behavior_AdjustPositionJumping(m_pActor, &m_MoveStuff.vDest);
    }

    CNtlBehaviorBase::Exit();
}

void CNtlBehaviorCharGroundAdjustMove::Update(RwReal fElapsed)
{
    if(IsFinish())
        return;

    if(m_bSlidingAdjust)
    {
        RwV3d vNewPos = m_pActor->GetPosition();
        RwV3d vSub = m_MoveStuff.vDest - m_pActor->GetPosition();
        vSub.y = 0.0f;

        RwReal fCurrLen = RwV3dLength(&vSub);
        RwReal fSpeed	= (m_MoveStuff.byMoveFlags == NTL_MOVE_B) ? Logic_GetBackSpeed(m_pActor) : Logic_GetFrontRunSpeed(m_pActor);

        RwV3d vDelta = m_vAdjustDir * fSpeed * fElapsed;      
        RwReal fDeltaLen = RwV3dLength(&vDelta);

        if(fDeltaLen < fCurrLen)
        {
            vNewPos += vDelta;

            Logic_GetWorldHeight(m_pActor, &vNewPos, m_sHStuff);
            vNewPos.y = m_sHStuff.fFinialHeight;
            m_pActor->SetPosition(&vNewPos);            
        }
        else
        {
            vNewPos = m_MoveStuff.vDest;
            Logic_GetWorldHeight(m_pActor, &vNewPos, m_sHStuff);
            vNewPos.y = m_sHStuff.fFinialHeight;
            m_pActor->SetPosition(&vNewPos);

            Finish();            
        }
    }
    else
    {
        Behavior_AdjustPositionJumping(m_pActor, &m_MoveStuff.vDest);
        m_pActor->SetDirection(&m_MoveStuff.vDir);

        Finish();
    }
}

void CNtlBehaviorCharGroundAdjustMove::UpdateData(void)
{
    CNtlBehaviorBase::UpdateData();
}

void CNtlBehaviorCharGroundAdjustMove::SetAnim() 
{
    if(m_MoveStuff.byMoveFlags == NTL_MOVE_NONE)
        return;

    CNtlSobCharProxy *pSobProxy = reinterpret_cast<CNtlSobCharProxy*>(m_pActor->GetSobProxy());
    RwUInt32 uiCurrAnimKey = pSobProxy->GetBaseAnimationKey();
    RwUInt32 uiNextAnimKey = 0;
    SWorldHeightStuff sHStuff;
	
	sITEM_TBLDAT *pItemTblData = Logic_GetEquipedWeaponItemTableData(m_pActor);

    if( m_MoveStuff.byMoveFlags == NTL_MOVE_F || m_MoveStuff.byMoveFlags == NTL_MOVE_F_TURN_L || m_MoveStuff.byMoveFlags == NTL_MOVE_F_TURN_R )
    {
        if(Logic_IsSwimmingActor(m_pActor, &m_pActor->GetPosition(), sHStuff))
        {
            uiNextAnimKey = SWIM_FRONT;
        }
        else if(Logic_IsTransformGreatNamek(m_pActor))  // 변신시
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
    else if( m_MoveStuff.byMoveFlags == NTL_MOVE_B || m_MoveStuff.byMoveFlags == NTL_MOVE_B_TURN_L || m_MoveStuff.byMoveFlags == NTL_MOVE_B_TURN_R )
    {
        if(Logic_IsSwimmingActor(m_pActor, &m_pActor->GetPosition(), sHStuff))
        {
            uiNextAnimKey = SWIM_BACK;
        }
        else if(Logic_IsTransformGreatNamek(m_pActor)) // 변신시
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
	else if (m_MoveStuff.byMoveFlags == NTL_MOVE_L || m_MoveStuff.byMoveFlags == NTL_MOVE_L_TURN_L || m_MoveStuff.byMoveFlags == NTL_MOVE_L_TURN_R)
	{
		sITEM_TBLDAT *pItemTblData = Logic_GetEquipedWeaponItemTableData(m_pActor);

		if (Logic_IsSwimmingActor(m_pActor, &m_pActor->GetPosition(), sHStuff))
		{
			uiNextAnimKey = SWIM_FRONT;
		}
		else if (Logic_IsTransformGreatNamek(m_pActor))  // 변신시
		{
			uiNextAnimKey = TRANS_RUN_FRONT;
		}
		else if (Logic_IsTransformSpinAttack(m_pActor))
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
	else if (m_MoveStuff.byMoveFlags == NTL_MOVE_R || m_MoveStuff.byMoveFlags == NTL_MOVE_R_TURN_L || m_MoveStuff.byMoveFlags == NTL_MOVE_R_TURN_R)
	{
		sITEM_TBLDAT *pItemTblData = Logic_GetEquipedWeaponItemTableData(m_pActor);

		if (Logic_IsSwimmingActor(m_pActor, &m_pActor->GetPosition(), sHStuff))
		{
			uiNextAnimKey = SWIM_FRONT;
		}
		else if (Logic_IsTransformGreatNamek(m_pActor))  // 변신시
		{
			uiNextAnimKey = TRANS_RUN_FRONT;
		}
		else if (Logic_IsTransformSpinAttack(m_pActor))
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

	//DBO_WARNING_MESSAGE("m_MoveStuff.byMoveFlags: " << m_MoveStuff.byMoveFlags << " animKey: " << uiNextAnimKey);

    if(uiNextAnimKey > 0 && uiCurrAnimKey != uiNextAnimKey)
        pSobProxy->SetBaseAnimation(uiNextAnimKey);
}

RwBool CNtlBehaviorCharGroundAdjustMove::IsNotAdjustMove() 
{
    CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData()); 
    SMoveStuff* pMoveStuff = pBeData->GetMoveStuff();    
    SJumpStuff* pJumpStuff = pBeData->GetJumpStuff();

    if(pMoveStuff->byMoveFlags == NTL_MOVE_NONE ||
       pMoveStuff->byMoveFlags == NTL_MOVE_TURN_L || pMoveStuff->byMoveFlags == NTL_MOVE_TURN_R ||
       (pMoveStuff->byMoveFlags >= NTL_MOVE_L_TURN_L_JUMP && pMoveStuff->byMoveFlags <= NTL_MOVE_B_TURN_R_JUMP))
    {
        return TRUE;
    }
    else if(pJumpStuff->eState != JUMPSTATE_NONE)       ///< Do not adjust after jumping.
    {
        pJumpStuff->eState = JUMPSTATE_NONE;
        return TRUE;
    }

    return FALSE;
}

//////////////////////////////////////////////////////////////////////////
DEFINITION_MEMORY_POOL(CNtlBehaviorCharBattleAdjustMove)

CNtlBehaviorCharBattleAdjustMove::CNtlBehaviorCharBattleAdjustMove()
{
    SetBehaviorId(NTL_BEID_CHAR_BATTLE_ADJUST_MOVE);
    SetBehaviorName(NTL_BENAME_BATTLE_ADJUST_MOVE);

    m_vDest = ZeroAxis;
    m_byAdjustState = ADJUST_STATE_SLIDE;

    // idle until a state starts it
    Finish();
}

CNtlBehaviorCharBattleAdjustMove::~CNtlBehaviorCharBattleAdjustMove()
{
}

void CNtlBehaviorCharBattleAdjustMove::Enter(void)
{
    if(!m_pActor)
    {
        Finish();
        return;
    }

    RwUInt32 uiClassId = m_pActor->GetClassID();
    if(uiClassId != SLCLASS_PLAYER && uiClassId != SLCLASS_AVATAR && uiClassId != SLCLASS_PET)
    {
        Finish();
        return;
    }

    CNtlFSMLayer* pFSMLayer = m_pActor->GetFSMLayer();
    CNtlFSMCharActAgent* pCharActAgent = pFSMLayer ? reinterpret_cast<CNtlFSMCharActAgent*>(pFSMLayer->GetFSMAgent()) : NULL;
    if(!pCharActAgent)
    {
        Finish();
        return;
    }

    m_vDest = pCharActAgent->GetServerPosition();

    RwV3d vSub = m_vDest - m_pActor->GetPosition();
    RwReal fLen = RwV3dLength(&vSub);

    // slide when it is less than two seconds of running away, otherwise jump there
    if(Logic_GetFrontRunSpeed(m_pActor) * 2.0f >= fLen)
        m_byAdjustState = ADJUST_STATE_SLIDE;
    else
        m_byAdjustState = ADJUST_STATE_WAIT;

    m_bFinish = FALSE;
}

void CNtlBehaviorCharBattleAdjustMove::Exit(void)
{
    CNtlBehaviorBase::Exit();

    Finish();
}

void CNtlBehaviorCharBattleAdjustMove::Update(RwReal fElapsed)
{
    if(IsFinish())
        return;

    if(m_byAdjustState == ADJUST_STATE_WAIT)
    {
        m_byAdjustState = ADJUST_STATE_JUMP;
        return;
    }

    if(m_byAdjustState == ADJUST_STATE_JUMP)
    {
        SetAdjustPosition(m_vDest);
        Finish();
        return;
    }

    RwV3d vPos = m_pActor->GetPosition();
    RwV3d vDir = m_vDest - vPos;
    vDir.y = 0.0f;
    RwReal fLen = RwV3dLength(&vDir);
    if(fLen > 0.0f)
        RwV3dScale(&vDir, &vDir, 1.0f / fLen);

    RwV3d vDelta = vDir * (Logic_GetFrontRunSpeed(m_pActor) * fElapsed);
    RwReal fDeltaLen = RwV3dLength(&vDelta);

    if(fLen <= 0.01f || fDeltaLen >= fLen)
    {
        Finish();
        SetAdjustPosition(m_vDest);
        return;
    }

    vPos += vDelta;
    SetAdjustPosition(vPos);
}

void CNtlBehaviorCharBattleAdjustMove::SetAdjustPosition(RwV3d vPos)
{
    SWorldHeightStuff sHStuff;
    Logic_GetWorldHeight(m_pActor, &vPos, sHStuff);
    vPos.y = sHStuff.fFinialHeight;

    m_pActor->SetPosition(&vPos);
}
