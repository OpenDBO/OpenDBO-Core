#pragma once

#include "NtlPacketCommon.h"
#include "NtlSystemTool.h"

enum eOPCODE_PG
{
	PG_OPCODE_BEGIN = 62000,

	PG_HEARTBEAT = PG_OPCODE_BEGIN,

	PG_CHAR_SET_GM_GROUP_NFY,
	PG_CHAR_PUNISH_REQ,

	PG_USERCOUNT_REQ,
	PG_GM_GROUP_AUTH_NFY,
	PG_GM_GROUP_AUTH_DEL_NFY,

	PG_GM_SERVER_COMMAND_REQ,

	PG_SERVER_STATUS_REQ,

	PG_OPCODE_DUMMY,
	PG_OPCODE_END = PG_OPCODE_DUMMY - 1
};


//------------------------------------------------------------------
//
//------------------------------------------------------------------
const char * NtlGetPacketName_PG(WORD wOpCode);
//------------------------------------------------------------------

#pragma pack(1)

//------------------------------------------------------------------
BEGIN_PROTOCOL(PG_HEARTBEAT)
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( PG_CHAR_SET_GM_GROUP_NFY )
	ACCOUNTID				accountID;
	ACCOUNTID				GMAccountID;
	DWORD					dwGroupSerial;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( PG_CHAR_PUNISH_REQ )
	ACCOUNTID				managerID;		      // GM ID
	CHARACTERID				charTargetID;		  // target CharID 	
	ACCOUNTID				AccountTargetID;		  // Target AccountID
	BYTE					byPunishType;	  // Punish Type
	DWORD					dwPunishAmount;  // How Many Day or Minutes
	bool					bDateOption;	  // 0: Day 1: Minutes 
	bool					bIsOn;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( PG_USERCOUNT_REQ )
	ACCOUNTID				managerID;		      // GM ID
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( PG_GM_GROUP_AUTH_NFY )			// 게임서버가 켜지거나 그룹권한이 변경 되었을때
	DWORD					dwGroupCount;
	sGM_GROUP_ENTITY		aGroupList[NTL_MAX_COUNT_GM_GROUP];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( PG_GM_GROUP_AUTH_DEL_NFY )			// 그룹권한이 삭제 되었을때
	DWORD					dwGroupSerial;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( PG_GM_SERVER_COMMAND_REQ )
	ACCOUNTID				managerID;		      // GM ID
	WORD					wMessageLengthInUnicode;
	WCHAR					awchCommand[NTL_MAX_LENGTH_OF_CHAT_MESSAGE_UNICODE + 1];
	WCHAR					awchUserID[NTL_MAX_SIZE_USERID_UNICODE + 1];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( PG_SERVER_STATUS_REQ )
	ACCOUNTID				managerID;		      // GM ID
	DWORD					dwSendServerSendTick;	// 출발지 시작틱	
END_PROTOCOL()
//------------------------------------------------------------------

#pragma pack()