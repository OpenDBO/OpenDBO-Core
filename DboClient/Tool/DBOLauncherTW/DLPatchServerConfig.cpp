/*****************************************************************************
*
* File			: DLPatchServerConfig.cpp
* Author		: agebreak
* Copyright		: (주)NTL
* Date			: 2007. 10. 4.
* Abstract		: 
*****************************************************************************
* Desc          : Patch Server에 존재하는 PatchServerConfig.XML 파일의 내용을
*				  읽어서 분석하는 클래스
*
*****************************************************************************/


#include "StdAfx.h"
#include "DLPatchServerConfig.h"
#include "NtlXMLDoc.h"
#include "DLUtility.h"
#include "DBOLauncherDef.h"


#define NODE_SERVER_CAPTION_NAME				_T( "PATCH_SERVER_CONFIG/PATCH_CAPTION_NAME" )

#define NODE_SERVER_WEB_PATCH_HISTORY_PATH		_T( "PATCH_SERVER_CONFIG/PATCH_WEB_PATCH_HISTORY_PATH" )

#define NODE_SERVER_LANGUAGE_SVR_IP				_T( "PATCH_SERVER_CONFIG/PATCH_LANGUAGE_SVR_IP" )
#define NODE_SERVER_LANGUAGE_SVR_PATH			_T( "PATCH_SERVER_CONFIG/PATCH_LANGUAGE_SVR_PATH" )
#define NODE_SERVER_LANGUAGE_SVR_FILENAME		_T( "PATCH_SERVER_CONFIG/PATCH_LANGUAGE_SVR_FILENAME" )

#define NODE_SERVER_PATCH_SVR_MASTER_MODE		_T( "PATCH_SERVER_CONFIG/PATCH_PATCH_SVR_MASTER_MODE" )
#define NODE_SERVER_PATCH_SVR_ENABLE			_T( "PATCH_SERVER_CONFIG/PATCH_PATCH_SVR_ENABLE" )
#define NODE_SERVER_PATCH_SVR_IP				_T( "PATCH_SERVER_CONFIG/PATCH_PATCH_SVR_IP" )
#define NODE_SERVER_PATCH_SVR_PATH				_T( "PATCH_SERVER_CONFIG/PATCH_PATCH_SVR_PATH" )
#define NODE_SERVER_PATCH_SVR_FILENAME			_T( "PATCH_SERVER_CONFIG/PATCH_PATCH_SVR_FILENAME" )

#define NODE_SERVER_INTEGRITY_SVR_ENABLE		_T( "PATCH_SERVER_CONFIG/PATCH_INTEGRITY_SVR_ENABLE" )
#define NODE_SERVER_INTEGRITY_SVR_IP			_T( "PATCH_SERVER_CONFIG/PATCH_INTEGRITY_SVR_IP" )
#define NODE_SERVER_INTEGRITY_SVR_PATH			_T( "PATCH_SERVER_CONFIG/PATCH_INTEGRITY_SVR_PATH" )
#define NODE_SERVER_INTEGRITY_SVR_FILENAME		_T( "PATCH_SERVER_CONFIG/PATCH_INTEGRITY_SVR_FILENAME" )

#define NODE_SERVER_PATCH_SERVER_SORTING		_T( "PATCH_SERVER_CONFIG/PATCH_SERVER_SORTING" )


//////////////////////////////////////////////////////////////////////////
//
// CDLPatchServerConfig
//
//////////////////////////////////////////////////////////////////////////


CDLPatchServerConfig g_clDLPatchServerConfig;


CDLPatchServerConfig::CDLPatchServerConfig( void )
{
	m_bPatchMasterMode = false;
	m_bPatchSvrEnable = false;
	m_bIntegritySvrEnable = false;
}

CDLPatchServerConfig::~CDLPatchServerConfig( void ) 
{

}

bool CDLPatchServerConfig::LoadServerConfigFile( CString strPath ) 
{
    CNtlXMLDoc doc;

	if ( !doc.Create() )
	{
		CString strDebug; strDebug.Format( _T( "Loading the configure is failed. %s, %s, %d" ), strPath, __FILEW__, __LINE__ );
		DLSendMessage_ForDebug( strDebug );

		return false;
	}

	LPCTSTR pPath = (LPCTSTR)strPath;

	if ( !doc.Load( (TCHAR*)pPath ) )
	{
		CString strDebug; strDebug.Format( _T( "Loading the configure is failed. %s, %s, %d" ), strPath, __FILEW__, __LINE__ );
		DLSendMessage_ForDebug( strDebug );

		return false;
	}

	TCHAR szBuffer[4096] = { 0, };

	//////////////////////////////////////////////////////////////////////////
	//
	//	Caption name
	//
	//////////////////////////////////////////////////////////////////////////

	if ( !doc.GetDataWithXPath( NODE_SERVER_CAPTION_NAME, szBuffer, 4096 ) )
	{
		CString strDebug; strDebug.Format( _T( "Can not find the node or attributes. %s, %s, %s, %d" ), strPath, NODE_SERVER_CAPTION_NAME, __FILEW__, __LINE__ );
		DLSendMessage_ForDebug( strDebug );

		return false;
	}

	m_strCaptionName = szBuffer;

	//////////////////////////////////////////////////////////////////////////
	//
	//	런처에서 웹페이지에 패치 정보를 출력하기 위해 사용되는 정보
	//
	//////////////////////////////////////////////////////////////////////////

	if ( !doc.GetDataWithXPath( NODE_SERVER_WEB_PATCH_HISTORY_PATH, szBuffer, 4096 ) )
	{
		CString strDebug; strDebug.Format( _T( "Can not find the node or attributes. %s, %s, %s, %d" ), strPath, NODE_SERVER_WEB_PATCH_HISTORY_PATH, __FILEW__, __LINE__ );
		DLSendMessage_ForDebug( strDebug );

		return false;
	}

	m_strWebPatchHistoryPath = szBuffer;

	//////////////////////////////////////////////////////////////////////////
	//
	//	런처에서 출력되는 문자열을 관리하기 위한 정보
	//
	//////////////////////////////////////////////////////////////////////////

	if ( !doc.GetDataWithXPath( NODE_SERVER_LANGUAGE_SVR_IP, szBuffer, 4096 ) )
	{
		CString strDebug; strDebug.Format( _T( "Can not find the node or attributes. %s, %s, %s, %d" ), strPath, NODE_SERVER_LANGUAGE_SVR_IP, __FILEW__, __LINE__ );
		DLSendMessage_ForDebug( strDebug );

		return false;
	}

	m_strLanguageSvrIP = szBuffer;

	if ( !doc.GetDataWithXPath( NODE_SERVER_LANGUAGE_SVR_PATH, szBuffer, 4096 ) )
	{
		CString strDebug; strDebug.Format( _T( "Can not find the node or attributes. %s, %s, %s, %d" ), strPath, NODE_SERVER_LANGUAGE_SVR_PATH, __FILEW__, __LINE__ );
		DLSendMessage_ForDebug( strDebug );

		return false;
	}

	m_strLanguageSvrPath = szBuffer;

	if ( !doc.GetDataWithXPath( NODE_SERVER_LANGUAGE_SVR_FILENAME, szBuffer, 4096 ) )
	{
		CString strDebug; strDebug.Format( _T( "Can not find the node or attributes. %s, %s, %s, %d" ), strPath, NODE_SERVER_LANGUAGE_SVR_FILENAME, __FILEW__, __LINE__ );
		DLSendMessage_ForDebug( strDebug );

		return false;
	}

	m_strLanguageSvrFileName = szBuffer;

	//////////////////////////////////////////////////////////////////////////
	//
	//	패치 관련 정보
	//
	//////////////////////////////////////////////////////////////////////////

	if ( !doc.GetDataWithXPath( NODE_SERVER_LANGUAGE_SVR_IP, szBuffer, 4096 ) )
	{
		CString strDebug; strDebug.Format( _T( "Can not find the node or attributes. %s, %s, %s, %d" ), strPath, NODE_SERVER_LANGUAGE_SVR_IP, __FILEW__, __LINE__ );
		DLSendMessage_ForDebug( strDebug );

		return false;
	}

	m_strLanguageSvrIP = szBuffer;

	if ( !doc.GetDataWithXPath( NODE_SERVER_LANGUAGE_SVR_PATH, szBuffer, 4096 ) )
	{
		CString strDebug; strDebug.Format( _T( "Can not find the node or attributes. %s, %s, %s, %d" ), strPath, NODE_SERVER_LANGUAGE_SVR_PATH, __FILEW__, __LINE__ );
		DLSendMessage_ForDebug( strDebug );

		return false;
	}

	m_strLanguageSvrPath = szBuffer;

	if ( !doc.GetDataWithXPath( NODE_SERVER_LANGUAGE_SVR_FILENAME, szBuffer, 4096 ) )
	{
		CString strDebug; strDebug.Format( _T( "Can not find the node or attributes. %s, %s, %s, %d" ), strPath, NODE_SERVER_LANGUAGE_SVR_FILENAME, __FILEW__, __LINE__ );
		DLSendMessage_ForDebug( strDebug );

		return false;
	}

	m_strLanguageSvrFileName = szBuffer;

	//////////////////////////////////////////////////////////////////////////
	//
	//	패치 관련 정보
	//
	//////////////////////////////////////////////////////////////////////////

	if ( doc.GetDataWithXPath( NODE_SERVER_PATCH_SVR_MASTER_MODE, szBuffer, 4096 ) )
	{
		CString strMasterMode( szBuffer );

		strMasterMode.MakeUpper();

		if ( strMasterMode == _T("TRUE") )
		{
			m_bPatchMasterMode = true;
		}
		else
		{
			m_bPatchMasterMode = false;
		}
	}
	else
	{
		m_bPatchMasterMode = false;
	}

	if ( !doc.GetDataWithXPath( NODE_SERVER_PATCH_SVR_ENABLE, szBuffer, 4096 ) )
	{	
		CString strDebug; strDebug.Format( _T( "Can not find the node or attributes. %s, %s, %s, %d" ), strPath, NODE_SERVER_PATCH_SVR_ENABLE, __FILEW__, __LINE__ );
		DLSendMessage_ForDebug( strDebug );

		return false;
	}

	m_bPatchSvrEnable = _tstoi( szBuffer ) ? true : false;

	if ( !doc.GetDataWithXPath( NODE_SERVER_PATCH_SVR_IP, szBuffer, 4096 ) )
	{	
		CString strDebug; strDebug.Format( _T( "Can not find the node or attributes. %s, %s, %s, %d" ), strPath, NODE_SERVER_PATCH_SVR_IP, __FILEW__, __LINE__ );
		DLSendMessage_ForDebug( strDebug );

		return false;
	}

	m_strPatchSvrIP = szBuffer;

	if ( !doc.GetDataWithXPath( NODE_SERVER_PATCH_SVR_PATH, szBuffer, 4096 ) )
	{	
		CString strDebug; strDebug.Format( _T( "Can not find the node or attributes. %s, %s, %s, %d" ), strPath, NODE_SERVER_PATCH_SVR_PATH, __FILEW__, __LINE__ );
		DLSendMessage_ForDebug( strDebug );

		return false;
	}

	m_strPatchSvrPath = szBuffer;

	if ( !doc.GetDataWithXPath( NODE_SERVER_PATCH_SVR_FILENAME, szBuffer, 4096 ) )
	{	
		CString strDebug; strDebug.Format( _T( "Can not find the node or attributes. %s, %s, %s, %d" ), strPath, NODE_SERVER_PATCH_SVR_FILENAME, __FILEW__, __LINE__ );
		DLSendMessage_ForDebug( strDebug );

		return false;
	}

	m_strPatchSvrFileName = szBuffer;

	//////////////////////////////////////////////////////////////////////////
	//
	//	무결성 관련 정보
	//
	//////////////////////////////////////////////////////////////////////////

	if ( !doc.GetDataWithXPath( NODE_SERVER_INTEGRITY_SVR_ENABLE, szBuffer, 4096 ) )
	{	
		CString strDebug; strDebug.Format( _T( "Can not find the node or attributes. %s, %s, %s, %d" ), strPath, NODE_SERVER_INTEGRITY_SVR_ENABLE, __FILEW__, __LINE__ );
		DLSendMessage_ForDebug( strDebug );

		return false;
	}

	m_bIntegritySvrEnable = _tstoi( szBuffer ) ? true : false;

	if ( !doc.GetDataWithXPath( NODE_SERVER_INTEGRITY_SVR_IP, szBuffer, 4096 ) )
	{	
		CString strDebug; strDebug.Format( _T( "Can not find the node or attributes. %s, %s, %s, %d" ), strPath, NODE_SERVER_INTEGRITY_SVR_IP, __FILEW__, __LINE__ );
		DLSendMessage_ForDebug( strDebug );

		return false;
	}

	m_strIntegritySvrIP = szBuffer;

	if ( !doc.GetDataWithXPath( NODE_SERVER_INTEGRITY_SVR_PATH, szBuffer, 4096 ) )
	{	
		CString strDebug; strDebug.Format( _T( "Can not find the node or attributes. %s, %s, %s, %d" ), strPath, NODE_SERVER_INTEGRITY_SVR_PATH, __FILEW__, __LINE__ );
		DLSendMessage_ForDebug( strDebug );

		return false;
	}

	m_strIntegritySvrPath = szBuffer;

	if ( !doc.GetDataWithXPath( NODE_SERVER_INTEGRITY_SVR_FILENAME, szBuffer, 4096 ) )
	{	
		CString strDebug; strDebug.Format( _T( "Can not find the node or attributes. %s, %s, %s, %d" ), strPath, NODE_SERVER_INTEGRITY_SVR_FILENAME, __FILEW__, __LINE__ );
		DLSendMessage_ForDebug( strDebug );

		return false;
	}

	m_strIntegritySvrFileName = szBuffer;

	// Patch server sorting (CT_TW only, optional). The numbers are written one
	// per line to "<launcher folder>\DBO" for the client.
	TCHAR szCurDir[4096];
	::GetCurrentDirectory( 4096, szCurDir );

	CString strSortingFileName = szCurDir;
	strSortingFileName += DL_PATCH_SERVER_SORTING_FILE_NAME;

	::SetFileAttributes( strSortingFileName, FILE_ATTRIBUTE_NORMAL );

	if ( doc.GetDataWithXPath( NODE_SERVER_PATCH_SERVER_SORTING, szBuffer, 4096 ) )
	{
		if ( ParseServerSorting( szBuffer ) )
		{
			FILE* pFile = NULL;
			_wfopen_s( &pFile, strSortingFileName, _T( "w" ) );

			if ( pFile )
			{
				for ( std::vector< int >::iterator it = m_vecServerSorting.begin(); it != m_vecServerSorting.end(); ++it )
				{
					fprintf( pFile, "%d\n", *it );
				}

				fclose( pFile );
			}
		}
	}

	return true;
}

bool CDLPatchServerConfig::ParseServerSorting( const CString& strSorting )
{
	m_vecServerSorting.clear();

	int nPos = 0;
	CString strToken = strSorting.Tokenize( _T( ";" ), nPos );

	while ( !strToken.IsEmpty() )
	{
		strToken.Trim();

		if ( !strToken.IsEmpty() )
		{
			m_vecServerSorting.push_back( _wtoi( strToken ) );
		}

		strToken = strSorting.Tokenize( _T( ";" ), nPos );
	}

	return !m_vecServerSorting.empty();
}
