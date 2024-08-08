#include "stdafx.h"
#include "NtlPacketUTCh.h"

SNtlPacketName* g_pNtlPacketName_UTCh[] =
{
	new SNtlPacketName(OpUTCh_Nop, "OpUTCh_Nop"),
	new SNtlPacketName(OpUTCh_CharLoadRequest, "OpUTCh_CharLoadRequest"),
	new SNtlPacketName(OpUTCh_CharCreateRequest, "OpUTCh_CharCreateRequest"),
	new SNtlPacketName(OpUTCh_CharDeleteRequest, "OpUTCh_CharDeleteRequest"),
	new SNtlPacketName(OpUTCh_CharSelectRequest, "OpUTCh_CharSelectRequest"),
	new SNtlPacketName(OpUTCh_CharLogOutRequest, "OpUTCh_CharLogOutRequest"),
	NULL
};