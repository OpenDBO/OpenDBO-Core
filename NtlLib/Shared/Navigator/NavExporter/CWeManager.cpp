//*****************************************************************************
// File       : CWeManager.cpp
// Desc       : 
// Author     : June C. Kim (cynis@hotmail.com)
// Update     : 2007. 1. 18
//*****************************************************************************

//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "StdAfx.h"

#include "VersionLog.h"

#include "CFixedStringTreeNodeW.h"
#include "CWeConverter.h"

#include "CWeManager.h"


//-----------------------------------------------------------------------------
// Name Space Table
//-----------------------------------------------------------------------------

USE_NAMESPACE( navex )


//-----------------------------------------------------------------------------
// Initialize Table
//-----------------------------------------------------------------------------

CFixedString CWeManager::m_cPathName = CFixedString();


//-----------------------------------------------------------------------------
// Name			: Initialize
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CWeManager::Initialize( void )
{
	return;
}


//-----------------------------------------------------------------------------
// Name			: Destroy
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CWeManager::Destroy( void )
{
	Initialize();

	return;
}


//-----------------------------------------------------------------------------
// Name			: ParsePathNode
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CWeManager::ParsePathNode( CNavFilePath & rcPath, const CFixedStringTreeNodeW * pcNode )
{
	if( pcNode == NULL )
	{
		return false;
	}

	CFixedStringTreeNodeW *	pcValue;


	// Get Path

	CFixedStringTreeNodeW *	pcPath = pcNode->FindChild( KEY_PATH );
	if( pcPath != NULL )
	{
		pcValue = pcPath->FindChild();
		Assert( pcValue != NULL );
		rcPath.SetPath( pcValue->GetString() );
	}
	else
	{
		rcPath.SetPath( CFixedString::NULLSTRING );
	}


	// Get Name

	CFixedStringTreeNodeW *	pcName = pcNode->FindChild( KEY_NAME );
	if( pcName != NULL )
	{
		pcValue = pcName->FindChild();
		Assert( pcValue != NULL );
		rcPath.SetName( pcValue->GetString() );
	}
	else
	{
		rcPath.SetName( CFixedString::NULLSTRING );
	}


	// Get Ext

	CFixedStringTreeNodeW *	pcExt = pcNode->FindChild( KEY_EXT );
	if( pcExt != NULL )
	{
		pcValue = pcExt->FindChild();
		Assert( pcValue != NULL );
		rcPath.SetExt( pcValue->GetString() );
	}
	else
	{
		rcPath.SetExt( CFixedString::NULLSTRING );
	}

	return true;
}


//-----------------------------------------------------------------------------
// Name			: ConvertWorld
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CWeManager::ConvertWorld( const CFixedString & rcConfigFileName )
{
	CFixedStringW	cFullFileName;

	if( GetPathName().GetLength() == 0 )
	{
		cFullFileName.Assign( rcConfigFileName );
	}
	else
	{
		CFixedString	cConverter;
		cConverter.Format( _T( "%s\\%s" ),
							GetPathName().GetBuffer(), rcConfigFileName.GetBuffer() );

		cFullFileName = cConverter;
	}

	CXMLParserW		cXMLParser;
	
	if( cXMLParser.Create( cFullFileName ) == false )
	{
		Assert( !"Fail to Open Convert Config File" );

		return false;
	}


	// Create Converter

	CWeConverter *	pcConverter = new CWeConverter;
	SNavEntityExternalProcess	sExternalProcess;
	sExternalProcess.pcExporter = pcConverter;
	sExternalProcess.pcImporter = pcConverter;


	// Keyword List

	CFixedStringTreeNodeW *	pcGroup;
	CNavFilePath	cPath;


	// Get Root

	CFixedStringTreeNodeW *	pcRoot = cXMLParser.FindChild( KEY_CONFIG );
	Assert( pcRoot != NULL );


	// Parse Reference

	pcGroup = pcRoot->FindChild( KEY_REFERENCE );

	if( ParsePathNode( cPath, pcGroup ) == true )
	{
		pcConverter->SetReferencePath( cPath );
	}
	else
	{
		Assert( !"Parse Reference Fail" );

		return false;
	}


	// Parse Height Map

	pcGroup = pcRoot->FindChild( KEY_HEIGHT_MAP );

	if( ParsePathNode( cPath, pcGroup ) == true )
	{
		pcConverter->SetHeightMapPath( cPath );
	}
	else
	{
		Assert( !"Parse Height Map Fail" );

		return false;
	}


	// Parse Attribute

	pcGroup = pcRoot->FindChild( KEY_ATTRIBUTE );

	if( ParsePathNode( cPath, pcGroup ) == true )
	{
		pcConverter->SetAttributePath( cPath );
	}
	else
	{
		Assert( !"Parse Attribute Map Fail" );

		return false;
	}


	// Parse Object

	pcGroup = pcRoot->FindChild( KEY_OBJECT );

	if( ParsePathNode( cPath, pcGroup ) == true )
	{
		pcConverter->SetObjectPath( cPath );
	}
	else
	{
		Assert( !"Parse Object Fail" );

		return false;
	}


	// Parse Modeling

	pcGroup = pcRoot->FindChild( KEY_MODELING );

	if( ParsePathNode( cPath, pcGroup ) == true )
	{
		pcConverter->SetModelPath( cPath );
	}
	else
	{
		Assert( !"Parse Modeling Fail" );

		return false;
	}


	// Parse Structure

	pcGroup = pcRoot->FindChild( KEY_STRUCTURE );

	if( ParsePathNode( cPath, pcGroup ) == true )
	{
		pcConverter->SetStructurePath( cPath );
	}
	else
	{
		Assert( !"Parse Structure Fail" );

		return false;
	}


	// Parse Output

	pcGroup = pcRoot->FindChild( KEY_OUTPUT );

	if( ParsePathNode( cPath, pcGroup ) == true )
	{
		pcConverter->SetOutputPath( cPath );
	}
	else
	{
		Assert( !"Parse Output Fail" );

		return false;
	}


	// Load Navigator

	MessageBox( NULL, _T( "Load Begin" ), _T( "CWeManager::ConvertWorld()" ), MB_OK );

	CFixedString	cSessionName( _T( "NavServer" ) );

#ifndef WIN64
	CFixedString	cEngineName( _T( "PathEngine" ) );
#else
	CFixedString	cEngineName( _T( "PathEngine_x64" ) );
#endif //WIN64

	if( CNavigator::CreateInstance( cEngineName, cSessionName, &sExternalProcess, E_NAV_ENTITY_CREATE_INFO_FLAG_EXPORT_MODE ) == NULL )
	{
		Assert( !"Engine Create Fail" );

		return false;
	}


	// Register Player Shape

	CFixedString	cShapeName( _T( "Shape001" ) );

	CNavigator::GetInstance()->RegisterShape( cShapeName, E_NAV_ENTITY_CREATE_INFO_FLAG_EXPORT_MODE );

	MessageBox( NULL, _T( "Load End" ), _T( "CWeManager::ConvertWorld()" ), MB_OK );

	delete pcConverter;

	return true;
}


//*****************************************************************************
//
// End of File : CWeManager.cpp
//
//*****************************************************************************