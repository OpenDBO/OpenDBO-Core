#include "precomp_ntlflasher.h"
#include "NtlFlasherManager.h"
#include "NtlFlasher.h"

CNtlFlasherManager::CNtlFlasherManager(VOID)
{
	memset( &m_d3dpp, 0, sizeof( D3DPRESENT_PARAMETERS ) );
	m_pStateBlock = NULL;
	m_hWnd = 0;

	m_nScreenWidth = 0;
	m_nScreenHeight = 0;
}

CNtlFlasherManager::~CNtlFlasherManager(VOID)
{
	
}

BOOL CNtlFlasherManager::CreateInstance( HWND hWnd )
{
	if( !( m_pRenderer = *GRendererD3D9::CreateRenderer() ) )
		return FALSE;

	m_hWnd = hWnd;

	m_pFontLib = *NTL_NEW GFxFontLib;
	m_pFontMap = *NTL_NEW GFxFontMap;

	SetVideoMode();
	
	m_hDeviceRestore = CNtlDeviceRestoreHandler::GetInstance()->LinkDeviceRestore( this, &CNtlFlasherManager::DeviceRestoreCallback );
	m_hDeviceRelease = CNtlDeviceRestoreHandler::GetInstance()->LinkDeviceRelease( this, &CNtlFlasherManager::DeviceReleaseCallback );
	
	return TRUE;
}

VOID CNtlFlasherManager::DeleteInstance(VOID)
{
	m_pFontLib = 0;
	m_pFontMap = 0;

	CNtlDeviceRestoreHandler::GetInstance()->UnLinkDeviceRestore( m_hDeviceRestore );
	CNtlDeviceRestoreHandler::GetInstance()->UnLinkDeviceRelease( m_hDeviceRelease );

	m_pRenderer = NULL;
}

VOID CNtlFlasherManager::ResetRenderer(VOID)
{
	if( m_pRenderer && m_bRendererEnable )
	{
		m_pRenderer->ResetVideoMode();	
		
		if( m_pStateBlock )
		{
			m_pStateBlock->Release();
			m_pStateBlock = NULL;
		}

		m_bRendererEnable = FALSE;
	}
}

VOID CNtlFlasherManager::SetVideoMode(VOID)
{
	RwVideoMode videomodeinfo;
	RwEngineGetVideoModeInfo( &videomodeinfo, RwEngineGetCurrentVideoMode() );

	m_d3dpp.Windowed			= ( videomodeinfo.flags & rwVIDEOMODEEXCLUSIVE ) ? FALSE : TRUE;
	m_d3dpp.SwapEffect			= D3DSWAPEFFECT_DISCARD;
	m_d3dpp.BackBufferFormat	= D3DFMT_R8G8B8;
	m_d3dpp.BackBufferWidth		= videomodeinfo.width;
	m_d3dpp.BackBufferHeight	= videomodeinfo.height;

	LPDIRECT3DDEVICE9 pDevice = (LPDIRECT3DDEVICE9)RwD3D9GetCurrentD3DDevice();

	m_pRenderer->SetDependentVideoMode( pDevice, &m_d3dpp, 0, m_hWnd );
	pDevice->CreateStateBlock( D3DSBT_ALL, &m_pStateBlock );

	m_bRendererEnable = TRUE;
}

INT CNtlFlasherManager::DeviceRestoreCallback(VOID)
{
	SetVideoMode();	

	//while( TRUE )
	//{
	//	GRendererD3D9::DisplayStatus eStatus = m_pRenderer->CheckDisplayStatus();

	//	if( eStatus != GRendererD3D9::DisplayStatus_Ok )
	//	{
	//		::Sleep( 10 );			
	//	}
	//	else
	//	{
	//		break;
	//	}
	//}	
	
	return 1;
}

INT CNtlFlasherManager::DeviceReleaseCallback(VOID)
{
	ResetRenderer();
	return 1;
}

VOID CNtlFlasherManager::SetFontLibData( std::string& strLibFileName )
{
	GFxLoader loader;
	GPtr<FlasherFileOpener> pFileOpener = *NTL_NEW FlasherFileOpener;
	loader.SetFileOpener( pFileOpener );

	GPtr<GFxMovieDef> movieDef = *loader.CreateMovie( strLibFileName.c_str() );
	m_pFontLib->AddFontsFrom( movieDef );
}

VOID CNtlFlasherManager::SetFontMapData( std::string& strMapName, std::string& strFontname, std::string& strStyle )
{
	GFxFontMap::MapFontFlags FontFlags = GFxFontMap::MFF_Normal;

	if( strStyle == "bold" )
		FontFlags = GFxFontMap::MFF_Bold;
	else if( strStyle == "italic" )
		FontFlags = GFxFontMap::MFF_Italic;
	else if( strStyle == "bolditalic" )
		FontFlags = GFxFontMap::MFF_BoldItalic;

	//m_pFontMap->MapFont( L"$NormalFont", L"¼Ò¸ÁM", FontFlags );	

	INT nLen = (INT)strlen( strMapName.c_str() );
	WCHAR pBuffer[1024];
	::MultiByteToWideChar(GetACP(), 0, strMapName.c_str(), -1, pBuffer, nLen+1);
	std::wstring wstrMapName = pBuffer;

	nLen = (INT)strlen( strFontname.c_str() );
	::MultiByteToWideChar(GetACP(), 0, strFontname.c_str(), -1, pBuffer, nLen+1);
	std::wstring wstrFontName = pBuffer;

	m_pFontMap->MapFont( wstrMapName.c_str(), wstrFontName.c_str(), FontFlags );	
}

CNtlFlasherManager* CNtlFlasherManager::GetInstance(VOID)
{
	static CNtlFlasherManager intance;
	return &intance;
}
