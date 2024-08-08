//***********************************************************************************
//
//	File		:	NtlSession.cpp
//
//	Begin		:	2005-12-13
//
//	Copyright	:	¨Ï NTL-Inc Co., Ltd
//
//	Author		:	Hyun Woo, Koo   ( zeroera@ntl-inc.com )
//
//	Desc		:	Network Session Class
//
//***********************************************************************************

#include "stdafx.h"
#include "NtlSession.h"

#include "NtlPacketSYS.h"
#include "NtlLog.h"
#include "NtlError.h"


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CNtlSession::CNtlSession(SESSIONTYPE sessionType)
:
m_sessionType( sessionType )
{
	Init();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CNtlSession::~CNtlSession()
{
	Destroy();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlSession::Init()
{
	m_hSession = INVALID_HSESSION;

	m_dwReferenceCount = 1;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlSession::Destroy()
{
	if( IsConnected() )
	{
		NTL_LOG( LOG_TRAFFIC, "%u,%s,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%d,%d",
									GetSessionType(), GetRemoteIP(), GetRemotePort(),
									GetTickCount() - GetConnectTime(),
									GetBytesTotalSize(),
									GetBytesRecvSize(),
									GetBytesRecvSizeMax(),
									GetBytesSendSize(),
									GetBytesSendSizeMax(),
									GetPacktTotalCount(),
									GetPacketRecvCount(),
									GetPacketSendCount(),
									GetRecvQueueMaxUseSize(),
									GetSendQueueMaxUseSize(),
									GetRecvBuffer()->GetQueueLoopCount(),
									GetSendBuffer()->GetQueueLoopCount() );
	}
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlSession::Acquire()
{
	InterlockedIncrement( (LONG*)&m_dwReferenceCount );
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlSession::Release()
{
	if( 0 == InterlockedDecrement( (LONG*)&m_dwReferenceCount ) )
	{
		delete this;
	}
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CNtlSession::IsInternalConnection(char* pIp)
{
#if defined(NTL_INTERNAL_ADDRESS_PREFIX)
	#undef NTL_INTERNAL_ADDRESS_PREFIX
#endif

#define NTL_INTERNAL_ADDRESS_PREFIX		"10.0.0."
	if (NULL == pIp)
	{
		return false;
	}

	if (0 == strncmp(pIp, NTL_INTERNAL_ADDRESS_PREFIX, strlen(NTL_INTERNAL_ADDRESS_PREFIX)))
	{
		return true;
	}
	else
	{
		return false;
	}
#undef NTL_INTERNAL_ADDRESS_PREFIX
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
int CNtlSession::OnDispatch(CNtlPacket * pPacket)
{
	PACKETDATA * pPacketData = (PACKETDATA *) pPacket->GetPacketData();

	switch( pPacketData->wOpCode )
	{
	case SYS_ALIVE:
		{
			ResetAliveTime();
		}
		break;

	case SYS_PING:
		break;

	default:
		NTL_DBGREPORT( "Session[%s:%X] Type[%u] Send Wrong Packet[%u]", typeid(this), this, m_sessionType, pPacketData->wOpCode );
		return NTL_ERR_NET_PACKET_OPCODE_WRONG;
	}

	return NTL_SUCCESS;
}
