//*****************************************************************************
// File       : CNavEntityFactory_Instance.cpp
// Desc       : 
// Author     : June C. Kim (cynis@hotmail.com)
// Update     : 2007. 1. 18
//*****************************************************************************

//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "StdAfx.h"

#include "CNavEntityFactory.h"

//-----------------------------------------------------------------------------
// Name Space Table
//-----------------------------------------------------------------------------

USE_NAMESPACE( nav )


//-----------------------------------------------------------------------------
// Initialize Table
//-----------------------------------------------------------------------------

CNavEntityFactory *	CNavEntityFactory::m_pInstance		= NULL;


//-----------------------------------------------------------------------------
// Name			: CreateInstance
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CNavEntityFactory * CNavEntityFactory::CreateInstance( void )
{
	DestroyInstance();

	m_pInstance = new CNavEntityFactory;
	Assert( m_pInstance != NULL );

	if( m_pInstance->Create() == false )
	{
		DestroyInstance();
	}

	return m_pInstance;
}


//-----------------------------------------------------------------------------
// Name			: DestroyInstance
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CNavEntityFactory::DestroyInstance( void )
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
// End of File : CNavEntityFactory_Instance.cpp
//
//*****************************************************************************