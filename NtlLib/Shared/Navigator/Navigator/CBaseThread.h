//*****************************************************************************
// File       : CBaseThread.h
// Desc       : 
// Author     : June C. Kim (cynis@hotmail.com)
// Update     : 2007. 1. 18
//*****************************************************************************

#ifndef ___C_BASE_THREAD_H___
#define ___C_BASE_THREAD_H___

//-----------------------------------------------------------------------------
// Pre-Compiler Option Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "BaseType.h"
#include "CLogMessageManager.h"
#include "UserDefinedAssert.h"


//-----------------------------------------------------------------------------
// Name Space Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Type Forward Definition Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Constant Table
//-----------------------------------------------------------------------------

#define	BASE_THREAD_REPORT_BUFFER_SIZE	( 1024 )


//-----------------------------------------------------------------------------
// Type Definition Table
//-----------------------------------------------------------------------------

typedef enum EThreadState
{
	E_THREAD_STATE_NONE					= 0,	
	E_THREAD_STATE_RUN					= 1,
	E_THREAD_STATE_STOP					= 2,
	E_THREAD_STATE_END					= 3,
	E_THREAD_STATE_COUNT
} EThreadState;


typedef enum EThreadPriority
{
	E_THREAD_PRIORITY_IDLE				= THREAD_PRIORITY_IDLE,
	E_THREAD_PRIORITY_LOWEST			= THREAD_PRIORITY_LOWEST,
	E_THREAD_PRIORITY_BELOW_NORMAL		= THREAD_PRIORITY_BELOW_NORMAL,
	E_THREAD_PRIORITY_NORMAL			= THREAD_PRIORITY_NORMAL,
	E_THREAD_PRIORITY_ABOVE_NORMAL		= THREAD_PRIORITY_ABOVE_NORMAL,
	E_THREAD_PRIORITY_HIGHEST			= THREAD_PRIORITY_HIGHEST,
	E_THREAD_PRIORITY_TIME_CRITICAL		= THREAD_PRIORITY_TIME_CRITICAL
} EThreadPriority;


//=============================================================================
// Name       : CBaseThread
// Desc       : 
// Author     : 
//=============================================================================

class CBaseThread
{
	protected:

		HANDLE							m_hThread;
		UInt32							m_nID;
		bool							m_bLoop;

		EThreadState					m_eState;
		EThreadPriority					m_ePriority;

		UInt32							m_nLastError;

		HANDLE							m_hFinished;

		HANDLE							m_hObserver;

	public:

		inline							CBaseThread( void ) { Initialize(); }

		inline virtual					~CBaseThread( void ) { Destroy(); }

	private:

										CBaseThread( const CBaseThread & rcThread );

		const CBaseThread &				operator =( const CBaseThread & rcThread );

	public:

		// Common

		bool							Create( EThreadPriority ePriority, bool bSuspended = false, bool bLoop = false );
		bool							Resume( void );
		bool							Suspend( void );
		bool							Terminate( void );

		inline EThreadState				GetState( void ) const { return m_eState; }
		inline HANDLE					GetHandle( void ) const { return m_hThread; }
		inline UInt32					GetID( void ) const { return m_nID; }

		inline bool						IsRun( void ) const { return ( m_eState == E_THREAD_STATE_RUN ); }

		inline EThreadPriority			GetPriority( void ) const { return m_ePriority; }
		void							SetPriority( EThreadPriority ePriority );

		bool							IsLoop( void ) const { return m_bLoop; }

		UInt32							GetLastError( void ) const { return m_nLastError; }
		void							SetLastError( UInt32 nLastError );

		void							SetObserver( HANDLE hObserver = NULL );
		inline HANDLE					GetObserver( void ) const { return m_hObserver; }
		void							ResetObserver( void );

		// Exclusive

		bool							ReportState( CLogMessageManager * pcLogMessageManager, UInt32 nLayer = 0 );

		void							Report( TCHAR * pchBuffer );
		void							Report( UInt32 nErrorCode );
		void							Report( TCHAR * pchBuffer, UInt32 nValue );

	protected:

		// Common

		virtual void					Initialize( void );
		virtual void					Destroy( void );

		void							SetLoop( bool bLoop );
		void							SetFinished( void );

		inline virtual UInt32			Begin( void ) { return 0; }
		virtual UInt32					Run( void ) = 0;
		inline virtual UInt32			End( void ) { return 0; }

		void							SetState( EThreadState eState );

		// Exclusive

	public:

		static DWORD WINAPI				ThreadProc( LPVOID lpParameter );
};

//=============================================================================
// End of Class : CBaseThread
//=============================================================================

#endif //___C_BASE_THREAD_H___

//*****************************************************************************
//
// End of File : CBaseThread.h
//
//*****************************************************************************