//*****************************************************************************
// File       : CNavEntityHierarchy.h
// Desc       : 
// Author     : June C. Kim (cynis@hotmail.com)
// Update     : 2007. 1. 18
//*****************************************************************************

#ifndef ___C_NAV_ENTITY_HIERARCHY_H___
#define ___C_NAV_ENTITY_HIERARCHY_H___

//-----------------------------------------------------------------------------
// Pre-Compiler Option Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "CNavEntity.h"


//-----------------------------------------------------------------------------
// Name Space Table
//-----------------------------------------------------------------------------

BEGIN_NAMESPACE( nav )


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
// Name       : CNavEntityHierarchy
// Desc       : 
// Author     : 
//=============================================================================

class CNavEntityHierarchy
{
	protected:

		CNavEntity *					m_pcWorld;
		CNavEntity *					m_pcZone;
		CNavEntity *					m_pcSection;
		CNavEntity *					m_pcArea;
		CNavEntity *					m_pcSector;

	public:

		inline							CNavEntityHierarchy( void ) : m_pcWorld( NULL ), m_pcZone( NULL ),
																		m_pcSection( NULL ), m_pcArea( NULL ),
																		m_pcSector( NULL ) {}

		inline							~CNavEntityHierarchy( void ) {}

	public:

		inline CNavEntity *	const		GetWorld( void ) const { return m_pcWorld; }
		inline void						SetWorld( CNavEntity * const pcWorld ) { m_pcWorld = pcWorld; }

		inline CNavEntity *	const		GetZone( void ) const { return m_pcZone; }
		inline void						SetZone( CNavEntity * const pcZone ) { m_pcZone = pcZone; }

		inline CNavEntity *	const		GetSection( void ) const { return m_pcSection; }
		inline void						SetSection( CNavEntity * const pcSection ) { m_pcSection = pcSection; }

		inline CNavEntity *	const		GetArea( void ) const { return m_pcArea; }
		inline void						SetArea( CNavEntity * const pcArea ) { m_pcArea = pcArea; }

		inline CNavEntity *	const		GetSector( void ) const { return m_pcSector; }
		inline void						SetSector( CNavEntity * const pcSector ) { m_pcSector = pcSector; }
};

//=============================================================================
// End of Class : CNavEntityHierarchy
//=============================================================================

END_NAMESPACE( nav )

#endif //___C_NAV_ENTITY_HIERARCHY_H___

//*****************************************************************************
//
// End of File : CNavEntityHierarchy.h
//
//*****************************************************************************