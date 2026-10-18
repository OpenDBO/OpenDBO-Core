#include "precomp_ntlsimulation.h"
#include "DboTLLoader.h"
#include "NtlLuaState.h"
#include "NtlCoreUtil.h"
#include "NtlDebug.h"
#include "DboTLEntity.h"
#include "DboTLDefine.h"
#include "NtlPLResourcePack.h"


namespace
{
	// tl.lua defines its own numbers for the tutorial enums and they are older than the C++ ones (mascot entries
	// added to the mouse inputs, one scouter mode key instead of on/off), so a lock/unlock or a condition of the
	// script hit another input. Its definitions are rewritten to the C++ values before the script runs.
	struct sTL_SCRIPT_CONSTANT
	{
		const char*	pcName;
		int			nValue;
	};

	const sTL_SCRIPT_CONSTANT s_aTLScriptConstant[] =
	{
		{ "ETL_CONDITION_TYPE_MOUSE_DRAG", ETL_CONDITION_TYPE_MOUSE_DRAG },
		{ "ETL_CONDITION_TYPE_IN_CHECK_POINT", ETL_CONDITION_TYPE_IN_CHECK_POINT },
		{ "ETL_CONDITION_TYPE_MAINMENU_OPEN", ETL_CONDITION_TYPE_MAINMENU_OPEN },
		{ "ETL_CONDITION_TYPE_SKILL_OPEN", ETL_CONDITION_TYPE_SKILL_OPEN },
		{ "ETL_CONDITION_TYPE_SKILL_CLOSE", ETL_CONDITION_TYPE_SKILL_CLOSE },
		{ "ETL_CONDITION_TYPE_QUICK_SLOT_1_REG_SKILL", ETL_CONDITION_TYPE_QUICK_SLOT_1_REG_SKILL },
		{ "ETL_CONDITION_TYPE_QUICK_SLOT_1_UNREG_SKILL", ETL_CONDITION_TYPE_QUICK_SLOT_1_UNREG_SKILL },
		{ "ETL_CONDITION_TYPE_STATUS_OPEN", ETL_CONDITION_TYPE_STATUS_OPEN },
		{ "ETL_CONDITION_TYPE_STATUS_SCOUTER_SLOT_REG", ETL_CONDITION_TYPE_STATUS_SCOUTER_SLOT_REG },
		{ "ETL_CONDITION_TYPE_STATUS_SCOUTER_SLOT_UNREG", ETL_CONDITION_TYPE_STATUS_SCOUTER_SLOT_UNREG },
		{ "ETL_CONDITION_TYPE_STATUS_CLOSE", ETL_CONDITION_TYPE_STATUS_CLOSE },
		{ "ETL_CONDITION_TYPE_MOB_TARGET", ETL_CONDITION_TYPE_MOB_TARGET },
		{ "ETL_CONDITION_TYPE_MOB_NORMAL_ATTACK", ETL_CONDITION_TYPE_MOB_NORMAL_ATTACK },
		{ "ETL_CONDITION_TYPE_MOB_SKILL_ATTACK", ETL_CONDITION_TYPE_MOB_SKILL_ATTACK },
		{ "ETL_CONDITION_TYPE_LOOT_ITEM", ETL_CONDITION_TYPE_LOOT_ITEM },
		{ "ETL_CONDITION_TYPE_SCOUTER_USE", ETL_CONDITION_TYPE_SCOUTER_USE },
		{ "ETL_CONDITION_TYPE_SCOUTER_POWERLEVEL", ETL_CONDITION_TYPE_SCOUTER_POWERLEVEL },
		{ "ETL_CONDITION_TYPE_SCOUTER_CLOSE", ETL_CONDITION_TYPE_SCOUTER_CLOSE },
		{ "ETL_CONDITION_TYPE_CAPSULEKIT_OPEN", ETL_CONDITION_TYPE_CAPSULEKIT_OPEN },
		{ "ETL_CONDITION_TYPE_CAPSULEKIT_CLOSE", ETL_CONDITION_TYPE_CAPSULEKIT_CLOSE },
		{ "ETL_CONDITION_TYPE_CINEMATIC_PLAY_FINISH", ETL_CONDITION_TYPE_CINEMATIC_PLAY_FINISH },
		{ "ETL_CONDITION_TYPE_QUEST_FINISH", ETL_CONDITION_TYPE_QUEST_FINISH },
		{ "ETL_CONDITION_TYPE_INVALID", ETL_CONDITION_TYPE_INVALID },
		{ "ETL_UI_INDICATOR_TYPE_LP_WND", ETL_UI_INDICATOR_TYPE_LP_WND },
		{ "ETL_UI_INDICATOR_TYPE_MINIMAP", ETL_UI_INDICATOR_TYPE_MINIMAP },
		{ "ETL_UI_INDICATOR_TYPE_CHAT", ETL_UI_INDICATOR_TYPE_CHAT },
		{ "ETL_UI_INDICATOR_TYPE_QUICK_SLOT", ETL_UI_INDICATOR_TYPE_QUICK_SLOT },
		{ "ETL_UI_INDICATOR_TYPE_QUICK_SLOT_1", ETL_UI_INDICATOR_TYPE_QUICK_SLOT_1 },
		{ "ETL_UI_INDICATOR_TYPE_CAPSULE_SLOT", ETL_UI_INDICATOR_TYPE_CAPSULE_SLOT },
		{ "ETL_UI_INDICATOR_TYPE_SCOUTER_SLOT", ETL_UI_INDICATOR_TYPE_SCOUTER_SLOT },
		{ "ETL_UI_INDICATOR_TYPE_MAIN_MENU", ETL_UI_INDICATOR_TYPE_MAIN_MENU },
		{ "ETL_UI_INDICATOR_TYPE_MAIN_MENU_SKILL", ETL_UI_INDICATOR_TYPE_MAIN_MENU_SKILL },
		{ "ETL_UI_INDICATOR_TYPE_MAIN_MENU_STATUS", ETL_UI_INDICATOR_TYPE_MAIN_MENU_STATUS },
		{ "ETL_UI_INDICATOR_TYPE_STATUS_SCOUTER_SLOT", ETL_UI_INDICATOR_TYPE_STATUS_SCOUTER_SLOT },
		{ "ETL_UI_INDICATOR_TYPE_CAPSULE_KIT_SCOUTER_ITEM", ETL_UI_INDICATOR_TYPE_CAPSULE_KIT_SCOUTER_ITEM },
		{ "ETL_UI_INDICATOR_TYPE_SKILL_ICON", ETL_UI_INDICATOR_TYPE_SKILL_ICON },
		{ "ETL_UI_INDICATOR_TYPE_SKILL_CLOSE_BTN", ETL_UI_INDICATOR_TYPE_SKILL_CLOSE_BTN },
		{ "ETL_UI_INDICATOR_TYPE_SCOUTER_POWERLEVEL", ETL_UI_INDICATOR_TYPE_SCOUTER_POWERLEVEL },
		{ "ETL_UI_INDICATOR_TYPE_SCOUTER_CLOSE_BTN", ETL_UI_INDICATOR_TYPE_SCOUTER_CLOSE_BTN },
		{ "ETL_UI_INDICATOR_TYPE_INVALID", ETL_UI_INDICATOR_TYPE_INVALID },
		{ "ETL_MOUSE_INPUT_TYPE_TARGETING", ETL_MOUSE_INPUT_TYPE_TARGETING },
		{ "ETL_MOUSE_INPUT_TYPE_MAINMENU_OPEN", ETL_MOUSE_INPUT_TYPE_MAINMENU_OPEN },
		{ "ETL_MOUSE_INPUT_TYPE_MAINMENU_CLOSE", ETL_MOUSE_INPUT_TYPE_MAINMENU_CLOSE },
		{ "ETL_MOUSE_INPUT_TYPE_MAINMENU_SKILL_OPEN", ETL_MOUSE_INPUT_TYPE_MAINMENU_SKILL_OPEN },
		{ "ETL_MOUSE_INPUT_TYPE_MAINMENU_SKILL_CLOSE", ETL_MOUSE_INPUT_TYPE_MAINMENU_SKILL_CLOSE },
		{ "ETL_MOUSE_INPUT_TYPE_SKILL_WND_CLOSE_BTN", ETL_MOUSE_INPUT_TYPE_SKILL_WND_CLOSE_BTN },
		{ "ETL_MOUSE_INPUT_TYPE_SKILL_WND_TAB", ETL_MOUSE_INPUT_TYPE_SKILL_WND_TAB },
		{ "ETL_MOUSE_INPUT_TYPE_QUICK_SLOT_CHANGE", ETL_MOUSE_INPUT_TYPE_QUICK_SLOT_CHANGE },
		{ "ETL_MOUSE_INPUT_TYPE_QUICK_SLOT_1_REG", ETL_MOUSE_INPUT_TYPE_QUICK_SLOT_1_REG },
		{ "ETL_MOUSE_INPUT_TYPE_QUICK_SLOT_1_UNREG", ETL_MOUSE_INPUT_TYPE_QUICK_SLOT_1_UNREG },
		{ "ETL_MOUSE_INPUT_TYPE_QUICK_SLOT_1_EXCUTE", ETL_MOUSE_INPUT_TYPE_QUICK_SLOT_1_EXCUTE },
		{ "ETL_MOUSE_INPUT_TYPE_QUICK_SLOT_2_LAST_REG", ETL_MOUSE_INPUT_TYPE_QUICK_SLOT_2_LAST_REG },
		{ "ETL_MOUSE_INPUT_TYPE_QUICK_SLOT_2_LAST_UNREG", ETL_MOUSE_INPUT_TYPE_QUICK_SLOT_2_LAST_UNREG },
		{ "ETL_MOUSE_INPUT_TYPE_QUICK_SLOT_2_LAST_EXCUTE", ETL_MOUSE_INPUT_TYPE_QUICK_SLOT_2_LAST_EXCUTE },
		{ "ETL_MOUSE_INPUT_TYPE_MAINMENU_STATUS_OPEN", ETL_MOUSE_INPUT_TYPE_MAINMENU_STATUS_OPEN },
		{ "ETL_MOUSE_INPUT_TYPE_MAINMENU_STATUS_CLOSE", ETL_MOUSE_INPUT_TYPE_MAINMENU_STATUS_CLOSE },
		{ "ETL_MOUSE_INPUT_TYPE_STATUS_WND_SCOUTER_SLOT", ETL_MOUSE_INPUT_TYPE_STATUS_WND_SCOUTER_SLOT },
		{ "ETL_MOUSE_INPUT_TYPE_STATUS_WND_CLOSE_BTN", ETL_MOUSE_INPUT_TYPE_STATUS_WND_CLOSE_BTN },
		{ "ETL_MOUSE_INPUT_TYPE_STATUS_WND_TAB", ETL_MOUSE_INPUT_TYPE_STATUS_WND_TAB },
		{ "ETL_MOUSE_INPUT_TYPE_MAINMENU_CAPSULE_KIT_OPEN", ETL_MOUSE_INPUT_TYPE_MAINMENU_CAPSULE_KIT_OPEN },
		{ "ETL_MOUSE_INPUT_TYPE_MAINMENU_CAPSULE_KIT_CLOSE", ETL_MOUSE_INPUT_TYPE_MAINMENU_CAPSULE_KIT_CLOSE },
		{ "ETL_MOUSE_INPUT_TYPE_CPASULE_SLOT_OPEN", ETL_MOUSE_INPUT_TYPE_CPASULE_SLOT_OPEN },
		{ "ETL_MOUSE_INPUT_TYPE_CPASULE_SLOT_CLOSE", ETL_MOUSE_INPUT_TYPE_CPASULE_SLOT_CLOSE },
		{ "ETL_MOUSE_INPUT_TYPE_CPASULE_KIT_WND_SLOTS_PICK_UP", ETL_MOUSE_INPUT_TYPE_CPASULE_KIT_WND_SLOTS_PICK_UP },
		{ "ETL_MOUSE_INPUT_TYPE_CPASULE_KIT_WND_SLOTS_DROP_DOWN", ETL_MOUSE_INPUT_TYPE_CPASULE_KIT_WND_SLOTS_DROP_DOWN },
		{ "ETL_MOUSE_INPUT_TYPE_CPASULE_KIT_WND_CLOSE_BTN", ETL_MOUSE_INPUT_TYPE_CPASULE_KIT_WND_CLOSE_BTN },
		{ "ETL_MOUSE_INPUT_TYPE_CPASULE_KIT_WND_TRASH", ETL_MOUSE_INPUT_TYPE_CPASULE_KIT_WND_TRASH },
		{ "ETL_MOUSE_INPUT_TYPE_SCOUTER_ON_MODE", ETL_MOUSE_INPUT_TYPE_SCOUTER_ON_MODE },
		{ "ETL_MOUSE_INPUT_TYPE_SCOUTER_OFF_MODE", ETL_MOUSE_INPUT_TYPE_SCOUTER_OFF_MODE },
		{ "ETL_MOUSE_INPUT_TYPE_SCOUTER_MODE_POWERLEVEL", ETL_MOUSE_INPUT_TYPE_SCOUTER_MODE_POWERLEVEL },
		{ "ETL_MOUSE_INPUT_TYPE_SCOUTER_MODE_CLOSE_BTN", ETL_MOUSE_INPUT_TYPE_SCOUTER_MODE_CLOSE_BTN },
		{ "ETL_MOUSE_INPUT_TYPE_SCOUTER_SLOT", ETL_MOUSE_INPUT_TYPE_SCOUTER_SLOT },
		{ "ETL_MOUSE_INPUT_TYPE_MAINMENU_HELP_OPEN", ETL_MOUSE_INPUT_TYPE_MAINMENU_HELP_OPEN },
		{ "ETL_MOUSE_INPUT_TYPE_MAINMENU_HELP_CLOSE", ETL_MOUSE_INPUT_TYPE_MAINMENU_HELP_CLOSE },
		{ "ETL_MOUSE_INPUT_TYPE_HELP_WND_CLOSE_BTN", ETL_MOUSE_INPUT_TYPE_HELP_WND_CLOSE_BTN },
		{ "ETL_MOUSE_INPUT_TYPE_MAINMENU_QUEST_OPEN", ETL_MOUSE_INPUT_TYPE_MAINMENU_QUEST_OPEN },
		{ "ETL_MOUSE_INPUT_TYPE_MAINMENU_QUEST_CLOSE", ETL_MOUSE_INPUT_TYPE_MAINMENU_QUEST_CLOSE },
		{ "ETL_MOUSE_INPUT_TYPE_QUEST_WND_CLOSE_BTN", ETL_MOUSE_INPUT_TYPE_QUEST_WND_CLOSE_BTN },
		{ "ETL_MOUSE_INPUT_TYPE_MAINMENU_COMMUNITY_OPEN", ETL_MOUSE_INPUT_TYPE_MAINMENU_COMMUNITY_OPEN },
		{ "ETL_MOUSE_INPUT_TYPE_MAINMENU_COMMUNITY_CLOSE", ETL_MOUSE_INPUT_TYPE_MAINMENU_COMMUNITY_CLOSE },
		{ "ETL_MOUSE_INPUT_TYPE_COMMUNITY_WND_CLOSE_BTN", ETL_MOUSE_INPUT_TYPE_COMMUNITY_WND_CLOSE_BTN },
		{ "ETL_MOUSE_INPUT_TYPE_MAINMENU_RANKBOARD_OPEN", ETL_MOUSE_INPUT_TYPE_MAINMENU_RANKBOARD_OPEN },
		{ "ETL_MOUSE_INPUT_TYPE_MAINMENU_RANKBOARD_CLOSE", ETL_MOUSE_INPUT_TYPE_MAINMENU_RANKBOARD_CLOSE },
		{ "ETL_MOUSE_INPUT_TYPE_RANKBOARD_WND_CLOSE_BTN", ETL_MOUSE_INPUT_TYPE_RANKBOARD_WND_CLOSE_BTN },
		{ "ETL_MOUSE_INPUT_TYPE_MAINMENU_OPTION_OPEN", ETL_MOUSE_INPUT_TYPE_MAINMENU_OPTION_OPEN },
		{ "ETL_MOUSE_INPUT_TYPE_MAINMENU_OPTION_CLOSE", ETL_MOUSE_INPUT_TYPE_MAINMENU_OPTION_CLOSE },
		{ "ETL_MOUSE_INPUT_TYPE_OPTION_WND_CLOSE_BTN", ETL_MOUSE_INPUT_TYPE_OPTION_WND_CLOSE_BTN },
		{ "ETL_MOUSE_INPUT_TYPE_MAINMENU_FRIEND_OPEN", ETL_MOUSE_INPUT_TYPE_MAINMENU_FRIEND_OPEN },
		{ "ETL_MOUSE_INPUT_TYPE_MAINMENU_FRIEND_CLOSE", ETL_MOUSE_INPUT_TYPE_MAINMENU_FRIEND_CLOSE },
		{ "ETL_MOUSE_INPUT_TYPE_FRIEND_WND_CLOSE_BTN", ETL_MOUSE_INPUT_TYPE_FRIEND_WND_CLOSE_BTN },
		{ "ETL_MOUSE_INPUT_TYPE_MINIMAP", ETL_MOUSE_INPUT_TYPE_MINIMAP },
		{ "ETL_MOUSE_INPUT_TYPE_MOVE", ETL_MOUSE_INPUT_TYPE_MOVE },
		{ "ETL_MOUSE_INPUT_TYPE_HELP_BUTTON_IN_DIALOG", ETL_MOUSE_INPUT_TYPE_HELP_BUTTON_IN_DIALOG },
		{ "ETL_MOUSE_INPUT_TYPE_ALL", ETL_MOUSE_INPUT_TYPE_ALL },
		{ "ETL_KEYBOARD_INPUT_TYPE_MOVE", ETL_KEYBOARD_INPUT_TYPE_MOVE },
		{ "ETL_KEYBOARD_INPUT_TYPE_LOOTING", ETL_KEYBOARD_INPUT_TYPE_LOOTING },
		{ "ETL_KEYBOARD_INPUT_TYPE_TARGETING", ETL_KEYBOARD_INPUT_TYPE_TARGETING },
		{ "ETL_KEYBOARD_INPUT_TYPE_ESC_TARGETING", ETL_KEYBOARD_INPUT_TYPE_ESC_TARGETING },
		{ "ETL_KEYBOARD_INPUT_TYPE_QUICK_SLOT_CHANGE", ETL_KEYBOARD_INPUT_TYPE_QUICK_SLOT_CHANGE },
		{ "ETL_KEYBOARD_INPUT_TYPE_QUICK_SLOT_1", ETL_KEYBOARD_INPUT_TYPE_QUICK_SLOT_1 },
		{ "ETL_KEYBOARD_INPUT_TYPE_QUICK_SLOT_2_LAST", ETL_KEYBOARD_INPUT_TYPE_QUICK_SLOT_2_LAST },
		{ "ETL_KEYBOARD_INPUT_TYPE_SCOUTER", ETL_KEYBOARD_INPUT_TYPE_SCOUTER },
		{ "ETL_KEYBOARD_INPUT_TYPE_ESC_SCOUTER_CLOSE", ETL_KEYBOARD_INPUT_TYPE_ESC_SCOUTER_CLOSE },
		{ "ETL_KEYBOARD_INPUT_TYPE_ESC_SCOUTER_MODE_CLOSE", ETL_KEYBOARD_INPUT_TYPE_ESC_SCOUTER_MODE_CLOSE },
		{ "ETL_KEYBOARD_INPUT_TYPE_SKILL_OPEN", ETL_KEYBOARD_INPUT_TYPE_SKILL_OPEN },
		{ "ETL_KEYBOARD_INPUT_TYPE_SKILL_CLOSE", ETL_KEYBOARD_INPUT_TYPE_SKILL_CLOSE },
		{ "ETL_KEYBOARD_INPUT_TYPE_ESC_SKILL_CLOSE", ETL_KEYBOARD_INPUT_TYPE_ESC_SKILL_CLOSE },
		{ "ETL_KEYBOARD_INPUT_TYPE_STATUS_OPEN", ETL_KEYBOARD_INPUT_TYPE_STATUS_OPEN },
		{ "ETL_KEYBOARD_INPUT_TYPE_STATUS_CLOSE", ETL_KEYBOARD_INPUT_TYPE_STATUS_CLOSE },
		{ "ETL_KEYBOARD_INPUT_TYPE_ESC_STATUS_CLOSE", ETL_KEYBOARD_INPUT_TYPE_ESC_STATUS_CLOSE },
		{ "ETL_KEYBOARD_INPUT_TYPE_CAPSULE_KIT_OPEN", ETL_KEYBOARD_INPUT_TYPE_CAPSULE_KIT_OPEN },
		{ "ETL_KEYBOARD_INPUT_TYPE_CAPSULE_KIT_CLOSE", ETL_KEYBOARD_INPUT_TYPE_CAPSULE_KIT_CLOSE },
		{ "ETL_KEYBOARD_INPUT_TYPE_ESC_CAPSULE_KIT_CLOSE", ETL_KEYBOARD_INPUT_TYPE_ESC_CAPSULE_KIT_CLOSE },
		{ "ETL_KEYBOARD_INPUT_TYPE_QUEST", ETL_KEYBOARD_INPUT_TYPE_QUEST },
		{ "ETL_KEYBOARD_INPUT_TYPE_ESC_QUEST_CLOSE", ETL_KEYBOARD_INPUT_TYPE_ESC_QUEST_CLOSE },
		{ "ETL_KEYBOARD_INPUT_TYPE_GUILD", ETL_KEYBOARD_INPUT_TYPE_GUILD },
		{ "ETL_KEYBOARD_INPUT_TYPE_ESC_COMMUNITY_CLOSE", ETL_KEYBOARD_INPUT_TYPE_ESC_COMMUNITY_CLOSE },
		{ "ETL_KEYBOARD_INPUT_TYPE_RANKBOARD", ETL_KEYBOARD_INPUT_TYPE_RANKBOARD },
		{ "ETL_KEYBOARD_INPUT_TYPE_ESC_RANKBOARD_CLOSE", ETL_KEYBOARD_INPUT_TYPE_ESC_RANKBOARD_CLOSE },
		{ "ETL_KEYBOARD_INPUT_TYPE_OPTION", ETL_KEYBOARD_INPUT_TYPE_OPTION },
		{ "ETL_KEYBOARD_INPUT_TYPE_ESC_OPTION_CLOSE", ETL_KEYBOARD_INPUT_TYPE_ESC_OPTION_CLOSE },
		{ "ETL_KEYBOARD_INPUT_TYPE_FRIEND", ETL_KEYBOARD_INPUT_TYPE_FRIEND },
		{ "ETL_KEYBOARD_INPUT_TYPE_ESC_FRIEND_CLOSE", ETL_KEYBOARD_INPUT_TYPE_ESC_FRIEND_CLOSE },
		{ "ETL_KEYBOARD_INPUT_TYPE_WORLDMAP", ETL_KEYBOARD_INPUT_TYPE_WORLDMAP },
		{ "ETL_KEYBOARD_INPUT_TYPE_ESC_WORLDMAP_CLOSE", ETL_KEYBOARD_INPUT_TYPE_ESC_WORLDMAP_CLOSE },
		{ "ETL_KEYBOARD_INPUT_TYPE_MINIMAP", ETL_KEYBOARD_INPUT_TYPE_MINIMAP },
		{ "ETL_KEYBOARD_INPUT_TYPE_HELP", ETL_KEYBOARD_INPUT_TYPE_HELP },
		{ "ETL_KEYBOARD_INPUT_TYPE_ESC_HELP_CLOSE", ETL_KEYBOARD_INPUT_TYPE_ESC_HELP_CLOSE },
		{ "ETL_KEYBOARD_INPUT_TYPE_MAINMENU", ETL_KEYBOARD_INPUT_TYPE_MAINMENU },
		{ "ETL_KEYBOARD_INPUT_TYPE_ESC_MAINMENU_CLOSE", ETL_KEYBOARD_INPUT_TYPE_ESC_MAINMENU_CLOSE },
		{ "ETL_KEYBOARD_INPUT_TYPE_AUTORUN", ETL_KEYBOARD_INPUT_TYPE_AUTORUN },
		{ "ETL_KEYBOARD_INPUT_TYPE_ALL", ETL_KEYBOARD_INPUT_TYPE_ALL },
		{ "ETL_ATTACK_TYPE_NORMAL_ATTACK", ETL_ATTACK_TYPE_NORMAL_ATTACK },
		{ "ETL_ATTACK_TYPE_SKILL_ATTACK", ETL_ATTACK_TYPE_SKILL_ATTACK },
		{ "ETL_ATTACK_TYPE_ALL", ETL_ATTACK_TYPE_ALL },
		{ "ETL_CAMERA_TYPE_MOB", ETL_CAMERA_TYPE_MOB },
		{ "ETL_CAMERA_TYPE_NPC", ETL_CAMERA_TYPE_NPC },
		{ "ETL_CAMERA_TYPE_POSITION", ETL_CAMERA_TYPE_POSITION },
		{ "ETL_KEYBOARD_INPUT_TYPE_SCOUTER_MODE_ON", ETL_KEYBOARD_INPUT_TYPE_SCOUTER_MODE },
		{ "ETL_KEYBOARD_INPUT_TYPE_SCOUTER_MODE_OFF", ETL_KEYBOARD_INPUT_TYPE_SCOUTER_MODE },
	};

	const sTL_SCRIPT_CONSTANT* FindTLScriptConstant( const std::string& strName )
	{
		for ( int i = 0; i < (int)(sizeof( s_aTLScriptConstant ) / sizeof( s_aTLScriptConstant[0] )); ++i )
		{
			if ( strName == s_aTLScriptConstant[i].pcName )
				return &s_aTLScriptConstant[i];
		}

		return NULL;
	}

	// "NAME = number;" at the start of a line (indented or not) gets the C++ value of NAME
	void FixTLScriptConstants( std::string& strScript )
	{
		std::string strResult;
		strResult.reserve( strScript.size() );

		std::string::size_type uiPos = 0;
		while ( uiPos < strScript.size() )
		{
			std::string::size_type uiEnd = strScript.find( '\n', uiPos );
			if ( uiEnd == std::string::npos ) uiEnd = strScript.size(); else ++uiEnd;

			std::string strLine = strScript.substr( uiPos, uiEnd - uiPos );
			uiPos = uiEnd;

			std::string::size_type uiName = strLine.find_first_not_of( " \t" );
			if ( uiName != std::string::npos && strLine.compare( uiName, 4, "ETL_" ) == 0 )
			{
				std::string::size_type uiNameEnd = strLine.find_first_not_of( "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_", uiName );
				std::string::size_type uiEqual = strLine.find_first_not_of( " \t", uiNameEnd );
				if ( uiNameEnd != std::string::npos && uiEqual != std::string::npos && strLine[uiEqual] == '=' )
				{
					std::string::size_type uiValue = strLine.find_first_not_of( " \t", uiEqual + 1 );
					std::string::size_type uiValueEnd = strLine.find_first_not_of( "-0123456789", uiValue );
					const sTL_SCRIPT_CONSTANT* pConstant = FindTLScriptConstant( strLine.substr( uiName, uiNameEnd - uiName ) );

					if ( pConstant && uiValue != std::string::npos && uiValueEnd != std::string::npos && uiValueEnd > uiValue )
					{
						char acValue[16];
						sprintf_s( acValue, sizeof( acValue ), "%d", pConstant->nValue );
						strLine.replace( uiValue, uiValueEnd - uiValue, acValue );
					}
				}
			}

			strResult += strLine;
		}

		strScript.swap( strResult );
	}
}

CDboTLLoader* CDboTLLoader::s_pIntance = NULL;


CDboTLLoader::CDboTLLoader( void )
{
	m_pScript = NULL;
}


CDboTLLoader::~CDboTLLoader( void )
{
	Unload();
}


RwBool CDboTLLoader::Load( void )
{
	Unload();

	NTL_ASSERTE( NULL == s_pIntance );
	s_pIntance = this;

	if ( Load_Script() || Load_CPP() )
	{
		return TRUE;
	}

	NtlLogFilePrint( "TL loader failed" );

	return FALSE;
}


void CDboTLLoader::Unload( void )
{
	m_defTLSyncGroupList.clear();

	listdef_TLEntityList::iterator it = m_defTLEntityList.begin();
	for ( ; it != m_defTLEntityList.end(); ++it )
	{
		NTL_DELETE( *it );
	}
	m_defTLEntityList.clear();

	NTL_DELETE( m_pScript );

	s_pIntance = NULL;
}


CDboTLLoader::listdef_TLSyncGroupList* CDboTLLoader::GetTLSyncGroupList( void )
{
	return &m_defTLSyncGroupList;
}


RwBool CDboTLLoader::Load_Script( void )
{
	RwBool bResult = TRUE;

	m_pScript = NTL_NEW CNtlLuaState;

	// 에러 핸들러 연결
	m_pScript->LinkErrorHandler( CDboTLLoader::Script_Error );

	// Lua glue 연결
	bResult &= (m_pScript->AddFunction( "TLSyncGroup",				&CDboTLLoader::LuaGlue_TLSyncGroup ) ? TRUE : FALSE);
	bResult &= (m_pScript->AddFunction( "TLPause",					&CDboTLLoader::LuaGlue_TLPause ) ? TRUE : FALSE);
	bResult &= (m_pScript->AddFunction( "TLDefaultSkin",			&CDboTLLoader::LuaGlue_TLDefaultSkin ) ? TRUE : FALSE);
	bResult &= (m_pScript->AddFunction( "TLGuideMsg_Show",			&CDboTLLoader::LuaGlue_TLGuideMsg_Show ) ? TRUE : FALSE);
	bResult &= (m_pScript->AddFunction( "TLGuideMsg_Hide",			&CDboTLLoader::LuaGlue_TLGuideMsg_Hide ) ? TRUE : FALSE);
	bResult &= (m_pScript->AddFunction( "TLGuideMsg_ChangeColor",	&CDboTLLoader::LuaGlue_TLGuideMsg_Color ) ? TRUE : FALSE);
	bResult &= (m_pScript->AddFunction( "TLGuideIMG_Show",			&CDboTLLoader::LuaGlue_TLGuideIMG_Show ) ? TRUE : FALSE);
	bResult &= (m_pScript->AddFunction( "TLGuideIMG_Hide",			&CDboTLLoader::LuaGlue_TLGuideIMG_Hide ) ? TRUE : FALSE);
	bResult &= (m_pScript->AddFunction( "TLGuideIMGOk_Show",		&CDboTLLoader::LuaGlue_TLGuideIMGOk_Show ) ? TRUE : FALSE);
	bResult &= (m_pScript->AddFunction( "TLGuideIMGOk_Hide",		&CDboTLLoader::LuaGlue_TLGuideIMGOk_Hide ) ? TRUE : FALSE);
	bResult &= (m_pScript->AddFunction( "TLCheckPoint",				&CDboTLLoader::LuaGlue_TLCheckPoint ) ? TRUE : FALSE);
	bResult &= (m_pScript->AddFunction( "TLUIIndicator_Show",		&CDboTLLoader::LuaGlue_TLUIIndicator_Show ) ? TRUE : FALSE);
	bResult &= (m_pScript->AddFunction( "TLUIIndicator_Hide",		&CDboTLLoader::LuaGlue_TLUIIndicator_Hide ) ? TRUE : FALSE);
	bResult &= (m_pScript->AddFunction( "TLMouseInputLock",			&CDboTLLoader::LuaGlue_TLMouseInputLock ) ? TRUE : FALSE);
	bResult &= (m_pScript->AddFunction( "TLMouseInputUnlock",		&CDboTLLoader::LuaGlue_TLMouseInputUnlock ) ? TRUE : FALSE);
	bResult &= (m_pScript->AddFunction( "TLKeyboardInputLock",		&CDboTLLoader::LuaGlue_TLKeyboardInputLock ) ? TRUE : FALSE);
	bResult &= (m_pScript->AddFunction( "TLKeyboardInputUnlock",	&CDboTLLoader::LuaGlue_TLKeyboardInputUnlock ) ? TRUE : FALSE);
	bResult &= (m_pScript->AddFunction( "TLActionMapBackup",		&CDboTLLoader::LuaGlue_TLActionMapBackup ) ? TRUE : FALSE);
	bResult &= (m_pScript->AddFunction( "TLActionMapRestore",		&CDboTLLoader::LuaGlue_TLActionMapRestore ) ? TRUE : FALSE);
	bResult &= (m_pScript->AddFunction( "TLExcuteQuest",			&CDboTLLoader::LuaGlue_TLExcuteQuest ) ? TRUE : FALSE);
	bResult &= (m_pScript->AddFunction( "TLBroadMsg",				&CDboTLLoader::LuaGlue_TLBroadMsg ) ? TRUE : FALSE);
	bResult &= (m_pScript->AddFunction( "TLAttackLock",				&CDboTLLoader::LuaGlue_TLAttackLock ) ? TRUE : FALSE);
	bResult &= (m_pScript->AddFunction( "TLAttackUnlock",			&CDboTLLoader::LuaGlue_TLAttackUnlock ) ? TRUE : FALSE);
	bResult &= (m_pScript->AddFunction( "TLPlayCinematic",			&CDboTLLoader::LuaGlue_TLPlayCinematic ) ? TRUE : FALSE);
	bResult &= (m_pScript->AddFunction( "TLPlaySound",				&CDboTLLoader::LuaGlue_TLPlaySound ) ? TRUE : FALSE);
	bResult &= (m_pScript->AddFunction( "TLPlayCamera_TBLIDX",		&CDboTLLoader::LuaGlue_TLPlayCamera_TBLIDX ) ? TRUE : FALSE);
	bResult &= (m_pScript->AddFunction( "TLPlayCamera_POS",			&CDboTLLoader::LuaGlue_TLPlayCamera_POS ) ? TRUE : FALSE);
	bResult &= (m_pScript->AddFunction( "TLSoundOnOff",				&CDboTLLoader::LuaGlue_TLSoundOnOff ) ? TRUE : FALSE);
	bResult &= (m_pScript->AddFunction( "TLCondition",				&CDboTLLoader::LuaGlue_TLCondition ) ? TRUE : FALSE);
	bResult &= (m_pScript->AddFunction( "TLDisableDialogMoveOption",	&CDboTLLoader::LuaGlue_TLDisableDialogMoveOption ) ? TRUE : FALSE);
	bResult &= (m_pScript->AddFunction( "TLEnableSkip",				&CDboTLLoader::LuaGlue_TLEnableSkip ) ? TRUE : FALSE);



	// Script 실행
	if ( GetNtlResourcePackManager()->GetActiveFlags() & NTL_PACK_TYPE_FLAG_SCRIPT )
	{
		void* pBuffer = NULL;
		int nBufferSize = 0;

		GetNtlResourcePackManager()->LoadScript( TL_SCRIPT_FILE_NAME, &pBuffer, &nBufferSize );

		if ( NULL == pBuffer )
		{
			DBO_ASSERTE( pBuffer );
			return FALSE;
		}

		std::string strScript( (const char*)pBuffer, nBufferSize );
		NTL_ARRAY_DELETE( pBuffer );

		FixTLScriptConstants( strScript );

		bResult &= (m_pScript->RunString( strScript.c_str() ) ? TRUE : FALSE);
	}
	else
	{
		FILE* fp = NULL;
		if ( fopen_s( &fp, TL_SCRIPT_FILE_NAME, "rb" ) != 0 || NULL == fp )
		{
			DBO_FAIL( "CDboTLLoader::Load_Script - can not open " << TL_SCRIPT_FILE_NAME );
			return FALSE;
		}

		std::string strScript;
		char acBuffer[4096];
		size_t uiRead;
		while ( (uiRead = fread( acBuffer, 1, sizeof( acBuffer ), fp )) > 0 )
		{
			strScript.append( acBuffer, uiRead );
		}
		fclose( fp );

		FixTLScriptConstants( strScript );

		bResult &= (m_pScript->RunString( strScript.c_str() ) ? TRUE : FALSE);
	}

	return bResult;
}


RwBool CDboTLLoader::Load_CPP( void )
{
	return FALSE;
}


CDboTLSyncGroup* CDboTLLoader::GetCurrentSyncGroup( void )
{
	if ( m_defTLSyncGroupList.empty() )
	{
		Create_TLSyncGroup(FALSE);
	}

	return *--m_defTLSyncGroupList.end();
}


void CDboTLLoader::Create_TLSyncGroup( RwBool bSKipable )
{
	CDboTLSyncGroup* pTLSyncGroup = NTL_NEW CDboTLSyncGroup;

	m_defTLEntityList.push_back( pTLSyncGroup );

	m_defTLSyncGroupList.push_back( pTLSyncGroup );
}


void CDboTLLoader::Create_TLPause( RwReal fPauseTime )
{
	CDboTLPause* pTLPause = NTL_NEW CDboTLPause;

	pTLPause->SetPauseTime( fPauseTime );

	m_defTLEntityList.push_back( pTLPause );
	GetCurrentSyncGroup()->PushChildEntity( pTLPause );
}


void CDboTLLoader::Create_TLDefaultSkin( const char* pFontName, RwRGBA& rgbFontBackColor, RwRGBA& rgbFontColor, RwInt32 nFontSize, RwUInt32 uiFontAllign, RwV2d& vFontOffset, RwV2d& vMsgLayerOffset, RwV2d& vIMGOKset, RwV2d& vIMGOffset )
{
	CDboTLDefaultSkin* pDefSkin = NTL_NEW CDboTLDefaultSkin;

	pDefSkin->SetFontName( pFontName );
	pDefSkin->SetFontBackColor( rgbFontBackColor );
	pDefSkin->SetFontColor( rgbFontColor );
	pDefSkin->SetFontSize( nFontSize );
	pDefSkin->SetFontAllign( uiFontAllign );
	pDefSkin->SetFontOffset( vFontOffset );

	pDefSkin->SetMsgLayerOffset( vMsgLayerOffset );
	pDefSkin->SetOkOffset( vIMGOKset );
	pDefSkin->SetIMGOffset( vIMGOffset );

	m_defTLEntityList.push_back( pDefSkin );
	GetCurrentSyncGroup()->PushChildEntity( pDefSkin );
}

void CDboTLLoader::Create_TLGuideMsg_Show( RwUInt32 uiTextTblIdx, RwReal fFadeInTime )
{
	CDboTLGuideMsg_Show* pTLGuideMsgShow = NTL_NEW CDboTLGuideMsg_Show;

	pTLGuideMsgShow->SetTextTblIdx( uiTextTblIdx );
	pTLGuideMsgShow->SetFadeInTime( fFadeInTime );

	m_defTLEntityList.push_back( pTLGuideMsgShow );
	GetCurrentSyncGroup()->PushChildEntity( pTLGuideMsgShow );
}


void CDboTLLoader::Create_TLGuideMsg_Hide( RwUInt32 uiTextTblIdx, RwReal fFadeOutTime )
{
	CDboTLGuideMsg_Hide* pTLGuideMsgHide = NTL_NEW CDboTLGuideMsg_Hide;

	pTLGuideMsgHide->SetTextTblIdx( uiTextTblIdx );
	pTLGuideMsgHide->SetFadeOutTime( fFadeOutTime );

	m_defTLEntityList.push_back( pTLGuideMsgHide );
	GetCurrentSyncGroup()->PushChildEntity( pTLGuideMsgHide );
}


void CDboTLLoader::Create_TLGuideMsg_Color( RwUInt32 uiTextTblIdx, RwRGBA& rgbFontBackColor, RwRGBA& rgbFontColor, RwReal fFadeInOutTime )
{
	CDboTLGuideMsg_ChangeColor* pTLGuideMsgChangeColor = NTL_NEW CDboTLGuideMsg_ChangeColor;

	pTLGuideMsgChangeColor->SetTextTblIdx( uiTextTblIdx );
	pTLGuideMsgChangeColor->SetFontBackColor( rgbFontBackColor );
	pTLGuideMsgChangeColor->SetFontColor( rgbFontColor );
	pTLGuideMsgChangeColor->SetFadeInOutTime( fFadeInOutTime );

	m_defTLEntityList.push_back( pTLGuideMsgChangeColor );
	GetCurrentSyncGroup()->PushChildEntity( pTLGuideMsgChangeColor );
}


void CDboTLLoader::Create_TLGuideIMG_Show( const char* pIMGName, RwReal fFadeInTime, RwReal fSizeRate )
{
	CDboTLGuideIMG_Show* pTLGuideIMGShow = NTL_NEW CDboTLGuideIMG_Show;

	pTLGuideIMGShow->SetIMGName( pIMGName );
	pTLGuideIMGShow->SetFadeInTime( fFadeInTime );
	pTLGuideIMGShow->SetSizeRate( fSizeRate );

	m_defTLEntityList.push_back( pTLGuideIMGShow );
	GetCurrentSyncGroup()->PushChildEntity( pTLGuideIMGShow );
}


void CDboTLLoader::Create_TLGuideIMG_Hide( const char* pIMGName, RwReal fFadeOutTime )
{
	CDboTLGuideIMG_Hide* pTLGuideIMGHide = NTL_NEW CDboTLGuideIMG_Hide;

	pTLGuideIMGHide->SetIMGName( pIMGName );
	pTLGuideIMGHide->SetFadeOutTime( fFadeOutTime );

	m_defTLEntityList.push_back( pTLGuideIMGHide );
	GetCurrentSyncGroup()->PushChildEntity( pTLGuideIMGHide );
}


void CDboTLLoader::Create_TLGuideIMGOk_Show( RwReal fFadeInTime )
{
	CDboTLGuideIMGOk_Show* pTLGuideIMGOkShow = NTL_NEW CDboTLGuideIMGOk_Show;

	pTLGuideIMGOkShow->SetFadeInTime( fFadeInTime );

	m_defTLEntityList.push_back( pTLGuideIMGOkShow );
	GetCurrentSyncGroup()->PushChildEntity( pTLGuideIMGOkShow );
}


void CDboTLLoader::Create_TLGuideIMGOk_Hide( RwReal fFadeOutTime )
{
	CDboTLGuideIMGOk_Hide* pTLGuideIMGOkHide = NTL_NEW CDboTLGuideIMGOk_Hide;

	pTLGuideIMGOkHide->SetFadeOutTime( fFadeOutTime );

	m_defTLEntityList.push_back( pTLGuideIMGOkHide );
	GetCurrentSyncGroup()->PushChildEntity( pTLGuideIMGOkHide );
}


void CDboTLLoader::Create_TLCheckPoint( RwV3d& vPos )
{
	CDboTLCheckPoint* pTLCheckPoint = NTL_NEW CDboTLCheckPoint;

	pTLCheckPoint->SetTargetPos( vPos );

	m_defTLEntityList.push_back( pTLCheckPoint );
	GetCurrentSyncGroup()->PushChildEntity( pTLCheckPoint );
}


void CDboTLLoader::Create_TLUIIndicator_Show( RwUInt32 uiTLIndicatorType, const char* pcFlashName, RwV2d& vFocusOffPos )
{
	CDboTLUIIndicator_Show* pTLUIIndicatorShow = NTL_NEW CDboTLUIIndicator_Show;

	pTLUIIndicatorShow->SetTLWindowType( uiTLIndicatorType );
	pTLUIIndicatorShow->SetFlashName( pcFlashName );
	pTLUIIndicatorShow->SetTLFocusOffsetPos( vFocusOffPos );

	m_defTLEntityList.push_back( pTLUIIndicatorShow );
	GetCurrentSyncGroup()->PushChildEntity( pTLUIIndicatorShow );
}


void CDboTLLoader::Create_TLUIIndicator_Hide( RwUInt32 uiTLIndicatorType )
{
	CDboTLUIIndicator_Hide* pTLUIIndicatorHide = NTL_NEW CDboTLUIIndicator_Hide;

	pTLUIIndicatorHide->SetTLWindowType( uiTLIndicatorType );

	m_defTLEntityList.push_back( pTLUIIndicatorHide );
	GetCurrentSyncGroup()->PushChildEntity( pTLUIIndicatorHide );
}


void CDboTLLoader::Create_TLMouseInputLock( RwUInt32 uiMouseInputType )
{
	CDboTLMouseInputLock* pTLMouseInputLock = NTL_NEW CDboTLMouseInputLock;

	pTLMouseInputLock->SetTLMouseInputType( uiMouseInputType );

	m_defTLEntityList.push_back( pTLMouseInputLock );
	GetCurrentSyncGroup()->PushChildEntity( pTLMouseInputLock );
}


void CDboTLLoader::Create_TLMouseInputUnlock( RwUInt32 uiMouseInputType )
{
	CDboTLMouseInputUnlock* pTLMouseInputUnlock = NTL_NEW CDboTLMouseInputUnlock;

	pTLMouseInputUnlock->SetTLMouseInputType( uiMouseInputType );

	m_defTLEntityList.push_back( pTLMouseInputUnlock );
	GetCurrentSyncGroup()->PushChildEntity( pTLMouseInputUnlock );
}


void CDboTLLoader::Create_TLKeyboardInputLock( RwUInt32 uiKeyboardInputType )
{
	CDboTLKeyboardInputLock* pTLKeyboardInputLock = NTL_NEW CDboTLKeyboardInputLock;

	pTLKeyboardInputLock->SetTLKeyboardInputType( uiKeyboardInputType );

	m_defTLEntityList.push_back( pTLKeyboardInputLock );
	GetCurrentSyncGroup()->PushChildEntity( pTLKeyboardInputLock );
}


void CDboTLLoader::Create_TLKeyboardInputUnlock( RwUInt32 uiKeyboardInputType )
{
	CDboTLKeyboardInputUnlock* pTLKeyboardInputUnlock = NTL_NEW CDboTLKeyboardInputUnlock;

	pTLKeyboardInputUnlock->SetTLKeyboardInputType( uiKeyboardInputType );

	m_defTLEntityList.push_back( pTLKeyboardInputUnlock );
	GetCurrentSyncGroup()->PushChildEntity( pTLKeyboardInputUnlock );
}


void CDboTLLoader::Create_TLActionMapBackup( void )
{
	CDboTLActionMapBackup* pTLActionMapBackup = NTL_NEW CDboTLActionMapBackup;

	m_defTLEntityList.push_back( pTLActionMapBackup );
	GetCurrentSyncGroup()->PushChildEntity( pTLActionMapBackup );
}


void CDboTLLoader::Create_TLActionMapRestore( void )
{
	CDboTLActionMapRestore* pTLActionMapRestore = NTL_NEW CDboTLActionMapRestore;

	m_defTLEntityList.push_back( pTLActionMapRestore );
	GetCurrentSyncGroup()->PushChildEntity( pTLActionMapRestore );
}


void CDboTLLoader::Create_TLExcuteQuest( RwUInt16 tQuestID )
{
	CDboTLExcuteQuest* pTLExcuteQuest = NTL_NEW CDboTLExcuteQuest;

	pTLExcuteQuest->SetQuestID( tQuestID );

	m_defTLEntityList.push_back( pTLExcuteQuest );
	GetCurrentSyncGroup()->PushChildEntity( pTLExcuteQuest );
}


void CDboTLLoader::Create_TLBroadMsg( RwUInt32 uiNPCTblIdx, RwUInt32 uiSpeechTblIdx, RwReal fRemainTime )
{
	CDboTLBroadMsg* pTLBroadMsg = NTL_NEW CDboTLBroadMsg;

	pTLBroadMsg->SetNPCTableIndex( uiNPCTblIdx );
	pTLBroadMsg->SetSpeechTableIndex( uiSpeechTblIdx );
	pTLBroadMsg->SetRemainTime( fRemainTime );

	m_defTLEntityList.push_back( pTLBroadMsg );
	GetCurrentSyncGroup()->PushChildEntity( pTLBroadMsg );
}


void CDboTLLoader::Create_TLAttackLock( RwUInt32 uiAttackType )
{
	CDboTLAttackLock* pTLAttackLock = NTL_NEW CDboTLAttackLock;

	pTLAttackLock->SetAttackType( uiAttackType );

	m_defTLEntityList.push_back( pTLAttackLock );
	GetCurrentSyncGroup()->PushChildEntity( pTLAttackLock );
}


void CDboTLLoader::Create_TLAttackUnlock( RwUInt32 uiAttackType )
{
	CDboTLAttackUnlock* pTLAttackUnlock = NTL_NEW CDboTLAttackUnlock;

	pTLAttackUnlock->SetAttackType( uiAttackType );

	m_defTLEntityList.push_back( pTLAttackUnlock );
	GetCurrentSyncGroup()->PushChildEntity( pTLAttackUnlock );
}


void CDboTLLoader::Create_TLPlayCinematic( const char* pLuaFuncName )
{
	CDboTLPlayCinematic* pTLPlayCinematic = NTL_NEW CDboTLPlayCinematic;

	pTLPlayCinematic->SetLuaFuncName( pLuaFuncName );

	m_defTLEntityList.push_back( pTLPlayCinematic );
	GetCurrentSyncGroup()->PushChildEntity( pTLPlayCinematic );
}


void CDboTLLoader::Create_TLPlaySound( const char* pSoundName )
{
	CDboTLPlaySound* pTLPlaySound = NTL_NEW CDboTLPlaySound;

	pTLPlaySound->SetSoundName( pSoundName );

	m_defTLEntityList.push_back( pTLPlaySound );
	GetCurrentSyncGroup()->PushChildEntity( pTLPlaySound );
}


void CDboTLLoader::Create_TLPlayCamera_TBLIDX( RwUInt32 uiCameraType, RwUInt32 uiTblIdx )
{
	CDboTLPlayCamera* pTLPlayCamera = NTL_NEW CDboTLPlayCamera;

	pTLPlayCamera->SetCameraType( uiCameraType );
	pTLPlayCamera->SetCameraData( uiTblIdx );

	m_defTLEntityList.push_back( pTLPlayCamera );
	GetCurrentSyncGroup()->PushChildEntity( pTLPlayCamera );
}


void CDboTLLoader::Create_TLPlayCamera_POS( RwUInt32 uiCameraType, RwV3d& vTargetPos )
{
	CDboTLPlayCamera* pTLPlayCamera = NTL_NEW CDboTLPlayCamera;

	pTLPlayCamera->SetCameraType( uiCameraType );
	pTLPlayCamera->SetCameraData( vTargetPos );

	m_defTLEntityList.push_back( pTLPlayCamera );
	GetCurrentSyncGroup()->PushChildEntity( pTLPlayCamera );
}


void CDboTLLoader::Create_TLSoundOnOff( RwBool bSoundOn )
{
	CDboTLSoundOnOff* pTLSoundOnOff = NTL_NEW CDboTLSoundOnOff;

	pTLSoundOnOff->SetSoundOnOff( bSoundOn );

	m_defTLEntityList.push_back( pTLSoundOnOff );
	GetCurrentSyncGroup()->PushChildEntity( pTLSoundOnOff );
}


void CDboTLLoader::Create_TLCondition( RwUInt32 uiCondType, RwUInt32 uiParam1 )
{
	CDboTLCondition* pTLCondition = NTL_NEW CDboTLCondition;

	pTLCondition->SetConditionType( uiCondType );
	pTLCondition->SetParam1( uiParam1 );

	m_defTLEntityList.push_back( pTLCondition );
	GetCurrentSyncGroup()->PushChildEntity( pTLCondition );
}

void CDboTLLoader::Create_TLDisableDialogMoveOption( RwBool bDialogMove )
{
	CDboTLDialogMove* pTLDialogMove = NTL_NEW CDboTLDialogMove;

	pTLDialogMove->DisableDialogMoveOption( bDialogMove );

	m_defTLEntityList.push_back( pTLDialogMove );
	GetCurrentSyncGroup()->PushChildEntity( pTLDialogMove );
}

void CDboTLLoader::Create_TLEnableSkip( RwBool bEnableSkip )
{
	CDboTLEnableSkip* pTLEnableSkip = NTL_NEW CDboTLEnableSkip;

	pTLEnableSkip->EnableSkip( bEnableSkip );

	m_defTLEntityList.push_back( pTLEnableSkip );
	GetCurrentSyncGroup()->PushChildEntity( pTLEnableSkip );
}

void CDboTLLoader::Script_Error( const char* pError )
{
	NtlLogFilePrint( (RwChar*)pError );
}


int CDboTLLoader::LuaGlue_TLSyncGroup( lua_State* L )
{
	RwBool bSKipable = (RwBool)s_pIntance->m_pScript->GetNumberArgument( 1 );

	CDboTLLoader::s_pIntance->Create_TLSyncGroup(bSKipable);

	return 0;
}


int CDboTLLoader::LuaGlue_TLPause( lua_State* L )
{
	RwReal fPauseTime = (RwReal)s_pIntance->m_pScript->GetNumberArgument( 1 );

	CDboTLLoader::s_pIntance->Create_TLPause( fPauseTime );

	return 0;
}


int CDboTLLoader::LuaGlue_TLDefaultSkin( lua_State* L )
{
	const char* pFontName = s_pIntance->m_pScript->GetStringArgument( 1 );

	RwRGBA rgbFontBackColor;
	rgbFontBackColor.alpha = 255;
	rgbFontBackColor.red = (RwUInt8)s_pIntance->m_pScript->GetNumberArgument( 2 );
	rgbFontBackColor.green = (RwUInt8)s_pIntance->m_pScript->GetNumberArgument( 3 );
	rgbFontBackColor.blue = (RwUInt8)s_pIntance->m_pScript->GetNumberArgument( 4 );

	RwRGBA rgbFontColor;
	rgbFontColor.alpha = 255;
	rgbFontColor.red = (RwUInt8)s_pIntance->m_pScript->GetNumberArgument( 5 );
	rgbFontColor.green = (RwUInt8)s_pIntance->m_pScript->GetNumberArgument( 6 );
	rgbFontColor.blue = (RwUInt8)s_pIntance->m_pScript->GetNumberArgument( 7 );

	RwInt32 nFontSize = (RwInt32)s_pIntance->m_pScript->GetNumberArgument( 8 );

	RwUInt32 uiFontAllign = (RwUInt32)s_pIntance->m_pScript->GetNumberArgument( 9 );

	RwV2d vFontOffset;
	vFontOffset.x  = (RwReal)s_pIntance->m_pScript->GetNumberArgument( 10 );
	vFontOffset.y  = (RwReal)s_pIntance->m_pScript->GetNumberArgument( 11 );

	RwV2d vMsgLayerOffset;
	vMsgLayerOffset.x  = (RwReal)s_pIntance->m_pScript->GetNumberArgument( 12 );
	vMsgLayerOffset.y  = (RwReal)s_pIntance->m_pScript->GetNumberArgument( 13 );

	RwV2d vIMGOKset;
	vIMGOKset.x  = (RwReal)s_pIntance->m_pScript->GetNumberArgument( 14 );
	vIMGOKset.y  = (RwReal)s_pIntance->m_pScript->GetNumberArgument( 15 );

	RwV2d vIMGOffset;
	vIMGOffset.x  = (RwReal)s_pIntance->m_pScript->GetNumberArgument( 16 );
	vIMGOffset.y  = (RwReal)s_pIntance->m_pScript->GetNumberArgument( 17 );

	CDboTLLoader::s_pIntance->Create_TLDefaultSkin( pFontName, rgbFontBackColor, rgbFontColor, nFontSize, uiFontAllign, vFontOffset, vMsgLayerOffset, vIMGOKset, vIMGOffset );

	return 0;
}


int CDboTLLoader::LuaGlue_TLGuideMsg_Show( lua_State* L )
{
	RwUInt32 uiTextTblIdx = (RwUInt32)s_pIntance->m_pScript->GetNumberArgument( 1 );
	RwReal fFadeInTime = (RwReal)s_pIntance->m_pScript->GetNumberArgument( 2 );

	CDboTLLoader::s_pIntance->Create_TLGuideMsg_Show( uiTextTblIdx, fFadeInTime );

	return 0;
}


int CDboTLLoader::LuaGlue_TLGuideMsg_Hide( lua_State* L )
{
	RwUInt32 uiTextTblIdx = (RwUInt32)s_pIntance->m_pScript->GetNumberArgument( 1 );
	RwReal fFadeOutTime = (RwReal)s_pIntance->m_pScript->GetNumberArgument( 2 );

	CDboTLLoader::s_pIntance->Create_TLGuideMsg_Hide( uiTextTblIdx, fFadeOutTime );

	return 0;
}


int CDboTLLoader::LuaGlue_TLGuideMsg_Color( lua_State* L )
{
	RwUInt32 uiTextTblIdx = (RwUInt32)s_pIntance->m_pScript->GetNumberArgument( 1 );

	RwRGBA rgbFontBackColor;
	rgbFontBackColor.alpha = 255;
	rgbFontBackColor.red = (RwUInt8)s_pIntance->m_pScript->GetNumberArgument( 2 );
	rgbFontBackColor.green = (RwUInt8)s_pIntance->m_pScript->GetNumberArgument( 3 );
	rgbFontBackColor.blue = (RwUInt8)s_pIntance->m_pScript->GetNumberArgument( 4 );

	RwRGBA rgbFontColor;
	rgbFontColor.alpha = 255;
	rgbFontColor.red = (RwUInt8)s_pIntance->m_pScript->GetNumberArgument( 5 );
	rgbFontColor.green = (RwUInt8)s_pIntance->m_pScript->GetNumberArgument( 6 );
	rgbFontColor.blue = (RwUInt8)s_pIntance->m_pScript->GetNumberArgument( 7 );

	RwReal fFadeInOutTime = (RwReal)s_pIntance->m_pScript->GetNumberArgument( 8 );

	CDboTLLoader::s_pIntance->Create_TLGuideMsg_Color( uiTextTblIdx, rgbFontBackColor, rgbFontColor, fFadeInOutTime );

	return 0;
}


int CDboTLLoader::LuaGlue_TLGuideIMG_Show( lua_State* L )
{
	const char* pIMGName = s_pIntance->m_pScript->GetStringArgument( 1 );
	RwReal fFadeInTime = (RwReal)s_pIntance->m_pScript->GetNumberArgument( 2 );
	RwReal fSizeRate = (RwReal)s_pIntance->m_pScript->GetNumberArgument( 3 );

	CDboTLLoader::s_pIntance->Create_TLGuideIMG_Show( pIMGName, fFadeInTime, fSizeRate );

	return 0;
}


int CDboTLLoader::LuaGlue_TLGuideIMG_Hide( lua_State* L )
{
	const char* pIMGName = s_pIntance->m_pScript->GetStringArgument( 1 );
	RwReal fFadeOutTime = (RwReal)s_pIntance->m_pScript->GetNumberArgument( 2 );

	CDboTLLoader::s_pIntance->Create_TLGuideIMG_Hide( pIMGName, fFadeOutTime );

	return 0;
}


int CDboTLLoader::LuaGlue_TLGuideIMGOk_Show( lua_State* L )
{
	RwReal fFadeInTime = (RwReal)s_pIntance->m_pScript->GetNumberArgument( 1 );

	CDboTLLoader::s_pIntance->Create_TLGuideIMGOk_Show( fFadeInTime );

	return 0;
}


int CDboTLLoader::LuaGlue_TLGuideIMGOk_Hide( lua_State* L )
{
	RwReal fFadeOutTime = (RwReal)s_pIntance->m_pScript->GetNumberArgument( 1 );

	CDboTLLoader::s_pIntance->Create_TLGuideIMGOk_Hide( fFadeOutTime );

	return 0;
}


int CDboTLLoader::LuaGlue_TLCheckPoint( lua_State* L )
{
	RwV3d vPos;
	vPos.x = (RwReal)s_pIntance->m_pScript->GetNumberArgument( 1 );
	vPos.y = (RwReal)s_pIntance->m_pScript->GetNumberArgument( 2 );
	vPos.z = (RwReal)s_pIntance->m_pScript->GetNumberArgument( 3 );

	CDboTLLoader::s_pIntance->Create_TLCheckPoint( vPos );

	return 0;
}


int CDboTLLoader::LuaGlue_TLUIIndicator_Show( lua_State* L )
{
	RwUInt32 uiTLIndicatorType = (RwUInt32)s_pIntance->m_pScript->GetNumberArgument( 1 );
	RwChar* pcFlashName = (RwChar*)s_pIntance->m_pScript->GetStringArgument( 2 );
	RwV2d vFocusOffset;

	vFocusOffset.x = (RwReal)s_pIntance->m_pScript->GetNumberArgument( 3 );
	vFocusOffset.y = (RwReal)s_pIntance->m_pScript->GetNumberArgument( 4 );

	CDboTLLoader::s_pIntance->Create_TLUIIndicator_Show( uiTLIndicatorType, pcFlashName, vFocusOffset );

	return 0;
}


int CDboTLLoader::LuaGlue_TLUIIndicator_Hide( lua_State* L )
{
	RwUInt32 uiTLIndicatorType = (RwUInt32)s_pIntance->m_pScript->GetNumberArgument( 1 );

	CDboTLLoader::s_pIntance->Create_TLUIIndicator_Hide( uiTLIndicatorType );

	return 0;
}


int CDboTLLoader::LuaGlue_TLMouseInputLock( lua_State* L )
{
	RwUInt32 uiMouseInputType = (RwUInt32)s_pIntance->m_pScript->GetNumberArgument( 1 );

	CDboTLLoader::s_pIntance->Create_TLMouseInputLock( uiMouseInputType );

	return 0;
}


int CDboTLLoader::LuaGlue_TLMouseInputUnlock( lua_State* L )
{
	RwUInt32 uiMouseInputType = (RwUInt32)s_pIntance->m_pScript->GetNumberArgument( 1 );

	CDboTLLoader::s_pIntance->Create_TLMouseInputUnlock( uiMouseInputType );

	return 0;
}


int CDboTLLoader::LuaGlue_TLKeyboardInputLock( lua_State* L )
{
	RwUInt32 uiMouseInputType = (RwUInt32)s_pIntance->m_pScript->GetNumberArgument( 1 );

	CDboTLLoader::s_pIntance->Create_TLKeyboardInputLock( uiMouseInputType );

	return 0;
}


int CDboTLLoader::LuaGlue_TLKeyboardInputUnlock( lua_State* L )
{
	RwUInt32 uiMouseInputType = (RwUInt32)s_pIntance->m_pScript->GetNumberArgument( 1 );

	CDboTLLoader::s_pIntance->Create_TLKeyboardInputUnlock( uiMouseInputType );

	return 0;
}


int CDboTLLoader::LuaGlue_TLActionMapBackup( lua_State* L )
{
	CDboTLLoader::s_pIntance->Create_TLActionMapBackup();

	return 0;
}


int CDboTLLoader::LuaGlue_TLActionMapRestore( lua_State* L )
{
	CDboTLLoader::s_pIntance->Create_TLActionMapRestore();

	return 0;
}


int CDboTLLoader::LuaGlue_TLExcuteQuest( lua_State* L )
{
	RwUInt16 tQuestID = (RwUInt16)s_pIntance->m_pScript->GetNumberArgument( 1 );

	CDboTLLoader::s_pIntance->Create_TLExcuteQuest( tQuestID );

	return 0;
}


int CDboTLLoader::LuaGlue_TLBroadMsg( lua_State* L )
{
	RwUInt32 uiNPCTblIdx = (RwUInt32)s_pIntance->m_pScript->GetNumberArgument( 1 );
	RwUInt32 uiSpeechTblIdx = (RwUInt32)s_pIntance->m_pScript->GetNumberArgument( 2 );
	RwReal fRemainTime = (RwReal)s_pIntance->m_pScript->GetNumberArgument( 3 );

	CDboTLLoader::s_pIntance->Create_TLBroadMsg( uiNPCTblIdx, uiSpeechTblIdx, fRemainTime );

	return 0;
}


int CDboTLLoader::LuaGlue_TLAttackLock( lua_State* L )
{
	RwUInt32 uiAttackType = (RwUInt32)s_pIntance->m_pScript->GetNumberArgument( 1 );

	CDboTLLoader::s_pIntance->Create_TLAttackLock( uiAttackType );

	return 0;
}


int CDboTLLoader::LuaGlue_TLAttackUnlock( lua_State* L )
{
	RwUInt32 uiAttackType = (RwUInt32)s_pIntance->m_pScript->GetNumberArgument( 1 );

	CDboTLLoader::s_pIntance->Create_TLAttackUnlock( uiAttackType );

	return 0;
}


int CDboTLLoader::LuaGlue_TLPlayCinematic( lua_State* L )
{
	const char* pLuaFuncName = s_pIntance->m_pScript->GetStringArgument( 1 );

	CDboTLLoader::s_pIntance->Create_TLPlayCinematic( pLuaFuncName );

	return 0;
}


int CDboTLLoader::LuaGlue_TLPlaySound( lua_State* L )
{
	const char* pSoundName = s_pIntance->m_pScript->GetStringArgument( 1 );

	CDboTLLoader::s_pIntance->Create_TLPlaySound( pSoundName );

	return 0;
}


int CDboTLLoader::LuaGlue_TLPlayCamera_TBLIDX( lua_State* L )
{
	RwUInt32 uiCameraType = (RwUInt32)s_pIntance->m_pScript->GetNumberArgument( 1 );
	RwUInt32 uiTblIdx = (RwUInt32)s_pIntance->m_pScript->GetNumberArgument( 2 );

	CDboTLLoader::s_pIntance->Create_TLPlayCamera_TBLIDX( uiCameraType, uiTblIdx );

	return 0;
}


int CDboTLLoader::LuaGlue_TLPlayCamera_POS( lua_State* L )
{
	RwUInt32 uiCameraType = (RwUInt32)s_pIntance->m_pScript->GetNumberArgument( 1 );

	RwV3d vTargetPos;
	vTargetPos.x = (RwReal)s_pIntance->m_pScript->GetNumberArgument( 2 );
	vTargetPos.z = (RwReal)s_pIntance->m_pScript->GetNumberArgument( 3 );
	vTargetPos.y = 0.f;

	CDboTLLoader::s_pIntance->Create_TLPlayCamera_POS( uiCameraType, vTargetPos );

	return 0;
}


int CDboTLLoader::LuaGlue_TLSoundOnOff( lua_State* L )
{
	RwBool bSoundOn = (s_pIntance->m_pScript->GetNumberArgument( 1 ) != 0.f) ? TRUE : FALSE;

	CDboTLLoader::s_pIntance->Create_TLSoundOnOff( bSoundOn );

	return 0;
}


int CDboTLLoader::LuaGlue_TLCondition( lua_State* L )
{
	RwUInt32 uiCondType = (RwUInt32)s_pIntance->m_pScript->GetNumberArgument( 1 );
	RwUInt32 uiParam1 = (RwUInt32)s_pIntance->m_pScript->GetNumberArgument( 2 );

	CDboTLLoader::s_pIntance->Create_TLCondition( uiCondType, uiParam1 );

	return 0;
}

int CDboTLLoader::LuaGlue_TLDisableDialogMoveOption( lua_State* L )
{
	RwBool bDialogMove = (RwBool)s_pIntance->m_pScript->GetNumberArgument( 1 );

	CDboTLLoader::s_pIntance->Create_TLDisableDialogMoveOption( bDialogMove );

	return 0;
}

int CDboTLLoader::LuaGlue_TLEnableSkip( lua_State* L )
{
	RwBool bEnableSkip = (RwBool)s_pIntance->m_pScript->GetNumberArgument( 1 );

	CDboTLLoader::s_pIntance->Create_TLEnableSkip( bEnableSkip );

	return 0;
}
