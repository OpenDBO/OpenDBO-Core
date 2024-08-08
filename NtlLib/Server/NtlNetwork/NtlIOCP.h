//***********************************************************************************
//
//	File		:	IOCP.h
//
//	Begin		:	2006-12-29
//
//	Copyright	:	¨Ï NTL-Inc Co., Ltd
//
//	Author		:	Hyun Woo, Koo   ( zeroera@ntl-inc.com )
//
//	Desc		:	
//
//***********************************************************************************

#pragma once

#include "NtlBase.h"


enum eIOMODE
{
	IOMODE_ACCEPT = 1,
	IOMODE_CONNECT,
	IOMODE_RECV,
	IOMODE_SEND,

	MAX_IOMODE
};


struct sIOCONTEXT : public OVERLAPPED
{
	WSABUF				wsabuf; // 
	eIOMODE				iomode;
	LPCVOID				param; // session ptr

	void Clear() { ZeroMemory( this, sizeof(sIOCONTEXT) ); }
	void Reset() { ZeroMemory( this, sizeof(OVERLAPPED) ); }
};


class CNtlNetwork;

class CNtlIocp
{
	friend class CIocpWorkerThread;

public:

	CNtlIocp();

	virtual ~CNtlIocp();

public:

	int							Create(CNtlNetwork * pNetwork, int nCreateThreads, int nConcurrentThreads);

	void						Destroy();


protected:

	int							CreateIOCP(int nConcurrentThreads);

	int							CreateThreads(int nOpenThreads);

	void						CloseThreads();


public:

	int							Associate(SOCKET hSock, LPCVOID pCompletionKey);


private:

	CNtlNetwork *					m_pNetworkRef;

	HANDLE						m_hIOCP;

	int							m_nCreatedThreads;

};

