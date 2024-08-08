/*****************************************************************************
*
* File			: NtlLocalize_CJIKor.cpp
* Author		: 
* Copyright		: (주)NTL
* Date			: 
* Abstract		: 
*****************************************************************************
* Desc          : 
*
*****************************************************************************/


#pragma once


#include "stdafx.h"
#include <vector>
#include "NtlClientService.h"
#include "NMCrypt.h"
#include "NPGameLib.h"


//////////////////////////////////////////////////////////////////////////
//
// CNtlSecSol
//
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//
// CNtlSecSol
//
//////////////////////////////////////////////////////////////////////////


class CNtlSecSol
{
public:
	struct sSECURITY_RESULT_DATA
	{
		unsigned int						uiRetFlag;
		unsigned int						uiRetCode;
		sSEC_RET_DATA*						pRetData;
	};

protected:
	CNPGameLib*								m_pNPGameLib;
	std::vector< sSECURITY_RESULT_DATA >	m_defNPSResultList;

public:
	CNtlSecSol( void );
	~CNtlSecSol( void );

public:
	bool									Create( void );
	void									Delete( void );

	void									AttachWindow( HWND hWnd );
	void									AttachUser( const char* pUserID );

	void									SecurityAuthServer( void* pData, int nSize );

	void									CheckGameMon( void );

	bool									IsEmptySecResultData( void );
	void									AddSecResultData( unsigned int uiRetFlag, unsigned int uiRetCode, sSEC_RET_DATA* pRetData );
	bool									PopSecResultData( unsigned int& uiRetFlag, unsigned int& uiRetCode, sSEC_RET_DATA** ppRetData );

	CNPGameLib*								GetNPGameLib( void );
};

CNtlSecSol::CNtlSecSol( void )
{
	m_pNPGameLib = NULL;
}

CNtlSecSol::~CNtlSecSol( void )
{
	Delete();
}

bool CNtlSecSol::Create( void )
{
	m_pNPGameLib = new CNPGameLib( "DragonBallKR" );

	if ( NULL == m_pNPGameLib )
	{
		return false;
	}

	DWORD dwRet = m_pNPGameLib->Init();

	if ( NPGAMEMON_SUCCESS != dwRet )
	{
		AddSecResultData( E_SEC_RET_FLAG_EXIT_APP, dwRet, NULL );

		return false;
	}

	return true;
}

void CNtlSecSol::Delete( void )
{
	std::vector< sSECURITY_RESULT_DATA >::iterator it = m_defNPSResultList.begin();
	for ( ; it != m_defNPSResultList.end(); ++it )
	{
		sSECURITY_RESULT_DATA& sData = *it;

		if ( sData.pRetData )
		{
			if ( sData.pRetData->pData )
			{
				delete [] sData.pRetData->pData;
			}

			delete sData.pRetData;
		}
	}
	m_defNPSResultList.clear();

	if ( m_pNPGameLib )
	{
		delete m_pNPGameLib;
		m_pNPGameLib = NULL;
	}
}

void CNtlSecSol::AttachWindow( HWND hWnd )
{
	if ( m_pNPGameLib )
	{
		m_pNPGameLib->SetHwnd( hWnd );
	}
}

void CNtlSecSol::AttachUser( const char* pUserID )
{
	if ( m_pNPGameLib && pUserID && strlen( pUserID ) > 0 )
	{
		m_pNPGameLib->Send( pUserID );
	}
}

void CNtlSecSol::SecurityAuthServer( void* pData, int nSize )
{
	if ( m_pNPGameLib && pData && sizeof( GG_AUTH_DATA ) == nSize )
	{
		m_pNPGameLib->Auth2( (GG_AUTH_DATA*) pData );
	}
}

void CNtlSecSol::CheckGameMon( void )
{
	if ( m_pNPGameLib )
	{
		DWORD dwRet = m_pNPGameLib->Check();

		if ( NPGAMEMON_SUCCESS != dwRet )
		{
			AddSecResultData( E_SEC_RET_FLAG_EXIT_APP, dwRet, NULL );
		}
	}
}

bool CNtlSecSol::IsEmptySecResultData( void )
{
	return m_defNPSResultList.empty() ? true : false;
}

void CNtlSecSol::AddSecResultData( unsigned int uiRetFlag, unsigned int uiRetCode, sSEC_RET_DATA* pRetData )
{
	sSECURITY_RESULT_DATA sRetData;
	sRetData.uiRetFlag = uiRetFlag;
	sRetData.uiRetCode = uiRetCode;
	sRetData.pRetData = pRetData;

	m_defNPSResultList.push_back( sRetData );
}

bool CNtlSecSol::PopSecResultData( unsigned int& uiRetFlag, unsigned int& uiRetCode, sSEC_RET_DATA** ppRetData )
{
	if ( m_defNPSResultList.empty() )
	{
		return false;
	}

	std::vector< sSECURITY_RESULT_DATA >::iterator it = m_defNPSResultList.begin();

	sSECURITY_RESULT_DATA& sData = *it;

	uiRetFlag = sData.uiRetFlag;
	uiRetCode = sData.uiRetCode;
	*ppRetData = sData.pRetData;

	m_defNPSResultList.erase( it );

	return true;
}

CNPGameLib* CNtlSecSol::GetNPGameLib( void )
{
	return m_pNPGameLib;
}


//////////////////////////////////////////////////////////////////////////
//
// CNtlService_CJIKor
//
//////////////////////////////////////////////////////////////////////////


class CNtlService_CJIKor : public INtlClientService
{
public:
	enum { eCLIP_DATA_SIZE = 1024 * 20 };
	enum { eFIELD_DATA_CNT = 5 };
	enum { eFIELD_DATA_SIZE = 1024 * 2 };

protected:
	char*									m_pKey;
	char*									m_pClipData;
	char									m_szFieldData[eFIELD_DATA_CNT][eFIELD_DATA_SIZE];

	CNtlSecSol*								m_pSecSol;

public:
	CNtlService_CJIKor( void );
	virtual ~CNtlService_CJIKor( void );

public:
	virtual bool							Begin( void* pData );
	virtual void							End( void );

	virtual eDBO_SERVICE_PUBLISHER			GetPublisherID( void );

	virtual void*							GetServiceData( int nIndex );

	virtual bool							CreateSecuritySolution( void );
	virtual void							DeleteSecuritySolution( void );
	virtual void							AttachSecuritySolutionWindow( HWND hWnd );
	virtual void							AttachSecuritySolutionUserID( const char* pUserID );
	virtual void							SecurityAuthServer( void* pData, int nSize );
	virtual void							CheckGameMon( void );
	virtual bool							IsEmptySecurityRetData( void );
	virtual void							AddSecurityRetData( unsigned int uiRetFlag, unsigned int uiRetCode, sSEC_RET_DATA* pRetData );
	virtual bool							PopSecurityRetData( unsigned int& uiRetFlag, unsigned int& uiRetCode, sSEC_RET_DATA** ppRetData );

	CNtlSecSol*								GetSecSol( void );
};


CNtlService_CJIKor::CNtlService_CJIKor( void )
{
	m_pKey = NULL;

	m_pClipData = NULL;

	memset( m_szFieldData, 0, sizeof( m_szFieldData ) );

	m_pSecSol = NULL;
}

CNtlService_CJIKor::~CNtlService_CJIKor( void )
{
	DeleteSecuritySolution();

	End();
}

bool CNtlService_CJIKor::Begin( void* pData )
{
	if ( NULL == pData )
	{
		return false;
	}

	//////////////////////////////////////////////////////////////////////////
	//
	// Key 복사
	//
	//////////////////////////////////////////////////////////////////////////

	int nKeyStrLen = (int)strlen( (char*)pData );

	if ( 0 == nKeyStrLen )
	{
		return false;
	}

	m_pKey = new char [ nKeyStrLen + 1 ];

	strcpy_s( m_pKey, nKeyStrLen + 1, (char*)pData );

	//////////////////////////////////////////////////////////////////////////
	//
	// Data 복사
	//
	//////////////////////////////////////////////////////////////////////////

	int nDataSize = eCLIP_DATA_SIZE;

	m_pClipData = new char [nDataSize];
	memset( m_pClipData, 0, sizeof( char ) * nDataSize );

	if ( !GetNMClipData( m_pClipData, ( sizeof( char ) * nDataSize ) - 1, m_pKey, true ) )
	{
		if ( m_pKey ) { delete m_pKey; m_pKey = NULL; }
		if ( m_pClipData ) { delete m_pClipData; m_pClipData = NULL; }

		return false;
	}

	if ( 0 == strlen( m_pClipData ) )
	{
		if ( m_pKey ) { delete m_pKey; m_pKey = NULL; }
		if ( m_pClipData ) { delete m_pClipData; m_pClipData = NULL; }

		return false;
	}

	int nCurFieldCnt = 0;

	char* pClipData = new char [nDataSize];
	memcpy( pClipData, m_pClipData, nDataSize );

	char* pToken = strtok( pClipData, "," );

	while ( pToken && nCurFieldCnt < eFIELD_DATA_CNT )
	{
		strcpy_s( m_szFieldData[nCurFieldCnt], pToken );

		nCurFieldCnt++;

		pToken = strtok( NULL, "," );
	}

	if ( pClipData )
	{
		delete [] pClipData;
		pClipData = NULL;
	}

	if ( eFIELD_DATA_CNT != nCurFieldCnt )
	{
		if ( m_pKey ) { delete [] m_pKey; m_pKey = NULL; }
		if ( m_pClipData ) { delete [] m_pClipData; m_pClipData = NULL; }

		return false;
	}

	return true;
}

void CNtlService_CJIKor::End( void )
{
	if ( m_pKey && m_pClipData )
	{
		SetNMClipData( m_pClipData, m_pKey );
	}

	if ( m_pKey )
	{
		delete m_pKey;
		m_pKey = NULL;
	}

	if ( m_pClipData )
	{
		delete m_pClipData;
		m_pClipData = NULL;
	}
}

eDBO_SERVICE_PUBLISHER CNtlService_CJIKor::GetPublisherID( void )
{
	return eDBO_SERVICE_PUBLISHER_CJI_KOR;
}

void* CNtlService_CJIKor::GetServiceData( int nIndex )
{
	if ( nIndex >= 0 || nIndex < eFIELD_DATA_CNT )
	{
		return m_szFieldData[nIndex];
	}

	return NULL;
}

bool CNtlService_CJIKor::CreateSecuritySolution( void )
{
	if ( NULL == m_pSecSol )
	{
		m_pSecSol = new CNtlSecSol;
		return m_pSecSol->Create();
	}

	return true;
}

void CNtlService_CJIKor::DeleteSecuritySolution( void )
{
	if ( m_pSecSol )
	{
		m_pSecSol->Delete();
		delete m_pSecSol;
		m_pSecSol = NULL;
	}
}

void CNtlService_CJIKor::AttachSecuritySolutionWindow( HWND hWnd )
{
	if ( m_pSecSol )
	{
		return m_pSecSol->AttachWindow( hWnd );
	}
}

void CNtlService_CJIKor::AttachSecuritySolutionUserID( const char* pUserID )
{
	if ( m_pSecSol )
	{
		return m_pSecSol->AttachUser( pUserID );
	}
}

void CNtlService_CJIKor::SecurityAuthServer( void* pData, int nSize )
{
	if ( m_pSecSol )
	{
		return m_pSecSol->SecurityAuthServer( pData, nSize );
	}
}

void CNtlService_CJIKor::CheckGameMon( void )
{
	if ( m_pSecSol )
	{
		m_pSecSol->CheckGameMon();
	}
}

bool CNtlService_CJIKor::IsEmptySecurityRetData( void )
{
	if ( m_pSecSol )
	{
		return m_pSecSol->IsEmptySecResultData();
	}

	return true;
}

void CNtlService_CJIKor::AddSecurityRetData( unsigned int uiRetFlag, unsigned int uiRetCode, sSEC_RET_DATA* pRetData )
{
	if ( m_pSecSol )
	{
		return m_pSecSol->AddSecResultData( uiRetFlag, uiRetCode, pRetData );
	}
}

bool CNtlService_CJIKor::PopSecurityRetData( unsigned int& uiRetFlag, unsigned int& uiRetCode, sSEC_RET_DATA** ppRetData )
{
	if ( m_pSecSol )
	{
		return m_pSecSol->PopSecResultData( uiRetFlag, uiRetCode, ppRetData );
	}

	return false;
}

CNtlSecSol* CNtlService_CJIKor::GetSecSol( void )
{
	return m_pSecSol;
}


//////////////////////////////////////////////////////////////////////////
//
// CNtlService_CJIKor
//
//////////////////////////////////////////////////////////////////////////


INtlClientService* GetServiceInterface( void )
{
	static CNtlService_CJIKor g_clService;
	return &g_clService;
}


//////////////////////////////////////////////////////////////////////////
//
// Callback function
//
//////////////////////////////////////////////////////////////////////////

BOOL CALLBACK NPGameMonCallback( DWORD dwMsg, DWORD dwArg )
{
	CNtlService_CJIKor* pService = (CNtlService_CJIKor*)GetServiceInterface();

	switch ( dwMsg )
	{
	case NPGAMEMON_COMM_ERROR:
	case NPGAMEMON_COMM_CLOSE:
	case NPGAMEMON_SPEEDHACK:
	case NPGAMEMON_GAMEHACK_KILLED:
	case NPGAMEMON_GAMEHACK_DETECT:
	case NPGAMEMON_GAMEHACK_DOUBT:
		{
			pService->AddSecurityRetData( E_SEC_RET_FLAG_EXIT_APP, dwMsg, NULL );
		}
		return FALSE;

	case NPGAMEMON_INIT_ERROR:
		{
			sSEC_RET_DATA* pRetData = new sSEC_RET_DATA;

			pRetData->dwDataSize = sizeof( DWORD );

			pRetData->pData = new DWORD;
			*(DWORD*)pRetData->pData = dwArg;

			pService->AddSecurityRetData( E_SEC_RET_FLAG_EXIT_APP, dwMsg, pRetData );
		}
		return FALSE;

	case NPGAMEMON_GAMEHACK_REPORT:
		{
			if ( pService->GetSecSol() && pService->GetSecSol()->GetNPGameLib() )
			{
				sSEC_RET_DATA* pRetData = new sSEC_RET_DATA;

				GetHackInfoFromGameMon( &pRetData->dwDataSize );

				pRetData->pData = new BYTE[pRetData->dwDataSize];

				BYTE* pData = pService->GetSecSol()->GetNPGameLib()->GetHackInfo( &pRetData->dwDataSize );

				memcpy( pRetData->pData, pData, pRetData->dwDataSize );

				pService->AddSecurityRetData( E_SEC_RET_FLAG_SEND_SERVER, dwMsg, pRetData );
			}
		}
		return TRUE;

	case NPGAMEMON_CHECK_CSAUTH2:
		{
			sSEC_RET_DATA* pRetData = new sSEC_RET_DATA;

			pRetData->dwDataSize = sizeof( GG_AUTH_DATA );

			pRetData->pData = new GG_AUTH_DATA;
			memcpy( pRetData->pData, (PVOID)dwArg, pRetData->dwDataSize );

			pService->AddSecurityRetData( E_SEC_RET_FLAG_SEND_SERVER, dwMsg, pRetData );
		}
		return TRUE;
	}

	return TRUE;
}
