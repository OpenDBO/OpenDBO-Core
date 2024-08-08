//*****************************************************************************
// File       : CNavEntityFactory.cpp
// Desc       : 
// Author     : June C. Kim (cynis@hotmail.com)
// Update     : 2007. 1. 18
//*****************************************************************************

//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "StdAfx.h"

#include "CNavObject.h"
#include "CNavPortal.h"
#include "CNavTerrain.h"
#include "CNavHeightMap.h"
#include "CNavSector.h"
#include "CNavArea.h"
#include "CNavSection.h"
#include "CNavZone.h"
#include "CNavWorld.h"
#include "CNavigator.h"

#include "CNavEntityNode.h"

#include "CNavEntityFactory.h"


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

void CNavEntityFactory::Initialize( void )
{
	return;
}


//-----------------------------------------------------------------------------
// Name			: Create
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CNavEntityFactory::Create( void )
{
	m_cRegisteredTable.Reserve( ENTITY_REGISTER_TABLE_SIZE_DEFAULT );

	return true;
}


//-----------------------------------------------------------------------------
// Name			: Create
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CNavEntityFactory::Destroy( void )
{
	m_cRegisteredTable.RemoveAll();

	return;
}


//-----------------------------------------------------------------------------
// Name			: RegisterEntity
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CNavEntityFactory::RegisterEntity( CNavEntity * pcEntity )
{
	Assert( pcEntity != NULL );

	CFixedStringListNode *	pcNode = m_cRegisteredTable.GetNode( pcEntity->GetName() );
	if( pcNode != NULL )
	{
		Assert( !"Already Registered Entity" );

		return false;
	}

	m_cRegisteredTable.Insert( *pcEntity );

	return true;
}


//-----------------------------------------------------------------------------
// Name			: UnregisterEntity
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CNavEntityFactory::UnregisterEntity( CNavEntity * pcEntity )
{
	Assert( pcEntity != NULL );

	m_cRegisteredTable.Remove( *pcEntity );

	return true;
}


//-----------------------------------------------------------------------------
// Name			: GetEntity
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

const CNavEntity & CNavEntityFactory::GetEntity( const CFixedString & rcEntityName ) const
{
	CNavEntityNode * const	pcEntityNode = down_cast<CNavEntityNode * const>(m_cRegisteredTable.GetNode( rcEntityName ));
	if( pcEntityNode != NULL )
	{
		return pcEntityNode->GetEntity();
	}

	return CNavEntity::NULL_ENTITY;
}


//-----------------------------------------------------------------------------
// Name			: CreateEntity
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CNavEntity * CNavEntityFactory::CreateEntity( SNavEntityCreateInfo & rsCreateInfo )
{
	CNavEntity *	pcEntity;

	switch( rsCreateInfo.eEntityType )
	{
		case E_NAV_ENTITY_TYPE_NAVIGATOR:
		{
			pcEntity = new CNavigator( rsCreateInfo.nUniqueId );

			break;
		}

		case E_NAV_ENTITY_TYPE_WORLD:
		{
			pcEntity = new CNavWorld( rsCreateInfo.nUniqueId );

			break;
		}

		case E_NAV_ENTITY_TYPE_ZONE:
		{
			pcEntity = new CNavZone( rsCreateInfo.nUniqueId );

			break;
		}

		case E_NAV_ENTITY_TYPE_SECTION:
		{
			pcEntity = new CNavSection( rsCreateInfo.nUniqueId );

			break;
		}

		case E_NAV_ENTITY_TYPE_AREA:
		{
			pcEntity = new CNavArea( rsCreateInfo.nUniqueId );

			break;
		}

		case E_NAV_ENTITY_TYPE_SECTOR:
		{
			pcEntity = new CNavSector( rsCreateInfo.nUniqueId );

			break;
		}

		case E_NAV_ENTITY_TYPE_HEIGHT_MAP:
		{
			pcEntity = new CNavHeightMap( rsCreateInfo.nUniqueId );

			break;
		}

		case E_NAV_ENTITY_TYPE_TERRAIN:
		{
			pcEntity = new CNavTerrain( rsCreateInfo.nUniqueId );

			break;
		}

		case E_NAV_ENTITY_TYPE_PORTAL:
		{
			pcEntity = new CNavPortal( rsCreateInfo.nUniqueId );

			break;
		}

		case E_NAV_ENTITY_TYPE_OBJECT:
		{
			pcEntity = new CNavObject( rsCreateInfo.nUniqueId );

			break;
		}

		default:
		{
			return NULL;
		}
	}

	Assert( pcEntity != NULL );

	if( pcEntity->Create( rsCreateInfo ) == false )
	{
		Assert( !"CEntity::Create() Fail" );

		delete pcEntity;
		pcEntity = NULL;

		return pcEntity;
	}

	if( RegisterEntity( pcEntity ) == false )
	{
		Assert( !"CEntity::RegisterEntity() Fail" );
	}

	if( pcEntity->PostCreate( rsCreateInfo ) == false )
	{
		Assert( !"CEntity::PostCreate() Fail" );

		delete pcEntity;
		pcEntity = NULL;
	}

	return pcEntity;
}


//-----------------------------------------------------------------------------
// Name			: DestroyEntity
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CNavEntityFactory::DestroyEntity( CNavEntity * pcEntity )
{
	Assert( pcEntity != NULL );

	if( UnregisterEntity( pcEntity ) == false )
	{
		Assert( !"CEntity::UnregisterEntity() Fail" );
	}

	delete pcEntity;

	return;
}


//*****************************************************************************
//
// End of File : CNavEntityFactory.cpp
//
//*****************************************************************************