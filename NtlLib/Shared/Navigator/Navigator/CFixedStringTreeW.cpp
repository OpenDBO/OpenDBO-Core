//*****************************************************************************
// File       : CFixedStringTreeW.cpp
// Desc       : 
// Author     : June C. Kim (cynis@hotmail.com)
// Update     : 2007. 1. 18
//*****************************************************************************

//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "StdAfx.h"

#include "CFixedStringTreeW.h"


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

void CFixedStringTreeW::Initialize( void )
{
	m_nCount = 0;

	m_pcHead = new CFixedStringTreeNodeW( NULL );
	m_pcTail = new CFixedStringTreeNodeW( NULL );

	m_pcHead->SetPrev( m_pcHead );
	m_pcHead->SetNext( m_pcTail );

	m_pcTail->SetPrev( m_pcHead );
	m_pcTail->SetNext( m_pcTail );

	return;
}


//-----------------------------------------------------------------------------
// Name			: Destroy
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CFixedStringTreeW::Destroy( void )
{
	RemoveAll();

	if( m_pcHead != NULL )
	{
		delete m_pcHead;
		m_pcHead = NULL;
	}

	if( m_pcTail != NULL )
	{
		delete m_pcTail;
		m_pcTail = NULL;
	}

	return;
}


//-----------------------------------------------------------------------------
// Name			: RemoveAll
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CFixedStringTreeW::RemoveAll( void )
{
	if( m_nCount == 0 )
	{
		return;
	}

	CFixedStringTreeNodeW *	pcRemove = m_pcHead->GetNext();

	while( pcRemove != m_pcTail )
	{
		m_pcHead->SetNext( pcRemove->GetNext() );

		delete pcRemove;

		pcRemove = m_pcHead->GetNext();
	}

	m_pcTail->SetPrev( m_pcHead );

	m_nCount = 0;

	return;
}


//-----------------------------------------------------------------------------
// Name			: PushFront
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CFixedStringTreeW::PushFront( CFixedStringTreeNodeW * pcFront )
{
	Assert( pcFront != NULL );

	pcFront->SetPrev( m_pcHead );
	pcFront->SetNext( m_pcHead->GetNext() );

	m_pcHead->GetNext()->SetPrev( pcFront );
	m_pcHead->SetNext( pcFront );

	++m_nCount;

	return;
}


//-----------------------------------------------------------------------------
// Name			: PushBack
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CFixedStringTreeW::PushBack( CFixedStringTreeNodeW * pcBack )
{
	Assert( pcBack != NULL );

	pcBack->SetPrev( m_pcTail->GetPrev() );
	pcBack->SetNext( m_pcTail );

	m_pcTail->GetPrev()->SetNext( pcBack );
	m_pcTail->SetPrev( pcBack );

	++m_nCount;

	return;
}


//-----------------------------------------------------------------------------
// Name			: PopFront
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CFixedStringTreeNodeW * CFixedStringTreeW::PopFront( void )
{
	Assert( IsEmpty() == false );

	CFixedStringTreeNodeW *	pcPop = m_pcHead->GetNext();

	m_pcHead->SetNext( pcPop->GetNext() );
	pcPop->GetNext()->SetPrev( m_pcHead );

	--m_nCount;

	return pcPop;
}


//-----------------------------------------------------------------------------
// Name			: PopBack
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CFixedStringTreeNodeW * CFixedStringTreeW::PopBack( void )
{
	Assert( IsEmpty() == false );

	CFixedStringTreeNodeW *	pcPop = m_pcTail->GetPrev();

	m_pcTail->SetPrev( pcPop->GetPrev() );
	pcPop->GetPrev()->SetNext( m_pcTail );

	--m_nCount;

	return pcPop;
}


//-----------------------------------------------------------------------------
// Name			: Find
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CFixedStringTreeNodeW * CFixedStringTreeW::Find( const CFixedStringW rcKeyword ) const
{
	CFixedStringTreeNodeW *		pcCurrent = m_pcHead->GetNext();

	while( pcCurrent->GetNext() != pcCurrent )
	{
		if( pcCurrent->GetString().IsEqual( rcKeyword ) )
		{
			return pcCurrent;
		}

		pcCurrent = pcCurrent->GetNext();
	}

	return NULL;
}


//-----------------------------------------------------------------------------
// Name			: Find
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CFixedStringTreeNodeW * CFixedStringTreeW::Find( const CFixedStringW rcKeyword, UInt32 nId ) const
{
	CFixedStringTreeNodeW *		pcCurrent = m_pcHead->GetNext();

	while( pcCurrent->GetNext() != pcCurrent )
	{
		if( pcCurrent->GetId() == nId &&
			pcCurrent->GetString().IsEqual( rcKeyword ) )
		{
			return pcCurrent;
		}

		pcCurrent = pcCurrent->GetNext();
	}

	return NULL;
}


//-----------------------------------------------------------------------------
// Name			: GetAt
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CFixedStringTreeNodeW * CFixedStringTreeW::GetAt( UInt32 nPosition ) const
{
	if( nPosition >= GetCount() )
	{
		return NULL;
	}

	CFixedStringTreeNodeW *	pcNode = GetFirst();

	while( nPosition != 0 )
	{
		--nPosition;

		pcNode = pcNode->GetNext();
	}

	return pcNode;
}


//*****************************************************************************
//
// End of File : CFixedStringTreeW.cpp
//
//*****************************************************************************