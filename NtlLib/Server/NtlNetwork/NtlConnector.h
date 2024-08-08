//***********************************************************************************
//
//	File		:	Connector.h
//
//	Begin		:	2005-12-19
//
//	Copyright	:	¨Ï NTL-Inc Co., Ltd
//
//	Author		:	Hyun Woo, Koo   ( zeroera@ntl-inc.com )
//
//	Desc		:	
//
//***********************************************************************************

#pragma once


#include "NtlIOCP.h"
#include "NtlSockAddr.h"
#include "NtlSocket.h"

#include "NtlMutex.h"


//#define __USE_CONNECTEX__


class CNtlSession;
class CNtlConnection;

class CNtlConnector
{
	friend class CConnectorThread;

public:

	CNtlConnector();

	virtual ~CNtlConnector();


public:

	int						Create(	LPCTSTR lpszConnectAddr,
									WORD wConnectPort,
									SESSIONTYPE sessionType,
									DWORD dwRetryTime = 1000,
									DWORD dwProcessTime = 1000);

	void					Destroy();


public:


	SESSIONTYPE				GetSessionType() { return m_sessionType; }

	CNtlSockAddr &			GetConnectAddr() { return m_connectAddr; }

	const char *			GetConnectIP() { return m_connectAddr.GetDottedAddr(); }

	WORD					GetConnectPort() { return m_connectAddr.GetPort(); }

	DWORD					GetTotalConnectCount() { return m_dwTotalConnectCount; }



public:

	int						OnAssociated(CNtlNetwork * pNetwork);

	void					OnConnected();

	void					OnDisconnected(bool bConnected);


protected:

	int						CreateThread();

	int						DoConnect();

	void					Init();


private:

	CNtlNetwork *			m_pNetwork;

	CNtlSockAddr			m_connectAddr;

	SESSIONTYPE				m_sessionType;

private:

	DWORD					m_dwRetryTime;

	DWORD					m_dwProcessTime;

	DWORD					m_dwTotalConnectCount;

	BOOL					m_bConnected;

private:

	CConnectorThread *		m_pThread;

	CNtlMutex				m_mutex;

};


//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
inline void	CNtlConnector::OnConnected()
{
	InterlockedExchange( (LONG*)&m_bConnected, TRUE );
	++m_dwTotalConnectCount;
}

inline void	CNtlConnector::OnDisconnected(bool bConnected)
{
	if( bConnected )
	{
		InterlockedExchange( (LONG*)&m_bConnected, FALSE );
	}
}
//-----------------------------------------------------------------------------------