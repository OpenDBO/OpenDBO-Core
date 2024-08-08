#include "StdAfx.h"
#include "NtlPacketPQ.h"


const char * s_packetName_PQ[] =
{
	DECLARE_PACKET_NAME( PQ_HEARTBEAT ),
	DECLARE_PACKET_NAME( PQ_GAME_SERVER_FARM_INFO ),
	DECLARE_PACKET_NAME( PQ_GAME_SERVER_CHANNEL_INFO ),
	DECLARE_PACKET_NAME( PQ_SERVERS_INFO ),
	DECLARE_PACKET_NAME( PQ_NOTIFY_ILLEGAL_SERVER ),
};


//------------------------------------------------------------------
//
//------------------------------------------------------------------
const char * NtlGetPacketName_PQ(WORD wOpCode)
{
	if( wOpCode < PQ_OPCODE_BEGIN )
	{
		return "NOT DEFINED PACKET NAME : OPCODE LOW";
	}

	if( wOpCode > PQ_OPCODE_END )
	{
		return "NOT DEFINED PACKET NAME : OPCODE HIGH";
	}

	int nIndex = wOpCode - PQ_OPCODE_BEGIN;
	if( nIndex >= _countof( s_packetName_PQ) )
	{
		return "OPCODE BUFFER OVERFLOW";
	}

	return s_packetName_PQ[ nIndex ];
}