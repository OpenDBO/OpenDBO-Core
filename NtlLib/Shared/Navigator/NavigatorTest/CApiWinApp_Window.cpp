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

#include "CApiWinApp.h"


//-----------------------------------------------------------------------------
// Initialize Table
//-----------------------------------------------------------------------------

CApiWinApp *			CApiWinApp::m_pcDefaultWindow = NULL;


//-----------------------------------------------------------------------------
// Name			: RegisterWindow
// Desc			: 
// Privilege	: static protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CApiWinApp::RegisterWindow( HWND hWnd, CApiWinApp * pcWinApp )
{
	UNREFERENCED_PARAMETER( hWnd );

//	Assert( pcWinApp->m_hWnd == hWnd );

	m_pcDefaultWindow = pcWinApp;

	return;
}


//-----------------------------------------------------------------------------
// Name			: UnregisterWindow
// Desc			: 
// Privilege	: static protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CApiWinApp::UnregisterWindow( HWND hWnd )
{
	UNREFERENCED_PARAMETER( hWnd );

//	Assert( m_pcDefaultWindow->m_hWnd == hWnd );

	m_pcDefaultWindow = NULL;

	return;
}


//-----------------------------------------------------------------------------
// Name			: GetWindow
// Desc			: 
// Privilege	: static protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CApiWinApp * CApiWinApp::GetWindow( HWND hWnd )
{
//	Assert( m_pcDefaultWindow != NULL );
//	Assert( m_pcDefaultWindow->m_hWnd == hWnd );

	if( hWnd == m_pcDefaultWindow->GetHwnd() )
	{
		return m_pcDefaultWindow;
	}
	else
	{
		return NULL;
	}
}


//*****************************************************************************
//
// End of File : CApiWinApp.cpp
//
//*****************************************************************************