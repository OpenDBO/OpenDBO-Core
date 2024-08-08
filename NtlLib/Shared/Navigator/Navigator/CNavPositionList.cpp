//*****************************************************************************
// File       : CNavPositionList.cpp
// Desc       : 
// Author     : June C. Kim (cynis@hotmail.com)
// Update     : 2007. 1. 18
//*****************************************************************************

//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "StdAfx.h"

#include "CNavPositionList.h"


//-----------------------------------------------------------------------------
// Name Space Table
//-----------------------------------------------------------------------------

USE_NAMESPACE( nav )


//-----------------------------------------------------------------------------
// Initialize Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Name			: Create
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CNavPositionList::Create( UInt32 nCount )
{
	m_pcPositionList	= new CNavPosition[nCount];
	Assert( m_pcPositionList != NULL );

	m_pnLengthList		= new Fixed32[nCount];
	Assert( m_pnLengthList != NULL );

	m_nCount			= nCount;

	return;
}


//-----------------------------------------------------------------------------
// Name			: Destroy
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CNavPositionList::Destroy( void )
{
	if( m_pcPositionList != NULL )
	{
		delete[] m_pcPositionList;
		m_pcPositionList = NULL;
	}

	if( m_pnLengthList != NULL )
	{
		delete[] m_pnLengthList;
		m_pnLengthList = NULL;
	}

	m_nCount = 0;

	return;
}


//-----------------------------------------------------------------------------
// Name			: Set
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CNavPositionList::Set( const CNavPositionList & rcList )
{
	Destroy();

	Create( rcList.GetCount() );

	for( UInt32 nPointIndex = 0; nPointIndex < m_nCount; ++nPointIndex )
	{
		m_pcPositionList[nPointIndex].Set( *(rcList.GetPosition( nPointIndex )) );
		m_pnLengthList[nPointIndex] = rcList.GetLength( nPointIndex );
	}

	return;
}


//-----------------------------------------------------------------------------
// Name			: GetPosition
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

const CNavPosition * CNavPositionList::GetPosition( UInt32 nIndex ) const
{
	Assert( m_pcPositionList != NULL );
	Assert( nIndex < m_nCount );

	return &m_pcPositionList[nIndex];
}


//-----------------------------------------------------------------------------
// Name			: GetLength
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

Int32 CNavPositionList::GetLength( UInt32 nIndex ) const
{
	Assert( nIndex < m_nCount );

	return m_pnLengthList[nIndex];
}


//-----------------------------------------------------------------------------
// Name			: SetPosition
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CNavPositionList::SetPosition( UInt32 nIndex, const CNavPosition & rcPosition )
{
	Assert( nIndex < m_nCount );

	m_pcPositionList[nIndex].Set( rcPosition );

	return;
}


//-----------------------------------------------------------------------------
// Name			: SetLength
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CNavPositionList::SetLength( UInt32 nIndex, Fixed32 nLength )
{
	Assert( nIndex < m_nCount );

	m_pnLengthList[nIndex] = nLength;

	return;
}


//*****************************************************************************
//
// End of File : CNavPositionList.cpp
//
//*****************************************************************************