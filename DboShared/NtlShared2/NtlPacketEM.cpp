#include "StdAfx.h"
#include "NtlPacketEM.h"

const char * s_packetName_EM[] =
{
	DECLARE_PACKET_NAME( EM_NOTIFY_SERVER_AGENT_BEGIN ),
	DECLARE_PACKET_NAME( EM_REPORT_SYSTEM_INFO ),

	DECLARE_PACKET_NAME( EM_SERVER_TURNED_ON_NFY ),
	DECLARE_PACKET_NAME( EM_SERVER_TURNED_OFF_NFY ),

	DECLARE_PACKET_NAME( EM_SERVER_COMMAND_START ),
	DECLARE_PACKET_NAME( EM_SERVER_COMMAND_INFO ),
	DECLARE_PACKET_NAME( EM_SERVER_COMMAND_END ),
	DECLARE_PACKET_NAME( EM_SERVER_STATUS_LOCALSETTING_INFO_RES ),
	DECLARE_PACKET_NAME( EM_SERVER_STATUS_LOCALSETTING_EDIT_RES ),
};


//------------------------------------------------------------------
//
//------------------------------------------------------------------
const char * NtlGetPacketName_EM(WORD wOpCode)
{
	if( wOpCode < EM_OPCODE_BEGIN )
	{
		return "NOT DEFINED PACKET NAME : OPCODE LOW";
	}

	if( wOpCode > EM_OPCODE_END )
	{
		return "NOT DEFINED PACKET NAME : OPCODE HIGH";
	}

	int nIndex = wOpCode - EM_OPCODE_BEGIN;
	if( nIndex >= _countof( s_packetName_EM) )
	{
		return "OPCODE BUFFER OVERFLOW";
	}

	return s_packetName_EM[ nIndex ];
}