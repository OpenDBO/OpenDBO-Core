#include "precomp_ntlsimulation.h"
#include "NtlBehaviorCharAirMove.h"

// shared 
#include "NtlMovement.h"

// presentation
#include "NtlPLCharacter.h"
#include "NtlPLEvent.h"
#include "NtlPLHelpers.h"


// simulation
#include "NtlFSMDef.h"
#include "NtlSLApi.h"
#include "NtlSobActor.h"
#include "NtlSobCharProxy.h"
#include "NtlSLVisualDeclear.h"
#include "NtlSLLogic.h"
#include "NtlSobManager.h"
#include "NtlSLEvent.h"
#include "NtlSLPacketGenerator.h"

#define AIR_TILT_MAX_DIR_Y			0.785398f	// tw: highest climb/dive of the flight direction
#define AIR_TILT_LEVEL_SPEED		2.0f		// tw: flying straight up/down levels the flight this fast
#define AIR_TILT_SYNC_DIR_Y			0.087266f	// tw: 5 degrees
#define AIR_TILT_SYNC_TIME			3.0f
#define AIR_MOVE_SYNC_SEND_TIME		0.5f		// tw: interval of UG_CHAR_AIR_MOVE_SYNC

DEFINITION_MEMORY_POOL(CNtlBehaviorCharAirMove)

CNtlBehaviorCharAirMove::CNtlBehaviorCharAirMove()
{
	SetBehaviorId(NTL_BEID_CHAR_AIR_MOVE);
	SetBehaviorName(NTL_BENAME_CHAR_AIR_MOVE);

	m_pPLAirEffect = NULL;

	m_bAirTiltSync = FALSE;
	m_fAirTiltStartY = 0.0f;
	m_fAirTiltTime = 0.0f;

	m_fSyncSendTime = 0.0f;
}

CNtlBehaviorCharAirMove::~CNtlBehaviorCharAirMove()
{
	DestroyAirEffect();
}

void CNtlBehaviorCharAirMove::Enter(void)
{
	CNtlBeCharData* pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData());
	SMoveStuff* pMoveStuff = pBeData->GetMoveStuff();
	memcpy(&m_MoveStuff, pMoveStuff, sizeof(SMoveStuff));

	m_bAirTiltSync = FALSE;

	SetAnim(pMoveStuff->byMoveFlags);

	// base class enter
	CNtlBehaviorBase::Enter();
}

void CNtlBehaviorCharAirMove::Exit(void)
{
	DestroyAirEffect();

	CNtlSobCharProxy* pSobProxy = reinterpret_cast<CNtlSobCharProxy*>(m_pActor->GetSobProxy());

	CNtlPLCharacter* pPLCharacter = reinterpret_cast<CNtlPLCharacter*>(pSobProxy->GetPLMainEntity());
	pPLCharacter->SetBlend(BLEND_TWEEN);

	// 나중에 base class enter를 호출한다.
	CNtlBehaviorBase::Exit();
}

void CNtlBehaviorCharAirMove::Update(RwReal fElapsed)
{
	// Avoid sudden splashes when the frame is low
	fElapsed = min(fElapsed, 0.2f);

	CNtlBeCharData* pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData());
	SMoveStuff* pMoveStuff = pBeData->GetMoveStuff();

	UpdateAirTilt(pMoveStuff, fElapsed);

	RwBool bFinish = FALSE;

	if (pMoveStuff->byFormFlag == NTL_MOVE_FLAG_FLY)
	{
		bFinish = UpdateAirFly(pMoveStuff, fElapsed);
	}
	else if (pMoveStuff->byFormFlag == NTL_MOVE_FLAG_FLY_DASH)
	{
		if (pMoveStuff->byType == NTL_MOVETARGET_AIR_DASH)
			bFinish = UpdateAirStart(pMoveStuff, fElapsed);
		else
			bFinish = UpdateAirDash(pMoveStuff, fElapsed);
	}
	else if (pMoveStuff->byFormFlag == NTL_MOVE_FLAG_FLY_ACCEL)
	{
		bFinish = UpdateAirAccel(pMoveStuff, fElapsed);
	}

	// effect position update
	if (m_pPLAirEffect)
	{
		RwV3d vPos = m_pActor->GetPosition();
		RwV3d vDir = m_pActor->GetDirection();
		vPos = vPos + m_vEffectOffset;
		m_pPLAirEffect->SetPosition(&vPos);
		m_pPLAirEffect->SetDirection(&vDir);

		UpdateAirEffect(vDir);
	}

	UpdateSendSync(fElapsed);

	if (bFinish)
	{
		Finish();
	}
}

void CNtlBehaviorCharAirMove::UpdateSendSync(RwReal fElapsed)
{
	if (m_pActor->GetClassID() != SLCLASS_AVATAR)
		return;

	m_fSyncSendTime += fElapsed;
	if (m_fSyncSendTime < AIR_MOVE_SYNC_SEND_TIME)
		return;

	RwV3d vPos = m_pActor->GetPosition();
	RwV3d vDir = m_pActor->GetDirection();
	API_GetSLPacketGenerator()->SendAirMoveSync(vPos, vDir, m_pActor->GetServerSyncAvatarType());

	m_fSyncSendTime = 0.0f;
}

// tw: a relayed GU_CHAR_AIR_MOVE_SYNC puts the flying player back where it really is, the
// keyboard move then carries on from there
void CNtlBehaviorCharAirMove::ApplyMoveSync(const RwV3d& vLoc, const RwV3d& vDir)
{
	m_pActor->SetPosition(&vLoc);
	m_pActor->SetDirection(&vDir);
}


void CNtlBehaviorCharAirMove::UpdateAirEffect(RwV3d& vDir)
{
	RwV3d vPos = m_pActor->GetPosition();

	// Effect
	RwMatrix mat;
	RwMatrixSetIdentity(&mat);

	RwV3dAssignMacro(&mat.at, &vDir);
	RwV3dCrossProduct(&mat.right, &mat.at, &CNtlPLGlobal::m_vYAxisV3);
	RwV3dNormalize(&mat.right, &mat.right);
	RwV3dCrossProduct(&mat.up, &mat.at, &mat.right);
	RwV3dNormalize(&mat.up, &mat.up);
	RwV3dAssignMacro(&mat.up, &CNtlPLGlobal::m_vYAxisV3);
	RwMatrixUpdate(&mat);

	CNtlMath::MathRwV3dAssign(&m_vEffectOffset, vDir.x * 0.6f, 0.5f, vDir.z * 0.5f);

	RwV3dAdd(&vPos, &vPos, &m_vEffectOffset);
	RwMatrixTranslate(&mat, &vPos, rwCOMBINEPOSTCONCAT);

	if (m_pPLAirEffect)
		m_pPLAirEffect->SetMatrix(mat);
}

void CNtlBehaviorCharAirMove::BeginAirTiltSync(RwReal fDirY)
{
	if (m_pActor->GetClassID() != SLCLASS_AVATAR || m_bAirTiltSync)
		return;

	m_bAirTiltSync = TRUE;
	m_fAirTiltStartY = fDirY;
	m_fAirTiltTime = 0.0f;
}

// tw: holding the up/down key while moving tilts the flight (1 per second, up to 45 degrees),
// flying straight up/down levels it. The avatar sends the heading every 5 degrees or 3 seconds.
void CNtlBehaviorCharAirMove::UpdateAirTilt(SMoveStuff* pMoveStuff, RwReal fElapsed)
{
	RwV3d vDir = m_pActor->GetDirection();
	RwBool bChange = FALSE;

	if (pMoveStuff->byMoveFlags == NTL_MOVE_HEIGHT_UP || pMoveStuff->byMoveFlags == NTL_MOVE_HEIGHT_DOWN)
	{
		BeginAirTiltSync(vDir.y);

		if (vDir.y != 0.0f)
		{
			RwReal fStep = fElapsed * AIR_TILT_LEVEL_SPEED;
			if (vDir.y > 0.0f)
				vDir.y = max(0.0f, vDir.y - fStep);
			else
				vDir.y = min(0.0f, vDir.y + fStep);

			bChange = TRUE;
		}
	}

	if (pMoveStuff->byMoveFlags != NTL_MOVE_NONE)
	{
		CNtlBeCharData* pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData());
		RwUInt8 byAirUpDown = pBeData->GetAirUpDown();

		if (byAirUpDown == AVATAR_AIR_UPDOWN_UP)
		{
			BeginAirTiltSync(vDir.y);
			vDir.y = min(vDir.y + fElapsed, AIR_TILT_MAX_DIR_Y);
			bChange = TRUE;
		}
		else if (byAirUpDown == AVATAR_AIR_UPDOWN_DOWN)
		{
			BeginAirTiltSync(vDir.y);
			vDir.y = max(vDir.y - fElapsed, -AIR_TILT_MAX_DIR_Y);
			bChange = TRUE;
		}
	}

	if (bChange)
	{
		RwV3dNormalize(&vDir, &vDir);
		m_pActor->SetDirection(&vDir);
	}

	if (m_bAirTiltSync)
	{
		m_fAirTiltTime += fElapsed;

		if (fabsf(vDir.y - m_fAirTiltStartY) > AIR_TILT_SYNC_DIR_Y || m_fAirTiltTime > AIR_TILT_SYNC_TIME)
		{
			m_bAirTiltSync = FALSE;
			API_GetSLPacketGenerator()->SendChangeHeading(vDir);
		}
	}
}

RwUInt32 CNtlBehaviorCharAirMove::HandleEvents(RWS::CMsg& pMsg)
{
	NTL_FUNCTION("CNtlBehaviorCharAirMove::HandleEvents");

	if (pMsg.Id == g_EventSobMoveSync)
	{
		// the avatar is the one sending the sync
		if (m_pActor->GetClassID() != SLCLASS_AVATAR)
		{
			SNtlEventSobMoveSync* pMoveSync = reinterpret_cast<SNtlEventSobMoveSync*>(pMsg.pData);
			ApplyMoveSync(pMoveSync->vLoc, pMoveSync->vDir);
		}
	}

	NTL_RETURN(NTL_FSM_EVENTRES_PASS);
}

void CNtlBehaviorCharAirMove::UpdateData(void)
{
	CNtlBeCharData* pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData());
	SMoveStuff* pMoveStuff = pBeData->GetMoveStuff();
	memcpy(&m_MoveStuff, pMoveStuff, sizeof(SMoveStuff));

	SetAnim(pMoveStuff->byMoveFlags);
}


void CNtlBehaviorCharAirMove::SetAnim(RwUInt8 byMoveFlags)
{
	CNtlSobCharProxy* pSobProxy = reinterpret_cast<CNtlSobCharProxy*>(m_pActor->GetSobProxy());

	RwUInt32 uiCurrAnimKey = pSobProxy->GetBaseAnimationKey();
	RwUInt32 uiNextAnimKey = 0;

	if (m_MoveStuff.byType == NTL_MOVETARGET_AIR_DASH)
	{
		uiNextAnimKey = FLYING_JUMP_HIGH;

		CreateAirEffect("max_fly_Up_effect_01");
		UpdateAirEffect(m_MoveStuff.vDir);
	}
	else if (m_MoveStuff.byType == NTL_MOVETARGET_DIR)
	{
		if (m_MoveStuff.byFormFlag == NTL_MOVE_FLAG_FLY)
		{
			if (byMoveFlags == NTL_MOVE_F || byMoveFlags == NTL_MOVE_F_TURN_L || byMoveFlags == NTL_MOVE_F_TURN_R)
			{
				uiNextAnimKey = FLYING_MOVE_FRONT;
			}
			else if (byMoveFlags == NTL_MOVE_B || byMoveFlags == NTL_MOVE_B_TURN_L || byMoveFlags == NTL_MOVE_B_TURN_R)
			{
				uiNextAnimKey = FLYING_MOVE_BACK;
			}
			else if (byMoveFlags == NTL_MOVE_L || byMoveFlags == NTL_MOVE_L_TURN_L || byMoveFlags == NTL_MOVE_L_TURN_R)
			{
				uiNextAnimKey = FLYING_MOVE_LEFT;
			}
			else if (byMoveFlags == NTL_MOVE_R || byMoveFlags == NTL_MOVE_R_TURN_L || byMoveFlags == NTL_MOVE_R_TURN_R)
			{
				uiNextAnimKey = FLYING_MOVE_RIGHT;
			}
			else if (byMoveFlags == NTL_MOVE_TURN_L || byMoveFlags == NTL_MOVE_TURN_R)
			{
				uiNextAnimKey = FLYING_STAND_IDLE;
			}
			else if (byMoveFlags == NTL_MOVE_HEIGHT_DOWN)
			{
				uiNextAnimKey = FLYING_STAND_DOWN;
			}
			else if (byMoveFlags == NTL_MOVE_HEIGHT_UP)
			{
				uiNextAnimKey = FLYING_STAND_UP;
			}
		}
		else if (m_MoveStuff.byFormFlag == NTL_MOVE_FLAG_FLY_DASH)
		{
			CreateAirEffect("max_floating_foward_01");
			UpdateAirEffect(m_MoveStuff.vDir);

			uiNextAnimKey = FLYING_DASH_FRONT;
		}
		else if (m_MoveStuff.byFormFlag == NTL_MOVE_FLAG_FLY_ACCEL)
		{
			CreateAirEffect("max_fly_effect_01");
			UpdateAirEffect(m_MoveStuff.vDir);

			uiNextAnimKey = FLYING_POSE_1;
		}
	}
	else if (m_MoveStuff.byType == NTL_MOVETARGET_LOC)
	{
		if (m_MoveStuff.byFormFlag == NTL_MOVE_FLAG_FLY_DASH)
		{
			uiNextAnimKey = FLYING_DASH_FRONT;
		}
		else if (m_MoveStuff.byFormFlag == NTL_MOVE_FLAG_FLY_ACCEL)
		{
			uiNextAnimKey = FLYING_POSE_1;
		}
		else
		{
			uiNextAnimKey = FLYING_MOVE_FRONT;
		}
	}

	if (uiCurrAnimKey != uiNextAnimKey)
	{
		CNtlPLCharacter* pPLCharacter = reinterpret_cast<CNtlPLCharacter*>(pSobProxy->GetPLMainEntity());
		pPLCharacter->SetBlend(BLEND_TWEEN, 0.0f, 0.20f);

		pSobProxy->SetBaseAnimation(uiNextAnimKey);
	}
}


void CNtlBehaviorCharAirMove::CreateAirEffect(const RwChar* effectName)
{
	if (!m_pActor)
		return;

	DestroyAirEffect();

	m_pPLAirEffect = GetSceneManager()->CreateEntity(PLENTITY_EFFECT, effectName);

	if (m_MoveStuff.byFormFlag == NTL_MOVE_FLAG_FLY_ACCEL)
	{
		RwV3d scale;
		scale.x = 1.5f;
		scale.y = 1.5f;
		scale.z = 1.5f;

		RwV3d Offset;
		Offset.x = 0.0f;
		Offset.y = 0.0f;
		Offset.z = 0.0f;

		Helper_AttachBone(m_pActor->GetSobProxy()->GetPLMainEntity(), m_pPLAirEffect, "Bip01", Offset, scale);
	}
	else
	{
		CNtlPLAttach* pPLChar = (CNtlPLAttach*)m_pActor->GetSobProxy()->GetPLMainEntity();
		pPLChar->Attach((CNtlPLAttach*)m_pPLAirEffect);
	}
}


void CNtlBehaviorCharAirMove::DestroyAirEffect(void)
{
	if (m_pPLAirEffect)
	{
		m_pPLAirEffect->Finish();
		m_pPLAirEffect = NULL;
	}
}





RwBool CNtlBehaviorCharAirMove::UpdatePositionMove(SMoveStuff* pMoveStuff, OUT RwV3d& vPos, RwBool bIncHeight, RwReal fElapsed, RwReal fSpeed)
{
	if (pMoveStuff->byMoveFlags != NTL_MOVE_NONE)
	{
		RwV3d vDir = m_pActor->GetDirection();

		CNtlVector vHeading, vDest;
		//	DBO_WARNING_MESSAGE("pMoveStuff->byMoveFlags: " << (int)pMoveStuff->byMoveFlags);
		NtlGetDestination_Keyboard(vDir.x, vDir.y, vDir.z, fSpeed, vPos.x, vPos.y, vPos.z, pMoveStuff->byMoveFlags, fElapsed * 1000.f, 1.0f, &vHeading, &vDest);

		RwV3d vNewDir;
		CNtlMath::MathRwV3dAssign(&vNewDir, vHeading.x, vHeading.y, vHeading.z);

		m_pActor->SetDirection(&vNewDir);

		vPos.x = vDest.x;
		vPos.y = vDest.y;
		vPos.z = vDest.z;

		// If it's a player, correct Move Sync.
		//if (m_pActor->GetClassID() == SLCLASS_PLAYER)
		//{
			//UpdateMoveSync(vPos, fElapsed);

			// Apply the calculated result.
			//vPos = m_pActor->GetPosition();
		//	return FALSE; // return, we dont need collision check on player
		//}

		// Prevent flying above the defined height.
		Logic_GetWorldHeight(m_pActor, &vPos, m_sHStuff);
		if (vPos.y >=  MAX_ALLOWED_FLY_HEIGHT)
		{
			vPos.y = MAX_ALLOWED_FLY_HEIGHT;
		}

		// Collision check.
		if (m_pActor->GetFlags() & SLFLAG_OBJECT_COLLISION)
		{
			RwReal fCurrHeight = vPos.y;
			RwBool bCollMoveImPossible;
			RwUInt8 byColliResult = Logic_CharacterCollisionEx(m_pActor, &vPos, fSpeed, vPos, bCollMoveImPossible, fElapsed, FALSE);
			vPos.y = fCurrHeight;

			// Uncommenting this will make the player fall when colliding with an obstacle (like invisible walls).
			// TW client doesn't have this behavior so I leave it commented out.
			//if (byColliResult != NTL_CHARACTER_COLLI_NONE && bCollMoveImPossible)
			//{
			//	SetFalling();
			//	return TRUE;
			//}
		}

		if (vPos.y <= m_sHStuff.fFinialHeight + 0.25f) {
			vPos.y = m_sHStuff.fFinialHeight;
			SetFalling();
			return TRUE;
		}
	}

	m_pActor->SetPosition(&vPos);
	return FALSE;
}

RwBool CNtlBehaviorCharAirMove::UpdateAirStart(SMoveStuff* pMoveStuff, RwReal fElapsed)
{
	RwV3d vPos = m_pActor->GetPosition();
	RwReal fMoveSpeed = Logic_GetFlyDashSpeed(m_pActor);

	return UpdatePositionMove(pMoveStuff, vPos, TRUE, fElapsed, fMoveSpeed);
}

RwBool CNtlBehaviorCharAirMove::UpdateAirFly(SMoveStuff* pMoveStuff, RwReal fElapsed)
{
	RwV3d vPos = m_pActor->GetPosition();
	RwReal fSpeed = Logic_GetFrontFlySpeed(m_pActor);

	return UpdatePositionMove(pMoveStuff, vPos, TRUE, fElapsed, fSpeed);
}

RwBool CNtlBehaviorCharAirMove::UpdateAirDash(SMoveStuff* pMoveStuff, RwReal fElapsed)
{
	RwV3d vPos = m_pActor->GetPosition();
	RwReal fSpeed = Logic_GetFlyDashSpeed(m_pActor);

	return UpdatePositionMove(pMoveStuff, vPos, TRUE, fElapsed, fSpeed);
}

RwBool CNtlBehaviorCharAirMove::UpdateAirAccel(SMoveStuff* pMoveStuff, RwReal fElapsed)
{
	RwV3d vPos = m_pActor->GetPosition();
	RwReal fSpeed = Logic_GetFlyAccelSpeed(m_pActor);

	return UpdatePositionMove(pMoveStuff, vPos, TRUE, fElapsed, fSpeed);
}

RwBool CNtlBehaviorCharAirMove::UpdateMoveSync(RwV3d vPos, RwReal fElapsedTime)
{
	if (m_pActor->GetClassID() != SLCLASS_PLAYER)
		return FALSE;

	CNtlBeCharData* pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData());
	SMoveSyncStuff* pMoveSyncStuff = pBeData->GetMoveSyncStuff();

	if (pMoveSyncStuff->m_pMoveSyncCurr == NULL)
	{
		if (pMoveSyncStuff->Next() == NULL)         // I have not received the packet yet.      
			return FALSE;
	}
	else
	{
		if (!pMoveSyncStuff->m_MoveSyncQ.empty())        // Receive and process the next packet.
		{
			pMoveSyncStuff->Next();
		}
	}

	// After calculating the position that should be in accordance with the original MoveSync, determine the force to go to that position.    
	RwV3d vSyncDir = pMoveSyncStuff->m_pMoveSyncCurr->vLoc - vPos;
	/*vSyncDir.y = 0.0f;*/
	RwReal vSyncDistance = RwV3dLength(&vSyncDir);
	RwV3dNormalize(&vSyncDir, &vSyncDir);
	vPos += vSyncDir * (vSyncDistance / MOVE_SYNC_SPEED) * fElapsedTime * 2.0f;

	// Final position
	m_pActor->SetPosition(&vPos);

	return TRUE;
}

void CNtlBehaviorCharAirMove::SetFalling()
{
	CNtlBeCharData* pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData());

	SMoveStuff* pMoveStuff = pBeData->GetMoveStuff();
	pMoveStuff->byMoveResult |= NTL_MOVE_RESULT_FALLING;

	SCtrlStuff* pCtrlStuff = pBeData->GetCtrlStuff();

	pCtrlStuff->uExtra.sFalling.vFallingDir = m_pActor->GetDirection();

	pCtrlStuff->uExtra.sFalling.byMoveDirection = m_MoveStuff.byMoveFlags;
	pCtrlStuff->uExtra.sFalling.fSpeed = DBO_FALLING_SPEED;
	pCtrlStuff->uExtra.sFalling.fSearchHeight = m_sHStuff.fFinialHeight;
}
