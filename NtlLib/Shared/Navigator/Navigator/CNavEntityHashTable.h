//*****************************************************************************
// File       : CNavEntityHashTable.h
// Desc       : 
// Author     : June C. Kim (cynis@hotmail.com)
// Update     : 2007. 1. 18
//*****************************************************************************

#ifndef ___C_NAV_ENTITY_HASH_TABLE_H___
#define ___C_NAV_ENTITY_HASH_TABLE_H___

//-----------------------------------------------------------------------------
// Pre-Compiler Option Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "CFixedStringHashTable.h"
#include "CNavEntity.h"


//-----------------------------------------------------------------------------
// Name Space Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Type Forward Definition Table
//-----------------------------------------------------------------------------

class CFixedStringNode;


//-----------------------------------------------------------------------------
// Constant Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Type Definition Table
//-----------------------------------------------------------------------------


//=============================================================================
// Name       : CNavEntityHashTable
// Desc       : 
// Author     : 
//=============================================================================

class CNavEntityHashTable : public CFixedStringHashTable
{
	public:

		inline							CNavEntityHashTable( void ) {}
		inline							CNavEntityHashTable( UInt32 nSize ) : CFixedStringHashTable( nSize ) {}

		virtual inline 					~CNavEntityHashTable( void ) {}

	private:

										CNavEntityHashTable( const CNavEntityHashTable & rcTable );

		const CNavEntityHashTable &		operator=( const CNavEntityHashTable & rcTable );

	public:

		void							Insert( const CFixedString & rcString )  { UNREFERENCED_PARAMETER( rcString ); Assert( !"Need Full Entity Data" ); }
		void							Insert( const CNavEntity & rcEntity );
		void							Insert( CFixedStringListNode * const pcNode );

		CFixedStringListNode * const	Remove( const CNavEntity & rcEntity );
};

//=============================================================================
// End of Class : CNavEntityHashTable
//=============================================================================

#endif //___C_NAV_ENTITY_HASH_TABLE_H___

//*****************************************************************************
//
// End of File : CNavEntityHashTable.h
//
//*****************************************************************************