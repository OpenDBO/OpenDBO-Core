#include "stdafx.h"
#include "NtlPacketATU.h"

SNtlPacketName* g_pNtlPacketName_ATU[] =
{
	new SNtlPacketName(OpATU_Nop, "OpATU_Nop"),
	new SNtlPacketName(OpATU_LoginResult, "OpATU_LoginResult"),
	new SNtlPacketName(OpATU_CreateUserResult, "OpATU_CreateUserResult"),
	NULL
};