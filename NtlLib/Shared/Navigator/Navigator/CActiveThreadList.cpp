//*****************************************************************************
// File       : CActiveThreadList.cpp
// Desc       : 
// Author     : June C. Kim (cynis@hotmail.com)
// Update     : 2007. 1. 18
//*****************************************************************************

//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "StdAfx.h"

#include "CActiveThreadList.h"


//-----------------------------------------------------------------------------
// Name Space Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Initialize Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Name			: Destroy
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CActiveThreadList::Destroy( void )
{
	if( m_ppcThreads != NULL )
	{
		delete[] m_ppcThreads;
		m_ppcThreads = NULL;
	}

	if( m_phThreads != NULL )
	{
		delete[] m_phThreads;
		m_phThreads = NULL;
	}

	return;
}


//-----------------------------------------------------------------------------
// Name			: Reserve
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CActiveThreadList::Reserve( UInt32 nSize )
{
	CBaseThread **	ppcNewThreads = new CBaseThread *[nSize];
	Assert( ppcNewThreads != NULL );
	ZeroMemory( ppcNewThreads, sizeof( CBaseThread * ) * nSize );

	HANDLE *	phNewThreads = new HANDLE[nSize];
	Assert( phNewThreads != NULL );
	ZeroMemory( phNewThreads, sizeof( HANDLE ) * nSize );

	UInt32	nEffectiveCount;

	if( nSize <= m_nSize )
	{
		nEffectiveCount = Min( m_nCount, nSize );
	}
	else
	{
		nEffectiveCount = m_nCount;
	}

	memcpy( ppcNewThreads, m_ppcThreads, sizeof( CBaseThread * ) * nEffectiveCount );
	memcpy( phNewThreads, m_phThreads, sizeof( HANDLE ) * nEffectiveCount );

	Destroy();

	m_nSize = nSize;
	m_ppcThreads = ppcNewThreads;
	m_phThreads = phNewThreads;

	return;
}


//-----------------------------------------------------------------------------
// Name			: AddThread
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

UInt32 CActiveThreadList::AddThread( CBaseThread * pcThread )
{
	Assert( pcThread != NULL );
	Assert( m_ppcThreads != NULL );
	Assert( m_phThreads != NULL );

	if( m_nSize == m_nCount )
	{
		Reserve( m_nSize << 1 );
	}

	m_ppcThreads[m_nCount] = pcThread;
	m_phThreads[m_nCount] = pcThread->GetHandle();

	++m_nCount;

	return m_nCount - 1;
}


//-----------------------------------------------------------------------------
// Name			: RemoveThread
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CBaseThread * CActiveThreadList::RemoveThread( UInt32 nIndex )
{
	Assert( m_ppcThreads != NULL );
	Assert( m_phThreads != NULL );
	Assert( m_nCount > nIndex );

	CBaseThread *	pcThreadToRemove = m_ppcThreads[nIndex];

	--m_nCount;

	m_ppcThreads[nIndex] = m_ppcThreads[m_nCount];
	m_phThreads[nIndex] = m_phThreads[m_nCount];

	m_ppcThreads[m_nCount] = NULL;
	m_phThreads[m_nCount] = NULL;

	return pcThreadToRemove;
}


//-----------------------------------------------------------------------------
// Name			: ResumeAll
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CActiveThreadList::ResumeAll( void )
{
	for( UInt32 nIndex = 0; nIndex < m_nCount; ++nIndex )
	{
		Assert( m_ppcThreads[m_nCount] != NULL );

		m_ppcThreads[m_nCount]->Resume();
	}

	return true;
}


//-----------------------------------------------------------------------------
// Name			: SuspendAll
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CActiveThreadList::SuspendAll( void )
{
	for( UInt32 nIndex = 0; nIndex < m_nCount; ++nIndex )
	{
		Assert( m_ppcThreads[m_nCount] != NULL );

		m_ppcThreads[m_nCount]->Suspend();
	}

	return true;
}


//-----------------------------------------------------------------------------
// Name			: TerminateAll
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CActiveThreadList::TerminateAll( void )
{
	for( UInt32 nIndex = 0; nIndex < m_nCount; ++nIndex )
	{
		Assert( m_ppcThreads[m_nCount] != NULL );

		m_ppcThreads[m_nCount]->Terminate();
	}

	return true;
}


//-----------------------------------------------------------------------------
// Name			: ReportState
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CActiveThreadList::ReportState( CLogMessageManager * pcLogMessageManager, UInt32 nLayer )
{
	if( pcLogMessageManager == NULL )
	{
		return false;
	}

	bool	bResult = true;
	TCHAR	szBuffer[BASE_THREAD_REPORT_BUFFER_SIZE];

	pcLogMessageManager->SetLastLogMessageWithTab( _T( "--------------------------------------------------------------------------------" ), nLayer );
	pcLogMessageManager->SetLastLogMessageWithTab( _T( "CActiveThreadList::ReportState() : Begin" ), nLayer );
	pcLogMessageManager->SetLastLogMessageWithTab( _T( "--------------------------------------------------------------------------------" ), nLayer );
	pcLogMessageManager->SetLastLogMessageWithTab( _T( "" ), nLayer );

	if( m_nSize > 0 )
	{
		_stprintf_s( szBuffer, BASE_THREAD_REPORT_BUFFER_SIZE, _T( "Count : %d, Size : %d, Capacity : %d%%" ),
					m_nCount, m_nSize, m_nCount * 100 / m_nSize );
		pcLogMessageManager->SetLastLogMessageWithTab( szBuffer, nLayer );
		pcLogMessageManager->SetLastLogMessageWithTab( _T( "" ), nLayer );

		for( UInt32 nIndex = 0; nIndex < m_nCount; ++nIndex )
		{
			Assert( m_ppcThreads[nIndex] != NULL );

			_stprintf_s( szBuffer, BASE_THREAD_REPORT_BUFFER_SIZE, _T( "Slot : %d, Ptr : 0x%08p, Handle : 0x%08p" ),
						nIndex, m_ppcThreads[nIndex], m_phThreads[nIndex] );
			pcLogMessageManager->SetLastLogMessageWithTab( szBuffer, nLayer + 1 );

			m_ppcThreads[nIndex]->ReportState( pcLogMessageManager, nLayer + 2 );
		}

	}
	else
	{
		pcLogMessageManager->SetLastLogMessageWithTab( _T( "(Not Allocated)" ), nLayer );
	}

	pcLogMessageManager->SetLastLogMessageWithTab( _T( "" ), nLayer );
	pcLogMessageManager->SetLastLogMessageWithTab( _T( "--------------------------------------------------------------------------------" ), nLayer );
	pcLogMessageManager->SetLastLogMessageWithTab( _T( "CActiveThreadList::ReportState() : End" ), nLayer );
	pcLogMessageManager->SetLastLogMessageWithTab( _T( "--------------------------------------------------------------------------------" ), nLayer );

	return bResult;
}


//*****************************************************************************
//
// End of File : CActiveThreadList.cpp
//
//*****************************************************************************