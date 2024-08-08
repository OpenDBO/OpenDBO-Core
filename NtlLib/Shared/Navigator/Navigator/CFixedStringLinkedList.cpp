//*****************************************************************************
// File       : CFixedStringLinkedList.cpp
// Desc       : 
// Author     : June C. Kim (cynis@hotmail.com)
// Update     : 2007. 1. 18
//*****************************************************************************

//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "StdAfx.h"

#include "CFixedStringListNode.h"

#include "CFixedStringLinkedList.h"


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

void CFixedStringLinkedList::Initialize( void )
{
	Assert( m_nCount == 0 );

	Assert( m_pcHead == NULL );
	m_pcHead = new CFixedStringListNode();
	Assert( m_pcHead != NULL );

	Assert( m_pcTail == NULL );
	m_pcTail = new CFixedStringListNode();
	Assert( m_pcTail != NULL );

	m_pcHead->SetPrev( NULL );
	m_pcHead->SetNext( m_pcTail );

	m_pcTail->SetPrev( m_pcHead );
	m_pcTail->SetNext( NULL );

	return;
}


//-----------------------------------------------------------------------------
// Name			: Destroy
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CFixedStringLinkedList::Destroy( void )
{
	Assert( m_pcHead != NULL );
	delete m_pcHead;
	m_pcHead = NULL;

	Assert( m_pcTail != NULL );
	delete m_pcTail;
	m_pcTail = NULL;

	m_nCount = 0;

	return;
}


//-----------------------------------------------------------------------------
// Name			: Insert
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CFixedStringLinkedList::Insert( const CFixedString & rcString )
{
	CFixedStringListNode *	pcNodeToInsert = new CFixedStringListNode( rcString );
	Assert( pcNodeToInsert != NULL );

	Insert( pcNodeToInsert );

	return;
}


//-----------------------------------------------------------------------------
// Name			: Insert
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CFixedStringLinkedList::Insert( CFixedStringListNode * const pcNode )
{
	pcNode->SetPrev( m_pcHead );
	pcNode->SetNext( m_pcHead->GetNext() );

	m_pcHead->GetNext()->SetPrev( pcNode );
	m_pcHead->SetNext( pcNode );

	++m_nCount;

	return;
}


//-----------------------------------------------------------------------------
// Name			: GetNode
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CFixedStringListNode * const CFixedStringLinkedList::GetNode( const CFixedString & rcString ) const
{
	CFixedStringListNode *	pcNode = m_pcHead->GetNext();

	while( pcNode != m_pcTail )
	{
		Assert( pcNode != NULL );

		if( pcNode->GetString().IsEqual( rcString ) == true )
		{
			return pcNode;
		}

		pcNode = pcNode->GetNext();
	}

	return NULL;
}


//-----------------------------------------------------------------------------
// Name			: Remove
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CFixedStringListNode * const CFixedStringLinkedList::Remove( const CFixedString & rcString )
{
	CFixedStringListNode *	pcNode = GetNode( rcString );

	if( pcNode == NULL )
	{
		return NULL;
	}

	pcNode->GetPrev()->SetNext( pcNode->GetNext() );
	pcNode->GetNext()->SetPrev( pcNode->GetPrev() );

	--m_nCount;

	return pcNode;
}


//-----------------------------------------------------------------------------
// Name			: RemoveAll
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CFixedStringLinkedList::RemoveAll( void )
{
	CFixedStringListNode *	pcNode = m_pcHead->GetNext();
	CFixedStringListNode *	pcTemp;

	while( pcNode != m_pcTail )
	{
		Assert( pcNode != NULL );

		pcTemp = pcNode;

		pcNode = pcNode->GetNext();

		delete pcTemp;
	}

	m_pcHead->SetNext( m_pcTail );
	m_pcTail->SetPrev( m_pcHead );

	m_nCount = 0;

	return;
}


//-----------------------------------------------------------------------------
// Name			: RemoveAll
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CFixedStringLinkedList::DoIt( bool (*pfnToDo)( void * pvData ) )
{
	CFixedStringListNode *	pcNode = m_pcHead->GetNext();
	CFixedStringListNode *	pcTemp;

	bool	bResult = true;

	while( pcNode != m_pcTail )
	{
		Assert( pcNode != NULL );

		pcTemp = pcNode;
		pcNode = pcNode->GetNext();

		bResult &= pcTemp->DoIt( pfnToDo );
	}

	return bResult;
}


//-----------------------------------------------------------------------------
// Name			: IsValid
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CFixedStringLinkedList::IsValid( void ) const
{
	CFixedStringListNode *	pcNode = m_pcHead->GetNext();
	UInt32		nCount = 0;

	while( pcNode != m_pcTail )
	{
		Assert( pcNode != NULL );

		++nCount;

		if( pcNode->IsValid() == false )
		{
			return false;
		}

		pcNode = pcNode->GetNext();
	}

	if( nCount != m_nCount )
	{
		return false;
	}

	return true;
}


//*****************************************************************************
//
// End of File : CFixedStringLinkedList.cpp
//
//*****************************************************************************