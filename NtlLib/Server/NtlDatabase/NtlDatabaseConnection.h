//***********************************************************************************
//
//	File		:	NtlDatabaseConnection.h
//
//	Begin		:	2006-02-17
//
//	Copyright	:	¨Ï NTL-Inc Co., Ltd
//
//	Author		:	Hyun Woo, Koo   ( zeroera@ntl-inc.com )
//
//	Desc		:	Database Connection Class
//
//***********************************************************************************

#pragma once

#include "NtlSqlBase.h"
#include "NtlLinkList.h"
#include "NtlSqlUnitMap.h"


class CNtlQuery;
class CNtlDatabase;
class CNtlSqlUnitHelper;

class CNtlDatabaseConnection : public CNtlLinkObject
{
public:

	CNtlDatabaseConnection();

	~CNtlDatabaseConnection();


public:

	int						Create(CNtlDatabase * pParent, CNtlSqlUnitHelper * pUnitHelper, SQLUNITID maxSqlUnitID, SQLHENV hEnv, LPCTSTR lpszDSN, LPCTSTR lpszUserID, LPCTSTR lpszUserPW);

	bool					IsAlive();

	int						Reconnect();

public:

	BOOL					RegisterSqlUnit(CNtlSqlUnit * pSqlUnit);

	CNtlSqlUnit *			FindSqlUnit(SQLUNITID sqlUnitID);


public:

	CNtlDatabase *			GetParent() { return m_pParent; }

	SQLHDBC					GetHDBC() { return m_hDbc; }


protected:

	void					Init();

	void					Destroy();

	int						Open(CNtlSqlUnitHelper * pUnitHelper, SQLUNITID maxSqlUnitID, SQLHENV hEnv, LPCTSTR lpszDSN, LPCTSTR lpszUserID, LPCTSTR lpszUserPW);


private:


	SQLHDBC					m_hDbc;

	CNtlDatabase *			m_pParent;

	CNtlSqlUnitMap 			m_sqlUnitMap;

	SQLHENV					m_hEnv;

	SQLUNITID				m_maxSqlUnitID;

	CNtlString				m_strDSN;

	CNtlString				m_strUserID;

	CNtlString				m_strUserPW;

};
