//***********************************************************************************
//
//	File		:	NtlNetwork.h
//
//	Begin		:	2005-12-13
//
//	Copyright	:	ⓒ NTL-Inc Co., Ltd
//
//	Author		:	Hyun Woo, Koo   ( zeroera@ntl-inc.com )
//
//	Desc		:	네트워크 메인 클래스
//
//***********************************************************************************

#pragma once


#include "NtlIOCP.h"
#include "NtlSessionList.h"

#include <map>


//---------------------------------------------------------------------------------------
//	NETWORK EVENT 
//---------------------------------------------------------------------------------------
enum eNETEVENT
{
	NETEVENT_ACCEPT = 1,
	NETEVENT_CONNECT,
	NETEVENT_CLOSE,
	NETEVENT_RECV,

	INVALID_NETEVENT,
	NETEVENT_COUNT,
};
//---------------------------------------------------------------------------------------



class CNtlConnection;
class CNtlSession;
class CNtlAcceptor;
class CNtlConnector;
class CNtlSessionFactory;
class CNtlNetworkMonitor;
class CNtlNetworkProcessor;
class CNtlPacket;


class CNtlNetwork
{
	friend class CNtlNetworkMonitor;
	friend class CNtlNetworkProcessor;

	typedef std::multimap<SESSIONTYPE, CNtlAcceptor*> CNtlAcceptorList;
	typedef std::multimap<SESSIONTYPE, CNtlConnector*> CNtlConnectorList;


public:

	CNtlNetwork();

	virtual ~CNtlNetwork();


public:

	int								Create(	CNtlSessionFactory * pFactory,											
											int nSessionSize,
											int nCreateThreads = 0,
											int nConcurrentThreads = 0 );

	void							Destroy();


public:

	int								Associate(CNtlConnection * pConnection, bool bAssociate);

	int								Associate(CNtlAcceptor * pAcceptor, bool bAssociate);

	int								Associate(CNtlConnector * pConnector, bool bAssociate);

	int								Send(HSESSION hSession, CNtlPacket * pPacket);

	int								Send(CNtlSession * pSession, CNtlPacket * pPacket);

	int								PostNetEventMessage(WPARAM wParam, LPARAM lParam);



public:

	CNtlSessionList *				GetSessionList() { return m_pSessionList; }

	CNtlSessionFactory *			GetSessionFactory() { return m_pSessionFactoryRef; }

	CNtlAcceptor *					GetAcceptor(SESSIONTYPE sessionType, const char * lpszAddr, WORD wPort);

	CNtlConnector *					GetConnector(SESSIONTYPE sessionType, const char * lpszAddr, WORD wPort);


protected:

	void							Init();

	int								StartUp();

	int								Shutdown();

	int								CreateMonitorThread();

	int								CreateDispatcherThread();


private:

	CNtlIocp						m_iocp;

	CNtlSessionList *				m_pSessionList;

	CNtlAcceptorList *				m_pAcceptorList;

	CNtlConnectorList *				m_pConnectorList;

	CNtlSessionFactory *			m_pSessionFactoryRef;

	CNtlNetworkMonitor *			m_pNetworkMonitor;

	CNtlNetworkProcessor *			m_pNetworkProcessor;

};

