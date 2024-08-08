#include "StdAfx.h"
#include "NtlPacketOE.h"


const char * s_packetName_OE[] =
{
	DECLARE_PACKET_NAME( OE_NOTIFY_AGENT_CONTROLLER_BEGIN ),

	DECLARE_PACKET_NAME( OE_SERVER_CONTROL_TURN_ON_REQ ),
	DECLARE_PACKET_NAME( OE_SERVER_CONTROL_TURN_OFF_REQ ),
};


//------------------------------------------------------------------
//
//------------------------------------------------------------------
const char * NtlGetPacketName_OE(WORD wOpCode)
{
	if( wOpCode < OE_OPCODE_BEGIN )
	{
		return "NOT DEFINED PACKET NAME : OPCODE LOW";
	}

	if( wOpCode > OE_OPCODE_END )
	{
		return "NOT DEFINED PACKET NAME : OPCODE HIGH";
	}

	int nIndex = wOpCode - OE_OPCODE_BEGIN;
	if( nIndex >= _countof( s_packetName_OE) )
	{
		return "OPCODE BUFFER OVERFLOW";
	}

	return s_packetName_OE[ nIndex ];
}