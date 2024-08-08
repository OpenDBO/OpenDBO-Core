//*****************************************************************************
// File       : CNavQueryCallback.cpp
// Desc       : 
//            : Frequency및 Threshold를 고정으로 사용할 것이라면,
//            : Frequency를 늘리는 것이 Threshold를 늘리는 것보다,
//            : Performance 측면에서 이득임
//            : ex) Frequency = 1, Threshold = 3000 => Frequency = 3000, Threshold = 1
// Author     : June C. Kim (cynis@hotmail.com)
// Update     : 2007. 1. 18
//*****************************************************************************

//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "StdAfx.h"

#include "CNavQueryCallback.h"


//-----------------------------------------------------------------------------
// Name Space Table
//-----------------------------------------------------------------------------

USE_NAMESPACE( nav )


//-----------------------------------------------------------------------------
// Initialize Table
//-----------------------------------------------------------------------------

UInt32	CNavQueryCallback::m_nFrequency = NAV_QUERY_CALLBACK_FREQUENCY_MIN;
UInt32	CNavQueryCallback::m_nThreshold = NAV_QUERY_CALLBACK_THRESHOLD_DEFAULT;


//-----------------------------------------------------------------------------
// Name			: desiredCallBackFrequency
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

long CNavQueryCallback::desiredCallBackFrequency()
{
	return m_nFrequency;
}


//-----------------------------------------------------------------------------
// Name			: abort
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CNavQueryCallback::abort()
{
	if( ++m_nCount >= m_nThreshold )
	{
		return true;
	}

	return false;
}


//*****************************************************************************
//
// End of File : CNavQueryCallback.cpp
//
//*****************************************************************************