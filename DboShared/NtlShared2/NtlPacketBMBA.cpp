//***********************************************************************************
//
//	File		:	NtlPacketBMBA.cpp
//
//	Begin		:	2007-08-09 ¿ÀÀü 11:27:49
//
//	Copyright	:	¨Ï NTL-Inc Co. Ltd
//
//	strAuthor	:	Seung Goon. Chi   ( sgpro@ntl-inc.com )
//
//	strDesc		:	
//
//***********************************************************************************

//-- include ------------------------------------------------------------------------
#include "StdAfx.h"
#include "NtlPacketBMBA.h"
//-----------------------------------------------------------------------------------

const char * s_packetName_BMBA[] =
{
	DECLARE_PACKET_NAME( BMBA_HEARTBEAT ),
	DECLARE_PACKET_NAME( BMBA_ENTER_RES ),
	DECLARE_PACKET_NAME( BMBA_COMMAND_REQ ),
	DECLARE_PACKET_NAME( BMBA_BOTCLIENTEXEC_REQ ),
	DECLARE_PACKET_NAME( BMBA_BOTAGENTCLOSE_NFY ),
	DECLARE_PACKET_NAME( BMBA_SETUSERNAME_REQ ),
};


//------------------------------------------------------------------
//
//------------------------------------------------------------------
const char * NtlGetPacketName_BMBA(WORD wOpCode)
{
	if( wOpCode < BMBA_OPCODE_BEGIN )
	{
		return "NOT DEFINED PACKET NAME : OPCODE LOW";
	}

	if( wOpCode > BMBA_OPCODE_END )
	{
		return "NOT DEFINED PACKET NAME : OPCODE HIGH";
	}

	int nIndex = wOpCode - BMBA_OPCODE_BEGIN;
	if( nIndex >= _countof( s_packetName_BMBA ) )
	{
		return "OPCODE BUFFER OVERFLOW";
	}

	return s_packetName_BMBA[ nIndex ];
}
//------------------------------------------------------------------