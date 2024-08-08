#pragma once
#include "..\NtlNetworkLayerTypeDef.h"
#include "NtlPacketInternal.h"

#define OpUTC_Nop								(0x03000000)
#define OpUTC_TellMessage						(0x03000001)
#define OpUTC_EnterChatServer					(0x03000002)
#define OpUTC_LeaveChatServer					(0x03000003)
#define OpUTC_Heartbeat							(0x03000004)

extern SNtlPacketName* g_pNtlPacketName_UTC[];

#pragma pack(1)

struct SUTC_Nop
{
	SPacketHeader	header;
};

struct SUTC_TellMessage
{
	SPacketHeader	header;
	WChar			wchReceiverCharName[NTL_MAX_LENGTH_OF_CHARACTER_NAME_UNICODE];
	UInt32			uiTellMessageID;
	UInt16			wMessageLengthInUnicode;
	Byte			byVariableField[NTL_MAX_LENGTH_OF_CHAT_MESSAGE_UNICODE * sizeof(WChar)];
};

struct SUTC_EnterChatServer
{
	SPacketHeader	header;
	UInt32			uiCharID;
	Byte			byUUID[NTL_MAX_LENGTH_OF_UUID];
};

struct SUTC_LeaveChatServer
{
	SPacketHeader	header;
};

struct SUTC_Heartbeat
{
	SPacketHeader	header;
};

#pragma pack()