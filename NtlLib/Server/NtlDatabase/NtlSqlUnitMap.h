//***********************************************************************************
//
//	File		:	NtlSqlUnitMap.h
//
//	Begin		:	2006-02-22
//
//	Copyright	:	¨Ï NTL-Inc Co., Ltd
//
//	Author		:	Hyun Woo, Koo   ( zeroera@ntl-inc.com )
//
//	Desc		:	Sql Unit Map class
//
//***********************************************************************************

#ifndef __NTLSQLUNITMAP_H__
#define __NTLSQLUNITMAP_H__

#include "NtlSqlUnit.h"

class CNtlSqlUnitMap
{
public:

	CNtlSqlUnitMap();

	virtual ~CNtlSqlUnitMap();


public:

	BOOL						Create(SQLUNITID maxSqlUnitMap);

	void						Destroy();

	CNtlSqlUnit *				Find(SQLUNITID sqlUnitID);

	BOOL						Add(CNtlSqlUnit * pSqlUnit);

	void						Remove(CNtlSqlUnit * pSqlUnit);


private:

	SQLUNITID					m_maxSqlUnitID;

	CNtlSqlUnit **				m_ppSqlUnitMap;
};

#endif // __NTLSQLUNITMAP_H__
