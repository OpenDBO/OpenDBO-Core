// SampleServer.cpp : Defines the entry point for the console application.
//


#include "stdafx.h"

#include "Protocol.h"

#include "NtlSfx.h"
#include "NtlSfxDB.h"

#include "NtlPacketEncoder_RandKey.h"
#include "NtlFile.h"

#include <iostream>
#include <map>
#include <list>


//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
enum APP_LOG
{
	PRINT_APP = 2,
};


using namespace std;



// 세션 타입 정의
enum SAMPLE_SESSION
{
	SESSION_CLIENT,
	SESSION_SERVER_ACTIVE,
	SESSION_SERVER_PASSIVE,
};


//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
const DWORD					MAX_NUMOF_GAME_CLIENT= 3;
const DWORD					MAX_NUMOF_SERVER = 1;
const DWORD					MAX_NUMOF_SESSION = MAX_NUMOF_GAME_CLIENT + MAX_NUMOF_SERVER;
//-----------------------------------------------------------------------------------

// 샘플 서버 설정
struct sSERVERCONFIG
{
	CNtlString		strClientAcceptAddr;
	WORD			wClientAcceptPort;

	CNtlString		strServerAcceptAddr;
	WORD			wServerAcceptPort;

	CNtlString		strServerConnectAddr;
	WORD			wServerConnectPort;
};


//-----------------------------------------------------------------------------------
//		접속되는 클라이언트 세션 클래스
//-----------------------------------------------------------------------------------
class CClientSession : public CNtlSession
{
public:

	CClientSession(bool bAliveCheck = false, bool bOpcodeCheck = false)
		:CNtlSession( SESSION_CLIENT )
	{
		SetControlFlag( CONTROL_FLAG_USE_SEND_QUEUE );

		if( bAliveCheck )
		{
			SetControlFlag( CONTROL_FLAG_CHECK_ALIVE );
		}

		if( bOpcodeCheck )
		{
			SetControlFlag( CONTROL_FLAG_CHECK_OPCODE );
		}

		SetPacketEncoder( &m_packetEncoder );
	}

	~CClientSession();


public:

	DWORD						GetMaxRecvPacketCount() { return 2; }

	DWORD						GetMaxSendPacketCount() { return 2; }

	DWORD						GetAliveCheckTime() { return 5000; }

	int							OnAccept();

	void						OnClose();

	int							OnDispatch(CNtlPacket * pPacket);


private:

	CNtlString					m_strUserID;

	CNtlPacketEncoder_RandKey	m_packetEncoder;

};


//-----------------------------------------------------------------------------------
//		접속되는 서버 세션 클래스
//-----------------------------------------------------------------------------------
class CServerPassiveSession : public CNtlSession
{
public:

	CServerPassiveSession()
		:CNtlSession(SESSION_SERVER_PASSIVE)
	{
	}

	~CServerPassiveSession() {}


public:

	int							OnAccept();

	void						OnClose();

	int							OnDispatch(CNtlPacket * pPacket);

};


//-----------------------------------------------------------------------------------
//		접속하는 클라이언트 세션 클래스
//-----------------------------------------------------------------------------------
class CServerActiveSession : public CNtlSession
{
public:

	CServerActiveSession()
		:CNtlSession(SESSION_SERVER_ACTIVE) {}

	~CServerActiveSession() {}

public:

	int							OnConnect();

	void						OnClose();

	int							OnDispatch(CNtlPacket * pPacket);

};


//-----------------------------------------------------------------------------------
//		샘플 서버 세션 팩토리
//-----------------------------------------------------------------------------------
class CSampleSessionFactory : public CNtlSessionFactory
{
public:

	CNtlSession * CreateSession(SESSIONTYPE sessionType)
	{
		CNtlSession * pSession = NULL;
		switch( sessionType )
		{
		case SESSION_CLIENT: 
			{
				pSession = new CClientSession;
			}
			break;

		case SESSION_SERVER_ACTIVE:
			{
				pSession = new CServerActiveSession;
			}
			break;

		case SESSION_SERVER_PASSIVE:
			{
				pSession = new CServerPassiveSession;
			}
			break;

		default:
			break;
		}

		return pSession;
	}
};



//-----------------------------------------------------------------------------------
//		샘플 서버 어플리케이션
//-----------------------------------------------------------------------------------
class CSampleServer : public CNtlServerApp
{
public:

	int					OnInitApp()
	{
		m_nMaxSessionCount = MAX_NUMOF_SESSION;

		m_pSessionFactory =  new CSampleSessionFactory;
		if( NULL == m_pSessionFactory )
		{
			return NTL_ERR_SYS_MEMORY_ALLOC_FAIL;
		}



		return NTL_SUCCESS;
	}


	int					OnCreate()
	{
		int rc = NTL_SUCCESS;

		rc = m_clientAcceptor.Create(	m_config.strClientAcceptAddr, m_config.wClientAcceptPort, SESSION_CLIENT, 
										MAX_NUMOF_GAME_CLIENT, 5, 2, MAX_NUMOF_GAME_CLIENT );
		if ( NTL_SUCCESS != rc )
		{
			return rc;
		}

		rc = m_serverAcceptor.Create(	m_config.strServerAcceptAddr, m_config.wServerAcceptPort, SESSION_SERVER_PASSIVE,
										MAX_NUMOF_SERVER, 5, 2, MAX_NUMOF_SERVER);
		if( NTL_SUCCESS != rc ) 
		{
			return rc;
		}

		rc = m_serverConnector.Create( m_config.strServerConnectAddr, m_config.wServerConnectPort, SESSION_SERVER_ACTIVE );
		if( NTL_SUCCESS != rc )
		{
			return rc;
		}


		rc = m_network.Associate( &m_clientAcceptor, true );
		if( NTL_SUCCESS != rc )
		{
			return rc;
		}

		rc = m_network.Associate( &m_serverAcceptor, true );
		if( NTL_SUCCESS != rc )
		{
			return rc;
		}

		rc = m_network.Associate( &m_serverConnector, true );
		if( NTL_SUCCESS != rc )
		{
			return rc;
		}


		return NTL_SUCCESS;

	}


	void				OnDestroy()
	{
	}


	int					OnCommandArgument(int argc, _TCHAR* argv[])
	{
		// 필요시 Argument 파싱
		return NTL_SUCCESS;
	}

	
	int					OnConfiguration(const char * lpszConfigFile)
	{
		CNtlIniFile file;

		int rc = file.Create( lpszConfigFile );
		if( NTL_SUCCESS != rc )
		{
			return rc;
		}


		if( !file.Read("Client Accept", "Address", m_config.strClientAcceptAddr) )
		{
			return NTL_ERR_SYS_CONFIG_FILE_READ_FAIL;
		}


		if( !file.Read("Client Accept", "Port",  m_config.wClientAcceptPort) )
		{
			return NTL_ERR_SYS_CONFIG_FILE_READ_FAIL;
		}


		if( !file.Read("Server Accept", "Address", m_config.strServerAcceptAddr) )
		{
			return NTL_ERR_SYS_CONFIG_FILE_READ_FAIL;
		}


		if( !file.Read("Server Accept", "Port",  m_config.wServerAcceptPort) )
		{
			return NTL_ERR_SYS_CONFIG_FILE_READ_FAIL;
		}


		if( !file.Read("Server Connect", "Address", m_config.strServerConnectAddr) )
		{
			return NTL_ERR_SYS_CONFIG_FILE_READ_FAIL;
		}


		if( !file.Read("Server Connect", "Port",  m_config.wServerConnectPort) )
		{
			return NTL_ERR_SYS_CONFIG_FILE_READ_FAIL;
		}


		return NTL_SUCCESS;
	}


	int					OnAppStart()
	{
		return NTL_SUCCESS;
	}


	void				Run()
	{
		DWORD dwTickCur, dwTickOld = ::GetTickCount();

		while( IsRunnable() )
		{		
			dwTickCur = ::GetTickCount();
			if( dwTickCur - dwTickOld >= 10000 )
			{
				NTL_PRINT(PRINT_APP, "SampleServer Run()");
				dwTickOld = dwTickCur;
			}		
		}
	}



private:


	CNtlAcceptor				m_clientAcceptor;

	CNtlAcceptor				m_serverAcceptor;

	CNtlConnector				m_serverConnector;

	CNtlLog  					m_log;

	sSERVERCONFIG				m_config;



public:

	bool						AddUser(const char * lpszUserID, CClientSession * pSession)
	{
		if( false == m_userList.insert( USERVAL(CNtlString(lpszUserID), pSession)).second )
		{
			return false;
		}

		return true;		
	}

	void						RemoveUser(const char * lpszUserID)
	{
		m_userList.erase( CNtlString(lpszUserID) );
	}

	bool						FindUser(const char * lpszUserID)
	{
		USERIT it = m_userList.find( CNtlString(lpszUserID) );
		if( it == m_userList.end() )
			return false;

		return true;
	}

	void						UserBroadcast(CNtlPacket * pPacket)
	{
		for( USERIT it = m_userList.begin(); it != m_userList.end(); it++ )
		{
			it->second->PushPacket( pPacket );
		}
	}


	bool						AddServer(CServerPassiveSession * pSession)
	{
		if( FindServer( pSession ) )
			return false;

		m_serverList.push_back( pSession );

		return true;
	}

	bool						FindServer(CServerPassiveSession * pSession)
	{
		for( SERVERIT it = m_serverList.begin(); it != m_serverList.end(); it++ )
		{
			if( pSession == *it )
			{
				return true;
			}
		}

		return false;
	}

	void						RemoveServer(CServerPassiveSession * pSession)
	{
		for( SERVERIT it = m_serverList.begin(); it != m_serverList.end(); it++ )
		{
			if( pSession == *it )
			{
				m_serverList.erase( it );
				break;
			}
		}
	}


	void						ServerBroadcast(CNtlPacket * pPacket)
	{
		for( SERVERIT it = m_serverList.begin(); it != m_serverList.end(); it++ )
		{
			(*it)->PushPacket( pPacket );
		}
	}


private:

	typedef std::map<CNtlString, CClientSession*> USERLIST;
	typedef USERLIST::value_type USERVAL;
	typedef USERLIST::iterator USERIT;

	typedef std::list<CServerPassiveSession*> SERVERLIST;
	typedef SERVERLIST::iterator  SERVERIT;

	USERLIST					m_userList;
	SERVERLIST					m_serverList;

};


//-----------------------------------------------------------------------------------
//		클라이언트
//-----------------------------------------------------------------------------------
CClientSession::~CClientSession()
{
	NTL_PRINT(PRINT_APP, "CClientSession Destructor Called");
}


int CClientSession::OnAccept()
{
	NTL_PRINT( PRINT_APP, "%s", __FUNCTION__ );

	return NTL_SUCCESS;
}


void CClientSession::OnClose()
{
	NTL_PRINT( PRINT_APP, "%s", __FUNCTION__ );

	CSampleServer * app = (CSampleServer*) NtlSfxGetApp();
	app->RemoveUser( m_strUserID );
}


int CClientSession::OnDispatch(CNtlPacket * pPacket)
{
	CSampleServer * app = (CSampleServer*) NtlSfxGetApp();

	mSAMPLE_PROTOCOL_HEADER * pHeader = (mSAMPLE_PROTOCOL_HEADER *) pPacket->GetPacketData();
	switch( pHeader->wProtocolID )
	{
	case CS_USER_JOIN_REQ:
		{
			NTL_PRINT(PRINT_APP, "Join Request");

			CNtlPacket packet( sizeof(mCS_USER_JOIN_RES) );

			mCS_USER_JOIN_REQ * req = (mCS_USER_JOIN_REQ *) pPacket->GetPacketData();
			mCS_USER_JOIN_RES * res = (mCS_USER_JOIN_RES *) packet.GetPacketData();

			req->abyUserID[MAX_SIZE_USER_ID] = 0x00;

			res->wProtocolID = CS_USER_JOIN_RES;

			if( false == app->AddUser( (const char*) req->abyUserID, this ) )
			{
				NTL_PRINT(PRINT_APP, "User[%s] Join Fail", req->abyUserID );
				res->byResultCode = CS_RESULT_NO;
			}
			else	
			{
				NTL_PRINT(PRINT_APP, "User[%s] Join Success", req->abyUserID );
				m_strUserID = (const char*)req->abyUserID;
				res->byResultCode = CS_RESULT_OK;
			}

			packet.SetPacketLen( sizeof(mCS_USER_JOIN_RES) );

			int rc = g_pApp->Send( this->GetHandle(), &packet );
			if( NTL_SUCCESS != rc )
			{
				NTL_PRINT(PRINT_APP, "Send CS_USER_JOIN_RES Fail %d(%s)", rc, NtlGetErrorMessage(rc) );
			}
		}
		break;


	case CS_USER_CHAT:
		{
			mCS_USER_CHAT * req = (mCS_USER_CHAT *) pPacket->GetPacketData();

			req->abyUserID[MAX_SIZE_USER_ID] = 0x00;
			if( false == app->FindUser((char*)req->abyUserID) )
			{
				NTL_PRINT(PRINT_APP, "User Not Found");
			}
			else
			{
				NTL_PRINT(PRINT_APP, "User[%s] Chat[%s]", req->abyUserID, req->abyChatData);
			}

			CNtlPacket packet( *pPacket );
			
			this->PushPacket( &packet );
			app->ServerBroadcast( &packet );
		}
		break;

	default:
		return CNtlSession::OnDispatch( pPacket );
	}

	return NTL_SUCCESS;
}


//-----------------------------------------------------------------------------------
//		자신에게 접속해온 서버세션 ( Passive )
//-----------------------------------------------------------------------------------
int CServerPassiveSession::OnAccept()
{
	NTL_PRINT( PRINT_APP, "%s", __FUNCTION__ );

	return NTL_SUCCESS;
}


void CServerPassiveSession::OnClose()
{
	NTL_PRINT( PRINT_APP, "%s", __FUNCTION__ );

	CSampleServer * app = (CSampleServer*) NtlSfxGetApp();
	app->RemoveServer( this );
}


int CServerPassiveSession::OnDispatch(CNtlPacket * pPacket)
{
	CSampleServer * app = (CSampleServer*) NtlSfxGetApp();

	mSAMPLE_PROTOCOL_HEADER * pHeader = (mSAMPLE_PROTOCOL_HEADER *) pPacket->GetPacketData();
	switch( pHeader->wProtocolID )
	{
	case SS_SERVER_REGISTER_NFY:
		{
			app->AddServer( this );
		}
		break;

	case SS_USER_CHAT_NFY:
		{
			NTL_PRINT(PRINT_APP, "PASSIVE SS_USER_CHAT_NFY received");
		}
		break;

	default:
		return CNtlSession::OnDispatch( pPacket );
	}

	return NTL_SUCCESS;
}


//-----------------------------------------------------------------------------------
//		자신이 접속한 서버 세션 ( Active )
//-----------------------------------------------------------------------------------
int CServerActiveSession::OnConnect()
{
	NTL_PRINT( PRINT_APP, "%s", __FUNCTION__ );

	CNtlPacket sendPacket( sizeof(mSS_SERVER_REGISTER_NFY) );
	mSS_SERVER_REGISTER_NFY * msg = (mSS_SERVER_REGISTER_NFY *) sendPacket.GetPacketData();
	msg->wProtocolID = SS_SERVER_REGISTER_NFY;	

	PushPacket( &sendPacket );

	return NTL_SUCCESS;
}


void CServerActiveSession::OnClose()
{
	NTL_PRINT( PRINT_APP, "%s", __FUNCTION__ );
}


int CServerActiveSession::OnDispatch(CNtlPacket * pPacket)
{
	CSampleServer * app = (CSampleServer*) NtlSfxGetApp();

	mSAMPLE_PROTOCOL_HEADER * pHeader = (mSAMPLE_PROTOCOL_HEADER *) pPacket->GetPacketData();
	switch( pHeader->wProtocolID )
	{
	case CS_USER_CHAT:
		{
			app->UserBroadcast( pPacket );
		}
		break;

	default:
		return CNtlSession::OnDispatch( pPacket );
	}

	return NTL_SUCCESS;
}


//-----------------------------------------------------------------------------------
//		샘플 서버 메인
//-----------------------------------------------------------------------------------
int SampleServerMain(int argc, _TCHAR* argv[])
{
	CNtlFileStream traceFileStream;

	int rc = traceFileStream.Create( "trace" );
	if( NTL_SUCCESS != rc )
	{
		printf( "log file CreateFile error %d(%s)", rc, NtlGetErrorMessage( rc ) );
		return rc;
	}


	NtlSetPrintStream( traceFileStream.GetFilePtr() );
	NtlSetPrintFlag( PRINT_APP | PRINT_SYSTEM );

	CSampleServer app;

	rc = app.Create(argc, argv, ".\\SampleServer.ini");
	if( NTL_SUCCESS != rc )
	{
		NTL_PRINT(PRINT_APP, "Server Application Create Fail %d(%s)", rc, NtlGetErrorMessage(rc) );
		return rc;
	}

	app.Start();

	app.WaitForTerminate();

	return 0;
}
