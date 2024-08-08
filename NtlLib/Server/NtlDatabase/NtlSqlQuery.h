// NtlSqlQuery.h: interface for the CNtlSqlQuery class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NTLSQLQUERY_H__00B479FC_5749_4269_B0BF_397FBD634F77__INCLUDED_)
#define AFX_NTLSQLQUERY_H__00B479FC_5749_4269_B0BF_397FBD634F77__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>
#include "NtlSqlBase.h"

class CNtlSqlQuery : public CNtlSqlBase
{
protected:
	string			m_strKey;	

public:		
	CNtlSqlQuery(CNtlSqlDatabase *pdb, LPCTSTR lpszQuery);
	CNtlSqlQuery(CNtlSqlDatabase *pdb, string strKey, LPCTSTR lpszQuery);
	virtual ~CNtlSqlQuery();

	void ResetKey();

	virtual BOOL AllocStatement();
	virtual SQLRETURN BindColumns();	
	virtual SQLRETURN BindParameters(BOOL bRewind=FALSE);	
};

#define DEF_QUERY(n,q)						public:\
												##n(CNtlSqlDatabase *pdb):CNtlSqlQuery(pdb, q){};\
												##n(CNtlSqlDatabase *pdb, string strKey):CNtlSqlQuery(pdb, strKey, q){};



#define BEGIN_PARAM(n)						int GetNumParam(){ int nParam = ##n; return nParam; };\
											SQLRETURN BindParameters(BOOL bRewind=FALSE)\
											{\
												SQLRETURN sqlret = SQL_SUCCESS;\
												SQLSMALLINT nParam = 1;

#define PARAM_ENTRY(t, v)						m_liPARAMS[nParam-1] = 0;\
												if(!bRewind)\
													sqlret = BindParam(t, nParam, &##v);\
												nParam++;\
												if(TSQL_FAIL(sqlret)) return sqlret;

#define PARAM_ENTRY_STR(t, v)					m_liPARAMS[nParam-1] = SQL_NTS;\
												if(!bRewind)\
													sqlret = BindParamStr(t, nParam, ##v, sizeof(##v));\
												nParam++;\
												if(TSQL_FAIL(sqlret)) return sqlret;
#define CUSTOM_PARAM_ENTRY(t, ct, st, v, l)     m_liPARAMS[nParam-1] = 0;\
												if(!bRewind)\
													sqlret = BindCustomParam(t, nParam, ct, st, ##v, l);\
												nParam++;\
												if(TSQL_FAIL(sqlret)) return sqlret;

#define BEGIN_COLUMN(n)						int GetNumCol(){ int nCol = ##n; return nCol; };\
											SQLRETURN BindColumns()\
											{\
												SQLRETURN sqlret = SQL_SUCCESS;\
												SQLSMALLINT nCol = 1;
												

#define COLUMN_ENTRY(v)							sqlret=BindCol( nCol, &##v);\
												nCol++;\
												if(TSQL_FAIL(sqlret)) return sqlret;

#define COLUMN_ENTRY_STR(v)						sqlret=BindColStr( nCol, ##v, sizeof(##v));\
												nCol++;\
												if(TSQL_FAIL(sqlret)) return sqlret;

#define CUSTOM_COLUMN_ENTRY(t, v, l)			sqlret=BindCustomCol( nCol, t, ##v, l);\
												nCol++;\
												if(TSQL_FAIL(sqlret)) return sqlret;

#define END_COL()								return sqlret;\
											}

#define END_PARAM()							END_COL()


#define EXPORT_INIT_QUERY(tag)				extern BOOL InitQuery##tag(CNtlSqlDatabase *pdb);

#define BEGIN_INIT_QUERY(tag)				BOOL InitQuery##tag(CNtlSqlDatabase *pdb)\
											{\
												CNtlSqlQuery *pQuery = NULL;

#define PREDEFINE_QUERY(x)						pQuery = (CNtlSqlQuery *) new (##x)(pdb, #x);\
												if(!pQuery) return FALSE;

#define END_INIT_QUERY()						return TRUE;\
											}


#define DEFINE_QUERY(p,c)					if((p))\
											{\
												##c *query = (##c*)(p)->GetQuery(#c);\
												if(query)\
												{

#define UNDEFINE_QUERY()						}\
											}


#define START_QUERY							DEFINE_QUERY
#define END_QUERY							UNDEFINE_QUERY

#endif // !defined(AFX_NTLSQLQUERY_H__00B479FC_5749_4269_B0BF_397FBD634F77__INCLUDED_)
