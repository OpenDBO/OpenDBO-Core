//*****************************************************************************
// File       : CMultipleQueue.h
// Desc       : 
// Author     : June C. Kim (cynis@hotmail.com)
// Update     : 2007. 1. 18
//*****************************************************************************

#ifndef ___C_MULTIPLE_QUEUE_H___
#define ___C_MULTIPLE_QUEUE_H___

//-----------------------------------------------------------------------------
// Pre-Compiler Option Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include <list>
using std::list;

#include "CSingleQueue.h"


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

typedef list<CSingleQueue *>			TQueueList;
typedef TQueueList::iterator			TQueueListIterator;
typedef TQueueList::const_iterator		TQueueListConstIterator;


//=============================================================================
// Name       : CMultipleQueue
// Desc       : 
// Author     : 
//=============================================================================

class CMultipleQueue
{
	protected:

		TQueueList						m_tQueues;
		UInt32							m_nQueueCount;
		UInt32							m_nCount;
		UInt32							m_nData;

	public:

		inline							CMultipleQueue( void ) { Initialize(); }

		inline							~CMultipleQueue( void ) { Destroy(); }

	private:

										CMultipleQueue( const CMultipleQueue & rcQueue );

		const CMultipleQueue &			operator =( const CMultipleQueue & rcQueue );

	public:

		inline void						SetData( UInt32 nData ) { m_nData = nData; }
		inline UInt32					GetData( void ) const { return m_nData; }

		inline UInt32					GetQueueCount( void ) const { return m_nQueueCount; }

		bool							Push( SQueueData * psData );
		SQueueData *					Pop( void );
		SQueueData *					PopForce( UInt32 nID );

		inline UInt32					GetCount( void ) const { return m_nCount; }
		inline bool						IsEmpty( void ) const { return ( m_nCount == 0 ); }
		bool							IsContain( UInt32 nID );

		bool							ReportState( CLogMessageManager * pcLogMessageManager, UInt32 nLayer = 0 );

	protected:

		void							Initialize( void );
		void							Destroy( void );

		CSingleQueue *					GetQueue( UInt32 nPriority ) const;
		bool							InsertQueue( CSingleQueue * pcQueue );
};

//=============================================================================
// End of Class : CMultipleQueue
//=============================================================================

#endif //___C_MULTIPLE_QUEUE_H___

//*****************************************************************************
//
// End of File : CMultipleQueue.h
//
//*****************************************************************************