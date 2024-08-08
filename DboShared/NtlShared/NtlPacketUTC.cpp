#include "stdafx.h"
#include "NtlPacketUTC.h"

SNtlPacketName* g_pNtlPacketName_UTC[] =
{
	new SNtlPacketName(OpUTC_Nop, "OpUTC_Nop"),
	new SNtlPacketName(OpUTC_TellMessage, "OpUTC_TellMessage"),
	new SNtlPacketName(OpUTC_EnterChatServer, "OpUTC_EnterChatServer"),
	new SNtlPacketName(OpUTC_LeaveChatServer, "OpUTC_LeaveChatServer"),
	new SNtlPacketName(OpUTC_Heartbeat, "OpUTC_Heartbeat"),
	NULL
};