#include "stdafx.h"
#include "NtlPacketChTG.h"

SNtlPacketName* g_pNtlPacketName_ChTG[] =
{
	new SNtlPacketName(OpChTG_Nop, "OpChTG_Nop"),
	new SNtlPacketName(OpChTG_CharacterInsert, "OpChTG_CharacterInsert"),
	new SNtlPacketName(OpChTG_Heartbeat, "OpChTG_Heartbeat"),
	new SNtlPacketName(OpChTG_NotifyConnection, "OpChTG_NotifyConnection"),
	NULL
};