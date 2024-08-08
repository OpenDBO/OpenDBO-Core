//*****************************************************************************
// File       : CNavigator.cpp
// Desc       : 
// Author     : June C. Kim (cynis@hotmail.com)
// Update     : 2006. 12. 5
//*****************************************************************************

//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "StdAfx.h"

#include "VersionLog.h"

#include "CNavEntityFactory.h"
#include "CNavEntityVector.h"
#include "CNavRenderer.h"
#include "CNavConverter.h"

#include "CNavWorld.h"
#include "CNavObject.h"

#include "CNavigator.h"


//-----------------------------------------------------------------------------
// Name Space Table
//-----------------------------------------------------------------------------

USE_NAMESPACE( nav )


//-----------------------------------------------------------------------------
// Initialize Table
//-----------------------------------------------------------------------------

const SVersionInfo		CNavigator::VERSION( _VERSION_MAJOR_, _VERSION_MINOR_, _VERSION_BUILD_, _VERSION_LABEL_ );


//-----------------------------------------------------------------------------
// Name			: Initialize
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CNavigator::Initialize( void )
{
	m_pcWorlds		= NULL;
	m_nWorldCount	= 0;

	return;
}


//-----------------------------------------------------------------------------
// Name			: Destroy
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CNavigator::Destroy( void )
{
	// Destroy Renderer

	if( m_pcRenderer != NULL )
	{
		delete m_pcRenderer;
	}

	// Destroy World List

	if( m_pcWorlds != NULL )
	{
		delete m_pcWorlds;
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

bool CNavigator::Create( SNavEntityCreateInfo & rsCreateInfo )
{
	if( CNavEntity::Create( rsCreateInfo ) == false )
	{
		return false;
	}

	SNavEntityExternalProcess *		psExternalProcess = reinterpret_cast<SNavEntityExternalProcess *>(rsCreateInfo.pvUserData);

	if( psExternalProcess != NULL )
	{
		m_pcRenderer = psExternalProcess->pcRenderer;
		m_pcImporter = psExternalProcess->pcImporter;
		m_pcExporter = psExternalProcess->pcExporter;
	}
	else
	{
		m_pcRenderer = NULL;
		m_pcImporter = NULL;
		m_pcExporter = NULL;
	}

	if( rsCreateInfo.cName.GetLength() > 0 )
	{
		if( CreateFromFile( rsCreateInfo ) == false )
		{
			return false;
		}
	}

	return true;
}


//-----------------------------------------------------------------------------
// Name			: InsertChild
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CNavigator::InsertChild( CNavEntity * const pcChild )
{
	Assert( pcChild != NULL );

	switch( pcChild->GetType() )
	{
		case E_NAV_ENTITY_TYPE_WORLD:
		{
			return LinkWorld( down_cast<CNavWorld * const>(pcChild) );
		}

		case E_NAV_ENTITY_TYPE_OBJECT:
		case E_NAV_ENTITY_TYPE_AGENT:
		case E_NAV_ENTITY_TYPE_PATH:
		{
			CNavEntity *	pcWorld = GetChildByBounding( pcChild->GetBounding() );

			if( pcWorld != NULL )
			{
				return pcWorld->InsertChild( pcChild );
			}

			Assert( !"No Match World" );

			return false;
		}

		case E_NAV_ENTITY_TYPE_NAVIGATOR:
		case E_NAV_ENTITY_TYPE_ZONE:
		case E_NAV_ENTITY_TYPE_SECTOR:
		case E_NAV_ENTITY_TYPE_TERRAIN:
		case E_NAV_ENTITY_TYPE_HEIGHT_MAP:
		{
			Assert( !"Unsupported Match" );

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

bool CNavigator::RemoveChild( CNavEntity * const pcChild )
{
	Assert( pcChild != NULL );

	switch( pcChild->GetType() )
	{
		case E_NAV_ENTITY_TYPE_WORLD:
		{
			return UnlinkWorld( down_cast<CNavWorld * const>(pcChild) );
		}

		case E_NAV_ENTITY_TYPE_OBJECT:
		case E_NAV_ENTITY_TYPE_AGENT:
		case E_NAV_ENTITY_TYPE_PATH:
		{
			CNavEntity *	pcWorld = GetChildByBounding( pcChild->GetBounding() );

			if( pcWorld != NULL )
			{
				return pcWorld->RemoveChild( pcChild );
			}

			Assert( !"No Match World" );

			return false;
		}

		case E_NAV_ENTITY_TYPE_NAVIGATOR:
		case E_NAV_ENTITY_TYPE_ZONE:
		case E_NAV_ENTITY_TYPE_SECTOR:
		case E_NAV_ENTITY_TYPE_TERRAIN:
		case E_NAV_ENTITY_TYPE_HEIGHT_MAP:
		{
			Assert( !"Unsupported Match" );

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

CNavEntity * const CNavigator::GetChildByUniqueId( UId32 nUniqueId ) const
{
	if( m_pcWorlds == NULL )
	{
		return NULL;
	}

	return m_pcWorlds->GetAt( m_pcWorlds->SearchChildByUniqueId( nUniqueId ) );
}


//-----------------------------------------------------------------------------
// Name			: GetChildByPosition
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CNavEntity * const CNavigator::GetChildByPosition( const CNavPosition & rcPosition1 ) const
{
	if( m_pcWorlds == NULL )
	{
		return NULL;
	}

	return m_pcWorlds->GetAt( m_pcWorlds->SearchChildByPosition( rcPosition1 ) );
}


//-----------------------------------------------------------------------------
// Name			: GetChildByPosition
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CNavEntity * const CNavigator::GetChildByPosition( const CNavPosition & rcPosition1, const CNavPosition & rcPosition2 ) const
{
	if( rcPosition1.GetW() != rcPosition2.GetW() )
	{
		return NULL;
	}

	return GetChildByPosition( rcPosition1 );
}


//-----------------------------------------------------------------------------
// Name			: GetHeight
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CNavigator::GetHeight( CNavPosition & rcPosition ) const
{
	const CNavEntity *	pcWorld = GetChildByPosition( rcPosition );
	if( pcWorld == NULL )
	{
		return false;
	}

	return pcWorld->GetHeight( rcPosition );
}


//-----------------------------------------------------------------------------
// Name			: GetAttribute
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

DWORD CNavigator::GetAttribute( CNavPosition & rcPosition ) const
{
	const CNavEntity *	pcWorld = GetChildByPosition( rcPosition );
	if( pcWorld == NULL )
	{
		return E_NAV_TILE_ATTRIBUTE_NONE;
	}

	return pcWorld->GetAttribute( rcPosition );
}


//-----------------------------------------------------------------------------
// Name			: IsBlockedPosition
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CNavigator::IsBlockedPosition( CNavPosition & rcSearchPosition, UId32 nShapeId ) const
{
	Assert( this != NULL );

	CNavPosition	cResultPosition( rcSearchPosition );

	if( FindUnblockedPositionInternal( cResultPosition, m_nFindUnblockedPositionDistance, nShapeId ) == false )
	{
		return true;
	}

	if( rcSearchPosition.GetX() != cResultPosition.GetX() )
	{
		return true;
	}

	if( rcSearchPosition.GetY() != cResultPosition.GetY() )
	{
		return true;
	}

	return false;
}


//-----------------------------------------------------------------------------
// Name			: FindUnblockedPositionInternal
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CNavigator::FindUnblockedPositionInternal( CNavPosition & rcSearchPosition, Fixed32 nDistance, UId32 nShapeId ) const
{
	const CNavWorld *	pcWorld = down_cast<const CNavWorld *>(GetChildByPosition( rcSearchPosition ));
	if( pcWorld == NULL )
	{
		return false;
	}

	return pcWorld->FindUnblockedPosition( rcSearchPosition, nDistance, nShapeId );
}


//-----------------------------------------------------------------------------
// Name			: PreProcess
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CNavigator::PreProcess( SNavEntityPreProcessInfo & rsPreProcessInfo )
{
	if( m_pcWorlds == NULL )
	{
		return true;
	}

	SNavEntityPreProcessInfo	sPreProcessInfo( rsPreProcessInfo );

	sPreProcessInfo.cPath.Format( _T( "%s\\%s" ), GetPathName().GetBuffer(), GetName().GetBuffer() );

	CBaseThreadManager *	pcThreadManager;

	if( m_nThreadSize > 0 )
	{
		sPreProcessInfo.nFlags |= E_NAV_ENTITY_CREATE_INFO_FLAG_MULTI_THREAD;

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

	sPreProcessInfo.pcThreadManager	= pcThreadManager;
	sPreProcessInfo.psResultInfo		= NULL;

	bool	bResult = m_pcWorlds->PreProcess( sPreProcessInfo );
	if( bResult == false )
	{
		if( pcThreadManager != NULL )
		{
			delete pcThreadManager;
		}

		return false;
	}

	if( pcThreadManager != NULL )
	{
		bResult = pcThreadManager->Run();

		delete pcThreadManager;
	}

	return bResult;
}


//-----------------------------------------------------------------------------
// Name			: PostProcess
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CNavigator::PostProcess( SNavEntityPreProcessInfo & rsPostProcessInfo )
{
	if( m_pcWorlds == NULL )
	{
		return true;
	}

	return m_pcWorlds->PreProcess( rsPostProcessInfo );
}


//-----------------------------------------------------------------------------
// Name			: Render
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

Int32 CNavigator::Render( CNavRenderer * const pcRenderer ) const
{
	if( m_pcWorlds == NULL )
	{
		return 0;
	}

	return m_pcWorlds->Render( pcRenderer );
}


//-----------------------------------------------------------------------------
// Name			: RegisterShape
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

UId32 CNavigator::RegisterShape( const TCHAR * pchShapeName, UInt32 nFlags )
{
	CFixedString	cShapeName( pchShapeName );

	return RegisterShape( cShapeName, nFlags );
}


//-----------------------------------------------------------------------------
// Name			: GetHierarchy
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CNavigator::GetHierarchy( CNavEntityHierarchy & rcHierarchy, const CNavPosition & rcPosition ) const
{
	rcHierarchy.SetWorld( GetChildByPosition( rcPosition ) );
	if( rcHierarchy.GetWorld() == NULL )
	{
		return false;
	}

	return rcHierarchy.GetWorld()->GetHierarchy( rcHierarchy, rcPosition );
}


//-----------------------------------------------------------------------------
// Name			: GetVersionInfo
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CNavigator::GetVersionInfo( TCHAR * pchBuffer, size_t nBufferLength )
{
	Assert( pchBuffer != NULL );

	_stprintf_s( pchBuffer, nBufferLength, _T( "%d.%d.%d%c, x%d.%c.%c" ),
				CNavigator::VERSION.MAJOR,
				CNavigator::VERSION.MINOR,
				CNavigator::VERSION.BUILD,
				CNavigator::VERSION.LABEL,
				CNavigator::VERSION.INT_TYPE,
				( CNavigator::VERSION.BLD_TYPE == BLD_TYPE_DEBUG ) ? 'd' : 'r',
				( CNavigator::VERSION.BLD_TYPE == CHR_TYPE_ANSI ) ? 'a' : 'u' );

	return;
}


//*****************************************************************************
//
// End of File : CNavigator.cpp
//
//*****************************************************************************