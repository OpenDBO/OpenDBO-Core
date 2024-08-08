//*****************************************************************************
// File       : CNavIndexedFaceList.cpp
// Desc       : 
// Author     : June C. Kim (cynis@hotmail.com)
// Update     : 2007. 1. 18
//*****************************************************************************

//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "StdAfx.h"

#include "CNavIndexedFaceList.h"


//-----------------------------------------------------------------------------
// Name Space Table
//-----------------------------------------------------------------------------

USE_NAMESPACE( navex )


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

void CNavIndexedFaceList::Create( UInt32 nCount )
{
	m_pcFaceList	= new CNavIndexedFace[nCount];
	Assert( m_pcFaceList != NULL );

	m_nCount	= nCount;

	return;
}


//-----------------------------------------------------------------------------
// Name			: Destroy
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CNavIndexedFaceList::Destroy( void )
{
	if( m_pcFaceList != NULL )
	{
		delete[] m_pcFaceList;
		m_pcFaceList = NULL;
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

void CNavIndexedFaceList::Set( const CNavIndexedFaceList & rcList )
{
	Destroy();

	Create( rcList.GetCount() );

	for( UInt32 nFaceIndex = 0; nFaceIndex < m_nCount; ++nFaceIndex )
	{
		m_pcFaceList[nFaceIndex].Set( *(rcList.GetFace( nFaceIndex )) );
	}

	return;
}


//-----------------------------------------------------------------------------
// Name			: GetFace
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CNavIndexedFace * CNavIndexedFaceList::GetFace( UInt32 nIndex ) const
{
	Assert( m_pcFaceList != NULL );
	Assert( nIndex < m_nCount );

	return &m_pcFaceList[nIndex];
}


//-----------------------------------------------------------------------------
// Name			: SetFace
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CNavIndexedFaceList::SetFace( UInt32 nIndex, const CNavIndexedFace & rcFace )
{
	Assert( nIndex < m_nCount );

	m_pcFaceList[nIndex].Set( rcFace );

	return;
}


//*****************************************************************************
//
// End of File : CNavIndexedFaceList.cpp
//
//*****************************************************************************