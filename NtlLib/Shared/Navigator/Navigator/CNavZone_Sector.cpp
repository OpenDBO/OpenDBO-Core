//*****************************************************************************
// File       : CNavZone_Sector.cpp
// Desc       : 
// Author     : June C. Kim (cynis@hotmail.com)
// Update     : 2007. 1. 18
//*****************************************************************************

//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "StdAfx.h"

#include "CNavEntityVector.h"

#include "CNavSector.h"

#include "CNavZone.h"


//-----------------------------------------------------------------------------
// Name Space Table
//-----------------------------------------------------------------------------

USE_NAMESPACE( nav )


//-----------------------------------------------------------------------------
// Initialize Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Name			: GetSector
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CNavSector * const CNavZone::GetSector( UInt32 nSectorOrder ) const
{
	if( m_pcSectors == NULL )
	{
		return NULL;
	}

	return down_cast<CNavSector *>(m_pcSectors->GetAt( nSectorOrder ));
}


//-----------------------------------------------------------------------------
// Name			: LinkSector
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CNavZone::LinkSector( CNavSector * const pcSector )
{
	Assert( m_pcSectors != NULL );

	return m_pcSectors->Insert( pcSector );
}


//-----------------------------------------------------------------------------
// Name			: UnlinkSector
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CNavZone::UnlinkSector( CNavSector * const pcSector )
{
	Assert( m_pcSectors != NULL );

	return m_pcSectors->Remove( pcSector );
}


//-----------------------------------------------------------------------------
// Name			: LinkTerrain
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CNavZone::LinkTerrain( CNavTerrain * const pcTerrain )
{
	Assert( m_pcTerrain == NULL );

	m_pcTerrain = pcTerrain;

	return false;
}


//-----------------------------------------------------------------------------
// Name			: UnlinkTerrain
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CNavZone::UnlinkTerrain( CNavTerrain * const pcTerrain )
{
	if( pcTerrain == NULL )
	{
		m_pcTerrain = NULL;

		return true;
	}

	if( m_pcTerrain == pcTerrain )
	{
		m_pcTerrain = NULL;

		return true;
	}

	return false;
}


//*****************************************************************************
//
// End of File : CNavZone_Sector.cpp
//
//*****************************************************************************