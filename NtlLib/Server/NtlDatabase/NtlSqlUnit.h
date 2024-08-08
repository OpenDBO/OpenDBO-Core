//***********************************************************************************
//
//	File		:	NtlSqlUnit.h
//
//	Begin		:	2006-02-21
//
//	Copyright	:	¨Ï NTL-Inc Co., Ltd
//
//	Author		:	Hyun Woo, Koo   ( zeroera@ntl-inc.com )
//
//	Desc		:	Sql Query Unit Class
//
//***********************************************************************************

#ifndef __NTLSQLUNIT_H__
#define __NTLSQLUNIT_H__


#include "NtlSqlBase.h"
#include "NtlString.h"

#include <vector>

class CNtlSqlUnit  
{
public:

	CNtlSqlUnit(SQLUNITID sqlUnitID, LPCTSTR lpszQuery);

	virtual ~CNtlSqlUnit();

private:

	CNtlSqlUnit():m_sqlUnitID(INVALID_SQLUNITID ) {}

	CNtlSqlUnit & operator=(const CNtlSqlUnit&)  {}


public:

	virtual int				GetNumCol() { return 0; }

	virtual int				GetNumParam() { return 0; }

	virtual SQLRETURN		BindParameters(BOOL bRewind=FALSE);

	virtual SQLRETURN		BindColumns();


public:

	SQLUNITID				GetSqlUnitID() { return m_sqlUnitID; }

	BOOL					Prepare(SQLHDBC hDbc);

	BOOL					Exec(SQLLEN * pRowCount = NULL);

	BOOL					Store();

	BOOL					Fetch();

	BOOL					StoreAndFetch(SQLLEN * pResultCount = NULL);

	void					Close();


protected:

	void					Init(LPCTSTR lpszQuery);

	void					Fini();

	BOOL					AllocStatement(SQLHDBC hDbc);

	BOOL					IsNull(int nCol);

	void					ReportDiagRec();


	int						GetNumBinaryColum() { return (int) m_binaryColum.size(); }

	int						GetNumBinaryParam() { return (int) m_binaryParam.size(); }

	void					ReadBinary();

	SQLRETURN				WriteBinary();

	BOOL					IsBinaryRead() { return GetNumBinaryColum() ? TRUE : FALSE; }


public:

// Bind Column

	SQLRETURN				BindCol( SQLSMALLINT ci, float *ptr);
	SQLRETURN				BindCol( SQLSMALLINT ci, double *ptr);
	SQLRETURN				BindCol( SQLSMALLINT ci, TIME_STRUCT *ptr);
	SQLRETURN				BindCol( SQLSMALLINT ci, DATE_STRUCT *ptr);
	SQLRETURN				BindCol( SQLSMALLINT ci, TIMESTAMP_STRUCT *ptr);

	SQLRETURN				BindCol( SQLSMALLINT ci, int* ptr);
	SQLRETURN				BindCol( SQLSMALLINT ci, long* ptr);
	SQLRETURN				BindCol( SQLSMALLINT ci, char* ptr);
	SQLRETURN				BindCol( SQLSMALLINT ci, short* ptr);	
	SQLRETURN				BindCol( SQLSMALLINT ci, __int64* ptr);

	SQLRETURN				BindCol( SQLSMALLINT ci, unsigned long* ptr);
	SQLRETURN				BindCol( SQLSMALLINT ci, unsigned char* ptr);
	SQLRETURN				BindCol( SQLSMALLINT ci, unsigned short* ptr);
	SQLRETURN				BindCol( SQLSMALLINT ci, unsigned __int64* ptr);	
	
	SQLRETURN				BindColStr( SQLSMALLINT ci, LPTSTR ptr, SQLINTEGER len);
	SQLRETURN				BindCustomCol( SQLSMALLINT ci, SQLSMALLINT tt, LPVOID ptr, SQLINTEGER len);


public:

// Bind Parameter
	
	SQLRETURN				BindParam( SQLSMALLINT pt, SQLSMALLINT pi, float* ptr);
	SQLRETURN				BindParam( SQLSMALLINT pt, SQLSMALLINT pi, double* ptr);
	SQLRETURN				BindParam( SQLSMALLINT pt, SQLSMALLINT pi, TIME_STRUCT* ptr);
	SQLRETURN				BindParam( SQLSMALLINT pt, SQLSMALLINT pi, DATE_STRUCT* ptr);
	SQLRETURN				BindParam( SQLSMALLINT pt, SQLSMALLINT pi, TIMESTAMP_STRUCT* ptr);

	SQLRETURN				BindParam( SQLSMALLINT pt, SQLSMALLINT pi, int* ptr );
	SQLRETURN				BindParam( SQLSMALLINT pt, SQLSMALLINT pi, long* ptr );
	SQLRETURN				BindParam( SQLSMALLINT pt, SQLSMALLINT pi, char* ptr );
	SQLRETURN				BindParam( SQLSMALLINT pt, SQLSMALLINT pi, short* ptr );	
	SQLRETURN				BindParam( SQLSMALLINT pt, SQLSMALLINT pi, __int64* ptr);

	SQLRETURN				BindParam( SQLSMALLINT pt, SQLSMALLINT pi, unsigned long* ptr );
	SQLRETURN				BindParam( SQLSMALLINT pt, SQLSMALLINT pi, unsigned char* ptr );
	SQLRETURN				BindParam( SQLSMALLINT pt, SQLSMALLINT pi, unsigned short* ptr );
	SQLRETURN				BindParam( SQLSMALLINT pt, SQLSMALLINT pi, unsigned __int64* ptr);

	SQLRETURN				BindParamStr( SQLSMALLINT pt, SQLSMALLINT pi, LPTSTR ptr, SQLINTEGER len );
	SQLRETURN				BindCustomParam( SQLSMALLINT pt, SQLSMALLINT pi, SQLSMALLINT ct, SQLSMALLINT st, LPVOID ptr, SQLINTEGER len);
	SQLRETURN				BindParamBinary(SQLSMALLINT pt, SQLSMALLINT pi, SQLSMALLINT cbID, SQLINTEGER size);


protected:

	typedef std::vector<sBINARYINFO> BINARYLIST;
	typedef BINARYLIST::iterator BINARYIT;

	const SQLUNITID			m_sqlUnitID;

	SQLHSTMT				m_hStmt;

	SQLLEN					m_aColumInd[MAX_LEN_IND_SIZE];

	SQLLEN					m_aParamInd[MAX_LEN_IND_SIZE];

	CNtlString				m_strQuery;


	BINARYLIST				m_binaryParam;

	BINARYLIST				m_binaryColum;
};

// Find sql unit at specified connection
#define FIND_SQLUNIT( SQLUNIT_NAME, CON, VAR)																\
	CSqlUnit_##SQLUNIT_NAME * VAR = dynamic_cast<CSqlUnit_##SQLUNIT_NAME *>( CON->FindSqlUnit( SQLUNIT_NAME ) )


#define MAKE_SQLUNIT( SQLUNIT_NAME, CON )																	\
	MakeSqlUnit( CON, new CSqlUnit_##SQLUNIT_NAME )

// Declare unit with name and query
#define BEGIN_DECLARE_SQLUNIT(SQLUNITNAME, QUERY)															\
class CSqlUnit_##SQLUNITNAME : public CNtlSqlUnit															\
{																											\
public:																										\
	CSqlUnit_##SQLUNITNAME():CNtlSqlUnit(SQLUNITNAME, TEXT(QUERY)) {}


// Declare unit with name ( need DEFINE_QUERY MACRO )
#define BEGIN_DECLARE_SQLUNIT2(SQLUNITNAME)																	\
class CSqlUnit_##SQLUNITNAME : public CNtlSqlUnit															\
{																											\
public:																										\
	CSqlUnit_##SQLUNITNAME():CNtlSqlUnit(SQLUNITNAME,


#define DEFINE_QUERY(QUERY) TEXT(QUERY) ) {}


#define END_DECLARE_SQLUNIT()	};




#define BEGIN_VARIABLE()																					\
public:

#define END_VARIABLE()



#define BEGIN_COLUMN(n)						int GetNumCol(){ int nCol = ##n; return nCol; };				\
											SQLRETURN BindColumns()											\
											{																\
												SQLRETURN sqlret = SQL_SUCCESS;								\
												SQLSMALLINT nCol = 1;


#define COLUMN_ENTRY(v)							sqlret=BindCol( nCol, &##v);								\
												nCol++;														\
												if(TSQL_FAIL(sqlret)) return sqlret;


#define COLUMN_ENTRY_STR(v)						sqlret=BindColStr( nCol, ##v, sizeof(##v));					\
												nCol++;														\
												if(TSQL_FAIL(sqlret)) return sqlret;


#define CUSTOM_COLUMN_ENTRY(t, v, l)			sqlret=BindCustomCol( nCol, t, ##v, l);						\
												nCol++;														\
												if(TSQL_FAIL(sqlret)) return sqlret;


#define COLUMN_ENTRY_BINARY(v)					m_binaryColum.push_back(sBINARYINFO(nCol,(BYTE*)&##v,sizeof(##v)));\
												nCol++;														\

#define END_COLUMN()							return sqlret;												\
											}



#define BEGIN_PARAM(n)						int GetNumParam(){ int nParam = ##n; return nParam; };			\
											SQLRETURN BindParameters(BOOL bRewind=FALSE)					\
											{																\
												SQLRETURN sqlret = SQL_SUCCESS;								\
												SQLSMALLINT nParam = 1;										

#define PARAM_ENTRY(t, v)						m_aParamInd[nParam-1] = 0;									\
												if(!bRewind)												\
													sqlret = BindParam(t, nParam, &##v);					\
												nParam++;													\
												if(TSQL_FAIL(sqlret)) return sqlret;


#define PARAM_ENTRY_STR(t, v)					m_aParamInd[nParam-1] = SQL_NTS;							\
												if(!bRewind)												\
													sqlret = BindParamStr(t, nParam, ##v, sizeof(##v));		\
												nParam++;													\
												if(TSQL_FAIL(sqlret)) return sqlret;


#define CUSTOM_PARAM_ENTRY(t, ct, st, v, l)     m_aParamInd[nParam-1] = 0;									\
												if(!bRewind)												\
													sqlret = BindCustomParam(t, nParam, ct, st, ##v, l);	\
												nParam++;													\
												if(TSQL_FAIL(sqlret)) return sqlret;


#define PARAM_ENTRY_BINARY(t, v)				m_aParamInd[nParam-1] = SQL_LEN_DATA_AT_EXEC((int)sizeof(##v)); \
												m_binaryParam.push_back(sBINARYINFO(nParam,(BYTE*)&##v,(int)sizeof(##v)));\
												if(!bRewind) \
													sqlret = BindParamBinary( t, nParam, nParam, sizeof(##v) ); \
												nParam++; \
												if(TSQL_FAIL(sqlret)) return sqlret;



#define END_PARAM()								return sqlret;												\
											}



#endif // __NTLSQLUNIT_H__
