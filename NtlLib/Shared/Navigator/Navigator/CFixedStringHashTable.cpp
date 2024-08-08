//*****************************************************************************
// File       : CFixedStringHashTable.cpp
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

#include "CFixedStringHashTable.h"


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

void CFixedStringHashTable::Initialize( void )
{
	Assert( m_nCount == 0 );
	Assert( m_nSize == 0 );

	Assert( m_pcHead == NULL );
	m_pcHead = new CFixedStringLinkedList();
	Assert( m_pcHead != NULL );

	Assert( m_pcTail == NULL );
	m_pcTail = new CFixedStringLinkedList();
	Assert( m_pcTail != NULL );

	m_pcHead->SetPrev( NULL );
	m_pcHead->SetNext( m_pcTail );

	m_pcTail->SetPrev( m_pcHead );
	m_pcTail->SetNext( NULL );

	return;
}


//-----------------------------------------------------------------------------
// Name			: Initialize
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CFixedStringHashTable::Destroy( void )
{
	Assert( m_pcHead != NULL );
	delete m_pcHead;
	m_pcHead = NULL;

	Assert( m_pcTail != NULL );
	delete m_pcTail;
	m_pcTail = NULL;

	if( m_ppcStringList != NULL )
	{
		Assert( m_nSize > 0 );

		delete[] m_ppcStringList;
		m_ppcStringList = NULL;

		m_nSize = 0;
	}

	return;
}


//-----------------------------------------------------------------------------
// Name			: Reserve
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CFixedStringHashTable::Reserve( UInt32 nSize )
{
	RemoveAll();

	if( nSize != NULL )
	{
		m_ppcStringList = new CFixedStringLinkedList *[nSize];
		Assert( m_ppcStringList != NULL );

		ZeroMemory( m_ppcStringList, sizeof( CFixedStringLinkedList * ) * nSize );

		m_nSize = nSize;
	}

	return;
}


//-----------------------------------------------------------------------------
// Name			: Insert
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CFixedStringHashTable::Insert( const CFixedString & rcString )
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

void CFixedStringHashTable::Insert( CFixedStringListNode * const pcNode )
{
	Assert( m_ppcStringList != NULL );
	Assert( m_nSize != 0 );

	UInt32		nSlot = ( pcNode->GetString().GetKey() % m_nSize );

	if( m_ppcStringList[nSlot] == NULL )
	{
		m_ppcStringList[nSlot] = new CFixedStringLinkedList( pcNode );
	}
	else
	{
		m_ppcStringList[nSlot]->Insert( pcNode );
	}

	m_ppcStringList[nSlot]->SetPrev( m_pcHead );
	m_ppcStringList[nSlot]->SetNext( m_pcHead->GetNext() );

	m_pcHead->GetNext()->SetPrev( m_ppcStringList[nSlot] );
	m_pcHead->SetNext( m_ppcStringList[nSlot] );

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

CFixedStringListNode * const CFixedStringHashTable::GetNode( const CFixedString & rcString ) const
{
	Assert( m_ppcStringList != NULL );
	Assert( m_nSize != 0 );

	UInt32		nSlot = rcString.GetKey() % m_nSize;

	if( m_ppcStringList[nSlot] == NULL )
	{
		return NULL;
	}

	return m_ppcStringList[nSlot]->GetNode( rcString );
}


//-----------------------------------------------------------------------------
// Name			: Remove
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CFixedStringListNode * const CFixedStringHashTable::Remove( const CFixedString & rcString )
{
	Assert( m_ppcStringList != NULL );
	Assert( m_nSize != 0 );

	UInt32	nSlot = rcString.GetKey() % m_nSize;

	if( m_ppcStringList[nSlot] == NULL )
	{
		return NULL;
	}

	CFixedStringListNode * const	pcNodeToRemove = m_ppcStringList[nSlot]->Remove( rcString );
	if( pcNodeToRemove != NULL )
	{
		if( m_ppcStringList[nSlot]->IsEmpty() == true )
		{
			m_ppcStringList[nSlot]->GetPrev()->SetNext( m_ppcStringList[nSlot]->GetNext() );
			m_ppcStringList[nSlot]->GetNext()->SetPrev( m_ppcStringList[nSlot]->GetPrev() );

	//		delete m_ppcStringList[nSlot];
	//		m_ppcStringList[nSlot] = NULL;
		}

		--m_nCount;
	}

	return pcNodeToRemove;
}


//-----------------------------------------------------------------------------
// Name			: RemoveAll
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CFixedStringHashTable::RemoveAll( void )
{
	CFixedStringLinkedList *	pcNode = m_pcHead->GetNext();
	CFixedStringLinkedList *	pcTemp;

	while( pcNode != m_pcTail )
	{
		Assert( pcNode != NULL );

		pcTemp = pcNode;

		pcNode = pcNode->GetNext();

		delete pcTemp;
	}

	m_pcHead->SetNext( m_pcTail );
	m_pcTail->SetPrev( m_pcHead );

	ZeroMemory( m_ppcStringList, sizeof( CFixedStringLinkedList * ) * m_nSize );

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

bool CFixedStringHashTable::DoIt( bool (*pfnToDo)( void * pvData ) )
{
	CFixedStringLinkedList *	pcNode = m_pcHead->GetNext();
	CFixedStringLinkedList *	pcTemp;

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

bool CFixedStringHashTable::IsValid( void ) const
{
	UInt32	nCount = 0;

	for( UInt32 nSlot = 0; nSlot < m_nSize; ++nSlot )
	{
		if( m_ppcStringList[nSlot] != NULL )
		{
			if( m_ppcStringList[nSlot]->IsValid() == false )
			{
				return false;
			}

			nCount += m_ppcStringList[nSlot]->GetCount();
		}
	}

	if( m_nCount != nCount )
	{
		return false;
	}

	return true;
}


//*****************************************************************************
//
// End of File : CFixedStringHashTable.cpp
//
//*****************************************************************************