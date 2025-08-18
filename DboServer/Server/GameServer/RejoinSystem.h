#pragma once
#include <cstdint>
#include <unordered_map>
#include "NtlVector.h"
#include "NtlSharedDef.h"
#include "World.h"

enum eREJOIN_DUNGEON_TYPE : BYTE
{
    REJOIN_TMQ = 0,
    REJOIN_TLQ = 1,
    REJOIN_UD = 2,
    REJOIN_BD = 3,
    REJOIN_CC = 5,
	REJOIN_BUDOKAI = 6,
};

struct sREJOIN_TARGET
{
    WORLDID    worldId;
    CNtlVector loc;
    CNtlVector dir;
};

struct sREJOIN_TICKET
{
	WORLDID		  worldId;
    CHARACTERID     charId;
    PARTYID         partyId;
    SERVERCHANNELID channelId;
    DWORD           expireAtMs;
    BYTE            dungeonType;
    uint64_t          inviteToken;   
};

struct sREJOIN_RECORD
{
    sREJOIN_TICKET ticket{};
    bool           hasSnapshot{ false };
    sREJOIN_TARGET snapshot{};
};

class CRejoinManager
{
public:
    void Put(const sREJOIN_TICKET& t);

    void PutWithTarget(const sREJOIN_TICKET& t, const sREJOIN_TARGET& target);

    sREJOIN_TICKET* Find(CHARACTERID cid);

    sREJOIN_RECORD* FindRecord(CHARACTERID cid);

    void Erase(CHARACTERID cid);

    bool ResolveRejoinTarget(const sREJOIN_TICKET& t, sREJOIN_TARGET& outTarget, CPlayer* cPlayer);

private:
    std::unordered_map<CHARACTERID, sREJOIN_RECORD> m_records;
};

extern CRejoinManager g_Rejoin;
