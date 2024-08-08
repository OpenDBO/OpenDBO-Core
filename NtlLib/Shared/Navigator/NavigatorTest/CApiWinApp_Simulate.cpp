//*****************************************************************************
// File       : CApiWinApp_StatusWindow.cpp
// Desc       : 
// Begin      : 
// Copyright  : 
// Author     : 
// Update     : 
//*****************************************************************************

//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "StdAfx.h"

#include "CApiWinApp.h"


//-----------------------------------------------------------------------------
// Initialize Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Name			: Simulate
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CApiWinApp::Simulate( void )
{
	srand( GetTickCount() ); 

	TCHAR	szLog[1024];

	CNavBounding	cSamplingBound;

	cSamplingBound.Set( -100000, -100000, -500, +100000, +100000, +500, 0 );
	
	SSimulationGroup	sSimulationGroup;
	sSimulationGroup.nShapeID = 0;
	sSimulationGroup.nWorldID = 0;
	sSimulationGroup.nMinDistance = 0;
	sSimulationGroup.nMaxDistance = 10000;
	sSimulationGroup.nDistanceIncreasement = 500;
	sSimulationGroup.nTestCount = 1000000;

	for( Fixed32 nDistance = sSimulationGroup.nMinDistance; nDistance <= sSimulationGroup.nMaxDistance; nDistance += sSimulationGroup.nDistanceIncreasement )
	{
		_stprintf_s( szLog, 1024, _T( "\r\n\r\n\r\nDistance : %7.2f" ), Fixed32ToFloat32( nDistance ) );
		g_Log( szLog );

		if( GenerateRandomPositionList( cSamplingBound, sSimulationGroup.nTestCount, nDistance ) == false )
		{
			return false;
		}

		if( SimulateFindPath( sSimulationGroup ) == false )
		{
			return false;
		}

		if( SimulateFindPathFast( sSimulationGroup ) == false )
		{
			return false;
		}

		if( SimulateFindPathWithBound( sSimulationGroup ) == false )
		{
			return false;
		}

		if( SimulateFindPathFastWithBound( sSimulationGroup ) == false )
		{
			return false;
		}

		sSimulationGroup.Reset();
	}

	return true;
}


//-----------------------------------------------------------------------------
// Name			: Simulate
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CApiWinApp::GenerateRandomPositionList( CNavBounding & rcSamplingBound, UInt32 nSampleCount, Fixed32 nRange )
{
	m_cSamplePositionList.Alloc( nSampleCount * 2 );

	CNavPosition	cSourcePosition;
	CNavPosition	cDestinationPosition;
	bool	bValidPosition;

	for( UInt32 nSampleIndex = 0; nSampleIndex < nSampleCount; ++nSampleIndex )
	{
		do
		{
			bValidPosition = GenerateRandomPosition( cSourcePosition, rcSamplingBound );
		} while( bValidPosition == false );

		bValidPosition = GenerateRandomPositionByDistance( cDestinationPosition, cSourcePosition, nRange );

		m_cSamplePositionList.SetPosition( nSampleIndex * 2 + 0, cSourcePosition );
		m_cSamplePositionList.SetPosition( nSampleIndex * 2 + 1, cDestinationPosition );
	}

	return true;
}


//-----------------------------------------------------------------------------
// Name			: SimulateFindPath
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CApiWinApp::SimulateFindPath( SSimulationGroup & rsSimulationGroup )
{
	CNavPosition	cSourcePosition;
	CNavPosition	cDestinationPosition;
	CNavBounding	cSamplingBound;
	CNavPath		cPath;

	bool	bFindPath = false;
	TCHAR	szLog[1024];

	_stprintf_s( szLog, 1024, _T( "\r\n\r\nSimulate : FindPath()\r\n" ) );
	g_Log( szLog );

	DWORD	dwElapsedTick;

	SSimulationCase *	psCase = rsSimulationGroup.GetCase( CASE_FIND_PATH_NB_NC );

	for( UInt32 nSampleIndex = 0; nSampleIndex < rsSimulationGroup.nTestCount; ++nSampleIndex )
	{
		cSourcePosition.Set( *(m_cSamplePositionList.GetPosition( nSampleIndex * 2 + 0 )) );
		cDestinationPosition.Set( *(m_cSamplePositionList.GetPosition( nSampleIndex * 2 + 1 )) );

		dwElapsedTick = ::GetTickCount();

		cPath.SetSourcePosition( cSourcePosition );
		cPath.SetDestinationPosition( cDestinationPosition );
		cPath.SetShape( 0 );
	
		bFindPath = cPath.FindPath();

		dwElapsedTick = ::GetTickCount() - dwElapsedTick;

		switch( cPath.GetState() )
		{
			case E_NAV_PATH_STATE_VALID_PATH:
			{
				++psCase->sValidPath.nTestCount;
				psCase->sValidPath.nTotalTickCount += dwElapsedTick;
				psCase->sValidPath.nTotalNode += cPath.GetWayPointCount() - 2;
				psCase->sValidPath.nTotalDistance += cPath.GetLength();

				break;
			}

			case E_NAV_PATH_STATE_INVALID_PATH:
			case E_NAV_PATH_STATE_INVALID_DIRECT_PATH:
			{
				++psCase->sInvalidPath.nTestCount;
				psCase->sInvalidPath.nTotalTickCount += dwElapsedTick;

				break;
			}

			case E_NAV_PATH_STATE_INVALID_PARAMETER:
			{
				++psCase->sInvalidMesh.nTestCount;
				psCase->sInvalidMesh.nTotalTickCount += dwElapsedTick;

				break;
			}

			default:
			{
				++psCase->sError.nTestCount;
				psCase->sError.nTotalTickCount += dwElapsedTick;

				_stprintf_s( szLog, 1024, _T( "State : %d, Error : %d" ), cPath.GetState(), cPath.GetError() );
				g_Log( szLog );
			}
		}
	}

	if( psCase->sValidPath.nTestCount > 0 )
	{
		_stprintf_s( szLog, 1024, _T( "Sucess   : Count : %d, Time : %d (Avg : %8.3fms), Length : %7.2f (Avg : %6.2fm), Node : %d (Avg : %6.2f)" ),
					psCase->sValidPath.nTestCount,
					psCase->sValidPath.nTotalTickCount,
					(Float32)(psCase->sValidPath.nTotalTickCount) / (Float32)(psCase->sValidPath.nTestCount),
					(Float32)(psCase->sValidPath.nTotalDistance) / 100.0f,
					(Float32)(psCase->sValidPath.nTotalDistance) / (Float32)(psCase->sValidPath.nTestCount) / 100.0f,
					psCase->sValidPath.nTotalNode,
					(Float32)(psCase->sValidPath.nTotalNode) / (Float32)(psCase->sValidPath.nTestCount) );
		g_Log( szLog );
	}
	else
	{
		_stprintf_s( szLog, 1024, _T( "Sucess   : No Case" ) );
		g_Log( szLog );
	}

	if( psCase->sInvalidPath.nTestCount > 0 )
	{
		_stprintf_s( szLog, 1024, _T( "Obstacle : Count : %d, Time : %d (Avg : %8.3fms)" ),
					psCase->sInvalidPath.nTestCount,
					psCase->sInvalidPath.nTotalTickCount,
					(Float32)(psCase->sInvalidPath.nTotalTickCount) / (Float32)(psCase->sInvalidPath.nTestCount) );
		g_Log( szLog );
	}
	else
	{
		_stprintf_s( szLog, 1024, _T( "Obstacle : No Case" ) );
		g_Log( szLog );
	}

	if( psCase->sInvalidMesh.nTestCount > 0 )
	{
		_stprintf_s( szLog, 1024, _T( "Mesh     : Count : %d, Time : %d (Avg : %8.3fms)" ),
					psCase->sInvalidMesh.nTestCount,
					psCase->sInvalidMesh.nTotalTickCount,
					(Float32)(psCase->sInvalidMesh.nTotalTickCount) / (Float32)(psCase->sInvalidMesh.nTestCount) );
		g_Log( szLog );
	}
	else
	{
		_stprintf_s( szLog, 1024, _T( "Mesh     : No Case" ) );
		g_Log( szLog );
	}

	if( psCase->sError.nTestCount > 0 )
	{
		_stprintf_s( szLog, 1024, _T( "Error    : Count : %d, Time : %d (Avg : %8.3fms)" ),
					psCase->sError.nTestCount,
					psCase->sError.nTotalTickCount,
					(Float32)(psCase->sError.nTotalTickCount) / (Float32)(psCase->sError.nTestCount) );
		g_Log( szLog );
	}
	else
	{
		_stprintf_s( szLog, 1024, _T( "Error    : No Case" ) );
		g_Log( szLog );
	}

	return true;
}


//-----------------------------------------------------------------------------
// Name			: SimulateFindPathFast
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CApiWinApp::SimulateFindPathFast( SSimulationGroup & rsSimulationGroup )
{
	CNavPosition	cSourcePosition;
	CNavPosition	cDestinationPosition;
	CNavBounding	cSamplingBound;
	CNavPath		cPath;

	bool	bFindPath = false;
	TCHAR	szLog[1024];

	_stprintf_s( szLog, 1024, _T( "\r\n\r\nSimulate : FindPathFast()\r\n" ) );
	g_Log( szLog );

	DWORD	dwElapsedTick;

	SSimulationCase *	psCase = rsSimulationGroup.GetCase( CASE_FIND_PATH_NB_UC );

	for( UInt32 nSampleIndex = 0; nSampleIndex < rsSimulationGroup.nTestCount; ++nSampleIndex )
	{
		cSourcePosition.Set( *(m_cSamplePositionList.GetPosition( nSampleIndex * 2 + 0 )) );
		cDestinationPosition.Set( *(m_cSamplePositionList.GetPosition( nSampleIndex * 2 + 1 )) );

		dwElapsedTick = ::GetTickCount();

		cPath.SetSourcePosition( cSourcePosition );
		cPath.SetDestinationPosition( cDestinationPosition );
		cPath.SetShape( 0 );
	
		bFindPath = cPath.FindPathFast();

		dwElapsedTick = ::GetTickCount() - dwElapsedTick;

		switch( cPath.GetState() )
		{
			case E_NAV_PATH_STATE_VALID_PATH:
			{
				++psCase->sValidPath.nTestCount;
				psCase->sValidPath.nTotalTickCount += dwElapsedTick;
				psCase->sValidPath.nTotalNode += cPath.GetWayPointCount() - 2;
				psCase->sValidPath.nTotalDistance += cPath.GetLength();

				break;
			}

			case E_NAV_PATH_STATE_INVALID_PATH:
			case E_NAV_PATH_STATE_INVALID_DIRECT_PATH:
			{
				++psCase->sInvalidPath.nTestCount;
				psCase->sInvalidPath.nTotalTickCount += dwElapsedTick;

				break;
			}

			case E_NAV_PATH_STATE_INVALID_PARAMETER:
			{
				++psCase->sInvalidMesh.nTestCount;
				psCase->sInvalidMesh.nTotalTickCount += dwElapsedTick;

				break;
			}

			default:
			{
				++psCase->sError.nTestCount;
				psCase->sError.nTotalTickCount += dwElapsedTick;

				_stprintf_s( szLog, 1024, _T( "State : %d, Error : %d" ), cPath.GetState(), cPath.GetError() );
				g_Log( szLog );
			}
		}
	}

	if( psCase->sValidPath.nTestCount > 0 )
	{
		_stprintf_s( szLog, 1024, _T( "Sucess   : Count : %d, Time : %d (Avg : %8.3fms), Length : %7.2f (Avg : %6.2fm), Node : %d (Avg : %6.2f)" ),
					psCase->sValidPath.nTestCount,
					psCase->sValidPath.nTotalTickCount,
					(Float32)(psCase->sValidPath.nTotalTickCount) / (Float32)(psCase->sValidPath.nTestCount),
					(Float32)(psCase->sValidPath.nTotalDistance) / 100.0f,
					(Float32)(psCase->sValidPath.nTotalDistance) / (Float32)(psCase->sValidPath.nTestCount) / 100.0f,
					psCase->sValidPath.nTotalNode,
					(Float32)(psCase->sValidPath.nTotalNode) / (Float32)(psCase->sValidPath.nTestCount) );
		g_Log( szLog );
	}
	else
	{
		_stprintf_s( szLog, 1024, _T( "Sucess   : No Case" ) );
		g_Log( szLog );
	}

	if( psCase->sInvalidPath.nTestCount > 0 )
	{
		_stprintf_s( szLog, 1024, _T( "Obstacle : Count : %d, Time : %d (Avg : %8.3fms)" ),
					psCase->sInvalidPath.nTestCount,
					psCase->sInvalidPath.nTotalTickCount,
					(Float32)(psCase->sInvalidPath.nTotalTickCount) / (Float32)(psCase->sInvalidPath.nTestCount) );
		g_Log( szLog );
	}
	else
	{
		_stprintf_s( szLog, 1024, _T( "Obstacle : No Case" ) );
		g_Log( szLog );
	}

	if( psCase->sInvalidMesh.nTestCount > 0 )
	{
		_stprintf_s( szLog, 1024, _T( "Mesh     : Count : %d, Time : %d (Avg : %8.3fms)" ),
					psCase->sInvalidMesh.nTestCount,
					psCase->sInvalidMesh.nTotalTickCount,
					(Float32)(psCase->sInvalidMesh.nTotalTickCount) / (Float32)(psCase->sInvalidMesh.nTestCount) );
		g_Log( szLog );
	}
	else
	{
		_stprintf_s( szLog, 1024, _T( "Mesh     : No Case" ) );
		g_Log( szLog );
	}

	if( psCase->sError.nTestCount > 0 )
	{
		_stprintf_s( szLog, 1024, _T( "Error    : Count : %d, Time : %d (Avg : %8.3fms)" ),
					psCase->sError.nTestCount,
					psCase->sError.nTotalTickCount,
					(Float32)(psCase->sError.nTotalTickCount) / (Float32)(psCase->sError.nTestCount) );
		g_Log( szLog );
	}
	else
	{
		_stprintf_s( szLog, 1024, _T( "Error    : No Case" ) );
		g_Log( szLog );
	}

	return true;
}


//-----------------------------------------------------------------------------
// Name			: SimulateFindPathWithBound
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CApiWinApp::SimulateFindPathWithBound( SSimulationGroup & rsSimulationGroup )
{
	CNavPosition	cSourcePosition;
	CNavPosition	cDestinationPosition;
	CNavBounding	cSamplingBound;
	CNavPath		cPath;
	CNavBounding	cSearchBound;
	cSearchBound.Set( -5000, -5000, 0, +5000, +5000, 0, 0 );

	bool	bFindPath = false;
	TCHAR	szLog[1024];

	_stprintf_s( szLog, 1024, _T( "\r\n\r\nSimulate : FindPathWithBound()\r\n" ) );
	g_Log( szLog );

	DWORD	dwElapsedTick;

	SSimulationCase *	psCase = rsSimulationGroup.GetCase( CASE_FIND_PATH_UB_NC );

	for( UInt32 nSampleIndex = 0; nSampleIndex < rsSimulationGroup.nTestCount; ++nSampleIndex )
	{
		cSourcePosition.Set( *(m_cSamplePositionList.GetPosition( nSampleIndex * 2 + 0 )) );
		cDestinationPosition.Set( *(m_cSamplePositionList.GetPosition( nSampleIndex * 2 + 1 )) );

		dwElapsedTick = ::GetTickCount();

		cPath.SetSourcePosition( cSourcePosition );
		cPath.SetDestinationPosition( cDestinationPosition );
		cPath.SetShape( 0 );
	
		bFindPath = cPath.FindPath( cSearchBound );

		dwElapsedTick = ::GetTickCount() - dwElapsedTick;

		switch( cPath.GetState() )
		{
			case E_NAV_PATH_STATE_VALID_PATH:
			{
				++psCase->sValidPath.nTestCount;
				psCase->sValidPath.nTotalTickCount += dwElapsedTick;
				psCase->sValidPath.nTotalNode += cPath.GetWayPointCount() - 2;
				psCase->sValidPath.nTotalDistance += cPath.GetLength();

				break;
			}

			case E_NAV_PATH_STATE_INVALID_PATH:
			case E_NAV_PATH_STATE_INVALID_DIRECT_PATH:
			{
				++psCase->sInvalidPath.nTestCount;
				psCase->sInvalidPath.nTotalTickCount += dwElapsedTick;

				break;
			}

			case E_NAV_PATH_STATE_INVALID_PARAMETER:
			{
				++psCase->sInvalidMesh.nTestCount;
				psCase->sInvalidMesh.nTotalTickCount += dwElapsedTick;

				break;
			}

			default:
			{
				++psCase->sError.nTestCount;
				psCase->sError.nTotalTickCount += dwElapsedTick;

				_stprintf_s( szLog, 1024, _T( "State : %d, Error : %d" ), cPath.GetState(), cPath.GetError() );
				g_Log( szLog );
			}
		}
	}

	if( psCase->sValidPath.nTestCount > 0 )
	{
		_stprintf_s( szLog, 1024, _T( "Sucess   : Count : %d, Time : %d (Avg : %8.3fms), Length : %7.2f (Avg : %6.2fm), Node : %d (Avg : %6.2f)" ),
					psCase->sValidPath.nTestCount,
					psCase->sValidPath.nTotalTickCount,
					(Float32)(psCase->sValidPath.nTotalTickCount) / (Float32)(psCase->sValidPath.nTestCount),
					(Float32)(psCase->sValidPath.nTotalDistance) / 100.0f,
					(Float32)(psCase->sValidPath.nTotalDistance) / (Float32)(psCase->sValidPath.nTestCount) / 100.0f,
					psCase->sValidPath.nTotalNode,
					(Float32)(psCase->sValidPath.nTotalNode) / (Float32)(psCase->sValidPath.nTestCount) );
		g_Log( szLog );
	}
	else
	{
		_stprintf_s( szLog, 1024, _T( "Sucess   : No Case" ) );
		g_Log( szLog );
	}

	if( psCase->sInvalidPath.nTestCount > 0 )
	{
		_stprintf_s( szLog, 1024, _T( "Obstacle : Count : %d, Time : %d (Avg : %8.3fms)" ),
					psCase->sInvalidPath.nTestCount,
					psCase->sInvalidPath.nTotalTickCount,
					(Float32)(psCase->sInvalidPath.nTotalTickCount) / (Float32)(psCase->sInvalidPath.nTestCount) );
		g_Log( szLog );
	}
	else
	{
		_stprintf_s( szLog, 1024, _T( "Obstacle : No Case" ) );
		g_Log( szLog );
	}

	if( psCase->sInvalidMesh.nTestCount > 0 )
	{
		_stprintf_s( szLog, 1024, _T( "Mesh     : Count : %d, Time : %d (Avg : %8.3fms)" ),
					psCase->sInvalidMesh.nTestCount,
					psCase->sInvalidMesh.nTotalTickCount,
					(Float32)(psCase->sInvalidMesh.nTotalTickCount) / (Float32)(psCase->sInvalidMesh.nTestCount) );
		g_Log( szLog );
	}
	else
	{
		_stprintf_s( szLog, 1024, _T( "Mesh     : No Case" ) );
		g_Log( szLog );
	}

	if( psCase->sError.nTestCount > 0 )
	{
		_stprintf_s( szLog, 1024, _T( "Error    : Count : %d, Time : %d (Avg : %8.3fms)" ),
					psCase->sError.nTestCount,
					psCase->sError.nTotalTickCount,
					(Float32)(psCase->sError.nTotalTickCount) / (Float32)(psCase->sError.nTestCount) );
		g_Log( szLog );
	}
	else
	{
		_stprintf_s( szLog, 1024, _T( "Error    : No Case" ) );
		g_Log( szLog );
	}

	return true;
}


//-----------------------------------------------------------------------------
// Name			: SimulateFindPathFastWithBound
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CApiWinApp::SimulateFindPathFastWithBound( SSimulationGroup & rsSimulationGroup )
{
	CNavPosition	cSourcePosition;
	CNavPosition	cDestinationPosition;
	CNavBounding	cSamplingBound;
	CNavPath		cPath;
	CNavBounding	cSearchBound;
	cSearchBound.Set( -5000, -5000, 0, +5000, +5000, 0, 0 );

	bool	bFindPath = false;
	TCHAR	szLog[1024];

	_stprintf_s( szLog, 1024, _T( "\r\n\r\nSimulate : FindPathWithBound()\r\n" ) );
	g_Log( szLog );

	DWORD	dwElapsedTick;

	SSimulationCase *	psCase = rsSimulationGroup.GetCase( CASE_FIND_PATH_UB_UC );

	for( UInt32 nSampleIndex = 0; nSampleIndex < rsSimulationGroup.nTestCount; ++nSampleIndex )
	{
		cSourcePosition.Set( *(m_cSamplePositionList.GetPosition( nSampleIndex * 2 + 0 )) );
		cDestinationPosition.Set( *(m_cSamplePositionList.GetPosition( nSampleIndex * 2 + 1 )) );

		dwElapsedTick = ::GetTickCount();

		cPath.SetSourcePosition( cSourcePosition );
		cPath.SetDestinationPosition( cDestinationPosition );
		cPath.SetShape( 0 );
	
		bFindPath = cPath.FindPathFast( cSearchBound );

		dwElapsedTick = ::GetTickCount() - dwElapsedTick;

		switch( cPath.GetState() )
		{
			case E_NAV_PATH_STATE_VALID_PATH:
			{
				++psCase->sValidPath.nTestCount;
				psCase->sValidPath.nTotalTickCount += dwElapsedTick;
				psCase->sValidPath.nTotalNode += cPath.GetWayPointCount() - 2;
				psCase->sValidPath.nTotalDistance += cPath.GetLength();

				break;
			}

			case E_NAV_PATH_STATE_INVALID_PATH:
			case E_NAV_PATH_STATE_INVALID_DIRECT_PATH:
			{
				++psCase->sInvalidPath.nTestCount;
				psCase->sInvalidPath.nTotalTickCount += dwElapsedTick;

				break;
			}

			case E_NAV_PATH_STATE_INVALID_PARAMETER:
			{
				++psCase->sInvalidMesh.nTestCount;
				psCase->sInvalidMesh.nTotalTickCount += dwElapsedTick;

				break;
			}

			default:
			{
				++psCase->sError.nTestCount;
				psCase->sError.nTotalTickCount += dwElapsedTick;

				_stprintf_s( szLog, 1024, _T( "State : %d, Error : %d" ), cPath.GetState(), cPath.GetError() );
				g_Log( szLog );
			}
		}
	}

	if( psCase->sValidPath.nTestCount > 0 )
	{
		_stprintf_s( szLog, 1024, _T( "Sucess   : Count : %d, Time : %d (Avg : %8.3fms), Length : %7.2f (Avg : %6.2fm), Node : %d (Avg : %6.2f)" ),
					psCase->sValidPath.nTestCount,
					psCase->sValidPath.nTotalTickCount,
					(Float32)(psCase->sValidPath.nTotalTickCount) / (Float32)(psCase->sValidPath.nTestCount),
					(Float32)(psCase->sValidPath.nTotalDistance) / 100.0f,
					(Float32)(psCase->sValidPath.nTotalDistance) / (Float32)(psCase->sValidPath.nTestCount) / 100.0f,
					psCase->sValidPath.nTotalNode,
					(Float32)(psCase->sValidPath.nTotalNode) / (Float32)(psCase->sValidPath.nTestCount) );
		g_Log( szLog );
	}
	else
	{
		_stprintf_s( szLog, 1024, _T( "Sucess   : No Case" ) );
		g_Log( szLog );
	}

	if( psCase->sInvalidPath.nTestCount > 0 )
	{
		_stprintf_s( szLog, 1024, _T( "Obstacle : Count : %d, Time : %d (Avg : %8.3fms)" ),
					psCase->sInvalidPath.nTestCount,
					psCase->sInvalidPath.nTotalTickCount,
					(Float32)(psCase->sInvalidPath.nTotalTickCount) / (Float32)(psCase->sInvalidPath.nTestCount) );
		g_Log( szLog );
	}
	else
	{
		_stprintf_s( szLog, 1024, _T( "Obstacle : No Case" ) );
		g_Log( szLog );
	}

	if( psCase->sInvalidMesh.nTestCount > 0 )
	{
		_stprintf_s( szLog, 1024, _T( "Mesh     : Count : %d, Time : %d (Avg : %8.3fms)" ),
					psCase->sInvalidMesh.nTestCount,
					psCase->sInvalidMesh.nTotalTickCount,
					(Float32)(psCase->sInvalidMesh.nTotalTickCount) / (Float32)(psCase->sInvalidMesh.nTestCount) );
		g_Log( szLog );
	}
	else
	{
		_stprintf_s( szLog, 1024, _T( "Mesh     : No Case" ) );
		g_Log( szLog );
	}

	if( psCase->sError.nTestCount > 0 )
	{
		_stprintf_s( szLog, 1024, _T( "Error    : Count : %d, Time : %d (Avg : %8.3fms)" ),
					psCase->sError.nTestCount,
					psCase->sError.nTotalTickCount,
					(Float32)(psCase->sError.nTotalTickCount) / (Float32)(psCase->sError.nTestCount) );
		g_Log( szLog );
	}
	else
	{
		_stprintf_s( szLog, 1024, _T( "Error    : No Case" ) );
		g_Log( szLog );
	}

	return true;
}


//-----------------------------------------------------------------------------
// Name			: SimulateFindDirectPath
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------
/*
bool CApiWinApp::SimulateFindDirectPath( SSimulationGroup & rsSimulationGroup )
{
	CNavPosition	cSourcePosition;
	CNavPosition	cDestinationPosition;
	CNavBounding	cSamplingBound;
	CNavPath		cPath;

	bool	bValidPosition = false;
	bool	bFindPath = false;

	TCHAR	szLog[1024];

	_stprintf_s( szLog, 1024, _T( "\r\n\r\nSimulate : FindDirectPath()\r\n" ) );
	g_Log( szLog );

	static const UInt32		REPEAT_COUNT = 100;
	static const Float32	REPEAT_COUNTF = (Float32)(REPEAT_COUNT);
	DWORD	dwElapsedTick;

	SSimulationGroup	sSimulationGroup( 0, 0, 0, 10000, 500, 100 );
	cSamplingBound.SetMin( 520000, 260000, -500, 0 );
	cSamplingBound.SetMax( 630000, 440000, +500, 0 );

	for( Fixed32 nDistance = sSimulationGroup.nMinDistance; nDistance <= sSimulationGroup.nMaxDistance; nDistance += sSimulationGroup.nDistanceIncreasement )
	{
		SSimulationCase *	psCase = sSimulationGroup.GetCase( nDistance );

		_stprintf_s( szLog, 1024, _T( "\r\nDistance : %7.2f" ), Fixed32ToFloat32( nDistance ) );
			g_Log( szLog );

		for( UInt32 nSampleIndex = 0; nSampleIndex < sSimulationGroup.nTestCount; ++nSampleIndex )
		{
			do
			{
				bValidPosition = GenerateRandomPosition( cSourcePosition, cSamplingBound );
			} while( bValidPosition == false );

			bValidPosition = GenerateRandomPositionByDistance( cDestinationPosition, cSourcePosition, nDistance );

			dwElapsedTick = ::GetTickCount();
			for( UInt32 nRepeatIndex = 0; nRepeatIndex < REPEAT_COUNT; ++nRepeatIndex )
			{
				cPath.SetSourcePosition( cSourcePosition );
				cPath.SetDestinationPosition( cDestinationPosition );
				cPath.SetShape( 0 );
	
				bFindPath = cPath.FindDirectPath( 0 );
			}
			dwElapsedTick = ::GetTickCount() - dwElapsedTick;

			switch( cPath.GetState() )
			{
				case E_NAV_PATH_STATE_VALID_PATH:
				{
					++psCase->sValidPath.nTestCount;
					psCase->sValidPath.nTotalTickCount += dwElapsedTick;
					psCase->sValidPath.nTotalNode += cPath.GetWayPointCount() - 2;
					psCase->sValidPath.nTotalDistance += cPath.GetLength();

					break;
				}

				case E_NAV_PATH_STATE_INVALID_PATH:
				case E_NAV_PATH_STATE_INVALID_DIRECT_PATH:
				{
					++psCase->sInvalidPath.nTestCount;
					psCase->sInvalidPath.nTotalTickCount += dwElapsedTick;

					break;
				}

				case E_NAV_PATH_STATE_INVALID_PARAMETER:
				{
					++psCase->sInvalidMesh.nTestCount;
					psCase->sInvalidMesh.nTotalTickCount += dwElapsedTick;

					break;
				}

				default:
				{
					++psCase->sError.nTestCount;
					psCase->sError.nTotalTickCount += dwElapsedTick;

					_stprintf_s( szLog, 1024, _T( "State : %d, Error : %d" ), cPath.GetState(), cPath.GetError() );
					g_Log( szLog );
				}
			}
		}

		if( psCase->sValidPath.nTestCount > 0 )
		{
			_stprintf_s( szLog, 1024, _T( "Sucess   : Count : %d, Time : %d (Avg : %7.3fsec), Length : %7.2f (Avg : %6.2fm), Node : %d (Avg : %6.2f)" ),
						psCase->sValidPath.nTestCount,
						psCase->sValidPath.nTotalTickCount, (Float32)(psCase->sValidPath.nTotalTickCount) / REPEAT_COUNTF / (Float32)(psCase->sValidPath.nTestCount) / 1000.0f,
						(Float32)(psCase->sValidPath.nTotalDistance) / 100.0f, (Float32)(psCase->sValidPath.nTotalDistance) / (Float32)(psCase->sValidPath.nTestCount) / 100.0f,
						psCase->sValidPath.nTotalNode, (Float32)(psCase->sValidPath.nTotalNode) / (Float32)(psCase->sValidPath.nTestCount) );
			g_Log( szLog );
		}
		else
		{
			_stprintf_s( szLog, 1024, _T( "Sucess   : No Case" ) );
			g_Log( szLog );
		}

		if( psCase->sInvalidPath.nTestCount > 0 )
		{
			_stprintf_s( szLog, 1024, _T( "Obstacle : Count : %d, Time : %d (Avg : %7.3fsec)" ),
						psCase->sInvalidPath.nTestCount,
						psCase->sInvalidPath.nTotalTickCount, (Float32)(psCase->sInvalidPath.nTotalTickCount) / REPEAT_COUNTF / (Float32)(psCase->sInvalidPath.nTestCount) / 1000.0f );
			g_Log( szLog );
		}
		else
		{
			_stprintf_s( szLog, 1024, _T( "Obstacle : No Case" ) );
			g_Log( szLog );
		}

		if( psCase->sInvalidMesh.nTestCount > 0 )
		{
			_stprintf_s( szLog, 1024, _T( "Mesh     : Count : %d, Time : %d (Avg : %7.3fsec)" ),
						psCase->sInvalidMesh.nTestCount,
						psCase->sInvalidMesh.nTotalTickCount, (Float32)(psCase->sInvalidMesh.nTotalTickCount) / REPEAT_COUNTF / (Float32)(psCase->sInvalidMesh.nTestCount) / 1000.0f );
			g_Log( szLog );
		}
		else
		{
			_stprintf_s( szLog, 1024, _T( "Mesh     : No Case" ) );
			g_Log( szLog );
		}

		if( psCase->sError.nTestCount > 0 )
		{
			_stprintf_s( szLog, 1024, _T( "Error    : Count : %d, Time : %d (Avg : %7.3fsec)" ),
						psCase->sError.nTestCount,
						psCase->sError.nTotalTickCount, (Float32)(psCase->sError.nTotalTickCount) / REPEAT_COUNTF / (Float32)(psCase->sInvalidMesh.nTestCount) / 1000.0f );
			g_Log( szLog );
		}
		else
		{
			_stprintf_s( szLog, 1024, _T( "Error    : No Case" ) );
			g_Log( szLog );
		}
	}

	return true;
}
*/

//-----------------------------------------------------------------------------
// Name			: GenerateRandomPosition
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CApiWinApp::GenerateRandomPosition( CNavPosition & cGeneratedPosition, const CNavBounding & rcGenerateBound )
{
	Fixed32		nBoundX = rcGenerateBound.GetSizeX();
	Fixed32		nBoundY = rcGenerateBound.GetSizeY();

	Fixed32		nRandomX = ( ( rand() * rand() ) % nBoundX ) + rcGenerateBound.GetMin().GetX();
	Fixed32		nRandomY = ( ( rand() * rand() ) % nBoundY ) + rcGenerateBound.GetMin().GetY();

	cGeneratedPosition.Set( nRandomX, nRandomY, 0, 0 );

	return !(CNavigator::GetInstance()->IsBlockedPosition( cGeneratedPosition, 0 ));
}


//-----------------------------------------------------------------------------
// Name			: GenerateRandomPositionByDistance
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CApiWinApp::GenerateRandomPositionByDistance( CNavPosition & cGeneratedPosition, const CNavPosition & rcCenter, Fixed32 nRadius )
{
	Float32		fRotation = static_cast<Float32>(( rand() % 360 ) - 180);
	Float32		fCosTheta = cosf( fRotation );
	Float32		fSinTheta = sinf( fRotation );

	Float32		fRadius = Fixed32ToFloat32( nRadius );

	Fixed32		nRandomX = Float32ToFixed32( fCosTheta * fRadius + rcCenter.GetFloatX() );
	Fixed32		nRandomY = Float32ToFixed32( fSinTheta * fRadius + rcCenter.GetFloatZ() );

	cGeneratedPosition.Set( nRandomX, nRandomY, 0, 0 );

	return !(CNavigator::GetInstance()->IsBlockedPosition( cGeneratedPosition, 0 ));
}


//*****************************************************************************
//
// End of File : CApiWinApp_Simulate.cpp
//
//*****************************************************************************