#pragma once
#include "..\NtlNetworkLayerTypeDef.h"
#include "NtlPacketInternal.h"

#define OpChTC_Nop								(0x73000000)
#define OpChTC_Heartbeat						(0x73000001)
#define OpChTC_NotifyConnection					(0x73000002)
#define OpChTC_CharacterInsert					(0x73000003)

extern SNtlPacketName* g_pNtlPacketName_ChTC[];

#pragma pack(1)

struct SChTC_Nop
{
	SPacketHeader	header;
};

struct SChTC_Heartbeat
{
	SPacketHeader	header;
};

struct SChTC_NotifyConnection
{
	SPacketHeader	header;
};

struct SChTC_CharacterInsert
{
	SPacketHeader	header;
	Byte            byUUID[NTL_MAX_LENGTH_OF_UUID];
	UInt32			uiAccountID;
	UInt32			uiCharID;
	UInt32			uiZoneID;
	WChar			wchName[NTL_MAX_LENGTH_OF_CHARACTER_NAME_UNICODE];
};
#pragma pack()