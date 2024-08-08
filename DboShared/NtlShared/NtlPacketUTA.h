#pragma once
#include "..\NtlNetworkLayerTypeDef.h"
#include "NtlPacketInternal.h"

#define OpUTA_Nop								(0x01000000)
#define OpUTA_LoginRequest						(0x01000001)
#define OpUTA_LogOutRequest                     (0x01000002)
#define OpUTA_CreateUserRequest					(0x01000003)

extern SNtlPacketName* g_pNtlPacketName_UTA[];

#pragma pack(1)

struct SUTA_Nop
{
	SPacketHeader	header;
};

struct SUTA_LoginRequest
{
	SPacketHeader	header;
	WChar			wchUserID[NTL_MAX_LENGTH_OF_ID_AND_PASS_UNICODE];
	WChar			wchPasswd[NTL_MAX_LENGTH_OF_ID_AND_PASS_UNICODE];
	UInt16			wCodePage;
	UInt16			wVersion;
};

struct SUTA_LogOutRequest
{
	SPacketHeader	header;
	WChar			wchUserID[NTL_MAX_LENGTH_OF_ID_AND_PASS_UNICODE];
	UInt16			wCodePage;
};

struct SUTA_CreateUserRequest
{
	SPacketHeader	header;
	WChar			wchUserID[NTL_MAX_LENGTH_OF_ID_AND_PASS_UNICODE];
	WChar			wchPasswd[NTL_MAX_LENGTH_OF_ID_AND_PASS_UNICODE];
	UInt16			wCodePage;
};

#pragma pack()