#include "precomp_dboclient.h"
#include "DboEventGenerator.h"
#include "NtlStage.h"
#include "StageManager.h"
#include "DisplayStringDef.h"
#include "DisplayStringManager.h"
#include "MsgBoxGui.h"
#include "ChatGui.h"
#include "NtlResultCode.h"
#include "AlarmManager.h"
#include "DumpCommand.h"
#include "NtlTokenizer.h"
#include "NtlPLResourcePack.h"
#include "NtlSLLogic.h"

CAlarmManager::CAlarmManager(VOID)
{
	ResetRepeatCheck();
}
 
CAlarmManager* CAlarmManager::GetInstance(VOID)
{
	static CAlarmManager intance;
	return &intance;
}

VOID CAlarmManager::ResetRepeatCheck(VOID)
{
	m_RepeatMessageCheck.nLastChatMessageID = -1;
	m_RepeatMessageCheck.nLastChatMessageType = -1;
	m_RepeatMessageCheck.nRepeatCount = 0;
}

VOID CAlarmManager::AlarmMessage( RwInt32 eStringID, RwBool bServerSync /* = FALSE */, sMsgBoxData* pExData /* = NULL */, RwReal fShowTime /* = 0.0f  */ )
{
	const WCHAR* szText = GetDisplayStringManager()->GetString( eStringID, (CDisplayStringManager::EDispType)bServerSync );

	if( !szText || wcslen( szText ) == 0 )
	{
		if( Logic_IsUIDevInfoVisible() )
		{
			WCHAR buf[256];
			swprintf_s( buf, 256, L"%d(0:Client,1:Server), %d(stringID) is not exist", (RwInt32)bServerSync, eStringID );

			CNtlStage *pStage = CStageManager::GetInstance()->GetActiveStage();
			std::string strStageName = pStage->GetName();

			if(strStageName == GAME_STAGE_NAME)
				CDboEventGenerator::MsgBoxShow( buf, MBW_NONE, MBTF_OK );
			else
				CDboEventGenerator::NetConnectBoxShow( buf, MBW_NONE, MBTF_OK );			
		}

		return;		
	}		
	
	AlarmProcess( szText, eStringID, bServerSync, pExData, fShowTime );
}

VOID CAlarmManager::AlarmMessage( WCHAR* szText, RwUInt32 uType /* = ALARM_TYPE_CHAT_WARN */, RwUInt32 uWorkID /* = MBW_NONE */, 
								  RwUInt32 uMsgBoxStyle /* = MBT_OK */, RwBool bPopup /* = TRUE */, RwReal fShowTime /* = 0.0f */, 
								  sMsgBoxData* pExData /* = NULL */, std::list<sMsgBoxCustomBtn>* plistCustomBtn /* = NULL  */)
{
	if( uType & ALARM_TYPE_CHAT_WARN )
	{
		RwInt32 nLength = wcslen( szText );
		CDboEventGenerator::SayMessage( CHAT_TYPE_SYSTEM, SENDERNAME_SYSTEM, (RwUInt16)nLength, szText );
	}

	if( uType & ALARM_TYPE_CHAT_NOTICE )
	{
		RwInt32 nLength = wcslen( szText );
		CDboEventGenerator::SayMessage( CHAT_TYPE_SYSTEM, SENDERNAME_NOTICE, (RwUInt16)nLength, szText );
	}

	if( uType & ALARM_TYPE_MSGBOX )
	{
		CDboEventGenerator::MsgBoxShow( szText, uWorkID, uMsgBoxStyle, bPopup, fShowTime, pExData, plistCustomBtn );		
	}

	if( uType & ALARM_ACTION_NOTIFY )
	{
		CDboEventGenerator::NotifyMessage( SDboEventNotify::ACTION, szText );
	}

	if( uType & ALARM_TYPE_CONSOLE )
	{
		DUMP_DEBUG_MSG(DUMP_DEFAULT, szText);
	}

	if( uType & ALARM_TYPE_LOBBY_MSGBOX )
	{
		//CDboEventGenerator::MsgBoxShow( szText, uWorkID, uMsgBoxStyle, bPopup, fShowTime, pExData, plistCustomBtn );		
		CDboEventGenerator::NetConnectBoxShow( szText, uWorkID, uMsgBoxStyle, fShowTime, pExData );
	}
}

VOID CAlarmManager::FormattedAlarmMessage( RwInt32 eStringID, RwBool bServerSync, sMsgBoxData* pExData, ... )
{
	va_list args;
	va_start( args, pExData );
	vFormattedAlarmMessage( eStringID, bServerSync, 0.0f, pExData, args );
	va_end( args );
}

VOID CAlarmManager::vFormattedAlarmMessage( RwInt32 eStringID, RwBool bServerSync, sMsgBoxData* pExData, va_list ArgList )
{
	vFormattedAlarmMessage( eStringID, bServerSync, 0.0f, pExData, ArgList );
}

VOID CAlarmManager::FormattedAlarmMessage( RwInt32 eStringID, RwBool bServerSync, RwReal fShowTime, sMsgBoxData* pExData, ... )
{
	va_list args;
	va_start( args, pExData );
	vFormattedAlarmMessage( eStringID, bServerSync, fShowTime, pExData, args );	
	va_end( args );
}

VOID CAlarmManager::vFormattedAlarmMessage( RwInt32 eStringID, RwBool bServerSync, RwReal fShowTime, sMsgBoxData* pExData, va_list ArgList )
{
	WCHAR formattedText[GUI_TEXT_BUFFER_SIZE];
	const WCHAR* szText = GetDisplayStringManager()->GetString( eStringID, bServerSync );

	if( !szText || wcslen( szText ) == 0 )
	{
		if( Logic_IsUIDevInfoVisible() )
		{
			swprintf_s( formattedText, GUI_TEXT_BUFFER_SIZE, L"Client %d(stringID) is not exist", eStringID );

			CNtlStage *pStage = CStageManager::GetInstance()->GetActiveStage();
			std::string strStageName = pStage->GetName();

			if(strStageName == GAME_STAGE_NAME)
				CDboEventGenerator::MsgBoxShow( formattedText, MBW_NONE, MBTF_OK );
			else
				CDboEventGenerator::NetConnectBoxShow( formattedText, MBW_NONE, MBTF_OK );

		}

		return;
	}

	vswprintf_s( formattedText, GUI_TEXT_BUFFER_SIZE, szText, ArgList );
	
	AlarmProcess( formattedText, eStringID, bServerSync, pExData, fShowTime );
}

VOID CAlarmManager::FormattedAlarmMessage( WCHAR* szText, ... )
{
	va_list args;
	va_start( args, szText );
	vFormattedAlarmMessage( szText, args );
	va_end( args );
}

VOID CAlarmManager::vFormattedAlarmMessage( WCHAR* szText, va_list ArgList )
{
	WCHAR formattedText[GUI_TEXT_BUFFER_SIZE];
	vswprintf_s( formattedText, GUI_TEXT_BUFFER_SIZE, szText, ArgList );

	RwInt32 nLength = wcslen( formattedText );
	CDboEventGenerator::SayMessage( CHAT_TYPE_SYSTEM, SENDERNAME_SYSTEM, (RwUInt16)nLength, formattedText );	
}

VOID CAlarmManager::UpdateAlarmMessage( RwInt32 eStringID, RwBool bServerSync , sMsgBoxData* pExData )
{
	const WCHAR* szText = GetDisplayStringManager()->GetString( eStringID, (CDisplayStringManager::EDispType)bServerSync );

	if( !szText || wcslen( szText ) == 0 )
	{
		if( Logic_IsUIDevInfoVisible() )
		{
			WCHAR buf[256];
			swprintf_s( buf, 256, L"%d(0:Client,1:Server), %d(stringID) is not exist", (RwInt32)bServerSync, eStringID );

			CNtlStage *pStage = CStageManager::GetInstance()->GetActiveStage();
			std::string strStageName = pStage->GetName();

			if(strStageName == GAME_STAGE_NAME)
				CDboEventGenerator::MsgBoxShow( buf, MBW_NONE, MBTF_OK );
			else
				CDboEventGenerator::NetConnectBoxShow( buf, MBW_NONE, MBTF_OK );			
		}

		return;		
	}		

	AlarmProcess( szText, eStringID, bServerSync, pExData, 0.0f, TRUE );
}

VOID CAlarmManager::UpdateFormattedAlarmMessage( RwInt32 eStringID, RwBool bServerSync, sMsgBoxData* pExData, ... )
{
	va_list args;
	va_start( args, pExData );
	UpdatevFormattedAlarmMessage( eStringID, bServerSync, pExData, args );	
	va_end( args );
}

VOID CAlarmManager::UpdatevFormattedAlarmMessage( RwInt32 eStringID, RwBool bServerSync, sMsgBoxData* pExData, va_list ArgList )
{
	WCHAR formattedText[GUI_TEXT_BUFFER_SIZE];
	const WCHAR* szText = GetDisplayStringManager()->GetString( eStringID, bServerSync );

	if( !szText || wcslen( szText ) == 0 )
	{
		if( Logic_IsUIDevInfoVisible() )
		{
			swprintf_s( formattedText, GUI_TEXT_BUFFER_SIZE, L"Client %d(stringID) is not exist", eStringID );

			CNtlStage *pStage = CStageManager::GetInstance()->GetActiveStage();
			std::string strStageName = pStage->GetName();

			if(strStageName == GAME_STAGE_NAME)
				CDboEventGenerator::MsgBoxShow( formattedText, MBW_NONE, MBTF_OK );
			else
				CDboEventGenerator::NetConnectBoxShow( formattedText, MBW_NONE, MBTF_OK );

		}

		return;
	}

	vswprintf_s( formattedText, GUI_TEXT_BUFFER_SIZE, szText, ArgList );

	AlarmProcess( formattedText, eStringID, bServerSync, pExData, 0.0f, TRUE );
}

VOID CAlarmManager::AlarmProcess( const WCHAR* pAlarmMsg, RwInt32 eStringID, RwBool bServerSync, sMsgBoxData* pExData, RwReal fShowTime, RwBool bMsgBoxUpdate /* = FALSE  */ )
{
	std::map<RwInt32,sALARMITEM>::iterator it;

	sALARMITEM sAlarmItem;

	if( bServerSync )
	{
		it = m_mapServerAlarm.find( eStringID );
		if( it != m_mapServerAlarm.end() )
			sAlarmItem = (*it).second;		
	}	
	else
	{
		it = m_mapClientAlarm.find( eStringID );
		if( it != m_mapClientAlarm.end() )
			sAlarmItem = (*it).second;		
	}

	if( bMsgBoxUpdate &&
		!( sAlarmItem.uType & ALARM_TYPE_MSGBOX || sAlarmItem.uType & ALARM_TYPE_LOBBY_MSGBOX ) )
		return;

	if( RepeatCheck( (CAlarmManager::eALARMTYPE)sAlarmItem.uType, eStringID, bServerSync ) )
		return;

	if( fShowTime > 0.0f )
		sAlarmItem.fMsgBoxShowTime = fShowTime;

	if( sAlarmItem.uType & ALARM_TYPE_CHAT_WARN )
	{
		RwInt32 nLength = wcslen( pAlarmMsg );
		CDboEventGenerator::SayMessage( CHAT_TYPE_SYSTEM, SENDERNAME_SYSTEM, (RwUInt16)nLength, pAlarmMsg );
	}

	if( sAlarmItem.uType & ALARM_TYPE_CHAT_NOTICE )
	{
		RwInt32 nLength = wcslen( pAlarmMsg );
		CDboEventGenerator::SayMessage( CHAT_TYPE_SYSTEM, SENDERNAME_NOTICE, (RwUInt16)nLength, pAlarmMsg );
	}

	if( sAlarmItem.uType & ALARM_TYPE_MSGBOX )
	{
		CDboEventGenerator::MsgBoxShow( pAlarmMsg, sAlarmItem.uWorkID, sAlarmItem.flagMsgBoxStyle, sAlarmItem.bMsgBoxPopup, sAlarmItem.fMsgBoxShowTime, pExData, &sAlarmItem.listCustomBtn, sAlarmItem.bAcceptDuplicate, eStringID, bMsgBoxUpdate );		
	}

	if( sAlarmItem.uType & ALARM_TYPE_LOG )
	{

	}

	if( sAlarmItem.uType & ALARM_ACTION_NOTIFY )
	{
		CDboEventGenerator::NotifyMessage( SDboEventNotify::ACTION, pAlarmMsg );		
	}

	if( sAlarmItem.uType & ALARM_TIMEFIXED_NOTIFY )
	{
		CDboEventGenerator::TimeFixedNotifyMessage( TRUE, eStringID, pAlarmMsg );
	}

	if( sAlarmItem.uType & ALARM_TYPE_LOBBY_MSGBOX )
	{
		CDboEventGenerator::NetConnectBoxShow( pAlarmMsg, sAlarmItem.uWorkID, sAlarmItem.flagMsgBoxStyle, sAlarmItem.fMsgBoxShowTime, pExData, &sAlarmItem.listCustomBtn, sAlarmItem.bAcceptDuplicate, eStringID, bMsgBoxUpdate );
	}
}

RwBool CAlarmManager::RepeatCheck( eALARMTYPE eAlarmType, RwInt32 eStringID, RwBool bServerSync )
{
	if( eAlarmType & ( ALARM_TYPE_LOBBY_MSGBOX | ALARM_TYPE_MSGBOX | ALARM_TIMEFIXED_NOTIFY | ALARM_ACTION_NOTIFY ) )
		return FALSE;

	if( m_RepeatMessageCheck.nLastChatMessageID == eStringID && m_RepeatMessageCheck.nLastChatMessageType == bServerSync )
	{
		++m_RepeatMessageCheck.nRepeatCount;

		if( m_RepeatMessageCheck.nRepeatCount > 3 )
			return TRUE;		
		else
			return FALSE;
	}
	else
	{
		m_RepeatMessageCheck.nRepeatCount = 0;				

		return FALSE;
	}

	m_RepeatMessageCheck.nLastChatMessageType = bServerSync;
	m_RepeatMessageCheck.nLastChatMessageID = eStringID;
}

RwBool CAlarmManager::ParseData( const CDisplayParser& dispParser, std::string strFilename, RwBool bServer )
{
	CallTokenPack fnTokenPack = NULL;

	if( GetNtlResourcePackManager()->GetActiveFlags() & NTL_PACK_TYPE_FLAG_LANGUAGE )
		fnTokenPack = LanguagePackLoad;

	CNtlTokenizer lexer( strFilename, fnTokenPack );

	if( !lexer.IsSuccess() )
		return FALSE;

	enum EParseState
	{
		PARSE_STATE_ALARM_ID,
		PARSE_STATE_ALARM_TYPE,
		PARSE_STATE_MSGBOX_WORKID,
		PARSE_STATE_MSGBOX_STYLE,
		PARSE_STATE_MSGBOX_POPUP,
		PARSE_STATE_MSGBOX_TIME,		
		PARSE_STATE_MSGBOX_DUPLICATE,
		PARSE_STATE_MSGBOX_CUSTOM_BTN_STRING,
		PARSE_STATE_MSGBOX_CUSTOM_BTN_WORKID,
	};

	EParseState eState = PARSE_STATE_ALARM_ID;
	const CDisplayParser::MapDefOption& mapDefOption = dispParser.GetDefOptions();
	CDisplayParser::MapDefOption::const_iterator DefIt;
	std::map<std::string,RwInt32>::const_iterator WorkIt;

	std::string strStringID;
	RwBool bReadData = TRUE;
	RwBool bInsertData = FALSE;
	sALARMITEM stAlarmItem;	
	sMsgBoxCustomBtn stCustomBtn;

	while( TRUE )
	{
		const std::string& token = lexer.PeekNextToken();

		if( token.empty() )
			break;

		switch( eState )
		{
		case PARSE_STATE_ALARM_ID:
			{
				if( token == "=" )
				{
					if( strStringID.empty() )
					{
						ErrorMessage( strStringID.c_str(), "ALARM_ID is invalid" );						
						return FALSE;
					}

					eState = PARSE_STATE_ALARM_TYPE;
					bReadData = TRUE;
				}
				else if( token == "," || token == ";" )
				{
					ErrorMessage( strStringID.c_str(), "ALARM_ID has Invalid parameter" );
					return FALSE;
				}
				else 
				{
					strStringID = token;
				}
			}
			break;
		case PARSE_STATE_ALARM_TYPE:
			{
				if( token == "," )
				{
					eState = PARSE_STATE_MSGBOX_WORKID;
					bReadData = TRUE;
				}
				else if( token == ";" )
				{
					eState = PARSE_STATE_ALARM_ID;
					bReadData = TRUE;
					bInsertData = TRUE;
				}
				else if( token == "=" )
				{
					ErrorMessage( strStringID.c_str(), "ALARM_TYPE Invalid parameter" );
					return FALSE;
				}
				else
				{
					if( bReadData )
					{
						stAlarmItem.uType = (RwUInt32)( atoi( token.c_str() ) );
						bReadData = FALSE;

						if( stAlarmItem.uType & ALARM_TYPE_LOBBY_MSGBOX && stAlarmItem.uType & ALARM_TYPE_MSGBOX )
						{
							ErrorMessage( strStringID.c_str(), "Msgbox Type can't duplicated" );
							return FALSE;
						}
					}
					else
					{
						ErrorMessage( strStringID.c_str(), "ALARM_TYPE value input is twiced" );
						return FALSE;
					}
				}
			}
			break;
		case PARSE_STATE_MSGBOX_WORKID:
			{
				if( token == "," )
				{
					eState = PARSE_STATE_MSGBOX_STYLE;
					bReadData = TRUE;
				}
				else if( token == ";" )
				{
					eState = PARSE_STATE_ALARM_ID;
					bReadData = TRUE;
					bInsertData = TRUE;
				}
				else if( token == "=" )
				{
					ErrorMessage( strStringID.c_str(), "MSGBOX_WORKID Invalid parameter" );
					return FALSE;
				}
				else
				{
					if( bReadData )
					{
						std::map<std::string,RwInt32>::iterator iter;
						iter = m_mapWorkID.find( token );

						if( iter == m_mapWorkID.end() )
						{
							ErrorMessage( strStringID.c_str(), "MSGBOX_WORKID is Invalid" );
							return FALSE;
						}

						stAlarmItem.uWorkID = (RwUInt32)( (*iter).second );
						bReadData = FALSE;						
					}
					else
					{
						ErrorMessage( strStringID.c_str(), "MSGBOX_WORKID value input is twiced" );
						return FALSE;
					}
				}
			}
			break;
		case PARSE_STATE_MSGBOX_STYLE:
			{
				if( token == "," )
				{
					eState = PARSE_STATE_MSGBOX_POPUP;
					bReadData = TRUE;
				}
				else if( token == ";" )
				{
					eState = PARSE_STATE_ALARM_ID;
					bReadData = TRUE;
					bInsertData = TRUE;
				}
				else if( token == "=" )
				{
					ErrorMessage( strStringID.c_str(), "MSGBOX_STYLE Invalid parameter" );
					return FALSE;
				}
				else
				{
					if( bReadData )
					{
						RwUInt32 flagMsgBoxStyle = (RwUInt32)( atoi( token.c_str() ) );

						// 추후 NetMessageBox와 통합 예정.
						//if( stAlarmItem.uType & ALARM_TYPE_MSGBOX )
						//{
						//	if( uMsgBoxStyle == MBT_CANCEL )
						//	{
						//		ErrorMessage( wstrStringID.c_str(), "CAlarmManager::ParseData : LOBBY_MSGBOX doesn't support Cancel type" );
						//		return FALSE;
						//	}
						//}
						//else if( stAlarmItem.uType & ALARM_TYPE_LOBBY_MSGBOX )
						//{
						//	if( uMsgBoxStyle == MBT_INPUT )
						//	{
						//		ErrorMessage( wstrStringID.c_str(), "CAlarmManager::ParseData : LOBBY_MSGBOX doesn't support INPUT type" );
						//		return FALSE;
						//	}
						//}

						stAlarmItem.flagMsgBoxStyle = flagMsgBoxStyle;
						bReadData = FALSE;
					}
					else
					{
						ErrorMessage( strStringID.c_str(), "MSGBOX_STYLE value input is twiced" );
						return FALSE;
					}
				}				
			}
			break;
		case PARSE_STATE_MSGBOX_POPUP:
			{
				if( token == "," )
				{
					eState = PARSE_STATE_MSGBOX_TIME;
					bReadData = TRUE;
				}
				else if( token == ";" )
				{
					eState = PARSE_STATE_ALARM_ID;
					bReadData = TRUE;
					bInsertData = TRUE;
				}
				else if( token == "=" )
				{
					ErrorMessage( strStringID.c_str(), "MSGBOX_POPUP Invalid parameter" );
					return FALSE;
				}
				else
				{
					if( bReadData )
					{
						stAlarmItem.bMsgBoxPopup = (RwUInt32)( atoi( token.c_str() ) );
						bReadData = FALSE;
					}
					else
					{
						ErrorMessage( strStringID.c_str(), "MSGBOX_POPUP value input is twiced" );	
						return FALSE;
					}
				}
			}
			break;
		case PARSE_STATE_MSGBOX_TIME:
			{
				if( token == "," )
				{
					eState = PARSE_STATE_MSGBOX_DUPLICATE;
					bReadData = TRUE;					
				}
				else if( token == ";" )
				{
					eState = PARSE_STATE_ALARM_ID;
					bReadData = TRUE;
					bInsertData = TRUE;
				}
				else if( token == "=" )
				{
					ErrorMessage( strStringID.c_str(), "MSGBOX_TIME Invalid parameter" );
					return FALSE;
				}
				else
				{
					if( bReadData )
					{
						stAlarmItem.fMsgBoxShowTime = (RwReal)( atof( token.c_str() ) );
						bReadData = FALSE;
					}
					else
					{
						ErrorMessage( strStringID.c_str(), "MSGBOX_TIME value input is twiced" );							
						return FALSE;
					}
				}
			}
			break;		
		case PARSE_STATE_MSGBOX_DUPLICATE:
			{
				if( token == "," )
				{
					eState = PARSE_STATE_MSGBOX_CUSTOM_BTN_STRING;
					bReadData = TRUE;					
				}
				else if( token == ";" )
				{
					eState = PARSE_STATE_ALARM_ID;
					bReadData = TRUE;
					bInsertData = TRUE;
				}
				else if( token == "=" )
				{
					ErrorMessage( strStringID.c_str(), "MSGBOX_Duplicate Invalid parameter" );
					return FALSE;
				}
				else
				{
					if( bReadData )
					{
						stAlarmItem.bAcceptDuplicate = (RwUInt32)( atoi( token.c_str() ) );
						bReadData = FALSE;
					}
					else
					{
						ErrorMessage( strStringID.c_str(), "MSGBOX_Duplicate value input is twiced" );							
						return FALSE;
					}
				}
			}
			break;
		case PARSE_STATE_MSGBOX_CUSTOM_BTN_STRING:
			{
				if( token == "," )
				{
					eState = PARSE_STATE_MSGBOX_CUSTOM_BTN_WORKID;
					bReadData = TRUE;
				}
				else if( token == ";" )
				{
					stCustomBtn.uiWorkID = MBW_NONE;
					stAlarmItem.listCustomBtn.push_back( stCustomBtn );		
					stCustomBtn.Init();

					eState = PARSE_STATE_ALARM_ID;
					bReadData = TRUE;
					bInsertData = TRUE;				
				}
				else if( token == "=" )
				{
					ErrorMessage( strStringID.c_str(), "MSGBOX_CUSTOM_BTN_STRING Invalid parameter" );
					return FALSE;
				}
				else
				{
					if( bReadData )
					{
						DefIt = mapDefOption.find( token );
						if( DefIt != mapDefOption.end() )
						{
							stCustomBtn.uiStringID = (*DefIt).second;	
							bReadData = FALSE;
						}
						else
						{
							ErrorMessage( strStringID.c_str(), "MSGBOX_CUSTOM_BTN_STRING StringID is Invalid" );
							return FALSE;
						}						
					}
					else
					{
						ErrorMessage( strStringID.c_str(), "MSGBOX_CUSTOM_BTN_STRING value input is twiced" );							
						return FALSE;
					}
				}
			}
			break;		
		case PARSE_STATE_MSGBOX_CUSTOM_BTN_WORKID:
			{
				if( token == "," )
				{
					stAlarmItem.listCustomBtn.push_back( stCustomBtn );		
					stCustomBtn.Init();

					eState = PARSE_STATE_MSGBOX_CUSTOM_BTN_STRING;
					bReadData = TRUE;
				}
				else if( token == ";" )
				{
					stAlarmItem.listCustomBtn.push_back( stCustomBtn );		
					stCustomBtn.Init();

					eState = PARSE_STATE_ALARM_ID;
					bReadData = TRUE;
					bInsertData = TRUE;				
				}
				else if( token == "=" )
				{
					ErrorMessage( strStringID.c_str(), "MSGBOX_CUSTOM_BTN_WORKID Invalid parameter" );
					return FALSE;
				}
				else
				{
					if( bReadData )
					{
						WorkIt = m_mapWorkID.find( token );
						if( WorkIt != m_mapWorkID.end() )
						{
							stCustomBtn.uiWorkID = (*WorkIt).second;	
							bReadData = FALSE;
						}
						else
						{
							ErrorMessage( strStringID.c_str(), "MSGBOX_CUSTOM_BTN_WORKID StringID is Invalid" );
							return FALSE;
						}						
					}
					else
					{
						ErrorMessage( strStringID.c_str(), "MSGBOX_CUSTOM_BTN_WORKID value input is twiced" );							
						return FALSE;
					}
				}
			}
			break;
		}

		if( bInsertData )
		{
			DefIt = mapDefOption.find( strStringID );
			if( DefIt != mapDefOption.end() )
			{
				if( bServer )
					m_mapServerAlarm[(*DefIt).second] = stAlarmItem;
				else
					m_mapClientAlarm[(*DefIt).second] = stAlarmItem;
			}
			else
			{
				ErrorMessage( strStringID.c_str(), "Insert Phase Mismatched String ID" );							
				return FALSE;
			}

			bInsertData = FALSE;
			stAlarmItem.Init();
		}		
	}

	return TRUE;
}

RwBool CAlarmManager::ParseDataW( const CDisplayParser& dispParser, std::string strFilename, RwBool bServer )
{
	CallTokenPack fnTokenPack = NULL;

	if( GetNtlResourcePackManager()->GetActiveFlags() & NTL_PACK_TYPE_FLAG_LANGUAGE )
		fnTokenPack = LanguagePackLoad;

	CNtlTokenizerW lexer( strFilename, fnTokenPack );

	if( !lexer.IsSuccess() )
		return FALSE;

	enum EParseState
	{
		PARSE_STATE_ALARM_ID,
		PARSE_STATE_ALARM_TYPE,
		PARSE_STATE_MSGBOX_WORKID,
		PARSE_STATE_MSGBOX_STYLE,
		PARSE_STATE_MSGBOX_POPUP,
		PARSE_STATE_MSGBOX_TIME,
		PARSE_STATE_MSGBOX_DUPLICATE,
		PARSE_STATE_MSGBOX_CUSTOM_BTN_STRING,
		PARSE_STATE_MSGBOX_CUSTOM_BTN_WORKID,
	};

	EParseState eState = PARSE_STATE_ALARM_ID;
	const CDisplayParser::MapDefOptionW& mapDefOption = dispParser.GetDefOptionsW();
	CDisplayParser::MapDefOptionW::const_iterator DefIt;
	std::map<std::wstring,RwInt32>::const_iterator WorkIt;

	std::wstring wstrStringID;
	RwBool bReadData = TRUE;
	RwBool bInsertData = FALSE;
	sALARMITEM stAlarmItem;	
	sMsgBoxCustomBtn stCustomBtn;
	
	while( TRUE )
	{
		const std::wstring& token = lexer.PeekNextToken();

		if( token.empty() )
			break;

		switch( eState )
		{
		case PARSE_STATE_ALARM_ID:
			{
				if( token == L"=" )
				{
					if( wstrStringID.empty() )
					{
						ErrorMessage( wstrStringID.c_str(), "ALARM_ID is invalid" );						
						return FALSE;
					}

					eState = PARSE_STATE_ALARM_TYPE;
					bReadData = TRUE;
				}
				else if( token == L"," || token == L";" )
				{
					ErrorMessage( wstrStringID.c_str(), "ALARM_ID has Invalid parameter" );
					return FALSE;
				}
				else 
				{
					wstrStringID = token;
				}
			}
			break;
		case PARSE_STATE_ALARM_TYPE:
			{
				if( token == L"," )
				{
					eState = PARSE_STATE_MSGBOX_WORKID;
					bReadData = TRUE;
				}
				else if( token == L";" )
				{
					eState = PARSE_STATE_ALARM_ID;
					bReadData = TRUE;
					bInsertData = TRUE;
				}
				else if( token == L"=" )
				{
					ErrorMessage( wstrStringID.c_str(), "ALARM_TYPE Invalid parameter" );
					return FALSE;
				}
				else
				{
					if( bReadData )
					{
						stAlarmItem.uType = (RwUInt32)( _wtoi( token.c_str() ) );
						bReadData = FALSE;

						if( stAlarmItem.uType & ALARM_TYPE_LOBBY_MSGBOX && stAlarmItem.uType & ALARM_TYPE_MSGBOX )
						{
							ErrorMessage( wstrStringID.c_str(), "Msgbox Type can't duplicated" );
							return FALSE;
						}
					}
					else
					{
						ErrorMessage( wstrStringID.c_str(), "ALARM_TYPE value input is twiced" );
						return FALSE;
					}
				}
			}
			break;
		case PARSE_STATE_MSGBOX_WORKID:
			{
				if( token == L"," )
				{
					eState = PARSE_STATE_MSGBOX_STYLE;
					bReadData = TRUE;
				}
				else if( token == L";" )
				{
					eState = PARSE_STATE_ALARM_ID;
					bReadData = TRUE;
					bInsertData = TRUE;
				}
				else if( token == L"=" )
				{
					ErrorMessage( wstrStringID.c_str(), "MSGBOX_WORKID Invalid parameter" );
					return FALSE;
				}
				else
				{
					if( bReadData )
					{
						std::map<std::wstring,RwInt32>::iterator iter;
						iter = m_mapWorkIDW.find( token );

						if( iter == m_mapWorkIDW.end() )
						{
							ErrorMessage( wstrStringID.c_str(), "MSGBOX_WORKID is Invalid" );
							return FALSE;
						}

						stAlarmItem.uWorkID = (RwUInt32)( (*iter).second );
						bReadData = FALSE;						
					}
					else
					{
						ErrorMessage( wstrStringID.c_str(), "MSGBOX_WORKID value input is twiced" );
						return FALSE;
					}
				}
			}
			break;
		case PARSE_STATE_MSGBOX_STYLE:
			{
				if( token == L"," )
				{
					eState = PARSE_STATE_MSGBOX_POPUP;
					bReadData = TRUE;
				}
				else if( token == L";" )
				{
					eState = PARSE_STATE_ALARM_ID;
					bReadData = TRUE;
					bInsertData = TRUE;
				}
				else if( token == L"=" )
				{
					ErrorMessage( wstrStringID.c_str(), "MSGBOX_STYLE Invalid parameter" );
					return FALSE;
				}
				else
				{
					if( bReadData )
					{
						RwUInt32 flagMsgBoxStyle = (RwUInt32)( _wtoi( token.c_str() ) );

						// 추후 NetMessageBox와 통합 예정.
						//if( stAlarmItem.uType & ALARM_TYPE_MSGBOX )
						//{
						//	if( uMsgBoxStyle == MBT_CANCEL )
						//	{
						//		ErrorMessage( wstrStringID.c_str(), "CAlarmManager::ParseData : LOBBY_MSGBOX doesn't support Cancel type" );
						//		return FALSE;
						//	}
						//}
						//else if( stAlarmItem.uType & ALARM_TYPE_LOBBY_MSGBOX )
						//{
						//	if( uMsgBoxStyle == MBT_INPUT )
						//	{
						//		ErrorMessage( wstrStringID.c_str(), "CAlarmManager::ParseData : LOBBY_MSGBOX doesn't support INPUT type" );
						//		return FALSE;
						//	}
						//}

						stAlarmItem.flagMsgBoxStyle = flagMsgBoxStyle;
						bReadData = FALSE;
					}
					else
					{
						ErrorMessage( wstrStringID.c_str(), "MSGBOX_STYLE value input is twiced" );
						return FALSE;
					}
				}				
			}
			break;
		case PARSE_STATE_MSGBOX_POPUP:
			{
				if( token == L"," )
				{
					eState = PARSE_STATE_MSGBOX_TIME;
					bReadData = TRUE;
				}
				else if( token == L";" )
				{
					eState = PARSE_STATE_ALARM_ID;
					bReadData = TRUE;
					bInsertData = TRUE;
				}
				else if( token == L"=" )
				{
					ErrorMessage( wstrStringID.c_str(), "MSGBOX_POPUP Invalid parameter" );
					return FALSE;
				}
				else
				{
					if( bReadData )
					{
						stAlarmItem.bMsgBoxPopup = (RwUInt32)( _wtoi( token.c_str() ) );
						bReadData = FALSE;
					}
					else
					{
						ErrorMessage( wstrStringID.c_str(), "MSGBOX_POPUP value input is twiced" );	
						return FALSE;
					}
				}
			}
			break;
		case PARSE_STATE_MSGBOX_TIME:
			{
				if( token == L"," )
				{
					eState = PARSE_STATE_MSGBOX_DUPLICATE;
					bReadData = TRUE;					
				}
				else if( token == L";" )
				{
					eState = PARSE_STATE_ALARM_ID;
					bReadData = TRUE;
					bInsertData = TRUE;
				}
				else if( token == L"=" )
				{
					ErrorMessage( wstrStringID.c_str(), "MSGBOX_TIME Invalid parameter" );
					return FALSE;
				}
				else
				{
					if( bReadData )
					{
						stAlarmItem.fMsgBoxShowTime = (RwReal)( _wtof( token.c_str() ) );
						bReadData = FALSE;
					}
					else
					{
						ErrorMessage( wstrStringID.c_str(), "MSGBOX_TIME value input is twiced" );							
						return FALSE;
					}
				}
			}
			break;	
		case PARSE_STATE_MSGBOX_DUPLICATE:
			{
				if( token == L"," )
				{
					eState = PARSE_STATE_MSGBOX_CUSTOM_BTN_STRING;
					bReadData = TRUE;					
				}
				else if( token == L";" )
				{
					eState = PARSE_STATE_ALARM_ID;
					bReadData = TRUE;
					bInsertData = TRUE;
				}
				else if( token == L"=" )
				{
					ErrorMessage( wstrStringID.c_str(), "MSGBOX_Duplicate Invalid parameter" );
					return FALSE;
				}
				else
				{
					if( bReadData )
					{
						stAlarmItem.bAcceptDuplicate = (RwUInt32)( _wtoi( token.c_str() ) );
						bReadData = FALSE;
					}
					else
					{
						ErrorMessage( wstrStringID.c_str(), "MSGBOX_Duplicate value input is twiced" );							
						return FALSE;
					}
				}
			}
			break;
		case PARSE_STATE_MSGBOX_CUSTOM_BTN_STRING:
			{
				if( token == L"," )
				{
					eState = PARSE_STATE_MSGBOX_CUSTOM_BTN_WORKID;
					bReadData = TRUE;
				}
				else if( token == L";" )
				{
					stCustomBtn.uiWorkID = MBW_NONE;
					stAlarmItem.listCustomBtn.push_back( stCustomBtn );		
					stCustomBtn.Init();

					eState = PARSE_STATE_ALARM_ID;
					bReadData = TRUE;
					bInsertData = TRUE;				
				}
				else if( token == L"=" )
				{
					ErrorMessage( wstrStringID.c_str(), "MSGBOX_CUSTOM_BTN_STRING Invalid parameter" );
					return FALSE;
				}
				else
				{
					if( bReadData )
					{
						DefIt = mapDefOption.find( token );
						if( DefIt != mapDefOption.end() )
						{
							stCustomBtn.uiStringID = (*DefIt).second;	
							bReadData = FALSE;
						}
						else
						{
							ErrorMessage( wstrStringID.c_str(), "MSGBOX_CUSTOM_BTN_STRING StringID is Invalid" );
							return FALSE;
						}						
					}
					else
					{
						ErrorMessage( wstrStringID.c_str(), "MSGBOX_CUSTOM_BTN_STRING value input is twiced" );							
						return FALSE;
					}
				}
			}
			break;		
		case PARSE_STATE_MSGBOX_CUSTOM_BTN_WORKID:
			{
				if( token == L"," )
				{
					stAlarmItem.listCustomBtn.push_back( stCustomBtn );		
					stCustomBtn.Init();

					eState = PARSE_STATE_MSGBOX_CUSTOM_BTN_STRING;
					bReadData = TRUE;
				}
				else if( token == L";" )
				{
					stAlarmItem.listCustomBtn.push_back( stCustomBtn );		
					stCustomBtn.Init();

					eState = PARSE_STATE_ALARM_ID;
					bReadData = TRUE;
					bInsertData = TRUE;				
				}
				else if( token == L"=" )
				{
					ErrorMessage( wstrStringID.c_str(), "MSGBOX_CUSTOM_BTN_WORKID Invalid parameter" );
					return FALSE;
				}
				else
				{
					if( bReadData )
					{
						WorkIt = m_mapWorkIDW.find( token );
						if( WorkIt != m_mapWorkIDW.end() )
						{
							stCustomBtn.uiWorkID = (*WorkIt).second;	
							bReadData = FALSE;
						}
						else
						{
							ErrorMessage( wstrStringID.c_str(), "MSGBOX_CUSTOM_BTN_WORKID StringID is Invalid" );
							return FALSE;
						}						
					}
					else
					{
						ErrorMessage( wstrStringID.c_str(), "MSGBOX_CUSTOM_BTN_WORKID value input is twiced" );							
						return FALSE;
					}
				}
			}
			break;
		}

		if( bInsertData )
		{
			DefIt = mapDefOption.find( wstrStringID );
			if( DefIt != mapDefOption.end() )
			{
				if( bServer )
					m_mapServerAlarm[(*DefIt).second] = stAlarmItem;
				else
					m_mapClientAlarm[(*DefIt).second] = stAlarmItem;
			}
			else
			{
				ErrorMessage( wstrStringID.c_str(), "Insert Phase Mismatched String ID" );							
				return FALSE;
			}

			bInsertData = FALSE;
			stAlarmItem.Init();
		}		
	}

	return TRUE;
}

RwBool CAlarmManager::ParseWorkID( std::string strFilename )
{
	CallTokenPack fnTokenPack = NULL;

	if( GetNtlResourcePackManager()->GetActiveFlags() & NTL_PACK_TYPE_FLAG_LANGUAGE )
		fnTokenPack = LanguagePackLoad;

	CNtlTokenizer lexer( strFilename, fnTokenPack );

	if( !lexer.IsSuccess() )
		return FALSE;

	enum EParseState
	{
		PARSE_STATE_GLOBAL,
		PARSE_STATE_WORKID,
		PARSE_STATE_WORKID_NAME,
		PARSE_STATE_WORKID_WAIT_VALUE,
		PARSE_STATE_WORKID_VALUE,		
	};

	EParseState eState = PARSE_STATE_GLOBAL;
	
	RwChar chType[1024];
	RwInt32 nIdx = 0;	
	RwBool bRun = TRUE;

	while( bRun )
	{
		const std::string& token = lexer.PeekNextToken();

		if( token.empty() )
			break;

		switch( eState )
		{
		case PARSE_STATE_GLOBAL:
			{
				if( token == "enum" )
					eState = PARSE_STATE_WORKID;
			}
			break;
		case PARSE_STATE_WORKID:
			{
				if( token == "{" )
					eState = PARSE_STATE_WORKID_NAME;				
			}
			break;
		case PARSE_STATE_WORKID_NAME:
			{
				strcpy_s( chType, 1024, token.c_str() );
				eState = PARSE_STATE_WORKID_VALUE;
			}
			break;
		case PARSE_STATE_WORKID_WAIT_VALUE:
			{
				if( token == "," )
				{
					m_mapWorkID[chType] = nIdx;
					++nIdx;
					eState = PARSE_STATE_WORKID_NAME;
				}
				else if( token == "}" )
				{
					bRun = FALSE;
				}
				else
				{
					nIdx = atoi( token.c_str() );
				}
			}
			break;
		case PARSE_STATE_WORKID_VALUE:
			{
				if( token == "," )
				{
					m_mapWorkID[chType] = nIdx;
					++nIdx;
					eState = PARSE_STATE_WORKID_NAME;
				}
				else if( token == "=" )
				{
					eState = PARSE_STATE_WORKID_WAIT_VALUE;
				}
				else if( token == "}" )
				{
					bRun = FALSE;
				}
			}
			break;		
		}
	}

	return TRUE;
}

RwBool CAlarmManager::ParseWorkIDW( std::string strFilename )
{
	CallTokenPack fnTokenPack = NULL;

	if( GetNtlResourcePackManager()->GetActiveFlags() & NTL_PACK_TYPE_FLAG_LANGUAGE )
		fnTokenPack = LanguagePackLoad;

	CNtlTokenizerW lexer( strFilename, fnTokenPack );

	if( !lexer.IsSuccess() )
		return FALSE;

	enum EParseState
	{
		PARSE_STATE_GLOBAL,
		PARSE_STATE_WORKID,
		PARSE_STATE_WORKID_NAME,
		PARSE_STATE_WORKID_WAIT_VALUE,
		PARSE_STATE_WORKID_VALUE,		
	};

	EParseState eState = PARSE_STATE_GLOBAL;

	WCHAR chType[1024];
	RwInt32 nIdx = 0;	
	RwBool bRun = TRUE;

	while( bRun )
	{
		const std::wstring& token = lexer.PeekNextToken();

		if( token.empty() )
			break;

		switch( eState )
		{
		case PARSE_STATE_GLOBAL:
			{
				if( token == L"enum" )
					eState = PARSE_STATE_WORKID;
			}
			break;
		case PARSE_STATE_WORKID:
			{
				if( token == L"{" )
					eState = PARSE_STATE_WORKID_NAME;				
			}
			break;
		case PARSE_STATE_WORKID_NAME:
			{
				wcscpy_s( chType, 1024, token.c_str() );
				eState = PARSE_STATE_WORKID_VALUE;
			}
			break;
		case PARSE_STATE_WORKID_WAIT_VALUE:
			{
				if( token == L"," )
				{
					m_mapWorkIDW[chType] = nIdx;
					++nIdx;
					eState = PARSE_STATE_WORKID_NAME;
				}
				else if( token == L"}" )
				{
					bRun = FALSE;
				}
				else
				{
					nIdx = _wtoi( token.c_str() );
				}
			}
			break;
		case PARSE_STATE_WORKID_VALUE:
			{
				if( token == L"," )
				{
					m_mapWorkIDW[chType] = nIdx;
					++nIdx;
					eState = PARSE_STATE_WORKID_NAME;
				}
				else if( token == L"=" )
				{
					eState = PARSE_STATE_WORKID_WAIT_VALUE;
				}
				else if( token == L"}" )
				{
					bRun = FALSE;
				}
			}
			break;		
		}
	}

	return TRUE;
}

VOID CAlarmManager::ErrorMessage( const RwChar* szID, const RwChar* szErrText )
{
	NTL_ASSERTFAIL( "[ " << szID << " ] " << szErrText );
}

VOID CAlarmManager::ErrorMessage( const WCHAR* wszID, const RwChar* szErrText )
{
	RwChar acBuffer[1024];
	::WideCharToMultiByte( GetACP(), 0, wszID, -1, acBuffer, 1024, NULL, NULL );
	NTL_ASSERTFAIL( "[" << acBuffer << " ] " << szErrText );
}