//*****************************************************************************
// File       : CApiWinApp_AboutWndProc.cpp
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
// Name			: AboutWndProc
// Desc			: 
// Privilege	: static protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

INT_PTR CALLBACK CApiWinApp::AboutWndProc( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam )
{
	UNREFERENCED_PARAMETER( lParam );

	switch (message)
	{
		case WM_INITDIALOG:
		{
			return (INT_PTR)TRUE;
		}

		case WM_COMMAND:
		{
			int	nRet = LOWORD( wParam );

			if( nRet == IDOK || nRet == IDCANCEL)
			{
				EndDialog( hDlg, nRet );

				return (INT_PTR)TRUE;
			}

			break;
		}
	}

	return (INT_PTR)FALSE;
}


//*****************************************************************************
//
// End of File : CApiWinApp_AboutWndProc.cpp
//
//*****************************************************************************