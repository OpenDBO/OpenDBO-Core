//*****************************************************************************
// File       : CNavFaceVertexMesh_iFaceVertexMesh.cpp
// Desc       : 
// Author     : June C. Kim (cynis@hotmail.com)
// Update     : 2007. 1. 18
//*****************************************************************************

//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "StdAfx.h"

#include "CNavFaceVertexMesh.h"


//-----------------------------------------------------------------------------
// Name Space Table
//-----------------------------------------------------------------------------

USE_NAMESPACE( nav )


//-----------------------------------------------------------------------------
// Initialize Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Name			: vertexIndex
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

long CNavFaceVertexMesh::vertexIndex( long face, long vertexInFace ) const
{
	Assert( face >= 0 );
	Assert( face < m_nFaceCount );
	Assert( vertexInFace >= 0 );
	Assert( vertexInFace < 3 );
	Assert( m_psFaceVertices != NULL );

	return m_psFaceVertices[face].GetIndex( vertexInFace );
}


//-----------------------------------------------------------------------------
// Name			: vertexX
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

long CNavFaceVertexMesh::vertexX( long vertex ) const
{
	Assert( vertex >= 0 );
	Assert( vertex < static_cast<long>(m_nVertexCount) );
	Assert( m_pcVertexPositions != NULL );

	return static_cast<long>(m_pcVertexPositions[vertex].GetX());
}


//-----------------------------------------------------------------------------
// Name			: vertexY
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

long CNavFaceVertexMesh::vertexY( long vertex ) const
{
	Assert( vertex >= 0 );
	Assert( vertex < static_cast<long>(m_nVertexCount) );
	Assert( m_pcVertexPositions != NULL );

	return static_cast<long>(m_pcVertexPositions[vertex].GetY());
}


//-----------------------------------------------------------------------------
// Name			: vertexZ
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

float CNavFaceVertexMesh::vertexZ( long vertex ) const
{
	Assert( vertex >= 0 );
	Assert( vertex < static_cast<long>(m_nVertexCount) );
	Assert( m_pcVertexPositions != NULL );

	return 0.0f; //static_cast<float>(m_pcVertexPositions[vertex].GetZ());
}


//-----------------------------------------------------------------------------
// Name			: vertexZ
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

long CNavFaceVertexMesh::faceAttribute( long face, long attributeIndex ) const
{
	Assert( face >= 0 );
	Assert( face < m_nFaceCount );
	Assert( m_psFaceAttributes != NULL );

	static SNavFaceAttribute		DEFAULT_FACE_ATTRIBUTE;
	SNavFaceAttribute *				psCurrentAttribute;

	psCurrentAttribute = &m_psFaceAttributes[face];

	switch( attributeIndex )
	{
		case PE_FaceAttribute_SurfaceType:
		{
			return psCurrentAttribute->nSurfaceType;
		}

		case PE_FaceAttribute_SectionID:
		{
			return psCurrentAttribute->nSectionID;
		}

		case PE_FaceAttribute_UserData:
		{
			return psCurrentAttribute->nUserData;
		}

		case PE_FaceAttribute_MarksPortal:
		{
			return psCurrentAttribute->bPortal;
		}

		case PE_FaceAttribute_MarksExternalShape:
		{
			return psCurrentAttribute->bExternalShape;
		}

		case PE_FaceAttribute_MarksConnectingEdge:
		{
			return psCurrentAttribute->bConnectingEdge;
		}

		default:
		{
			return -1;
		}
	}
}


//*****************************************************************************
//
// End of File : CNavFaceVertexMesh.cpp
//
//*****************************************************************************