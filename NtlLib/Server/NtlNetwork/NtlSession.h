//***********************************************************************************
//
//	File		:	NtlSession.h
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

#pragma once


#include "NtlConnection.h"


// Session delete macro
#define RELEASE_SESSION(p)			if(p) { p->Release(); p = NULL; }


class CNtlSession : public CNtlConnection
{
	friend class CNtlSessionList;

public:

	CNtlSession(SESSIONTYPE sessionType);

	virtual ~CNtlSession();


public:

	virtual int						OnConnect() { return NTL_SUCCESS; }

	virtual int						OnAccept() { return NTL_SUCCESS; }

	virtual void					OnClose() {}

	virtual int						OnDispatch(CNtlPacket * pPacket);

	//int								Send(CNtlPacket * pPacket) { return CNtlConnection::PushPacket( pPacket ); }


public:

	HSESSION						GetHandle() { return m_hSession; }

	SESSIONTYPE						GetSessionType() { return m_sessionType; }


public:

	void							Acquire();

	void							Release();


public:

	static bool						IsInternalConnection(char* pIp);


protected:

	void							Init();

	void							Destroy();


private:

	CNtlSession&					operator=(CNtlSession&);


private:

	DWORD							m_dwReferenceCount;

	HSESSION						m_hSession;

	const SESSIONTYPE				m_sessionType;

};

class CNtlSessionAutoPtr
{

public:
	explicit CNtlSessionAutoPtr( CNtlSession * pSession ) 
		: m_pSession ( pSession ) 
	{
	}

	~CNtlSessionAutoPtr( ) 
	{ 
		RELEASE_SESSION( m_pSession );
	}

private:
	CNtlSession * m_pSession;

};//end of class ( CNtlSessionAutoPtr )