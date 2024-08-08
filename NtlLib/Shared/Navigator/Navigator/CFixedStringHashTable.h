//*****************************************************************************
// File       : CFixedStringHashTable.h
// Desc       : 
// Author     : June C. Kim (cynis@hotmail.com)
// Update     : 2007. 1. 18
//*****************************************************************************

#ifndef ___C_FIXED_STRING_HASH_TABLE_H___
#define ___C_FIXED_STRING_HASH_TABLE_H___

//-----------------------------------------------------------------------------
// Pre-Compiler Option Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "CFixedString.h"


//-----------------------------------------------------------------------------
// Name Space Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Type Forward Definition Table
//-----------------------------------------------------------------------------

class CFixedStringListNode;
class CFixedStringLinkedList;


//-----------------------------------------------------------------------------
// Constant Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Type Definition Table
//-----------------------------------------------------------------------------


//=============================================================================
// Name       : CFixedStringHashTable
// Desc       : 
// Author     : 
//=============================================================================

class CFixedStringHashTable
{
	protected:

		CFixedStringLinkedList **		m_ppcStringList;
		UInt32							m_nCount;
		UInt32							m_nSize;

		CFixedStringLinkedList *		m_pcHead;
		CFixedStringLinkedList *		m_pcTail;

	public:

		inline							CFixedStringHashTable( void ) : m_ppcStringList( NULL ), m_nCount( 0 ), m_nSize( 0 ), m_pcHead( NULL ), m_pcTail( NULL ) { Initialize(); }
		inline							CFixedStringHashTable( UInt32 nSize ) : m_ppcStringList( NULL ), m_nCount( 0 ), m_nSize( 0 ), m_pcHead( NULL ), m_pcTail( NULL ) { Initialize(); Reserve( nSize ); }

		inline virtual 					~CFixedStringHashTable( void ) { RemoveAll(); Destroy(); }

	private:

										CFixedStringHashTable( const CFixedStringHashTable & rcTable );

		const CFixedStringHashTable &	operator=( const CFixedStringHashTable & rcTable );

	public:

		void							Reserve( UInt32 nSize );

		virtual void					Insert( const CFixedString & rcString );
		virtual void					Insert( CFixedStringListNode * const pcNode );

		CFixedStringListNode * const	Remove( const CFixedString & rcString );

		void							RemoveAll( void );

		CFixedStringListNode * const	GetNode( const CFixedString & rcString ) const;

		inline UInt32					GetCount( void ) const { return m_nCount; }
		inline bool						IsEmpty( void ) const { return ( GetCount() == 0 ); }

		inline UInt32					GetSize( void ) const { return m_nSize; }

		bool							DoIt( bool (*pfnToDo)( void * pvData ) );

		bool							IsValid( void ) const;

	protected:

		virtual void					Initialize( void );
		virtual void					Destroy( void );
};

//=============================================================================
// End of Class : CFixedStringHashTable
//=============================================================================

#endif //___C_FIXED_STRING_HASH_TABLE_H___

//*****************************************************************************
//
// End of File : CFixedStringHashTable.h
//
//*****************************************************************************