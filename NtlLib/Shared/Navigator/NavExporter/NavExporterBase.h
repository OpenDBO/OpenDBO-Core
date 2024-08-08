//*****************************************************************************
// File       : NavExporterBase.h
// Desc       : 
// Author     : June C. Kim (cynis@hotmail.com)
// Update     : 2007. 1. 18
//*****************************************************************************

#ifndef ___NAV_EXPORTER_BASE_H___
#define ___NAV_EXPORTER_BASE_H___

//-----------------------------------------------------------------------------
// Pre-Compiler Option Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "NavBase.h"
#include "CNavPositionList.h"
#include "CNavPolygon.h"


//-----------------------------------------------------------------------------
// Name Space Table
//-----------------------------------------------------------------------------

BEGIN_NAMESPACE( navex )


//-----------------------------------------------------------------------------
// Type Forward Definition Table
//-----------------------------------------------------------------------------

typedef enum ENavTileVertexType
{
	E_NAV_TILE_VERTEX_TYPE_NONE						= 0x00,
	E_NAV_TILE_VERTEX_TYPE_LEFT_TOP					= 0x01,
	E_NAV_TILE_VERTEX_TYPE_RIGHT_TOP				= 0x02,
	E_NAV_TILE_VERTEX_TYPE_LEFT_BOTTOM				= 0x04,
	E_NAV_TILE_VERTEX_TYPE_RIGHT_BOTTOM				= 0x08,

	E_NAV_TILE_VERTEX_TYPE_TOP						= E_NAV_TILE_VERTEX_TYPE_LEFT_TOP | E_NAV_TILE_VERTEX_TYPE_RIGHT_TOP,			// 0x03
	E_NAV_TILE_VERTEX_TYPE_LEFT						= E_NAV_TILE_VERTEX_TYPE_LEFT_TOP | E_NAV_TILE_VERTEX_TYPE_LEFT_BOTTOM,			// 0x05
	E_NAV_TILE_VERTEX_TYPE_RIGHT					= E_NAV_TILE_VERTEX_TYPE_RIGHT_TOP | E_NAV_TILE_VERTEX_TYPE_RIGHT_BOTTOM,		// 0x0A
	E_NAV_TILE_VERTEX_TYPE_BOTTOM					= E_NAV_TILE_VERTEX_TYPE_LEFT_BOTTOM | E_NAV_TILE_VERTEX_TYPE_RIGHT_BOTTOM,		// 0x0C

	E_NAV_TILE_VERTEX_TYPE_LEFT_TOP_RIGHT_BOTTOM	= E_NAV_TILE_VERTEX_TYPE_LEFT_TOP | E_NAV_TILE_VERTEX_TYPE_RIGHT_BOTTOM,	// 0x09
	E_NAV_TILE_VERTEX_TYPE_RIGHT_TOP_LEFT_BOTTOM	= E_NAV_TILE_VERTEX_TYPE_RIGHT_TOP | E_NAV_TILE_VERTEX_TYPE_LEFT_BOTTOM,	// 0x06

	E_NAV_TILE_VERTEX_TYPE_EXCEPT_LEFT_TOP			= E_NAV_TILE_VERTEX_TYPE_RIGHT_TOP | E_NAV_TILE_VERTEX_TYPE_BOTTOM,				// 0x0E
	E_NAV_TILE_VERTEX_TYPE_EXCEPT_RIGHT_TOP			= E_NAV_TILE_VERTEX_TYPE_LEFT_TOP | E_NAV_TILE_VERTEX_TYPE_BOTTOM,				// 0x0D
	E_NAV_TILE_VERTEX_TYPE_EXCEPT_LEFT_BOTTOM		= E_NAV_TILE_VERTEX_TYPE_RIGHT_BOTTOM | E_NAV_TILE_VERTEX_TYPE_TOP,				// 0x0B
	E_NAV_TILE_VERTEX_TYPE_EXCEPT_RIGHT_BOTTOM		= E_NAV_TILE_VERTEX_TYPE_LEFT_BOTTOM | E_NAV_TILE_VERTEX_TYPE_TOP,				// 0x07

	E_NAV_TILE_VERTEX_TYPE_ALL						= E_NAV_TILE_VERTEX_TYPE_LEFT | E_NAV_TILE_VERTEX_TYPE_RIGHT,					// 0x0F
	E_NAV_TILE_VERTEX_TYPE_COUNT
} ENavVertexType;


typedef enum ENavTileDivideType
{
	E_NAV_TILE_DIVIDE_TYPE_DOWN_LEFT	= 0x00,
	E_NAV_TILE_DIVIDE_TYPE_DOWN_RIGHT	= 0x01,
	E_NAV_TILE_DIVIDE_TYPE_COUNT
} ENavTileDivideType;


typedef enum ENavTileFaceType
{
	E_NAV_TILE_FACE_TYPE_LEFT			= 0x00,
	E_NAV_TILE_FACE_TYPE_RIGHT			= 0x01,
	E_NAV_TILE_FACE_TYPE_COUNT
} ENavTileFaceType;


typedef struct SNavTileCreateInfo
{
	Fixed32						nLeftTop;
	Fixed32						nRightTop;
	Fixed32						nLeftBottom;
	Fixed32						nRightBottom;

	Fixed32						nWidth;
	Fixed32						nHeight;

	Float32						fSlopeThreshold;

	UInt32						nAttribute;

	ENavTileDivideType			eDivideType;
	bool						bLocked;
	bool						bFiltered;
} SNavTileCreateInfo;


typedef struct SWeObstacleInfo
{
	char				szName[128];
	float				scale[3];
	float				direction[3];
	float				position[3];
} SWeObstacleInfo;


typedef struct SWePolygonNode
{
	CNavPolygon			cPolygon;
	SWePolygonNode *	psNext;

	SWePolygonNode( void ) : psNext( NULL ) {}
} SWePolygonNode;


typedef struct SWeModelInfo
{
	CFixedString		cName;
	SWePolygonNode *	psPolygon;
	UInt32				nFlags;

	SWeModelInfo( void ) : psPolygon( NULL ), nFlags( 0 ) {}
	SWeModelInfo( const SWeModelInfo & rsInfo );
} SWeModelInfo;


typedef struct SWeModelInfoNode
{
	SWeModelInfo		sInfo;
	SWeModelInfoNode *	psNext;

	SWeModelInfoNode( void ) : psNext( NULL ) {}
	SWeModelInfoNode( const SWeModelInfo & rsInfo ) : sInfo( rsInfo ), psNext( NULL ) {}
} SWeModelingInfoNode;


typedef struct SFaceVertex
{
	ENavTileVertexType		eVertexType;
	ENavTileDivideType		eDivideType;
	ENavTileFaceType		eFaceType;
	Int32					nSizeX;
	Int32					nSizeY;
} SFaceVertex;


typedef list<SFaceVertex *>	 	TFaceVertexList;
typedef TFaceVertexList::iterator	TFaceVertexListIterator;
typedef TFaceVertexList::const_iterator	TFaceVertexListConstIterator;


typedef struct STileVertex
{
	inline								STileVertex( void ) : nCount( 0 ), ptList( NULL ) {}
	inline								~STileVertex( void )
										{
											if( ptList != NULL )
											{
												SFaceVertex *	psVertex;
												TFaceVertexListIterator	tCurrentPosition = ptList->begin();

												while( tCurrentPosition != ptList->end() )
												{
													psVertex = *tCurrentPosition;
													++tCurrentPosition;

													delete psVertex;
												}

												ptList->clear();
												delete ptList;
											}
										}

	void								AddVertex( SFaceVertex * const pcVertex )
										{
											if( ptList == NULL )
											{
												ptList = new TFaceVertexList;
												ptList->clear();
											}

											ptList->push_back( pcVertex );

											++nCount;
										}

	SFaceVertex * const					GetVertex( UInt32 nOrder ) const
										{
											TFaceVertexListIterator	tCurrentPosition = ptList->begin();
											while( nOrder-- != 0 )
											{
												++tCurrentPosition;
											}

											return *tCurrentPosition;
										}

	UInt32								GetCount( void ) const { return nCount; }
	UInt32								nCount;
	TFaceVertexList * 					ptList;
} STileVertex;


//-----------------------------------------------------------------------------
// Constant Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Type Definition Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Macro, Inline & Template Function Table
//-----------------------------------------------------------------------------

inline bool RemoveFileExt( CFixedStringA & rcResult, const CFixedStringA & rcOriginal )
{
	for( size_t nPosition = rcOriginal.GetLength(); nPosition > 0; ++nPosition )
	{
		if( rcOriginal.GetAt( nPosition - 1 ) == '.' )
		{
			rcResult.CreateSubStringFrom( rcOriginal, 0, nPosition - 1 );

			return true;
		}
	}

	return false;
}


inline bool RemoveFileExt( CFixedStringW & rcResult, const CFixedStringW & rcOriginal )
{
	for( size_t nPosition = rcOriginal.GetLength(); nPosition > 0; ++nPosition )
	{
		if( rcOriginal.GetAt( nPosition - 1 ) == L'.' )
		{
			rcResult.CreateSubStringFrom( rcOriginal, 0, nPosition - 1 );

			return true;
		}
	}

	return false;
}


inline bool RemoveFileExt( CFixedStringW & rcResult, const CFixedStringA & rcOriginal )
{
	for( size_t nPosition = rcOriginal.GetLength(); nPosition > 0; ++nPosition )
	{
		if( rcOriginal.GetAt( nPosition - 1 ) == '.' )
		{
			CFixedStringA	cResult;
			cResult.CreateSubStringFrom( rcOriginal, 0, nPosition - 1 );

			rcResult.Assign( cResult );

			return true;
		}
	}

	return false;
}


inline bool RemoveFileExt( CFixedStringA & rcResult, const CFixedStringW & rcOriginal )
{
	for( size_t nPosition = rcOriginal.GetLength(); nPosition > 0; ++nPosition )
	{
		if( rcOriginal.GetAt( nPosition - 1 ) == L'.' )
		{
			CFixedStringW	cResult;

			cResult.CreateSubStringFrom( rcOriginal, 0, nPosition - 1 );

			rcResult.Assign( cResult );

			return true;
		}
	}

	return false;
}


END_NAMESPACE( navex )

#endif //___NAV_EXPORTER_BASE_H___

//*****************************************************************************
//
// End of File : NavExporterBase.h
//
//*****************************************************************************