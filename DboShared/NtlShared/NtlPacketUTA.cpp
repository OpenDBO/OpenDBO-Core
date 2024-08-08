#include "stdafx.h"
#include "NtlPacketUTA.h"

SNtlPacketName* g_pNtlPacketName_UTA[] =
{
	new SNtlPacketName(OpUTA_Nop, "OpUTA_Nop"),
	new SNtlPacketName(OpUTA_LoginRequest, "OpUTA_LoginRequest"),
	new SNtlPacketName(OpUTA_LogOutRequest, "OpUTA_LogOutRequest"),
	new SNtlPacketName(OpUTA_CreateUserRequest, "OpUTA_CreateUserRequest"),
	NULL
};