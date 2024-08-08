//*****************************************************************************
// File       : CApiRenderer.cpp
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

#include "NavigatorTest.h"

#include "CApiRenderer.h"


//-----------------------------------------------------------------------------
// Type Definition Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Initialize Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Name			: Initialize
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CApiRenderer::Initialize( void )
{
	m_cCameraPosition.SetFloat( DEFAULT_POSITION_X, DEFAULT_POSITION_Y );
	m_nMeterPerPixel = 4;

	m_bRedraw = true;

	return;
}


//-----------------------------------------------------------------------------
// Name			: SetCamera
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CApiRenderer::SetCamera( const CNavPosition2D & rcPosition, Fixed32 nMeterPerPixel )
{
	m_cCameraPosition.Set( rcPosition );
	m_nMeterPerPixel = nMeterPerPixel;

	m_bRedraw = true;

	return;
}


//-----------------------------------------------------------------------------
// Name			: MoveCamera
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CApiRenderer::MoveCamera( const CNavPosition2D & rcMovement, UInt32 nFlags )
{
	UNREFERENCED_PARAMETER( nFlags );

	m_cCameraPosition.Add( rcMovement );

	m_bRedraw = true;

	return;
}


//-----------------------------------------------------------------------------
// Name			: SetScreen
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CApiRenderer::SetScreen( const RECT & rsRect, HDC hDC )
{
	m_hScreenDC = hDC;

	m_cScreenSize.SetFloat( static_cast<Float32>(rsRect.right), static_cast<Float32>(rsRect.bottom) );
	m_cScreenPosition.Set( (rsRect.right >> 1), (rsRect.bottom >> 1) );

	m_bRedraw = true;

	return;
}


//-----------------------------------------------------------------------------
// Name			: RenderLine
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CApiRenderer::RenderLine( const CNavPosition & rcFromPosition, const CNavPosition & rcToPosition, UInt32 nColor, UInt32 nStyle )
{
	if( m_bRedraw == false )
	{
		return true;
	}

	CNavPosition2D		cFromPosition2D;
	ProjectPosition( cFromPosition2D, rcFromPosition );

	CNavPosition2D		cToPosition2D;
	ProjectPosition( cToPosition2D, rcToPosition );

	if( IsVisiblePosition( cFromPosition2D ) == true ||
		IsVisiblePosition( cToPosition2D ) == true )
	{
		HPEN	hNewPen;
		HPEN	hOldPen;

		hNewPen = CreatePen( PS_SOLID, nStyle, nColor );
		hOldPen = reinterpret_cast<HPEN>(SelectObject( m_hScreenDC, hNewPen ));

		MoveToEx( m_hScreenDC, static_cast<int>(cFromPosition2D.GetFloatX()), static_cast<int>(cFromPosition2D.GetFloatY()), NULL );
		LineTo( m_hScreenDC, static_cast<int>(cToPosition2D.GetFloatX()), static_cast<int>(cToPosition2D.GetFloatY()) );

		SelectObject( m_hScreenDC, hOldPen );
		DeleteObject( hNewPen );
	}

	return true;
}


//-----------------------------------------------------------------------------
// Name			: PreRender
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CApiRenderer::PreRender( void * pvData )
{
	m_hScreenDC = *(reinterpret_cast<HDC *>(pvData)); 

	return true;
}


//-----------------------------------------------------------------------------
// Name			: PostRender
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CApiRenderer::PostRender( void * pvData )
{
	UNREFERENCED_PARAMETER( pvData );

	m_bRedraw = false;

	return true;
}


//-----------------------------------------------------------------------------
// Name			: ProjectPosition
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CApiRenderer::ProjectPosition( CNavPosition2D & rcResult, const CNavPosition & rcOriginal )
{
	rcResult.Set( rcOriginal.GetX(), rcOriginal.GetY() );
	rcResult.Sub( m_cCameraPosition.GetX(), m_cCameraPosition.GetY() );

	rcResult.Mul( m_nMeterPerPixel );

	rcResult.Add( m_cScreenPosition );

	return;
}


//-----------------------------------------------------------------------------
// Name			: UnprojectPosition
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CApiRenderer::UnprojectPosition( CNavPosition & rcResult, const CNavPosition2D & rcOriginal )
{
	rcResult.Set( rcOriginal.GetX(), rcOriginal.GetY(), 0 );

	CNavPosition	cScreenPosition3D( m_cScreenPosition.GetX(), m_cScreenPosition.GetY(), 0 );
	rcResult.Sub( cScreenPosition3D );

	rcResult.Div( m_nMeterPerPixel );

	CNavPosition	cCameraPosition3D( m_cCameraPosition.GetX(), m_cCameraPosition.GetY(), 0 );
	rcResult.Add( cCameraPosition3D );

	return;
}


//-----------------------------------------------------------------------------
// Name			: IsVisiblePosition
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------
		
bool CApiRenderer::IsVisiblePosition( const CNavPosition2D & rcPosition ) const
{
	if( rcPosition.GetX() < 0 )
	{
		return false;
	}

	if( rcPosition.GetY() < 0 )
	{
		return false;
	}

	if( rcPosition.GetX() > m_cScreenSize.GetX() )
	{
		return false;
	}

	if( rcPosition.GetY() > m_cScreenSize.GetY() )
	{
		return false;
	}

	return true;
}


//*****************************************************************************
//
// End of File : CApiRenderer.cpp
//
//*****************************************************************************