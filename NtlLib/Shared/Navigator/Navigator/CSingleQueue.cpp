//*****************************************************************************
// File       : CSingleQueue.cpp
// Desc       : 
// Author     : June C. Kim (cynis@hotmail.com)
// Update     : 2007. 1. 18
//*****************************************************************************

//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "StdAfx.h"

#include "CSingleQueue.h"


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

void CSingleQueue::Initialize( void )
{
	m_nCount = 0;
	m_nData = 0;

	return;
}


//-----------------------------------------------------------------------------
// Name			: Destroy
// Desc			: 
// Privilege	: Protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CSingleQueue::Destroy( void )
{
	Initialize();

	return;
}


//-----------------------------------------------------------------------------
// Name			: Push
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CSingleQueue::Push( SQueueData * psData )
{
	Assert( psData != NULL );

	m_tQueue.push( psData);

	++m_nCount;

	return true;
}


//-----------------------------------------------------------------------------
// Name			: Pop
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

SQueueData * CSingleQueue::Pop( void )
{
	Assert( m_tQueue.size() == m_nCount );

	SQueueData *	psData = m_tQueue.front();
	Assert( psData != NULL );

	m_tQueue.pop();

	--m_nCount;

	return psData;
}


//-----------------------------------------------------------------------------
// Name			: PopForce
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

SQueueData * CSingleQueue::PopForce( UInt32 nID )
{
	SQueueData *	psData;
	SQueueData *	psResult = NULL;

	for( UInt32 nIndex = 0; nIndex < m_nCount; ++nIndex )
	{
		psData = Pop();

		if( psData->nID == nID )
		{
			psResult = psData;
		}
		else
		{
			Push( psData );
		}
	}

	if( psResult != NULL )
	{
		--m_nCount;
	}

	return psResult;
}


//-----------------------------------------------------------------------------
// Name			: IsContain
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CSingleQueue::IsContain( UInt32 nID )
{
	SQueueData *	psData;
	bool	bFound = false;

	for( UInt32 nIndex = 0; nIndex < m_nCount; ++nIndex )
	{
		psData = Pop();

		if( psData->nID == nID )
		{
			bFound = true;
		}

		Push( psData );
	}

	return bFound;
}


//-----------------------------------------------------------------------------
// Name			: ReportState
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CSingleQueue::ReportState( CLogMessageManager * pcLogMessageManager, UInt32 nLayer )
{
	bool	bResult = true;
	TCHAR	szBuffer[QUEUE_REPORT_BUFFER_SIZE];

	pcLogMessageManager->SetLastLogMessageWithTab( _T( "--------------------------------------------------------------------------------" ), nLayer );
	pcLogMessageManager->SetLastLogMessageWithTab( _T( "CSingleQueue::ReportState() : Begin" ), nLayer );
	pcLogMessageManager->SetLastLogMessageWithTab( _T( "--------------------------------------------------------------------------------" ), nLayer );
	pcLogMessageManager->SetLastLogMessageWithTab( _T( "" ) );

	_stprintf_s( szBuffer, QUEUE_REPORT_BUFFER_SIZE, _T( "Count : %d, Priority : %d" ),
					m_nCount, m_nData );
	pcLogMessageManager->SetLastLogMessageWithTab( szBuffer, nLayer );
	pcLogMessageManager->SetLastLogMessageWithTab( _T( "" ), nLayer );

	SQueueData *	psData;

	for( UInt32 nIndex = 0; nIndex < m_nCount; ++nIndex )
	{
		psData = Pop();
		Assert( psData != NULL );

		_stprintf_s( szBuffer, QUEUE_REPORT_BUFFER_SIZE, _T( "ID : %d, Priority : %d, Thread : 0x%08p" ),
					psData->nID, psData->nPriority, psData->pvData );
		pcLogMessageManager->SetLastLogMessageWithTab( szBuffer, nLayer + 1 );

		Push( psData );
	}

	pcLogMessageManager->SetLastLogMessageWithTab( _T( "" ), nLayer );
	pcLogMessageManager->SetLastLogMessageWithTab( _T( "--------------------------------------------------------------------------------" ), nLayer );
	pcLogMessageManager->SetLastLogMessageWithTab( _T( "CSingleQueue::ReportState() : End" ), nLayer );
	pcLogMessageManager->SetLastLogMessageWithTab( _T( "--------------------------------------------------------------------------------" ), nLayer );

	return bResult;
}

//*****************************************************************************
//
// End of File : CSingleQueue.cpp
//
//*****************************************************************************