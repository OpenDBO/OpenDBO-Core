/*****************************************************************************
*
* File			: DLMessageSys.cpp
* Author		: 
* Copyright		: (주) NTL
* Date			:
* Abstract		: 
*****************************************************************************
* Desc          : 
*
*****************************************************************************/

#include "stdafx.h"
#include "DLMessageSys.h"
#include "DLMessageImp.h"
#include "DLUtility.h"
#include "NtlClientService.h"


//////////////////////////////////////////////////////////////////////////
//
// CDLMessageSys
//
//////////////////////////////////////////////////////////////////////////


CDLMessageSys::CDLMessageSys( void )
{
	m_pMsgFinder = NULL;
}

CDLMessageSys::~CDLMessageSys( void )
{
	Delete();
}

bool CDLMessageSys::Create( void )
{
	Delete();

	// The message table follows the service publisher, not the code page
	switch ( GetServiceInterface()->GetPublisherID() )
	{
	case eDBO_SERVICE_PUBLISHER_NTL:
	case eDBO_SERVICE_PUBLISHER_CJI_KOR:
		{
			m_pMsgFinder = new CDLMsgFinder_KOR;

			if ( NULL == m_pMsgFinder )
			{
				return false;
			}
		}
		break;

	case eDBO_SERVICE_PUBLISHER_JPN:
		{
			m_pMsgFinder = new CDLMsgFinder_JPN;

			if ( NULL == m_pMsgFinder )
			{
				return false;
			}
		}
		break;

	case eDBO_SERVICE_PUBLISHER_CT_TW:
		{
			m_pMsgFinder = new CDLMsgFinder_CT_TW;

			if ( NULL == m_pMsgFinder )
			{
				return false;
			}
		}
		break;

	case eDBO_SERVICE_PUBLISHER_CT_HK:
		{
			m_pMsgFinder = new CDLMsgFinder_CT_HK;

			if ( NULL == m_pMsgFinder )
			{
				return false;
			}
		}
		break;

	default:
		{
			m_pMsgFinder = new CDLMsgFinder_ENG;

			if ( NULL == m_pMsgFinder )
			{
				return false;
			}
		}
		break;
	}

	return m_pMsgFinder->Create();
}

void CDLMessageSys::Delete( void )
{
	if ( m_pMsgFinder )
	{
		delete m_pMsgFinder;
		m_pMsgFinder = NULL;
	}
}

void CDLMessageSys::DLMessageProc( eDL_EVENT eMsg, sDL_EVENT_DATA_BASE* pData )
{
	if ( eDL_EVENT_OUTPUT_MESSAGE_TO_USER == eMsg )
	{
		sDL_ED_MSGOUT_TO_USER* pEvtData = (sDL_ED_MSGOUT_TO_USER*)pData;

		if ( m_pMsgFinder )
		{
			CString strMsg = m_pMsgFinder->FindString( (eDL_MSG)pEvtData->uiMsgID );

			AfxMessageBox( strMsg );
		}
	}
	else if ( eDL_EVENT_GET_MESSAGE_TO_UI == eMsg )
	{
		sDL_ED_GETMSG_TO_UI* pEvtData = (sDL_ED_GETMSG_TO_UI*)pData;

		if ( m_pMsgFinder )
		{
			pEvtData->strMsg = m_pMsgFinder->FindString( (eDL_MSG)pEvtData->uiMsgID );
		}
	}
	else if ( eDL_EVENT_OUTPUT_MESSAGE_FOR_DEBUG == eMsg )
	{
		sDL_ED_MSGOUT_FOR_DEBUG* pEvtData = (sDL_ED_MSGOUT_FOR_DEBUG*)pData;

		SYSTEMTIME sTime;
		GetLocalTime( &sTime );

		CString strTime;
		strTime.Format( _T( "[Year:%d, Month:%d, Day:%d, Hour:%d, Minute:%d, Second:%d]\t" ), sTime.wYear, sTime.wMonth, sTime.wDay, sTime.wHour, sTime.wMinute, sTime.wSecond );

		CString strMsg = strTime;
		strMsg += pEvtData->strMsg;
		strMsg += _T( "\n\n" );

		CStdioFile cLogFile;

		if ( cLogFile.Open( DL_LOG_FILE_NAME, CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite ) )
		{
			cLogFile.SeekToEnd();
			cLogFile.WriteString( strMsg );
		}
	}
}
