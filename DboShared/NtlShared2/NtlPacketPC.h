#pragma once

#include "NtlPacketCommon.h"
#include "NtlSystemTool.h"


enum eOPCODE_PC
{
	PC_OPCODE_BEGIN = 63000,

	PC_HEARTBEAT = PC_OPCODE_BEGIN,

	PC_GM_GROUP_AUTH_NFY,
	PC_GM_GROUP_AUTH_DEL_NFY,
	PC_CHAR_PUNISH_REQ,

	PC_SERVER_STATUS_REQ,

	PC_OPCODE_DUMMY,
	PC_OPCODE_END = PC_OPCODE_DUMMY - 1
};


//------------------------------------------------------------------
//
//------------------------------------------------------------------
const char * NtlGetPacketName_PC(WORD wOpCode);
//------------------------------------------------------------------


#pragma pack(1)

//------------------------------------------------------------------
BEGIN_PROTOCOL(PC_HEARTBEAT)
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( PC_GM_GROUP_AUTH_NFY )				// 서버가 켜지거나 그룹권한이 변경 되었을때
	DWORD					dwGroupCount;
	sGM_GROUP_ENTITY		aGroupList[NTL_MAX_COUNT_GM_GROUP];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( PC_GM_GROUP_AUTH_DEL_NFY )			// 그룹권한이 삭제 되었을때
	DWORD					dwGroupSerial;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( PC_CHAR_PUNISH_REQ )
	ACCOUNTID				managerID;		      // GM ID
	CHARACTERID				charTargetID;		  // target CharID 	
	ACCOUNTID				AccountTargetID;		  // Target AccountID
	BYTE					byPunishType;	  // Punish Type
	DWORD					dwPunishAmount;  // How Many Day or Minutes
	bool					bDateOption;	  // 0: Day 1: Minutes 
	bool					bIsOn;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( PC_SERVER_STATUS_REQ )
	ACCOUNTID				managerID;		      // GM ID
	DWORD					dwSendServerSendTick;	// 출발지 시작틱	
	DWORD					dwRecServerTick;		// 도착지 도착틱
END_PROTOCOL()
//------------------------------------------------------------------
#pragma pack()