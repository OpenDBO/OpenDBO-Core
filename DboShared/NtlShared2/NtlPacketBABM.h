#pragma once
//***********************************************************************************
//
//	File		:	NtlPacketBABM.h
//
//	Begin		:	2007-08-09 ¿ÀÀü 11:24:43
//
//	Copyright	:	¨Ï NTL-Inc Co. Ltd
//
//	strAuthor	:	Seung Goon. Chi   ( sgpro@ntl-inc.com )
//
//	strDesc		:	
//
//***********************************************************************************

//-- include ------------------------------------------------------------------------
#include "NtlPacketCommon.h"
//-----------------------------------------------------------------------------------
enum eOPCODE_BABM
{
	BABM_OPCODE_BEGIN = 64000,

	BABM_ENTER_REQ,
	BABM_COMMAND_RES,
	BMBA_BOTCLIENTEXEC_RES,
	BMBA_SETUSERNAME_RES,

	BABM_OPCODE_DUMMY,
	BABM_OPCODE_END = BABM_OPCODE_DUMMY - 1
};


//------------------------------------------------------------------
//
//------------------------------------------------------------------
const char * NtlGetPacketName_BABM(WORD wOpCode);
//------------------------------------------------------------------



#pragma pack(1)

//------------------------------------------------------------------
BEGIN_PROTOCOL( BABM_ENTER_REQ )
	char	szUserName[NTL_MAX_SIZE_TOOL_USERNAME];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( BABM_COMMAND_RES )
	WORD	wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( BMBA_BOTCLIENTEXEC_RES )
	WORD	wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( BMBA_SETUSERNAME_RES )
	bool	bResultCode;
	char	szUserName[NTL_MAX_SIZE_TOOL_USERNAME];	
END_PROTOCOL()
//------------------------------------------------------------------

#pragma pack()