//***********************************************************************************
//
//	File		:	NtlTimeQuest.h
//
//	Begin		:	2007-05-31
//
//	Copyright	:	ⓒ NTL-Inc Co., Ltd
//
//	Author		:	Hyun Woo, Koo   ( zeroera@ntl-inc.com )
//
//	Desc		:	
//
//***********************************************************************************

#pragma once

#include "NtlParty.h"
#include "NtlTSCoreDefine.h"

#include "NtlBitFlag.h"


//-----------------------------------------------------------------------------------
// Enumeration
//-----------------------------------------------------------------------------------

// 타임머신 타입
enum eTIMEQUEST_TYPE
{
	TIMEQUEST_TYPE_TUTORIAL, // 튜토리얼 타임 퀘스트 ( 대기실이 생성되지 않는다 )
	TIMEQUEST_TYPE_PARTY, // 파티형 타임 퀘스트
	TIMEQUEST_TYPE_GROUP, // 그룹형 타임 퀘스트

	MAX_TIMEQUEST_TYPE,
};


// 타임머신 모드
enum eTIMEQUEST_MODE
{
	TIMEQUEST_MODE_INDIVIDUAL, // 개인등록의 경우
	TIMEQUEST_MODE_PARTY, // 파티등록의 경우

	MAX_TIMEQUEST_MODE,
	INVALID_TIMEQUEST_MODE = 0xFF, // 아직 등록이 안되었을 경우
};


// 타임머신 모드
enum eTIMEQUEST_ROOM_STATE
{
	TIMEQUEST_ROOM_STATE_REGISTRAITION,		// 등록
	TIMEQUEST_ROOM_STATE_WAITENTRY,			// 진입 대기
	TIMEQUEST_ROOM_STATE_PC_DIRECTION,		// PC 진입 연출
	TIMEQUEST_ROOM_STATE_DAYRECORD_REQ,		// Query로 DayRecord를 요청
	TIMEQUEST_ROOM_STATE_OBJECT_DIRECTION,	// OBJECT 진입 연출
	TIMEQUEST_ROOM_STATE_PREPARE_WORLD,		// WorldPool 에서 World를 예약한다.

	MAX_TIMEQUEST_ROOM_STATE,
	INVALID_TIMEQUEST_ROOM_STATE = 0xFF, 
};


// 타임머신 게임 상태
enum eTIMEQUEST_GAME_STATE
{
	TIMEQUEST_GAME_STATE_WAIT, // TMQ 생성하고 플레이어 진입 대기
	TIMEQUEST_GAME_STATE_PREPARE, // 모든 플레이어 진입후 컨디션 체크 후 시작 여부 결정
	TIMEQUEST_GAME_STATE_ARRIVE, // TMQ 도착 상태 ( TS부터의 이벤트 대기, 유저 나타나는 시간 이벤트 대기 )
	TIMEQUEST_GAME_STATE_BEGIN, // 게임 시작 ( 스크립트 제어 )
	TIMEQUEST_GAME_STATE_STAGE_READY, // 스테이지 준비 ( 스크립트 제어 )
	TIMEQUEST_GAME_STATE_STAGE_START, // 스테이지 시작 ( 스크립트 제어 )
	TIMEQUEST_GAME_STATE_STAGE_FINISH, // 스테이지 종료 ( 스크립트 제어 )
	TIMEQUEST_GAME_STATE_END, // 게임 끝
	TIMEQUEST_GAME_STATE_LEAVE, // TMQ 나가기 상태
	TIMEQUEST_GAME_STATE_FAIL, // 게임 끝 ( TMQ 실패 )
	TIMEQUEST_GAME_STATE_CLOSE, // TMQ 종료

	MAX_TIMEQUEST_GAME_STATE
};


// 타임머신 스테이지 상태
enum eTIMEQUEST_STAGE_STATE
{

	MAX_TIMEQUEST_STAGE_STATE
};


// 타임머신 난이도
enum eTIMEQUEST_DIFFICULTY
{
	TIMEQUEST_DIFFICULTY_EASY,
	TIMEQUEST_DIFFICULTY_NORMAL,
	TIMEQUEST_DIFFICULTY_HARD,

	MAX_TIMEQUEST_DIFFICULTY,
	FIRST_TIMEQUEST_DIFFICULTY = TIMEQUEST_DIFFICULTY_EASY,
	INVALID_TIMEQUEST_DIFFICULTY = 0xFF,
};


// 타임머신 난이도 플래그
enum eTIMEQUEST_DIFFICULTY_FLAG
{
	TIMEQUEST_DIFFICULTY_FLAG_EASY		= MAKE_BIT_FLAG( TIMEQUEST_DIFFICULTY_EASY ),
	TIMEQUEST_DIFFICULTY_FLAG_NORMAL	= MAKE_BIT_FLAG( TIMEQUEST_DIFFICULTY_NORMAL ),
	TIMEQUEST_DIFFICULTY_FLAG_HARD		= MAKE_BIT_FLAG( TIMEQUEST_DIFFICULTY_HARD ),
};


// 타임머신 진행 메시지 타입
enum eTIMEQUEST_PROGRESS_MESSAGE_TYPE
{
	TIMEQUEST_PROGRESS_MESSAGE_TYPE_START,
	TIMEQUEST_PROGRESS_MESSAGE_TYPE_END,
	TIMEQUEST_PROGRESS_MESSAGE_TYPE_CLEAR,
	TIMEQUEST_PROGRESS_MESSAGE_TYPE_FAIL,

	MAX_TIMEQUEST_PROGRESS_MESSAGE_TYPE,
	INVALID_TIMEQUEST_PROGRESS_MESSAGE_TYPE = 0xFF,
};


// 타임퀘스트 정보출력 타입
enum eTIMEQUEST_INFORMATION_TYPE
{
	TIMEQUEST_INFORMATION_TYPE_SCENARIO,
	TIMEQUEST_INFORMATION_TYPE_STAGE,

	MAX_TIMEQUEST_INFORMATION_TYPE,
	INVALID_TIMEQUEST_INFORMATION_TYPE = 0xFF,
};


//-----------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------
//	상수 정의
//-----------------------------------------------------------------------------------
const DWORD		DBO_TIMEQUEST_MAX_ROOM_COUNT = 10; // 타임머신 맵당 방의 최대 개수

const DWORD		DBO_TIMEQUEST_MAX_MEMBER_COUNT = NTL_MAX_MEMBER_IN_PARTY; // 타임머신 최대 출발 인원

const DWORD		DBO_TIMEQUEST_ROOM_START_REPORT_TIME = 30000; // 타임머신 출발 알림을 보내주는 시간

const DWORD		DBO_TIMEQUEST_ROOM_LEAVE_LIMIT_TIME = 60000; // 타임머신 대기룸에서 나가지 못하는 시간

const DWORD		DBO_TIMEQUEST_MAX_STAGE_NUMBER = 64; // 타임머신 퀘스트의 최대 스테이지

const DWORD		DBO_TIMEQUEST_MAX_KILL_COUNT = 3; // 타임머신 퀘스트의 최대 킬카운트 (넘어가면 자동 퇴장)

const DWORD		DBO_TIMEQUEST_ROOM_REFRESH_TIME = 10000; // 타임머신 퀘스트의 룸정보 리프레쉬 시간

const WORD		DBO_TIMEQUEST_COUNPON_CHANGE_POINT = 1000; // 쿠폰 1장으로 전환되는 타임퀘스트 포인트점수

const WORD		DBO_TIMEQUEST_COUNPON_LIMIT_COUNT = 4; // 가질수 있는 최대 쿠폰 개수

const WORD		DBO_TIMEQUEST_DEFAULT_DAYRECORD_RANK_COUNT = 5;	// tblidx의 난이도 별로 남기는 rank의 갯수

const WORD		DBO_TIMEQUEST_DEFAULT_TOTAL_RANK_COUNT = DBO_TIMEQUEST_DEFAULT_DAYRECORD_RANK_COUNT + 1;	
													// tblidx의 난이도 별로 남기는 rank의 갯수 와 BestRecord +
//-----------------------------------------------------------------------------------



//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
#pragma pack( 1 )

// 방인원 정보
struct sTIMEQUEST_ROOM_MEMBER
{
	HOBJECT					handle;
	WCHAR					awchName[ NTL_MAX_SIZE_CHAR_NAME_UNICODE + 1 ]; // 캐릭터 이름
	BYTE					byClass; // 해당 캐릭터 클래스
};


// 방에 있는 인원 정보
struct sTIMEQUEST_ROOM_MEMBER_INFO
{
	BYTE byMemberCount;
	sTIMEQUEST_ROOM_MEMBER	asRoomMember[ DBO_TIMEQUEST_MAX_MEMBER_COUNT ];
};


// 방에 있는 파티 정보
struct sTIMEQUEST_ROOM_PARTY_INFO
{
	WCHAR					awchName[ NTL_MAX_SIZE_PARTY_NAME_IN_UNICODE + 1 ];
};


// 방정보
struct sTIMEQUEST_ROOM
{
	ROOMID					roomID; // 방 번호
	TBLIDX					timeQuestTblidx; // 해당 타임 퀘스트 테이블 인덱스
	DWORD					dwRemainSec; // 출발시간까지 남은 시간 (초)
	BYTE					byStartHour; // 시작시간 (시)
	BYTE					byStartMin; // 시작시간 (분)
	BYTE					byRoomMode; // eTIMEQUEST_MODE
	BYTE					byDifficulty; // eTIMEQUEST_DIFFICULTY
	BYTE					byRoomState; // eTIMEQUEST_ROOM_STATE	
};


// 타임머신 퀘스트 게임 상태
struct sTIMEQUEST_STAGE
{
	BYTE					byStageNumber; // 스테이지 번호
};


// 타임머신 퀘스트 게임 상태
struct sTIMEQUEST_STATE
{
	BYTE					byGameState; // eTIMEQUEST_GAME_STATE
	sTIMEQUEST_STAGE		sTimeQuestStage;
};


// 타임퀘스트 게임 정보
struct sTIMEQUEST_RULE_INFO
{
	TBLIDX					timeQuestTblidx; // 해당 타임 퀘스트 테이블 인덱스
	BYTE					byStartHour; // 시작시간 (시)
	BYTE					byStartMin; // 시작시간 (분)
	BYTE					byTimeQuestMode; // eTIMEQUEST_MODE
	BYTE					byDifficulty; // eTIMEQUEST_DIFFICULTY
	bool					bCountDown; // 카운트 다운 여부
	DWORD					dwLimitTime; // 제한 시간
	DWORD					dwBonusTime; // 보너스 시간
	DWORD					dwDayRecord; // 해당 타임퀘스트의 DayRecord

	sTIMEQUEST_STATE		sTimeQuestState;
};


// TMQ 에서 사용하는 시간 관리 구조
struct sTIMEQUEST_TIME
{
	// 다음 출발 시간을 계산
	void	NextTime(BYTE byTermMinute)
	{
		byHour = byHour + (BYTE)(byTermMinute / 60);
		byMinute += byTermMinute % 60;

		if( 60 <= byMinute)
		{
			byMinute -= 60;
			byHour++;
		}

		if(24 < byHour)
		{
			byHour -= 24;
		}
	}

	void	NextBySec(DWORD dwTermSec)
	{
		NextTime( (BYTE)(dwTermSec / 60));
	}

	BYTE	byHour;
	BYTE	byMinute;
};



//-----------------------------------------------------------------------------
// Protocol 에서만 사용한다

//-------------------------------------
// GU_TIMEQUEST_ROOM_LIST_RES 에서 사용
struct sTIMEQUEST_ROOM_TMQ_INFO
{
	TBLIDX				tmqTblidx;				// tmq tblidx
	BYTE				byDifficult;			// 난이도 - eTIMEQUEST_DIFFICULTY
	sTIMEQUEST_TIME		sCurTime;				// 현재 시간
	sTIMEQUEST_TIME		sNextEnterTime;			// 다음 출발 시간
	WORD				wWaitPartyCount;		// 현재까지의신청 파티 수
	WORD				wWaitIndividualCount;	// 신청 PC 수
};


//-------------------------------------
// GU_TIMEQUEST_ROOM_JOIN_RES, GU_TIMEQUEST_ROOM_JOIN_NFY 에서 사용
struct sTIMEQUEST_ROOM_JOIN_INFO
{
	TBLIDX					tmqTblidx;
	ROOMID					roomId;
	BYTE					byRoomState;			// eTIMEQUEST_ROOM_STATE
	BYTE					byTimeQuestMode;		// eTIMEQUEST_MODEz
	DWORD					dwRemainTime;			// [추첨|진입]까지 남은 시간
	BYTE					byDifficult;			// 난이도
	WORD					wWaitPartyCount;		// 신청 파티 수
	WORD					wWaitIndividualCount;	// 신청 PC 수
};


//-------------------------------------
// GU_TIMEQUEST_ROOM_INFO_RES 에서 사용
// TMQ 신청 후 화면 우측 하단에 나오는 TMQ Info Icon을 업데이트 하기 위한 정보
struct sTIMEQUEST_ROOM_REGISTRATION_ICON_INFO
{
	WORD					wWaitPartyCount;		// 신청 파티 수
	WORD					wWaitIndividualCount;	// 신청 PC 수
};

struct sTIMEQUEST_ROOM_WAITENTRY_ICON_INFO
{
	BYTE					bHaveItem;				// 아이템을 가지고 있나?
};

union uTIMEQUEST_ROOM_ICON_INFO
{
	sTIMEQUEST_ROOM_REGISTRATION_ICON_INFO	sRegistrationInfo;	// eTIMEQUEST_ROOM_STATE == TIMEQUEST_ROOM_STATE_REGISTRAITION
	sTIMEQUEST_ROOM_WAITENTRY_ICON_INFO		sWaitEntryInfo;		// eTIMEQUEST_ROOM_STATE == TIMEQUEST_ROOM_STATE_WAITENTRY
};



//-------------------------------------
// GU_TIMEQUEST_SELECTION_NFY 에서 사용
struct sTIMEQUEST_ROOM_ENTRY_INFO
{
	DWORD					dwReaminEntryTime;		// 진입까지 남은 시간
	bool					bHaveItem;				// 필요 아이템을 가지고 있는가?
};

struct sTIMEQUEST_ROOM_NEXT_TMQ_INFO
{
	TBLIDX					tmqTblidx;
	sTIMEQUEST_TIME			sNextTMQTime;			// 다음 시작 시간
};

union uTIMEQUEST_ROOM_SELECTION_INFO
{
	sTIMEQUEST_ROOM_ENTRY_INFO		sEntryInfo;
	sTIMEQUEST_ROOM_NEXT_TMQ_INFO	sNextTmqInfo;
};




struct sTIMEQUEST_DAYRECORD_DATA
{
	BYTE		byDifficult;
	DWORD		dwClearTime;
	BYTE		byMemberCount;
	CHARACTERID	aMemberCharId[NTL_MAX_MEMBER_IN_PARTY];
};


//--------------------------------------------
// Query Server <-> Game Server
struct sTIMEQUEST_TEAM_DATA
{
	DWORD				dwClearTime;	// clear time
	BYTE				byMode;			// eTIMEQUEST_MODE
	WCHAR				wszPartyName[NTL_MAX_SIZE_PARTY_NAME_IN_UNICODE];
	BYTE				byMemberCount;
	WCHAR				awszMember[NTL_MAX_MEMBER_IN_PARTY][NTL_MAX_SIZE_CHAR_NAME_UNICODE + 1];
};

struct sTIMEQUEST_MEMBER_DATA
{
	CHARACTERID			charId;
	WCHAR				wszName[NTL_MAX_SIZE_CHAR_NAME_UNICODE + 1];
};

//------------------------------------------------------------------
struct sTIMEQUEST_PARTY_MEMBER_DATA
{
	BYTE				byLevel;
	BYTE				byClass;
	WCHAR				wszName[NTL_MAX_SIZE_CHAR_NAME_UNICODE + 1];
};
	
struct sTIMEQUEST_TEAM_RANK_DATA
{
	DWORD				dwClearTime;	// clear time
	BYTE				byMode;			// eTIMEQUEST_MODE
	WCHAR				wszPartyName[NTL_MAX_SIZE_PARTY_NAME_IN_UNICODE];
};

struct sTIMEQUEST_TEAM_MEMBER_DATA
{
	BYTE							byMemberCount;
	sTIMEQUEST_PARTY_MEMBER_DATA	asMember[NTL_MAX_MEMBER_IN_PARTY];
};


//----------------------------------------------
// Game Server <-> Client

struct sTIMEQUEST_MEMBER_VAR
{
	WORD		wName;		// WCHAR[NTL_MAX_SIZE_CHAR_NAME_UNICODE]
	CHARACTERID	charId;
};

struct sTIMEQUEST_TEAM_VAR
{
	DWORD				dwClearTime;	// clear time
	WORD				wPartyName;		// WCHAR [NTL_MAX_SIZE_PARTY_NAME_IN_UNICODE];
	BYTE				byMemberCount;
	WORD				wMember;		// sTIMEQUEST_MEMBER [NTL_MAX_MEMBER_IN_PARTY];
};




//-----------------------------------------------------------------------------
//	index - data의 위치
//	[ 1byte | .... ] [ 2byte | .... ] [ 2byte | .... ] [ 1byte | .... ]
//  [ size  | data ] [ size  | data ] [ size  | data ] [ size  | data ]
//  size : 1~2byte  첫번째 byte의 첫번째 bit는 크기가 127을 넘어가는지를 체크하는 비트

struct sVARIABLE_DATA
{
protected:
	WORD	wReserveSize;
	WORD	wOffset;			// 새로운 데이터가 들어갈 위치이자 데이터의 크기이다
	BYTE	abyData[1];

public:
	sVARIABLE_DATA() : wOffset(0), wReserveSize(1){}						// 프로토콜에서만 사용하도록 한다.
	sVARIABLE_DATA(WORD wReserve) : wOffset(0)
	{
		Init( wReserve );
	}

	void Init( WORD wReserve )
	{
		wOffset = 0;

		if( wReserve >= 4096/*PACKET_MAX_SIZE*/ )
		{
			_ASSERT(0);

			wReserveSize = 0;
			return;
		}

		wReserveSize = wReserve;

		::memset( abyData, 0xff, sizeof(BYTE) * wReserve );
	}


	// 패킷 사용시 초기화를 쉽게 하기 위핸 함수(패킷 사용에 특화된 함수)
	void InitPacketSize( WORD wDefaultPacketSize )
	{
		wOffset = 0;

		wReserveSize = 4096/*PACKET_MAX_SIZE*/ - wDefaultPacketSize + sizeof(BYTE);

		if( wReserveSize >= 4096/*PACKET_MAX_SIZE*/ )
		{
			_ASSERT(0);

			wReserveSize = 0;
			return;
		}

		::memset( abyData, 0xff, sizeof(BYTE) * wReserveSize );
	}


	// 패킷 사용시 패킷의 크기를 쉽게 계산하기 위한 함수.(패킷 사용에 특화된 함수)
	WORD GetPacketSize( WORD wDefaultPacketSize ) { return wDefaultPacketSize - sizeof(BYTE) + GetDataSize(); }


	// sVARIABLE_DAT의 총 크기 : offset + sizeof(sVARIABLE_DATA) - abyData[1] 크기
	WORD GetTotalSize() { return wOffset + sizeof(sVARIABLE_DATA) - sizeof(BYTE);}
	// Data의 크기
	WORD GetDataSize() { return wOffset;}

	// 예약 크기를 설정(실제 메모리의 크기를 변환시키지는 않는다. 이 값은 생성할때 혹은 생성 이후 외부에서 알려준다.)
//	void SetReserveSize(WORD wReserve) { wReserveSize = wReserve; }
//	WORD GetReserveSize() { return wReserveSize;}

	void WriteLock() { wReserveSize = GetDataSize(); }


	// Data를 복사한다.
	void __SetData( WORD wOff, BYTE * pData)
	{
		wOffset = wOff;
		::memcpy( abyData, pData, wOff);
	}

	// pData : 복사할 위치, wReserverSize : 저장 가능한 실제 공간의 크기(여유 공간)
	bool CopyTo( sVARIABLE_DATA * pData, WORD wReserve)
	{
		if(NULL == pData)
		{
			return false;
		}

		if(wReserve < GetDataSize())
		{
			return false;
		}

		pData->__SetData( wOffset, abyData);
		pData->WriteLock();

		return true;
	}


	//-----------------------------------------------------------------------------------
	//	Purpose	: 127 보다 작을 경우와 127 보다 크거나 같은 경우를 분리하여 저장
	//			  크기의 첫번째 bit는 data의 크기가 127을 넘는지 넘지 않는지를 표시한다
	//			  전체 크기는 2에 15승을 넘지 못한다.
	//	Return	: 
	//-----------------------------------------------------------------------------------
	WORD Write( WORD wSize, void * pData)
	{
		WORD wTemp = wOffset;

		if( 127 >= wSize)	// 길이가 127보다 작거나 같은 경우
		{
			// 예약 공간이 충분한지 체크
			if( wOffset + wSize + 1 > wReserveSize)
			{
				return INVALID_WORD;
			}


			// write
			abyData[wOffset] = (BYTE)wSize;
			::memcpy( &(abyData[wOffset + 1]), pData, wSize);

			// offset 조정
			wOffset += wSize + 1;
		}
		else				// 길이가 127보다 큰 경우 WORD 형으로 크기 저장
		{
			// 예약 공간이 충분한지 체크
			if( wOffset + wSize + 2 > wReserveSize)
			{
				return INVALID_WORD;
			}

			// write
			abyData[wOffset] = (BYTE)((wSize % 0x80) | 0x80);	// 첫번째 byte
			abyData[wOffset+1] = (BYTE)(wSize >> 7);			// 두번째 byte

			::memcpy( &(abyData[wOffset + 2]), pData, wSize);

			// offset 조정
			wOffset += wSize + 2;
		}

		return wTemp;
	}


	//-----------------------------------------------------------------------------------
	//	Purpose	: 127 보다 작을 경우와 127 보다 크거나 같은 경우를 분리하여 저장
	//			  크기의 첫번째 bit는 data의 크기가 127을 넘는지 넘지 않는지를 표시한다
	//			  전체 크기는 2에 15승을 넘지 못한다.
	//	Return	: wSize 크기만큼의 메모리를 할당하고 해당 오프셋을 반환한다.
	//			  (메모리할당은 Write()와 동일하지만 할당한 메모리에 어떠한 값도 쓰지는 않는다.)
	//-----------------------------------------------------------------------------------
	WORD Malloc( WORD wSize)
	{
		WORD wTemp = wOffset;

		if( 127 >= wSize)	// 길이가 127보다 작거나 같은 경우
		{
			// 예약 공간이 충분한지 체크
			if( wOffset + wSize + 1 > wReserveSize)
			{
				return INVALID_WORD;
			}


			// write
			abyData[wOffset] = (BYTE)wSize;

			// offset 조정
			wOffset += wSize + 1;
		}
		else				// 길이가 127보다 큰 경우 WORD 형으로 크기 저장
		{
			// 예약 공간이 충분한지 체크
			if( wOffset + wSize + 2 > wReserveSize)
			{
				return INVALID_WORD;
			}

			// write
			abyData[wOffset] = (BYTE)((wSize % 0x80) | 0x80);	// 첫번째 byte
			abyData[wOffset+1] = (BYTE)(wSize >> 7);			// 두번째 byte

			// offset 조정
			wOffset += wSize + 2;
		}

		return wTemp;
	}


	//-----------------------------------------------------------------------------------
	//	Purpose	: wReadLoc에 위치한 Data의 크기를 반환
	//	Return	: 
	//-----------------------------------------------------------------------------------
	WORD GetSize( WORD wReadLoc)
	{
		if( wReadLoc >= wOffset)
		{
			return 0;
		}

		// 크기가 127을 넘는 경우
		if( 0x80 & abyData[wReadLoc])
		{
			// 크기가 저장된 위치가 유효한지 체크(2byte 를 사용하므로)
			if(wReadLoc + 1 >= wOffset)
			{
				return 0;
			}

			// get size
			WORD wSize = (abyData[wReadLoc] & 0x7f) + (abyData[wReadLoc + 1] << 7);

			// 저장된 크기와 저장된 데이터의 크기가 유효한지 체크
			if(wReadLoc + wSize + 2 > wOffset)
			{
				return 0;
			}

			return wSize;
		}
		// 크기가 127을 넘지 않는 경우
		else								
		{
			// 저장된 크기와 저장된 데이터의 크기가 유효한지 체크
			if(wReadLoc + abyData[wReadLoc] + 1 > wOffset)
			{
				return 0;
			}

			return abyData[wReadLoc];
		}
	}


	//-----------------------------------------------------------------------------------
	//	Purpose	: wReadLoc에 위치한 Data를 pData에 복사한다.
	//	Return	: 
	//-----------------------------------------------------------------------------------
	bool Read( WORD wReadLoc, void * pData)
	{
		// 크기를 저장하는 공간이 유효한지 체크
		if( wReadLoc >= wOffset)
		{
			return false;
		}

		// 크기가 127을 넘는 경우
		if( 0x80 & abyData[wReadLoc])
		{
			// 크기를 저장하는 공간이 유효한지 체크(2byte 를 사용하므로)
			if(wReadLoc + 1 >= wOffset)
			{
				return false;
			}

			// get size
			WORD wSize = (abyData[wReadLoc] & 0x7f) + (abyData[wReadLoc + 1] << 7);

			// 데이터를 저장하는 공간이 유효한지 체크
			if(wReadLoc + wSize + 2 > wOffset )
			{
				return false;
			}

			// copy
			::memcpy( pData, &(abyData[wReadLoc + 2]), wSize);
		}
		// 크기가 127을 넘지 않는 경우
		else
		{
			// 데이터를 저장하는 공간이 유효한지 체크
			if(wReadLoc + abyData[wReadLoc] + 1 > wOffset)
			{
				return false;
			}

			// copy
			::memcpy( pData, &(abyData[wReadLoc + 1]), abyData[wReadLoc]);
		}

		return true;
	}


	//-----------------------------------------------------------------------------------
	//	Purpose	: wReadLoc에 위치한 Data를 반환한다.
	//	Return	: 
	//-----------------------------------------------------------------------------------
	void * Read( WORD wReadLoc)
	{
		// 크기를 저장하는 공간이 유효한지 체크
		if( wReadLoc >= wOffset)
		{
			return NULL;
		}


		// 크기가 127을 넘는 경우
		if( 0x80 & abyData[wReadLoc])
		{
			// 크기를 저장하는 공간이 유효한지 체크(2byte 를 사용하므로)
			if(wReadLoc + 1 >= wOffset)
			{
				return NULL;
			}

			// get size
			WORD wSize = (abyData[wReadLoc] & 0x7f) + (abyData[wReadLoc + 1] << 7);

			// 데이터를 저장하는 공간이 유효한지를 체크
			if(wReadLoc + wSize + 2 > wOffset)
			{
				return NULL;
			}

			return &(abyData[wReadLoc + 2]);
		}
		// 크기가 127을 넘지 않는 경우
		else
		{
			// 데이터를 저장하는 공간이 유효한지를 체크
			if(wReadLoc + abyData[wReadLoc] + 1 > wOffset)
			{
				return NULL;
			}

			return &(abyData[wReadLoc + 1]);
		} // end if
	} // end void * Read() func
};


#pragma pack()



