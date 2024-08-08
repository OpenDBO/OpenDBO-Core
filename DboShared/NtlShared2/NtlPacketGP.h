#pragma once

#include "NtlPacketCommon.h"

enum eOPCODE_GP
{
	GP_OPCODE_BEGIN = 62500,

	GP_HEARTBEAT = GP_OPCODE_BEGIN,

	GP_NOTIFY_SERVER_BEGIN,

	GP_CHAR_PUNISH_RES,
	GP_USERCOUNT_RES,
	GP_GM_SERVER_COMMAND_RES,
	GP_SERVER_STATUS_RES,

	GP_OPCODE_DUMMY,
	GP_OPCODE_END = GP_OPCODE_DUMMY - 1
};


//------------------------------------------------------------------
//
//------------------------------------------------------------------
const char * NtlGetPacketName_GP(WORD wOpCode);
//------------------------------------------------------------------


#pragma pack(1)

//------------------------------------------------------------------
BEGIN_PROTOCOL(GP_HEARTBEAT)
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GP_NOTIFY_SERVER_BEGIN)
	DWORD		dwMaxLoad;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GP_CHAR_PUNISH_RES )
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
BEGIN_PROTOCOL( GP_USERCOUNT_RES )
	ACCOUNTID				managerID;		  // GM ID
	DWORD					dwConnectingCurrent;
	DWORD					dwConnectingMax;
	DWORD					dwPlayingCurrent;
	DWORD					dwPlayingMax;
	DWORD					dwPlayerCurrent;
	DWORD					dwPlayerMax;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GP_GM_SERVER_COMMAND_RES )
	ACCOUNTID				managerID;		      // GM ID
	WORD					wMessageLengthInUnicode;
	WCHAR					awchMessage[NTL_MAX_LENGTH_OF_CHAT_MESSAGE_UNICODE + 1];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GP_SERVER_STATUS_RES )
	ACCOUNTID				managerID;		  // GM ID
	DWORD					dwSendServerSendTick;	// Ãâ¹ßÁö ½ÃÀÛÆ½		
	DWORD					dwRecServerTick;		// µµÂøÁö µµÂøÆ½	
	DWORD					dwRecServerSendTick;	// µµÂøÁö ½ÃÀÛÆ½
END_PROTOCOL()
//------------------------------------------------------------------

#pragma pack()