// NtlSqlQuery.cpp: implementation of the CNtlSqlQuery class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NtlSqlQuery.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CNtlSqlQuery::CNtlSqlQuery(CNtlSqlDatabase *pdb, LPCTSTR lpszQuery)
:CNtlSqlBase(pdb, lpszQuery)
{
	m_strKey = "";		
}

CNtlSqlQuery::CNtlSqlQuery(CNtlSqlDatabase *pdb, string strKey, LPCTSTR lpszQuery)
:CNtlSqlBase(pdb, lpszQuery)
{	
	m_strKey = strKey;	
	if( AllocStatement() &&
		pdb->AddQuery(this, strKey))
	{
		m_strKey = strKey;
	}
}

CNtlSqlQuery::~CNtlSqlQuery()
{
	FreeStatement();

	if(m_strKey != "")
		m_pdb->DelQuery(m_strKey);
}

SQLRETURN CNtlSqlQuery::BindParameters(BOOL bRewind)
{
	(void)bRewind;
	return SQL_SUCCESS;
}

SQLRETURN CNtlSqlQuery::BindColumns()
{
	return SQL_SUCCESS;
}

BOOL CNtlSqlQuery::AllocStatement()
{	
	if(!CNtlSqlBase::AllocStatement()) return FALSE;

	if(TSQL_FAIL(BindParameters()))
	{
		SQLFreeHandle(SQL_HANDLE_STMT, m_hstmt);
		m_hstmt = SQL_NULL_HANDLE;
		return FALSE;
	}	
	
	if(TSQL_FAIL(BindColumns()))
	{
		SQLFreeHandle(SQL_HANDLE_STMT, m_hstmt);
		m_hstmt = SQL_NULL_HANDLE;		
		return FALSE;
	}
	return TRUE;
}

void CNtlSqlQuery::ResetKey()
{
	m_strKey = "";
}



