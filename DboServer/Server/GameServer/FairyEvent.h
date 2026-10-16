#ifndef __FAIRY_EVENT_SYSTEM__
#define __FAIRY_EVENT_SYSTEM__

#include "NtlSingleton.h"
#include "NtlSharedType.h"


class CMonster;
class CCharacter;

class CFairyEvent : public CNtlSingleton<CFairyEvent>
{

public:

	CFairyEvent();
	virtual ~CFairyEvent();

private:

	void				Init();

public:

	void				StartEvent(BYTE byHours = 3);

	void				EndEvent();

	void				LoadEvent(HSESSION hSession);

	bool				IsOn() { return m_bOn; }

	DBOTIME				GetRemainingTime();

	int					GetSummonsUntilCarp();

public:

	void				TickProcess(DWORD dwTick);

	void				Update(CMonster* pMob, CCharacter* pPlayer);

private:

	void				SpawnMob(CMonster* pMob, TBLIDX mobTblidx);

	void				DropItem(CMonster* pMob, CCharacter* pOwner, float fRate, float fRange);

private:

	bool				m_bOn;

	DBOTIME				m_timeStart;

	DBOTIME				m_timeEnd;

	DWORD				m_dwNextUpdateTick;

	int					m_nFairySummoned;

};

#define GetFairyEvent()			CFairyEvent::GetInstance()
#define g_pFairyEvent			GetFairyEvent()

#endif
