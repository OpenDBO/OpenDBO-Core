//*****************************************************************************
// File       : CNavigator_World.cpp
// Desc       : 
// Author     : June C. Kim (cynis@hotmail.com)
// Update     : 2007. 1. 18
//*****************************************************************************

//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "StdAfx.h"

#include "CNavEntityVector.h"
#include "CNavPathEngine.h"

#include "CNavWorld.h"

#include "CNavigator.h"


//-----------------------------------------------------------------------------
// Name Space Table
//-----------------------------------------------------------------------------

USE_NAMESPACE( nav )


//-----------------------------------------------------------------------------
// Initialize Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Name			: CreateEntity
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CNavEntity * const CNavigator::CreateEntity( SNavEntityCreateInfo & rsCreateInfo, bool bInsertChild )
{
	if( bInsertChild == true )
	{
		if( rsCreateInfo.pcParent == NULL )
		{
			rsCreateInfo.pcParent = this;
		}
	}
	else
	{
		rsCreateInfo.pcParent = NULL;
	}

	CNavEntity *	pcEntity = CNavEntityFactory::GetInstance()->CreateEntity( rsCreateInfo );
	Assert( pcEntity != NULL );

	return pcEntity;
}


//-----------------------------------------------------------------------------
// Name			: DestroyEntity
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CNavigator::DestroyEntity( CNavEntity * pcEntity )
{
	Assert( pcEntity != NULL );

	CNavEntityFactory::GetInstance()->DestroyEntity( pcEntity );

	return;
}


//-----------------------------------------------------------------------------
// Name			: LinkWorld
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CNavigator::LinkWorld( CNavWorld * const pcWorld )
{
	Assert( pcWorld != NULL );
	Assert( pcWorld->GetType() == E_NAV_ENTITY_TYPE_WORLD );

	if( m_pcWorlds == NULL )
	{
		m_pcWorlds = new CNavEntityVector( true );
		Assert( m_pcWorlds != NULL );

		if( m_pcWorlds->Create( WORLD_LIST_SIZE_DEFAULT ) == false )
		{
			return false;
		}
	}

	return m_pcWorlds->Insert( pcWorld );
}


//-----------------------------------------------------------------------------
// Name			: UnlinkWorld
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CNavigator::UnlinkWorld( CNavWorld * const pcWorld )
{
	Assert( pcWorld != NULL );

	if( m_pcWorlds == NULL )
	{
		return false;
	}

	return m_pcWorlds->Remove( pcWorld );
}


//-----------------------------------------------------------------------------
// Name			: GetWorld
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CNavWorld * const CNavigator::GetWorld( UInt32 nOrder ) const
{
	if( m_pcWorlds == NULL )
	{
		return NULL;
	}

	return down_cast<CNavWorld * const>(m_pcWorlds->GetAt( nOrder ));
}


//*****************************************************************************
//
// End of File : CNavigator_World.cpp
//
//*****************************************************************************