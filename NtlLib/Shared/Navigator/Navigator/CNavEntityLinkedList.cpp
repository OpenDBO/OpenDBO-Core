//*****************************************************************************
// File       : CNavEntityLinkedList.cpp
// Desc       : 
// Author     : June C. Kim (cynis@hotmail.com)
// Update     : 2007. 1. 18
//*****************************************************************************

//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "StdAfx.h"

#include "CNavEntityNode.h"

#include "CNavEntityLinkedList.h"


//-----------------------------------------------------------------------------
// Name Space Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Initialize Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Name			: Insert
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CNavEntityLinkedList::Insert( const CNavEntity & rcEntity )
{
	CFixedStringListNode *		pcNodeToInsert = new CNavEntityNode( rcEntity );
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

void CNavEntityLinkedList::Insert( CFixedStringListNode * const pcNode )
{
	Assert( pcNode != NULL );
	Assert( pcNode->IsValid() == true );

	CFixedStringLinkedList::Insert( up_cast<CFixedStringListNode * const>(pcNode) );

	return;
}


//-----------------------------------------------------------------------------
// Name			: Remove
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CFixedStringListNode * const CNavEntityLinkedList::Remove( const CNavEntity & rcEntity )
{
	return CFixedStringLinkedList::Remove( rcEntity.GetName() );
}


//*****************************************************************************
//
// End of File : CNavEntityLinkedList.cpp
//
//*****************************************************************************