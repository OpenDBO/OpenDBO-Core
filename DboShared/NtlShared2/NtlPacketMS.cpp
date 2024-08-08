#include "StdAfx.h"
#include "NtlPacketMS.h"


const char * s_packetName_MS[] =
{
	DECLARE_PACKET_NAME( MS_HEARTBEAT ),

	DECLARE_PACKET_NAME( MS_LOGIN_RES ),
	DECLARE_PACKET_NAME( MS_CHANGE_PASSWORD_RES ),

	DECLARE_PACKET_NAME( MS_SERVER_STATUS_RES),
	DECLARE_PACKET_NAME( MS_SERVER_CONTROL_TURN_ON_RES),
	DECLARE_PACKET_NAME( MS_SERVER_CONTROL_TURN_OFF_RES),
};


//------------------------------------------------------------------
//
//------------------------------------------------------------------
const char * NtlGetPacketName_MS(WORD wOpCode)
{
	if( wOpCode < MS_OPCODE_BEGIN )
	{
		return "NOT DEFINED PACKET NAME : OPCODE LOW";
	}

	if( wOpCode > MS_OPCODE_END )
	{
		return "NOT DEFINED PACKET NAME : OPCODE HIGH";
	}

	int nIndex = wOpCode - MS_OPCODE_BEGIN;
	if( nIndex >= _countof( s_packetName_MS) )
	{
		return "OPCODE BUFFER OVERFLOW";
	}

	return s_packetName_MS[ nIndex ];
}