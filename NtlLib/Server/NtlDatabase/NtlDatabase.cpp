//***********************************************************************************
//
//	File		:	NtlDatabase.cpp
//
//	Begin		:	2006-02-17
//
//	Copyright	:	¨Ï NTL-Inc Co., Ltd
//
//	Author		:	Hyun Woo, Koo   ( zeroera@ntl-inc.com )
//
//	Desc		:	Database Class Implementation
//
//***********************************************************************************

#include "stdafx.h"

#include "NtlDatabase.h"
#include "NtlDatabaseManager.h"
#include "NtlDatabaseConnection.h"
#include "NtlQuery.h"
#include "NtlSqlUnitHelper.h"
#include "NtlError.h"

//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CNtlDatabase::CNtlDatabase()
{
	Init();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CNtlDatabase::~CNtlDatabase()
{
	Destroy();
}
//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
int CNtlDatabase::Create(CNtlDatabaseManager * pParent, SQLUNITID maxSqlUnitID, SQLHENV hEnv, LPCTSTR lpszDSN, LPCTSTR lpszUserID, LPCTSTR lpszUserPW, int nConnectionCount)
{
	int rc = NTL_SUCCESS;

	for( int i = 0; i < nConnectionCount; i++ )
	{
		CNtlDatabaseConnection * pConnection = new CNtlDatabaseConnection;
		if( NULL == pConnection )
		{
			return NTL_ERR_SYS_MEMORY_ALLOC_FAIL;
		}


		rc = pConnection->Create( this, pParent->GetSqlUnitHelper(), maxSqlUnitID, hEnv, lpszDSN, lpszUserID, lpszUserPW );
		if( NTL_SUCCESS != rc )
		{
			SAFE_DELETE( pConnection );
			return rc;
		}


		if( FALSE == AddConnection( pConnection ) )
		{
			SAFE_DELETE( pConnection );
			return NTL_FAIL;
		}
	}


	m_pParent = pParent;


	return NTL_SUCCESS;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlDatabase::Destroy()
{
	// Clear Database Connection
	m_listMutex.Lock();
	CNtlDatabaseConnection * pConnection = (CNtlDatabaseConnection *) m_connectionList.Pop();
	while( pConnection )
	{
		m_connectionList.Remove( pConnection );

		SAFE_DELETE( pConnection );

		pConnection = (CNtlDatabaseConnection *) m_connectionList.Pop();
	}
	m_listMutex.Unlock();


	// Clear Query
	CNtlQuery * pQuery = m_queryQueue.Pop();
	while( pQuery )
	{
		SAFE_DELETE( pQuery );

		pQuery = m_queryQueue.Pop();
	}
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
int CNtlDatabase::Query(CNtlQuery * pQuery)
{
	m_queryQueue.Push( pQuery );
	m_dwRecvQuery++;
	
	return NTL_SUCCESS;
}


//-----------------------------------------------------------------------------------
//		Purpose	:	
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlDatabase::Proc()
{
	CNtlQuery * pQuery = m_queryQueue.Peek();
	while( pQuery )
	{
		CNtlDatabaseConnection * pConnection = PopConnection();
		if( NULL == pConnection )
		{
			break;
		}


		if( false == m_pParent->RunQuery( pQuery, pConnection ) )
		{
			break;
		}


		m_queryQueue.Pop();
		m_dwEndQuery++;
		pQuery = m_queryQueue.Peek();
	}
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlDatabase::Init()
{
	m_pParent = NULL;
	ResetCount();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
BOOL CNtlDatabase::AddConnection(CNtlDatabaseConnection * pConnection)
{
	CNtlAutoMutex mutex( &m_listMutex );

	mutex.Lock();

	m_connectionList.Push( pConnection );

	return TRUE;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlDatabase::RemoveConnection(CNtlDatabaseConnection * pConnection)
{
	CNtlAutoMutex mutex( &m_listMutex );

	mutex.Lock();

	m_connectionList.Remove( pConnection );

	SAFE_DELETE( pConnection );
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CNtlDatabaseConnection * CNtlDatabase::PopConnection()
{
	CNtlAutoMutex mutex( &m_listMutex );

	mutex.Lock();

	return (CNtlDatabaseConnection *) m_connectionList.Pop( true );
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlDatabase::PushConnection(CNtlDatabaseConnection * pConnection)
{
	CNtlAutoMutex mutex( &m_listMutex );

	mutex.Lock();

	m_connectionList.Push( pConnection, true );
}

//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlDatabase::ResetCount()
{
	m_dwRecvQuery = 0;
	m_dwEndQuery = 0;
}

