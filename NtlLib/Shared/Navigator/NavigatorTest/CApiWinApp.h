//*****************************************************************************
// File       : CApiWinApp.h
// Desc       : 
// Begin      : 
// Copyright  : 
// Author     : 
// Update     : 
//*****************************************************************************

#ifndef __C_API_WIN_APP_H__
#define __C_API_WIN_APP_H__

//-----------------------------------------------------------------------------
// Pre-Compiler Option Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Type Forward Definition Table
//-----------------------------------------------------------------------------

class CApiRenderer;


//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "NavigatorTest.h"


//-----------------------------------------------------------------------------
// Constant Table
//-----------------------------------------------------------------------------

#define MAX_LOADSTRING			100
#define ID_STATUS_WINDOW		100

#define CASE_FIND_PATH_NB_NC	0
#define CASE_FIND_PATH_NB_UC	1
#define CASE_FIND_PATH_UB_NC	2
#define CASE_FIND_PATH_UB_UC	3
#define CASE_COUNT				4


//-----------------------------------------------------------------------------
// Type Definition Table
//-----------------------------------------------------------------------------

typedef struct SCommand
{
	UInt32			nCommandID;
	Int32			nData0;
	Int32			nData1;
	Int32			nData2;
	Int32			nData3;
	Int32			nData4;
} SCommand;


typedef struct SSimulationNode
{
	UInt32			nTestCount;
	UInt32			nTotalTickCount;

	UInt32			nTotalNode;
	UInt32			nTotalDistance;

	SSimulationNode( void ) : nTestCount( 0 ), nTotalTickCount( 0 ), nTotalNode( 0 ), nTotalDistance( 0 ) {}
	
	void Reset( void ) { ZeroMemory( this, sizeof( SSimulationNode ) ); }

} SSimulationNode;


typedef struct SSimulationCase
{
	UInt32				nTestCount;
	SSimulationNode		sInvalidMesh;
	SSimulationNode		sInvalidPath;
	SSimulationNode		sValidPath;
	SSimulationNode		sError;

	SSimulationCase( void ) : nTestCount( 0 ) {}

	void Reset( void ) { nTestCount = 0; sInvalidMesh.Reset(); sInvalidPath.Reset(); sValidPath.Reset(); sError.Reset(); }
} SSimulationCase;


typedef struct SSimulationGroup
{
	UInt32				nShapeID;
	UInt32				nWorldID;
	Fixed32				nMinDistance;
	Fixed32				nMaxDistance;
	Fixed32				nDistanceIncreasement;
	UInt32				nTestCount;
	SSimulationCase *	psCases;

	SSimulationGroup( void )
	{
		ZeroMemory( this, sizeof( SSimulationGroup ) );

		psCases = new SSimulationCase[CASE_COUNT];
		for( UInt32 nCaseIndex = 0; nCaseIndex < CASE_COUNT; ++nCaseIndex )
		{
			psCases[nCaseIndex].nTestCount = nTestCount;
		}
	}

	SSimulationGroup( UInt32 nShapeID, UInt32 nNewWorldID, Fixed32 nNewMinDistance, Fixed32 nNewMaxDistance, Fixed32 nNewDistanceIncreasement, UInt32 nNewTestCount )
	{
		nShapeID = nShapeID;
		nWorldID = nNewWorldID;
		nTestCount = nNewTestCount;

		nMaxDistance = nNewMaxDistance;
		nMinDistance = nNewMinDistance;
		nDistanceIncreasement = nNewDistanceIncreasement;

		psCases = new SSimulationCase[CASE_COUNT];
		for( UInt32 nCaseIndex = 0; nCaseIndex < CASE_COUNT; ++nCaseIndex )
		{
			psCases[nCaseIndex].nTestCount = nTestCount;
		}

		return;
	}

	~SSimulationGroup( void ) { delete[] psCases; }

	SSimulationCase * GetCase( UInt32 nCaseIndex ) const
	{
		return &psCases[nCaseIndex];
	}

	void Reset( void )
	{
		for( UInt32 nCaseIndex = 0; nCaseIndex < CASE_COUNT; ++nCaseIndex )
		{
			psCases[nCaseIndex].Reset();
		}

		return;
	}

} SSimulationGroup;


//=============================================================================
// Name       : CApiWinApp
// Desc       : 
// Author     : 
//=============================================================================

class CApiWinApp
{
	protected:

		HINSTANCE						m_hInstance;
		HWND							m_hWnd;
		TCHAR							m_szTitle[MAX_LOADSTRING];
		TCHAR							m_szWindowClass[MAX_LOADSTRING];

		HWND							m_hStatusWindow;

		bool							m_bActive;

		CNavPath						m_cPath;

		CApiRenderer *					m_pcRenderer;

		CNavPositionList				m_cSamplePositionList;

		TCHAR							m_szConsoleText[1024];
		RECT							m_sConsoleRect;
		bool							m_bConsoleMode;

	public:

		inline							CApiWinApp( void ) { Initialize(); }

		inline							~CApiWinApp( void ) { Destroy(); }

	private:

										CApiWinApp( const CApiWinApp & rcWinApp );

		const CApiWinApp &				operator =( const CApiWinApp & rcWinApp );

	public:

		ATOM							RegisterClass( HINSTANCE hInstance );
		BOOL							InitInstance( int nCmdShow );
		int								Run( void );
		int								Idle( void );

		inline HWND						GetHwnd( void ) { return m_hWnd; }

		void							Invalidate( bool bErase = true ) const;

	protected:

		LRESULT							OnCreate( CREATESTRUCT * psCreateStruct );
		LRESULT							OnDestroy( void );

		LRESULT							OnMove( void );

		LRESULT							OnKeyUp( UINT nVirtualKey );

		LRESULT							OnLButtonDown( UINT nKeyFlags, POINT * psPoint );
		LRESULT							OnLButtonUp( UINT nKeyFlags, POINT * psPoint );

		LRESULT							OnMouseMove( UINT nKeyFlags, POINT * psPoint );

		LRESULT							OnPaint( HDC hDC, RECT * psRect = NULL );

		LRESULT							OnAbout( void );

	protected:

		void							Initialize( void );
		void							Destroy( void );
		inline void						SetHwnd( HWND hWnd ) { m_hWnd = hWnd; }

		bool							CreateStatusWindow( void );
		void							SetStatusWindowText( const CFixedString & rcString );

		void							UpdateKeyState( void );

		bool							FindPath( CNavPath & rcPath );

		bool							Simulate( void );
		bool							SimulateFindPath( SSimulationGroup & rsSimulationGroup );
		bool							SimulateFindPathFast( SSimulationGroup & rsSimulationGroup );
		bool							SimulateFindPathWithBound( SSimulationGroup & rsSimulationGroup );
		bool							SimulateFindPathFastWithBound( SSimulationGroup & rsSimulationGroup );

		bool							GenerateRandomPositionList( CNavBounding & rcGenerateBound, UInt32 nSampleCount, Fixed32 nRange );
		bool							GenerateRandomPosition( CNavPosition & cGeneratedPosition, const CNavBounding & rcGenerateBound );
		bool							GenerateRandomPositionByDistance( CNavPosition & cGeneratedPosition, const CNavPosition & rcCenter, Fixed32 nRadius );

		bool							ExecuteConsoleCommand( TCHAR * pchCommnd );
		bool							ExecuteCommand( SCommand & rsCommand );

		bool							ParseCommand( CFixedStringList & rcCommandStringList, SCommand & rsCommand );
		bool							ParseFindPathCommand( CFixedStringList & rcCommandStringList, SCommand & rsCommand );
		bool							ParseSetCameraCommand( CFixedStringList & rcCommandStringList, SCommand & rsCommand );		

		LRESULT							WndProc( UINT nMessage, WPARAM wParam, LPARAM lParam );

	public:

		static LRESULT CALLBACK			WndProc( HWND hWnd, UINT nMessage, WPARAM wParam, LPARAM lParam );
		static INT_PTR CALLBACK			AboutWndProc( HWND hWnd, UINT nMessage, WPARAM wParam, LPARAM lParam );

		static void						RegisterWindow( HWND hWnd, CApiWinApp * pcWinApp );
		static void						UnregisterWindow( HWND hWnd );

		static CApiWinApp *				GetWindow( HWND hWnd );

	public:

		static CApiWinApp *				m_pcDefaultWindow;

		static const SVersionInfo		VERSION;
};

//=============================================================================
// End of Class : CApiWinApp
//=============================================================================

#endif //__C_API_WIN_APP_H__

//*****************************************************************************
//
// End of File : CApiWinApp.h
//
//*****************************************************************************