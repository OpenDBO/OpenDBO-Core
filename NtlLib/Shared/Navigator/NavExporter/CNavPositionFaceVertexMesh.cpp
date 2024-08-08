//*****************************************************************************
// File       : CNavPositionFaceVertexMesh.cpp
// Desc       : 
// Author     : June C. Kim (cynis@hotmail.com)
// Update     : 2007. 1. 18
//*****************************************************************************

//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "StdAfx.h"

#include "CNavTile.h"

#include "CNavPositionFaceVertexMesh.h"


//-----------------------------------------------------------------------------
// Name Space Table
//-----------------------------------------------------------------------------

USE_NAMESPACE( navex )


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

void CNavPositionFaceVertexMesh::Initialize( void )
{
	m_pcTiles		= NULL;

	m_nTileCount	= 0;
	m_nTileCountX	= 0;
	m_nTileCountY	= 0;

	m_psVertexInfo	= NULL;

	return;
}


//-----------------------------------------------------------------------------
// Name			: Destroy
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CNavPositionFaceVertexMesh::Destroy( void )
{
	DestroyTiles();

	Initialize();

	return;
}


//-----------------------------------------------------------------------------
// Name			: CreateTiles
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CNavPositionFaceVertexMesh::CreateTiles( Int32 nTileCountX, Int32 nTileCountY )
{
	m_nTileCountX = nTileCountX;
	m_nTileCountY = nTileCountY;
	m_nTileCount = (nTileCountX + 2) * ( nTileCountY + 2);
	m_pcTiles = new CNavTile[m_nTileCount];		// +2 for Outline Dummy Tiles

	m_psVertexInfo = new STileVertex[(nTileCountX + 1) * (nTileCountY + 1)];

	return;
}


//-----------------------------------------------------------------------------
// Name			: DestroyTiles
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CNavPositionFaceVertexMesh::DestroyTiles( void )
{
	if( m_pcTiles != NULL )
	{
		delete[] m_pcTiles;
		m_pcTiles = NULL;
	}

	m_nTileCountX = 0;
	m_nTileCountY = 0;
	m_nTileCount = 0;

	if( m_psVertexInfo != NULL )
	{
		delete[] m_psVertexInfo;

		m_psVertexInfo = NULL;
	}

	return;
}


//-----------------------------------------------------------------------------
// Name			: GetTileByPosition
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CNavTile * const CNavPositionFaceVertexMesh::GetTileByPosition( const CNavPosition & rcPosition ) const
{
	Assert( m_cBound.IsContain( rcPosition ) == true );

	CNavPosition	cOffset( rcPosition );
	cOffset.Sub( m_cBound.GetMin() );

	SNavIndex2D		sIndex( cOffset.GetX() / m_cPrecision.GetX(),
							cOffset.GetY() / m_cPrecision.GetY() );

	return GetTile( sIndex.GetX(), sIndex.GetY() );
}


//-----------------------------------------------------------------------------
// Name			: GetTileByPosition
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CNavTile * const CNavPositionFaceVertexMesh::GetTileByPosition( Fixed32 nPositionX, Fixed32 nPositionY ) const
{
	CNavPosition	cPosition( nPositionX, nPositionY, 0 );

	return GetTileByPosition( cPosition );
}


//-----------------------------------------------------------------------------
// Name			: GetTileByPosition
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CNavPositionFaceVertexMesh::GetTilePosition( CNavPosition & rcPosition, Int32 nIndexX, Int32 nIndexY, ENavTileVertexType eType ) const
{
	switch( eType )
	{
		case E_NAV_TILE_VERTEX_TYPE_LEFT_TOP:
		{
			CNavTile *	pcTile = GetTile( nIndexX, nIndexY );
			if( pcTile == NULL )
			{
				return false;
			}

			rcPosition.Set( static_cast<Fixed32>(nIndexX) * m_cPrecision.GetX() + m_cBound.GetMin().GetX(),
							static_cast<Fixed32>(nIndexY) * m_cPrecision.GetY() + m_cBound.GetMin().GetY(),
							pcTile->GetHeight() );

			break;
		}

		case E_NAV_TILE_VERTEX_TYPE_RIGHT_TOP:
		{
			CNavTile *	pcTile = GetTile( nIndexX + 1, nIndexY );
			if( pcTile == NULL )
			{
				return false;
			}

			rcPosition.Set( static_cast<Fixed32>(nIndexX + 1) * m_cPrecision.GetX() + m_cBound.GetMin().GetX(),
							static_cast<Fixed32>(nIndexY) * m_cPrecision.GetY() + m_cBound.GetMin().GetY(),
							pcTile->GetHeight() );

			break;
		}

		case E_NAV_TILE_VERTEX_TYPE_LEFT_BOTTOM:
		{
			CNavTile *	pcTile = GetTile( nIndexX, nIndexY + 1 );
			if( pcTile == NULL )
			{
				return false;
			}

			rcPosition.Set( static_cast<Fixed32>(nIndexX) * m_cPrecision.GetX() + m_cBound.GetMin().GetX(),
							static_cast<Fixed32>(nIndexY + 1) * m_cPrecision.GetY() + m_cBound.GetMin().GetY(),
							pcTile->GetHeight() );

			break;
		}

		case E_NAV_TILE_VERTEX_TYPE_RIGHT_BOTTOM:
		{
			CNavTile *	pcTile = GetTile( nIndexX + 1, nIndexY + 1 );
			if( pcTile == NULL )
			{
				return false;
			}

			rcPosition.Set( static_cast<Fixed32>(nIndexX + 1) * m_cPrecision.GetX() + m_cBound.GetMin().GetX(),
							static_cast<Fixed32>(nIndexY + 1) * m_cPrecision.GetY() + m_cBound.GetMin().GetY(),
							pcTile->GetHeight() );

			break;
		}

		default:
		{
			return false;
		}
	}

	return true;
}


//-----------------------------------------------------------------------------
// Name			: AddFace
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

Int32 CNavPositionFaceVertexMesh::AddFace( void * pvVertex0, void * pvVertex1, void * pvVertex2, void * pvFace )
{
	Assert( pvVertex0 != NULL );
	Assert( pvVertex1 != NULL );
	Assert( pvVertex2 != NULL );

	CNavPosition *		pcPosition;
	SNavIndexedTriangle		sVertexIndex;

	// Register Vertex Position

	pcPosition = reinterpret_cast<CNavPosition *>(pvVertex0);
	sVertexIndex.SetIndex( 0, RegisterVertex( *pcPosition ) );

	pcPosition = reinterpret_cast<CNavPosition *>(pvVertex1);
	sVertexIndex.SetIndex( 1, RegisterVertex( *pcPosition ) );

	pcPosition = reinterpret_cast<CNavPosition *>(pvVertex2);
	sVertexIndex.SetIndex( 2, RegisterVertex( *pcPosition ) );

	// Register Face Vertex Index & Attribute

	Int32	nFaceIndex = RegisterFace( sVertexIndex, static_cast<SNavFaceAttribute *>(pvFace) );

	return nFaceIndex;
}


//-----------------------------------------------------------------------------
// Name			: AddRect
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

Int32 CNavPositionFaceVertexMesh::AddRect( void * pvVertexLB, void * pvVertexRB, void * pvVertexLT, void * pvVertexRT, void * pvFace )
{
	// Add Left Top Face

	Int32	nFaceIndex = AddFace( pvVertexLT, pvVertexRT, pvVertexLB, pvFace );

	nFaceIndex = AddFace( pvVertexRB, pvVertexLB, pvVertexRT, pvFace );

	return nFaceIndex;
}


//-----------------------------------------------------------------------------
// Name			: RegisterVertexAndFace
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

Int32 CNavPositionFaceVertexMesh::RegisterVertexAndFace( Int32 nFirstX, Int32 nFirstY, Int32 nSecondX, Int32 nSecondY, Int32 nThirdX, Int32 nThirdY )
{
	CNavPosition	cPosition[3];
	SNavIndexedTriangle		sTriangle;

	Assert( nFirstX != nSecondX || nSecondX != nThirdX );
	Assert( nFirstY != nSecondY || nSecondY != nThirdY );

	if( GetTilePosition( cPosition[0], nFirstX, nFirstY ) == false )
	{
		Assert( !"Invalid Position" );

		return -1;
	}

	if( GetTilePosition( cPosition[1], nSecondX, nSecondY ) == false )
	{
		Assert( !"Invalid Position" );

		return -1;
	}

	if( GetTilePosition( cPosition[2], nThirdX, nThirdY ) == false )
	{
		Assert( !"Invalid Position" );

		return -1;
	}

	return AddFace( (void *)&(cPosition[0]), (void *)&(cPosition[1]), (void *)&(cPosition[2]) );
}


//-----------------------------------------------------------------------------
// Name			: AddHeightMap
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

Int32 CNavPositionFaceVertexMesh::AddHeightMap( void * pvHeightMap )
{
	CNavHeightMap *		pcHeightMap = reinterpret_cast<CNavHeightMap *>(pvHeightMap);
	Assert( pcHeightMap != NULL );

	CNavSector *	pcSector = down_cast<CNavSector *>(pcHeightMap->GetParent());
	Assert( pcSector != NULL );

	CNavZone *	pcZone = reinterpret_cast<CNavZone *>(pcSector->GetParent());
	Assert( pcZone != NULL );

	const CNavBounding & 	cBounding = pcZone->GetBounding();
	const CNavPosition & 	cPrecision = pcHeightMap->GetPrecision();

	if( m_pcTiles == NULL )
	{
		if( Reserve( cBounding, cPrecision ) == false )
		{
			return 0;
		}
	}

	Fixed32		nPositionX, nPositionY;
	CNavPosition	cPosition;
	bool	bDownRight = false;
	SNavIndex2D	sTileCount = pcHeightMap->GetHeightCount();
	sTileCount.IncX();
	sTileCount.IncY();

	UInt32	nRegisteredCount = 0;

	SNavTileCreateInfo	sTileCreateInfo;
	sTileCreateInfo.bFiltered		= false;
	sTileCreateInfo.nWidth			= cPrecision.GetX();
	sTileCreateInfo.nHeight			= cPrecision.GetY();
	sTileCreateInfo.bLocked			= false;
	sTileCreateInfo.fSlopeThreshold	= 1.0f;

	for( UInt32 nIndexY = 0; nIndexY < sTileCount.GetY() - 2; ++nIndexY )
	{
		bDownRight = !bDownRight;
		nPositionY = pcHeightMap->GetBounding().GetMin().GetY() + pcHeightMap->GetPrecision().GetY() * nIndexY;

		for( UInt32 nIndexX = 0; nIndexX < sTileCount.GetX() - 2; ++nIndexX )
		{
			nPositionX = pcHeightMap->GetBounding().GetMin().GetX() + pcHeightMap->GetPrecision().GetX() * nIndexX;

			// Attribute

			cPosition.Set( nPositionX,
							nPositionY,
							0 );
			sTileCreateInfo.nAttribute = pcHeightMap->GetAttribute( cPosition );

		//	TCHAR	szBuf[1024];
		//	_stprintf_s( szBuf, 1024, _T( "%d" ), sTileCreateInfo.nAttribute );
		//	g_Log( szBuf );

			// Left Top

			cPosition.Set( nPositionX,
							nPositionY,
							0 );
			if( pcHeightMap->GetHeight( cPosition ) == false ||
				cPosition.GetW() < 0 )
			{
				Assert( !"Height Error" );

				continue;
			}

			sTileCreateInfo.nLeftTop = cPosition.GetZ();

			// Right Top

			cPosition.Set( nPositionX + pcHeightMap->GetPrecision().GetX(),
							nPositionY,
							0 );
			if( pcHeightMap->GetHeight( cPosition ) == false ||
				cPosition.GetW() < 0 )
			{
				Assert( !"Height Error" );

				continue;
			}

			sTileCreateInfo.nRightTop = cPosition.GetZ();

			// Left Bottom

			cPosition.Set( nPositionX,
							nPositionY + pcHeightMap->GetPrecision().GetY(),
							0 );
			if( pcHeightMap->GetHeight( cPosition ) == false ||
				cPosition.GetW() < 0 )
			{
				Assert( !"Height Error" );

				continue;
			}

			sTileCreateInfo.nLeftBottom = cPosition.GetZ();

			// Right Bottom

			cPosition.Set( nPositionX + pcHeightMap->GetPrecision().GetX(),
							nPositionY + pcHeightMap->GetPrecision().GetY(),
							0 );
			if( pcHeightMap->GetHeight( cPosition ) == false ||
				cPosition.GetW() < 0 )
			{
				Assert( !"Height Error" );

				continue;
			}

			sTileCreateInfo.nRightBottom = cPosition.GetZ();

			// Divide Type

			if( bDownRight == true )
			{
				sTileCreateInfo.eDivideType = E_NAV_TILE_DIVIDE_TYPE_DOWN_RIGHT;

				bDownRight = false;
			}
			else
			{
				sTileCreateInfo.eDivideType = E_NAV_TILE_DIVIDE_TYPE_DOWN_LEFT;

				bDownRight = true;
			}

			// Create Tile

			nRegisteredCount += GetTileByPosition( nPositionX, nPositionY )->Create( sTileCreateInfo );
		}
	}

	return nRegisteredCount;
}


//-----------------------------------------------------------------------------
// Name			: BuildAndOptimize
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

Int32 CNavPositionFaceVertexMesh::BuildAndOptimize( Int32 nOptimizeLevel, UInt32 nFlags )
{
	if( nOptimizeLevel == 0 )
	{
		return BuildUnoptimizedMesh( nFlags );
	}

	g_Log( _T( "Begin Optimize" ) );

	Int32	nOptimizeInterval = 0x00000001 << nOptimizeLevel;
	Int32	nMaxInterval = Min( Max( m_nTileCountX, m_nTileCountY ), static_cast<Int32>(0x00000001 << OPTIMIZATION_LEVEL_MAX) );

	while( nOptimizeInterval <= nMaxInterval )
	{
		if( OptimizeMesh( nOptimizeInterval, nFlags ) == false )
		{
			break;
		}

		nOptimizeInterval <<= 1;
	}

	BuildOptimizedMesh( nFlags );

	g_Log( _T( "End Optimize" ) );

	return m_nFaceCount;
}


//-----------------------------------------------------------------------------
// Name			: BuildUnoptimizedMesh
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

Int32 CNavPositionFaceVertexMesh::BuildUnoptimizedMesh( UInt32 nFlags )
{
	UNREFERENCED_PARAMETER( nFlags );

	CNavPosition	cPosition;
	SNavIndexedTriangle		sIndex;
	Int32	nLeftTop, nRightTop, nLeftBottom, nRightBottom;

	for( Int32 nIndexY = 0; nIndexY < m_nTileCountY; ++nIndexY )
	{
		for( Int32 nIndexX = 0; nIndexX < m_nTileCountX; ++nIndexX )
		{
			if( GetTile( nIndexX, nIndexY )->IsFiltered( E_NAV_TILE_FACE_TYPE_LEFT ) == false )
			{
				if( GetTile( nIndexX, nIndexY )->IsFiltered( E_NAV_TILE_FACE_TYPE_RIGHT ) == false )
				{
					// Left Top

					cPosition.Set( m_cBound.GetMin().GetX() + nIndexX * m_cPrecision.GetX(),
									m_cBound.GetMin().GetY() + nIndexY * m_cPrecision.GetY(),
									0 );
					nLeftTop = RegisterVertex( cPosition );

					// Right Top

					cPosition.Set( m_cBound.GetMin().GetX() + (nIndexX + 1) * m_cPrecision.GetX(),
									m_cBound.GetMin().GetY() + nIndexY * m_cPrecision.GetY(),
									0 );
					nRightTop = RegisterVertex( cPosition );

					// Left Bottom

					cPosition.Set( m_cBound.GetMin().GetX() + nIndexX * m_cPrecision.GetX(),
									m_cBound.GetMin().GetY() + (nIndexY + 1) * m_cPrecision.GetY(),
									0 );
					nLeftBottom = RegisterVertex( cPosition );

					// Right Bottom

					cPosition.Set( m_cBound.GetMin().GetX() + (nIndexX + 1) * m_cPrecision.GetX(),
									m_cBound.GetMin().GetY() +  (nIndexY + 1) * m_cPrecision.GetY(),
									0 );
					nRightBottom = RegisterVertex( cPosition );

					if( GetTile( nIndexX, nIndexY )->GetDivideType() == E_NAV_TILE_DIVIDE_TYPE_DOWN_LEFT )
					{
						sIndex.Set( nLeftTop, nRightTop, nLeftBottom );
						RegisterFace( sIndex );

						sIndex.Set( nRightBottom, nLeftBottom, nRightTop );
						RegisterFace( sIndex );
					}
					else
					{
						sIndex.Set( nLeftBottom, nLeftTop, nRightBottom );
						RegisterFace( sIndex );

						sIndex.Set( nRightTop, nRightBottom, nLeftTop );
						RegisterFace( sIndex );
					}
				}
				else
				{
					if( GetTile( nIndexX, nIndexY )->GetDivideType() == E_NAV_TILE_DIVIDE_TYPE_DOWN_LEFT )
					{
						// Left Top

						cPosition.Set( m_cBound.GetMin().GetX() + nIndexX * m_cPrecision.GetX(),
										m_cBound.GetMin().GetY() + nIndexY * m_cPrecision.GetY(),
										0 );
						nLeftTop = RegisterVertex( cPosition );

						// Right Top

						cPosition.Set( m_cBound.GetMin().GetX() + (nIndexX + 1) * m_cPrecision.GetX(),
										m_cBound.GetMin().GetY() + nIndexY * m_cPrecision.GetY(),
										0 );
						nRightTop = RegisterVertex( cPosition );

						// Left Bottom

						cPosition.Set( m_cBound.GetMin().GetX() + nIndexX * m_cPrecision.GetX(),
										m_cBound.GetMin().GetY() + (nIndexY + 1) * m_cPrecision.GetY(),
										0 );
						nLeftBottom = RegisterVertex( cPosition );

						sIndex.Set( nLeftTop, nRightTop, nLeftBottom );
						RegisterFace( sIndex );
					}
					else
					{
						// Left Top

						cPosition.Set( m_cBound.GetMin().GetX() + nIndexX * m_cPrecision.GetX(),
										m_cBound.GetMin().GetY() + nIndexY * m_cPrecision.GetY(),
										0 );
						nLeftTop = RegisterVertex( cPosition );

						// Left Bottom

						cPosition.Set( m_cBound.GetMin().GetX() + nIndexX * m_cPrecision.GetX(),
										m_cBound.GetMin().GetY() + (nIndexY + 1) * m_cPrecision.GetY(),
										0 );
						nLeftBottom = RegisterVertex( cPosition );

						// Right Bottom

						cPosition.Set( m_cBound.GetMin().GetX() + (nIndexX + 1) * m_cPrecision.GetX(),
										m_cBound.GetMin().GetY() +  (nIndexY + 1) * m_cPrecision.GetY(),
										0 );
						nRightBottom = RegisterVertex( cPosition );

						sIndex.Set( nLeftBottom, nLeftTop, nRightBottom );
						RegisterFace( sIndex );
					}
				}
			}
			else
			{
				if( GetTile( nIndexX, nIndexY )->IsFiltered( E_NAV_TILE_FACE_TYPE_RIGHT ) == false )
				{
					if( GetTile( nIndexX, nIndexY )->GetDivideType() == E_NAV_TILE_DIVIDE_TYPE_DOWN_LEFT )
					{
						// Right Top

						cPosition.Set( m_cBound.GetMin().GetX() + (nIndexX + 1) * m_cPrecision.GetX(),
										m_cBound.GetMin().GetY() + nIndexY * m_cPrecision.GetY(),
										0 );
						nRightTop = RegisterVertex( cPosition );

						// Left Bottom

						cPosition.Set( m_cBound.GetMin().GetX() + nIndexX * m_cPrecision.GetX(),
										m_cBound.GetMin().GetY() + (nIndexY + 1) * m_cPrecision.GetY(),
										0 );
						nLeftBottom = RegisterVertex( cPosition );

						// Right Bottom

						cPosition.Set( m_cBound.GetMin().GetX() + (nIndexX + 1) * m_cPrecision.GetX(),
										m_cBound.GetMin().GetY() +  (nIndexY + 1) * m_cPrecision.GetY(),
										0 );
						nRightBottom = RegisterVertex( cPosition );

						sIndex.Set( nRightBottom, nLeftBottom, nRightTop );
						RegisterFace( sIndex );
					}
					else
					{
						// Left Top

						cPosition.Set( m_cBound.GetMin().GetX() + nIndexX * m_cPrecision.GetX(),
										m_cBound.GetMin().GetY() + nIndexY * m_cPrecision.GetY(),
										0 );
						nLeftTop = RegisterVertex( cPosition );

						// Right Top

						cPosition.Set( m_cBound.GetMin().GetX() + (nIndexX + 1) * m_cPrecision.GetX(),
										m_cBound.GetMin().GetY() + nIndexY * m_cPrecision.GetY(),
										0 );
						nRightTop = RegisterVertex( cPosition );

						// Right Bottom

						cPosition.Set( m_cBound.GetMin().GetX() + (nIndexX + 1) * m_cPrecision.GetX(),
										m_cBound.GetMin().GetY() +  (nIndexY + 1) * m_cPrecision.GetY(),
										0 );
						nRightBottom = RegisterVertex( cPosition );

						sIndex.Set( nRightTop, nRightBottom, nLeftTop );
						RegisterFace( sIndex );
					}
				}
				else
				{
					// Skip
				}
			}
		}
	}

	return m_nFaceCount;
}


//-----------------------------------------------------------------------------
// Name			: Reserve
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CNavPositionFaceVertexMesh::Reserve( const CNavBounding & rcBound, const CNavPosition & rcPrecision )
{
	m_cBound = rcBound;
	m_cPrecision = rcPrecision;

	Int32	nTileCountX = rcBound.GetSizeX() / rcPrecision.GetX();
	Int32	nTileCountY = rcBound.GetSizeY() / rcPrecision.GetY();

	UInt32	nMax = Max( nTileCountX, nTileCountY );
	UInt32	nPower = 1;
	while( (nMax = nMax / 2) != 0 )
	{
		++nPower;
	}

	nMax = 1 << nPower;

	CreateTiles( nMax, nMax );

	if( m_pcTiles == NULL )
	{
		return false;
	}

	return true;
}


//*****************************************************************************
//
// End of File : CNavPositionFaceVertexMesh.cpp
//
//*****************************************************************************