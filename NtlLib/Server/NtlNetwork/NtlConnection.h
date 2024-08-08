//***********************************************************************************
//
//	File		:	NtlConnection.h
//
//	Begin		:	2005-12-19
//
//	Copyright	:	ⓒ NTL-Inc Co., Ltd
//
//	Author		:	Hyun Woo, Koo   ( zeroera@ntl-inc.com )
//
//	Desc		:	Network IO Connection Class
//
//***********************************************************************************

#pragma once


#include "NtlBitFlag.h"
#include "NtlIOCP.h"
#include "NtlSocket.h"
#include "NtlNetBuffer.h"
#include "NtlPacketEncoder.h"

#include "NtlMutex.h"
#include "NetworkCompileOption.h"// [5/26/2008 SGpro]

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
const DWORD CONNECTION_KEEP_ALIVE_CHECK_TIME	= 15 * 1000; // 이 시간 이내에 Alive Ping이 들어오지 않으면 클라이언트를 강제 종료 시킴
//-----------------------------------------------------------------------------------


class CNtlNetwork;
class CNtlSession;
class CNtlAcceptor;
class CNtlConnector;

#if __SGPRO_BOTSYSTEM_SENDANDRECVTESTCODE__
#include "BotSystemLog_Server.h"
#endif

class CNtlConnection
{
	friend class CIocpWorkerThread;

#if __SGPRO_BOTSYSTEM_SENDANDRECVTESTCODE__
	public:
		CBotSystemLog_Server m_cConsolLog;
		void SetCharacterPCID( __int64 nCharacterPCID ) { m_nCharacterPCID = nCharacterPCID; }

		void CreateLogFile( void )
		{
			char szCharacterPCID[1024] = { 0, };

			sprintf_s( szCharacterPCID, sizeof( szCharacterPCID ), "%d.log", m_nCharacterPCID );
			m_cConsolLog.Create( szCharacterPCID );
			return;
		}

	private:
		__int64 m_nCharacterPCID;
#endif

public:

	enum eCONTROLFLAG
	{
		CONTROL_FLAG_CHECK_ALIVE = 0x01 << 0,
		CONTROL_FLAG_CHECK_OPCODE = 0x01 << 1,
		CONTROL_FLAG_USE_SEND_QUEUE = 0x01 << 2,
		CONTROL_FLAG_USE_RECV_QUEUE = 0x01 << 3,

		MAX_CONTROL_FLAG
	};


	enum eSTATUS					
	{
		STATUS_INIT = 0,			// 초기상태
		STATUS_CREATE,				// 생성상태
		STATUS_ACCEPT,				// Accept 대기 상태
		STATUS_CONNECT,				// Connect 대기 상태 ( AcceptEx, ConnectEx 에서 사용 )
		STATUS_ACTIVE,				// 활동상태
		STATUS_CLOSE,				// 닫힌 상태
		STATUS_SHUTDOWN,			// 종료 상태
		STATUS_DESTROY,				// 소멸 상태

		MAX_STATUS
	};


public:

	CNtlConnection();

	virtual ~CNtlConnection();

public:

	int									Create(CNtlNetwork * pNetwork);

public:


	int									CompleteIO(sIOCONTEXT * pIOContext, DWORD dwParam);

	void								Close(bool bForce = false);




public:

	virtual DWORD						GetMaxRecvPacketSize() { return PACKET_MAX_SIZE; }

	virtual DWORD						GetMaxSendPacketSize() { return PACKET_MAX_SIZE; }

	virtual DWORD						GetMaxRecvPacketCount() { return DEF_PACKET_MAX_COUNT; }

	virtual DWORD						GetMaxSendPacketCount() { return DEF_PACKET_MAX_COUNT; }

	virtual DWORD						GetAliveCheckTime() { return CONNECTION_KEEP_ALIVE_CHECK_TIME; }

	void								ResetAliveTime() { m_dwAliveTime = 0; }


public:

	int									Disconnect(bool bGraceful);

	bool								ValidCheck(DWORD dwTickDiff);

	bool								IsShutdownable();

	int									Shutdown();

public:


	int									PostRecv();

	int									PostSend();	

	int									PostAccept(CNtlAcceptor* pAcceptor);

	int									PostConnect(CNtlConnector* pConnector);


	int									CompleteRecv(DWORD dwTransferedBytes);

	int									CompleteSend(DWORD dwTransferedBytes);

	int									CompleteAccept(DWORD dwTransferedBytes);

	int									CompleteConnect(DWORD dwTransferedBytes);


	int									PushPacket(CNtlPacket * pPacket);

	bool								PopPacket(CNtlPacket * pPacket);

	virtual bool						IsValidPacket(void * pvPacketHeader, WORD wPacketLength);

	virtual BYTE						GetSequence(void * pvPacketHeader);


	void								Lock() { m_mutex.Lock(); }

	void								Unlock() { m_mutex.Unlock(); }

	CNtlMutex *							GetMutex() { return &m_mutex; }


public:


	sIOCONTEXT *						GetRecvContext() { return &m_recvContext; }

	sIOCONTEXT *						GetSendContext() { return &m_sendContext; }

	CNtlNetBuffer *						GetRecvBuffer() { return &m_recvBuffer; }

	CNtlNetBuffer *						GetSendBuffer() { return &m_sendBuffer; }

	CNtlSocket &						GetSocket() { return m_socket; }

	CNtlSockAddr &						GetRemoteAddr() { return m_remoteAddr; }

	CNtlSockAddr &						GetLocalAddr() { return m_localAddr; }

	void								SetAddress(SOCKADDR_IN * pLocalAddr, SOCKADDR_IN * pRemoteAddr);

	const char *						GetRemoteIP() { return m_remoteAddr.GetDottedAddr(); }

	WORD								GetRemotePort() { return m_remoteAddr.GetPort(); }

	const char *						GetLocalIP() { return m_localAddr.GetDottedAddr(); }

	WORD								GetLocalPort() { return m_localAddr.GetPort(); }

	bool								GetAddressInfo(CNtlString * pString, WORD * pPort, bool bRemote /* = true */);

	CNtlConnector *						GetConnector() { return m_pConnectorRef; }

	CNtlAcceptor *						GetAcceptor() { return m_pAcceptorRef; }

	int									MakeSureCompletedPacket(CNtlNetBuffer* pBuffer, WORD* pwPacketLength, CNtlPacketEncoder * pPacketEncoder);
										
public:

	void								SetControlFlag(DWORD dwControlFlag) { BIT_FLAG_SET( m_dwControlFlag, dwControlFlag); }

	void								UnsetControlFlag(DWORD dwControlFlag) { BIT_FLAG_UNSET( m_dwControlFlag, dwControlFlag); }

	bool								IsSetControlFlag(DWORD dwControlFlag) { return BIT_FLAG_TEST( m_dwControlFlag, dwControlFlag); }

	void								SetPacketEncoder(CNtlPacketEncoder * pPacketEncoder) { m_pPacketEncoder = pPacketEncoder; }

	CNtlPacketEncoder *					GetPacketEncoder() { return m_pPacketEncoder; }


public:

	eSTATUS								GetStatus();

	void								SetStatus(eSTATUS status);

	bool								IsStatus(eSTATUS status);

	bool								ExchangeStatus(const eSTATUS prevStatus, bool bResult, const eSTATUS newStatus);

	const char *						GetStatusString();

	bool								IsConnected() { return m_bConnected; }


public:

	DWORD								GetConnectTime() { return m_dwConnectTime; }

	DWORD								GetBytesTotalSize() { return GetBytesRecvSize() + GetBytesSendSize(); }
	DWORD								GetBytesRecvSize() { return m_dwBytesRecvSize; }
	DWORD								GetBytesRecvSizeMax() { return m_dwBytesRecvSizeMax; }
	DWORD								GetBytesSendSize() { return m_dwBytesSendSize; }
	DWORD								GetBytesSendSizeMax() { return m_dwBytesSendSizeMax; }
	void								IncreaseBytesRecv(DWORD dwSize);
	void								IncreaseBytesSend(DWORD dwSize);

	DWORD								GetPacktTotalCount() { return GetPacketRecvCount() + GetPacketSendCount(); }
	DWORD								GetPacketRecvCount() { return m_dwPacketRecvCount; }
	DWORD								GetPacketSendCount() { return m_dwPacketSendCount; }
	void								IncreasePacketRecv();
	void								IncreasePacketSend();

	DWORD								GetRecvQueueMaxUseSize() { return m_recvQueue.GetMaxUsedSize(); }
	DWORD								GetSendQueueMaxUseSize() { return m_sendQueue.GetMaxUsedSize(); }

	virtual int							GetHeaderSize() {return PACKET_HEADSIZE;}
	virtual int							GetPacketLen(BYTE* pHeaderPointer);

	virtual void						SetSequence(void* pvPacketHeader, BYTE bySequence);

	virtual bool						HasValidSequence(void* pvPacketHeader, BYTE bySequence);


private:

	void								Init();

	void								Destroy();


private:

	void								IncreasePostIoCount() { InterlockedIncrement( (LONG*)&m_dwIoPostCount); }

	void								DecreasePostIoCount() { InterlockedDecrement( (LONG*)&m_dwIoPostCount); }


private:

	eSTATUS								m_status;

	CNtlNetwork *						m_pNetworkRef;

	CNtlAcceptor *						m_pAcceptorRef;

	CNtlConnector *						m_pConnectorRef;

	DWORD								m_dwIoPostCount;


	CNtlSocket							m_socket;

	CNtlSockAddr						m_remoteAddr;

	CNtlSockAddr						m_localAddr;

	sIOCONTEXT							m_recvContext;

	sIOCONTEXT							m_sendContext;

	DWORD								m_dwAliveTime;	


private:

	CNtlNetBuffer 						m_recvBuffer;

	CNtlNetBuffer 						m_sendBuffer;

	CNtlNetQueue						m_recvQueue;

	CNtlNetQueue						m_sendQueue;

	CNtlPacketEncoder *					m_pPacketEncoder;


private:

	DWORD								m_dwControlFlag;

	bool								m_bSending;

	bool								m_bDisconnect;

	bool								m_bConnected;

	CNtlMutex							m_mutex;

	CNtlMutex							m_mutexSend;


private:

	DWORD								m_dwConnectTime;

	DWORD								m_dwBytesRecvSize;
	DWORD								m_dwBytesSendSize;
	DWORD								m_dwBytesRecvSizeMax;
	DWORD								m_dwBytesSendSizeMax;

	DWORD								m_dwPacketRecvCount;
	DWORD								m_dwPacketSendCount;

};



//-----------------------------------------------------------------------------------
// inline function
//-----------------------------------------------------------------------------------
inline void CNtlConnection::IncreaseBytesRecv(DWORD dwSize)
{
	InterlockedExchangeAdd( (LONG*)&m_dwBytesRecvSize, dwSize );

	if( dwSize > m_dwBytesRecvSizeMax )
	{
		m_dwBytesRecvSizeMax = dwSize;
	}
}

inline void CNtlConnection::IncreaseBytesSend(DWORD dwSize)
{
	InterlockedExchangeAdd( (LONG*)&m_dwBytesSendSize, dwSize );

	if( dwSize > m_dwBytesSendSizeMax )
	{
		m_dwBytesSendSizeMax = dwSize;
	}
}

inline void CNtlConnection::IncreasePacketRecv()
{
	InterlockedIncrement( (LONG*)&m_dwPacketRecvCount );
}

inline void CNtlConnection::IncreasePacketSend()
{
	InterlockedIncrement( (LONG*)&m_dwPacketSendCount );
}

inline CNtlConnection::eSTATUS CNtlConnection::GetStatus()
{
	CNtlAutoMutex mutex( &m_mutex );
	mutex.Lock();

	eSTATUS status = m_status;

	return status;
}

inline void	CNtlConnection::SetStatus(eSTATUS status)
{
	CNtlAutoMutex mutex( &m_mutex );
	mutex.Lock();

	m_status = status;
}

inline bool CNtlConnection::IsStatus(eSTATUS status)
{
	CNtlAutoMutex mutex( &m_mutex );
	mutex.Lock();

	return m_status == status;

}

inline bool CNtlConnection::ExchangeStatus(const eSTATUS prevStatus, const bool bResult, const eSTATUS newStatus)
{
	CNtlAutoMutex mutex( &m_mutex );
	mutex.Lock();

	if( bResult )
	{
		if( prevStatus != m_status )
			return false;
	}
	else
	{
		if( prevStatus == m_status )
			return false;
	}

	m_status = newStatus;

	return true;
}

//---------------------------------------------------------------------------------------
// 
//---------------------------------------------------------------------------------------
inline int CNtlConnection::MakeSureCompletedPacket(CNtlNetBuffer* pBuffer, WORD* pwPacketLength, CNtlPacketEncoder * pPacketEncoder)
{
	if (NULL == pwPacketLength)
	{
		_ASSERT( 0 );
		return NTL_ERR_SYS_INPUT_PARAMETER_WRONG;
	}


	// 헤더가 완성되지 않은 경우
	if ( pBuffer->GetWorkRemainSize() < GetHeaderSize() )
	{
		return NTL_ERR_NET_PACKET_PENDING_HEADER;
	}


	int nLinearSize = (int) ( pBuffer->InGetQueueExtraPtr() - pBuffer->GetQueueWorkPtr() );


	// 헤더가 버퍼에서 잘린 경우
	if ( nLinearSize < GetHeaderSize() )
	{
		::CopyMemory( pBuffer->InGetQueueExtraPtr(), pBuffer->InGetQueueBufferPtr(), GetHeaderSize() - nLinearSize );
	}

	if( pPacketEncoder )
	{
		int rc = pPacketEncoder->RxDecrypt( pBuffer->GetQueueWorkPtr() );
		if ( NTL_SUCCESS != rc )
		{
			return rc;
		}
	}

	int wPacketLen = GetPacketLen( pBuffer->GetQueueWorkPtr() );

	int nPacketSize = GetHeaderSize() + wPacketLen;
	if( nPacketSize >= pBuffer->GetMaxPacketSize() )
	{
		_ASSERT( 0 );
		return NTL_ERR_NET_PACKET_EXCEED_ALLOWED_SIZE;
	}


	// 패킷이 완성되지 않은 경우
	if ( pBuffer->GetWorkRemainSize() < nPacketSize )
	{
		return NTL_ERR_NET_PACKET_PENDING_DATA;
	}


	// 패킷이 버퍼에서 잘린 경우
	if ( nLinearSize < nPacketSize )
	{
		if ( nLinearSize < GetHeaderSize() )
		{
			::CopyMemory(	pBuffer->GetQueueWorkPtr() + GetHeaderSize(),
				pBuffer->InGetQueueBufferPtr() + GetHeaderSize() - nLinearSize,
				wPacketLen );
		}
		else
		{
			::CopyMemory(	pBuffer->InGetQueueExtraPtr(),
				pBuffer->InGetQueueBufferPtr(),
				nPacketSize - nLinearSize );
		}
	}

	*pwPacketLength = (WORD) nPacketSize;


	return NTL_SUCCESS;
}