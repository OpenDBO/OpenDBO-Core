//*****************************************************************************
// File       : CNavTerrain_Parse.cpp
// Desc       : 
// Author     : June C. Kim (cynis@hotmail.com)
// Update     : 2007. 1. 18
//*****************************************************************************

//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "StdAfx.h"

#include "CNavPathEngine.h"
#include "CNavConverter.h"
#include "CNavOutputStream.h"
#include "CNavExportMeshThread.h"
#include "CNavLoadMeshThread.h"
#include "CNavigator.h"

#include "CNavTerrain.h"


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
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CNavTerrain::CreateFromFile( SNavEntityCreateInfo & rsCreateInfo )
{
	if( IsInFlags( rsCreateInfo.nFlags, E_NAV_ENTITY_CREATE_INFO_FLAG_MULTI_THREAD ) == true )
	{
		CNavLoadMeshThread *	pcThread = new CNavLoadMeshThread;
		Assert( pcThread != NULL );

		CNavTerrain *	pcThis = this;
		Assert( pcThis != NULL );

		Assert( rsCreateInfo.pcThreadManager != NULL );

		pcThread->SetCreateInfo( rsCreateInfo );

		m_ePriority = static_cast<EThreadPriority>(GetLocalFlags( rsCreateInfo.nFlags ));

		if( pcThread->Create( m_ePriority, true, false ) == false )
		{
			return false;
		}

		rsCreateInfo.pcThreadManager->AddThread( pcThread );
	}
	else
	{
#ifdef __PATH_ENGINE_TOK__
		ENavFileType	eFileType =  E_NAV_FILE_TYPE_TOK;
#else //__PATH_ENGINE_TOK__
		ENavFileType	eFileType =  E_NAV_FILE_TYPE_XML;
#endif //__PATH_ENGINE_TOK__

		CFixedString	cFullFilePathName;
		cFullFilePathName.AppendFormat( _T( "%s\\%s.%s" ),
									rsCreateInfo.cPath.GetBuffer(),
									rsCreateInfo.cName.GetBuffer(),
									CNavPathEngine::KEYWORD_FILE_FORMAT[eFileType].GetBuffer() );

		m_pcMesh = CNavPathEngine::GetInstance()->LoadMesh( eFileType, cFullFilePathName );
		if( m_pcMesh == NULL )
		{
			return false;
		}
	}

	return true;
}


//-----------------------------------------------------------------------------
// Name			: ImportFromFile
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CNavTerrain::ImportFromFile( SNavEntityCreateInfo & rsCreateInfo )
{
	if( IsInFlags( rsCreateInfo.nFlags, E_NAV_ENTITY_CREATE_INFO_FLAG_MULTI_THREAD ) == true )
	{
		CNavExportMeshThread *	pcThread = new CNavExportMeshThread;
		Assert( pcThread != NULL );

		CNavTerrain *	pcThis = this;
		Assert( pcThis != NULL );

		Assert( rsCreateInfo.pcThreadManager != NULL );

		pcThread->SetCreateInfo( rsCreateInfo );

		m_ePriority = static_cast<EThreadPriority>(GetLocalFlags( rsCreateInfo.nFlags ));

		if( pcThread->Create( m_ePriority, true, false ) == false )
		{
			return false;
		}

		rsCreateInfo.pcThreadManager->AddThread( pcThread );
	}
	else
	{
		if( rsCreateInfo.pcParent == NULL )
		{
			return false;
		}

		CNavigator *	pcNavigator = down_cast<CNavigator *>(rsCreateInfo.pcParent->GetRoot());
		CNavConverter *		pcImporter = ( pcNavigator->GetImporter() != NULL ) ?
											pcNavigator->GetImporter() :
											CNavigator::GetDefaultConverter();
		Assert( pcImporter != NULL );

		rsCreateInfo.pvUserData = this;
		rsCreateInfo.nUserDataSize = sizeof( CNavEntity * );

		pcImporter->ImportTerrain( &rsCreateInfo );

		m_cExportName.Format( _T( "%s\\%s" ),
							rsCreateInfo.cPath.GetBuffer(),
							rsCreateInfo.cName.GetBuffer() );
	}

	return true;
}


//-----------------------------------------------------------------------------
// Name			: ExportToFile
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CNavTerrain::ExportToFile( ENavFileType eExportFileType, CFixedString & rcExportFileName )
{
	Assert( m_pcMesh != NULL );
	Assert( eExportFileType == E_NAV_FILE_TYPE_XML ||
			eExportFileType == E_NAV_FILE_TYPE_TOK );

	CNavOutputStream *	pcStream = new CNavOutputStream;
	CFixedString	cExportFileName;


	// Save Pathfind Pre-Process

	cExportFileName.Assign( rcExportFileName );
	cExportFileName.Append( _T( ".ppp" ) );

	pcStream->Create( cExportFileName );

	CNavigator *	pcNavigator = down_cast<CNavigator *>(GetRoot());
	Assert( pcNavigator != NULL );

	iShape *	pcShape = pcNavigator->GetShape( 0 );
	Assert( pcShape != NULL );

	m_pcMesh->savePathfindPreprocessFor( pcShape, pcStream );


	// Save Collision Pre-Process

	cExportFileName.Assign( rcExportFileName );
	cExportFileName.Append( _T( ".cpp" ) );

	pcStream->Create( cExportFileName );

	m_pcMesh->saveCollisionPreprocessFor( pcShape, pcStream );


	// Save Mesh

	cExportFileName.Assign( rcExportFileName );
	cExportFileName.Append( _T( "." ) );
	cExportFileName.Append( CNavPathEngine::KEYWORD_FILE_FORMAT[eExportFileType] );

	pcStream->Create( cExportFileName );

	m_pcMesh->saveGround( CNavPathEngine::KEYWORD_FILE_FORMAT[eExportFileType].GetBuffer(), true, pcStream );

	delete pcStream;

	return true;
}


//*****************************************************************************
//
// End of File : CNavTerrain_Parse.cpp
//
//*****************************************************************************