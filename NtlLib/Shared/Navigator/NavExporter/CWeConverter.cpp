//*****************************************************************************
// File       : CWeConverter.cpp
// Desc       : 
// Author     : June C. Kim (cynis@hotmail.com)
// Update     : 2007. 1. 18
//*****************************************************************************

//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "StdAfx.h"

#include "VersionLog.h"

#include "CWeConverter.h"


//-----------------------------------------------------------------------------
// Name Space Table
//-----------------------------------------------------------------------------

USE_NAMESPACE( navex )


//-----------------------------------------------------------------------------
// Initialize Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Name			: LoadReference
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CWeConverter::LoadReference( const CFixedString & rcFileName )
{
	UNREFERENCED_PARAMETER( rcFileName );

	m_cTileSize.SetFloat( 4.0f, 0.0f, 4.0f );
	m_cSectorSize.SetFloat( 256.0f, 0.0f, 256.0f );
	m_nSectorsInField	= 4;
	m_nSectorsInWorld	= 64;

	CFixedString	cModelListName( _T( "ObjectPropertyList" ) );

	if( CreateModelInfoes( cModelListName ) == false )
	{
		return false;
	}

	return true;
}


//*****************************************************************************
//
// End of File : CWeConverter.cpp
//
//*****************************************************************************