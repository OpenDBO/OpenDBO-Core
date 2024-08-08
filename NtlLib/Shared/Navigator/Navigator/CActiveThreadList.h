//*****************************************************************************
// File       : CActiveThreadList.h
// Desc       : 
// Author     : June C. Kim (cynis@hotmail.com)
// Update     : 2007. 1. 18
//*****************************************************************************

#ifndef ___C_ACTIVE_THREAD_LIST_H___
#define ___C_ACTIVE_THREAD_LIST_H___

//-----------------------------------------------------------------------------
// Pre-Compiler Option Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "CBaseThread.h"


//-----------------------------------------------------------------------------
// Name Space Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Type Forward Definition Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Constant Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Type Definition Table
//-----------------------------------------------------------------------------


//=============================================================================
// Name       : CActiveThreadList
// Desc       : 
// Author     : 
//=============================================================================

class CActiveThreadList
{
	protected:

		CBaseThread **					m_ppcThreads;
		HANDLE * 						m_phThreads;
		UInt32							m_nCount;
		UInt32							m_nSize;

	public:

		inline							CActiveThreadList( void ) : m_ppcThreads( NULL ), m_phThreads( NULL ), m_nCount( 0 ), m_nSize( 0 ) {}
		inline							CActiveThreadList( UInt32 nSize ) : m_ppcThreads( NULL ), m_phThreads( NULL ), m_nCount( 0 ), m_nSize( 0 ) { Reserve( nSize ); }

		inline							~CActiveThreadList( void ) { Destroy(); }

	public:

		UInt32							AddThread( CBaseThread * pcThread );
		CBaseThread *					RemoveThread( UInt32 nIndex );

		bool							ResumeAll( void );
		bool							SuspendAll( void );
		bool							TerminateAll( void );

		inline HANDLE *					GetHandleList( void ) const { return m_phThreads; }

		inline UInt32					GetCount( void ) const { return m_nCount; }
		inline UInt32					GetSize( void ) const { return m_nSize; }

		void							Reserve( UInt32 nSize );

		bool							ReportState( CLogMessageManager * pcLogMessageManager, UInt32 nLayer = 0 );

	protected:

		void							Destroy( void );
};

//=============================================================================
// End of Class : CActiveThreadList
//=============================================================================

#endif //___C_ACTIVE_THREAD_LIST_H___

//*****************************************************************************
//
// End of File : CActiveThreadList.h
//
//*****************************************************************************