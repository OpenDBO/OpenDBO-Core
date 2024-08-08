#include "StdAfx.h"
#include "NtlPacketML.h"


const char * s_packetName_ML[] =
{
	DECLARE_PACKET_NAME( ML_HEARTBEAT ),
	DECLARE_PACKET_NAME( ML_SERVERS_INFO ),
	DECLARE_PACKET_NAME( ML_SERVERS_INFO_END ),
	DECLARE_PACKET_NAME( ML_NOTIFY_ILLEGAL_SERVER ),

	DECLARE_PACKET_NAME( ML_SERVER_INFO_CHANGED_NFY ),
	DECLARE_PACKET_NAME( ML_SERVER_VERSION_CHANGED_NFY ), // by sooshia 07/23/2008
	DECLARE_PACKET_NAME( ML_SERVER_TURNOFF_FOR_DEVEL_NFY ),

	DECLARE_PACKET_NAME( ML_PING_RES ),

};


//------------------------------------------------------------------
//
//------------------------------------------------------------------
const char * NtlGetPacketName_ML(WORD wOpCode)
{
	if( wOpCode < ML_OPCODE_BEGIN )
	{
		return "NOT DEFINED PACKET NAME : OPCODE LOW";
	}

	if( wOpCode > ML_OPCODE_END )
	{
		return "NOT DEFINED PACKET NAME : OPCODE HIGH";
	}

	int nIndex = wOpCode - ML_OPCODE_BEGIN;
	if( nIndex >= _countof( s_packetName_ML) )
	{
		return "OPCODE BUFFER OVERFLOW";
	}

	return s_packetName_ML[ nIndex ];
}