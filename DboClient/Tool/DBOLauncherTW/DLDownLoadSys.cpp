/*****************************************************************************
*
* File			: DLDownLoadSys.cpp
* Author		:
* Copyright		: (주) NTL
* Date			:
* Abstract		:
*****************************************************************************
* Desc          : HTTP download of the CT_TW build. Unlike the MFC
*                 CHttpConnection version it talks to WinInet directly
*                 (InternetOpenUrl) and has a "Range:" resume path. The local
*                 file is deleted before the size check, so in practice the
*                 resume path is never taken; this mirrors the retail code.
*
*****************************************************************************/


#include "StdAfx.h"
#include <wininet.h>
#include "afxinet.h"
#include "DLDownLoadSys.h"
#include "DLUtility.h"


#pragma comment( lib, "wininet.lib" )


#define DL_RF_ERROR_OPEN_FAILED						(1)
#define DL_RF_ERROR_QUERYINFO_FAILED				(2)
#define DL_RF_ERROR_QUERYDATA_FAILED				(3)
#define DL_RF_ERROR_READFILE_FAILED					(4)
#define DL_RF_USER_EXIT								(5)
#define DL_RF_ERROR_SAVEFILEOPEN_FAILED				(6)

#define DL_DOWNLOAD_BUFFER_SIZE						(64 * 1024)


static unsigned char s_abyDownloadBuffer[DL_DOWNLOAD_BUFFER_SIZE];


//////////////////////////////////////////////////////////////////////////
//
// CDLDownLoadSys
//
//////////////////////////////////////////////////////////////////////////


CDLDownLoadSys::CDLDownLoadSys( void )
{
	m_bPendingDestroy = false;
}

CDLDownLoadSys::~CDLDownLoadSys( void )
{
	Delete();
}

bool CDLDownLoadSys::Create( void )
{
	Delete();

	return true;
}

void CDLDownLoadSys::Delete( void )
{
}

void CDLDownLoadSys::DLMessageProc( eDL_EVENT eMsg, sDL_EVENT_DATA_BASE* pData )
{
	if ( eDL_EVENT_DO_DOWNLOAD == eMsg )
	{
		sDL_ED_DO_DOWNLOAD* pEvtData = (sDL_ED_DO_DOWNLOAD*) pData;

		bool bRet = DoDownload( &m_bPendingDestroy,
								pEvtData->strDownloadServerName,
								pEvtData->strDownloadServerPath,
								pEvtData->strDownloadFileName,
								pEvtData->strClientPath,
								pEvtData->strClientFileName );

		if ( bRet )
		{
			pEvtData->eResult = sDL_ED_DO_DOWNLOAD::eRESULT_SUCCESS;
		}
		else
		{
			pEvtData->eResult = m_bPendingDestroy ? sDL_ED_DO_DOWNLOAD::eRESULT_USER_EXIT : sDL_ED_DO_DOWNLOAD::eRESULT_ERROR;
		}
	}
	else if ( eDL_EVENT_USER_EXIT == eMsg )
	{
		m_bPendingDestroy = true;
	}
}

bool CDLDownLoadSys::DoDownload( bool* pbPendingDestroy,
								 CString strServerName,
								 CString strServerPath,
								 CString strServerFileName,
								 CString strClientPath,
								 CString strClientFileName )
{
	//////////////////////////////////////////////////////////////////////////
	//
	// 경로 설정
	//
	//////////////////////////////////////////////////////////////////////////

	CString strURL = _T( "http://" );
	strURL += strServerName;
	if ( strURL.IsEmpty() || strURL[strURL.GetLength() - 1] != _T( '\\' ) )
	{
		strURL += _T( "\\" );
	}
	strURL += strServerPath;
	if ( strURL.IsEmpty() || strURL[strURL.GetLength() - 1] != _T( '\\' ) )
	{
		strURL += _T( "\\" );
	}
	strURL += strServerFileName;

	// HTTP에서는 \을 쓰지않는다.
	strURL.Replace( _T( "\\" ), _T( "/" ) );

	CString strClientFullPathFileName = strClientPath;
	if ( strClientFullPathFileName.GetLength() > 0 && strClientFullPathFileName[strClientFullPathFileName.GetLength() - 1] != _T( '\\' ) )
	{
		strClientFullPathFileName += _T( "\\" );
	}
	strClientFullPathFileName += strClientFileName;
	strClientFullPathFileName.Replace( _T( "\\" ), _T( "/" ) );

	HINTERNET hInternet = NULL;
	HINTERNET hURL = NULL;
	CFile* pSaveFile = NULL;

	try
	{
		if ( *pbPendingDestroy )
		{
			throw new CInternetException( DL_RF_USER_EXIT );
		}

		hInternet = ::InternetOpen( _T( "HTTP" ), INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0 );
		if ( NULL == hInternet )
		{
			throw new CInternetException( DL_RF_ERROR_OPEN_FAILED );
		}

		hURL = ::InternetOpenUrl( hInternet, strURL, NULL, 0, INTERNET_FLAG_RELOAD, 0 );
		if ( NULL == hURL )
		{
			throw new CInternetException( DL_RF_ERROR_OPEN_FAILED );
		}

		TCHAR szContentLength[MAX_PATH] = { 0, };
		DWORD dwBufferLength = MAX_PATH;
		DWORD dwIndex = 0;
		if ( !::HttpQueryInfo( hURL, HTTP_QUERY_CONTENT_LENGTH, szContentLength, &dwBufferLength, &dwIndex ) )
		{
			throw new CInternetException( DL_RF_ERROR_QUERYINFO_FAILED );
		}

		// 저장할 파일이 이미 존재하면 지운다
		if ( -1 != _waccess( strClientFullPathFileName, 0 ) )
		{
			::SetFileAttributes( strClientFullPathFileName, FILE_ATTRIBUTE_NORMAL );
			::DeleteFile( strClientFullPathFileName );
		}

		unsigned int uiDownloadFileSize = (unsigned int)_wtoi( szContentLength );
		unsigned int uiFileSaveSize = 0;

		FILE* pExistFile = NULL;
		_wfopen_s( &pExistFile, strClientFullPathFileName, _T( "r" ) );

		if ( pExistFile )
		{
			// 이어 받기
			fseek( pExistFile, 0, SEEK_END );
			uiFileSaveSize = (unsigned int)ftell( pExistFile );
			fclose( pExistFile );

			::InternetCloseHandle( hURL );
			hURL = NULL;
			::InternetCloseHandle( hInternet );

			hInternet = ::InternetOpen( _T( "HTTP" ), INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0 );
			if ( NULL == hInternet )
			{
				throw new CInternetException( DL_RF_ERROR_OPEN_FAILED );
			}

			CString strRange;
			strRange.Format( _T( "Range:bytes=%d-" ), uiFileSaveSize );

			hURL = ::InternetOpenUrl( hInternet, strURL, strRange, strRange.GetLength(), INTERNET_FLAG_RELOAD, 0 );
			if ( NULL == hURL )
			{
				throw new CInternetException( DL_RF_ERROR_OPEN_FAILED );
			}

			pSaveFile = new CFile;
			if ( !pSaveFile->Open( strClientFullPathFileName, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::shareDenyWrite ) )
			{
				throw new CInternetException( DL_RF_ERROR_SAVEFILEOPEN_FAILED );
			}

			pSaveFile->SeekToEnd();
		}
		else
		{
			pSaveFile = new CFile;
			if ( !pSaveFile->Open( strClientFullPathFileName, CFile::modeCreate | CFile::modeWrite | CFile::shareDenyWrite ) )
			{
				throw new CInternetException( DL_RF_ERROR_SAVEFILEOPEN_FAILED );
			}
		}

		//////////////////////////////////////////////////////////////////////////
		//
		// 파일 다운로드 및 저장
		//
		//////////////////////////////////////////////////////////////////////////

		DWORD dwRead = 0;

		do
		{
			DWORD dwAvailable = 0;
			if ( !::InternetQueryDataAvailable( hURL, &dwAvailable, 0, 0 ) )
			{
				throw new CInternetException( DL_RF_ERROR_QUERYDATA_FAILED );
			}

			if ( dwAvailable > DL_DOWNLOAD_BUFFER_SIZE )
			{
				dwAvailable = DL_DOWNLOAD_BUFFER_SIZE;
			}

			if ( !::InternetReadFile( hURL, s_abyDownloadBuffer, dwAvailable, &dwRead ) )
			{
				throw new CInternetException( DL_RF_ERROR_READFILE_FAILED );
			}

			if ( dwRead > 0 )
			{
				pSaveFile->Write( s_abyDownloadBuffer, dwRead );

				uiFileSaveSize += dwRead;

				sDL_ED_DOWNLOADING_NFY sDownloadNfy;
				sDownloadNfy.strFileName		= strServerFileName;
				sDownloadNfy.uiFileSize			= uiDownloadFileSize;
				sDownloadNfy.uiCurUpdateSize	= uiFileSaveSize;
				sDownloadNfy.bShowComplete		= true;
				DLSendEvent( eDL_EVENT_DOWNLOADING_NFY, &sDownloadNfy );
			}

			if ( *pbPendingDestroy )
			{
				throw new CInternetException( DL_RF_USER_EXIT );
			}
		}
		while ( dwRead > 0 );

		sDL_ED_DOWNLOADING_NFY sDownloadNfy;
		sDownloadNfy.strFileName		= strServerFileName;
		sDownloadNfy.uiFileSize			= uiDownloadFileSize;
		sDownloadNfy.uiCurUpdateSize	= uiFileSaveSize;
		sDownloadNfy.bShowComplete		= true;
		DLSendEvent( eDL_EVENT_DOWNLOADING_NFY, &sDownloadNfy );

		pSaveFile->Close();
		delete pSaveFile;
		pSaveFile = NULL;

		::InternetCloseHandle( hURL );
		::InternetCloseHandle( hInternet );
	}
	catch ( CInternetException* pEx )
	{
		if ( pSaveFile )
		{
			pSaveFile->Abort();
			delete pSaveFile;
		}

		if ( hInternet )
		{
			::InternetCloseHandle( hInternet );
		}

		if ( hURL )
		{
			::InternetCloseHandle( hURL );
		}

		if ( DL_RF_USER_EXIT != pEx->m_dwError )
		{
			CString strDebug;
			strDebug.Format( _T( "Downloading the file is failed. %d, %s, %s, %s, %d" ), pEx->m_dwError, strURL, strClientFullPathFileName, __FILEW__, __LINE__ );
			DLSendMessage_ForDebug( strDebug );
		}

		pEx->Delete();

		return false;
	}

	return true;
}
