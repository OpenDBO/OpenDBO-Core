//*****************************************************************************
// File       : CBaseThreadManager.cpp
// Desc       : 
// Author     : June C. Kim (cynis@hotmail.com)
// Update     : 2007. 1. 18
//*****************************************************************************

//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "StdAfx.h"

#include "CBaseThreadManager.h"


//-----------------------------------------------------------------------------
// Name Space Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Initialize Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Name			: Initialize
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CBaseThreadManager::Initialize( void )
{
	m_nRemainThreadCount = 0;

	return;
}


//-----------------------------------------------------------------------------
// Name			: Destroy
// Desc			: 
// Privilege	: Protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CBaseThreadManager::Destroy( void )
{	
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

bool CBaseThreadManager::Create( UInt32 nActiveThreadSize )
{
	Destroy();

	if( nActiveThreadSize == 0 )
	{
		return false;
	}

	m_cActiveThreadList.Reserve( nActiveThreadSize );

	return true;
}


//-----------------------------------------------------------------------------
// Name			: AddThread
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CBaseThreadManager::AddThread( CBaseThread * pcThread )
{
	Assert( pcThread != NULL );

	SQueueData *	psData = new SQueueData;
	Assert( psData != NULL );

	psData->pvData = pcThread;
	psData->nID = pcThread->GetID();
	psData->nPriority = pcThread->GetPriority();

	m_cRemainThreadQueue.Push( psData );

	++m_nRemainThreadCount;

	return true;
}


//-----------------------------------------------------------------------------
// Name			: RemoveThread
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CBaseThread * CBaseThreadManager::RemoveThread( void )
{
	if( m_cRemainThreadQueue.GetCount() == 0 )
	{
		return NULL;
	}

	SQueueData *	psData = m_cRemainThreadQueue.Pop();
	Assert( psData != NULL );

	CBaseThread *	pcThread = reinterpret_cast<CBaseThread *>(psData->pvData);
	Assert( pcThread != NULL );

	--m_nRemainThreadCount;

	delete psData;

	return pcThread;
}


//-----------------------------------------------------------------------------
// Name			: Run
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CBaseThreadManager::Run( void )
{
	TCHAR		szBuffer[BASE_THREAD_REPORT_BUFFER_SIZE];

	g_Log( _T( "CBaseThreadManager::Run() : Begin" ) );

	Assert( m_cActiveThreadList.GetSize() > 0 );

	CBaseThread *	pcThread;
	UInt32		nSlotIndex;

	while( m_nRemainThreadCount != 0 ||
			m_cActiveThreadList.GetCount() > 0 )
	{
		// Activate Thread Pool

		while( m_cActiveThreadList.GetCount() < m_cActiveThreadList.GetSize() &&
				m_nRemainThreadCount != 0 )
		{
			pcThread = RemoveThread();
			Assert( pcThread != NULL );

			nSlotIndex = m_cActiveThreadList.AddThread( pcThread );

			_stprintf_s( szBuffer, BASE_THREAD_REPORT_BUFFER_SIZE, _T( "CBaseThreadManager::Run() : Register Thread : Slot %d (Id:%d, Handle:%d, Priority:%d)" ),
						nSlotIndex, pcThread->GetID(), reinterpret_cast<size_t>(pcThread->GetHandle()), pcThread->GetPriority() );
			g_Log( szBuffer );

			pcThread->Resume();
		}

		// Detect Inactive Thread

		DWORD	dwResult = WaitForMultipleObjects( m_cActiveThreadList.GetCount(), m_cActiveThreadList.GetHandleList(), false, INFINITE );
		if( dwResult >= WAIT_ABANDONED )
		{
			Assert( !"WaitForMultipleObjects() Error" );

			return false;
		}
		else
		{
			nSlotIndex = dwResult - WAIT_OBJECT_0;

			pcThread = m_cActiveThreadList.RemoveThread( nSlotIndex );
			Assert( pcThread != NULL );

			_stprintf_s( szBuffer, BASE_THREAD_REPORT_BUFFER_SIZE, _T( "CBaseThreadManager::Run() : Unregister Thread : Slot %d (Id:%d, Handle:%d, Priority:%d)" ),
						nSlotIndex, pcThread->GetID(), reinterpret_cast<size_t>(pcThread->GetHandle()), pcThread->GetPriority() );
			g_Log( szBuffer );

			delete pcThread;
		}
	}

	g_Log( _T( "CBaseThreadManager::Run() : End" ) );

	return true;
}


//-----------------------------------------------------------------------------
// Name			: ReportState
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CBaseThreadManager::ReportState( CLogMessageManager * pcLogMessageManager, UInt32 nLayer )
{
	if( pcLogMessageManager == NULL )
	{
		return false;
	}

	bool	bResult = true;

	pcLogMessageManager->SetLastLogMessageWithTab( _T( "" ), nLayer );
	pcLogMessageManager->SetLastLogMessageWithTab( _T( "================================================================================" ), nLayer );
	pcLogMessageManager->SetLastLogMessageWithTab( _T( "CBaseThreadManager::ReportState() : Begin" ), nLayer );
	pcLogMessageManager->SetLastLogMessageWithTab( _T( "================================================================================" ), nLayer );
	pcLogMessageManager->SetLastLogMessageWithTab( _T( "" ), nLayer );

	bResult &= m_cActiveThreadList.ReportState( pcLogMessageManager, nLayer + 1 );

	pcLogMessageManager->SetLastLogMessageWithTab( _T( "" ), nLayer );

	bResult &= m_cRemainThreadQueue.ReportState( pcLogMessageManager, nLayer + 1 );

	pcLogMessageManager->SetLastLogMessageWithTab( _T( "" ), nLayer );
	pcLogMessageManager->SetLastLogMessageWithTab( _T( "================================================================================" ), nLayer );
	pcLogMessageManager->SetLastLogMessageWithTab( _T( "CBaseThreadManager::ReportState() : End" ), nLayer );
	pcLogMessageManager->SetLastLogMessageWithTab( _T( "================================================================================" ), nLayer );
	pcLogMessageManager->SetLastLogMessageWithTab( _T( "" ), nLayer );

	return bResult;
}


//*****************************************************************************
//
// End of File : CBaseThreadManager.cpp
//
//*****************************************************************************