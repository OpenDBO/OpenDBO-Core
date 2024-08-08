//*****************************************************************************
// File       : CNavPreProcessThread.cpp
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

#include "CNavPreProcessThread.h"


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

void CNavPreProcessThread::Initialize( void )
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

void CNavPreProcessThread::Destroy( void )
{
	Initialize();

	return;
}


//-----------------------------------------------------------------------------
// Name			: SetPreProcessInfo
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CNavPreProcessThread::SetPreProcessInfo( const SNavEntityPreProcessInfo & rsPreProcessInfo )
{
	m_sPreProcessInfo.CopyFrom( rsPreProcessInfo );

	return;
}


//-----------------------------------------------------------------------------
// Name			: Begin
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

UInt32 CNavPreProcessThread::Begin( void )
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

UInt32 CNavPreProcessThread::Run( void )
{
	CNavTerrain &	rcEntity = const_cast<CNavTerrain &>(down_cast<const CNavTerrain &>(CNavEntityFactory::GetInstance()->GetEntity( m_cEntityName )));
//	Assert( pcEntity != NULL );

	m_sPreProcessInfo.nFlags = RemoveFlags( m_sPreProcessInfo.nFlags, E_NAV_ENTITY_CREATE_INFO_FLAG_MULTI_THREAD );

	rcEntity.PreProcess( m_sPreProcessInfo );

	return 0;
}


//-----------------------------------------------------------------------------
// Name			: End
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

UInt32 CNavPreProcessThread::End( void )
{
	return 0;
}


//*****************************************************************************
//
// End of File : CNavLoadMeshThread.cpp
//
//*****************************************************************************