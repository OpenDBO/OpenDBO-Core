#pragma once

#include "NtlPacketCommon.h"


enum eOPCODE_AP
{
	AP_OPCODE_BEGIN = 65100,

	AP_HEARTBEAT = AP_OPCODE_BEGIN,

	AP_NOTIFY_SERVER_BEGIN,
	AP_SERVER_STATUS_RES,

	AP_OPCODE_DUMMY,
	AP_OPCODE_END = AP_OPCODE_DUMMY - 1
};


//------------------------------------------------------------------
//
//------------------------------------------------------------------
const char * NtlGetPacketName_AP(WORD wOpCode);
//------------------------------------------------------------------


#pragma pack(1)

//------------------------------------------------------------------
BEGIN_PROTOCOL(AP_HEARTBEAT)
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(AP_NOTIFY_SERVER_BEGIN)
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( AP_SERVER_STATUS_RES )
	ACCOUNTID				managerID;		  // GM ID
	DWORD					dwSendServerSendTick;	// Ãâ¹ßÁö ½ÃÀÛÆ½		
	DWORD					dwRecServerTick;		// µµÂøÁö µµÂøÆ½	
	DWORD					dwRecServerSendTick;	// µµÂøÁö ½ÃÀÛÆ½
END_PROTOCOL()
//------------------------------------------------------------------

#pragma pack()