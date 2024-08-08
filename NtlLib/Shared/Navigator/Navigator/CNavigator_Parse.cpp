//*****************************************************************************
// File       : CNavigator_Parse.cpp
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

#include "CNavigator.h"


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

bool CNavigator::CreateFromFile( SNavEntityCreateInfo & rsCreateInfo )
{
	CFixedString	cFullFileName;
	cFullFileName.Format( _T( "%s\\%s.xml" ),
							rsCreateInfo.cPath.GetBuffer(),
							rsCreateInfo.cName.GetBuffer() );

	CXMLParserW		cXMLParser;
	if( cXMLParser.Create( cFullFileName ) == false )
	{
		Assert( !"Fail to Open Navigator Script File" );

		return false;
	}


	// Keyword List

	CFixedStringTreeNodeW *		pcRoot;
	CFixedStringTreeNodeW *		pcSession;
	CFixedStringTreeNodeW *		pcHeader;
	CFixedStringTreeNodeW *		pcShape;
	CFixedStringTreeNodeW *		pcThread;
	CFixedStringTreeNodeW *		pcWorldList;


	// Get Root

	pcRoot = cXMLParser.GetRoot();
	Assert( pcRoot != NULL );

	pcSession = pcRoot->FindChild( CNavStringTable::SESSION );
	Assert( pcSession != NULL );


	// Parse Header

	pcHeader = pcSession->FindChild( CNavStringTable::HEADER );
	Assert( pcHeader != NULL );

	if( ParseHeader( pcHeader ) == false )
	{
		return false;
	}


	// Parse Shape

	pcShape = pcSession->FindChild( CNavStringTable::SHAPE );
	Assert( pcShape != NULL );

	if( ParseShape( rsCreateInfo, pcShape ) == false )
	{
		return false;
	}


	// Parse Thread
	
	pcThread = pcSession->FindChild( CNavStringTable::THREAD );
	Assert( pcThread != NULL );

	if( ParseThread( pcThread ) == false )
	{
		return false;
	}


	// Create LoadMesh Thread Manager

	CBaseThreadManager *	pcThreadManager;

	if( m_nThreadSize > 0 )
	{
		pcThreadManager = new CBaseThreadManager;
		Assert( pcThreadManager != NULL );

		if( pcThreadManager->Create( m_nThreadSize ) == false )
		{
			Assert( !"Create Thread Manager Fail" );

			return false;
		}
	}
	else
	{
		pcThreadManager = NULL;
	}

	rsCreateInfo.pcThreadManager = pcThreadManager;


	// Set Thread Flags

	if( m_nThreadSize > 0 )
	{
		rsCreateInfo.nFlags |= E_NAV_ENTITY_CREATE_INFO_FLAG_MULTI_THREAD;
	}


	// Parse World List

	pcWorldList = pcSession->FindChild( CNavStringTable::WORLDLIST );
	Assert( pcWorldList != NULL );

	if( ParseWorldList( rsCreateInfo, pcWorldList ) == false )
	{
		return false;
	}


	// Run Thread

	bool	bResult = true;

	if( pcThreadManager != NULL )
	{
		bResult = pcThreadManager->Run();

		delete pcThreadManager;
	}

	return bResult;
}


//-----------------------------------------------------------------------------
// Name			: ParseHeader
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CNavigator::ParseHeader( CFixedStringTreeNodeW * pcHeader )
{
	Assert( pcHeader != NULL );

	CFixedStringTreeNodeW *		pcName;
	CFixedStringTreeNodeW *		pcValue;


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
// Name			: ParseShape
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CNavigator::ParseShape( SNavEntityCreateInfo & rsCreateInfo, CFixedStringTreeNodeW * pcShape )
{
	Assert( pcShape != NULL );

	CFixedStringTreeNodeW *	pcPath;
	CFixedStringTreeNodeW *	pcLocal;
	CFixedStringTreeNodeW *	pcValue;


	// Get Object Path

	pcPath = pcShape->FindChild( CNavStringTable::PATH );
	Assert( pcPath != NULL );

	pcValue = pcPath->FindChild();
	Assert( pcValue != NULL );

	CFixedString	cObjectPath( pcValue->GetString() );

	bool	bIsLocalPath;

	pcLocal = pcShape->FindChild( CNavStringTable::LOCAL );
	if( pcLocal != NULL )
	{
		pcValue = pcLocal->FindChild();
		Assert( pcValue != NULL );

		bIsLocalPath = pcValue->GetString().IsEqual( CNavStringTable::_TRUE );
	}
	else
	{
		bIsLocalPath = false;
	}

	if( bIsLocalPath == true )
	{
		cObjectPath.Format( _T( "%s\\%s\\%s" ),
							rsCreateInfo.cPath.GetBuffer(),
							rsCreateInfo.cName.GetBuffer(),
							cObjectPath.GetBuffer() );
	}

	m_cShapeManager.SetPathName( cObjectPath );

	return true;
}


//-----------------------------------------------------------------------------
// Name			: ParseThread
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CNavigator::ParseThread( CFixedStringTreeNodeW * pcThread )
{
	Assert( pcThread != NULL );

	CFixedStringTreeNodeW *	pcThreadSize;
	CFixedStringTreeNodeW *	pcValue;


	pcThreadSize = pcThread->FindChild( CNavStringTable::SIZE );
	Assert( pcThreadSize != NULL );

	pcValue = pcThreadSize->FindChild();
	Assert( pcValue != NULL );

	m_nThreadSize = pcValue->GetString().ConvertToInt32();

	return true;
}


//-----------------------------------------------------------------------------
// Name			: ParseWorldList
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CNavigator::ParseWorldList( SNavEntityCreateInfo & rsCreateInfo, CFixedStringTreeNodeW * pcWorldList )
{
	Assert( pcWorldList != NULL );

	CFixedStringTreeNodeW *		pcWorldListHeader;
	CFixedStringTreeNodeW *		pcWorldListSize;
	CFixedStringTreeNodeW *		pcWorld;
	CFixedStringTreeNodeW *		pcUniqueId;
	CFixedStringTreeNodeW *		pcWorldType;
	CFixedStringTreeNodeW *		pcName;
	CFixedStringTreeNodeW *		pcTitle;
	CFixedStringTreeNodeW *		pcFlags;
	CFixedStringTreeNodeW *		pcValue;


	// Get World List Size

	pcWorldListHeader = pcWorldList->FindChild( CNavStringTable::HEADER );
	Assert( pcWorldListHeader != NULL );

	pcWorldListSize = pcWorldListHeader->FindChild( CNavStringTable::SIZE );
	Assert( pcWorldListSize != NULL );

	pcValue = pcWorldListSize->FindChild();
	Assert( pcValue != NULL );

	UInt32	nWorldListSize = pcValue->GetString().ConvertToInt32();
	Assert( nWorldListSize != 0 );


	// Create World List

	Assert( m_pcWorlds == NULL );

	m_pcWorlds = new CNavEntityVector( true );
	Assert( m_pcWorlds != NULL );
	m_pcWorlds->Create( nWorldListSize );


	// Create Worlds

	SNavEntityCreateInfo	sCreateInfo( rsCreateInfo );

	for( UInt32 nWorldIndex = 0; nWorldIndex < nWorldListSize; ++nWorldIndex )
	{
		pcWorld = pcWorldList->FindChild( CNavStringTable::WORLD, nWorldIndex );
		if( pcWorld == NULL )
		{
			continue;
		}


		// Get Unique Id

		pcUniqueId = pcWorld->FindChild( CNavStringTable::UID );
		Assert( pcUniqueId != NULL );

		pcValue = pcUniqueId->FindChild();
		Assert( pcValue != NULL );

		sCreateInfo.nUniqueId = static_cast<UId32>(pcValue->GetString().ConvertToInt32());


		// Get Name

		pcName = pcWorld->FindChild( CNavStringTable::NAME );
		Assert( pcName != NULL );

		pcValue = pcName->FindChild();
		Assert( pcValue != NULL );

		sCreateInfo.cName = pcValue->GetString();


		// Get Title

		pcTitle = pcWorld->FindChild( CNavStringTable::TITLE );
		Assert( pcTitle != NULL );

		pcValue = pcTitle->FindChild();
		Assert( pcValue != NULL );

		sCreateInfo.cTitle = pcValue->GetString();


		// Get Type

		pcWorldType = pcWorld->FindChild( CNavStringTable::TYPE );
		Assert( pcWorldType != NULL );

		pcValue = pcWorldType->FindChild();
		Assert( pcValue != NULL );

		if( pcValue->GetString().IsEqual( CNavStringTable::OUTDOOR ) == true )
		{
			sCreateInfo.uSubType.eWorldType	= E_NAV_WORLD_TYPE_OUTDOOR;
		}
		else if( pcValue->GetString().IsEqual( CNavStringTable::INDOOR ) == true )
		{
			sCreateInfo.uSubType.eWorldType	= E_NAV_WORLD_TYPE_INDOOR;
		}
		else
		{
			sCreateInfo.uSubType.eWorldType	= E_NAV_WORLD_TYPE_NONE;
		}


		// Get Flags

		pcFlags = pcWorld->FindChild( CNavStringTable::FLAGS );
		Assert( pcFlags != NULL );

		pcValue = pcFlags->FindChild();
		Assert( pcValue != NULL );


		// Set Create Info

		sCreateInfo.eEntityType		= E_NAV_ENTITY_TYPE_WORLD;
		sCreateInfo.eCreateType		= E_NAV_CREATE_TYPE_FILE;
		sCreateInfo.pvUserData		= NULL;
		sCreateInfo.nUserDataSize	= 0;
		sCreateInfo.nFlags			= MakeFlags( rsCreateInfo.nFlags, pcFlags->GetString().ConvertToInt32() );							// 
		sCreateInfo.pcParent		= this;

		sCreateInfo.cBounding.Invalidate();
		sCreateInfo.cPath.Format( _T( "%s\\%s" ),
									rsCreateInfo.cPath.GetBuffer(),
									rsCreateInfo.cName.GetBuffer() );


		// Create World Entity

		if( CreateEntity( sCreateInfo, true ) == NULL )
		{
			return false;
		}

		++m_nWorldCount;
	}

	return true;
}


//*****************************************************************************
//
// End of File : CNavigator_Parse.cpp
//
//*****************************************************************************