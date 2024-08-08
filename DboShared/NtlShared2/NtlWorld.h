//***********************************************************************************
//
//	File		:	NtlWorld.h
//
//	Begin		:	2006-11-15
//
//	Copyright	:	ⓒ NTL-Inc Co., Ltd
//
//	Author		:	Hyun Woo, Koo   ( zeroera@ntl-inc.com )
//
//	Desc		:	
//
//***********************************************************************************

#pragma once

#include "NtlSharedType.h"
#include "NtlTimeQuest.h"


//-----------------------------------------------------------------------------------
//  
//-----------------------------------------------------------------------------------

// 월드 게임 룰 
enum eGAMERULE_TYPE
{
	GAMERULE_NORMAL,		// 일반 필드
	GAMERULE_RANKBATTLE,	// 랭킹 배틀
	GAMERULE_MUDOSA,		// 천하제일무도회장
	GAMERULE_DOJO,			// 도장
	GAMERULE_RAID,			// 레이드
	GAMERULE_HUNT,			// 사냥 (개인/파티별 인스턴스)
	GAMERULE_TIMEQUEST,		// 타임머신 퀘스트
	GAMERULE_TUTORIAL,		// 튜토리얼
	GAMERULE_MINORMATCH,	// 천하제일무도회 예선
	GAMERULE_MAJORMATCH,	// 천하제일무도회 본선
	GAMERULE_FINALMATCH,	// 천하제일무도회 결선
	GAMERULE_TEINKAICHIBUDOKAI,	// 천하제일무도회
	GAMERULE_TLQ,			// 타임리프 퀘스트

	GAMERULE_TYPE_COUNT,
	INVALID_GAMERULE = 0xFFFFFFFF, // 없음
};

// WorldPlay Executer
enum eWORLDPLAY_EXECUTER_TYPE
{
	WORLDPLAY_EXECUTER_TS,
	WORLDPLAY_EXECUTER_WPS,
	WORLDPLAY_EXECUTER_TLQ,
	WORLDPLAY_EXECUTER_STATIC_WORLD,
	WORLDPLAY_EXECUTER_DYNAMIC_WORLD,
	WORLDPLAY_EXECUTER_OBJECTTRIGGER,
	WORLDPLAY_EXECUTER_ITEM_USE_BY_LOCATION,

	WORLDPLAY_EXECUTER_TYPE_COUNT,
	INVALID_WORLDPLAY_EXECUTER_TYPE = 0xff
};


// World Play Share Type
enum eWORLDPLAY_SHARE_TYPE
{
	WORLDPLAY_SHARE_NONE,
	WORLDPLAY_SHARE_INDIVIDUAL,
	WORLDPLAY_SHARE_PARTY,
//	WORLDPLAY_SHARE_ALL,

	WORLDPLAY_SHARE_TYPE_COUNT,
	INVALID_WORLDPLAY_SHARE_TYPE = 0xff,
};


//-----------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------
// 월드 관련 정의
//-----------------------------------------------------------------------------------
const float			NTL_DUMMY_AREA_WIDTH_IN_WORLD = 512.0f;		// in meters

const unsigned int	DBO_MAX_WAYPOINT_COUNT = 255; //
//-----------------------------------------------------------------------------------



//-----------------------------------------------------------------------------------
// 월드 관련 함수
//-----------------------------------------------------------------------------------
HOBJECT MakeTriggerObjectHandle(HOBJECT hWorldTriggerObjectOffset, DWORD dwTriggerObjectSequence);

const char * NtlGetGameRuleTypeString(BYTE byRuleType);
//-----------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------
// 월드 관련 packet structure
//-----------------------------------------------------------------------------------
#pragma pack(1)


struct sGAME_RULE_INFO
{
	BYTE			byRuleType;	// eGAMERULE_TYPE
	union
	{
		sTIMEQUEST_RULE_INFO	sTimeQuestRuleInfo;
		//sRANKBATTLE_RULE_INFO		sRankBattleRuleInfo;
		//sBUDOKAI_RULE_INFO	sBudokaiRuleInfo;
		//sDOJO_RULE_INFO		sDojoRuleInfo;
	};
};


struct sWORLD_INFO
{
	// World Base Info
	WORLDID			worldID;	// 월드 인스턴스 핸들
	TBLIDX			tblidx;		// 월드 테이블 인덱스

	// World Game Info
	HOBJECT			hTriggerObjectOffset; // 트리거 오브젝트 시작 핸들값 (트리거 오브젝트 테이블의 오브젝트를 생성시 테이블 번호 + 시작 핸들번호로 생성할 것 )

	// World Rule Info
	sGAME_RULE_INFO	sRuleInfo;
};

struct sWORLD_ZONE_INFO
{
	ZONEID			zoneId;
	bool			bIsDark;
};

#pragma pack()
//-----------------------------------------------------------------------------------
