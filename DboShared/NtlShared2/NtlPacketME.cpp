#include "StdAfx.h"
#include "NtlPacketME.h"


const char * s_packetName_ME[] =
{
	DECLARE_PACKET_NAME( ME_SERVER_FARM_INFO ),
	DECLARE_PACKET_NAME( ME_SERVER_CHANNEL_INFO ),
	DECLARE_PACKET_NAME( ME_SERVER_INFO ),

	DECLARE_PACKET_NAME( ME_SERVER_CONTROL_TURN_ON_REQ ),
	DECLARE_PACKET_NAME( ME_SERVER_CONTROL_TURN_OFF_REQ ),
	DECLARE_PACKET_NAME( ME_SERVER_CONTROL_AGENT_PATCH ),
	DECLARE_PACKET_NAME( ME_SERVER_COMMAND_REQ ),
	DECLARE_PACKET_NAME( ME_SERVER_STATUS_LOCALSETTING_INFO_REQ ),
	DECLARE_PACKET_NAME( ME_SERVER_STATUS_LOCALSETTING_EDIT_REQ ),
};


//------------------------------------------------------------------
//
//------------------------------------------------------------------
const char * NtlGetPacketName_ME(WORD wOpCode)
{
	if( wOpCode < ME_OPCODE_BEGIN )
	{
		return "NOT DEFINED PACKET NAME : OPCODE LOW";
	}

	if( wOpCode > ME_OPCODE_END )
	{
		return "NOT DEFINED PACKET NAME : OPCODE HIGH";
	}

	int nIndex = wOpCode - ME_OPCODE_BEGIN;
	if( nIndex >= _countof( s_packetName_ME) )
	{
		return "OPCODE BUFFER OVERFLOW";
	}

	return s_packetName_ME[ nIndex ];
}