#pragma once

#include "NtlPacketCommon.h"
#include "NtlCSArchitecture.h"
#include "NtlStatistics.h"

enum eOPCODE_GM
{
	GM_OPCODE_BEGIN = 13000,

	GM_NOTIFY_SERVER_BEGIN = GM_OPCODE_BEGIN,
	GM_HEARTBEAT,
	GM_REPORT_LOAD,
	GM_REPORT_CONNECTED_USER_INFO,

	GM_REPORT_MONSTER_STATISTICS_BEGIN,
	GM_REPORT_MONSTER_STATISTICS,
	GM_REPORT_MONSTER_STATISTICS_SENDING_PAUSED_NFY,
	GM_REPORT_MONSTER_STATISTICS_END,

	GM_USERCOUNT_RES,
	GM_SERVER_CONTROL_TURN_OFF_ALL_NFY,
	GM_LOGIN_REQ,
	GM_LOGOUT_REQ,
	GM_MOVE_REQ,
	GM_KICKOUT_RES,
	GM_ON_PLAYER_INFO,

	GM_PING_REQ, // by itzrnb 08/05/2009

	GM_OPCODE_DUMMY,
	GM_OPCODE_END = GM_OPCODE_DUMMY - 1
};


//------------------------------------------------------------------
//
//------------------------------------------------------------------
const char * NtlGetPacketName_GM(WORD wOpCode);
//------------------------------------------------------------------


#pragma pack(1)

//------------------------------------------------------------------
BEGIN_PROTOCOL(GM_NOTIFY_SERVER_BEGIN)
	DWORD		dwMaxLoad;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GM_HEARTBEAT)
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GM_REPORT_LOAD)
	DWORD		dwMaxLoad;
	DWORD		dwLoad;
	DWORD		dwProcessUsage;			// Process Usage
	DWORD		dwSystemUsage;			// System Usage
	DWORD		dwMemoryUsage;			// Memory Usage
	DWORD		dwPing;					// by itzrnb 08/05/2009
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GM_REPORT_CONNECTED_USER_INFO)
	sDBO_SERVER_CHANNEL_CONNECTED_USER_INFO		connectedUserInfo;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GM_REPORT_MONSTER_STATISTICS_BEGIN)
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GM_REPORT_MONSTER_STATISTICS)
	BYTE										byCount;
	sDBO_MONSTER_STATISTICS						aMonsterStatistics[DBO_MAX_MONSTER_STATISTICS_COUNT_PER_PACKET];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GM_REPORT_MONSTER_STATISTICS_SENDING_PAUSED_NFY)
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GM_REPORT_MONSTER_STATISTICS_END)
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GM_USERCOUNT_RES )
	ACCOUNTID				managerID;		  // GM ID
	DWORD					dwConnectingCurrent;
	DWORD					dwConnectingMax;
	DWORD					dwPlayingCurrent;
	DWORD					dwPlayingMax;
	DWORD					dwPlayerCurrent;
	DWORD					dwPlayerMax;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GM_SERVER_CONTROL_TURN_OFF_ALL_NFY )
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GM_LOGIN_REQ )
	ACCOUNTID				accountId;
	BYTE					abyAuthKey[NTL_MAX_SIZE_AUTH_KEY];	// 인증키
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GM_LOGOUT_REQ )
	ACCOUNTID				accountId;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GM_MOVE_REQ )
	ACCOUNTID				accountId;
	CHARACTERID				charId;
	BYTE					abyAuthKey[NTL_MAX_SIZE_AUTH_KEY];	// 인증키
	BYTE					byState;
	HOBJECT					handle;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GM_KICKOUT_RES )
	ACCOUNTID	accountId;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GM_ON_PLAYER_INFO)
	WORD		wCount;			
	ACCOUNTID	aAccountId[NTL_MAX_COUNT_ON_PLAY_USER];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GM_PING_REQ)
	DWORD		dwTick;
END_PROTOCOL()
//------------------------------------------------------------------

#pragma pack()
