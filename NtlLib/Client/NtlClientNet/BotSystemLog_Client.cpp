//-- include ------------------------------------------------------------------------
#include "StdAfx.h"
#include "BotSystemLog_Client.h"
#include "NtlPacket.h"
#include "NtlBase.h"
#include "NtlPacket.h"
#include "NtlSocket.h"
#include "NtlNetBuffer.h"
#include "NtlPacketSYS.h"

#include "NtlPacketEncoder_XOR.h"
#include "NtlPacketEncoder_RandKey.h"


#include "NtlQueue.h"
#include "NtlMutex.h"
#include "NtlError.h"
#include "NtlDebug.h"
//-----------------------------------------------------------------------------------

//-- SGpro --------------------------------------------------------------------------
//  Purpose : 
//  Return  : 
//-----------------------------------------------------------------------------------
CBotSystemLog_Client::CBotSystemLog_Client()
: m_pOutFile ( NULL )
, m_bIsValid ( false )
{
}

//-- SGpro --------------------------------------------------------------------------
//  Purpose : 
//  Return  : 
//-----------------------------------------------------------------------------------
CBotSystemLog_Client::~CBotSystemLog_Client()
{
	Destroy();
}

//-- SGpro --------------------------------------------------------------------------
//  Purpose :
//  Return  :
//-----------------------------------------------------------------------------------
void CBotSystemLog_Client::Create( char* pszFileName )
{
	m_pOutFile = new std::ofstream(pszFileName, std::ios::out);
	if( NULL == m_pOutFile )
	{
		_ASSERT( 0 );
	}

	m_bIsValid = true;

	return;
}

//-- SGpro --------------------------------------------------------------------------
//  Purpose :
//  Return  :
//-----------------------------------------------------------------------------------
void CBotSystemLog_Client::Destroy( void )
{
	if( NULL != m_pOutFile )
	{
		m_pOutFile->close();
	}

	delete m_pOutFile;
	m_pOutFile = NULL;
	return;
}

//-- SGpro --------------------------------------------------------------------------
//  Purpose :
//  Return  :
//-----------------------------------------------------------------------------------
void CBotSystemLog_Client::Write( TCHAR* ptszData, bool bIsAutoEnter )
{
	if( NULL == m_pOutFile )
	{
		return;
	}

	if( true == bIsAutoEnter )
	{
		*m_pOutFile << ptszData << "\r\n";
	}
	else
	{
		*m_pOutFile << ptszData;
	}
	return;
}

//-- SGpro --------------------------------------------------------------------------
//  Purpose :
//  Return  :
//-----------------------------------------------------------------------------------
void CBotSystemLog_Client::Flush( void )
{
	*m_pOutFile << std::flush;
}

//-- SGpro --------------------------------------------------------------------------
//  Purpose :
//  Return  :
//-----------------------------------------------------------------------------------
void CBotSystemLog_Client::WriteBotSystemLog( void* pPacketBuf, __int64 nCharacterPCID, CNtlNetBuffer* pSendBuffer, int nSize )
{
	if( false == IsValid() )
	{
		return;
	}

	TCHAR szLogBuffer[1024] = { 0x00, };
	PACKETHEADER* pPacketHeader = (PACKETHEADER* )pPacketBuf;

	struct sNTLPACKETHEADER
	{
		WORD			wOpCode;

	public:
		sNTLPACKETHEADER(WORD wGivenOpCode) 
			: wOpCode(wGivenOpCode) 
		{
		}
	};

	sNTLPACKETHEADER * pPacket = ( sNTLPACKETHEADER * ) ((BYTE*)pPacketBuf + 4);

	_stprintf_s( szLogBuffer, _T( "-----------------------------------------------" ) );
	Write( szLogBuffer );

	bool bEncrypt = false;
	if( 0 < pPacketHeader->bEncrypt )
	{
		bEncrypt = true;
	}

	_stprintf_s( szLogBuffer, _T( "Session[%X], CharID[%d] \r\n" )
		, this, nCharacterPCID );
	Write( szLogBuffer );

	_stprintf_s( szLogBuffer, _T("OpCode : %d \r\n" )
		, pPacket->wOpCode );
	Write( szLogBuffer );

	_stprintf_s( szLogBuffer, _T("Encrypt : %d \r\n"
		"PacketLen : %d ( PacketHeader 포함 : %d ) \r\n"
		"Sequence: %d \r\n"
		"Checksum : %d \r\n"
		"nSize : %d \r\n"
		"pSendBuffer->GetPushPos() : %d \r\n"
		"pSendBuffer->GetPopPos() : %d \r\n"
		"pSendBuffer->GetWorkPos() : %d \r\n"
		"pSendBuffer->GetCurSize() : %d \r\n"
		"pSendBuffer->GetFreeSize() : %d \r\n"
		"pSendBuffer->GetLinearFreeSize() : %d \r\n"
		"pSendBuffer->GetQueueSize() : %d \r\n"
		"pSendBuffer->GetQueueTotalSize() : %d \r\n"
		"pSendBuffer->GetQueueExtraSize() : %d \r\n"
		)
		, bEncrypt
		, pPacketHeader->wPacketLen, pPacketHeader->wPacketLen + 4
		, pPacketHeader->bySequence
		, pPacketHeader->byChecksum 
		, nSize
		, pSendBuffer->GetPushPos()
		, pSendBuffer->GetPopPos()
		, pSendBuffer->GetWorkPos()
		, pSendBuffer->GetCurSize()
		, pSendBuffer->GetFreeSize()
		, pSendBuffer->GetLinearFreeSize()			
		, pSendBuffer->GetQueueSize()
		, pSendBuffer->GetQueueTotalSize()
		, pSendBuffer->GetQueueExtraSize()
		);

	Write( szLogBuffer );


	//_stprintf_s( szLogBuffer, _T( "Data (PACKET_HEADSIZE까지 포함) : " ) );
	//Write( szLogBuffer );

	//BYTE* byHex = (BYTE*) pPacketBuf;
	//int nHexEnterCnt = 0;
	//
	//_stprintf_s( szLogBuffer, _T( "0x%X(%d) \t" )
	//	, &(byHex[0])
	//	, &(byHex[0]) );
	//Write( szLogBuffer );

	//for( int nT1 = 0, nT2 = 0
	//	; pPacketHeader->wPacketLen + 4 > nT1 
	//	; ++nT1 )
	//{
	//	if( 8 > nHexEnterCnt )
	//	{
	//		_stprintf_s( szLogBuffer, _T( "%02X " ), byHex[nT1]);
	//		++nHexEnterCnt;
	//		nT2 += 3;
	//		Write( szLogBuffer, false );
	//	}
	//	else
	//	{
	//		_stprintf_s(  szLogBuffer, _T( "%02X " )
	//			, byHex[nT1]
	//			, &(byHex[nT1])
	//			, &(byHex[nT1]) );
	//		Write( szLogBuffer, false );

	//		_stprintf_s( szLogBuffer, _T( "0x%X(%d) \t")
	//			, byHex[nT1]
	//			, &(byHex[nT1])
	//			, &(byHex[nT1]) );

	//		Write( szLogBuffer );

	//		nHexEnterCnt = 0;
	//		nT2 += 5;
	//	}
	//}

	//Write( _T( "" ) );
	Flush();

	return;
}