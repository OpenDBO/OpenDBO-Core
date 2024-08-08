/*****************************************************************************
*
* File			: NtlLocalize_Dev.cpp
* Author		: 
* Copyright		: (аж)NTL
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
#include "NPGameLib.h"


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
// CNtlService_Dev
//
//////////////////////////////////////////////////////////////////////////


class CNtlService_Dev : public INtlClientService
{
protected:
	CNtlSecSol*								m_pSecSol;

public:
	CNtlService_Dev( void );
	virtual ~CNtlService_Dev( void );

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


CNtlService_Dev::CNtlService_Dev( void )
{
	m_pSecSol = NULL;
}

CNtlService_Dev::~CNtlService_Dev( void )
{
	DeleteSecuritySolution();

	End();
}

bool CNtlService_Dev::Begin( void* /*pData*/ )
{
	return true;
}

void CNtlService_Dev::End( void )
{
}

eDBO_SERVICE_PUBLISHER CNtlService_Dev::GetPublisherID( void )
{
	return eDBO_SERVICE_PUBLISHER_NTL;
}

void* CNtlService_Dev::GetServiceData( int /*nIndex*/ )
{
	return NULL;
}

bool CNtlService_Dev::CreateSecuritySolution( void )
{
	if ( NULL == m_pSecSol )
	{
		m_pSecSol = new CNtlSecSol;
		return m_pSecSol->Create();
	}

	return true;
}

void CNtlService_Dev::DeleteSecuritySolution( void )
{
	if ( m_pSecSol )
	{
		m_pSecSol->Delete();
		delete m_pSecSol;
		m_pSecSol = NULL;
	}
}

void CNtlService_Dev::AttachSecuritySolutionWindow( HWND hWnd )
{
	if ( m_pSecSol )
	{
		return m_pSecSol->AttachWindow( hWnd );
	}
}

void CNtlService_Dev::AttachSecuritySolutionUserID( const char* pUserID )
{
	if ( m_pSecSol )
	{
		return m_pSecSol->AttachUser( pUserID );
	}
}

void CNtlService_Dev::SecurityAuthServer( void* pData, int nSize )
{
	if ( m_pSecSol )
	{
		return m_pSecSol->SecurityAuthServer( pData, nSize );
	}
}

void CNtlService_Dev::CheckGameMon( void )
{
	if ( m_pSecSol )
	{
		m_pSecSol->CheckGameMon();
	}
}

bool CNtlService_Dev::IsEmptySecurityRetData( void )
{
	if ( m_pSecSol )
	{
		return m_pSecSol->IsEmptySecResultData();
	}

	return true;
}

void CNtlService_Dev::AddSecurityRetData( unsigned int uiRetFlag, unsigned int uiRetCode, sSEC_RET_DATA* pRetData )
{
	if ( m_pSecSol )
	{
		return m_pSecSol->AddSecResultData( uiRetFlag, uiRetCode, pRetData );
	}
}

bool CNtlService_Dev::PopSecurityRetData( unsigned int& uiRetFlag, unsigned int& uiRetCode, sSEC_RET_DATA** ppRetData )
{
	if ( m_pSecSol )
	{
		return m_pSecSol->PopSecResultData( uiRetFlag, uiRetCode, ppRetData );
	}

	return false;
}

CNtlSecSol* CNtlService_Dev::GetSecSol( void )
{
	return m_pSecSol;
}


//////////////////////////////////////////////////////////////////////////
//
// CNtlService_Dev
//
//////////////////////////////////////////////////////////////////////////


INtlClientService* GetServiceInterface( void )
{
	static CNtlService_Dev g_clService;
	return &g_clService;
}


//////////////////////////////////////////////////////////////////////////
//
// Callback function
//
//////////////////////////////////////////////////////////////////////////

BOOL CALLBACK NPGameMonCallback( DWORD dwMsg, DWORD dwArg )
{
	CNtlService_Dev* pService = (CNtlService_Dev*)GetServiceInterface();

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
