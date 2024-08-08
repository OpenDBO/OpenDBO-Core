//*****************************************************************************
// File       : OnLButtonDown.cpp
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
// Name			: OnLButtonDown
// Desc			: 
// Privilege	: Protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

LRESULT CApiWinApp::OnLButtonDown( UINT nKeyFlags, POINT * psPoint )
{
	UNREFERENCED_PARAMETER( nKeyFlags );

	if( CNavigator::GetInstance() == NULL )
	{
		return 0L;
	}

	CNavPosition2D		cMousePosition;
	cMousePosition.SetFloat( static_cast<Float32>(psPoint->x), static_cast<Float32>(psPoint->y) );

	CNavPosition		cSourcePosition;

	m_pcRenderer->UnprojectPosition( cSourcePosition, cMousePosition );

	m_cPath.SetSourcePosition( cSourcePosition );

	return 0L;
}


//*****************************************************************************
//
// End of File : OnLButtonDown.cpp
//
//*****************************************************************************