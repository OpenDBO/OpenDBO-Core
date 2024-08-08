//*****************************************************************************
// File       : CBaseThreadManager.h
// Desc       : 
// Author     : June C. Kim (cynis@hotmail.com)
// Update     : 2007. 1. 18
//*****************************************************************************

#ifndef ___C_BASE_THREAD_MANAGER_H___
#define ___C_BASE_THREAD_MANAGER_H___

//-----------------------------------------------------------------------------
// Pre-Compiler Option Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "CBaseThread.h"
#include "CSingleQueue.h"
#include "CMultipleQueue.h"
#include "CActiveThreadList.h"


//-----------------------------------------------------------------------------
// Name Space Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Type Forward Definition Table
//-----------------------------------------------------------------------------

class CLogMessageManager;


//-----------------------------------------------------------------------------
// Constant Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Type Definition Table
//-----------------------------------------------------------------------------


//=============================================================================
// Name       : CBaseThread
// Desc       : 
// Author     : 
//=============================================================================

class CBaseThreadManager
{
	protected:

		UInt32							m_nRemainThreadCount;

		CMultipleQueue					m_cRemainThreadQueue;
		CActiveThreadList				m_cActiveThreadList;

	public:

		inline							CBaseThreadManager( void ) { Initialize(); }

		inline virtual					~CBaseThreadManager( void ) { Destroy(); }

	private:

										CBaseThreadManager( const CBaseThreadManager & rcManager );

		const CBaseThreadManager &		operator =( const CBaseThreadManager & rcManager );

	public:

		// Common

		bool							AddThread( CBaseThread * pcThread );
		CBaseThread *					RemoveThread( void );

		virtual bool					Create( UInt32 nActiveThreadSize );

		virtual bool					Run( void );

		virtual bool					ReportState( CLogMessageManager * pcLogMessageManager = NULL, UInt32 nLayer = 0 );

		// Exclusive

	protected:

		// Common

		virtual void					Initialize( void );
		virtual void					Destroy( void );

		// Exclusive
};

//=============================================================================
// End of Class : CBaseThreadManager
//=============================================================================

#endif //___C_BASE_THREAD_MANAGER_H___

//*****************************************************************************
//
// End of File : CBaseThreadManager.h
//
//*****************************************************************************