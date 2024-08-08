//*****************************************************************************
// File       : CApiWinApp_Command.cpp
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

#include "CApiRenderer.h"
#include "CNavQueryCallback.h"
#include "CApiWinApp.h"


//-----------------------------------------------------------------------------
// Initialize Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Name			: ExecuteConsoleCommand
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CApiWinApp::ExecuteConsoleCommand( TCHAR * pchCommnd )
{
	CFixedStringList	cCommndStringList;

	TCHAR	achCommand[1024];
	_stprintf_s( achCommand, 1024, _T( "%s ;" ), &pchCommnd[1] );

	cCommndStringList.CreateFrom( achCommand );

	SCommand	sParsedCommand;

	if( ParseCommand( cCommndStringList, sParsedCommand ) == false )
	{
		return false;
	}

	return ExecuteCommand( sParsedCommand );
}


//-----------------------------------------------------------------------------
// Name			: ExecuteCommand
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CApiWinApp::ExecuteCommand( SCommand & rsCommand )
{
	switch( rsCommand.nCommandID )
	{
		case 1:
		{
			CNavPosition		cSourcePosition( rsCommand.nData0, rsCommand.nData1, 0 );
			CNavPosition		cDestinationPosition( rsCommand.nData2, rsCommand.nData3, 0 );

			m_cPath.SetSourcePosition( cSourcePosition );
			m_cPath.SetDestinationPosition( cDestinationPosition );
			m_cPath.SetShape( 0 );

			bool	bResult = FindPath( m_cPath );

			if( bResult == false )
			{
				return false;
			}

			m_pcRenderer->Invalidate();
			Invalidate();

			return true;
		}

		case 2:
		{
			CNavPosition2D		cCameraPosition;
			cCameraPosition.Set( rsCommand.nData0, rsCommand.nData1 );

			m_pcRenderer->SetCamera( cCameraPosition, m_pcRenderer->GetMeterPerPixel() );

			InvalidateRect( m_hWnd, NULL, TRUE );

			return true;
		}
	}

	return false;
}


//-----------------------------------------------------------------------------
// Name			: ParseCommand
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CApiWinApp::ParseCommand( CFixedStringList & rcCommandStringList, SCommand & rsCommand )
{
	CFixedString *	pcCommand = rcCommandStringList.GetAt( 0 );

	if( pcCommand == NULL )
	{
		return false;
	}

	static const CFixedString	KEYWORD_FINDPATH( _T( "FINDPATH" ) );

	if( pcCommand->IsEquivalent( KEYWORD_FINDPATH ) == true )
	{
		rsCommand.nCommandID = 1;

		return ParseFindPathCommand( rcCommandStringList, rsCommand );
	}

	static const CFixedString	KEYWORD_SETCAMERA( _T( "SETCAMERA" ) );

	if( pcCommand->IsEquivalent( KEYWORD_SETCAMERA ) == true )
	{
		rsCommand.nCommandID = 2;

		return ParseSetCameraCommand( rcCommandStringList, rsCommand );
	}

	return false;
}


//-----------------------------------------------------------------------------
// Name			: ParseFindPathCommand
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CApiWinApp::ParseFindPathCommand( CFixedStringList & rcCommandStringList, SCommand & rsCommand )
{
	CFixedString *	pcValue = rcCommandStringList.GetAt( 1 );

	if( pcValue == NULL )
	{
		return false;
	}

	rsCommand.nData0 = static_cast<Int32>(pcValue->ConvertToInt32());

	pcValue = rcCommandStringList.GetAt( 2 );

	if( pcValue == NULL )
	{
		return false;
	}

	rsCommand.nData1 = static_cast<Int32>(pcValue->ConvertToInt32());

	pcValue = rcCommandStringList.GetAt( 3 );

	if( pcValue == NULL )
	{
		return false;
	}

	rsCommand.nData2 = static_cast<Int32>(pcValue->ConvertToInt32());

	pcValue = rcCommandStringList.GetAt( 4 );

	if( pcValue == NULL )
	{
		return false;
	}

	rsCommand.nData3 = static_cast<Int32>(pcValue->ConvertToInt32());

	pcValue = rcCommandStringList.GetAt( 5 );

	if( pcValue == NULL )
	{
		rsCommand.nData4 = 0;

		return true;
	}

	rsCommand.nData4 = static_cast<Int32>(pcValue->ConvertToInt32());

	return true;
}


//-----------------------------------------------------------------------------
// Name			: ParseSetCameraCommand
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CApiWinApp::ParseSetCameraCommand( CFixedStringList & rcCommandStringList, SCommand & rsCommand )
{
	CFixedString *	pcValue = rcCommandStringList.GetAt( 1 );

	if( pcValue == NULL )
	{
		return false;
	}

	rsCommand.nData0 = static_cast<Int32>(pcValue->ConvertToInt32());

	pcValue = rcCommandStringList.GetAt( 2 );

	if( pcValue == NULL )
	{
		return false;
	}

	rsCommand.nData1 = static_cast<Int32>(pcValue->ConvertToInt32());

	return true;
}


//-----------------------------------------------------------------------------
// Name			: FindPath
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CApiWinApp::FindPath( CNavPath & rcPath )
{
	DWORD	dwTickCount = ::GetTickCount();
#ifdef FIND_PATH_RESTRICT_BOUND
	CNavBounding	cBound;
	cBound.SetMinFloat( -FIND_PATH_BOUND_X, -FIND_PATH_BOUND_X, 0 );
	cBound.SetMaxFloat( +FIND_PATH_BOUND_X, +FIND_PATH_BOUND_X, 0 );

#ifdef FIND_PATH_RESTRICT_COUNT
	CNavQueryCallback::SetThreshold( 1024 );
	bool	bPathFound = rcPath.FindPathFast( cBound );
#else //FIND_PATH_RESTRICT_COUNT
	bool	bPathFound = rcPath.FindPath( cBound );
#endif //FIND_PATH_RESTRICT_COUNT
#else //FIND_PATH_RESTRICT_BOUND

#ifdef FIND_PATH_RESTRICT_COUNT
	CNavQueryCallback::SetThreshold( 1024 );
	bool	bPathFound = rcPath.FindPathFast();
#else //FIND_PATH_RESTRICT_COUNT
	bool	bPathFound = rcPath.FindPath();
#endif //FIND_PATH_RESTRICT_COUNT
#endif //FIND_PATH_RESTRICT_BOUND

	dwTickCount = ::GetTickCount() - dwTickCount;

	// Log Result

	TCHAR	szText[1024];

	if( bPathFound == true )
	{
		_stprintf_s( szText, 1024, _T( "Path : (%.2f, %.2f, %.2f) - (%.2f, %.2f, %.2f), Result : Success, Node : %d, Time : %d" ),
					m_cPath.GetSourcePosition().GetFloatX(), m_cPath.GetSourcePosition().GetFloatY(), m_cPath.GetSourcePosition().GetFloatZ(),
					m_cPath.GetDestinationPosition().GetFloatX(), m_cPath.GetDestinationPosition().GetFloatY(), m_cPath.GetDestinationPosition().GetFloatZ(),
					m_cPath.GetWayPointCount(), dwTickCount );

		for( UInt32 nIndex = 0; nIndex < m_cPath.GetWayPointCount(); ++nIndex )
		{
			g_Log( szText );

			_stprintf_s( szText, 1024, _T( "  Waypoint : (%.2f, %.2f, %.2f), Distance : %7.2f" ),
						m_cPath.GetWayPoint( nIndex )->GetFloatX(), m_cPath.GetWayPoint( nIndex )->GetFloatY(), m_cPath.GetWayPoint( nIndex )->GetFloatZ(),
						Fixed32ToFloat32( m_cPath.GetWayPointList().GetLength( nIndex ) ) );
		}
	}
	else
	{
		_stprintf_s( szText, 1024, _T( "Path : (%.2f, %.2f, %.2f) - (%.2f, %.2f, %.2f), Result : Failure, Code : %d, Time : %d" ),
					m_cPath.GetSourcePosition().GetFloatX(), m_cPath.GetSourcePosition().GetFloatY(), m_cPath.GetSourcePosition().GetFloatZ(),
					m_cPath.GetDestinationPosition().GetFloatX(), m_cPath.GetDestinationPosition().GetFloatY(), m_cPath.GetDestinationPosition().GetFloatZ(),
					m_cPath.GetError(), dwTickCount );
	}

	g_Log( szText );

	dwTickCount = ::GetTickCount();
	bool	bDirectPath = m_cPath.FindDirectPath();
	dwTickCount = ::GetTickCount() - dwTickCount;

	if( bDirectPath == true )
	{
		_stprintf_s( szText, 1024, _T( "DirectPath : Exist, Time = %d " ),
					dwTickCount );
	}
	else
	{
		_stprintf_s( szText, 1024, _T( "DirectPath : Not Exist, Time = %d " ),
					dwTickCount );
	}

	g_Log( szText );

	return true;
}


//*****************************************************************************
//
// End of File : CApiWinApp_Command.cpp
//
//*****************************************************************************