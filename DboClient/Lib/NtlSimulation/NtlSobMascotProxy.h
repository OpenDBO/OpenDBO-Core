/*****************************************************************************
 *
 * File			: NtlSobMascotProxy.h
 * Author		: Alejandro-HUB
 * Copyright	: (C)NTL
 * Date			: 2024. 11. 10
 * Abstract		: Simulation object mascot proxy class
 *****************************************************************************
 * Desc         : Owns the world model of a summoned mascot.
 *
 *****************************************************************************/

#ifndef _NTL_SOB_MASCOT_PROXY_H_
#define _NTL_SOB_MASCOT_PROXY_H_

#include <string>

#include "NtlSobProxy.h"

class CNtlPLCharacter;

class CNtlSobMascotProxy : public CNtlSobProxy
{
private:

	CNtlPLCharacter*	m_pCharacter;
	RwBool				m_bResLoaded;
	RwBool				m_bVisible;
	std::string			m_strModel;

public:

	CNtlSobMascotProxy();
	virtual ~CNtlSobMascotProxy();

	virtual RwBool	Create(RwUInt32 uiCompType);
	virtual void	Destroy(void);
	virtual void	Update(RwReal fElapsed);

	RwBool			CreatePLEntity(const RwChar* pModelKey);
	void			DeletePLEntity(void);
	RwBool			IsResourceLoaded(void) const { return m_bResLoaded; }

	virtual void	AddWorld(void);
	virtual void	RemoveWorld(void);

	virtual void	SetPosition(const RwV3d* pPos);
	virtual void	SetDirection(const RwV3d* pDir);
	virtual void	SetAngleY(RwReal fAngle);
	virtual void	SetDeltaAngleY(RwReal fAngle);
	virtual void	SetScale(RwReal fScale);
	virtual void	SetAlpha(RwUInt8 byAlpha);

	virtual void	SetBaseAnimation(RwUInt32 uiAnimKey, RwBool bLoop = TRUE, RwReal fStartTime = 0.0f);
	virtual RwBool	IsExistBaseAnimation(RwUInt32 uiAnimKey);
	virtual RwUInt32 GetBaseAnimationKey(void);
	virtual RwBool	IsBaseAnimationEnd(void);
	virtual void	SetAnimSpeed(RwReal fAniSpeed);
	virtual RwReal	GetAnimSpeed(void);
	RwReal			GetAnimPlayTime(RwUInt32 uiAnimKey);		///< length of an animation of the model, 0 when unknown

	virtual CNtlPLEntity* GetPLMainEntity(void);
	virtual RwV3d	GetBonePosition(const RwChar* pBoneName);
	virtual RwReal	GetPLEntityHeight(void);
	virtual RwReal	GetPLEntityWidth(void);
	virtual RwReal	GetPLEntityDepth(void);
	virtual RwReal	GetPLEntityBaseScale(void);
	virtual RwReal	GetScale(void);

	virtual void	EnableVisible(RwBool bEnable);
	RwBool			IsVisible(void) const { return m_bVisible; }
};

#endif
