//***********************************************************************************
//
//	File		:	NtlDragonBall.h
//
//	Begin		:	2007-6-20
//
//	Copyright	:	ⓒ NTL-Inc Co., Ltd
//
//	Author		:	Doo Sup, Chung   ( john@ntl-inc.com )
//
//	Desc		:	
//
//***********************************************************************************

#pragma once

#include "NtlSharedType.h"
#include "NtlItem.h"

#pragma pack(1)
enum eSCHEDULE_EVENT_TYPE
{
	SCHEDULE_EVENT_TYPE_NORMAL_DRAGONBALL,			// 노말 드래곤볼 수집
	SCHEDULE_EVENT_TYPE_BASIC_DRAGONBALL,			// 베이식 드래곤볼 수집

	SCHEDULE_EVENT_TYPE_COUNT,

	SCHEDULE_EVENT_TYPE_FIRST = SCHEDULE_EVENT_TYPE_NORMAL_DRAGONBALL,
	SCHEDULE_EVENT_TYPE_LAST  = SCHEDULE_EVENT_TYPE_BASIC_DRAGONBALL,
};

struct sSCHEDULE_INFO
{
	BYTE					byEventType;			//eSCHEDULE_EVENT_TYPE
	__int64					nStartTime;
	__int64					nEndTime;
	int						nMainTerm;
	int						nSubTerm;
	BYTE					byTermType;			//eSCHEDULE_TERM_TYPE
	sDBO_TIME				tCreateTime;
};

enum eCOLLECT_STATE
{
	COLLECT_STATE_NONE	= 0,
	COLLECT_STATE_DB_UI_OK,
	COLLECT_STATE_DRAGON_OUT,
	COLLECT_STATE_DRAGON_DESPAWN,

	COLLECT_STATE_COUNT,

	COLLECT_STATE_FIRST = COLLECT_STATE_DB_UI_OK,
	COLLECT_STATE_LAST  = COLLECT_STATE_DRAGON_DESPAWN,
	
};
struct sCOLLECT_BRIEF
{
	HOBJECT		hOwner;		// 클릭한 유져의 핸들
	BYTE		byState;	// 진행상황
	DWORD		dwTick;		// 진행시간
	HOBJECT		hDragon;	// 용신 핸들
	sITEM_DELETE_DATA asData[NTL_ITEM_MAX_DRAGONBALL];	// 지워야할 드래곤볼
};

enum eDRAGONBALL_REWARD_TYPE
{
	DRAGONBALL_REWARD_TYPE_ITEM,
	DRAGONBALL_REWARD_TYPE_SKILL,
	DRAGONBALL_REWARD_TYPE_ZENNY,
	DRAGONBALL_REWARD_TYPE_HTB,
};

enum eDRAGON_BALL_TYPE
{
	DRAGON_BALL_TYPE_BASIC = 0,
	DRAGON_BALL_TYPE_NORMAL,
    DRAGON_BALL_TYPE_LEGENDARY,
    DRAGON_BALL_TYPE_EVENT,

    DRAGON_BALL_TYPE_COUNT,
	DRAGON_BALL_TYPE_NONE = 0xFF,
};

const unsigned int	DBO_MAX_TIMER_DB_UI_OK = 3 * 60 * 1000;		// 3분

const unsigned int	DBO_MAX_TIMER_DRAGON_OUT = 6 * 1000;		// 6초

const unsigned int	DBO_MAX_TIMER_DRAGON_DESPAWN = 5 * 1000;	// 5초

const unsigned int	DBO_MAX_DRAGONBALL_BASIC_MOB_LEVEL = 20;	// 20 레벨 미만만 
#pragma pack()
//-----------------------------------------------------------------------------------