#pragma once

#include "NtlPacketCommon.h"


enum eOPCODE_CP
{
	CP_OPCODE_BEGIN = 63500,

	CP_HEARTBEAT = CP_OPCODE_BEGIN,

	CP_NOTIFY_SERVER_BEGIN,
	CP_CHAR_PUNISH_RES,

	CP_SERVER_STATUS_RES,

	CP_OPCODE_DUMMY,
	CP_OPCODE_END = CP_OPCODE_DUMMY - 1
};


//------------------------------------------------------------------
//
//------------------------------------------------------------------
const char * NtlGetPacketName_CP(WORD wOpCode);
//------------------------------------------------------------------


#pragma pack(1)

//------------------------------------------------------------------
BEGIN_PROTOCOL(CP_HEARTBEAT)
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(CP_NOTIFY_SERVER_BEGIN)
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(CP_CHAR_PUNISH_RES)
	WORD					wResultCode;
	ACCOUNTID				managerID;				  // GM ID
	CHARACTERID				charTargetID;			  // target CharID
	ACCOUNTID				AccountTargetID;		  // Target AccountID
	BYTE					byPunishType;		      // Punish Type
	DWORD					dwPunishAmount;          // How Many Day or Minutes
	bool					bDateOption;	          // 0: Day 1: Minutes
	bool					bIsOn;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( CP_SERVER_STATUS_RES )
	ACCOUNTID				managerID;		  // GM ID
	DWORD					dwSendServerSendTick;	// Ãâ¹ßÁö ½ÃÀÛÆ½		
	DWORD					dwRecServerTick;		// µµÂøÁö µµÂøÆ½	
	DWORD					dwRecServerSendTick;	// µµÂøÁö ½ÃÀÛÆ½
END_PROTOCOL()
//------------------------------------------------------------------
#pragma pack()