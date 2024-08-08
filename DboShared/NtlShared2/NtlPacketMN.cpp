#include "StdAfx.h"
#include "NtlPacketMN.h"

const char * s_packetName_MN[] =
{
	DECLARE_PACKET_NAME( MN_HEARTBEAT ),
	DECLARE_PACKET_NAME( MN_GAME_SERVER_FARM_INFO ),
	DECLARE_PACKET_NAME( MN_GAME_SERVER_CHANNEL_INFO ),
	DECLARE_PACKET_NAME( MN_SERVERS_INFO ),
	DECLARE_PACKET_NAME( MN_SERVERS_INFO_END ),
	DECLARE_PACKET_NAME( MN_NOTIFY_ILLEGAL_SERVER ),

	DECLARE_PACKET_NAME( MN_SERVER_INFO_CHANGED_NFY ),
	DECLARE_PACKET_NAME( MN_GAME_SERVER_FARM_INFO_CHANGED_NFY ),
	DECLARE_PACKET_NAME( MN_GAME_SERVER_CHANNEL_INFO_CHANGED_NFY ),
	DECLARE_PACKET_NAME( MN_SERVER_TURNOFF_FOR_DEVEL_NFY ),

	DECLARE_PACKET_NAME( MN_PING_RES ),
};


//------------------------------------------------------------------
//
//------------------------------------------------------------------
const char * NtlGetPacketName_MN(WORD wOpCode)
{
	if( wOpCode < MN_OPCODE_BEGIN )
	{
		return "NOT DEFINED PACKET NAME : OPCODE LOW";
	}

	if( wOpCode > MN_OPCODE_END )
	{
		return "NOT DEFINED PACKET NAME : OPCODE HIGH";
	}

	int nIndex = wOpCode - MN_OPCODE_BEGIN;
	if( nIndex >= _countof( s_packetName_MN) )
	{
		return "OPCODE BUFFER OVERFLOW";
	}

	return s_packetName_MN[ nIndex ];
}