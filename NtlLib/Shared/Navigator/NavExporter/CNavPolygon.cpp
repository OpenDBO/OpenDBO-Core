//*****************************************************************************
// File       : CNavPolygon.cpp
// Desc       : 
// Author     : June C. Kim (cynis@hotmail.com)
// Update     : 2007. 1. 18
//*****************************************************************************

//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "StdAfx.h"

#include "CNavPosition2D.h"

#include "CNavPolygon.h"


//-----------------------------------------------------------------------------
// Name Space Table
//-----------------------------------------------------------------------------

USE_NAMESPACE( navex )


//-----------------------------------------------------------------------------
// Initialize Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Name			: Create
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CNavPolygon::Create( const CNavPositionList & rcVertexList, const CNavIndexedFaceList & rcFaceList )
{
	if( SortByOutline( rcVertexList, rcFaceList ) == false )
	{
		return false;
	}

	if( SortByClockwise() == false )
	{
		return false;
	}

	return CalculateCenter();
}


//-----------------------------------------------------------------------------
// Name			: SortByOutline
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CNavPolygon::SortByOutline( const CNavPositionList & rcVertexList, const CNavIndexedFaceList & rcFaceList )
{
	CNavIndexedFaceList		cEdgeList;
	cEdgeList.Alloc( rcVertexList.GetCount() );

	Int32	nBegin;
	Int32	nEnd;
	UInt32	nEdge;

	for( UInt32 nIndex = 0; nIndex < rcFaceList.GetCount(); ++nIndex )
	{
		if( rcFaceList.GetFace( nIndex )->GetFlags( 0 ) == 1 )
		{
			nBegin = rcFaceList.GetFace( nIndex )->GetIndex( 0 );
			nEnd = rcFaceList.GetFace( nIndex )->GetIndex( 1 );

			nEdge = 0;
			while( cEdgeList.GetFace( nBegin )->GetIndex( nEdge ) != -1 )
			{
				++nEdge;
			}

			cEdgeList.GetFace( nBegin )->SetIndex( nEdge, nEnd );

			nEdge = 0;
			while( cEdgeList.GetFace( nEnd )->GetIndex( nEdge ) != -1 )
			{
				++nEdge;
			}

			cEdgeList.GetFace( nEnd )->SetIndex( nEdge, nBegin );
		}

		if( rcFaceList.GetFace( nIndex )->GetFlags( 1 ) == 1 )
		{
			nBegin = rcFaceList.GetFace( nIndex )->GetIndex( 1 );
			nEnd = rcFaceList.GetFace( nIndex )->GetIndex( 2 );

			nEdge = 0;
			while( cEdgeList.GetFace( nBegin )->GetIndex( nEdge ) != -1 )
			{
				++nEdge;
			}

			cEdgeList.GetFace( nBegin )->SetIndex( nEdge, nEnd );

			nEdge = 0;
			while( cEdgeList.GetFace( nEnd )->GetIndex( nEdge ) != -1 )
			{
				++nEdge;
			}

			cEdgeList.GetFace( nEnd )->SetIndex( nEdge, nBegin );
		}

		if( rcFaceList.GetFace( nIndex )->GetFlags( 2 ) == 1 )
		{
			nBegin = rcFaceList.GetFace( nIndex )->GetIndex( 2 );
			nEnd = rcFaceList.GetFace( nIndex )->GetIndex( 0 );

			nEdge = 0;
			while( cEdgeList.GetFace( nBegin )->GetIndex( nEdge ) != -1 )
			{
				++nEdge;
			}

			cEdgeList.GetFace( nBegin )->SetIndex( nEdge, nEnd );

			nEdge = 0;
			while( cEdgeList.GetFace( nEnd )->GetIndex( nEdge ) != -1 )
			{
				++nEdge;
			}

			cEdgeList.GetFace( nEnd )->SetIndex( nEdge, nBegin );
		}
	}

	m_cVertexList.Alloc( rcVertexList.GetCount() );

	Int32	nPrevIndex = -1;
	Int32	nCurrIndex = 0;
	Int32	nNextIndex;
	UInt32	nSortedIndex = 0;

	while( nSortedIndex < m_cVertexList.GetCount() )
	{
		m_cVertexList.SetPosition( nSortedIndex++, *(rcVertexList.GetPosition( nCurrIndex )) );

		nEdge = 0;
		while( cEdgeList.GetFace( nCurrIndex )->GetIndex( nEdge ) == nPrevIndex )
		{
			++nEdge;
		}

		nNextIndex = cEdgeList.GetFace( nCurrIndex )->GetIndex( nEdge );
		nPrevIndex = nCurrIndex;
		nCurrIndex = nNextIndex;
	}

	return true;
}


//-----------------------------------------------------------------------------
// Name			: SortByClockwise
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CNavPolygon::SortByClockwise( void )
{
	CNavPosition	cVector0( *(m_cVertexList.GetPosition( 0 )) );
	cVector0.Sub( *(m_cVertexList.GetPosition( 1 )) );

	CNavPosition	cVector1( *(m_cVertexList.GetPosition( 2 )) );
	cVector1.Sub( *(m_cVertexList.GetPosition( 1 )) );

	if( cVector0.GetX() * cVector1.GetZ() - cVector0.GetZ() * cVector1.GetX() < 0 )
	{
		CNavPositionList	cReversePositionList( m_cVertexList );

		for( UInt32 nIndex = 0; nIndex < m_cVertexList.GetCount(); ++nIndex )
		{
			m_cVertexList.SetPosition( m_cVertexList.GetCount() - nIndex - 1, *(cReversePositionList.GetPosition( nIndex )) );
		}
	}

	return true;
}


//-----------------------------------------------------------------------------
// Name			: CalculateCenter
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CNavPolygon::CalculateCenter( void )
{
	if( m_cVertexList.GetCount() < MINIMUM_VERTEX_COUNT_FOR_POLYGON )
	{
		m_cCenter.Reset();

		return false;
	}

	for( UInt32 nIndex = 0; nIndex < m_cVertexList.GetCount(); ++nIndex )
	{
		m_cCenter.Add( *(m_cVertexList.GetPosition( nIndex )) );
	}

	m_cCenter.Div( m_cVertexList.GetCount() );

	return true;
}


//-----------------------------------------------------------------------------
// Name			: Convert
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CNavPolygon::Convert( Fixed32 * pnResultBuffer, CNavPosition2D & rcPosition, Float32 & rfDirection, CNavPosition2D & rcScale )
{
	Assert( pnResultBuffer != NULL );

	CNavPosition2D	cTransformedCenter;
	cTransformedCenter.SetX( m_cCenter.GetX() );
	cTransformedCenter.SetY( m_cCenter.GetZ() );

	cTransformedCenter.Scale( rcScale );
	cTransformedCenter.Rotate( rfDirection );

	CNavPosition2D	cTransformedVertex;

	for( UInt32 nVertexIndex = 0; nVertexIndex < m_cVertexList.GetCount(); ++nVertexIndex )
	{
		cTransformedVertex.SetX( m_cVertexList.GetPosition( nVertexIndex )->GetX() );
		cTransformedVertex.SetY( m_cVertexList.GetPosition( nVertexIndex )->GetZ() );

		cTransformedVertex.Scale( rcScale );
		cTransformedVertex.Rotate( rfDirection );
		cTransformedVertex.Sub( cTransformedCenter );

		pnResultBuffer[nVertexIndex * 2 + 0] = cTransformedVertex.GetX();
		pnResultBuffer[nVertexIndex * 2 + 1] = cTransformedVertex.GetY();
	}

	rcPosition.Add( cTransformedCenter );

	return true;
}


//-----------------------------------------------------------------------------
// Name			: Initialize
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CNavPolygon::Initialize( void )
{
	return;
}


//-----------------------------------------------------------------------------
// Name			: Destroy
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CNavPolygon::Destroy( void )
{
	return;
}


//*****************************************************************************
//
// End of File : CNavPolygon.cpp
//
//*****************************************************************************