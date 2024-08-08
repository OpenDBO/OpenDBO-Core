#pragma once

#include "NtlPacketCommon.h"


enum eOPCODE_LM
{
	LM_OPCODE_BEGIN = 21000,

	LM_NOTIFY_SERVER_BEGIN = LM_OPCODE_BEGIN,
	LM_HEARTBEAT,
	LM_SERVER_CONTROL_TURN_OFF_ALL_NFY,
	LM_REPORT_LOAD,
	
	LM_PING_REQ, // by itzrnb 08/05/2009

	LM_OPCODE_DUMMY,
	LM_OPCODE_END = LM_OPCODE_DUMMY - 1
};


//------------------------------------------------------------------
//
//------------------------------------------------------------------
const char * NtlGetPacketName_LM(WORD wOpCode);
//------------------------------------------------------------------


#pragma pack(1)

//------------------------------------------------------------------
BEGIN_PROTOCOL(LM_NOTIFY_SERVER_BEGIN)
DWORD		dwMaxLoad;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(LM_HEARTBEAT)
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(LM_SERVER_CONTROL_TURN_OFF_ALL_NFY)
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(LM_REPORT_LOAD)
DWORD		dwProcessUsage;			// Process Usage
DWORD		dwSystemUsage;			// System Usage
DWORD		dwMemoryUsage;			// Memory Usage
DWORD		dwPing;					// by itzrnb 08/05/2009
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(LM_PING_REQ)
DWORD		dwTick;
END_PROTOCOL()
//------------------------------------------------------------------

#pragma pack()