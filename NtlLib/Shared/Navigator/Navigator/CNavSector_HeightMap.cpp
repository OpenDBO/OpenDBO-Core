//*****************************************************************************
// File       : CNavSector_Terrain.cpp
// Desc       : 
// Author     : June C. Kim (cynis@hotmail.com)
// Update     : 2007. 1. 18
//*****************************************************************************

//------------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "StdAfx.h"

#include "CNavEntityVector.h"

#include "CNavTerrain.h"
#include "CNavHeightMap.h"

#include "CNavSector.h"


//-----------------------------------------------------------------------------
// Name Space Table
//-----------------------------------------------------------------------------

USE_NAMESPACE( nav )


//-----------------------------------------------------------------------------
// Initialize Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Name			: LinkTerrain
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CNavSector::LinkTerrain( CNavTerrain * const pcTerrain )
{
	Assert( pcTerrain != NULL );
	Assert( pcTerrain->GetType() == E_NAV_ENTITY_TYPE_TERRAIN );

	m_pcTerrain = pcTerrain;

	return true;
}


//-----------------------------------------------------------------------------
// Name			: UnlinkTerrain
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CNavSector::UnlinkTerrain( CNavTerrain * const pcTerrain )
{
	if( m_pcTerrain != pcTerrain )
	{
		return false;
	}

	m_pcTerrain = NULL;

	return true;
}


//-----------------------------------------------------------------------------
// Name			: LinkHeightMap
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CNavSector::LinkHeightMap( CNavHeightMap * const pcHeightMap )
{
	Assert( pcHeightMap != NULL );
	Assert( pcHeightMap->GetType() == E_NAV_ENTITY_TYPE_HEIGHT_MAP );

	m_pcHeightMap = pcHeightMap;

	return true;
}


//-----------------------------------------------------------------------------
// Name			: UnlinkHeightMap
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CNavSector::UnlinkHeightMap( CNavHeightMap * const pcHeightMap )
{
	if( m_pcHeightMap != pcHeightMap )
	{
		return false;
	}

	m_pcHeightMap = NULL;

	return true;
}


//-----------------------------------------------------------------------------
// Name			: GetHeight
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CNavSector::GetHeight( CNavPosition & rcPosition ) const
{
	if( m_pcHeightMap == NULL )
	{
		return false;
	}

	return m_pcHeightMap->GetHeight( rcPosition );
}


//-----------------------------------------------------------------------------
// Name			: GetAttribute
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

DWORD CNavSector::GetAttribute( CNavPosition & rcPosition ) const
{
	if( m_pcHeightMap == NULL )
	{
		return E_NAV_TILE_ATTRIBUTE_NONE;
	}

	return m_pcHeightMap->GetAttribute( rcPosition );
}


//*****************************************************************************
//
// End of File : CNavSector_Terrain.cpp
//
//*****************************************************************************