/*****************************************************************************
 *
 * File			: NtlSobMascot.h
 * Author		: Alejandro-HUB
 * Copyright	: (C)NTL
 * Date			: 2024. 11. 10
 * Abstract		: Simulation object mascot
 *****************************************************************************
 * Desc         : A summoned mascot is not a server object. The client creates
 *				  one for every player whose mascot tblidx is valid; it follows
 *				  its owner and reacts to the owner's state with the animation
 *				  set every mascot model ships with.
 *****************************************************************************/

#ifndef __NTL_SOBMASCOT_H__
#define __NTL_SOBMASCOT_H__

#include "NtlSobActor.h"
#include "NtlSLDef.h"

class CNtlSobPlayer;
class CNtlSobMascotAttr;
class CNtlSobMascotProxy;

class CNtlSobMascot : public CNtlSobActor
{
	DECLEAR_MEMORY_POOL(CNtlSobMascot, NTL_DEFAULT_MEMORY_POOL)

public:

	enum EMascotState
	{
		MASCOT_STATE_SPAWN,			///< playing the appear animation
		MASCOT_STATE_IDLE,
		MASCOT_STATE_MOVE,
		MASCOT_STATE_ACTION,		///< one shot animation: idle variant, emotion or skill
		MASCOT_STATE_SLEEP,			///< owner is sitting
		MASCOT_STATE_FAINT,			///< owner is fainting
	};

private:

	SERIAL_HANDLE		m_hOwner;
	EMascotState		m_eState;
	RwBool				m_bFinish;

	RwUInt32			m_uiAnimKey;
	RwBool				m_bAnimLoop;
	RwReal				m_fActionTime;			///< time a one shot animation has played
	RwReal				m_fActionLength;

	RwUInt32			m_uiOwnerStateId;
	RwV3d				m_vLastOwnerPos;
	RwReal				m_fOwnerSpeed;

	RwReal				m_fIdleTime;
	RwReal				m_fNextIdleAction;

	RwBool				m_bDespawning;			///< dismissed, rising and fading out before it is removed
	RwReal				m_fDespawnTime;

public:

	CNtlSobMascot();
	virtual ~CNtlSobMascot();

	virtual RwBool		Create(void);
	virtual void		Destroy(void);
	virtual void		Update(RwReal fElapsed);
	virtual RwBool		IsFinish(void) { return m_bFinish; }

	void				SetOwner(SERIAL_HANDLE hOwner) { m_hOwner = hOwner; }
	SERIAL_HANDLE		GetOwner(void) const { return m_hOwner; }

	RwBool				SetMascotTblidx(TBLIDX mascotTblidx);
	TBLIDX				GetMascotTblidx(void);

	CNtlSobMascotAttr*	GetMascotAttr(void);
	CNtlSobMascotProxy*	GetMascotProxy(void);

	/// place the mascot next to its owner, optionally playing the appear animation
	void				Spawn(RwBool bPlayAppear);

	/// play a one shot animation (emotion, skill), ignored while the owner sits or faints
	void				PlayAction(RwUInt32 uiAnimKey);

	void				Finish(void) { m_bFinish = TRUE; }

private:

	CNtlSobPlayer*		GetOwnerPlayer(void);

	void				UpdateOwnerState(CNtlSobPlayer* pOwner);
	void				UpdateVisible(CNtlSobPlayer* pOwner);
	void				UpdateAnimation(RwReal fElapsed);
	void				UpdateFollow(CNtlSobPlayer* pOwner, RwReal fElapsed);

	void				ChangeAnim(RwUInt32 uiAnimKey, RwBool bLoop);
	void				ResetIdleTimer(void);

	void				GetFollowPos(CNtlSobPlayer* pOwner, RwV3d& vPos);
	RwReal				GetFloatHeight(CNtlSobPlayer* pOwner);	///< height the mascot floats at next to its owner
	void				SnapToOwner(CNtlSobPlayer* pOwner);
	void				TurnTo(const RwV3d& vDir, RwReal fElapsed);

	void				CreateAppearEffect(void);
	void				StartDespawn(void);
	void				UpdateDespawn(RwReal fElapsed);
};

#endif
