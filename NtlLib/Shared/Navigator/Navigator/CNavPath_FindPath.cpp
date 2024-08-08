//*****************************************************************************
// File       : CNavPath_FindPath.cpp
// Desc       : 
// Author     : June C. Kim (cynis@hotmail.com)
// Update     : 2007. 1. 18
//*****************************************************************************

//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "StdAfx.h"

#include "CNavRenderer.h"

#include "CNavWorld.h"
#include "CNavSector.h"
#include "CNavTerrain.h"
#include "CNavigator.h"
#include "CNavZone.h"
#include "CNavQueryCallback.h"

#include "CNavPath.h"


//-----------------------------------------------------------------------------
// Name Space Table
//-----------------------------------------------------------------------------

USE_NAMESPACE( nav )


//-----------------------------------------------------------------------------
// Initialize Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Name			: FindPath
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CNavPath::FindPath( UInt32 nFlags )
{
	if( DetectStateForPath( nFlags ) == false )
	{
		return m_bResult;
	}

	if( FindMeshForPath() == false )
	{
		return m_bResult;
	}

	iPath *		pcPePath = m_sInternalParameter.pcMesh->findShortestPath( m_sInternalParameter.pcShape, m_sInternalParameter.pcPeContext, m_sInternalParameter.cSourcePosition, m_sInternalParameter.cDestinationPosition );

	if( pcPePath == NULL )
	{
		SetError( E_NAV_PATH_ERROR_NO_PATH );
		SetState( E_NAV_PATH_STATE_INVALID_PATH );

		m_bResult = false;
	}
	else if( pcPePath->size() < 2 )
	{
		SetError( E_NAV_PATH_ERROR_NO_PATH );
		SetState( E_NAV_PATH_STATE_INVALID_PATH );

		delete pcPePath;

		m_bResult = false;
	}
	else
	{
		m_nMinimalWayPointCount = pcPePath->size();

		if( CreatePathMap( pcPePath, nFlags ) == false )
		{
			SetError( E_NAV_PATH_ERROR_NO_PATH );
			SetState( E_NAV_PATH_STATE_INVALID_PATH );

			delete pcPePath;

			m_bResult = false;
		}
		else
		{
			SetError( E_NAV_PATH_ERROR_NONE );
			SetState( E_NAV_PATH_STATE_VALID_PATH );

			delete pcPePath;

			m_nFlags = nFlags;

			m_bResult = true;
		}
	}

	return m_bResult;
}


//-----------------------------------------------------------------------------
// Name			: FindPath
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CNavPath::FindPath( const CNavBounding & rcSearchBound, UInt32 nFlags )
{
	if( DetectStateForPath( nFlags ) == false )
	{
		return m_bResult;
	}

	if( FindMeshForPath() == false )
	{
		return m_bResult;
	}

	cHorizontalRange	cRange;

	cRange.minX = Min( m_cSourcePosition.GetX(), m_cDestinationPosition.GetX() ) + rcSearchBound.GetMin().GetX();
	cRange.minY = Min( m_cSourcePosition.GetY(), m_cDestinationPosition.GetY() ) + rcSearchBound.GetMin().GetY();
	cRange.maxX = Max( m_cSourcePosition.GetX(), m_cDestinationPosition.GetX() ) + rcSearchBound.GetMax().GetX();
	cRange.maxY = Max( m_cSourcePosition.GetY(), m_cDestinationPosition.GetY() ) + rcSearchBound.GetMax().GetY();

	m_sInternalParameter.pcPeContext->setQueryBounds( cRange );

//	g_Log( _T( "iMesh::findShortestPath() : Begin" ) );
	iPath *		pcPePath = m_sInternalParameter.pcMesh->findShortestPath( m_sInternalParameter.pcShape, m_sInternalParameter.pcPeContext, m_sInternalParameter.cSourcePosition, m_sInternalParameter.cDestinationPosition );
//	g_Log( _T( "iMesh::findShortestPath() : End" ) );

	m_sInternalParameter.pcPeContext->clearQueryBounds();

	if( pcPePath == NULL )
	{
		SetError( E_NAV_PATH_ERROR_NO_PATH );
		SetState( E_NAV_PATH_STATE_INVALID_PATH );

		m_bResult = false;
	}
	else if( pcPePath->size() < 2 )
	{
		SetError( E_NAV_PATH_ERROR_NO_PATH );
		SetState( E_NAV_PATH_STATE_INVALID_PATH );

		delete pcPePath;

		m_bResult = false;
	}
	else
	{
		m_nMinimalWayPointCount = pcPePath->size();

		if( CreatePathMap( pcPePath, nFlags ) == false )
		{
			SetError( E_NAV_PATH_ERROR_NO_PATH );
			SetState( E_NAV_PATH_STATE_INVALID_PATH );

			delete pcPePath;

			m_bResult = false;
		}
		else
		{
			SetError( E_NAV_PATH_ERROR_NONE );
			SetState( E_NAV_PATH_STATE_VALID_PATH );

			delete pcPePath;

			m_nFlags = nFlags;

			m_bResult = true;
		}
	}

	return m_bResult;
}


//-----------------------------------------------------------------------------
// Name			: FindPathFast
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CNavPath::FindPathFast( UInt32 nFlags )
{
	if( DetectStateForPath( nFlags ) == false )
	{
		return m_bResult;
	}

	if( FindMeshForPath() == false )
	{
		return m_bResult;
	}

	(down_cast<CNavQueryCallback *>(m_pcQueryCallback))->SetThreshold( CalculatePathFindThreshold() );
	(down_cast<CNavQueryCallback *>(m_pcQueryCallback))->ResetCount();

//	g_Log( _T( "iMesh::findShortestPath() : Begin" ) );
	iPath *		pcPePath = m_sInternalParameter.pcMesh->findShortestPath_WithQueryCallBack( m_sInternalParameter.pcShape, m_sInternalParameter.pcPeContext, m_sInternalParameter.cSourcePosition, m_sInternalParameter.cDestinationPosition, m_pcQueryCallback );
//	g_Log( _T( "iMesh::findShortestPath() : End" ) );

	if( pcPePath == NULL )
	{
		SetError( E_NAV_PATH_ERROR_NO_PATH );
		SetState( E_NAV_PATH_STATE_INVALID_PATH );

		m_bResult = false;
	}
	else if( pcPePath->size() < 2 )
	{
		SetError( E_NAV_PATH_ERROR_NO_PATH );
		SetState( E_NAV_PATH_STATE_INVALID_PATH );

		delete pcPePath;

		m_bResult = false;
	}
	else
	{
		m_nMinimalWayPointCount = pcPePath->size();

		if( CreatePathMap( pcPePath, nFlags ) == false )
		{
			SetError( E_NAV_PATH_ERROR_NO_PATH );
			SetState( E_NAV_PATH_STATE_INVALID_PATH );

			delete pcPePath;

			m_bResult = false;
		}
		else
		{
			SetError( E_NAV_PATH_ERROR_NONE );
			SetState( E_NAV_PATH_STATE_VALID_PATH );

			delete pcPePath;

			m_nFlags = nFlags;

			m_bResult = true;
		}
	}

	return m_bResult;
}


//-----------------------------------------------------------------------------
// Name			: FindPath
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CNavPath::FindPathFast( const CNavBounding & rcSearchBound, UInt32 nFlags )
{
	if( DetectStateForPath( nFlags ) == false )
	{
		return m_bResult;
	}

	if( FindMeshForPath() == false )
	{
		return m_bResult;
	}

	cHorizontalRange	cRange;

	cRange.minX = Min( m_cSourcePosition.GetX(), m_cDestinationPosition.GetX() ) + rcSearchBound.GetMin().GetX();
	cRange.minY = Min( m_cSourcePosition.GetY(), m_cDestinationPosition.GetY() ) + rcSearchBound.GetMin().GetY();
	cRange.maxX = Max( m_cSourcePosition.GetX(), m_cDestinationPosition.GetX() ) + rcSearchBound.GetMax().GetX();
	cRange.maxY = Max( m_cSourcePosition.GetY(), m_cDestinationPosition.GetY() ) + rcSearchBound.GetMax().GetY();

	m_sInternalParameter.pcPeContext->setQueryBounds( cRange );

	(down_cast<CNavQueryCallback *>(m_pcQueryCallback))->SetThreshold( CalculatePathFindThreshold() );
	(down_cast<CNavQueryCallback *>(m_pcQueryCallback))->ResetCount();

//	g_Log( _T( "iMesh::findShortestPath() : Begin" ) );
	iPath *		pcPePath = m_sInternalParameter.pcMesh->findShortestPath_WithQueryCallBack( m_sInternalParameter.pcShape, m_sInternalParameter.pcPeContext, m_sInternalParameter.cSourcePosition, m_sInternalParameter.cDestinationPosition, m_pcQueryCallback );
//	g_Log( _T( "iMesh::findShortestPath() : End" ) );

	m_sInternalParameter.pcPeContext->clearQueryBounds();

	if( pcPePath == NULL )
	{
		SetError( E_NAV_PATH_ERROR_NO_PATH );
		SetState( E_NAV_PATH_STATE_INVALID_PATH );

		m_bResult = false;
	}
	else if( pcPePath->size() < 2 )
	{
		SetError( E_NAV_PATH_ERROR_NO_PATH );
		SetState( E_NAV_PATH_STATE_INVALID_PATH );

		delete pcPePath;

		m_bResult = false;
	}
	else
	{
		m_nMinimalWayPointCount = pcPePath->size();

		if( CreatePathMap( pcPePath, nFlags ) == false )
		{
			SetError( E_NAV_PATH_ERROR_NO_PATH );
			SetState( E_NAV_PATH_STATE_INVALID_PATH );

			delete pcPePath;

			m_bResult = false;
		}
		else
		{
			SetError( E_NAV_PATH_ERROR_NONE );
			SetState( E_NAV_PATH_STATE_VALID_PATH );

			delete pcPePath;

			m_nFlags = nFlags;

			m_bResult = true;
		}
	}

	return m_bResult;
}


//-----------------------------------------------------------------------------
// Name			: FindDirectPath
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CNavPath::FindDirectPath( UInt32 nFlags )
{
	// Detect State

	switch( m_eState )
	{
		case E_NAV_PATH_STATE_VALID_PATH:
		{
			if( HasDirectPath() == true )
			{
				return true;
			}

			break;
		}

		case E_NAV_PATH_STATE_INVALID_DIRECT_PATH:
		case E_NAV_PATH_STATE_READY_TO_FIND:
		{
			break;
		}

		case E_NAV_PATH_STATE_INVALID_PATH:
		{
			return false;
		}

		default:
		{
			SetError( E_NAV_PATH_ERROR_IN_PROCESS );

			return false;
		}
	}

	SetState( E_NAV_PATH_STATE_IN_PROCESS );

	// Find Correct Shape

	iShape *	pcShape = CNavigator::GetInstance()->GetShape( m_nShapeId );
	if( pcShape == NULL )
	{
		SetError( E_NAV_PATH_ERROR_NO_MATCH_SHAPE );
		SetState( E_NAV_PATH_STATE_INVALID_PARAMETER );

		return false;
	}

	
	cPosition	cSourcePosition;
	cPosition	cDestinationPosition;
	iCollisionContext *		pcPeContext;

	if( m_sInternalParameter.pcMesh == NULL )
	{
		// Find Correct World

		const CNavWorld *	pcNavWorld = down_cast<CNavWorld *>(CNavigator::GetInstance()->GetChildByPosition( m_cSourcePosition, m_cDestinationPosition ));
		if( pcNavWorld == NULL )
		{
			SetError( E_NAV_PATH_ERROR_NO_MATCH_WORLD );
			SetState( E_NAV_PATH_STATE_INVALID_PARAMETER );

			return false;
		}

		// Fine Correct Sector

		const CNavSector *	pcNavSector = pcNavWorld->GetSectorByPosition( m_cSourcePosition );
		if( pcNavSector == NULL )
		{
			SetError( E_NAV_PATH_ERROR_NO_MATCH_SECTOR );
			SetState( E_NAV_PATH_STATE_INVALID_PARAMETER );

			return false;
		}

		// Fine Correct Zone

		const CNavEntity *	pcNavZone = pcNavSector->GetParent();
		if( pcNavZone == NULL )
		{
			SetError( E_NAV_PATH_ERROR_NO_MATCH_ZONE );
			SetState( E_NAV_PATH_STATE_INVALID_PARAMETER );

			return false;
		}

		// Fine Correct Terrain

		CNavTerrain *	const	pcNavTerrain = down_cast<const CNavZone *>(pcNavZone)->GetTerrain();
		if( pcNavTerrain == NULL )
		{
			SetError( E_NAV_PATH_ERROR_NO_TERRAIN );
			SetState( E_NAV_PATH_STATE_INVALID_PARAMETER );

			return false;
		}

		// Find Correct Mesh

		m_sInternalParameter.pcMesh = pcNavTerrain->GetMesh();
		if( m_sInternalParameter.pcMesh == NULL )
		{
			SetError( E_NAV_PATH_ERROR_NO_MESH );
			SetState( E_NAV_PATH_STATE_INVALID_PARAMETER );

			return false;
		}

		pcPeContext = pcNavTerrain->GetContext();

		// Convert Position

		if( pcNavTerrain->ConvertPosition( cSourcePosition, m_cSourcePosition ) == false )
		{
			SetError( E_NAV_PATH_ERROR_NO_SOURCE_POSITION );
			SetState( E_NAV_PATH_STATE_INVALID_PARAMETER );

			return false;
		}

		if( m_sInternalParameter.pcMesh->testPointCollision( pcShape, NULL, cSourcePosition ) == true )
		{
			SetError( E_NAV_PATH_ERROR_OBSTRUCT_SOURCE_POSITION );
			SetState( E_NAV_PATH_STATE_INVALID_PARAMETER );

			return false;
		}

		if( pcNavTerrain->ConvertPosition( cDestinationPosition, m_cDestinationPosition ) == false )
		{
			SetError( E_NAV_PATH_ERROR_NO_DESTINATION_POSITION );
			SetState( E_NAV_PATH_STATE_INVALID_PARAMETER );

			return false;
		}

		if( m_sInternalParameter.pcMesh->testPointCollision( pcShape, NULL, cDestinationPosition ) == true )
		{
			SetError( E_NAV_PATH_ERROR_OBSTRUCT_DESTINATION_POSITION );
			SetState( E_NAV_PATH_STATE_INVALID_PARAMETER );

			return false;
		}

		m_sInternalParameter.nSourceCell = cSourcePosition.cell;
		m_sInternalParameter.nDestinationCell = cDestinationPosition.cell;
	}
	else
	{
		cSourcePosition.x		= m_cSourcePosition.GetX();
		cSourcePosition.y		= m_cSourcePosition.GetY();
		cSourcePosition.cell		= m_sInternalParameter.nSourceCell;

		cDestinationPosition.x		= m_cDestinationPosition.GetX();
		cDestinationPosition.y		= m_cDestinationPosition.GetY();
		cDestinationPosition.cell		= m_sInternalParameter.nDestinationCell;

		pcPeContext = NULL;
	}

//	g_Log( _T( "iMesh::testLineCollision() : Begin" ) );
	bool	bResult = m_sInternalParameter.pcMesh->testLineCollision( pcShape, NULL, cSourcePosition, cDestinationPosition );
//	g_Log( _T( "iMesh::testLineCollision() : End" ) );

	if( bResult == true )
	{
		SetError( E_NAV_PATH_ERROR_NO_PATH );
		SetState( E_NAV_PATH_STATE_INVALID_DIRECT_PATH );

		return false;
	}

	m_nMinimalWayPointCount = 2;

	CreateDirectPathMap( nFlags );

	m_nFlags = nFlags;

	SetError( E_NAV_PATH_ERROR_NONE );
	SetState( E_NAV_PATH_STATE_VALID_PATH );

	return true;
}


//-----------------------------------------------------------------------------
// Name			: DetectStateForPath
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CNavPath::DetectStateForPath( UInt32 nFlags )
{
	switch( m_eState )
	{
		case E_NAV_PATH_STATE_VALID_PATH:
		{
			Assert( m_nFlags == nFlags );

			m_bResult = true;

			return false;
		}

		case E_NAV_PATH_STATE_READY_TO_FIND:
		case E_NAV_PATH_STATE_INVALID_DIRECT_PATH:
		{
			break;
		}

		default:
		{
			SetError( E_NAV_PATH_ERROR_IN_PROCESS );

			m_bResult = false;

			return false;
		}
	}

	// Find Correct Shape

	m_sInternalParameter.pcShape = CNavigator::GetInstance()->GetShape( m_nShapeId );
	if( m_sInternalParameter.pcShape == NULL )
	{
		SetError( E_NAV_PATH_ERROR_NO_MATCH_SHAPE );
		SetState( E_NAV_PATH_STATE_INVALID_PARAMETER );

		m_bResult = false;

		return false;
	}

	SetState( E_NAV_PATH_STATE_IN_PROCESS );

	return true;
}


//-----------------------------------------------------------------------------
// Name			: FindMeshForPath
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CNavPath::FindMeshForPath( void )
{
	if( m_sInternalParameter.pcMesh != NULL )
	{
		m_sInternalParameter.cSourcePosition.x			= m_cSourcePosition.GetX();
		m_sInternalParameter.cSourcePosition.y			= m_cSourcePosition.GetY();
		m_sInternalParameter.cSourcePosition.cell		= m_sInternalParameter.nSourceCell;

		m_sInternalParameter.cDestinationPosition.x		= m_cDestinationPosition.GetX();
		m_sInternalParameter.cDestinationPosition.y		= m_cDestinationPosition.GetY();
		m_sInternalParameter.cDestinationPosition.cell	= m_sInternalParameter.nDestinationCell;

		m_sInternalParameter.pcPeContext = NULL;

		return true;
	}


	// Find Correct World

	const CNavWorld *	pcNavWorld = down_cast<CNavWorld *>(CNavigator::GetInstance()->GetChildByPosition( m_cSourcePosition, m_cDestinationPosition ));
	if( pcNavWorld == NULL )
	{
		SetError( E_NAV_PATH_ERROR_NO_MATCH_WORLD );
		SetState( E_NAV_PATH_STATE_INVALID_PARAMETER );

		m_bResult = false;

		return false;
	}


	// Fine Correct Sector

	const CNavSector *	pcNavSector = pcNavWorld->GetSectorByPosition( m_cSourcePosition );
	if( pcNavSector == NULL )
	{
		SetError( E_NAV_PATH_ERROR_NO_MATCH_SECTOR );
		SetState( E_NAV_PATH_STATE_INVALID_PARAMETER );

		m_bResult = false;

		return false;
	}


	// Fine Correct Zone

	const CNavZone *	pcNavZone =  down_cast<const CNavZone *>(pcNavSector->GetParent());
	if( pcNavZone == NULL )
	{
		SetError( E_NAV_PATH_ERROR_NO_MATCH_ZONE );
		SetState( E_NAV_PATH_STATE_INVALID_PARAMETER );

		m_bResult = false;

		return false;
	}

	m_sInternalParameter.fComplexity = pcNavZone->GetComplexity();


	// Fine Correct Terrain

	CNavTerrain *	const	pcNavTerrain = pcNavZone->GetTerrain();
	if( pcNavTerrain == NULL )
	{
		SetError( E_NAV_PATH_ERROR_NO_TERRAIN );
		SetState( E_NAV_PATH_STATE_INVALID_PARAMETER );

		m_bResult = false;

		return false;
	}


	// Find Correct Mesh

	m_sInternalParameter.pcMesh = pcNavTerrain->GetMesh();
	if( m_sInternalParameter.pcMesh == NULL )
	{
		SetError( E_NAV_PATH_ERROR_NO_MESH );
		SetState( E_NAV_PATH_STATE_INVALID_PARAMETER );

		m_bResult = false;

		return false;
	}

	m_sInternalParameter.pcPeContext = pcNavTerrain->GetContext();


	// Convert Position

	if( pcNavTerrain->ConvertPosition( m_sInternalParameter.cSourcePosition, m_cSourcePosition ) == false )
	{
		SetError( E_NAV_PATH_ERROR_NO_SOURCE_POSITION );
		SetState( E_NAV_PATH_STATE_INVALID_PARAMETER );

		m_bResult = false;

		return false;
	}

	if( m_sInternalParameter.pcMesh->testPointCollision( m_sInternalParameter.pcShape, NULL, m_sInternalParameter.cSourcePosition ) == true )
	{
		SetError( E_NAV_PATH_ERROR_OBSTRUCT_SOURCE_POSITION );
		SetState( E_NAV_PATH_STATE_INVALID_PARAMETER );

		m_bResult = false;

		return false;
	}

	if( pcNavTerrain->ConvertPosition( m_sInternalParameter.cDestinationPosition, m_cDestinationPosition ) == false )
	{
		SetError( E_NAV_PATH_ERROR_NO_DESTINATION_POSITION );
		SetState( E_NAV_PATH_STATE_INVALID_PARAMETER );

		m_bResult = false;

		return false;
	}

	if( m_sInternalParameter.pcMesh->testPointCollision( m_sInternalParameter.pcShape, NULL, m_sInternalParameter.cDestinationPosition ) == true )
	{
		SetError( E_NAV_PATH_ERROR_OBSTRUCT_DESTINATION_POSITION );
		SetState( E_NAV_PATH_STATE_INVALID_PARAMETER );

		m_bResult = false;

		return false;
	}

	m_sInternalParameter.nSourceCell		= m_sInternalParameter.cSourcePosition.cell;
	m_sInternalParameter.nDestinationCell	= m_sInternalParameter.cDestinationPosition.cell;

	return true;
}


//-----------------------------------------------------------------------------
// Name			: CalculatePathFindThreshold
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

UInt32 CNavPath::CalculatePathFindThreshold( void ) const
{
	Fixed32		nPartial = ( m_cSourcePosition.GetX() - m_cDestinationPosition.GetX() );
	Fixed32		nSquaredDistance = nPartial * nPartial;
	nPartial = ( m_cSourcePosition.GetY() - m_cDestinationPosition.GetY() );
	nSquaredDistance += nPartial * nPartial;

	UInt32		nCalculatedThreshold = static_cast<UInt32>(static_cast<Float32>(nSquaredDistance) * NODE_PER_DISTANCE_SQUARED_FACTOR * m_sInternalParameter.fComplexity);

	return Med( NAV_QUERY_CALLBACK_THRESHOLD_MIN, nCalculatedThreshold, NAV_QUERY_CALLBACK_THRESHOLD_MAX );
}


//-----------------------------------------------------------------------------
// Name			: CreatePathMap
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CNavPath::CreatePathMap( const iPath * pcPePath, UInt32 nFlags )
{
	Assert( pcPePath != NULL );

	const cPosition *	pcPositionArray = pcPePath->getPositionArray();
	CNavPosition	cPosition;
	UInt32	nLastPositionIndex = pcPePath->size() - 1;

	cPosition.Set( pcPositionArray[nLastPositionIndex].x, pcPositionArray[nLastPositionIndex].y, 0, m_cSourcePosition.GetW() );
	if( cPosition.GetX() != m_cDestinationPosition.GetX() ||
		cPosition.GetY() != m_cDestinationPosition.GetY() )
	{
		return false;
	}

	if( nFlags == 0 )
	{
		Fixed32		nLength;
		Fixed32		nTotalLength = 0;

		m_cWayPointList.Alloc( pcPePath->size() );

		cPosition.Set( pcPositionArray[0].x, pcPositionArray[0].y, 0, m_cSourcePosition.GetW() );
		CNavigator::GetInstance()->GetHeight( cPosition );

		m_cWayPointList.SetPosition( 0, cPosition );
		m_cWayPointList.SetLength( 0, 0 );

		for( UInt32 nPointIndex = 1; nPointIndex < m_cWayPointList.GetCount(); ++nPointIndex )
		{
			cPosition.Set( pcPositionArray[nPointIndex].x, pcPositionArray[nPointIndex].y, 0, m_cSourcePosition.GetW() );
			CNavigator::GetInstance()->GetHeight( cPosition );

			m_cWayPointList.SetPosition( nPointIndex, cPosition );

			nLength = m_cWayPointList.GetPosition( nPointIndex - 1 )->GetDistance2DFrom( cPosition );
			nTotalLength += nLength;
			m_cWayPointList.SetLength( nPointIndex, nTotalLength );
		}
	}
	else
	{
		CNavPosition	cPartialSource;
		CNavPosition	cPartialDestination;
		CNavPosition	cPartialVector;
		Fixed32			nTotalLength = pcPePath->getLength();
		const Fixed32	nUnitLength = static_cast<Fixed32>(nFlags);
		Fixed32			nPartialLength;
		Fixed32			nCurrentAdvance = nUnitLength;
		Fixed32			nTotalAdvance = nUnitLength;
		UInt32			nSourceNodeCount = static_cast<UInt32>(pcPePath->size());

		m_cWayPointList.Alloc( 1 + ( nTotalLength + nFlags - 1 ) / nFlags );

		cPosition.Set( pcPositionArray[0].x, pcPositionArray[0].y, 0, m_cSourcePosition.GetW() );
		CNavigator::GetInstance()->GetHeight( cPosition );

		m_cWayPointList.SetPosition( 0, cPosition );
		m_cWayPointList.SetLength( 0, 0 );

		UInt32		nTargetPointIndex = 0;
		for( UInt32 nSourcePointIndex = 1; nSourcePointIndex < nSourceNodeCount; ++nSourcePointIndex )
		{
			cPartialSource.Set( pcPositionArray[--nSourcePointIndex].x, pcPositionArray[nSourcePointIndex].y, 0, m_cSourcePosition.GetW() );
			cPartialDestination.Set( pcPositionArray[++nSourcePointIndex].x, pcPositionArray[nSourcePointIndex].y, 0, m_cSourcePosition.GetW() );
			cPartialVector.Set( cPartialDestination );
			cPartialVector.Sub( cPartialSource );
			nPartialLength = cPartialVector.GetDistance2D();

			while( nCurrentAdvance < nPartialLength )
			{
				cPosition.Set( cPartialVector );
				cPosition.Mul( nCurrentAdvance );
				cPosition.Div( nPartialLength );
				cPosition.Add( cPartialSource );

				CNavigator::GetInstance()->GetHeight( cPosition );

				m_cWayPointList.SetPosition( ++nTargetPointIndex, cPosition );
				m_cWayPointList.SetLength( nTargetPointIndex, nTotalAdvance );

				nCurrentAdvance += nUnitLength;
				nTotalAdvance += nUnitLength;
			}

			nCurrentAdvance -= nPartialLength;
		}

		if( nTargetPointIndex < m_cWayPointList.GetCount() - 1 )
		{
			cPosition.Set( pcPositionArray[--nSourceNodeCount].x, pcPositionArray[nSourceNodeCount].y, 0, m_cSourcePosition.GetW() );
			CNavigator::GetInstance()->GetHeight( cPosition );

			m_cWayPointList.SetPosition( m_cWayPointList.GetCount() - 1, cPosition );
			m_cWayPointList.SetLength( m_cWayPointList.GetCount() - 1, nTotalLength );
		}
	}

	return true;
}


//-----------------------------------------------------------------------------
// Name			: CreateDirectPathMap
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CNavPath::CreateDirectPathMap( UInt32 nFlags )
{
	UNREFERENCED_PARAMETER( nFlags );

	CNavPosition	cPosition;
	Fixed32		nLength;

	m_cWayPointList.Alloc( 2 );

	cPosition.Set( m_cSourcePosition );
	CNavigator::GetInstance()->GetHeight( cPosition );
	m_cWayPointList.SetPosition( 0, cPosition );
	m_cWayPointList.SetLength( 0, 0 );

	cPosition.Set( m_cDestinationPosition );
	CNavigator::GetInstance()->GetHeight( cPosition );
	m_cWayPointList.SetPosition( 1, cPosition );

	nLength = m_cSourcePosition.GetDistance2DFrom( m_cDestinationPosition );
	m_cWayPointList.SetLength( 1, nLength );

	return true;
}


//*****************************************************************************
//
// End of File : CNavPath_FindPath.cpp
//
//*****************************************************************************