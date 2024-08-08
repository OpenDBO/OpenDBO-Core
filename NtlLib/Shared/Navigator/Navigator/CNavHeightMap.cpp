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

#include "CNavBounding2D.h"
#include "CNavRenderer.h"
#include "CNavFilePath.h"

#include "CNavHeightMap.h"


//-----------------------------------------------------------------------------
// Name Space Table
//-----------------------------------------------------------------------------

USE_NAMESPACE( nav )


//-----------------------------------------------------------------------------
// Initialize Table
//-----------------------------------------------------------------------------

CFixedString	CNavHeightMap::m_cPathName		= CFixedString::NULLSTRING;


//-----------------------------------------------------------------------------
// Name			: Initialize
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CNavHeightMap::Initialize( void )
{
	m_pnHeights			= NULL;
	m_sHeightCount.Reset();
	m_nHeightCount		= 0L;

	m_pnAttributes		= NULL;
	m_sAttributeCount.Reset();
	m_nAttributeCount	= 0L;

	m_cPrecision.Reset();

	return;
}


//-----------------------------------------------------------------------------
// Name			: Destroy
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CNavHeightMap::Destroy( void )
{
	DestroyBuffer();

	Initialize();

	return;
}


//-----------------------------------------------------------------------------
// Name			: Create
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CNavHeightMap::Create( SNavEntityCreateInfo & rsCreateInfo )
{
	if( CNavEntity::Create( rsCreateInfo ) == false )
	{
		return false;
	}

	switch( rsCreateInfo.eCreateType )
	{
		case E_NAV_CREATE_TYPE_FILE:
		{
			if( rsCreateInfo.nFlags & E_NAV_ENTITY_CREATE_INFO_FLAG_EXPORT_MODE )
			{
				if( ImportFromFile( rsCreateInfo ) == false )
				{
					return false;
				}
			}
			else
			{
				if( CreateFromFile( rsCreateInfo ) == false )
				{
					return false;
				}
			}

			break;
		}

		case E_NAV_CREATE_TYPE_INSTANCE:
		{
			Assert( !"Unsupported Create Type" );

			break;
		}

		case E_NAV_CREATE_TYPE_EMPTY:
		{
			if( CreateEmpty( rsCreateInfo ) == false )
			{
				return false;
			}

			break;
		}

		default:
		{
			Assert( !"Undefined Create Type" );
		}
	}

	return true;
}


//-----------------------------------------------------------------------------
// Name			: PostCreate
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CNavHeightMap::PostCreate( SNavEntityCreateInfo & rsCreateInfo )
{
	if( m_bExport == true )
	{
		CNavFilePath	cFilePath;
		cFilePath.SetPath( rsCreateInfo.cPath );
		cFilePath.SetName( rsCreateInfo.cName );
		cFilePath.SetExt( CNavStringTable::HMD );

		CFixedString	cFullFileName;
		cFilePath.GetFullPathName( cFullFileName );

		return ExportToFile( cFullFileName, 0 );
	}

	return true;
}


//-----------------------------------------------------------------------------
// Name			: Render
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

Int32 CNavHeightMap::Render( CNavRenderer * const pcRenderer ) const
{
	Assert( pcRenderer != NULL );

	pcRenderer->Skip();

	return 0;
}


//-----------------------------------------------------------------------------
// Name			: Create
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CNavHeightMap::Reserve( const CNavBounding & rcBounding, const CNavPosition & rcPrecision )
{
	// Set Bounding

	Assert( rcBounding.IsValid() );
	m_cBounding.Set( rcBounding );

	// Set Precision

	m_cPrecision.Set( rcPrecision );

	// Create Buffer

	return CreateBuffer();
}


//-----------------------------------------------------------------------------
// Name			: CreateBuffer
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CNavHeightMap::CreateBuffer( void )
{
	// HeightMap Buffer

	m_sHeightCount.SetX( ( m_cBounding.GetSizeX() / m_cPrecision.GetX() ) + 1 );
	m_sHeightCount.SetY( ( m_cBounding.GetSizeY() / m_cPrecision.GetY() ) + 1 );
	m_nHeightCount = m_sHeightCount.GetArea();

	Assert( m_pnHeights == NULL );
	Assert( m_nHeightCount > 0 );

	m_pnHeights = new Fixed32[m_nHeightCount];
	Assert( m_pnHeights != NULL );

	// AttributeMap Buffer

	m_sAttributeCount.SetX( m_cBounding.GetSizeX() / m_cPrecision.GetX() );
	m_sAttributeCount.SetY( m_cBounding.GetSizeY() / m_cPrecision.GetY() );
	m_nAttributeCount = m_sAttributeCount.GetArea();

	m_pnAttributes = new UInt32[m_nAttributeCount];
	Assert( m_pnAttributes != NULL );

	ZeroMemory( m_pnAttributes, sizeof( UInt32 ) * m_nAttributeCount );

	return true;
}


//-----------------------------------------------------------------------------
// Name			: DestroyBuffer
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CNavHeightMap::DestroyBuffer( void )
{
	if( m_pnAttributes != NULL )
	{
		delete[] m_pnAttributes;
	}

	if( m_pnHeights != NULL )
	{
		delete[] m_pnHeights;
	}

	return;
}


//-----------------------------------------------------------------------------
// Name			: GetHeight
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CNavHeightMap::GetHeight( CNavPosition & rcPosition ) const
{
#if _DEBUG
	CNavBounding2D	cBounding2D( GetBounding() );
	CNavPosition2D	cPosition2D( rcPosition );
	Assert( cBounding2D.IsContain( cPosition2D ) == true );
#endif //_DEBUG

	SNavIndex2D		sHeightIndex( ( rcPosition.GetX() - GetBounding().GetMin().GetX() ) / m_cPrecision.GetX(),
								( rcPosition.GetY() - GetBounding().GetMin().GetY() ) / m_cPrecision.GetY()	);

/*
	if( sHeightIndex.GetX() < m_sHeightCount.GetX() )
	{
		if( sHeightIndex.GetY() < m_sHeightCount.GetY() )
		{
			nHeightLT = rcPosition.SetZ( m_pnHeights[sHeightIndex.GetIndex( m_sHeightCount.GetX() )] );
		}
		else
		{
		}
	}
	else
	{
		if( sHeightIndex.GetY() < m_sHeightCount.GetY() )
		{
		}
		else
		{*/
			rcPosition.SetZ( m_pnHeights[sHeightIndex.GetIndex( m_sHeightCount.GetX() )] );
/*		}
	}
*/
	return true;
}


//-----------------------------------------------------------------------------
// Name			: SetHeight
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CNavHeightMap::SetHeight( const CNavPosition & rcPosition )
{
#if _DEBUG
	CNavBounding2D	cBounding2D( GetBounding() );
	CNavPosition2D	cPosition2D( rcPosition );
	Assert( cBounding2D.IsContain( cPosition2D ) == true );
#endif //_DEBUG

	SNavIndex2D		sHeightIndex( ( rcPosition.GetX() - GetBounding().GetMin().GetX() ) / m_cPrecision.GetX(),
								( rcPosition.GetY() - GetBounding().GetMin().GetY() ) / m_cPrecision.GetY()	);

	m_pnHeights[sHeightIndex.GetIndex( m_sHeightCount.GetX() )] = rcPosition.GetZ();

	if( rcPosition.GetZ() > GetBounding().GetMax().GetZ() )
	{
		m_cBounding.SetMaxZ( rcPosition.GetZ() );
	}

	if( rcPosition.GetZ() < GetBounding().GetMin().GetZ() )
	{
		m_cBounding.SetMinZ( rcPosition.GetZ() );
	}

	return true;
}


//-----------------------------------------------------------------------------
// Name			: GetAttribute
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

UInt32 CNavHeightMap::GetAttribute( CNavPosition & rcPosition ) const
{
	CNavBounding2D	cBounding2D( GetBounding() );
	CNavPosition2D	cPosition2D( rcPosition );
#if _DEBUG
	
	Assert( cBounding2D.IsContain( cPosition2D ) == true );
#endif //_DEBUG

	if( cPosition2D.GetX() < cBounding2D.GetMax().GetX() &&
		 cPosition2D.GetY() < cBounding2D.GetMax().GetY() )
	{
		SNavIndex2D		sAttributetIndex( ( rcPosition.GetX() - GetBounding().GetMin().GetX() ) / m_cPrecision.GetX(),
										( rcPosition.GetY() - GetBounding().GetMin().GetY() ) / m_cPrecision.GetY()	);

		return m_pnAttributes[sAttributetIndex.GetIndex( m_sAttributeCount.GetX() )];
	}
	else
	{
		Assert( !"Illegal Bound" );

		return E_NAV_TILE_ATTRIBUTE_NONE;
	}
}


//-----------------------------------------------------------------------------
// Name			: SetAttribute
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CNavHeightMap::SetAttribute( const CNavPosition & rcPosition, UInt32 nAttribute )
{
	CNavBounding2D	cBounding2D( GetBounding() );
	CNavPosition2D	cPosition2D( rcPosition );

#if _DEBUG
	Assert( cBounding2D.IsContain( cPosition2D ) == true );
#endif //_DEBUG

	if( cPosition2D.GetX() < cBounding2D.GetMax().GetX() &&
		 cPosition2D.GetY() < cBounding2D.GetMax().GetY() )
	{
		SNavIndex2D		sAttributetIndex( ( rcPosition.GetX() - GetBounding().GetMin().GetX() ) / m_cPrecision.GetX(),
										( rcPosition.GetY() - GetBounding().GetMin().GetY() ) / m_cPrecision.GetY()	);

		m_pnAttributes[sAttributetIndex.GetIndex( m_sAttributeCount.GetX() )] = nAttribute;

		return true;
	}
	else
	{
		return false;
	}	
}


//-----------------------------------------------------------------------------
// Name			: ExportToFile
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CNavHeightMap::ExportToFile( const CFixedString & rcFullFileName, UInt32 nFlags ) const
{
	UNREFERENCED_PARAMETER( nFlags );

	HANDLE		hFile = CreateFileSafe( rcFullFileName.GetBuffer(), GENERIC_WRITE );
	DWORD		dwWriteSize;

	// Write Type & Byte Order Marker

	static const WORD	MARKER = ( 'H' << 8 ) | 'M';
	WriteFile( hFile, &MARKER, sizeof( WORD ), &dwWriteSize, NULL );

	// Write Version

	static const WORD	VERSION = 0x0200;
	WriteFile( hFile, &VERSION, sizeof( WORD ), &dwWriteSize, NULL );

	// Write Bounding

	WriteFile( hFile, &m_cBounding, sizeof( CNavBounding ), &dwWriteSize, NULL );

	// Write Precision

	WriteFile( hFile, &m_cPrecision, sizeof( CNavPosition ), &dwWriteSize, NULL );

	// Write Data Offset

	static const UInt32		nDataOffset = 0;

	WriteFile( hFile, &nDataOffset, sizeof( UInt32 ), &dwWriteSize, NULL );

	// Write Height Data

	WriteFile( hFile, m_pnHeights, sizeof( Fixed32 ) * m_nHeightCount, &dwWriteSize, NULL );

	// Write Attribute Data

	WriteFile( hFile, m_pnAttributes, sizeof( UInt32 ) * m_nAttributeCount, &dwWriteSize, NULL );

	CloseHandle( hFile );

	return true;
}



//-----------------------------------------------------------------------------
// Name			: IsValid
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CNavHeightMap::IsValid( void ) const
{
	if( CNavEntity::IsValid() == false )
	{
		return false;
	}

	for( UInt32 nIndex = 0; nIndex < m_nAttributeCount; ++nIndex )
	{
		if( m_pnAttributes[nIndex] == E_NAV_TILE_ATTRIBUTE_NONE )
		{
			return false;
		}
	}

	return true;
}


//*****************************************************************************
//
// End of File : CNavHeightMap.cpp
//
//*****************************************************************************