//*****************************************************************************
// File       : CApiWinApp_StatusWindow.cpp
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
// Name			: CreateStatusWindow
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CApiWinApp::CreateStatusWindow( void )
{
	InitCommonControls();

	m_hStatusWindow = ::CreateStatusWindow( WS_CHILD | WS_VISIBLE, _T( "Ready" ), m_hWnd, ID_STATUS_WINDOW );

	return true;
}


//-----------------------------------------------------------------------------
// Name			: SetStatusWindowText
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CApiWinApp::SetStatusWindowText( const CFixedString & rcString )
{
	if( m_hStatusWindow != NULL )
	{
		::SetWindowText( m_hStatusWindow, rcString.GetBuffer() );
	}

	return;
}

//*****************************************************************************
//
// End of File : CApiWinApp_StatusWindow.cpp
//
//*****************************************************************************

