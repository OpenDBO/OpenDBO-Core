#pragma once
#include "..\NtlNetworkLayerTypeDef.h"
#include "NtlPacketInternal.h"

#define OpCTCh_Nop								(0x37000000)
#define OpCTCh_Heartbeat						(0x37000001)
#define OpCTCh_ReadyToAcceptPlayer				(0x37000002)

extern SNtlPacketName* g_pNtlPacketName_CTCh[];

#pragma pack(1)

struct SCTCh_Nop
{
	SPacketHeader	header;
};

struct SCTCh_Heartbeat
{
	SPacketHeader	header;
};

struct SCTCh_ReadyToAcceptPlayer
{
	SPacketHeader	header;
	UInt32			uiAccountID;
	UInt32			uiCharID;
	UInt16			wErrorCode;
};

#pragma pack()