//*****************************************************************************
// File       : CFixedStringLinkedList.h
// Desc       : 
// Author     : June C. Kim (cynis@hotmail.com)
// Update     : 2006. 12. 5
//*****************************************************************************

#ifndef ___C_FIXED_STRING_LINKED_LIST_H___
#define ___C_FIXED_STRING_LINKED_LIST_H___

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


//-----------------------------------------------------------------------------
// Constant Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Type Definition Table
//-----------------------------------------------------------------------------


//=============================================================================
// Name       : CFixedStringLinkedList
// Desc       : 
// Author     : 
//=============================================================================

class CFixedStringLinkedList
{
	protected:

		CFixedStringListNode *			m_pcHead;
		CFixedStringListNode *			m_pcTail;
		UInt32							m_nCount;

		CFixedStringLinkedList *		m_pcPrev;
		CFixedStringLinkedList *		m_pcNext;

	public:

		inline							CFixedStringLinkedList( void ) : m_pcHead( NULL ), m_pcTail( NULL ), m_nCount( 0 ), m_pcPrev( NULL ), m_pcNext( NULL ) { Initialize(); }
		inline							CFixedStringLinkedList( const CFixedString & rcString ) : m_pcHead( NULL ), m_pcTail( NULL ), m_nCount( 0 ), m_pcPrev( NULL ), m_pcNext( NULL ) { Initialize(); Insert( rcString ); }
		inline							CFixedStringLinkedList( CFixedStringListNode * const pcNode ) : m_pcHead( NULL ), m_pcTail( NULL ), m_nCount( 0 ), m_pcPrev( NULL ), m_pcNext( NULL ) { Initialize(); Insert( pcNode ); }

		inline virtual 					~CFixedStringLinkedList( void ) { RemoveAll(); Destroy(); }

	private:

										CFixedStringLinkedList( const CFixedStringLinkedList & rcList );

		const CFixedStringLinkedList &	operator=( const CFixedStringLinkedList & rcList );

	public:

		virtual void					Insert( const CFixedString & rcString );
		virtual void					Insert( CFixedStringListNode * const pcNode );

		CFixedStringListNode * const	Remove( const CFixedString & rcString );

		void							RemoveAll( void );

		CFixedStringListNode * const	GetNode( const CFixedString & rcString ) const;

		inline UInt32					GetCount( void ) const { return m_nCount; }
		inline bool						IsEmpty( void ) const { return ( GetCount() == 0 ); }

		inline void						SetPrev( CFixedStringLinkedList * const pcPrev ) { m_pcPrev = pcPrev; }
		inline void						SetNext( CFixedStringLinkedList * const pcNext ) { m_pcNext = pcNext; }

		inline CFixedStringLinkedList * const	GetPrev( void ) { return m_pcPrev; }
		inline CFixedStringLinkedList * const	GetNext( void ) { return m_pcNext; }

		bool							DoIt( bool (*pfnToDo)( void * pvData ) );

		bool							IsValid( void ) const;

	protected:

		virtual void					Initialize( void );
		virtual void					Destroy( void );
};

//=============================================================================
// End of Class : CFixedStringLinkedList
//=============================================================================

#endif //___C_FIXED_STRING_LINKED_LIST_H___

//*****************************************************************************
//
// End of File : CFixedStringLinkedList.h
//
//*****************************************************************************