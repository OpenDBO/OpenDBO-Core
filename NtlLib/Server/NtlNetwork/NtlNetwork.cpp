//***********************************************************************************
//
//	File		:	NtlNetwork.cpp
//
//	Begin		:	2005-12-15
//
//	Copyright	:	ⓒ NTL-Inc Co., Ltd
//
//	Author		:	Hyun Woo, Koo   ( zeroera@ntl-inc.com )
//
//	Desc		:	Network Manager Class
//
//***********************************************************************************

#include "stdafx.h"
#include "NtlNetwork.h"

#include "NtlSession.h"
#include "NtlAcceptor.h"
#include "NtlConnector.h"
#include "NtlSessionFactory.h"
#include "NtlNetworkProcessor.h"


#include "NtlThread.h"
#include "NtlDebug.h"
#include "NtlError.h"
#include "NtlQueue.h"

#include "Utils.h"


//---------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------
const ULONG_PTR THREAD_CLOSE = (ULONG_PTR)(-1);	// thread terminate value
//---------------------------------------------------------------------------------------



//---------------------------------------------------------------------------------------
// Network Monitor Thread class ( Network 클래스 내부용 )
//---------------------------------------------------------------------------------------
class CNtlNetworkMonitor : public CNtlRunObject
{
public:

	CNtlNetworkMonitor(CNtlNetwork *  pNetwork) { SetArg( pNetwork); }

	void Run()
	{
		CNtlNetwork * pNetwork = (CNtlNetwork*) GetArg();
		if( NULL == pNetwork )
		{
			NTL_LOG_ASSERT( "fail : NULL == pNetwork" );
			return;
		}

		DWORD dwTickCur = 0;
		DWORD dwTickOld = 0;
		DWORD dwTickDiff = 0;

		dwTickCur = dwTickOld = ::GetTickCount();

		while( IsRunnable() )
		{	
			Wait( 1000 );

			dwTickCur	= ::GetTickCount();
			dwTickDiff	= dwTickCur - dwTickOld;
			dwTickOld	= dwTickCur;

			if( NULL == pNetwork->GetSessionList() )
			{
				continue;
			}

			pNetwork->GetSessionList()->ValidCheck( dwTickDiff );
		}
	}

};
//-----------------------------------------------------------------------------------



//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CNtlNetwork::CNtlNetwork()
{
	Init();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CNtlNetwork::~CNtlNetwork()
{
	Destroy();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
int	CNtlNetwork::Create(CNtlSessionFactory * pFactory, int nSessionSize, int nCreateThreads /* = 0 */, int nConcurrentThreads /* = 0  */)
{
	if( NULL == pFactory )
	{
		NTL_LOG_ASSERT("(NULL == pFactory)");
		return NTL_ERR_SYS_INPUT_PARAMETER_WRONG;
	}


	int rc = StartUp();
	if( NTL_SUCCESS != rc )
	{
		NTL_LOG_ASSERT("StartUp() failed.(NTL_SUCCESS != rc) rc = %d", rc);
		return rc;
	}


	m_pSessionList = new CNtlSessionList;
	if( NULL == m_pSessionList )
	{
		NTL_LOG_ASSERT("\"new CNtlSessionList\" failed.");
		return NTL_ERR_SYS_MEMORY_ALLOC_FAIL;
	}


	m_pAcceptorList = new CNtlAcceptorList;
	if( NULL == m_pAcceptorList )
	{
		NTL_LOG_ASSERT("\"new CNtlAcceptorList\" failed.");
		return NTL_ERR_SYS_MEMORY_ALLOC_FAIL;
	}


	m_pConnectorList = new CNtlConnectorList;
	if( NULL == m_pConnectorList )
	{
		NTL_LOG_ASSERT("\"new CNtlConnectorList\" failed.");
		return NTL_ERR_SYS_MEMORY_ALLOC_FAIL;
	}


	rc = m_pSessionList->Create( this, nSessionSize, nSessionSize / 10 );
	if( NTL_SUCCESS != rc )
	{
		NTL_LOG_ASSERT("m_pSessionList->Create( this, nSessionSize, nSessionSize / 10 ) failed.(NTL_SUCCESS != rc) rc = %d", rc);
		return rc;
	}


	rc = m_iocp.Create(this, nCreateThreads, nConcurrentThreads );
	if( NTL_SUCCESS != rc )
	{
		NTL_LOG_ASSERT("m_iocp.Create(this, nCreateThreads, nConcurrentThreads ) failed.(NTL_SUCCESS != rc) rc = %d", rc);
		return rc;
	}


	rc = CreateMonitorThread();
	if( NTL_SUCCESS != rc )
	{
		NTL_LOG_ASSERT("CreateMonitorThread() failed.(NTL_SUCCESS != rc) rc = %d", rc);
		return rc;
	}


	rc = CreateDispatcherThread();
	if( NTL_SUCCESS != rc )
	{
		NTL_LOG_ASSERT("CreateDispatcherThread() failed.(NTL_SUCCESS != rc) rc = %d", rc);
		return rc;
	}


	m_pSessionFactoryRef = pFactory;


	return NTL_SUCCESS;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlNetwork::Destroy()
{
	SAFE_DELETE( m_pSessionList );

	if (NULL != m_pAcceptorList)
	{
		for( CNtlAcceptorList::iterator it = m_pAcceptorList->begin(); it != m_pAcceptorList->end(); it++ )
		{
			SAFE_DELETE( it->second );
		}
		SAFE_DELETE( m_pAcceptorList );
	}

	if (NULL != m_pConnectorList)
	{
		for( CNtlConnectorList::iterator it = m_pConnectorList->begin(); it != m_pConnectorList->end(); it++ )
		{
			SAFE_DELETE( it->second );
		}
		SAFE_DELETE( m_pConnectorList );
	}

	//SAFE_DELETE( m_pNetworkMonitor );

	//SAFE_DELETE( m_pNetworkProcessor );

	//Shutdown();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlNetwork::Init()
{
	m_pSessionFactoryRef = NULL;

	m_pNetworkMonitor = NULL;

	m_pNetworkProcessor = NULL;

	m_pSessionList = NULL;

	m_pAcceptorList = NULL;

	m_pConnectorList = NULL;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
int CNtlNetwork::StartUp()
{
	int rc = CNtlSocket::StartUp();
	if( NTL_SUCCESS != rc )
	{
		NTL_LOG_ASSERT("CNtlSocket::StartUp() failed.(NTL_SUCCESS != rc) rc = %d", rc);
		return rc;
	}

	return NTL_SUCCESS;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
int CNtlNetwork::Shutdown()
{
	int rc = CNtlSocket::CleanUp();
	if( NTL_SUCCESS != rc )
	{
		NTL_LOG_ASSERT("CNtlSocket::CleanUp() failed.(NTL_SUCCESS != rc) rc = %d", rc);
		return rc;
	}

	return NTL_SUCCESS;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
int CNtlNetwork::CreateMonitorThread()
{
	CNtlString strName;
	strName.Format("Network Monitor");

	m_pNetworkMonitor = new CNtlNetworkMonitor(this);
	if ( NULL == m_pNetworkMonitor )
	{
		NTL_LOG_ASSERT("\"new CNtlNetworkMonitor(this)\" failed.");
		return NTL_ERR_SYS_MEMORY_ALLOC_FAIL;
	}

	CNtlThread * pThread = CNtlThreadFactory::CreateThread( m_pNetworkMonitor, strName.c_str(), true );		
	if( NULL == pThread )
	{
		NTL_LOG_ASSERT("CNtlThreadFactory::CreateThread( m_pNetworkMonitor, strName, false) failed.(NULL == pThread)");
		SAFE_DELETE( m_pNetworkMonitor );
		return NTL_ERR_NET_THREAD_CREATE_FAIL;
	}

	pThread->Start();

	return NTL_SUCCESS;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
int CNtlNetwork::CreateDispatcherThread()
{
	CNtlString strName;
	strName.Format("Network Processor");

	m_pNetworkProcessor = new CNtlNetworkProcessor( this );
	if ( NULL == m_pNetworkProcessor )
	{
		NTL_LOG_ASSERT("\"new CNtlNetworkProcessor( this )\" failed.");
		return NTL_ERR_SYS_MEMORY_ALLOC_FAIL;
	}

	int rc = m_pNetworkProcessor->Create();
	if(  NTL_SUCCESS != rc )
	{
		NTL_LOG_ASSERT("m_pNetworkProcessor->Create() failed.(NTL_SUCCESS != rc) rc = %d", rc);
		return NTL_ERR_NET_THREAD_CREATE_FAIL;
	}

	CNtlThread * pThread = CNtlThreadFactory::CreateThread( m_pNetworkProcessor, strName.c_str(), true );		
	if( NULL == pThread )
	{
		NTL_LOG_ASSERT("CNtlThreadFactory::CreateThread( m_pNetworkProcessor, strName, false ) failed.(NULL == pThread)");
		SAFE_DELETE( m_pNetworkProcessor );
		return NTL_ERR_NET_THREAD_CREATE_FAIL;
	}

	pThread->Start();

	return NTL_SUCCESS;
}


//-----------------------------------------------------------------------------------
//		Purpose	: 
//		Return	:
//-----------------------------------------------------------------------------------
int CNtlNetwork::Associate(CNtlConnection * pConnection, bool bAssociate)
{
	CNtlSession * pSession = check_cast<CNtlSession*>( pConnection );
	if( NULL == pSession )
	{
		NTL_LOG_ASSERT("(NULL == pSession) pConnection = %016x", pConnection);
		return NTL_ERR_SYS_INPUT_PARAMETER_WRONG;
	}


	if( bAssociate )
	{
		if( false == m_pSessionList->Add( pSession ) )
		{
			NTL_LOG_ASSERT("m_pSessionList->Add( pSession ) failed.");
			return  NTL_ERR_NET_CONNECTION_ADD_FAIL;
		}

		int rc = m_iocp.Associate( pSession->GetSocket().GetRawSocket(), pSession );
		if( NTL_SUCCESS != rc )
		{	
			NTL_LOG_ASSERT("m_iocp.Associate( pSession->GetSocket().GetRawSocket(), pSession ) failed.(NTL_SUCCESS != rc), rc = %d", rc);
			return rc;
		}
	}
	else
	{
		if( false == m_pSessionList->Remove( pSession ) )
		{
			NTL_LOG_ASSERT("m_pSessionList->Remove( pSession ) failed.");
			return  NTL_ERR_NET_CONNECTION_ADD_FAIL;
		}
	}


	return NTL_SUCCESS;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
int CNtlNetwork::Associate(CNtlAcceptor * pAcceptor, bool bAssociate)
{
	if( NULL == pAcceptor )
	{
		NTL_LOG_ASSERT("(NULL == pAcceptor)");
		return NTL_ERR_SYS_INPUT_PARAMETER_WRONG;
	}


	if( bAssociate )
	{
		CNtlAcceptorList::iterator it = m_pAcceptorList->insert( CNtlAcceptorList::value_type( pAcceptor->GetSessionType(), pAcceptor ) );
		if( m_pAcceptorList->end() == it )
		{
			NTL_LOG_ASSERT("m_pAcceptorList->insert( CNtlAcceptorList::value_type( pAcceptor->GetSessionType(), pAcceptor ) ) failed.(m_pAcceptorList->end() == it), pAcceptor->GetSessionType() = %u", pAcceptor->GetSessionType());
			return NTL_ERR_NET_ACCEPTOR_ALREADY_CREATED;
		}


		int rc = pAcceptor->OnAssociated( this );
		if( NTL_SUCCESS != rc )
		{
			NTL_LOG_ASSERT("pAcceptor->OnAssociated( this ) failed.(NTL_SUCCESS != rc), rc = %d", rc);
			return rc;
		}


		rc = m_iocp.Associate( pAcceptor->GetListenSocket().GetRawSocket(), pAcceptor );
		if( NTL_SUCCESS != rc )
		{
			NTL_LOG_ASSERT("m_iocp.Associate( pAcceptor->GetListenSocket().GetRawSocket(), pAcceptor ) failed.(NTL_SUCCESS != rc), rc = %d", rc);
			return rc;
		}

	}


	return NTL_SUCCESS;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
int CNtlNetwork::Associate(CNtlConnector * pConnector, bool bAssociate)
{
	if( NULL == pConnector )
	{
		NTL_LOG_ASSERT("(NULL == pConnector)");
		return NTL_ERR_SYS_INPUT_PARAMETER_WRONG;
	}


	if( bAssociate )
	{
		CNtlConnectorList::iterator it = m_pConnectorList->insert( CNtlConnectorList::value_type(pConnector->GetSessionType(), pConnector) );
		if( m_pConnectorList->end() == it )
		{
			NTL_LOG_ASSERT("m_pConnectorList->insert( CNtlConnectorList::value_type(pConnector->GetSessionType(), pConnector) ) failed.(m_pConnectorList->end() == it), pConnector->GetSessionType() = %u", pConnector->GetSessionType());
			return NTL_ERR_NET_CONNECTOR_ALREADY_CREATED;
		}


		int rc = pConnector->OnAssociated( this );
		if( NTL_SUCCESS != rc )
		{
			NTL_LOG_ASSERT("pConnector->OnAssociated() failed.(NTL_SUCCESS != rc) rc = %d", rc);
			return rc;
		}
	}


	return NTL_SUCCESS;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CNtlAcceptor * CNtlNetwork::GetAcceptor(SESSIONTYPE sessionType, const char * lpszAddr, WORD wPort)
{
	CNtlAcceptorList::iterator beg = m_pAcceptorList->lower_bound( sessionType );
	CNtlAcceptorList::iterator end = m_pAcceptorList->upper_bound( sessionType );

	for( CNtlAcceptorList::iterator it = beg; it != end; it++ )
	{
		CNtlAcceptor * pAcceptor = it->second;
		if( NULL == pAcceptor )
		{
			NTL_LOG_ASSERT("The acceptor object is NULL in m_pAcceptorList.(NULL == pAcceptor) sessionType = %u", sessionType);
			continue;
		}

		if( 0 != strncmp( pAcceptor->GetListenIP(), lpszAddr, strlen( lpszAddr ) ) )
			continue;

		if( wPort == pAcceptor->GetListenPort() )
			return pAcceptor;
	}

	return NULL;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CNtlConnector * CNtlNetwork::GetConnector(SESSIONTYPE sessionType, const char * lpszAddr, WORD wPort)
{
	CNtlConnectorList::iterator beg = m_pConnectorList->lower_bound( sessionType );
	CNtlConnectorList::iterator end = m_pConnectorList->upper_bound( sessionType );

	for( CNtlConnectorList::iterator it = beg; it != end; it++ )
	{
		CNtlConnector * pConnector = it->second;
		if( NULL == pConnector )
		{
			NTL_LOG_ASSERT("The connector object is NULL in m_pConnectorList.(NULL == pConnector) sessionType = %u", sessionType);
			continue;
		}

		if( 0 != strncmp( pConnector->GetConnectIP(), lpszAddr, strlen( lpszAddr ) ) )
			continue;

		if( wPort == pConnector->GetConnectPort() )
			return pConnector;
	}

	return NULL;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
//  [1/3/2007 zeroera] : 임시 : 이전 버전 호환용
//-----------------------------------------------------------------------------------
int CNtlNetwork::Send(HSESSION hSession, CNtlPacket * pPacket)
{
	if (NULL == pPacket)
	{
		NTL_LOG_ASSERT("(NULL == pPacket)");
		return NTL_ERR_NET_PACKET_INVALID;
	}

	if (NULL == m_pSessionList)
	{
		return NTL_ERR_NET_NETWORK_NOT_CREATED;
	}

	CNtlSession * pSession = m_pSessionList->Find( hSession );
	if( NULL == pSession )
	{
		return NTL_ERR_NET_CONNECTION_NOT_EXIST;
	}

	int rc = pSession->PushPacket( pPacket );
	if( NTL_SUCCESS != rc )
	{
		pSession->Disconnect( false );

		NTL_LOGDL( LOG_NETWORK, "Session[%X] PushPacket Error : Err:%d(%s)", pSession, rc, NtlGetErrorMessage(rc) );
		return rc;
	}

	return NTL_SUCCESS;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
int CNtlNetwork::Send(CNtlSession * pSession, CNtlPacket * pPacket)
{
	if (NULL == pSession)
	{
		return NTL_ERR_NET_CONNECTION_NOT_EXIST;
	}

	if (NULL == pPacket)
	{
		NTL_LOG_ASSERT("(NULL == pPacket)");
		return NTL_ERR_NET_PACKET_INVALID;
	}

	int rc = pSession->PushPacket( pPacket );
	if( NTL_SUCCESS != rc )
	{
		pSession->Disconnect( false );

		NTL_LOGDL( LOG_NETWORK, "Session[%X] PushPacket Error : Err:%d(%s)", pSession, rc, NtlGetErrorMessage(rc) );
		return rc;
	}

	return NTL_SUCCESS;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
int CNtlNetwork::PostNetEventMessage(WPARAM wParam, LPARAM lParam)
{
	return m_pNetworkProcessor->PostNetEvent( wParam, lParam );
}