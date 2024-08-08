//*****************************************************************************
// File       : CNavSector_Callback.cpp
// Desc       : 
// Author     : June C. Kim (cynis@hotmail.com)
// Update     : 2007. 1. 18
//*****************************************************************************

//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "StdAfx.h"

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
// Name			: DoIt
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CNavSector::DoIt( bool (*pfnToDo)( CNavEntity * pcThis, void * pvData ), void * pvData )
{
	bool	bResult = CNavEntity::DoIt( pfnToDo, pvData );

	if( m_pcTerrain != NULL )
	{
		bResult &= m_pcTerrain->DoIt( pfnToDo, pvData );
	}

	if( m_pcHeightMap != NULL )
	{
		bResult &= m_pcHeightMap->DoIt( pfnToDo, pvData );
	}

	if( m_pcAgents != NULL )
	{
		bResult &= m_pcAgents->DoIt( pfnToDo, pvData );
	}

	return bResult;
}


//-----------------------------------------------------------------------------
// Name			: DoItAfter
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CNavSector::DoItAfter( bool (*pfnToDo)( CNavEntity * pcThis, void * pvData ), void * pvData )
{
	bool	bResult = true;

	if( m_pcAgents != NULL )
	{
		bResult &= m_pcAgents->DoItAfter( pfnToDo, pvData );
	}

	if( m_pcHeightMap != NULL )
	{
		bResult &= m_pcHeightMap->DoIt( pfnToDo, pvData );
	}

	if( m_pcTerrain != NULL )
	{
		bResult &= m_pcTerrain->DoIt( pfnToDo, pvData );
	}

	return bResult & CNavEntity::DoItAfter( pfnToDo, pvData );
}


//*****************************************************************************
//
// End of File : CNavSector_Callback.cpp
//
//*****************************************************************************