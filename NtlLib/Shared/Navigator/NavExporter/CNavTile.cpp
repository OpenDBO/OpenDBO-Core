//*****************************************************************************
// File       : CNavTile.cpp
// Desc       : 
// Author     : June C. Kim (cynis@hotmail.com)
// Update     : 2007. 1. 18
//*****************************************************************************

//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "StdAfx.h"

#include "CNavTile.h"


//-----------------------------------------------------------------------------
// Initialize Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Name			: Create
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

UInt32 CNavTile::Create( SNavTileCreateInfo & rsCreateInfo )
{
	m_eDivideType = rsCreateInfo.eDivideType;
	SetLock( rsCreateInfo.bLocked );
	UInt32		nFaceCreateCount = 0;

	m_nHeight = rsCreateInfo.nLeftTop;

	if( m_eDivideType == E_NAV_TILE_DIVIDE_TYPE_DOWN_LEFT )
	{
		m_sSlope[E_NAV_TILE_FACE_TYPE_LEFT].Set( GetFixed32Ratio( rsCreateInfo.nRightTop - rsCreateInfo.nLeftTop, rsCreateInfo.nWidth ),
												GetFixed32Ratio( rsCreateInfo.nLeftBottom - rsCreateInfo.nLeftTop, rsCreateInfo.nHeight ) );
		m_sSlope[E_NAV_TILE_FACE_TYPE_RIGHT].Set( GetFixed32Ratio( rsCreateInfo.nRightBottom - rsCreateInfo.nLeftBottom,rsCreateInfo.nWidth ),
												GetFixed32Ratio( rsCreateInfo.nRightBottom - rsCreateInfo.nRightTop,rsCreateInfo.nHeight ) );
	}
	else
	{
		m_sSlope[E_NAV_TILE_FACE_TYPE_LEFT].Set( GetFixed32Ratio( rsCreateInfo.nRightBottom - rsCreateInfo.nLeftBottom, rsCreateInfo.nWidth ),
												GetFixed32Ratio( rsCreateInfo.nLeftBottom - rsCreateInfo.nLeftTop, rsCreateInfo.nHeight ) );
		m_sSlope[E_NAV_TILE_FACE_TYPE_RIGHT].Set( GetFixed32Ratio( rsCreateInfo.nRightTop - rsCreateInfo.nLeftTop, rsCreateInfo.nWidth ),
												GetFixed32Ratio( rsCreateInfo.nRightBottom - rsCreateInfo.nRightTop, rsCreateInfo.nHeight ) );
	}

	if( rsCreateInfo.bFiltered == true ||
		rsCreateInfo.nAttribute & E_NAV_TILE_ATTRIBUTE_MOB_UNMOVABLE ||
		rsCreateInfo.nAttribute & E_NAV_TILE_ATTRIBUTE_ALL_UNMOVABLE )
	{
		SetFilter( true );
		SetLock( true );
	}
	else
	{
		Float32		fSlopeTresholdSquared = rsCreateInfo.fSlopeThreshold * rsCreateInfo.fSlopeThreshold;

		if( m_sSlope[E_NAV_TILE_FACE_TYPE_LEFT].IsFiltered( fSlopeTresholdSquared ) == true )
		{
			SetFilter( E_NAV_TILE_FACE_TYPE_LEFT, true );
			SetLock( true );
		}
		else
		{
			SetFilter( E_NAV_TILE_FACE_TYPE_LEFT, false );
			++nFaceCreateCount;
		}

		if( m_sSlope[E_NAV_TILE_FACE_TYPE_RIGHT].IsFiltered( fSlopeTresholdSquared ) == true )
		{
			SetFilter( E_NAV_TILE_FACE_TYPE_RIGHT, true );
			SetLock( true );
		}
		else
		{
			SetFilter( E_NAV_TILE_FACE_TYPE_RIGHT, false );
			++nFaceCreateCount;
		}
	}

	return nFaceCreateCount;
}


//*****************************************************************************
//
// End of File : CNavTile.cpp
//
//*****************************************************************************