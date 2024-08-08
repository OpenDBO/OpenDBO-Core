#include "stdafx.h"
#include "NtlPacketCTCh.h"

SNtlPacketName* g_pNtlPacketName_CTCh[] =
{
	new SNtlPacketName(OpCTCh_Nop, "OpCTCh_Nop"),
	new SNtlPacketName(OpCTCh_Heartbeat, "OpCTCh_Heartbeat"),
	new SNtlPacketName(OpCTCh_ReadyToAcceptPlayer, "OpCTCh_ReadyToAcceptPlayer"),
	NULL
};