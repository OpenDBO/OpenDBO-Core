//*****************************************************************************
// File       : CNavExportMeshThread.cpp
// Desc       : 
// Author     : June C. Kim (cynis@hotmail.com)
// Update     : 2007. 1. 18
//*****************************************************************************

//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "StdAfx.h"

#include "CNavPathEngine.h"
#include "CNavTerrain.h"

#include "CNavExportMeshThread.h"


//-----------------------------------------------------------------------------
// Name Space Table
//-----------------------------------------------------------------------------

USE_NAMESPACE( nav )


//-----------------------------------------------------------------------------
// Initialize Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Name			: Initialize
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CNavExportMeshThread::Initialize( void )
{
	return;
}


//-----------------------------------------------------------------------------
// Name			: Destroy
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CNavExportMeshThread::Destroy( void )
{
	Initialize();

	return;
}


//-----------------------------------------------------------------------------
// Name			: Destroy
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CNavExportMeshThread::SetCreateInfo( const SNavEntityCreateInfo & rsCreateInfo )
{
	m_sCreateInfo.CopyFrom( rsCreateInfo );

	return;
}


//-----------------------------------------------------------------------------
// Name			: Begin
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

UInt32 CNavExportMeshThread::Begin( void )
{
	SetObserver( NULL );

	return 0;
}


//-----------------------------------------------------------------------------
// Name			: Run
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

UInt32 CNavExportMeshThread::Run( void )
{
	CNavTerrain &	rcEntity = const_cast<CNavTerrain &>(down_cast<const CNavTerrain &>(CNavEntityFactory::GetInstance()->GetEntity( m_sCreateInfo.cName )));
//	Assert( pcEntity != NULL );

	m_sCreateInfo.nFlags = RemoveFlags( m_sCreateInfo.nFlags, E_NAV_ENTITY_CREATE_INFO_FLAG_MULTI_THREAD );

	rcEntity.ImportFromFile( m_sCreateInfo );

	return 0;
}


//-----------------------------------------------------------------------------
// Name			: End
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

UInt32 CNavExportMeshThread::End( void )
{
	return 0;
}


//*****************************************************************************
//
// End of File : CNavExportMeshThread.cpp
//
//*****************************************************************************