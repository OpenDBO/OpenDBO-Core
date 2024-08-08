//*****************************************************************************
// File       : CNavPositionFaceVertexMesh.h
// Desc       : 
// Author     : June C. Kim (cynis@hotmail.com)
// Update     : 2007. 1. 18
//*****************************************************************************

#ifndef ___C_NAV_POSITION_FACE_VERTEX_MESH_H___
#define ___C_NAV_POSITION_FACE_VERTEX_MESH_H___

//-----------------------------------------------------------------------------
// Pre-Compiler Option Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include <list>
using std::list;

#include "NavExporter.h"
#include "NtlWeBase.h"
#include "CNavTile.h"
#include "NavExporterBase.h"


//-----------------------------------------------------------------------------
// Name Space Table
//-----------------------------------------------------------------------------

BEGIN_NAMESPACE( navex )


//-----------------------------------------------------------------------------
// Type Forward Definition Table
//-----------------------------------------------------------------------------

class CNavTile;


//-----------------------------------------------------------------------------
// Constant Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Type Definition Table
//-----------------------------------------------------------------------------


//=============================================================================
// Name       : CNavPositionFaceVertexMesh
// Desc       : 
// Author     : 
//=============================================================================

class NAVEXPORTER_API CNavPositionFaceVertexMesh : public CNavFaceVertexMesh
{
	protected:

		CNavTile *						m_pcTiles;
		UInt32							m_nTileCount;
		Int32							m_nTileCountX;
		Int32							m_nTileCountY;

		CNavPosition					m_cPrecision;

		STileVertex *					m_psVertexInfo;

	protected:

		inline							CNavPositionFaceVertexMesh( void ) { Initialize(); }

		inline							~CNavPositionFaceVertexMesh( void ) { Destroy(); }

	private:

										CNavPositionFaceVertexMesh( const CNavPositionFaceVertexMesh & rcMesh );

		const CNavPositionFaceVertexMesh &	operator =( const CNavPositionFaceVertexMesh & rcMesh );

	public:

		// Common

		Int32							AddFace( void * pvVertex0, void * pvVertex1, void * pvVertex2, void * pvFace = NULL );
		Int32							AddRect( void * pvVertexLB, void * pvVertexRB, void * pvVertexLT, void * pvVertexRT, void * pvFace = NULL );
		Int32							AddMesh( void * pvVertexLT, void * pvVertexRB, void * pvSize, void * pvHeights = NULL, void * pvFace = NULL );
		Int32							AddHeightMap( void * pvHeightMap );

		// Exclusive
		
		Int32							BuildAndOptimize( Int32 nOptimizeLevel, UInt32 nFlags );

	protected:

		void							Initialize( void );
		void							Destroy( void );

		void							CreateTiles( Int32 nTileIndexX, Int32 nTileIndexY );
		void							DestroyTiles( void );

		inline UInt32					GetTileCount( void ) const { return m_nTileCount; }
		inline CNavTile * const			GetTile( UInt32 nTileIndex ) const { Assert( nTileIndex < m_nTileCount ); return &(m_pcTiles[nTileIndex]); }
		inline CNavTile * const			GetTile( Int32 nTileIndexX, Int32 nTileIndexY ) const { return GetTile( GetTileIndex( nTileIndexX, nTileIndexY ) ); }

		CNavTile * const				GetTileByPosition( const CNavPosition & rcPosition ) const;
		CNavTile * const				GetTileByPosition( Fixed32 nPositionX, Fixed32 nPositionY ) const;

		bool							GetTilePosition( CNavPosition & rcPosition, Int32 nIndexX, Int32 nIndexY, ENavTileVertexType eType = E_NAV_TILE_VERTEX_TYPE_LEFT_TOP ) const;

		inline UInt32					GetTileIndex( Int32 nTileIndexX, Int32 nTileIndexY ) const
										{
											Assert( nTileIndexX <= m_nTileCountX );
											Assert( nTileIndexY <= m_nTileCountY );

											return static_cast<UInt32>((++nTileIndexX) + (++nTileIndexY) * (m_nTileCountX + 2));
										}

		inline UInt32					GetVertexIndex( Int32 nVertexIndexX, Int32 nVertexIndexY ) const
										{
											Assert( nVertexIndexX <= m_nTileCountX );
											Assert( nVertexIndexY <= m_nTileCountY );

											return static_cast<UInt32>(nVertexIndexX + nVertexIndexY * (m_nTileCountX + 1));
										}

		Int32							BuildUnoptimizedMesh( UInt32 nFlags );
		Int32							BuildOptimizedMesh( UInt32 nFlags );

		bool							OptimizeMesh( Int32 nOptimizeInterval, UInt32 nFlags );

		bool							ExportTile( Int32 nSearchIndexX, Int32 nSearchIndexY, Int32 nExportRange );

		bool							Reserve( const CNavBounding & rcBound, const CNavPosition & rcPrecision );

		Int32							RegisterVertexAndFace( Int32 nFirstX, Int32 nFirstY, Int32 nSecondX, Int32 nSecondY, Int32 nThirdX, Int32 nThirdY );

		STileVertex *					GetTileVertex( Int32 nIndexX, Int32 nIndexY ) const;
		bool							AddTileVertex( Int32 nIndexX, Int32 nIndexY, Int32 nSizeX, Int32 nSizeY, ENavTileFaceType eFaceType, ENavTileDivideType eDivideType, ENavTileVertexType eVertexType );

	public:

		static CNavFaceVertexMesh *		CreateFaceVertexMesh( void ) { return static_cast<CNavFaceVertexMesh *>(new CNavPositionFaceVertexMesh); }
		static void						DestroyFaceVertexMesh( CNavPositionFaceVertexMesh ** ppcMesh )  { if( ppcMesh != NULL ) if( *ppcMesh != NULL ) { delete *ppcMesh; *ppcMesh = NULL; } }
};

//=============================================================================
// End of Class : CNavPositionFaceVertexMesh
//=============================================================================

END_NAMESPACE( navex )

#endif //___C_NAV_POSITION_FACE_VERTEX_MESH_H___

//*****************************************************************************
//
// End of File : CNavPositionFaceVertexMesh.h
//
//*****************************************************************************