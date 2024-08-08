//*****************************************************************************
// File       : CApiWinApp_WndProc.cpp
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

#include "CApiWinApp.h"


//-----------------------------------------------------------------------------
// Initialize Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Name			: WndProc
// Desc			: 
// Privilege	: static public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

LRESULT CALLBACK CApiWinApp::WndProc( HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam )
{
	CApiWinApp *		pcWindow = GetWindow( hWnd );

	if( pcWindow == NULL )
	{
		pcWindow = m_pcDefaultWindow;

		pcWindow->SetHwnd( hWnd );
	}

	return pcWindow->WndProc( nMsg, wParam, lParam );
}


//-----------------------------------------------------------------------------
// Name			: WndProc
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

LRESULT CApiWinApp::WndProc( UINT nMsg, WPARAM wParam, LPARAM lParam )
{
	LRESULT		nResult = 0L;

	switch( nMsg )
	{
		case WM_CREATE:
		{
			nResult = OnCreate( reinterpret_cast<CREATESTRUCT *>(lParam) );

			break;
		}

		case WM_DESTROY:
		{
			OnDestroy();

			PostQuitMessage( 0 );

			break;
		}

		case WM_COMMAND:
		{
			int	wmId    = LOWORD( wParam );
			int	wmEvent = HIWORD( wParam );

			DBG_UNREFERENCED_LOCAL_VARIABLE( wmEvent );

			switch( wmId )
			{
				case IDM_ABOUT:
				{
					nResult = OnAbout();

					break;
				}

				case IDM_EXIT:
				{
					DestroyWindow( m_hWnd );

					break;
				}

				default:
				{
					return DefWindowProc( m_hWnd, nMsg, wParam, lParam );
				}
			}

			break;
		}

		case WM_MOVE:
		case WM_SIZE:
		{
			nResult = OnMove();

			break;
		}

		case WM_PAINT:
		{
			PAINTSTRUCT		sPaintStruct;
			HDC		hDC;

			hDC = BeginPaint( m_hWnd, &sPaintStruct );

			nResult = OnPaint( hDC, &(sPaintStruct.rcPaint) );

			EndPaint( m_hWnd, &sPaintStruct );

			break;
		}

		case WM_LBUTTONDOWN:
		{
			UINT	nKeyFlags = static_cast<UINT>(wParam);
			POINT	sPoint;

			sPoint.x = LOWORD( lParam );
			sPoint.y = HIWORD( lParam );

			nResult = OnLButtonDown( nKeyFlags, &sPoint );

			break;
		}

		case WM_LBUTTONUP:
		{
			UINT	nKeyFlags = static_cast<UINT>(wParam);
			POINT	sPoint;
			sPoint.x = LOWORD( lParam );
			sPoint.y = HIWORD( lParam );

			nResult = OnLButtonUp( nKeyFlags, &sPoint );

			break;
		}

		case WM_MOUSEMOVE:
		{
			UINT	nKeyFlags = static_cast<UINT>(wParam);
			POINT	sPoint;
			sPoint.x = LOWORD( lParam );
			sPoint.y = HIWORD( lParam );

			nResult = OnMouseMove( nKeyFlags, &sPoint );

			break;
		}

		case WM_KEYUP:
		{
			if( m_bConsoleMode == true )
			{
				if( wParam >= 0x20 && wParam < 0x80 )
				{
					OnKeyUp( static_cast<UINT>(wParam) );
				}
			}

			break;
		}

		default:
		{
			return DefWindowProc( m_hWnd, nMsg, wParam, lParam );
		}
	}

	return nResult;
}


//-----------------------------------------------------------------------------
// Name			: WndProc
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CApiWinApp::Invalidate( bool bErase ) const
{
	InvalidateRect( m_hWnd, NULL, bErase );

	return;
}


//*****************************************************************************
//
// End of File : CApiWinApp_WndProc.cpp
//
//*****************************************************************************