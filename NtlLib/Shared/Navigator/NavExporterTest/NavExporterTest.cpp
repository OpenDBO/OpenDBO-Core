// NavExporterTest.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "CNtlWeManager.h"
#include "NavExporterTest.h"

#define MAX_LOADSTRING		100
#define MAX_TEXT_STRING		65536


// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
TCHAR szText[MAX_TEXT_STRING];					// The title bar text
BOOL g_bConverting = FALSE;
BOOL g_bDone = FALSE;

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass( HINSTANCE hInstance );
BOOL				InitInstance( HINSTANCE, int );
LRESULT CALLBACK	WndProc( HWND, UINT, WPARAM, LPARAM );
INT_PTR CALLBACK	About( HWND, UINT, WPARAM, LPARAM );

bool				ConvertWorld( HWND hWnd );

int APIENTRY _tWinMain( HINSTANCE hInstance,
						HINSTANCE hPrevInstance,
						LPTSTR    lpCmdLine,
						int       nCmdShow)
{
	UNREFERENCED_PARAMETER( hPrevInstance );
	UNREFERENCED_PARAMETER( lpCmdLine );

 	// TODO: Place code here.
	MSG		sMsg;
	HACCEL	hAccelTable;

	// Initialize global strings
	LoadString( hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING );
	LoadString( hInstance, IDC_NAVEXPORTERTEST, szWindowClass, MAX_LOADSTRING );
	_tcscpy_s( szText, MAX_TEXT_STRING, _T( "Ready" ) );

	MyRegisterClass(hInstance);


	// Perform application initialization:
	if( !InitInstance( hInstance, nCmdShow ) )
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators( hInstance, MAKEINTRESOURCE( IDC_NAVEXPORTERTEST ) );

	while( GetMessage( &sMsg, NULL, 0, 0 ) )
	{
		if( !TranslateAccelerator( sMsg.hwnd, hAccelTable, &sMsg ) )
		{
			TranslateMessage( &sMsg );
			DispatchMessage( &sMsg );
		}
	}

	return (int)sMsg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_NAVEXPORTERTEST));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_NAVEXPORTERTEST);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
						CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//

LRESULT CALLBACK WndProc( HWND hWnd, UINT nMessage, WPARAM wParam, LPARAM lParam )
{
	switch( nMessage )
	{
		case WM_CREATE:
		{
		//	CFixedString	cEngineName( _T( "PathEngine" ) );
		//	CFixedString	cSessionName( _T( "NavServer" ) );

		//	CNavigator::CreateInstance( cEngineName, cSessionName );

			break;
		}

		case WM_COMMAND:
		{
			int		wmId    = LOWORD( wParam );
			int		wmEvent = HIWORD( wParam );

			DBG_UNREFERENCED_LOCAL_VARIABLE( wmEvent );

			switch( wmId )
			{
				case IDM_ABOUT:
				{
					ConvertWorld( hWnd );

					break;
				}

				case IDM_EXIT:
				{
					DestroyWindow( hWnd );

					break;
				}

				default:
				{
					return DefWindowProc( hWnd, nMessage, wParam, lParam );
				}

				break;
			}
		}

		case WM_PAINT:
		{
			PAINTSTRUCT		sPaintStruct;
			HDC		hDC = BeginPaint( hWnd, &sPaintStruct );

			if( g_bConverting == TRUE )
			{
				TextOut( hDC, 0, 0, "Converting", static_cast<int>(_tcslen( "Done" )) );
			}
			else if( g_bDone == TRUE )
			{
				TextOut( hDC, 0, 0, "Done", static_cast<int>(_tcslen( "Done" )) );
			}
			else
			{
				TextOut( hDC, 0, 0, "Ready", static_cast<int>(_tcslen( "Ready" )) );
			}

			EndPaint( hWnd, &sPaintStruct );

			break;
		}

		case WM_DESTROY:
		{
			CNavigator::DestroyInstance();

			PostQuitMessage( 0 );

			break;
		}

		default:
		{
			return DefWindowProc( hWnd, nMessage, wParam, lParam );
		}
	}

	return 0;
}


// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT nMessage, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER( lParam );

	switch( nMessage )
	{
		case WM_INITDIALOG:
		{
			return (INT_PTR)TRUE;
		}

		case WM_COMMAND:
		{
			if( LOWORD( wParam ) == IDOK ||
				LOWORD( wParam ) == IDCANCEL )
			{
				EndDialog( hDlg, LOWORD( wParam ) );

				return (INT_PTR)TRUE;
			}

			break;
		}
	}

	return (INT_PTR)FALSE;
}


bool ConvertWorld( HWND hWnd )
{
	g_bConverting = TRUE;

	CNtlWeManager *		pcFileConverter = down_cast<CNtlWeManager *>(CNtlWeManager::CreateFileConverter());

	pcFileConverter->SetWnd( hWnd );

	CFixedString	cConfigFileName( _T( "NavData\\WeConfig.xml" ) );

	pcFileConverter->ConvertWorld( cConfigFileName );

	CNtlWeManager::DestroyFileConverter( (CNtlWeManager **)&pcFileConverter );

	return true;
}