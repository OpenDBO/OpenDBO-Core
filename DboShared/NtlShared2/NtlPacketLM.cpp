#include "StdAfx.h"
#include "NtlPacketLM.h"

const char * s_packetName_LM[] =
{
	DECLARE_PACKET_NAME( LM_NOTIFY_SERVER_BEGIN ),
	DECLARE_PACKET_NAME( LM_HEARTBEAT ),
	DECLARE_PACKET_NAME( LM_SERVER_CONTROL_TURN_OFF_ALL_NFY ),
	DECLARE_PACKET_NAME( LM_REPORT_LOAD ),
	DECLARE_PACKET_NAME( LM_PING_REQ ),
};


//------------------------------------------------------------------
//
//------------------------------------------------------------------
const char * NtlGetPacketName_LM(WORD wOpCode)
{
	if( wOpCode < LM_OPCODE_BEGIN )
	{
		return "NOT DEFINED PACKET NAME : OPCODE LOW";
	}

	if( wOpCode > LM_OPCODE_END )
	{
		return "NOT DEFINED PACKET NAME : OPCODE HIGH";
	}

	int nIndex = wOpCode - LM_OPCODE_BEGIN;
	if( nIndex >= _countof( s_packetName_LM) )
	{
		return "OPCODE BUFFER OVERFLOW";
	}

	return s_packetName_LM[ nIndex ];
}