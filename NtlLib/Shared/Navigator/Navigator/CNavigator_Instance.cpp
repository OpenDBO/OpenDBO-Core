//*****************************************************************************
// File       : CNavigator_Instance.cpp
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
#include "CNavConverter.h"

#include "CNavigator.h"


//-----------------------------------------------------------------------------
// Name Space Table
//-----------------------------------------------------------------------------

USE_NAMESPACE( nav )


//-----------------------------------------------------------------------------
// Initialize Table
//-----------------------------------------------------------------------------

CNavigator *		CNavigator::m_pcInstance			= NULL;
CNavConverter *		CNavigator::m_pcDefaultConverter	= NULL;
CFixedString		CNavigator::m_cPathName				= CFixedString( NAVIGATOR_DATA_PATH_DEFAULT );
bool				CNavigator::m_bAbsolutePath			= false;


//-----------------------------------------------------------------------------
// Name			: CreateInstance
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CNavigator * CNavigator::CreateInstance( const CFixedString & rcEngineName, const CFixedString & rcSessionName, SNavEntityExternalProcess * const psExternalProcess, UInt32 nFlag )
{
	DestroyInstance();

	if( m_bAbsolutePath == false )
	{
		TCHAR		szCurrentDirectory[MAX_PATH_LENGTH];
		::GetCurrentDirectory( MAX_PATH_LENGTH, szCurrentDirectory );
		m_cPathName.Format( _T( "%s\\%s" ), szCurrentDirectory, m_cPathName.GetBuffer() );
	}

	CNavErrorHandler::CreateInstance();
	CNavPathEngine::CreateInstance( rcEngineName );
	CNavEntityFactory::CreateInstance();

	m_pcDefaultConverter = new CNavConverter;
	Assert( m_pcDefaultConverter != NULL );

	// Create Navigator

	SNavEntityCreateInfo				sCreateInfo;
	sCreateInfo.nUniqueId				= 0;
	sCreateInfo.eEntityType				= E_NAV_ENTITY_TYPE_NAVIGATOR;
	sCreateInfo.uSubType.eNavigatorType	= E_NAV_NAVIGATOR_TYPE_CLIENT;
	sCreateInfo.eCreateType				= E_NAV_CREATE_TYPE_FILE;
	sCreateInfo.pcParent				= NULL;
	sCreateInfo.pvUserData				= reinterpret_cast<void *>(psExternalProcess);
	sCreateInfo.nUserDataSize			= sizeof( SNavEntityExternalProcess * );
	sCreateInfo.nFlags					= nFlag;

	if( nFlag & E_NAV_ENTITY_CREATE_INFO_FLAG_EXPORT_MODE )
	{
		sCreateInfo.bExport				= true;
	}
	else
	{
		sCreateInfo.bExport				= false;
	}

	sCreateInfo.cPath					= m_cPathName;
	sCreateInfo.cName					= rcSessionName;

	sCreateInfo.cPosition.Reset();
	sCreateInfo.cBounding.Reset();

	m_pcInstance = down_cast<CNavigator *>(CNavEntityFactory::GetInstance()->CreateEntity( sCreateInfo ));
	Assert( m_pcInstance != NULL );

	return m_pcInstance;
}


//-----------------------------------------------------------------------------
// Name			: CreateInstance
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CNavigator * CNavigator::CreateInstance( iPathEngine * const pcEngineInstance, const CFixedString & rcSessionName, SNavEntityExternalProcess * const psExternalProcess, UInt32 nFlag )
{
	DestroyInstance();

	TCHAR		szCurrentDirectory[MAX_PATH_LENGTH];
	::GetCurrentDirectory( MAX_PATH_LENGTH, szCurrentDirectory );
	m_cPathName.Format( _T( "%s\\%s" ), szCurrentDirectory, m_cPathName.GetBuffer() );

	CNavErrorHandler::CreateInstance();
	CNavPathEngine::CreateInstance( pcEngineInstance );
	CNavEntityFactory::CreateInstance();

	m_pcDefaultConverter = new CNavConverter;

	// Create Navigator

	SNavEntityCreateInfo				sCreateInfo;
	sCreateInfo.nUniqueId				= 0;
	sCreateInfo.eEntityType				= E_NAV_ENTITY_TYPE_NAVIGATOR;
	sCreateInfo.uSubType.eNavigatorType	= E_NAV_NAVIGATOR_TYPE_CLIENT;
	sCreateInfo.eCreateType				= E_NAV_CREATE_TYPE_FILE;
	sCreateInfo.pcParent				= NULL;
	sCreateInfo.pvUserData				= reinterpret_cast<void *>(psExternalProcess);
	sCreateInfo.nUserDataSize			= sizeof( SNavEntityExternalProcess * );
	sCreateInfo.nFlags					= nFlag;
	sCreateInfo.cName					= rcSessionName;

	sCreateInfo.cPosition.Reset();
	sCreateInfo.cBounding.Reset();

	m_pcInstance = down_cast<CNavigator *>(CNavEntityFactory::GetInstance()->CreateEntity( sCreateInfo ));

	return m_pcInstance;
}


//-----------------------------------------------------------------------------
// Name			: CreateInstance
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CNavigator * CNavigator::CreateInstance( const TCHAR * const pchEngineName, const TCHAR * const pchSessionName, SNavEntityExternalProcess * const psExternalProcess, UInt32 nFlag )
{
	CFixedString	cEngineName( pchEngineName );
	CFixedString	cSessionName( pchSessionName );

	return CNavigator::CreateInstance( cEngineName, cSessionName, psExternalProcess, nFlag );
}


//-----------------------------------------------------------------------------
// Name			: DestroyInstance
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CNavigator::DestroyInstance( void )
{
	if( m_pcInstance != NULL )
	{
		delete m_pcInstance;
		m_pcInstance = NULL;
	}

	delete m_pcDefaultConverter;
	m_pcDefaultConverter = NULL;

	CNavEntityFactory::DestroyInstance();
	CNavPathEngine::DestroyInstance();
	CNavErrorHandler::DestroyInstance();

	return;
}


//*****************************************************************************
//
// End of File : CNavigator_Instance.cpp
//
//*****************************************************************************