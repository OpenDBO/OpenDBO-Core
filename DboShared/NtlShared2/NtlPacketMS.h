#pragma once

#include "NtlPacketCommon.h"
#include "NtlCharacter.h"
#include "NtlSystemTool.h"
#include "NtlAdmin.h"


enum eOPCODE_MS
{
	MS_OPCODE_BEGIN = 24000,

	MS_HEARTBEAT = MS_OPCODE_BEGIN,

	MS_LOGIN_RES,					// 로그인 요청 응답
	MS_CHANGE_PASSWORD_RES,			// 패스워드 변경 응답

	MS_SERVER_STATUS_RES,			// 서버 상태 응답
	MS_SERVER_CONTROL_TURN_ON_RES,	// 서버 시작 요청 응답
	MS_SERVER_CONTROL_TURN_OFF_RES,	// 서버 종료 요청 응답

	MS_OPCODE_DUMMY,
	MS_OPCODE_END = MS_OPCODE_DUMMY - 1
};


//------------------------------------------------------------------
//
//------------------------------------------------------------------
const char * NtlGetPacketName_MS(WORD wOpCode);
//------------------------------------------------------------------


#pragma pack(1)

//------------------------------------------------------------------
BEGIN_PROTOCOL(MS_HEARTBEAT)
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MS_LOGIN_RES)
	WORD					wResultCode;								// 결과

	ACCOUNTID				gmAccountID;								// GM Account ID
	WCHAR					wszUser[NTL_MAX_SIZE_USERID_UNICODE + 1];	// ID
	DWORD					dwGroupSerial;								// Group Serial
	ACCOUNTID				accountID;									// Account ID(게임에서 사용)
	WORD					wKaioshinVersion;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MS_CHANGE_PASSWORD_RES)
	WORD					wResultCode;								// 결과

	WCHAR					wszUserID[NTL_MAX_SIZE_USERID_UNICODE + 1];	// 패스워드를 변경한 ID
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MS_SERVER_STATUS_RES)
	WORD					wResultCode;

	WORD					wCount;
	sSERVERSTATUS_ENTITY	aServerStatusList[1];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MS_SERVER_CONTROL_TURN_ON_RES)
	WORD					wResultCode;

	BYTE				byServerType;
	SERVERFARMID			serverFarmId;
	BYTE					byServerChannelIndex;
	BYTE					byServerIndex;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MS_SERVER_CONTROL_TURN_OFF_RES)
	WORD					wResultCode;

	BYTE				byServerType;
	SERVERFARMID			serverFarmId;
	BYTE					byServerChannelIndex;
	BYTE					byServerIndex;
END_PROTOCOL()
//------------------------------------------------------------------
#pragma pack()