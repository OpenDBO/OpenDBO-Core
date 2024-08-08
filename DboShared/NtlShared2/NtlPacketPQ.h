#pragma once

#include "NtlPacketCommon.h"
#include "NtlCSArchitecture.h"


enum eOPCODE_PQ
{
	PQ_OPCODE_BEGIN = 16000,

	PQ_HEARTBEAT = PQ_OPCODE_BEGIN,
	PQ_GAME_SERVER_FARM_INFO,
	PQ_GAME_SERVER_CHANNEL_INFO,
	PQ_SERVERS_INFO,
	PQ_NOTIFY_ILLEGAL_SERVER,

	PQ_OPCODE_DUMMY,
	PQ_OPCODE_END = PQ_OPCODE_DUMMY - 1
};

//------------------------------------------------------------------
//
//------------------------------------------------------------------
const char * NtlGetPacketName_PQ(WORD wOpCode);
//------------------------------------------------------------------

#pragma pack(1)

//------------------------------------------------------------------
BEGIN_PROTOCOL(PQ_HEARTBEAT)
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(PQ_GAME_SERVER_FARM_INFO)
	sDBO_GAME_SERVER_FARM_INFO			gameServerFarmInfo;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(PQ_GAME_SERVER_CHANNEL_INFO)
	sDBO_GAME_SERVER_CHANNEL_INFO		gameServerChannelInfo;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(PQ_SERVERS_INFO)
	sDBO_SERVER_INFO		serverInfo;
	BYTE					byIsMyInfo;		// 0x00 : false, 0x01 : true
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(PQ_NOTIFY_ILLEGAL_SERVER)
END_PROTOCOL()
//------------------------------------------------------------------

#pragma pack()