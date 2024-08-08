//*****************************************************************************
// File       : OnMove.cpp
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
// Name			: OnMove
// Desc			: 
// Privilege	: Protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

LRESULT CApiWinApp::OnMove( void )
{
	if( CNavigator::GetInstance() == NULL )
	{
		return 0L;
	}

	if( m_pcRenderer == NULL )
	{
		return 0L;
	}

	RECT	sRect;

	GetClientRect( m_hWnd, &sRect );

	m_pcRenderer->SetScreen( sRect );

	RECT	sChildRect;

	if( m_hStatusWindow != NULL )
	{
		GetWindowRect( m_hStatusWindow, &sChildRect );

		MoveWindow( m_hStatusWindow,
					sRect.top, sRect.bottom - ( sChildRect.bottom - sChildRect.top ),
					sChildRect.right - sChildRect.left, sRect.bottom - sRect.top,
					TRUE );
	}
	else
	{
		ZeroMemory( &sChildRect, sizeof( RECT ) );
	}

	m_sConsoleRect.left = sRect.left + 4;
	m_sConsoleRect.right = sRect.right - 4;
	m_sConsoleRect.bottom = sRect.bottom - ( sChildRect.bottom - sChildRect.top ) - 4;
	m_sConsoleRect.top = m_sConsoleRect.bottom - 16;

	return 0L;
}


//*****************************************************************************
//
// End of File : OnMove.cpp
//
//*****************************************************************************