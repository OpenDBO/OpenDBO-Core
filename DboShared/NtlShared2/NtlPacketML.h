#pragma once

#include "NtlPacketCommon.h"
#include "NtlCSArchitecture.h"


enum eOPCODE_ML
{
	ML_OPCODE_BEGIN = 20000,

	ML_HEARTBEAT = ML_OPCODE_BEGIN,
	ML_SERVERS_INFO,
	ML_SERVERS_INFO_END,
	ML_NOTIFY_ILLEGAL_SERVER,

	ML_SERVER_INFO_CHANGED_NFY,
	ML_SERVER_VERSION_CHANGED_NFY, // by sooshia 07/23/2008
	ML_SERVER_TURNOFF_FOR_DEVEL_NFY,

	ML_PING_RES, // by itzrnb 08/05/2009

	ML_OPCODE_DUMMY,
	ML_OPCODE_END = ML_OPCODE_DUMMY - 1
};


//------------------------------------------------------------------
//
//------------------------------------------------------------------
const char * NtlGetPacketName_ML(WORD wOpCode);
//------------------------------------------------------------------

#pragma pack(1)

//------------------------------------------------------------------
BEGIN_PROTOCOL(ML_HEARTBEAT)
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(ML_SERVERS_INFO)
sDBO_SERVER_INFO		serverInfo;
BYTE					byIsMyInfo;		// 0x00 : false, 0x01 : true
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(ML_SERVERS_INFO_END)
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(ML_NOTIFY_ILLEGAL_SERVER)
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(ML_SERVER_INFO_CHANGED_NFY)
BYTE				byServerType;
SERVERFARMID		serverFarmId;
SERVERCHANNELID		serverChannelId;
SERVERINDEX			serverIndex;

bool				bIsOn;
DWORD				dwLoad;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(ML_SERVER_VERSION_CHANGED_NFY) // by sooshia 07/23/2008
WORD				wLVersion;
WORD				wRVersion;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(ML_SERVER_TURNOFF_FOR_DEVEL_NFY) // by sooshia 07/23/2008
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(ML_PING_RES)
DWORD				dwTick;
END_PROTOCOL()
//------------------------------------------------------------------
#pragma pack()