#include "StdAfx.h"
#include "NtlPacketMR.h"

const char * s_packetName_MR[] =
{
	DECLARE_PACKET_NAME( MR_HEARTBEAT ),
	DECLARE_PACKET_NAME( MR_GAME_SERVER_FARM_INFO ),
	DECLARE_PACKET_NAME( MR_GAME_SERVER_CHANNEL_INFO ),
	DECLARE_PACKET_NAME( MR_SERVERS_INFO ),
	DECLARE_PACKET_NAME( MR_SERVERS_INFO_END ),
	DECLARE_PACKET_NAME( MR_NOTIFY_ILLEGAL_SERVER ),

	DECLARE_PACKET_NAME( MR_SERVER_INFO_CHANGED_NFY ),
	DECLARE_PACKET_NAME( MR_GAME_SERVER_FARM_INFO_CHANGED_NFY ),
	DECLARE_PACKET_NAME( MR_GAME_SERVER_CHANNEL_INFO_CHANGED_NFY ),
	DECLARE_PACKET_NAME( MR_LOG_SERVER_NFY ),
	DECLARE_PACKET_NAME( MR_SERVER_VERSION_CHANGED_NFY ), // by sooshia 07/23/2008
	DECLARE_PACKET_NAME( MR_SERVER_TURNOFF_FOR_DEVEL_NFY ),

	DECLARE_PACKET_NAME( MR_PING_RES ),

};


//------------------------------------------------------------------
//
//------------------------------------------------------------------
const char * NtlGetPacketName_MR(WORD wOpCode)
{
	if( wOpCode < MR_OPCODE_BEGIN )
	{
		return "NOT DEFINED PACKET NAME : OPCODE LOW";
	}

	if( wOpCode > MR_OPCODE_END )
	{
		return "NOT DEFINED PACKET NAME : OPCODE HIGH";
	}

	int nIndex = wOpCode - MR_OPCODE_BEGIN;
	if( nIndex >= _countof( s_packetName_MR) )
	{
		return "OPCODE BUFFER OVERFLOW";
	}

	return s_packetName_MR[ nIndex ];
}