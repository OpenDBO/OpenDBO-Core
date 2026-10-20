/*****************************************************************************
*
* File			: DLSkinDlg_CT.cpp
* Author		:
* Copyright		: (주) NTL
* Date			:
* Abstract		:
*****************************************************************************
* Desc          : 881x622 skin with the web patch notes on the right, the
*                 big start orb at the bottom left and vertical integrity /
*                 option tabs on the right edge. The whole window is a
*                 caption (WM_NCHITTEST), so it can be dragged anywhere.
*
*****************************************************************************/


#include "stdafx.h"
#include "DLSkinDlg_CT.h"
#include "DBOLauncherDef.h"
#include "DLUtility.h"
#include "DLPatchInfo.h"
#include "DLPatchServerConfig.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//////////////////////////////////////////////////////////////////////////
//
// CDLSkinDlg_CT
//
//////////////////////////////////////////////////////////////////////////


CDLSkinDlg_CT::CDLSkinDlg_CT( void )
{
	m_nIDDownloadProgress = -1;
	m_nIDPatchProgress = -1;

	m_hIcon = NULL;

	m_uiTotalFilesInUpdateFile = 0;
	m_uiCurFilesInUpdateFile = 0;
}

CDLSkinDlg_CT::~CDLSkinDlg_CT( void )
{
	DestroyWindow();
}

bool CDLSkinDlg_CT::CreateDlg( void )
{
	return Create( IDD_SKIN_CT_DLG, NULL ) ? true : false;
}

void CDLSkinDlg_CT::DLMessageProc( eDL_EVENT eMsg, sDL_EVENT_DATA_BASE* pData )
{
	if ( eDL_EVENT_DOWNLOADING_NFY == eMsg )
	{
		sDL_ED_DOWNLOADING_NFY* pEvtData = (sDL_ED_DOWNLOADING_NFY*) pData;

		float fRatio = (float)pEvtData->uiCurUpdateSize / (float)pEvtData->uiFileSize;
		float fCurPos = (float)CT_MAX_PROGRESS_RANGE * fRatio;

		SetBltRateX( m_nIDDownloadProgress, (int)fCurPos, CT_MAX_PROGRESS_RANGE, TRUE );

		static int nRenderCount = 0;
		if ( nRenderCount >= 180 )
		{
			nRenderCount = 0;

			CString strBuffer;

			strBuffer.Format( _T( "DownLoadingFile:%s    %4.2fMb%s%4.2fMb" ), pEvtData->strFileName,
																			  (float)pEvtData->uiCurUpdateSize / (1024.f * 1024.f),
																			  _T( "/" ),
																			  (float)pEvtData->uiFileSize / (1024.f * 1024.f) );
			m_ctrDownload.EraseBack( this );
			m_ctrDownload.SetWindowText( strBuffer );
		}

		nRenderCount++;

		if ( pEvtData->uiCurUpdateSize == pEvtData->uiFileSize && pEvtData->bShowComplete )
		{
			m_ctrDownload.EraseBack( this );
			m_ctrDownload.SetWindowText( _T( "DownLoad Complete" ) );
		}

		UpdateDialog();
	}
	else if ( eDL_EVENT_PATCH_START_NFY == eMsg )
	{
		sDL_ED_PATCH_START_NFY* pEvtData = (sDL_ED_PATCH_START_NFY*) pData;

		m_uiTotalFilesInUpdateFile = pEvtData->uiPatchFileCnt;
		m_uiCurFilesInUpdateFile = 0;

		UpdateDialog();
	}
	else if ( eDL_EVENT_PATCH_UPDATE_NFY == eMsg )
	{
		sDL_ED_PATCH_UPDATE_NFY* pEvtData = (sDL_ED_PATCH_UPDATE_NFY*) pData;

		CString strBuffer;
		strBuffer.Format( _T( "%s  %d%s" ), pEvtData->strPatchFileName, (int)( (float)m_uiCurFilesInUpdateFile / (float)m_uiTotalFilesInUpdateFile * 100.f ), _T( "%" ) );

		m_ctrPatch.EraseBack( this );
		m_ctrPatch.SetWindowText( strBuffer );

		m_uiCurFilesInUpdateFile++;

		float fRatio = (float)m_uiCurFilesInUpdateFile / (float)m_uiTotalFilesInUpdateFile;
		float fCurPos = (float)CT_MAX_PROGRESS_RANGE * fRatio;

		SetBltRateX( m_nIDPatchProgress, (int)fCurPos, CT_MAX_PROGRESS_RANGE, TRUE );

		UpdateDialog();
	}
	else if ( eDL_EVENT_PATCH_COMPLETE_NFY == eMsg )
	{
		m_ctrPatch.EraseBack( this );
		m_ctrPatch.SetWindowText( _T( "DBO Patch Complete!!" ) );

		ShowVersion( g_clDLPatchHistory.GetCurVerClient()->Get(), g_clDLPatchHistory.GetPatchFileDate(), g_clDLPatchHistory.GetCurVerLauncher()->Get() );

		UpdateDialog();
	}
	else if ( eDL_EVENT_PATCH_ETC_UPDATE_NFY == eMsg )
	{
		UpdateDialog();
	}
	else if ( eDL_EVENT_ACTIVE_START == eMsg )
	{
		sDL_ED_ACTIVE_START* pEvtData = (sDL_ED_ACTIVE_START*) pData;
		m_ctrGameStart.EnableWindow( pEvtData->bOn ? TRUE : FALSE );

		SetBltRateX( m_nIDDownloadProgress, pEvtData->bOn ? CT_MAX_PROGRESS_RANGE : 0, CT_MAX_PROGRESS_RANGE, TRUE );

		UpdateDialog();
	}
	else if ( eDL_EVENT_ACTIVE_INTEGRITY == eMsg )
	{
		sDL_ED_ACTIVE_INTEGRITY* pEvtData = (sDL_ED_ACTIVE_INTEGRITY*) pData;
		m_ctrGameIntegrity.EnableWindow( pEvtData->bOn ? TRUE : FALSE );

		SetBltRateX( m_nIDPatchProgress, pEvtData->bOn ? CT_MAX_PROGRESS_RANGE : 0, CT_MAX_PROGRESS_RANGE, TRUE );

		UpdateDialog();
	}
	else if ( eDL_EVENT_ACTIVE_OPTION == eMsg )
	{
		sDL_ED_ACTIVE_OPTION* pEvtData = (sDL_ED_ACTIVE_OPTION*) pData;
		m_ctrOption.EnableWindow( pEvtData->bOn ? TRUE : FALSE );

		UpdateDialog();
	}
	else if ( eDL_EVENT_INTEGRITY_START_NFY == eMsg )
	{
		sDL_ED_DO_INTEGRITY_START_NFY* pEvtData = (sDL_ED_DO_INTEGRITY_START_NFY*) pData;

		m_uiTotalFilesInUpdateFile = pEvtData->uiIntegrithFileCnt;
		m_uiCurFilesInUpdateFile = 0;

		UpdateDialog();
	}
	else if ( eDL_EVENT_INTEGRITY_UPDATE_NFY == eMsg )
	{
		sDL_ED_DO_INTEGRITY_UPDATE_NFY* pEvtData = (sDL_ED_DO_INTEGRITY_UPDATE_NFY*) pData;

		CString strBuffer;
		strBuffer.Format( _T( "%s  %d%s" ), pEvtData->strUpdateFileName, (int)( (float)m_uiCurFilesInUpdateFile / (float)m_uiTotalFilesInUpdateFile * 100.f ), _T( "%" ) );

		m_ctrPatch.EraseBack( this );
		m_ctrPatch.SetWindowText( strBuffer );

		m_uiCurFilesInUpdateFile++;

		float fRatio = (float)m_uiCurFilesInUpdateFile / (float)m_uiTotalFilesInUpdateFile;
		float fCurPos = (float)CT_MAX_PROGRESS_RANGE * fRatio;

		SetBltRateX( m_nIDPatchProgress, (int)fCurPos, CT_MAX_PROGRESS_RANGE, TRUE );

		UpdateDialog();
	}
	else if ( eDL_EVENT_INTEGRITY_COMPLETE_NFY == eMsg )
	{
		m_ctrPatch.EraseBack( this );
		m_ctrPatch.SetWindowText( _T( "Checking the DBO integrity is complete." ) );

		UpdateDialog();
	}
	else if ( eDL_EVENT_UPDATE_VERSION_NFY == eMsg )
	{
		sDL_ED_UPDATE_VERSION_NFY* pEvtData = (sDL_ED_UPDATE_VERSION_NFY*) pData;

		ShowVersion( pEvtData->strClinetVer, pEvtData->strDate, pEvtData->strLauncherVer );

		UpdateDialog();
	}
}

void CDLSkinDlg_CT::DoDataExchange( CDataExchange* pDX )
{
	CDLSkinBaseDlg::DoDataExchange( pDX );

	DDX_Control( pDX, IDC_CT_EXPLORER_INFORMATION, m_ctrExplorerInfo );
	DDX_Control( pDX, IDC_CT_DOWNLOAD, m_ctrDownload );
	DDX_Control( pDX, IDC_CT_VERSION_INFO, m_ctrVersionInfo );
	DDX_Control( pDX, IDC_CT_PATCH, m_ctrPatch );
	DDX_Control( pDX, IDC_CT_CANCEL, m_ctrCancel );
	DDX_Control( pDX, IDC_CT_GAME_START, m_ctrGameStart );
	DDX_Control( pDX, IDC_CT_GAME_TEST, m_ctrGameIntegrity );
	DDX_Control( pDX, IDC_CT_OPTION, m_ctrOption );
}

BOOL CDLSkinDlg_CT::OnInitDialog( void )
{
	CDLSkinBaseDlg::OnInitDialog();

	MoveWindow( 0, 0, 881, 622 );

	// Background image 설정
	EnableEasyMove();
	SetBitmap( IDB_CT_BACK );
	SetStyle( LO_RESIZE );
	SetTransparentColor( RGB( 255, 0, 255 ) );

	int nScreenX = GetSystemMetrics( SM_CXSCREEN ) / 2 - 350;
	int nScreenY = GetSystemMetrics( SM_CYSCREEN ) / 2 - 250;

	// 화면 중앙으로 dialog 이동
	MoveWindow( nScreenX, nScreenY, 881, 622 );

	// Explorer 설정
	m_ctrExplorerInfo.MoveWindow( 402, 63, 402, 449 );

	// 출력 문자열 설정
	m_ctrDownload.MoveWindow( 428, 511, 369, 14 );
	m_ctrDownload.SetWindowText( _T("") );

	m_ctrVersionInfo.MoveWindow( 417, 17, 356, 14 );
	m_ctrVersionInfo.m_clrText = RGB( 0, 0, 0 );
	m_ctrVersionInfo.SetWindowText( _T("") );

	m_ctrPatch.MoveWindow( 428, 536, 369, 14 );
	m_ctrPatch.SetWindowText( _T("") );

	// Cancel 버튼 설정
	m_ctrCancel.SetButtonImg( IDB_CT_CLOSE_UP, IDB_CT_CLOSE_DOWN, IDB_CT_CLOSE_FOC );
	m_ctrCancel.MoveWindow( 827, 20, 32, 32 );

	// Start 버튼 설정
	m_ctrGameStart.SetButtonImg( IDB_CT_START_UP, IDB_CT_START_DOWN, IDB_CT_START_FOC, IDB_CT_START_DISABLE );
	m_ctrGameStart.MoveWindow( 153, 481, 124, 124 );
	m_ctrGameStart.EnableWindow( FALSE );

	// Integrity 버튼 설정
	m_ctrGameIntegrity.SetButtonImg( IDB_CT_INTEGRITY_UP, IDB_CT_INTEGRITY_DOWN, IDB_CT_INTEGRITY_FOC, IDB_CT_INTEGRITY_DISABLE );
	m_ctrGameIntegrity.MoveWindow( 840, 314, 37, 99 );
	m_ctrGameIntegrity.EnableWindow( FALSE );

	// Option 버튼 설정
	m_ctrOption.SetButtonImg( IDB_CT_OPTION_UP, IDB_CT_OPTION_DOWN, IDB_CT_OPTION_FOC, IDB_CT_OPTION_DISABLE );
	m_ctrOption.MoveWindow( 840, 415, 37, 99 );
	m_ctrOption.EnableWindow( FALSE );

	// Download progress bar 설정
	m_nIDDownloadProgress = AddBitmap( IDB_CT_PROGRESS, 431, 527, 363, 7 );
	SetBltRateX( m_nIDDownloadProgress, 0, CT_MAX_PROGRESS_RANGE );

	// Patch progress bar 설정
	m_nIDPatchProgress = AddBitmap( IDB_CT_PROGRESS, 431, 552, 363, 7 );
	SetBltRateX( m_nIDPatchProgress, 0, CT_MAX_PROGRESS_RANGE );

	// Icon 설정
	m_hIcon = AfxGetApp()->LoadIcon( IDR_MAINFRAME );
	SetIcon( m_hIcon, TRUE );
	SetIcon( m_hIcon, FALSE );

	// HTTP
	CString strHtmlInformation = g_clDLPatchServerConfig.GetWebPatchHistoryPath();
	m_ctrExplorerInfo.Navigate( strHtmlInformation, NULL, NULL, NULL, NULL );

	// Current version
	ShowVersion( g_clDLPatchHistory.GetCurVerClient()->Get(), g_clDLPatchHistory.GetPatchFileDate(), g_clDLPatchHistory.GetCurVerLauncher()->Get() );

	// Caption
	SetWindowText( g_clDLPatchServerConfig.GetCaptionName() );

	UpdateDialog();

	return TRUE;
}

BOOL CDLSkinDlg_CT::DestroyWindow( void )
{
	if ( m_hIcon )
	{
		::DestroyIcon( m_hIcon );
		m_hIcon = NULL;
	}

	return CDLSkinBaseDlg::DestroyWindow();
}

void CDLSkinDlg_CT::ShowVersion( const CString& strClientVer, const CString& strDate, const CString& strLauncherVer )
{
	CString strLastUpdate;
	strLastUpdate.Format( _T( "CurrentVersion: Client: %s(%s), Launcher: %s" ), strClientVer, strDate, strLauncherVer );

	m_ctrVersionInfo.EraseBack( this );
	m_ctrVersionInfo.SetWindowText( strLastUpdate );
}

void CDLSkinDlg_CT::OnPaint( void )
{
	if ( IsIconic() )
	{
		CPaintDC dc( this );

		SendMessage( WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0 );

		int cxIcon = GetSystemMetrics( SM_CXICON );
		int cyIcon = GetSystemMetrics( SM_CYICON );

		CRect rect;
		GetClientRect( &rect );

		int x = ( rect.Width() - cxIcon + 1 ) / 2;
		int y = ( rect.Height() - cyIcon + 1 ) / 2;

		dc.DrawIcon( x, y, m_hIcon );
	}
	else
	{
		CDLSkinBaseDlg::OnPaint();
	}
}

HCURSOR CDLSkinDlg_CT::OnQueryDragIcon( void )
{
	return (HCURSOR)m_hIcon;
}

void CDLSkinDlg_CT::OnClose( void )
{
	DoUserExit();
}

LRESULT CDLSkinDlg_CT::OnNcHitTest( CPoint point )
{
	return HTCAPTION;
}

void CDLSkinDlg_CT::OnBnClickedCTGameStart( void )
{
	DoGameStart();
}

void CDLSkinDlg_CT::OnBnClickedCTCancel( void )
{
	DoUserExit();
}

void CDLSkinDlg_CT::OnBnClickedCTIntegrityCheck( void )
{
	DoIntegrityCheck();
}

void CDLSkinDlg_CT::OnBnClickedCTOption( void )
{
	DoOption();
}

BEGIN_MESSAGE_MAP( CDLSkinDlg_CT, CBitmapDlg )
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	ON_BN_CLICKED( IDC_CT_GAME_START, &CDLSkinDlg_CT::OnBnClickedCTGameStart )
	ON_BN_CLICKED( IDC_CT_CANCEL, &CDLSkinDlg_CT::OnBnClickedCTCancel )
	ON_BN_CLICKED( IDC_CT_GAME_TEST, &CDLSkinDlg_CT::OnBnClickedCTIntegrityCheck )
	ON_BN_CLICKED( IDC_CT_OPTION, &CDLSkinDlg_CT::OnBnClickedCTOption )
	ON_WM_NCHITTEST()
END_MESSAGE_MAP()
