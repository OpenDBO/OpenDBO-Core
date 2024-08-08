#pragma once

#include "NtlPacketCommon.h"

enum eOPCODE_EM
{
	EM_OPCODE_BEGIN = 53000,

	EM_NOTIFY_SERVER_AGENT_BEGIN = EM_OPCODE_BEGIN,
	EM_REPORT_SYSTEM_INFO,

	EM_SERVER_TURNED_ON_NFY,
	EM_SERVER_TURNED_OFF_NFY,

	EM_SERVER_COMMAND_START,
	EM_SERVER_COMMAND_INFO,
	EM_SERVER_COMMAND_END,
	EM_SERVER_STATUS_LOCALSETTING_INFO_RES,
	EM_SERVER_STATUS_LOCALSETTING_EDIT_RES,

	EM_OPCODE_DUMMY,
	EM_OPCODE_END = EM_OPCODE_DUMMY - 1
};


//------------------------------------------------------------------
//
//------------------------------------------------------------------
const char * NtlGetPacketName_EM(WORD wOpCode);
//------------------------------------------------------------------


#pragma pack(1)

//------------------------------------------------------------------
BEGIN_PROTOCOL(EM_NOTIFY_SERVER_AGENT_BEGIN)
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(EM_REPORT_SYSTEM_INFO)
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(EM_SERVER_TURNED_ON_NFY)
	BYTE				byServerType;
	SERVERFARMID		serverFarmId;
	BYTE				byServerChannelIndex;
	BYTE				byServerIndex;
	char				szRecentModifiedDate[NTL_MAX_LENGTH_OF_RECENT_DATE + 1];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(EM_SERVER_TURNED_OFF_NFY)
	BYTE				byServerType;
	SERVERFARMID		serverFarmId;
	BYTE				byServerChannelIndex;
	BYTE				byServerIndex;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(EM_SERVER_COMMAND_START)
	ACCOUNTID			managerID;		      // GM ID
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(EM_SERVER_COMMAND_INFO)
	ACCOUNTID			managerID;		      // GM ID
	WORD				wMessageLengthInUnicode;
	WCHAR				awchMessage[NTL_MAX_LENGTH_OF_CHAT_MESSAGE_UNICODE + 1];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(EM_SERVER_COMMAND_END)
	ACCOUNTID			managerID;		      // GM ID
	WORD				wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(EM_SERVER_STATUS_LOCALSETTING_INFO_RES)
	ACCOUNTID			managerID;		      // GM ID
	int					nFileLength;
	char				szFileName[NTL_MAX_SIZE_FILE_NAME + 1];
	char				szLocalSettingFile[NTL_MAX_SIZE_LOCALSETTING_FILE +  1];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(EM_SERVER_STATUS_LOCALSETTING_EDIT_RES)
	ACCOUNTID			managerID;		      // GM ID
	WORD				wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
#pragma pack()