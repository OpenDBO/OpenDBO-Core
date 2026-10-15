#include "precomp_ntlsimulation.h"
#include "NtlSobMascot.h"

// shared
#include "GraphicDataTable.h"

// core
#include "NtlDebug.h"
#include "NtlMath.h"

// presentation
#include "NtlPLEntity.h"
#include "NtlCharacterData.h"

// simulation
#include "NtlSLLogic.h"
#include "NtlSLLogicDef.h"
#include "NtlSobManager.h"
#include "NtlSobPlayer.h"
#include "NtlFSMDef.h"
#include "NtlSobMascotAttr.h"
#include "NtlSobMascotProxy.h"

namespace
{
	const RwReal MASCOT_FOLLOW_SIDE			= 0.9f;		///< offset to the right of the owner
	const RwReal MASCOT_FOLLOW_BACK			= 0.9f;		///< offset behind the owner
	const RwReal MASCOT_START_MOVE_DIST		= 1.5f;		///< distance to the follow point that starts a move while the owner stands
	const RwReal MASCOT_STOP_MOVE_DIST		= 0.25f;
	const RwReal MASCOT_SNAP_DIST			= 25.0f;	///< further than this the mascot is put back next to its owner
	const RwReal MASCOT_MIN_SPEED			= 2.0f;
	const RwReal MASCOT_WALK_SPEED			= 4.0f;		///< below this speed the walk animation is played
	const RwReal MASCOT_ANIM_REF_SPEED		= 6.0f;		///< move speed the run animation was made for
	const RwReal MASCOT_FLOAT_RATIO			= 1.0f;		///< floats at this share of the owner's model height, about head level
	const RwReal MASCOT_FLOAT_DEFAULT		= 1.4f;		///< used while the owner model is not loaded
	const RwReal MASCOT_HEIGHT_FOLLOW		= 8.0f;		///< how fast the float height catches up, smooths owner jumps
	const RwReal MASCOT_SPAWN_DESCENT		= 3.0f;		///< slower height follow while the summoned mascot floats down
	const RwReal MASCOT_SPAWN_RISE			= 0.6f;		///< a summoned mascot appears this far above its owner's head
	const RwReal MASCOT_DESPAWN_TIME		= 0.6f;		///< a dismissed mascot rises and fades out for this long
	const RwReal MASCOT_DESPAWN_RISE_SPEED	= 1.5f;
	const RwChar* MASCOT_EFFECT_APPEAR		= "P_MCT_APPEAR";	///< summon and dismiss effect of every mascot model
	const RwReal MASCOT_IDLE_ACTION_MIN		= 8.0f;
	const RwReal MASCOT_IDLE_ACTION_MAX		= 16.0f;

	RwBool IsOwnerFainting(RwUInt32 uiStateId)
	{
		return uiStateId == NTL_FSMSID_FAINTING || uiStateId == NTL_FSMSID_DIE;
	}

	RwBool IsOwnerSitting(RwUInt32 uiStateId)
	{
		return uiStateId == NTL_FSMSID_SITDOWN || uiStateId == NTL_FSMSID_SIT;
	}

	RwBool IsOwnerHidden(RwUInt32 uiStateId)
	{
		return uiStateId == NTL_FSMSID_TELEPORT || uiStateId == NTL_FSMSID_ONBUS || uiStateId == NTL_FSMSID_DESPAWN;
	}
}

DEFINITION_MEMORY_POOL(CNtlSobMascot)

CNtlSobMascot::CNtlSobMascot()
{
	m_hOwner = INVALID_SERIAL_ID;
	m_eState = MASCOT_STATE_IDLE;
	m_bFinish = FALSE;

	m_uiAnimKey = INVALID_GRAPHIC_ANIMATION_ID;
	m_bAnimLoop = TRUE;
	m_fActionTime = 0.0f;
	m_fActionLength = 0.0f;

	m_uiOwnerStateId = NTL_FSMSID_INVALID;
	CNtlMath::MathRwV3dAssign(&m_vLastOwnerPos, 0.0f, 0.0f, 0.0f);
	m_fOwnerSpeed = 0.0f;

	m_fIdleTime = 0.0f;
	m_fNextIdleAction = MASCOT_IDLE_ACTION_MAX;

	m_bDespawning = FALSE;
	m_fDespawnTime = 0.0f;

	// a mascot is decoration: no targeting, no lua, no collision
	SetFlags(SLFLAG_ADD_UPDATE | SLFLAG_NOT_TARGETED | SLFLAG_NOT_HIGHLIGHTED | SLFLAG_NOT_WORLD_HEIGHT_CHECK);
	SetActorFlags(SLFLAG_ACTOR_HAVE_LIFE);
}

CNtlSobMascot::~CNtlSobMascot()
{
}

RwBool CNtlSobMascot::Create(void)
{
	NTL_FUNCTION("CNtlSobMascot::Create");

	m_pSobProxy = NTL_NEW CNtlSobMascotProxy;
	m_pSobProxy->Create(0);
	m_pSobProxy->SetSobObj(this);

	if (!CNtlSobActor::Create())
		NTL_RETURN(FALSE);

	SetClassName(SLCLASS_NAME_MASCOT);

	NTL_RETURN(TRUE);
}

void CNtlSobMascot::Destroy(void)
{
	CNtlSobActor::Destroy();
}

CNtlSobMascotAttr* CNtlSobMascot::GetMascotAttr(void)
{
	return reinterpret_cast<CNtlSobMascotAttr*>(GetSobAttr());
}

CNtlSobMascotProxy* CNtlSobMascot::GetMascotProxy(void)
{
	return reinterpret_cast<CNtlSobMascotProxy*>(GetSobProxy());
}

RwBool CNtlSobMascot::SetMascotTblidx(TBLIDX mascotTblidx)
{
	CNtlSobMascotAttr* pAttr = GetMascotAttr();
	if (pAttr == NULL || !pAttr->SetMascotTblidx(mascotTblidx))
		return FALSE;

	if (!GetMascotProxy()->CreatePLEntity(pAttr->GetModelName()))
		return FALSE;

	m_uiAnimKey = INVALID_GRAPHIC_ANIMATION_ID;
	return TRUE;
}

TBLIDX CNtlSobMascot::GetMascotTblidx(void)
{
	CNtlSobMascotAttr* pAttr = GetMascotAttr();
	return pAttr ? pAttr->GetMascotTblidx() : INVALID_TBLIDX;
}

CNtlSobPlayer* CNtlSobMascot::GetOwnerPlayer(void)
{
	CNtlSob* pSob = GetNtlSobManager()->GetSobObject(m_hOwner);
	if (pSob == NULL)
		return NULL;

	if (pSob->GetClassID() != SLCLASS_PLAYER && pSob->GetClassID() != SLCLASS_AVATAR)
		return NULL;

	return reinterpret_cast<CNtlSobPlayer*>(pSob);
}

void CNtlSobMascot::Spawn(RwBool bPlayAppear)
{
	CNtlSobPlayer* pOwner = GetOwnerPlayer();
	if (pOwner)
	{
		SnapToOwner(pOwner);
		m_uiOwnerStateId = Logic_GetActorStateId(pOwner);
	}

	ResetIdleTimer();

	if (bPlayAppear && GetSobProxy()->IsExistBaseAnimation(NML_SPAWN))
	{
		m_eState = MASCOT_STATE_SPAWN;
		ChangeAnim(NML_SPAWN, FALSE);

		// appears above the owner's head and floats down to its place
		if (pOwner)
		{
			RwV3d vPos = GetPosition();
			RwReal fOwnerHeight = pOwner->GetSobProxy()->GetPLEntityHeight() * pOwner->GetSobProxy()->GetPLEntityBaseScale();
			vPos.y = pOwner->GetPosition().y + max(fOwnerHeight, MASCOT_FLOAT_DEFAULT) + MASCOT_SPAWN_RISE;
			SetPosition(&vPos);
		}

		CreateAppearEffect();
	}
	else
	{
		m_eState = MASCOT_STATE_IDLE;
		ChangeAnim(NML_IDLE_LOOP, TRUE);
	}

	if (pOwner)
		UpdateOwnerState(pOwner);
}

void CNtlSobMascot::PlayAction(RwUInt32 uiAnimKey)
{
	if (m_eState == MASCOT_STATE_SLEEP || m_eState == MASCOT_STATE_FAINT)
		return;

	if (!GetSobProxy()->IsExistBaseAnimation(uiAnimKey))
		return;

	m_eState = MASCOT_STATE_ACTION;
	ChangeAnim(uiAnimKey, FALSE);
}

void CNtlSobMascot::Update(RwReal fElapsed)
{
	if (m_bFinish)
		return;

	if (m_bDespawning)
	{
		UpdateDespawn(fElapsed);
		CNtlSobActor::Update(fElapsed);
		return;
	}

	// gone with an owner that left our view
	CNtlSobPlayer* pOwner = GetOwnerPlayer();
	if (pOwner == NULL)
	{
		m_bFinish = TRUE;
		return;
	}

	// the owner tells us we were dismissed by dropping our handle
	if (pOwner->GetMascotSerial() != GetSerialID())
	{
		StartDespawn();
		return;
	}

	UpdateOwnerState(pOwner);
	UpdateVisible(pOwner);
	UpdateFollow(pOwner, fElapsed);
	UpdateAnimation(fElapsed);

	CNtlSobActor::Update(fElapsed);
}

void CNtlSobMascot::UpdateOwnerState(CNtlSobPlayer* pOwner)
{
	RwUInt32 uiStateId = Logic_GetActorStateId(pOwner);
	if (uiStateId == m_uiOwnerStateId)
		return;

	RwUInt32 uiOldStateId = m_uiOwnerStateId;
	m_uiOwnerStateId = uiStateId;

	if (IsOwnerFainting(uiStateId))
	{
		if (m_eState != MASCOT_STATE_FAINT)
		{
			m_eState = MASCOT_STATE_FAINT;
			ChangeAnim(GetSobProxy()->IsExistBaseAnimation(FAINTING) ? FAINTING : SOC_SAD, FALSE);
		}
		return;
	}

	if (m_eState == MASCOT_STATE_FAINT)
	{
		// owner got back up; revivals that teleport are snapped back in UpdateFollow
		m_eState = MASCOT_STATE_IDLE;
		ChangeAnim(NML_IDLE_LOOP, TRUE);
		if (!IsOwnerHidden(uiStateId))
			PlayAction(SOC_HAPPY);
		return;
	}

	if (IsOwnerSitting(uiStateId))
	{
		if (m_eState != MASCOT_STATE_SLEEP && GetSobProxy()->IsExistBaseAnimation(PC_TRIGGER_ANIMATION_START))
		{
			m_eState = MASCOT_STATE_SLEEP;
			ChangeAnim(PC_TRIGGER_ANIMATION_START, TRUE);
		}
		return;
	}

	if (m_eState == MASCOT_STATE_SLEEP)
	{
		m_eState = MASCOT_STATE_IDLE;
		ChangeAnim(NML_IDLE_LOOP, TRUE);
		ResetIdleTimer();
	}

	// copy the owner's emotes the mascot has an animation for
	if (uiStateId == NTL_FSMSID_SOCIAL_ACTION)
	{
		RwUInt32 uiOwnerAnim = pOwner->GetSobProxy()->GetBaseAnimationKey();
		if (uiOwnerAnim >= SOCIAL_ANIMATION_START && uiOwnerAnim <= SOCIAL_ANIMATION_END)
			PlayAction(uiOwnerAnim);
	}
}

void CNtlSobMascot::UpdateVisible(CNtlSobPlayer* pOwner)
{
	CNtlSobProxy* pOwnerProxy = pOwner->GetSobProxy();
	CNtlPLEntity* pOwnerEntity = pOwnerProxy ? pOwnerProxy->GetPLMainEntity() : NULL;

	RwBool bVisible = pOwnerEntity != NULL
		&& pOwnerEntity->IsVisible()
		&& pOwnerProxy->IsProxySystemVisible()
		&& !IsOwnerHidden(m_uiOwnerStateId);

	GetSobProxy()->EnableVisible(bVisible);
}

void CNtlSobMascot::UpdateAnimation(RwReal fElapsed)
{
	CNtlSobMascotProxy* pProxy = GetMascotProxy();
	if (!pProxy->IsResourceLoaded())
		return;

	if (m_eState == MASCOT_STATE_SPAWN || m_eState == MASCOT_STATE_ACTION)
	{
		// timed rather than waiting for the end flag, which is not reliable for one shot animations
		m_fActionTime += fElapsed;
		if (m_fActionTime >= m_fActionLength)
		{
			m_eState = MASCOT_STATE_IDLE;
			ChangeAnim(NML_IDLE_LOOP, TRUE);
			ResetIdleTimer();
		}
		return;
	}

	if (m_eState != MASCOT_STATE_IDLE)
		return;

	// play one of the idle variants from time to time
	m_fIdleTime += fElapsed;
	if (m_fIdleTime >= m_fNextIdleAction)
	{
		ResetIdleTimer();
		PlayAction((rand() % 2) == 0 ? NML_IDLE_01 : NML_IDLE_02);
	}
}

void CNtlSobMascot::UpdateFollow(CNtlSobPlayer* pOwner, RwReal fElapsed)
{
	if (fElapsed <= 0.0f)
		return;

	RwV3d vOwnerPos = pOwner->GetPosition();

	RwV3d vOwnerDelta;
	RwV3dSub(&vOwnerDelta, &vOwnerPos, &m_vLastOwnerPos);
	vOwnerDelta.y = 0.0f;
	RwReal fOwnerStep = RwV3dLength(&vOwnerDelta);
	m_vLastOwnerPos = vOwnerPos;

	// owner teleported: do not count the jump as speed
	if (fOwnerStep > MASCOT_SNAP_DIST)
	{
		m_fOwnerSpeed = 0.0f;
		SnapToOwner(pOwner);
		return;
	}

	m_fOwnerSpeed = m_fOwnerSpeed * 0.8f + (fOwnerStep / fElapsed) * 0.2f;

	if (m_eState == MASCOT_STATE_SLEEP || m_eState == MASCOT_STATE_FAINT)
		return;

	RwV3d vPos = GetPosition();
	RwV3d vTarget;
	GetFollowPos(pOwner, vTarget);

	RwV3d vToTarget;
	RwV3dSub(&vToTarget, &vTarget, &vPos);
	vToTarget.y = 0.0f;
	RwReal fDist = RwV3dLength(&vToTarget);

	if (fDist > MASCOT_SNAP_DIST)
	{
		SnapToOwner(pOwner);
		return;
	}

	RwBool bOwnerMoving = m_fOwnerSpeed > 0.5f;

	if (m_eState == MASCOT_STATE_MOVE)
	{
		if (fDist <= MASCOT_STOP_MOVE_DIST && !bOwnerMoving)
		{
			m_eState = MASCOT_STATE_IDLE;
			ChangeAnim(NML_IDLE_LOOP, TRUE);
			GetSobProxy()->SetAnimSpeed(1.0f);
			ResetIdleTimer();
		}
	}
	else if (fDist > MASCOT_START_MOVE_DIST || (bOwnerMoving && fDist > MASCOT_STOP_MOVE_DIST))
	{
		// interrupts idle variants and emotes
		m_eState = MASCOT_STATE_MOVE;
	}

	if (m_eState != MASCOT_STATE_MOVE)
	{
		// stay on the ground and look where the owner looks
		vPos.y += (GetFloatHeight(pOwner) - vPos.y) * min(1.0f, fElapsed * (m_eState == MASCOT_STATE_SPAWN ? MASCOT_SPAWN_DESCENT : MASCOT_HEIGHT_FOLLOW));
		SetPosition(&vPos);
		TurnTo(pOwner->GetDirection(), fElapsed);
		return;
	}

	// keep up with the owner and close the gap when lagging behind
	RwReal fRunSpeed = Logic_GetFrontRunSpeed(pOwner);
	RwReal fSpeed = max(m_fOwnerSpeed, min(fRunSpeed, fDist * 3.0f));
	if (fDist > 4.0f)
		fSpeed *= 1.3f;
	fSpeed = max(fSpeed, MASCOT_MIN_SPEED);

	RwReal fStep = min(fSpeed * fElapsed, fDist);

	RwV3d vMoveDir = vToTarget;
	if (fDist > 0.001f)
		RwV3dScale(&vMoveDir, &vMoveDir, 1.0f / fDist);

	vPos.x += vMoveDir.x * fStep;
	vPos.z += vMoveDir.z * fStep;
	vPos.y += (GetFloatHeight(pOwner) - vPos.y) * min(1.0f, fElapsed * MASCOT_HEIGHT_FOLLOW);
	SetPosition(&vPos);

	if (fDist > 0.05f)
		TurnTo(vMoveDir, fElapsed);

	ChangeAnim(fSpeed < MASCOT_WALK_SPEED ? WALK_FRONT : RUN_FRONT, TRUE);

	RwReal fAnimSpeed = fSpeed / MASCOT_ANIM_REF_SPEED;
	fAnimSpeed = max(0.7f, min(fAnimSpeed, 2.0f));
	GetSobProxy()->SetAnimSpeed(fAnimSpeed);
}

void CNtlSobMascot::ChangeAnim(RwUInt32 uiAnimKey, RwBool bLoop)
{
	CNtlSobProxy* pProxy = GetSobProxy();

	if (!pProxy->IsExistBaseAnimation(uiAnimKey))
	{
		// every mascot model has an idle loop
		if (uiAnimKey == NML_IDLE_LOOP)
			return;

		uiAnimKey = NML_IDLE_LOOP;
		bLoop = TRUE;
	}

	// looping animations are only restarted when they change
	if (bLoop && m_bAnimLoop && m_uiAnimKey == uiAnimKey)
		return;

	if (uiAnimKey != RUN_FRONT && uiAnimKey != WALK_FRONT)
		pProxy->SetAnimSpeed(1.0f);

	m_uiAnimKey = uiAnimKey;
	m_bAnimLoop = bLoop;

	if (!bLoop)
	{
		m_fActionTime = 0.0f;
		m_fActionLength = GetMascotProxy()->GetAnimPlayTime(uiAnimKey);
		if (m_fActionLength <= 0.0f)
			m_fActionLength = 2.0f;
	}
	pProxy->SetBaseAnimation(uiAnimKey, bLoop);
}

void CNtlSobMascot::ResetIdleTimer(void)
{
	m_fIdleTime = 0.0f;
	m_fNextIdleAction = MASCOT_IDLE_ACTION_MIN + (MASCOT_IDLE_ACTION_MAX - MASCOT_IDLE_ACTION_MIN) * ((RwReal)rand() / (RwReal)RAND_MAX);
}

void CNtlSobMascot::GetFollowPos(CNtlSobPlayer* pOwner, RwV3d& vPos)
{
	vPos = pOwner->GetPosition();

	RwV3d vDir = pOwner->GetDirection();
	vDir.y = 0.0f;
	if (RwV3dLength(&vDir) < 0.001f)
		CNtlMath::MathRwV3dAssign(&vDir, 0.0f, 0.0f, 1.0f);
	else
		RwV3dNormalize(&vDir, &vDir);

	// right hand side of the owner, slightly behind
	RwV3d vRight;
	CNtlMath::MathRwV3dAssign(&vRight, -vDir.z, 0.0f, vDir.x);

	vPos.x += vRight.x * MASCOT_FOLLOW_SIDE - vDir.x * MASCOT_FOLLOW_BACK;
	vPos.z += vRight.z * MASCOT_FOLLOW_SIDE - vDir.z * MASCOT_FOLLOW_BACK;
}

RwReal CNtlSobMascot::GetFloatHeight(CNtlSobPlayer* pOwner)
{
	// the owner's height is already on the real floor; world height queries return the terrain
	// under town floors and bridges, which buried the mascot
	RwReal fOwnerHeight = pOwner->GetSobProxy()->GetPLEntityHeight() * pOwner->GetSobProxy()->GetPLEntityBaseScale();
	RwReal fFloat = fOwnerHeight > 0.1f ? fOwnerHeight * MASCOT_FLOAT_RATIO : MASCOT_FLOAT_DEFAULT;

	return pOwner->GetPosition().y + fFloat;
}

void CNtlSobMascot::SnapToOwner(CNtlSobPlayer* pOwner)
{
	RwV3d vPos;
	GetFollowPos(pOwner, vPos);
	vPos.y = GetFloatHeight(pOwner);

	m_vLastOwnerPos = pOwner->GetPosition();

	SetPosition(&vPos);

	RwV3d vDir = pOwner->GetDirection();
	SetDirection(&vDir);

	if (m_eState == MASCOT_STATE_MOVE)
	{
		m_eState = MASCOT_STATE_IDLE;
		ChangeAnim(NML_IDLE_LOOP, TRUE);
		ResetIdleTimer();
	}
}

void CNtlSobMascot::TurnTo(const RwV3d& vDir, RwReal fElapsed)
{
	RwV3d vTarget = vDir;
	vTarget.y = 0.0f;
	if (RwV3dLength(&vTarget) < 0.001f)
		return;
	RwV3dNormalize(&vTarget, &vTarget);

	RwV3d vCur = GetDirection();
	vCur.y = 0.0f;

	RwReal fWeight = min(1.0f, fElapsed * 10.0f);
	RwV3d vNew;
	vNew.x = vCur.x + (vTarget.x - vCur.x) * fWeight;
	vNew.y = 0.0f;
	vNew.z = vCur.z + (vTarget.z - vCur.z) * fWeight;

	if (RwV3dLength(&vNew) < 0.001f)
		vNew = vTarget;

	SetDirection(&vNew);
}

void CNtlSobMascot::CreateAppearEffect(void)
{
	RwV3d vPos = GetPosition();

	CNtlPLEntity* pEffect = GetSobProxy()->CreatePLEffect(MASCOT_EFFECT_APPEAR, &vPos, TRUE);
	if (pEffect)
		pEffect->SetScale(GetSobProxy()->GetPLEntityBaseScale());
}

void CNtlSobMascot::StartDespawn(void)
{
	m_bDespawning = TRUE;
	m_fDespawnTime = 0.0f;

	CreateAppearEffect();
}

void CNtlSobMascot::UpdateDespawn(RwReal fElapsed)
{
	m_fDespawnTime += fElapsed;
	if (m_fDespawnTime >= MASCOT_DESPAWN_TIME)
	{
		m_bFinish = TRUE;
		return;
	}

	// rises and fades out
	RwV3d vPos = GetPosition();
	vPos.y += MASCOT_DESPAWN_RISE_SPEED * fElapsed;
	SetPosition(&vPos);

	RwReal fAlpha = 1.0f - m_fDespawnTime / MASCOT_DESPAWN_TIME;
	GetSobProxy()->SetAlpha((RwUInt8)(255.0f * fAlpha));
}
