//*****************************************************************************
// File       : CNavTerrain_Mesh.cpp
// Desc       : 
// Author     : June C. Kim (cynis@hotmail.com)
// Update     : 2007. 1. 18
//*****************************************************************************

//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "StdAfx.h"

#include "CNavPathEngine.h"
#include "CNavEntityVector.h"

#include "CNavTerrain.h"


//-----------------------------------------------------------------------------
// Name Space Table
//-----------------------------------------------------------------------------

USE_NAMESPACE( nav )


//-----------------------------------------------------------------------------
// Initialize Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Name			: ConvertPosition
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CNavTerrain::ConvertPosition( cPosition & rcResultPosition, const CNavPosition & rcPosition ) const
{
	Assert( m_pcMesh != NULL );

	long	anVertex[3];

	anVertex[0] = static_cast<long>(rcPosition.GetX());
	anVertex[1] = static_cast<long>(rcPosition.GetY());
	anVertex[2] = static_cast<long>(rcPosition.GetZ());

//	g_Log( _T( "iMesh::positionFor3DPoint() : Begin" ) );
	rcResultPosition = m_pcMesh->positionFor3DPoint( reinterpret_cast<const long *>(anVertex) );
//	g_Log( _T( "iMesh::positionFor3DPoint() : End" ) );

	return (rcResultPosition.cell >= 0 );
}


//-----------------------------------------------------------------------------
// Name			: GetVertex
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CNavTerrain::GetVertex( UInt32 nFaceIndex, UInt32 nVertexIndex, CNavPosition & rcPostion ) const
{
	Assert( m_pcMesh != NULL );

	if( nFaceIndex >= m_nFaceCount )
	{
		return false;
	}

	Assert( nVertexIndex < 3 );

	long	nX, nY;
	float	fZ;

	m_pcMesh->get3DFaceVertex( nFaceIndex, nVertexIndex, nX, nY, fZ );

	rcPostion.Set( nX, nY, Float32ToFixed32( fZ ) );

	return true;
}


//-----------------------------------------------------------------------------
// Name			: GetConnection
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

UInt32 CNavTerrain::GetConnection( UInt32 nFaceIndex, UInt32 nEdgeIndex ) const
{
	Assert( m_pcMesh != NULL );

	if( nFaceIndex >= m_nFaceCount )
	{
		return FACE_ID_INVALID;
	}

	Assert( nEdgeIndex < 3 );

	return static_cast<UInt32>(m_pcMesh->get3DFaceConnection( static_cast<long>(nFaceIndex), static_cast<long>(nEdgeIndex) ) );
}


//*****************************************************************************
//
// End of File : CNavTerrain_Mesh.cpp
//
//*****************************************************************************