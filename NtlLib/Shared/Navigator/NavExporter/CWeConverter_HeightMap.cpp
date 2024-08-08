//*****************************************************************************
// File       : CWeConverter_HeightMap.cpp
// Desc       : 
// Author     : June C. Kim (cynis@hotmail.com)
// Update     : 2007. 1. 18
//*****************************************************************************

//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "StdAfx.h"

#include "CWeConverter.h"


//-----------------------------------------------------------------------------
// Name Space Table
//-----------------------------------------------------------------------------

USE_NAMESPACE( navex )


//-----------------------------------------------------------------------------
// Initialize Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Name			: ImportHeightMap
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CWeConverter::ImportHeightMap( void * pvData )
{
	SNavEntityCreateInfo *	psCreateInfo = reinterpret_cast<SNavEntityCreateInfo *>(pvData);

	if( m_bHasReference == false )
	{
		CFixedString	cReferenceFileName( CNavigator::GetPathName() );

		m_cReferencePath.GetFullPathName( cReferenceFileName );

		if( LoadReference( cReferenceFileName ) == false )
		{
			return false;
		}

		m_bHasReference = true;
	}

	// Get Entity

	CNavHeightMap *	pcHeightMap = reinterpret_cast<CNavHeightMap *>(psCreateInfo->pvUserData);
	Assert( pcHeightMap != NULL );

	CNavSector *	pcSector = down_cast<CNavSector *>(psCreateInfo->pcParent);
	Assert( pcSector != NULL );

	pcHeightMap->Reserve( psCreateInfo->cBounding, m_cTileSize );
	UInt32	nSectorOrder = pcSector->GetCellPosition();

	CFixedString	cHeightMapFileName( CNavigator::GetPathName() );
	CFixedString	cAttributeFileName( CNavigator::GetPathName() );
	CFixedString	cFileIndex;
	cFileIndex.Format( _T( "%d" ), nSectorOrder );

	m_cHeightMapPath.GetFullPathName( cHeightMapFileName, cFileIndex );
	m_cAttributePath.GetFullPathName( cAttributeFileName, cFileIndex );

	if( ConvertWeHeightMapFile( cHeightMapFileName, cAttributeFileName, pcHeightMap ) == false )
	{
		return false;
	}

	return true;
}


//-----------------------------------------------------------------------------
// Name			: ExportHeightMap
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CWeConverter::ExportHeightMap( void * pvData )
{
	UNREFERENCED_PARAMETER( pvData );

	return false;
}


//-----------------------------------------------------------------------------
// Name			: ConvertWeHeightMapFile
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CWeConverter::ConvertWeHeightMapFile( const CFixedString & rcHeightFileName, CNavHeightMap * pcHeightMap )
{
	CMemoryBuffer	cHeightBuffer;

	if( cHeightBuffer.Create( rcHeightFileName ) == false )
	{
		return false;
	}

	CNavPosition	cPosition;
	Float32			afPosition[3];

	while( cHeightBuffer.IsEnd() == false )
	{
		cHeightBuffer.Copy( afPosition, sizeof( Float32 ) * 3 );

		cPosition.SetFloat( afPosition[0], afPosition[1], afPosition[2] );

		pcHeightMap->SetHeight( cPosition );
		pcHeightMap->SetAttribute( cPosition, E_NAV_TILE_ATTRIBUTE_NORMAL );
	}

	if( pcHeightMap->IsValid() == false )
	{
		Assert( !"Height Map Convert Fail" );

		return false;
	}

	return true;
}


//-----------------------------------------------------------------------------
// Name			: ConvertWeHeightMapFile
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CWeConverter::ConvertWeHeightMapFile( const CFixedString & rcHeightFileName, const CFixedString & rcAttributeFileName, CNavHeightMap * pcHeightMap )
{
	if( ConvertWeHeightMapFile( rcHeightFileName, pcHeightMap ) == false )
	{
		Assert( !"ConvertWeHeightMapFile() Fail" );

		return false;
	}

	CMemoryBuffer	cAttributeBuffer;

	if( cAttributeBuffer.Create( rcAttributeFileName ) == false )
	{
		return false;
	}

	CNavPosition	cPosition;
	DWORD			dwAttribute;

	for( cPosition.SetY( pcHeightMap->GetBounding().GetMin().GetY() ); cPosition.GetY() < pcHeightMap->GetBounding().GetMax().GetY(); cPosition.SetY( cPosition.GetY() + pcHeightMap->GetPrecision().GetY() ) )
	{
		for( cPosition.SetX( pcHeightMap->GetBounding().GetMin().GetX() ); cPosition.GetX() < pcHeightMap->GetBounding().GetMax().GetX(); cPosition.SetX( cPosition.GetX() + pcHeightMap->GetPrecision().GetX() ) )
		{
			cAttributeBuffer.Copy( &dwAttribute, sizeof( DWORD ) );

			pcHeightMap->SetAttribute( cPosition, dwAttribute );

			if( dwAttribute != 0 )
			{
				TCHAR s[1024];
				_stprintf_s( s, 1024, _T( "(%f %f) : %.2f (0x%08X)" ),
							cPosition.GetFloatX(), cPosition.GetFloatZ(), cPosition.GetFloatY(), dwAttribute );
				g_Log( s );
			}
		}
	}

	return true;
}


//*****************************************************************************
//
// End of File : CWeConverter_HeightMap.cpp
//
//*****************************************************************************