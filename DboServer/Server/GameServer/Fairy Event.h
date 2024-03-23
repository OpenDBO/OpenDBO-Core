#ifndef __Fairy_EVENT_SYSTEM__
#define __Fairy_EVENT_SYSTEM__

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

public:

	void				TickProcess(DWORD dwTick);

	void				Update(CMonster* pMob, CCharacter* pPlayer);

private:

	bool				m_bOn;

	DBOTIME				m_timeStart;

	DBOTIME				m_timeEnd;

	DWORD				m_dwNextUpdateTick;

	int					m_nMonsterSummoned;

	int					FairyKilled;

};

#define GetFairyEvent()			CFairyEvent::GetInstance()
#define g_pFairyEvent			GetFairyEvent()

#endif#pragma once
