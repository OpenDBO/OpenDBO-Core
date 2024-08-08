#pragma once

#include "NtlPacketCommon.h"
#include "NtlCSArchitecture.h"


enum eOPCODE_MR
{
	MR_OPCODE_BEGIN = 58000,

	MR_HEARTBEAT = MR_OPCODE_BEGIN,
	MR_GAME_SERVER_FARM_INFO,
	MR_GAME_SERVER_CHANNEL_INFO,
	MR_SERVERS_INFO,
	MR_SERVERS_INFO_END,
	MR_NOTIFY_ILLEGAL_SERVER,

	MR_SERVER_INFO_CHANGED_NFY,
	MR_GAME_SERVER_FARM_INFO_CHANGED_NFY,
	MR_GAME_SERVER_CHANNEL_INFO_CHANGED_NFY,
	MR_LOG_SERVER_NFY,
	MR_SERVER_VERSION_CHANGED_NFY, // by sooshia 07/23/2008
	MR_SERVER_TURNOFF_FOR_DEVEL_NFY,

	MR_PING_RES, // by itzrnb 08/05/2009

	MR_OPCODE_DUMMY,
	MR_OPCODE_END = MR_OPCODE_DUMMY - 1
};


//------------------------------------------------------------------
//
//------------------------------------------------------------------
const char * NtlGetPacketName_MR(WORD wOpCode);
//------------------------------------------------------------------


#pragma pack(1)

//------------------------------------------------------------------
BEGIN_PROTOCOL(MR_HEARTBEAT)
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MR_GAME_SERVER_FARM_INFO)
sDBO_GAME_SERVER_FARM_INFO			serverFarmInfo;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MR_GAME_SERVER_CHANNEL_INFO)
sDBO_GAME_SERVER_CHANNEL_INFO		serverChannelInfo;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MR_SERVERS_INFO)
sDBO_SERVER_INFO		serverInfo;
BYTE					byIsMyInfo;		// 0x00 : false, 0x01 : true
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MR_SERVERS_INFO_END)
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MR_NOTIFY_ILLEGAL_SERVER)
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MR_SERVER_INFO_CHANGED_NFY)
BYTE				byServerType;
SERVERFARMID		serverFarmId;
SERVERCHANNELID		serverChannelId;
SERVERINDEX			serverIndex;

bool				bIsOn;
DWORD				dwLoad;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MR_GAME_SERVER_FARM_INFO_CHANGED_NFY)
SERVERFARMID		serverFarmId;

BYTE				byServerStatus;		// eDBO_SERVER_STATUS
DWORD				dwMaxLoad;
DWORD				dwLoad;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MR_GAME_SERVER_CHANNEL_INFO_CHANGED_NFY)
SERVERFARMID		serverFarmId;
SERVERCHANNELID		serverChannelId;

BYTE				byServerStatus;		// eDBO_SERVER_STATUS
DWORD				dwMaxLoad;
DWORD				dwLoad;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( MR_LOG_SERVER_NFY )
BYTE				byServerType;
SERVERFARMID		serverFarmId;
BYTE				byServerChannelIndex;
BYTE				byServerIndex;
DWORD				dwReportType;
DWORD				dwParameter[15];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MR_SERVER_VERSION_CHANGED_NFY) // by sooshia 07/23/2008
WORD				wLVersion;
WORD				wRVersion;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MR_SERVER_TURNOFF_FOR_DEVEL_NFY) // by sooshia 07/23/2008
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MR_PING_RES)
DWORD				dwTick;
END_PROTOCOL()
//------------------------------------------------------------------

#pragma pack()