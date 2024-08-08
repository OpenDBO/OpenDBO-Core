//*****************************************************************************
// File       : CWeConverter_Obsoleted.cpp
// Desc       : 
// Author     : June C. Kim (cynis@hotmail.com)
// Update     : 2007. 1. 18
//*****************************************************************************

//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "StdAfx.h"

#include "VersionLog.h"

#include "CWeConverter.h"


//-----------------------------------------------------------------------------
// Name Space Table
//-----------------------------------------------------------------------------

USE_NAMESPACE( navex )


//-----------------------------------------------------------------------------
// Initialize Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Name			: LoadReference
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

/* obsoleted

bool CWeConverter::LoadReference( const CFixedString & rcFileName )
{
	CMemoryBuffer	cBuffer;

	if( cBuffer.Create( rcFileName ) == false )
	{
		return false;
	}

	SNtlWeWorldInfo		sWorldParameter;

	cBuffer.Copy( &sWorldParameter.WorldHeightfieldNum, sizeof( RwInt32 ) );
	cBuffer.Copy( &sWorldParameter.WorldSectorTileSize, sizeof( RwInt32 ) );
	cBuffer.Copy( &sWorldParameter.WorldSectorTileNum, sizeof( RwInt32 ) );
	cBuffer.Copy( &sWorldParameter.WorldSectorVertNum, sizeof( RwInt32 ) );
	cBuffer.Copy( &sWorldParameter.WorldSectorSize, sizeof( RwInt32 ) );
	cBuffer.Copy( &sWorldParameter.WorldSectorNum, sizeof( RwInt32 ) );
	cBuffer.Copy( &sWorldParameter.WorldSectorHalfNum, sizeof( RwInt32 ) );
	cBuffer.Copy( &sWorldParameter.WorldSectorPolyNum, sizeof( RwInt32 ) );
	cBuffer.Copy( &sWorldParameter.WorldFieldSectorNum, sizeof( RwInt32 ) );
	cBuffer.Copy( &sWorldParameter.WorldFieldSize, sizeof( RwInt32 ) );
	cBuffer.Copy( &sWorldParameter.WorldFieldNum, sizeof( RwInt32 ) );
	cBuffer.Copy( &sWorldParameter.WorldFieldHalfNum, sizeof( RwInt32 ) );
	cBuffer.Copy( &sWorldParameter.WorldSize, sizeof( RwInt32 ) );
	cBuffer.Copy( &sWorldParameter.WorldSizeHalf, sizeof( RwInt32 ) );
	cBuffer.Copy( &sWorldParameter.WorldValueMax, sizeof( RwInt32 ) );
	cBuffer.Copy( &sWorldParameter.WorldValueMin, sizeof( RwInt32 ) );
	cBuffer.Copy( &sWorldParameter.WorldTexAddr, sizeof( RwInt32 ) );
	cBuffer.Copy( &sWorldParameter.WorldPolyCnt, sizeof( RwInt32 ) );
	cBuffer.Copy( &sWorldParameter.WorldNearPlane, sizeof( RwReal ) );
	cBuffer.Copy( &sWorldParameter.WorldFarPlane, sizeof( RwReal ) );
	cBuffer.Copy( &sWorldParameter.WorldCamSpeedPerSec, sizeof( RwReal ) );

	m_cTileSize.SetFloat( static_cast<Float32>(sWorldParameter.WorldSectorTileSize), 0.0f, static_cast<Float32>(sWorldParameter.WorldSectorTileSize) );
	m_cSectorSize.SetFloat( static_cast<Float32>(sWorldParameter.WorldFieldSize), 0.0f, static_cast<Float32>(sWorldParameter.WorldFieldSize) );
	m_nSectorsInField = sWorldParameter.WorldFieldSectorNum;
	m_nSectorsInWorld = sWorldParameter.WorldSectorNum;

	CFixedString	cModelListName( _T( "ObjectPropertyList" ) );

	if( CreateModelInfoes( cModelListName ) == false )
	{
		return false;
	}

	return true;
}
*/


/*
	SWeObstacleInfo		sObstacle;
	CNavPosition		cSamplePosition;
	cPosition		cPlacePosition;
	iAgent *	pcAgent;
	iShape *	pcShape;

	SNavCellPosition2D	sCellPosition;
	pcSector->GetCellPosition( sCellPosition );

	UInt32		nFileIdBeginX = sCellPosition.sIndex.GetX() * m_nSectorsInField;
	UInt32		nFileIdBeginZ = sCellPosition.sIndex.GetY() * m_nSectorsInField;
	UInt32		nFileIdEndX = nFileIdBeginX + m_nSectorsInField - 1;
	UInt32		nFileIdEndZ = nFileIdBeginZ + m_nSectorsInField - 1;
	UInt32		nFileId;
	UInt32		nAddedCount = 0;
	UInt32		nFailCount = 0;
	HANDLE		hFile;
	UInt32		nFileSize;
	CFixedString	cObjectFileName;
	CFixedString	cFileIndex;

	for( UInt32 nFileIdX = nFileIdBeginX; nFileIdX <= nFileIdEndX; ++nFileIdX )
	{
		for( UInt32 nFileIdZ = nFileIdBeginZ; nFileIdZ <= nFileIdEndZ; ++nFileIdZ )
		{
			nFileId = nFileIdZ * m_nSectorsInWorld + nFileIdX;

			cFileIndex.Format( _T( "%d" ), nFileId );

			m_cObjectPath.GetFullPathName( cObjectFileName, cFileIndex );

			hFile = CreateFileSafe( cObjectFileName, GENERIC_READ );
			Assert( hFile != INVALID_HANDLE_VALUE );

			nFileSize = GetFileSize( hFile, NULL );
			if( nFileSize <= 4 )
			{
				CloseHandle( hFile );

				continue;
			}

			UInt32		nObstacleCount;

			ReadFile( hFile, &nObstacleCount, 4, &dwReadSize, NULL );

			nObstacleCount = ( nFileSize - 4 ) / sizeof( sObstacle );

			for( UInt32 nObstacleIndex = 0; nObstacleIndex < nObstacleCount; ++nObstacleIndex )
			{
				ReadFile( hFile, &sObstacle, sizeof( sObstacle ), &dwReadSize, NULL );
				cSamplePosition.SetFloat( static_cast<Float32>(sObstacle.position[0]),
										0.0f,
										static_cast<Float32>(sObstacle.position[2]) );

				if( ConvertPosition( &cPlacePosition, &cSamplePosition, pcMesh ) == true )
				{
					CFixedString	cModelName( sObstacle.szName );
					const SWeModelInfoNode &	rsModelInfo = GetModelInfo( cModelName );

					if( rsModelInfo.sInfo.nFlags == 0 )				// Skip Non-Obstacle Object
					{
						continue;
					}

					// Get Original & Scale

					afOriginalVertices[0] = Fixed32ToFloat32( -rsModelInfo.sInfo.cBounding.GetX() / 2 ) * sObstacle.scale[0];
					afOriginalVertices[1] = Fixed32ToFloat32( +rsModelInfo.sInfo.cBounding.GetY() / 2 ) * sObstacle.scale[2];

					afOriginalVertices[2] = Fixed32ToFloat32( +rsModelInfo.sInfo.cBounding.GetX() / 2 ) * sObstacle.scale[0];
					afOriginalVertices[3] = Fixed32ToFloat32( +rsModelInfo.sInfo.cBounding.GetY() / 2 ) * sObstacle.scale[2];

					afOriginalVertices[4] = Fixed32ToFloat32( +rsModelInfo.sInfo.cBounding.GetX() / 2 ) * sObstacle.scale[0];
					afOriginalVertices[5] = Fixed32ToFloat32( -rsModelInfo.sInfo.cBounding.GetY() / 2 ) * sObstacle.scale[2];

					afOriginalVertices[6] = Fixed32ToFloat32( -rsModelInfo.sInfo.cBounding.GetX() / 2 ) * sObstacle.scale[0];
					afOriginalVertices[7] = Fixed32ToFloat32( -rsModelInfo.sInfo.cBounding.GetY() / 2 ) * sObstacle.scale[2];

					for( UInt32	nIndex = 0; nIndex < 8; ++nIndex )
					{
						if( afOriginalVertices[nIndex] > OBSTACLE_SIZE_MAX )
						{
							afOriginalVertices[nIndex] = OBSTACLE_SIZE_MAX;
						}
						else if( afOriginalVertices[nIndex] < -OBSTACLE_SIZE_MAX )
						{
							afOriginalVertices[nIndex] = -OBSTACLE_SIZE_MAX;
						}
					}

					// Rotate

					Float32		fTheta = PI * sObstacle.direction[1] / 180.0f;
					Float32		fCosTheta = cos( fTheta );
					Float32		fSinTheta = sin( fTheta );

					afTranformedVertices[0] = fCosTheta * afOriginalVertices[0] - fSinTheta * afOriginalVertices[1];
					afTranformedVertices[1] = fSinTheta * afOriginalVertices[0] + fCosTheta * afOriginalVertices[1];

					afTranformedVertices[2] = fCosTheta * afOriginalVertices[2] - fSinTheta * afOriginalVertices[3];
					afTranformedVertices[3] = fSinTheta * afOriginalVertices[2] + fCosTheta * afOriginalVertices[3];

					afTranformedVertices[4] = fCosTheta * afOriginalVertices[4] - fSinTheta * afOriginalVertices[5];
					afTranformedVertices[5] = fSinTheta * afOriginalVertices[4] + fCosTheta * afOriginalVertices[5];

					afTranformedVertices[6] = fCosTheta * afOriginalVertices[6] - fSinTheta * afOriginalVertices[7];
					afTranformedVertices[7] = fSinTheta * afOriginalVertices[6] + fCosTheta * afOriginalVertices[7];

					for( UInt32	nIndex = 0; nIndex < 8; ++nIndex )
					{
						anQuantitizedVertices[nIndex] = Float32ToFixed32( afTranformedVertices[nIndex] );
					}

					pcShape = pcPathEngine->newShape( MAX_VERTEX_PER_SHAPE / 2, anQuantitizedVertices );
					if( pcShape == NULL )
					{
						++nFailCount;

						//	TCHAR	szBuf[1024];
						//	_stprintf_s( szBuf, 1024, _T( "Fail by Shape (%d) : (%d, %d) (%d, %d) (%d, %d) (%d, %d)" ),
						//				nFailCount,
						//				anQuantitizedVertices[0], anQuantitizedVertices[1],
						//				anQuantitizedVertices[2], anQuantitizedVertices[3],
						//				anQuantitizedVertices[4], anQuantitizedVertices[5],
						//				anQuantitizedVertices[6], anQuantitizedVertices[7] );
						//	AfxMessageBox( szBuf );

						continue;
					}

					pcAgent = pcMesh->placeAgent( pcShape, cPlacePosition );
					if( pcAgent == NULL )
					{
						++nFailCount;

						//TCHAR	szBuf[1024];
						//_stprintf_s( szBuf, 1024, _T( "Fail by Place(%d) : (%d, %d) (%d, %d) (%d, %d) (%d, %d)" ),
						//			nFailCount,
						//			anQuantitizedVertices[0], anQuantitizedVertices[1],
						//			anQuantitizedVertices[2], anQuantitizedVertices[3],
						//			anQuantitizedVertices[4], anQuantitizedVertices[5],
						//			anQuantitizedVertices[6], anQuantitizedVertices[7] );
						//g_Log( szBuf );

						continue;
					}

					++nAddedCount;

					//TCHAR	szBuf[1024];
					//_stprintf_s( szBuf, 1024, _T( "Success(%d) : (%d, %d) (%d, %d) (%d, %d) (%d, %d)" ),
					//			nAddedCount,
					//			anQuantitizedVertices[0], anQuantitizedVertices[1],
					//			anQuantitizedVertices[2], anQuantitizedVertices[3],
					//			anQuantitizedVertices[4], anQuantitizedVertices[5],
					//			anQuantitizedVertices[6], anQuantitizedVertices[7] );
					//g_Log( szBuf );

					pcContext->addAgent( pcAgent );
					pcObstacleSet->addAgent( pcAgent );
				}
				else
				{
					++nFailCount;

					//TCHAR	szBuf[1024];
					//_stprintf_s( szBuf, 1024, _T( "Failed By Position(%d) : (%d, %d)" ),
					//			nFailCount,
					//			cSamplePosition.GetX(), cSamplePosition.GetY() );
					//g_Log( szBuf );
				}
			}

			CloseHandle( hFile );
		}
	}

	pcMesh->burnContextIntoMesh( pcContext );

	return nAddedCount;
}
*/

//*****************************************************************************
//
// End of File : CWeConverter_Obsoleted.cpp
//
//*****************************************************************************