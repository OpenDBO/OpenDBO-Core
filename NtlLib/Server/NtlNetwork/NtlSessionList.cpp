//***********************************************************************************
//
//	File		:	SessionList.cpp
//
//	Begin		:	2007-01-02
//
//	Copyright	:	¨Ï NTL-Inc Co., Ltd
//
//	Author		:	Hyun Woo, Koo   ( zeroera@ntl-inc.com )
//
//	Desc		:	
//
//***********************************************************************************


#include "stdafx.h"
#include "NtlSessionList.h"

#include "NtlNetwork.h"
#include "NtlSession.h"

#include "NtlLog.h"
#include "NtlError.h"



//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CNtlSessionList::CNtlSessionList()
{
	Init();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CNtlSessionList::~CNtlSessionList()
{
	Destroy();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
int CNtlSessionList::Create(CNtlNetwork * pNetwork, int nSessionSize, int nExtraSize)
{
	CNtlAutoMutex mutex( &m_mutex );
	mutex.Lock();


	if( false == m_sessionList.Create( nSessionSize, nExtraSize) )
	{
		return NTL_FAIL;
	}


	m_pNetworkRef = pNetwork;


	return NTL_SUCCESS;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlSessionList::Destroy()
{
	CNtlAutoMutex mutex( &m_mutex );
	mutex.Lock();


	CNtlSession ** ppSession = NULL;
	for( LISTIT it = m_sessionList.Begin(); it != m_sessionList.End(); it = m_sessionList.Next( it ))
	{
		ppSession = m_sessionList.GetPtr( it );
		if( *ppSession )
		{
			SAFE_DELETE( *ppSession );
		}
	}
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlSessionList::Init()
{
	m_pNetworkRef = NULL;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CNtlSession * CNtlSessionList::Find(LISTIT pos)
{
	CNtlAutoMutex mutex( &m_mutex );
	mutex.Lock();

	CNtlSession ** ppSession = m_sessionList.GetPtr( pos );
	if( NULL == ppSession )
	{
		return NULL;
	}

	return *ppSession;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CNtlSessionList::Add(CNtlSession * pSession)
{
	if( NULL == pSession )
	{
		NTL_LOG_ASSERT("NULL == pSession");
		return false;
	}

	if( INVALID_HSESSION != pSession->GetHandle() )
	{
		NTL_LOG_ASSERT("INVALID_HSESSION != pSession->GetHandle(), pSession->GetHandle() = %u", pSession->GetHandle());
		return false;
	}


	CNtlAutoMutex mutex( &m_mutex );
	mutex.Lock();

	LISTIT it = m_sessionList.Insert( pSession );
	if( it == m_sessionList.End() )
	{
		return false;
	}

	pSession->m_hSession = it;


	return true;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CNtlSessionList::Remove(CNtlSession * pSession)
{
	if( NULL == pSession )
	{
		NTL_LOG_ASSERT("NULL == pSession");
		return false;
	}

	if( INVALID_HSESSION ==  pSession->GetHandle() )
	{
		return true;
	}


	CNtlAutoMutex mutex( &m_mutex );
	mutex.Lock();

	m_sessionList.Remove( pSession->GetHandle() );

	pSession->m_hSession = INVALID_HSESSION;


	return true;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
int CNtlSessionList::GetCurCount()
{
	CNtlAutoMutex mutex( &m_mutex );
	mutex.Lock();

	return m_sessionList.GetSize();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
int CNtlSessionList::GetMaxCount()
{
	CNtlAutoMutex mutex( &m_mutex );
	mutex.Lock();

	return m_sessionList.GetMaxSize();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlSessionList::ValidCheck(DWORD dwTickTime)
{
	UNREFERENCED_PARAMETER( dwTickTime );

	CNtlAutoMutex mutex( &m_mutex );
	mutex.Lock();

	CNtlSession * pSession = NULL;
	for( LISTIT it = m_sessionList.Begin(); it != m_sessionList.End(); )
	{
		pSession = *( m_sessionList.GetPtr( it ) );

		if( pSession )
		{
			if( false == pSession->ValidCheck( dwTickTime ) )
			{
				NTL_PRINT(PRINT_SYSTEM, "The session[%X] should be disconnected due to timeout.", pSession);

				pSession->Disconnect( false );	
			}

			if( true == pSession->IsShutdownable() )
			{
				pSession->Shutdown();

				m_pNetworkRef->PostNetEventMessage( (WPARAM)NETEVENT_CLOSE, (LPARAM)pSession );

				it = m_sessionList.Remove( it );

				continue;
			}
		}

		it = m_sessionList.Next( it );
	}

}