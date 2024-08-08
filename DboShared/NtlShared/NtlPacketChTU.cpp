#include "stdafx.h"
#include "NtlPacketChTU.h"

SNtlPacketName* g_pNtlPacketName_ChTU[] =
{
	new SNtlPacketName(OpChTU_Nop, "OpChTU_Nop"),
	new SNtlPacketName(OpChTU_CharLoadResult, "OpChTU_CharLoadResult"),
	new SNtlPacketName(OpChTU_CharCreateResult, "OpChTU_CharCreateResult"),
	new SNtlPacketName(OpChTU_CharDeleteResult, "OpChTU_CharDeleteResult"),
	new SNtlPacketName(OpChTU_CharSelectResult, "OpChTU_CharSelectResult"),
	NULL
};