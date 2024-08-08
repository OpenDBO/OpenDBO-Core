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

#include "CNavEntityHierarchy.h"

#include "CApiWinApp.h"


//-----------------------------------------------------------------------------
// Initialize Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Name			: OnMouseMove
// Desc			: 
// Privilege	: Protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

LRESULT CApiWinApp::OnMouseMove( UINT nKeyFlags, POINT * psPoint )
{
	UNREFERENCED_PARAMETER( nKeyFlags );

	if( CNavigator::GetInstance() == NULL )
	{
		return 0L;
	}

	CNavPosition2D		cMousePosition;
	
	cMousePosition.SetFloat( static_cast<Float32>(psPoint->x), static_cast<Float32>(psPoint->y) );

	CNavPosition		cCursorPosition;

	if( m_pcRenderer != NULL )
	{
		m_pcRenderer->UnprojectPosition( cCursorPosition, cMousePosition );
	}

	CFixedString	cHierarchyInfo;
	CNavEntityHierarchy		cHierarchy;

	CNavigator::GetInstance()->GetHierarchy( cHierarchy, cCursorPosition );

	if( cHierarchy.GetWorld() != NULL )
	{
		cHierarchyInfo.Append( cHierarchy.GetWorld()->GetName() );
	}

	if( cHierarchy.GetZone() != NULL )
	{
		cHierarchyInfo.Append( _T( ", " ) );
		cHierarchyInfo.Append( cHierarchy.GetZone()->GetName() );
	}

	if( cHierarchy.GetSection() != NULL )
	{
		cHierarchyInfo.Append( _T( ", " ) );
		cHierarchyInfo.Append( cHierarchy.GetSection()->GetName() );
	}

	if( cHierarchy.GetArea() != NULL )
	{
		cHierarchyInfo.Append( _T( ", " ) );
		cHierarchyInfo.Append( cHierarchy.GetArea()->GetName() );
	}

	if( cHierarchy.GetSector() != NULL )
	{
		cHierarchyInfo.Append( _T( ", " ) );
		cHierarchyInfo.Append( cHierarchy.GetSector()->GetName() );
	}


	static const CFixedString	sBlock[2] = { _T( "Unblocked" ), _T( "Blocked" ) };
	bool	bBlocked;

	bBlocked = CNavigator::GetInstance()->IsBlockedPosition( cCursorPosition );

	TCHAR	szText[1024];

	if( CNavigator::GetInstance()->GetHeight( cCursorPosition ) == true )
	{
		_stprintf_s( szText, 1024, _T( "%f, %f, %f (%s : 0x%08X) %s" ),
					cCursorPosition.GetFloatX(), cCursorPosition.GetFloatY(), cCursorPosition.GetFloatZ(),
					sBlock[bBlocked].GetBuffer(), CNavigator::GetInstance()->GetAttribute( cCursorPosition ),
					cHierarchyInfo );
	}
	else
	{
		_stprintf_s( szText, 1024, _T( "%f, %f (%s : 0x%08X) %s" ),
					cCursorPosition.GetFloatX(), cCursorPosition.GetFloatZ(),
					sBlock[bBlocked].GetBuffer(), CNavigator::GetInstance()->GetAttribute( cCursorPosition ),
					cHierarchyInfo );
	}

	CFixedString	cStatus( szText );

	SetStatusWindowText( cStatus );

	return 0L;
}


//*****************************************************************************
//
// End of File : OnMouseMove.cpp
//
//*****************************************************************************