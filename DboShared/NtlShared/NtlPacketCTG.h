#pragma once
#include "..\NtlNetworkLayerTypeDef.h"
#include "NtlPacketInternal.h"

#define OpCTG_Nop								(0x32000000)
#define OpCTG_Heartbeat							(0x32000001)

extern SNtlPacketName* g_pNtlPacketName_CTG[];

#pragma pack(1)

struct SCTG_Nop
{
	SPacketHeader	header;
};

struct SCTG_Heartbeat
{
	SPacketHeader	header;
};

#pragma pack()