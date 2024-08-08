#include "StdAfx.h"
#include "NtlPacketGP.h"


const char * s_packetName_GP[] =
{
	DECLARE_PACKET_NAME( GP_HEARTBEAT ),
	DECLARE_PACKET_NAME( GP_NOTIFY_SERVER_BEGIN ),

	DECLARE_PACKET_NAME( GP_CHAR_PUNISH_RES ),
	DECLARE_PACKET_NAME( GP_USERCOUNT_RES ),
	DECLARE_PACKET_NAME( GP_GM_SERVER_COMMAND_RES ),
	DECLARE_PACKET_NAME( GP_SERVER_STATUS_RES ),
	
};


//------------------------------------------------------------------
//
//------------------------------------------------------------------
const char * NtlGetPacketName_GP(WORD wOpCode)
{
	if( wOpCode < GP_OPCODE_BEGIN )
	{
		return "NOT DEFINED PACKET NAME : OPCODE LOW";
	}

	if( wOpCode > GP_OPCODE_END )
	{
		return "NOT DEFINED PACKET NAME : OPCODE HIGH";
	}

	int nIndex = wOpCode - GP_OPCODE_BEGIN;
	if( nIndex >= _countof( s_packetName_GP) )
	{
		return "OPCODE BUFFER OVERFLOW";
	}

	return s_packetName_GP[ nIndex ];
}