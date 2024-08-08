//***********************************************************************************
//
//	File		:	NtlPacketBCBM.cpp
//
//	Begin		:	2007-08-09 ¿ÀÀü 11:28:37
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
#include "NtlPacketBCBM.h"
//-----------------------------------------------------------------------------------


const char * s_packetName_BCBM[] =
{
	DECLARE_PACKET_NAME( BCBM_ENTER_REQ ),
	DECLARE_PACKET_NAME( BCBM_SERVERDATA_REQ ),
	DECLARE_PACKET_NAME( BCBM_BOTCOMMAND_RES ),

	DECLARE_PACKET_NAME( BCBM_PROFILE_BEGIN ),
	DECLARE_PACKET_NAME( BCBM_PROFILE_DATA ),
	DECLARE_PACKET_NAME( BCBM_PROFILE_END ),

	DECLARE_PACKET_NAME( BCBM_BOTLIST_DATA ),
	DECLARE_PACKET_NAME( BCBM_BOTLIST_RES ),

	DECLARE_PACKET_NAME( BCBM_SENDTEST_RES ),
	DECLARE_PACKET_NAME( BCBM_CLIENT_SENDTEST_REQ ),	

	DECLARE_PACKET_NAME( BCBM_CLIENT_LOG_NFY ),
};


//------------------------------------------------------------------
//
//------------------------------------------------------------------
const char * NtlGetPacketName_BCBM(WORD wOpCode)
{
	if( wOpCode < BCBM_OPCODE_BEGIN )
	{
		return "NOT DEFINED PACKET NAME : OPCODE LOW";
	}

	if( wOpCode > BCBM_OPCODE_END )
	{
		return "NOT DEFINED PACKET NAME : OPCODE HIGH";
	}

	int nIndex = wOpCode - BCBM_OPCODE_BEGIN;
	if( nIndex >= _countof( s_packetName_BCBM) )
	{
		return "OPCODE BUFFER OVERFLOW";
	}

	return s_packetName_BCBM[ nIndex ];
}
