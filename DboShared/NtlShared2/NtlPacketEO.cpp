#include "StdAfx.h"
#include "NtlPacketEO.h"

const char * s_packetName_EO[] =
{
	DECLARE_PACKET_NAME( EO_SERVER_INFO ),
	DECLARE_PACKET_NAME( EO_SERVER_TURNED_ON_NFY ),
	DECLARE_PACKET_NAME( EO_SERVER_TURNED_OFF_NFY ),
	DECLARE_PACKET_NAME( EO_ALL_SERVER_INFO_DESTROYED_NFY ),
};


//------------------------------------------------------------------
//
//------------------------------------------------------------------
const char * NtlGetPacketName_EO(WORD wOpCode)
{
	if( wOpCode < EO_OPCODE_BEGIN )
	{
		return "NOT DEFINED PACKET NAME : OPCODE LOW";
	}

	if( wOpCode > EO_OPCODE_END )
	{
		return "NOT DEFINED PACKET NAME : OPCODE HIGH";
	}

	int nIndex = wOpCode - EO_OPCODE_BEGIN;
	if( nIndex >= _countof( s_packetName_EO) )
	{
		return "OPCODE BUFFER OVERFLOW";
	}

	return s_packetName_EO[ nIndex ];
}