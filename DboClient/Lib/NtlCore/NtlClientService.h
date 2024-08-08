/*****************************************************************************
*
* File			: NtlClientService.h
* Author		: 
* Copyright		: (주) NTL
* Date			:
* Abstract		: 
*****************************************************************************
* Desc          : 
*				eDBO_SERVICE_PUBLISHER 타입이 eDBO_SERVICE_PUBLISHER_NTL 일 때는
*				GetServiceData() 함수는 사용되지 않는다
*
*				eDBO_SERVICE_PUBLISHER 타입이 eDBO_SERVICE_PUBLISHER_CJI_KOR 일 때는
*				0 : 서버 IP
*				1 : 서버 Port
*				2 : 인증 쿠키	- 버퍼 크기를1024*2 정도로 잡아야 한다
*				3 : 데이터 쿠키	- 버퍼 크기를1024*2 정도로 잡아야 한다
*				4 : CP쿠키		- 버퍼 크기를 NTL_MAX_SIZE_CP_COOKIE로 잡아야 한다
*				5 : 여분의 인자(CP게임등에서 사용)
*****************************************************************************/


#pragma once


#include "NtlService.h"


enum E_SEC_RET_FLAG
{
	E_SEC_RET_FLAG_INVALID					= 0x00000000,
	E_SEC_RET_FLAG_EXIT_APP					= 0x00000001,
	E_SEC_RET_FLAG_SEND_SERVER				= 0x00000002,
};


struct sSEC_RET_DATA
{
	DWORD									dwDataSize;
	void*									pData;
};

class INtlClientService
{
public:
	virtual ~INtlClientService( void ) { return; }

public:
	virtual bool							Begin( void* pData )				= 0;
	virtual void							End( void )							= 0;

	virtual eDBO_SERVICE_PUBLISHER			GetPublisherID( void )				= 0;

	virtual void*							GetServiceData( int nIndex )		= 0;

	virtual bool							CreateSecuritySolution( void )		= 0;
	virtual void							DeleteSecuritySolution( void )		= 0;
	virtual void							AttachSecuritySolutionWindow( HWND hWnd )				= 0;
	virtual void							AttachSecuritySolutionUserID( const char* pUserID )		= 0;
	virtual void							SecurityAuthServer( void* pData, int nSize )			= 0;
	virtual void							CheckGameMon( void )									= 0;
	virtual bool							IsEmptySecurityRetData( void )							= 0;
	virtual void							AddSecurityRetData( unsigned int uiRetFlag, unsigned int uiRetCode, sSEC_RET_DATA* pRetData )		= 0;
	virtual bool							PopSecurityRetData( unsigned int& uiRetFlag, unsigned int& uiRetCode, sSEC_RET_DATA** ppRetData )	= 0;
};


INtlClientService* GetServiceInterface( void );