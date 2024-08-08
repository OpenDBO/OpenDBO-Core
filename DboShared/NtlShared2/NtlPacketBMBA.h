#pragma once
//***********************************************************************************
//
//	File		:	NtlPacketBMBA.h
//
//	Begin		:	2007-08-09 ¿ÀÀü 11:27:07
//
//	Copyright	:	¨Ï NTL-Inc Co. Ltd
//
//	strAuthor	:	Seung Goon. Chi   ( sgpro@ntl-inc.com )
//
//	strDesc		:	
//
//***********************************************************************************

//-- include ------------------------------------------------------------------------
#include "NtlSharedBotSystemDef.h"
#include "NtlPacketCommon.h"
#include "NtlSharedDef.h"
//-----------------------------------------------------------------------------------

enum eOPCODE_BMBA
{
	BMBA_OPCODE_BEGIN = 64100,

	BMBA_HEARTBEAT,
	BMBA_ENTER_RES,
	BMBA_COMMAND_REQ,	
	BMBA_BOTCLIENTEXEC_REQ,
	BMBA_BOTAGENTCLOSE_NFY,
	BMBA_SETUSERNAME_REQ,

	BMBA_OPCODE_DUMMY,
	BMBA_OPCODE_END = BMBA_OPCODE_DUMMY - 1
};


//------------------------------------------------------------------
//
//------------------------------------------------------------------
const char * NtlGetPacketName_BMBA(WORD wOpCode);
//------------------------------------------------------------------


#pragma pack(1)

//------------------------------------------------------------------
BEGIN_PROTOCOL( BMBA_HEARTBEAT )
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( BMBA_ENTER_RES )
	WORD			wResultCode;
	unsigned int	botgroupID;
	unsigned int	botagentID;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( BMBA_COMMAND_REQ )
	char szCommand[NTL_MAX_BOTAGENTCOMMAND_STRING_SIZE + 1];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( BMBA_BOTCLIENTEXEC_REQ )
	int nExecCount;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( BMBA_BOTAGENTCLOSE_NFY )
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( BMBA_SETUSERNAME_REQ )
	char	szUserName[NTL_MAX_SIZE_TOOL_USERNAME];
END_PROTOCOL()
//------------------------------------------------------------------

#pragma pack()