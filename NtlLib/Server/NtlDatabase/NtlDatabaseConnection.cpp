//***********************************************************************************
//
//	File		:	NtlDatabaseConnection.cpp
//
//	Begin		:	2006-02-17
//
//	Copyright	:	¨Ï NTL-Inc Co., Ltd
//
//	Author		:	Hyun Woo, Koo   ( zeroera@ntl-inc.com )
//
//	Desc		:	Database Connection Class Implementation
//
//***********************************************************************************

#include "stdafx.h"

#include "NtlDatabaseConnection.h"
#include "NtlDatabase.h"
#include "NtlDatabaseManager.h"
#include "NtlQuery.h"
#include "NtlSqlUnitHelper.h"

#include "NtlError.h"


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CNtlDatabaseConnection::CNtlDatabaseConnection()
{
	Init();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CNtlDatabaseConnection::~CNtlDatabaseConnection()
{
	Destroy();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlDatabaseConnection::Init()
{
	m_pParent = NULL;

	m_hDbc = SQL_NULL_HANDLE;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
int	CNtlDatabaseConnection::Create(CNtlDatabase * pParent, CNtlSqlUnitHelper * pUnitHelper, SQLUNITID maxSqlUnitID, SQLHENV hEnv, LPCTSTR lpszDSN, LPCTSTR lpszUserID, LPCTSTR lpszUserPW)
{
	int rc = Open( pUnitHelper, maxSqlUnitID, hEnv, lpszDSN, lpszUserID, lpszUserPW);
	if( NTL_SUCCESS != rc )
	{
		return rc;
	}

	m_pParent = pParent;
	m_hEnv = hEnv;
	m_strDSN = lpszDSN;
	m_strUserID = lpszUserID;
	m_strUserPW = lpszUserPW;
	m_maxSqlUnitID = maxSqlUnitID;


	return NTL_SUCCESS;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlDatabaseConnection::Destroy()
{
	if( SQL_NULL_HANDLE != m_hDbc )
	{
		// free stmt

		SQLDisconnect( m_hDbc );
		SQLFreeHandle( SQL_HANDLE_DBC, m_hDbc );

		m_hDbc = SQL_NULL_HANDLE;
	}

	m_sqlUnitMap.Destroy();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
BOOL CNtlDatabaseConnection::RegisterSqlUnit(CNtlSqlUnit * pSqlUnit)
{
	return m_sqlUnitMap.Add( pSqlUnit );
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CNtlSqlUnit * CNtlDatabaseConnection::FindSqlUnit(SQLUNITID sqlUnitID)
{
	return m_sqlUnitMap.Find( sqlUnitID );
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CNtlDatabaseConnection::IsAlive()
{
	SQLINTEGER isDead = 0;
	int rc = SQLGetConnectAttr( m_hDbc, SQL_ATTR_CONNECTION_DEAD, &isDead, 0, NULL );
	if( TSQL_FAIL( rc ) )
	{
		return false;
	}

	if( SQL_CD_TRUE == isDead )
	{
		return false;
	}


	return true;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
int	CNtlDatabaseConnection::Reconnect()
{
	Destroy();

	return Open( m_pParent->GetParent()->GetSqlUnitHelper(), m_maxSqlUnitID, m_hEnv, m_strDSN.c_str(), m_strUserID.c_str(), m_strUserPW.c_str() );
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
int	CNtlDatabaseConnection::Open(CNtlSqlUnitHelper * pUnitHelper, SQLUNITID maxSqlUnitID, SQLHENV hEnv, LPCTSTR lpszDSN, LPCTSTR lpszUserID, LPCTSTR lpszUserPW)
{
	int rc = SQLAllocHandle( SQL_HANDLE_DBC, hEnv, &m_hDbc );
	if( TSQL_FAIL( rc ) )
	{
		Destroy();
		return NTL_ERR_DBC_CONNECTION_ALLOC_FAIL;
	}


	rc = SQLConnect( m_hDbc, (SQLCHAR*) lpszDSN, SQL_NTS, (SQLCHAR*) lpszUserID, SQL_NTS, (SQLCHAR*) lpszUserPW, SQL_NTS );
	if( TSQL_FAIL( rc ) )
	{
		Destroy();
		return NTL_ERR_DBC_CONNECTION_CONNECT_FAIL;
	}


	//SQLINTEGER connectionTimeout = 100;
	//rc = SQLGetConnectAttr( m_hDbc, SQL_ATTR_CONNECTION_TIMEOUT, &connectionTimeout, 0, NULL );
	//if( TSQL_FAIL( rc ) )
	//{
	//	return NTL_FAIL;
	//}


	if( FALSE == m_sqlUnitMap.Create( maxSqlUnitID ) )
	{
		return NTL_ERR_SYS_MEMORY_ALLOC_FAIL;
	}


	if( FALSE == pUnitHelper->PreCreateSqlUnit( this ) )
	{
		return NTL_ERR_DBC_DATABASE_SQL_PRECREATE_FAIL;
	}


	return NTL_SUCCESS;
}
