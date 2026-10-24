#pragma once

#include "DungeonTable.h"

class CPlayer;
class CWorld;
class CWpsAlgoObject;

// Sky dungeons share the time leap dungeon instance. Entries are limited per day, the day starts at 04:00.
const BYTE		SKY_DUNGEON_DAILY_ENTER_LIMIT = 4;
const int		SKY_DUNGEON_DAILY_RESET_HOUR = 4;
const BYTE		SKY_DUNGEON_ENTER_MIN_LEVEL = 40;

class CTimeLeapDungeon
{

public:
	CTimeLeapDungeon(CPlayer* pPlayer, sDUNGEON_TBLDAT* pDungeonTbldat);
	virtual ~CTimeLeapDungeon();


public:

	bool						Create();

	void						Destroy();

public:

	CPlayer*					GetOwner() { return m_pOwner; }

	sDUNGEON_TBLDAT*			GetTbldat() { return m_pDungeonTbldat; }

	CWorld*						GetWorld() { return m_pWorld; }

private:

	CPlayer*					m_pOwner;

	sDUNGEON_TBLDAT*			m_pDungeonTbldat;

	CWorld*						m_pWorld;

};

