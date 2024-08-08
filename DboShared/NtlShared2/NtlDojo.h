//***********************************************************************************
//
//	File		:	NtlDojo.h
//
//	Begin		:	2009-01-07
//
//	Copyright	:	ⓒ NTL-Inc Co., Ltd
//
//	Author		:	Chung,DooSup (mailto:john@ntl-inc.com)
//
//	Desc		:	
//
//***********************************************************************************

#pragma once

#include "NtlSharedType.h"
#include "NtlSharedDef.h"
#include "NtlGuild.h"
#include "NtlSharedType.h"
#include "NtlVector.h"

#pragma pack(1)

const DWORD DBO_MAX_COUNT_DOJO_IN_WORLD = 7;
const DWORD DBO_MAX_COUNT_DOJO_BANK_HISTORY = 40;
const DWORD DBO_DOJO_SCRAMBLE_REQUEST_FARE = 1000000;		// 도장 쟁탈전 신청금액 (비무전 없이 할때)
const DWORD DBO_DOJO_SCRAMBLE_REJECT_FARE = 1000000;		// 도장 쟁탈전 거부금액 (비무전 없이 할때)
const DWORD DBO_DOJO_SCRAMBLE_REQUIRED_REPUTATION = 100000;

const DWORD DBO_DOJO_SCRAMBLE_MAX_WAIT_TICK = 10 * 60 * 1000; // 리더에게 신청알림 최대대기시간 10 분
const DWORD DBO_DOJO_SCRAMBLE_INIT_WAIT_SEC = 10; // 쟁탈전 끝난후 평상으로 돌아가는데 10초

const DWORD DBO_DOJO_SCRAMBLE_MAX_MEMBER = 30; // 쟁탈전 한팀 참여 가능 최대 인원

/*const DWORD	DBO_DOJO_SCRAMBLE_CHANGE_SCRAMBLE_POINT = 1;	// seal 획득시 쟁탈점수
const DWORD	DBO_DOJO_SCRAMBLE_FULL_CHARGED_SCRAMBLE_POINT = 8;		// 풀충전시 쟁탈점수
const DWORD	DBO_DOJO_SCRAMBLE_FULL_GOAL_SCRAMBLE_POINT = 100;		// 쟁탈점수GOAL
const DWORD	DBO_DOJO_SCRAMBLE_FULL_SEAL_POINT = 300;		// 씰충전포인트        
const DWORD	DBO_DOJO_SCRAMBLE_SEC_SEAL_POINT = 2;			// 1초당 씰충전포인트  */

const DWORD	DBO_DOJO_SCRAMBLE_BROADCAST_TERM = 5 * 1000;	// 쟁탈전 점수 방송 텀 
const DWORD DBO_MAX_LEVEL_DOJO_IN_WORLD = 7;

enum eDOJO_STATE
{
	DOJO_STATE_CLOSE,
	DOJO_STATE_OPEN,
	DOJO_STATE_CLEAR,

	MAX_DOJO_STATE,
	INVALID_DOJO_STATE = 0xff,
};

//---------------------------------------------------------------------------------------
struct sDOJO_STATE_INFO
{
	eDOJO_STATE		eState;		// eDOJO_STATE
	DOJOTIME		tmNextStepTime;
	DOJOTIME		tmRemainTime;
};

enum eDBO_DOJO_STATUS
{
	eDBO_DOJO_STATUS_NORMAL = 0,		// 쟁탈전 없는 평소 상태 (쟁탈전 이외 상태)
	eDBO_DOJO_STATUS_RECEIVE,			// 쟁탈전 신청 대기 상태 (신청 접수시작 BroadCast)
	eDBO_DOJO_STATUS_REJECT_END,		// 쟁탈전 거부 종료 상태 (거부가능시간 종료상태 )
	eDBO_DOJO_STATUS_STANDBY,			// 쟁탈전 준비 상태 참여길드에 GUI 표시 (공격방어참여길드한정 알림메세지) 
	eDBO_DOJO_STATUS_INITIAL,			// 쟁탈전 도장 초기화 (도장내 캐릭 도장입구로 NPC삭제 도장내 이동불가 인장생성)		
	eDBO_DOJO_STATUS_READY,				// 쟁탈전 (도장 입장 가능 캐릭이동불가 스킬버프가능 )	
	eDBO_DOJO_STATUS_START,				// 쟁탈전 시작~ ( 인장을 통한 점수 획득 시작 )
	eDBO_DOJO_STATUS_END,				// 쟁탈전 완료 (패자퇴장 승자보상)
};	


struct sDOJO_MATCHSTATE_INFO
{
	eDBO_DOJO_STATUS	eState;		// eDBO_DOJO_STATUS
	DOJOTIME			tmNextStepTime;
	DOJOTIME			tmRemainTime;
};

struct sDBO_DOJO_DATA	
{
public:
	sDBO_DOJO_DATA()
	{
		guildId = INVALID_GUILDID;
		byLevel = 0;
		dojoTblidx = INVALID_TBLIDX;		
		byPeaceStatus = 0;	
		dwPeacePoint = 0;	
		memset( wszName,0x00, sizeof(wszName) );
		challengeGuildId = INVALID_GUILDID;		
		memset( wszSeedCharName,0x00, sizeof(wszSeedCharName) );
		memset( wchLeaderName,0x00, sizeof(wchLeaderName) );
		memset( wchNotice,0x00, sizeof(wchNotice) );
	}
	GUILDID			guildId;
	BYTE			byLevel;											// 도장 레벨
	TBLIDX			dojoTblidx;											// 해당 도장테이블 인덱스
	BYTE			byPeaceStatus;										// 치안상태 eDBO_DOJO_PEACE_TYPE
	DWORD			dwPeacePoint;										// 치안포인트
	WCHAR			wszName[NTL_MAX_SIZE_GUILD_NAME_IN_UNICODE + 1];	// 길드명
	GUILDID			challengeGuildId;									//	도전길드id
	WCHAR			wszSeedCharName[NTL_MAX_SIZE_CHAR_NAME_UNICODE + 1];		// 시드배정캐릭명

	WCHAR			wchLeaderName[NTL_MAX_SIZE_CHAR_NAME_UNICODE + 1];	// ''	공지자
	WCHAR			wchNotice[NTL_MAX_LENGTH_OF_DOJO_NOTICE_UNICODE + 1];	//'' 공지	
};
struct sDOJO_SCRAMBLE_POINT
{
	TBLIDX				dojoTblidx;
	BYTE				byDefCount;		// 방어 인원
	BYTE				byAttCount;		// 공격 인원	
	BYTE				byDefSealCount;	// 방어길드 인장획득갯수
	BYTE				byAttSealCount; // 공격길드 인장획득갯수
	DWORD				dwDefPoint;	// Goal 100
	DWORD				dwAttPoint;	// Goal 100
	DWORD				dwDefCharge;	// 300 -> 8 Point
	DWORD				dwAttCharge;	// 300 -> 8 Point
	
};
struct sDBO_DOJO_BRIEF
{
	GUILDID			guildId;
	TBLIDX			dojoTblidx;
	BYTE			byLevel;
	sDBO_GUILD_MARK sMark;
};

enum eDBO_DOJO_PEACE_TYPE
{
	eDBO_DOJO_PEACE_TYPE_PANIC = 0,
	eDBO_DOJO_PEACE_TYPE_FEAR,
	eDBO_DOJO_PEACE_TYPE_COMMON,
	eDBO_DOJO_PEACE_TYPE_QUIET,
	eDBO_DOJO_PEACE_TYPE_FESTIVAL,
};	

struct sDBO_DOJO_BANK_HISTORY_DATA
{
	sDBO_TIME		sTime;
	BYTE			byType;			// eDBO_GUILD_ZENNY_UPDATE_TYPE
	DWORD			dwAmount;		
	bool			bIsSave;		//	0: withdraw 1:save
	DWORD			dwBalance;		// 잔액
};

struct sDBO_DOJO_BANK_HISTORY_INFO
{
	sDBO_TIME		sTime;
	BYTE			byType;			// eDBO_GUILD_ZENNY_UPDATE_TYPE
	DWORD			dwAmount;		
	bool			bIsSave;		//	0: withdraw 1:save
};

struct sDBO_DOJO_NPC_INFO
{
	TBLIDX				dojoTblidx;										// 도장인덱스
	WCHAR				wszName[NTL_MAX_SIZE_GUILD_NAME_IN_UNICODE + 1];// 유파이름
	BYTE				byLevel;
	DWORD				dwGuildReputation;		// 길드명성
	DWORD				dwMaxGuildPointEver;	// Load 그동안 dwGuildReputation 가장 최고값
	BYTE				byPeaceStatus;	
	WCHAR				wszSeedCharName[NTL_MAX_SIZE_CHAR_NAME_UNICODE + 1];		// 시드배정캐릭명
	WCHAR				wchLeaderName[NTL_MAX_SIZE_CHAR_NAME_UNICODE + 1];		// 리더 캐릭명
	WCHAR				wchNotice[NTL_MAX_LENGTH_OF_DOJO_NOTICE_UNICODE + 1];
	WCHAR				wszChallengeName[NTL_MAX_SIZE_GUILD_NAME_IN_UNICODE + 1];
	WCHAR				wszChallengeLeaderName[NTL_MAX_SIZE_CHAR_NAME_UNICODE + 1];			// 도전유파장
};

struct sDBO_DOJO_TELEPORT_DATA
{
	BYTE								byTeleportType; 
	WORD								wWaitTime;
	WORLDID								destWorldId;
	TBLIDX								destWorldTblidx;
	CNtlVector							vDestDefLoc;
	CNtlVector							vDestAttLoc;
	SERVERCHANNELID						destServerChannelId;
	SERVERINDEX							destServerIndex;
};


//--------------------------
// 출력 Condition

enum eDOJO_PROGRESS_MESSAGE
{
	DOJO_PROGRESS_MESSAGE_NORMAL,			// 쟁탈전 신청전 상태		
	DOJO_PROGRESS_MESSAGE_RECEIVE,			// 쟁탈전 신청 시작
	DOJO_PROGRESS_MESSAGE_REJECT_END,		// 쟁탈전 거부 종료 
	DOJO_PROGRESS_MESSAGE_STANDBY,			// 쟁탈전 준비 상태 참여길드에 GUI 표시 (공격방어참여길드한정 알림메세지) 
	DOJO_PROGRESS_MESSAGE_INITIAL,			// 쟁탈전 도장 초기화 (도장내 캐릭 도장입구로 NPC삭제 도장내 이동불가 인장생성)		
	DOJO_PROGRESS_MESSAGE_READY,			// 쟁탈전 (도장 입장 가능 캐릭이동불가 스킬버프가능 )	
	DOJO_PROGRESS_MESSAGE_START,			// 쟁탈전 시작~ ( 인장을 통한 점수 획득 시작 )
	DOJO_PROGRESS_MESSAGE_END,				// 쟁탈전 완료 (패자퇴장 승자보상)

	INVALID_DOJO_PROGRESS_MESSAGE = 0xff,
};


enum eDBO_DOJO_REPEAT_TYPE
{
	eDBO_DOJO_REPEAT_TYPE_TIME = 0,
	eDBO_DOJO_REPEAT_TYPE_DAY,
	eDBO_DOJO_REPEAT_TYPE_WEEK,
};	

enum eDBO_DOJO_WEEK_BITFLAG
{
	eDBO_DOJO_WEEK_BITFLAG_MON = 0x01 << 0,
	eDBO_DOJO_WEEK_BITFLAG_TUE = 0x01 << 1,
	eDBO_DOJO_WEEK_BITFLAG_WED = 0x01 << 2,
	eDBO_DOJO_WEEK_BITFLAG_THU = 0x01 << 3,
	eDBO_DOJO_WEEK_BITFLAG_FRI = 0x01 << 4,
	eDBO_DOJO_WEEK_BITFLAG_SAT = 0x01 << 5,
	eDBO_DOJO_WEEK_BITFLAG_SUN = 0x01 << 6,
};

enum eDBO_DOJO_PC_TYPE
{
	eDBO_DOJO_PC_TYPE_OWNNER = 0,
	eDBO_DOJO_PC_TYPE_CHALLENGER,

	eDBO_DOJO_PC_TYPE_NONE = 0xff,
};	

enum eDBO_DOJO_REWARD_TYPE
{
	eDBO_DOJO_REWARD_TYPE_PERFECT_DEF = 0,		// 방어 최대 포인트 획득
	eDBO_DOJO_REWARD_TYPE_WIN_DEF,				// 방어 우위
	eDBO_DOJO_REWARD_TYPE_SAME_DEF,				// 방어 점수동일
	
	eDBO_DOJO_REWARD_TYPE_PERFECT_ATT,			// 공격 최대 포인트 획득
	eDBO_DOJO_REWARD_TYPE_WIN_ATT,				// 공격 우위

	eDBO_DOJO_REWARD_TYPE_NONE = 0xff,
};	

enum eDBO_DOJO_COMMAND_TYPE
{
	eDBO_DOJO_COMMAND_TYPE_CLEAR = 0,
	eDBO_DOJO_COMMAND_TYPE_START,
	eDBO_DOJO_COMMAND_TYPE_NEXT,
	eDBO_DOJO_COMMAND_TYPE_RESPONSE,

};	
#pragma pack()