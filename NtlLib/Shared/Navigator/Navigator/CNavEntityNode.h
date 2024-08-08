//*****************************************************************************
// File       : CNavEntityNode.h
// Desc       : 
// Author     : June C. Kim (cynis@hotmail.com)
// Update     : 2007. 1. 18
//*****************************************************************************

#ifndef ___C_NAV_ENTITY_NODE_H___
#define ___C_NAV_ENTITY_NODE_H___

//-----------------------------------------------------------------------------
// Pre-Compiler Option Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "CFixedStringListNode.h"
#include "CNavEntity.h"


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
// Name       : CNavEntityNode
// Desc       : 
// Author     : 
//=============================================================================

class CNavEntityNode : public CFixedStringListNode
{
	protected:

		const CNavEntity &				m_rcEntity;

	public:

		inline							CNavEntityNode( void ) : m_rcEntity( CNavEntity::NULL_ENTITY ) {}
		inline							CNavEntityNode( const CNavEntity & rcEntity, CFixedStringListNode * const pcPrev = NULL, CNavEntityNode * const pcNext = NULL ) : m_rcEntity( rcEntity ), CFixedStringListNode( rcEntity.GetName(), pcPrev, pcNext ) {}

		inline							~CNavEntityNode( void ) {}

	private:

										CNavEntityNode( const CNavEntityNode & rcNode );

		const CNavEntityNode &			operator=( const CNavEntityNode & rcNode );

	public:

		inline const CNavEntity &		GetEntity( void ) const { return m_rcEntity; }

		bool							IsValid( void ) const;
};

//=============================================================================
// End of Class : CNavEntityNode
//=============================================================================

#endif //___C_NAV_ENTITY_NODE_H___

//*****************************************************************************
//
// End of File : CNavEntityNode.h
//
//*****************************************************************************