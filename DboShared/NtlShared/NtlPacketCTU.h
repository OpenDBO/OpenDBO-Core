#pragma once
#include "..\NtlNetworkLayerTypeDef.h"
#include "NtlPacketInternal.h"

#define OpCTU_Nop								(0x30000000)
#define OpCTU_TellMessage						(0x30000001)
#define OpCTU_TellMessageResult					(0x30000002)
#define OpCTU_SayMessage						(0x30000003)
#define OpCTU_EnterChatServerResult             (0x30000004)
#define OpCTU_Heartbeat							(0x30000005)

extern SNtlPacketName* g_pNtlPacketName_CTU[];

#pragma pack(1)

struct SCTU_Nop
{
	SPacketHeader	header;
};

struct SCTU_TellMessage
{
	SPacketHeader	header;
	WChar			wchSenderCharName[NTL_MAX_LENGTH_OF_CHARACTER_NAME_UNICODE];
	UInt16			wMessageLengthInUnicode;
	Byte			byVariableField[NTL_MAX_LENGTH_OF_CHAT_MESSAGE_UNICODE * sizeof(WChar)];
};

struct SCTU_TellMessageResult
{
	SPacketHeader	header;
	UInt32			uiTellMessageID;
	WORD			wResult;		// 0x200 : Success
									// 0x201 : An error has occured.
									// 0x202 : The receiver is not on-line.
};

struct SCTU_SayMessage
{
	SPacketHeader	header;
	WChar			wchSenderCharName[NTL_MAX_LENGTH_OF_CHARACTER_NAME_UNICODE];
	UInt16			wMessageLengthInUnicode;
	Byte			byVariableField[NTL_MAX_LENGTH_OF_CHAT_MESSAGE_UNICODE * sizeof(WChar)];
};

struct SCTU_EnterChatServerResult
{
	SPacketHeader	header;
	UInt32			uiCharID;
	WORD			wResult;		// 0x200 : Success
									// 0x201 : An error has occured.
};

struct SCTU_Heartbeat
{
	SPacketHeader	header;
};

#pragma pack()