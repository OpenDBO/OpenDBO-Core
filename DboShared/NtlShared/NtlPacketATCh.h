#pragma once
#include "..\NtlNetworkLayerTypeDef.h"
#include "NtlPacketInternal.h"

#define OpATCh_Nop								(0x17000000)
#define OpATCh_LoginUserInsert					(0x17000001)
#define OpATCh_Heartbeat						(0x17000002)
#define OpATCh_NotifyConnection					(0x17000003)

extern SNtlPacketName* g_pNtlPacketName_ATCh[];

#pragma pack(1)

struct SATCh_Nop
{
	SPacketHeader	header;
};

struct SATCh_LoginUserInsert
{
	SPacketHeader	header;
	Byte            byUUID[NTL_MAX_LENGTH_OF_UUID];
	UInt32			uiLoginUserID;
};

struct SATCh_Heartbeat
{
	SPacketHeader	header;
};

struct SATCh_NotifyConnection
{
	SPacketHeader	header;
};

#pragma pack()