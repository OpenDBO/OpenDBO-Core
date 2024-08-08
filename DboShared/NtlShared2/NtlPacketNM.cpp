#include "StdAfx.h"
#include "NtlPacketNM.h"

const char * s_packetName_NM[] =
{
	DECLARE_PACKET_NAME( NM_NOTIFY_SERVER_BEGIN ),
	DECLARE_PACKET_NAME( NM_HEARTBEAT ),
	DECLARE_PACKET_NAME( NM_SERVER_CONTROL_TURN_OFF_ALL_NFY ),
	DECLARE_PACKET_NAME( NM_REPORT_LOAD ),
	DECLARE_PACKET_NAME( NM_PING_REQ ),
};


//------------------------------------------------------------------
//
//------------------------------------------------------------------
const char * NtlGetPacketName_NM(WORD wOpCode)
{
	if( wOpCode < NM_OPCODE_BEGIN )
	{
		return "NOT DEFINED PACKET NAME : OPCODE LOW";
	}

	if( wOpCode > NM_OPCODE_END )
	{
		return "NOT DEFINED PACKET NAME : OPCODE HIGH";
	}

	int nIndex = wOpCode - NM_OPCODE_BEGIN;
	if( nIndex >= _countof( s_packetName_NM) )
	{
		return "OPCODE BUFFER OVERFLOW";
	}

	return s_packetName_NM[ nIndex ];
}