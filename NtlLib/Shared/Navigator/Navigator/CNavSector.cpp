//*****************************************************************************
// File       : CNavSector.cpp
// Desc       : 
// Author     : June C. Kim (cynis@hotmail.com)
// Update     : 2007. 1. 18
//*****************************************************************************

//------------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "StdAfx.h"
#include "CNavSector.h"

#include "CNavPathEngine.h"
#include "CNavEntityFactory.h"
#include "CNavRenderer.h"
#include "CNavEntityVector.h"

#include "CNavZone.h"
#include "CNavTerrain.h"
#include "CNavHeightMap.h"

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

void CNavSector::Initialize( void )
{
	m_pcTerrain		= NULL;
	m_pcHeightMap	= NULL;
	m_pcAgents		= NULL;

	m_nSectionOrder	= 0;
	m_nAreaOrder	= 0;

	return;
}


//-----------------------------------------------------------------------------
// Name			: Destroy
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CNavSector::Destroy( void )
{
	if( m_pcTerrain != NULL )
	{
		m_pcTerrain = NULL;
	}

	if( m_pcHeightMap != NULL )
	{
		CNavEntityFactory::GetInstance()->DestroyEntity( m_pcHeightMap );
	}

	Initialize();

	return;
}


//-----------------------------------------------------------------------------
// Name			: Create
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CNavSector::Create( SNavEntityCreateInfo & rsCreateInfo )
{
	if( CNavEntity::Create( rsCreateInfo ) == false )
	{
		return false;
	}

	if( CreateByDirect( rsCreateInfo ) == false )
	{
		return false;
	}

	return true;
}


//-----------------------------------------------------------------------------
// Name			: CreateByDirect
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CNavSector::CreateByDirect( SNavEntityCreateInfo & rsCreateInfo )
{
	SNavSectorCreateInfo *		psSubCreateInfo = reinterpret_cast<SNavSectorCreateInfo *>(rsCreateInfo.pvUserData);
	m_sCellPosition.Set( psSubCreateInfo->sCellPosition );

	m_nSectionOrder = psSubCreateInfo->nSectionOrder;
	m_nAreaOrder = psSubCreateInfo->nAreaOrder;

	SNavEntityCreateInfo	sCreateInfo( rsCreateInfo );

	if( rsCreateInfo.pcParent != NULL )
	{
		sCreateInfo.nUniqueId				= ( E_NAV_ENTITY_TYPE_HEIGHT_MAP << 24 ) |
												( rsCreateInfo.nUniqueId & 0x00FFFFFF );
		sCreateInfo.eEntityType				= E_NAV_ENTITY_TYPE_HEIGHT_MAP;
		sCreateInfo.uSubType.eHeightMapType	= E_NAV_HEIGHT_MAP_TYPE_NORMAL;
		sCreateInfo.eCreateType				= E_NAV_CREATE_TYPE_FILE;
		sCreateInfo.pvUserData				= NULL;
		sCreateInfo.nUserDataSize			= sCreateInfo.nUserDataSize	= MakeFlags( rsCreateInfo.nFlags );
		sCreateInfo.pcParent				= this;
		sCreateInfo.nFlags					= rsCreateInfo.nFlags;
		sCreateInfo.cBounding				= rsCreateInfo.cBounding;
		sCreateInfo.cPath					= rsCreateInfo.cPath;
		sCreateInfo.cName.Format( _T( "%s%s%d%s%d%s" ),
									psSubCreateInfo->cPrefix.GetBuffer(),
									psSubCreateInfo->cSeparator.GetBuffer(),
									psSubCreateInfo->sCellPosition.sIndex.GetX(),
									psSubCreateInfo->cSeparator.GetBuffer(),
									psSubCreateInfo->sCellPosition.sIndex.GetY(),
									psSubCreateInfo->cPostfix.GetBuffer() );

		m_pcHeightMap = down_cast<CNavHeightMap *>(CNavEntityFactory::GetInstance()->CreateEntity( sCreateInfo ));
		Assert( m_pcHeightMap != NULL );

		if( rsCreateInfo.nFlags & E_NAV_ENTITY_CREATE_INFO_FLAG_EXPORT_MODE )
		{	
			m_pcTerrain = NULL;
		}
		else
		{
			m_pcTerrain = down_cast<CNavZone *>(rsCreateInfo.pcParent)->GetTerrain();
			Assert( m_pcTerrain != NULL );
		}
	}

	return true;
}


//-----------------------------------------------------------------------------
// Name			: PostCreate
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CNavSector::PostCreate( SNavEntityCreateInfo & rsCreateInfo )
{
	if( CNavEntity::PostCreate( rsCreateInfo ) == false )
	{
		return false;
	}

	if( GetParent() != NULL && GetParent()->GetType() == E_NAV_ENTITY_TYPE_ZONE )
	{
		CNavEntity *	pcGrandParent = GetParent()->GetParent();

		if( pcGrandParent != NULL && pcGrandParent->GetType() == E_NAV_ENTITY_TYPE_WORLD )
		{
			down_cast<CNavWorld *>(pcGrandParent)->InsertChild( this );
		}
	}

	return true;
}


//-----------------------------------------------------------------------------
// Name			: GetSection
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CNavSection * const CNavSector::GetSection( void ) const
{
	if( GetParent() == NULL )
	{
		return NULL;
	}

	return down_cast<CNavZone *>(GetParent())->GetSection( m_nSectionOrder );
}


//-----------------------------------------------------------------------------
// Name			: GetArea
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CNavArea * const CNavSector::GetArea( void ) const
{
	if( GetParent() == NULL )
	{
		return NULL;
	}

	return down_cast<CNavZone *>(GetParent())->GetArea( m_nAreaOrder );
}


//-----------------------------------------------------------------------------
// Name			: InsertChild
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CNavSector::InsertChild( CNavEntity * const pcEntity )
{
	switch( pcEntity->GetType() )
	{
		case E_NAV_ENTITY_TYPE_HEIGHT_MAP:
		{
			return LinkHeightMap( down_cast<CNavHeightMap * const>(pcEntity) );
		}

		case E_NAV_ENTITY_TYPE_NAVIGATOR:
		case E_NAV_ENTITY_TYPE_WORLD:
		case E_NAV_ENTITY_TYPE_ZONE:
		case E_NAV_ENTITY_TYPE_TERRAIN:
		case E_NAV_ENTITY_TYPE_OBJECT:
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

bool CNavSector::RemoveChild( CNavEntity * const pcEntity )
{
	switch( pcEntity->GetType() )
	{
		case E_NAV_ENTITY_TYPE_HEIGHT_MAP:
		{
			return UnlinkHeightMap( down_cast<CNavHeightMap * const>(pcEntity) );
		}

		case E_NAV_ENTITY_TYPE_NAVIGATOR:
		case E_NAV_ENTITY_TYPE_WORLD:
		case E_NAV_ENTITY_TYPE_ZONE:
		case E_NAV_ENTITY_TYPE_TERRAIN:
		case E_NAV_ENTITY_TYPE_OBJECT:
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
// Name			: GetChildByPosition
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CNavEntity * const CNavSector::GetChildByUniqueId( UId32 nUniqueId ) const
{
	if( m_pcTerrain == NULL )
	{
		return NULL;
	}

	if( m_pcTerrain->GetUniqueId() != nUniqueId )
	{
		return NULL;
	}

	return m_pcTerrain;
}



//-----------------------------------------------------------------------------
// Name			: GetChildByPosition
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CNavEntity * const CNavSector::GetChildByPosition( const CNavPosition & rcPosition1 ) const
{
	if( m_pcTerrain == NULL )
	{
		return NULL;
	}

	if( m_pcTerrain->IsInBound( rcPosition1 ) == false )
	{
		return NULL;
	}

	return m_pcTerrain;
}


//-----------------------------------------------------------------------------
// Name			: GetChildByPosition
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CNavEntity * const CNavSector::GetChildByPosition( const CNavPosition & rcPosition1, const CNavPosition & rcPosition2 ) const
{
	if( m_pcTerrain == NULL )
	{
		return NULL;
	}

	if( m_pcTerrain->IsInBound( rcPosition1 ) == false )
	{
		return NULL;
	}

	if( m_pcTerrain->IsInBound( rcPosition2 ) == false )
	{
		return NULL;
	}

	return m_pcTerrain;
}


//-----------------------------------------------------------------------------
// Name			: Render
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

Int32 CNavSector::Render( CNavRenderer * const pcRenderer ) const
{
	Assert( pcRenderer != NULL );

	CNavPosition	cFromPosition( m_cBounding.GetMin() );
	CNavPosition	cToPosition( m_cBounding.GetMax().GetX(), m_cBounding.GetMin().GetY(), m_cBounding.GetMin().GetZ() );

	pcRenderer->RenderLine( cFromPosition, cToPosition, 0x0000FFFF, 5 );

	cFromPosition.Set( cToPosition );
	cToPosition.Set( m_cBounding.GetMax().GetX(), m_cBounding.GetMax().GetY(), m_cBounding.GetMin().GetZ() );

	pcRenderer->RenderLine( cFromPosition, cToPosition, 0x0000FFFF, 5 );

	cFromPosition.Set( cToPosition );
	cToPosition.Set( m_cBounding.GetMin().GetX(), m_cBounding.GetMax().GetY(), m_cBounding.GetMin().GetZ() );

	pcRenderer->RenderLine( cFromPosition, cToPosition, 0x0000FFFF, 5 );

	cFromPosition.Set( cToPosition );
	cToPosition.Set( m_cBounding.GetMin().GetX(), m_cBounding.GetMin().GetY(), m_cBounding.GetMin().GetZ() );

	pcRenderer->RenderLine( cFromPosition, cToPosition, 0x0000FFFF, 5 );

	if( m_pcTerrain != NULL )
	{
		return m_pcTerrain->Render( pcRenderer ) + 1L;
	}

	return 1L;
}


//-----------------------------------------------------------------------------
// Name			: GetHierarchy
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CNavSector::GetHierarchy( CNavEntityHierarchy & rcHierarchy, const CNavPosition & rcPosition ) const
{
	UNREFERENCED_PARAMETER( rcPosition );

	rcHierarchy.SetSection( up_cast<CNavEntity *>(GetSection()) );
	rcHierarchy.SetArea( up_cast<CNavEntity *>(GetArea()) );

	if( rcHierarchy.GetSection() == NULL || rcHierarchy.GetArea() == NULL )
	{
		return false;
	}

	return true;
}


//*****************************************************************************
//
// End of File : CNavSector.cpp
//
//*****************************************************************************