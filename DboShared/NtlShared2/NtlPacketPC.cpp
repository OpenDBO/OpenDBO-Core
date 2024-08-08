#include "StdAfx.h"
#include "NtlPacketPC.h"


const char * s_packetName_PC[] =
{
	DECLARE_PACKET_NAME( PC_HEARTBEAT ),

	DECLARE_PACKET_NAME( PC_GM_GROUP_AUTH_NFY ),
	DECLARE_PACKET_NAME( PC_GM_GROUP_AUTH_DEL_NFY ),
	DECLARE_PACKET_NAME( PC_CHAR_PUNISH_REQ ),

	DECLARE_PACKET_NAME( PC_SERVER_STATUS_REQ ),
};


//------------------------------------------------------------------
//
//------------------------------------------------------------------
const char * NtlGetPacketName_PC(WORD wOpCode)
{
	if( wOpCode < PC_OPCODE_BEGIN )
	{
		return "NOT DEFINED PACKET NAME : OPCODE LOW";
	}

	if( wOpCode > PC_OPCODE_END )
	{
		return "NOT DEFINED PACKET NAME : OPCODE HIGH";
	}

	int nIndex = wOpCode - PC_OPCODE_BEGIN;
	if( nIndex >= _countof( s_packetName_PC) )
	{
		return "OPCODE BUFFER OVERFLOW";
	}

	return s_packetName_PC[ nIndex ];
}