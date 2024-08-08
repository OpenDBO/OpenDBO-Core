//***********************************************************************************
//
//	File		:	NtlAcceptor.cpp
//
//	Begin		:	2005-12-13
//
//	Copyright	:	ⓒ NTL-Inc Co., Ltd
//
//	Author		:	Hyun Woo, Koo   ( zeroera@ntl-inc.com )
//
//	Desc		:	Network 접속을 받아들이는 Acceptor 클래스
//
//***********************************************************************************

#include "stdafx.h"
#include "NtlAcceptor.h"

#include "NtlSession.h"
#include "NtlSessionList.h"
#include "NtlSessionFactory.h"
#include "NtlNetwork.h"

#include "NtlLog.h"
#include "NtlError.h"
#include "NtlThread.h"


//---------------------------------------------------------------------------------------
// 
//---------------------------------------------------------------------------------------
const unsigned int ACCEPT_RESERVE_CHECK_TIME = 1;
//---------------------------------------------------------------------------------------


//---------------------------------------------------------------------------------------
// Acceptor Running class ( Network 클래스 내부용 )
//---------------------------------------------------------------------------------------
class CAcceptorThread : public CNtlRunObject
{
public:

	CAcceptorThread(CNtlAcceptor * pAcceptor) { SetArg( pAcceptor ); }

	void Run()
	{
		CNtlAcceptor * pAcceptor = (CNtlAcceptor*) GetArg();


		int rc = pAcceptor->ReserveAccept( pAcceptor->m_nCreateAcceptCount );
		if( NTL_SUCCESS != rc )
		{
			NTL_PRINT(PRINT_SYSTEM, "%s Precreate ReserveAccept Fail :%d[%s]", GetName(),  rc, NtlGetErrorMessage(rc));
		}
		else
		{
			NTL_PRINT(PRINT_SYSTEM, "%s Precreate ReserveAccept Success : Current[%d]", GetName(), pAcceptor->m_nAcceptingCount);
		}


		while( IsRunnable() )
		{	
			Wait( ACCEPT_RESERVE_CHECK_TIME );

			int nAcceptCount = pAcceptor->GetReserveAcceptCount();
			if( nAcceptCount > 0 )
			{
				rc = pAcceptor->ReserveAccept( nAcceptCount );
				if( NTL_SUCCESS != rc )
				{
					NTL_PRINT(PRINT_SYSTEM, "%s ReserveAccept Fail :%d[%s]", GetName(),  rc, NtlGetErrorMessage(rc));
				}
				else
				{
					NTL_PRINT(PRINT_SYSTEM, "%s ReserveAccept Success : ReserveCount[%d] Accepting[%d] Accepted[%u]", GetName(), nAcceptCount, pAcceptor->m_nAcceptingCount, pAcceptor->m_nAcceptedCount );
				}
			}

		} // end of while(1)

	} // end of Run()

};



//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CNtlAcceptor::CNtlAcceptor()
{
	Init();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CNtlAcceptor::~CNtlAcceptor()
{
	Destroy();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlAcceptor::Init()
{
	m_pNetwork = NULL;

	m_pThread = NULL;


	m_nMaxAcceptCount = 0;

	m_nMinAcceptCount = 0;

	m_nPostAcceptCount = 0;

	m_nCreateAcceptCount = 0;


	m_nAcceptingCount = 0;

	m_nAcceptedCount = 0;

	m_dwTotalAcceptCount = 0;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
int CNtlAcceptor::Create(const char * lpszAddr, unsigned short port, SESSIONTYPE sessionType, int nMaxAcceptCount, int nPostAcceptCount, int nMinAcceptCount, int nCreateAcceptCount)
{
	if( nCreateAcceptCount > nMaxAcceptCount )
	{
		NTL_LOG_ASSERT("nCreateAcceptCount > nMaxAcceptCount, nCreateAcceptCount = %d, nMaxAcceptCount = %d", nCreateAcceptCount, nMaxAcceptCount);
		return NTL_ERR_SYS_INPUT_PARAMETER_WRONG;
	}


	if( NULL != m_pNetwork )
	{
		NTL_LOG_ASSERT("NULL != m_pNetwork, m_pNetwork = %016x", m_pNetwork);
		return NTL_ERR_SYS_OBJECT_ALREADY_CREATED;
	}


	m_sessionType = sessionType;

	m_nMaxAcceptCount = nMaxAcceptCount;

	m_nPostAcceptCount = nPostAcceptCount;

	m_nMinAcceptCount = nMinAcceptCount;

	m_nCreateAcceptCount = nCreateAcceptCount;

	m_listenAddr.SetSockAddr( lpszAddr, htons(port) );


	int rc = Listen();
	if( NTL_SUCCESS != rc )
	{
		NTL_LOG_ASSERT("Listen() failed.(NTL_SUCCESS != rc), rc = %d", rc);
		return rc;
	}


	return NTL_SUCCESS;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlAcceptor::Destroy()
{
	m_listenSocket.Close();

	//SAFE_DELETE( m_pThread );
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
int CNtlAcceptor::Listen()
{
	int rc = m_listenSocket.Create( CNtlSocket::eSOCKET_TCP );
	if( NTL_SUCCESS != rc )
	{
		NTL_LOG_ASSERT("m_listenSocket.Create() failed.(NTL_SUCCESS != rc), rc = %d", rc);
		return rc;
	}


	rc = m_listenSocket.Bind( m_listenAddr );
	if( NTL_SUCCESS != rc )
	{
		NTL_LOG_ASSERT("m_listenSocket.Bind() failed.(NTL_SUCCESS != rc), rc = %d", rc);
		return rc;
	}


	rc = m_listenSocket.SetReuseAddr( TRUE );
	if( NTL_SUCCESS != rc )
	{
		NTL_LOG_ASSERT("m_listenSocket.SetReuseAddr() failed.(NTL_SUCCESS != rc), rc = %d", rc);
		return rc;
	}


	//  [1/5/2007 zeroera] : 실제 Accept한 이상의 Connect Call은 응답이 가지 않게 하는 소켓 옵션
	// 성능 저하  버그가 있었으므로, 테스트 후 사용할 것
	//rc = m_listenSocket.SetConditionalAccept( TRUE );
	//if( NTL_SUCCESS != rc )
	//{
	//	NTL_ASSERT( 0 );
	//	return rc;
	//}


	rc = m_listenSocket.Listen();
	if( NTL_SUCCESS != rc )
	{
		NTL_LOG_ASSERT("m_listenSocket.Listen() failed.(NTL_SUCCESS != rc), rc = %d", rc);
		return rc;
	}


	return NTL_SUCCESS;
}



//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
int CNtlAcceptor::OnAssociated(CNtlNetwork * pNetwork)
{
	if( NULL == pNetwork )
	{
		NTL_LOG_ASSERT("NULL == pNetwork");
		return NTL_ERR_SYS_INPUT_PARAMETER_WRONG;
	}

	if( NULL != m_pNetwork )
	{
		NTL_LOG_ASSERT("NULL != m_pNetwork, m_pNetwork = %016x", m_pNetwork);
		return NTL_ERR_NET_ACCEPTOR_ASSOCIATE_FAIL;
	}


	m_pNetwork = pNetwork;


	int rc = CreateThread();
	if( NTL_SUCCESS != rc )
	{
		NTL_LOG_ASSERT("CreateThread() failed.(NTL_SUCCESS != rc), rc = %d", rc);
		return rc;
	}


	return NTL_SUCCESS;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
int CNtlAcceptor::CreateThread()
{
	CNtlString strName;
	strName.Format("AcceptorThread [%s:%u] Type[%u]", m_listenAddr.GetDottedAddr(), m_listenAddr.GetPort(), m_sessionType);

	m_pThread = new CAcceptorThread( this );
	if (NULL == m_pThread)
	{
		NTL_LOG_ASSERT("\"new CAcceptorThread( this )\" failed.");
		return NTL_ERR_SYS_MEMORY_ALLOC_FAIL;
	}

	CNtlThread * pThread = CNtlThreadFactory::CreateThread( m_pThread, strName.c_str(), true );		
	if( NULL == pThread )
	{
		NTL_LOG_ASSERT("NULL == pThread");
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
int CNtlAcceptor::ReserveAccept(int nReserveCount)
{
	if( NULL == m_pNetwork )
	{
		NTL_LOG_ASSERT("NULL == m_pNetwork");
		return NTL_ERR_SYS_INPUT_PARAMETER_WRONG;
	}


	for( int i = 0; i < nReserveCount; i++ )
	{
		CNtlSession * pSession = m_pNetwork->GetSessionFactory()->CreateSession( m_sessionType );
		if( NULL == pSession )
		{
			//NTL_LOG_ASSERT("m_pNetwork->GetSessionFactory()->CreateSession() failed.(NULL == pSession), i = %d, m_sessionType = %u", i, m_sessionType);
			return NTL_ERR_NET_CONNECTION_POOL_ALLOC_FAIL;
		}

		int rc = pSession->Create( m_pNetwork ); 
		if( NTL_SUCCESS != rc )
		{
			NTL_LOG_ASSERT("pSession->Create() failed(NTL_SUCCESS != rc), rc = %d", rc);
			RELEASE_SESSION( pSession );
			return rc;
		}

		rc = pSession->PostAccept( this );
		if( NTL_SUCCESS != rc )
		{
			NTL_LOG_ASSERT("pSession->PostAccept() failed(NTL_SUCCESS != rc), rc = %d", rc);
			RELEASE_SESSION( pSession );
			return rc;
		}
	}

	return NTL_SUCCESS;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
// Accept 하려는 개수와 Accept 가능 개수를 조사해 가능한 Accept개수를 반환한다 
//-----------------------------------------------------------------------------------
int CNtlAcceptor::GetReserveAcceptCount()
{
	CNtlAutoMutex mutex( &m_mutex );
	mutex.Lock();

	if( m_nAcceptingCount < m_nMinAcceptCount )
	{
		int nAcceptDiff = m_nPostAcceptCount - m_nAcceptingCount;
		int nAvailableAccept = m_nMaxAcceptCount - ( m_nAcceptingCount + m_nAcceptedCount );

		return min( nAvailableAccept, nAcceptDiff );
	}

	return 0;
}
