//*****************************************************************************
// File       : CNavFaceVertexMesh.h
// Desc       : 
// Author     : June C. Kim (cynis@hotmail.com)
// Update     : 2007. 1. 18
//*****************************************************************************

#ifndef ___C_NAV_FACE_VERTEX_MESH_H___
#define ___C_NAV_FACE_VERTEX_MESH_H___

//-----------------------------------------------------------------------------
// Pre-Compiler Option Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "CNavBounding.h"
#include "FaceVertexMeshBase.h"


//-----------------------------------------------------------------------------
// Name Space Table
//-----------------------------------------------------------------------------

BEGIN_NAMESPACE( nav )


//-----------------------------------------------------------------------------
// Type Forward Definition Table
//-----------------------------------------------------------------------------

class CNavPosition;
class CNavFaceVertexMesh;
class CNavEntity;


//-----------------------------------------------------------------------------
// Constant Table
//-----------------------------------------------------------------------------

#define NAV_FACE_VERTEX_MESH_VERTEX_BUFFER_SIZE_DEFAULT			( 4096 )
#define NAV_FACE_VERTEX_MESH_FACE_BUFFER_SIZE_DEFAULT			( 2048 )


//-----------------------------------------------------------------------------
// Type Definition Table
//-----------------------------------------------------------------------------


//=============================================================================
// Name       : CNavFaceVertexMesh
// Desc       : 
// Author     : 
//=============================================================================

class CNavFaceVertexMesh : public iFaceVertexMesh
{
	protected:

		CNavPosition *					m_pcVertexPositions;
		Int32							m_nVertexCount;
		Int32							m_nVertexBufferSize;

		SNavIndexedTriangle *			m_psFaceVertices;
		SNavFaceAttribute *				m_psFaceAttributes;
		Int32							m_nFaceCount;
		Int32							m_nFaceBufferSize;

		CNavBounding					m_cBound;

	protected:

		inline							CNavFaceVertexMesh( void ) { Initialize(); }

		inline virtual					~CNavFaceVertexMesh( void ) { Destroy(); }

	private:

										CNavFaceVertexMesh( const CNavFaceVertexMesh & rcMesh );

		const CNavFaceVertexMesh &		operator =( const CNavFaceVertexMesh & rcMesh );

	public:

		inline virtual long				faces() const { return m_nFaceCount; }
		inline virtual long				vertices() const { return m_nVertexCount; }
		virtual long					vertexIndex(long face, long vertexInFace) const;
		virtual long					vertexX(long vertex) const;
		virtual long					vertexY(long vertex) const;
		virtual float					vertexZ(long vertex) const;
		virtual long					faceAttribute(long face, long attributeIndex) const;

	public:

		virtual CNavEntity *			CreateTerrain( const CFixedString & rcName, Float32 fFilterSlope = 0.0f, Float32 fOptimizePrecision = 0.0f );

		virtual Int32					AddFace( void * pVertex0, void * pVertex1, void * pVertex2, void * pFace = NULL ) = 0;
		virtual Int32					AddRect( void * pVertexLB, void * pVertexRB, void * pVertexLT, void * pVertexRT, void * pFace = NULL ) = 0;
		virtual Int32					AddHeightMap( void * pvHeightMap ) = 0;

		inline const CNavBounding &		GetBound( void ) const { return m_cBound; }

	protected:

		virtual void					Initialize( void );
		virtual void					Destroy( void );

		void							ExpandVertexBuffer( UInt32 nBufferSize = 0 );
		void							ExpandFaceBuffer( UInt32 nBufferSize = 0 );

		virtual Int32					RegisterVertex( const CNavPosition & rcPosition );
		virtual Int32					RegisterFace( const SNavIndexedTriangle & rsIndex, SNavFaceAttribute * psAttribute = NULL );

		virtual Int32					RegisterVertexForce( const CNavPosition & rcPosition );
		virtual Int32					RegisterFaceForce( const SNavIndexedTriangle & rsIndex, SNavFaceAttribute * psAttribute = NULL );
};

//=============================================================================
// End of Class : CNavFaceVertexMesh
//=============================================================================

END_NAMESPACE( nav )

#endif //___C_NAV_FACE_VERTEX_MESH_H___

//*****************************************************************************
//
// End of File : CNavFaceVertexMesh.h
//
//*****************************************************************************