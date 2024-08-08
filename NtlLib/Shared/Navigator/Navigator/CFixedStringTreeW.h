//*****************************************************************************
// File       : CFixedStringTreeW.h
// Desc       : 
// Author     : June C. Kim (cynis@hotmail.com)
// Update     : 2007. 2. 22
//*****************************************************************************

#ifndef ___C_FIXED_STRING_TREE_W_H___
#define ___C_FIXED_STRING_TREE_W_H___

//-----------------------------------------------------------------------------
// Pre-Compiler Option Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "CFixedStringW.h"

#include "CFixedStringTreeNodeW.h"


//-----------------------------------------------------------------------------
// Name Space Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Type Forward Definition Table
//-----------------------------------------------------------------------------

class CFixedStringTreeNodeW;


//-----------------------------------------------------------------------------
// Constant Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Type Definition Table
//-----------------------------------------------------------------------------


//=============================================================================
// Name       : CFixedStringTreeNodeW
// Desc       : 
// Author     : 
//=============================================================================

class CFixedStringTreeW
{
	protected:

		UInt32							m_nCount;

		CFixedStringTreeNodeW *			m_pcHead;
		CFixedStringTreeNodeW *			m_pcTail;

	public:

		inline							CFixedStringTreeW( void ) { Initialize(); }
		inline							CFixedStringTreeW( CFixedStringTreeNodeW * pcNode ) { Initialize(); PushBack( pcNode ); }

		inline							~CFixedStringTreeW( void ) { Destroy(); }

	private:

										CFixedStringTreeW( const CFixedStringTreeW & rcList );

		const CFixedStringTreeW &		operator =( const CFixedStringTreeW & rcList );

	public:

		inline UInt32					GetCount( void ) const { return m_nCount; }
		inline bool						IsEmpty( void ) const { return ( m_nCount == 0 ); }

		CFixedStringTreeNodeW *			GetFirst( void ) const { return m_pcHead->GetNext(); }
		CFixedStringTreeNodeW *			GetLast( void ) const { return m_pcTail->GetPrev(); }

		CFixedStringTreeNodeW *			GetAt( UInt32 nPosition ) const;

		void							PushFront( CFixedStringTreeNodeW * pcFront );
		void							PushBack( CFixedStringTreeNodeW * pcBack );

		CFixedStringTreeNodeW *					PopFront( void );
		CFixedStringTreeNodeW *					PopBack( void );

		void							RemoveAll( void );

		CFixedStringTreeNodeW *					Find( const CFixedStringW rcKeyword ) const;
		CFixedStringTreeNodeW *					Find( const CFixedStringW rcKeyword, UInt32 nId ) const;

	protected:

		void							Initialize( void );
		void							Destroy( void );

		inline void						SetCount( UInt32 nCount ) { m_nCount = nCount; }
};

//=============================================================================
// End of Class : CFixedStringTreeW
//=============================================================================

#endif //___C_FIXED_STRING_TREE_W_H___

//*****************************************************************************
//
// End of File : CFixedStringTreeW.h
//
//*****************************************************************************