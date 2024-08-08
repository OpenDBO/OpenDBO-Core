//***********************************************************************************
//
//	File		:	NtlDatabaseManager.cpp
//
//	Begin		:	2006-02-17
//
//	Copyright	:	¨Ï NTL-Inc Co., Ltd
//
//	Author		:	Hyun Woo, Koo   ( zeroera@ntl-inc.com )
//
//	Desc		:	Database Manager class Implementation
//
//***********************************************************************************

#include "stdafx.h"
#include "NtlDatabaseManager.h"
#include "NtlDatabase.h"
#include "NtlDatabaseConnection.h"
#include "NtlQueryTask.h"
#include "NtlSqlUnitHelper.h"

#include "NtlLog.h"

#include "NtlError.h"


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CNtlDatabaseManager::CNtlDatabaseManager()
{
	Init();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CNtlDatabaseManager::~CNtlDatabaseManager()
{
	Destroy();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlDatabaseManager::Init()
{
	m_pAppThread = NULL;

	m_dwDatabaseID = 1;

	m_hEnv = SQL_NULL_HANDLE;

	m_pSqlUnitHelper = NULL;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
int CNtlDatabaseManager::Create(CNtlSqlUnitHelper * pSqlUnitHelper, int nThreadCount)
{
	int rc = NTL_SUCCESS;


	rc = SQLAllocHandle( SQL_HANDLE_ENV, SQL_NULL_HANDLE, &m_hEnv );	
	if( TSQL_FAIL( rc ) )
	{
		Destroy();
		return NTL_ERR_DBC_ENVIRONMENT_ALLOC_FAIL;
	}


	rc = SQLSetEnvAttr( m_hEnv, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0 ); 
	if( TSQL_FAIL( rc ) )
	{	
		Destroy();
		return NTL_ERR_DBC_ENVIRONMENT_SETATT_FAIL;
	}


	//rc = m_threadPool.Create( CNtlThreadPool::POOL_USER_MODE, nThreadCount );
	rc = m_threadPool.Create( CNtlThreadPool::POOL_KERNEL_MODE, nThreadCount );
	if( NTL_SUCCESS != rc )
	{
		return rc;
	}


	m_threadPool.Start();


	m_pSqlUnitHelper = pSqlUnitHelper;


	return NTL_SUCCESS;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlDatabaseManager::Destroy()
{
	m_threadPool.Shutdown();


	for( DATABASEIT it = m_databaseList.begin(); it != m_databaseList.end(); it++ )
	{
		SAFE_DELETE( it->second );
	}
	m_databaseList.clear();


	if( m_hEnv )
	{
		SQLFreeHandle( SQL_HANDLE_ENV, m_hEnv );

		m_hEnv = SQL_NULL_HANDLE;
	}


	m_pSqlUnitHelper = NULL;
}

//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlDatabaseManager::Start()
{
	m_pAppThread = CNtlThreadFactory::CreateThread(this, "NtlDatabaseManagerThread", false); // use daemon thread
	m_pAppThread->Start();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlDatabaseManager::Stop()
{
	SetRunnable( false );
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlDatabaseManager::WaitForTerminate()
{
	if (NULL == m_pAppThread)
	{
		NTL_LOG_ASSERT("NULL == m_pAppThread");
	}
	else
	{
		m_pAppThread->Join();
	}
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
int CNtlDatabaseManager::Open(LPCTSTR lpszDSN, LPCTSTR lpszUserID, LPCTSTR lpszUserPW, HDATABASE * phDB, int nConnectionCount)
{
	int rc = NTL_SUCCESS;

	CNtlDatabase * pDatabase = new CNtlDatabase;
	if( NULL == pDatabase )
	{
		return NTL_ERR_SYS_MEMORY_ALLOC_FAIL;
	}

	rc = pDatabase->Create( this, m_pSqlUnitHelper->GetMaxSqlUnitID(), m_hEnv, lpszDSN, lpszUserID, lpszUserPW, nConnectionCount );
	if( NTL_SUCCESS != rc )
	{
		SAFE_DELETE( pDatabase );
		return rc;
	}


	HDATABASE hDB = AddDatabase( pDatabase );
	if( INVALID_HDATABASE == hDB )
	{
		SAFE_DELETE( pDatabase );
		return NTL_ERR_SYS_MEMORY_ALLOC_FAIL;
	}


	*phDB = hDB;


	return NTL_SUCCESS;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlDatabaseManager::Close(HDATABASE hDB)
{
	UNREFERENCED_PARAMETER( hDB );
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
int CNtlDatabaseManager::Query(HDATABASE hDB, CNtlQuery * pQuery)
{
	CNtlDatabase * pDatabase = FindDatabase( hDB );
	if( NULL == pDatabase )
	{
		return NTL_ERR_DBC_DATABASE_FIND_FAIL;
	}


	pDatabase->Query( pQuery );

	return NTL_SUCCESS;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
HDATABASE CNtlDatabaseManager::AddDatabase(CNtlDatabase * pDatabase)
{
	CNtlAutoMutex mutex( &m_listMutex );
	mutex.Lock();

	if( false == m_databaseList.insert( DATABASEVAL( m_dwDatabaseID, pDatabase ) ).second )
	{
		return INVALID_HDATABASE;
	}

	return m_dwDatabaseID++;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlDatabaseManager::RemoveDatabase(HDATABASE hDB)
{
	CNtlAutoMutex mutex( &m_listMutex );
	mutex.Lock();

	DATABASEIT it = m_databaseList.find( hDB );
	if( it ==  m_databaseList.end() )
	{
		return;
	}


	SAFE_DELETE( it->second );

	m_databaseList.erase( it );
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CNtlDatabase * CNtlDatabaseManager::FindDatabase(HDATABASE hDB)
{
	CNtlAutoMutex mutex( &m_listMutex );
	mutex.Lock();

	DATABASEIT it = m_databaseList.find( hDB );
	if( it ==  m_databaseList.end() )
	{
		return NULL;
	}

	return it->second;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlDatabaseManager::Run()
{
	while( IsRunnable() )
	{
		m_listMutex.Lock();

		for( DATABASEIT it = m_databaseList.begin(); it != m_databaseList.end(); it++ )
		{
			it->second->Proc();
		}

		m_listMutex.Unlock();

		Wait( 1 );
	}
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CNtlDatabaseManager::RunQuery(CNtlQuery * pQuery, CNtlDatabaseConnection * pDatabaseConnection)
{
	m_threadPool.QueueWorkItem( new CNtlQueryTask( pQuery, pDatabaseConnection ) );

	return true;
}

//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CNtlDatabaseManager::ResetCount(HDATABASE hDB)
{
	CNtlDatabase * pDatabase = FindDatabase( hDB );
	if( NULL == pDatabase )
	{
		return false;
	}


	pDatabase->ResetCount();

	return true;
}

//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
DWORD CNtlDatabaseManager::GetRecvQueryCount(HDATABASE hDB)
{
	CNtlDatabase * pDatabase = FindDatabase( hDB );
	if( NULL == pDatabase )
	{
		return 0;
	}

	return pDatabase->GetRecvQueryCount();
}

//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
DWORD CNtlDatabaseManager::GetEndQueryCount(HDATABASE hDB)
{
	CNtlDatabase * pDatabase = FindDatabase( hDB );
	if( NULL == pDatabase )
	{
		return 0;
	}

	return pDatabase->GetEndQueryCount();
}

//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
DWORD CNtlDatabaseManager::GetCurQueryCount(HDATABASE hDB)
{
	CNtlDatabase * pDatabase = FindDatabase( hDB );
	if( NULL == pDatabase )
	{
		return 0;
	}

	return pDatabase->GetCurQueryCount();
}
