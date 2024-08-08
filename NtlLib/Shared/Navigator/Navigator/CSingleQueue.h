//*****************************************************************************
// File       : CSingleQueue.h
// Desc       : 
// Author     : June C. Kim (cynis@hotmail.com)
// Update     : 2007. 1. 18
//*****************************************************************************

#ifndef ___C_SINGLE_QUEUE_H___
#define ___C_SINGLE_QUEUE_H___

//-----------------------------------------------------------------------------
// Pre-Compiler Option Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include <queue>
using std::queue;

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

#define QUEUE_REPORT_BUFFER_SIZE		( 1024 )


//-----------------------------------------------------------------------------
// Type Definition Table
//-----------------------------------------------------------------------------

typedef struct SQueueData
{
	UInt32			nID;
	Int32			nPriority;
	void *			pvData;

public:

	inline 			SQueueData( void ) : nID( 0 ), nPriority( 0 ), pvData( NULL ) {}
} SQueueData;

typedef queue<SQueueData *>		TQueue;


//=============================================================================
// Name       : CSingleQueue
// Desc       : 
// Author     : 
//=============================================================================

class CSingleQueue
{
	protected:

		TQueue							m_tQueue;
		UInt32							m_nCount;
		Int32							m_nData;

	public:

		inline							CSingleQueue( void ) { Initialize(); }
		inline							CSingleQueue( UInt32 nData ) { Initialize(); m_nData = nData; }

		inline							~CSingleQueue( void ) { Destroy(); }

	private:

										CSingleQueue( const CSingleQueue & rcQueue );

		const CSingleQueue &			operator =( const CSingleQueue & rcQueue );

	public:

		inline void						SetData( UInt32 nData ) { m_nData = nData; }
		inline UInt32					GetData( void ) const { return m_nData; }

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
};

//=============================================================================
// End of Class : CSingleQueue
//=============================================================================

#endif //___C_SINGLE_QUEUE_H___

//*****************************************************************************
//
// End of File : CSingleQueue.h
//
//*****************************************************************************