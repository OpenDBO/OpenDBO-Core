//*****************************************************************************
// File       : CNavPathEngine.cpp
// Desc       : 
// Author     : June C. Kim (cynis@hotmail.com)
// Update     : 2007. 1. 18
//*****************************************************************************

//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "StdAfx.h"

#include "interface\Singletons.h"

#include "CMemoryBuffer.h"
#include "CNavErrorHandler.h"
#include "CNavFaceVertexMesh.h"

#include "CFixedStringTreeNodeW.h"
#include "CXMLParserW.h"

#include "NavEntityBase.h"

#include "CNavPathEngine.h"


//-----------------------------------------------------------------------------
// Name Space Table
//-----------------------------------------------------------------------------

USE_NAMESPACE( nav )


//-----------------------------------------------------------------------------
// Initialize Table
//-----------------------------------------------------------------------------

const CFixedStringA CNavPathEngine::KEYWORD_FILE_FORMAT[E_NAV_FILE_TYPE_COUNT] =
{
	CFixedStringA( "" ),
	CFixedStringA( "xml" ),
	CFixedStringA( "tok" ),
	CFixedStringA( "vfm" )
};


//-----------------------------------------------------------------------------
// Name			: CNavPathEngine
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CNavPathEngine::CNavPathEngine( void )
{
	Initialize();

	return;
}


//-----------------------------------------------------------------------------
// Name			: ~CNavPathEngine
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CNavPathEngine::~CNavPathEngine( void )
{
	UnloadPathEngine();

	Initialize();

	return;
}


//-----------------------------------------------------------------------------
// Name			: Initialize
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CNavPathEngine::Initialize( void )
{
	m_pcPathEngine	= NULL;
	m_hDllInstance	= NULL;

	m_dwLastError	= ERROR_SUCCESS;

	return;
}


//-----------------------------------------------------------------------------
// Name			: LoadPathEngine
// Desc			: 
// Privilege	: static public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CNavPathEngine::LoadPathEngine( const CFixedString & rcDllName, iErrorHandler * pcErrorHandler )
{
	UNREFERENCED_PARAMETER( rcDllName );

	if( m_pcPathEngine != NULL )
	{
		UnloadPathEngine();
	}
	
	if( pcErrorHandler == NULL )
	{
		pcErrorHandler = CNavErrorHandler::CreateInstance();
	}

#ifdef __PATH_ENGINE_DLL__
	CFixedStringA		rcDllNameAnsi;

	if( rcDllName.GetLength() == 0L )
	{
		rcDllNameAnsi.Assign( PE_ENGINE_NAME_DEFAULT );
	}
	else
	{
		rcDllNameAnsi.Assign( rcDllName );
	}

	m_hDllInstance = LoadLibraryA( rcDllNameAnsi.GetBuffer() );
	if( m_hDllInstance == NULL )
	{
		m_dwLastError = ::GetLastError();

		return false;
	}

	FARPROC		pProcessAddress = GetProcAddress( m_hDllInstance, (LPCSTR)1 );
	if( pProcessAddress == NULL )
	{
		m_dwLastError = ::GetLastError();

		return false;
	}

	TGetInterfaceFunction	tGetInterfaceFunction = reinterpret_cast<TGetInterfaceFunction>(pProcessAddress);

	m_pcPathEngine = tGetInterfaceFunction( pcErrorHandler );
#else //__PATH_ENGINE_DLL__
    if(!nSingletons::isInitialised_StandAlone())
    {
        nSingletons::init_StandAlone();
    }

    m_pcPathEngine = &nSingletons::pathEngineI();
#endif //__PATH_ENGINE_DLL__

    m_pcPathEngine->setErrorHandler( pcErrorHandler );

	return ( m_pcPathEngine != NULL );
}


//-----------------------------------------------------------------------------
// Name			: LoadPathEngine
// Desc			: 
// Privilege	: static public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CNavPathEngine::LoadPathEngine( iPathEngine * const pcPathEngine, iErrorHandler * pcErrorHander )
{
	if( m_pcPathEngine != NULL )
	{
		UnloadPathEngine();
	}

	if( pcErrorHander == NULL )
	{
		pcErrorHander = CNavErrorHandler::CreateInstance();
	}

	m_pcPathEngine = pcPathEngine;

	return ( m_pcPathEngine != NULL );
}


//-----------------------------------------------------------------------------
// Name			: UnloadPathEngine
// Desc			: 
// Privilege	: static public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CNavPathEngine::UnloadPathEngine( void )
{
	if( m_pcPathEngine != NULL )
	{
		m_pcPathEngine->deleteAllObjects();

		m_pcPathEngine = NULL;
	}

	if( m_hDllInstance != NULL )
	{
		FreeLibrary( m_hDllInstance );
	}

	Initialize();

	return;
}


//-----------------------------------------------------------------------------
// Name			: LoadMesh
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

iMesh * CNavPathEngine::LoadMesh( ENavFileType eFileType, const CFixedString & rcSourceFileName, UInt32 nFlags )
{
	UNREFERENCED_PARAMETER( nFlags );

	if( m_pcPathEngine == NULL )
	{
		return NULL;
	}

	iMesh *		pcMesh;

	switch( eFileType )
	{
		case E_NAV_FILE_TYPE_XML:
		case E_NAV_FILE_TYPE_TOK:
		{
			CMemoryBuffer	cBuffer( rcSourceFileName );

			g_Log( "iPathEngine::loadMeshFromBuffer() : Begin" );

			pcMesh = m_pcPathEngine->loadMeshFromBuffer( KEYWORD_FILE_FORMAT[eFileType].GetBuffer(), reinterpret_cast<char *>(cBuffer.GetBuffer()), static_cast<unsigned long>(cBuffer.GetLength()), NULL );

			g_Log( "iPathEngine::loadMeshFromBuffer() : End" );

			break;
		}

		case E_NAV_FILE_TYPE_VFM:
		{
			pcMesh = NULL;

			break;
		}

		default:
		{
			pcMesh = NULL;
		}
	}

	return pcMesh;
}


//-----------------------------------------------------------------------------
// Name			: LoadMesh
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

iMesh * CNavPathEngine::LoadMesh( ENavMemoryType eMemoryType, void * pvData, UInt32 nFlags )
{
	UNREFERENCED_PARAMETER( nFlags );

	if( m_pcPathEngine == NULL )
	{
		return NULL;
	}

	Assert( pvData != NULL );

	iMesh *		pcMesh;

	switch( eMemoryType )
	{
		case E_NAV_MEMORY_TYPE_INSTANCE:
		{
			pcMesh = reinterpret_cast<iMesh *>(pvData);

			break;
		}

		case E_NAV_MEMORY_TYPE_RESOURCE:
		{
			SNavFaceVertexMeshList *	psVertexMeshList = reinterpret_cast<SNavFaceVertexMeshList *>(pvData);

			g_Log( "iPathEngine::buildMeshFromContent() : Begin" );

			pcMesh = m_pcPathEngine->buildMeshFromContent( (const iFaceVertexMesh *const *)psVertexMeshList->ppcFaceVertexMeshList, psVertexMeshList->nFaceVertexMeshCount, psVertexMeshList->ppchFlags );

			g_Log( "iPathEngine::buildMeshFromContent() : End" );

			break;
		}

		default:
		{
			pcMesh = NULL;
		}
	}

	return pcMesh;
}


//-----------------------------------------------------------------------------
// Name			: CreateMesh
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

iMesh * CNavPathEngine::CreateMeshFromFaceVertexMesh( const iFaceVertexMesh * const * ppcGeometryObjectPointers, long nNumberOfGeometryObjects, const char * const * ppchOptions ) const
{
	if( ppchOptions == NULL )
	{
		static const char *		pchOption = NULL;

		ppchOptions = &pchOption;
	}

	return m_pcPathEngine->buildMeshFromContent( ppcGeometryObjectPointers, nNumberOfGeometryObjects, ppchOptions );
}


//-----------------------------------------------------------------------------
// Name			: UnloadMesh
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CNavPathEngine::UnloadMesh( iMesh ** ppMesh )
{
	if( ppMesh == NULL || *ppMesh == NULL )
	{
		return;
	}

	delete *ppMesh;
	ppMesh = NULL;

	return;
}


//-----------------------------------------------------------------------------
// Name			: LoadShape
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

iShape * CNavPathEngine::LoadShape( ENavFileType eFileType, const CFixedString & rcSourceFileName, UInt32 iFlags )
{
	UNREFERENCED_PARAMETER( iFlags );

	if( m_pcPathEngine == NULL )
	{
		return NULL;
	}

	iShape *	pcShape;

	switch( eFileType )
	{
		case E_NAV_FILE_TYPE_XML:
		{
			pcShape = ParseShapeXML( rcSourceFileName );

			break;
		}

		case E_NAV_FILE_TYPE_TOK:
		case E_NAV_FILE_TYPE_VFM:
		{
			Assert( !"Not Supported" );

			pcShape = NULL;

			break;
		}

		default:
		{
			Assert( !"Undefined Shape File Type" );

			pcShape = NULL;
		}
	}

	
	return pcShape;
}


//-----------------------------------------------------------------------------
// Name			: UnloadShape
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CNavPathEngine::UnloadShape( iShape ** ppcShape )
{
	if( ppcShape == NULL || *ppcShape == NULL )
	{
		return;
	}

	delete *ppcShape;
	ppcShape = NULL;

	return;
}


//-----------------------------------------------------------------------------
// Name			: LoadPreProcess
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CNavPathEngine::LoadPreProcess( ENavFileType eFileType, const CFixedString & rcSource, iShape * pcShape, iMesh * pcMesh, UInt32 nFlags )
{
	UNREFERENCED_PARAMETER( nFlags );

	CMemoryBuffer	cBuffer;

	switch( eFileType )
	{
		case E_NAV_FILE_TYPE_CPP:
		{
			if( ( ( nFlags & E_NAV_ENTITY_CREATE_INFO_FLAG_EXPORT_MODE ) == 0 ) &&
				( cBuffer.Create( rcSource ) == true ) )
			{
				g_Log( "iMesh::loadCollisionPreprocessFor() : Begin" );

				pcMesh->loadCollisionPreprocessFor( pcShape, reinterpret_cast<const char *>(cBuffer.GetBuffer()), static_cast<UInt32>(cBuffer.GetLength()) );

				g_Log( "iMesh::loadCollisionPreprocessFor() : End" );
			}
			else
			{
				g_Log( "iMesh::generateCollisionPreprocessFor() : Begin" );

				pcMesh->generateCollisionPreprocessFor( pcShape, NULL );

				g_Log( "iMesh::generateCollisionPreprocessFor() : End" );
			}

			break;
		}

		case E_NAV_FILE_TYPE_PPP:
		{
			if( ( ( nFlags & E_NAV_ENTITY_CREATE_INFO_FLAG_EXPORT_MODE ) == 0 ) &&
				( cBuffer.Create( rcSource ) == true ) )
			{
				g_Log( "iMesh::loadPathfindPreprocessFor() : Begin" );

				pcMesh->loadPathfindPreprocessFor( pcShape, down_cast<const char *>(cBuffer.GetBuffer()), static_cast<UInt32>(cBuffer.GetLength()) );

				g_Log( "iMesh::loadPathfindPreprocessFor() : End" );
			}
			else
			{
				g_Log( "iMesh::generatePathfindPreprocessFor() : Begin" );

				pcMesh->generatePathfindPreprocessFor( pcShape, NULL );

				g_Log( "iMesh::generatePathfindPreprocessFor() : End" );
			}

			break;
		}

		default:
		{
			Assert( !"Undefined PreProcess File Type" );
		}
	}

	return;
}


//-----------------------------------------------------------------------------
// Name			: UnloadPreProcess
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CNavPathEngine::UnloadPreProcess( iShape * pcShape, iMesh * pcMesh, UInt32 nFlags )
{
	UNREFERENCED_PARAMETER( nFlags );

	pcMesh->releaseAllPreprocessFor( pcShape );

	return;
}


//-----------------------------------------------------------------------------
// Name			: ValidatePathEngine
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CNavPathEngine::ValidatePathEngine( void ) const
{
	if( ValidateEngineVersion() == false )
	{
		return false;
	}

	return true;
}


//-----------------------------------------------------------------------------
// Name			: ValidateEngineVersion
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CNavPathEngine::ValidateEngineVersion( void ) const
{
	if( m_pcPathEngine == NULL )
	{
		return false;
	}

	if( m_pcPathEngine->getInterfaceMajorVersion() != PATHENGINE_INTERFACE_MAJOR_VERSION )
	{
        return false;
    }

	if( m_pcPathEngine->getInterfaceMinorVersion() < PATHENGINE_INTERFACE_MINOR_VERSION )
	{
        return false;
    }

	return true;
}


//-----------------------------------------------------------------------------
// Name			: ParseShapeXML
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

iShape * CNavPathEngine::ParseShapeXML( const CFixedString & rcXMLFileName )
{
	CFixedStringW	cFileName( rcXMLFileName );
	CXMLParserW		cXMLParser;

	if( cXMLParser.Create( rcXMLFileName ) == false )
	{
		return NULL;
	}

	// Keyword List

	CFixedStringTreeNodeW *	pcValue;


	// Get Root

	CFixedStringTreeNodeW *	pcRoot = cXMLParser.GetRoot();
	Assert( pcRoot != NULL );

	CFixedStringTreeNodeW *	pcShape = pcRoot->FindChild(CNavStringTable::SHAPE );
	Assert( pcShape != NULL );


	// Parse Header

	CFixedStringTreeNodeW *	pcHeader = pcShape->FindChild( CNavStringTable::HEADER );
	Assert( pcHeader != NULL );

	// Get Vertex Count

	CFixedStringTreeNodeW *	pcCount = pcHeader->FindChild( CNavStringTable::COUNT );
	Assert( pcCount != NULL );

	pcValue = pcCount->FindChild();
	Assert( pcValue != NULL );

	UInt32		nVertexCount = pcValue->GetString().ConvertToInt32();
	Assert( nVertexCount >= 3 );

	long *	pnVertexArray = new long[nVertexCount * 2];


	// Parse Vertex

	CFixedStringTreeNodeW *	pcVertex;
	CFixedStringTreeNodeW *	pcVertexX;
	CFixedStringTreeNodeW *	pcVertexY;
	Float32		fX;
	Float32		fY;

	for( UInt32 nVertexIndex = 0; nVertexIndex < nVertexCount; ++nVertexIndex )
	{
		pcVertex = pcShape->FindChild( CNavStringTable::VERTEX, nVertexIndex );
		Assert( pcVertex != NULL );

		pcVertexX = pcVertex->FindChild( CNavStringTable::X );
		Assert( pcVertexX != NULL );

		pcValue = pcVertexX->FindChild();
		Assert( pcValue != NULL );
		fX = pcValue->GetString().ConvertToFloat32();

		pcVertexY = pcVertex->FindChild( CNavStringTable::Y );
		Assert( pcVertexX != NULL );

		pcValue = pcVertexY->FindChild();
		Assert( pcValue != NULL );
		fY = pcValue->GetString().ConvertToFloat32();

		pnVertexArray[nVertexIndex * 2 + 0] = Float32ToFixed32( fX );
		pnVertexArray[nVertexIndex * 2 + 1] = Float32ToFixed32( fY );
	}

	iShape *	pcResultShape = m_pcPathEngine->newShape( nVertexCount, pnVertexArray );

	delete[] pnVertexArray;

	return pcResultShape;
}


//*****************************************************************************
//
// End of File : CNavPathEngine.cpp
//
//*****************************************************************************