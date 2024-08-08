//***********************************************************************************
//
//	File		:	ServerApp.cpp
//
//	Begin		:	2007-01-04
//
//	Copyright	:	ⓒ NTL-Inc Co., Ltd
//
//	Author		:	Hyun Woo, Koo   ( zeroera@ntl-inc.com )
//
//	Desc		:	
//
//***********************************************************************************

#include "stdafx.h"
#include "NtlServerApp.h"

#include "NtlSessionFactory.h"
#include "NtlServerLog.h"

#include "NtlString.h"
#include "NtlError.h"
#include "NtlMiniDump.h"


//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
CNtlMiniDump					s_miniDump;	// 프로그램 예외발생시의 메모리 덤프 오브젝트
//-----------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CNtlServerApp::CNtlServerApp()
{
	Init();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CNtlServerApp::~CNtlServerApp()
{
	Destroy();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlServerApp::Init()
{
	m_pSessionFactory = NULL;

	m_pServerLog = NULL;

	m_nMaxSessionCount = 0;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlServerApp::Destroy()
{
	CNtlThreadFactory::CloseAll();

	SAFE_DELETE( m_pSessionFactory );
	SAFE_DELETE( m_pServerLog );
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
int CNtlServerApp::Create(int argc, _TCHAR* argv[], const char * lpszConfigFile /* = NULL */)
{
	int rc = 0;

	SetConfigFileName(argv[0], lpszConfigFile);
	rc = CNtlServerApp::OnConfiguration( m_strConfigFile.c_str() );
	if( NTL_SUCCESS != rc )
	{
		return rc;
	}
	rc = OnConfiguration( m_strConfigFile.c_str() );
	if( NTL_SUCCESS !=  rc )
	{
		return rc;
	}


	rc = CNtlServerApp::OnCommandArgument( argc, argv );
	if( NTL_SUCCESS != rc )
	{
		return rc;
	}
	rc = OnCommandArgument( argc, argv );
	if( NTL_SUCCESS != rc )
	{
		return rc;
	}


	rc = CNtlServerApp::OnInitApp();
	if( NTL_SUCCESS != rc )
	{
		return rc;
	}
	rc = OnInitApp();
	if( NTL_SUCCESS != rc )
	{
		return rc;
	}


	rc = CNtlServerApp::OnCreate();
	if( NTL_SUCCESS != rc )
	{
		return rc;
	}
	rc = OnCreate();
	if( NTL_SUCCESS != rc )
	{
		return rc;
	}


	return NTL_SUCCESS;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
int	CNtlServerApp::OnInitApp()
{
	return NTL_SUCCESS;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
int	CNtlServerApp::OnCreate()
{
	int rc = m_performance.Create();
	if( NTL_SUCCESS != rc )
	{
		return rc;
	}


	rc = m_network.Create( m_pSessionFactory, m_nMaxSessionCount ); 
	if( NTL_SUCCESS != rc )
	{
		return rc;
	}


	return NTL_SUCCESS;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
int CNtlServerApp::OnCommandArgument(int argc, _TCHAR* argv[])
{
	UNREFERENCED_PARAMETER( argc );
	UNREFERENCED_PARAMETER( argv );

	return NTL_SUCCESS;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
int CNtlServerApp::OnConfiguration(const char * lpszConfigFile)
{
	UNREFERENCED_PARAMETER( lpszConfigFile );

	return NTL_SUCCESS;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
int CNtlServerApp::OnAppStart()
{
	return NTL_SUCCESS;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlServerApp::OnRun()
{
	while( IsRunnable() )
	{
		NTL_PRINT(PRINT_SYSTEM, "Run");
	}
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlServerApp::Start(bool bAutoDelete /* = false */)
{
	if (NULL == m_pSessionFactory)
	{
		NTL_LOG_ASSERT("NULL == m_pSessionFactory");
	}

	m_pAppThread = CNtlThreadFactory::CreateThread( this, "NtlSfxAppThread", bAutoDelete ); 
	m_pAppThread->Start();

	OnAppStart();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlServerApp::Stop()
{
	CNtlThreadFactory::CloseAll();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlServerApp::WaitForTerminate()
{
	CNtlThreadFactory::JoinAll();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlServerApp::SetConfigFileName(const char * lpszAppName, const char * lpszFileName)
{
	if( lpszFileName )
	{
		m_strConfigFile = lpszFileName;
	}
	else
	{
		std::string::size_type pos;

		m_strConfigFile = lpszAppName;

		pos = m_strConfigFile.rfind(".");
		m_strConfigFile.erase(pos);
		//pos = strConfigFile.rfind("\\");
		//strConfigFile.erase(0, pos + 1);

		m_strConfigFile += ".ini";
	}
}
