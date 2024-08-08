//*****************************************************************************
// File       : CNavShapeManager.cpp
// Desc       : 
// Author     : June C. Kim (cynis@hotmail.com)
// Update     : 2007. 1. 18
//*****************************************************************************

//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "StdAfx.h"

#include "CNavPathEngine.h"
#include "CNavigator.h"

#include "CNavShapeManager.h"


//-----------------------------------------------------------------------------
// Name Space Table
//-----------------------------------------------------------------------------

USE_NAMESPACE( nav )


//-----------------------------------------------------------------------------
// Initialize Table
//-----------------------------------------------------------------------------

CFixedString CNavShapeManager::m_cPathName = CFixedString();


//-----------------------------------------------------------------------------
// Name			: Initialize
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CNavShapeManager::Initialize( void )
{
	m_nShapeCount	= 0;
	ZeroMemory( m_apcShapeList, sizeof( iShape * ) * NAV_SHAPE_COUNT_MAX );

	return;
}


//-----------------------------------------------------------------------------
// Name			: Destroy
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CNavShapeManager::Destroy( void )
{
	for( UInt32 nShapeIndex = 0; nShapeIndex < NAV_SHAPE_COUNT_MAX; ++nShapeIndex )
	{
		if( m_apcShapeList[nShapeIndex] != NULL )
		{
			delete m_apcShapeList[nShapeIndex];
			m_apcShapeList[nShapeIndex] = NULL;
		}
	}

	return;
}


//-----------------------------------------------------------------------------
// Name			: Register
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

UId32 CNavShapeManager::Register( const CFixedString & rcShapeName, UInt32 nFlags )
{
	for( UId32 nShapeIndex = 0; nShapeIndex < NAV_SHAPE_COUNT_MAX; ++nShapeIndex )
	{
		if( m_apcShapeList[nShapeIndex] == NULL )
		{
			CFixedString	cFullFileName;
			cFullFileName.Format( _T( "%s\\%s.xml" ),
								GetPathName().GetBuffer(), rcShapeName.GetBuffer() );

			m_apcShapeList[nShapeIndex] = CNavPathEngine::GetInstance()->LoadShape( E_NAV_FILE_TYPE_XML, cFullFileName );
			Assert( m_apcShapeList[nShapeIndex] != NULL );

			SNavEntityPreProcessInfo	sPreProcessInfo;
			sPreProcessInfo.nShapeId	= nShapeIndex;
			sPreProcessInfo.nFlags		= nFlags;

			CNavigator::GetInstance()->PreProcess( sPreProcessInfo );

			return nShapeIndex;
		}
	}

	return INVALID_ID;
}


//-----------------------------------------------------------------------------
// Name			: Unregister
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CNavShapeManager::Unregister( UId32 nShapeId, UInt32 nFlags )
{
	Assert( m_apcShapeList[nShapeId] != NULL );

	Assert( GetByShapeId( nShapeId ) != NULL );

	SNavEntityPreProcessInfo	sPostProcessInfo;
	sPostProcessInfo.nShapeId	= nShapeId;
	sPostProcessInfo.nFlags		= nFlags;

	CNavigator::GetInstance()->PostProcess( sPostProcessInfo );

	delete m_apcShapeList[nShapeId];
	m_apcShapeList[nShapeId] = NULL;

	return;
}


//-----------------------------------------------------------------------------
// Name			: GetByShapeId
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

iShape * const CNavShapeManager::GetByShapeId( UId32 nShapeId ) const
{
	if( nShapeId >= NAV_SHAPE_COUNT_MAX )
	{
		return NULL;
	}

	Assert( m_apcShapeList[nShapeId] != NULL );

	return m_apcShapeList[nShapeId];
}


//-----------------------------------------------------------------------------
// Name			: SearchByShape
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

UId32 CNavShapeManager::SearchByShape( const iShape * const pcShape ) const
{
	for( UInt32 nShapeIndex = 0; nShapeIndex < NAV_SHAPE_COUNT_MAX; ++nShapeIndex )
	{
		if( m_apcShapeList[nShapeIndex] == pcShape )
		{
			return nShapeIndex;
		}
	}

	return INVALID_ID;
}


//*****************************************************************************
//
// End of File : CNavShapeManager.cpp
//
//*****************************************************************************