//*****************************************************************************
// File       : CNavPath.cpp
// Desc       : 
// Author     : June C. Kim (cynis@hotmail.com)
// Update     : 2006. 12. 5
//*****************************************************************************

//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "StdAfx.h"

#include "CNavQueryCallback.h"

#include "CNavRenderer.h"

#include "CNavPath.h"


//-----------------------------------------------------------------------------
// Name Space Table
//-----------------------------------------------------------------------------

USE_NAMESPACE( nav )


//-----------------------------------------------------------------------------
// Initialize Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Name			: CNavPath
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CNavPath::CNavPath( void ) :
m_eState( E_NAV_PATH_STATE_NONE ),
m_eError( E_NAV_PATH_ERROR_NONE ),
m_bResult( false ),
m_pcQueryCallback( NULL ),
m_nFlags( 0 ),
m_nShapeId( 0 ),
m_nMinimalWayPointCount( 0 )
{
	m_pcQueryCallback = new CNavQueryCallback;
	Assert( m_pcQueryCallback != NULL );

	return;
}


//-----------------------------------------------------------------------------
// Name			: CNavPath
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CNavPath::CNavPath( const CNavPosition & rcSourcePoint, const CNavPosition & rcDestinationPoint, UId32 nShapeId ) :
m_eState( E_NAV_PATH_STATE_NONE ),
m_eError( E_NAV_PATH_ERROR_NONE ),
m_bResult( false ),
m_pcQueryCallback( NULL ),
m_nFlags( 0 ),
m_nShapeId( nShapeId ),
m_nMinimalWayPointCount( 0 )
{
	m_pcQueryCallback = new CNavQueryCallback;
	Assert( m_pcQueryCallback != NULL );

	m_cSourcePosition.Set( rcSourcePoint );
	m_cDestinationPosition.Set( rcDestinationPoint );

	SetState( E_NAV_PATH_STATE_READY_TO_FIND );

	return;
}


//-----------------------------------------------------------------------------
// Name			: CNavPath
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CNavPath::CNavPath( const CNavPath & rcPath ) :
m_eState( E_NAV_PATH_STATE_NONE ),
m_eError( E_NAV_PATH_ERROR_NONE ),
m_bResult( false ),
m_pcQueryCallback( NULL ),
m_nFlags( 0 ),
m_nShapeId( 0 ),
m_nMinimalWayPointCount( 0 )
{
	m_pcQueryCallback = new CNavQueryCallback;
	Assert( m_pcQueryCallback != NULL );

	Set( rcPath );

	return;
}


//-----------------------------------------------------------------------------
// Name			: ~CNavPath
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CNavPath::~CNavPath( void )
{
	Destroy();

	delete m_pcQueryCallback;

	return;
}


//-----------------------------------------------------------------------------
// Name			: Initialize
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CNavPath::Initialize( void )
{
	SetState( E_NAV_PATH_STATE_NONE );
	m_eError = E_NAV_PATH_ERROR_NONE;
	m_bResult = false;

	m_nShapeId = 0;
	m_nFlags = 0;

	m_sInternalParameter.Reset();

	m_nMinimalWayPointCount = 0;

	m_cSourcePosition.Reset();
	m_cDestinationPosition.Reset();

	m_cWayPointList.Alloc( 0 );

	return;
}


//-----------------------------------------------------------------------------
// Name			: Destroy
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CNavPath::Destroy( void )
{
	Initialize();

	return;
}


//-----------------------------------------------------------------------------
// Name			: Clone
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CNavPath * CNavPath::Clone( void ) const
{
	return new CNavPath( *this );
}


//-----------------------------------------------------------------------------
// Name			: Set
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CNavPath::Set( const CNavPath & rcPath )
{
	SetState( E_NAV_PATH_STATE_PRE_PROCESS );

	SetError( rcPath.GetError() );

	m_nShapeId	= rcPath.GetShapeId();
	m_nFlags	= rcPath.GetFlags();
	m_bResult	= rcPath.GetResult();

	m_sInternalParameter.Set( rcPath.GetInternalParameter() );

	m_nMinimalWayPointCount = rcPath.GetMinimalWayPointCount();

	m_cSourcePosition.Set( rcPath.GetSourcePosition() );
	m_cDestinationPosition.Set( rcPath.GetDestinationPosition() );

	m_cWayPointList.Set( rcPath.GetWayPointList() );

	SetState( rcPath.GetState() );

	return;
}


//-----------------------------------------------------------------------------
// Name			: Set
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CNavPath::Set( const CNavPosition & rcSourcePoint, const CNavPosition & rcDestinationPoint, UId32 nShapeId )
{
	SetState( E_NAV_PATH_STATE_PRE_PROCESS );

	m_cSourcePosition.Set( rcSourcePoint );
	m_cDestinationPosition.Set( rcDestinationPoint );
	m_nShapeId	= nShapeId;

	m_sInternalParameter.Reset();

	m_cWayPointList.Alloc( 0 );

	SetError( E_NAV_PATH_ERROR_NONE );

	SetState( E_NAV_PATH_STATE_READY_TO_FIND );

	return;
}


//-----------------------------------------------------------------------------
// Name			: SetSourcePosition
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CNavPath::SetSourcePosition( const CNavPosition & rcSourcePosition )
{
	SetState( E_NAV_PATH_STATE_PRE_PROCESS );

	m_cSourcePosition.Set( rcSourcePosition );

	m_cWayPointList.Alloc( 0 );

	m_sInternalParameter.pcMesh	= NULL;
	m_sInternalParameter.nSourceCell = INTERNAL_CELL_INDEX_RESET;

	SetError( E_NAV_PATH_ERROR_NONE );
	SetState( E_NAV_PATH_STATE_READY_TO_FIND );

	return;
}


//-----------------------------------------------------------------------------
// Name			: SetDestinationPosition
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CNavPath::SetDestinationPosition( const CNavPosition & rcDestinationPosition )
{
	SetState( E_NAV_PATH_STATE_PRE_PROCESS );

	m_cDestinationPosition.Set( rcDestinationPosition );

	m_cWayPointList.Alloc( 0 );

	m_sInternalParameter.pcMesh	= NULL;
	m_sInternalParameter.nDestinationCell = INTERNAL_CELL_INDEX_RESET;

	SetError( E_NAV_PATH_ERROR_NONE );
	SetState( E_NAV_PATH_STATE_READY_TO_FIND );

	return;
}


//-----------------------------------------------------------------------------
// Name			: SetShape
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CNavPath::SetShape( UId32 nShapeId )
{
	m_nShapeId = nShapeId;

	switch( m_eState )
	{
		case E_NAV_PATH_STATE_VALID_PATH:
		case E_NAV_PATH_STATE_INVALID_DIRECT_PATH:
		case E_NAV_PATH_STATE_INVALID_PATH:
		{
			break;
		}

		default:
		{
			return;
		}
	}

	SetError( E_NAV_PATH_ERROR_NONE );
	SetState( E_NAV_PATH_STATE_READY_TO_FIND );

	return;
}


//-----------------------------------------------------------------------------
// Name			: GetPositionByMoveLength
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CNavPath::GetPositionByMoveLength( Float32 fMoveLength, CNavPosition & rcResultPosition ) const
{
	Assert( fMoveLength >= 0.0f );

	if( HasPath() == false )
	{
		return false;
	}

	Int32		nMoveLength = Float32ToFixed32( fMoveLength );

	if( nMoveLength >= m_cWayPointList.GetTotalLength() )
	{
		rcResultPosition.Set( *(m_cWayPointList.GetPosition( m_cWayPointList.GetCount() - 1 )) );

		return true;
	}

	UInt32		nIndex = 0;
	while( ++nIndex <= m_cWayPointList.GetCount() - 1 )
	{
		if( GetLength( nIndex ) >= nMoveLength )
		{
			rcResultPosition.Set( *(GetWayPoint( nIndex )) );
			rcResultPosition.Sub( *(GetWayPoint( --nIndex )) );

			rcResultPosition.Mul( nMoveLength - GetLength( nIndex ) );

			Fixed32		nOffset = GetLength( ++nIndex ) - GetLength( --nIndex );
			if( nOffset > 0 )
			{
				rcResultPosition.Div( nOffset );
			}
			else
			{
				Assert( !"nOffset <= 0" );
			}

			rcResultPosition.Add( *(GetWayPoint( nIndex )) );

			return true;
		}
	}

	rcResultPosition.Set( *(GetWayPoint( nIndex )) );

	return true;
}


//-----------------------------------------------------------------------------
// Name			: GetLength
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

Fixed32 CNavPath::GetLength( UInt32 nPositionIndex ) const
{
	if( nPositionIndex >= m_cWayPointList.GetCount() )
	{
		nPositionIndex = m_cWayPointList.GetCount() - 1;
	}

	return m_cWayPointList.GetLength( nPositionIndex );
}


//-----------------------------------------------------------------------------
// Name			: Render
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CNavPath::Render( CNavRenderer * pcRenderer )
{
	if( m_cWayPointList.GetCount() < 2L )
	{
		pcRenderer->RenderLine( m_cSourcePosition, m_cDestinationPosition, 0x00FF00FF, 3 );

		return true;
	}

	for( UInt32 nPointIndex = 0L; nPointIndex < m_cWayPointList.GetCount() - 1; ++nPointIndex )
	{
		pcRenderer->RenderLine( *(m_cWayPointList.GetPosition( nPointIndex )), *(m_cWayPointList.GetPosition( nPointIndex + 1 )), 0x0000FF00, 3 );
	}

	return true;
}


//*****************************************************************************
//
// End of File : CNavPath.cpp
//
//*****************************************************************************