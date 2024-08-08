//*****************************************************************************
// File       : CFixedStringTreeNodeW.h
// Desc       : 
// Author     : June C. Kim (cynis@hotmail.com)
// Update     : 2007. 2. 22
//*****************************************************************************

#ifndef ___C_FIXED_STRING_TREE_NODE_W_H___
#define ___C_FIXED_STRING_TREE_NODE_W_H___

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

class CFixedStringTreeW;


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

class CFixedStringTreeNodeW
{
	protected:

		CFixedStringW					m_cString;
		UInt32							m_nId;

		CFixedStringTreeNodeW *			m_pcPrev;
		CFixedStringTreeNodeW *			m_pcNext;

		CFixedStringTreeNodeW *			m_pcParent;
		CFixedStringTreeW *				m_pcChild;

	public:

		inline							CFixedStringTreeNodeW( CFixedStringTreeNodeW * pcParent ) : m_nId( 0 ), m_pcParent( pcParent) { Initialize(); }
		inline							CFixedStringTreeNodeW( const CFixedStringW & rcString, CFixedStringTreeNodeW * pcParent ) : m_cString( rcString ), m_nId( 0 ), m_pcParent( pcParent)  { Initialize(); }

		inline							~CFixedStringTreeNodeW( void ) { Destroy(); }

	private:

										CFixedStringTreeNodeW( const CFixedStringTreeNodeW & rcNode );

		const CFixedStringTreeNodeW &	operator =( const CFixedStringTreeNodeW & rcNode );

	public:

		inline void						SetString( const CFixedStringW & rcString ) { m_cString.Assign( rcString ); }
		inline const CFixedStringW &	GetString( void ) const { return m_cString; }

		inline void						SetId( UInt32 nId ) { m_nId = nId; }
		inline UInt32					GetId( void ) const { return m_nId; }

		inline void						SetPrev( CFixedStringTreeNodeW * pcPrev ) { m_pcPrev = pcPrev; }
		inline CFixedStringTreeNodeW *	GetPrev( void ) const { return m_pcPrev; }

		inline void						SetNext( CFixedStringTreeNodeW * pcNext ) { m_pcNext = pcNext; }
		inline CFixedStringTreeNodeW *	GetNext( void ) const { return m_pcNext; }

		inline void						SetParent( CFixedStringTreeNodeW * pcParent ) { m_pcParent = pcParent; }
		inline CFixedStringTreeNodeW *	GetParent( void ) const { return m_pcParent; }

		inline void						SetChildList( CFixedStringTreeW * pcChild ) { m_pcChild = pcChild; }
		inline CFixedStringTreeW *		GetChildList( void ) const { return m_pcChild; }

		UInt32							GetChildCount( void ) const;

		CFixedStringTreeNodeW *			FindChild( UInt32 nOrder = 0 ) const;
		CFixedStringTreeNodeW *			FindChild( const CFixedStringW rcKeyword ) const;
		CFixedStringTreeNodeW *			FindChild( const CFixedStringW rcKeyword, UInt32 nId ) const;

		void							PushFrontChild( CFixedStringTreeNodeW * pcFront );
		void							PushBackChild( CFixedStringTreeNodeW * pcBack );

		CFixedStringTreeNodeW *			PopFrontChild( void );
		CFixedStringTreeNodeW *			PopBackChild( void );

	protected:

		void							Initialize( void );
		void							Destroy( bool bDestroyNode = true );
};

//=============================================================================
// End of Class : CFixedStringTreeNodeW
//=============================================================================

#endif //___C_FIXED_STRING_TREE_NODE_W_H___

//*****************************************************************************
//
// End of File : CFixedStringTreeNodeW.h
//
//*****************************************************************************