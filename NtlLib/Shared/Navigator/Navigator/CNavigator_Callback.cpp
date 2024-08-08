//*****************************************************************************
// File       : CNavigator_Callback.cpp
// Desc       : 
// Author     : June C. Kim (cynis@hotmail.com)
// Update     : 2007. 1. 18
//*****************************************************************************

//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "StdAfx.h"

#include "CNavigator.h"


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

bool CNavigator::DoIt( bool (*pfnToDo)( CNavEntity * pcThis, void * pvData ), void * pvData )
{
	bool	bResult = CNavEntity::DoIt( pfnToDo, pvData );

	if( m_pcWorlds != NULL )
	{
		bResult &= m_pcWorlds->DoIt( pfnToDo, pvData );
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

bool CNavigator::DoItAfter( bool (*pfnToDo)( CNavEntity * pcThis, void * pvData ), void * pvData )
{
	bool	bResult = true;

	if( m_pcWorlds != NULL )
	{
		bResult &= m_pcWorlds->DoItAfter( pfnToDo, pvData );
	}

	return bResult & CNavEntity::DoItAfter( pfnToDo, pvData );
}


//*****************************************************************************
//
// End of File : CNavigator_Callback.cpp
//
//*****************************************************************************