//***********************************************************************************
//
//	File		:	NetworkDispatcher.cpp
//
//	Begin		:	2007-01-04
//
//	Copyright	:	¨Ï NTL-Inc Co., Ltd
//
//	Author		:	Hyun Woo, Koo   ( zeroera@ntl-inc.com )
//
//	Desc		:	
//
//***********************************************************************************

#include "stdafx.h"
#include "NtlNetworkProcessor.h"

#include "NtlNetwork.h"
#include "NtlSession.h"
#include "NtlPacket.h"

#include "NtlLog.h"
#include "NtlError.h"


//---------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------
const ULONG_PTR THREAD_CLOSE = (ULONG_PTR)(-1);	// thread terminate value
//---------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CNtlNetworkProcessor::CNtlNetworkProcessor(CNtlNetwork *  pNetwork):
m_hEventIOCP( 0 )
{
	SetArg( pNetwork);
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CNtlNetworkProcessor::~CNtlNetworkProcessor()
{
	Destroy();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
int CNtlNetworkProcessor::Create()
{
	if (NULL != m_hEventIOCP)
	{
		NTL_LOG_ASSERT("(NULL != m_hEventIOCP) m_hEventIOCP = %016x", m_hEventIOCP);
	}

	m_hEventIOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 1);
	if( NULL == m_hEventIOCP )
	{
		return GetLastError();
	}

	return NTL_SUCCESS;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlNetworkProcessor::Destroy()
{
	if( NULL != m_hEventIOCP )
	{
		CloseHandle( m_hEventIOCP );
	}
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlNetworkProcessor::Run()
{
	CNtlNetwork * pNetwork = (CNtlNetwork*) GetArg();
	if( NULL == pNetwork )
	{
		NTL_LOG_ASSERT("(NULL == pNetwork)");
		return;	
	}


	BOOL bResult = FALSE;
	DWORD dwBytesTransferred = 0;
	ULONG_PTR netEvent = INVALID_NETEVENT;
	CNtlSession * pSession = NULL;

	while( IsRunnable() )
	{	
		bResult = GetQueuedCompletionStatus(	m_hEventIOCP,
												&dwBytesTransferred,
												(ULONG_PTR*) &netEvent,
												(LPOVERLAPPED*) &pSession,
												INFINITE );


		if( THREAD_CLOSE == (ULONG_PTR) netEvent )
		{
			NTL_PRINT( PRINT_SYSTEM,"Thread Close" );
			break;
		}	

		if( FALSE == bResult )
		{
			int rc = GetLastError();
			NTL_PRINT( PRINT_SYSTEM, "Dispatcher\tGQCS Failed : Err:%d(%s)", rc, NtlGetErrorMessage(rc) );
			continue;
		}

		if( NULL == pSession )
		{
			continue;
		}

		switch( netEvent )
		{
		case NETEVENT_ACCEPT: 
			{
				int rc = pSession->OnAccept();
				if( NTL_SUCCESS != rc )
				{
					pSession->Disconnect( false );
				}
				
			}
			break;

		case NETEVENT_CONNECT:
			{
				int rc = pSession->OnConnect();
				if( NTL_SUCCESS != rc )
				{
					pSession->Disconnect( false );
				}
			}
			break;

		case NETEVENT_CLOSE:
			{
				static int nCount = 0;
				NTL_PRINT( 2, "Session[%X] NETEVENT_CLOSE Close Count[%u]", pSession, ++nCount );
				pSession->OnClose();
				RELEASE_SESSION( pSession );
			}
			break;

		case NETEVENT_RECV:
			{
				int rc = NTL_SUCCESS;
				CNtlPacket packet;
				if( pSession->PopPacket( &packet ) )
				{
					rc = pSession->OnDispatch( &packet );

					int nPacketLen = pSession->GetPacketLen( (BYTE*) (packet.GetPacketHeader()) );
					pSession->GetRecvBuffer()->IncreasePopPos( pSession->GetHeaderSize() + nPacketLen );

					if( pSession->IsSetControlFlag( CNtlSession::CONTROL_FLAG_CHECK_OPCODE ) )
					{
						if( NTL_SUCCESS != rc )
						{
							pSession->Disconnect( false );
							break;
						}
					}
				}
			}
			break;

		default:
			NTL_LOG_ASSERT("netEvent is not valid. netEvent = %d", netEvent);
			break;

		}

	} // end of while( m_bRunning )

}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
int CNtlNetworkProcessor::PostNetEvent(WPARAM wParam, LPARAM lParam)
{
	if (NULL == m_hEventIOCP)
	{
		NTL_LOG_ASSERT("(NULL == m_hEventIOCP)");
	}

	if( 0 == PostQueuedCompletionStatus( m_hEventIOCP, 0, (ULONG_PTR)wParam, (LPOVERLAPPED)lParam ) )
	{
		return GetLastError();
	}

	return NTL_SUCCESS;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlNetworkProcessor::Close()
{
	PostQueuedCompletionStatus( m_hEventIOCP, 0, THREAD_CLOSE, NULL );

	CNtlRunObject::Close();
}
