#include "stdafx.h"
#include "NtlPacketChTC.h"

SNtlPacketName* g_pNtlPacketName_ChTC[] =
{
	new SNtlPacketName(OpChTC_Nop, "OpChTC_Nop"),
	new SNtlPacketName(OpChTC_Heartbeat, "OpChTC_Heartbeat"),
	new SNtlPacketName(OpChTC_NotifyConnection, "OpChTC_NotifyConnection"),
	new SNtlPacketName(OpChTC_CharacterInsert, "OpChTC_CharacterInsert"),
	NULL
};