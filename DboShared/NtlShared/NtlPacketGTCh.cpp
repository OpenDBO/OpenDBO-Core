#include "stdafx.h"
#include "NtlPacketGTCh.h"

SNtlPacketName* g_pNtlPacketName_GTCh[] =
{
	new SNtlPacketName(OpGTCh_Nop, "OpGTCh_Nop"),
	new SNtlPacketName(OpGTCh_Heartbeat, "OpGTCh_Heartbeat"),
	new SNtlPacketName(OpGTCh_ReadyToAcceptPlayer, "OpGTCh_ReadyToAcceptPlayer"),
	NULL
};