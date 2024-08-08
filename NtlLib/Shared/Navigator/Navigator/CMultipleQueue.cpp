//*****************************************************************************
// File       : CMultipleQueue.cpp
// Desc       : 
// Author     : June C. Kim (cynis@hotmail.com)
// Update     : 2007. 1. 18
//*****************************************************************************

//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "StdAfx.h"

#include "CMultipleQueue.h"


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

void CMultipleQueue::Initialize( void )
{
	m_nCount	= 0;
	m_nData		= 0;
	m_nQueueCount	= 0;

	return;
}


//-----------------------------------------------------------------------------
// Name			: Destroy
// Desc			: 
// Privilege	: Protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CMultipleQueue::Destroy( void )
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

bool CMultipleQueue::Push( SQueueData * psData )
{
	CSingleQueue *	pcQueue = GetQueue( psData->nPriority );

	if( pcQueue == NULL )
	{
		pcQueue = new CSingleQueue( psData->nPriority );
		Assert( pcQueue != NULL );

		InsertQueue( pcQueue );

		++m_nQueueCount;
	}

	if( pcQueue->Push( psData ) == true )
	{
		++m_nCount;

		return true;
	}
	else
	{
		return false;
	}
}


//-----------------------------------------------------------------------------
// Name			: Pop
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

SQueueData * CMultipleQueue::Pop( void )
{
	if( m_nCount == 0 )
	{
		return NULL;
	}

	CSingleQueue *	pcQueue = m_tQueues.front();
	SQueueData *	psData = pcQueue->Pop();
	Assert( psData != NULL );

	if( pcQueue->GetCount() == 0 )
	{
		m_tQueues.pop_front();

		delete pcQueue;

		--m_nQueueCount;
	}

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

SQueueData * CMultipleQueue::PopForce( UInt32 nID )
{
	CSingleQueue *	pcQueue;
	SQueueData *	psData;
	TQueueListConstIterator		tCurrentPosition = m_tQueues.begin();

	while( tCurrentPosition != m_tQueues.end() )
	{
		pcQueue = *tCurrentPosition;
		Assert( pcQueue != NULL );

		psData = pcQueue->PopForce( nID );

		if( psData != NULL )
		{
			--m_nCount;

			return psData;
		}

		if( pcQueue->GetCount() == 0 )
		{
			m_tQueues.remove( pcQueue );
		}

		++tCurrentPosition;
	}

	return NULL;
}


//-----------------------------------------------------------------------------
// Name			: IsContain
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CMultipleQueue::IsContain( UInt32 nID )
{
	CSingleQueue *	pcQueue;
	TQueueListConstIterator		tCurrentPosition = m_tQueues.begin();

	while( tCurrentPosition != m_tQueues.end() )
	{
		pcQueue = *tCurrentPosition;
		Assert( pcQueue != NULL );

		if( pcQueue->IsContain( nID ) == true )
		{
			return true;
		}

		++tCurrentPosition;
	}

	return false;
}


//-----------------------------------------------------------------------------
// Name			: GetQueue
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CSingleQueue * CMultipleQueue::GetQueue( UInt32 nPriority ) const
{
	CSingleQueue *	pcQueue;

	TQueueListConstIterator		tCurrentPosition = m_tQueues.begin();

	while( tCurrentPosition != m_tQueues.end() )
	{
		pcQueue = *tCurrentPosition;
		Assert( pcQueue != NULL );

		if( pcQueue->GetData() == nPriority )
		{
			return pcQueue;
		}

		++tCurrentPosition;
	}

	return NULL;
}


//-----------------------------------------------------------------------------
// Name			: InsertQueue
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CMultipleQueue::InsertQueue( CSingleQueue * pcQueue )
{
	Assert( pcQueue != NULL );

	CSingleQueue *	pcCurrentQueue;
	TQueueListIterator	tCurrentPosition = m_tQueues.begin();

	while( tCurrentPosition != m_tQueues.end() )
	{
		pcCurrentQueue = *tCurrentPosition;
		Assert( pcCurrentQueue != NULL );

		if( pcQueue->GetData() > pcCurrentQueue->GetData() )
		{
			m_tQueues.insert( tCurrentPosition, pcQueue );

			return true;
		}

		Assert( pcQueue->GetData() < pcCurrentQueue->GetData() );

		++tCurrentPosition;
	}

	m_tQueues.push_back( pcQueue );

	return true;
}


//-----------------------------------------------------------------------------
// Name			: ReportState
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CMultipleQueue::ReportState( CLogMessageManager * pcLogMessageManager, UInt32 nLayer )
{
	bool	bResult = true;
	TCHAR	szBuffer[QUEUE_REPORT_BUFFER_SIZE];

	pcLogMessageManager->SetLastLogMessageWithTab( _T( "--------------------------------------------------------------------------------" ), nLayer );
	pcLogMessageManager->SetLastLogMessageWithTab( _T( "CMultipleQueue::ReportState() : Begin" ), nLayer );
	pcLogMessageManager->SetLastLogMessageWithTab( _T( "--------------------------------------------------------------------------------" ), nLayer );
	pcLogMessageManager->SetLastLogMessageWithTab( _T( "" ) );

	_stprintf_s( szBuffer, QUEUE_REPORT_BUFFER_SIZE, _T( "Count : %d, QueueCount : %d, Priority : %d" ),
					m_nCount, m_nQueueCount, m_nData );
	pcLogMessageManager->SetLastLogMessageWithTab( szBuffer, nLayer );
	pcLogMessageManager->SetLastLogMessageWithTab( _T( "" ), nLayer );

	CSingleQueue *	pcQueue;

	TQueueListConstIterator		tCurrentPosition = m_tQueues.begin();

	while( tCurrentPosition != m_tQueues.end() )
	{
		pcQueue = *tCurrentPosition;
		Assert( pcQueue != NULL );

		bResult &= pcQueue->ReportState( pcLogMessageManager, nLayer + 1 );

		++tCurrentPosition;
	}

	pcLogMessageManager->SetLastLogMessageWithTab( _T( "" ), nLayer );
	pcLogMessageManager->SetLastLogMessageWithTab( _T( "--------------------------------------------------------------------------------" ), nLayer );
	pcLogMessageManager->SetLastLogMessageWithTab( _T( "CMultipleQueue::ReportState() : End" ), nLayer );
	pcLogMessageManager->SetLastLogMessageWithTab( _T( "--------------------------------------------------------------------------------" ), nLayer );

	return bResult;
}

//*****************************************************************************
//
// End of File : CMultipleQueue.cpp
//
//*****************************************************************************