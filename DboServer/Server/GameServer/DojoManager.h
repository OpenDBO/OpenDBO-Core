#pragma once

#include "NtlSingleton.h"
#include "NtlDojo.h"
#include "DojoTable.h"
#include <vector>

class CWorld;
class CTriggerObject;

class CDojo
{

public:

	CDojo(sDBO_DOJO_DATA& dojoData, sDBO_GUILD_MARK* pMark);
	virtual ~CDojo();

public:

	inline TBLIDX								GetDojoTblidx() { return m_brief.dojoTblidx; }

	inline GUILDID								GetGuildID() { return m_brief.guildId; }

	inline sDBO_DOJO_BRIEF&						GetDojoBrief() { return m_brief; }

	void										SetDojoMark(sDBO_GUILD_MARK * pMark);
	void										SetDojoData(GUILDID guildId);

	inline void									SetAttGuild(GUILDID guildId) { m_attGuildId = guildId; }
	inline GUILDID								GetAttGuildID() { return m_attGuildId; }

	CWorld*										GetWorld() { return m_pWorld; }

	BYTE										GetWarState() { return m_matchStateInfo.eState; }

	inline bool									IsGmWar() { return m_bGmWar; }

public:

	void										TickProcess(DWORD dwTickDiff);

	void										GmStart();
	void										GmNext();
	void										GmStop();

	void										SetState(BYTE byState, DOJOTIME tmNextStepTime);

	void										SetUserCount(BYTE byDefCount, BYTE byAttCount);

	bool										CanEnterDojo(GUILDID guildId);

	bool										UpdateSeal(WCHAR* wchName, GUILDID guildId, TBLIDX sealIdx, BYTE & rbyMainState);

	void										SendSealState(HOBJECT hSession);
	void										SendSealUnderAttack(HOBJECT hSeal, GUILDID attackerGuildId);
	void										SendStopSealUnderAttack(HOBJECT hSeal);

private:

	void										SpawnSeals();

	void										DespawnSeals();

	void										SendWarPointNfy();

	void										UpdatePoints();

	void										DecideWinner(bool bTime);

	void										TeleportAllOut();

	void										EndWar();

	void										SendStateChange(BYTE byState, DOJOTIME tmNextStepTime);

	sDOJO_TBLDAT*								GetDojoTbldat();
	DOJOTIME									GetPhaseDuration(BYTE sDOJO_TBLDAT::* pDurationMinutes, DOJOTIME tmFallback);

	void										SkipCurrentStep();

private:

	sDBO_DOJO_BRIEF								m_brief;

	sDOJO_MATCHSTATE_INFO						m_matchStateInfo;

	sDOJO_SCRAMBLE_POINT						m_warPoint;

	DWORD										m_dwNextUpdate;

	GUILDID										m_attGuildId;

	CWorld*										m_pWorld;

	std::vector<CTriggerObject*>				m_setSeals;

	bool										m_bGmWar; // started by a gm, runs outside of the weekly schedule
};



class CDojoManager : public CNtlSingleton<CDojoManager>
{

public:

	CDojoManager();
	virtual ~CDojoManager();
	

private:

	typedef std::map<GUILDID, CDojo*> TMAP_DOJO;
	TMAP_DOJO m_map_Dojo;

private:

	void										Init();

public:

	inline BYTE									GetDojoCount() { return (BYTE)m_map_Dojo.size(); }

	inline TMAP_DOJO::iterator					GetDojoSetBegin() { return m_map_Dojo.begin(); }
	inline TMAP_DOJO::iterator					GetDojoSetEnd() { return m_map_Dojo.end(); }

	void										UpdateDojoMark(GUILDID guildId, sDBO_GUILD_MARK* mark);

	CDojo*										GetDojo(GUILDID guildId);
	CDojo*										GetDojoWithTblidx(TBLIDX dojoTblidx);

	sDBO_DOJO_BRIEF*							GetDojoBrief(GUILDID guildId);

	void										AddDojo(sDBO_DOJO_DATA& dojoData, sDBO_GUILD_MARK* pMark);
	void										DelDojo(GUILDID guildid);

	void										FillDojoData(sDBO_DOJO_BRIEF* pBrief, BYTE & rCount);

	void										RemoveDojo(GUILDID guildId);
	void										AddDojo(GUILDID guildId, CDojo* pDojo);

	void										OnGmCommand(BYTE byCommand, TBLIDX dojoTblidx);

private:

	DWORD							m_dwNextUpdateTick;

public:

	void							TickProcess(DWORD dwTickDiff);

};

#define GetDojoManager()		CDojoManager::GetInstance()
#define g_pDojoManager			GetDojoManager()
