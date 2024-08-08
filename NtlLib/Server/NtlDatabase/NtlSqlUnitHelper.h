//***********************************************************************************
//
//	File		:	NtlSqlUnitHelper.h
//
//	Begin		:	2006-02-22
//
//	Copyright	:	¨Ï NTL-Inc Co., Ltd
//
//	Author		:	Hyun Woo, Koo   ( zeroera@ntl-inc.com )
//
//	Desc		:	Sql query unit help class 
//
//***********************************************************************************

#ifndef __NTLSQLUNITHELPER_H__
#define __NTLSQLUNITHELPER_H__

#include "NtlSqlBase.h"

class CNtlSqlUnit;
class CNtlDatabaseConnection;
class CNtlSqlUnitHelper
{
public:

	CNtlSqlUnitHelper()
		:m_maxSqlUnitID( 0 ) {}

	CNtlSqlUnitHelper(SQLUNITID maxSqlUnitID)
		:m_maxSqlUnitID( maxSqlUnitID ) {}


public:

	virtual BOOL		PreCreateSqlUnit(CNtlDatabaseConnection * pConnection) = 0;

public:

	int					Create(SQLUNITID maxSqlUnitID);

	SQLUNITID			GetMaxSqlUnitID() { return m_maxSqlUnitID; }


protected:

	BOOL				MakeSqlUnit(CNtlDatabaseConnection * pConnection, CNtlSqlUnit * pSqlUnit);

private:

	SQLUNITID			m_maxSqlUnitID;

};


#endif // __NTLSQLUNITHELPER_H__
