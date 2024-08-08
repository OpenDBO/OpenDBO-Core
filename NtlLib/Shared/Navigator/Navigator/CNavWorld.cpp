//*****************************************************************************
// File       : CNavWorld.cpp
// Desc       : 
// Author     : June C. Kim (cynis@hotmail.com)
// Update     : 2007. 1. 18
//*****************************************************************************

//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "StdAfx.h"

#include "CNavEntityFactory.h"
#include "CNavEntityVector.h"

#include "CNavigator.h"
#include "CNavZone.h"
#include "CNavSector.h"

#include "CNavWorld.h"


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

void CNavWorld::Initialize( void )
{
	m_eWorldType	= E_NAV_WORLD_TYPE_NONE;

	m_pcSectors		= NULL;
	m_sSectorCount.Reset();
	m_nActiveSectorCount	= 0;

	m_pcZones	= NULL;
	m_nActiveZoneCount	= 0;

	m_cSectorSize.Reset();
	m_cOverlapSize.Reset();

	return;
}


//-----------------------------------------------------------------------------
// Name			: Destroy
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CNavWorld::Destroy( void )
{
	if( m_pcSectors != NULL )
	{
		delete m_pcSectors;
	}

	if( m_pcZones != NULL )
	{
		delete m_pcZones;
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

bool CNavWorld::Create( SNavEntityCreateInfo & rsCreateInfo )
{
	if( CNavEntity::Create( rsCreateInfo ) == false )
	{
		return false;
	}

	m_eWorldType = rsCreateInfo.uSubType.eWorldType;

	switch( rsCreateInfo.eCreateType )
	{
		case E_NAV_CREATE_TYPE_FILE:
		{
			if( rsCreateInfo.cName.GetLength() > 0 )
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

bool CNavWorld::InsertChild( CNavEntity * const pcEntity )
{
	Assert( pcEntity != NULL );

	switch( pcEntity->GetType() )
	{
		case E_NAV_ENTITY_TYPE_ZONE:
		{
			return LinkZone( down_cast<CNavZone * const>(pcEntity) );
		}

		case E_NAV_ENTITY_TYPE_SECTOR:
		{
			return LinkSector( down_cast<CNavSector * const>(pcEntity) );
		}

		case E_NAV_ENTITY_TYPE_NAVIGATOR:
		case E_NAV_ENTITY_TYPE_WORLD:
		case E_NAV_ENTITY_TYPE_TERRAIN:
		case E_NAV_ENTITY_TYPE_HEIGHT_MAP:
		{
			Assert( !"Unsupported Match" );

			return false;
		}

		case E_NAV_ENTITY_TYPE_OBJECT:
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

bool CNavWorld::RemoveChild( CNavEntity * const pcEntity )
{
	switch( pcEntity->GetType() )
	{
		case E_NAV_ENTITY_TYPE_ZONE:
		{
			return UnlinkZone( down_cast<CNavZone * const>(pcEntity) );
		}

		case E_NAV_ENTITY_TYPE_SECTOR:
		{
			return UnlinkSector( down_cast<CNavSector * const>(pcEntity) );
		}

		case E_NAV_ENTITY_TYPE_NAVIGATOR:
		case E_NAV_ENTITY_TYPE_WORLD:
		case E_NAV_ENTITY_TYPE_TERRAIN:
		case E_NAV_ENTITY_TYPE_HEIGHT_MAP:
		{
			Assert( !"Unsupported Match" );

			return false;
		}

		case E_NAV_ENTITY_TYPE_OBJECT:
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

CNavEntity * const CNavWorld::GetChildByUniqueId( UId32 nChildId ) const
{
	if( m_pcSectors == NULL )
	{
		return NULL;
	}

	return m_pcSectors->GetAt( m_pcSectors->SearchChildByUniqueId( nChildId ) );
}


//-----------------------------------------------------------------------------
// Name			: GetChildByPosition
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CNavEntity * const CNavWorld::GetChildByPosition( const CNavPosition & rcPosition1 ) const
{
	if( m_pcSectors == NULL )
	{
		return NULL;
	}

	// Get Sector

	CNavEntity *	pcSector = GetSector( rcPosition1 );

	if( pcSector == NULL )
	{
		return NULL;
	}

	CNavEntity *	pcZone = pcSector->GetParent();

	if( pcZone == NULL )
	{
		return NULL;
	}

	return pcZone;
}


//-----------------------------------------------------------------------------
// Name			: GetChildByPosition
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CNavEntity * const CNavWorld::GetChildByPosition( const CNavPosition & rcPosition1, const CNavPosition & rcPosition2 ) const
{
	if( m_pcSectors == NULL )
	{
		return NULL;
	}

	CNavEntity *	pcZone1 = GetChildByPosition( rcPosition1 );
	if( pcZone1 == NULL )
	{
		return NULL;
	}

	CNavEntity *	pcZone2 = GetChildByPosition( rcPosition2 );
	if( pcZone2 == NULL )
	{
		return NULL;
	}

	if( pcZone1 != pcZone2 )
	{
		return NULL;
	}

	return pcZone1;
}


//-----------------------------------------------------------------------------
// Name			: GetHeight
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CNavWorld::GetHeight( CNavPosition & rcPosition ) const
{
	const CNavSector *	pcSector = GetSector( rcPosition );
	if( pcSector == NULL )
	{
		return false;
	}

	return pcSector->GetHeight( rcPosition );
}


//-----------------------------------------------------------------------------
// Name			: GetAttribute
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

DWORD CNavWorld::GetAttribute( CNavPosition & rcPosition ) const
{
	const CNavSector *	pcSector = GetSector( rcPosition );
	if( pcSector == NULL )
	{
		return E_NAV_TILE_ATTRIBUTE_NONE;
	}

	return pcSector->GetAttribute( rcPosition );
}


//-----------------------------------------------------------------------------
// Name			: FindUnblockedPosition
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CNavWorld::FindUnblockedPosition( CNavPosition & rcSearchPosition, Fixed32 nDistance, UId32 nShapeId ) const
{
	Assert( this != NULL );

	// Fine Correct Sector

	const CNavSector *	pcSector = GetSectorByPosition( rcSearchPosition );
	if( pcSector == NULL )
	{
		return false;
	}

	// Fine Correct Zone

	const CNavZone *	pcZone = down_cast<const CNavZone *>(pcSector->GetParent());
	if( pcZone == NULL )
	{
		return false;
	}

	bool bResult = pcZone->FindUnblockedPosition( rcSearchPosition, nDistance, nShapeId );

	if( bResult == true )
	{
		if( GetHeight( rcSearchPosition ) == true )
		{
			return true;
		}
	}

	return false;
}


//-----------------------------------------------------------------------------
// Name			: PreProcess
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CNavWorld::PreProcess( SNavEntityPreProcessInfo & rsPreProcessInfo )
{
	if( m_pcZones != NULL )
	{
		rsPreProcessInfo.cPath.AppendFormat( _T( "\\%s" ), GetName().GetBuffer() );

		return m_pcZones->PreProcess( rsPreProcessInfo );
	}
	else
	{
		return true;
	}
}


//-----------------------------------------------------------------------------
// Name			: PostProcess
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CNavWorld::PostProcess( SNavEntityPreProcessInfo & rsPostProcessInfo )
{
	if( m_pcZones != NULL )
	{
		return m_pcZones->PostProcess( rsPostProcessInfo );
	}
	else
	{
		return true;
	}
}


//-----------------------------------------------------------------------------
// Name			: Render
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

Int32 CNavWorld::Render( CNavRenderer * const pcRenderer ) const
{
	if( m_pcZones == NULL )
	{
		return 0;
	}

	return m_pcZones->Render( pcRenderer );
}


//-----------------------------------------------------------------------------
// Name			: GetHierarchy
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CNavWorld::GetHierarchy( CNavEntityHierarchy & rcHierarchy, const CNavPosition & rcPosition ) const
{
	rcHierarchy.SetZone( GetChildByPosition( rcPosition ) );
	rcHierarchy.SetSector( GetSectorByPosition( rcPosition ) );

	if( rcHierarchy.GetZone() == NULL )
	{
		return false;
	}

	if( rcHierarchy.GetSector() == NULL )
	{
		return false;
	}

	return rcHierarchy.GetSector()->GetHierarchy( rcHierarchy, rcPosition );
}


//-----------------------------------------------------------------------------
// Name			: DetectLineCollisionWithMultipleZone
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CNavWorld::DetectLineCollisionWithMultipleZone( const CNavPosition & rcBeginPosition, const CNavPosition & rcEndPosition, UId32 nShapeId ) const
{
	UNREFERENCED_PARAMETER( rcBeginPosition );
	UNREFERENCED_PARAMETER( rcEndPosition );
	UNREFERENCED_PARAMETER( nShapeId );

	Assert( !"CNavWorld::DetectLineCollisionWithMultipleZone() is not supported in this time" );

	return true;
}


//*****************************************************************************
//
// End of File : CNavWorld.cpp
//
//*****************************************************************************