#include "stdafx.h"
#include "NtlPacketCTU.h"

SNtlPacketName* g_pNtlPacketName_CTU[] =
{
	new SNtlPacketName(OpCTU_Nop, "OpCTU_Nop"),
	new SNtlPacketName(OpCTU_TellMessage, "OpCTU_TellMessage"),
	new SNtlPacketName(OpCTU_TellMessageResult, "OpCTU_TellMessageResult"),
	new SNtlPacketName(OpCTU_SayMessage, "OpCTU_SayMessage"),
	new SNtlPacketName(OpCTU_EnterChatServerResult, "OpCTU_EnterChatServerResult"),
	new SNtlPacketName(OpCTU_Heartbeat, "OpCTU_Heartbeat"),
	NULL
};