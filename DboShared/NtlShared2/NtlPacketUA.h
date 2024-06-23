#pragma once

#include "NtlPacketCommon.h"
#include "NtlCSArchitecture.h"

enum eOPCODE_UA
{
	UA_OPCODE_BEGIN = 100,

	UA_LOGIN_REQ,
	UA_LOGIN_KR_REQ,
	UA_LOGIN_TW_REQ,
	UA_LOGIN_HK_REQ,
	UA_LOGIN_CREATEUSER_REQ,
	UA_LOGIN_DISCONNECT_REQ,

	UA_OPCODE_DUMMY,
	UA_OPCODE_END = UA_OPCODE_DUMMY - 1
};


//------------------------------------------------------------------
//
//------------------------------------------------------------------
const char * NtlGetPacketName_UA(WORD wOpCode);
//------------------------------------------------------------------

#pragma pack(1)

//------------------------------------------------------------------
BEGIN_PROTOCOL(UA_LOGIN_REQ)
WCHAR		awchUserId[NTL_MAX_SIZE_USERID + 1];
char		achPasswd[NTL_MAX_SIZE_USERPW_ENCRYPT + 1];
WORD		wLVersion;
WORD		wRVersion;
BYTE		abyMacAddress[DBO_MAX_ADAPTER_ADDRESS_LENGTH];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(UA_LOGIN_TW_REQ)
WCHAR		awcUserID[NTL_MAX_SIZE_USERID + 1];
WCHAR		awcPasswd[NTL_MAX_SIZE_USERPW + 1];
DWORD		dwCodePage;
WORD		wLVersion;
WORD		wRVersion;
BYTE		abyMacAddress[DBO_MAX_ADAPTER_ADDRESS_LENGTH];
BYTE		byState;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(UA_LOGIN_DISCONNECT_REQ)
WCHAR		awchUserId[NTL_MAX_SIZE_USERID + 1];
bool		bIsEnteringCharacterServer;
END_PROTOCOL()
//------------------------------------------------------------------
#pragma pack()