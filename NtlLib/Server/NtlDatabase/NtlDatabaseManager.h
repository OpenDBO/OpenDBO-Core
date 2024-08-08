//***********************************************************************************
//
//	File		:	NtlDatabaseManager.h
//
//	Begin		:	2006-02-17
//
//	Copyright	:	¨Ï NTL-Inc Co., Ltd
//
//	Author		:	Hyun Woo, Koo   ( zeroera@ntl-inc.com )
//
//	Desc		:	Database Manager class
//
//***********************************************************************************


#ifndef __NTLDATABASEMANAGER_H__
#define __NTLDATABASEMANAGER_H__


#include "NtlSqlBase.h"
#include "NtlThread.h"
#include "NtlThreadPool.h"

#include <map>


class CNtlQuery;
class CNtlDatabase;
class CNtlDatabaseConnection;
class CNtlSqlUnitHelper;
class CNtlDatabaseManager : public CNtlRunObject
{
friend class CNtlDatabase;

public:

	CNtlDatabaseManager();

	virtual ~CNtlDatabaseManager();


public:

	int						Create(CNtlSqlUnitHelper * pSqlUnitHelper, int nThreadCount);

	void					Start();

	void					Stop();

	void					WaitForTerminate();

	void					Destroy();

	int						Open(LPCTSTR lpszDSN, LPCTSTR lpszUserID, LPCTSTR lpszUserPW, HDATABASE * phDB, int nConnectionCount);

	void					Close(HDATABASE hDB);

	int						Query(HDATABASE hDB, CNtlQuery * pQuery);

	CNtlSqlUnitHelper *		GetSqlUnitHelper() { return m_pSqlUnitHelper; }

	DWORD					GetRecvQueryCount(HDATABASE hDB);

	DWORD					GetEndQueryCount(HDATABASE hDB);

	DWORD					GetCurQueryCount(HDATABASE hDB);

	bool					ResetCount(HDATABASE hDB);
protected:

	void					Init();

	HDATABASE				AddDatabase(CNtlDatabase * pDatabase);

	void					RemoveDatabase(HDATABASE hDB);

	CNtlDatabase *			FindDatabase(HDATABASE hDB);

	void					Run();

	bool					RunQuery(CNtlQuery * pQuery, CNtlDatabaseConnection * pDatabaseConnection);
	
private:

	typedef std::map<DWORD, CNtlDatabase*> DATABASELIST;
	typedef DATABASELIST::iterator DATABASEIT;
	typedef DATABASELIST::value_type DATABASEVAL;


	CNtlSqlUnitHelper *		m_pSqlUnitHelper;

	DWORD					m_dwDatabaseID;

	DATABASELIST			m_databaseList;

	CNtlMutex				m_listMutex;

	SQLHENV					m_hEnv;

	CNtlThreadPool			m_threadPool;

	CNtlThread *			m_pAppThread;
};


#endif // __NTLDATABASEMANAGER_H__
