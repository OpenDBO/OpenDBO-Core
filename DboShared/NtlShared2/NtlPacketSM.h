#pragma once

#include "NtlPacketCommon.h"
#include "NtlAdmin.h"
#include "NtlSystemTool.h"
#include "NtlItem.h"


enum eOPCODE_SM
{
	SM_OPCODE_BEGIN = 21000,

	SM_NOTIFY_SERVER_BEGIN = SM_OPCODE_BEGIN,
	SM_HEARTBEAT,

	SM_LOGIN_REQ,						// 로그인 요청
	SM_CHANGE_PASSWORD_REQ,				// 패스워드 변경 요청

	SM_SERVER_STATUS_REQ,				// 서버 상태 요청

	SM_SERVER_CONTROL_TURN_ON_REQ,		// 서버 프로세스 실행 요청
	SM_SERVER_CONTROL_TURN_OFF_REQ,		// 서버 프로세스 종료 요청

	SM_OPCODE_DUMMY,
	SM_OPCODE_END = SM_OPCODE_DUMMY - 1
};


//------------------------------------------------------------------
//
//------------------------------------------------------------------
const char * NtlGetPacketName_SM(WORD wOpCode);
//------------------------------------------------------------------


#pragma pack(1)

//------------------------------------------------------------------
BEGIN_PROTOCOL(SM_NOTIFY_SERVER_BEGIN)
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(SM_HEARTBEAT)
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(SM_LOGIN_REQ)
	WCHAR			wszUserID[NTL_MAX_SIZE_USERID_UNICODE + 1];
	WCHAR			wszUserPWD[NTL_MAX_SIZE_USERPW_UNICODE + 1];
	WORD			wKaioshinVersion;
	
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(SM_CHANGE_PASSWORD_REQ)
	WCHAR			wszUserID[NTL_MAX_SIZE_USERID_UNICODE + 1];		// 변경할 ID
	WCHAR			wszCurPWD[NTL_MAX_SIZE_USERPW_UNICODE + 1];		// 현재 패스워드
	WCHAR			wszNewPWD[NTL_MAX_SIZE_USERPW_UNICODE + 1];		// 변경할 패스워드
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(SM_SERVER_STATUS_REQ)
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(SM_SERVER_CONTROL_TURN_ON_REQ)
	BYTE				byServerType;
	SERVERFARMID		serverFarmId;
	BYTE				byServerChannelIndex;
	BYTE				byServerIndex;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(SM_SERVER_CONTROL_TURN_OFF_REQ)
	BYTE				byServerType;
	SERVERFARMID		serverFarmId;
	BYTE				byServerChannelIndex;
	BYTE				byServerIndex;
END_PROTOCOL()
//------------------------------------------------------------------
#pragma pack()