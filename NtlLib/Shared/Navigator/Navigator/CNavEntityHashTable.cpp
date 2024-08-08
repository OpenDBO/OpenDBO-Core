//*****************************************************************************
// File       : CNavEntityHashTable.cpp
// Desc       : 
// Author     : June C. Kim (cynis@hotmail.com)
// Update     : 2007. 1. 18
//*****************************************************************************

//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "StdAfx.h"

#include "CNavEntityNode.h"

#include "CNavEntityHashTable.h"


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

void CNavEntityHashTable::Insert( const CNavEntity & rcEntity )
{
	CFixedStringListNode *		pcNodeToInsert = new CNavEntityNode( rcEntity );
	Assert( pcNodeToInsert != NULL );

	CFixedStringHashTable::Insert( pcNodeToInsert );

	return;
}


//-----------------------------------------------------------------------------
// Name			: Insert
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CNavEntityHashTable::Insert( CFixedStringListNode * const pcNode )
{
	Assert( pcNode != NULL );
	Assert( pcNode->IsValid() == true );

	CFixedStringHashTable::Insert( pcNode );

	return;
}


//-----------------------------------------------------------------------------
// Name			: Remove
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CFixedStringListNode * const CNavEntityHashTable::Remove( const CNavEntity & rcEntity )
{
	return CFixedStringHashTable::Remove( rcEntity.GetName() );
}


//*****************************************************************************
//
// End of File : CNavEntityHashTable.cpp
//
//*****************************************************************************