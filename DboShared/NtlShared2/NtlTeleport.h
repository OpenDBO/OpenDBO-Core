#pragma once

#include "NtlSharedType.h"
#include "NtlVector.h"

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
#define NTL_MAX_BIND_RADIUS		(8.0f)
//-----------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------

// 텔레포트 종류
enum eTELEPORT_TYPE
{
	TELEPORT_TYPE_DEFAULT,				// 일반적인 상황
	TELEPORT_TYPE_GAME_IN,				// 게임에 처음으로 들어온 경우
	TELEPORT_TYPE_GAME_OUT,				// 게임에서 나가는 경우
	TELEPORT_TYPE_TELEPOPO,				// 텔레포포 아이템을 사용한 경우
	TELEPORT_TYPE_SKILL,				// 순간 이동 등의 스킬을 사용한 경우
	TELEPORT_TYPE_NPC_PORTAL,			// NPC 포탈 서비스를 이용한 경우
	TELEPORT_TYPE_POPOSTONE,			// 부활하여 포포의 돌로 소환된 경우
	TELEPORT_TYPE_WORLD_MOVE,			// 월드간 이동을 한 경우
	TELEPORT_TYPE_TMQ_WORLD_MOVE,		// TMQ 지역으로 진입하거나 TMQ 지역에서 퇴장한 경우
	TELEPORT_TYPE_TMQ_PORTAL,			// TMQ 지역 내에서 텔레포트한 경우

	TELEPORT_TYPE_RANKBATTLE,
	TELEPORT_TYPE_TIMEQUEST,
	TELEPORT_TYPE_TUTORIAL,
	TELEPORT_TYPE_BUDOKAI,				// 천하제일무도회 일반 플로우 관련
	TELEPORT_TYPE_MATCH,				// 천하제일무도회 경기
	TELEPORT_TYPE_MINORMATCH,			// 천하제일무도회 예선전 텔레포트
	TELEPORT_TYPE_MAJORMATCH,			// 천하제일무도회 본선 텔레포트
	TELEPORT_TYPE_FINALMATCH,			// 천하제일무도회 결선 텔레포트

	TELEPORT_TYPE_COMMAND,
	TELEPORT_TYPE_GM,
	TELEPORT_TYPE_DUNGEON,
	TELEPORT_TYPE_DOJO,

	TELEPORT_TYPE_COUNT,

	TELEPORT_TYPE_INVALID = 0xFFui8,

	TELEPORT_TYPE_FIRST = TELEPORT_TYPE_GAME_IN,
	TELEPORT_TYPE_LAST = TELEPORT_TYPE_DUNGEON,
};

enum eDBO_REVIVAL_REQUEST_TYPE
{
	DBO_REVIVAL_REQUEST_TYPE_TELEPORT_SOMEWHERE = 0,
	DBO_REVIVAL_REQUEST_TYPE_CURRENT_POSITION,

	DBO_REVIVAL_REQUEST_TYPE_COUNT,

	DBO_REVIVAL_REQUEST_TYPE_FIRST = DBO_REVIVAL_REQUEST_TYPE_TELEPORT_SOMEWHERE,
	DBO_REVIVAL_REQUEST_TYPE_LAST = DBO_REVIVAL_REQUEST_TYPE_COUNT - 1,
};

// 부활 종류
enum eREVIVAL_TYPE
{
	REVIVAL_TYPE_CURRENT_POSITION = 0, // 현재 위치에서 바로 부활
	REVIVAL_TYPE_BIND_POINT, // 바인드 위치로 이동해서 부활
	REVIVAL_TYPE_RESCUED, // 스킬이나 기타 gm 명령에 의해 바로 부활
	REVIVAL_TYPE_SPECIFIED_POSITION, // 지정한 위치에서 부활

	REVIVAL_TYPE_FIRST = REVIVAL_TYPE_CURRENT_POSITION,
	REVIVAL_TYPE_LAST = REVIVAL_TYPE_RESCUED,
	INVALID_REVIVAL_TYPE = 0xFF
};

// Bind 종류
enum eDBO_BIND_TYPE
{
	DBO_BIND_TYPE_INITIAL_LOCATION = 0,		// 캐릭터가 생성된 후 한 번도 bind를 하지 않은 상태
	DBO_BIND_TYPE_POPO_STONE,				// 포포스톤으로 bind 위치가 지정된 상태
	DBO_BIND_TYPE_GM_TOOL,					// GM tool에 의해 bind 위치가 지정된 상태
	DBO_BIND_TYPE_SKILL,					// 스킬에 의해 bind 위치가 지정된 상태

	DBO_BIND_TYPE_FIRST = DBO_BIND_TYPE_INITIAL_LOCATION,
	DBO_BIND_TYPE_LAST = DBO_BIND_TYPE_SKILL,
	DBO_BIND_TYPE_INVALID = 0xFF
};

//-----------------------------------------------------------------------------------
// 포탈저장 최대개수
//-----------------------------------------------------------------------------------
const int		NTL_PORTAL_MAX_COUNT	= 100;


const DWORD		DBO_TELEPORT_PROPOSAL_WAIT_TIME		= 120;		// 텔레포트 제안창 최대 대기시간



//-----------------------------------------------------------------------------------
#pragma pack(1)

// 서버이동 정보
struct sSERVERTELEPORT_INFO
{
	BYTE			byTeleportType;	// eTELEPORT_TYPE
	WORLDID			worldId;
	TBLIDX			worldTblidx;
	sVECTOR3		vLoc;
	sVECTOR3		vDir;
	SERVERCHANNELID	serverChannelId;
	SERVERINDEX		serverIndex;
};

#pragma pack()
