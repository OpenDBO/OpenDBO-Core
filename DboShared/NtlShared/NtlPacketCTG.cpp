#include "stdafx.h"
#include "NtlPacketCTG.h"

SNtlPacketName* g_pNtlPacketName_CTG[] =
{
	new SNtlPacketName(OpCTG_Nop, "OpCTG_Nop"),
	new SNtlPacketName(OpCTG_Heartbeat, "OpCTG_Heartbeat"),
	NULL
};