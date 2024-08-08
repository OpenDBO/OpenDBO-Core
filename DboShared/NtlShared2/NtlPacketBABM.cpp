//***********************************************************************************
//
//	File		:	NtlPacketBABM.cpp
//
//	Begin		:	2007-08-09 ¿ÀÀü 11:26:37
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
#include "NtlPacketBABM.h"
//-----------------------------------------------------------------------------------


//------------------------------------------------------------------
//
//------------------------------------------------------------------
const char * s_packetName_BABM[] =
{
	DECLARE_PACKET_NAME( BABM_ENTER_REQ ),
	DECLARE_PACKET_NAME( BABM_COMMAND_RES ),
	DECLARE_PACKET_NAME( BMBA_BOTCLIENTEXEC_RES ),
	DECLARE_PACKET_NAME( BMBA_SETUSERNAME_RES ),
};

//------------------------------------------------------------------
//
//------------------------------------------------------------------
const char * NtlGetPacketName_BABM(WORD wOpCode)
{
	if( wOpCode < BABM_OPCODE_BEGIN )
	{
		return "NOT DEFINED PACKET NAME : OPCODE LOW";
	}

	if( wOpCode > BABM_OPCODE_END )
	{
		return "NOT DEFINED PACKET NAME : OPCODE HIGH";
	}

	int nIndex = wOpCode - BABM_OPCODE_BEGIN;
	if( nIndex >= _countof( s_packetName_BABM ) )
	{
		return "OPCODE BUFFER OVERFLOW";
	}

	return s_packetName_BABM[ nIndex ];
}
//------------------------------------------------------------------