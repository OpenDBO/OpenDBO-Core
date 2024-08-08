//***********************************************************************************
//
//	File		:	NtlRankBattle.h
//
//	Begin		:	2007-05-31
//
//	Copyright	:	ⓒ NTL-Inc Co., Ltd
//
//	Author		:	
//
//	Desc		:	
//
//***********************************************************************************

#pragma once

#include "NtlSharedType.h"
#include "NtlSharedDef.h"
#include "NtlParty.h"

//-----------------------------------------------------------------------------------
// Enumeration
//-----------------------------------------------------------------------------------

// Room State
enum eRANKBATTLE_ROOMSTATE
{
	RANKBATTLE_ROOMSTATE_NONE,			// 초기
	RANKBATTLE_ROOMSTATE_WAIT,			// 대전상대를 기다림
	RANKBATTLE_ROOMSTATE_MATCH_READY,	// 대전 준비중
	RANKBATTLE_ROOMSTATE_WAIT_MATCH_WORLD,	// 경기장이 생성되기를 기다림
	RANKBATTLE_ROOMSTATE_MATCH_RUN,		// 대전 시작
	RANKBATTLE_ROOMSTATE_MATCH_END,		// 대전 종료중 -> 이후 다시 WAIT 상태로 변경됨

	RANKBATTLE_ROOMSTATE_COUNT,
	INVALID_RANKBATTLE_ROOMSTATE = INVALID_BYTE,
};


// 랭크배틀 모드
enum eRANKBATTLE_MODE
{
	RANKBATTLE_MODE_INDIVIDUAL, // 개인전
	RANKBATTLE_MODE_PARTY, // 파티전

	MAX_RANKBATTLE_MODE,
	INVALID_RANKBATTLE_MODE = INVALID_BYTE
};

enum eRANKBATTLE_TEAM_TYPE
{
	RANKBATTLE_TEAM_NONE		= 0,
	RANKBATTLE_TEAM_OWNER		= 0x01 << 0,
	RANKBATTLE_TEAM_CHALLENGER	= 0x01 << 1,
	RANKBATTLE_TEAM_OTHER		= 0x01 << 2,				// 팀이 아닌경우, ex.GM

	RANKBATTLE_TEAM_ALL			= RANKBATTLE_TEAM_OWNER | RANKBATTLE_TEAM_CHALLENGER,		// RANKBATTLE_TEAM_OTHER 는 제외
};


// 랭크배틀 상태 모드
enum eRANKBATTLE_BATTLESTATE
{
	RANKBATTLE_BATTLESTATE_NONE,			// 초기 상태
	RANKBATTLE_BATTLESTATE_WAIT,			// 대전자들을 기다림
	RANKBATTLE_BATTLESTATE_DIRECTION,		// 연출
	RANKBATTLE_BATTLESTATE_STAGE_READY,		// 스테이지 준비
	RANKBATTLE_BATTLESTATE_STAGE_RUN,		// 스테이지 진행
	RANKBATTLE_BATTLESTATE_STAGE_FINISH,	// 스테이지 종료
	RANKBATTLE_BATTLESTATE_MATCH_FINISH,	// 경기 종료
	RANKBATTLE_BATTLESTATE_BOSS_DIRECTION,	// BOSS 연출
	RANKBATTLE_BATTLESTATE_BOSS_READY,		// BOSS 준비
	RANKBATTLE_BATTLESTATE_BOSS_RUN,		// BOSS 진행
	RANKBATTLE_BATTLESTATE_BOSS_FINISH,		// BOSS 종료
	RANKBATTLE_BATTLESTATE_END,				// 종료

	MAX_RANKBATTLE_BATTLESTATE,
	INVALID_RANKBATTLE_BATTLESTATE = INVALID_BYTE
};


// Member 의 상태
enum eRANKBATTLE_MEMBER_STATE
{
	RANKBATTLE_MEMBER_STATE_NONE,			// 초기 상태
	RANKBATTLE_MEMBER_STATE_NORMAL,			// 일반 상태
	RANKBATTLE_MEMBER_STATE_FAINT,			// 죽음
	RANKBATTLE_MEMBER_STATE_OUTOFAREA,		// 장외
	RANKBATTLE_MEMBER_STATE_GIVEUP,			// 포기상태(접속 종료 등)
	RANKBATTLE_MEMBER_STATE_NOAPPLY,		// PC 정보는 있으나 경기 설정 및 판정에 적용하지 않는다.
};


// Match Result
enum eRANKBATTLE_MATCH_RESULT
{
	RANKBATTLE_MATCH_NONE,				// 예외 상황 : 모두 경기장을 나간 경우등.. 전적을 - 시킨다.
	RANKBATTLE_MATCH_WIN_OWNER,		// 주인 승
	RANKBATTLE_MATCH_WIN_CHALLENGER,	// 도전자 승
	RANKBATTLE_MATCH_DRAW,				// 비김

	INVALID_RANKBATTLE_MATCH = INVALID_BYTE,
};



// 쿼리서버에서 RankBattle Score를 Update 할때 사용
enum eRANKBATTLE_QUERY_UPDATE_TYPE
{
	RANKBATTLE_QUERY_UPDATE_NONE,		// 그대로 둔다,(업데이트 하지 않는다.)
	RANKBATTLE_QUERY_UPDATE_ADD,		// 1 추가한다
	RANKBATTLE_QUERY_UPDATE_INIT,		// 0으로 초기화 시킨다.
};


//-----------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------
//	상수 정의
//-----------------------------------------------------------------------------------
const DWORD		DBO_RANKBATTLE_MAX_ROOM_COUNT = 100;			// 랭크배틀 맵당 방의 최대 개수

const DWORD		DBO_RANKBATTLE_MAX_ROOM_COUNT_PER_PAGE = 10;	// 한페이지당 출력할 최대 방의 개수 (서버전송)

const DWORD		DBO_RANKBATTLE_USER_WAIT_TIME = 60000;			// 유저 대기 1분

const DWORD		DBO_RANKBATTLE_NAME_LENGTH = 128;				// RankBattle의 이름

const BYTE		DBO_RANKBATTLE_PARTY_MEMBER_RESTRICTION = 1;	// 파티인원 제한

const DWORD		DBO_RANKBATTLE_ROOM_READY_TIME = 10000;			// 대전 시작 알림 후 대전까지의 대기 시간

const DWORD		DBO_RANKBATTLE_ENDSTAGE_WAIT_TIME = 5000;		// End Stage 에서 대기 시간

const BYTE		DBO_RANKBATTLE_MAX_RANK_BOARD_RECORD_COUNT = 10;	// Rank 게시판에 출력되는 최대 레코드 카운트

const BYTE		DBO_RANKBATTLE_MAX_COMPARE_DAY = 7;					// Rank 게시판에서 비교할수 있는 최대 일수

const DWORD		DBO_RANKBATTLE_RANK_REQUEST_TERM = 500;		// 2초에 한번씩 Rank list 요청을 할 수 있다.

const BYTE		DBO_RANKBATTLE_RANK_BACKUP_TIME = 6;			// Rank를 Backup하는 시간 0~23, * 주의 DB의 스케줄과 같이 업데이트 해야 한다. *


const WORD		DBO_RANKBATTLE_MAX_ARENA_TYPE = 10;				// 아레나 월드의 최대 갯수 개인전,파티전 각각 10개

const DWORD		DBO_RANKBATTLE_MATCH_START_TERM = 60;			// 경기가 시작하는 주기

const BYTE		DBO_RANKBATTLE_MAX_MATCHING_LEVEL = 1;			// 경기를 시작할 수 있는 각 PC 및 Party 간의 최대 레벨 차이

//-----------------------------------------------------------------------------------



//-----------------------------------------------------------------------------------
//	자료 구조 정의
//-----------------------------------------------------------------------------------
#pragma pack(1)

struct sRANKBATTLE_ARENA_INFO
{
	TBLIDX		rankBattleTblidx;
	WORD		wRegisterCount;
};



// 개인전 방 정보
struct sRANKBATTLE_ROOMLIST_INDIVIDUAL
{
	ROOMID	roomId;
	WCHAR	wszOwnerPc[NTL_MAX_SIZE_CHAR_NAME_UNICODE + 1];	// 주인
	WORD	wWinCounter;
};



// 파티전 방 정보
struct sRANKBATTLE_ROOMLIST_PARTY
{
	ROOMID	roomId;
	WCHAR	wszOwnerParty[NTL_MAX_SIZE_PARTY_NAME_IN_UNICODE + 1];	// 주인
	WORD	wWinCounter;
};


//--------------------------------------------------------------------------------
// 방 상세 정보

// 방 상세 정보 전송에 사용하는 Member의 정보
struct sRANKBATTLE_ROOMINFO_MEMBER
{
	WCHAR	wszName[NTL_MAX_SIZE_CHAR_NAME_UNICODE + 1];	//
	BYTE	byClass;
	BYTE	byLevel;
};


// 방 상세 정보 전송에 사용하는 개인 방 정보
struct sRANKBATTLE_ROOMINFO_INDIVIDUAL
{
	TBLIDX						rankBattleTblidx;	// RankBattle Tblidx
	ROOMID						roomId;
	WORD						wWinCount;

	sRANKBATTLE_ROOMINFO_MEMBER	sMember;			// Member
};


// 방 상세 정보 전송에 사용하는 파티 방 정보
struct sRANKBATTLE_ROOMINFO_PARTY
{
	TBLIDX						rankBattleTblidx;	// RankBattle Tblidx
	ROOMID						roomId;
	WORD						wWinCount;
	WCHAR						wszPartyName[NTL_MAX_SIZE_PARTY_NAME_IN_UNICODE + 1];

	BYTE						byMemberCount;		// Room의 MemberCount
	sRANKBATTLE_ROOMINFO_MEMBER	asMember[NTL_MAX_MEMBER_IN_PARTY];		// Member
};


// 
union uRANKBATTLE_ROOMINFO
{
	sRANKBATTLE_ROOMINFO_INDIVIDUAL	sIndividual;
	sRANKBATTLE_ROOMINFO_PARTY		sParty;
};


//-------------------------------------------------------------------------------


// 클라이언트로 방 리스트를 전송할 때 사용
union uRANKBATTLE_ROOMINFO_LIST
{
	// 배열의 크기 : 실제 전송되는 데이터는 byRoomCount 만큼만 전송된다.
	sRANKBATTLE_ROOMLIST_INDIVIDUAL	asIndividualRoom[DBO_RANKBATTLE_MAX_ROOM_COUNT_PER_PAGE];	// byRoomMode == RANKBATTLE_MODE_INDIVIDUAL
	sRANKBATTLE_ROOMLIST_PARTY		asPartyRoom[DBO_RANKBATTLE_MAX_ROOM_COUNT_PER_PAGE];	// byRoomMode == RANKBATTLE_MODE_PARTY
};

struct sRANKBATTLE_SCORE_INFO
{
	WORD						wMaxStraightWin;	// 최대 연승 카운트
	WORD						wStraightWin;		// 연승 카운트
	WORD						wMaxStraightKOWin;	// 최대 KO 연승 카운트
	WORD						wStraightKOWin;		// KO 연승 카운트
	DWORD						dwWin;				// 승리 카운트
	DWORD						dwDraw;				// 무승부 카운트
	DWORD						dwLose;				// 패배 카운트
	float						fPoint;				// Point
};


// 
struct sRANKBATTLE_MEMBER_RESULT
{
	HOBJECT	hPc;
	WORD	wResultCode;
};

// RankBattle 에 참여하는 PC 들의 정보
struct sRANKBATTLE_MATCH_MEMBER_INFO
{
	HOBJECT hPc;
	BYTE	byTeam;		// eRANKBATTLE_TEAM_TYPE
	BYTE	byState;	// eRANKBATTLE_MEMBER_STATE
};



// Score : [Battle Score - 경기의 결과, Stage Score - 해당 Stage의 결과] 등에 사용된다.
struct sRANKBATTLE_SCORE
{
	BYTE	byOwner;
	BYTE	byChallenger;
};



// Point : BattlePoint
struct sRANKBATTLE_POINT
{
	float					fOwner;
	float					fChallenger;
};


// 경기 결과
struct sRANKBATTLE_MATCH_FINISH_INFO
{
	struct sWINER_RECORD
	{
		WORD				wStraightKOWinCount;	// KO 연승 카운트
		WORD				wStraightWinCount;		// 연승 카운트
	};

	struct sTEAM_MATCH_FINISH_INFO
	{
		BYTE			byScore;		// score

		float			fPoint;			// 획득 포인트
		float			fBonusPoint;	// 보너스 포인트

		WORD			wWinCount;
		WORD			wDrawCount;
		WORD			wLoseCount;
	};

	//------------------------------------------------------------------------

	BYTE					byMatchWinner;			// eRANKBATTLE_MATCH_RESULT : Winner
	sWINER_RECORD			sWinerRecord;

	sTEAM_MATCH_FINISH_INFO	sOwnerInfo;
	sTEAM_MATCH_FINISH_INFO	sChallengerInfo;
};


// 
struct sRANKBATTLE_RANK_DATA
{
	WCHAR				wszCharName[NTL_MAX_SIZE_CHAR_NAME_UNICODE];
	BYTE				byLevel;
	BYTE				byClass;
	WCHAR				wszGuildName[NTL_MAX_SIZE_GUILD_NAME_IN_UNICODE];
	int					nPoint;
	WORD				wStraightWinCount;
	DWORD				adwCompareRank[DBO_RANKBATTLE_MAX_COMPARE_DAY];
};

struct sRANKBATTLE_RANK_INFO
{
	WORD				wCharName;				// WCHAR [NTL_MAX_SIZE_CHAR_NAME_UNICODE]
	BYTE				byLevel;
	BYTE				byClass;
	WORD				wGuildName;				// WCHAR [NTL_MAX_SIZE_GUILD_NAME_IN_UNICODE]
	int					nPoint;
	WORD				wStraightWinCount;
	DWORD				dwCompareRank;
};


#pragma pack()
//-------------------------------------------------------------------------------------