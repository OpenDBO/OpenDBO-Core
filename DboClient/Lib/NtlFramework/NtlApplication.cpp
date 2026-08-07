#include "precomp_ntlframework.h"
#include "NtlApplication.h"
#include "memoryhandler.h"
#include "NtlTimer.h"
//#include "NtlCoreUtil.h"
#include "NtlCoreApi.h"
#include "NtlDeviceRestoreHandler.h"
#include "NtlLoadingController.h"


CNtlApplication *CNtlApplication::m_pInstance = NULL;
RwUInt32 CNtlApplication::m_uiFrameRate = 1000;


bool minimized = false;

//Resource Arena占쏙옙 Size
#define rsRESOURCESDEFAULTARENASIZE (128 << 20) // 128MB for x64
//File System에서 다룰수 있는 동시에 Open할수 있는 File의 최대 갯수
#define MAX_NB_FILES_PER_FS (200)   

CNtlApplication::CNtlApplication() : m_hWnd(NULL), 
									 m_hAccel(NULL),
									 m_bMinimized(FALSE),
									 m_iWindowPosX(0),
									 m_iWindowPosY(0),
									 m_iScreenWidth(NTL_APP_SCREEN_WIDTH),
									 m_iScreenHeight(NTL_APP_SCREEN_HEIGHT),
									 m_iWindowWidth(NTL_APP_SCREEN_WIDTH), 
									 m_iWindowHeight(NTL_APP_SCREEN_HEIGHT),
									 m_iUserWidth(NTL_APP_SCREEN_WIDTH),
									 m_iUserHeight(NTL_APP_SCREEN_HEIGHT),
									 m_iBitDepth(NTL_APP_BIT_DEPTH),
									 m_bFullScreen(FALSE),
									m_bShowFps(false),
									 m_bQuit(FALSE),
									 m_fFps(0.f),
									 m_fElapsedTime(0.f),
									 m_fTime(0.f),
									 m_bRenderEnable(TRUE)
{
	m_pInstance = this;
}

CNtlApplication::~CNtlApplication() 
{

}

/**
 * Window占쏙옙占쏙옙 占쏙옙占쏙옙 Message占쏙옙 처占쏙옙占싹댐옙 占쌉쇽옙
 * \param hWnd Window Handle
 * \param message Window Message
 * \param wParam 
 * \param lParam 
 * \return message의 처리 결과
 */
LRESULT CALLBACK MainWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return CNtlApplication::GetInstance()->WndProc(hWnd, message, wParam, lParam);
}

/**
 * Application占쏙옙占쏙옙 占십깍옙화 占쌔억옙占쏙옙 占쏙옙占쏙옙 占쌉쇽옙
 */
void CNtlApplication::Init()
{
	
}

/**
 * 
 *
 * \return Window Class占쏙옙 
 */
bool CNtlApplication::RegisterMainWindowClass(HANDLE processInstance)
{
	  WNDCLASS windowClass;
      
#ifdef UNDER_CE
      windowClass.style = CS_DBLCLKS;
#else
      windowClass.style = CS_BYTEALIGNWINDOW | CS_DBLCLKS;
#endif
      windowClass.lpfnWndProc = (WNDPROC)MainWndProc;
      windowClass.cbClsExtra = 0;
      windowClass.cbWndExtra = 0;
      windowClass.hInstance = (HINSTANCE)processInstance;
      windowClass.hIcon = 0;
      windowClass.hCursor = LoadCursor(0, IDC_ARROW);
      windowClass.hbrBackground = 0;
      windowClass.lpszMenuName = 0;
	  /*windowClass.*/
      windowClass.lpszClassName = kMainWindowClassName;
      
      return ( RegisterClass(&windowClass) != 0 );
}

HWND CNtlApplication::CreateMainWindow(HANDLE processInstance, int posX, int posY, int sizeX, int sizeY, RwBool bFullScreen)
{          
    // If we are in exclusive mode i.e. full screen in windows ensure the window flags
    // are correctly set.
    if (bFullScreen)
    {
        m_wndStyle = WS_POPUP;
		m_wndFlags = SWP_NOMOVE | SWP_NOSIZE;
		m_bFullScreen = TRUE;
    }
    else
    {
		m_wndStyle = WS_OVERLAPPEDWINDOW | WS_THICKFRAME;
		m_wndFlags = SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW;
		m_bFullScreen = FALSE;
    }

	RECT rect;
	rect.left = rect.top = 0;
	rect.right = sizeX;
	rect.bottom = sizeY;

	m_iWindowWidth	= m_iScreenWidth = m_iUserWidth = sizeX;
	m_iWindowHeight = m_iScreenHeight = m_iUserHeight = sizeY;
	
	// 현재 스타일이 적용된 윈도우의 크기를 계산한다.
	AdjustWindowRect(&rect, m_wndStyle, FALSE);
	
	// 占쏙옙占쏙옙占쏙옙 크占쏙옙占쏙옙 Window占쏙옙 占쏙옙占쏙옙占싼댐옙.
    HWND hwnd = CreateWindow(  kMainWindowClassName, kMainWindowName,
        m_wndStyle,
        posX, posY,
        rect.right - rect.left, rect.bottom - rect.top,
        (HWND)0, (HMENU)0, (HINSTANCE)processInstance, 0);

	// Window �� ���� ũ�� ���
	GetWindowRect( hwnd, &rect );
	m_iWindowWidth = rect.right - rect.left;
	m_iWindowHeight = rect.bottom - rect.top;

	// Screen �� ���� ũ�� ���
	GetClientRect( hwnd, &rect );
	m_iScreenWidth = rect.right - rect.left;
	m_iScreenHeight = rect.bottom - rect.top;

    SetWindowPos(hwnd, 0, 0, 0, 0, 0, m_wndFlags);

	return hwnd;
}

static rwD3D9DeviceReleaseCallBack g_OldDeviceReleaseCallBack = NULL;
static rwD3D9DeviceRestoreCallBack g_OldDeviceRestoreCallBack = NULL;

/**
 * Ntl占쏙옙 DeviceRelease CallBack 占쌉쇽옙
 */
static void NtlDeviceReleaseCallBack(void)
{
    if (g_OldDeviceReleaseCallBack)    
        g_OldDeviceReleaseCallBack();
	
	CNtlDeviceRestoreHandler::GetInstance()->ReleaseHandler();
	return;
}


/**
 * Ntl占쏙옙 DeviceRestore CallBack 占쌉쇽옙
 */
static void NtlDeviceRestoreCallBack(void)
{
    if (g_OldDeviceRestoreCallBack)
        g_OldDeviceRestoreCallBack();
	
	CNtlDeviceRestoreHandler::GetInstance()->RestoreHandler();
	return;
}

/**
 * Ntl�� DeviceRestore CallBack �Լ��� ����� �ϴ� �Լ�
 */
static void NtlSetDxDeviceEventHandler(void)
{
	g_OldDeviceReleaseCallBack = _rwD3D9DeviceGetReleaseCallback();
	_rwD3D9DeviceSetReleaseCallback(NtlDeviceReleaseCallBack);

	g_OldDeviceRestoreCallBack = _rwD3D9DeviceGetRestoreCallback();
	_rwD3D9DeviceSetRestoreCallback(NtlDeviceRestoreCallBack);
}

/**
 * Renderware의 Debug 정보를 출력하는 CallBack 함수
 */

// 
static void NtlRenderwareDebugMessageHandler(RwDebugType type __RWUNUSED__, const RwChar *str)
{
    OutputDebugString(str);
    OutputDebugString(RWSTRING("\n"));
	
	return;
}


// Disable Shortcut Keys by HoDong(2007.4.16)
// DirectX SDK Technical Articles (Disabling Shotcut Keys in Games)
STICKYKEYS g_StartupStickyKeys = {sizeof(STICKYKEYS), 0};
TOGGLEKEYS g_StartupToggleKeys = {sizeof(TOGGLEKEYS), 0};
FILTERKEYS g_StartupFilterKeys = {sizeof(FILTERKEYS), 0};    

void AllowAccessibilityShortcutKeys( bool bAllowKeys )
{
    if( bAllowKeys )
    {
        // Restore StickyKeys/etc to original state and enable Windows key      
        STICKYKEYS sk = g_StartupStickyKeys;
        TOGGLEKEYS tk = g_StartupToggleKeys;
        FILTERKEYS fk = g_StartupFilterKeys;
        
        SystemParametersInfo(SPI_SETSTICKYKEYS, sizeof(STICKYKEYS), &g_StartupStickyKeys, 0);
        SystemParametersInfo(SPI_SETTOGGLEKEYS, sizeof(TOGGLEKEYS), &g_StartupToggleKeys, 0);
        SystemParametersInfo(SPI_SETFILTERKEYS, sizeof(FILTERKEYS), &g_StartupFilterKeys, 0);
    }
    else
    {
        // Disable StickyKeys/etc shortcuts but if the accessibility feature is on, 
        // then leave the settings alone as its probably being usefully used
 
        STICKYKEYS skOff = g_StartupStickyKeys;
        if( (skOff.dwFlags & SKF_STICKYKEYSON) == 0 )
        {
            // Disable the hotkey and the confirmation
            skOff.dwFlags &= ~SKF_HOTKEYACTIVE;
            skOff.dwFlags &= ~SKF_CONFIRMHOTKEY;
 
            SystemParametersInfo(SPI_SETSTICKYKEYS, sizeof(STICKYKEYS), &skOff, 0);
        }
 
        TOGGLEKEYS tkOff = g_StartupToggleKeys;
        if( (tkOff.dwFlags & TKF_TOGGLEKEYSON) == 0 )
        {
            // Disable the hotkey and the confirmation
            tkOff.dwFlags &= ~TKF_HOTKEYACTIVE;
            tkOff.dwFlags &= ~TKF_CONFIRMHOTKEY;
 
            SystemParametersInfo(SPI_SETTOGGLEKEYS, sizeof(TOGGLEKEYS), &tkOff, 0);
        }
 
        FILTERKEYS fkOff = g_StartupFilterKeys;
        if( (fkOff.dwFlags & FKF_FILTERKEYSON) == 0 )
        {
            // Disable the hotkey and the confirmation
            fkOff.dwFlags &= ~FKF_HOTKEYACTIVE;
            fkOff.dwFlags &= ~FKF_CONFIRMHOTKEY;
 
            SystemParametersInfo(SPI_SETFILTERKEYS, sizeof(FILTERKEYS), &fkOff, 0);
        }
    }
}


/**
 * Application占쏙옙 占쏙옙占쏙옙占싹댐옙 占쌉쇽옙占쏙옙 Window占쏙옙 占쏙옙占쏙옙 占쏙옙 Renderware Engine占쏙옙 占십깍옙화
 * 占쏙옙占쏙옙 占싼댐옙.
 * \param hInstance Window Instance
 * \param posX Window X 占쏙옙占쏙옙占쏙옙치
 * \param posY Window Y 占쏙옙占쏙옙占쏙옙치
 * \param sizeX Window 占쏙옙占쏙옙 크占쏙옙
 * \param sizeY Window 占쏙옙占쏙옙 크占쏙옙
 * \param bFullScreen Full 화占쏙옙占쏙옙占쏙옙 Window 화占쏙옙占쏙옙占쏙옙 확占쏙옙
 * \return Application占쏙옙 占쏙옙占쏙옙占쌩댐옙占쏙옙占쏙옙 占쏙옙占쏙옙 占쏙옙占쏙옙
 */
RwBool CNtlApplication::Create( HINSTANCE hInstance, 
							    RwInt32 posX, 
								RwInt32 posY, 
								RwInt32 sizeX, 
								RwInt32 sizeY, 
								RwBool bFullScreen)
{
	//Window 占쏙옙占쏙옙
	RegisterMainWindowClass(hInstance);
	m_hWnd = CreateMainWindow(hInstance, posX, posY, sizeX, sizeY, bFullScreen);

	// Disable Shortcut Keys by HoDong(2007.4.16)
	// Save the current sticky/toggle/filter key settings so they can be restored them later 
	SystemParametersInfo(SPI_GETSTICKYKEYS, sizeof(STICKYKEYS), &g_StartupStickyKeys, 0);
    SystemParametersInfo(SPI_GETTOGGLEKEYS, sizeof(TOGGLEKEYS), &g_StartupToggleKeys, 0);
    SystemParametersInfo(SPI_GETFILTERKEYS, sizeof(FILTERKEYS), &g_StartupFilterKeys, 0);
	AllowAccessibilityShortcutKeys( false );
	
	//Memory Function 占쏙옙 ArenaSize 占쏙옙占쏙옙
	if( !RwEngineInit(NULL, rwENGINEINITNOFREELISTS, rsRESOURCESDEFAULTARENASIZE))
	{
		DBO_FAIL("Engine initial fail !!!");
		return FALSE;
	}
	
	RwDebugSetHandler(NtlRenderwareDebugMessageHandler);

	//Plugin 등록
	AttachPlugin();

	//Renderware 占쏙옙占쏙옙占쏙옙 Opne占싼댐옙.
	RwEngineOpenParams openParams;
	openParams.displayID = m_hWnd;
	if( !RwEngineOpen(&openParams) )
	{
		DBO_FAIL("Engine open fail !!!");
		RwEngineTerm();
		return FALSE;
	}

	//占쏙옙占쏙옙占쏙옙 Graphics占쏙옙 Mode占쏙옙 占쏙옙占쏙옙占싹댐옙占쏙옙占쏙옙 확占쏙옙占싼댐옙.
	if( !SelectVideoMode() )
	{
		DBO_FAIL("Select video mode fail !!!");
		return FALSE;
	}
	
	//multisampling test by HoDong
	//RwUInt32 uiMaxMultiSampling = RwD3D9EngineGetMaxMultiSamplingLevels();
	//RwBool bSuccess = RwD3D9ChangeMultiSamplingLevels(1);
	
	//Renderware Engine占쏙옙 占쏙옙占쏙옙占싼댐옙.
	RwD3D9EngineSetMultiThreadSafe( TRUE );
	if( !RwEngineStart() )
	{
		DBO_FAIL("Select start fail !!!");
		return FALSE;
	}	
	
	//File System Manager占쏙옙 占십깍옙화 占싹곤옙 Install占쏙옙 占싼댐옙.
	if (RtFSManagerOpen(RTFSMAN_UNLIMITED_NUM_FS) != FALSE)
    {
        if (!RsInstallFileSystem())
        {
            return FALSE;
        }
    }
    else
    {
        return FALSE;
    }
	
	//Image Loader(Bmp, Png) Enrollment
	RegisterImageLoader();
	

	ShowWindow(m_hWnd, 1);
	UpdateWindow(m_hWnd);

	// ShowWindow 한 후에 생성된 윈도우즈의 크기가 변경될 수도 있다. 변경된 크기를 업데이트한다.
	RECT rect;
	GetClientRect( m_hWnd, &rect );
	Resize( rect.right - rect.left, rect.bottom - rect.top, TRUE, FALSE );
	
	//SubSystem占쏙옙 占쏙옙占쏙옙(Camera, Charset ...)
	if(!CreateSubSystem(m_iScreenWidth, m_iScreenHeight, TRUE))
		return FALSE;

	//Timer Start
	CNtlTimer::StartTimer();
	g_SetElapsedTime(0.f);

	//Device Restore Callback
	NtlSetDxDeviceEventHandler();

	return TRUE;
}


/**
 * Application占쏙옙 占쏙옙占쏙옙占싹댐옙 占쌉쇽옙占쏙옙 Window占쏙옙 占쏙옙占쏙옙 占쏙옙 Renderware Engine占쏙옙 占십깍옙화
 * 占쏙옙占쏙옙 占싼댐옙.(Mfc 占쏙옙)
 * \param HWND Window Handle
 * \param bFullScreen Full 화占쏙옙占쏙옙占쏙옙 Window 화占쏙옙占쏙옙占쏙옙 확占쏙옙
 * \return Application占쏙옙 占쏙옙占쏙옙占쌩댐옙占쏙옙占쏙옙 占쏙옙占쏙옙 占쏙옙占쏙옙
 */
RwBool CNtlApplication::Create(HWND hHandle, RwBool bFullScreen)
{
	m_hWnd			= hHandle;
	m_bFullScreen	= bFullScreen;

	RECT rtRect;
	GetClientRect(m_hWnd, &rtRect);
	m_iScreenWidth  = rtRect.right - rtRect.left;
	m_iScreenHeight = rtRect.bottom - rtRect.top;

	//Memory Function 占쏙옙 ArenaSize 占쏙옙占쏙옙
	if( !RwEngineInit(NULL, rwENGINEINITNOFREELISTS, rsRESOURCESDEFAULTARENASIZE))
		return FALSE;

	RwDebugSetHandler(NtlRenderwareDebugMessageHandler);

	AttachPlugin();

	//Renderware 占쏙옙占쏙옙占쏙옙 Opne占싼댐옙.
	RwEngineOpenParams openParams;
	openParams.displayID = m_hWnd;
	if( !RwEngineOpen(&openParams) )
	{
		RwEngineTerm();
		return FALSE;
	}

	//占쏙옙占쏙옙占쏙옙 Graphics占쏙옙 Mode占쏙옙 占쏙옙占쏙옙占싹댐옙占쏙옙占쏙옙 확占쏙옙占싼댐옙.
	if( !SelectVideoMode() )
	{
		return FALSE;
	}
	
	//Renderware Engine占쏙옙 占쏙옙占쏙옙占싼댐옙.
	if( !RwEngineStart() )
	{
		return FALSE;
	}	
	
	//File System Manager占쏙옙 占십깍옙화 占싹곤옙 Install占쏙옙 占싼댐옙.
	if (RtFSManagerOpen(RTFSMAN_UNLIMITED_NUM_FS) != FALSE)
    {
        if (!RsInstallFileSystem())
        {
            return FALSE;
        }
    }
    else
    {
        return FALSE;
    }
	
	//PlugIn 등록 및 Image Loader(Bmp, Png) 등록
	
	RegisterImageLoader();
	
	//SubSystem占쏙옙 占쏙옙占쏙옙(Camera, Charset ...)
	if(!CreateSubSystem(m_iScreenWidth, m_iScreenHeight, TRUE))
		return FALSE;

	//Timer Start
	CNtlTimer::StartTimer();

	//Device Restore Callback
	NtlSetDxDeviceEventHandler();
	
	return TRUE;
}

/**
 * Application의 기능을 종료한다.
 */
void CNtlApplication::Destroy()
{
	// Disable Shortcut Keys by HoDong(2007.4.16)
	AllowAccessibilityShortcutKeys( true );
	
	RtFSManagerClose();
	
	//Stop RW engine
	RwEngineStop();
	
	//Close RW engine
	RwEngineClose();
	
	//Terminate RW engine
	RwEngineTerm();
}



/** 
 * m_nWindowWidth, m_nWindowHeight, m_nBitDepth占쏙옙 占쏙옙占쏙옙占쏙옙 카占쏙옙
 * 占쏙옙占쏙옙 占쏙옙占쏙옙占쏙옙 占실댐옙占쏙옙占쏙옙 占싯삼옙占싼댐옙.
 * \return Mode占쏙옙 占쏙옙占쏙옙占싹댐옙占쏙옙占쏙옙 占쏙옙占쏙옙 占쏙옙占쏙옙
 */
RwBool CNtlApplication::SelectVideoMode()
{
	#define MAX_SUBSYSTEMS      (16)
	RwSubSystemInfo GsubSysInfo[MAX_SUBSYSTEMS];

	RwVideoMode     vm;
	RwInt32			nGnumSubSystems = 0;
	RwInt32			nGcurSel = 0;
	RwInt32			nGcurSelVM = 0;
	RwInt32			nDefDeviceNum = 0;

	//占쏙옙占쏙옙 System占쏙옙 Rendering占쏙옙 占쌀쇽옙 占쌍댐옙(Graphic Device)占쏙옙 占쏙옙占쏙옙占쏙옙 占쏙옙占쏙옙占싼댐옙. 
	nGnumSubSystems = RwEngineGetNumSubSystems();
	
	/* Just to be sure ... */
    nGnumSubSystems = (nGnumSubSystems > MAX_SUBSYSTEMS) ? MAX_SUBSYSTEMS : nGnumSubSystems;
	
	//Graphics Device의 모든 이름들을 얻는다.
    for (RwInt32 subSysNum = 0; subSysNum < nGnumSubSystems; ++ subSysNum)
    {
        RwEngineGetSubSystemInfo(&GsubSysInfo[subSysNum], subSysNum);
    }
	
	//현재의 Graphics Device를 얻는다.
	nGcurSel = RwEngineGetCurrentSubSystem();
	
	//占쏙옙占쏙옙占쏙옙 Graphics Device占쏙옙 Engine Setting 占싼댐옙.
    if (!RwEngineSetSubSystem(nGcurSel))
    {
        return FALSE;
    }
	
	// 占쏙옙占쏙옙占쏙옙 占쌔상도몌옙 찾占승댐옙. Mode占쏙옙 占쏙옙占쏙옙占싹댐옙占쏙옙 확占쏙옙占쏙옙 占싼댐옙.
	nGcurSelVM = GetVideoMode( m_bFullScreen, m_iScreenWidth, m_iScreenHeight, m_iBitDepth );
	if( nGcurSelVM < 0 )
	{
		nGcurSelVM = GetBestVideoMode(m_bFullScreen, m_iScreenWidth, m_iScreenHeight, m_iBitDepth);
		if ( nGcurSelVM < 0 )
		{
			DBO_FAIL("Not failed best video mode !!!");
			return FALSE;
		}
	}
	
	//Set up the video mode and set the apps window
	//dimensions to match
    if (!RwEngineSetVideoMode(nGcurSelVM))
    {
        return FALSE;
    }

	RwEngineGetVideoModeInfo(&vm, RwEngineGetCurrentVideoMode());
	if( !(vm.flags & rwVIDEOMODEEXCLUSIVE) )
	{
		RECT rect;
		GetWindowRect(m_hWnd, &rect);

		m_iWindowPosX = rect.left;
		m_iWindowPosY = rect.top;
	}
	
	// 占쏙옙占쏙옙 占쏙옙占시듸옙 占쌔삼옙占쏙옙 크占썩를 占쏙옙占쏙옙占쏙옙트
	m_iUserWidth = vm.width;
	m_iUserHeight = vm.height;
	m_iBitDepth = vm.depth;

	return TRUE;
}

void CNtlApplication::ChangeWindowStyle(RwBool bFullScreen)
{
	m_bFullScreen = bFullScreen;

	if(m_bFullScreen)
	{
		m_wndStyle = WS_POPUP;
		m_wndFlags = SWP_SHOWWINDOW;
		m_hWndInsertAfter = HWND_TOPMOST;
	}
	else
	{
		m_wndStyle = WS_OVERLAPPEDWINDOW | WS_VISIBLE;
		m_wndFlags = SWP_SHOWWINDOW;
		m_hWndInsertAfter = HWND_NOTOPMOST;		
	}

	RECT rect;
	rect.left = 0;
	rect.top = 0;
	rect.right = m_iUserWidth;
	rect.bottom = m_iUserHeight;
	
	AdjustWindowRect(&rect, m_wndStyle, FALSE);

	SetWindowLong(m_hWnd, GWL_STYLE, m_wndStyle);
	SetWindowPos(m_hWnd, m_hWndInsertAfter, 0, 0, rect.right - rect.left, rect.bottom - rect.top, m_wndFlags);

	// Resize �� �Ŀ� GetClientRect�� �ؼ� ���� Size�� ũ�Ⱑ �� �´´ٸ� ���缭 Resize ���ش�.
	RECT rectAfter;
	GetClientRect( m_hWnd, &rectAfter );
	if( rect.right - rect.left != rectAfter.right - rectAfter.left ||
		rect.bottom - rect.top != rectAfter.bottom - rectAfter.top )
	{
		Resize( rectAfter.right - rectAfter.left, rectAfter.bottom - rectAfter.top, TRUE, FALSE );
	}

	// Ǯ ��ũ���� �ƴ� ��� �߰� ��ġ�� ����
	if( !m_bFullScreen )
	{
		RECT rectWindow;
		GetWindowRect( m_hWnd, &rectWindow );

		RwInt32 nScreenWidth = GetSystemMetrics( SM_CXSCREEN );
		RwInt32 nScreenHeight = GetSystemMetrics( SM_CYSCREEN );
		RwInt32 nWindowWidth = rectWindow.right - rectWindow.left;
		RwInt32 nWindowHeight = rectWindow.bottom - rectWindow.top;

		m_iWindowPosX = (RwInt32)(nScreenWidth / 2 ) - (RwInt32)( nWindowWidth / 2 );
		m_iWindowPosY = (RwInt32)(nScreenHeight / 2 ) - (RwInt32)( nWindowHeight / 2 );

		SetWindowPos( m_hWnd, 0, m_iWindowPosX, m_iWindowPosY, 0, 0, SWP_NOZORDER | SWP_NOSIZE );
	}	
}

RwBool CNtlApplication::ToggleFullMode(void)
{
	RwBool bFullScreen = !m_bFullScreen;

	ChangeWindowStyle(bFullScreen);

	//Mode占쏙옙 占쏙옙占쏙옙占싹댐옙占쏙옙 확占쏙옙占쏙옙 占싼댐옙.
	RwInt32 nGcurSelVM = GetVideoMode(bFullScreen, m_iUserWidth, m_iUserHeight, m_iBitDepth);	
	if( nGcurSelVM < 0 )
	{
		nGcurSelVM = GetBestVideoMode( bFullScreen, m_iUserWidth, m_iUserHeight, m_iBitDepth );
		if( nGcurSelVM < 0 )
		{
			DBO_FAIL("failed to change video mode : " << m_iUserWidth << "*" << m_iUserHeight << "," << m_iBitDepth );
			return FALSE;
		}
	}
	
	//Set up the video mode and set the apps window
	//dimensions to match
    if (!RwD3D9ChangeVideoMode (nGcurSelVM))
    {
        return FALSE;
    }

	RwVideoMode vm;
	RwEngineGetVideoModeInfo(&vm, RwEngineGetCurrentVideoMode());

	// 만약 풀 스크린 Mode인데 vm.flags가 EXCLUSIVE가 아니라면 창 화면 -> 전체 화면 모드 전환 실패로 간주하고
	// �����츦 �ٽ� �����ش�. �ݴ��� ��쵵 �Ȱ��� �������� �����Ѵ�.
	// ȭ�� ��ȯ�� �����ߴ��� ������ �ػ󵵿� ���缭 �ٽ� �ѹ� ������ ��Ÿ���� �����Ѵ�.
	if( (vm.flags & rwVIDEOMODEEXCLUSIVE) == 0)
	{
		ChangeWindowStyle( FALSE );
	}
	else
		ChangeWindowStyle( TRUE );
	

	API_CoreToggleFullScreenNotify( B2b(bFullScreen) );

	return TRUE;
}


void CNtlApplication::ChangeVideoMode(RwInt32 iWidth, RwInt32 iHeight, RwInt32 iBitDepth)
{
	// ChangeVideo Mode will not work unless it is in FullScreen Mode.
	if(m_bFullScreen)
	{
		// Check if Mode is supported and apply if supported.
		RwInt32 nGcurSelVM = GetVideoMode(m_bFullScreen, iWidth, iHeight, iBitDepth);
		if( nGcurSelVM < 0)
		{
			DBO_WARNING_MESSAGE("Monitor does not support resolution : " << iWidth << "*" << iHeight << "," << iBitDepth );
			
			iWidth = NTL_APP_SCREEN_WIDTH;
			iHeight = NTL_APP_SCREEN_HEIGHT;
			iBitDepth = NTL_APP_BIT_DEPTH;

			nGcurSelVM = GetVideoMode(m_bFullScreen, iWidth, iHeight, iBitDepth);
			if (nGcurSelVM < 0)
			{
				DBO_FAIL("Go to DBOG/user and open SystemEnv.txt and change WINDOW_MODE to true");
				return;
			}
		}	

		// User占쏙옙 占쏙옙占쏙옙占쏙옙 占쌔삼옙 占쏙옙占쏙옙占쏙옙트
		m_iUserWidth = iWidth;
		m_iUserHeight = iHeight;
		m_iBitDepth = iBitDepth;

		if (!RwD3D9ChangeVideoMode(nGcurSelVM))
			return;

		// ToggleFullMode() 의 경우 Window의 Resize message를 발생하지만 ChangeVideoMode의 경우 Resize를 강제적으로 발생시킨다.
		RECT rect;
		GetClientRect( m_hWnd, &rect );
		Resize( rect.right - rect.left, rect.bottom - rect.top, TRUE, FALSE );
	}
	// Window Mode
	else
	{
		// User占쏙옙 占쏙옙占쏙옙占쏙옙 占쌔삼옙 占쏙옙占쏙옙占쏙옙트
		m_iUserWidth = iWidth;
		m_iUserHeight = iHeight;
		m_iBitDepth = iBitDepth;

		// ���� ����ȭ���� ũ�⸦ �����ͼ� ����ȭ�麸�� �� ũ�ٸ� ũ��� ������ �����Ѵ�.
		// 선택된 크기는 그대로 가진다.
		int nSystemWidth = GetSystemMetrics(SM_CXSCREEN);
		int nSystemHeight = GetSystemMetrics(SM_CYSCREEN);

		if( nSystemWidth < iWidth )
			iWidth = nSystemWidth;
		
		if( nSystemHeight < iHeight )
			iHeight = nSystemHeight;

		int nPosX = ( nSystemWidth - iWidth ) ? ((nSystemWidth - iWidth )/2) : 0;
		int nPosY = ( nSystemHeight - iHeight ) ? ((nSystemHeight - iHeight )/2) : 0;

		SetWindowPos( m_hWnd, NULL, nPosX, nPosY, iWidth, iHeight, SWP_NOZORDER );
	}
}

/**
 *
 * \param bFullScreen
 * \param nWidth Window Width
 * \param nHeight Window Height
 * \return 占쏙옙占쏙옙占쏙옙 Video Mode占쏙옙 占쏙옙占쏙옙占쏙옙 占싹댐옙占쏙옙
 */
RwInt32 CNtlApplication::GetVideoMode(RwBool bFullScreen, RwInt32 nWidth, RwInt32 nHeight, RwInt32 bitDepth)
{
	RwInt32 numVidModes = RwEngineGetNumVideoModes();
	
	RwVideoMode vidModemInfo;
	for (RwInt32 vidMode = 0; vidMode < numVidModes; vidMode++)
	{
		RwEngineGetVideoModeInfo(&vidModemInfo, vidMode);		
		if (bFullScreen)
		{
			if (vidModemInfo.flags & rwVIDEOMODEEXCLUSIVE && 
				vidModemInfo.width  == nWidth && 
				vidModemInfo.height == nHeight && 
				vidModemInfo.depth  == bitDepth)
			{
				return vidMode;
			}
		}		
		else if (vidModemInfo.flags == 0)
		{
			return vidMode;
		}
	}

	return -1;
}


/**
 * File System占쏙옙 Install占쏙옙 占싼댐옙.
 * \return Install占쏙옙 占실억옙占쏙옙占쏙옙占쏙옙 占쏙옙占쏙옙 占쏙옙占쏙옙 占쏙옙占쏙옙
 */
RwBool CNtlApplication::RsInstallFileSystem()
{
    RwChar      curDirBuffer[NTL_MAX_DIR_PATH];
    RwUInt32    retValue;
    RtFileSystem *wfs, *unc;

    RwUInt32 drivesMask;
    RwInt32 drive;
    RwChar  fsName[2];
    
    /* get the current directory */
    retValue = GetCurrentDirectory(NTL_MAX_DIR_PATH, curDirBuffer);
    if (0 == retValue || retValue > NTL_MAX_DIR_PATH)
    {
        return FALSE;
    }

    /* Register a unc file system */
    
    /* tag a \ to the end of the current directory */
    /* only fails if the buffer size is exhausted */
    rwstrcat( curDirBuffer, "\\" );

    /** Setup the file system manager */
    if ((unc = RtWinFSystemInit(MAX_NB_FILES_PER_FS, curDirBuffer, "unc")) != NULL)
    {
        /* Now actually register the file system */
        if (RtFSManagerRegister(unc) == FALSE)
        {
            return (FALSE);
        }
    }
    else
    {
        return (FALSE);
    }
    
    /* Now register local file systems */
    
    CharUpper(curDirBuffer);

    /* 
     * loop through all logical drives and register a file system for each
     * valid one.
     * Start at 2: this removes a:
     */
    drivesMask = GetLogicalDrives();

    for( drive = 2, drivesMask >>= 1; drivesMask != 0; drive++, drivesMask >>= 1)
    {
        if (drivesMask & 0x01)
        {
            RwInt32 ret;
            RwChar  deviceName[4];

            deviceName[0] = drive + 'A' - 1;
            deviceName[1] = ':';
            deviceName[2] = '\\';   /* Needed for Win98 */
            deviceName[3] = '\0';

            ret = GetDriveType(deviceName);

            if ( ret != DRIVE_UNKNOWN && ret != DRIVE_NO_ROOT_DIR )
            {
                /* Fix device name */
                deviceName[2] = '\0';

                fsName[0] = deviceName[0];
                fsName[1] = '\0';

                /** Setup the file system manager */
                if ((wfs = RtWinFSystemInit(MAX_NB_FILES_PER_FS, deviceName, fsName)) != NULL)
                {
                    /* Now actually register the file system */
                    if (RtFSManagerRegister(wfs) == FALSE)
                    {
                        return (FALSE);
                    }
                    else
                    {
                        /* Set the unc file system as default if we have a unc path */
                        if (curDirBuffer[1] != ':')
                        {
                            RtFSManagerSetDefaultFileSystem(unc);
                        }
                        else if (deviceName[0] == curDirBuffer[0])
                        {
                            RtFSManagerSetDefaultFileSystem(wfs);
						//	RtFileSystemDefaultToAsync(wfs, TRUE);
                        }
                    }
                }
                else
                {
                    return (FALSE);
                }
            }
        }
    }
    
    return (TRUE);
}

/**
 * Window占쏙옙 Size占쏙옙 占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙 호占쏙옙占쏙옙 占실댐옙 占쌉쇽옙
 * \param nWidth Window占쏙옙占쏙옙 Size
 * \param nHeight Window占쏙옙占쏙옙 Size
 * \param zBuffer ZBuffer ��� ����
 * \return Resize占쏙옙 占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙 占실억옙占쏙옙占쏙옙占쏙옙 占쏙옙占쏙옙
 */
RwBool CNtlApplication::Resize(RwInt32 iWidth, RwInt32 iHeight, RwBool zBuffer, RwBool bRestore_from_Minimize)
{
	RECT rect;

	// Window �� ���� ũ�� ���
	GetWindowRect( m_hWnd, &rect );
	m_iWindowWidth = rect.right - rect.left;
	m_iWindowHeight = rect.bottom - rect.top;

	// Screen �� ���� ũ�� ���
	GetClientRect( m_hWnd, &rect );
	m_iScreenWidth = rect.right - rect.left;
	m_iScreenHeight = rect.bottom - rect.top;

	return TRUE;
}

/**
* 占쏙옙占시몌옙占쏙옙占싱쇽옙占쏙옙 占쏙옙치占쏙옙 占쏙옙占쏙옙占쏘가 占쏙옙占쏙옙 占쏙옙
* WM_SIZE �� WM_MOVE �޼��� ���� �켱 ó���ȴ�
*/
void CNtlApplication::SetApplicationMinMax(LPMINMAXINFO lpmmi)
{
}

/**
 * �߰������� ������ �ؾ��Ұ͵鿡 ���� �Լ�
 * \param nWidth 占쏙옙占쏙옙 占쏙옙占쏙옙占쏙옙 Window占쏙옙 Client占쏙옙 占쏙옙占쏙옙크占쏙옙
 * \param nHeight 占쏙옙占쏙옙 占쏙옙占쏙옙占쏙옙 Window占쏙옙 Client占쏙옙 占쏙옙占쏙옙크占쏙옙
 * \param zBuffer ZBuffer�� �������
 * \return 
 */
RwBool CNtlApplication::CreateSubSystem(RwUInt32 iWidth, RwUInt32 iHeight, RwBool zBuffer)
{
	return TRUE;
}

LRESULT CNtlApplication::WndProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
	if(PreTranslateMessage(window, message, wParam, lParam))
		return 0L;

	switch (message)
    {
    case WM_ACTIVATEAPP:
        minimized = !wParam;
        return( 0L ); 
    
	case WM_CLOSE:
        DestroyWindow(window);
        return( 0L);

    case WM_DESTROY:
        PostQuitMessage(0);
        return ( 0L );

    case WM_PAINT:
        PAINTSTRUCT Paint;
        BeginPaint(window, &Paint);
        //RWS::MainLoop::Render::Poll();
        EndPaint(window, &Paint);
        return ( 0L );
	
	case WM_SIZE:		
		RwRect r;
		RwBool bRestore_from_Minimize;

		r.x = 0;
		r.y = 0;
		r.w = LOWORD(lParam);
		r.h = HIWORD(lParam);

		bRestore_from_Minimize = FALSE;

		if( wParam == SIZE_MINIMIZED )
		{
			m_bMinimized = TRUE;
		}
		else if( wParam == SIZE_RESTORED )
		{
			if( m_bMinimized )
			{
				m_bMinimized = FALSE;
				bRestore_from_Minimize = TRUE;
			}
		}

		Resize(r.w, r.h, TRUE, bRestore_from_Minimize);
        return ( 0L );
	case WM_SIZING:
		return FixedWindowSizing( wParam, lParam );
	case WM_GETMINMAXINFO:
		SetApplicationMinMax( (LPMINMAXINFO)lParam );
		return( 0L );
	
	case WM_SYSKEYUP:
	case WM_SYSKEYDOWN:
		return ( 0L );	
	default:
        return ( DefWindowProc(window, message, wParam, lParam) );
    }
	return (-1L);
}

RwBool CNtlApplication::Update()
{
	CNtlTimer::UpdateTimer();

	m_fTime			= CNtlTimer::GetAppTime();
	m_fElapsedTime	= CNtlTimer::GetElapsedTime();
	m_fFps			= CNtlTimer::GetFps();

	g_SetElapsedTime(m_fElapsedTime);

	if(Update(m_fTime, m_fElapsedTime))
		return FALSE;

	return TRUE;
}

RwBool CNtlApplication::Update(RwReal fTime, RwReal fElapsedTime)
{
	return TRUE;
}

int CNtlApplication::MessagePump()
{
	int result = 0;
	LARGE_INTEGER frequency;
	LARGE_INTEGER start;
	LARGE_INTEGER end;

	QueryPerformanceFrequency(&frequency);

	while (!m_bQuit)
	{
		MSG message;

		if (PeekMessage(&message, 0, 0U, 0U, PM_REMOVE | PM_NOYIELD))
		{
			if (message.message == WM_QUIT)
			{
				SetQuit();
				result = (int)message.wParam;
			}
			else
			{
				if (m_hAccel == NULL || TranslateAccelerator(m_hWnd, m_hAccel, &message) == 0)
				{
					TranslateMessage(&message);
					DispatchMessage(&message);
				}
			}
		}
		else
		{
			QueryPerformanceCounter(&start);

			Update();

			// 0 = unlimited frame rate: skip the throttle spin entirely.
			if (m_uiFrameRate > 0)
			{
				RwUInt32 uiRateTime = (1000000 / m_uiFrameRate); // in microseconds

				do {
					QueryPerformanceCounter(&end);
				} while ((end.QuadPart - start.QuadPart) * 1000000.0 / frequency.QuadPart < uiRateTime);
			}
		}
	}

	if (m_hAccel != NULL)
	{
		DestroyAcceleratorTable(m_hAccel);
		m_hAccel = NULL;
	}

	return (result);
}


RwBool CNtlApplication::AttachPlugin()
{
	return TRUE;
}


CallTextureLoad		g_fnCallTextureLoad		= NULL;
//CallTextureLoadPath g_fnCallTextureLoadPath	= NULL;

void LinkTextureLoad(CallTextureLoad fn/*, CallTextureLoadPath fnPath*/)
{
	g_fnCallTextureLoad		= fn;
	//g_fnCallTextureLoadPath	= fnPath;
}

void UnLinkTextureLoad(void)
{
	g_fnCallTextureLoad = NULL;
}


RwTextureCallBackRead OldCB;

RwTexture * DDSTextureRead(const RwChar * szName, const RwChar * szMaskName)
{
	CNtlLoadingController::GetInstance()->Lock();

	RwTexture*	pTexture			= NULL; 
	//RwBool		bTextureLoadPath	= FALSE;
	RwChar*		pcPath				= NULL;
	RwInt32		iPathSize			= 0;

// 	if(g_fnCallTextureLoadPath && (bTextureLoadPath = g_fnCallTextureLoadPath(RwImageGetPath(), &pcPath, &iPathSize)))
// 	{
// 		_rwImageSwapPath(&pcPath, &iPathSize);
// 	}

	if(g_fnCallTextureLoad)
	{
		char *pPath = RwImageGetPath();
		pTexture = (*g_fnCallTextureLoad)(pPath, szName, szMaskName);
		if(pTexture)
		{
			CNtlLoadingController::GetInstance()->Unlock();
			
// 			if(bTextureLoadPath)
// 			{
// 				_rwImageSwapPath(&pcPath, &iPathSize);
// 			}

			return pTexture;
		}
	}
	
	RwChar *extender = rwstrrchr(szName, '.');
	if( extender != NULL )
	{
		if( _stricmp(extender, ".png") == 0 ||
		    _stricmp(extender, ".bmp") == 0 )
		{
			pTexture = OldCB(szName, szMaskName);
		}
	}
	
	//There are times when png comes in without extension.
	//(Eg skydummy) by HoDong (7/8/2006)
	if(!pTexture)
	{
		pTexture = RwD3D9DDSTextureRead(szName, szMaskName);
		if(pTexture == NULL)
			pTexture = OldCB(szName, szMaskName);
	}
	
	CNtlLoadingController::GetInstance()->Unlock();

// 	if(bTextureLoadPath)
// 	{
// 		_rwImageSwapPath(&pcPath, &iPathSize);
// 	}

	return pTexture;
}

void HookDDSTextureRead(void)
{    
    OldCB = RwTextureGetReadCallBack(); 
    RwTextureSetReadCallBack(DDSTextureRead);
}

RwBool CNtlApplication::RegisterImageLoader()
{
	if (!RwImageRegisterImageFormat(RWSTRING("bmp"), RtBMPImageRead, RtBMPImageWrite))
    {
        return FALSE;
    }

    if (!RwImageRegisterImageFormat(RWSTRING("png"), RtPNGImageRead, RtPNGImageWrite))
    {
        return FALSE;
    }

	HookDDSTextureRead();

	return TRUE;
}

// Gamma
void CNtlApplication::SetGammaRamp(RwReal fGamma)
{
	// 상속 받아서 구현한다.
}

/**
* \brief ���� ������ ������带 �����Ѵ�.
*/
RwInt32 CNtlApplication::GetBestVideoMode( RwBool bFullScreen, RwInt32 iWidth, RwInt32 iHeight, RwInt32 iBitDepth )
{
	RwInt32 numVidModes = RwEngineGetNumVideoModes();

	RwInt32 nBestVideoMode = -1;

	RwInt32 nMinWidthMargin = 99999;
	RwInt32 nMinHeightMargin = 99999;
	RwInt32 nMinBitDepthMargin = 99999;
	RwInt32 nMinSummary = 99999;
	RwInt32 nCompare = 0;

	RwVideoMode sModeInfo;

	for (RwInt32 nModeIndex = 0; nModeIndex < numVidModes; nModeIndex++)
	{
		RwEngineGetVideoModeInfo(&sModeInfo, nModeIndex);

		if( sModeInfo.width <= 800 && sModeInfo.height <= 600 )
			continue;

		// 전체 화면의 경우 넓이, 높이, 깊이 오차의 합이 가장 낮은 Video Mode를 리턴하고
		// 창 모드의 경우 Flags가 창 모드로 체크되어 있으면 무조건 리턴한다.
		if (bFullScreen)
		{
			if (sModeInfo.flags & rwVIDEOMODEEXCLUSIVE )
			{
				// 占쏙옙占쏙옙 占쏙옙占쏙옙
				nCompare = iWidth - sModeInfo.width;
				if( nCompare >= 0 )
					nMinWidthMargin = nCompare;

				// 占쏙옙占쏙옙 占쏙옙占쏙옙
				nCompare = iHeight - sModeInfo.height;
				if( nCompare >= 0 ) 
					nMinHeightMargin = nCompare;

				// 占쏙옙占쏙옙 占쏙옙占쏙옙
				nCompare = iBitDepth - sModeInfo.depth;
				if( nCompare >= 0 )
					nMinBitDepthMargin = nCompare;

				// 占쏙옙占쏙옙占쏙옙 占쏙옙
				nCompare = nMinWidthMargin + nMinHeightMargin + nMinBitDepthMargin;
				if( nCompare >= 0 &&
					nCompare < nMinSummary )
				{
					nMinSummary = nCompare;
					nBestVideoMode = nModeIndex;
				}

				// 占쏙옙확占싹곤옙 占쏙옙치占쏙옙
				if( nMinSummary == 0 )
					return nModeIndex;
			}
		}		
		else if (sModeInfo.flags == 0)
		{
			nBestVideoMode = nModeIndex;
		}
	}

	return nBestVideoMode;
}

/**
* \brief 占쏙옙占쏙옙占쏙옙 占쏙옙占쏙옙占쏙옙 占쏙옙占쏙옙占쏙옙 Resizing占쏙옙 占싹댐옙 占쌉쇽옙
*/
RwBool CNtlApplication::FixedWindowSizing( WPARAM wParam, LPARAM lParam )
{
	RECT* pRect = reinterpret_cast<RECT*>(lParam);
	int nCurrentWindowWidth = pRect->right - pRect->left;
	int nCurrentWindowHeight = pRect->bottom - pRect->top;

	float fWidthAdjust = (float)m_iWindowWidth / (float)m_iWindowHeight;
	float fHeightAdjust = (float)m_iWindowHeight / (float)m_iWindowWidth;

	switch( wParam )
	{
		// 占쏙옙占싱곤옙 占시어난占쏙옙.
	case WMSZ_BOTTOMLEFT:
	case WMSZ_BOTTOMRIGHT:
	case WMSZ_LEFT:
	case WMSZ_RIGHT:
		{
			int nFixHeight = (int)((float)nCurrentWindowWidth * fHeightAdjust);

			pRect->bottom = pRect->top + nFixHeight;
		}
		break;
	case WMSZ_TOP:
		{
			int nFixWidth = (int)((float)nCurrentWindowHeight * fWidthAdjust);

			pRect->left = pRect->right - nFixWidth;
		}
		break;
	case WMSZ_BOTTOM:
		{		
			int nFixWidth = (int)((float)nCurrentWindowHeight * fWidthAdjust);

			pRect->right = pRect->left + nFixWidth;
		}
		break;
	case WMSZ_TOPLEFT:
	case WMSZ_TOPRIGHT:
		{
			int nFixHeight = (int)((float)nCurrentWindowWidth * fHeightAdjust);

			pRect->top = pRect->bottom - nFixHeight;
		}
		break;
	default:
		break;
	}

	return TRUE;
}
