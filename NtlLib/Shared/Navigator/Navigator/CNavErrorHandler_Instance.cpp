//*****************************************************************************
// File       : CNavErrorHandler_Singleton.cpp
// Desc       : 
// Author     : June C. Kim (cynis@hotmail.com)
// Update     : 2007. 1. 18
//*****************************************************************************

//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "StdAfx.h"

#include "CNavErrorHandler.h"


//-----------------------------------------------------------------------------
// Name Space Table
//-----------------------------------------------------------------------------

USE_NAMESPACE( nav )


//-----------------------------------------------------------------------------
// Initialize Table
//-----------------------------------------------------------------------------

CNavErrorHandler * CNavErrorHandler::m_pInstance = NULL;


//-----------------------------------------------------------------------------
// Name			: CreateInstance
// Desc			: 
// Privilege	: static public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CNavErrorHandler * CNavErrorHandler::CreateInstance( void )
{
	if( m_pInstance != NULL )
	{
		DestroyInstance();
	}

	m_pInstance = new CNavErrorHandler;

	return m_pInstance;
}


//-----------------------------------------------------------------------------
// Name			: DestroyInstance
// Desc			: 
// Privilege	: static public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CNavErrorHandler::DestroyInstance( void )
{
	if( m_pInstance != NULL )
	{
		delete m_pInstance;
		m_pInstance = NULL;
	}

	return;
}


//*****************************************************************************
//
// End of File : CNavErrorHandler_Singleton.cpp
//
//*****************************************************************************