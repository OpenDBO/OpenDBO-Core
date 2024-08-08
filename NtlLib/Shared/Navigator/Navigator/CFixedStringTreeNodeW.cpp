//*****************************************************************************
// File       : CFixedStringTreeNodeW.cpp
// Desc       : 
// Author     : June C. Kim (cynis@hotmail.com)
// Update     : 2007. 1. 18
//*****************************************************************************

//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "StdAfx.h"

#include "CFixedStringTreeW.h"

#include "CFixedStringTreeNodeW.h"


//-----------------------------------------------------------------------------
// Name Space Table
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// Initialize Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Name			: Initialize
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CFixedStringTreeNodeW::Initialize( void )
{
	SetPrev( this );
	SetNext( this );

	m_pcChild = NULL;

	return;
}


//-----------------------------------------------------------------------------
// Name			: Destroy
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CFixedStringTreeNodeW::Destroy( bool bDestroyNode )
{
	UNREFERENCED_PARAMETER( bDestroyNode );

	if( m_pcChild != NULL )
	{
		delete m_pcChild;
		m_pcChild = NULL;
	}

	return;
}


//-----------------------------------------------------------------------------
// Name			: GetChildCount
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

UInt32 CFixedStringTreeNodeW::GetChildCount( void ) const
{
	if( GetChildList() == NULL )
	{
		return 0;
	}
	else
	{
		return m_pcChild->GetCount();
	}
}


//-----------------------------------------------------------------------------
// Name			: FindChild
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CFixedStringTreeNodeW * CFixedStringTreeNodeW::FindChild( UInt32 nOrder ) const
{
	if( GetChildCount() == 0 )
	{
		return NULL;
	}

	return m_pcChild->GetAt( nOrder );
}


//-----------------------------------------------------------------------------
// Name			: FindChild
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CFixedStringTreeNodeW * CFixedStringTreeNodeW::FindChild( const CFixedStringW rcKeyword ) const
{
	if( GetChildCount() == 0 )
	{
		return NULL;
	}

	return m_pcChild->Find( rcKeyword );
}


//-----------------------------------------------------------------------------
// Name			: FindChild
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CFixedStringTreeNodeW * CFixedStringTreeNodeW::FindChild( const CFixedStringW rcKeyword, UInt32 nId ) const
{
	if( GetChildCount() == 0 )
	{
		return NULL;
	}

	return m_pcChild->Find( rcKeyword, nId );
}


//-----------------------------------------------------------------------------
// Name			: PushFrontChild
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CFixedStringTreeNodeW::PushFrontChild( CFixedStringTreeNodeW * pcFront )
{
	if( m_pcChild == NULL )
	{
		m_pcChild = new CFixedStringTreeW;
		Assert( m_pcChild != NULL );
	}

	m_pcChild->PushFront( pcFront );

	return;
}


//-----------------------------------------------------------------------------
// Name			: PushBackChild
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CFixedStringTreeNodeW::PushBackChild( CFixedStringTreeNodeW * pcBack )
{
	if( m_pcChild == NULL )
	{
		m_pcChild = new CFixedStringTreeW;

		Assert( m_pcChild != NULL );
	}

	m_pcChild->PushBack( pcBack );

	return;
}


//-----------------------------------------------------------------------------
// Name			: PopFrontChild
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CFixedStringTreeNodeW * CFixedStringTreeNodeW::PopFrontChild( void )
{
	if( m_pcChild == NULL )
	{
		return NULL;
	}

	return m_pcChild->PopFront();
}


//-----------------------------------------------------------------------------
// Name			: PopBackChild
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CFixedStringTreeNodeW * CFixedStringTreeNodeW::PopBackChild( void )
{
	if( m_pcChild == NULL )
	{
		return NULL;
	}

	return m_pcChild->PopBack();
}


//*****************************************************************************
//
// End of File : CFixedStringTreeNodeW.cpp
//
//*****************************************************************************