//*****************************************************************************
// File       : CNavTerrain_Object.cpp
// Desc       : 
// Author     : June C. Kim (cynis@hotmail.com)
// Update     : 2007. 1. 18
//*****************************************************************************

//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "StdAfx.h"

#include "CNavPathEngine.h"
#include "CNavEntityVector.h"
#include "CNavObject.h"

#include "CNavTerrain.h"


//-----------------------------------------------------------------------------
// Name Space Table
//-----------------------------------------------------------------------------

USE_NAMESPACE( nav )


//-----------------------------------------------------------------------------
// Initialize Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Name			: LinkObject
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CNavTerrain::LinkObject( CNavObject * const pcObject )
{
	Assert( pcObject->GetShape() != NULL );

	cPosition	rcPePosition;

	if( ConvertPosition( rcPePosition, pcObject->GetPosition() ) == false )
	{
		return false;
	}

	iAgent *	pcAgent = m_pcMesh->placeAgent( pcObject->GetShape(), rcPePosition );
	Assert( pcAgent != NULL );

	pcObject->SetAgent( pcAgent );

	m_pcContext->addAgent( pcAgent );
	m_pcObstacleSet->addAgent( pcAgent );

	if( m_pcObjects == NULL )
	{
		m_pcObjects = new CNavEntityVector( true );
		Assert( m_pcObjects != NULL );
	}

	return m_pcObjects->Insert( pcObject );
}


//-----------------------------------------------------------------------------
// Name			: UnlinkObject
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CNavTerrain::UnlinkObject( CNavObject * const pcObject )
{
	if( m_pcObjects == NULL )
	{
		return false;
	}

	return m_pcObjects->Remove( pcObject );
}


//*****************************************************************************
//
// End of File : CNavTerrain_Object.cpp
//
//*****************************************************************************