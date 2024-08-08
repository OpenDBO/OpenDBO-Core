//***********************************************************************************
//
//	File		:	NtlSkill.cpp
//
//	Begin		:	2006-08-31
//
//	Copyright	:	ⓒ NTL-Inc Co., Ltd
//
//	Author		:	Hyun Woo, Koo   ( zeroera@ntl-inc.com )
//
//	Desc		:	
//
//***********************************************************************************

#include "stdafx.h"

#include "NtlSkill.h"

//-----------------------------------------------------------------------------------
// 스킬 이펙트 코드 정의
//-----------------------------------------------------------------------------------
#ifdef DEF_SYSTEM_EFFECT_CODE
	#undef DEF_SYSTEM_EFFECT_CODE
#endif
#define DEF_SYSTEM_EFFECT_CODE(c, can_cast_when_being_interrupted) #c,

const char * s_system_effect_string[ MAX_SYSTEM_EFFECT_CODE ] =
{
	#include "NtlSystemEffect.h"
};

#ifdef DEF_SYSTEM_EFFECT_CODE
	#undef DEF_SYSTEM_EFFECT_CODE
#endif
#define DEF_SYSTEM_EFFECT_CODE(c, can_cast_when_being_interrupted) can_cast_when_being_interrupted,

const bool s_system_effect_can_cast_when_being_interrupted[ MAX_SYSTEM_EFFECT_CODE ] =
{
	#include "NtlSystemEffect.h"
};
//-----------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
const char * NtlGetSystemEffectString(DWORD dwSystemEffectCode)
{
	if( dwSystemEffectCode >= MAX_SYSTEM_EFFECT_CODE )
	{
		return "NOT DEFINED CODE";
	}

	return s_system_effect_string[ dwSystemEffectCode ];
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
const bool NtlCanCastWhenBeingInterrupted(eSYSTEM_EFFECT_CODE eSystemEffectCode)
{
	if( eSystemEffectCode >= MAX_SYSTEM_EFFECT_CODE )
	{
		return false;
	}

	return s_system_effect_can_cast_when_being_interrupted[ eSystemEffectCode ];
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool Dbo_IsForBlessBuff(BYTE byActiveEffectType)
{
	switch (byActiveEffectType)
	{
	case SKILL_ACTIVE_TYPE_DD:
	case SKILL_ACTIVE_TYPE_DOT:
	case SKILL_ACTIVE_TYPE_DH:
		return false;

	case SKILL_ACTIVE_TYPE_HOT:
		return true;

	case SKILL_ACTIVE_TYPE_DB:
		return false;

	case SKILL_ACTIVE_TYPE_BB:
		return true;

	case SKILL_ACTIVE_TYPE_DC:
	case SKILL_ACTIVE_TYPE_CB:
		return false;

	default:
		return false;
	}
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool Dbo_IsForCurseBuff(BYTE byActiveEffectType)
{
	switch (byActiveEffectType)
	{
	case SKILL_ACTIVE_TYPE_DD:
		return false;

	case SKILL_ACTIVE_TYPE_DOT:
		return true;

	case SKILL_ACTIVE_TYPE_DH:
	case SKILL_ACTIVE_TYPE_HOT:
	case SKILL_ACTIVE_TYPE_DB:
	case SKILL_ACTIVE_TYPE_BB:
	case SKILL_ACTIVE_TYPE_DC:
		return false;

	case SKILL_ACTIVE_TYPE_CB:
		return true;

	default:
		return false;
	}
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool Dbo_IsHarmfulEffectType(BYTE byActiveEffectType)
{
	switch (byActiveEffectType)
	{
	case SKILL_ACTIVE_TYPE_DD:
	case SKILL_ACTIVE_TYPE_DOT:
		return true;

	case SKILL_ACTIVE_TYPE_DH:
	case SKILL_ACTIVE_TYPE_HOT:
	case SKILL_ACTIVE_TYPE_DB:
	case SKILL_ACTIVE_TYPE_BB:
		return false;

	case SKILL_ACTIVE_TYPE_DC:
	case SKILL_ACTIVE_TYPE_CB:
		return true;

	default:
		return false;
	}
}

DWORD Dbo_GetLevelUpGainSP( BYTE byLevel, BYTE byLevelDiff )
{
	return Dbo_GetLevelSP( byLevel ) - Dbo_GetLevelSP( byLevel-byLevelDiff );
}
DWORD Dbo_GetLevelSP( BYTE byLevel )
{
	if ( byLevel<1 )
	{
		return 0;
	}
	else if ( 1 == byLevel )
	{
		return static_cast<DWORD>( 0 );
		//return static_cast<DWORD>( 2 );	
	}
	else
	{
		float fLevel( byLevel );
		return static_cast<DWORD>( fLevel-1 );
		//return static_cast<DWORD>( fLevel*(fLevel/3)+8 );
	}
}