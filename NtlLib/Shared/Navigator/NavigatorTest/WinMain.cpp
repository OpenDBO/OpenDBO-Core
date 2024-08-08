//*****************************************************************************
// File       : WinMain.cpp
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

#include "WinMain.h"


//-----------------------------------------------------------------------------
// Initialize Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Name			: _tWinMain
// Desc			: 
// Privilege	: Global
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

int APIENTRY _tWinMain( HINSTANCE hInstance,
						HINSTANCE hPrevInstance,
						LPTSTR    lpCmdLine,
						int       nCmdShow )
{
	UNREFERENCED_PARAMETER( hPrevInstance );
	UNREFERENCED_PARAMETER( lpCmdLine );

 	// TODO: Place code here.

	CApiWinApp		cApp;

	cApp.RegisterClass( hInstance );

	if( !cApp.InitInstance( nCmdShow ) )
	{
		return FALSE;
	} 

	return cApp.Run();
}


//*****************************************************************************
//
// End of File : WinMain.cpp
//
//*****************************************************************************