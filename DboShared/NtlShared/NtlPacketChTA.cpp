#include "stdafx.h"
#include "NtlPacketChTA.h"

SNtlPacketName* g_pNtlPacketName_ChTA[] =
{
	new SNtlPacketName(OpChTA_Nop, "OpChTA_Nop"),
	new SNtlPacketName(OpChTA_Heartbeat, "OpChTA_Heartbeat"),
	new SNtlPacketName(OpChTA_ReadyToAcceptUser, "OpChTA_ReadyToAcceptUser"),
	NULL
};