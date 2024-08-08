//*****************************************************************************
// File       : FaceVertexMeshBase.h
// Desc       : 
// Author     : June C. Kim (cynis@hotmail.com)
// Update     : 2007. 1. 18
//*****************************************************************************

#ifndef ___FACE_VERTEX_MESH_BASE_H___
#define ___FACE_VERTEX_MESH_BASE_H___

//-----------------------------------------------------------------------------
// Pre-Compiler Option Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// Name Space Table
//-----------------------------------------------------------------------------

BEGIN_NAMESPACE( nav )


//-----------------------------------------------------------------------------
// Type Forward Definition Table
//-----------------------------------------------------------------------------

class CNavFaceVertexMesh;


//-----------------------------------------------------------------------------
// Constant Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Type Definition Table
//-----------------------------------------------------------------------------

//=============================================================================
// Name       : SNavFaceAttribute
// Desc       : 
// Author     : 
//=============================================================================

typedef struct SNavFaceAttribute
{
	public:

		Int32					nSurfaceType;
		Int32					nSectionID;
		Int32					nUserData;
		bool					bPortal;
		bool					bExternalShape;
		bool					bConnectingEdge;

	public:

		inline					SNavFaceAttribute( void ) : nSurfaceType( 0 ), nSectionID( -1 ), nUserData( 0 ),
															bPortal( false ), bExternalShape( false ), bConnectingEdge( false ) {}

		inline void				Set( const SNavFaceAttribute & rsAttribute )
		{
			memcpy( this, &rsAttribute, sizeof( SNavFaceAttribute ) );
		}

		inline void				Reset( void )
		{
			nSurfaceType = 0; nSectionID = -1; nUserData = 0;
			bPortal = false; bExternalShape = false; bConnectingEdge = false;
		}
} SNavFaceAttribute;

//=============================================================================
// End of Struct : SNavFaceAttribute
//=============================================================================


//=============================================================================
// Name       : SNavFaceVertexMeshList
// Desc       : 
// Author     : 
//=============================================================================

typedef struct SNavFaceVertexMeshList
{
	public:

		UInt32					nFaceVertexMeshCount;
		CNavFaceVertexMesh **	ppcFaceVertexMeshList;
		char **					ppchFlags;

	public:

		inline					SNavFaceVertexMeshList( void ) : nFaceVertexMeshCount( 0 ), ppcFaceVertexMeshList( NULL ), ppchFlags( NULL ) {}
		inline					~SNavFaceVertexMeshList( void ) { Destroy(); }

		inline void				Create( UInt32 nNewFaceVertexMeshCount ) { Destroy(); nFaceVertexMeshCount = nNewFaceVertexMeshCount; ppcFaceVertexMeshList = new CNavFaceVertexMesh *[nFaceVertexMeshCount]; ZeroMemory( ppcFaceVertexMeshList, sizeof( CNavFaceVertexMesh * ) * nFaceVertexMeshCount ); }
		inline void				Destroy( void ) { if( ppcFaceVertexMeshList != NULL ) delete[] ppcFaceVertexMeshList; ppcFaceVertexMeshList = NULL; }
} SNavFaceVertexMeshList;

//=============================================================================
// End of Struct : SNavFaceVertexMeshList
//=============================================================================


//=============================================================================
// Name       : SNavIndexedTriangle
// Desc       : 
// Author     : 
//=============================================================================

typedef struct SNavIndexedTriangle
{
	public:

		Int32			anIndex[3];

	public:

		inline			SNavIndexedTriangle( void ) { Reset(); }

		inline void		Set( const SNavIndexedTriangle & rsIndex )
		{
			memcpy( this, &rsIndex, sizeof( SNavIndexedTriangle ) );
		}

		inline void		Set( Int32 nIndex0, Int32 nIndex1, Int32 nIndex2 )
		{
			anIndex[0] = nIndex0;
			anIndex[1] = nIndex1;
			anIndex[2] = nIndex2;
		}

		inline Int32	SetIndex( Int32 nDataIndex, Int32 nIndexData )
		{
			Assert( nDataIndex >= 0 );
			Assert( nDataIndex < 3 );

			return anIndex[nDataIndex] = nIndexData;
		}

		inline void		Reset( void )
		{
			Set( -1, -1, -1 );
		}

		inline void		Get( SNavIndexedTriangle & rsIndex ) const
		{
			rsIndex.anIndex[0] = anIndex[0];
			rsIndex.anIndex[1] = anIndex[1];
			rsIndex.anIndex[2] = anIndex[2];
		}

		inline Int32	GetIndex( Int32 nDataIndex ) const
		{
			Assert( nDataIndex >= 0 );
			Assert( nDataIndex < 3 );

			return anIndex[2-nDataIndex];
		}

		inline bool		IsEqual( const SNavIndexedTriangle & rsTriangle ) const
		{
			if( anIndex[0] == rsTriangle.anIndex[0] )
			{
				if( ( anIndex[1] == rsTriangle.anIndex[1] ) &&
					( anIndex[2] == rsTriangle.anIndex[2] ) )
				{
					return true;
				}
				else
				{
					return false;
				}
			}
			else if( anIndex[0] == rsTriangle.anIndex[1] )
			{
				if( ( anIndex[1] == rsTriangle.anIndex[2] ) &&
					( anIndex[2] == rsTriangle.anIndex[0] ) )
				{
					return true;
				}
				else
				{
					return false;
				}
			}
			else if( anIndex[0] == rsTriangle.anIndex[2] )
			{
				if( ( anIndex[1] == rsTriangle.anIndex[0] ) &&
					( anIndex[2] == rsTriangle.anIndex[1] ) )
				{
					return true;
				}
				else
				{
					return false;
				}
			}
			else
			{
				return false;
			}
		}
} SNavIndexedTriangle;

//=============================================================================
// End of Struct : SNavIndexedTriangle
//=============================================================================

END_NAMESPACE( nav )

#endif //___FACE_VERTEX_MESH_BASE_H___

//*****************************************************************************
//
// End of File : FaceVertexMeshBase.h
//
//*****************************************************************************