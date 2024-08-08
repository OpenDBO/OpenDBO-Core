//*****************************************************************************
// File       : CNavPositionFaceVertexMesh_Optimize.cpp
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
// Name			: OptimizeMesh
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CNavPositionFaceVertexMesh::OptimizeMesh( Int32 nOptimizeInterval, UInt32 nFlags )
{
	UNREFERENCED_PARAMETER( nFlags );

	UInt8	nOptimizeLevel = 0;
	Int32	nTemp = nOptimizeInterval;

	while( nTemp > 1 )
	{
		++nOptimizeLevel;
		nTemp >>= 1;
	}

	Int32	nSearchInterval = nOptimizeInterval >> 1;
	bool	bLocked;
	UInt32	nExportCount = 0;

	for( Int32 nOptimizeIndexY = 0; nOptimizeIndexY < m_nTileCountY; nOptimizeIndexY += nOptimizeInterval )
	{
		for( Int32 nOptimizeIndexX = 0; nOptimizeIndexX < m_nTileCountX; nOptimizeIndexX += nOptimizeInterval )
		{
			bLocked = false;

			for( Int32 nSearchIndexY = nOptimizeIndexY; nSearchIndexY < nOptimizeIndexY + nOptimizeInterval; nSearchIndexY += nSearchInterval )
			{
				for( Int32 nSearchIndexX = nOptimizeIndexX; nSearchIndexX < nOptimizeIndexX + nOptimizeInterval; nSearchIndexX += nSearchInterval )
				{
					if( GetTile( nSearchIndexX, nSearchIndexY )->IsLocked( E_NAV_TILE_FACE_TYPE_LEFT ) == true )
					{
						bLocked = true;

						break;
					}
					else if( GetTile( nSearchIndexX, nSearchIndexY )->IsLocked( E_NAV_TILE_FACE_TYPE_RIGHT ) == true )
					{
						bLocked = true;

						break;
					}
				}

				if( bLocked == true )
				{
					break;
				}
			}

			if( bLocked == true )
			{
				for( Int32 nSearchIndexY = nOptimizeIndexY; nSearchIndexY < nOptimizeIndexY + nOptimizeInterval; nSearchIndexY += nSearchInterval )
				{
					for( Int32 nSearchIndexX = nOptimizeIndexX; nSearchIndexX < nOptimizeIndexX + nOptimizeInterval; nSearchIndexX += nSearchInterval )
					{
						ExportTile( nSearchIndexX, nSearchIndexY, nSearchInterval );

						++nExportCount;
					}
				}
			}
		}
	}

	if( nExportCount == 0 )
	{
		ExportTile( 0, 0, m_nTileCountX - 1 );

		return false;
	}

	return true;
}


//-----------------------------------------------------------------------------
// Name			: ExportTile
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CNavPositionFaceVertexMesh::ExportTile( Int32 nX, Int32 nY, Int32 nR )
{
	if( GetTile( nX, nY )->IsExported() == true )
	{
		return false;
	}

	bool	bExported = false;

	if( GetTile( nX, nY )->IsFiltered( E_NAV_TILE_FACE_TYPE_LEFT ) == false )
	{
		if( GetTile( nX, nY )->GetDivideType() == E_NAV_TILE_DIVIDE_TYPE_DOWN_LEFT )
		{
			AddTileVertex( nX, nY, nR, nR,				E_NAV_TILE_FACE_TYPE_LEFT, E_NAV_TILE_DIVIDE_TYPE_DOWN_LEFT, E_NAV_TILE_VERTEX_TYPE_LEFT_TOP );
			AddTileVertex( nX + nR,	nY, -nR, nR,		E_NAV_TILE_FACE_TYPE_LEFT, E_NAV_TILE_DIVIDE_TYPE_DOWN_LEFT, E_NAV_TILE_VERTEX_TYPE_RIGHT_TOP );
			AddTileVertex( nX, nY + nR, nR, -nR,		E_NAV_TILE_FACE_TYPE_LEFT, E_NAV_TILE_DIVIDE_TYPE_DOWN_LEFT, E_NAV_TILE_VERTEX_TYPE_LEFT_BOTTOM );
		}
		else
		{
			AddTileVertex( nX, nY + nR, nR, -nR,		E_NAV_TILE_FACE_TYPE_LEFT, E_NAV_TILE_DIVIDE_TYPE_DOWN_RIGHT, E_NAV_TILE_VERTEX_TYPE_LEFT_BOTTOM );
			AddTileVertex( nX, nY, nR, nR,				E_NAV_TILE_FACE_TYPE_LEFT, E_NAV_TILE_DIVIDE_TYPE_DOWN_RIGHT, E_NAV_TILE_VERTEX_TYPE_LEFT_TOP );
			AddTileVertex( nX + nR, nY + nR, -nR, -nR,	E_NAV_TILE_FACE_TYPE_LEFT, E_NAV_TILE_DIVIDE_TYPE_DOWN_RIGHT, E_NAV_TILE_VERTEX_TYPE_RIGHT_BOTTOM );
		}

		bExported = true;
	}

	if( GetTile( nX, nY )->IsFiltered( E_NAV_TILE_FACE_TYPE_RIGHT ) == false )
	{
		if( GetTile( nX, nY )->GetDivideType() == E_NAV_TILE_DIVIDE_TYPE_DOWN_LEFT )
		{
			AddTileVertex( nX + nR, nY + nR, -nR, -nR,	E_NAV_TILE_FACE_TYPE_RIGHT, E_NAV_TILE_DIVIDE_TYPE_DOWN_LEFT, E_NAV_TILE_VERTEX_TYPE_RIGHT_BOTTOM );
			AddTileVertex( nX, nY + nR, nR, -nR,		E_NAV_TILE_FACE_TYPE_RIGHT, E_NAV_TILE_DIVIDE_TYPE_DOWN_LEFT, E_NAV_TILE_VERTEX_TYPE_LEFT_BOTTOM );
			AddTileVertex( nX + nR, nY, -nR, nR,		E_NAV_TILE_FACE_TYPE_RIGHT, E_NAV_TILE_DIVIDE_TYPE_DOWN_LEFT, E_NAV_TILE_VERTEX_TYPE_RIGHT_TOP );
		}
		else
		{
			AddTileVertex( nX + nR, nY, -nR, nR,		E_NAV_TILE_FACE_TYPE_RIGHT, E_NAV_TILE_DIVIDE_TYPE_DOWN_RIGHT, E_NAV_TILE_VERTEX_TYPE_RIGHT_TOP );
			AddTileVertex( nX + nR, nY + nR, -nR, -nR,	E_NAV_TILE_FACE_TYPE_RIGHT, E_NAV_TILE_DIVIDE_TYPE_DOWN_RIGHT, E_NAV_TILE_VERTEX_TYPE_RIGHT_BOTTOM );
			AddTileVertex( nX, nY, nR, nR,				E_NAV_TILE_FACE_TYPE_RIGHT, E_NAV_TILE_DIVIDE_TYPE_DOWN_RIGHT, E_NAV_TILE_VERTEX_TYPE_LEFT_TOP );
		}

		bExported = true;
	}

	for( Int32 nIndexY = nY; nIndexY < nY + nR; ++nIndexY )
	{
		for( Int32 nIndexX = nX; nIndexX < nX + nR; ++nIndexX )
		{
			GetTile( nIndexX, nIndexY )->SetExported( bExported );
			GetTile( nIndexX, nIndexY )->SetDivideType( GetTile( nX, nY )->GetDivideType() );

			if( bExported == true )
			{
				GetTile( nIndexX, nIndexY )->SetFilter( true );
				GetTile( nIndexX, nIndexY )->SetLock( true );
			}
		}
	}

	return bExported;
}


//-----------------------------------------------------------------------------
// Name			: AddTileVertex
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CNavPositionFaceVertexMesh::AddTileVertex( Int32 nIndexX, Int32 nIndexY, Int32 nSizeX, Int32 nSizeY, ENavTileFaceType eFaceType, ENavTileDivideType eDivideType, ENavTileVertexType eVertexType )
{
	SFaceVertex *	psFaceVertex = new SFaceVertex;
	Assert( psFaceVertex != NULL );

	psFaceVertex->eVertexType	= eVertexType;
	psFaceVertex->eDivideType	= eDivideType;
	psFaceVertex->eFaceType		= eFaceType;
	psFaceVertex->nSizeX		= nSizeX;
	psFaceVertex->nSizeY		= nSizeY;

	Assert( GetTileVertex( nIndexX, nIndexY ) != NULL );

	GetTileVertex( nIndexX, nIndexY )->AddVertex( psFaceVertex );

	return true;
}


//-----------------------------------------------------------------------------
// Name			: GetTileVertex
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

STileVertex * CNavPositionFaceVertexMesh::GetTileVertex( Int32 nIndexX, Int32 nIndexY ) const
{
	Assert( m_psVertexInfo != NULL );

	return &(m_psVertexInfo[nIndexY * m_nTileCountX + nIndexX]);
}


//-----------------------------------------------------------------------------
// Name			: BuildOptimizedMesh
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

Int32 CNavPositionFaceVertexMesh::BuildOptimizedMesh( UInt32 nFlags )
{
	UNREFERENCED_PARAMETER( nFlags );

	Int32	nLocalX;
	Int32	nLocalY;
	Int32	nTempX;
	Int32	nTempY;
	bool	bHorizental = true;
	bool	bFoundApex = false;
	UInt32	nExportCount = 0;
	STileVertex	*	psTileVertex;
	SFaceVertex *	psFaceVertex;
	STileVertex	*	psLocalTileVertex;
	SFaceVertex *	psLocalFaceVertex;

	for( Int32 nIndexY = 0; nIndexY < m_nTileCountY; ++nIndexY )
	{
		for( Int32 nIndexX = 0; nIndexX < m_nTileCountX; ++nIndexX )
		{
			psTileVertex = GetTileVertex( nIndexX, nIndexY );

			for( UInt32 nIndex = 0; nIndex < psTileVertex->GetCount(); ++nIndex )
			{
				psFaceVertex = psTileVertex->GetVertex( nIndex );

				nLocalX	= nIndexX + psFaceVertex->nSizeX;
				nLocalY	= nIndexY + psFaceVertex->nSizeY;

				if( psFaceVertex->eVertexType == E_NAV_TILE_VERTEX_TYPE_LEFT_TOP &&
					psFaceVertex->eFaceType == E_NAV_TILE_FACE_TYPE_LEFT &&
					psFaceVertex->eDivideType == E_NAV_TILE_DIVIDE_TYPE_DOWN_LEFT )
				{
					RegisterVertexAndFace( nIndexX, nIndexY,
											nLocalX, nIndexY,
											nIndexX, nLocalY );
					++nExportCount;
				}

				if( psFaceVertex->eVertexType == E_NAV_TILE_VERTEX_TYPE_RIGHT_TOP &&
					psFaceVertex->eFaceType == E_NAV_TILE_FACE_TYPE_RIGHT &&
					psFaceVertex->eDivideType == E_NAV_TILE_DIVIDE_TYPE_DOWN_RIGHT )
				{
					nTempX = nLocalX;
					nTempY = nLocalY;
					bHorizental = true;
					bFoundApex = false;

					while( nTempX < nIndexX && nTempY > nIndexY )
					{
						bFoundApex = false;
						if( bHorizental == true )
						{
							nTempX = nLocalX + 1;

							while( nTempX < nIndexX )
							{								
								psLocalTileVertex = GetTileVertex( nTempX, nIndexY );
								for( UInt32 nOrder = 0; nOrder < psLocalTileVertex->GetCount(); ++nOrder )
								{
									psLocalFaceVertex = psLocalTileVertex->GetVertex( nOrder );
									if( psLocalFaceVertex->eVertexType == E_NAV_TILE_VERTEX_TYPE_LEFT_BOTTOM ||
										psLocalFaceVertex->eVertexType == E_NAV_TILE_VERTEX_TYPE_RIGHT_BOTTOM )
									{
										RegisterVertexAndFace( nTempX, nIndexY,
																nIndexX, nLocalY,
																nLocalX, nIndexY );
										++nExportCount;

										bFoundApex = true;
										nLocalX = nTempX;

										break;
									}
								}

								if( bFoundApex == true )
								{
									break;
								}

								++nTempX;
							}

							bHorizental = false;
						}
						else
						{
							nTempY = nLocalY - 1;

							while( nTempY > nIndexY )
							{
								psLocalTileVertex = GetTileVertex( nIndexX, nTempY );
								for( UInt32 nOrder = 0; nOrder < psLocalTileVertex->GetCount(); ++nOrder )
								{
									psLocalFaceVertex = psLocalTileVertex->GetVertex( nOrder );
									if( psLocalFaceVertex->eVertexType == E_NAV_TILE_VERTEX_TYPE_LEFT_TOP ||
										psLocalFaceVertex->eVertexType == E_NAV_TILE_VERTEX_TYPE_LEFT_BOTTOM )
									{
										RegisterVertexAndFace( nIndexX, nTempY,
																nIndexX, nLocalY,
																nLocalX, nIndexY );
										++nExportCount;

										bFoundApex = true;
										nLocalY = nTempY;

										break;
									}
								}

								if( bFoundApex == true )
								{
									break;
								}

								--nTempY;
							}

							bHorizental = true;
						}
					}

					if( bHorizental == true )
					{
						nTempX = nLocalX + 1;

						while( nTempX < nIndexX )
						{
							psLocalTileVertex = GetTileVertex( nTempX, nIndexY );
							for( UInt32 nOrder = 0; nOrder < psLocalTileVertex->GetCount(); ++nOrder )
							{
								psLocalFaceVertex = psLocalTileVertex->GetVertex( nOrder );
								if( psLocalFaceVertex->eVertexType == E_NAV_TILE_VERTEX_TYPE_LEFT_BOTTOM ||
									psLocalFaceVertex->eVertexType == E_NAV_TILE_VERTEX_TYPE_RIGHT_BOTTOM )
								{
									RegisterVertexAndFace( nTempX, nIndexY,
															nIndexX, nLocalY,
															nLocalX, nIndexY );
									++nExportCount;

									nLocalX = nTempX;

									break;
								}
							}

							++nTempX;
						}
					}
					else
					{
						nTempY = nLocalY - 1;

						while( nTempY > nIndexY )
						{
							psLocalTileVertex = GetTileVertex( nIndexX, nTempY );
							for( UInt32 nOrder = 0; nOrder < psLocalTileVertex->GetCount(); ++nOrder )
							{
								psLocalFaceVertex = psLocalTileVertex->GetVertex( nOrder );
								if( psLocalFaceVertex->eVertexType == E_NAV_TILE_VERTEX_TYPE_LEFT_TOP ||
									psLocalFaceVertex->eVertexType == E_NAV_TILE_VERTEX_TYPE_LEFT_BOTTOM )
								{
									RegisterVertexAndFace( nIndexX, nTempY,
															nIndexX, nLocalY,
															nLocalX, nIndexY );
									++nExportCount;

									nLocalY = nTempY;

									break;
								}
							}

							--nTempY;
						}
					}

					RegisterVertexAndFace( nIndexX, nIndexY,
											nIndexX, nLocalY,
											nLocalX, nIndexY );
					++nExportCount;
				}

				if( psFaceVertex->eVertexType == E_NAV_TILE_VERTEX_TYPE_LEFT_BOTTOM &&
					psFaceVertex->eFaceType == E_NAV_TILE_FACE_TYPE_LEFT &&
					psFaceVertex->eDivideType == E_NAV_TILE_DIVIDE_TYPE_DOWN_RIGHT )
				{
					nTempX = nLocalX;
					nTempY = nLocalY;
					bHorizental = true;
					bFoundApex = false;

					while( nTempX > nIndexX && nTempY < nIndexY )
					{
						bFoundApex = false;
						if( bHorizental == true )
						{
							nTempX = nLocalX - 1;

							while( nTempX > nIndexX )
							{								
								psLocalTileVertex = GetTileVertex( nTempX, nIndexY );
								for( UInt32 nOrder = 0; nOrder < psLocalTileVertex->GetCount(); ++nOrder )
								{
									psLocalFaceVertex = psLocalTileVertex->GetVertex( nOrder );
									if( psLocalFaceVertex->eVertexType == E_NAV_TILE_VERTEX_TYPE_LEFT_TOP ||
										psLocalFaceVertex->eVertexType == E_NAV_TILE_VERTEX_TYPE_RIGHT_TOP )
									{
										RegisterVertexAndFace( nTempX, nIndexY,
																nIndexX, nLocalY,
																nLocalX, nIndexY );
										++nExportCount;

										bFoundApex = true;
										nLocalX = nTempX;

										break;
									}
								}

								if( bFoundApex == true )
								{
									break;
								}

								--nTempX;
							}

							bHorizental = false;
						}
						else
						{
							nTempY = nLocalY + 1;

							while( nTempY < nIndexY )
							{
								psLocalTileVertex = GetTileVertex( nIndexX, nTempY );
								for( UInt32 nOrder = 0; nOrder < psLocalTileVertex->GetCount(); ++nOrder )
								{
									psLocalFaceVertex = psLocalTileVertex->GetVertex( nOrder );
									if( psLocalFaceVertex->eVertexType == E_NAV_TILE_VERTEX_TYPE_RIGHT_TOP ||
										psLocalFaceVertex->eVertexType == E_NAV_TILE_VERTEX_TYPE_RIGHT_BOTTOM )
									{
										RegisterVertexAndFace( nIndexX, nTempY,
																nIndexX, nLocalY,
																nLocalX, nIndexY );
										++nExportCount;

										bFoundApex = true;
										nLocalY = nTempY;

										break;
									}
								}

								if( bFoundApex == true )
								{
									break;
								}

								++nTempY;
							}

							bHorizental = true;
						}
					}

					if( bHorizental == true )
					{
						nTempX = nLocalX - 1;

						while( nTempX > nIndexX )
						{
							psLocalTileVertex = GetTileVertex( nTempX, nIndexY );
							for( UInt32 nOrder = 0; nOrder < psLocalTileVertex->GetCount(); ++nOrder )
							{
								psLocalFaceVertex = psLocalTileVertex->GetVertex( nOrder );
								if( psLocalFaceVertex->eVertexType == E_NAV_TILE_VERTEX_TYPE_LEFT_TOP ||
									psLocalFaceVertex->eVertexType == E_NAV_TILE_VERTEX_TYPE_RIGHT_TOP )
								{
									RegisterVertexAndFace( nTempX, nIndexY,
															nIndexX, nLocalY,
															nLocalX, nIndexY );
									++nExportCount;

									nLocalX = nTempX;

									break;
								}
							}

							--nTempX;
						}
					}
					else
					{
						nTempY = nLocalY + 1;

						while( nTempY < nIndexY )
						{
							psLocalTileVertex = GetTileVertex( nIndexX, nTempY );
							for( UInt32 nOrder = 0; nOrder < psLocalTileVertex->GetCount(); ++nOrder )
							{
								psLocalFaceVertex = psLocalTileVertex->GetVertex( nOrder );
								if( psLocalFaceVertex->eVertexType == E_NAV_TILE_VERTEX_TYPE_RIGHT_TOP ||
									psLocalFaceVertex->eVertexType == E_NAV_TILE_VERTEX_TYPE_RIGHT_BOTTOM )
								{
									RegisterVertexAndFace( nIndexX, nTempY,
															nIndexX, nLocalY,
															nLocalX, nIndexY );
									++nExportCount;

									nLocalY = nTempY;

									break;
								}
							}

							++nTempY;
						}
					}

					RegisterVertexAndFace( nIndexX, nIndexY,
											nIndexX, nLocalY,
											nLocalX, nIndexY );
					++nExportCount;
				}

				if( psFaceVertex->eVertexType == E_NAV_TILE_VERTEX_TYPE_RIGHT_BOTTOM &&
					psFaceVertex->eFaceType == E_NAV_TILE_FACE_TYPE_RIGHT &&
					psFaceVertex->eDivideType == E_NAV_TILE_DIVIDE_TYPE_DOWN_LEFT )
				{
					RegisterVertexAndFace( nIndexX, nIndexY,
											nLocalX, nIndexY,
											nIndexX, nLocalY );
					++nExportCount;
				}
			}
		}
	}

	return nExportCount;
}


//*****************************************************************************
//
// End of File : CNavPositionFaceVertexMesh_Optimize.cpp
//
//*****************************************************************************