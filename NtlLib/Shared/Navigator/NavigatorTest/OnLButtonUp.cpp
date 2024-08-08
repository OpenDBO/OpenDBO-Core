//*****************************************************************************
// File       : OnLButtonUp.cpp
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
// Name			: OnLButtonUp
// Desc			: 
// Privilege	: Protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

LRESULT CApiWinApp::OnLButtonUp( UINT nKeyFlags, POINT * psPoint )
{
	UNREFERENCED_PARAMETER( nKeyFlags );

	if( CNavigator::GetInstance() == NULL )
	{
		return 0L;
	}

	CNavPosition2D		cMousePosition;
	cMousePosition.SetFloat( static_cast<Float32>(psPoint->x), static_cast<Float32>(psPoint->y) );

	CNavPosition		cDestinationPosition;

	m_pcRenderer->UnprojectPosition( cDestinationPosition, cMousePosition );

	m_cPath.SetDestinationPosition( cDestinationPosition );
	m_cPath.SetShape( 0 );

	bool	bResult = FindPath( m_cPath );

	if( bResult == false )
	{
		return -1;
	}

	m_pcRenderer->Invalidate();
	Invalidate();

	return 0L;
}


//*****************************************************************************
//
// End of File : OnLButtonUp.cpp
//
//*****************************************************************************