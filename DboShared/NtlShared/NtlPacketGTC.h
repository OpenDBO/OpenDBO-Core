#pragma once
#include "..\NtlNetworkLayerTypeDef.h"
#include "NtlPacketInternal.h"

#define OpGTC_Nop								(0x23000000)
#define OpGTC_SayMessage						(0x23000001)
#define OpGTC_Heartbeat							(0x23000002)
#define OpGTC_NotifyConnection					(0x23000003)

extern SNtlPacketName* g_pNtlPacketName_GTC[];

#pragma pack(1)

struct SGTC_Nop
{
	SPacketHeader	header;
};

struct SGTC_SayMessage
{
	SPacketHeader	header;
	UInt32			uiSenderCharID;
	UInt16			wNumberOfPlayers;
	UInt16			wMessageLengthInUnicode;
	Byte			byVariableField[NTL_NUMBER_OF_PLAYERS_IN_SAY_PACKET * sizeof(UInt32) + NTL_MAX_LENGTH_OF_CHAT_MESSAGE_UNICODE * sizeof(WChar)];
};

struct SGTC_Heartbeat
{
	SPacketHeader	header;
};

struct SGTC_NotifyConnection
{
	SPacketHeader	header;
};

#pragma pack()