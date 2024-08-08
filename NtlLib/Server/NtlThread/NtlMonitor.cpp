//***********************************************************************************
//
//	File		:	NtlMonitor.cpp
//
//	Begin		:	2005-11-30
//
//	Copyright	:	ⓒ NTL-Inc Co., Ltd
//
//	Author		:	Hyun Woo, Koo   ( zeroera@ntl-inc.com )
//
//	Desc		:	
//
//***********************************************************************************


#include "StdAfx.h"
#include "NtlMonitor.h"
#include "NtlThread.h"
#include "NtlThreadException.h"
#include "NtlDebug.h"


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CNtlMonitor::CNtlMonitor(void)
:
m_pOwner( 0 ),
m_nLockCount( 0 )
{
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CNtlMonitor::~CNtlMonitor(void)
{
	CNtlAutoMutex mutex( &m_mutex );
	mutex.Lock();

	// ready, wait에 쓰레드가 있거나, 현재 소유하고 있는 쓰레드가 있다면 busy 상태
	bool bBusyStatus = ( !m_readyList.IsEmpty() ) || ( !m_waitList.IsEmpty() ) || ( 0 != m_pOwner );

	mutex.Unlock();

	if( bBusyStatus )
	{
		NTL_ASSERT( 0 );
		THROW_THREAD_EXCEPTION(eTHREAD_ERR_MONITOR_BUSY, ERROR_BUSY);
	}
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlMonitor::Reset()
{
	m_pOwner = 0;
	m_nLockCount = 0;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlMonitor::Lock()
{
	CNtlThread *  pRequester = CNtlThread::GetCurrentThread();

	CNtlAutoMutex mutex( &m_mutex );
	mutex.Lock();


	if( !m_pOwner )
	{
		m_pOwner = pRequester;

		++m_nLockCount;
	}
	else if( m_pOwner == pRequester )
	{
		++m_nLockCount;
	}
	else
	{
		// 쓰레드 대기상태로...
		pRequester->SetSignaled( false );

		m_readyList.Append( pRequester );

		mutex.Unlock();

		pRequester->Wait();

		mutex.Lock();
	}

	mutex.Unlock();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CNtlMonitor::TryLock()
{
	CNtlThread *  pRequester = CNtlThread::GetCurrentThread();

	CNtlAutoMutex mutex( &m_mutex );
	mutex.Lock();

	bool bResult = true;

	if( !m_pOwner )
	{
		m_pOwner = pRequester;

		++m_nLockCount;
	}
	else if( m_pOwner == pRequester )
	{
		++m_nLockCount;
	}
	else
	{
		bResult = false;
	}

	mutex.Unlock();

	return bResult;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlMonitor::Unlock()
{
	CNtlThread * pRequester = CNtlThread::GetCurrentThread();

	CNtlAutoMutex mutex( &m_mutex );
	mutex.Lock();

	if( pRequester != m_pOwner )
	{
		if( !m_pOwner )
		{
			// 소유주 없음
		}
		else
		{
			// 다른 소유주가 있음
		}

		NTL_ASSERT( 0 );
		THROW_THREAD_EXCEPTION(eTHREAD_ERR_MONITOR_STATE, ERROR_NOT_OWNER);
	}


	--m_nLockCount;


	// 소유주가 없을 경우 다음 쓰레드로 넘김
	if( 0 == m_nLockCount )
	{
		Reset();
		Grant();
	}
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlMonitor::Grant()
{
	m_pOwner = (CNtlThread *) m_readyList.GetFirst();

	if( m_pOwner )
	{
		++m_nLockCount;

		m_readyList.Remove( m_pOwner );

		CNtlThread::Notify( m_pOwner );
	}
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlMonitor::Wait()
{
	CNtlThread * pRequester = CNtlThread::GetCurrentThread();

	CNtlAutoMutex mutex( &m_mutex );
	mutex.Lock();

	if( pRequester != m_pOwner )
	{
		if( !m_pOwner )
		{
			// 소유주 없음
		}
		else
		{
			// 다른 소유주가 있음
		}

		NTL_ASSERT( 0 );
		THROW_THREAD_EXCEPTION(eTHREAD_ERR_MONITOR_STATE, ERROR_NOT_OWNER);
	}

	pRequester->SetSignaled( false );

	m_waitList.Append( pRequester );

	int nSavedCount = m_nLockCount;

	Reset();
	Grant();


	mutex.Unlock();

	pRequester->Wait();

	mutex.Lock();


	m_nLockCount = nSavedCount;

	mutex.Unlock();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CNtlMonitor::Wait(unsigned int millisecs)
{
	UNREFERENCED_PARAMETER( millisecs );

	NTL_ASSERT( 0 );
	return true;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlMonitor::Notify()
{
	CNtlAutoMutex mutex( &m_mutex );
	mutex.Lock();

	CNtlThread * pWaiter = (CNtlThread *) m_waitList.GetFirst();
	if( pWaiter )
	{
		pWaiter->SetSignaled( true );

		m_waitList.Remove( pWaiter );
		m_readyList.Append( pWaiter );

		if( !m_pOwner )
		{
			Grant();
		}
	}

	mutex.Unlock();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlMonitor::NotifyAll()
{
	CNtlAutoMutex mutex( &m_mutex );
	mutex.Lock();

	CNtlThread * pWaiter = (CNtlThread *) m_waitList.GetFirst();
	while( pWaiter )
	{
		pWaiter->SetSignaled( true );

		m_waitList.Remove( pWaiter );
		m_readyList.Append( pWaiter );

		pWaiter = (CNtlThread *) m_waitList.GetFirst();
	}

	if( !m_pOwner && !m_readyList.IsEmpty() )
	{
		Grant();
	}

	mutex.Unlock();
}

