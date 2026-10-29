#pragma once

#include "DwcTable.h"

class CPlayer;
class CWorld;
class CWpsAlgoObject;

// the system quests the dwc scripts run (dwc 1: 13000-13012, dwc 2: 13100-13109), reset on every entry and exit
// because they can only be completed once and the dwc is played many times
const QUESTID	DWC_SYSTEM_QUEST_MIN = 13000;
const QUESTID	DWC_SYSTEM_QUEST_MAX = 13200;

enum eDWC_DUNGEON_STATE
{
	DWC_DUNGEON_STATE_PROPOSAL,		// every member has to accept the entry
	DWC_DUNGEON_STATE_DIRECTION,	// the enter direction plays, members are teleported afterwards
	DWC_DUNGEON_STATE_PLAY,
	DWC_DUNGEON_STATE_FAIL,			// members are sent home once the fail wait time is over
	DWC_DUNGEON_STATE_END,			// no member left, the dungeon can be deleted
};

class CDwcDungeon
{

	struct sMEMBER
	{
		CHARACTERID		charId;
		bool			bAccepted;
		bool			bInside;		// teleported into the dwc world
		WORLDID			returnWorldId;
		CNtlVector		vReturnLoc;
		CNtlVector		vReturnDir;
	};

public:

	CDwcDungeon(DWORD dwID, sDWC_TBLDAT* pTbldat, HOBJECT hNpc);
	virtual ~CDwcDungeon();

public:

	bool						Create(const std::vector<CPlayer*>& rMembers, bool bProposal);

	void						Destroy();

	bool						TickProcess(DWORD dwTickDiff); // true once the dungeon can be deleted

public:

	DWORD						GetID() { return m_dwID; }

	sDWC_TBLDAT*				GetTbldat() { return m_pTbldat; }

	CWorld*						GetWorld() { return m_pWorld; }

	BYTE						GetState() { return m_byState; }

public:

	void						Confirm(CPlayer* pPlayer, bool bEnter);

	void						Fail();

	void						Leave();

	void						LeaveDungeon(CPlayer* pPlayer);

	bool						GetReturnLocation(HOBJECT hPlayer, WORLDID& rWorldId, CNtlVector& rLoc, CNtlVector& rDir);

private:

	void						AddMember(CPlayer* pMember, bool bAccepted);

	void						Cancel(WORD wResultCode);

	void						StartDirection(bool bDirection);

	void						EnterWorld();

private:

	DWORD						m_dwID;

	sDWC_TBLDAT*				m_pTbldat;

	HOBJECT						m_hNpc;

	CWorld*						m_pWorld;

	CWpsAlgoObject*				m_pScript;

	BYTE						m_byState;

	DWORD						m_dwWaitTime;

	bool						m_bLeaving;

	std::map<HOBJECT, sMEMBER>	m_mapMembers;
};
