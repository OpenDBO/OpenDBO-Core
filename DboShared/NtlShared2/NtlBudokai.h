//***********************************************************************************
//
//	File		:	NtlBudokai.h
//
//	Begin		:	2008-04-22
//
//	Copyright	:	ⓒ NTL-Inc Co., Ltd
//
//	Author		:	Ju-hyung Lee
//
//	Desc		:	천하제일무도회
//
//***********************************************************************************

#pragma once

#include "NtlParty.h"

// 천하제일무도회의 다음주기를 알아낸다.
BUDOKAITIME CalcBudokaiNextStartTime( BUDOKAITIME tmDefaultTime, BYTE byOpenTerm, BYTE byOpenDayOfWeek, BYTE byOpenHour, BYTE byOpenMinute );


// 천하제일무도회 타입
enum eBUDOKAI_TYPE
{
	BUDOKAI_TYPE_JUNIOR,	// 소년부
	BUDOKAI_TYPE_ADULT,		// 성인부

	INVALID_BUDOKAI_TYPE = 0xff
};


// 천하제일무도회 경기 타입
enum eBUDOKAI_MATCH_TYPE
{
	BUDOKAI_MATCH_TYPE_INDIVIDIAUL,
	BUDOKAI_MATCH_TYPE_TEAM,

	MAX_BUDOKAI_MATCH_TYPE,
	INVALID_BUDOKAI_MATCH_TYPE = 0xff,
};


// 아이콘 공지 타입
enum eBUDOKAI_NOTICE_TYPE
{
	BUDOKAI_NOTICE_SYSTEM,					// 서버에서 테이블 인덱스를 보냄
	BUDOKAI_NOTICE_GM,						// 서버에서 스트링을 보냄
	BUDOKAI_NOTICE_ONLY_CLIENT,				// 클라이언트에서 자체 처리

	INVALID_BUDOKAI_NOTICE_TYPE = 0xff,
};


//
enum eBUDOKAI_STATE
{
	// 소년부
	BUDOKAI_STATE_JUNIOR_CLOSE,
	BUDOKAI_STATE_JUNIOR_OPEN_NOTICE,
	BUDOKAI_STATE_JUNIOR_DOJO_RECOMMEND,
	BUDOKAI_STATE_JUNIOR_OPEN,
	BUDOKAI_STATE_JUNIOR_CLEAR,

	// 성인부
	BUDOKAI_STATE_CLOSE,
	BUDOKAI_STATE_OPEN_NOTICE,		// 시작 예고
	BUDOKAI_STATE_DOJO_RECOMMEND,	// 도장 추천
	BUDOKAI_STATE_OPEN,
	BUDOKAI_STATE_CLEAR,

	MAX_BUDOKAI_STATE,
	BUDOKAI_STATE_JUNIOR_FIRST = BUDOKAI_STATE_JUNIOR_CLOSE,
	BUDOKAI_STATE_JUNIOR_LAST = BUDOKAI_STATE_JUNIOR_CLEAR,
	BUDOKAI_STATE_ADULT_FIRST = BUDOKAI_STATE_CLOSE,
	BUDOKAI_STATE_ADULT_LAST = BUDOKAI_STATE_CLEAR,
	BUDOKAI_STATE_FIRST = BUDOKAI_STATE_JUNIOR_CLOSE,
	BUDOKAI_STATE_LAST = BUDOKAI_STATE_CLEAR,
	INVALID_BUDOKAI_STATE = 0xff,
};

// eBUDOKAI_STATE 가 변경되어야 할 경우 다음 함수도 변경해야 한다.
eBUDOKAI_TYPE GetBudokaiType( BYTE byState );	// eBUDOKAI_STATE

// is open
bool IsBudokaiOpen( BYTE byState );


//
enum eBUDOKAI_MATCHSTATE
{
	BUDOKAI_MATCHSTATE_REGISTER,				// 등록 기간
	BUDOKAI_MATCHSTATE_WAIT_MINOR_MATCH,		// 예선전 대기
	BUDOKAI_MATCHSTATE_MINOR_MATCH,				// 예선전 
	BUDOKAI_MATCHSTATE_WAIT_MAJORMATCH_32,
	BUDOKAI_MATCHSTATE_MAJORMATCH_32,
	BUDOKAI_MATCHSTATE_WAIT_MAJORMATCH_16,
	BUDOKAI_MATCHSTATE_MAJORMATCH_16,
	BUDOKAI_MATCHSTATE_WAIT_MAJORMATCH_08,
	BUDOKAI_MATCHSTATE_MAJORMATCH_08,
	BUDOKAI_MATCHSTATE_WAIT_SEMIFINAL_MATCH,	// 준결승 대기
	BUDOKAI_MATCHSTATE_SEMIFINAL_MATCH,			// 준결승
	BUDOKAI_MATCHSTATE_WAIT_FINAL_MATCH,		// 결승 대기 : 사용하지 않음
	BUDOKAI_MATCHSTATE_FINAL_MATCH,				// 결승
	BUDOKAI_MATCHSTATE_MATCH_END,				// 끝. 유예시간

	MAX_BUDOKAI_MATCHSTATE,
	INVALID_BUDOKAI_MATCHSTATE = 0xff,
};


// player 의 참가 상태
enum eBUDOKAI_JOIN_STATE
{
	BUDOKAI_JOIN_STATE_PLAY = 0,		// 참가중 - DB와 맞추어진 초기값이므로 변경되어서는 안된다.
	BUDOKAI_JOIN_STATE_DROPOUT,			// 탈락자 - DB와 맞추어진 값이므로 변경되어서는 안된다.

	INVALID_BUDOKAI_JOIN_STATE = 0xff,	// 참가 안함
};


// player 의 참가 결과
enum eBUDOKAI_JOIN_RESULT
{
	BUDOKAI_JOIN_RESULT_REGISTER = 0,	// 등록중 - DB와 맞추어진 초기값이므로 변경되어서는 안된다.
	BUDOKAI_JOIN_RESULT_MINORMATCH,		// 예선진출 - DB와 맞추어진 값이므로 변경되어서는 안된다.
	BUDOKAI_JOIN_RESULT_ENTER_32,
	BUDOKAI_JOIN_RESULT_ENTER_16,
	BUDOKAI_JOIN_RESULT_ENTER_8,
	BUDOKAI_JOIN_RESULT_ENTER_4,
	BUDOKAI_JOIN_RESULT_ENTER_2,
	BUDOKAI_JOIN_RESULT_WINNER,

	INVALID_BUDOKAI_JOIN_RESULT = 0xff
};


enum eBUDOKAI_MATCH_DEPTH
{
	BUDOKAI_MATCH_DEPTH_2 = 0,	// 1	2^0
	BUDOKAI_MATCH_DEPTH_4,		// 2	2^1
	BUDOKAI_MATCH_DEPTH_8,		// 4	2^2
	BUDOKAI_MATCH_DEPTH_16,		// 8	2^3
	BUDOKAI_MATCH_DEPTH_32,		// 16	2^4

	BUDOKAI_MATCH_DEPTH_COUNT,
	MAX_BUDOKAI_MATCH_DEPTH_INDIVIDUAL = BUDOKAI_MATCH_DEPTH_32,
	MAX_BUDOKAI_MATCH_DEPTH_TEAM = BUDOKAI_MATCH_DEPTH_16,
	INVALID_BUDOKAI_MATCH_DEPTH = 0xff,
};


//--------------------------
// 출력 Condition

enum eBUDOKAI_PROGRESS_MESSAGE
{
	BUDOKAI_PROGRESS_MESSAGE_START = 0,					// 천하제일무도회가 시작됩니다. 소식지....

	// 예선
	BUDOKAI_PROGRESS_MESSAGE_MINORMATCH_ENTERING,		// 예선 진입 시작합니다. 입장을 시작하....
	BUDOKAI_PROGRESS_MESSAGE_MINORMATCH_BEFORE_1MIN,	// 곧 예선 전투가 시작됩니다. 입장을 서...
	BUDOKAI_PROGRESS_MESSAGE_MINORMATCH_WINNER,			// OOO님의 ... 32강 진출이 확정 되셨습....
	BUDOKAI_PROGRESS_MESSAGE_MINORMATCH_LOSER,			// OOO님의 ... 안타깝게도 32강에 진출하...
	BUDOKAI_PROGRESS_MESSAGE_MINORMATCH_ALL_FINISHED,	// 예선이 모두 종료되었습니다. 본선 진....

	// 본선
	BUDOKAI_PROGRESS_MESSAGE_DEPTH_32_BEFORE_10SEC,		// 10초 후 32강 경기장으로 자동 진입합니다.
	BUDOKAI_PROGRESS_MESSAGE_DEPTH_32_WINNER,			// OOO님의 32강.... 축하합니다. 16강진출이.
	BUDOKAI_PROGRESS_MESSAGE_DEPTH_32_LOSER,			// OOO님의 32강.... 안타깝게도, 16강에 준..
	BUDOKAI_PROGRESS_MESSAGE_DEPTH_32_ALL_FINISHED,		// 32강이 모두 종료.... 16강 진출자가 모...

	BUDOKAI_PROGRESS_MESSAGE_DEPTH_16_BEFORE_10SEC,		// 10초 후 16강 경기장으로 자동 진입합니다.
	BUDOKAI_PROGRESS_MESSAGE_DEPTH_16_WINNER,			// OOO님의 16강.... 축하합니다. 16강진출이.
	BUDOKAI_PROGRESS_MESSAGE_DEPTH_16_LOSER,			// OOO님의 16강.... 안타깝게도, 16강에 준..
	BUDOKAI_PROGRESS_MESSAGE_DEPTH_16_ALL_FINISHED,		// 16강이 모두 종료.... 8강 진출자가 모...

	BUDOKAI_PROGRESS_MESSAGE_DEPTH_08_BEFORE_10SEC,		// 10초 후 8강 경기장으로 자동 진입합니다.
	BUDOKAI_PROGRESS_MESSAGE_DEPTH_08_WINNER,			// OOO님의 8강.... 축하합니다. 8강진출이.
	BUDOKAI_PROGRESS_MESSAGE_DEPTH_08_LOSER,			// OOO님의 8강.... 안타깝게도, 8강에 준..
	BUDOKAI_PROGRESS_MESSAGE_DEPTH_08_ALL_FINISHED,		// 8강이 모두 종료... 준결승 진출자가 모...

	//
	BUDOKAI_PROGRESS_MESSAGE_SEMI_FINAL_BEFORE_10SEC,
	BUDOkAI_PROGRESS_MESSAGE_SEMI_FINAL_RUNNING,
	BUDOkAI_PROGRESS_MESSAGE_FINAL_WAIT_LOSER,

	BUDOkAI_PROGRESS_MESSAGE_END,

	INVALID_BUDOKAI_PROGRESS_MESSAGE = 0xff,
};


//------------------------------------------------------------------------------------
// define

//-----------------------------------------------
// [8/4/2008 SGpro]
#define INVALID_MUDOSAPOINT INVALID_DWORD 

#define MAX_MUDOSA_POINT				(500000)

#define BUDOKAI_SERVER_INDEX			0


#define BUDOKAI_MAX_NOTICE_LENGTH		255


// Game <-> Query 사이에서 한 패킷에 들어갈 수 있는 최대 갯수
#define BUDOKAI_MAX_INDIVIDUAL_DB_DATA_PACKET_COUNT	20
#define BUDOKAI_MAX_TEAM_DB_DATA_PACKET_COUNT		(int)(BUDOKAI_MAX_INDIVIDUAL_DB_DATA_PACKET_COUNT / NTL_MAX_MEMBER_IN_PARTY)


// 프로토콜 관련
#define	BUDOKAI_MAX_TOURNAMENT_INDIVIDUAL_ENTRY_PACKET_COUNT	(NTL_MAX_MEMBER_IN_PARTY * 4)
#define	BUDOKAI_MAX_TOURNAMENT_TEAM_ENTRY_PACKET_COUNT			4



//-----------------------------------------------
// Rule define

// 최대 예선전 신청 인원 및 팀 인원
#define BUDOKAI_MAX_INDIVIDUAL_REGISTER_COUNT		10000
#define BUDOKAI_MAX_TEAM_REGISTER_COUNT				2000

// 최소, 최대 팀 인원
#define BUDOKAI_MIN_TEAM_MEMBER_COUNT	1
#define BUDOKAI_MAX_TEAM_MEMBER_COUNT	NTL_MAX_MEMBER_IN_PARTY;

// 예선전 경기수, 본선 ??강
#define	BUDOKAI_MAX_INDIVIDUAL_MATCH_COUNT			32
#define	BUDOKAI_MAX_TEAM_MATCH_COUNT				16

#if BUDOKAI_MAX_INDIVIDUAL_MATCH_COUNT > BUDOKAI_MAX_TEAM_MATCH_COUNT
#define BUDOKAI_MAX_MATCH_COUNT		BUDOKAI_MAX_INDIVIDUAL_MATCH_COUNT
#else
#define BUDOKAI_MAX_MATCH_COUNT		BUDOKAI_MAX_TEAM_MATCH_COUNT
#endif


// 예선 경기당 인원 및 팀
#define BUDOKAI_MINOR_MATCH_INDIVIDUAL_COUNT		15
#define BUDOKAI_MINOR_MATCH_TEAM_COUNT				5

#if BUDOKAI_MINOR_MATCH_INDIVIDUAL_COUNT > BUDOKAI_MINOR_MATCH_TEAM_COUNT
#define BUDOKAI_MAX_MINORMATCH_TEAM_COUNT		BUDOKAI_MINOR_MATCH_INDIVIDUAL_COUNT
#else
#define BUDOKAI_MAX_MINORMATCH_TEAM_COUNT		BUDOKAI_MINOR_MATCH_TEAM_COUNT
#endif


// 총 예선 선발 인원
#define BUDOKAI_MINOR_MATCH_MAX_INDIVIDUAL_COUNT	(BUDOKAI_MINOR_MATCH_INDIVIDUAL_COUNT * BUDOKAI_MAX_INDIVIDUAL_MATCH_COUNT)
#define BUDOKAI_MINOR_MATCH_MAX_TEAM_COUNT			(BUDOKAI_MINOR_MATCH_TEAM_COUNT * BUDOKAI_MAX_TEAM_MATCH_COUNT)
#define BUDOKAI_MINOR_MATCH_MAX_TEAM_MEMBER_COUNT	(BUDOKAI_MINOR_MATCH_MAX_TEAM_COUNT * NTL_MAX_MEMBER_IN_PARTY)


// 개인전 본선 진출 인원 : 변경할 경우 QG_BUDOKAI_INDIVIDUAL_LIST_RES 패킷의 크기가 넘어가지 않는지 체크한다.
#define BUDOKAI_MAJOR_MATCH_PLAYER_NUMBER			(BUDOKAI_MAX_INDIVIDUAL_MATCH_COUNT)


// 팀전 본선 진출 인원 : 변경할 경우 QG_BUDOKAI_TEAM_LIST_RES 패킷의 크기가 넘어가지 않는지 체크한다.
#define BUDOKAI_MAJOR_MATCH_TEAM_NUMBER				(BUDOKAI_MAX_TEAM_MATCH_COUNT)


// 예선전 자동 부활 시간(초)
#define BUDOKAI_MINORMATCH_RESCUE_TIME				10

// 예선전 STAGE SELECTION state 시간
#define BUDOKAI_MINORMATCH_SELECTION_STATE_TIME		20

// Stage Selection 에서 굴리는 Random Number의 크기
#define BUDOKAI_MINORMATCH_SELECTION_NUMBER			100


//-----------------------------------------------
// Mudosa

// 무도사 정보 최대 개수
#define BUDOKAI_MAX_MUDOSA_INFO_COUNT				5

// 무도사 최대 수용 인원
#define BUDOKAI_MAX_MUDOSA_PLAYER_COUNT				200

// 무도사 최대 이름 길이 : DBO_MAX_LENGTH_WORLD_NAME_IN_UNICODE/
#define BUDOKAI_MAX_MUDOSA_NAME_IN_UNICODE			32




// 최대 토너먼트 개인전 대전 횟수
#define BUDOKAI_MAX_TOURNAMENT_INDIVIDUAL_MATCH_COUNT	(BUDOKAI_MAJOR_MATCH_PLAYER_NUMBER - 1)

// 최대 토너먼트 팀 대전 횟수
#define BUDOKAI_MAX_TOURNAMENT_TEAM_MATCH_COUNT			(BUDOKAI_MAJOR_MATCH_TEAM_NUMBER - 1)


// FinalMatch Max Team Count
#define BUDOKAI_MAX_FINALMATCH_TEAM_COUNT			4



// DB 쿼리 관련
// 토너먼트 개인전 결과 최대 레코드 카운트
#define BUDOKAI_MAX_TOURNAMENT_INDIVIDUAL_MATCH_RECORD_COUNT	(BUDOKAI_MAJOR_MATCH_PLAYER_NUMBER * 2)
// 토너먼트 팀전 결과 최대 레코드 카운트
#define BUDOKAI_MAX_TOURNAMENT_TEAM_MATCH_RECORD_COUNT			(BUDOKAI_MAJOR_MATCH_TEAM_NUMBER * 2)



//------------------------------------------------------------------------------------
// history 관련

// 개인전 입상자 카운트 : 결선 4명
#define	BUDOKAI_MAX_SEASON_HISTORY_INDIVIAUL_WINNER_COUNT		(4)
// 팀전 입상자 카운트 : 결선 4개팀 20명
#define	BUDOKAI_MAX_SEASON_HISTORY_TEAM_WINNER_COUNT			(NTL_MAX_MEMBER_IN_PARTY * 4)

// 개인전 및 팀전 입상자 카운트
#define	BUDOKAI_MAX_SEASON_HISTORY_WINNER_PLAYER_COUNT			(BUDOKAI_MAX_SEASON_HISTORY_INDIVIAUL_WINNER_COUNT + BUDOKAI_MAX_SEASON_HISTORY_TEAM_WINNER_COUNT)


// join state를 보낼때 한패킷에 넣을수 있는 최대 갯수
const BYTE BUDOkAI_MAX_PLAYER_JOIN_STATE_DATA_COUNT = 100;


//------------------------------------------------------------------------------------


#pragma pack(1)


// 상태 정보 : DB 저장용
struct sBUDOKAI_STATE_DATA
{
	BYTE		byState;		// eBUDOKAI_STATE or eBUDOKAI_MATCHSTATE
	BUDOKAITIME	tmNextStepTime;
};

#define sBUDOKAI_MATCHSTATE_DATA sBUDOKAI_STATE_DATA	


struct sBUDOKAI_UPDATEDB_INITIALIZE
{
	WORD						wSeasonCount;				// 몇회의 천하제일 무도회 인지?
	BUDOKAITIME					tmDefaultOpenTime;			// 기본 시작시간

	bool						bRankPointInitialized;

	sBUDOKAI_STATE_DATA			sStateData;
	sBUDOKAI_MATCHSTATE_DATA	sIndividualStateData;
	sBUDOKAI_MATCHSTATE_DATA	sTeamStateData;
};

//-----------------------------------------------------------------------------------


// 상태 정보 : 사용
struct sBUDOKAI_STATE_INFO
{
	eBUDOKAI_STATE	eState;		// eBUDOKAI_STATE
	BUDOKAITIME		tmNextStepTime;
//	BUDOKAITIME		tmRemainTime;
};


//
struct sBUDOKAI_MATCHSTATE_INFO
{
	eBUDOKAI_MATCHSTATE	eState;		// eBUDOKAI_MATCHSTATE
	BUDOKAITIME			tmNextStepTime;
//	BUDOKAITIME			tmRemainTime;
};


//---------------------------------------------------------------------------------------

// 천하제일무도회에 신청한 player의 개인 정보정보
struct sBUDOKAI_PLAYER_DATA
{
//	sBUDOKAI_PLAYER_DATA() : charId(INVALID_CHARACTERID), byClass(INVALID_BYTE), byLevel(INVALID_BYTE), fPoint(0)
//	{
//		memset( wszName, 0x00, sizeof(wszName));
//	}

	CHARACTERID	charId;
	BYTE		byClass;
	BYTE		byLevel;
	float		fPoint;
	WCHAR		wszName[NTL_MAX_SIZE_CHAR_NAME_UNICODE + 1];
};



//---------------------------------------------------------------------------------------
// 예선 진출자 리스트

//
struct sBUDOKAI_REGISTER_DATA
{
	JOINID			wJoinId;
	WORD			wRanking;
	BYTE			byMinorMatch_MatchIndex;
	float			fPoint;
};

// DB의 개인 Data : 변경할 경우 QG_BUDOKAI_INDIVIDUAL_LIST_DATA 패킷의 크기가 넘어가지 않는지 확인한다.
struct sBUDOKAI_REGISTER_INDIVIDUAL_DATA : public sBUDOKAI_REGISTER_DATA
{
	CHARACTERID		charId;
	bool			bDojoRecommender;
};

// DB의 개인 Data : 변경할 경우 QG_BUDOKAI_TEAM_LIST_RES 패킷의 크기가 넘어가지 않는지 체크한다.
struct sBUDOKAI_REGISTER_TEAM_DATA : public sBUDOKAI_REGISTER_DATA
{
	WCHAR					wszTeamName[NTL_MAX_LENGTH_BUDOKAI_TEAM_NAME_IN_UNICODE + 1];
	CHARACTERID				aMembers[NTL_MAX_MEMBER_IN_PARTY];
};


//---------------------------------------------------------------------------------------
// 본선 진출자 리스트

struct sBUDOKAI_TOURNAMENT_ENTRY_DATA
{
	JOINID			wJoinId;
	BYTE			byMinorMatch_MatchIndex;
};


struct sBUDOKAI_TOURNAMENT_INDIVIDUAL_ENTRY_DATA : public sBUDOKAI_TOURNAMENT_ENTRY_DATA
{
	CHARACTERID		charId;
	WCHAR			wszCharName[NTL_MAX_SIZE_CHAR_NAME_UNICODE + 1];
	BYTE			byClass;
	BYTE			byLevel;
	float			fRankPoint;
	bool			bDojoRecommender;
	WORD			wRankBattleWinCount;
	WORD			wRankBattleDrawCount;
	WORD			wRankBattleLoseCount;
	WCHAR			wszGuildName[NTL_MAX_SIZE_GUILD_NAME_IN_UNICODE + 1];
};


struct sBUDOKAI_TOURNAMENT_TEAM_ENTRY_MEMBER_DATA
{
	CHARACTERID		charId;
	WCHAR			wszCharName[NTL_MAX_SIZE_CHAR_NAME_UNICODE + 1];
	BYTE			byClass;
	BYTE			byLevel;
	float			fRankPoint;
};

struct sBUDOKAI_TOURNAMENT_TEAM_ENTRY_DATA : public sBUDOKAI_TOURNAMENT_ENTRY_DATA
{
	WCHAR			wszTeamName[NTL_MAX_LENGTH_BUDOKAI_TEAM_NAME_IN_UNICODE + 1];
	BYTE			byMemberCount;
	sBUDOKAI_TOURNAMENT_TEAM_ENTRY_MEMBER_DATA	aMembers[NTL_MAX_MEMBER_IN_PARTY];
};


// variable data ----

struct sBUDOKAI_TOURNAMENT_INDIVIDUAL_ENTRY_DATA_VAR : sBUDOKAI_TOURNAMENT_ENTRY_DATA
{
	CHARACTERID		charId;
	WORD			wCharName;				//WCHAR			wszCharName[NTL_MAX_SIZE_CHAR_NAME_UNICODE + 1];
	BYTE			byClass;
	BYTE			byLevel;
	float			fRankPoint;
	bool			bDojoRecommender;		// 도장 추천자
	WORD			wRankBattleWinCount;
	WORD			wRankBattleDrawCount;
	WORD			wRankBattleLoseCount;
	WORD			wGuildName;				//WCHAR			wszGuildName[NTL_MAX_SIZE_GUILD_NAME_IN_UNICODE + 1];
};


struct sBUDOKAI_TOURNAMENT_TEAM_ENTRY_MEMBER_DATA_VAR
{
	CHARACTERID		charId;
	WORD			wCharName;				//WCHAR			wszCharName[NTL_MAX_SIZE_CHAR_NAME_UNICODE + 1];
	BYTE			byClass;
	BYTE			byLevel;
	float			fRankPoint;
};

struct sBUDOKAI_TOURNAMENT_TEAM_ENTRY_DATA_VAR : public sBUDOKAI_TOURNAMENT_ENTRY_DATA
{
	WORD			wTeamName;				//WCHAR			wszTeamName[NTL_MAX_LENGTH_BUDOKAI_TEAM_NAME_IN_UNICODE + 1];
	BYTE			byMemberCount;
	WORD			wMembers;				//sBUDOKAI_TOURNAMENT_TEAM_ENTRY_MEMBER_DATA	aMembers[NTL_MAX_MEMBER_IN_PARTY];
};


//---------------------------------------------------------------------------------------
// Game Server <-> Client



// 팀 신청할때 신청 팀의 정보 요청(신천 전의 정보를 출력하기 위함)
struct sBUDOKAI_TEAM_POINT_INFO
{
	BYTE		byClass;
	WCHAR		wszName[NTL_MAX_SIZE_CHAR_NAME_UNICODE + 1];
	BYTE		byLevel;
	float		fPoint;
};


//
struct sBUDOKAI_REGISTER_INDIVIDUAL_INFO
{
	bool			bDojoRecommender;	// 도장추천자인지
	JOINID			wJoinId;
	WCHAR			wszCharName[NTL_MAX_SIZE_CHAR_NAME_UNICODE + 1];
};


//
struct sBUDOKAI_REGISTER_TEAM_INFO
{
	JOINID			wJoinId;
	WCHAR			wszTeamName[NTL_MAX_LENGTH_BUDOKAI_TEAM_NAME_IN_UNICODE + 1];

	sBUDOKAI_TEAM_POINT_INFO	aMemberInfo[NTL_MAX_MEMBER_IN_PARTY];
};


// 접속시 클라이언트에게 보내주는 개인 정보
struct sBUDOKAI_JOIN_INFO
{
	BYTE					byMatchType;		// eBUDOKAI_MATCH_TYPE
	BYTE					byJoinState;		// 참가 상태
	BYTE					byJoinResult;		// 참가 결과

	union
	{
		sBUDOKAI_REGISTER_INDIVIDUAL_INFO	sIndividualInfo;
		sBUDOKAI_REGISTER_TEAM_INFO			sTeamInfo;
	};
};



//-----------------------
// 본선 진출자 리스트


struct sBUDOKAI_TOURNAMENT_ENTRY_TEAM_INFO
{
	JOINID	wJoinId;
	bool	bDojoRecommender;
	WCHAR	wszTeamName[NTL_MAX_LENGTH_BUDOKAI_TEAM_NAME_IN_UNICODE + 1];
};


struct sBUDOKAI_TOURNAMENT_ENTRY_TEAM_INFO_VAR
{
	JOINID	wJoinId;
	bool	bDojoRecommender;
	WORD	wTeamName;		// WCHAR	wszTeamName[NTL_MAX_LENGTH_BUDOKAI_TEAM_NAME_IN_UNICODE + 1];
};



//-----------------------

struct sBUDOKAI_TOURNAMENT_MATCH_DATA
{
	BYTE	byDepth;
	BYTE	byMatchNumber;
	BYTE	byMatchResult;		// eMATCH_RESULT
	JOINID	wWinnerTeam;		// Winner Team
	JOINID	wJoinId1;
	JOINID	wJoinId2;
	BYTE	byScore1;
	BYTE	byScore2;
};


//---------------------------------------------------------------------------------------
struct sBUDOKAI_UPDATE_STATE_INFO
{
	BYTE			byState;		// eBUDOKAI_STATE
	BUDOKAITIME		tmNextStepTime;
	BUDOKAITIME		tmRemainTime;
	WORD			wSeasonCount;
};

struct sBUDOKAI_UPDATE_MATCH_STATE_INFO
{
	BYTE			byState;		// eBUDOKAI_MATCHSTATE
	BUDOKAITIME		tmNextStepTime;
	BUDOKAITIME		tmRemainTime;
};


//---------------------------------------------------------------------------------------

struct sBUDOKAI_MUDOSA_INFO
{
	WCHAR			wszMudosaName[BUDOKAI_MAX_MUDOSA_NAME_IN_UNICODE + 1];
	WORD			wCurrentUserCount;
};


// 천하제일무도회 이전 시즌 경기자의 정보
struct sBUDOKAI_PREV_SEASON_PLAYER_INFO
{
	BYTE			byMatchType;		// eBUDOKAI_MATCH_TYPE
	CHARACTERID		charId;
	WCHAR			wszName[NTL_MAX_SIZE_CHAR_NAME_UNICODE + 1];
	BYTE			byClass;
	BYTE			byLevel;
	BYTE			byJoinResult;		// eBUDOKAI_JOIN_RESULT
};


//---------------------------------------------------------------------------------------
// 천하제일무도회 서버가 일반 서버로 알려주는 예선전 참가자 정보

// 예선 개인전 텔레포트 정보
struct sMINORMATCH_TELEPORT_INDIVIDUAL_DATA
{
	WORLDID		worldId;
	TBLIDX		worldTblidx;
	BYTE		byPlayerCount;
	CHARACTERID	aPlayers[BUDOKAI_MINOR_MATCH_INDIVIDUAL_COUNT];
};


// 예선 팀전 텔레포트 정보
struct sMINORMATCH_TELEPORT_TEAM_MEMBER_LIST
{
	BYTE		byCount;
	CHARACTERID	charId[NTL_MAX_MEMBER_IN_PARTY];
};

struct sMINORMATCH_TELEPORT_TEAM_DATA
{
	WORLDID									worldId;
	TBLIDX									worldTblidx;
	BYTE									byTeamCount;
	sMINORMATCH_TELEPORT_TEAM_MEMBER_LIST	aTeamList[BUDOKAI_MINOR_MATCH_TEAM_COUNT];
};



enum eBUDOKAI_GM_MATCH_PROGRESS_STATE
{
	BUDOKAI_GM_MATCH_PROGRESS_STATE_READY,		// 준비중 default
	BUDOKAI_GM_MATCH_PROGRESS_STATE_RUN,		// 진행중
	BUDOKAI_GM_MATCH_PROGRESS_STATE_FINISHED,	// 완료
};


// GM 정보 출력
struct sBUDOKAI_GM_MATCH_PROGRESS_STATE
{
	BYTE		byMatchIndex;
	BYTE		byProgressState;		// eBUDOKAI_GM_MATCH_PROGRESS_STATE

	BYTE		byMatchResult;			// eMATCH_RESULT
	TEAMTYPE	winnerTeamType;			// eMATCH_TEAM_TYPE

	// Team1
	WCHAR		wszTeamName1[NTL_MAX_LENGTH_BUDOKAI_TEAM_NAME_IN_UNICODE + 1];
	BYTE		byScore1;

	// Team2
	WCHAR		wszTeamName2[NTL_MAX_LENGTH_BUDOKAI_TEAM_NAME_IN_UNICODE + 1];
	BYTE		byScore2;
};


// 예선전 동점자 추첨 관련 데이터
struct sMINORMATCH_SELECTION_DATA
{
	TEAMTYPE	teamType;
	BYTE		byNumber;
};


struct sBUDOKAI_JOIN_STATE_DATA
{
	CHARACTERID				charId;
	JOINID					joinId;
	BYTE					byJoinState;	// eBUDOKAI_JOIN_STATE
	BYTE					byJoinResult;	// eBUDOKAI_JOIN_RESULT
};

#pragma pack()

