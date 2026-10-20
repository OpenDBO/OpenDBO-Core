/*****************************************************************************
*
* File			: DLDBOSettingDlg.cpp
* Author		:
* Copyright		: (주) NTL
* Date			:
* Abstract		:
*****************************************************************************
* Desc          :
*
*****************************************************************************/


#include "stdafx.h"
#include "DLDBOSettingDlg.h"
#include "DLMessageDef.h"
#include "DLUtility.h"
#include "DLSystemEnv.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//////////////////////////////////////////////////////////////////////////
//
// Layout of the option dialog (350 x 500)
//
//////////////////////////////////////////////////////////////////////////


namespace
{
	struct sSLIDER_LAYOUT
	{
		int		nButtonY;
		int		nSliderY;
		int		nRangeMax;
	};

	const sSLIDER_LAYOUT s_asSliderLayout[CDLDBOSettingDlg::eSLIDER_COUNT] =
	{
		{ 122, 118, 4	},		// eSLIDER_GAMMA
		{ 150, 146, 3	},		// eSLIDER_QUALITY
		{ 271, 267, 100	},		// eSLIDER_MAIN_VOLUME
		{ 298, 294, 100	},		// eSLIDER_BGM_VOLUME
		{ 322, 319, 100	},		// eSLIDER_EFFECT_VOLUME
		{ 350, 346, 100	},		// eSLIDER_ENV_VOLUME
	};

	const unsigned int s_auiVolumeKey[4] =
	{
		DL_STORAGE_SOUND_MAIN_VOLUME,
		DL_STORAGE_SOUND_BACK_VOLUME,
		DL_STORAGE_SOUND_EFFECT_VOLUME,
		DL_STORAGE_SOUND_ENV_VOLUME,
	};

	// Graphic keys written by the low / normal / high presets
	enum ePRESET_VALUE_TYPE
	{
		ePRESET_VALUE_INT,
		ePRESET_VALUE_BOOL
	};

	const int PRESET_KEY_COUNT	= 10;
	const int PRESET_COUNT		= 3;

	const unsigned int s_auiPresetKey[PRESET_KEY_COUNT] =
	{
		DL_STORAGE_GRAPHIC_TERRAIN_RANGE,
		DL_STORAGE_GRAPHIC_OBJECT_RANGE,
		DL_STORAGE_GRAPHIC_TERRAIN_SHADOW,
		DL_STORAGE_GRAPHIC_WATER_EFFECT,
		DL_STORAGE_GRAPHIC_CHARACTER_SHADOW,
		DL_STORAGE_GRAPHIC_MAGIC_EFFECT,
		DL_STORAGE_GRAPHIC_SHADER_HDR,
		DL_STORAGE_GRAPHIC_CHARACTER_EDGE,
		DL_STORAGE_GRAPHIC_WEATHER_EFFECT,
		DL_STORAGE_GRAPHIC_TEXTURE_LEVEL,
	};

	const int s_anPresetKeyType[PRESET_KEY_COUNT] =
	{
		ePRESET_VALUE_INT, ePRESET_VALUE_INT, ePRESET_VALUE_BOOL, ePRESET_VALUE_BOOL, ePRESET_VALUE_BOOL,
		ePRESET_VALUE_INT, ePRESET_VALUE_BOOL, ePRESET_VALUE_BOOL, ePRESET_VALUE_INT, ePRESET_VALUE_INT,
	};

	const int s_anPresetValue[PRESET_COUNT][PRESET_KEY_COUNT] =
	{
		{ 0, 0, 0, 0, 0, 1, 0, 0, 0, 0 },		// 낮음
		{ 2, 2, 1, 0, 1, 2, 0, 1, 0, 1 },		// 보통
		{ 3, 3, 1, 1, 1, 3, 1, 1, 1, 2 },		// 높음
	};

	// Texts drawn by OnPaint
	struct sLABEL
	{
		unsigned int	uiMsgID;
		bool			bBold;
		COLORREF		clrText;
		RECT			rc;
		UINT			uiFormat;
	};

	const COLORREF LABEL_WHITE	= RGB( 255, 255, 255 );
	const COLORREF LABEL_YELLOW	= RGB( 255, 255, 0 );
	const COLORREF LABEL_ORANGE	= RGB( 211, 144, 3 );

	const sLABEL s_asLabel[] =
	{
		{ eDL_MSG_OPTION_TITLE,				true,	LABEL_WHITE,	{  26,   4, 126,  24 }, DT_LEFT },
		{ eDL_MSG_OPTION_GRAPHIC,			true,	LABEL_YELLOW,	{  26,  64, 126,  84 }, DT_LEFT },
		{ eDL_MSG_OPTION_RESOLUTION,		false,	LABEL_WHITE,	{  37,  90, 137, 110 }, DT_LEFT },
		{ eDL_MSG_OPTION_GAMMA,				false,	LABEL_WHITE,	{  37, 120, 137, 140 }, DT_LEFT },
		{ eDL_MSG_OPTION_GRAPHIC_QUALITY,	false,	LABEL_WHITE,	{  37, 150, 137, 170 }, DT_LEFT },
		{ eDL_MSG_OPTION_QUALITY_LOW,		false,	LABEL_ORANGE,	{ 157, 177, 257, 197 }, DT_LEFT },
		{ eDL_MSG_OPTION_QUALITY_MIDDLE,	false,	LABEL_ORANGE,	{ 199, 177, 299, 197 }, DT_LEFT },
		{ eDL_MSG_OPTION_QUALITY_HIGH,		false,	LABEL_ORANGE,	{ 240, 177, 340, 197 }, DT_LEFT },
		{ eDL_MSG_OPTION_QUALITY_USER,		false,	LABEL_ORANGE,	{ 276, 177, 376, 197 }, DT_LEFT },
		{ eDL_MSG_OPTION_WINDOW_MODE,		false,	LABEL_WHITE,	{  37, 192, 137, 212 }, DT_LEFT },
		{ eDL_MSG_OPTION_SOUND,				true,	LABEL_YELLOW,	{  26, 241, 126, 261 }, DT_LEFT },
		{ eDL_MSG_OPTION_MAIN_VOLUME,		false,	LABEL_WHITE,	{  37, 271, 137, 291 }, DT_LEFT },
		{ eDL_MSG_OPTION_BGM_VOLUME,		false,	LABEL_WHITE,	{  37, 298, 137, 318 }, DT_LEFT },
		{ eDL_MSG_OPTION_EFFECT_VOLUME,		false,	LABEL_WHITE,	{  37, 322, 137, 342 }, DT_LEFT },
		{ eDL_MSG_OPTION_ENV_VOLUME,		false,	LABEL_WHITE,	{  37, 350, 137, 370 }, DT_LEFT },
		{ eDL_MSG_OPTION_DESC,				true,	LABEL_YELLOW,	{  26, 394, 326, 444 }, DT_WORDBREAK },
	};
}


//////////////////////////////////////////////////////////////////////////
//
// CDLDBOSettingDlg
//
//////////////////////////////////////////////////////////////////////////


CDLDBOSettingDlg::CDLDBOSettingDlg( CWnd* pParent )
: CBitmapDlg( IDD_DBO_SETTING_DLG, pParent )
{
	m_pFontBold = NULL;
	m_pFont = NULL;
}

CDLDBOSettingDlg::~CDLDBOSettingDlg( void )
{
	if ( m_pFontBold )
	{
		delete m_pFontBold;
		m_pFontBold = NULL;
	}

	if ( m_pFont )
	{
		delete m_pFont;
		m_pFont = NULL;
	}
}

void CDLDBOSettingDlg::DoDataExchange( CDataExchange* pDX )
{
	CBitmapDlg::DoDataExchange( pDX );

	DDX_Control( pDX, IDC_SETTING_CLOSE_X, m_ctrCloseX );
	DDX_Control( pDX, IDC_SETTING_RESOLUTION, m_ctrResolution );

	DDX_Control( pDX, IDC_SETTING_GAMMA_LEFT, m_asSlider[eSLIDER_GAMMA].ctrLeft );
	DDX_Control( pDX, IDC_SETTING_GAMMA, m_asSlider[eSLIDER_GAMMA].ctrSlider );
	DDX_Control( pDX, IDC_SETTING_GAMMA_RIGHT, m_asSlider[eSLIDER_GAMMA].ctrRight );

	DDX_Control( pDX, IDC_SETTING_QUALITY_LEFT, m_asSlider[eSLIDER_QUALITY].ctrLeft );
	DDX_Control( pDX, IDC_SETTING_QUALITY, m_asSlider[eSLIDER_QUALITY].ctrSlider );
	DDX_Control( pDX, IDC_SETTING_QUALITY_RIGHT, m_asSlider[eSLIDER_QUALITY].ctrRight );

	DDX_Control( pDX, IDC_SETTING_WINDOW_MODE, m_ctrWindowMode );

	DDX_Control( pDX, IDC_SETTING_MAIN_LEFT, m_asSlider[eSLIDER_MAIN_VOLUME].ctrLeft );
	DDX_Control( pDX, IDC_SETTING_MAIN_VOLUME, m_asSlider[eSLIDER_MAIN_VOLUME].ctrSlider );
	DDX_Control( pDX, IDC_SETTING_MAIN_RIGHT, m_asSlider[eSLIDER_MAIN_VOLUME].ctrRight );

	DDX_Control( pDX, IDC_SETTING_BGM_LEFT, m_asSlider[eSLIDER_BGM_VOLUME].ctrLeft );
	DDX_Control( pDX, IDC_SETTING_BGM_VOLUME, m_asSlider[eSLIDER_BGM_VOLUME].ctrSlider );
	DDX_Control( pDX, IDC_SETTING_BGM_RIGHT, m_asSlider[eSLIDER_BGM_VOLUME].ctrRight );

	DDX_Control( pDX, IDC_SETTING_EFFECT_LEFT, m_asSlider[eSLIDER_EFFECT_VOLUME].ctrLeft );
	DDX_Control( pDX, IDC_SETTING_EFFECT_VOLUME, m_asSlider[eSLIDER_EFFECT_VOLUME].ctrSlider );
	DDX_Control( pDX, IDC_SETTING_EFFECT_RIGHT, m_asSlider[eSLIDER_EFFECT_VOLUME].ctrRight );

	DDX_Control( pDX, IDC_SETTING_ENV_LEFT, m_asSlider[eSLIDER_ENV_VOLUME].ctrLeft );
	DDX_Control( pDX, IDC_SETTING_ENV_VOLUME, m_asSlider[eSLIDER_ENV_VOLUME].ctrSlider );
	DDX_Control( pDX, IDC_SETTING_ENV_RIGHT, m_asSlider[eSLIDER_ENV_VOLUME].ctrRight );

	DDX_Control( pDX, IDC_SETTING_APPLY, m_ctrApply );
	DDX_Control( pDX, IDC_SETTING_DEFAULT, m_ctrDefault );
	DDX_Control( pDX, IDC_SETTING_CLOSE, m_ctrClose );
}

BOOL CDLDBOSettingDlg::OnInitDialog( void )
{
	CBitmapDlg::OnInitDialog();

	// Fonts of the labels
	CString strFaceName = DLGetMessage_ToUI( eDL_MSG_UI_FONT_NAME );

	m_pFontBold = CreateDlgFont( true, strFaceName, 13 );
	m_pFont = CreateDlgFont( false, strFaceName, 13 );

	// Background image 설정
	MoveWindow( 0, 0, 350, 500 );

	EnableEasyMove();
	SetBitmap( IDB_SETTING_BACK );
	SetStyle( LO_RESIZE );
	SetTransparentColor( RGB( 0, 255, 0 ) );

	// Parent window 중앙으로 이동
	CRect rcParent;
	CWnd* pParent = GetParent();
	if ( pParent )
	{
		pParent->GetWindowRect( &rcParent );
	}
	else
	{
		rcParent.SetRect( 0, 0, GetSystemMetrics( SM_CXSCREEN ), GetSystemMetrics( SM_CYSCREEN ) );
	}

	MoveWindow( rcParent.left + rcParent.Width() / 2 - 175, rcParent.top + rcParent.Height() / 2 - 250, 350, 500 );

	// Close 버튼
	m_ctrCloseX.SetButtonImg( IDB_DEV_CLOSE_UP, IDB_DEV_CLOSE_DOWN, IDB_DEV_CLOSE_FOC );
	m_ctrCloseX.MoveWindow( 331, 5, 14, 14 );

	// 해상도
	m_ctrResolution.MoveWindow( 157, 84, 153, 21 );

	// Sliders
	for ( int i = 0; i < eSLIDER_COUNT; ++i )
	{
		sSLIDER_GROUP& sGroup = m_asSlider[i];
		const sSLIDER_LAYOUT& sLayout = s_asSliderLayout[i];

		sGroup.ctrLeft.SetButtonImg( IDB_SETTING_LEFT_UP, IDB_SETTING_LEFT_UP, IDB_SETTING_LEFT_FOC );
		sGroup.ctrLeft.MoveWindow( 157, sLayout.nButtonY, 12, 12 );

		sGroup.ctrSlider.InitSlider( IDB_SLIDER_THUMB, IDB_SLIDER_CHANNEL );
		sGroup.ctrSlider.SetRange( 0, sLayout.nRangeMax, FALSE );
		sGroup.ctrSlider.MoveWindow( 170, sLayout.nSliderY, 127, 20 );

		sGroup.ctrRight.SetButtonImg( IDB_SETTING_RIGHT_UP, IDB_SETTING_RIGHT_UP, IDB_SETTING_RIGHT_FOC );
		sGroup.ctrRight.MoveWindow( 299, sLayout.nButtonY, 12, 12 );

		// 창 모드 check box sits between the graphic and the sound sliders
		if ( eSLIDER_QUALITY == i )
		{
			m_ctrWindowMode.MoveWindow( 93, 193, 12, 12 );
		}
	}

	// 적용 / 기본값 / 닫기
	CImageTextButton* apButton[3] = { &m_ctrApply, &m_ctrDefault, &m_ctrClose };
	int anButtonX[3] = { 29, 138, 247 };
	unsigned int auiButtonMsg[3] = { eDL_MSG_OPTION_APPLY, eDL_MSG_OPTION_DEFAULT, eDL_MSG_OPTION_CLOSE };

	for ( int i = 0; i < 3; ++i )
	{
		apButton[i]->SetButtonImg( IDB_SETTING_BTN_UP, IDB_SETTING_BTN_DOWN, IDB_SETTING_BTN_FOC );
		apButton[i]->MoveWindow( anButtonX[i], 464, 72, 25 );
		apButton[i]->SetTextColor( RGB( 255, 255, 255 ), RGB( 100, 100, 100 ) );
		apButton[i]->SetButtonText( DLGetMessage_ToUI( auiButtonMsg[i] ) );
	}

	RefreshResolutionList();

	CDLSystemEnv::GetInstance()->Load();

	LoadOption();

	return TRUE;
}

CFont* CDLDBOSettingDlg::CreateDlgFont( bool bBold, const CString& strFaceName, int nHeight )
{
	LOGFONT sLogFont;
	memset( &sLogFont, 0, sizeof( sLogFont ) );

	_tcsncpy_s( sLogFont.lfFaceName, LF_FACESIZE, strFaceName, _TRUNCATE );
	sLogFont.lfHeight	= nHeight;
	sLogFont.lfWeight	= bBold ? FW_BOLD : FW_NORMAL;
	sLogFont.lfCharSet	= DEFAULT_CHARSET;

	CFont* pFont = new CFont;
	if ( !pFont->CreateFontIndirect( &sLogFont ) )
	{
		delete pFont;
		return NULL;
	}

	return pFont;
}

void CDLDBOSettingDlg::RefreshResolutionList( void )
{
	m_ctrResolution.ResetContent();
	m_defResolutionList.clear();

	bool bWindowMode = ( BST_CHECKED == m_ctrWindowMode.GetCheck() );

	EnumResolution( m_defResolutionList, bWindowMode );

	for ( mapdef_ResolutionList::iterator it = m_defResolutionList.begin(); it != m_defResolutionList.end(); ++it )
	{
		int nIndex = m_ctrResolution.AddString( it->first );
		m_ctrResolution.SetItemDataPtr( nIndex, &it->second );
	}
}

void CDLDBOSettingDlg::EnumResolution( mapdef_ResolutionList& defList, bool bWindowMode )
{
	HDC hDC = ::CreateDC( _T( "DISPLAY" ), NULL, NULL, NULL );

	int nScreenWidth	= ::GetDeviceCaps( hDC, HORZRES );
	int nScreenHeight	= ::GetDeviceCaps( hDC, VERTRES );
	int nScreenBits		= ::GetDeviceCaps( hDC, BITSPIXEL );
	int nScreenRefresh	= ::GetDeviceCaps( hDC, VREFRESH );

	::DeleteDC( hDC );

	DEVMODE sDevMode;
	memset( &sDevMode, 0, sizeof( sDevMode ) );
	sDevMode.dmSize = sizeof( sDevMode );

	for ( DWORD dwMode = 0; ::EnumDisplaySettings( NULL, dwMode, &sDevMode ); ++dwMode )
	{
		int nWidth	= (int)sDevMode.dmPelsWidth;
		int nHeight	= (int)sDevMode.dmPelsHeight;
		int nBits	= (int)sDevMode.dmBitsPerPel;

		if ( nWidth < 1024 || nHeight < 768 )
		{
			continue;
		}

		if ( bWindowMode )
		{
			// The window has to fit on the desktop and use its color depth
			if ( nWidth > nScreenWidth || nHeight > nScreenHeight || nBits != nScreenBits )
			{
				continue;
			}
		}
		else
		{
			if ( nBits < 16 )
			{
				continue;
			}
		}

		if ( (int)sDevMode.dmDisplayFrequency != nScreenRefresh )
		{
			continue;
		}

		CString strKey;
		strKey.Format( _T( "%d*%d*%dBit" ), nWidth, nHeight, nBits );

		if ( defList.find( strKey ) == defList.end() )
		{
			defList[strKey] = sDevMode;
		}
	}
}

DEVMODE* CDLDBOSettingDlg::FindResolution( int nWidth, int nHeight, int nDepth )
{
	CString strKey;
	strKey.Format( _T( "%d*%d*%dBit" ), nWidth, nHeight, nDepth );

	mapdef_ResolutionList::iterator it = m_defResolutionList.find( strKey );
	if ( it != m_defResolutionList.end() )
	{
		return &it->second;
	}

	// Same size with another color depth
	CString strPrefix;
	strPrefix.Format( _T( "%d*%d*" ), nWidth, nHeight );

	for ( it = m_defResolutionList.begin(); it != m_defResolutionList.end(); ++it )
	{
		if ( 0 == it->first.Find( strPrefix ) )
		{
			return &it->second;
		}
	}

	return NULL;
}

void CDLDBOSettingDlg::LoadOption( void )
{
	CDLSystemEnv* pEnv = CDLSystemEnv::GetInstance();

	// 해상도
	DEVMODE* pDevMode = FindResolution( pEnv->GetInt( DL_STORAGE_GRAPHIC_SELECT_VIDEOWIDTH ),
										pEnv->GetInt( DL_STORAGE_GRAPHIC_SELECT_VIDEOHEIGHT ),
										pEnv->GetInt( DL_STORAGE_GRAPHIC_SELECT_VIDEODEPTH ) );
	if ( pDevMode )
	{
		int nCount = m_ctrResolution.GetCount();

		for ( int i = 0; i < nCount; ++i )
		{
			if ( m_ctrResolution.GetItemDataPtr( i ) == pDevMode )
			{
				m_ctrResolution.SetCurSel( i );
				break;
			}
		}
	}

	// 밝기 / 품질
	m_asSlider[eSLIDER_GAMMA].ctrSlider.SetPos( pEnv->GetInt( DL_STORAGE_GRAPHIC_GAMMA ) );
	m_asSlider[eSLIDER_GAMMA].ctrSlider.Invalidate( TRUE );

	m_asSlider[eSLIDER_QUALITY].ctrSlider.SetPos( pEnv->GetInt( DL_STORAGE_GRAPHIC_PRESET ) );
	m_asSlider[eSLIDER_QUALITY].ctrSlider.Invalidate( TRUE );

	// 창 모드
	m_ctrWindowMode.SetCheck( pEnv->GetBool( DL_STORAGE_GRAPHIC_WINDOW_MODE ) ? BST_CHECKED : BST_UNCHECKED );

	// 볼륨
	for ( int i = 0; i < 4; ++i )
	{
		CBitmapSlider& ctrSlider = m_asSlider[eSLIDER_MAIN_VOLUME + i].ctrSlider;

		ctrSlider.SetPos( (int)( pEnv->GetFloat( s_auiVolumeKey[i] ) * 100.0 ) );
		ctrSlider.Invalidate( TRUE );
	}
}

bool CDLDBOSettingDlg::SaveOption( void )
{
	CDLSystemEnv* pEnv = CDLSystemEnv::GetInstance();

	int nSel = m_ctrResolution.GetCurSel();
	DEVMODE* pDevMode = ( CB_ERR == nSel ) ? NULL : (DEVMODE*)m_ctrResolution.GetItemDataPtr( nSel );

	if ( NULL == pDevMode || (DEVMODE*)-1 == pDevMode )
	{
		DLSendMessage_ToUser( eDL_MSG_SELECT_RESOLUTION );

		return false;
	}

	// 해상도
	pEnv->SetInt( DL_STORAGE_GRAPHIC_SELECT_VIDEOWIDTH, (int)pDevMode->dmPelsWidth );
	pEnv->SetInt( DL_STORAGE_GRAPHIC_SELECT_VIDEOHEIGHT, (int)pDevMode->dmPelsHeight );
	pEnv->SetInt( DL_STORAGE_GRAPHIC_SELECT_VIDEODEPTH, (int)pDevMode->dmBitsPerPel );

	// 밝기 (the retail launcher stores the slider position as is)
	pEnv->SetInt( DL_STORAGE_GRAPHIC_GAMMA, m_asSlider[eSLIDER_GAMMA].ctrSlider.GetPos() );

	// 품질 preset; 사용자 설정(3) keeps the individual graphic keys
	int nPreset = m_asSlider[eSLIDER_QUALITY].ctrSlider.GetPos();
	pEnv->SetInt( DL_STORAGE_GRAPHIC_PRESET, nPreset );

	if ( nPreset >= 0 && nPreset < PRESET_COUNT )
	{
		for ( int i = 0; i < PRESET_KEY_COUNT; ++i )
		{
			if ( ePRESET_VALUE_BOOL == s_anPresetKeyType[i] )
			{
				pEnv->SetBool( s_auiPresetKey[i], 0 != s_anPresetValue[nPreset][i] );
			}
			else
			{
				pEnv->SetInt( s_auiPresetKey[i], s_anPresetValue[nPreset][i] );
			}
		}
	}

	// 창 모드
	pEnv->SetBool( DL_STORAGE_GRAPHIC_WINDOW_MODE, BST_CHECKED == m_ctrWindowMode.GetCheck() );

	// 볼륨
	for ( int i = 0; i < 4; ++i )
	{
		pEnv->SetFloat( s_auiVolumeKey[i], (float)( m_asSlider[eSLIDER_MAIN_VOLUME + i].ctrSlider.GetPos() / 100.0 ) );
	}

	return true;
}

void CDLDBOSettingDlg::SaveOptionFile( void )
{
	// ".\user" 폴더가 없으면 만든다
	WIN32_FIND_DATAA sFindData;
	HANDLE hFind = ::FindFirstFileA( DL_SYSTEM_ENV_FOLDER, &sFindData );

	if ( INVALID_HANDLE_VALUE == hFind || !( sFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) )
	{
		::CreateDirectoryA( DL_SYSTEM_ENV_FOLDER, NULL );
	}

	if ( INVALID_HANDLE_VALUE != hFind )
	{
		::FindClose( hFind );
	}

	CDLSystemEnv::GetInstance()->Save();
}

void CDLDBOSettingDlg::StepSlider( int nSlider, int nStep )
{
	CBitmapSlider& ctrSlider = m_asSlider[nSlider].ctrSlider;

	ctrSlider.SetPos( ctrSlider.GetPos() + nStep );
	ctrSlider.Invalidate( TRUE );
}

void CDLDBOSettingDlg::OnBnClickedClose( void )
{
	OnCancel();
}

void CDLDBOSettingDlg::OnBnClickedApply( void )
{
	if ( SaveOption() )
	{
		SaveOptionFile();
	}
}

void CDLDBOSettingDlg::OnBnClickedDefault( void )
{
	CDLSystemEnv::GetInstance()->Reset();

	LoadOption();

	SaveOptionFile();
}

void CDLDBOSettingDlg::OnBnClickedWindowMode( void )
{
	RefreshResolutionList();
}

void CDLDBOSettingDlg::OnPaint( void )
{
	CPaintDC dc( this );

	dc.SetBkMode( TRANSPARENT );

	for ( int i = 0; i < _countof( s_asLabel ); ++i )
	{
		const sLABEL& sLabel = s_asLabel[i];

		CFont* pFont = sLabel.bBold ? m_pFontBold : m_pFont;
		CFont* pOldFont = pFont ? dc.SelectObject( pFont ) : NULL;

		dc.SetTextColor( sLabel.clrText );

		CRect rc( sLabel.rc );
		CString strText = DLGetMessage_ToUI( sLabel.uiMsgID );

		dc.DrawText( strText, &rc, sLabel.uiFormat );

		if ( pOldFont )
		{
			dc.SelectObject( pOldFont );
		}
	}
}

BEGIN_MESSAGE_MAP( CDLDBOSettingDlg, CBitmapDlg )
	ON_BN_CLICKED( IDC_SETTING_GAMMA_LEFT, &CDLDBOSettingDlg::OnGammaLeft )
	ON_BN_CLICKED( IDC_SETTING_GAMMA_RIGHT, &CDLDBOSettingDlg::OnGammaRight )
	ON_BN_CLICKED( IDC_SETTING_QUALITY_LEFT, &CDLDBOSettingDlg::OnQualityLeft )
	ON_BN_CLICKED( IDC_SETTING_QUALITY_RIGHT, &CDLDBOSettingDlg::OnQualityRight )
	ON_BN_CLICKED( IDC_SETTING_MAIN_LEFT, &CDLDBOSettingDlg::OnMainVolumeLeft )
	ON_BN_CLICKED( IDC_SETTING_MAIN_RIGHT, &CDLDBOSettingDlg::OnMainVolumeRight )
	ON_BN_CLICKED( IDC_SETTING_BGM_LEFT, &CDLDBOSettingDlg::OnBgmVolumeLeft )
	ON_BN_CLICKED( IDC_SETTING_BGM_RIGHT, &CDLDBOSettingDlg::OnBgmVolumeRight )
	ON_BN_CLICKED( IDC_SETTING_EFFECT_LEFT, &CDLDBOSettingDlg::OnEffectVolumeLeft )
	ON_BN_CLICKED( IDC_SETTING_EFFECT_RIGHT, &CDLDBOSettingDlg::OnEffectVolumeRight )
	ON_BN_CLICKED( IDC_SETTING_ENV_LEFT, &CDLDBOSettingDlg::OnEnvVolumeLeft )
	ON_BN_CLICKED( IDC_SETTING_ENV_RIGHT, &CDLDBOSettingDlg::OnEnvVolumeRight )
	ON_BN_CLICKED( IDC_SETTING_CLOSE_X, &CDLDBOSettingDlg::OnBnClickedClose )
	ON_BN_CLICKED( IDC_SETTING_APPLY, &CDLDBOSettingDlg::OnBnClickedApply )
	ON_BN_CLICKED( IDC_SETTING_DEFAULT, &CDLDBOSettingDlg::OnBnClickedDefault )
	ON_BN_CLICKED( IDC_SETTING_CLOSE, &CDLDBOSettingDlg::OnBnClickedClose )
	ON_WM_PAINT()
	ON_BN_CLICKED( IDC_SETTING_WINDOW_MODE, &CDLDBOSettingDlg::OnBnClickedWindowMode )
END_MESSAGE_MAP()
