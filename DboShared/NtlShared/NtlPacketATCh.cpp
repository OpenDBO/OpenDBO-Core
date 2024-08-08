#include "stdafx.h"
#include "NtlPacketATCh.h"

SNtlPacketName* g_pNtlPacketName_ATCh[] =
{
	new SNtlPacketName(OpATCh_Nop, "OpATCh_Nop"),
	new SNtlPacketName(OpATCh_LoginUserInsert, "OpATCh_LoginUserInsert"),
	new SNtlPacketName(OpATCh_Heartbeat, "OpATCh_Heartbeat"),
	new SNtlPacketName(OpATCh_NotifyConnection, "OpATCh_NotifyConnection"),
	NULL
};