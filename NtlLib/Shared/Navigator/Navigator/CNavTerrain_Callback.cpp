//*****************************************************************************
// File       : CNavTerrain_Callback.cpp
// Desc       : 
// Author     : June C. Kim (cynis@hotmail.com)
// Update     : 2007. 1. 18
//*****************************************************************************

//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "StdAfx.h"

#include "CNavTerrain.h"


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

bool CNavTerrain::DoIt( bool (*pfnToDo)( CNavEntity * pcThis, void * pvData ), void * pvData )
{
	bool	bResult = CNavEntity::DoIt( pfnToDo, pvData );

	if( m_pcObjects != NULL )
	{
		bResult &= m_pcObjects->DoIt( pfnToDo, pvData );
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

bool CNavTerrain::DoItAfter( bool (*pfnToDo)( CNavEntity * pcThis, void * pvData ), void * pvData )
{
	bool	bResult = true;

	if( m_pcObjects != NULL )
	{
		bResult &= m_pcObjects->DoItAfter( pfnToDo, pvData );
	}

	return bResult & CNavEntity::DoItAfter( pfnToDo, pvData );
}


//*****************************************************************************
//
// End of File : CNavTerrain_Callback.cpp
//
//*****************************************************************************