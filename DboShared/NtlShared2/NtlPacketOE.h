#pragma once

#include "NtlPacketCommon.h"


enum eOPCODE_OE
{
	OE_OPCODE_BEGIN = 55000,

	OE_NOTIFY_AGENT_CONTROLLER_BEGIN = OE_OPCODE_BEGIN,

	OE_SERVER_CONTROL_TURN_ON_REQ,
	OE_SERVER_CONTROL_TURN_OFF_REQ,

	OE_OPCODE_DUMMY,
	OE_OPCODE_END = OE_OPCODE_DUMMY - 1
};

//------------------------------------------------------------------
//
//------------------------------------------------------------------
const char * NtlGetPacketName_OE(WORD wOpCode);
//------------------------------------------------------------------

#pragma pack(1)

//------------------------------------------------------------------
BEGIN_PROTOCOL(OE_NOTIFY_AGENT_CONTROLLER_BEGIN)
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(OE_SERVER_CONTROL_TURN_ON_REQ)
	BYTE				byServerType;
	SERVERFARMID		serverFarmId;
	BYTE				byServerChannelIndex;
	BYTE				byServerIndex;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(OE_SERVER_CONTROL_TURN_OFF_REQ)
	BYTE				byServerType;
	SERVERFARMID		serverFarmId;
	BYTE				byServerChannelIndex;
	BYTE				byServerIndex;
END_PROTOCOL()
//------------------------------------------------------------------

#pragma pack()