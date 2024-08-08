// SampleServer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "NtlSfx.h"
#include "NtlSfxDB.h"


const unsigned int MAX_SIZE_USER_ID = 20;
const unsigned int MAX_SIZE_USER_PW = 20;
const unsigned int MAX_SIZE_CHAR_NAME = 20;
const unsigned int MAX_SIZE_CHAR_DATA = 10;

enum APP_LOG
{
	PRINT_APP = 2,
};


struct sCHAR_DATA
{
	float fLocX;
	float fLocY;
	float fLocZ;
	int nItemCount;
	BYTE abyData[MAX_SIZE_CHAR_DATA];
};





// 1. SQL UNIT ID 정의 ( Application Level )
enum SQL_ID
{
	SP_AccountCreate = 0,
	SP_AccountAuth,
	SP_AccountSelect,
	SP_CharacterCreate,
	SP_CharacterCreate2,
	SP_CharacterSelect,

	MAX_SQL_ID
};


// 2. SQL Unit 클래스 정의한다 ( DBO SQL Unit Library Level )
//------------------------------------------------------------------
BEGIN_DECLARE_SQLUNIT( SP_AccountCreate, "{ ? = call AccountCreate(?, ?, ?) }" )

	BEGIN_VARIABLE()
		char	m_szUserID[MAX_SIZE_USER_ID + 1];
		char	m_szUserPW[MAX_SIZE_USER_ID + 1];
		DWORD	m_dwAccountID;
		int		m_nResultCode;
	END_VARIABLE()

	// 순서에 주의
	BEGIN_PARAM(4)
		PARAM_ENTRY(SQL_PARAM_OUTPUT, m_nResultCode)
		PARAM_ENTRY_STR(SQL_PARAM_INPUT, m_szUserID)
		PARAM_ENTRY_STR(SQL_PARAM_INPUT, m_szUserPW)
		PARAM_ENTRY(SQL_PARAM_OUTPUT, m_dwAccountID)
	END_PARAM()

END_DECLARE_SQLUNIT()
//------------------------------------------------------------------
BEGIN_DECLARE_SQLUNIT( SP_AccountAuth, "{ ? = call AccountAuth(?, ?, ?) }" )

	BEGIN_VARIABLE()
		char	m_szUserID[MAX_SIZE_USER_ID + 1];
		char	m_szUserPW[MAX_SIZE_USER_ID + 1];
		DWORD	m_dwAccountID;
		int		m_nResultCode;
	END_VARIABLE()

	// 순서에 주의
	BEGIN_PARAM(4)
		PARAM_ENTRY(SQL_PARAM_OUTPUT, m_nResultCode)	
		PARAM_ENTRY_STR(SQL_PARAM_INPUT, m_szUserID)
		PARAM_ENTRY_STR(SQL_PARAM_INPUT, m_szUserPW)
		PARAM_ENTRY(SQL_PARAM_OUTPUT, m_dwAccountID)
	END_PARAM()

END_DECLARE_SQLUNIT()
//------------------------------------------------------------------
BEGIN_DECLARE_SQLUNIT2( SP_AccountSelect )

	//DEFINE_QUERY( "Select UserID, UserPW from Account where AccountID = ?" )
	DEFINE_QUERY( "Select AccountID, UserPW from Account where UserID = ?" )

	BEGIN_VARIABLE()
		DWORD	m_dwAccountID;
		char	m_szUserID[MAX_SIZE_USER_ID + 1];
		char	m_szUserPW[MAX_SIZE_USER_PW + 1];
	END_VARIABLE()

	// 순서에 주의
	BEGIN_COLUMN(2)
		COLUMN_ENTRY(m_dwAccountID)
		COLUMN_ENTRY_STR(m_szUserPW)
	END_PARAM()

	// 순서에 주의
	BEGIN_PARAM(1)
		PARAM_ENTRY_STR(SQL_PARAM_INPUT, m_szUserID)
	END_PARAM()

END_DECLARE_SQLUNIT()
//------------------------------------------------------------------
//  [11/10/2006 zeroera] : 주의 : 현재 SP에 Binary를 Param으로 넣는것과 동시에 다른 Param으로 데이타를 받아오는것은 동작하지 않는다
//------------------------------------------------------------------
BEGIN_DECLARE_SQLUNIT( SP_CharacterCreate, "{ ? = call CharacterCreate(?, ?, ?) }" )

	BEGIN_VARIABLE()
		int			m_nResultCode;
		int			m_charID;
		char		m_szCharName[MAX_SIZE_USER_ID + 1];
		sCHAR_DATA	m_sCharData;
	END_VARIABLE()

	// 순서에 주의
	BEGIN_PARAM(4)
		PARAM_ENTRY(SQL_PARAM_OUTPUT, m_nResultCode)
		PARAM_ENTRY_STR(SQL_PARAM_INPUT, m_szCharName)
		PARAM_ENTRY_BINARY(SQL_PARAM_INPUT, m_sCharData)
		PARAM_ENTRY(SQL_PARAM_OUTPUT, m_charID)
	END_PARAM()

END_DECLARE_SQLUNIT()
//------------------------------------------------------------------
BEGIN_DECLARE_SQLUNIT2( SP_CharacterCreate2 )

	DEFINE_QUERY( "insert Character(CharName, CharData) Values(?, ?)" )

	BEGIN_VARIABLE()
		char		m_szCharName[MAX_SIZE_USER_ID + 1];
		sCHAR_DATA	m_sCharData;
	END_VARIABLE()

	// 순서에 주의
	BEGIN_PARAM(2)
		PARAM_ENTRY_STR(SQL_PARAM_INPUT, m_szCharName)
		PARAM_ENTRY_BINARY(SQL_PARAM_INPUT, m_sCharData)
	END_PARAM()

END_DECLARE_SQLUNIT()

//------------------------------------------------------------------
BEGIN_DECLARE_SQLUNIT2( SP_CharacterSelect )

	DEFINE_QUERY( "Select top 1 CharID, CharName, CharData from Character where CharName = ?" )

	BEGIN_VARIABLE()
		DWORD		m_charID;
		char		m_szCharName[MAX_SIZE_CHAR_NAME + 1];
		sCHAR_DATA	m_sCharData;
	END_VARIABLE()

	// 순서에 주의
	BEGIN_COLUMN(3)
		COLUMN_ENTRY(m_charID)
		COLUMN_ENTRY_STR(m_szCharName)
		COLUMN_ENTRY_BINARY(m_sCharData)
	END_PARAM()

	// 순서에 주의
	BEGIN_PARAM(1)
		PARAM_ENTRY_STR(SQL_PARAM_INPUT, m_szCharName)
	END_PARAM()

END_DECLARE_SQLUNIT()
//------------------------------------------------------------------

/*
BEGIN_DECLARE_SQLUNIT2( SP_PROCSALES )

	DEFINE_QUERY( "{ call procSales }" )

	BEGIN_VARIABLE()
		int			m_nAffectCount;
		DWORD		m_dwUserID;
		DWORD		m_dwLevel;
		DWORD		m_dwExp;
	END_VARIABLE()

	BEGIN_COLUMN(3)
		COLUMN_ENTRY(m_dwUserID)
		COLUMN_ENTRY(m_dwLevel)
		COLUMN_ENTRY(m_dwExp)
	END_COLUMN()

	BEGIN_PARAM(1)
		PARAM_ENTRY(SQL_PARAM_INPUT, m_dwUserID)
	END_PARAM()

END_DECLARE_SQLUNIT()
*/
//------------------------------------------------------------------


// 3. Application에서 사용할 Unit Helper 클래스를 정의 ( Application Level )
class CSampleSqlUnitHelper : public CNtlSqlUnitHelper
{
public:

	CSampleSqlUnitHelper(SQLUNITID maxSqlUnitID)
		:CNtlSqlUnitHelper(maxSqlUnitID) {}

	virtual BOOL	PreCreateSqlUnit(CNtlDatabaseConnection * pConnection)
	{
		if( FALSE == MAKE_SQLUNIT(SP_AccountCreate, pConnection ) )
		{
			return FALSE;
		}

		if( FALSE == MAKE_SQLUNIT(SP_AccountAuth, pConnection ) )
		{
			return FALSE;
		}

		if( FALSE == MAKE_SQLUNIT(SP_AccountSelect, pConnection ) )
		{
			return FALSE;
		}

		if( FALSE == MAKE_SQLUNIT(SP_CharacterCreate, pConnection ) )
		{
			return FALSE;
		}

		if( FALSE == MAKE_SQLUNIT(SP_CharacterCreate2, pConnection ) )
		{
			return FALSE;
		}

		if( FALSE == MAKE_SQLUNIT(SP_CharacterSelect, pConnection ) )
		{
			return FALSE;
		}

		return TRUE;
	}
};



CNtlEvent g_exitEvent;


// 4. Application에서 사용할 Query 클래스를 작성한다. ( Application Level )
class CQuery_Account : public CNtlQuery
{
public:

	CQuery_Account(const char * lpszUserID, const char * lpszUserPW)
	{
		ZeroMemory( m_szUserID, MAX_SIZE_USER_ID + 1 );
		ZeroMemory( m_szUserPW, MAX_SIZE_USER_PW + 1 );

		strncpy_s( m_szUserID, lpszUserID, MAX_SIZE_USER_ID );
		strncpy_s( m_szUserPW, lpszUserPW, MAX_SIZE_USER_PW );
		strncpy_s( m_szCharName, lpszUserID, MAX_SIZE_CHAR_NAME );
	}
		
	int ExecuteQuery(CNtlDatabaseConnection * pConnection)
	{
		SQLLEN nRowCount = 0;

		////---------------------------------------------------------------
		//FIND_SQLUNIT( SP_AccountCreate, pConnection, pSqlUnit1 );
		//if( NULL == pSqlUnit1 )
		//{
		//	return NTL_FAIL;
		//}

		//strncpy_s( pSqlUnit1->m_szUserID, m_szUserID, MAX_SIZE_USER_ID );
		//strncpy_s( pSqlUnit1->m_szUserPW, m_szUserPW, MAX_SIZE_USER_PW );
		//pSqlUnit1->Exec();
		////---------------------------------------------------------------

		////---------------------------------------------------------------
		//FIND_SQLUNIT( SP_AccountAuth, pConnection, pSqlUnit2 );
		//if( NULL == pSqlUnit2 )
		//{
		//	return NTL_FAIL;
		//}

		//strncpy_s( pSqlUnit2->m_szUserID, m_szUserID, MAX_SIZE_USER_ID );
		//strncpy_s( pSqlUnit2->m_szUserPW, m_szUserPW, MAX_SIZE_USER_PW );
		//
		//pSqlUnit2->Exec(&nRowCount);		
		////---------------------------------------------------------------


		////---------------------------------------------------------------
		//FIND_SQLUNIT( SP_AccountSelect, pConnection, pSqlUnit3 );
		//if( NULL == pSqlUnit3 )
		//{
		//	return NTL_FAIL;
		//}

		////pSqlUnit3->m_dwAccountID = 1;
		//strncpy_s( pSqlUnit3->m_szUserID, m_szUserID, MAX_SIZE_USER_ID );


		//// Store와 Fetch를 같이 하는 경우, TRUE와 리턴된 데이타의 수를 비교하여 판단할 것
		////SQLINTEGER nResultCount = 0;
		////pSqlUnit3->StoreAndFetch(&nResultCount);

		//

		//// Store와 Fetch를 따로 하는 경우 ( 여러개의 데이타를 가져 올때 )
		//if( pSqlUnit3->Store() )
		//{
		//	pSqlUnit3->Fetch();
		//}
		////---------------------------------------------------------------


		////---------------------------------------------------------------
		//sCHAR_DATA sCharData;
		//sCharData.fLocX = 1.1f;
		//sCharData.fLocY = 2.2f;
		//sCharData.fLocZ = 999999.999f;
		//sCharData.nItemCount = 30;
		//memset( (void*)sCharData.abyData, 0xAE, sizeof(sCharData.abyData) );

		//FIND_SQLUNIT( SP_CharacterCreate, pConnection, pSqlUnit4 );
		//if( NULL == pSqlUnit4 )
		//{
		//	return NTL_FAIL;
		//}

		//strncpy_s( pSqlUnit4->m_szCharName, m_szCharName, MAX_SIZE_CHAR_NAME );
		//memcpy( (void*) &pSqlUnit4->m_sCharData, &sCharData, sizeof(sCharData) );

		//pSqlUnit4->Exec( &nRowCount );
		////---------------------------------------------------------------


		////---------------------------------------------------------------
		//FIND_SQLUNIT( SP_AccountSelect, pConnection, pSqlUnit3 );
		//if( NULL == pSqlUnit3 )
		//{
		//	return NTL_FAIL;
		//}

		////pSqlUnit3->m_dwAccountID = 1;
		//strncpy_s( pSqlUnit3->m_szUserID, m_szUserID, MAX_SIZE_USER_ID );


		//// Store와 Fetch를 같이 하는 경우, TRUE와 리턴된 데이타의 수를 비교하여 판단할 것
		//SQLINTEGER nResultCount = 0;
		//pSqlUnit3->StoreAndFetch(&nResultCount);


		//// Store와 Fetch를 따로 하는 경우 ( 여러개의 데이타를 가져 올때 )
		//if( pSqlUnit3->Store() )
		//{
		//	pSqlUnit3->Fetch();
		//}
		////---------------------------------------------------------------


		//---------------------------------------------------------------
		FIND_SQLUNIT( SP_CharacterSelect, pConnection, pSqlUnit5 );
		if( NULL == pSqlUnit5 )
		{
			return NTL_FAIL;
		}

		strncpy_s( pSqlUnit5->m_szCharName, m_szUserID, MAX_SIZE_USER_ID );


		// Store와 Fetch를 같이 하는 경우, TRUE와 리턴된 데이타의 수를 비교하여 판단할 것
		//SQLINTEGER nResultCount = 0;
		//pSqlUnit5->StoreAndFetch(&nResultCount);


		// Store와 Fetch를 따로 하는 경우 ( 여러개의 데이타를 가져 올때 )
		if( pSqlUnit5->Store() )
		{
			pSqlUnit5->Fetch();
		}
		//---------------------------------------------------------------


		NTL_PRINT(PRINT_APP, "ExecuteQuery Done" );

		return NTL_SUCCESS;
	}


	int ExecuteResult()
	{
		NTL_PRINT(PRINT_APP, "ExecuteResult Done" );

		g_exitEvent.Notify();

		return NTL_SUCCESS;
	}		


private:

	DWORD				m_dwAccountID;

	char				m_szUserID[MAX_SIZE_USER_ID + 1];

	char				m_szUserPW[MAX_SIZE_USER_PW + 1];

	char				m_szCharName[MAX_SIZE_CHAR_NAME + 1];

};



//-----------------------------------------------------------------------------------
//		DB 샘플 서버 메인
//-----------------------------------------------------------------------------------
int DBSampleServerMain(int argc, _TCHAR* argv[])
{
	int rc = NTL_SUCCESS;

	HDATABASE hDB = INVALID_HDATABASE;



	// 1. 데이테 베이스 생성에 필요한 SqlUnitHelper ( 최대 SQLUNIT ID값을 넣는다 )
	CSampleSqlUnitHelper sqlUnitHelper(MAX_SQL_ID);


	// 2. 데이타 베이스
	CNtlDatabaseManager databaseManager;


	// 3. 데이타 베이스 생성 ( SqlUnit Helper Ptr, 최대 Thread값 ) 
	rc = databaseManager.Create(&sqlUnitHelper, 10);
	if( NTL_SUCCESS != rc )
	{
		NTL_PRINT(PRINT_APP, "DatabaseManager Create Error %d(%s)", rc, NtlGetErrorMessage(rc) );
		return -1;
	}


	// 4. 데이타 베이스 연결 ( DSN, userid, userpw, DATABASE HANDLE, 최대 Connection 값 )
	rc = databaseManager.Open("dbtest", "dbtester", "1234", &hDB, 10);
	if( NTL_SUCCESS != rc )
	{
		NTL_PRINT(PRINT_APP, "DatabaseManager Open Error %d(%s)", rc, NtlGetErrorMessage(rc) );
		return -1;
	}


	// 5. 데이타 베이스 시작 ( 내부적으로 Thread )
	databaseManager.Start();



	// 6. 쿼리 보내기
	databaseManager.Query(  hDB, new CQuery_Account(TEXT("tester2"), "1234") );




	g_exitEvent.Wait();


	return 0;

}
