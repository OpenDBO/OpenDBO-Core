//*****************************************************************************
// File       : CFixedStringListNode.h
// Desc       : 
// Author     : June C. Kim (cynis@hotmail.com)
// Update     : 2007. 1. 18
//*****************************************************************************

#ifndef ___C_FIXED_STRING_LIST_NODE_H___
#define ___C_FIXED_STRING_LIST_NODE_H___

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


//-----------------------------------------------------------------------------
// Constant Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Type Definition Table
//-----------------------------------------------------------------------------


//=============================================================================
// Name       : CFixedStringListNode
// Desc       : 
// Author     : 
//=============================================================================

class CFixedStringListNode
{
	protected:

		CFixedString					m_cString;
		CFixedStringListNode *			m_pcPrev;
		CFixedStringListNode *			m_pcNext;

	public:

		inline							CFixedStringListNode( void ) : m_pcPrev( NULL ), m_pcNext( NULL ) {}
		inline							CFixedStringListNode( const CFixedString & rcString, CFixedStringListNode * const pcPrev = NULL, CFixedStringListNode * const pcNext = NULL ) : m_cString( rcString ), m_pcPrev( pcPrev ), m_pcNext( pcNext ) {}

		inline virtual 					~CFixedStringListNode( void ) {}

	private:

										CFixedStringListNode( const CFixedStringListNode & rcNode );

		const CFixedStringListNode &	operator=( const CFixedStringListNode & rcNode );

	public:

		inline void						SetString( const CFixedString & rcString ) { m_cString.Assign( rcString ); }
		inline void						SetPrev( CFixedStringListNode * const pcPrev ) { m_pcPrev = pcPrev; }
		inline void						SetNext( CFixedStringListNode * const pcNext ) { m_pcNext = pcNext; }

		inline const CFixedString &		GetString( void ) const { return m_cString; }
		inline CFixedStringListNode * const	GetPrev( void ) const { return m_pcPrev; }
		inline CFixedStringListNode * const	GetNext( void ) const { return m_pcNext; }

		bool							DoIt( bool (*pfnToDo)( void * pvData ) );

		bool							IsValid( void ) const;
};

//=============================================================================
// End of Class : CFixedStringListNode
//=============================================================================

#endif //___C_FIXED_STRING_LIST_NODE_H___

//*****************************************************************************
//
// End of File : CFixedStringListNode.h
//
//*****************************************************************************