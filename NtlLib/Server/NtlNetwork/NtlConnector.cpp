//***********************************************************************************
//
//	File		:	NtlConnector.cpp
//
//	Begin		:	2005-12-14
//
//	Copyright	:	ⓒ NTL-Inc Co., Ltd
//
//	Author		:	Hyun Woo, Koo   ( zeroera@ntl-inc.com )
//
//	Desc		:	Connector Class
//
//***********************************************************************************

#include "stdafx.h"
#include "NtlConnector.h"

#include "NtlNetwork.h"
#include "NtlSession.h"
#include "NtlSessionFactory.h"

#include "NtlDebug.h"
#include "NtlError.h"
#include "NtlThread.h"

#include "Utils.h"


//---------------------------------------------------------------------------------------
// Network Monitor Thread class ( Network 클래스 내부용 )
//---------------------------------------------------------------------------------------
class CConnectorThread : public CNtlRunObject
{
public:

	CConnectorThread(CNtlConnector * pConnector) { SetArg( pConnector ); }

	void Run()
	{
		CNtlConnector * pConnector = (CNtlConnector*) GetArg();

		while( IsRunnable() ) 
		{
			if( TRUE == InterlockedCompareExchange( (LONG*)& pConnector->m_bConnected, TRUE, TRUE ) )
			{
				Wait( pConnector->m_dwProcessTime );
			}
			else
			{
				NTL_PRINT(PRINT_SYSTEM, "%s Connector Try Connect", GetName());

				int rc = pConnector->DoConnect();
				if( NTL_SUCCESS != rc )
				{
					NTL_PRINT(PRINT_SYSTEM, "%s Connector Connect Fail :%d[%s]", GetName(),  rc, NtlGetErrorMessage(rc));
					Wait( pConnector->m_dwRetryTime );
				}
				else
				{
					NTL_PRINT(PRINT_SYSTEM, "%s Connector Connect Success", GetName() );
				}
			}

		} // end of while( IsRunnable() )

	}

};


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CNtlConnector::CNtlConnector()
{
	Init();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CNtlConnector::~CNtlConnector()
{
	Destroy();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlConnector::Init()
{
	m_pNetwork = NULL;

	m_pThread = NULL;


	m_dwRetryTime = 0;

	m_dwProcessTime = 0;

	m_dwTotalConnectCount = 0;


	m_bConnected = FALSE;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
int CNtlConnector::Create(LPCTSTR lpszConnectAddr, WORD wConnectPort, SESSIONTYPE sessionType, DWORD dwRetryTime /* = 1000 */, DWORD dwProcessTime /* = 1000 */)
{
	m_connectAddr.SetSockAddr(lpszConnectAddr, htons(wConnectPort));

	m_sessionType = sessionType;

	m_dwProcessTime = dwProcessTime;

	m_dwRetryTime = dwRetryTime;


	return NTL_SUCCESS;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlConnector::Destroy()
{
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
int CNtlConnector::CreateThread()
{
	CNtlString strName;
	strName.Format("ConnectorThread [%s:%u] Type[%u]", m_connectAddr.GetDottedAddr(), m_connectAddr.GetPort(), m_sessionType);

	m_pThread = new CConnectorThread( this );
	if ( NULL == m_pThread )
	{
		NTL_LOG_ASSERT("\"new CConnectorThread( this )\" failed.");
		return NTL_ERR_SYS_MEMORY_ALLOC_FAIL;
	}

	CNtlThread * pThread = CNtlThreadFactory::CreateThread( m_pThread, strName.c_str(), true );		
	if( NULL == pThread )
	{
		NTL_LOG_ASSERT("CNtlThreadFactory::CreateThread( m_pThread, strName, false ) failed.(NULL == pThread)");
		SAFE_DELETE( m_pThread );
		return NTL_ERR_NET_THREAD_CREATE_FAIL;
	}


	pThread->Start();


	return NTL_SUCCESS;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
int CNtlConnector::DoConnect()
{
	CNtlSession * pSession = m_pNetwork->GetSessionFactory()->CreateSession( m_sessionType );
	if( NULL == pSession )
	{
		return NTL_ERR_NET_CONNECTION_POOL_ALLOC_FAIL;
	}


	int rc = pSession->Create( m_pNetwork );
	if( NTL_SUCCESS != rc )
	{
		RELEASE_SESSION( pSession );
		return rc;
	}


	rc = pSession->PostConnect( this );
	if( NTL_SUCCESS != rc )
	{
		RELEASE_SESSION( pSession );
		return rc;
	}


#if !defined( __USE_CONNECTEX__ )
	rc = pSession->CompleteConnect( m_sessionType );
	if( NTL_SUCCESS != rc )
	{
		NTL_PRINT( PRINT_SYSTEM, "Session[%X] CompleteConnect Error : Err:%d(%s)", pSession, rc, NtlGetErrorMessage(rc) );
		pSession->Close();
		return rc;
	}
#endif


	return NTL_SUCCESS;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
int CNtlConnector::OnAssociated(CNtlNetwork * pNetwork)
{
	if( NULL == pNetwork )
	{
		NTL_LOG_ASSERT("(NULL == pNetwork)");
		return NTL_ERR_SYS_INPUT_PARAMETER_WRONG;
	}


	if( NULL != m_pNetwork )
	{
		NTL_LOG_ASSERT("(NULL != m_pNetwork) m_pNetwork = %016x", m_pNetwork);
		return NTL_ERR_SYS_OBJECT_ALREADY_CREATED;
	}


	m_pNetwork = pNetwork; 


	int rc = CreateThread();
	if( NTL_SUCCESS != rc )
	{
		NTL_LOG_ASSERT("CreateThread() failed.(NTL_SUCCESS != rc) rc = %d", rc);
		return NTL_ERR_NET_THREAD_CREATE_FAIL;
	}



	return NTL_SUCCESS;
}
