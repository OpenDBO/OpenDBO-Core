#include "precomp_dboclient.h"
#include "ChatSender.h"

// core
#include "NtlDebug.h"

// shared
#include "ChatCommandTable.h"
#include "ActionTable.h"

// simulation
#include "NtlSobActionSkill.h"
#include "NtlSobActionSkillAttr.h"

// dbo
#include "DboDef.h"
#include "DboGlobal.h"
#include "DboEvent.h"
#include "DboEventGenerator.h"
#include "DumpCommand.h"
#include "ChatBalloonManager.h"
#include "DialogManager.h"
#include "AlarmManager.h"
#include "DisplayStringManager.h"

namespace
{
	enum eSendWorkType
	{
		SEND_WORK_NORMAL,
		SEND_WORK_PARTY_NOTIFY,
		SEND_WORK_GUILD_NOTIFY,
	};
};


CChatSender::CChatSender()
{

}

CChatSender::~CChatSender()
{

}

RwBool CChatSender::Create(CChatGui* pChatGui)
{
	NTL_FUNCTION( "CChatLog::Create" );

	m_pChatGui = pChatGui;

	NTL_RETURN(TRUE);
}

VOID CChatSender::Destroy()
{
	NTL_FUNCTION("CChatLog::Destroy");

	UnregAllSocialAction();	

	NTL_RETURNVOID();
}

VOID CChatSender::RegSocialAction(CNtlSobActionSkill* pSobActionSkill)
{
	if( !pSobActionSkill )
	{
		DBO_FAIL("invalid sob pointer");
		return;
	}

	SERIAL_HANDLE hSocialActionHandle = pSobActionSkill->GetSerialID();
	if( m_mapSocialAction.find(hSocialActionHandle) != m_mapSocialAction.end() )
		return;

	CChatCommandTable*		pChatCommandTable	= API_GetTableContainer()->GetChatCommandTable();
	CNtlSobActionSkillAttr*	pActionSkillAttr	= reinterpret_cast<CNtlSobActionSkillAttr*>( pSobActionSkill->GetSobAttr() );
	if( !pActionSkillAttr )
	{
		DBO_FAIL("Not exist sob attr of handle : " << hSocialActionHandle );
		return;
	}

	sACTION_TBLDAT* pACTION_TBLDAT = reinterpret_cast<sACTION_TBLDAT*>( pActionSkillAttr->GetActionSkillTbl() );
	if( !pACTION_TBLDAT )
	{
		DBO_FAIL("Not exist action table of sob handle : " << hSocialActionHandle );
		return;
	}

	if( ACTION_TYPE_SOCIAL_ACTION != pACTION_TBLDAT->byAction_Type )
		return;


	sSOCIAL_ACTION* pSOCIAL_ACTION = NTL_NEW sSOCIAL_ACTION;
	pSOCIAL_ACTION->pSobActionSkill			= pSobActionSkill;
	pSOCIAL_ACTION->pCHAT_COMMAND_TBLDAT	= reinterpret_cast<sCHAT_COMMAND_TBLDAT*>( pChatCommandTable->FindData( pACTION_TBLDAT->chat_Command_Index ) );

	if( !pSOCIAL_ACTION->pCHAT_COMMAND_TBLDAT )
	{
		DBO_FAIL("not exist chat command table of index : " << pACTION_TBLDAT->chat_Command_Index);
		NTL_DELETE(pSOCIAL_ACTION);
		return;
	}
	
	m_mapSocialAction[hSocialActionHandle] = pSOCIAL_ACTION;
}

VOID CChatSender::UnregAllSocialAction()
{
	MAP_SOCIAL_ACTION::iterator it_SocialAction = m_mapSocialAction.begin();
	for( ; it_SocialAction != m_mapSocialAction.end() ; ++it_SocialAction )
	{
		sSOCIAL_ACTION* pSOCIAL_ACTION = it_SocialAction->second;
		NTL_DELETE(pSOCIAL_ACTION);
	}
	m_mapSocialAction.clear();
}

VOID CChatSender::InputText( const WCHAR* pwcText )
{
	if( !pwcText )
		return;

	if( pwcText[0] == L'@' )
	{
		// server command
		GetDboGlobal()->GetGamePacketGenerator()->SendServerCommand( pwcText );
	}
	else if( !GetDumpCmdManager()->Command( pwcText ) )
	{
		Process_Send( pwcText );
	}
}

VOID CChatSender::Process_Send( const WCHAR* pText )
{
	if( !pText )
	{
		DBO_FAIL("Text pointer id NULL");
		return;
	}

	CChatPacketGenerator *pChatPacketGenerator = CDboGlobal::GetInstance()->GetChatPacketGenerator();
	CNtlSobAvatar* pSobAvatar = GetNtlSLGlobal()->GetSobAvatar(); 
	if( !pSobAvatar )
		return;

	// 명령어 해석
	std::wstring strParseText;
	eChatType ePostType = m_pChatGui->GetChatType();
	RwUInt8  bySendWorkType = SEND_WORK_NORMAL;
	eChatType eChangedType = Process_Parsing( strParseText, pText, bySendWorkType );

	if( eChangedType == CHAT_SHORTCUT )
		return;

	// 욕설 필터링
	std::wstring strFilterText = GetChattingFilter()->Filtering( strParseText.c_str() );

	// 채팅 모드가 변경이 됬다면...
	if( eChangedType == CHAT_TYPE_PARTY )
	{
		if( pSobAvatar->GetParty()->IsHaveGroup() == FALSE )
			eChangedType = CHAT_TYPE_GENERAL;
	}
	else if( eChangedType == CHAT_TYPE_GUILD )
	{
		if( pSobAvatar->GetGuild()->IsHaveGroup() == FALSE )
			eChangedType = CHAT_TYPE_GENERAL;
	}


	if( eChangedType == INVALID_CHAT_TYPE )
		eChangedType = ePostType;

	if( strFilterText.empty() )
		return;


	switch( eChangedType )
	{
	case CHAT_TYPE_GENERAL:
		{
			pChatPacketGenerator->SendChatMsgSay( strFilterText.c_str() );
			break;
		}
	case CHAT_TYPE_TRADE:
		{
			break;
		}
	case CHAT_TYPE_GUILD:
		{
			if( SEND_WORK_NORMAL == bySendWorkType)
			{
				pChatPacketGenerator->SendChatMsgGuild( DBO_CHATTING_TYPE_GENERAL, strFilterText.c_str() );
			}
			else if( SEND_WORK_GUILD_NOTIFY == bySendWorkType)
			{
				pChatPacketGenerator->SendChatMsgGuild( DBO_CHATTING_TYPE_GUILD_SCRAMBLE_NOTICE, strFilterText.c_str() );
			}
			
			break;
		}
	case CHAT_TYPE_PARTY:
		{
			if( SEND_WORK_NORMAL == bySendWorkType )
			{
				pChatPacketGenerator->SendChatMsgParty( DBO_CHATTING_TYPE_GENERAL, strFilterText.c_str() );
			}
			else if( SEND_WORK_PARTY_NOTIFY == bySendWorkType )
			{
				pChatPacketGenerator->SendChatMsgParty( DBO_CHATTING_TYPE_PARTY_NOTICE, strFilterText.c_str() );
			}

			break;
		}	
	case CHAT_TYPE_WHISPER:
		{
			std::wstring::size_type nPos = strFilterText.find_first_of( L' ' );
			std::wstring wstrTargetName = strFilterText.substr( 0, nPos );
			std::wstring wstrMsg = strFilterText.substr( nPos + 1, strFilterText.size() - 1 );

			if( nPos != std::wstring::npos && nPos <= NTL_MAX_SIZE_CHAR_NAME )
			{
				if( wstrMsg.empty() )
					break;

				pChatPacketGenerator->SendChatMsgWhisper( wstrMsg.c_str(), wstrTargetName.c_str() );
				m_pChatGui->SetLastRecieveWhisperName( (WCHAR*)wstrTargetName.c_str() );
			}					
			else
			{
				const WCHAR* pwcAlramMessage = GetDisplayStringManager()->GetString( DST_CHAT_FAIL_TO_USE_WHISPER );
				if( pwcAlramMessage )
					CDboEventGenerator::SayMessage(CHAT_TYPE_SYSTEM, Logic_GetAvatarName(), (RwUInt16)wcslen(pwcAlramMessage), pwcAlramMessage, Logic_GetAvatarHandle());
			}

			break;
		}
	case CHAT_TYPE_SHOUT:
		{			
			pChatPacketGenerator->SendChatMsgShout( strFilterText.c_str() );
			break;
		}
	case CHAT_TYPE_SYSTEM:
		{
			CDboEventGenerator::SayMessage(CHAT_TYPE_SYSTEM, Logic_GetAvatarName(), (RwUInt16)strFilterText.size(), strFilterText.c_str(), Logic_GetAvatarHandle());
			break;
		}
	}

	if( eChangedType != INVALID_CHAT_TYPE )
		m_pChatGui->SetChatType(eChangedType);

	GetAlarmManager()->ResetRepeatCheck();
}

eChatType CChatSender::Process_Parsing( std::wstring& wstrOutputText, const WCHAR* pInputText, RwUInt8& bySendWorkType )
{
	if( !pInputText )
		return INVALID_CHAT_TYPE;

	if( pInputText[0] != L'/' )
	{
		wstrOutputText = pInputText;
		return INVALID_CHAT_TYPE;
	}


	std::wstring wstrOriginal = pInputText;

	// 일반
	if( HaveShortCut(wstrOutputText, wstrOriginal, DST_CHAT_SHORT_CUT_GENERAL, DST_CHAT_SHORT_CUT_GENERAL_LOCAL) )
	{
		bySendWorkType = SEND_WORK_NORMAL;
		return CHAT_TYPE_GENERAL;
	}

	// 파티
	if( HaveShortCut(wstrOutputText, wstrOriginal, DST_CHAT_SHORT_CUT_PARTY, DST_CHAT_SHORT_CUT_PARTY_LOCAL) )
	{
		bySendWorkType = SEND_WORK_NORMAL;
		return CHAT_TYPE_PARTY;
	}

	// 우선 순위를 지키자 : 길드 알림 -> 파티 알림
	// 길드 알림
	if( Logic_I_am_GuildLeader() )
	{
		if( HaveShortCut(wstrOutputText, wstrOriginal, DST_CHAT_SHORT_CUT_PARTY_NOTIFY, DST_CHAT_SHORT_CUT_PARTY_LOCAL_NOTIFY) )
		{
			bySendWorkType = SEND_WORK_GUILD_NOTIFY;
			return CHAT_TYPE_GUILD;
		}
	}

	// 파티 알림
	if( Logic_I_am_PartyLeader() )
	{
		if( HaveShortCut(wstrOutputText, wstrOriginal, DST_CHAT_SHORT_CUT_PARTY_NOTIFY, DST_CHAT_SHORT_CUT_PARTY_LOCAL_NOTIFY) )
		{
			bySendWorkType = SEND_WORK_PARTY_NOTIFY;
			return CHAT_TYPE_PARTY;
		}
	}	

	// 길드
	if( HaveShortCut(wstrOutputText, wstrOriginal, DST_CHAT_SHORT_CUT_GUILD, DST_CHAT_SHORT_CUT_GUILD_LOCAL) )
	{
		bySendWorkType = SEND_WORK_NORMAL;
		return CHAT_TYPE_GUILD;
	}

	// 귓속말 
	if( HaveShortCut(wstrOutputText, wstrOriginal, DST_CHAT_SHORT_CUT_WHISPER, DST_CHAT_SHORT_CUT_WHISPER_LOCAL) )
	{
		bySendWorkType = SEND_WORK_NORMAL;
		return CHAT_TYPE_WHISPER;
	}

	// 귓속말 응답
	if( HaveShortCut(wstrOutputText, wstrOriginal, DST_CHAT_SHORT_CUT_LAST_WHISPER_USER, DST_CHAT_SHORT_CUT_LAST_WHISPER_USER_LOCAL) )
	{
		std::wstring& wstrLastRecieveWhisperName = m_pChatGui->GetLastRecieveWhisperName();
		bySendWorkType = SEND_WORK_NORMAL;

		if( wstrLastRecieveWhisperName.empty() )
		{
			wstrOutputText = GetDisplayStringManager()->GetString( DST_CHAT_HAVE_NO_USER_TO_REPLY );
			return CHAT_TYPE_SYSTEM;
		}

		wstrOutputText = wstrLastRecieveWhisperName + L' ' + wstrOutputText;
		return CHAT_TYPE_WHISPER;
	}

	// 외치기
	if( HaveShortCut(wstrOutputText, wstrOriginal, DST_CHAT_SHORT_CUT_SHOUT, DST_CHAT_SHORT_CUT_SHOUT_LOCAL) )
	{
		bySendWorkType = SEND_WORK_NORMAL;
		return CHAT_TYPE_SHOUT;
	}

	// 헬프
	if( HaveShortCut(wstrOutputText, wstrOriginal, DST_CHAT_SHORT_CUT_HELP, 0xffffffff) )
	{
		wstrOutputText = GetDisplayStringManager()->GetString( DST_CHAT_HELP_TEXT );
		bySendWorkType = SEND_WORK_NORMAL;
		return CHAT_TYPE_SYSTEM;
	}

	// 파티 초대
	if( HaveShortCut(wstrOutputText, wstrOriginal, 0xffffffff, DST_CHAT_SHORT_CUT_INVITE_PARTY) )
	{
		if( Logic_CanPartyInvite() == FALSE )
		{
			Logic_WarnningWorldConceptActionMessage(WORLD_CONCEPT_FIRST_GRADE);
		}
		else
		{
			GetDboGlobal()->GetGamePacketGenerator()->SendPartyInvite_Name( wstrOutputText.c_str() );
		}

		bySendWorkType = SEND_WORK_NORMAL;

		return CHAT_SHORTCUT;
	}

	// 소셜 액션
	SERIAL_HANDLE hHanle = GetSocialActionHandle(wstrOriginal);
	if( hHanle != INVALID_SERIAL_ID )
	{
		Logic_UseProc(hHanle);
		return CHAT_SHORTCUT;
	}

	return INVALID_CHAT_TYPE;
}

RwBool CChatSender::HaveShortCut(std::wstring& wstrOutput, std::wstring& wstrOriginal,
								 RwUInt32 uiShortcutStringIndex, RwUInt32 uiLocalShortcutStringIndex)
{
	const WCHAR* pwcShortcut = NULL;
	size_t stringSize;

	// 영문 오리지널 명령어
	if( uiShortcutStringIndex != 0xffffffff )
	{
		pwcShortcut = const_cast<WCHAR*>( GetDisplayStringManager()->GetString( uiShortcutStringIndex ) );
		if( pwcShortcut )
		{
			stringSize = wcslen(pwcShortcut);

			if( stringSize > 0 )
			{
				// 소문자 비교
				if( wstrOriginal.compare(0, stringSize, pwcShortcut) == 0 )
				{
					wstrOutput = wstrOriginal.substr( stringSize, wstrOriginal.size() - 1 );
					return TRUE;
				}

				// 대문자 비교
				static WCHAR awcBuffer[64] = L"";
				swprintf_s(awcBuffer, 64, L"%s", pwcShortcut);
				wcsupr( awcBuffer );

				if( wstrOriginal.compare(0, stringSize, pwcShortcut) == 0 )
				{
					wstrOutput = wstrOriginal.substr( stringSize, wstrOriginal.size() - 1 );
					return TRUE;
				}
			}
		}
	}

	// 지역별 언어에 따른 명령어
	if( uiLocalShortcutStringIndex != 0xffffffff )
	{
		pwcShortcut = const_cast<WCHAR*>( GetDisplayStringManager()->GetString( uiLocalShortcutStringIndex ) );
		if( pwcShortcut )
		{
			stringSize = wcslen(pwcShortcut);

			if( stringSize > 0 )
			{
				if( wstrOriginal.compare(0, stringSize, pwcShortcut) == 0 )
				{
					wstrOutput = wstrOriginal.substr( stringSize, wstrOriginal.size() - 1 );
					return TRUE;
				}
			}
		}
	}

	return FALSE;
}

SERIAL_HANDLE CChatSender::GetSocialActionHandle(std::wstring& wstrOriginal)
{
	CTextTable* pChatCommandText = API_GetTableContainer()->GetTextAllTable()->GetChatCommandTbl();
	size_t stringSize = wstrOriginal.length();
	std::wstring wstrSocialActionCommand;

	if( stringSize <= 1 )
		return INVALID_SERIAL_ID;

	MAP_SOCIAL_ACTION::iterator it_SocialAction = m_mapSocialAction.begin();
	for( ; it_SocialAction != m_mapSocialAction.end() ; ++it_SocialAction )
	{
		sSOCIAL_ACTION*			pSOCIAL_ACTION			= it_SocialAction->second;
		sCHAT_COMMAND_TBLDAT*	pCHAT_COMMAND_TBLDAT	= pSOCIAL_ACTION->pCHAT_COMMAND_TBLDAT;

		for( RwUInt8 i = 0 ; i < NTL_MAX_CHAT_COMMAND ; ++i )	
		{
			if( pCHAT_COMMAND_TBLDAT->aChat_Command[i] == INVALID_TBLIDX )
				continue;

			wstrSocialActionCommand = pChatCommandText->GetText( pCHAT_COMMAND_TBLDAT->aChat_Command[i] );
			if( wstrOriginal.compare(1, stringSize - 1, wstrSocialActionCommand.c_str() ) == 0 )
			{				
				return pSOCIAL_ACTION->pSobActionSkill->GetSerialID();
			}
		}
	}

	return INVALID_SERIAL_ID;
}