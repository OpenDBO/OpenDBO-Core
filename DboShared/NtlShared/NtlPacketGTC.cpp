#include "stdafx.h"
#include "NtlPacketGTC.h"

SNtlPacketName* g_pNtlPacketName_GTC[] =
{
	new SNtlPacketName(OpGTC_Nop, "OpGTC_Nop"),
	new SNtlPacketName(OpGTC_SayMessage, "OpGTC_SayMessage"),
	new SNtlPacketName(OpGTC_Heartbeat, "OpGTC_Heartbeat"),
	new SNtlPacketName(OpGTC_NotifyConnection, "OpGTC_NotifyConnection"),
	NULL
};