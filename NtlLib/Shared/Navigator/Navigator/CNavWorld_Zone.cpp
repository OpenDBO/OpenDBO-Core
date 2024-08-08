//*****************************************************************************
// File       : CNavWorld_Zone.cpp
// Desc       : 
// Author     : June C. Kim (cynis@hotmail.com)
// Update     : 2007. 1. 18
//*****************************************************************************

//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "StdAfx.h"

#include "CNavEntityVector.h"
#include "CNavZone.h"

#include "CNavWorld.h"


//-----------------------------------------------------------------------------
// Name Space Table
//-----------------------------------------------------------------------------

USE_NAMESPACE( nav )


//-----------------------------------------------------------------------------
// Initialize Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Name			: LinkZone
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CNavWorld::LinkZone( CNavZone * const pcZone )
{
	Assert( m_pcZones != NULL );

	if( m_pcZones->Insert( pcZone ) == true )
	{
		++m_nActiveZoneCount;
		
		return true;
	}
	else
	{
		return false;
	}
}


//-----------------------------------------------------------------------------
// Name			: UnlinkZone
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CNavWorld::UnlinkZone( CNavZone * const pcZone )
{
	Assert( m_pcZones != NULL );

	if( m_pcZones->Remove( pcZone ) == true )
	{
		--m_nActiveZoneCount;
		
		return true;
	}
	else
	{
		return false;
	}
}


//-----------------------------------------------------------------------------
// Name			: GetZone
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CNavZone * const CNavWorld::GetZone( UInt32 nZoneOrder ) const
{
	if( m_pcZones == NULL )
	{
		return NULL;
	}

	return down_cast<CNavZone * const>(m_pcZones->GetAt( nZoneOrder ));
}


//*****************************************************************************
//
// End of File : CNavWorld_Zone.cpp
//
//*****************************************************************************