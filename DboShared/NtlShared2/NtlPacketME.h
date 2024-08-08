#pragma once

#include "NtlPacketCommon.h"
#include "NtlCSArchitecture.h"

enum eOPCODE_ME
{
	ME_OPCODE_BEGIN = 52000,

	ME_SERVER_FARM_INFO = ME_OPCODE_BEGIN,
	ME_SERVER_CHANNEL_INFO,
	ME_SERVER_INFO,

	ME_SERVER_CONTROL_TURN_ON_REQ,
	ME_SERVER_CONTROL_TURN_OFF_REQ,
	ME_SERVER_CONTROL_AGENT_PATCH,
	ME_SERVER_COMMAND_REQ,
	ME_SERVER_STATUS_LOCALSETTING_INFO_REQ,
	ME_SERVER_STATUS_LOCALSETTING_EDIT_REQ,

	ME_OPCODE_DUMMY,
	ME_OPCODE_END = ME_OPCODE_DUMMY - 1
};


//------------------------------------------------------------------
//
//------------------------------------------------------------------
const char * NtlGetPacketName_ME(WORD wOpCode);
//------------------------------------------------------------------

#pragma pack(1)

//------------------------------------------------------------------
BEGIN_PROTOCOL(ME_SERVER_FARM_INFO)
	sDBO_GAME_SERVER_FARM_INFO			serverFarmInfo;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(ME_SERVER_CHANNEL_INFO)
	sDBO_GAME_SERVER_CHANNEL_INFO		serverChannelInfo;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(ME_SERVER_INFO)
	sDBO_SERVER_INFO		serverInfo;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(ME_SERVER_CONTROL_TURN_ON_REQ)
	BYTE				byServerType;
	SERVERFARMID		serverFarmId;
	BYTE				byServerChannelIndex;
	BYTE				byServerIndex;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(ME_SERVER_CONTROL_TURN_OFF_REQ)
	BYTE				byServerType;
	SERVERFARMID		serverFarmId;
	BYTE				byServerChannelIndex;
	BYTE				byServerIndex;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(ME_SERVER_CONTROL_AGENT_PATCH)
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(ME_SERVER_COMMAND_REQ)
	ACCOUNTID			managerID;		      // GM ID
	BYTE				byServerType; // eNtlServerType
	WORD				wMessageLengthInUnicode;
	WCHAR				awchCommand[NTL_MAX_LENGTH_OF_CHAT_MESSAGE_UNICODE + 1];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(ME_SERVER_STATUS_LOCALSETTING_INFO_REQ)
	ACCOUNTID			managerID;		      // GM ID
	BYTE				byServerType; // eNtlServerType
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(ME_SERVER_STATUS_LOCALSETTING_EDIT_REQ)
	ACCOUNTID			managerID;		      // GM ID
	BYTE				byServerType; // eNtlServerType
	int					nFileLength;
	char				szFileContent[NTL_MAX_SIZE_LOCALSETTING_FILE + 1];
END_PROTOCOL()
//------------------------------------------------------------------
#pragma pack()