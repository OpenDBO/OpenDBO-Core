//*****************************************************************************
// File       : CNavWorld_Sector.cpp
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

#include "CNavWorld.h"


//-----------------------------------------------------------------------------
// Name Space Table
//-----------------------------------------------------------------------------

USE_NAMESPACE( nav )


//-----------------------------------------------------------------------------
// Initialize Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Name			: GetSectorByPosition
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CNavSector * const CNavWorld::GetSectorByPosition( const CNavPosition & rcPosition1 ) const
{
	UInt32		nSectorIndex = GetSectorIndex( rcPosition1 );

	return GetSector( nSectorIndex );
}


//-----------------------------------------------------------------------------
// Name			: GetSectorByPosition
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CNavSector * const CNavWorld::GetSectorByPosition( const CNavPosition & rcPosition1, const CNavPosition & rcPosition2 ) const
{
	UInt32		nSectorIndex = GetSectorIndex( rcPosition1 );
	if( nSectorIndex != GetSectorIndex( rcPosition2 ) )
	{
		return NULL;
	}

	return GetSector( nSectorIndex );
}


//-----------------------------------------------------------------------------
// Name			: GetSectorIndex
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

UInt32 CNavWorld::GetSectorIndex( const CNavPosition & rcPosition ) const
{
	CNavPosition	cOffsetInWorld( rcPosition );
	cOffsetInWorld.Sub( m_cBounding.GetMin() );

	SNavIndex2D		sSectorIndex( cOffsetInWorld, m_cSectorSize );

	return sSectorIndex.GetIndex( m_sSectorCount );
}


//-----------------------------------------------------------------------------
// Name			: LinkSector
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CNavWorld::LinkSector( CNavSector * const pcSector )
{
	Assert( m_pcSectors != NULL );

	if( m_pcSectors->InsertAt( pcSector, pcSector->GetCellPosition() ) == true )
	{
		++m_nActiveSectorCount;

		return true;
	}
	else
	{
		return false;
	}
}


//-----------------------------------------------------------------------------
// Name			: UnlinkSector
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CNavWorld::UnlinkSector( CNavSector * const pcSector )
{
	Assert( m_pcSectors != NULL );

	if( m_pcSectors->RemoveAt( pcSector->GetCellPosition() ) == true )
	{
		--m_nActiveSectorCount;

		return true;
	}
	else
	{
		return false;
	}
}


//-----------------------------------------------------------------------------
// Name			: GetSector
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CNavSector * const CNavWorld::GetSector( UInt32 nSectorOrder ) const
{
	if( m_pcSectors == NULL )
	{
		return NULL;
	}

	return down_cast<CNavSector * const>(m_pcSectors->GetAt( nSectorOrder ));
}


//-----------------------------------------------------------------------------
// Name			: GetSector
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CNavSector * const CNavWorld::GetSector( const SNavIndex2D & rsIndex ) const
{
	return GetSector( rsIndex.GetIndex( m_sSectorCount ) );
}


//-----------------------------------------------------------------------------
// Name			: GetSector
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CNavSector * const CNavWorld::GetSector( const CNavPosition & rcPosition ) const
{
	return GetSector( GetSectorIndex( rcPosition ) );
}


//*****************************************************************************
//
// End of File : CNavWorld_Sector.cpp
//
//*****************************************************************************