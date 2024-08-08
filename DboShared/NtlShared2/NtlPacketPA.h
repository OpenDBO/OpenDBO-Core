#pragma once

#include "NtlPacketCommon.h"
#include "NtlSystemTool.h"


enum eOPCODE_PA
{
	PA_OPCODE_BEGIN = 63000,

	PA_HEARTBEAT = PA_OPCODE_BEGIN,

	PA_GM_GROUP_AUTH_NFY,
	PA_GM_GROUP_AUTH_DEL_NFY,
	PA_SERVER_STATUS_REQ,

	PA_OPCODE_DUMMY,
	PA_OPCODE_END = PA_OPCODE_DUMMY - 1
};


//------------------------------------------------------------------
//
//------------------------------------------------------------------
const char * NtlGetPacketName_PA(WORD wOpCode);
//------------------------------------------------------------------


#pragma pack(1)

//------------------------------------------------------------------
BEGIN_PROTOCOL(PA_HEARTBEAT)
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( PA_GM_GROUP_AUTH_NFY )				// 서버가 켜지거나 그룹권한이 변경 되었을때
	DWORD					dwGroupCount;
	sGM_GROUP_ENTITY		aGroupList[NTL_MAX_COUNT_GM_GROUP];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( PA_GM_GROUP_AUTH_DEL_NFY )			// 그룹권한이 삭제 되었을때
	DWORD					dwGroupSerial;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( PA_SERVER_STATUS_REQ )
	ACCOUNTID				managerID;		      // GM ID
	DWORD					dwSendServerSendTick;	// 출발지 시작틱	
	DWORD					dwRecServerTick;		// 도착지 도착틱
END_PROTOCOL()
//------------------------------------------------------------------

#pragma pack()