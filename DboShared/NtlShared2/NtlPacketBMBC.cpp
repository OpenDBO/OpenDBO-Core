//***********************************************************************************
//
//	File		:	NtlPacketBMBC.cpp
//
//	Begin		:	2007-08-09 ¿ÀÀü 11:30:35
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
#include "NtlPacketBMBC.h"
//-----------------------------------------------------------------------------------


const char * s_packetName_BMBC[] =
{
	DECLARE_PACKET_NAME( BMBC_HEARTBEAT ),
	DECLARE_PACKET_NAME( BMBC_ENTER_RES ),
	DECLARE_PACKET_NAME( BMBC_SERVERDATA_RES ),
	DECLARE_PACKET_NAME( BMBC_BOTCOMMAND_REQ ),
	DECLARE_PACKET_NAME( BMBC_BOTCLIENTCLOSE_NFY ),

	DECLARE_PACKET_NAME( BMBC_BOTLIST_REQ ),

	DECLARE_PACKET_NAME( BMBC_SENDTEST_REQ ),
	DECLARE_PACKET_NAME( BMBC_CLIENT_SENDTEST_NFY ),
	DECLARE_PACKET_NAME( BMBC_CLIENT_SENDTEST_RES ),
};


//------------------------------------------------------------------
//
//------------------------------------------------------------------
const char * NtlGetPacketName_BMBC(WORD wOpCode)
{
	if( wOpCode < BMBC_OPCODE_BEGIN )
	{
		return "NOT DEFINED PACKET NAME : OPCODE LOW";
	}

	if( wOpCode > BMBC_OPCODE_END )
	{
		return "NOT DEFINED PACKET NAME : OPCODE HIGH";
	}

	int nIndex = wOpCode - BMBC_OPCODE_BEGIN;
	if( nIndex >= _countof( s_packetName_BMBC) )
	{
		return "OPCODE BUFFER OVERFLOW";
	}

	return s_packetName_BMBC[ nIndex ];
}