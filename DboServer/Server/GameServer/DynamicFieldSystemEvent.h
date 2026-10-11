#ifndef __SERVERSCRIPT_DYNAMIC_FIELD_EVENT_SYSTEM__
#define __SERVERSCRIPT_DYNAMIC_FIELD_EVENT_SYSTEM__

#include "NtlSingleton.h"
#include "DynamicFieldSystemTable.h"


class CMonster;
class CCharacter;
class CNtlPacket;
class CWpsAlgoObject;

/*
	Dynamic Field (World Boss) event.

	Every kill of a lv 40+ monster in a normal field increases a server wide counter (stored by the QueryServer).
	Once the counter reaches the setting count, the WPS script spawns the field bosses all over the main world
	for DBO_DYNAMIC_FIELD_EVENT_TIME seconds. Clients show the progress through the side icon and can see the
	boss locations on the world map while the event is running.
*/
class CDynamicFieldSystemEvent : public CNtlSingleton<CDynamicFieldSystemEvent>
{

public:

	CDynamicFieldSystemEvent();
	virtual ~CDynamicFieldSystemEvent();

private:

	void				Init();

	void				Destroy();

public:

	void				Create();

public:

	void				SetCount(DWORD dwCount) { m_dwCurCount = dwCount; }

	void				TickProcess(DWORD dwTickDiff);

	void				LoadDynamicField(CCharacter* pPlayer);

	void				UpdateDynamicFieldCount(DWORD dwCount);

	void				Update(CMonster* pMob, CCharacter* pPlayer); //called when killed a monster

	void				GetBosses(CCharacter* pPlayer);

	bool				IsEventActive() const { return m_bEventActive; }
	DWORD				GetCurCount() const { return m_dwCurCount; }
	DWORD				GetMaxCount() const { return m_dwMaxCount; }
	BYTE				GetBossCount() const { return m_bEventActive ? m_byBossCount : 0; }
	DBOTIME				GetRemainingEventTime() const { return GetRemainingEventTime(GetTickCount()); }

	// GM helpers (main thread only)
	bool				AddCount(DWORD dwCount);	// adds kills through the query server, like real kills
	bool				ForceStartEvent();
	bool				ForceEndEvent();

private:

	void				LoadSettings();

	void				FlushCounting();

	void				StartEvent(DWORD dwCurTick);

	void				EndEvent();

	CWpsAlgoObject*		GetEventWps() const;

	BYTE				CountAliveBosses() const;

	DBOTIME				GetRemainingEventTime(DWORD dwCurTick) const;

	void				BuildCountNfy(CNtlPacket& rPacket, DWORD dwCurTick) const;

	void				BroadcastCountNfy(DWORD dwCurTick) const;

	void				BroadcastSystemText(const WCHAR* pwszMessage) const;

	void				SendCountingReq(int nAddCount) const;

private:

	int					m_nAddCount;			// kills not yet sent to the query server
	BYTE				m_byBossCount;			// alive bosses (only valid while event is active)
	DWORD				m_dwCurCount;
	DWORD				m_dwMaxCount;			// kills required to start the event
	DWORD				m_dwEventTimeSecs;		// how long the bosses stay
	TBLIDX				m_wpsTblidx;			// WPS script which spawns the bosses
	DWORD				m_dwNextCountingFlush;	// remaining ms until pending kills are sent to the query server

	bool				m_bEventActive;
	DWORD				m_dwEventStartTick;		// GetTickCount() when the event started

	DWORD				m_dwNextStatusUpdate;	// remaining ms until we broadcast the event status again

};

#define GetDynamicFieldSystemEvent()			CDynamicFieldSystemEvent::GetInstance()
#define g_pDynamicFieldSystemEvent				GetDynamicFieldSystemEvent()

#endif
