#include "StdAfx.h"
#include "NtlPacketRM.h"

const char * s_packetName_RM[] =
{
	DECLARE_PACKET_NAME( RM_NOTIFY_SERVER_BEGIN ),
	DECLARE_PACKET_NAME( RM_HEARTBEAT ),
	DECLARE_PACKET_NAME( RM_SERVER_CONTROL_TURN_OFF_ALL_NFY ),
	DECLARE_PACKET_NAME( RM_REPORT_LOAD ),
	DECLARE_PACKET_NAME( RM_PING_REQ ),
};


//------------------------------------------------------------------
//
//------------------------------------------------------------------
const char * NtlGetPacketName_RM(WORD wOpCode)
{
	if( wOpCode < RM_OPCODE_BEGIN )
	{
		return "NOT DEFINED PACKET NAME : OPCODE LOW";
	}

	if( wOpCode > RM_OPCODE_END )
	{
		return "NOT DEFINED PACKET NAME : OPCODE HIGH";
	}

	int nIndex = wOpCode - RM_OPCODE_BEGIN;
	if( nIndex >= _countof( s_packetName_RM) )
	{
		return "OPCODE BUFFER OVERFLOW";
	}

	return s_packetName_RM[ nIndex ];
}