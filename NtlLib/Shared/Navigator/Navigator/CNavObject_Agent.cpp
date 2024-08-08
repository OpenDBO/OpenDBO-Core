//*****************************************************************************
// File       : CNavObject_Shape.cpp
// Desc       : 
// Author     : June C. Kim (cynis@hotmail.com)
// Update     : 2006. 12. 5
//*****************************************************************************

//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "StdAfx.h"

#include "CNavPathEngine.h"

#include "CNavObject.h"


//-----------------------------------------------------------------------------
// Name Space Table
//-----------------------------------------------------------------------------

USE_NAMESPACE( nav )


//-----------------------------------------------------------------------------
// Initialize Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Name			: GetVertex
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CNavObject::GetVertex( UInt32 nVertexIndex, CNavPosition & rcPostion ) const
{
	Assert( m_pcShape != NULL );

	if( nVertexIndex >= m_nVertexCount )
	{
		return false;
	}

	long	nX, nY;

	m_pcShape->vertex( nVertexIndex, nX, nY );

	rcPostion.Set( m_cPosition.GetX() + nX, m_cPosition.GetY() + nY, m_cPosition.GetZ() );

	return true;
}


//-----------------------------------------------------------------------------
// Name			: CalculateBoundingFromShape
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CNavObject::CalculateBoundingFromShape( void )
{
	Assert( m_pcShape != NULL );

	long	nMinX	= MAX_VAL_INT_32;
	long	nMinY	= MAX_VAL_INT_32;
	long	nMaxX	= MIN_VAL_INT_32;
	long	nMaxY	= MIN_VAL_INT_32;
	long	nVertexX;
	long	nVertexY;

	for( Int32 nVertexIndex = 0L; nVertexIndex < m_pcShape->size(); ++nVertexIndex )
	{
		m_pcShape->vertex( nVertexIndex, nVertexX, nVertexY );

		nMinX = Min( nMinX, nVertexX );
		nMaxX = Max( nMaxX, nVertexX );

		nMinY = Min( nMinY, nVertexY );
		nMaxY = Max( nMaxY, nVertexY );
	}

	m_cBounding.SetMin( m_cPosition.GetX() + nMinX, m_cPosition.GetY() + nMinY, m_cPosition.GetZ() );
	m_cBounding.SetMax( m_cPosition.GetX() + nMaxX, m_cPosition.GetY() + nMaxY, m_cPosition.GetZ() );

	return;
}

//*****************************************************************************
//
// End of File : CNavObject_Shape.cpp
//
//*****************************************************************************