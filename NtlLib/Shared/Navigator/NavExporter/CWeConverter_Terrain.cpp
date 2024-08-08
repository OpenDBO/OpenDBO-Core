//*****************************************************************************
// File       : CWeConverter_Terrain.cpp
// Desc       : 
// Author     : June C. Kim (cynis@hotmail.com)
// Update     : 2007. 1. 18
//*****************************************************************************

//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "StdAfx.h"

#include "CNavPositionFaceVertexMesh.h"

#include "CWeConverter.h"


//-----------------------------------------------------------------------------
// Name Space Table
//-----------------------------------------------------------------------------

USE_NAMESPACE( navex )


//-----------------------------------------------------------------------------
// Initialize Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Name			: ImportTerrain
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CWeConverter::ImportTerrain( void * pvData )
{
	SNavEntityCreateInfo *	psCreateInfo = reinterpret_cast<SNavEntityCreateInfo *>(pvData);

	// Load Reference File (Get World Create Info)

	if( m_bHasReference == false )
	{
		CFixedString	cReferenceFileName;

		m_cReferencePath.GetFullPathName( cReferenceFileName );

		if( LoadReference( cReferenceFileName ) == false )
		{
			return false;
		}

		m_bHasReference = true;
	}

	// Create Base Face Vertex Mesh

	iMesh *		pcMesh = CreateMeshFromZone( *psCreateInfo );

	if( pcMesh == NULL )
	{
		return false;
	}

	CNavTerrain *	pcTerrain = reinterpret_cast<CNavTerrain *>(psCreateInfo->pvUserData);
	Assert( pcTerrain != NULL );

	pcTerrain->SetMesh( pcMesh );

	return true;
}


//-----------------------------------------------------------------------------
// Name			: CreateMeshFromZone
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

iMesh * CWeConverter::CreateMeshFromZone( const SNavEntityCreateInfo & rsCreateInfo ) const
{
	// Get Mesh Bound

	CNavBounding	cMeshBounding;
	if( GetMeshBound( cMeshBounding, rsCreateInfo ) == false )
	{
		return NULL;
	}

	// Create Base Face Vertex Mesh

	CNavTerrain *	pcTerrain = reinterpret_cast<CNavTerrain *>(rsCreateInfo.pvUserData);
	Assert( pcTerrain != NULL );

	CNavZone *		pcZone = reinterpret_cast<CNavZone *>(pcTerrain->GetParent());
	Assert( pcZone != NULL );

	const CNavEntityVector * const	pcSectors = pcZone->GetSectors();

	UInt32		nConvertCount = pcSectors->GetCount();
	if( nConvertCount == 0 )
	{
		return NULL;
	}

	CNavPositionFaceVertexMesh **	ppcFaceVertexMeshes = new CNavPositionFaceVertexMesh *[1];

	ppcFaceVertexMeshes[0] = down_cast<CNavPositionFaceVertexMesh *>(CNavPositionFaceVertexMesh::CreateFaceVertexMesh());


	// Load Height Map

	CNavSector *	pcSector;
	CNavHeightMap *		pcHeightMap;

	UInt32		nOriginalFaceCount = 0;

	for( UInt32 nOrder = 0; nOrder < nConvertCount; ++nOrder )
	{
		pcSector = down_cast<CNavSector *>(pcSectors->GetAt( nOrder ));
		Assert( pcSector != NULL );

		pcHeightMap = const_cast<CNavHeightMap *>(pcSector->GetHeightMap());
		Assert( pcHeightMap != NULL );

		nOriginalFaceCount += ppcFaceVertexMeshes[0]->AddHeightMap( pcHeightMap );
	}

	Assert( nOriginalFaceCount != 0 );

	ppcFaceVertexMeshes[0]->BuildAndOptimize( __COMPILE_OPTION_OPTIMIZE_MESH__, 0 );

	//UInt32		nOptimizedFaceCount = ppcFaceVertexMeshes[0]->faces();

	//TCHAR	szBuffer[1024];
	//_stprintf_s( szBuffer, 1024, _T( "Orignal Face Count : %d" ), nOriginalFaceCount );
	//g_Log( szBuffer );

	//_stprintf_s( szBuffer, 1024, _T( "Optimized Face Count : %d" ), nOptimizedFaceCount );
	//g_Log( szBuffer );

	//_stprintf_s( szBuffer, 1024, _T( "Optimized Ratio : %f" ), 100.0f * (1.0f - (float)nOptimizedFaceCount / (float)nOriginalFaceCount) );
	//g_Log( szBuffer );

	// Create iMesh

	iMesh *		pcMesh = CNavPathEngine::GetInstance()->CreateMeshFromFaceVertexMesh( reinterpret_cast<iFaceVertexMesh **>(ppcFaceVertexMeshes), 1, NULL );

	CNavPositionFaceVertexMesh::DestroyFaceVertexMesh( &ppcFaceVertexMeshes[0] );
	delete[] ppcFaceVertexMeshes;

	if( pcMesh == NULL )
	{
		Assert( !"CNavPathEngine::CreateMeshFromFaceVertexMesh() Fail" );

		return NULL;
	}

	// Convert Obstacle

#ifdef __COMPILE_OPTION_PLACE_OPSTACLE__
	ConvertObstacles( pcMesh, pcSectors );
//	_stprintf_s( szBuffer, 1024, _T( "Converted Opstacle Count : %d" ), nConvertedObstacleCount );
//	g_Log( szBuffer );
#endif //__COMPILE_OPTION_PLACE_OPSTACLE__

	// Generate Preprocess

	return pcMesh;
}


//-----------------------------------------------------------------------------
// Name			: GetMeshBound
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CWeConverter::GetMeshBound( CNavBounding & rcMeshBounding, const SNavEntityCreateInfo & rsCreateInfo ) const
{
	CNavZone *	pcZone = down_cast<CNavZone *>(rsCreateInfo.pcParent);
	Assert( pcZone != NULL );

	if( pcZone->GetBounding().IsValid() == true )
	{
		rcMeshBounding.Set( rsCreateInfo.cBounding );

		return true;
	}

	Assert( !"Not Supported Yet" );

	// Get Sector List

	// Merge Sector Boundings

	return false;
}


//-----------------------------------------------------------------------------
// Name			: ExportTerrain
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CWeConverter::ExportTerrain( void * pvData )
{
	UNREFERENCED_PARAMETER( pvData );

	AfxMessageBox( _T( "ImportTerrain" ) );

	// Burn Into Context

	// Export iMesh

	return false;
}


//*****************************************************************************
//
// End of File : CWeConverter_Terrain.cpp
//
//*****************************************************************************