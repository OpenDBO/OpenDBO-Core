#include "StdAfx.h"
#include "NtlPacketPG.h"


const char * s_packetName_PG[] =
{
	DECLARE_PACKET_NAME( PG_HEARTBEAT ),

	DECLARE_PACKET_NAME( PG_CHAR_SET_GM_GROUP_NFY ),
	DECLARE_PACKET_NAME( PG_CHAR_PUNISH_REQ ),

	DECLARE_PACKET_NAME( PG_USERCOUNT_REQ ),
	DECLARE_PACKET_NAME( PG_GM_GROUP_AUTH_NFY ),
	DECLARE_PACKET_NAME( PG_GM_GROUP_AUTH_DEL_NFY ),
	DECLARE_PACKET_NAME( PG_GM_SERVER_COMMAND_REQ ),
	DECLARE_PACKET_NAME( PG_SERVER_STATUS_REQ ),
	
};


//------------------------------------------------------------------
//
//------------------------------------------------------------------
const char * NtlGetPacketName_PG(WORD wOpCode)
{
	if( wOpCode < PG_OPCODE_BEGIN )
	{
		return "NOT DEFINED PACKET NAME : OPCODE LOW";
	}

	if( wOpCode > PG_OPCODE_END )
	{
		return "NOT DEFINED PACKET NAME : OPCODE HIGH";
	}

	int nIndex = wOpCode - PG_OPCODE_BEGIN;
	if( nIndex >= _countof( s_packetName_PG) )
	{
		return "OPCODE BUFFER OVERFLOW";
	}

	return s_packetName_PG[ nIndex ];
}