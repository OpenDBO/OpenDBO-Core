#pragma once

#include "NtlPacketCommon.h"
#include "NtlCharacter.h"
#include "NtlSystemTool.h"
#include "NtlAdmin.h"

enum eOPCODE_QP
{
	QP_OPCODE_BEGIN = 17000,

	QP_NOTIFY_SERVER_BEGIN = QP_OPCODE_BEGIN,
	QP_HEARTBEAT,

	QP_CHAR_PUNISH_RES,						// PUNISH CHARACTER

	QP_OPCODE_DUMMY,
	QP_OPCODE_END = QP_OPCODE_DUMMY - 1
};


//------------------------------------------------------------------
//
//------------------------------------------------------------------
const char * NtlGetPacketName_QP(WORD wOpCode);
//------------------------------------------------------------------

#pragma pack(1)

//------------------------------------------------------------------
BEGIN_PROTOCOL(QP_NOTIFY_SERVER_BEGIN)
	DWORD					dwMaxLoad;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QP_HEARTBEAT)
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( QP_CHAR_PUNISH_RES )
	WORD					wResultCode;
	ACCOUNTID				managerID;			// SYSTEM Tool 에 접속한 유저의 시리얼
	BYTE					byQueryType;			  // 

	CHARACTERID				charTargetID;								// target CharID 	
	ACCOUNTID				AccountTargetID;							// Target AccountID
	WCHAR					wchTargetCharName[NTL_MAX_SIZE_CHAR_NAME+1];  // Target Char Name
	WCHAR				    wchTargetUserName[NTL_MAX_SIZE_USERID+1];		// Target User Name

	BYTE					byPunishType;		      // Punish Type
	DWORD					dwPunishAmount;          // How Many Day or Minutes
	WCHAR				    wchGMCharName[NTL_MAX_SIZE_CHAR_NAME+1];	      // GM Char Name
	bool					bDateOption;	          // 0: Day 1: Minutes 
	bool					bIsOn;
END_PROTOCOL()
//------------------------------------------------------------------
#pragma pack()