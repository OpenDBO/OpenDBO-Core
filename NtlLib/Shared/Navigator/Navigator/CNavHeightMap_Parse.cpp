//*****************************************************************************
// File       : CNavHeightMap.cpp
// Desc       : 
// Author     : June C. Kim (cynis@hotmail.com)
// Update     : 2006. 12. 5
//*****************************************************************************

//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "StdAfx.h"

#include "CMemoryBuffer.h"
#include "CNavPathEngine.h"
#include "CNavConverter.h"

#include "CNavigator.h"

#include "CNavHeightMap.h"


//-----------------------------------------------------------------------------
// Name Space Table
//-----------------------------------------------------------------------------

USE_NAMESPACE( nav )


//-----------------------------------------------------------------------------
// Initialize Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Name			: CreateEmpty
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CNavHeightMap::CreateEmpty( SNavEntityCreateInfo & rsCreateInfo )
{
	CNavPosition *	pcTileSize = reinterpret_cast<CNavPosition *>(rsCreateInfo.pvUserData);
	Assert( pcTileSize != NULL );

	m_cPrecision.Set( *pcTileSize );

	if( CreateBuffer() == false )
	{
		return false;
	}

	return true;
}


//-----------------------------------------------------------------------------
// Name			: CreateFromFile
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CNavHeightMap::CreateFromFile( SNavEntityCreateInfo & rsCreateInfo )
{
	CFixedString	cFullFileName;

	if( rsCreateInfo.cPath.GetLength() > 0L )
	{
		cFullFileName.Format( _T( "%s\\%s.hmd" ),
							rsCreateInfo.cPath.GetBuffer(),
							rsCreateInfo.cName.GetBuffer() );
	}
	else
	{
		cFullFileName.Assign( rsCreateInfo.cName );		
	}

	CMemoryBuffer		cBuffer(cFullFileName );

	if( cBuffer.GetLength() == 0 )
	{
		Assert( !"Can't Find File or Empty File" );

		return false;
	}

	// Read Type & Byte Order Marker

	WORD		wMarker;
	cBuffer.Read( &wMarker, sizeof( WORD ) );

	if( wMarker != ( ( (WORD)'H' << 8 ) | (WORD)'M' ) )
	{
		if( wMarker != ( ( (WORD)'M' << 8 ) | (WORD)'H' ) )
		{
			Assert( !"Different Endian Format" );
		}
		else
		{
			Assert( !"Invalid Height Map File Format" );
		}
	}

	// Read Version

	WORD		wVersion;
	cBuffer.Read( &wVersion, sizeof( WORD ) );

	if( wVersion < 0x0200 )
	{
		Assert( !"Old Version Format" );
	}
	else if( wVersion > 0x0200 )
	{
		Assert( !"Newer Version Format" );
	}

	// Read Bounding

	CNavBounding	cBounding;
	cBuffer.Read( &cBounding, sizeof( CNavBounding ) );

	Assert( cBounding.IsValid() );

	// Read Precision

	CNavPosition	cPrecision;
	cBuffer.Read( &cPrecision, sizeof( CNavPosition ) );

	// Create Buffer

	Reserve( cBounding, cPrecision );

	// Read Height Data

	cBuffer.Read( m_pnHeights, sizeof( Fixed32 ) * m_nHeightCount );

	// Read Attribute Data

	cBuffer.Read( m_pnAttributes, sizeof( UInt32 ) * m_nAttributeCount );

	return true;
}


//-----------------------------------------------------------------------------
// Name			: ImportFromFile
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CNavHeightMap::ImportFromFile( SNavEntityCreateInfo & rsCreateInfo )
{
	UNREFERENCED_PARAMETER( rsCreateInfo );

	CNavigator *	pcNavigator = down_cast<CNavigator *>(rsCreateInfo.pcParent->GetRoot());

	CNavConverter *		pcImporter = ( pcNavigator->GetImporter() != NULL ) ?
										pcNavigator->GetImporter() :
										CNavigator::GetDefaultConverter();
	Assert( pcImporter != NULL );

	SNavEntityCreateInfo	sCreateInfo( rsCreateInfo );
	sCreateInfo.pvUserData = this;
	sCreateInfo.nUserDataSize = sizeof( CNavHeightMap * );

	return pcImporter->ImportHeightMap( &sCreateInfo );
}


//*****************************************************************************
//
// End of File : CNavHeightMap_Parse.cpp
//
//*****************************************************************************