#pragma once
#include "..\NtlNetworkLayerTypeDef.h"
#include "NtlPacketInternal.h"

#define OpGTCh_Nop								(0x27000000)
#define OpGTCh_Heartbeat						(0x27000001)
#define OpGTCh_ReadyToAcceptPlayer				(0x27000002)

extern SNtlPacketName* g_pNtlPacketName_GTCh[];

#pragma pack(1)

struct SGTCh_Nop
{
	SPacketHeader	header;
};

struct SGTCh_Heartbeat
{
	SPacketHeader	header;
};

struct SGTCh_ReadyToAcceptPlayer
{
	SPacketHeader	header;
	UInt32			uiAccountID;
	UInt32			uiCharID;
	UInt16			wErrorCode;
};

#pragma pack()