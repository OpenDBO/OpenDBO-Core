// NtlSqlBase.cpp: implementation of the CNtlSqlBase class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NtlSqlbase.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNtlSqlBase::CNtlSqlBase(CNtlSqlDatabase *pdb, LPCTSTR lpszQuery)
{
	Init(pdb, lpszQuery);
}

CNtlSqlBase::~CNtlSqlBase()
{
	FreeStatement();
}

void CNtlSqlBase::Init(CNtlSqlDatabase *pdb, LPCTSTR lpszQuery)
{
	if(MAX_QUERY_LEN > lstrlen(lpszQuery))
	{
		m_pdb = pdb;
		m_hdbc = pdb->HDBC();	
		m_hstmt = SQL_NULL_HANDLE;	

		ZeroMemory(m_szQuery, MAX_QUERY_LEN);
		//strcpy( (char *) m_szQuery, (const char *)lpszQuery);	
		strcpy_s((char *)m_szQuery, _countof(m_szQuery), (const char *)lpszQuery);

		ZeroMemory(m_liCOLS, MAX_LEN_IND_SIZE*sizeof(SQLINTEGER));
		ZeroMemory(m_liPARAMS, MAX_LEN_IND_SIZE*sizeof(SQLINTEGER));			
	}
}

int CNtlSqlBase::GetNumCol()
{
	return 0;
}

int CNtlSqlBase::GetNumParam()
{
	return 0;
}

BOOL CNtlSqlBase::IsNull(int nCol)
{
	if(nCol < GetNumCol())
		return (m_liCOLS[nCol] == SQL_NULL_DATA)? TRUE : FALSE;
	
	return FALSE;
}

void CNtlSqlBase::FreeStatement()
{	
	if(SQL_NULL_HANDLE != m_hstmt)
	{
		SQLFreeHandle(SQL_HANDLE_STMT, m_hstmt);
		m_hstmt = SQL_NULL_HANDLE;		
	}	
}

BOOL CNtlSqlBase::AllocStatement()
{
	if(!m_hstmt)
	{
		SQLRETURN sqlret = SQLAllocHandle(SQL_HANDLE_STMT, m_hdbc, &m_hstmt);
		if(TSQL_FAIL(sqlret)) return FALSE;

		sqlret = SQLPrepare( m_hstmt, m_szQuery, SQL_NTS);
		if(TSQL_FAIL(sqlret))
		{			
			SQLFreeHandle(SQL_HANDLE_STMT, m_hstmt);
			m_hstmt = SQL_NULL_HANDLE;
			return FALSE;
		}
	}
	return TRUE;
}

BOOL CNtlSqlBase::Call()
{
	return Open(TRUE);
}

BOOL CNtlSqlBase::Open(BOOL bRetAtOnce)
{	
	if(!AllocStatement()) return FALSE;

	SQLRETURN sqlret = SQLExecute( m_hstmt );
	if(TSQL_REC(sqlret))
	{
		SQLSMALLINT cntrec = 1;
		SQLDIAGREC diag;
		ZeroMemory(&diag, sizeof(SQLDIAGREC));

		while(SQL_NO_DATA != SQLGetDiagRec(
			SQL_HANDLE_STMT, m_hstmt, cntrec++, diag.state,
			&diag.err, diag.msg, MAX_SQLDIAGREC_MSG, &diag.cbmsg) && cntrec != 65535)
		{
 			printf(("[%s] %s\n"), diag.state, diag.msg);
		}
	}
	
	if(bRetAtOnce)
		while(!TSQL_FAIL(SQLMoreResults(m_hstmt)));

	if(TSQL_FAIL(sqlret))
	{
		FreeStatement();
		return FALSE;		
	}	
	
	return TRUE;
}

void CNtlSqlBase::Close()
{		
	if(m_hstmt)
		SQLCloseCursor(m_hstmt);
}

BOOL CNtlSqlBase::Fetch()
{
	if(SQL_NULL_HANDLE == m_hstmt) return FALSE;

	SQLRETURN sqlret = SQLFetch( m_hstmt );	
	if(TSQL_FAIL(sqlret))
	{
		if( !TSQL_NODATA(sqlret ))
		{
			SQLSMALLINT cntrec = 1;
			SQLDIAGREC diag;
			ZeroMemory(&diag, sizeof(SQLDIAGREC));

			while(SQL_NO_DATA != SQLGetDiagRec(
				SQL_HANDLE_STMT, m_hstmt, cntrec++, diag.state,
				&diag.err, diag.msg, MAX_SQLDIAGREC_MSG, &diag.cbmsg) && cntrec != 65535)
			{
				printf(("[%s] %s\n"), diag.state, diag.msg);
			}
			FreeStatement();
		}
		return FALSE;
	}

	return TRUE;
}

//Bind Param
SQLRETURN CNtlSqlBase::BindParam( SQLSMALLINT pt, SQLSMALLINT pi, TIME_STRUCT* ptr)
{
	return SQLBindParameter(m_hstmt, pi, pt,
		SQL_C_TYPE_TIME, SQL_TIME, 0, 0, ptr, 0, &m_liPARAMS[pi-1]);
}

SQLRETURN CNtlSqlBase::BindParam( SQLSMALLINT pt, SQLSMALLINT pi, DATE_STRUCT* ptr)
{
	return SQLBindParameter(m_hstmt, pi, pt,
		SQL_C_TYPE_DATE, SQL_DATE, 0, 0, ptr, 0, &m_liPARAMS[pi-1]);
}

SQLRETURN CNtlSqlBase::BindParam( SQLSMALLINT pt, SQLSMALLINT pi, TIMESTAMP_STRUCT* ptr)
{
	return SQLBindParameter(m_hstmt, pi, pt,
		SQL_C_TYPE_TIMESTAMP, SQL_TIMESTAMP, 0, 0, ptr, 0, &m_liPARAMS[pi-1]);
}

SQLRETURN CNtlSqlBase::BindParam( SQLSMALLINT pt, SQLSMALLINT pi, float* ptr)
{
	return SQLBindParameter(m_hstmt, pi, pt, 
		SQL_C_FLOAT, SQL_REAL, 0, 0, ptr, 0, &m_liPARAMS[pi-1]);
}

SQLRETURN CNtlSqlBase::BindParam( SQLSMALLINT pt, SQLSMALLINT pi, double* ptr)
{
	return SQLBindParameter(m_hstmt, pi, pt,
		SQL_C_DOUBLE, SQL_DOUBLE, 0, 0, ptr, 0, &m_liPARAMS[pi-1]);
}

SQLRETURN CNtlSqlBase::BindParam( SQLSMALLINT pt, SQLSMALLINT pi, int* ptr )
{
	return SQLBindParameter(m_hstmt, pi, pt, 
		SQL_C_SLONG, SQL_INTEGER, 0, 0, ptr, 0, &m_liPARAMS[pi-1]);
}

SQLRETURN CNtlSqlBase::BindParam( SQLSMALLINT pt, SQLSMALLINT pi, long* ptr )
{
	return SQLBindParameter(m_hstmt, pi, pt, 
		SQL_C_SLONG, SQL_INTEGER, 0, 0, ptr, 0, &m_liPARAMS[pi-1]);
}

SQLRETURN CNtlSqlBase::BindParam( SQLSMALLINT pt, SQLSMALLINT pi, short* ptr )
{
	return SQLBindParameter(m_hstmt, pi, pt, 
		SQL_C_SSHORT, SQL_SMALLINT, 0, 0, ptr, 0, &m_liPARAMS[pi-1]);
}

SQLRETURN CNtlSqlBase::BindParam( SQLSMALLINT pt, SQLSMALLINT pi, char* ptr )
{
	return SQLBindParameter(m_hstmt, pi, pt, 
		SQL_C_STINYINT, SQL_TINYINT, 0, 0, ptr, 0, &m_liPARAMS[pi-1]);
}

SQLRETURN CNtlSqlBase::BindParam( SQLSMALLINT pt, SQLSMALLINT pi, __int64* ptr )
{
	return SQLBindParameter(m_hstmt, pi, pt, 
		SQL_C_SBIGINT, SQL_BIGINT, 0, 0, ptr, 0, &m_liPARAMS[pi-1]);
}

SQLRETURN CNtlSqlBase::BindParam( SQLSMALLINT pt, SQLSMALLINT pi, unsigned long* ptr)
{
	return SQLBindParameter(m_hstmt, pi, pt, 
		SQL_C_ULONG, SQL_INTEGER, 0, 0, ptr, 0, &m_liPARAMS[pi-1]);
}

SQLRETURN CNtlSqlBase::BindParam( SQLSMALLINT pt, SQLSMALLINT pi, unsigned short* ptr )
{
	return SQLBindParameter(m_hstmt, pi, pt, 
		SQL_C_USHORT, SQL_SMALLINT, 0, 0, ptr, 0, &m_liPARAMS[pi-1]);
}

SQLRETURN CNtlSqlBase::BindParam( SQLSMALLINT pt, SQLSMALLINT pi, unsigned char* ptr )
{
	return SQLBindParameter(m_hstmt, pi, pt, 
		SQL_C_UTINYINT, SQL_TINYINT, 0, 0, ptr, 0, &m_liPARAMS[pi-1]);
}

SQLRETURN CNtlSqlBase::BindParam( SQLSMALLINT pt, SQLSMALLINT pi, unsigned __int64* ptr )
{
	return SQLBindParameter(m_hstmt, pi, pt, 
		SQL_C_UBIGINT, SQL_BIGINT, 0, 0, ptr, 0, &m_liPARAMS[pi-1]);
}

SQLRETURN CNtlSqlBase::BindParamStr( SQLSMALLINT pt, SQLSMALLINT pi, LPTSTR ptr, SQLINTEGER len )
{	
	return SQLBindParameter(m_hstmt, pi, pt, SQL_C_CHAR, SQL_VARCHAR, len, 0, ptr, len, &m_liPARAMS[pi-1]);
}

SQLRETURN CNtlSqlBase::BindCustomParam(SQLSMALLINT pt, SQLSMALLINT pi, SQLSMALLINT ct, SQLSMALLINT st, LPVOID ptr, SQLINTEGER len)
{
	return SQLBindParameter(m_hstmt, pi, pt, ct, st, len, 0, ptr, len, &m_liPARAMS[pi-1]);
}

//BindCol
SQLRETURN CNtlSqlBase::BindCol( SQLSMALLINT ci, DATE_STRUCT *ptr)
{
	return SQLBindCol( m_hstmt, ci, SQL_C_TYPE_DATE, ptr, 0, &m_liCOLS[ci-1]);
}

SQLRETURN CNtlSqlBase::BindCol( SQLSMALLINT ci, TIMESTAMP_STRUCT *ptr)
{
	return SQLBindCol( m_hstmt, ci, SQL_C_TYPE_TIMESTAMP, ptr, 0, &m_liCOLS[ci-1]);
}

SQLRETURN CNtlSqlBase::BindCol( SQLSMALLINT ci, TIME_STRUCT *ptr)
{
	return SQLBindCol(m_hstmt, ci, SQL_C_TYPE_TIME, ptr, 0, &m_liCOLS[ci-1]);
}

SQLRETURN CNtlSqlBase::BindCol( SQLSMALLINT ci, double* ptr)
{
	return SQLBindCol(m_hstmt, ci, SQL_C_DOUBLE, ptr, 0, &m_liCOLS[ci-1]);
}

SQLRETURN CNtlSqlBase::BindCol( SQLSMALLINT ci, float *ptr)
{
	return SQLBindCol(m_hstmt, ci, SQL_C_FLOAT, ptr, 0, &m_liCOLS[ci-1]);
}

SQLRETURN CNtlSqlBase::BindCol( SQLSMALLINT ci, int* ptr )
{
	return SQLBindCol(m_hstmt, ci, SQL_C_SLONG, ptr, 0, &m_liCOLS[ci-1]);
}

SQLRETURN CNtlSqlBase::BindCol( SQLSMALLINT ci, long* ptr )
{	
	return SQLBindCol(m_hstmt, ci, SQL_C_SLONG, ptr, 0, &m_liCOLS[ci-1]);
}

SQLRETURN CNtlSqlBase::BindCol( SQLSMALLINT ci, short* ptr )
{
	return SQLBindCol(m_hstmt, ci, SQL_C_SSHORT, ptr, 0, &m_liCOLS[ci-1]);
}

SQLRETURN CNtlSqlBase::BindCol( SQLSMALLINT ci, char* ptr )
{
	return SQLBindCol(m_hstmt, ci, SQL_C_STINYINT, ptr, 0, &m_liCOLS[ci-1]);
}

SQLRETURN CNtlSqlBase::BindCol( SQLSMALLINT ci, __int64* ptr )
{
	return SQLBindCol(m_hstmt, ci, SQL_C_SBIGINT, ptr, 0, &m_liCOLS[ci-1]);
}

SQLRETURN CNtlSqlBase::BindCol( SQLSMALLINT ci, unsigned long* ptr)
{
	return SQLBindCol(m_hstmt, ci, SQL_C_ULONG, ptr, 0, &m_liCOLS[ci-1]);
}

SQLRETURN CNtlSqlBase::BindCol( SQLSMALLINT ci, unsigned short* ptr )
{
	return SQLBindCol(m_hstmt, ci, SQL_C_USHORT, ptr, 0, &m_liCOLS[ci-1]);
}

SQLRETURN CNtlSqlBase::BindCol( SQLSMALLINT ci, unsigned char* ptr )
{
	return SQLBindCol(m_hstmt, ci, SQL_C_UTINYINT, ptr, 0, &m_liCOLS[ci-1]);
}

SQLRETURN CNtlSqlBase::BindCol( SQLSMALLINT ci, unsigned __int64* ptr )
{
	return SQLBindCol(m_hstmt, ci, SQL_C_UBIGINT, ptr, 0, &m_liCOLS[ci-1]);
}

SQLRETURN CNtlSqlBase::BindColStr( SQLSMALLINT ci, LPTSTR ptr, SQLINTEGER len )
{
	return SQLBindCol(m_hstmt, ci, SQL_C_CHAR, ptr, len, &m_liCOLS[ci-1]);
}

SQLRETURN CNtlSqlBase::BindCustomCol( SQLSMALLINT ci, SQLSMALLINT tt, LPVOID ptr, SQLINTEGER len)
{
	return SQLBindCol(m_hstmt, ci, tt, ptr, len, &m_liCOLS[ci-1]);
}

