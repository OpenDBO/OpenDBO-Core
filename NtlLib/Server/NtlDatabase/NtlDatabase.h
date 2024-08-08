//***********************************************************************************
//
//	File		:	NtlDatabase.h
//
//	Begin		:	2006-02-17
//
//	Copyright	:	¨Ï NTL-Inc Co., Ltd
//
//	Author		:	Hyun Woo, Koo   ( zeroera@ntl-inc.com )
//
//	Desc		:	Database Class
//
//***********************************************************************************

#ifndef __NTLDATABASE_H__
#define __NTLDATABASE_H__

#include "NtlSqlBase.h"
#include "NtlQueue.h"
#include "NtlLinkList.h"

class CNtlQuery;
class CNtlDatabaseConnection;
class CNtlDatabaseManager;
class CNtlDatabase
{
public:
	CNtlDatabase();

	virtual ~CNtlDatabase();

public:
	

	int							Create(CNtlDatabaseManager * pParent, SQLUNITID maxSqlUnitID, SQLHENV hEnv, LPCTSTR lpszDSN, LPCTSTR lpszUserID, LPCTSTR lpszUserPW, int nConnectionCount);

	void						Destroy();

	int							Query(CNtlQuery * pQuery);

	void						Proc();

	void						ResetCount();

	DWORD						GetRecvQueryCount() { return m_dwRecvQuery; }

	DWORD						GetEndQueryCount() { return m_dwEndQuery; }

	DWORD						GetCurQueryCount() { return m_queryQueue.GetSize(); }

public:

	CNtlDatabaseConnection *	PopConnection();

	void						PushConnection(CNtlDatabaseConnection * pConnection);

	CNtlDatabaseManager *		GetParent() { return m_pParent; }


protected:

	void						Init();

	BOOL						AddConnection(CNtlDatabaseConnection * pConnection);

	void						RemoveConnection(CNtlDatabaseConnection * pConnection);


private:


	CNtlLinkList				m_connectionList;

	CNtlMutex					m_listMutex;

	CNtlQueue<CNtlQuery*>		m_queryQueue;

	CNtlDatabaseManager *		m_pParent;

	DWORD						m_dwRecvQuery;

	DWORD						m_dwEndQuery;
};

#endif // __NTLDATABASE_H__
