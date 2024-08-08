//*****************************************************************************
// File       : CNavWorld_Parse.cpp
// Desc       : 
// Author     : June C. Kim (cynis@hotmail.com)
// Update     : 2007. 1. 18
//*****************************************************************************

//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "StdAfx.h"

#include "CFixedStringTreeNodeW.h"
#include "CXMLParserW.h"
#include "CNavEntityVector.h"

#include "CNavWorld.h"


//-----------------------------------------------------------------------------
// Name Space Table
//-----------------------------------------------------------------------------

USE_NAMESPACE( nav )


//-----------------------------------------------------------------------------
// Initialize Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Name			: CreateFromFile
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CNavWorld::CreateFromFile( SNavEntityCreateInfo & rsCreateInfo )
{
	CFixedString	cFullFileName;
	cFullFileName.Format( _T( "%s\\%s.xml" ),
							rsCreateInfo.cPath.GetBuffer(),
							rsCreateInfo.cName.GetBuffer() );

	CXMLParserW		cXMLParser;
	if( cXMLParser.Create( cFullFileName ) == false )
	{
		Assert( !"Fail to Open World Script File" );

		return false;
	}

	// Keyword List

	CFixedStringTreeNodeW *	pcRoot;
	CFixedStringTreeNodeW *	pcWorld;
	CFixedStringTreeNodeW *	pcHeader;
	CFixedStringTreeNodeW *	pcBound;
	CFixedStringTreeNodeW *	pcSector;
	CFixedStringTreeNodeW *	pcZoneList;


	// Get Root

	pcRoot = cXMLParser.GetRoot();
	Assert( pcRoot != NULL );

	pcWorld = pcRoot->FindChild( CNavStringTable::WORLD );
	Assert( pcWorld != NULL );


	// Parse Header

	pcHeader = pcWorld->FindChild( CNavStringTable::HEADER );
	Assert( pcHeader != NULL );

	if( ParseHeader( pcHeader ) == false )
	{
		return false;
	}


	// Parse Bound

	pcBound = pcWorld->FindChild( CNavStringTable::BOUND );
	Assert( pcBound != NULL );

	if( ParseBound( pcBound ) == false )
	{
		return false;
	}


	// Parse Sector

	pcSector = pcWorld->FindChild( CNavStringTable::SECTOR );
	Assert( pcSector != NULL );

	if( ParseSector( pcSector ) == false )
	{
		return false;
	}


	// Create Sector Map

	m_sSectorCount.SetX( m_cBounding.GetSizeX() / m_cSectorSize.GetX() );
	m_sSectorCount.SetY( m_cBounding.GetSizeY() / m_cSectorSize.GetY() );

	m_pcSectors = new CNavEntityVector( false );
	Assert( m_pcSectors != NULL );

	if( m_pcSectors->Create( m_sSectorCount.GetArea() ) == false )
	{
		return false;
	}


	// Parse Zone List

	pcZoneList = pcWorld->FindChild( CNavStringTable::ZONELIST );
	Assert( pcZoneList != NULL );

	if( ParseZoneList( rsCreateInfo, pcZoneList ) == false )
	{
		return false;
	}

	return true;
}


//-----------------------------------------------------------------------------
// Name			: ParseHeader
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CNavWorld::ParseHeader( CFixedStringTreeNodeW * pcHeader )
{
	Assert( pcHeader != NULL );

	CFixedStringTreeNodeW *	pcName;
	CFixedStringTreeNodeW *	pcValue;


	// Get Name

	pcName = pcHeader->FindChild( CNavStringTable::NAME );
	Assert( pcName != NULL );

	pcValue = pcName->FindChild();
	Assert( pcValue != NULL );

	CFixedString	cName( pcValue->GetString() );
	Assert( m_cName == cName );

	return true;
}


//-----------------------------------------------------------------------------
// Name			: ParseBound
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CNavWorld::ParseBound( CFixedStringTreeNodeW * pcBound )
{
	Assert( pcBound != NULL );

	CFixedStringTreeNodeW *	pcBoundMin;
	CFixedStringTreeNodeW *	pcBoundMinX;
	CFixedStringTreeNodeW *	pcBoundMinY;
	CFixedStringTreeNodeW *	pcBoundMinZ;
	CFixedStringTreeNodeW *	pcBoundMax;
	CFixedStringTreeNodeW *	pcBoundMaxX;
	CFixedStringTreeNodeW *	pcBoundMaxY;
	CFixedStringTreeNodeW *	pcBoundMaxZ;
	CFixedStringTreeNodeW *	pcValue;


	// Get Min

	pcBoundMin = pcBound->FindChild( CNavStringTable::MIN );
	Assert( pcBoundMin != NULL );

	pcBoundMinX = pcBoundMin->FindChild( CNavStringTable::X );
	Assert( pcBoundMinX != NULL );

	pcValue = pcBoundMinX->FindChild();
	Assert( pcValue != NULL );

	m_cBounding.SetFloatMinX( pcValue->GetString().ConvertToFloat32() );

	pcBoundMinY = pcBoundMin->FindChild( CNavStringTable::Y );
	Assert( pcBoundMinY != NULL );

	pcValue = pcBoundMinY->FindChild();
	Assert( pcValue != NULL );

	m_cBounding.SetFloatMinY( pcValue->GetString().ConvertToFloat32() );

	pcBoundMinZ = pcBoundMin->FindChild( CNavStringTable::Z );
	Assert( pcBoundMinZ != NULL );

	pcValue = pcBoundMinZ->FindChild();
	Assert( pcValue != NULL );

	m_cBounding.SetFloatMinZ( pcValue->GetString().ConvertToFloat32() );


	// Get Max

	pcBoundMax = pcBound->FindChild( CNavStringTable::MAX );
	Assert( pcBoundMax != NULL );

	pcBoundMaxX = pcBoundMax->FindChild( CNavStringTable::X );
	Assert( pcBoundMaxX != NULL );

	pcValue = pcBoundMaxX->FindChild();
	Assert( pcValue != NULL );

	m_cBounding.SetFloatMaxX( pcValue->GetString().ConvertToFloat32() );

	pcBoundMaxY = pcBoundMax->FindChild( CNavStringTable::Y );
	Assert( pcBoundMaxY != NULL );

	pcValue = pcBoundMaxY->FindChild();
	Assert( pcValue != NULL );

	m_cBounding.SetFloatMaxY( pcValue->GetString().ConvertToFloat32() );

	pcBoundMaxZ = pcBoundMax->FindChild( CNavStringTable::Z );
	Assert( pcBoundMaxZ != NULL );

	pcValue = pcBoundMaxZ->FindChild();
	Assert( pcValue != NULL );

	m_cBounding.SetFloatMaxZ( pcValue->GetString().ConvertToFloat32() );

	return true;
}


//-----------------------------------------------------------------------------
// Name			: ParseSector
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CNavWorld::ParseSector( CFixedStringTreeNodeW * pcSector )
{
	Assert( pcSector != NULL );

	CFixedStringTreeNodeW *	pcSize;
	CFixedStringTreeNodeW *	pcSizeX;
	CFixedStringTreeNodeW *	pcSizeY;
	CFixedStringTreeNodeW *	pcSizeZ;
	CFixedStringTreeNodeW *	pcOverlap;
	CFixedStringTreeNodeW *	pcOverlapX;
	CFixedStringTreeNodeW *	pcOverlapY;
	CFixedStringTreeNodeW *	pcOverlapZ;
	CFixedStringTreeNodeW *	pcValue;


	// Get Size

	pcSize = pcSector->FindChild( CNavStringTable::SIZE );
	Assert( pcSize != NULL );

	pcSizeX = pcSize->FindChild( CNavStringTable::X );
	Assert( pcSizeX != NULL );

	pcValue = pcSizeX->FindChild();
	Assert( pcValue != NULL );

	m_cSectorSize.SetFloatX( pcValue->GetString().ConvertToFloat32() );

	pcSizeY = pcSize->FindChild( CNavStringTable::Y );
	Assert( pcSizeY != NULL );

	pcValue = pcSizeY->FindChild();
	Assert( pcValue != NULL );

	m_cSectorSize.SetFloatY( pcValue->GetString().ConvertToFloat32() );

	pcSizeZ = pcSize->FindChild( CNavStringTable::Z );
	Assert( pcSizeZ != NULL );

	pcValue = pcSizeZ->FindChild();
	Assert( pcValue != NULL );

	m_cSectorSize.SetFloatZ( pcValue->GetString().ConvertToFloat32() );


	// Get Overlap

	pcOverlap = pcSector->FindChild( CNavStringTable::OVERLAP );
	Assert( pcOverlap != NULL );

	pcOverlapX = pcOverlap->FindChild( CNavStringTable::X );
	Assert( pcOverlapX != NULL );

	pcValue = pcOverlapX->FindChild();
	Assert( pcValue != NULL );

	m_cOverlapSize.SetFloatX( pcValue->GetString().ConvertToFloat32() );

	pcOverlapY = pcOverlap->FindChild( CNavStringTable::Y );
	Assert( pcOverlapY != NULL );

	pcValue = pcOverlapY->FindChild();
	Assert( pcValue != NULL );

	m_cOverlapSize.SetFloatY( pcValue->GetString().ConvertToFloat32() );

	pcOverlapZ = pcOverlap->FindChild(CNavStringTable::Z );
	Assert( pcOverlapZ != NULL );

	pcValue = pcOverlapZ->FindChild();
	Assert( pcValue != NULL );

	m_cOverlapSize.SetFloatZ( pcValue->GetString().ConvertToFloat32() );

	return true;
}


//-----------------------------------------------------------------------------
// Name			: ParseZoneList
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CNavWorld::ParseZoneList( SNavEntityCreateInfo & rsCreateInfo, CFixedStringTreeNodeW * pcZoneList )
{
	Assert( pcZoneList != NULL );
	
	CFixedStringTreeNodeW *	pcHeader;
	CFixedStringTreeNodeW *	pcZoneListSize;
	CFixedStringTreeNodeW *	pcZone;
	CFixedStringTreeNodeW *	pcUniqueId;
	CFixedStringTreeNodeW *	pcName;
	CFixedStringTreeNodeW *	pcTitle;
	CFixedStringTreeNodeW *	pcFlags;
	CFixedStringTreeNodeW *	pcValue;


	// Get Zone List Size

	pcHeader = pcZoneList->FindChild( CNavStringTable::HEADER );
	Assert( pcHeader != NULL );

	pcZoneListSize = pcHeader->FindChild( CNavStringTable::SIZE );
	Assert( pcZoneListSize != NULL );

	pcValue = pcZoneListSize->FindChild();
	Assert( pcValue != NULL );

	UInt32	nZoneListSize = pcValue->GetString().ConvertToInt32();

	// Create Zone List

	m_pcZones = new CNavEntityVector( true );
	Assert( m_pcZones != NULL );

	if( m_pcZones->Create( nZoneListSize ) == false )
	{
		return false;
	}


	// Create Zones

	SNavEntityCreateInfo	sCreateInfo( rsCreateInfo );

	for( UInt32 nZoneIndex = 0; nZoneIndex < nZoneListSize; ++nZoneIndex )
	{
		pcZone = pcZoneList->FindChild( CNavStringTable::ZONE, nZoneIndex );
		if( pcZone == NULL )
		{
			continue;
		}

		// Get Unique Id

		pcUniqueId = pcZone->FindChild( CNavStringTable::UID );
		Assert( pcUniqueId != NULL );

		pcValue = pcUniqueId->FindChild();
		Assert( pcValue != NULL );

		sCreateInfo.nUniqueId = static_cast<UId32>(pcValue->GetString().ConvertToInt32());

		// Get Name

		pcName = pcZone->FindChild( CNavStringTable::NAME );
		Assert( pcName != NULL );

		pcValue = pcName->FindChild();
		Assert( pcValue != NULL );

		sCreateInfo.cName = pcValue->GetString();

		// Get Title

		pcTitle = pcZone->FindChild( CNavStringTable::TITLE );
		Assert( pcTitle != NULL );

		pcValue = pcTitle->FindChild();
		Assert( pcValue != NULL );

		sCreateInfo.cTitle = pcValue->GetString();

		// Get Flags

		pcFlags = pcZone->FindChild( CNavStringTable::FLAGS );
		Assert( pcFlags != NULL );

		pcValue = pcFlags->FindChild();
		Assert( pcValue != NULL );

		// Set Create Info

		sCreateInfo.eEntityType			= E_NAV_ENTITY_TYPE_ZONE;
		sCreateInfo.eCreateType			= E_NAV_CREATE_TYPE_FILE;
		sCreateInfo.uSubType.eZoneType	= E_NAV_ZONE_TYPE_NONE;
		sCreateInfo.pvUserData			= NULL;
		sCreateInfo.nUserDataSize		= 0;
		sCreateInfo.nFlags				= MakeFlags( rsCreateInfo.nFlags, pcFlags->GetString().ConvertToInt32() );
		sCreateInfo.pcParent			= this;

		sCreateInfo.cBounding.Invalidate();
		sCreateInfo.cPath.Format( _T( "%s\\%s" ),
									rsCreateInfo.cPath.GetBuffer(),
									rsCreateInfo.cName.GetBuffer() );

		// Create Zone Entity

		if( CNavEntityFactory::GetInstance()->CreateEntity( sCreateInfo ) == NULL )
		{
			return false;
		}
	}

	return true;
}


//*****************************************************************************
//
// End of File : CNavWorld_Parse.cpp
//
//*****************************************************************************
