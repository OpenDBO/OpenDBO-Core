#include "precomp_ntlsimulation.h"
#include "NtlSobMascotProxy.h"

// core
#include "NtlDebug.h"
#include "NtlMath.h"

// presentation
#include "NtlPLSceneManager.h"
#include "NtlPLCharacter.h"
#include "NtlCharacterData.h"
#include "NtlPLPropertyContainer.h"
#include "NtlPLCharacterProperty.h"
#include "NtlTypeAnimData.h"

// simulation
#include "NtlSob.h"


CNtlSobMascotProxy::CNtlSobMascotProxy()
{
	m_pCharacter = NULL;
	m_bResLoaded = FALSE;
	m_bVisible = TRUE;
}

CNtlSobMascotProxy::~CNtlSobMascotProxy()
{
}

RwBool CNtlSobMascotProxy::Create(RwUInt32 uiCompType)
{
	return CNtlSobProxy::Create(uiCompType);
}

void CNtlSobMascotProxy::Destroy(void)
{
	DeletePLEntity();

	CNtlSobProxy::Destroy();
}

void CNtlSobMascotProxy::Update(RwReal fElapsed)
{
	// keep the model hidden until its clump is loaded so it never pops in at the origin
	if (m_pCharacter && !m_bResLoaded && m_pCharacter->GetCharScheduleResInfo()->bLoadComplete)
	{
		m_bResLoaded = TRUE;
		m_pCharacter->SetVisible(m_bVisible);
	}

	CNtlSobProxy::Update(fElapsed);
}

RwBool CNtlSobMascotProxy::CreatePLEntity(const RwChar* pModelKey)
{
	DeletePLEntity();

	if (pModelKey == NULL || pModelKey[0] == '\0')
		return FALSE;

	// creating a character without its property crashes the scene manager
	if (CNtlPLPropertyContainer::GetInstance()->GetProperty(PLENTITY_CHARACTER, pModelKey) == NULL)
	{
		DBO_WARNING_MESSAGE("Mascot model has no character property: " << pModelKey);
		return FALSE;
	}

	RwV3d vPos = m_pSobObj->GetPosition();

	SPLCharacterCreateParam sParam;
	sParam.pPos = &vPos;
	sParam.uiSerialId = m_pSobObj->GetSerialID();
	sParam.bPcFlag = FALSE;
	sParam.uiClass = 0;
	sParam.uiRace = 0;
	sParam.uiGender = 0;
	sParam.uiFaceType = 0;
	sParam.uiHeadType = 0;
	sParam.uiHeadColorType = 0;
	sParam.uiSkinColorType = 0;
	sParam.bNotShadingFlag = FALSE;
	sParam.bIsAdult = FALSE;

	m_pCharacter = static_cast<CNtlPLCharacter*>(GetSceneManager()->CreateEntity(PLENTITY_CHARACTER, pModelKey, &sParam));
	if (m_pCharacter == NULL)
	{
		DBO_WARNING_MESSAGE("Failed to create mascot model: " << pModelKey);
		return FALSE;
	}

	m_strModel = pModelKey;
	m_pCharacter->SetSerialID(m_pSobObj->GetSerialID());
	m_pCharacter->SetPicking(FALSE);

	// stored until the clump loads and applied before the first animation, so the appear
	// animation spawns its effects (a hidden character skips them)
	m_pCharacter->SetVisible(m_bVisible);
	m_bResLoaded = FALSE;

	return TRUE;
}

void CNtlSobMascotProxy::DeletePLEntity(void)
{
	if (m_pCharacter)
	{
		GetSceneManager()->DeleteEntity(m_pCharacter);
		m_pCharacter = NULL;
	}

	m_bResLoaded = FALSE;
}

void CNtlSobMascotProxy::AddWorld(void)
{
	if (m_pCharacter)
		m_pCharacter->AddWorld();
}

void CNtlSobMascotProxy::RemoveWorld(void)
{
	if (m_pCharacter)
		m_pCharacter->RemoveWorld();
}

void CNtlSobMascotProxy::SetPosition(const RwV3d* pPos)
{
	if (m_pCharacter)
		m_pCharacter->SetPosition(pPos);
}

void CNtlSobMascotProxy::SetDirection(const RwV3d* pDir)
{
	// CNtlPLCharacter::SetDirection adds to the current angle, so set the angle directly
	if (m_pCharacter)
		m_pCharacter->SetAngleY(CNtlMath::LineToAngleY(pDir));
}

void CNtlSobMascotProxy::SetAngleY(RwReal fAngle)
{
	if (m_pCharacter)
		m_pCharacter->SetAngleY(fAngle);
}

void CNtlSobMascotProxy::SetDeltaAngleY(RwReal fAngle)
{
	if (m_pCharacter)
		m_pCharacter->SetAngleY(m_pCharacter->GetAngleY() + fAngle);
}

void CNtlSobMascotProxy::SetScale(RwReal fScale)
{
	if (m_pCharacter)
		m_pCharacter->SetScale(fScale);
}

void CNtlSobMascotProxy::SetAlpha(RwUInt8 byAlpha)
{
	if (m_pCharacter)
		m_pCharacter->SetAlpha(byAlpha);
}

void CNtlSobMascotProxy::SetBaseAnimation(RwUInt32 uiAnimKey, RwBool bLoop, RwReal fStartTime)
{
	if (m_pCharacter)
		m_pCharacter->SetBaseAnimation(uiAnimKey, fStartTime, bLoop);
}

RwBool CNtlSobMascotProxy::IsExistBaseAnimation(RwUInt32 uiAnimKey)
{
	if (m_pCharacter)
		return m_pCharacter->IsExistAnim(uiAnimKey);

	return FALSE;
}

RwUInt32 CNtlSobMascotProxy::GetBaseAnimationKey(void)
{
	if (m_pCharacter)
		return m_pCharacter->GetCurBaseAnimKey();

	return INVALID_GRAPHIC_ANIMATION_ID;
}

RwBool CNtlSobMascotProxy::IsBaseAnimationEnd(void)
{
	if (m_pCharacter)
		return m_pCharacter->GetBaseCurrentAnimEnd();

	return TRUE;
}

void CNtlSobMascotProxy::SetAnimSpeed(RwReal fAniSpeed)
{
	if (m_pCharacter)
		m_pCharacter->SetAnimSpeed(fAniSpeed);
}

RwReal CNtlSobMascotProxy::GetAnimSpeed(void)
{
	if (m_pCharacter)
		return m_pCharacter->GetAnimSpeed();

	return 1.0f;
}

RwReal CNtlSobMascotProxy::GetAnimPlayTime(RwUInt32 uiAnimKey)
{
	CNtlPLCharacterProperty* pProperty = static_cast<CNtlPLCharacterProperty*>(CNtlPLPropertyContainer::GetInstance()->GetProperty(PLENTITY_CHARACTER, m_strModel.c_str()));
	if (pProperty == NULL)
		return 0.0f;

	STypeAnimData* pAnimData = pProperty->GetAnimTable()->Get(uiAnimKey);
	return pAnimData ? pAnimData->fPlayTime : 0.0f;
}

CNtlPLEntity* CNtlSobMascotProxy::GetPLMainEntity(void)
{
	return m_pCharacter;
}

RwV3d CNtlSobMascotProxy::GetBonePosition(const RwChar* pBoneName)
{
	if (m_pCharacter)
	{
		RwMatrix* pMat = m_pCharacter->GetBoneMatrix(pBoneName);
		if (pMat)
			return pMat->pos;
	}

	return m_pSobObj->GetPosition();
}

RwReal CNtlSobMascotProxy::GetPLEntityHeight(void)
{
	if (m_pCharacter)
		return m_pCharacter->GetHeight();

	return 0.0f;
}

RwReal CNtlSobMascotProxy::GetPLEntityWidth(void)
{
	if (m_pCharacter)
		return m_pCharacter->GetWidth();

	return 0.0f;
}

RwReal CNtlSobMascotProxy::GetPLEntityDepth(void)
{
	if (m_pCharacter)
		return m_pCharacter->GetDepth();

	return 0.0f;
}

RwReal CNtlSobMascotProxy::GetPLEntityBaseScale(void)
{
	if (m_pCharacter)
		return m_pCharacter->GetBaseScale();

	return 1.0f;
}

RwReal CNtlSobMascotProxy::GetScale(void)
{
	if (m_pCharacter)
		return m_pCharacter->GetScale();

	return 1.0f;
}

void CNtlSobMascotProxy::EnableVisible(RwBool bEnable)
{
	if (m_bVisible == bEnable)
		return;

	m_bVisible = bEnable;

	if (m_pCharacter)
		m_pCharacter->SetVisible(bEnable);

	CNtlSobProxy::EnableVisible(bEnable);
}
