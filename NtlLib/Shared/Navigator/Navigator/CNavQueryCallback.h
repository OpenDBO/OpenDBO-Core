//*****************************************************************************
// File       : CNavQueryCallback.h
// Desc       : 
// Author     : June C. Kim (cynis@hotmail.com)
// Update     : 2007. 1. 18
//*****************************************************************************

#ifndef ___C_NAV_QUERY_CALLBACK_H___
#define ___C_NAV_QUERY_CALLBACK_H___

//-----------------------------------------------------------------------------
// Pre-Compiler Option Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "NavBase.h"


//-----------------------------------------------------------------------------
// Name Space Table
//-----------------------------------------------------------------------------

BEGIN_NAMESPACE( nav )


//-----------------------------------------------------------------------------
// Type Forward Definition Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Constant Table
//-----------------------------------------------------------------------------

#define NAV_QUERY_CALLBACK_FREQUENCY_MIN			static_cast<UInt32>(1)
#define NAV_QUERY_CALLBACK_THRESHOLD_MIN			static_cast<UInt32>(2)
#define NAV_QUERY_CALLBACK_THRESHOLD_MAX			static_cast<UInt32>(65536)

#define NAV_QUERY_CALLBACK_THRESHOLD_DEFAULT		( 1000 )

#define NODE_PER_DISTANCE_SQUARED_FACTOR			( 0.000001f )


//-----------------------------------------------------------------------------
// Type Definition Table
//-----------------------------------------------------------------------------


//=============================================================================
// Name       : CNavQueryCallback
// Desc       : 
// Author     : 
//=============================================================================

class CNavQueryCallback : public iQueryCallBack
{
	protected:

		UInt32							m_nCount;

	public:

		inline							CNavQueryCallback( void ) {}
		inline							~CNavQueryCallback( void ) {}

	public:

		long							desiredCallBackFrequency();
		bool							abort();

		void							ResetCount( void ) { m_nCount = 0; }

	protected:

		static UInt32					m_nFrequency;
		static UInt32					m_nThreshold;

	public:

		static UInt32					GetFrequency( void ) { return m_nFrequency; }
		static void						SetFrequency( UInt32 nFreqauency ) { m_nFrequency = Max( NAV_QUERY_CALLBACK_FREQUENCY_MIN, nFreqauency ); }

		static UInt32					GetThreshold( void ) { return m_nThreshold; }
		static void						SetThreshold( UInt32 nThreshold ) { m_nThreshold = nThreshold; }
};

//=============================================================================
// End of Class : CNavQueryCallback
//=============================================================================

END_NAMESPACE( nav )

#endif //___C_NAV_QUERY_CALLBACK_H___

//*****************************************************************************
//
// End of File : CNavQueryCallback.h
//
//*****************************************************************************