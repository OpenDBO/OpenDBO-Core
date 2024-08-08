#pragma once

#include "NtlSharedType.h"
#include "NtlSharedDef.h"
#include "NtlCharacter.h"

// Server Type Enumeration
enum eNtlServerType
{
	NTL_SERVER_TYPE_MASTER = 0,
	NTL_SERVER_TYPE_AUTH,//SP_SERVER_LOCK_REQ - byServerType
	NTL_SERVER_TYPE_CHARACTER,//SP_SERVER_LOCK_REQ - byServerType
	NTL_SERVER_TYPE_GAME,//SP_SERVER_LOCK_REQ - byServerType
	NTL_SERVER_TYPE_NPC,
	NTL_SERVER_TYPE_QUERY,
	NTL_SERVER_TYPE_COMMUNITY,
	NTL_SERVER_TYPE_LOG,
	NTL_SERVER_TYPE_OPERATING,
	NTL_SERVER_TYPE_REPORT,
	NTL_SERVER_TYPE_SERVER_AGENT,
	NTL_SERVER_TYPE_SERVER_AGENT_CONTROLLER,
	
	NTL_SERVER_TYPE_INVALID = 0xFF,

	NTL_SERVER_TYPE_FIRST = NTL_SERVER_TYPE_MASTER,
	NTL_SERVER_TYPE_LAST = NTL_SERVER_TYPE_SERVER_AGENT_CONTROLLER,
	NTL_SERVER_TYPE_COUNT = NTL_SERVER_TYPE_LAST - NTL_SERVER_TYPE_FIRST + 1,

	NTL_SERVER_TYPE_SLAVE_FIRST = NTL_SERVER_TYPE_AUTH,
	NTL_SERVER_TYPE_SLAVE_LAST = NTL_SERVER_TYPE_REPORT,
	NTL_SERVER_TYPE_SLAVE_COUNT = NTL_SERVER_TYPE_SLAVE_LAST - NTL_SERVER_TYPE_SLAVE_FIRST + 1
};

// Server Type Enumeration [11/20/2007 SGpro]
enum eNtlServerGroupType
{
	NTL_SERVER_GROUP_TYPE_AUTH = 0,
	NTL_SERVER_GROUP_TYPE_CHARACTER,
	NTL_SERVER_GROUP_TYPE_SERVERFARM_TOP,//모든 서버팜에 최상위
	NTL_SERVER_GROUP_TYPE_SERVERFARM, //각각의 서버팜
	NTL_SERVER_GROUP_TYPE_CHANNEL,//서버팜 밑에 채널

	NTL_SERVER_GROUP_TYPE_INVALID = 0xFF,

	NTL_SERVER_GROUP_TYPE_FIRST = NTL_SERVER_GROUP_TYPE_AUTH,
	NTL_SERVER_GROUP_TYPE_LAST = NTL_SERVER_GROUP_TYPE_CHANNEL,
	NTL_SERVER_GROUP_TYPE_COUNT = NTL_SERVER_TYPE_LAST - NTL_SERVER_TYPE_FIRST + 1,

	NTL_SERVER_GROUP_TYPE_SLAVE_FIRST = NTL_SERVER_GROUP_TYPE_SERVERFARM_TOP,
	NTL_SERVER_GROUP_TYPE_SLAVE_LAST = NTL_SERVER_GROUP_TYPE_CHANNEL,
	NTL_SERVER_GROUP_TYPE_SLAVE_COUNT = NTL_SERVER_GROUP_TYPE_SLAVE_LAST - NTL_SERVER_GROUP_TYPE_SLAVE_FIRST + 1
};

enum eDBO_SERVER_STATUS
{
	DBO_SERVER_STATUS_UP = 0,			//정상 동작
	DBO_SERVER_STATUS_DOWN,				//점검중
	DBO_SERVER_STATUS_LOCKED			//잠김
};

enum eDBO_SERVER_STATUS_DETAIL
{
	DBO_SERVER_STATUS_DETAIL_ENTERABLE =0,
	DBO_SERVER_STATUS_DETAIL_FULL,

	DBO_SERVER_STATUS_DETAIL_COUNT
};

enum eDBO_GAME_SERVER_ROLE
{
	DBO_GAME_SERVER_ROLE_GENERAL = 0,
	DBO_GAME_SERVER_ROLE_TENKAICHI_BODOKAI,

	DBO_GAME_SERVER_ROLE_COUNT
};


enum eDBO_STATS_SEX_RANGE
{
	DBO_STATS_SEX_RANGE_MALE = 0,		// Male
	DBO_STATS_SEX_RANGE_FEMALE,			// Female

	DBO_STATS_SEX_RANGE_COUNT,

	DBO_STATS_SEX_RANGE_FIRST = DBO_STATS_SEX_RANGE_MALE,
	DBO_STATS_SEX_RANGE_LAST = DBO_STATS_SEX_RANGE_COUNT - 1
};

enum eDBO_STATS_LEVEL_RANGE
{
	DBO_STATS_LEVEL_RANGE_10 = 0,		//  1 ~ 10
	DBO_STATS_LEVEL_RANGE_20,			// 11 ~ 20
	DBO_STATS_LEVEL_RANGE_30,			// 21 ~ 30
	DBO_STATS_LEVEL_RANGE_40,			// 31 ~ 40
	DBO_STATS_LEVEL_RANGE_50,			// 41 ~

	DBO_STATS_LEVEL_RANGE_COUNT,

	DBO_STATS_LEVEL_RANGE_FIRST = DBO_STATS_LEVEL_RANGE_10,
	DBO_STATS_LEVEL_RANGE_LAST = DBO_STATS_LEVEL_RANGE_COUNT - 1
};

enum eDBO_STATS_AGE_RANGE
{
	DBO_STATS_AGE_RANGE_10 = 0,		//  1 ~ 10
	DBO_STATS_AGE_RANGE_20,			// 11 ~ 20
	DBO_STATS_AGE_RANGE_30,			// 21 ~ 30
	DBO_STATS_AGE_RANGE_40,			// 31 ~ 40
	DBO_STATS_AGE_RANGE_50,			// 41 ~ 50
	DBO_STATS_AGE_RANGE_60,			// 51 ~

	DBO_STATS_AGE_RANGE_COUNT,

	DBO_STATS_AGE_RANGE_FIRST = DBO_STATS_AGE_RANGE_10,
	DBO_STATS_AGE_RANGE_LAST = DBO_STATS_AGE_RANGE_COUNT - 1
};

#define DBO_MAX_CHARACTER_SERVER_COUNT		(10)
#define DBO_MAX_WORLD_COUNT_IN_INFO_PACKET		(100)
#define DBO_MAX_ADAPTER_ADDRESS_LENGTH		(6)

#pragma pack(1)

struct sNTLSERVERINFO
{
	sNTLSERVERINFO() :
			byServerType(0xFF),
			serverFarmId(INVALID_SERVERFARMID),
			byServerChannelIndex(INVALID_SERVERCHANNELID),
			byServerIndex(DEFAULT_SERVERINDEX),
			wPortForClient(0),
			wPortForAuthServer(0),
			wPortForCharacterServer(0),
			wPortForGameServer(0),
			wPortForNpcServer(0),
			wPortForQueryServer(0),
			wPortForCommunityServer(0),
			wPortForLogServer(0),
			wPortForOperatingServer(0),
			wPortForReportServer(0),
			wPortForSystemTool(0),
			byGameServerRole(DBO_GAME_SERVER_ROLE_GENERAL)
	{
		::ZeroMemory(achInternalAddress, sizeof(achInternalAddress));
		::ZeroMemory(achExternalAddress, sizeof(achExternalAddress));
	};

	sNTLSERVERINFO(sNTLSERVERINFO* pNtlServerInfo)
	{
		::CopyMemory(this, pNtlServerInfo, sizeof(*this));
	};

	BYTE				byServerType;
	SERVERFARMID		serverFarmId;
	BYTE				byServerChannelIndex;
	BYTE				byServerIndex;
	char				achInternalAddress[NTL_MAX_LENGTH_OF_IP + 1];
	char				achExternalAddress[NTL_MAX_LENGTH_OF_IP + 1];
	WORD				wPortForClient;
	WORD				wPortForAuthServer;
	WORD				wPortForCharacterServer;
	WORD				wPortForGameServer;
	WORD				wPortForNpcServer;
	WORD				wPortForQueryServer;
	WORD				wPortForCommunityServer;
	WORD				wPortForLogServer;
	WORD				wPortForOperatingServer;
	WORD				wPortForReportServer;
	WORD				wPortForSystemTool;
	BYTE				byGameServerRole;		// eDBO_GAME_SERVER_ROLE
};

struct sNTL_SERVER_IP_PORT
{
	char achServerIP[NTL_MAX_LENGTH_OF_IP + 1];
	WORD wServerPortForClient;
};

struct sDBO_SERVER_INFO : public sNTLSERVERINFO
{
	sDBO_SERVER_INFO() :
			sNTLSERVERINFO(),
			bIsOn(false),
			dwMaxLoad(0),
			dwLoad(0),
			dwProcessUsage(0),
			dwSystemUsage(0),
			dwMemoryUsage(0),
			dwRecvQuery(0),
			dwEndQuery(0),
			dwCurQuery(0),
			dwPing(0) // itzrnb
	{
	};

	sDBO_SERVER_INFO(sDBO_SERVER_INFO* pServerInfo) :
			sNTLSERVERINFO(pServerInfo),
			bIsOn(pServerInfo->bIsOn),
			dwMaxLoad(pServerInfo->dwMaxLoad),
			dwLoad(pServerInfo->dwLoad),
			dwProcessUsage(pServerInfo->dwProcessUsage),
			dwSystemUsage(pServerInfo->dwSystemUsage),
			dwMemoryUsage(pServerInfo->dwMemoryUsage),
			dwRecvQuery(pServerInfo->dwRecvQuery),
			dwEndQuery(pServerInfo->dwEndQuery),
			dwCurQuery(pServerInfo->dwCurQuery),
			dwPing(pServerInfo->dwPing) // itzrnb
	{
	};

	sDBO_SERVER_INFO& operator=(sDBO_SERVER_INFO& rServerInfo)
	{
		::CopyMemory(this, &rServerInfo, sizeof(sDBO_SERVER_INFO));

		return *this;
	}

	bool bIsOn;
	DWORD dwMaxLoad;
	DWORD dwLoad;
	DWORD dwProcessUsage;			// Process Usage
	DWORD dwSystemUsage;			// System Usage
	DWORD dwMemoryUsage;			// Memory Usage
	DWORD dwRecvQuery;				// During Queue Recieved Query
	DWORD dwEndQuery;				// During Queue Ended Query
	DWORD dwCurQuery;				// Current time Queue Query Count
	DWORD dwPing;					// itzrnb
};

struct sDBO_AUTH_SERVER_FARM_INFO
{
	sDBO_AUTH_SERVER_FARM_INFO() :
			byServerStatus(DBO_SERVER_STATUS_DOWN),
			dwMaxLoad(0),
			dwLoad(0)
	{
	};

	BYTE byServerStatus;		// eDBO_SERVER_STATUS
	DWORD dwMaxLoad;
	DWORD dwLoad;
};

struct sDBO_CHARACTER_SERVER_FARM_INFO
{
	sDBO_CHARACTER_SERVER_FARM_INFO() :
			byServerStatus(DBO_SERVER_STATUS_DOWN),
			dwMaxLoad(0),
			dwLoad(0)
	{
	};

	BYTE byServerStatus;		// eDBO_SERVER_STATUS
	DWORD dwMaxLoad;
	DWORD dwLoad;
};

struct sDBO_GAME_SERVER_CHANNEL_INFO
{
	sDBO_GAME_SERVER_CHANNEL_INFO() :
			serverFarmId(INVALID_SERVERFARMID),
			byServerChannelIndex(INVALID_SLOTID),
			bIsVisible(true),
			byServerStatus(DBO_SERVER_STATUS_DOWN),
			dwMaxLoad(0),
			dwLoad(0)
	{
	};

	SERVERFARMID serverFarmId;
	BYTE byServerChannelIndex;
	bool bIsVisible;

	BYTE byServerStatus;		// eDBO_SERVER_STATUS
	// Load value is stored in percents if it's for Client.
	// 'dwMaxLoad' should be 100.
	// 클라이언트에서 사용할 목적으로 값을 세팅할 때는 퍼센트 단위로 저장한다.
	// 'dwMaxLoad'는 100이 된다.
	// by YOSHIKI(2007-01-29)
	DWORD dwMaxLoad;
	DWORD dwLoad;
};

struct sDBO_GAME_SERVER_FARM_INFO
{
	sDBO_GAME_SERVER_FARM_INFO() :
			serverFarmId(INVALID_SERVERFARMID),
			byServerStatus(DBO_SERVER_STATUS_DOWN),
			dwMaxLoad(0),
			dwLoad(0)
	{
		::ZeroMemory(wszGameServerFarmName, sizeof(wszGameServerFarmName));
	};

	SERVERFARMID serverFarmId;
	WCHAR wszGameServerFarmName[NTL_MAX_SIZE_SERVER_FARM_NAME_UNICODE + 1];

	BYTE byServerStatus;		// eDBO_SERVER_STATUS
	// Load value is stored in percents if it's for Client.
	// 'dwMaxLoad' should be 100.
	// 클라이언트에서 사용할 목적으로 값을 세팅할 때는 퍼센트 단위로 저장한다.
	// 'dwMaxLoad'는 100이 된다.
	// by YOSHIKI(2007-01-29)
	DWORD dwMaxLoad;
	DWORD dwLoad;
};

struct sDBO_SERVER_LOCK_INFO
{
	sDBO_SERVER_LOCK_INFO() :
			byServerType(0xFF),
			serverFarmId(INVALID_SERVERFARMID),
			byServerChannelIndex(INVALID_SLOTID),
			bIsLock(false)
	{
	};
	BYTE				byServerType;
	SERVERFARMID		serverFarmId;
	BYTE				byServerChannelIndex;
	bool				bIsLock;
};



// AU_LOGIN_RES, GU_CHAR_EXIT_RES 등에서 사용
struct sSERVER_INFO
{
	char		szCharacterServerIP[NTL_MAX_LENGTH_OF_IP + 1];
	WORD		wCharacterServerPortForClient;
	DWORD		dwLoad;
};

struct sDBO_SERVER_CHANNEL_CONNECTED_USER_INFO
{
	sDBO_SERVER_CHANNEL_CONNECTED_USER_INFO()
	{
		::ZeroMemory(adwPlayerCountBySex, sizeof(adwPlayerCountBySex));
		::ZeroMemory(adwPlayerCountByRace, sizeof(adwPlayerCountByRace));
		::ZeroMemory(adwPlayerCountByLevel, sizeof(adwPlayerCountByLevel));
		::ZeroMemory(adwPlayerCountByAge, sizeof(adwPlayerCountByAge));
	}

	sDBO_SERVER_CHANNEL_CONNECTED_USER_INFO& operator+=(const sDBO_SERVER_CHANNEL_CONNECTED_USER_INFO& rDiff)
	{
		for (BYTE byIndex = 0 ; byIndex < DBO_STATS_SEX_RANGE_COUNT ; byIndex++)
		{
			adwPlayerCountBySex[byIndex] += (rDiff.adwPlayerCountBySex)[byIndex];
		}
		for (BYTE byIndex = 0 ; byIndex < RACE_COUNT ; byIndex++)
		{
			adwPlayerCountByRace[byIndex] += (rDiff.adwPlayerCountByRace)[byIndex];
		}
		for (BYTE byIndex = 0 ; byIndex < DBO_STATS_LEVEL_RANGE_COUNT ; byIndex++)
		{
			adwPlayerCountByLevel[byIndex] += (rDiff.adwPlayerCountByLevel)[byIndex];
		}
		for (BYTE byIndex = 0 ; byIndex < DBO_STATS_AGE_RANGE_COUNT ; byIndex++)
		{
			adwPlayerCountByAge[byIndex] += (rDiff.adwPlayerCountByAge)[byIndex];
		}

		return *this;
	}

	DWORD GetPlayerCountTotal()
	{
		DWORD dwTotal = 0;

		for (BYTE byIndex = 0 ; byIndex < DBO_STATS_SEX_RANGE_COUNT ; byIndex++)
		{
			dwTotal += adwPlayerCountBySex[byIndex];
		}

		return dwTotal;
	}

	DWORD				adwPlayerCountBySex[DBO_STATS_SEX_RANGE_COUNT];
	DWORD				adwPlayerCountByRace[RACE_COUNT];
	DWORD				adwPlayerCountByLevel[DBO_STATS_LEVEL_RANGE_COUNT];
	DWORD				adwPlayerCountByAge[DBO_STATS_AGE_RANGE_COUNT];
};

#pragma pack()