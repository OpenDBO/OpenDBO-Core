#pragma once

#include "NtlPacketCommon.h"
#include "NtlCSArchitecture.h"


enum eOPCODE_MN
{
	MN_OPCODE_BEGIN = 14000,

	MN_HEARTBEAT = MN_OPCODE_BEGIN,
	MN_GAME_SERVER_FARM_INFO,
	MN_GAME_SERVER_CHANNEL_INFO,
	MN_SERVERS_INFO,
	MN_SERVERS_INFO_END,
	MN_NOTIFY_ILLEGAL_SERVER,

	MN_SERVER_INFO_CHANGED_NFY,
	MN_GAME_SERVER_FARM_INFO_CHANGED_NFY,
	MN_GAME_SERVER_CHANNEL_INFO_CHANGED_NFY,
	MN_SERVER_TURNOFF_FOR_DEVEL_NFY,

	MN_PING_RES, // by itzrnb 08/05/2009

	MN_OPCODE_DUMMY,
	MN_OPCODE_END = MN_OPCODE_DUMMY - 1
};


//------------------------------------------------------------------
//
//------------------------------------------------------------------
const char * NtlGetPacketName_MN(WORD wOpCode);
//------------------------------------------------------------------


#pragma pack(1)

//------------------------------------------------------------------
BEGIN_PROTOCOL(MN_HEARTBEAT)
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MN_GAME_SERVER_FARM_INFO)
sDBO_GAME_SERVER_FARM_INFO			gameServerFarmInfo;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MN_GAME_SERVER_CHANNEL_INFO)
sDBO_GAME_SERVER_CHANNEL_INFO		gameServerChannelInfo;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MN_SERVERS_INFO)
sDBO_SERVER_INFO		serverInfo;
BYTE					byIsMyInfo;		// 0x00 : false, 0x01 : true
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MN_SERVERS_INFO_END)
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MN_NOTIFY_ILLEGAL_SERVER)
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MN_SERVER_INFO_CHANGED_NFY)
BYTE				byServerType;
SERVERFARMID		serverFarmId;
SERVERCHANNELID		serverChannelId;
SERVERINDEX			serverIndex;

bool				bIsOn;
DWORD				dwLoad;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MN_GAME_SERVER_FARM_INFO_CHANGED_NFY)
BYTE				byServerStatus;		// eDBO_SERVER_STATUS
DWORD				dwMaxLoad;
DWORD				dwLoad;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MN_GAME_SERVER_CHANNEL_INFO_CHANGED_NFY)
BYTE				byServerStatus;		// eDBO_SERVER_STATUS
DWORD				dwMaxLoad;
DWORD				dwLoad;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MN_SERVER_TURNOFF_FOR_DEVEL_NFY)
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MN_PING_RES)
DWORD				dwTick;
END_PROTOCOL()
//------------------------------------------------------------------

#pragma pack()