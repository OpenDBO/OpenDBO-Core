//-- include ------------------------------------------------------------------------
#include "StdAfx.h"
#include "BotSystemLog_Server.h"
#include "NtlNetBuffer.h"
#include <tchar.h>
//-----------------------------------------------------------------------------------

//-- SGpro --------------------------------------------------------------------------
//  Purpose : 
//  Return  : 
//-----------------------------------------------------------------------------------
CBotSystemLog_Server::CBotSystemLog_Server()
: m_pOutFile ( NULL )
, m_bIsValid ( false )
{
}

//-- SGpro --------------------------------------------------------------------------
//  Purpose : 
//  Return  : 
//-----------------------------------------------------------------------------------
CBotSystemLog_Server::~CBotSystemLog_Server()
{
	Destroy();
}

//-- SGpro --------------------------------------------------------------------------
//  Purpose :
//  Return  :
//-----------------------------------------------------------------------------------
void CBotSystemLog_Server::Create( char* pszFileName )
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
void CBotSystemLog_Server::Destroy( void )
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
void CBotSystemLog_Server::Write( TCHAR* ptszData, bool bIsAutoEnter )
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
void CBotSystemLog_Server::Flush( void )
{
	*m_pOutFile << std::flush;
}

//-- SGpro --------------------------------------------------------------------------
//  Purpose :
//  Return  :
//-----------------------------------------------------------------------------------
void CBotSystemLog_Server::WriteBotSystemLog( void* pPacketBuf, int len, bool bIsIf, CNtlNetBuffer* pSendBuffer, __int64 nCharacterPCID )
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
			:wOpCode(wGivenOpCode) 
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
		"bIsIf : %d \r\n"
		"m_sendContext.wsabuf.len : %d \r\n"
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
		, bIsIf
		, len
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