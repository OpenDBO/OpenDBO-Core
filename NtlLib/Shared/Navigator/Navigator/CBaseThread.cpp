//*****************************************************************************
// File       : CBaseThread.cpp
// Desc       : 
// Author     : June C. Kim (cynis@hotmail.com)
// Update     : 2007. 1. 18
//*****************************************************************************

//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "StdAfx.h"

#include "CBaseThread.h"


//-----------------------------------------------------------------------------
// Name Space Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Initialize Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Name			: Initialize
// Desc			: 
// Privilege	: Protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CBaseThread::Initialize( void )
{
	m_bLoop			= false;

	m_hThread		= NULL;
	m_nID			= 0;

	m_eState		= E_THREAD_STATE_NONE;
	m_ePriority		= E_THREAD_PRIORITY_NORMAL;

	m_nLastError	= ERROR_SUCCESS;

	m_hFinished		= NULL;

	m_hObserver		= NULL;

	return;
}


//-----------------------------------------------------------------------------
// Name			: Destroy
// Desc			: 
// Privilege	: Protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CBaseThread::Destroy( void )
{
	if( m_eState != E_THREAD_STATE_NONE &&
		m_eState != E_THREAD_STATE_END )
	{
		Assert( m_hThread != NULL );

		Terminate();
	}

	ResetObserver();

	if( m_hFinished != NULL )
	{
		CloseHandle( m_hFinished );
	}

	if( m_hThread != NULL )
	{
		CloseHandle( m_hThread );
	}

	Initialize();

	return;
}


//-----------------------------------------------------------------------------
// Name			: Create
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CBaseThread::Create( EThreadPriority ePriority, bool bSuspended, bool bLoop )
{
	Assert( m_hThread == NULL );
	Assert( m_eState == E_THREAD_STATE_NONE );

	SetLoop( bLoop );

	DWORD	dwCreationFlags = ( ( bSuspended == true ) ? CREATE_SUSPENDED : 0 );

	m_hFinished = CreateEvent( NULL, TRUE, FALSE, NULL );

	m_hThread = ::CreateThread( NULL, 0, &CBaseThread::ThreadProc, this, dwCreationFlags, &m_nID );
	if( m_hThread == NULL )
	{
		SetLastError( ::GetLastError() );

		Assert( !"CreateThread() Error" );

		return false;
	}

	SetState( ( ( bSuspended == true ) ? E_THREAD_STATE_STOP : E_THREAD_STATE_RUN ) );

	SetPriority( ePriority );

	Report( _T( "Created" ) );

	return true;
}


//-----------------------------------------------------------------------------
// Name			: Resume
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CBaseThread::Resume( void )
{
	Assert( m_hThread != NULL );

	if( m_eState == E_THREAD_STATE_END )
	{
		return false;
	}

	Assert( m_eState == E_THREAD_STATE_STOP );

	Report( _T( "Resumed" ) );

	DWORD	dwResult = ::ResumeThread( m_hThread );
	if( dwResult == (DWORD)(-1) )
	{
		SetLastError( ::GetLastError() );

		Assert( !"ResumeThread() Error" );

		return false;
	}

	SetState( E_THREAD_STATE_RUN );

	return true;
}


//-----------------------------------------------------------------------------
// Name			: Suspend
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CBaseThread::Suspend( void )
{
	Assert( m_hThread != NULL );

	if( m_eState == E_THREAD_STATE_END )
	{
		return false;
	}

	Assert( m_eState == E_THREAD_STATE_RUN );

	DWORD	dwResult = ::SuspendThread( m_hThread );
	if( dwResult == (DWORD)(-1) )
	{
		SetLastError( ::GetLastError() );

		Assert( !"SuspendThread() Error" );

		return false;
	}

	SetState( E_THREAD_STATE_STOP );

	Report( _T( "Suspended" ) );

	return true;
}


//-----------------------------------------------------------------------------
// Name			: Terminate
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CBaseThread::Terminate( void )
{
	DWORD	dwResult = ::TerminateThread( m_hThread, 0 );
	if( dwResult == 0 )
	{
		SetLastError( ::GetLastError() );

		Assert( !"TerminateThread() Error" );

		return false;
	}

	SetState( E_THREAD_STATE_NONE );

	Report( _T( "Terminated" ) );

	return true;
}


//-----------------------------------------------------------------------------
// Name			: SetState
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CBaseThread::SetState( EThreadState eState )
{
	m_eState = eState;

	return;
}


//-----------------------------------------------------------------------------
// Name			: SetLoop
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CBaseThread::SetLoop( bool bLoop )
{
	Assert( m_eState == E_THREAD_STATE_NONE );

	m_bLoop = bLoop;

	return;
}


//-----------------------------------------------------------------------------
// Name			: SetPriority
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CBaseThread::SetPriority( EThreadPriority ePriority )
{
	m_ePriority = ePriority;

	::SetThreadPriority( m_hThread, static_cast<int>(ePriority) );

	Report( _T( "Set Priority" ) );

	return;
}


//-----------------------------------------------------------------------------
// Name			: SetObserver
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CBaseThread::SetObserver( HANDLE hObserver )
{
	ResetObserver();

	if( hObserver == NULL )
	{
		m_hObserver = CreateEvent( NULL, FALSE, FALSE, NULL );

		Assert( m_hObserver != NULL );
	}
	else
	{
		m_hObserver = hObserver;
	}

	return;
}


//-----------------------------------------------------------------------------
// Name			: ResetObserver
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CBaseThread::ResetObserver( void )
{
	if( m_hObserver != NULL )
	{
		CloseHandle( m_hObserver );
		m_hObserver = NULL;
	}

	return;
}
		 

//-----------------------------------------------------------------------------
// Name			: SetFinished
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CBaseThread::SetFinished( void )
{
	SetState( E_THREAD_STATE_END );

	if( m_hFinished != NULL )
	{
		SetEvent( m_hFinished );

		Report( _T( "Flag Finish" ) );
	}

	if( m_hObserver != NULL )
	{
		Report( _T( "Flag Observer" ) );

		SetEvent( m_hObserver );
	}

	return;
}


//-----------------------------------------------------------------------------
// Name			: SetLastError
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CBaseThread::SetLastError( UInt32 nLastError )
{
	m_nLastError = nLastError;

	Report( m_nLastError );

	return;
}


//-----------------------------------------------------------------------------
// Name			: ThreadProc
// Desc			: 
// Privilege	: static public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

DWORD WINAPI CBaseThread::ThreadProc( LPVOID lpParameter )
{
	CBaseThread *	pcThread = reinterpret_cast<CBaseThread *>(lpParameter);
	Assert( pcThread != NULL );

	pcThread->Begin();

	do
	{
		pcThread->Run();
	} while( pcThread->IsLoop() == true );

	pcThread->End();

	pcThread->SetFinished();

	return 0;
}


//-----------------------------------------------------------------------------
// Name			: Report
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CBaseThread::Report( TCHAR * pchBuffer )
{
	TCHAR		szBuffer[BASE_THREAD_REPORT_BUFFER_SIZE];

	if( pchBuffer != NULL )
	{
		_stprintf_s( szBuffer, BASE_THREAD_REPORT_BUFFER_SIZE, _T( "Thread ID :%d (Sta:%d) : %s" ),
					m_nID, m_eState, pchBuffer );
	}
	else
	{
		_stprintf_s( szBuffer, BASE_THREAD_REPORT_BUFFER_SIZE, _T( "Thread %d (State:%d)" ),
					m_nID, m_eState );
	}

	g_Log( szBuffer );

	return;
}


//-----------------------------------------------------------------------------
// Name			: Report
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CBaseThread::Report( UInt32 nErrorCode )
{
	TCHAR		szBuffer[BASE_THREAD_REPORT_BUFFER_SIZE];

	_stprintf_s( szBuffer, BASE_THREAD_REPORT_BUFFER_SIZE, _T( "Thread %d (State:%d) : Error Code %d" ),
				m_nID, m_eState, nErrorCode );

	g_Log( szBuffer );

	return;
}


//-----------------------------------------------------------------------------
// Name			: Report
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CBaseThread::Report( TCHAR * pchBuffer, UInt32 nValue )
{
	TCHAR		szBuffer[BASE_THREAD_REPORT_BUFFER_SIZE];

	if( pchBuffer != NULL )
	{
		_stprintf_s( szBuffer, BASE_THREAD_REPORT_BUFFER_SIZE, _T( "Thread %d (State:%d) : %s %d" ),
					m_nID, m_eState, pchBuffer, nValue );
	}
	else
	{
		_stprintf_s( szBuffer, BASE_THREAD_REPORT_BUFFER_SIZE, _T( "Thread %d (State:%d) : %d" ),
					m_nID, m_eState, nValue );
	}

	g_Log( szBuffer );

	return;
}


//-----------------------------------------------------------------------------
// Name			: ReportState
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CBaseThread::ReportState( CLogMessageManager * pcLogMessageManager, UInt32 nLayer )
{
	if( pcLogMessageManager == NULL )
	{
		return false;
	}

	TCHAR	szBuffer[BASE_THREAD_REPORT_BUFFER_SIZE];

	_stprintf_s( szBuffer, BASE_THREAD_REPORT_BUFFER_SIZE, _T( "ID : %d, Handle : 0x%08p, Loop : %d, State : %d, Priority : %d, Error : %d" ),
				m_nID, m_hThread, m_bLoop, m_eState, m_ePriority, m_nLastError );
	pcLogMessageManager->SetLastLogMessageWithTab( szBuffer, nLayer );

	return true;
}


//*****************************************************************************
//
// End of File : CBaseThread.cpp
//
//*****************************************************************************