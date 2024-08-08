//***********************************************************************************
//
//	File		:	NtlSqlUnit.cpp
//
//	Begin		:	2006-02-20
//
//	Copyright	:	ⓒ NTL-Inc Co., Ltd
//
//	Author		:	Hyun Woo, Koo   ( zeroera@ntl-inc.com )
//
//	Desc		:	Sql base object class implementation
//
//***********************************************************************************

#include "stdafx.h"
#include "NtlSqlUnit.h"
#include "NtlLog.h"
#include "NtlError.h"

#include <BaseTsd.h>


#pragma message( "// [11/10/2006 zeroera] : 주의: 현재 BinaryData를 Input Parameter로 넣으면서 Output Parameter로 데이타를 받아오는 SP는 동작하지 않습니다." )


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CNtlSqlUnit::CNtlSqlUnit(SQLUNITID sqlUnitID, LPCTSTR lpszQuery)
:
m_sqlUnitID( sqlUnitID )
{
	Init( lpszQuery );
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CNtlSqlUnit::~CNtlSqlUnit()
{
	Fini();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlSqlUnit::Init(LPCTSTR lpszQuery)
{
	m_hStmt = SQL_NULL_HANDLE;	

	ZeroMemory( m_aColumInd, MAX_LEN_IND_SIZE * sizeof(SQLLEN) );
	ZeroMemory( m_aParamInd, MAX_LEN_IND_SIZE * sizeof(SQLLEN) );			

	m_strQuery.GetString().append( lpszQuery );
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlSqlUnit::Fini()
{
	if( SQL_NULL_HANDLE != m_hStmt )
	{
		SQLFreeHandle( SQL_HANDLE_STMT, m_hStmt );
		m_hStmt = SQL_NULL_HANDLE;		
	}	
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
BOOL CNtlSqlUnit::IsNull(int nCol)
{
	if( nCol < GetNumCol() )
	{
		return ( m_aColumInd[nCol] == SQL_NULL_DATA ) ? TRUE : FALSE;
	}
	
	return FALSE;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
SQLRETURN CNtlSqlUnit::BindParameters(BOOL bRewind/* =FALSE */)
{
	UNREFERENCED_PARAMETER( bRewind );
	return SQL_SUCCESS;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
SQLRETURN CNtlSqlUnit::BindColumns()
{
	return SQL_SUCCESS;
}

//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
BOOL CNtlSqlUnit::Prepare(SQLHDBC hDbc)
{
	if( FALSE == AllocStatement( hDbc ) )
	{
		NTL_LOG_ASSERT("AllocStatement() failed.");
		return FALSE;
	}


	SQLRETURN sqlret = BindParameters();
	if( SQL_SUCCESS != sqlret )
	{
		return FALSE;
	}

	
	sqlret = BindColumns();
	if( SQL_SUCCESS != sqlret )
	{
		return FALSE;
	}


	return TRUE;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
BOOL CNtlSqlUnit::AllocStatement(SQLHDBC hDbc)
{
	if( SQL_NULL_HANDLE != m_hStmt )
	{
		NTL_LOG_ASSERT("SQL_NULL_HANDLE != m_hStmt, m_hStmt = %016x", m_hStmt);
		return FALSE;
	}


	SQLRETURN sqlret = SQLAllocHandle( SQL_HANDLE_STMT, hDbc, &m_hStmt );
	if( TSQL_FAIL(sqlret) )
	{
		return FALSE;
	}


	sqlret = SQLPrepare( m_hStmt, (SQLCHAR*)m_strQuery.c_str(), SQL_NTS );
	if( TSQL_FAIL(sqlret) )
	{			
		SQLFreeHandle( SQL_HANDLE_STMT, m_hStmt );
		m_hStmt = SQL_NULL_HANDLE;
		return FALSE;
	}


	return TRUE;
}


//-----------------------------------------------------------------------------------
//		Purpose	: update/insert/delete statement
//		Return	: 
//-----------------------------------------------------------------------------------
BOOL CNtlSqlUnit::Exec(SQLLEN * pRowCount /* = NULL */)
{
	if( SQL_NULL_HANDLE == m_hStmt )
	{
		NTL_LOG_ASSERT("SQL_NULL_HANDLE == m_hStmt");
		return FALSE;
	}


	SQLRETURN rc = SQLExecute( m_hStmt );

	if( TSQL_DATA_EXEC(rc) )
	{
		rc = WriteBinary();	
	}


	if( TSQL_REC(rc) )
	{
		ReportDiagRec();
	}


	if( !TSQL_FAIL(rc) )
	{	
		if( pRowCount)
		{
			SQLRowCount( m_hStmt, pRowCount );
		}
	}


	while( !TSQL_FAIL( SQLMoreResults(m_hStmt) ) );


	if( TSQL_FAIL(rc) )
	{
		return FALSE;
	}


	return TRUE;
}


//-----------------------------------------------------------------------------------
//		Purpose	:	select statement
//		Return	:
//-----------------------------------------------------------------------------------
BOOL CNtlSqlUnit::Store()
{
	if( SQL_NULL_HANDLE == m_hStmt )
	{
		NTL_LOG_ASSERT("SQL_NULL_HANDLE == m_hStmt");
		return FALSE;
	}


	SQLRETURN rc = SQLExecute( m_hStmt );
	if( TSQL_REC(rc) )
	{
		ReportDiagRec();
	}


	if( TSQL_FAIL(rc) )
	{
		return FALSE;
	}


	return TRUE;
}


//-----------------------------------------------------------------------------------
//		Purpose	:	select statement
//		Return	:
//-----------------------------------------------------------------------------------
BOOL CNtlSqlUnit::Fetch()
{
	SQLRETURN rc = SQLFetch( m_hStmt );
	if( TSQL_FAIL(rc) )
	{
		if( !TSQL_NODATA(rc ))
		{
			ReportDiagRec();
		}

		return FALSE;
	}

	if( IsBinaryRead() )
	{
		ReadBinary();
	}


	return TRUE;
}


//-----------------------------------------------------------------------------------
//		Purpose	:	select statement
//		Return	:	TRUE일 경우라도 ResultCount값을 조사하여 사용해야 한다.
//-----------------------------------------------------------------------------------
BOOL CNtlSqlUnit::StoreAndFetch(SQLLEN * pResultCount /* = NULL */)
{
	if( SQL_NULL_HANDLE == m_hStmt )
	{
		NTL_LOG_ASSERT("SQL_NULL_HANDLE == m_hStmt");
		return FALSE;
	}


	SQLRETURN rc = SQLExecute( m_hStmt );
	if( TSQL_REC(rc) )
	{
		ReportDiagRec();
	}


	if( TSQL_FAIL(rc) )
	{
		return FALSE;
	}


	do 
	{
		rc = SQLFetch( m_hStmt );
		if( TSQL_FAIL(rc) )
		{
			if( !TSQL_NODATA(rc ))
			{
				ReportDiagRec();
				return FALSE;
			}
			else
			{
				return TRUE;
			}
		}

		if( IsBinaryRead() )
		{
			ReadBinary();
		}

		if( pResultCount)
		{
			(*pResultCount)++;
		}

	} while( !TSQL_FAIL(rc) );


	return TRUE;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlSqlUnit::Close()
{		
	if( m_hStmt )
	{
		SQLCloseCursor(m_hStmt);
	}
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlSqlUnit::ReportDiagRec()
{
	SQLSMALLINT	cntrec = 1;
	sSQLDIAGREC	diag;
	ZeroMemory( &diag, sizeof(sSQLDIAGREC) );

	while( SQL_NO_DATA != SQLGetDiagRec(	SQL_HANDLE_STMT, m_hStmt, cntrec++, diag.state,
											&diag.err, diag.msg, MAX_SQLDIAGREC_MSG, &diag.cbmsg) && cntrec != 65535)
	{
		CNtlString strErrorInfo;

		strErrorInfo.Format("SqlState[%s] NativeErrorCode[%u] ErrorMsg[%s]", diag.state, diag.err, diag.msg);
		NTL_PRINT(PRINT_SYSTEM, "%s\n", strErrorInfo.c_str());
		NTL_LOGDL(LOG_WARNING, strErrorInfo.c_str());

		strErrorInfo.Format("Query = \"%s\"", m_strQuery.c_str());
		NTL_PRINT(PRINT_SYSTEM, "%s\n", strErrorInfo.c_str());
		NTL_LOGDL(LOG_WARNING, strErrorInfo.c_str());
	}
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
SQLRETURN CNtlSqlUnit::WriteBinary()
{
	SQLPOINTER token;

	SQLRETURN rc = SQLParamData( m_hStmt, &token );
	while( SQL_NEED_DATA == rc )
	{
		sBINARYINFO * pInfo = NULL;

		BYTE* pBuffer = NULL;
		int nDataSize = 0;
		int nDataOffset = 0;
		for( BINARYIT it = m_binaryParam.begin(); it != m_binaryParam.end(); it++ )
		{
			if( LongToPtr(it->dataID) == token )
			{
				pInfo = (sBINARYINFO*)&(*it);

				pBuffer = pInfo->pBufferPtr;
				nDataSize = pInfo->nDataSize;
				nDataOffset = pInfo->nDataOffset;

				break;
			}
		}

		if( pInfo )
		{
			while( nDataOffset > 0 )
			{
				int nPutSize = min( MAX_BINARY_BATCH_SIZE, nDataOffset );
				rc = SQLPutData ( m_hStmt, pBuffer, nPutSize );

				pBuffer += nPutSize;
				nDataOffset -= nPutSize;
			}
		}


		rc = SQLParamData( m_hStmt, &token );

	} // end of while


	return rc;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlSqlUnit::ReadBinary()
{
	for( BINARYIT it = m_binaryColum.begin(); it != m_binaryColum.end(); it++ )
	{
		SQLRETURN rc;
		SQLLEN BinaryLenOrInd = 0;
		sBINARYINFO * pInfo = (sBINARYINFO*)&(*it);
		BYTE* pBuffer = pInfo->pBufferPtr;

		int nBufferSize = pInfo->nDataSize;

		while( SQL_NO_DATA != ( rc = SQLGetData( m_hStmt, (SQLUSMALLINT)pInfo->dataID, SQL_C_BINARY, (SQLPOINTER)pBuffer, nBufferSize, (SQLLEN*) &BinaryLenOrInd ) ) )
		{
			if( SQL_NULL_DATA == BinaryLenOrInd )
			{
				break;
			}

			int nGetSize = ( BinaryLenOrInd > nBufferSize ) || ( BinaryLenOrInd == SQL_NO_TOTAL ) ? nBufferSize : (int)BinaryLenOrInd;

			pBuffer += nGetSize;
			nBufferSize = nBufferSize - nGetSize;

			if( 0 >= nBufferSize )
			{
				break;
			}

		} // end of while

	} // end of for

}



//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
SQLRETURN CNtlSqlUnit::BindParam( SQLSMALLINT pt, SQLSMALLINT pi, TIME_STRUCT* ptr)
{
	return SQLBindParameter(m_hStmt, pi, pt,
		SQL_C_TYPE_TIME, SQL_TIME, 0, 0, ptr, 0, &m_aParamInd[pi-1]);
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
SQLRETURN CNtlSqlUnit::BindParam( SQLSMALLINT pt, SQLSMALLINT pi, DATE_STRUCT* ptr)
{
	return SQLBindParameter(m_hStmt, pi, pt,
		SQL_C_TYPE_DATE, SQL_DATE, 0, 0, ptr, 0, &m_aParamInd[pi-1]);
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
SQLRETURN CNtlSqlUnit::BindParam( SQLSMALLINT pt, SQLSMALLINT pi, TIMESTAMP_STRUCT* ptr)
{
	return SQLBindParameter(m_hStmt, pi, pt,
		SQL_C_TYPE_TIMESTAMP, SQL_TIMESTAMP, 0, 0, ptr, 0, &m_aParamInd[pi-1]);
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
SQLRETURN CNtlSqlUnit::BindParam( SQLSMALLINT pt, SQLSMALLINT pi, float* ptr)
{
	return SQLBindParameter(m_hStmt, pi, pt, 
		SQL_C_FLOAT, SQL_REAL, 0, 0, ptr, 0, &m_aParamInd[pi-1]);
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
SQLRETURN CNtlSqlUnit::BindParam( SQLSMALLINT pt, SQLSMALLINT pi, double* ptr)
{
	return SQLBindParameter(m_hStmt, pi, pt,
		SQL_C_DOUBLE, SQL_DOUBLE, 0, 0, ptr, 0, &m_aParamInd[pi-1]);
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
SQLRETURN CNtlSqlUnit::BindParam( SQLSMALLINT pt, SQLSMALLINT pi, int* ptr )
{
	return SQLBindParameter(m_hStmt, pi, pt, 
		SQL_C_SLONG, SQL_INTEGER, 0, 0, ptr, 0, &m_aParamInd[pi-1]);
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
SQLRETURN CNtlSqlUnit::BindParam( SQLSMALLINT pt, SQLSMALLINT pi, long* ptr )
{
	return SQLBindParameter(m_hStmt, pi, pt, 
		SQL_C_SLONG, SQL_INTEGER, 0, 0, ptr, 0, &m_aParamInd[pi-1]);
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
SQLRETURN CNtlSqlUnit::BindParam( SQLSMALLINT pt, SQLSMALLINT pi, short* ptr )
{
	return SQLBindParameter(m_hStmt, pi, pt, 
		SQL_C_SSHORT, SQL_SMALLINT, 0, 0, ptr, 0, &m_aParamInd[pi-1]);
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
SQLRETURN CNtlSqlUnit::BindParam( SQLSMALLINT pt, SQLSMALLINT pi, char* ptr )
{
	return SQLBindParameter(m_hStmt, pi, pt, 
		SQL_C_STINYINT, SQL_TINYINT, 0, 0, ptr, 0, &m_aParamInd[pi-1]);
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
SQLRETURN CNtlSqlUnit::BindParam( SQLSMALLINT pt, SQLSMALLINT pi, __int64* ptr )
{
	return SQLBindParameter(m_hStmt, pi, pt, 
		SQL_C_SBIGINT, SQL_BIGINT, 0, 0, ptr, 0, &m_aParamInd[pi-1]);
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
SQLRETURN CNtlSqlUnit::BindParam( SQLSMALLINT pt, SQLSMALLINT pi, unsigned long* ptr)
{
	return SQLBindParameter(m_hStmt, pi, pt, 
		SQL_C_ULONG, SQL_INTEGER, 0, 0, ptr, 0, &m_aParamInd[pi-1]);
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
SQLRETURN CNtlSqlUnit::BindParam( SQLSMALLINT pt, SQLSMALLINT pi, unsigned short* ptr )
{
	return SQLBindParameter(m_hStmt, pi, pt, 
		SQL_C_USHORT, SQL_SMALLINT, 0, 0, ptr, 0, &m_aParamInd[pi-1]);
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
SQLRETURN CNtlSqlUnit::BindParam( SQLSMALLINT pt, SQLSMALLINT pi, unsigned char* ptr )
{
	return SQLBindParameter(m_hStmt, pi, pt, 
		SQL_C_UTINYINT, SQL_TINYINT, 0, 0, ptr, 0, &m_aParamInd[pi-1]);
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
SQLRETURN CNtlSqlUnit::BindParam( SQLSMALLINT pt, SQLSMALLINT pi, unsigned __int64* ptr )
{
	return SQLBindParameter(m_hStmt, pi, pt, 
		SQL_C_UBIGINT, SQL_BIGINT, 0, 0, ptr, 0, &m_aParamInd[pi-1]);
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
SQLRETURN CNtlSqlUnit::BindParamStr( SQLSMALLINT pt, SQLSMALLINT pi, LPTSTR ptr, SQLINTEGER len )
{	
	return SQLBindParameter(m_hStmt, pi, pt, SQL_C_CHAR, SQL_VARCHAR, len, 0, ptr, len, &m_aParamInd[pi-1]);
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
SQLRETURN CNtlSqlUnit::BindCustomParam(SQLSMALLINT pt, SQLSMALLINT pi, SQLSMALLINT ct, SQLSMALLINT st, LPVOID ptr, SQLINTEGER len)
{
	return SQLBindParameter(m_hStmt, pi, pt, ct, st, len, 0, ptr, len, &m_aParamInd[pi-1]);
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
SQLRETURN CNtlSqlUnit::BindParamBinary(SQLSMALLINT pt, SQLSMALLINT pi, SQLSMALLINT cbID ,SQLINTEGER size)
{
	return SQLBindParameter( m_hStmt, pi, pt, SQL_C_BINARY, SQL_VARBINARY, size, 0, (SQLPOINTER) cbID, 0, &m_aParamInd[pi-1] );
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
SQLRETURN CNtlSqlUnit::BindCol( SQLSMALLINT ci, DATE_STRUCT *ptr)
{
	return SQLBindCol( m_hStmt, ci, SQL_C_TYPE_DATE, ptr, 0, &m_aColumInd[ci-1]);
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
SQLRETURN CNtlSqlUnit::BindCol( SQLSMALLINT ci, TIMESTAMP_STRUCT *ptr)
{
	return SQLBindCol( m_hStmt, ci, SQL_C_TYPE_TIMESTAMP, ptr, 0, &m_aColumInd[ci-1]);
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
SQLRETURN CNtlSqlUnit::BindCol( SQLSMALLINT ci, TIME_STRUCT *ptr)
{
	return SQLBindCol(m_hStmt, ci, SQL_C_TYPE_TIME, ptr, 0, &m_aColumInd[ci-1]);
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
SQLRETURN CNtlSqlUnit::BindCol( SQLSMALLINT ci, double* ptr)
{
	return SQLBindCol(m_hStmt, ci, SQL_C_DOUBLE, ptr, 0, &m_aColumInd[ci-1]);
}

SQLRETURN CNtlSqlUnit::BindCol( SQLSMALLINT ci, float *ptr)
{
	return SQLBindCol(m_hStmt, ci, SQL_C_FLOAT, ptr, 0, &m_aColumInd[ci-1]);
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
SQLRETURN CNtlSqlUnit::BindCol( SQLSMALLINT ci, int* ptr )
{
	return SQLBindCol(m_hStmt, ci, SQL_C_SLONG, ptr, 0, &m_aColumInd[ci-1]);
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
SQLRETURN CNtlSqlUnit::BindCol( SQLSMALLINT ci, long* ptr )
{	
	return SQLBindCol(m_hStmt, ci, SQL_C_SLONG, ptr, 0, &m_aColumInd[ci-1]);
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
SQLRETURN CNtlSqlUnit::BindCol( SQLSMALLINT ci, short* ptr )
{
	return SQLBindCol(m_hStmt, ci, SQL_C_SSHORT, ptr, 0, &m_aColumInd[ci-1]);
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
SQLRETURN CNtlSqlUnit::BindCol( SQLSMALLINT ci, char* ptr )
{
	return SQLBindCol(m_hStmt, ci, SQL_C_STINYINT, ptr, 0, &m_aColumInd[ci-1]);
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
SQLRETURN CNtlSqlUnit::BindCol( SQLSMALLINT ci, __int64* ptr )
{
	return SQLBindCol(m_hStmt, ci, SQL_C_SBIGINT, ptr, 0, &m_aColumInd[ci-1]);
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
SQLRETURN CNtlSqlUnit::BindCol( SQLSMALLINT ci, unsigned long* ptr)
{
	return SQLBindCol(m_hStmt, ci, SQL_C_ULONG, ptr, 0, &m_aColumInd[ci-1]);
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
SQLRETURN CNtlSqlUnit::BindCol( SQLSMALLINT ci, unsigned short* ptr )
{
	return SQLBindCol(m_hStmt, ci, SQL_C_USHORT, ptr, 0, &m_aColumInd[ci-1]);
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
SQLRETURN CNtlSqlUnit::BindCol( SQLSMALLINT ci, unsigned char* ptr )
{
	return SQLBindCol(m_hStmt, ci, SQL_C_UTINYINT, ptr, 0, &m_aColumInd[ci-1]);
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
SQLRETURN CNtlSqlUnit::BindCol( SQLSMALLINT ci, unsigned __int64* ptr )
{
	return SQLBindCol(m_hStmt, ci, SQL_C_UBIGINT, ptr, 0, &m_aColumInd[ci-1]);
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
SQLRETURN CNtlSqlUnit::BindColStr( SQLSMALLINT ci, LPTSTR ptr, SQLINTEGER len )
{
	return SQLBindCol(m_hStmt, ci, SQL_C_CHAR, ptr, len, &m_aColumInd[ci-1]);
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
SQLRETURN CNtlSqlUnit::BindCustomCol( SQLSMALLINT ci, SQLSMALLINT tt, LPVOID ptr, SQLINTEGER len)
{
	return SQLBindCol(m_hStmt, ci, tt, ptr, len, &m_aColumInd[ci-1]);
}
