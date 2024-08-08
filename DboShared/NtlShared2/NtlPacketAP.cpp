#include "StdAfx.h"
#include "NtlPacketAP.h"


const char * s_packetName_AP[] =
{
	DECLARE_PACKET_NAME( AP_HEARTBEAT ),
	DECLARE_PACKET_NAME( AP_NOTIFY_SERVER_BEGIN ),
	DECLARE_PACKET_NAME( AP_SERVER_STATUS_RES ),
	
};


//------------------------------------------------------------------
//
//------------------------------------------------------------------
const char * NtlGetPacketName_AP(WORD wOpCode)
{
	if( wOpCode < AP_OPCODE_BEGIN )
	{
		return "NOT DEFINED PACKET NAME : OPCODE LOW";
	}

	if( wOpCode > AP_OPCODE_END )
	{
		return "NOT DEFINED PACKET NAME : OPCODE HIGH";
	}

	int nIndex = wOpCode - AP_OPCODE_BEGIN;
	if( nIndex >= _countof( s_packetName_AP) )
	{
		return "OPCODE BUFFER OVERFLOW";
	}

	return s_packetName_AP[ nIndex ];
}