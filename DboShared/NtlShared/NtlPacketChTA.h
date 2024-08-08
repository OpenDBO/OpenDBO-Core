#pragma once
#include "..\NtlNetworkLayerTypeDef.h"
#include "NtlPacketInternal.h"

#define OpChTA_Nop								(0x71000000)
#define OpChTA_Heartbeat						(0x71000001)
#define OpChTA_ReadyToAcceptUser				(0x71000002)

extern SNtlPacketName* g_pNtlPacketName_ChTA[];

#pragma pack(1)

struct SChTA_Nop
{
	SPacketHeader	header;
};

struct SChTA_Heartbeat
{
	SPacketHeader	header;
};

struct SChTA_ReadyToAcceptUser
{
	SPacketHeader	header;
	UInt32			uiAccountID;
};

#pragma pack()