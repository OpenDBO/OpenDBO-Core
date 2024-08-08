#pragma once
#include "..\NtlNetworkLayerTypeDef.h"
#include "NtlPacketInternal.h"

#define OpChTG_Nop								(0x72000000)
#define OpChTG_CharacterInsert					(0x72000001)
#define OpChTG_Heartbeat						(0x72000002)
#define OpChTG_NotifyConnection					(0x72000003)

extern SNtlPacketName* g_pNtlPacketName_ChTG[];

#pragma pack(1)

struct SChTG_Nop
{
	SPacketHeader	header;
};

struct SChTG_CharacterInsert
{
	SPacketHeader	header;
	Byte            byUUID[NTL_MAX_LENGTH_OF_UUID];
	UInt32			uiAccountID;
	UInt32			uiCharID;
};

struct SChTG_Heartbeat
{
	SPacketHeader	header;
};

struct SChTG_NotifyConnection
{
	SPacketHeader	header;
};

#pragma pack()