#include "StdAfx.h"
#include "NtlPacketCP.h"


const char * s_packetName_CP[] =
{
	DECLARE_PACKET_NAME( CP_HEARTBEAT ),
	DECLARE_PACKET_NAME( CP_NOTIFY_SERVER_BEGIN ),
	DECLARE_PACKET_NAME( CP_CHAR_PUNISH_RES ),

	DECLARE_PACKET_NAME( CP_SERVER_STATUS_RES ),
};


//------------------------------------------------------------------
//
//------------------------------------------------------------------
const char * NtlGetPacketName_CP(WORD wOpCode)
{
	if( wOpCode < CP_OPCODE_BEGIN )
	{
		return "NOT DEFINED PACKET NAME : OPCODE LOW";
	}

	if( wOpCode > CP_OPCODE_END )
	{
		return "NOT DEFINED PACKET NAME : OPCODE HIGH";
	}

	int nIndex = wOpCode - CP_OPCODE_BEGIN;
	if( nIndex >= _countof( s_packetName_CP) )
	{
		return "OPCODE BUFFER OVERFLOW";
	}

	return s_packetName_CP[ nIndex ];
}