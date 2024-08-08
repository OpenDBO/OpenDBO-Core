#include "precomp_dboclient.h"
#include "ChatReceiver.h"

// core
#include "NtlDebug.h"

// simulation
#include "NtlSobManager.h"
#include "NtlSobPlayer.h"
#include "NtlSobPlayerAttr.h"
#include "NtlSLLogic.h"
#include "NtlSLParty.h"
#include "NtlSobAvatar.h"
#include "NtlSLGlobal.h"

// dbo
#include "DboEventGenerator.h"
#include "ChatDisplayGui.h"
#include "AlarmManager.h"
#include "ChatBalloonManager.h"
#include "DboLogic.h"
#include "DiceManager.h"


CChatReceiver::CChatReceiver()
{

}

CChatReceiver::~CChatReceiver()
{

}

RwBool CChatReceiver::Create()
{
	NTL_FUNCTION( "CChatReceiver::Create" );

	LinkMsg(g_EventSayMessage);
	LinkMsg(g_EventDiceResultShow);

	NTL_RETURN(TRUE);
}

VOID CChatReceiver::Destroy()
{
	NTL_FUNCTION("CChatReceiver::Destroy");


	ITER_CHATLOG it_chatLog = m_listChatLog.begin();
	for( ; it_chatLog != m_listChatLog.end() ; ++it_chatLog )
	{
		sChatLog* pChatLog = *it_chatLog;
		NTL_DELETE(pChatLog);
	}
	m_listChatLog.clear();

	m_listNotify.clear();


	UnLinkMsg(g_EventSayMessage);
	UnLinkMsg(g_EventDiceResultShow);

	NTL_RETURNVOID();
}

ITER_CHATLOG CChatReceiver::Begin()
{
	return m_listChatLog.begin();
}

ITER_CHATLOG CChatReceiver::End()
{
	return m_listChatLog.end();
}

VOID CChatReceiver::RegisterNotify(CChatDisplayGui* pDisplayDialog)
{
	m_listNotify.push_back(pDisplayDialog);
}

VOID CChatReceiver::UnregisterNotify(CChatDisplayGui* pDisplayDialog)
{
	ITER_NOTIFY it_notify = m_listNotify.begin();
	for( ; it_notify != m_listNotify.end() ; ++it_notify )
	{
		CChatDisplayGui* pRegDisplayGui = *it_notify;

		if( pRegDisplayGui == pDisplayDialog )
		{
			m_listNotify.erase(it_notify);
			return;
		}
	}
}

VOID CChatReceiver::Chatlog_Save(RwUInt8 byType, const WCHAR* pText)
{
	if( m_listChatLog.size() > CHAT_DISP_MAX_LINE )
	{
		ITER_CHATLOG it_chatLog = m_listChatLog.begin();
		sChatLog* pDelChatLog = *it_chatLog;

		NTL_DELETE(pDelChatLog);
		m_listChatLog.erase(it_chatLog);
	}

	sChatLog* pNewChatLog = NTL_NEW sChatLog;
	pNewChatLog->wstrString	= pText;
	pNewChatLog->byChatType	= byType;

	m_listChatLog.push_back(pNewChatLog);	
}

VOID CChatReceiver::Chatlog_Notify(RwUInt8 byType, const WCHAR* pwcText)
{
	ITER_NOTIFY it_notify = m_listNotify.begin();
	for( ; it_notify != m_listNotify.end() ; ++it_notify )
	{
		CChatDisplayGui* pChatDisplayGui = *it_notify;
		pChatDisplayGui->AddText(byType, pwcText);
	}	
}

VOID CChatReceiver::HandleEvents( RWS::CMsg &msg )
{
	NTL_FUNCTION("CChatReceiver::HandleEvents");

	const RwInt32 constnBufLength = CHAT_MSG_BUFFER_LEN * 2 + 1;
	static WCHAR wchBuffer[constnBufLength];

	if( msg.Id == g_EventSayMessage )
	{
		SDboEventSayMessage* pSayMessage = reinterpret_cast<SDboEventSayMessage*>( msg.pData ); 		

		memset( wchBuffer, 0, sizeof( wchBuffer ) );

		if( pSayMessage->byMsgType == CHAT_TYPE_SYSTEM )
		{
			if( wcscmp( pSayMessage->wchUserName, SENDERNAME_NOTICE ) == 0 )
				pSayMessage->byMsgType = CHAT_TYPE_NOTIFY;

			swprintf_s( wchBuffer, constnBufLength, L"%s", pSayMessage->wchMessage );  
		}
		else 
		{
			swprintf_s( wchBuffer, constnBufLength, L"%s : %s", pSayMessage->wchUserName, pSayMessage->wchMessage );

			// 말풍선 띄우기
			if( pSayMessage->uiSerial != INVALID_SERIAL_ID )
			{
				if( Logic_GetAvatarHandle() == pSayMessage->uiSerial )
				{
					GetBalloonManager()->AddAvatarBalloon(pSayMessage->wchMessage);
				}
				else
				{
					CNtlSob* pObject = GetNtlSobManager()->GetSobObject( pSayMessage->uiSerial );
					if( pObject )
					{
						GetBalloonManager()->AddPlayerBalloon(pObject, pSayMessage->wchMessage);
					}
				}
			}
		}

		Chatlog_Save(pSayMessage->byMsgType, wchBuffer);
		Chatlog_Notify(pSayMessage->byMsgType, wchBuffer);

		if( pSayMessage->uiSerial != Logic_GetAvatarHandle() &&
			wcscmp(pSayMessage->wchUserName, Logic_GetAvatarName()) != 0 )
			Logic_NameNotify(wchBuffer);
	}
	else if( msg.Id == g_EventDiceResultShow )
	{
		SDboEventDiceResultShow* pEvent = reinterpret_cast<SDboEventDiceResultShow*>( msg.pData );

		switch(pEvent->byWorkID)
		{
		case DICE_WORKD_ID_ACTION:		
			{
				CNtlSobPlayer* pSobPlayer = reinterpret_cast<CNtlSobPlayer*>( GetNtlSobManager()->GetSobObject(pEvent->hHandle) );
				if( !pSobPlayer )
					NTL_RETURNVOID();

				CNtlSobPlayerAttr* pSobPlayerAttr = reinterpret_cast<CNtlSobPlayerAttr*>( pSobPlayer->GetSobAttr() );

				memset( wchBuffer, 0, sizeof( wchBuffer ) );
				swprintf_s(wchBuffer, constnBufLength, L"%s : %d", pSobPlayerAttr->GetName(), pEvent->byResultNum);

				Chatlog_Save(CHAT_TYPE_GENERAL, wchBuffer);
				Chatlog_Notify(CHAT_TYPE_GENERAL, wchBuffer);
				break;
			}
		case DICE_WORKD_ID_PARTY_INVEN_DICE:
		case DICE_WORKD_ID_PARTY_CHARM_DICE:
			{
				CNtlParty* pParty = GetNtlSLGlobal()->GetSobAvatar()->GetParty();
				sPartyMember* pMember = reinterpret_cast<sPartyMember*>( pParty->GetMemberbyKey(pEvent->hHandle) );
				if( !pMember )
				{
					DBO_FAIL("Not exist party member of handle : " << pEvent->hHandle);
					NTL_RETURNVOID();
				}

				memset( wchBuffer, 0, sizeof( wchBuffer ) );
				swprintf_s(wchBuffer, constnBufLength, L"%s : %d", pMember->wszMemberName, pEvent->byResultNum);

				Chatlog_Save(CHAT_TYPE_PARTY, wchBuffer);
				Chatlog_Notify(CHAT_TYPE_PARTY, wchBuffer);

				break;
			}
		case DICE_WORKD_ID_BUDOKAI_TENKAICHI_SELECT_WINNER:
			// peessi : 천하제일무도회 결과는 팀단위로 따로 표시함.
			break;
		default:
			{
				DBO_FAIL("Invalid dice work ID : " << pEvent->byWorkID);
				NTL_RETURNVOID();
			}
		}		
	}

	NTL_RETURNVOID();
}