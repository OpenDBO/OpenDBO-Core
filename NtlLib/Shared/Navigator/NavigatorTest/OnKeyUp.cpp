//*****************************************************************************
// File       : OnKeyUp.cpp
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
// Name			: OnKeyUp
// Desc			: 
// Privilege	: Protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

LRESULT CApiWinApp::OnKeyUp( UINT nVirtualKey )
{
	if( CNavigator::GetInstance() == NULL )
	{
		return 0L;
	}

	Fixed32		nMeterPerPixel = m_pcRenderer->GetMeterPerPixel();
	Fixed32		SPEED = 1000;

	switch( nVirtualKey )
	{
		case VK_UP:
		{
			CNavPosition2D		cMovement( 0, -(SPEED) );

			m_pcRenderer->MoveCamera( cMovement );

			InvalidateRect( m_hWnd, NULL, TRUE );

			break;
		}

		case VK_DOWN:
		{
			CNavPosition2D		cMovement( 0, (SPEED) );

			m_pcRenderer->MoveCamera( cMovement );

			InvalidateRect( m_hWnd, NULL, TRUE );

			break;
		}

		case VK_LEFT:
		{
			CNavPosition2D		cMovement( -(SPEED), 0 );

			m_pcRenderer->MoveCamera( cMovement );

			InvalidateRect( m_hWnd, NULL, TRUE );

			break;
		}

		case VK_RIGHT:
		{
			CNavPosition2D		cMovement( (SPEED), 0 );

			m_pcRenderer->MoveCamera( cMovement );
			
			InvalidateRect( m_hWnd, NULL, TRUE );

			break;
		}

		case VK_PAGEUP:
		{
			nMeterPerPixel *= 2;
			if( nMeterPerPixel > 2048 )
			{
				nMeterPerPixel = 2048;
			}

			m_pcRenderer->SetMeterPerPixel( nMeterPerPixel );
			
			InvalidateRect( m_hWnd, NULL, TRUE );

			break;
		}

		case VK_PAGEDOWN:
		{
			nMeterPerPixel /= 2;
			if( nMeterPerPixel < 1 )
			{
				nMeterPerPixel = 1;
			}

			m_pcRenderer->SetMeterPerPixel( nMeterPerPixel );

			InvalidateRect( m_hWnd, NULL, TRUE );

			break;
		}

		case VK_HOME:
		{
			AfxMessageBox( "Simulate : Begin" );

			Simulate();

			AfxMessageBox( "Simulate : Done" );

			break;
		}

		default:
		{
			size_t	nLength = _tcslen( m_szConsoleText );

			if( nLength < 1024 )
			{
				m_szConsoleText[nLength] = static_cast<TCHAR>(nVirtualKey);
				m_szConsoleText[nLength + 1] = _T( '\0' );

				InvalidateRect( m_hWnd, &m_sConsoleRect, FALSE );
			}
		}
	}

	return 0;
}


//*****************************************************************************
//
// End of File : OnKeyUp.cpp
//
//*****************************************************************************