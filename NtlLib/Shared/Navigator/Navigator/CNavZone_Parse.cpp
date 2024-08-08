//*****************************************************************************
// File       : CNavZone_Parse.cpp
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
#include "CNavSector.h"

#include "CNavZone.h"


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

bool CNavZone::CreateFromFile( SNavEntityCreateInfo & rsCreateInfo )
{
	CFixedString	cFullFileName;
	cFullFileName.Format( _T( "%s\\%s.xml" ),
						rsCreateInfo.cPath.GetBuffer(),
						rsCreateInfo.cName.GetBuffer() );

	CXMLParserW		cXMLParser;
	if( cXMLParser.Create( cFullFileName ) == false )
	{
		Assert( !"Fail to Open Zone Script File" );

		return false;
	}


	// Block List

	CFixedStringTreeNodeW *	pcRoot;
	CFixedStringTreeNodeW *	pcZone;
	CFixedStringTreeNodeW *	pcHeader;
	CFixedStringTreeNodeW *	pcBound;
	CFixedStringTreeNodeW *	pcTerrain;
	CFixedStringTreeNodeW *	pcSectionList;
	CFixedStringTreeNodeW *	pcAreaList;
	CFixedStringTreeNodeW *	pcPortalList;
	CFixedStringTreeNodeW *	pcSectorList;


	// Get Root

	pcRoot = cXMLParser.GetRoot();
	Assert( pcRoot != NULL );

	pcZone = pcRoot->FindChild( CNavStringTable::ZONE );
	Assert( pcZone != NULL );


	// Parse Header

	pcHeader = pcZone->FindChild( CNavStringTable::HEADER );
	Assert( pcHeader != NULL );

	if( ParseHeader( pcHeader ) == false )
	{
		return false;
	}


	// Parse Bound

	pcBound = pcZone->FindChild( CNavStringTable::BOUND );
	Assert( pcBound != NULL );

	if( ParseBound( pcBound ) == false )
	{
		return false;
	}


	// Parse Terrain

	pcTerrain = pcZone->FindChild( CNavStringTable::TERRAIN );
	Assert( pcTerrain != NULL );

	if( ParseTerrain( rsCreateInfo, pcTerrain ) == false )
	{
		return false;
	}


	// Parse Section List

	pcSectionList = pcZone->FindChild( CNavStringTable::SECTIONLIST );
	Assert( pcSectionList != NULL );

	if( ParseSectionList( rsCreateInfo, pcSectionList ) == false )
	{
		return false;
	}


	// Parse Area List

	pcAreaList = pcZone->FindChild( CNavStringTable::AREALIST );
	Assert( pcAreaList != NULL );

	if( ParseAreaList( rsCreateInfo, pcAreaList ) == false )
	{
		return false;
	}


	// Parse Portal List

	pcPortalList = pcZone->FindChild( CNavStringTable::PORTALLIST );
	Assert( pcPortalList != NULL );

	if( ParsePortalList( rsCreateInfo, pcPortalList ) == false )
	{
		return false;
	}


	// Parse Sector List

	pcSectorList = pcZone->FindChild( CNavStringTable::SECTORLIST );
	Assert( pcSectorList != NULL );

	if( ParseSectorList( rsCreateInfo, pcSectorList ) == false )
	{
		return false;
	}

	return true;
}


//-----------------------------------------------------------------------------
// Name			: ImportFromFile
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CNavZone::ImportFromFile( SNavEntityCreateInfo & rsCreateInfo )
{
	CFixedString	cFullFileName;
	cFullFileName.Format( _T( "%s\\%s.xml" ),
						rsCreateInfo.cPath.GetBuffer(),
						rsCreateInfo.cName.GetBuffer() );

	CXMLParserW		cXMLParser;
	if( cXMLParser.Create( cFullFileName ) == false )
	{
		Assert( !"Fail to Open Zone Script File" );

		return false;
	}

//	cXMLParser.Report( _T( "Parse.txt" ) );


	// Block List

	CFixedStringTreeNodeW *	pcRoot;
	CFixedStringTreeNodeW *	pcZone;
	CFixedStringTreeNodeW *	pcHeader;
	CFixedStringTreeNodeW *	pcBound;
	CFixedStringTreeNodeW *	pcTerrain;
	CFixedStringTreeNodeW *	pcSectionList;
	CFixedStringTreeNodeW *	pcAreaList;
	CFixedStringTreeNodeW *	pcPortalList;
	CFixedStringTreeNodeW *	pcSectorList;


	// Get Root

	pcRoot = cXMLParser.GetRoot();
	Assert( pcRoot != NULL );

	pcZone = pcRoot->FindChild( CNavStringTable::ZONE );
	Assert( pcZone != NULL );


	// Parse Header

	pcHeader = pcZone->FindChild( CNavStringTable::HEADER );
	Assert( pcHeader != NULL );

	if( ParseHeader( pcHeader ) == false )
	{
		return false;
	}


	// Parse Bound

	pcBound = pcZone->FindChild( CNavStringTable::BOUND );
	Assert( pcBound != NULL );

	if( ParseBound( pcBound ) == false )
	{
		return false;
	}


	// Parse Section List

	pcSectionList = pcZone->FindChild( CNavStringTable::SECTIONLIST );
	Assert( pcSectionList != NULL );

	if( ParseSectionList( rsCreateInfo, pcSectionList ) == false )
	{
		return false;
	}

	// Parse Area List

	pcAreaList = pcZone->FindChild( CNavStringTable::AREALIST );
	Assert( pcAreaList != NULL );

	if( ParseAreaList( rsCreateInfo, pcAreaList ) == false )
	{
		return false;
	}


	// Parse Portal List

	pcPortalList = pcZone->FindChild( CNavStringTable::PORTALLIST );
	Assert( pcPortalList != NULL );

	if( ParsePortalList( rsCreateInfo, pcPortalList ) == false )
	{
		return false;
	}


	// Parse Sector List

	pcSectorList = pcZone->FindChild( CNavStringTable::SECTORLIST );
	Assert( pcSectorList != NULL );

	if( ParseSectorList( rsCreateInfo, pcSectorList ) == false )
	{
		return false;
	}


	// Parse Terrain

	pcTerrain = pcZone->FindChild( CNavStringTable::TERRAIN );
	Assert( pcTerrain != NULL );

	if( ParseTerrain( rsCreateInfo, pcTerrain ) == false )
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

bool CNavZone::ParseHeader( CFixedStringTreeNodeW * pcHeader )
{
	Assert( pcHeader != NULL );

	CFixedStringTreeNodeW *	pcValue;


	// Get Name

	CFixedStringTreeNodeW *	pcName;

	pcName = pcHeader->FindChild( CNavStringTable::NAME );
	Assert( pcName != NULL );

	pcValue = pcName->FindChild();
	Assert( pcValue != NULL );

	CFixedString	cName( pcValue->GetString() );
	Assert( m_cName == cName );


	// Get Complexity

	CFixedStringTreeNodeW *	pcComplexity;

	pcComplexity = pcHeader->FindChild( CNavStringTable::COMPLEXITY );
	if( pcComplexity != NULL )
	{
		pcValue = pcComplexity->FindChild();
		Assert( pcValue != NULL );

		SetComplexity( pcValue->GetString().ConvertToFloat32() );
	}

	return true;
}


//-----------------------------------------------------------------------------
// Name			: ParseBound
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CNavZone::ParseBound( CFixedStringTreeNodeW * pcBound )
{
	Assert( pcBound != NULL );

	CFixedStringTreeNodeW *		pcBoundMin;
	CFixedStringTreeNodeW *		pcBoundMinX;
	CFixedStringTreeNodeW *		pcBoundMinY;
	CFixedStringTreeNodeW *		pcBoundMinZ;
	CFixedStringTreeNodeW *		pcBoundMax;
	CFixedStringTreeNodeW *		pcBoundMaxX;
	CFixedStringTreeNodeW *		pcBoundMaxY;
	CFixedStringTreeNodeW *		pcBoundMaxZ;
	CFixedStringTreeNodeW *		pcValue;


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
// Name			: ParseTerrain
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CNavZone::ParseTerrain( SNavEntityCreateInfo & rsCreateInfo, CFixedStringTreeNodeW * pcTerrain )
{
	Assert( pcTerrain != NULL );

	CFixedStringTreeNodeW *	pcMesh;
	CFixedStringTreeNodeW *	pcPriority;
	CFixedStringTreeNodeW *	pcValue;


	// Get Mesh File Name

	pcMesh = pcTerrain->FindChild( CNavStringTable::MESH );
	Assert( pcMesh != NULL );

	pcValue = pcMesh->FindChild();
	Assert( pcValue != NULL );

	CFixedString	cMeshFileName( pcValue->GetString() );


	// Get Mesh Load Priority

	pcPriority = pcTerrain->FindChild( CNavStringTable::PRIORITY );
	Assert( pcPriority != NULL );

	pcValue = pcPriority->FindChild();
	Assert( pcValue != NULL );

	Int32	nPriority = pcValue->GetString().ConvertToInt32();


	// Create Terrain

	SNavEntityCreateInfo	sCreateInfo( rsCreateInfo );

	sCreateInfo.nUniqueId		= ( E_NAV_ENTITY_TYPE_TERRAIN << 24 ) |
									( GetUniqueId() & 0x00FFFFFF );
	sCreateInfo.eEntityType		= E_NAV_ENTITY_TYPE_TERRAIN;

	sCreateInfo.eCreateType		= E_NAV_CREATE_TYPE_FILE;

	sCreateInfo.pvUserData		= NULL;
	sCreateInfo.nUserDataSize	= 0;
	sCreateInfo.cName			= cMeshFileName;
	sCreateInfo.pcParent		= this;
	sCreateInfo.nFlags			= MakeFlags( rsCreateInfo.nFlags, nPriority );
	sCreateInfo.pcThreadManager	= rsCreateInfo.pcThreadManager;

	sCreateInfo.cPath.Format( _T( "%s\\%s" ),
								rsCreateInfo.cPath.GetBuffer(),
								rsCreateInfo.cName.GetBuffer() );

	m_pcTerrain = down_cast<CNavTerrain *>(CNavEntityFactory::GetInstance()->CreateEntity( sCreateInfo ));
	Assert( m_pcTerrain != NULL );

	return true;
}


//-----------------------------------------------------------------------------
// Name			: ParseSectionList
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CNavZone::ParseSectionList( SNavEntityCreateInfo & rsCreateInfo, CFixedStringTreeNodeW * pcSectionList )
{
	Assert( pcSectionList != NULL );

	CFixedStringTreeNodeW *	pcHeader;
	CFixedStringTreeNodeW *	pcSectionListSize;
	CFixedStringTreeNodeW *	pcValue;
	CFixedStringTreeNodeW *	pcSection;
	CFixedStringTreeNodeW *	pcUniqueId;
	CFixedStringTreeNodeW *	pcName;
	CFixedStringTreeNodeW *	pcTitle;
	CFixedStringTreeNodeW *	pcType;
	CFixedStringTreeNodeW *	pcArea;
	CFixedStringTreeNodeW *	pcSector;
	CFixedStringTreeNodeW *	pcFlags;


	// Get Section List Size

	pcHeader = pcSectionList->FindChild( CNavStringTable::HEADER );
	Assert( pcHeader != NULL );

	pcSectionListSize = pcHeader->FindChild( CNavStringTable::SIZE );
	Assert( pcSectionListSize != NULL );

	pcValue = pcSectionListSize->FindChild();
	Assert( pcValue != NULL );

	UInt32	nSectionListSize = pcValue->GetString().ConvertToInt32();


	// Create Section

	SNavEntityCreateInfo		sCreateInfo( rsCreateInfo );

	sCreateInfo.eEntityType		= E_NAV_ENTITY_TYPE_SECTION;
	sCreateInfo.eCreateType		= E_NAV_CREATE_TYPE_EMPTY;
	sCreateInfo.pvUserData		= NULL;
	sCreateInfo.nUserDataSize	= 0;
	sCreateInfo.pcParent		= this;

	for( UInt32 nSectionIndex = 0; nSectionIndex < nSectionListSize; ++nSectionIndex )
	{
		pcSection = pcSectionList->FindChild( CNavStringTable::SECTION, nSectionIndex );
		if( pcSection == NULL )
		{
			continue;
		}


		// Get Unique Id

		pcUniqueId = pcSection->FindChild( CNavStringTable::UID );
		Assert( pcUniqueId != NULL );

		pcValue = pcUniqueId->FindChild();
		Assert( pcValue != NULL );

		sCreateInfo.nUniqueId = static_cast<UId32>(pcValue->GetString().ConvertToInt32());


		// Get Name

		pcName = pcSection->FindChild( CNavStringTable::NAME );
		Assert( pcName != NULL );

		pcValue = pcName->FindChild();
		Assert( pcValue != NULL );

		sCreateInfo.cName = pcValue->GetString();

		
		// Get Title

		pcTitle = pcSection->FindChild( CNavStringTable::TITLE );
		Assert( pcTitle != NULL );

		pcValue = pcTitle->FindChild();
		Assert( pcValue != NULL );

		sCreateInfo.cTitle = pcValue->GetString();


		// Get Type

		pcType = pcSection->FindChild( CNavStringTable::TYPE );
		Assert( pcType != NULL );

		pcValue = pcType->FindChild();
		Assert( pcValue != NULL );

		if( pcValue->GetString().IsEqual( CNavStringTable::FIELD ) == true )
		{
			sCreateInfo.uSubType.eSectionType	= E_NAV_SECTOR_TYPE_FIELD;
		}
		else if( pcValue->GetString().IsEqual( CNavStringTable::TOWN ) == true )
		{
			sCreateInfo.uSubType.eSectionType	= E_NAV_SECTOR_TYPE_TOWN;
		}
		else if( pcValue->GetString().IsEqual( CNavStringTable::DUNGEON ) == true )
		{
			sCreateInfo.uSubType.eSectionType	= E_NAV_SECTOR_TYPE_DUNGEON;
		}
		else
		{
			sCreateInfo.uSubType.eSectionType	= E_NAV_SECTOR_TYPE_NONE;
		}


		// Get Area

		pcArea = pcSection->FindChild( CNavStringTable::AREA );
		Assert( pcArea != NULL );


		// Get Sector

		pcSector = pcSection->FindChild( CNavStringTable::SECTOR );
		Assert( pcSector != NULL );


		// Get Flags

		pcFlags = pcSection->FindChild( CNavStringTable::FLAGS );
		if( pcFlags != NULL )
		{
			sCreateInfo.nFlags = MakeFlags( rsCreateInfo.nFlags, pcFlags->GetString().ConvertToInt32() );
		}
		else
		{
			sCreateInfo.nFlags = rsCreateInfo.nFlags;
		}


		// Create Sector Entity
		
		if( CNavEntityFactory::GetInstance()->CreateEntity( sCreateInfo ) == NULL )
		{
			return false;
		}
	}

	return true;
}


//-----------------------------------------------------------------------------
// Name			: ParseAreaList
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CNavZone::ParseAreaList( SNavEntityCreateInfo & rsCreateInfo, CFixedStringTreeNodeW * pcAreaList )
{
	Assert( pcAreaList != NULL );

	CFixedStringTreeNodeW *	pcHeader;
	CFixedStringTreeNodeW *	pcAreaListSize;
	CFixedStringTreeNodeW *	pcValue;
	CFixedStringTreeNodeW *	pcArea;
	CFixedStringTreeNodeW *	pcUniqueId;
	CFixedStringTreeNodeW *	pcName;
	CFixedStringTreeNodeW *	pcTitle;
	CFixedStringTreeNodeW *	pcType;
	CFixedStringTreeNodeW *	pcSection;
	CFixedStringTreeNodeW *	pcSector;
	CFixedStringTreeNodeW *	pcFlags;


	// Get Section List Size

	pcHeader = pcAreaList->FindChild( CNavStringTable::HEADER );
	Assert( pcHeader != NULL );

	pcAreaListSize = pcHeader->FindChild( CNavStringTable::SIZE );
	Assert( pcAreaListSize != NULL );

	pcValue = pcAreaListSize->FindChild();
	Assert( pcValue != NULL );

	UInt32	nAreaListSize = pcValue->GetString().ConvertToInt32();


	// Create Area

	SNavEntityCreateInfo		sCreateInfo( rsCreateInfo );

	sCreateInfo.eEntityType		= E_NAV_ENTITY_TYPE_AREA;
	sCreateInfo.eCreateType		= E_NAV_CREATE_TYPE_EMPTY;
	sCreateInfo.pvUserData		= NULL;
	sCreateInfo.nUserDataSize	= 0;
	sCreateInfo.pcParent		= this;

	for( UInt32 nAreaIndex = 0; nAreaIndex < nAreaListSize; ++nAreaIndex )
	{
		pcArea = pcAreaList->FindChild( CNavStringTable::AREA, nAreaIndex );
		if( pcArea == NULL )
		{
			continue;
		}


		// Get Unique Id

		pcUniqueId = pcArea->FindChild( CNavStringTable::UID );
		Assert( pcUniqueId != NULL );

		pcValue = pcUniqueId->FindChild();
		Assert( pcValue != NULL );

		sCreateInfo.nUniqueId = static_cast<UId32>(pcValue->GetString().ConvertToInt32());


		// Get Name

		pcName = pcArea->FindChild( CNavStringTable::NAME );
		Assert( pcName != NULL );

		pcValue = pcName->FindChild();
		Assert( pcValue != NULL );

		sCreateInfo.cName = pcValue->GetString();

		
		// Get Title

		pcTitle = pcArea->FindChild( CNavStringTable::TITLE );
		Assert( pcTitle != NULL );

		pcValue = pcTitle->FindChild();
		Assert( pcValue != NULL );

		sCreateInfo.cTitle = pcValue->GetString();


		// Get Type

		pcType = pcArea->FindChild( CNavStringTable::TYPE );
		Assert( pcType != NULL );

		pcValue = pcType->FindChild();
		Assert( pcValue != NULL );

		if( pcValue->GetString().IsEqual( CNavStringTable::FIELD ) == true )
		{
			sCreateInfo.uSubType.eAreaType	= E_NAV_SECTOR_TYPE_FIELD;
		}
		else if( pcValue->GetString().IsEqual( CNavStringTable::TOWN ) == true )
		{
			sCreateInfo.uSubType.eAreaType	= E_NAV_SECTOR_TYPE_TOWN;
		}
		else if( pcValue->GetString().IsEqual( CNavStringTable::DUNGEON ) == true )
		{
			sCreateInfo.uSubType.eAreaType	= E_NAV_SECTOR_TYPE_DUNGEON;
		}
		else
		{
			sCreateInfo.uSubType.eAreaType	= E_NAV_SECTOR_TYPE_NONE;
		}


		// Get Section

		pcSection = pcArea->FindChild( CNavStringTable::SECTION );
		Assert( pcSection != NULL );


		// Get Sector

		pcSector = pcArea->FindChild( CNavStringTable::SECTOR );
		Assert( pcSector != NULL );


		// Get Flags

		pcFlags = pcSection->FindChild( CNavStringTable::FLAGS );
		if( pcFlags != NULL )
		{
			sCreateInfo.nFlags = MakeFlags( rsCreateInfo.nFlags, pcFlags->GetString().ConvertToInt32() );
		}
		else
		{
			sCreateInfo.nFlags = rsCreateInfo.nFlags;
		}


		// Create Sector Entity
		
		if( CNavEntityFactory::GetInstance()->CreateEntity( sCreateInfo ) == NULL )
		{
			return false;
		}
	}

	return true;
}


//-----------------------------------------------------------------------------
// Name			: ParsePortalList
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CNavZone::ParsePortalList( SNavEntityCreateInfo & rsCreateInfo, CFixedStringTreeNodeW * pcPortalList )
{
	Assert( pcPortalList != NULL );

	CFixedStringTreeNodeW *	pcHeader;
	CFixedStringTreeNodeW *	pcPortalListSize;
	CFixedStringTreeNodeW *	pcValue;
	CFixedStringTreeNodeW *	pcPortal;
	CFixedStringTreeNodeW *	pcUniqueId;
	CFixedStringTreeNodeW *	pcName;
	CFixedStringTreeNodeW *	pcTitle;
	CFixedStringTreeNodeW *	pcType;
	CFixedStringTreeNodeW *	pcSector;
	CFixedStringTreeNodeW *	pcLink;
	CFixedStringTreeNodeW *	pcFlags;


	// Get Section List Size

	pcHeader = pcPortalList->FindChild( CNavStringTable::HEADER );
	Assert( pcHeader != NULL );

	pcPortalListSize = pcHeader->FindChild( CNavStringTable::SIZE );
	Assert( pcPortalListSize != NULL );

	pcValue = pcPortalListSize->FindChild();
	Assert( pcValue != NULL );

	UInt32	nPortalListSize = pcValue->GetString().ConvertToInt32();


	// Create Area

	SNavEntityCreateInfo		sCreateInfo( rsCreateInfo );

	sCreateInfo.eEntityType		= E_NAV_ENTITY_TYPE_PORTAL;
	sCreateInfo.eCreateType		= E_NAV_CREATE_TYPE_EMPTY;
	sCreateInfo.pvUserData		= NULL;
	sCreateInfo.nUserDataSize	= 0;
	sCreateInfo.pcParent		= this;

	for( UInt32 nPortalIndex = 0; nPortalIndex < nPortalListSize; ++nPortalIndex )
	{
		pcPortal = pcPortalList->FindChild( CNavStringTable::PORTAL, nPortalIndex );
		if( pcPortal == NULL )
		{
			continue;
		}


		// Get Unique Id

		pcUniqueId = pcPortal->FindChild( CNavStringTable::UID );
		Assert( pcUniqueId != NULL );

		pcValue = pcUniqueId->FindChild();
		Assert( pcValue != NULL );

		sCreateInfo.nUniqueId = static_cast<UId32>(pcValue->GetString().ConvertToInt32());


		// Get Name

		pcName = pcPortal->FindChild( CNavStringTable::NAME );
		Assert( pcName != NULL );

		pcValue = pcName->FindChild();
		Assert( pcValue != NULL );

		sCreateInfo.cName = pcValue->GetString();

		
		// Get Title

		pcTitle = pcPortal->FindChild( CNavStringTable::TITLE );
		Assert( pcTitle != NULL );

		pcValue = pcTitle->FindChild();
		Assert( pcValue != NULL );

		sCreateInfo.cTitle = pcValue->GetString();


		// Get Type

		pcType = pcPortal->FindChild( CNavStringTable::TYPE );
		Assert( pcType != NULL );

		pcValue = pcType->FindChild();
		Assert( pcValue != NULL );

		if( pcValue->GetString().IsEqual( CNavStringTable::FIELD ) == true )
		{
			sCreateInfo.uSubType.ePortalType	= E_NAV_PORTAL_TYPE_FIELD;
		}
		else if( pcValue->GetString().IsEqual( CNavStringTable::DUNGEON ) == true )
		{
			sCreateInfo.uSubType.ePortalType	= E_NAV_PORTAL_TYPE_DUNGEON;
		}
		else
		{
			sCreateInfo.uSubType.ePortalType	= E_NAV_PORTAL_TYPE_NONE;
		}


		// Get Sector

		pcSector = pcPortal->FindChild( CNavStringTable::SECTOR );
		Assert( pcSector != NULL );


		// Get Link

		pcLink = pcPortal->FindChild( CNavStringTable::LINK );
		Assert( pcLink != NULL );


		// Get Flags

		pcFlags = pcPortal->FindChild( CNavStringTable::FLAGS );
		if( pcFlags != NULL )
		{
			sCreateInfo.nFlags = MakeFlags( rsCreateInfo.nFlags, pcFlags->GetString().ConvertToInt32() );
		}
		else
		{
			sCreateInfo.nFlags = rsCreateInfo.nFlags;
		}


		// Create Sector Entity
		
		if( CNavEntityFactory::GetInstance()->CreateEntity( sCreateInfo ) == NULL )
		{
			return false;
		}
	}

	return true;
}


//-----------------------------------------------------------------------------
// Name			: ParseSectorList
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CNavZone::ParseSectorList( SNavEntityCreateInfo & rsCreateInfo, CFixedStringTreeNodeW * pcSectorList )
{
	Assert( pcSectorList != NULL );

	CFixedStringTreeNodeW *	pcHeader;
	CFixedStringTreeNodeW *	pcSectorListSize;
	CFixedStringTreeNodeW *	pcValue;
	CFixedStringTreeNodeW *	pcSection;
	CFixedStringTreeNodeW *	pcArea;
	CFixedStringTreeNodeW *	pcSector;
	CFixedStringTreeNodeW *	pcUniqueId;
	CFixedStringTreeNodeW *	pcName;
	CFixedStringTreeNodeW *	pcType;
	CFixedStringTreeNodeW *	pcFlags;
	CFixedStringTreeNodeW *	pcX;
	CFixedStringTreeNodeW *	pcY;


	// Get Sector List Size

	pcHeader = pcSectorList->FindChild( CNavStringTable::HEADER );
	Assert( pcHeader != NULL );

	pcSectorListSize = pcHeader->FindChild( CNavStringTable::SIZE );
	Assert( pcSectorListSize != NULL );

	pcValue = pcSectorListSize->FindChild();
	Assert( pcValue != NULL );

	UInt32	nSectorListSize = pcValue->GetString().ConvertToInt32();

	m_pcSectors = new CNavEntityVector( true );
	if( m_pcSectors->Create( nSectorListSize ) == false )
	{
		Assert( !"m_pcSectors Create Fail" );

		return false;
	}


	// Get World Info To Create Sector

	CNavWorld *		pcWorld = down_cast<CNavWorld *>(rsCreateInfo.pcParent);
	Assert( pcWorld != NULL );

	SNavSectorCreateInfo	sSectorCreateInfo;
	sSectorCreateInfo.cWorldBound	= pcWorld->GetBounding();
	sSectorCreateInfo.cSectorSize	= pcWorld->GetSectorSize();
	sSectorCreateInfo.sCellPosition.SetSize( pcWorld->GetSectorCount() );


	// Get Sector File Name Prefix

	CFixedStringTreeNodeW *	pcPrefix = pcHeader->FindChild( CNavStringTable::PREFIX );
	Assert( pcPrefix != NULL );

	pcValue = pcPrefix->FindChild();
	Assert( pcValue != NULL );
	sSectorCreateInfo.cPrefix = pcValue->GetString();


	// Get Sector File Name Postfix

	CFixedStringTreeNodeW *	pcPostfix = pcHeader->FindChild( CNavStringTable::POSTFIX );
	Assert( pcPostfix != NULL );

	pcValue = pcPostfix->FindChild();
	Assert( pcValue != NULL );
	sSectorCreateInfo.cPostfix = pcValue->GetString();


	// Get Sector File Name Separator

	CFixedStringTreeNodeW *	pcSeparator = pcHeader->FindChild( CNavStringTable::SEPARATOR );
	Assert( pcSeparator != NULL );

	pcValue = pcSeparator->FindChild();
	Assert( pcValue != NULL );
	sSectorCreateInfo.cSeparator = pcValue->GetString();


	// Create Sectors

	SNavEntityCreateInfo		sCreateInfo( rsCreateInfo );

	sCreateInfo.eEntityType		= E_NAV_ENTITY_TYPE_SECTOR;
	sCreateInfo.eCreateType		= E_NAV_CREATE_TYPE_EMPTY;
	sCreateInfo.pvUserData		= &sSectorCreateInfo;
	sCreateInfo.nUserDataSize	= sizeof( SNavSectorCreateInfo );
	sCreateInfo.pcParent		= this;
	sCreateInfo.nFlags			= MakeFlags( rsCreateInfo.nFlags );
	sCreateInfo.cPath.Format( _T( "%s\\Sectors" ),
							rsCreateInfo.cPath.GetBuffer() );

	CNavBounding	cCellOffsetBound;

	for( UInt32 nSectorIndex = 0; nSectorIndex < nSectorListSize; ++nSectorIndex )
	{
		pcSector = pcSectorList->FindChild( CNavStringTable::SECTOR, nSectorIndex );
		if( pcSector == NULL )
		{
			continue;
		}


		// Get Unique Id

		pcUniqueId = pcSector->FindChild( CNavStringTable::UID );
		Assert( pcUniqueId != NULL );

		pcValue = pcUniqueId->FindChild();
		Assert( pcValue != NULL );

		sCreateInfo.nUniqueId = static_cast<UId32>(pcValue->GetString().ConvertToInt32());


		// Get Name

		pcName = pcSector->FindChild( CNavStringTable::NAME );
		Assert( pcName != NULL );

		pcValue = pcName->FindChild();
		Assert( pcValue != NULL );

		sCreateInfo.cName = pcValue->GetString();


		// Get Type

		pcType = pcSector->FindChild( CNavStringTable::TYPE );
		Assert( pcType != NULL );

		pcValue = pcType->FindChild();
		Assert( pcValue != NULL );

		if( pcValue->GetString().IsEqual( CNavStringTable::FIELD ) == true )
		{
			sCreateInfo.uSubType.eSectorType	= E_NAV_SECTOR_TYPE_FIELD;
		}
		else if( pcValue->GetString().IsEqual( CNavStringTable::TOWN ) == true )
		{
			sCreateInfo.uSubType.eSectorType	= E_NAV_SECTOR_TYPE_TOWN;
		}
		else if( pcValue->GetString().IsEqual( CNavStringTable::DUNGEON ) == true )
		{
			sCreateInfo.uSubType.eSectorType	= E_NAV_SECTOR_TYPE_DUNGEON;
		}
		else
		{
			sCreateInfo.uSubType.eSectorType	= E_NAV_SECTOR_TYPE_NONE;
		}


		// Get X & Y Position

		pcX = pcSector->FindChild( CNavStringTable::X );
		Assert( pcX != NULL );

		pcValue = pcX->FindChild();
		Assert( pcValue != NULL );

		sSectorCreateInfo.sCellPosition.SetIndexX( pcValue->GetString().ConvertToInt32() );

		pcY = pcSector->FindChild( CNavStringTable::Y );
		Assert( pcY != NULL );

		pcValue = pcY->FindChild();
		Assert( pcValue != NULL );

		sSectorCreateInfo.sCellPosition.SetIndexY( pcValue->GetString().ConvertToInt32() );


		// Set Bound

		sCreateInfo.cBounding.SetMinX( pcWorld->GetBounding().GetMin().GetX() + sSectorCreateInfo.sCellPosition.GetIndexX() * pcWorld->GetSectorSize().GetX() );
		sCreateInfo.cBounding.SetMinY( pcWorld->GetBounding().GetMin().GetY() + sSectorCreateInfo.sCellPosition.GetIndexY() * pcWorld->GetSectorSize().GetY() );
		sCreateInfo.cBounding.SetMinZ( pcWorld->GetBounding().GetMin().GetZ() );

		sCreateInfo.cBounding.SetMaxX( sCreateInfo.cBounding.GetMin().GetX() + pcWorld->GetSectorSize().GetX() );
		sCreateInfo.cBounding.SetMaxY( sCreateInfo.cBounding.GetMin().GetY() + pcWorld->GetSectorSize().GetY() );
		sCreateInfo.cBounding.SetMaxZ( pcWorld->GetBounding().GetMax().GetZ() );


		// Get Section

		pcSection = pcSector->FindChild( CNavStringTable::SECTION );
		Assert( pcSection != NULL );

		pcValue = pcSection->FindChild();
		Assert( pcValue != NULL );

		sSectorCreateInfo.nSectionOrder = pcValue->GetString().ConvertToInt32();


		// Get Area

		pcArea = pcSector->FindChild( CNavStringTable::AREA );
		Assert( pcArea != NULL );

		pcValue = pcArea->FindChild();
		Assert( pcValue != NULL );

		sSectorCreateInfo.nAreaOrder = pcValue->GetString().ConvertToInt32();


		// Get Flags

		pcFlags = pcSector->FindChild( CNavStringTable::FLAGS );
		Assert( pcFlags != NULL );

		sCreateInfo.nFlags = MakeFlags( rsCreateInfo.nFlags, pcFlags->GetString().ConvertToInt32() );

		pcValue = pcFlags->FindChild();
		Assert( pcValue != NULL );


		// Create Sector Entity
		
		if( CNavEntityFactory::GetInstance()->CreateEntity( sCreateInfo ) == NULL )
		{
			return false;
		}
	}

	return true;
}


//*****************************************************************************
//
// End of File : CNavZone_Parse.cpp
//
//*****************************************************************************