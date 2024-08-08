//***********************************************************************************
//
//	File		:	NtlServerApp.h
//
//	Begin		:	2007-01-17
//
//	Copyright	:	¨Ï NTL-Inc Co., Ltd
//
//	Author		:	Hyun Woo, Koo   ( zeroera@ntl-inc.com )
//
//	Desc		:	
//
//***********************************************************************************

#pragma once


#include <tchar.h>

#include "NtlNetwork.h"

#include "NtlThread.h"
#include "NtlSingleton.h"
#include "NtlPerformance.h"

class CNtlSessionFactory;
class CNtlServerLog;

class CNtlServerApp : public CNtlRunObject, public CNtlSingleton<CNtlServerApp>
{
public:

	CNtlServerApp();

	virtual ~CNtlServerApp();


public:


// virtual 

	virtual int						OnInitApp();

	virtual int						OnCreate();

	virtual int						OnCommandArgument(int argc, _TCHAR* argv[]);

	virtual int						OnConfiguration(const char * lpszConfigFile);

	virtual int						OnAppStart();

	virtual void					OnRun();


public:


	int								Create(int argc, _TCHAR* argv[], const char * lpszConfigFile = NULL);

	void							Destroy();

	void							Start(bool bAutoDelete = false);

	void							Stop();

	void							WaitForTerminate();

	int								Send(HSESSION hSession, CNtlPacket * pPacket) { return m_network.Send(hSession, pPacket); }

	int								SendTo(CNtlSession * pSession, CNtlPacket * pPacket) { return m_network.Send(pSession, pPacket); }


	CNtlNetwork *					GetNetwork() { return &m_network; }

	CNtlPerformance *				GetPerformance() { return &m_performance; }

	CNtlServerLog *					GetServerLog() { return m_pServerLog; }

	void							SetConfigFileName(const char * lpszAppName, const char * lpszFileName);


protected:

	void							Init();


private:

	void							run() { this->OnRun(); }


protected:

	CNtlSessionFactory *			m_pSessionFactory;

	CNtlServerLog *					m_pServerLog;

	CNtlNetwork 					m_network;

	CNtlPerformance					m_performance;

	int								m_nMaxSessionCount;

	std::string						m_strConfigFile;


private:

	CNtlThread *					m_pAppThread;
};

#define NtlSfxGetApp()			CNtlServerApp::GetInstance()
#define g_pApp					NtlSfxGetApp()