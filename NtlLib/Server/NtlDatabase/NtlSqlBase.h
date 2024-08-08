//***********************************************************************************
//
//	File		:	NtlSqlBase.h
//
//	Begin		:	2006-02-17
//
//	Copyright	:	¨Ï NTL-Inc Co., Ltd
//
//	Author		:	Hyun Woo, Koo   ( zeroera@ntl-inc.com )
//
//	Desc		:	SQL Base Header
//
//***********************************************************************************

#ifndef __NTLSQLBASE_H__
#define __NTLSQLBASE_H__

#include <sql.h>
#include <sqlext.h>


#define TSQL_NODATA(sqlret)						( SQL_NO_DATA == sqlret )
#define TSQL_REC(sqlret)						( SQL_SUCCESS_WITH_INFO == sqlret || SQL_ERROR == sqlret )
#define TSQL_FAIL(sqlret)						( SQL_SUCCESS != sqlret && SQL_SUCCESS_WITH_INFO != sqlret )
#define TSQL_DATA_EXEC(sqlret)					( SQL_NEED_DATA == sqlret )

#define TSQL_NULL_INDEX							-1


#define MAX_PP_STMT								256
#define MAX_QUERY_LEN							2048
#define MAX_SQL_DATA_TYPE						9

#define MAX_SQLDIAGREC_MSG						512
#define MAX_SQLDIAGREC_STATE					6

#define MAX_LEN_IND_SIZE						512

#define MAX_BINARY_BATCH_SIZE					4096


/* SQL & C++ Data Types ------------
C Type					SQL Type
------------------------------------
float					REAL
double					FLOAT
------------------------------------*/



struct sSQLDIAGREC
{
	SQLINTEGER	err;
	SQLSMALLINT cbmsg;
	SQLTCHAR	msg[ MAX_SQLDIAGREC_MSG ];
	SQLTCHAR	state[ MAX_SQLDIAGREC_STATE ];	
};

struct sBINARYINFO
{
	int			dataID;	// binary data id
	BYTE *		pBufferPtr; // binary buffer pointer ( read or write )
	int			nDataSize; // binary data size ( read or write )
	int			nDataOffset; // data offset ( remain size )

	sBINARYINFO(int id, BYTE * ptr, int size):dataID(id), pBufferPtr(ptr), nDataSize(size), nDataOffset(size) {}
};


typedef DWORD SQLUNITID;
const SQLUNITID INVALID_SQLUNITID = 0xFFFFFFFF;


#endif // __NTLSQLBASE_H__
