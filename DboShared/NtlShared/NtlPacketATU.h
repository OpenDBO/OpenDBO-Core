#pragma once
#include "..\NtlNetworkLayerTypeDef.h"
#include "NtlPacketInternal.h"

#define OpATU_Nop								(0x10000000)
#define OpATU_LoginResult						(0x10000001)
#define OpATU_CreateUserResult	                (0x10000002)				

extern SNtlPacketName* g_pNtlPacketName_ATU[];

#pragma pack(1)

struct SATU_Nop
{
	SPacketHeader	header;
};

struct SATU_LoginResult
{
	SPacketHeader	header;
	WChar			wchUserID[NTL_MAX_LENGTH_OF_ID_AND_PASS_UNICODE];
	WORD            wResult;
	Byte            byUUID[NTL_MAX_LENGTH_OF_UUID];
	char			szCharacterServerIP[NTL_MAX_LENGTH_OF_IP_AND_PORT + 1];
	UInt32			uiCharacterServerPortForCient;
};

struct SATU_CreateUserResult
{
	SPacketHeader	header;
	WChar			wchUserID[NTL_MAX_LENGTH_OF_ID_AND_PASS_UNICODE];
	WORD            wResult;
};


#pragma pack()