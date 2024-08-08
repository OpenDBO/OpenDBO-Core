//*****************************************************************************
// File       : OnPaint.cpp
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
// Name			: OnPaint
// Desc			: 
// Privilege	: Protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

LRESULT CApiWinApp::OnPaint( HDC hDC, RECT * psRect )
{
	UNREFERENCED_PARAMETER( psRect );

	if( CNavigator::GetInstance() == NULL )
	{
		return 0L;
	}

	if( m_pcRenderer == NULL )
	{
		return 0L;
	}

	m_pcRenderer->PreRender( &hDC );

	m_cPath.Render( m_pcRenderer );

	CNavigator::GetInstance()->Render( m_pcRenderer ); 

	m_pcRenderer->PostRender();

	SetBkColor( hDC, 0x00000000 );
	SetTextColor( hDC, 0x0000FFFF );

	TextOut( hDC, m_sConsoleRect.left, m_sConsoleRect.top, m_szConsoleText, static_cast<Int32>(_tcslen( _T( m_szConsoleText ) )) );

	return 0L;
}


//*****************************************************************************
//
// End of File : OnPaint.cpp
//
//*****************************************************************************