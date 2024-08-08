#include "StdAfx.h"
#include "NtlPacketPA.h"


const char * s_packetName_PA[] =
{
	DECLARE_PACKET_NAME( PA_HEARTBEAT ),

	DECLARE_PACKET_NAME( PA_GM_GROUP_AUTH_NFY ),
	DECLARE_PACKET_NAME( PA_GM_GROUP_AUTH_DEL_NFY ),
	DECLARE_PACKET_NAME( PA_SERVER_STATUS_REQ ),
	
};


//------------------------------------------------------------------
//
//------------------------------------------------------------------
const char * NtlGetPacketName_PA(WORD wOpCode)
{
	if( wOpCode < PA_OPCODE_BEGIN )
	{
		return "NOT DEFINED PACKET NAME : OPCODE LOW";
	}

	if( wOpCode > PA_OPCODE_END )
	{
		return "NOT DEFINED PACKET NAME : OPCODE HIGH";
	}

	int nIndex = wOpCode - PA_OPCODE_BEGIN;
	if( nIndex >= _countof( s_packetName_PA) )
	{
		return "OPCODE BUFFER OVERFLOW";
	}

	return s_packetName_PA[ nIndex ];
}