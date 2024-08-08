//*****************************************************************************
// File       : OnCreate.cpp
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

#include "CNavEntityFactory.h"
#include "CNavObject.h"

#include "CApiRenderer.h"

#include "CApiWinApp.h"


//-----------------------------------------------------------------------------
// Initialize Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Name			: OnCreate
// Desc			: 
// Privilege	: Protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

LRESULT CApiWinApp::OnCreate( CREATESTRUCT * psCreateStruct )
{
	UNREFERENCED_PARAMETER( psCreateStruct );

	g_Log( "CApiWinApp::OnCreate() : Begin" );

	// Set Title Bar

	TCHAR		acVersion[1024];
	TCHAR		acTitle[1024];
	CNavigator::GetVersionInfo( acVersion, 1024 );
	_stprintf_s( acTitle, 1024, _T( "%s (%s)" ), m_szTitle, acVersion );

	SetWindowText( m_hWnd, acTitle );

	// Set Status Bar

	if( CreateStatusWindow() == false )
	{
		return -1;
	}

	// Create Navigator

#ifndef WIN64
	CFixedString	cEngineName( _T( "PathEngine" ) );
#else
	CFixedString	cEngineName( _T( "PathEngine_x64" ) );
#endif //WIN64

	CFixedString	cSessionName( _T( "NavServer" ) );

	if( CNavigator::CreateInstance( cEngineName, cSessionName, NULL, E_NAV_ENTITY_CREATE_INFO_FLAG_CLIENT_MODE ) == NULL )
	{
		Assert( !"Engine Create Fail" );

		return -1;
	}

	// Register Player Shape

	CFixedString	cShapeName( _T( "Shape001" ) );

	CNavigator::GetInstance()->RegisterShape( cShapeName );

	m_pcRenderer = new CApiRenderer;
	Assert( m_pcRenderer != NULL );

	g_Log( "CApiWinApp::OnCreate() : End" );

	return 0;
}


//*****************************************************************************
//
// End of File : OnCreate.cpp
//
//*****************************************************************************