#include "StdAfx.h"
#include "NtlPacketSM.h"


const char * s_packetName_SM[] =
{
	DECLARE_PACKET_NAME( SM_NOTIFY_SERVER_BEGIN ),
	DECLARE_PACKET_NAME( SM_HEARTBEAT ),

	DECLARE_PACKET_NAME( SM_LOGIN_REQ ),
	DECLARE_PACKET_NAME( SM_CHANGE_PASSWORD_REQ	),

	DECLARE_PACKET_NAME( SM_SERVER_STATUS_REQ ),

	DECLARE_PACKET_NAME( SM_SERVER_CONTROL_TURN_ON_REQ ),
	DECLARE_PACKET_NAME( SM_SERVER_CONTROL_TURN_OFF_REQ ),
};


//------------------------------------------------------------------
//
//------------------------------------------------------------------
const char * NtlGetPacketName_SM(WORD wOpCode)
{
	if( wOpCode < SM_OPCODE_BEGIN )
	{
		return "NOT DEFINED PACKET NAME : OPCODE LOW";
	}

	if( wOpCode > SM_OPCODE_END )
	{
		return "NOT DEFINED PACKET NAME : OPCODE HIGH";
	}

	int nIndex = wOpCode - SM_OPCODE_BEGIN;
	if( nIndex >= _countof( s_packetName_SM) )
	{
		return "OPCODE BUFFER OVERFLOW";
	}

	return s_packetName_SM[ nIndex ];
}