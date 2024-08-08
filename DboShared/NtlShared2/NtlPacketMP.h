#pragma once

#include "NtlPacketCommon.h"
#include "NtlCSArchitecture.h"
#include "NtlSystemTool.h"


enum eOPCODE_MP
{
	MP_OPCODE_BEGIN = 22000,

	MP_HEARTBEAT = MP_OPCODE_BEGIN,
	MP_AUTH_SERVER_FARM_INFO,
	MP_CHARACTER_SERVER_FARM_INFO,
	MP_GAME_SERVER_FARM_INFO,
	MP_GAME_SERVER_CHANNEL_INFO,
	MP_SERVERS_INFO,
	MP_SERVERS_INFO_END,
	MP_NOTIFY_ILLEGAL_SERVER,

	MP_SERVER_INFO_CHANGED_NFY,
	MP_AUTH_SERVER_FARM_INFO_CHANGED_NFY,
	MP_CHARACTER_SERVER_FARM_INFO_CHANGED_NFY,

	MP_GAME_SERVER_FARM_INFO_CHANGED_NFY,
	MP_GAME_SERVER_CHANNEL_INFO_CHANGED_NFY,

	MP_SERVERFARM_NAME_RES,			// 서버군 이름 응답
	MP_SERVER_CONTROL_TURN_ON_RES,	// 서버 시작 요청 응답
	MP_SERVER_CONTROL_TURN_OFF_RES,	// 서버 종료 요청 응답
	MP_SERVER_LOCK_RES,				// 서버/서버군/서버 채널 잠금 설정 요청 응답
	MP_SERVER_UNLOCK_RES,			// 서버/서버군/서버 채널 잠금 해제 요청 응답
	MP_SERVER_USER_ALL_KICK_RES,		// 접속 중인 사용자의 강제 logout 응답
	MP_SERVER_AGENT_PATCH_RES,			// Server Agent 패치 요청에 대한 응답
	MP_SERVER_VERSION_CHANGED_NFY, // by sooshia 07/22/2008'

	MP_SERVER_COMMAND_START,
	MP_SERVER_COMMAND_INFO,
	MP_SERVER_COMMAND_END,
	MP_SERVERS_RECENTDATE_NFY,
	MP_SERVER_TURNOFF_FOR_DEVEL_NFY,
	MP_SERVER_STATUS_LOCALSETTING_INFO_RES,
	MP_SERVER_STATUS_LOCALSETTING_EDIT_RES,

	MP_PING_RES, // by itzrnb 08/05/2009

	MP_OPCODE_DUMMY,
	MP_OPCODE_END = MP_OPCODE_DUMMY - 1
};


//------------------------------------------------------------------
//
//------------------------------------------------------------------
const char * NtlGetPacketName_MP(WORD wOpCode);
//------------------------------------------------------------------


#pragma pack(1)

//------------------------------------------------------------------
BEGIN_PROTOCOL(MP_HEARTBEAT)
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MP_AUTH_SERVER_FARM_INFO)
sDBO_AUTH_SERVER_FARM_INFO			serverFarmInfo;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MP_CHARACTER_SERVER_FARM_INFO)
sDBO_CHARACTER_SERVER_FARM_INFO		serverFarmInfo;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MP_GAME_SERVER_FARM_INFO)
sDBO_GAME_SERVER_FARM_INFO			serverFarmInfo;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MP_GAME_SERVER_CHANNEL_INFO)
sDBO_GAME_SERVER_CHANNEL_INFO		serverChannelInfo;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MP_SERVERS_INFO)
sDBO_SERVER_INFO		serverInfo;
BYTE					byIsMyInfo;		// 0x00 : false, 0x01 : true
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MP_SERVERS_INFO_END)
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MP_NOTIFY_ILLEGAL_SERVER)
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MP_SERVER_INFO_CHANGED_NFY)
BYTE				byServerType;
SERVERFARMID		serverFarmId;
SERVERCHANNELID		serverChannelId;
SERVERINDEX			serverIndex;

sDBO_SERVER_INFO	serverInfo;
//bool				bIsOn;
//DWORD				dwLoad;
//DWORD				dwProcessUsage;			// Process Usage
//DWORD				dwSystemUsage;			// System Usage
//DWORD				dwMemoryUsage;			// Memory Usage
//DWORD				dwRecvQuery;			// During Queue Recieved Query
//DWORD				dwEndQuery;				// During Queue Ended Query
//DWORD				dwCurQuery;				// Current time Queue Query Count
char				szRecentModifiedDate[NTL_MAX_LENGTH_OF_RECENT_DATE + 1];			
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MP_AUTH_SERVER_FARM_INFO_CHANGED_NFY)
BYTE								byServerStatus;		// eDBO_SERVER_STATUS
DWORD								dwMaxLoad;
DWORD								dwLoad;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MP_CHARACTER_SERVER_FARM_INFO_CHANGED_NFY)
BYTE								byServerStatus;		// eDBO_SERVER_STATUS
DWORD								dwMaxLoad;
DWORD								dwLoad;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MP_GAME_SERVER_FARM_INFO_CHANGED_NFY)
SERVERFARMID						serverFarmId;

BYTE								byServerStatus;		// eDBO_SERVER_STATUS
DWORD								dwMaxLoad;
DWORD								dwLoad;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MP_GAME_SERVER_CHANNEL_INFO_CHANGED_NFY)
SERVERFARMID						serverFarmId;
BYTE								byServerChannelIndex;

BYTE								byServerStatus;		// eDBO_SERVER_STATUS
DWORD								dwMaxLoad;
DWORD								dwLoad;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MP_SERVERFARM_NAME_RES)
WORD					wResultCode;			// 결과

ACCOUNTID				gmAccountId;
BYTE					byServerFarmCount;		// 서버군의 갯수
sSERVERFARM_NAME		asServerFarmName[1];	// 서버 군의 정보
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MP_SERVER_CONTROL_TURN_ON_RES)
WORD					wResultCode;
ACCOUNTID				gmAccountId;
BYTE					byServerType;
SERVERFARMID			serverFarmId;
BYTE					byServerChannelIndex;
BYTE					byServerIndex;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MP_SERVER_CONTROL_TURN_OFF_RES)
WORD					wResultCode;
ACCOUNTID				gmAccountId;
BYTE					byServerType;
SERVERFARMID			serverFarmId;
BYTE					byServerChannelIndex;
BYTE					byServerIndex;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MP_SERVER_LOCK_RES)
WORD					wResultCode;
ACCOUNTID				gmAccountId;
BYTE					byServerType;
SERVERFARMID			serverFarmId;
BYTE					byServerChannelIndex;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MP_SERVER_UNLOCK_RES)
WORD					wResultCode;

ACCOUNTID				gmAccountId;
BYTE				byServerType;
SERVERFARMID			serverFarmId;
BYTE					byServerChannelIndex;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MP_SERVER_USER_ALL_KICK_RES)
WORD					wResultCode;

ACCOUNTID				gmAccountId;
BYTE				byServerType;
SERVERFARMID			serverFarmId;
BYTE					byServerChannelIndex;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MP_SERVER_AGENT_PATCH_RES)
WORD					wResultCode;

ACCOUNTID				gmAccountId;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MP_SERVER_VERSION_CHANGED_NFY) // by sooshia 07/22/2008
WORD				wLVersion;
WORD				wRVersion;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MP_SERVER_COMMAND_START)
ACCOUNTID			managerID;		      // GM ID
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MP_SERVER_COMMAND_INFO)
ACCOUNTID			managerID;		      // GM ID
WORD				wMessageLengthInUnicode;
WCHAR				awchMessage[NTL_MAX_LENGTH_OF_CHAT_MESSAGE_UNICODE + 1];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MP_SERVER_COMMAND_END)
ACCOUNTID			managerID;		      // GM ID
WORD				wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MP_SERVERS_RECENTDATE_NFY)
BYTE				byServerType;
SERVERFARMID		serverFarmId;
SERVERCHANNELID		serverChannelId;
SERVERINDEX			serverIndex;
char				szRecentModifiedDate[NTL_MAX_LENGTH_OF_RECENT_DATE + 1];	
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MP_SERVER_TURNOFF_FOR_DEVEL_NFY)
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MP_SERVER_STATUS_LOCALSETTING_INFO_RES)
ACCOUNTID			managerID;		      // GM ID
int					nFileLength;
char				szFileName[NTL_MAX_SIZE_FILE_NAME + 1];
char				szFileContent[NTL_MAX_SIZE_LOCALSETTING_FILE + 1];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MP_SERVER_STATUS_LOCALSETTING_EDIT_RES)
ACCOUNTID			managerID;		      // GM ID
WORD				wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MP_PING_RES)
DWORD				dwTick;
END_PROTOCOL()
//------------------------------------------------------------------

#pragma pack()