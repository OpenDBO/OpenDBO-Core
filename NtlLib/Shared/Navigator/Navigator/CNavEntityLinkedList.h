///*****************************************************************************
// File       : CNavEntityLinkedList.h
// Desc       : 
// Author     : June C. Kim (cynis@hotmail.com)
// Update     : 2007. 1. 18
//*****************************************************************************

#ifndef ___C_NAV_ENTITY_LINKED_LIST_H___
#define ___C_NAV_ENTITY_LINKED_LIST_H___

//-----------------------------------------------------------------------------
// Pre-Compiler Option Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "CFixedStringLinkedList.h"
#include "CNavEntity.h"


//-----------------------------------------------------------------------------
// Name Space Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Type Forward Definition Table
//-----------------------------------------------------------------------------

class CNavEntityNode;


//-----------------------------------------------------------------------------
// Constant Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Type Definition Table
//-----------------------------------------------------------------------------


//=============================================================================
// Name       : CNavEntityLinkedList
// Desc       : 
// Author     : 
//=============================================================================

class CNavEntityLinkedList : public CFixedStringLinkedList
{
	public:

		inline							CNavEntityLinkedList( void ) {}
		inline							CNavEntityLinkedList( const CFixedString & rcString ) : CFixedStringLinkedList( rcString ) {}
		inline							CNavEntityLinkedList( CFixedStringListNode * const pcNode ) : CFixedStringLinkedList( pcNode ) {}

		virtual inline					~CNavEntityLinkedList( void ) {}

	private:

										CNavEntityLinkedList( const CNavEntityLinkedList & rcList );

		const CNavEntityLinkedList &	operator=( const CNavEntityLinkedList & rcList );

	public:

		inline void						Insert( const CFixedString & rcString ) { UNREFERENCED_PARAMETER( rcString ); Assert( !"Need Full Entity Data" ); }
		void							Insert( const CNavEntity & rcEntity );
		void							Insert( CFixedStringListNode * const pcNode );

		CFixedStringListNode * const	Remove( const CNavEntity & rcEntity );
};

//=============================================================================
// End of Class : CNavEntityLinkedList
//=============================================================================

#endif //___C_NAV_ENTITY_LINKED_LIST_H___

//*****************************************************************************
//
// End of File : CNavEntityLinkedList.h
//
//*****************************************************************************