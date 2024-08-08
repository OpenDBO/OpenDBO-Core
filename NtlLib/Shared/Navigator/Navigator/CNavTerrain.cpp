//*****************************************************************************
// File       : CNavTerrain.cpp
// Desc       : 
// Author     : June C. Kim (cynis@hotmail.com)
// Update     : 2007. 1. 18
//*****************************************************************************

//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "StdAfx.h"

#include "CLogMessageManager.h"

#include "CNavPathEngine.h"
#include "CNavRenderer.h"
#include "CNavEntityVector.h"
#include "CNavPreProcessThread.h"

#include "CNavigator.h"
#include "CNavObject.h"

#include "CNavTerrain.h"


//-----------------------------------------------------------------------------
// Name Space Table
//-----------------------------------------------------------------------------

USE_NAMESPACE( nav )


//-----------------------------------------------------------------------------
// Initialize Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Name			: Initialize
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CNavTerrain::Initialize( void )
{
	m_pcMesh		= NULL;
	m_pcContext		= NULL;
	m_pcObstacleSet	= NULL;

	m_nFaceCount	= 0;

	m_pcObjects		= NULL;

	return;
}


//-----------------------------------------------------------------------------
// Name			: Destroy
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CNavTerrain::Destroy( void )
{
	if( m_pcObjects != NULL )
	{
		delete m_pcObjects;
	}

	if( m_pcContext != NULL )
	{
		delete m_pcContext;
	}

	if( m_pcMesh != NULL )
	{
		delete m_pcMesh;
	}

	return;
}


//-----------------------------------------------------------------------------
// Name			: Create
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CNavTerrain::Create( SNavEntityCreateInfo & rsCreateInfo )
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
			Assert( !"Not Supported Yet" );

			break;
		}

		default:
		{
			return false;
		}
	}

	if( m_pcMesh != NULL )
	{
		m_nFaceCount	= m_pcMesh->getNumberOf3DFaces();
		m_pcContext		= m_pcMesh->newContext();
		m_pcObstacleSet	= m_pcMesh->newObstacleSet();

		if( m_pcContext != NULL )
		{
			m_pcContext->addObstacleSet( m_pcObstacleSet );
		}
	}

//	Assert( m_pcObstacleSet == m_pcContext->getObstacleSet( 0 ) );

	return true;
}


//-----------------------------------------------------------------------------
// Name			: InsertChild
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CNavTerrain::InsertChild( CNavEntity * const pcEntity )
{
	Assert( pcEntity != NULL );

	switch( pcEntity->GetType() )
	{
		case E_NAV_ENTITY_TYPE_OBJECT:
		{
			return LinkObject( down_cast<CNavObject * const>(pcEntity) );
		}

		case E_NAV_ENTITY_TYPE_NAVIGATOR:
		case E_NAV_ENTITY_TYPE_WORLD:
		case E_NAV_ENTITY_TYPE_ZONE:
		case E_NAV_ENTITY_TYPE_SECTOR:
		case E_NAV_ENTITY_TYPE_TERRAIN:
		case E_NAV_ENTITY_TYPE_HEIGHT_MAP:
		{
			Assert( !"Unsupported Match" );

			return false;
		}

		case E_NAV_ENTITY_TYPE_AGENT:
		case E_NAV_ENTITY_TYPE_PATH:
		{
			Assert( !"Not Suppported Yet" );

			return false;
		}

		default:
		{
			Assert( !"Undefined Entity Type" );

			return false;
		}
	}
}


//-----------------------------------------------------------------------------
// Name			: RemoveChild
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CNavTerrain::RemoveChild( CNavEntity * const pcEntity )
{
	Assert( pcEntity != NULL );

	switch( pcEntity->GetType() )
	{
		case E_NAV_ENTITY_TYPE_OBJECT:
		{
			return UnlinkObject( down_cast<CNavObject * const>(pcEntity) );
		}

		case E_NAV_ENTITY_TYPE_NAVIGATOR:
		case E_NAV_ENTITY_TYPE_WORLD:
		case E_NAV_ENTITY_TYPE_ZONE:
		case E_NAV_ENTITY_TYPE_SECTOR:
		case E_NAV_ENTITY_TYPE_TERRAIN:
		case E_NAV_ENTITY_TYPE_HEIGHT_MAP:
		{
			Assert( !"Unsupported Match" );

			return false;
		}

		case E_NAV_ENTITY_TYPE_AGENT:
		case E_NAV_ENTITY_TYPE_PATH:
		{
			Assert( !"Not Suppported Yet" );

			return false;
		}

		default:
		{
			Assert( !"Undefined Entity Type" );

			return false;
		}
	}
}


//-----------------------------------------------------------------------------
// Name			: GetChildByUniqueId
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CNavEntity * const CNavTerrain::GetChildByUniqueId( UId32 nEntityId ) const
{
	if( m_pcObjects == NULL )
	{
		return NULL;
	}

	return m_pcObjects->GetAt( m_pcObjects->SearchChildByUniqueId( nEntityId ) );
}


//-----------------------------------------------------------------------------
// Name			: GetChildByPosition
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CNavEntity * const CNavTerrain::GetChildByPosition( const CNavPosition & rcPosition1 ) const
{
	if( m_pcObjects == NULL )
	{
		return NULL;
	}

	return m_pcObjects->GetAt( m_pcObjects->SearchChildByPosition( rcPosition1 ) );
}


//-----------------------------------------------------------------------------
// Name			: GetChildByPosition
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CNavEntity * const CNavTerrain::GetChildByPosition( const CNavPosition & rcPosition1, const CNavPosition & rcPosition2 ) const
{
	if( m_pcObjects == NULL )
	{
		return NULL;
	}

	return m_pcObjects->GetAt( m_pcObjects->SearchChildByPosition( rcPosition1, rcPosition2 ) );
}


//-----------------------------------------------------------------------------
// Name			: FindUnblockedPosition
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CNavTerrain::FindUnblockedPosition( CNavPosition & rcSearchPosition, Fixed32 nDistance, UId32 nShapeId ) const
{
	if( m_pcMesh == NULL )
	{
		return false;
	}

	cPosition		cSearchPosition;
	
	iShape *		pcShape = CNavigator::GetInstance()->GetShape( nShapeId );
	if( pcShape == NULL )
	{
		return false;
	}

	if( ConvertPosition( cSearchPosition, rcSearchPosition ) == false )
	{
		cSearchPosition = m_pcMesh->positionInSection( -1, rcSearchPosition.GetX(), rcSearchPosition.GetY() );

		if( cSearchPosition.cell < 0 )
		{
			return false;
		}
	}

	cPosition		cResultPosition = m_pcMesh->findClosestUnobstructedPosition( pcShape, m_pcContext, cSearchPosition, nDistance );

	rcSearchPosition.Set( cResultPosition.x, cResultPosition.y, 0 );

	return true;
}


//-----------------------------------------------------------------------------
// Name			: PreProcess
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CNavTerrain::PreProcess( SNavEntityPreProcessInfo & rsPreProcessInfo )
{
	if( m_pcMesh == NULL )
	{
		return false;
	}

	iShape *	pcShape;

	if( IsInFlags( rsPreProcessInfo.nFlags, E_NAV_ENTITY_CREATE_INFO_FLAG_MULTI_THREAD ) == true )
	{
		CNavPreProcessThread *	pcThread = new CNavPreProcessThread;
		Assert( pcThread != NULL );

		Assert( rsPreProcessInfo.pcThreadManager != NULL );

		pcThread->SetPreProcessInfo( rsPreProcessInfo );
		pcThread->SetEntityName( m_cName );

		if( pcThread->Create( m_ePriority, true, false ) == false )
		{
			return false;
		}

		Assert( rsPreProcessInfo.pcThreadManager != NULL );
		rsPreProcessInfo.pcThreadManager->AddThread( pcThread );
	}
	else
	{
		pcShape = CNavigator::GetInstance()->GetShape( rsPreProcessInfo.nShapeId );
		Assert( pcShape != NULL );

		// Load Collision Pre-Process

		CFixedString	cFullFilePathName;
		cFullFilePathName.Format( _T( "%s\\%s.cpp" ),
								rsPreProcessInfo.cPath.GetBuffer(),
								GetName().GetBuffer() );

		CNavPathEngine::GetInstance()->LoadPreProcess( E_NAV_FILE_TYPE_CPP, cFullFilePathName, pcShape, m_pcMesh, rsPreProcessInfo.nFlags );

		// Load Path-Finding Pre-Process

		cFullFilePathName.Format( _T( "%s\\%s.ppp" ),
								rsPreProcessInfo.cPath.GetBuffer(),
								GetName().GetBuffer() );

		CNavPathEngine::GetInstance()->LoadPreProcess( E_NAV_FILE_TYPE_PPP, cFullFilePathName, pcShape, m_pcMesh, rsPreProcessInfo.nFlags );

		m_nFaceCount	= m_pcMesh->getNumberOf3DFaces();
		m_pcContext		= m_pcMesh->newContext();
		m_pcObstacleSet	= m_pcMesh->newObstacleSet();

	//	m_pcContext->addObstacleSet( m_pcObstacleSet );

		if( m_bExport == true )
		{
#ifdef __PATH_ENGINE_TOK__
			ExportToFile( E_NAV_FILE_TYPE_TOK, m_cExportName );
#else //__PATH_ENGINE_TOK__
			ExportToFile( E_NAV_FILE_TYPE_XML, m_cExportName );
#endif //__PATH_ENGINE_TOK__
		}
	}

	return true;
}


//-----------------------------------------------------------------------------
// Name			: PostProcess
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CNavTerrain::PostProcess( SNavEntityPreProcessInfo & rsPostProcessInfo )
{
	Assert( m_pcMesh != NULL );

	iShape *	pcShape = CNavigator::GetInstance()->GetShape( rsPostProcessInfo.nShapeId );
	Assert( pcShape != NULL );

	CNavPathEngine::GetInstance()->UnloadPreProcess( pcShape, m_pcMesh );

	return true;
}


//-----------------------------------------------------------------------------
// Name			: Render
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

Int32 CNavTerrain::Render( CNavRenderer * const pcRenderer ) const
{
	Int32		nRenderCount = 0;

	// Render Mesh

	CNavPosition	cFaceVertex[3];

	for( UInt32 nFaceIndex = 0; nFaceIndex < m_nFaceCount; ++nFaceIndex )
	{
		for( long nVertexIndex = 0; nVertexIndex < 3; ++nVertexIndex )
		{
			GetVertex( nFaceIndex, nVertexIndex, cFaceVertex[nVertexIndex] );
		}

		for( long nVertexIndex = 0; nVertexIndex < 3; ++nVertexIndex )
		{
			if( GetConnection( nFaceIndex, nVertexIndex ) < m_nFaceCount )
			{
				pcRenderer->RenderLine( cFaceVertex[nVertexIndex], cFaceVertex[(nVertexIndex + 1) % 3], 0x00FF0000, 1 );
			}
			else
			{
				pcRenderer->RenderLine( cFaceVertex[nVertexIndex], cFaceVertex[(nVertexIndex + 1) % 3], 0x000000FF, 3 );
			}

			++nRenderCount;
		}
	}

	// Render Obstacle

	if( m_pcObjects != NULL )
	{
		nRenderCount += m_pcObjects->Render( pcRenderer );
	}

	return nRenderCount;
}


//-----------------------------------------------------------------------------
// Name			: IsValid
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CNavTerrain::IsValid( void ) const
{
	return true;
}


//*****************************************************************************
//
// End of File : CNavTerrain.cpp
//
//*****************************************************************************