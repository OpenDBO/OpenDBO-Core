//***********************************************************************************
//
//	File		:	NtlBattle.h
//
//	Begin		:	2006-04-24
//
//	Copyright	:	ⓒ NTL-Inc Co., Ltd
//
//	Author		:	Hyun Woo, Koo   ( zeroera@ntl-inc.com )
//
//	Desc		:	
//
//***********************************************************************************

#pragma once

#include "NtlBitFlag.h"

// 데미지 타입
enum eDBO_DAMAGE_TYPE
{
	DBO_DAMAGE_TYPE_PHYSICAL,
	DBO_DAMAGE_TYPE_ENERGY,

	DBO_DAMAGE_TYPE_COUNT,

	DBO_DAMAGE_TYPE_INVALID = 0xFFui8,

	DBO_DAMAGE_TYPE_FIRST = DBO_DAMAGE_TYPE_PHYSICAL,
	DBO_DAMAGE_TYPE_LAST = DBO_DAMAGE_TYPE_ENERGY
};

// 공격 타입
enum eBATTLE_ATTACK_TYPE
{
	BATTLE_ATTACK_TYPE_PHYSICAL,
	BATTLE_ATTACK_TYPE_ENERGY,

	BATTLE_ATTACK_TYPE_COUNT,

	BATTLE_ATTACK_TYPE_INVALID = 0xFFui8
};


// 공격 결과
enum eBATTLE_ATTACK_RESULT
{
	BATTLE_ATTACK_RESULT_HIT, // 일반 공격
	BATTLE_ATTACK_RESULT_CRITICAL_HIT, // 크리티칼 공격
	BATTLE_ATTACK_RESULT_DODGE, // 회피
	BATTLE_ATTACK_RESULT_RESISTED, // 저항
	BATTLE_ATTACK_RESULT_BLOCK, // 블락
	BATTLE_ATTACK_RESULT_REFLECTED_DAMAGE, // 타겟으로부터 반사되어 입은 피해
	BATTLE_ATTACK_RESULT_KNOCKDOWN, // 넉다운
	BATTLE_ATTACK_RESULT_SLIDING, // 슬라이딩

	BATTLE_ATTACK_RESULT_COUNT,

	BATTLE_ATTACK_RESULT_FIRST = BATTLE_ATTACK_RESULT_HIT,
	BATTLE_ATTACK_RESULT_LAST = BATTLE_ATTACK_RESULT_SLIDING
};


// 전투 속성 ( 진/기/맹/여/락 )
enum eBATTLE_ATTRIBUTE
{
	BATTLE_ATTRIBUTE_NONE, // 무속성
	BATTLE_ATTRIBUTE_HONEST, // 정당
	BATTLE_ATTRIBUTE_STRANGE, // 괴팍
	BATTLE_ATTRIBUTE_WILD, // 열혈
	BATTLE_ATTRIBUTE_ELEGANCE, // 고상
	BATTLE_ATTRIBUTE_FUNNY, // 즐김

	BATTLE_ATTRIBUTE_COUNT,
	BATTLE_ATTRIBUTE_UNKNOWN	= 0xFF,

	BATTLE_ATTRIBUTE_FIRST = BATTLE_ATTRIBUTE_NONE,
	BATTLE_ATTRIBUTE_LAST = BATTLE_ATTRIBUTE_FUNNY
};


// 캐릭터 기절시의 reason
enum eFAINT_REASON
{
	FAINT_REASON_HIT, // 맞아서 
	FAINT_REASON_SKILL, // 스킬로
	FAINT_REASON_BUFF, // 버프로
	FAINT_REASON_KNOCKDOWN, // 넉다운으로
	FAINT_REASON_SLIDING, // 넉다운으로
	FAINT_REASON_COMMAND, // 명령어로
	FAINT_REASON_HTB, // HTB로 인한 사망
	FAINT_REASON_REMOVE, // 제거로 인한 사망 ( TMQ 등에서 사용 )
	FAINT_REASON_SELF_FAINTING, // 자폭 스킬 등에 의한 실신

	FAINT_REASON_COUNT,

	FAINT_REASON_UNKNOWN = 0xFF
};


// HTB 관련 정의
enum eHTB_SKILL_TYPE
{
	HTB_SKILL_NONE,
	HTB_SKILL_DASH,			// htb를 처음 시작할 때 client가 target에 dash 이동으로 가까이 접근한다.(자연스러운 연출을 위하여)
	HTB_SKILL_HOMING_UP,	// attacker가 target을 따라서 올라간다.
	HTB_SKILL_HOMING_DOWN,	// attacket가 지면으로 다시 내려온다.
	HTB_SKILL_JUMPING_UP,	// 공중 12m 높이로 무조건 올라간다
	HTB_SKILL_JUMPING_DOWN,	// 공중 12m 높이에서 무조건 내려온다

	HTB_SKILL_TYPE_UNKNOWN	= 0xFF,

	HTB_SKILL_TYPE_FIRST = HTB_SKILL_NONE,
	HTB_SKILL_TYPE_TYPE_LAST  = HTB_SKILL_JUMPING_DOWN
};


// 프리배틀 결과
enum eFREEBATTLE_RESULT
{
	FREEBATTLE_RESULT_WIN, // 대전중에 한명이 HP가 0이 되어 승리함
	FREEBATTLE_RESULT_LOSE, // 대전중에 한명이 대전범위를 벗어나 승리함
	FREEBATTLE_RESULT_DRAW, // 시간이 경과되어 무승부가 됌

	FREEBATTLE_RESULT_COUNT
};


// 공격 액션 정의
enum eATTACK_ACTION
{
	ATTACK_ACTION_HTB,
	ATTACK_ACTION_PUSH,
	ATTACK_ACTION_KNOCKDOWN,
	ATTACK_ACTION_ACTIVE_PULL,

	MAX_ATTACK_ACTION,
	INVALID_ATTACK_ACTION,
};

// 공격 액션 플래그
enum eATTACK_ACTION_FLAG
{
	ATTACK_ACTION_FLAG_HTB			= MAKE_BIT_FLAG( ATTACK_ACTION_HTB ),
	ATTACK_ACTION_FLAG_PUSH			= MAKE_BIT_FLAG( ATTACK_ACTION_PUSH ),
	ATTACK_ACTION_FLAG_KNOCKDOWN	= MAKE_BIT_FLAG( ATTACK_ACTION_KNOCKDOWN ),
	ATTACK_ACTION_FLAG_ACTIVE_PULL	= MAKE_BIT_FLAG( ATTACK_ACTION_ACTIVE_PULL ),
};

enum eDBO_AGGRO_CHANGE_TYPE
{
	DBO_AGGRO_CHANGE_TYPE_INCREASE,
	DBO_AGGRO_CHANGE_TYPE_DECREASE,
	DBO_AGGRO_CHANGE_TYPE_HIGHEST,
	DBO_AGGRO_CHANGE_TYPE_LOWEST,

	DBO_AGGRO_CHANGE_TYPE_PERCENT,
	DBO_AGGRO_CHANGE_TYPE_HIGHEST_PERCENT,
	DBO_AGGRO_CHANGE_TYPE_LOWEST_PERCENT,

	DBO_AGGRO_CHANGE_TYPE_COUNT,

	DBO_AGGRO_CHANGE_TYPE_FIRST = DBO_AGGRO_CHANGE_TYPE_INCREASE,
	DBO_AGGRO_CHANGE_TYPE_LAST = DBO_AGGRO_CHANGE_TYPE_COUNT - 1,
};

//-----------------------------------------------------------------------------------
// 전투 관련 상수 정의 : [4/25/2006 zeroera] : 수정필요 : lua로 옮길 것
//-----------------------------------------------------------------------------------
const float			NTL_BATTLE_PUSH_DISTANCE					= 1.0f; // PUSH 거리
const float			NTL_BATTLE_KNOCKDOWN_DISTANCE				= 5.6f;	// KNOCKDOWN 거리.
const float			NTL_BATTLE_SLIDING_DISTANCE					= 4.0f; // SLIDING 거리

const int			NTL_BATTLE_CHAIN_ATTACK_START				= 1; // chain attack start id 

const int			NTL_BATTLE_MAX_CHAIN_ATTACK_COUNT			= 6; // 최대 체인 어택 값

const float			NTL_BATTLE_BLOCK_DAMAGE_REDUCE_RATE			= 0.3f;
const float			DBO_BATTLE_OFFENCE_CHANGE_RATE_BY_BLOCK			= 0.3f;
const float			NTL_BATTLE_CRITICAL_DAMAGE_INCREASE_RATE	= 1.5f;
const float			DBO_BATTLE_OFFENCE_BONUS_RATE_BY_CRITICAL		= 50.0f;		// in percent

const float			NTL_BATTLE_HIT_FRONT_SIDE_BONUS_RATE		= 1.0f;
const float			NTL_BATTLE_HIT_BACK_SIDE_BONUS_RATE			= 0.5f;

const DWORD			NTL_BATTLE_ATTACK_DELAY_TIME				= 1000;

const DWORD			NTL_BATTLE_KNOCKDOWN_WAKEUP_TIME			= 4000; // 넉다운시 일어나는 타임 ( NPC / MOB 용 )
const DWORD			NTL_BATTLE_KNOCKDOWN_MIN_WAKEUP_TIME		= 2000; // 넉다운시 일어나는 타임 ( PC용 )
const DWORD			NTL_BATTLE_KNOCKDOWN_MAX_WAKEUP_TIME		= 6000; // 넉다운시 일어나는 타임 ( PC용 )
const DWORD			NTL_BATTLE_KNOCKDOWN_WAKEUP_TIME_DIFF		= NTL_BATTLE_KNOCKDOWN_MAX_WAKEUP_TIME - NTL_BATTLE_KNOCKDOWN_MIN_WAKEUP_TIME;
const DWORD			NTL_BATTLE_SLIDING_END_TIME					= 1200; // 슬라이딩 끝나는 타임
const DWORD			NTL_BATTLE_HTB_SANDBAG_END_TIME				= 10000; // HTB시 SANDBAG 끝나는 타임

const int			NTL_BATTLE_MAX_NPC_ATTACK_TYPE				= 2; // NPC 의 공격 형태 ( 현재는 2가지 )

// Free Battle
const float			DBO_FREEBATTLE_CHALLENGE_RANGE				= 10.0f; // 프리배틀시 도전할 수 있는 거리
const float			DBO_FREEBATTLE_MATCH_INSIDE_RADIUS			= 45.0f; // 프리배틀시 기준점으로 부터의 안쪽 대전 범위(반지름)
const float			DBO_FREEBATTLE_MATCH_OUTSIDE_RADIUS			= 50.0f; // 프리배틀시 기준점으로 부터의 바깥쪽 대전 범위(반지름)
const DWORD			DBO_FREEBATTLE_CHALLENGE_RETRY_TIME			= 10; // 프리배틀시 재도전(초)
const DWORD			DBO_FREEBATTLE_CHALLENGE_RETRY_MILLITIME	= DBO_FREEBATTLE_CHALLENGE_RETRY_TIME * 1000; // 프리배틀시 재도전(밀리세컨드)
const DWORD			DBO_FREEBATTLE_CHALLENGE_WAIT_TIME			= 60; // 프리배틀시 재도전(초)
const DWORD			DBO_FREEBATTLE_CHALLENGE_WAIT_MILLITIME		= DBO_FREEBATTLE_CHALLENGE_WAIT_TIME * 1000; // 프리배틀시 재도전(밀리세컨드)
const DWORD			DBO_FREEBATTLE_OUTSIDE_ALLOW_TIME			= 10; // 프리배틀시 장외허용 시간(초)
const DWORD			DBO_FREEBATTLE_OUTSIDE_ALLOW_MILLITIME		= DBO_FREEBATTLE_OUTSIDE_ALLOW_TIME * 1000; // 프리배틀시 장외허용 시간(밀리세컨드)
const DWORD			DBO_FREEBATTLE_MATCH_TIME					= 3600; // 프리배틀 경기 시간(초)
const DWORD			DBO_FREEBATTLE_MATCH_MILLITIME				= DBO_FREEBATTLE_MATCH_TIME * 1000; // 프리배틀 경기시간(밀리세컨드단위)

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
const char * NtlGetBattleAttackTypeString(BYTE byAttackType);

const char * NtlGetBattleAttackResultString(BYTE byAttackResult);

const char * DboGetFreeBattleResultString(BYTE byBattleResult);


//-----------------------------------------------------------------------------------
// 전투 관련 함수 정의 : [4/25/2006 zeroera] : 수정필요 : lua로 옮길 것
//-----------------------------------------------------------------------------------
BYTE				NtlGetBattleChainAttackSequence(BYTE byCharLevel); // 레벨당 최대 CHAIN ATTACK 수 

float				NtlGetBattleAttributeBonusRate(BYTE bySubjectAtt, BYTE byTargetAtt); // 캐릭터 전투 속성에 따른 보너스 공격률

float				NtlGetBattleChainAttackBounsRate(BYTE byAttackSequence); // 체인 어택에 따른 보너스 공격률
