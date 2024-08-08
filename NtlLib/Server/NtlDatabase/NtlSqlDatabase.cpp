// NtlSqlDatabase.cpp: implementation of the CNtlSqlDatabase class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NtlSqlDatabase.h"
#include "NtlSqlQuery.h"

// STL header
#include <string>
#include <map>
using namespace std;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CNtlSqlDatabase::CNtlSqlDatabase()
{
	Clear();
}

CNtlSqlDatabase::~CNtlSqlDatabase()
{
	Close();
}

BOOL CNtlSqlDatabase::Open(LPCTSTR lpszDsn,
						LPCTSTR lpszUser,
						LPCTSTR lpszPasswd)
{
	SQLRETURN sqlret;

	sqlret = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &m_henv);	
	if( TSQL_FAIL( sqlret ) )
		return FALSE;

	sqlret = SQLSetEnvAttr(m_henv, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0); 
	if( TSQL_FAIL( sqlret ) )
	{	
		Close();
		return FALSE;
	}

	sqlret = SQLAllocHandle(SQL_HANDLE_DBC, m_henv, &m_hdbc);
	if( TSQL_FAIL( sqlret ) )
	{		
		Close();
		return FALSE;
	}

	sqlret = SQLConnect( m_hdbc,
		(SQLCHAR*)lpszDsn, SQL_NTS,
		(SQLCHAR*)lpszUser, SQL_NTS, 
		(SQLCHAR*)lpszPasswd, SQL_NTS);
	if( TSQL_FAIL( sqlret ) )
	{		
		Close();
		return FALSE;
	}
	
	m_bOpen = TRUE;

	return TRUE;
}

void CNtlSqlDatabase::Close()
{
	ClearQuery();

	if( IsOpen() )
		SQLDisconnect(m_hdbc );

	if( SQL_NULL_HANDLE != m_hdbc )
		SQLFreeHandle( SQL_HANDLE_DBC, m_hdbc);

	if( SQL_NULL_HANDLE != m_henv )
		SQLFreeHandle( SQL_HANDLE_ENV, m_henv);

	Clear();
}

void CNtlSqlDatabase::Clear()
{
	m_bOpen = FALSE;
	m_hdbc = m_henv = SQL_NULL_HANDLE;
}

BOOL CNtlSqlDatabase::IsOpen()
{
	return m_bOpen;
}


SQLHDBC CNtlSqlDatabase::HDBC()
{
	return m_hdbc;
}

BOOL CNtlSqlDatabase::AddQuery(CNtlSqlQuery *pQuery, string strKey)
{
	QUERY_MAP::iterator it;

	it = m_mapQuery.find(strKey);
	if( it != m_mapQuery.end()) return FALSE;

	m_mapQuery.insert(QUERY_MAP::value_type(strKey, pQuery));
	return TRUE;		
}

CNtlSqlQuery * CNtlSqlDatabase::DelQuery(string strKey)
{
	CNtlSqlQuery *pQuery = NULL;

	QUERY_MAP::iterator it = m_mapQuery.find(strKey);
	if( it != m_mapQuery.end())
	{
		pQuery =(*it).second;
		m_mapQuery.erase(it);
	}

	return pQuery;
}

CNtlSqlQuery * CNtlSqlDatabase::GetQuery(string strKey)
{
	CNtlSqlQuery *pQuery = NULL;

	QUERY_MAP::iterator it = m_mapQuery.find(strKey);
	if( it != m_mapQuery.end()) pQuery =(*it).second;

	return pQuery;
}

void CNtlSqlDatabase::ClearQuery()
{
	QUERY_MAP::iterator it;

	for( it = m_mapQuery.begin(); it != m_mapQuery.end(); it++)
	{
		(*it).second->ResetKey();
		delete (*it).second;
	}
	m_mapQuery.clear();
}
