//*****************************************************************************
// File       : CNavZone.cpp
// Desc       : 
// Author     : June C. Kim (cynis@hotmail.com)
// Update     : 2007. 1. 18
//*****************************************************************************

//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "StdAfx.h"

#include "CNavEntityVector.h"

#include "CNavTerrain.h"

#include "CNavZone.h"


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

void CNavZone::Initialize( void )
{
	m_pcTerrain	= NULL;
	m_pcSectors	= NULL;

	return;
}


//-----------------------------------------------------------------------------
// Name			: Destroy
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CNavZone::Destroy( void )
{
	if( m_pcSectors != NULL )
	{
		delete m_pcSectors;
	}

	if( m_pcTerrain != NULL )
	{
		CNavEntityFactory::GetInstance()->DestroyEntity( m_pcTerrain );
	}

	m_cSections.Destroy( true );
	m_cAreas.Destroy( true );
	m_cPortals.Destroy( true );

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

bool CNavZone::Create( SNavEntityCreateInfo & rsCreateInfo )
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

		case E_NAV_CREATE_TYPE_MEMORY:
		{
			Assert( !"Unsupported Create Type" );

			break;
		}

		default:
		{
			Assert( !"Undefined Create Type" );

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

bool CNavZone::InsertChild( CNavEntity * const pcEntity )
{
	Assert( pcEntity != NULL );

	switch( pcEntity->GetType() )
	{
		case E_NAV_ENTITY_TYPE_SECTOR:
		{
			return LinkSector( down_cast<CNavSector * const>(pcEntity) );
		}

		case E_NAV_ENTITY_TYPE_TERRAIN:
		{
			return LinkTerrain( down_cast<CNavTerrain * const>(pcEntity) );
		}

		case E_NAV_ENTITY_TYPE_SECTION:
		{
			return LinkSection( down_cast<CNavSection * const>(pcEntity) );
		}

		case E_NAV_ENTITY_TYPE_AREA:
		{
			return LinkArea( down_cast<CNavArea * const>(pcEntity) );
		}

		case E_NAV_ENTITY_TYPE_PORTAL:
		{
			return LinkPortal( down_cast<CNavPortal * const>(pcEntity) );
		}

		case E_NAV_ENTITY_TYPE_NAVIGATOR:
		case E_NAV_ENTITY_TYPE_WORLD:
		case E_NAV_ENTITY_TYPE_ZONE:
		case E_NAV_ENTITY_TYPE_HEIGHT_MAP:
		{
			Assert( !"Unsupported Match" );

			return false;
		}

		case E_NAV_ENTITY_TYPE_PATH:
		case E_NAV_ENTITY_TYPE_AGENT:
		case E_NAV_ENTITY_TYPE_OBJECT:
		{
			Assert( !"Not Supported Yet" );

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

bool CNavZone::RemoveChild( CNavEntity * const pcEntity )
{
	switch( pcEntity->GetType() )
	{
		case E_NAV_ENTITY_TYPE_SECTOR:
		{
			return UnlinkSector( down_cast<CNavSector * const>(pcEntity) );
		}

		case E_NAV_ENTITY_TYPE_TERRAIN:
		{
			return UnlinkTerrain( down_cast<CNavTerrain * const>(pcEntity) );
		}

		case E_NAV_ENTITY_TYPE_SECTION:
		{
			return UnlinkSection( down_cast<CNavSection * const>(pcEntity) );
		}

		case E_NAV_ENTITY_TYPE_AREA:
		{
			return UnlinkArea( down_cast<CNavArea * const>(pcEntity) );
		}

		case E_NAV_ENTITY_TYPE_PORTAL:
		{
			return UnlinkPortal( down_cast<CNavPortal * const>(pcEntity) );
		}

		case E_NAV_ENTITY_TYPE_NAVIGATOR:
		case E_NAV_ENTITY_TYPE_WORLD:
		case E_NAV_ENTITY_TYPE_ZONE:
		case E_NAV_ENTITY_TYPE_HEIGHT_MAP:
		{
			Assert( !"Unsupported Match" );

			return false;
		}

		case E_NAV_ENTITY_TYPE_PATH:
		case E_NAV_ENTITY_TYPE_AGENT:
		case E_NAV_ENTITY_TYPE_OBJECT:
		{
			Assert( !"Not Supported Yet" );

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
// Name			: FindUnblockedPosition
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CNavZone::FindUnblockedPosition( CNavPosition & rcSearchPosition, Fixed32 nDistance, UId32 nShapeId ) const
{
	if( m_pcTerrain == NULL )
	{
		return false;
	}

	return m_pcTerrain->FindUnblockedPosition( rcSearchPosition, nDistance, nShapeId );
}


//-----------------------------------------------------------------------------
// Name			: PreProcess
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CNavZone::PreProcess( SNavEntityPreProcessInfo & rsPreProcessInfo )
{
	bool	bResult = true;

	SNavEntityPreProcessInfo	sPreProcessInfo( rsPreProcessInfo );
	sPreProcessInfo.cPath.AppendFormat( _T( "\\%s" ), GetName().GetBuffer() );

	if( m_pcTerrain != NULL )
	{
		bResult = m_pcTerrain->PreProcess( sPreProcessInfo );
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

bool CNavZone::PostProcess( SNavEntityPreProcessInfo & rsPostProcessInfo )
{
	bool	bResult = true;

	if( m_pcTerrain != NULL )
	{
		bResult = m_pcTerrain->PostProcess( rsPostProcessInfo );
	}

	if( m_pcSectors != NULL )
	{
		bResult &= m_pcSectors->PostProcess( rsPostProcessInfo );
	}

	return bResult;
}


//-----------------------------------------------------------------------------
// Name			: Render
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

Int32 CNavZone::Render( CNavRenderer * const pcRenderer ) const
{
	if( m_pcTerrain == NULL )
	{
		return 0L;
	}

	return m_pcTerrain->Render( pcRenderer );
}


//-----------------------------------------------------------------------------
// Name			: GetHeight
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CNavZone::GetHeight( CNavPosition & rcPosition ) const
{
	UNREFERENCED_PARAMETER( rcPosition );

	Assert( !"Use Shortcut Path (CNavWorld->CNavSector->CNavHeightMap)" );

	return false;
}


//*****************************************************************************
//
// End of File : CNavZone.cpp
//
//*****************************************************************************