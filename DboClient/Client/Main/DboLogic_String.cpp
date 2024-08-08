/*****************************************************************************
* File			: DboLogic_String.h
* Author		: Hong sungbock
* Copyright		: (주)NTL
* Date			: 2007. 1. 12
* Abstract		: 스트링 관련 로직을 모아둔다
*****************************************************************************
* Desc         : 찾으려는 스트링이 없을 시에는 반드시 알 수 있도록
*				 Assert를 걸던지 스트링이 없다는 메세지를 돌려주도록 하자
*****************************************************************************/

#include "precomp_dboclient.h"
#include "DboLogic.h"

// shared
#include "NtlCharacter.h"
#include "SkillTable.h"
#include "HTBSetTable.h"
#include "ItemRecipeTable.h"
#include "SystemEffectTable.h"
#include "ItemTable.h"
#include "TextAllTable.h"
#include "TableContainer.h"
#include "NtlBattle.h"
#include "NtlService.h"
#include "NtlResultCode.h"

// simulation
#include "NtlSLApi.h"
#include "NtlSobAvatar.h"
#include "NtlSLLogic.h"

// dbo
#include "DisplayStringManager.h"
#include "DboEventGenerator.h"
#include "ChatGui.h"
#include "AlarmManager.h"
#include "NtlClientService.h"
#include "SayFilter.h"


const WCHAR* Logic_GetClassName(RwUInt8 byClass)
{
	// 인덱스를 일일이 검사하여 인덱스의 숫자에 변경이 있을 시 Assert를 발생시켜 바로 알 수 있도록 한다
	// 또한 스트링 테이블의 순서에 영향이 없도록...
	switch( byClass )
	{
	case PC_CLASS_HUMAN_FIGHTER:			return GetDisplayStringManager()->GetString( DST_HUMAN_FIGHTER );
	case PC_CLASS_HUMAN_MYSTIC:				return GetDisplayStringManager()->GetString( DST_HUMAN_MYSTIC );
	case PC_CLASS_HUMAN_ENGINEER:			return GetDisplayStringManager()->GetString( DST_ENGINEER );
	case PC_CLASS_NAMEK_FIGHTER:			return GetDisplayStringManager()->GetString( DST_NAMEK_FIGHTER );
	case PC_CLASS_NAMEK_MYSTIC:				return GetDisplayStringManager()->GetString( DST_NAMEK_MYSTIC );
	case PC_CLASS_MIGHTY_MAJIN:				return GetDisplayStringManager()->GetString( DST_MIGHTY_MAJIN );
	case PC_CLASS_WONDER_MAJIN:				return GetDisplayStringManager()->GetString( DST_WONDER_MAJIN );
	case PC_CLASS_STREET_FIGHTER:			return GetDisplayStringManager()->GetString( DST_STREET_FIGHTER );
	case PC_CLASS_SWORD_MASTER:				return GetDisplayStringManager()->GetString( DST_SWORD_MASTER );
	case PC_CLASS_CRANE_ROSHI:				return GetDisplayStringManager()->GetString( DST_CRANE_ROSHI );
	case PC_CLASS_TURTLE_ROSHI:				return GetDisplayStringManager()->GetString( DST_TURTLE_ROSHI );
	case PC_CLASS_GUN_MANIA:				return GetDisplayStringManager()->GetString( DST_GUN_MANIA );
	case PC_CLASS_MECH_MANIA:				return GetDisplayStringManager()->GetString( DST_MECH_MANIA );
	case PC_CLASS_DARK_WARRIOR:				return GetDisplayStringManager()->GetString( DST_DARK_WARRIOR );
	case PC_CLASS_SHADOW_KNIGHT:			return GetDisplayStringManager()->GetString( DST_SHADOW_KNIGHT );
	case PC_CLASS_POCO_SUMMONER:			return GetDisplayStringManager()->GetString( DST_POCO_SUMMONER );
	case PC_CLASS_DENDEN_HEALER:			return GetDisplayStringManager()->GetString( DST_DENDEN_HEALER );
	case PC_CLASS_GRAND_MA:					return GetDisplayStringManager()->GetString( DST_GRAND_MA );
	case PC_CLASS_ULTI_MA:					return GetDisplayStringManager()->GetString( DST_ULTI_MA );
	case PC_CLASS_PLAS_MA:					return GetDisplayStringManager()->GetString( DST_PLAS_MA );
	case PC_CLASS_KAR_MA:					return GetDisplayStringManager()->GetString( DST_KAR_MA );
	}

	static WCHAR awcBuffer[] = L"Not exist class name";
	NTL_ASSERT(false, "Logic_GetClassName, Not exist class name of class index : " << byClass);

	return awcBuffer;
}

const WCHAR* Logic_GetHoipoiMixSkillName(RwUInt8 byType)
{
	switch( byType )
	{
	case ITEM_RECIPE_TYPE_FOOD_DRINK:		return GetDisplayStringManager()->GetString( DST_ITEM_RECIPE_TYPE_FOOD_DRINK );
	case ITEM_RECIPE_TYPE_MAIN_WEAPON:		return GetDisplayStringManager()->GetString( DST_ITEM_RECIPE_TYPE_MAIN_WEAPON );
	case ITEM_RECIPE_TYPE_ARMOR:			return GetDisplayStringManager()->GetString( DST_ITEM_RECIPE_TYPE_ARMOR );
	case ITEM_RECIPE_TYPE_ACCESSORY:		return GetDisplayStringManager()->GetString( DST_ITEM_RECIPE_TYPE_ACCESSORY );
	case ITEM_RECIPE_TYPE_SCOUTER:			return GetDisplayStringManager()->GetString( DST_ITEM_RECIPE_TYPE_SCOUTER );
	case ITEM_RECIPE_TYPE_SUB_WEAPON:		return GetDisplayStringManager()->GetString( DST_ITEM_RECIPE_TYPE_SUB_WEAPON );
	}

	static WCHAR awcBuffer[] = L"Not exist class name";
	NTL_ASSERT(false, "Logic_GetClassName, Not exist job name of recipe type : " << byType );

	return awcBuffer;
}

const WCHAR* Logic_GetNPCJobName(RwUInt8 byJob)
{
	// 인덱스를 일일이 검사하여 인덱스의 숫자에 변경이 있을 시 Assert를 발생시켜 바로 알 수 있도록 한다
	// 또한 스트링 테이블의 순서에 영향이 없도록...
	switch( byJob )
	{
	case NPC_JOB_WEAPON_MERCHANT:			return GetDisplayStringManager()->GetString( DST_JOB_WEAPON_MERCHANT );
	case NPC_JOB_ARMOR_MERCHANT:			return GetDisplayStringManager()->GetString( DST_JOB_ARMOR_MERCHANT );
	case NPC_JOB_GOODS_MERCHANT:			return GetDisplayStringManager()->GetString( DST_JOB_GOODS_MERCHANT );
	case NPC_JOB_SCOUTER_MERCHANT:			return GetDisplayStringManager()->GetString( DST_JOB_SCOUTER_MERCHANT );
	case NPC_JOB_GUARD:						return GetDisplayStringManager()->GetString( DST_JOB_GUARD );
	case NPC_JOB_SKILL_TRAINER_HFI:			return GetDisplayStringManager()->GetString( DST_JOB_SKILL_TRAINER_HFI );
	case NPC_JOB_SKILL_TRAINER_HMY:			return GetDisplayStringManager()->GetString( DST_JOB_SKILL_TRAINER_HMY );
	case NPC_JOB_SKILL_TRAINER_HEN:			return GetDisplayStringManager()->GetString( DST_JOB_SKILL_TRAINER_HEN );
	case NPC_JOB_SKILL_TRAINER_NFI:			return GetDisplayStringManager()->GetString( DST_JOB_SKILL_TRAINER_NFI );
	case NPC_JOB_SKILL_TRAINER_NMY:			return GetDisplayStringManager()->GetString( DST_JOB_SKILL_TRAINER_NMY );
	case NPC_JOB_SKILL_TRAINER_MMI:			return GetDisplayStringManager()->GetString( DST_JOB_SKILL_TRAINER_MMI );
	case NPC_JOB_SKILL_TRAINER_MWO:			return GetDisplayStringManager()->GetString( DST_JOB_SKILL_TRAINER_MWO );
	case NPC_JOB_BANKER:					return GetDisplayStringManager()->GetString( DST_JOB_BANKER );
	case NPC_JOB_TALKER:					return GetDisplayStringManager()->GetString( DST_JOB_TALKER );
	case NPC_JOB_GUILD_MANAGER:				return GetDisplayStringManager()->GetString( DST_JOB_GUILD_MANAGER );
	case NPC_JOB_SUMMON_PET:				return GetDisplayStringManager()->GetString( DST_JOB_SUMMON_PET );
	case NPC_JOB_DOGI_MERCHANT:				return GetDisplayStringManager()->GetString( DST_JOB_DOGI_MERCHANT );
	case NPC_JOB_SPECIAL_WEAPON_MERCHANT:	return GetDisplayStringManager()->GetString( DST_JOB_SPECIAL_WEAPON_MERCHANT );
	case NPC_JOB_SPECIAL_ARMOR_MERCHANT:	return GetDisplayStringManager()->GetString( DST_JOB_SPECIAL_ARMOR_MERCHANT );
	case NPC_JOB_SPECIAL_GOODS_MERCHANT:	return GetDisplayStringManager()->GetString( DST_JOB_SPECIAL_GOODS_MERCHANT );
	case NPC_JOB_SPECIAL_FOODS_MERCHANT:	return GetDisplayStringManager()->GetString( DST_JOB_SPECIAL_FOODS_MERCHANT );
	case NPC_JOB_SPECIAL_SCOUTER_MERCHANT:	return GetDisplayStringManager()->GetString( DST_JOB_SPECIAL_SCOUTER_MERCHANT );
	case NPC_JOB_GRAND_SKILL_TRAINER_HFI:	return GetDisplayStringManager()->GetString( DST_JOB_GRAND_SKILL_TRAINER_HFI );
	case NPC_JOB_GRAND_SKILL_TRAINER_HMY:	return GetDisplayStringManager()->GetString( DST_JOB_GRAND_SKILL_TRAINER_HMY );
	case NPC_JOB_GRAND_SKILL_TRAINER_HEN:	return GetDisplayStringManager()->GetString( DST_JOB_GRAND_SKILL_TRAINER_HEN );
	case NPC_JOB_GRAND_SKILL_TRAINER_NFI:	return GetDisplayStringManager()->GetString( DST_JOB_GRAND_SKILL_TRAINER_NFI );
	case NPC_JOB_GRAND_SKILL_TRAINER_NMY:	return GetDisplayStringManager()->GetString( DST_JOB_GRAND_SKILL_TRAINER_NMY );
	case NPC_JOB_GRAND_SKILL_TRAINER_MMI:	return GetDisplayStringManager()->GetString( DST_JOB_GRAND_SKILL_TRAINER_MMI );
	case NPC_JOB_GRAND_SKILL_TRAINER_MWO:	return GetDisplayStringManager()->GetString( DST_JOB_GRAND_SKILL_TRAINER_MWO );
	case NPC_JOB_SUB_WEAPON_MERCHANT:		return GetDisplayStringManager()->GetString( DST_JOB_SUB_WEAPON_MERCHANT );
	case NPC_JOB_GATE_KEEPER:				return GetDisplayStringManager()->GetString( DST_JOB_GATE_KEEPER );
	case NPC_JOB_VENDING_MACHINE:			return GetDisplayStringManager()->GetString( DST_JOB_VENDING_MACHINE );
	case NPC_JOB_TIMEMACHINE_MERCHANT:		return GetDisplayStringManager()->GetString( DST_JOB_TIMEMACHINE_MERCHANT );
	case NPC_JOB_PORTAL_MAN:				return GetDisplayStringManager()->GetString( DST_JOB_POTALMAN );
	case NPC_JOB_BUS:						return GetDisplayStringManager()->GetString( DST_JOB_BUS );
	case NPC_JOB_RECEPTION:					return GetDisplayStringManager()->GetString( DST_JOB_RECEPTION );
	case NPC_JOB_BUDOHSI_MERCHANT:			return GetDisplayStringManager()->GetString( DST_JOB_BUDOHSI_MERCHANT );
	case NPC_JOB_REFEREE:					return GetDisplayStringManager()->GetString( DST_JOB_REFEREE );
	case NPC_JOB_GAMBLE_MERCHANT:			return GetDisplayStringManager()->GetString( DST_JOB_GAMBLE_MERCHANT );
	case NPC_JOB_CHAMPION_MERCHANT:			return GetDisplayStringManager()->GetString( DST_JOB_CHAMPION_MERCHANT );
	case NPC_JOB_DOJO_MANAGER:				return GetDisplayStringManager()->GetString( DST_JOB_DOJO_MANAGER );
	case NPC_JOB_DOJO_MERCHANT:				return GetDisplayStringManager()->GetString( DST_JOB_DOJO_MERCHANT );
	case NPC_JOB_DOJO_SEAL:					return GetDisplayStringManager()->GetString( DST_JOB_DOJO_SEAL );
	case NPC_JOB_DOJO_BANKER:				return GetDisplayStringManager()->GetString( DST_JOB_DOJO_BANKER );
	case NPC_JOB_MIX_MASTER:				return GetDisplayStringManager()->GetString( DST_JOB_MIX_MASTER );
	}

	static WCHAR awcBuffer[] = L"Not exist NPC job name";
	NTL_ASSERT(false, "Logic_GetNPCJobName, Not exist NPC job name of NPC job index : " << byJob);

	return awcBuffer;
}

const WCHAR* Logic_GetPlayerRaceName(RwUInt8 byRace)
{
	switch( byRace )
	{
	case RACE_HUMAN:					return GetDisplayStringManager()->GetString( DST_HUMAN );
	case RACE_NAMEK:					return GetDisplayStringManager()->GetString( DST_NAMEK );
	case RACE_MAJIN:					return GetDisplayStringManager()->GetString( DST_MAJIN );	
	}

	NTL_ASSERT(false, "Logic_GetNPCRaceName, Invalid Player race index : " << byRace);
	return NULL;
}

const WCHAR* Logic_GetNPCRaceName(RwUInt8 byRace)
{
	return Logic_GetMobRaceName(byRace);
}

const WCHAR* Logic_GetMobRaceName(RwUInt8 byRace)
{
	// 인덱스를 일일이 검사하여 인덱스의 숫자에 변경이 있을 시 Assert를 발생시켜 바로 알 수 있도록 한다
	// 또한 스트링 테이블의 순서에 영향이 없도록...
	switch( byRace )
	{
	case MOB_TYPE_ANIMAL:					return GetDisplayStringManager()->GetString( DST_MOB_TYPE_ANIMAL );
	case MOB_TYPE_HUMAN_ANIMAL:				return GetDisplayStringManager()->GetString( DST_MOB_TYPE_HUMAN_ANIMAL );
	case MOB_TYPE_DINOSAUR:					return GetDisplayStringManager()->GetString( DST_MOB_TYPE_DINOSAUR );
	case MOB_TYPE_ALIEN:					return GetDisplayStringManager()->GetString( DST_MOB_TYPE_ALIEN );
	case MOB_TYPE_ANDROID:					return GetDisplayStringManager()->GetString( DST_MOB_TYPE_ANDROID );
	case MOB_TYPE_ROBOT:					return GetDisplayStringManager()->GetString( DST_MOB_TYPE_ROBOT );
	case MOB_TYPE_DRAGON:					return GetDisplayStringManager()->GetString( DST_MOB_TYPE_DRAGON );
	case MOB_TYPE_DEVIL:					return GetDisplayStringManager()->GetString( DST_MOB_TYPE_DEVIL );
	case MOB_TYPE_UNDEAD:					return GetDisplayStringManager()->GetString( DST_MOB_TYPE_UNDEAD );
	case MOB_TYPE_PLANT:					return GetDisplayStringManager()->GetString( DST_MOB_TYPE_PLANT );
	case MOB_TYPE_INSECT:					return GetDisplayStringManager()->GetString( DST_MOB_TYPE_INSECT );
	}

	static WCHAR awcBuffer[] = L"Not exist race name";
	NTL_ASSERT(false, "Logic_GetMobRaceName, Not exist race name of race index : " << byRace);

	return awcBuffer;
}

const WCHAR* Logic_GetSkillName(RwUInt32 tblidx)
{
	static WCHAR wBuf[256];

	CSkillTable* pSkillTable = API_GetTableContainer()->GetSkillTable();
	CTextTable* pSkillTextTable = API_GetTableContainer()->GetTextAllTable()->GetSkillTbl();

	sSKILL_TBLDAT* pSkillData = reinterpret_cast<sSKILL_TBLDAT*>( pSkillTable->FindData( tblidx ) );
	sTEXT_TBLDAT* pTextData = NULL;
	if( pSkillData )
	{
		pTextData = reinterpret_cast<sTEXT_TBLDAT*>( pSkillTextTable->FindData( pSkillData->Skill_Name ) );
		if( pTextData )
		{
			return pTextData->wstrText.c_str();
		}
		else
			swprintf_s( wBuf, 256, L"Invalid Name Text Index : %u", pSkillData->Skill_Name );
	}
	else
		swprintf_s( wBuf, 256, L"Invalid Skill Table Index : %u", tblidx );
	
	return wBuf;
}	

const WCHAR* Logic_GetHTBName(RwUInt32 tblidx)
{
	static WCHAR wBuf[256];

	CHTBSetTable* pHTBTable = API_GetTableContainer()->GetHTBSetTable();
	CTextTable* pHTBTextTable = API_GetTableContainer()->GetTextAllTable()->GetHTBSetTbl();

	sHTB_SET_TBLDAT* pHTBData = reinterpret_cast<sHTB_SET_TBLDAT*>( pHTBTable->FindData( tblidx ) );
	sTEXT_TBLDAT* pTextData = NULL;
	if( pHTBData )
	{
		pTextData = reinterpret_cast<sTEXT_TBLDAT*>( pHTBTextTable->FindData( pHTBData->HTB_Skill_Name ) );
		if( pTextData )
		{
			return pTextData->wstrText.c_str();
		}
		else
			swprintf_s( wBuf, 256, L"Invalid Name Text Index : %u", pHTBData->HTB_Skill_Name );
	}
	else
		swprintf_s( wBuf, 256, L"Invalid Skill Table Index : %u", tblidx );

	return wBuf;
}

const WCHAR* Logic_GetItemName(RwUInt32 tblidx)
{
	static WCHAR wBuf[256];

	CItemTable* pItemTable = API_GetTableContainer()->GetItemTable();
	CTextTable* pItemTextTable = API_GetTableContainer()->GetTextAllTable()->GetItemTbl();

	sITEM_TBLDAT* pItemData = reinterpret_cast<sITEM_TBLDAT*>( pItemTable->FindData( tblidx ) );
	sTEXT_TBLDAT* pTextData = NULL;
	if( pItemData )
	{
		pTextData = reinterpret_cast<sTEXT_TBLDAT*>( pItemTextTable->FindData( pItemData->Name ) );
		if( pTextData )
		{
			return pTextData->wstrText.c_str();
		}
		else
			swprintf_s( wBuf, 256, L"Invalid Name Text Index : %u", pItemData->Name );
	}
	else
		swprintf_s( wBuf, 256, L"Invalid Item Table Index : %u", tblidx );

	return wBuf;
}

RwBool	Logic_GetSystemEffectText( RwUInt32 uiSystemEffectIdx, RwReal fValue, std::wstring& wstrText, RwUInt8 byPercentType, sSKILL_TBLDAT* pSkillData /* = NULL */, RwUInt32* pColor /* = NULL  */, RwBool* pBuffShow /* = NULL */ )
{
	if( uiSystemEffectIdx == INVALID_TBLIDX )
		return FALSE;

	CSystemEffectTable* pSystemEffectTable = API_GetTableContainer()->GetSystemEffectTable();
	sSYSTEM_EFFECT_TBLDAT* pSystemEffectData = reinterpret_cast<sSYSTEM_EFFECT_TBLDAT*>( pSystemEffectTable->FindData( uiSystemEffectIdx ) );		

	if( !pSystemEffectData ) 
	{
		if( Logic_IsUIDevInfoVisible() )
		{
			WCHAR strError[256];
			swprintf_s( strError, 256, L" (%u) SystemEffectError", uiSystemEffectIdx );
			wstrText = strError;		
		}					

		return FALSE;
	}

	// Buff의 경우 Direct계열은 출력안함
	if( pBuffShow )
	{
		if( pSystemEffectData->byActive_Effect_Type == SKILL_ACTIVE_TYPE_DD ||
			pSystemEffectData->byActive_Effect_Type == SKILL_ACTIVE_TYPE_DH ||
			pSystemEffectData->byActive_Effect_Type == SKILL_ACTIVE_TYPE_DB ||
			pSystemEffectData->byActive_Effect_Type == SKILL_ACTIVE_TYPE_DC )
		{
			*pBuffShow = FALSE;
			return TRUE;
		}

		*pBuffShow = TRUE;
	}

	CTextTable* pSystemEffectText = API_GetTableContainer()->GetTextAllTable()->GetSystemEffectTbl();
	wstrText = pSystemEffectText->GetText( pSystemEffectData->Effect_Info_Text );

	WCHAR buf[256];

	// Excpetion : 아이템 업그레이드 파츠 Value예외
	//if( pSystemEffectData->effectCode == ACTIVE_ITEM_UPGRADE_RATE_UP )
	//{
	//	fValue = fValue / 100.0f +1.0f;
	//	swprintf_s( buf, 256, wstrText.c_str(), fValue );
	//	wstrText = buf;
	//	return TRUE;
	//}

	// Excpetion : 스킬 타입 표기 예외
	if( pSkillData )
	{
		if( pSystemEffectData->effectCode == ACTIVE_DIRECT_DAMAGE ||
			pSystemEffectData->effectCode == ACTIVE_BACKSTAB ||
			pSystemEffectData->effectCode == ACTIVE_WARP_BACK_ATTACK ||
			pSystemEffectData->effectCode == ACTIVE_THROUGH_ATTACK )
		{
			switch( pSkillData->bySkill_Type )
			{
			case NTL_SKILL_TYPE_PHYSICAL: 
				wstrText = GetDisplayStringManager()->GetString( DST_SYSTEMEFFECT_PHYSICAL ) + wstrText;
				if( pColor ) *pColor = INFOCOLOR_6;
				break;
			case NTL_SKILL_TYPE_ENERGY:
				wstrText = GetDisplayStringManager()->GetString( DST_SYSTEMEFFECT_ENERGY ) + wstrText;
				if( pColor ) *pColor = INFOCOLOR_3;
				break;
			case NTL_SKILL_TYPE_STATE:
				wstrText = GetDisplayStringManager()->GetString( DST_SYSTEMEFFECT_STATE ) + wstrText;
				if( pColor ) *pColor = INFOCOLOR_7;
				break;
			}	
		}
	}
	WCHAR* szFormat = L"%d";
	RwInt32 nInsertPosition = wstrText.find( szFormat );
	RwBool bINTValue = TRUE;

	if( (RwUInt32)nInsertPosition == std::wstring::npos )
	{
		szFormat = L"%g";
		nInsertPosition = wstrText.find( szFormat );
		bINTValue = FALSE;
	}	

	// Exception : 단위 관련 표기 예외
	if( byPercentType )
	{
		if( nInsertPosition >= 0 )
			wstrText.insert( nInsertPosition + 2, L"%%" );		

		if( bINTValue )
			swprintf_s( buf, 256, wstrText.c_str(), (RwInt32)fValue );
		else
			swprintf_s( buf, 256, wstrText.c_str(), fValue );
	}
	else
	{
		if( pSystemEffectData->effectCode == PASSIVE_SKILL_CASTING_TIME_DOWN ||
			pSystemEffectData->effectCode == ACTIVE_PARALYZE_KEEPTIME_DOWN ||
			pSystemEffectData->effectCode == ACTIVE_TERROR_KEEPTIME_DOWN ||
			pSystemEffectData->effectCode == ACTIVE_CONFUSE_KEEPTIME_DOWN ||
			pSystemEffectData->effectCode == ACTIVE_STONE_KEEPTIME_DOWN ||
			pSystemEffectData->effectCode == ACTIVE_CANDY_KEEPTIME_DOWN ||
			pSystemEffectData->effectCode == ACTIVE_BLEEDING_KEEPTIME_DOWN ||
			pSystemEffectData->effectCode == ACTIVE_POISON_KEEPTIME_DOWN ||
			pSystemEffectData->effectCode == ACTIVE_STOMACHACHE_KEEPTIME_DOWN )
		{
			if( nInsertPosition >= 0 )
				wstrText.replace( nInsertPosition, 2, L"%s" );

			swprintf_s( buf, 256, wstrText.c_str(), Logic_GetTimeString( fValue ) );			
		}	
		else if( pSystemEffectData->effectCode == PASSIVE_APPLY_AREA_SIZE_UP || 
				 pSystemEffectData->effectCode == PASSIVE_USE_RANGE_UP ||
				 pSystemEffectData->effectCode == ACTIVE_ATTACK_RANGE_UP ||
				 pSystemEffectData->effectCode == ACTIVE_ATTACK_RANGE_DOWN )
		{
			if( nInsertPosition >= 0 )
				wstrText.insert( nInsertPosition + 2, GetDisplayStringManager()->GetString( DST_SYSTEMEFFECT_METER ) );		

			if( bINTValue )
				swprintf_s( buf, 256, wstrText.c_str(), (RwInt32)fValue );			
			else
				swprintf_s( buf, 256, wstrText.c_str(), fValue );			
		}
		else if( pSystemEffectData->effectCode == PASSIVE_SKILL_KEEP_TIME_UP ||
			     pSystemEffectData->effectCode == PASSIVE_SKILL_COOL_TIME_DOWN )
		{
			if( pSkillData && pSkillData->Root_Skill != INVALID_TBLIDX )
			{
				if( nInsertPosition >= 0 )
					wstrText.replace( nInsertPosition, 2, L"%s" );

				swprintf_s( buf, 256, wstrText.c_str(), Logic_GetTimeString( fValue ) );			
			}
			else
			{
				if( nInsertPosition >= 0 )
					wstrText.insert( nInsertPosition + 2, L"%%" );		

				if( bINTValue )
					swprintf_s( buf, 256, wstrText.c_str(), (RwInt32)fValue );
				else
					swprintf_s( buf, 256, wstrText.c_str(), fValue );
			}
		}	
		else if( pSystemEffectData->effectCode == PASSIVE_REQUIRE_EP_DOWN )
		{
			if( pSkillData && pSkillData->Root_Skill != INVALID_TBLIDX )
			{
				if( bINTValue )
					swprintf_s( buf, 256, wstrText.c_str(), (RwInt32)fValue );
				else
					swprintf_s( buf, 256, wstrText.c_str(), fValue );
			}
			else
			{
				if( nInsertPosition >= 0 )
					wstrText.insert( nInsertPosition + 2, L"%%" );		

				if( bINTValue )
					swprintf_s( buf, 256, wstrText.c_str(), (RwInt32)fValue );
				else
					swprintf_s( buf, 256, wstrText.c_str(), fValue );
			}
		}
		else
		{
			if( bINTValue )
				swprintf_s( buf, 256, wstrText.c_str(), (RwInt32)fValue );
			else
				swprintf_s( buf, 256, wstrText.c_str(), fValue );
		}
	}

	wstrText = buf;	

	return TRUE;
}

void Logic_DialogModeMessage(eDialogMode eMode)
{
	switch(eMode)
	{
	case DIALOGMODE_ITEM_REPAIR:
		{
			GetAlarmManager()->AlarmMessage(DST_DIALOG_MODE_ITEM_REPAIR);
			break;
		}
	case DIALOGMODE_ITEM_IDENTIFICATION:
	case DIALOGMODE_NPCSHOP_ITEM_IDENTIFICATION:
		{
			GetAlarmManager()->AlarmMessage(DST_DIALOG_MODE_ITEM_IDENTIFICATION);
			break;
		}
	case DIALOGMODE_NARRATION:
		{
			GetAlarmManager()->AlarmMessage(DST_DIALOG_MODE_NARRATION);
			break;
		}
	case DIALOGMODE_CHATTING_RESIZE_HORI:
		{
			GetAlarmManager()->AlarmMessage(DST_DIALOG_MODE_CHAT_RESIZING);
			break;
		}
	case DIALOGMODE_CHATTING_RESIZE_RIGHTUP:
		{
			GetAlarmManager()->AlarmMessage(DST_DIALOG_MODE_CHAT_RESIZING);
			break;
		}
	case DIALOGMODE_CHATTING_RESIZE_RIGHTDOWN:
		{
			GetAlarmManager()->AlarmMessage(DST_DIALOG_MODE_CHAT_RESIZING);
			break;
		}
	case DIALOGMODE_CHATTING_RESIZE_VERT:
		{
			GetAlarmManager()->AlarmMessage(DST_DIALOG_MODE_CHAT_RESIZING);
			break;
		}
	default:
		{
			NTL_ASSERT(false, "Wrong dialog mode : " << eMode);
		}
	}
}

RwUInt32 Logic_GetWarnningWorldConceptActionMessageIndex(RwUInt32 uiGrade)
{
	if( uiGrade == WORLD_CONCEPT_FIRST_GRADE )
	{
		CNtlWorldConceptController *pController = GetNtlWorldConcept()->FindGradeExtController(WORLD_CONCEPT_FIRST_GRADE);
		if(pController == NULL)
			return 0xffffffff;

		switch(pController->GetConceptType())
		{
		case WORLD_PLAY_TUTORIAL:
			{
				return DST_WORLD_CONCEPT_TUTORIAL;
			}
		case WORLD_PLAY_TIME_MACHINE:
			{
				return DST_WORLD_CONCEPT_TMQ;
			}
		case WORLD_PLAY_RANK_BATTLE:
			{
				return DST_WORLD_CONCEPT_RANK_BATTLE;
			}
		case WORLD_PLAY_T_BUDOKAI:
			{
				return DST_WORLD_CONCEPT_BUDOKAI;
			}
		case WORLD_PLAY_PARTY_DUNGEON:
			{
				return DST_WORLD_CONCEPT_PARTY_DUNGEON;
			}
		case WORLD_PLAY_TLQ:
			{
				return DST_WORLD_CONCEPT_TLQ;
			}
		case WORLD_PLAY_DOJO_SCRAMBLE:
			{
				return DST_WORLD_CONCEPT_DOJO_SCRAMBLE;
			}
		default:
			{
				DBO_FAIL("Invalid world concept");
			}
		}
	}
	else if( uiGrade == WORLD_CONCEPT_SECOND_GRADE )
	{
		CNtlWorldConceptController *pController = GetNtlWorldConcept()->FindGradeExtController(WORLD_CONCEPT_SECOND_GRADE);
		if(pController == NULL)
			return 0xffffffff;

		switch(pController->GetConceptType())
		{
		case WORLD_PLAY_FREEPVP:
			{
				return DST_WORLD_CONCEPT_FREEBATTLE;
			}
		case WORLD_PLAY_NPC_COMMU:
			{
				return DST_WORLD_CONCEPT_NPC_COMMUNICATION;
			}
		case WORLD_PLAY_TRADE:
			{
				return DST_WORLD_CONCEPT_TRADE;
			}
		case WORLD_PLAY_DRAGONBALL_COLLECT:
			{
				return DST_WORLD_CONCEPT_PLAY_DRAGONBALL;
			}
		case WORLD_PLAY_PRIVATESHOP:
			{
				return DST_WORLD_CONCEPT_PLAY_PRIVATESHOP;
			}
		case WORLD_PLAY_PRIVATESHOP_VISITOR:
			{
				return DST_WORLD_CONCEPT_PLAY_PRIVATESHOP_VISITOR;
			}
		case WORLD_PLAY_DIRECT:
			{
				return DST_WORLD_CONCEPT_PLAY_DIRECT;
			}
		default:
			{
				DBO_FAIL("Invalid world concept");
			}
		}
	}

	return 0xffffffff;
}

void Logic_WarnningWorldConceptActionMessage(RwUInt32 uiGrade)
{
	RwUInt32 uiStringIndex = Logic_GetWarnningWorldConceptActionMessageIndex(uiGrade);

	if( uiStringIndex != 0xffffffff )
		GetAlarmManager()->AlarmMessage(uiStringIndex);
}

// 테이블 인덱스로 지역의 이름을 가져온다.
// return FALSE면 테이블에 없음
RwBool Logic_GetAreaNameFromTblIDX( TBLIDX tblIdx, std::wstring* pstrString ) 
{
	CMapNameTextTable* pTable = API_GetTableContainer()->GetTextAllTable()->GetMapNameTbl();
	if(!pTable->GetAreaName(tblIdx, pstrString))
	{
		if(!pTable->GetSectorName(tblIdx, pstrString))
		{
			if(!pTable->GetZoneName(tblIdx, pstrString))
			{
				// 텍스트 테이블이 존재하지 않는다
				static WCHAR awcBuffer[] = L"Not exist terrain name";
				*pstrString = awcBuffer;
				return FALSE;
			}
		}
	}

	return TRUE;
}

const WCHAR* Logic_GetBattleAttributeName(RwUInt8 byAttr)
{
	switch( byAttr )
	{
	case BATTLE_ATTRIBUTE_NONE:				return GetDisplayStringManager()->GetString( DST_ITEM_ATTR_NONE );
	case BATTLE_ATTRIBUTE_HONEST:			return GetDisplayStringManager()->GetString( DST_ITEM_ATTR_HONEST );
	case BATTLE_ATTRIBUTE_STRANGE:			return GetDisplayStringManager()->GetString( DST_ITEM_ATTR_STRANGE );
	case BATTLE_ATTRIBUTE_WILD:				return GetDisplayStringManager()->GetString( DST_ITEM_ATTR_WILD );
	case BATTLE_ATTRIBUTE_ELEGANCE:			return GetDisplayStringManager()->GetString( DST_ITEM_ATTR_ELEGANCE );
	case BATTLE_ATTRIBUTE_FUNNY:			return GetDisplayStringManager()->GetString( DST_ITEM_ATTR_FUNNY );
	case BATTLE_ATTRIBUTE_UNKNOWN:			return L"";
	}

	static WCHAR awcBuffer[] = L"Not exist attribute name";
	NTL_ASSERT(false, "Logic_GetBattleAttributeName, Not exist attribute name of attribute index : " << byAttr);

	return awcBuffer;
}

void Logic_timeToDetailTime(time_t tTime, sDetailTime& sTime)
{
	tm curTime;

	localtime_s(&curTime, &tTime);

	sTime.uiYear		= curTime.tm_year + 1900;
	sTime.byMonth		= (RwUInt8)(curTime.tm_mon + 1);
	sTime.byDay			= (RwUInt8)curTime.tm_mday;
	sTime.byHour		= (RwUInt8)curTime.tm_hour;
	sTime.byMinute		= (RwUInt8)curTime.tm_min;
	sTime.bySecond		= (RwUInt8)curTime.tm_sec;


	switch(curTime.tm_wday)
	{
	case 0:			sTime.strDay = GetDisplayStringManager()->GetString(DST_SUNDAY);		break;
	case 1:			sTime.strDay = GetDisplayStringManager()->GetString(DST_MONDAY);		break;
	case 2:			sTime.strDay = GetDisplayStringManager()->GetString(DST_TUESDAY);		break;
	case 3:			sTime.strDay = GetDisplayStringManager()->GetString(DST_WEDNESDAY);		break;
	case 4:			sTime.strDay = GetDisplayStringManager()->GetString(DST_THURSDAY);		break;
	case 5:			sTime.strDay = GetDisplayStringManager()->GetString(DST_FRIDAY);		break;
	case 6:			sTime.strDay = GetDisplayStringManager()->GetString(DST_SATURDAY);		break;	
	default:		NTL_ASSERT(false, "Logic_timeToDetailTime, localtime_s function return wrong value : " << curTime.tm_wday);
	}


	sTime.byDay_of_Year	= (RwUInt8)(curTime.tm_yday + 1);
}

WCHAR* Logic_GetTimeString(RwReal fSecond)
{
	static WCHAR awcBuffer[256] = L"";
	RwUInt32 uiDay, uiHour, uiMinute, uiSecond;
	RwReal fLeftSecond = fSecond;

	uiDay = uiHour = uiMinute = uiSecond = 0;
	memset(awcBuffer, 0, 64);


	// 시간 계산
	uiDay = (RwUInt32)(fLeftSecond / 86400.f);

	fLeftSecond -= (RwReal)uiDay * 86400.f;
	uiHour = (RwUInt32)(fLeftSecond / 3600.f);

	fLeftSecond -= (RwReal)uiHour * 3600.f;
	uiMinute = (RwUInt32)(fLeftSecond / 60.f);

	fLeftSecond -= (RwReal)uiMinute * 60.f;
	uiSecond = (RwUInt32)fLeftSecond;
	

	// 시간 스트링 조합
	if( uiDay > 0 )
	{
		const WCHAR* pwcText = GetDisplayStringManager()->GetString(DST_TIME_DAY);
		swprintf_s(awcBuffer, 64, L"%u%s", uiDay, pwcText);
	}

	if( uiHour > 0 )
	{
		const WCHAR* pwcText = GetDisplayStringManager()->GetString(DST_TIME_AN_HOUR);
		RwUInt32 uiBufferStart = wcslen(awcBuffer);

		// 이미 스트링이 있다. 빈칸을 확보하자
		if( uiBufferStart > 0 )
		{
			swprintf_s(awcBuffer + uiBufferStart, 64 - uiBufferStart, L" ");
			++uiBufferStart;
		}

		swprintf_s(awcBuffer + uiBufferStart, 64 - uiBufferStart, L"%u%s", uiHour, pwcText);
	}

	if( uiMinute > 0 )
	{
		const WCHAR* pwcText	= GetDisplayStringManager()->GetString(DST_TIME_MINUTE);
		RwUInt32 uiBufferStart = wcslen(awcBuffer);

		// 이미 스트링이 있다. 빈칸을 확보하자
		if( uiBufferStart > 0 )
		{
			swprintf_s(awcBuffer + uiBufferStart, 64 - uiBufferStart, L" ");
			++uiBufferStart;
		}

		swprintf_s(awcBuffer + uiBufferStart, 64 - uiBufferStart, L"%u%s", uiMinute, pwcText);
	}

	if( uiSecond > 0 ||
		( !uiDay && !uiHour && !uiMinute ) )
	{
		const WCHAR* pwcText	= GetDisplayStringManager()->GetString(DST_TIME_SECOND);
		RwUInt32 uiBufferStart = wcslen(awcBuffer);

		// 이미 스트링이 있다. 빈칸을 확보하자
		if( uiBufferStart > 0 )
		{
			swprintf_s(awcBuffer + uiBufferStart, 64 - uiBufferStart, L" ");
			++uiBufferStart;
		}

		swprintf_s(awcBuffer + uiBufferStart, 64 - uiBufferStart, L"%u%s", uiSecond, pwcText);
	}
	
	return awcBuffer;
}

WCHAR* Logic_GetCoolTimeString(RwReal fSecond)
{
	static WCHAR awcBuffer[64] = L"";	

	if( fSecond <= 60.f )
	{
		const WCHAR* pwcText	= GetDisplayStringManager()->GetString(DST_TIME_SECOND);

		swprintf_s(awcBuffer, 64, L"%u%s", (RwUInt32)fSecond, pwcText);
	}
	else if( fSecond < 3600.f )
	{
		const WCHAR* pwcText	= GetDisplayStringManager()->GetString(DST_TIME_MINUTE);
		RwUInt32 uiMinute		= (RwUInt32)(fSecond / 60.f) + 1;

		swprintf_s(awcBuffer, 64, L"%u%s", uiMinute, pwcText);
	}
	else if( fSecond <= 86400.f )
	{
		const WCHAR* pwcText	= GetDisplayStringManager()->GetString(DST_TIME_AN_HOUR);
		RwUInt32 uiHour		= (RwUInt32)(fSecond / 3600.f);

		RwReal fLeftSecond = fSecond - ((RwReal)uiHour * 3600.f);
		RwUInt32 uiMinute = (RwUInt32)(fLeftSecond / 60.f);

		if( uiMinute > 0 )
		{
			const WCHAR* pwcText2 = GetDisplayStringManager()->GetString(DST_TIME_MINUTE);
			swprintf_s(awcBuffer, 64, L"%u%s %d%s", uiHour, pwcText, uiMinute, pwcText2);
		}
		else
			swprintf_s(awcBuffer, 64, L"%u%s", uiHour, pwcText);
	}
	else
	{
		const WCHAR* pwcText	= GetDisplayStringManager()->GetString(DST_TIME_DAY);
		RwUInt32 uiDay		= (RwUInt32)(fSecond / 86400.f);

		RwReal fLeftSecond = fSecond - ((RwReal)uiDay * 86400.f);
		RwUInt32 uiHour = (RwUInt32)(fLeftSecond / 3600.f);

		if( uiHour > 0 )
		{
			const WCHAR* pwcText2 = GetDisplayStringManager()->GetString(DST_TIME_AN_HOUR);
			swprintf_s(awcBuffer, 64, L"%u%s %u%s", uiDay, pwcText, uiHour, pwcText2);
		}
		else
			swprintf_s(awcBuffer, 64, L"%u%s", uiDay, pwcText);
	}

	return awcBuffer;
}

/**
* \brief 천하제일 무도회의 매치 스테이트를 스트링으로 반환한다.
*/
const WCHAR* Logic_GetStringTBMatchType( RwUInt8 byTBMatchType )
{
	if( byTBMatchType == BUDOKAI_MATCH_TYPE_INDIVIDIAUL )
	{
		return GetDisplayStringManager()->GetString( DST_BUDOKAI_NOTICE_SOLO );
	}
	else if( byTBMatchType == BUDOKAI_MATCH_TYPE_TEAM )
	{
		return GetDisplayStringManager()->GetString( DST_BUDOKAI_NOTICE_PARTY );
	}
	
	static WCHAR awcBuffer[] = L"Invalid Budokai Match Type(INDI/TEAM)";
	return awcBuffer;
}

const WCHAR* Logic_GetStringTBMatchState( RwUInt8 byTBMatchState )
{
	switch( byTBMatchState )
	{
	case BUDOKAI_MATCHSTATE_REGISTER:
		return GetDisplayStringManager()->GetString( DST_BUDOKAI_REGISTER_INFO_TITLE ); 				// 등록 기간
	case BUDOKAI_MATCHSTATE_WAIT_MINOR_MATCH:		// 예선전 대기
		return GetDisplayStringManager()->GetString( DST_BUDOKAI_MINORMATCH_INFO_TITLE_WAIT );
	case BUDOKAI_MATCHSTATE_MINOR_MATCH:				// 예선전 
		return GetDisplayStringManager()->GetString( DST_BUDOKAI_MINORMATCH_INFO_TITLE_BATTLE );
	case BUDOKAI_MATCHSTATE_WAIT_MAJORMATCH_32:
		return GetDisplayStringManager()->GetString( DST_BUDOKAI_MAJORMATCH_INFO_TITLE_32_WAIT );
	case BUDOKAI_MATCHSTATE_MAJORMATCH_32:
		return GetDisplayStringManager()->GetString( DST_BUDOKAI_MAJORMATCH_INFO_TITLE_32 );
	case BUDOKAI_MATCHSTATE_WAIT_MAJORMATCH_16:
		return GetDisplayStringManager()->GetString( DST_BUDOKAI_MAJORMATCH_INFO_TITLE_16_WAIT );
	case BUDOKAI_MATCHSTATE_MAJORMATCH_16:
		return GetDisplayStringManager()->GetString( DST_BUDOKAI_MAJORMATCH_INFO_TITLE_16 );
	case BUDOKAI_MATCHSTATE_WAIT_MAJORMATCH_08:
		return GetDisplayStringManager()->GetString( DST_BUDOKAI_MAJORMATCH_INFO_TITLE_8_WAIT );
	case BUDOKAI_MATCHSTATE_MAJORMATCH_08:
		return GetDisplayStringManager()->GetString( DST_BUDOKAI_MAJORMATCH_INFO_TITLE_8 );
	case BUDOKAI_MATCHSTATE_WAIT_SEMIFINAL_MATCH:	// 준결승 대기
		return GetDisplayStringManager()->GetString( DST_BUDOKAI_MAJORMATCH_INFO_TITLE_SEMIFINAL_WAIT );
	case BUDOKAI_MATCHSTATE_SEMIFINAL_MATCH:
		return GetDisplayStringManager()->GetString( DST_BUDOKAI_MAJORMATCH_INFO_TITLE_SEMIFINAL );
	case BUDOKAI_MATCHSTATE_FINAL_MATCH:
		return GetDisplayStringManager()->GetString( DST_BUDOKAI_MAJORMATCH_INFO_TITLE_FINAL );
	case BUDOKAI_MATCHSTATE_MATCH_END:
		return GetDisplayStringManager()->GetString( DST_BUDOKAI_MAJORMATCH_INFO_TITLE_END );
	default:
		break;
	}

	static WCHAR awcBuffer[] = L"Invalid Budokai Match State(REGISTER ~ END)";
	return awcBuffer;
}

/**
* \brief 현재 아바타의 참가 상태를 스트링으로 가져온다. ( 예선 진출자, 16강 진출자, 탈락자 등등 )
*/
const WCHAR* Logic_GetAvatarTBJoinState()
{
	CNtlSobAvatar* pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();

	sBUDOKAI_JOIN_INFO* pJoinInfo;

	// NULL 일 때는 아바타 생성 전에 정보가 들어왔을 경우
	// NULL이 아닐 때는 정상적으로 데이터가 셋팅 되었을 경우
	if( pSobAvatar == NULL )
		pJoinInfo = &GetNtlSLGlobal()->GetTBudokaiStateInfo()->sJoinInfo;
	else
		pJoinInfo = pSobAvatar->GetTenkaichiBudokai()->GetBudokaiJoinInfo();	
	
	if( pJoinInfo->byJoinState == INVALID_BUDOKAI_JOIN_STATE )
	{
		return GetDisplayStringManager()->GetString( DST_BUDOKAI_PC_STATE_NONE );
	}
	else
	{
		if( pJoinInfo->byJoinState == BUDOKAI_JOIN_STATE_DROPOUT )
		{
			switch( pJoinInfo->byJoinResult )
			{
			case BUDOKAI_JOIN_RESULT_REGISTER:
			case BUDOKAI_JOIN_RESULT_MINORMATCH:
				return GetDisplayStringManager()->GetString( DST_BUDOKAI_PC_STATE_MINOR_DROPOUT );
				break;
			case BUDOKAI_JOIN_RESULT_ENTER_32:
				return GetDisplayStringManager()->GetString( DST_BUDOKAI_PC_STATE_32_DROPOUT );
				break;
			case BUDOKAI_JOIN_RESULT_ENTER_16:
				return GetDisplayStringManager()->GetString( DST_BUDOKAI_PC_STATE_16_DROPOUT );
				break;
			case BUDOKAI_JOIN_RESULT_ENTER_8:
				return GetDisplayStringManager()->GetString( DST_BUDOKAI_PC_STATE_8_DROPOUT );
				break;
			case BUDOKAI_JOIN_RESULT_ENTER_4:
				return GetDisplayStringManager()->GetString( DST_BUDOKAI_PC_STATE_PRIZE_WINNER );
				break;
			case BUDOKAI_JOIN_RESULT_ENTER_2:
				return GetDisplayStringManager()->GetString( DST_BUDOKAI_PC_STATE_SEMIFINAL_WINNER );
				break;
			case BUDOKAI_JOIN_RESULT_WINNER:
				return GetDisplayStringManager()->GetString( DST_BUDOKAI_PC_STATE_FINAL_WINNER );
				break;
			default:
				break;
			}	
		}
		else if( pJoinInfo->byJoinState == BUDOKAI_JOIN_STATE_PLAY )
		{
			switch( pJoinInfo->byJoinResult )
			{
			case BUDOKAI_JOIN_RESULT_REGISTER:
				return GetDisplayStringManager()->GetString( DST_BUDOKAI_PC_STATE_MINOR_APPLICANT );
				break;
			case BUDOKAI_JOIN_RESULT_MINORMATCH:
				return GetDisplayStringManager()->GetString( DST_BUDOKAI_PC_STATE_MINOR_ENTRY );
				break;
			case BUDOKAI_JOIN_RESULT_ENTER_32:
				return GetDisplayStringManager()->GetString( DST_BUDOKAI_PC_STATE_32_ENTRY );
				break;
			case BUDOKAI_JOIN_RESULT_ENTER_16:
				return GetDisplayStringManager()->GetString( DST_BUDOKAI_PC_STATE_16_ENTRY );
				break;
			case BUDOKAI_JOIN_RESULT_ENTER_8:
				return GetDisplayStringManager()->GetString( DST_BUDOKAI_PC_STATE_8_ENTRY );
				break;
			case BUDOKAI_JOIN_RESULT_ENTER_4:
				return GetDisplayStringManager()->GetString( DST_BUDOKAI_PC_STATE_SEMIFINAL_ENTRY );
				break;
			case BUDOKAI_JOIN_RESULT_ENTER_2:
				return GetDisplayStringManager()->GetString( DST_BUDOKAI_PC_STATE_FINAL_ENTRY );
				break;
			case BUDOKAI_JOIN_RESULT_WINNER:
				return GetDisplayStringManager()->GetString( DST_BUDOKAI_PC_STATE_FINAL_WINNER );
				break;
			default:
				break;
			}
		}
	}

	return GetDisplayStringManager()->GetString( DST_BUDOKAI_PC_STATE_NONE );
}

/**
* \brief 천하제일 무도회 64 비트 정수 시간에 관련된 스트링 ( 단위에 따라서 다르게 리턴된다. )
*/
const WCHAR* Logic_GetTBRemainTimeString( BUDOKAITIME tmTime )
{
	static WCHAR awcBuffer[256] = L"";
	
	RwReal fDay = (RwReal)tmTime / 86400.f;
	if( fDay >= 1.f )
	{
		swprintf_s( awcBuffer, 256, GetDisplayStringManager()->GetString( DST_BUDOKAI_TIME_DAY ), (RwInt32)fDay );
		
		return awcBuffer;
	}

	RwReal fHour = (RwReal)tmTime / 3600.f;
	if( fHour >= 1.f )
	{
		swprintf_s( awcBuffer, 256, GetDisplayStringManager()->GetString( DST_BUDOKAI_TIME_HOUR ), (RwInt32)fHour );

		return awcBuffer;
	}

	RwReal fMinute = (RwReal)tmTime / 60.f;
	if( fMinute >= 1.f )
	{
		swprintf_s( awcBuffer, 256, GetDisplayStringManager()->GetString( DST_BUDOKAI_TIME_MINUTE ), (RwInt32)fMinute );

		return awcBuffer;
	}

	swprintf_s( awcBuffer, 256, GetDisplayStringManager()->GetString( DST_BUDOKAI_TIME_SECOND ), tmTime );
	
	return  awcBuffer;
}

const WCHAR* Logic_GetTBDateToDateString( tm tmAtTime, tm tmToTime )
{
	WCHAR awcBuffer[256];
	WCHAR awcBuffer2[256];

	static WCHAR awcBufferResult[512];

	// 도장 추천 기간
	// 예선전 기간
	swprintf_s( awcBuffer, 128, GetDisplayStringManager()->GetString( DST_BUDOKAI_NEWS_NOTICE_GUIDE_TIME),
		tmAtTime.tm_mon+1, tmAtTime.tm_mday, 
		(tmAtTime.tm_hour >= 12 ? GetDisplayStringManager()->GetString( DST_BUDOKAI_NEWS_NOTICE_GUIDE_TIME_PM ) : GetDisplayStringManager()->GetString( DST_BUDOKAI_NEWS_NOTICE_GUIDE_TIME_AM ) ),
		(tmAtTime.tm_hour > 12 ? (tmAtTime.tm_hour) - 12 : tmAtTime.tm_hour ),
		tmAtTime.tm_min );

	swprintf_s( awcBuffer2, 128, GetDisplayStringManager()->GetString( DST_BUDOKAI_NEWS_NOTICE_GUIDE_TIME),
		tmToTime.tm_mon+1, tmToTime.tm_mday, 
		(tmToTime.tm_hour >= 12 ? GetDisplayStringManager()->GetString( DST_BUDOKAI_NEWS_NOTICE_GUIDE_TIME_PM ) : GetDisplayStringManager()->GetString( DST_BUDOKAI_NEWS_NOTICE_GUIDE_TIME_AM ) ),
		(tmToTime.tm_hour > 12 ? (tmToTime.tm_hour) - 12 : tmToTime.tm_hour ),
		tmToTime.tm_min );

	swprintf_s( awcBufferResult, 256, GetDisplayStringManager()->GetString( DST_BUDOKAI_NEWS_NOTICE_GUIDE_TIME_FULL ),
		awcBuffer, awcBuffer2 );
	
	return awcBufferResult;
}

const WCHAR* Logic_GetTBTypeString( RwUInt8 byMainState )
{
	static WCHAR awcBuffer[16] = L"";

	if( GetBudokaiType(byMainState) == BUDOKAI_TYPE_JUNIOR )
	{
		return GetDisplayStringManager()->GetString( DST_BUDOKAI_TYPE_CHILD_TITLE );
	}
	else
	{
		return GetDisplayStringManager()->GetString( DST_BUDOKAI_TYPE_ADULT_TITLE );
	}

	return awcBuffer;
}

const WCHAR* Logic_GetTBDateString( tm tmTime )
{
	static WCHAR awcBuffer[128];

	swprintf_s( awcBuffer, 128, GetDisplayStringManager()->GetString( DST_BUDOKAI_NEWS_NOTICE_GUIDE_TIME),
		tmTime.tm_mon+1, tmTime.tm_mday, 
		(tmTime.tm_hour >= 12 ? GetDisplayStringManager()->GetString( DST_BUDOKAI_NEWS_NOTICE_GUIDE_TIME_PM ) : GetDisplayStringManager()->GetString( DST_BUDOKAI_NEWS_NOTICE_GUIDE_TIME_AM ) ),
		(tmTime.tm_hour > 12 ? (tmTime.tm_hour) - 12 : tmTime.tm_hour ),
		tmTime.tm_min );

	return awcBuffer;
}

const WCHAR* Logic_GetMailRemainTimeString( DBOTIME tmTime )
{
	static WCHAR awcBuffer[256] = L"";

	DBOTIME serverTime = GetNtlSLGlobal()->GetServerTime();

	tm tmServerTime;

	errno_t errid = _localtime64_s( &tmServerTime, (__time64_t*)&serverTime );
	if( 0 != errid )
	{
		DBO_FAIL( "Logic_GetMailRemainTimeString - "<< serverTime );
		return awcBuffer;
	}

	DBOTIME remainTime = tmTime - serverTime; 
	if( remainTime <= 0 )
	{
		swprintf_s( awcBuffer, 256, GetDisplayStringManager()->GetString( DST_BUDOKAI_TIME_MINUTE ), 0. );
		return awcBuffer;
	}

	tm tmRemainTime;
	
	errid = _localtime64_s( &tmRemainTime, (__time64_t*)&remainTime );
	if( 0 != errid )
	{
		DBO_FAIL( "Logic_GetMailRemainTimeString - "<< remainTime );
		return awcBuffer;
	}

	if( tmRemainTime.tm_yday > 0 )
	{
		// 남은 일수
		swprintf_s( awcBuffer, 256, GetDisplayStringManager()->GetString( DST_BUDOKAI_TIME_DAY ), tmRemainTime.tm_yday );

		return awcBuffer;
	}

	if( tmRemainTime.tm_hour > 0 )
	{
		// 남은 시간
		swprintf_s( awcBuffer, 256, GetDisplayStringManager()->GetString( DST_BUDOKAI_TIME_HOUR ), tmRemainTime.tm_hour );

		return awcBuffer;
	}

	if( tmRemainTime.tm_min > 0 )
	{
		// 남은 분
		swprintf_s( awcBuffer, 256, GetDisplayStringManager()->GetString( DST_BUDOKAI_TIME_MINUTE ), tmRemainTime.tm_min );

		return awcBuffer;
	}

	swprintf_s( awcBuffer, 256, GetDisplayStringManager()->GetString( DST_BUDOKAI_TIME_MINUTE ), 0. );

	return  awcBuffer;
}

void Logic_NameNotify(const WCHAR* pwcText)
{
	if( !pwcText )
		return;

	if( wcsstr( pwcText, Logic_GetAvatarName() ) )
		CDboEventGenerator::NameNotify((WCHAR*)pwcText);
}

RwBool Logic_CanUseLocalName(const WCHAR* pwcText)
{
	if( !pwcText )
		FALSE;

	RwUInt32 uiTextLength = wcslen(pwcText);
	eDBO_SERVICE_PUBLISHER eService = GetServiceInterface()->GetPublisherID();
	RwBool bResult = !Dbo_AnyInvalidCharacterInName((BYTE)eService, (WCHAR*)pwcText, uiTextLength);

	return bResult;
}

RwBool Logic_IsUsableCharacterName_with_WarningMessage(const WCHAR* pwcText)
{
	if( !pwcText )
	{
		GetAlarmManager()->AlarmMessage(DST_LOBBY_EMPTYNAME);
		return FALSE;
	}

	RwUInt32 uiTextLength = wcslen(pwcText);
	if( 0 == uiTextLength )
	{
		GetAlarmManager()->AlarmMessage(DST_LOBBY_EMPTYNAME);
		return FALSE;
	}

	eDBO_SERVICE_PUBLISHER eService = GetServiceInterface()->GetPublisherID();
	if( Dbo_IsNameTooShort((BYTE)eService, (WCHAR*)pwcText) )
	{
		GetAlarmManager()->AlarmMessage(CHARACTER_TOO_SHORT_NAME, TRUE);
		return FALSE;
	}

	if( Dbo_IsNameTooLong((BYTE)eService, (WCHAR*)pwcText) )
	{
		GetAlarmManager()->AlarmMessage(CHARACTER_TOO_LONG_NAME, TRUE);
		return FALSE;
	}

	if( GetChattingFilter()->IsAnyInvalidText(pwcText) )
	{
		GetAlarmManager()->AlarmMessage( DST_LOBBY_CAN_NOT_USE_THE_WORD );
		return FALSE;
	}

	if( Dbo_AnyInvalidCharacterInName((BYTE)eService, (WCHAR*)pwcText, uiTextLength) )
	{
		GetAlarmManager()->AlarmMessage( DST_LOBBY_ENABLE_LOCAL_TEXT );
		return FALSE;
	}

	return TRUE;
}

const WCHAR* Logic_GetHoipoiMixRecipeName(RwUInt32 tblIdx)
{
	static WCHAR wBuf[256];

	CItemRecipeTable* pRecipeItemTable = API_GetTableContainer()->GetItemRecipeTable();
	CTextTable* pItemTextTable = API_GetTableContainer()->GetTextAllTable()->GetItemTbl();

	sITEM_RECIPE_TBLDAT* pRecipeItemData = reinterpret_cast<sITEM_RECIPE_TBLDAT*>( pRecipeItemTable->FindData( tblIdx ) );
	sTEXT_TBLDAT* pTextData = NULL;
	if( pRecipeItemData )
	{
		pTextData = reinterpret_cast<sTEXT_TBLDAT*>( pItemTextTable->FindData( pRecipeItemData->dwName ) );
		if( pTextData )
		{
			return pTextData->wstrText.c_str();
		}
		else
			swprintf_s( wBuf, 256, L"Invalid Name Text Index : %u", pRecipeItemData->dwName );
	}
	else
		swprintf_s( wBuf, 256, L"Invalid Item Table Index : %u", tblIdx );

	return wBuf;
}

const WCHAR* Logic_GetTeleportTypeName(RwUInt32 uiTeleportType)
{
	switch( uiTeleportType )
	{
		case TELEPORT_TYPE_GAME_IN:			return GetDisplayStringManager()->GetString( DST_TELEPORT_TYPE_GAME_IN );
		case TELEPORT_TYPE_GAME_OUT:		return GetDisplayStringManager()->GetString( DST_TELEPORT_TYPE_GAME_OUT );
		case TELEPORT_TYPE_SKILL:			return GetDisplayStringManager()->GetString( DST_TELEPORT_TYPE_SKILL );
		case TELEPORT_TYPE_NPC_PORTAL:		return GetDisplayStringManager()->GetString( DST_TELEPORT_TYPE_NPC_PORTAL );
		case TELEPORT_TYPE_POPOSTONE:		return GetDisplayStringManager()->GetString( DST_TELEPORT_TYPE_POPOSTONE );
		case TELEPORT_TYPE_WORLD_MOVE:		return GetDisplayStringManager()->GetString( DST_TELEPORT_TYPE_WORLD_MOVE );
		case TELEPORT_TYPE_TMQ_WORLD_MOVE:	return GetDisplayStringManager()->GetString( DST_TELEPORT_TYPE_TMQ_WORLD_MOVE );
		case TELEPORT_TYPE_TMQ_PORTAL:		return GetDisplayStringManager()->GetString( DST_TELEPORT_TYPE_TMQ_PORTAL );
		case TELEPORT_TYPE_RANKBATTLE:		return GetDisplayStringManager()->GetString( DST_TELEPORT_TYPE_RANKBATTLE );
		case TELEPORT_TYPE_TIMEQUEST:		return GetDisplayStringManager()->GetString( DST_TELEPORT_TYPE_TIMEQUEST );
		case TELEPORT_TYPE_TUTORIAL:		return GetDisplayStringManager()->GetString( DST_TELEPORT_TYPE_TUTORIAL );
		case TELEPORT_TYPE_BUDOKAI:			return GetDisplayStringManager()->GetString( DST_TELEPORT_TYPE_BUDOKAI );
		case TELEPORT_TYPE_MATCH:			return GetDisplayStringManager()->GetString( DST_TELEPORT_TYPE_MATCH );
		case TELEPORT_TYPE_MINORMATCH:		return GetDisplayStringManager()->GetString( DST_TELEPORT_TYPE_MINORMATCH );
		case TELEPORT_TYPE_MAJORMATCH:		return GetDisplayStringManager()->GetString( DST_TELEPORT_TYPE_MAJORMATCH );
		case TELEPORT_TYPE_FINALMATCH:		return GetDisplayStringManager()->GetString( DST_TELEPORT_TYPE_FINALMATCH );
		case TELEPORT_TYPE_COMMAND:			return GetDisplayStringManager()->GetString( DST_TELEPORT_TYPE_COMMAND );
		case TELEPORT_TYPE_GM:				return GetDisplayStringManager()->GetString( DST_TELEPORT_TYPE_GM );
		case TELEPORT_TYPE_DUNGEON:			return GetDisplayStringManager()->GetString( DST_TELEPORT_TYPE_DUNGEON );
		case TELEPORT_TYPE_DOJO:			return GetDisplayStringManager()->GetString( DST_TELEPORT_TYPE_DOJO );
		default:
			break;
	}

	static WCHAR awcBuffer[32];
	swprintf_s( awcBuffer, 32, L"%u", uiTeleportType );
	
	return awcBuffer;
}

RwChar* Logic_GetFormatString(const RwChar* pcFormat, ...)
{
	static RwChar acBuffer[1024];

	va_list args;
	va_start(args, pcFormat);

#if defined(_MSC_VER) && (_MSC_VER >= 1400)
	vsprintf_s(acBuffer, 1024, pcFormat, args);
#else
	vsprintf(acBuffer, pcFormat, args);
#endif
	va_end( args );

	return acBuffer;
}

WCHAR* Logic_GetFormatWideString(const WCHAR* pwcFormat, ...)
{
	static WCHAR awcBuffer[1024];

	va_list args;
	va_start(args, pwcFormat);

#if defined(_MSC_VER) && (_MSC_VER >= 1400)
	vswprintf_s(awcBuffer, 1024, pwcFormat, args);
#else
	vswprintf(awcBuffer, pwcFormat, args);
#endif
	va_end( args );

	return awcBuffer;
}