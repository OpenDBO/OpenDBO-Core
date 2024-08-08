//*****************************************************************************
// File       : CNavPathEngine_Singleton.cpp
// Desc       : 
// Author     : June C. Kim (cynis@hotmail.com)
// Update     : 2007. 1. 18
//*****************************************************************************

//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "StdAfx.h"

#include "CNavErrorHandler.h"

#include "CNavPathEngine.h"


//-----------------------------------------------------------------------------
// Name Space Table
//-----------------------------------------------------------------------------

USE_NAMESPACE( nav )


//-----------------------------------------------------------------------------
// Initialize Table
//-----------------------------------------------------------------------------

CNavPathEngine * CNavPathEngine::m_pInstance = NULL;


//-----------------------------------------------------------------------------
// Name			: CreateInstance
// Desc			: 
// Privilege	: static public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CNavPathEngine * CNavPathEngine::CreateInstance( const CFixedString & rcDllName )
{
	if( m_pInstance != NULL )
	{
		DestroyInstance();
	}

	m_pInstance = new CNavPathEngine;
	m_pInstance->LoadPathEngine( rcDllName, g_pErrorHandler );

	return m_pInstance;
}


//-----------------------------------------------------------------------------
// Name			: CreateInstance
// Desc			: 
// Privilege	: static public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CNavPathEngine * CNavPathEngine::CreateInstance( iPathEngine * const pcPathEngine )
{
	if( m_pInstance != NULL )
	{
		DestroyInstance();
	}

	m_pInstance = new CNavPathEngine;
	m_pInstance->LoadPathEngine( pcPathEngine, g_pErrorHandler );

	return m_pInstance;
}


//-----------------------------------------------------------------------------
// Name			: DestroyInstance
// Desc			: 
// Privilege	: static public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CNavPathEngine::DestroyInstance( void )
{
	if( m_pInstance != NULL )
	{
		m_pInstance->UnloadPathEngine();

		delete m_pInstance;
		m_pInstance = NULL;
	}

	return;
}


//*****************************************************************************
//
// End of File : CNavPathEngine_Singleton.cpp
//
//*****************************************************************************