#pragma once

#include "NtlPacketCommon.h"

enum eOPCODE_NM
{
	NM_OPCODE_BEGIN = 15000,

	NM_NOTIFY_SERVER_BEGIN = NM_OPCODE_BEGIN,
	NM_HEARTBEAT,
	NM_SERVER_CONTROL_TURN_OFF_ALL_NFY,
	NM_REPORT_LOAD,
	
	NM_PING_REQ, // by itzrnb 08/05/2009

	NM_OPCODE_DUMMY,
	NM_OPCODE_END = NM_OPCODE_DUMMY - 1
};


//------------------------------------------------------------------
//
//------------------------------------------------------------------
const char * NtlGetPacketName_NM(WORD wOpCode);
//------------------------------------------------------------------


#pragma pack(1)

//------------------------------------------------------------------
BEGIN_PROTOCOL(NM_NOTIFY_SERVER_BEGIN)
DWORD		dwMaxLoad;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(NM_HEARTBEAT)
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(NM_SERVER_CONTROL_TURN_OFF_ALL_NFY)
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(NM_REPORT_LOAD)
DWORD		dwProcessUsage;			// Process Usage
DWORD		dwSystemUsage;			// System Usage
DWORD		dwMemoryUsage;			// Memory Usage
DWORD		dwPing;					// by itzrnb 08/05/2009
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(NM_PING_REQ)
DWORD		dwTick;
END_PROTOCOL()
//------------------------------------------------------------------

#pragma pack()