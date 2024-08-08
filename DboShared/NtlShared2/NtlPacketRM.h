#pragma once

#include "NtlPacketCommon.h"

enum eOPCODE_RM
{
	RM_OPCODE_BEGIN = 58500,

	RM_NOTIFY_SERVER_BEGIN = RM_OPCODE_BEGIN,
	RM_HEARTBEAT,
	RM_SERVER_CONTROL_TURN_OFF_ALL_NFY,
	RM_REPORT_LOAD,
	
	RM_PING_REQ, // by itzrnb 08/05/2009

	RM_OPCODE_DUMMY,
	RM_OPCODE_END = RM_OPCODE_DUMMY - 1
};

//------------------------------------------------------------------
//
//------------------------------------------------------------------
const char * NtlGetPacketName_RM(WORD wOpCode);
//------------------------------------------------------------------


#pragma pack(1)

//------------------------------------------------------------------
BEGIN_PROTOCOL(RM_NOTIFY_SERVER_BEGIN)
DWORD		dwMaxLoad;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(RM_HEARTBEAT)
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(RM_SERVER_CONTROL_TURN_OFF_ALL_NFY)
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(RM_REPORT_LOAD)
DWORD		dwProcessUsage;			// Process Usage
DWORD		dwSystemUsage;			// System Usage
DWORD		dwMemoryUsage;			// Memory Usage
DWORD		dwRecvQuery;			// During Queue Recieved Query
DWORD		dwEndQuery;				// During Queue Ended Query
DWORD		dwCurQuery;				// Current time Queue Query Count
DWORD		dwPing;					// by itzrnb 08/05/2009
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(RM_PING_REQ)
DWORD		dwTick;
END_PROTOCOL()
//------------------------------------------------------------------
#pragma pack()