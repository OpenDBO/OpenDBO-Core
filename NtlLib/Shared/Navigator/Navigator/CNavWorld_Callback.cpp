//*****************************************************************************
// File       : CNavWorld_Callback.cpp
// Desc       : 
// Author     : June C. Kim (cynis@hotmail.com)
// Update     : 2007. 1. 18
//*****************************************************************************

//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "StdAfx.h"

#include "CNavWorld.h"


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

bool CNavWorld::DoIt( bool (*pfnToDo)( CNavEntity * pcThis, void * pvData ), void * pvData )
{
	bool	bResult = CNavEntity::DoIt( pfnToDo, pvData );

	if( m_pcZones != NULL )
	{
		bResult &= m_pcZones->DoIt( pfnToDo, pvData );
	}

	if( m_pcSectors != NULL )
	{
		bResult &= m_pcSectors->DoIt( pfnToDo, pvData );
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

bool CNavWorld::DoItAfter( bool (*pfnToDo)( CNavEntity * pcThis, void * pvData ), void * pvData )
{
	bool	bResult = true;

	if( m_pcSectors != NULL )
	{
		bResult &= m_pcSectors->DoItAfter( pfnToDo, pvData );
	}

	if( m_pcZones != NULL )
	{
		bResult &= m_pcZones->DoIt( pfnToDo, pvData );
	}

	return bResult & CNavEntity::DoItAfter( pfnToDo, pvData );
}


//*****************************************************************************
//
// End of File : CNavWorld_Callback.cpp
//
//*****************************************************************************