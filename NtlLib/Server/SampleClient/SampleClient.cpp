// SampleClient.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "NtlSfx.h"
#include "Protocol.h"

#include "NtlPacketEncoder_RandKey.h"
#include "NtlFile.h"

#include <iostream>


//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
enum APP_LOG
{
	PRINT_APP = 2,
};


// 세션 타입 정의
enum SAMPLE_SESSION
{
	SESSION_SERVER,
};


// 샘플 클라이언트 설정
struct sCLIENTCONFIG
{
	CNtlString		strServerConnectAddr;
	WORD			wServerConnectPort;	
};


//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
const DWORD					MAX_NUMOF_SERVER = 10;
//-----------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------
//		서버 세션 클래스
//-----------------------------------------------------------------------------------
class CServerSession : public CNtlSession
{
public:
			
	CServerSession()
		:CNtlSession(SESSION_SERVER), m_pPacketEncoder( true )
	{
		SetControlFlag( CONTROL_FLAG_USE_SEND_QUEUE );
		SetPacketEncoder( &m_pPacketEncoder );
	}

	virtual ~CServerSession() { }

public:

	DWORD						GetMaxRecvPacketCount() { return 2; }

	DWORD						GetMaxSendPacketCount() { return 2; }

	DWORD						GetAliveCheckTime() { return 5000; }

	int							OnConnect();

	void						OnClose();

	int							OnDispatch(CNtlPacket * pPacket);

private:

	CNtlPacketEncoder_RandKey	m_pPacketEncoder;

};


//-----------------------------------------------------------------------------------
//		샘플 클라이언트 세션 팩토리
//-----------------------------------------------------------------------------------
class CSampleSessionFactory : public CNtlSessionFactory
{
public:

	// 세션이 생성될때 호출
	CNtlSession * CreateSession(SESSIONTYPE sessionType)
	{
		CNtlSession * pSession = NULL;
		switch( sessionType )
		{
		case SESSION_SERVER:
			{
				pSession = new CServerSession;
			}
			break;

		default:
			break;
		}

		return pSession;
	}
};


//-----------------------------------------------------------------------------------
//		샘플 클라이언트 어플리케이션
//-----------------------------------------------------------------------------------
class CSampleClient : public CNtlServerApp
{
public:

	enum STATE
	{
		STATE_CONNECTED,
		STATE_JOINED,
		STATE_DISCONNECTED,
	};


public:

	CSampleClient()
		:m_state(STATE_DISCONNECTED) 
	{}


public:

	int	OnInitApp()
	{
		m_nMaxSessionCount = MAX_NUMOF_SERVER;

		m_pSessionFactory =  new CSampleSessionFactory;
		if( NULL == m_pSessionFactory )
		{
			return NTL_ERR_SYS_MEMORY_ALLOC_FAIL;
		}

		m_pServerSession = NULL;


		return NTL_SUCCESS;
	}


	int	OnCreate()
	{
		int rc = 0;

		rc = m_serverConnector.Create( m_config.strServerConnectAddr, m_config.wServerConnectPort, SESSION_SERVER) ;
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


	void OnDestroy()
	{
	}


	int	OnCommandArgument(int argc, _TCHAR* argv[])
	{
		// 필요시 Argument 파싱
		return NTL_SUCCESS;
	}


	int	OnConfiguration(const char * lpszConfigFile)
	{
		CNtlIniFile file;

		int rc = file.Create( lpszConfigFile );
		if( NTL_SUCCESS != rc )
		{
			return rc;
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
		CNtlString strTemp;
		std::cout << "Client Try Connecting";

		while( IsRunnable() )
		{		
			if( IsState( STATE_DISCONNECTED) )
			{
				std::cout << ".";
				Wait(1000);
			}
			else
			{
				std::cout << "> ";
				getline( std::cin, strTemp );

				if( IsState( STATE_CONNECTED) )
				{
					if( !strTemp.compare("join") )
					{
						std::cout << "insert your id > ";
						getline( std::cin, m_strUserId );

						CNtlPacket packet( sizeof(mCS_USER_JOIN_REQ) );
						mCS_USER_JOIN_REQ * req = (mCS_USER_JOIN_REQ *) packet.GetPacketData();
						req->wProtocolID = CS_USER_JOIN_REQ;
						strncpy_s( (char*)req->abyUserID , MAX_SIZE_USER_ID, m_strUserId, m_strUserId.size() );

						CNtlSessionAutoPtr autoPtr( AcquireServerSession() );
						if( m_pServerSession )
						{
							m_pServerSession->PushPacket( &packet );
							NTL_PRINT(PRINT_APP, "Send join");
						}
					}
				}
				else
				{
					if( !strTemp.compare("exit") )
					{
						break;
					}
					else
					{
						CNtlPacket packet( sizeof(mCS_USER_CHAT) );
						mCS_USER_CHAT * req = (mCS_USER_CHAT *) packet.GetPacketData();
						req->wProtocolID = CS_USER_CHAT;

						req->byChatLen = (BYTE) strTemp.length();
						strncpy_s( (char*)req->abyUserID , MAX_SIZE_USER_ID, m_strUserId, m_strUserId.size() );
						strncpy_s( (char*)req->abyChatData , MAX_SIZE_CHAT_DATA, strTemp, req->byChatLen );

						CNtlSessionAutoPtr autoPtr( AcquireServerSession() );
						if( m_pServerSession )
						{
							for( int i = 0; i < 28; i++ )
							{
								m_pServerSession->PushPacket( &packet );

								Wait( 10 );
							}
							NTL_PRINT(PRINT_APP, "Send USER_CHAT");
						}
					}
				}
			}
		}
	}


	void						SetState(STATE state) { m_state = state; }
	bool						IsState(STATE state) { return m_state == state; }

	void						SetServerSession(CServerSession * pServerSession)
	{
		CNtlAutoMutex mutex( &m_serverMutex );
		mutex.Lock();

		if( NULL == pServerSession )
		{
			m_pServerSession->Release();
			m_pServerSession = pServerSession;	
		}
		else
		{
			pServerSession->Acquire();
			m_pServerSession = pServerSession;
		}
	}

	CServerSession *			AcquireServerSession()
	{
		CNtlAutoMutex mutex( &m_serverMutex );
		mutex.Lock();

		if( NULL == m_pServerSession ) return NULL;

		m_pServerSession->Acquire();
		return m_pServerSession;
	}


private:


	CNtlConnector				m_serverConnector;

	CNtlMutex					m_serverMutex;

	CServerSession *			m_pServerSession;

	CNtlLog  					m_log;

	sCLIENTCONFIG				m_config;

	STATE						m_state;

	CNtlString					m_strUserId;

};


//-----------------------------------------------------------------------------------
//		샘플 서버 세션 디스패치
//-----------------------------------------------------------------------------------
int CServerSession::OnConnect()
{
	NTL_PRINT( PRINT_APP, "%s", __FUNCTION__ );

	CSampleClient * app = (CSampleClient*) NtlSfxGetApp();
	app->SetServerSession( this );
	app->SetState( CSampleClient::STATE_CONNECTED );

	CNtlPacket packet( sizeof(mSYSTEM_ALIVE) );
	mSYSTEM_ALIVE * msg = (mSYSTEM_ALIVE *) packet.GetPacketData();
	msg->wProtocolID = SYSTEM_ALIVE;	

	PushPacket( &packet );

	return NTL_SUCCESS;
}


void CServerSession::OnClose()
{
	NTL_PRINT( PRINT_APP, "%s", __FUNCTION__ );

	CSampleClient * app = (CSampleClient*) NtlSfxGetApp();
	app->SetServerSession( NULL );
	app->SetState( CSampleClient::STATE_DISCONNECTED );
}


int CServerSession::OnDispatch(CNtlPacket * pPacket)
{
	CSampleClient * app = (CSampleClient*) NtlSfxGetApp();

	mSAMPLE_PROTOCOL_HEADER * pHeader = (mSAMPLE_PROTOCOL_HEADER *) pPacket->GetPacketData();
	switch( pHeader->wProtocolID )
	{
	case CS_USER_JOIN_RES:
		{
			mCS_USER_JOIN_RES * res = (mCS_USER_JOIN_RES *) pPacket->GetPacketData();

			if( CS_RESULT_NO == res->byResultCode )
			{
				std::cout << "RECV : server join fail!!" << std::endl;
			}
			else
			{
				std::cout << "RECV : server join success;" << std::endl;
				app->SetState( CSampleClient::STATE_JOINED );
			}
		}
		break;

	case CS_USER_CHAT:
		{
			mCS_USER_CHAT * req = (mCS_USER_CHAT *) pPacket->GetPacketData();

			req->abyUserID[MAX_SIZE_USER_ID] = 0x00;

			std::cout << "RECV : [CHAT] " << req->abyUserID << " : " << req->abyChatData << std::endl;
		}
		break;

	default:
		return CNtlSession::OnDispatch( pPacket );
	}


	return NTL_SUCCESS;
}


//-----------------------------------------------------------------------------------
//		샘플 클라이언트 메인
//-----------------------------------------------------------------------------------
int _tmain(int argc, _TCHAR* argv[])
{
	CNtlFileStream traceFileStream;

	int rc = traceFileStream.Create( "trace" );
	if( NTL_SUCCESS != rc )
	{
		printf( "log file CreateFile error %d(%s)", rc, NtlGetErrorMessage( rc ) );
		return rc;
	}


	NtlSetPrintStream( traceFileStream.GetFilePtr() );
	::NtlSetPrintFlag( PRINT_APP | PRINT_SYSTEM );

	CSampleClient app;

	rc = app.Create(argc, argv, ".\\SampleClient.ini" );
	if( NTL_SUCCESS != rc )
	{
		NTL_PRINT(PRINT_SYSTEM, "Server Application Create Fail %d(%s)", rc, NtlGetErrorMessage(rc) );
		return rc;
	}

	app.Start();

	app.WaitForTerminate();


	return 0;
}

