//*****************************************************************************
// File       : CApiWinApp.cpp
// Desc       : 
// Begin      : 
// Copyright  : 
// Author     : 
// Update     : 
//*****************************************************************************

//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "StdAfx.h"

#include "CApiRenderer.h"

#include "CApiWinApp.h"


//-----------------------------------------------------------------------------
// Initialize Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Name			: Initialize
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CApiWinApp::Initialize( void )
{
	m_hInstance		= NULL;
	m_hWnd			= NULL;

	m_pcRenderer	= NULL;

	m_bActive		= true;

	_tcscpy_s( m_szTitle, MAX_LOADSTRING, _T( "" ) );
	_tcscpy_s( m_szWindowClass, MAX_LOADSTRING, _T( "" ) );

	m_hStatusWindow	= NULL;

	ZeroMemory( m_szConsoleText, sizeof( TCHAR ) * 1024 );

	m_bConsoleMode = false;

	return;
}


//-----------------------------------------------------------------------------
// Name			: Destroy
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CApiWinApp::Destroy( void )
{
	UnregisterWindow( m_hWnd );

	Initialize();

	if( m_pcRenderer != NULL )
	{
		delete m_pcRenderer;
		m_pcRenderer = NULL;
	}

	return;
}


//-----------------------------------------------------------------------------
// Name			: RegisterClass
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

ATOM CApiWinApp::RegisterClass( HINSTANCE hInstance )
{
	m_hInstance = hInstance;

	LoadString( m_hInstance, IDS_APP_TITLE, m_szTitle, MAX_LOADSTRING );
	LoadString( m_hInstance, IDC_NAVIGATORTEST, m_szWindowClass, MAX_LOADSTRING );

	WNDCLASSEX		sMainWindowClass;

	sMainWindowClass.cbSize			= sizeof( WNDCLASSEX );
	sMainWindowClass.style			= CS_HREDRAW | CS_VREDRAW;
	sMainWindowClass.lpfnWndProc	= CApiWinApp::WndProc;
	sMainWindowClass.cbClsExtra		= 0;
	sMainWindowClass.cbWndExtra		= 0;
	sMainWindowClass.hInstance		= m_hInstance;
	sMainWindowClass.hIcon			= LoadIcon( hInstance, MAKEINTRESOURCE( IDI_NAVIGATORTEST ) );
	sMainWindowClass.hCursor		= LoadCursor( NULL, IDC_ARROW );
	sMainWindowClass.hbrBackground	= (HBRUSH)(COLOR_WINDOW + 2);
	sMainWindowClass.lpszMenuName	= MAKEINTRESOURCE( IDC_NAVIGATORTEST );
	sMainWindowClass.lpszClassName	= m_szWindowClass;
	sMainWindowClass.hIconSm		= LoadIcon( hInstance, MAKEINTRESOURCE( IDI_SMALL ) );

	return RegisterClassEx( &sMainWindowClass );
}


//-----------------------------------------------------------------------------
// Name			: InitInstance
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

BOOL CApiWinApp::InitInstance( int nCmdShow )
{
	RegisterWindow( m_hWnd, this );

	m_hWnd = CreateWindow( m_szWindowClass, m_szTitle, WS_OVERLAPPEDWINDOW,
							CW_USEDEFAULT, 0, CW_USEDEFAULT, 0,
							NULL, NULL, m_hInstance, NULL );
	if( m_hWnd == NULL )
	{
		return FALSE;
	}

	ShowWindow( m_hWnd, nCmdShow );
	UpdateWindow( m_hWnd );

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: Run
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

int CApiWinApp::Run( void )
{
	HACCEL	hAccelTable;
	hAccelTable = LoadAccelerators( m_hInstance, MAKEINTRESOURCE( IDC_NAVIGATORTEST ) );

	MSG		sMsg;
	sMsg.message = WM_NULL;

	PeekMessage( &sMsg, NULL, 0U, 0U, PM_NOREMOVE );

	while( sMsg.message != WM_QUIT )
	{
		BOOL	bGotMessage;

		if( m_bActive )
		{
            bGotMessage = PeekMessage( &sMsg, NULL, 0U, 0U, PM_REMOVE );
		}
        else
		{
            bGotMessage = GetMessage( &sMsg, NULL, 0U, 0U );
		}

		if( bGotMessage == TRUE )
		{
			if( TranslateAccelerator( m_hWnd, hAccelTable, &sMsg ) == 0 )
            {
				TranslateMessage( &sMsg );
				DispatchMessage( &sMsg );
			}
		}
		else
		{
			if( m_bActive == true )
			{
				Idle();
			}
		}
	}

	return (int)sMsg.wParam;
}


//-----------------------------------------------------------------------------
// Name			: Idle
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

int CApiWinApp::Idle( void )
{
	UpdateKeyState();

	return 0L;
}


//-----------------------------------------------------------------------------
// Name			: UpdateKeyState
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CApiWinApp::UpdateKeyState( void )
{
	if( GetAsyncKeyState( VK_UP ) )
	{
		OnKeyUp( VK_UP );
	}

	if( GetAsyncKeyState( VK_DOWN ) )
	{
		OnKeyUp( VK_DOWN );
	}

	if( GetAsyncKeyState( VK_LEFT ) )
	{
		OnKeyUp( VK_LEFT );
	}

	if( GetAsyncKeyState( VK_RIGHT ) )
	{
		OnKeyUp( VK_RIGHT );
	}

	if( GetAsyncKeyState( VK_PAGEUP ) )
	{
		OnKeyUp( VK_PAGEUP );
	}

	if( GetAsyncKeyState( VK_PAGEDOWN ) )
	{
		OnKeyUp( VK_PAGEDOWN );
	}

	if( GetAsyncKeyState( VK_HOME ) )
	{
		OnKeyUp( VK_HOME );
	}

	if( m_bConsoleMode == false )
	{
		if( GetAsyncKeyState( VK_CONSOLE ) )
		{
			m_bConsoleMode = true;

			ZeroMemory( m_szConsoleText, sizeof( TCHAR ) * 1024 );

			m_szConsoleText[0] = _T( '#' );

			InvalidateRect( m_hWnd, &m_sConsoleRect, FALSE );
		}
	}
	else
	{
		if( GetAsyncKeyState( VK_ENTER ) )
		{
			m_bConsoleMode = false;

			ExecuteConsoleCommand( m_szConsoleText );

			ZeroMemory( m_szConsoleText, sizeof( TCHAR ) * 1024 );

			InvalidateRect( m_hWnd, &m_sConsoleRect, TRUE );
		}

		if( GetAsyncKeyState( VK_ESC ) )
		{
			m_bConsoleMode = false;

			ZeroMemory( m_szConsoleText, sizeof( TCHAR ) * 1024 );

			InvalidateRect( m_hWnd, &m_sConsoleRect, TRUE );
		}

		if( GetAsyncKeyState( VK_BACKSPACE ) )
		{
			Int32	nLength = static_cast<Int32>(_tcslen( m_szConsoleText ));
			if( nLength > 0 )
			{
				m_szConsoleText[nLength - 1] = _T( '\0' );

				InvalidateRect( m_hWnd, &m_sConsoleRect, TRUE );
			}
		}
	}

	return;
}


//*****************************************************************************
//
// End of File : CApiWinApp.cpp
//
//*****************************************************************************