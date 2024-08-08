//*****************************************************************************
// File       : CNavSection.h
// Desc       : 
// Author     : June C. Kim (cynis@hotmail.com)
// Update     : 2007. 1. 18
//*****************************************************************************

#ifndef ___C_NAV_SECTION_H___
#define ___C_NAV_SECTION_H___

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
// Name       : CNavSection
// Desc       : 
// Author     : 
//=============================================================================

class CNavSection : public CNavEntity
{
	friend class						CNavEntityFactory;

	protected:

		ENavSectorType					m_eSectorType;
		UInt32							m_nFlags;

	protected:

		inline							CNavSection( UId32 nUniqueId ) : CNavEntity( nUniqueId ) { Initialize(); m_eType = E_NAV_ENTITY_TYPE_SECTION; }

		inline							~CNavSection( void ) { Destroy(); }

	private:

										CNavSection( void );
										CNavSection( const CNavSection & rcSection );

		const CNavSection &				operator =( const CNavSection & rcSection );

	public:

		// Common

		inline bool						Create( SNavEntityCreateInfo & rsCreateInfo )
										{
											if( CNavEntity::Create( rsCreateInfo ) == false )
											{
												return false;
											}

											m_nFlags = rsCreateInfo.nFlags;

											return true;
										}

		inline bool						InsertChild( CNavEntity * const pcEntity ) { UNREFERENCED_PARAMETER( pcEntity ); return false; }
		inline bool						RemoveChild( CNavEntity * const pcEntity ) { UNREFERENCED_PARAMETER( pcEntity ); return false; }

		inline Int32					Render( CNavRenderer * const pcRenderer ) const { UNREFERENCED_PARAMETER( pcRenderer ); return 0; }

		inline bool						PreProcess( SNavEntityPreProcessInfo & rsPreProcessInfo ) { UNREFERENCED_PARAMETER( rsPreProcessInfo ); return false; }
		inline bool						PostProcess( SNavEntityPreProcessInfo & rsPostProcessInfo ) { UNREFERENCED_PARAMETER( rsPostProcessInfo ); return false; }

		// Exclusive

		inline ENavSectorType			GetSectorType( void ) const { return m_eSectorType; }
		inline UInt32					GetFlags( void ) const { return m_nFlags; }

	protected:

		// Common

		inline void						Initialize( void ) { m_eSectorType = E_NAV_SECTOR_TYPE_NONE; m_nFlags = 0; }	
		inline void						Destroy( void ) { Initialize(); }

		inline bool						CreateFromFile( SNavEntityCreateInfo & rsCreateInfo ) { UNREFERENCED_PARAMETER( rsCreateInfo ); return false; }
		inline bool						ImportFromFile( SNavEntityCreateInfo & rsCreateInfo ) { UNREFERENCED_PARAMETER( rsCreateInfo ); return false; }
//		bool							DestroyChild( void );

		// Exclusive
};

//=============================================================================
// End of Class : CNavSection
//=============================================================================

END_NAMESPACE( nav )

#endif //___C_NAV_SECTION_H___

//*****************************************************************************
//
// End of File : CNavSection.h
//
//*****************************************************************************