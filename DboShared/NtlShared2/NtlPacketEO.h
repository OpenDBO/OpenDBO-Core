#pragma once

#include "NtlPacketCommon.h"
#include "NtlCSArchitecture.h"


enum eOPCODE_EO
{
	EO_OPCODE_BEGIN = 54000,

	EO_SERVER_INFO = EO_OPCODE_BEGIN,
	EO_SERVER_TURNED_ON_NFY,
	EO_SERVER_TURNED_OFF_NFY,
	EO_ALL_SERVER_INFO_DESTROYED_NFY,

	EO_OPCODE_DUMMY,
	EO_OPCODE_END = EO_OPCODE_DUMMY - 1
};


//------------------------------------------------------------------
//
//------------------------------------------------------------------
const char * NtlGetPacketName_EO(WORD wOpCode);
//------------------------------------------------------------------


#pragma pack(1)

//------------------------------------------------------------------
BEGIN_PROTOCOL(EO_SERVER_INFO)
	sDBO_SERVER_INFO		serverInfo;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(EO_SERVER_TURNED_ON_NFY)
	BYTE				byServerType;
	SERVERFARMID		serverFarmId;
	BYTE				byServerChannelIndex;
	BYTE				byServerIndex;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(EO_SERVER_TURNED_OFF_NFY)
	BYTE				byServerType;
	SERVERFARMID		serverFarmId;
	BYTE				byServerChannelIndex;
	BYTE				byServerIndex;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(EO_ALL_SERVER_INFO_DESTROYED_NFY)
END_PROTOCOL()
//------------------------------------------------------------------

#pragma pack()