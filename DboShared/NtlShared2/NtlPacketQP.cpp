#include "StdAfx.h"
#include "NtlPacketQP.h"


const char * s_packetName_QP[] =
{
	DECLARE_PACKET_NAME( QP_NOTIFY_SERVER_BEGIN ),
	DECLARE_PACKET_NAME( QP_HEARTBEAT ),

	DECLARE_PACKET_NAME( QP_CHAR_PUNISH_RES ),
};


//------------------------------------------------------------------
//
//------------------------------------------------------------------
const char * NtlGetPacketName_QP(WORD wOpCode)
{
	if( wOpCode < QP_OPCODE_BEGIN )
	{
		return "NOT DEFINED PACKET NAME : OPCODE LOW";
	}

	if( wOpCode > QP_OPCODE_END )
	{
		return "NOT DEFINED PACKET NAME : OPCODE HIGH";
	}

	int nIndex = wOpCode - QP_OPCODE_BEGIN;
	if( nIndex >= _countof( s_packetName_QP) )
	{
		return "OPCODE BUFFER OVERFLOW";
	}

	return s_packetName_QP[ nIndex ];
}