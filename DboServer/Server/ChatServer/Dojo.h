#ifndef __SERVER_DOJO___
#define __SERVER_DOJO___

#include "NtlDojo.h"
#include "EventableObject.h"

class CPlayer;
class CGuild;

class CDojo : public EventableObject
{

public:

	CDojo(sDBO_DOJO_DATA* dojo);
	virtual ~CDojo();

private:

	CGuild*								m_pkGuild;

	sDBO_DOJO_DATA						dojoData;

public:

	inline void							SetGuild(CGuild* guild) {m_pkGuild = guild; }
	inline CGuild*						GetGuild() { return m_pkGuild; }

	sDBO_DOJO_DATA*						GetDojoData() { return &dojoData; }

	void								ChangeDojoOwner(CGuild* pNewGuild);


//dojo war
public:

	void								RequestWar(GUILDID guildChallengerID, CHARACTERID requestorCharID, WORD& rwResultcode);

	void								OnEvent_WarRequestTimer();

	void								WarRequestReponse(bool& rbIsAccept, WORD& rwResultcode, bool& rbIsRetry);

	inline bool							CanReceiveWarRequest() { return m_eDojoState == eDBO_DOJO_STATUS_RECEIVE; }

	inline bool							CanRejectWar() { return m_byRejectCount < DBO_DOJO_SCRAMBLE_MAX_REJECT_COUNT; }

	inline eDBO_DOJO_STATUS				GetDojoWarStatus() { return m_eDojoState; }

	void								BeginWarPreparation();

	void								UpdateState(BYTE byState, DOJOTIME tmNextStepTime);

	void								Reset(BYTE byState, DOJOTIME tmNextStepTime);

	void								SendDojoState(HSESSION hSession);

	void								UpdateWarPoints(sDOJO_SCRAMBLE_POINT& warPoint);

	void								UpdatePlayerCount(GUILDID guildId, CHARACTERID charId, bool bPlus);

	void								UpdateSeal(GUILDID guildId, TBLIDX objectTblidx);

	bool								CanEnterDojo(GUILDID guildId);

	// teleport into the battle, kept so members coming online during the war can still join
	void								SetTeleportData(const sDBO_DOJO_TELEPORT_DATA& rData);
	void								SendTeleportProposal(CPlayer* pPlayer);

public:

	bool								GetWarRequest() { return m_bHasWarRequest; }
	void								SetWarRequest(bool bFlag) { m_bHasWarRequest = bFlag; }

	bool								GetWarInProgress() { return m_bWarInProgress; }

	CHARACTERID							GetChallengerCharID() { return m_lastChallengerCharID; }

private:

	bool								m_bHasTeleportData;
	sDBO_DOJO_TELEPORT_DATA				m_teleportData;
	DOJOTIME							m_tmTeleportEnd;

	DOJOTIME							m_lastDojoWar;
	std::set<GUILDID>					m_setRejectedGuilds;	// guilds refused during this challenge period

	CHARACTERID							m_lastChallengerCharID;

	BYTE								m_byRejectCount; //max DBO_DOJO_SCRAMBLE_MAX_REJECT_COUNT

	bool								m_bHasWarRequest;
	bool								m_bWarInProgress;

	eDBO_DOJO_STATUS					m_eDojoState;

	DOJOTIME							m_tmNextStepTime;

	sDOJO_SCRAMBLE_POINT				m_warPoint;

	std::set<CHARACTERID>				m_setDefender;
	std::set<CHARACTERID>				m_setAttacker;
};


#endif