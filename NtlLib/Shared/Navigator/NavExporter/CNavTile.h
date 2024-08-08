//*****************************************************************************
// File       : CNavTile.h
// Desc       : 
// Author     : June C. Kim (cynis@hotmail.com)
// Update     : 2007. 1. 18
//*****************************************************************************

#ifndef ___C_NAV_TILE_H___
#define ___C_NAV_TILE_H___

//-----------------------------------------------------------------------------
// Pre-Compiler Option Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Type Forward Definition Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "NavBase.h"
#include "NavExporterBase.h"


//-----------------------------------------------------------------------------
// Name Space Table
//-----------------------------------------------------------------------------

BEGIN_NAMESPACE( navex )


//-----------------------------------------------------------------------------
// Constant Table
//-----------------------------------------------------------------------------

#define OPTIMIZATION_LEVEL_MAX						( 16 )


//-----------------------------------------------------------------------------
// Type Definition Table
//-----------------------------------------------------------------------------


//=============================================================================
// Name       : CNavTile
// Desc       : 
// Author     : 
//=============================================================================

class CNavTile
{
	protected:

		Fixed32							m_nHeight;
		bool							m_bExported;
		ENavTileDivideType				m_eDivideType;
		SNavSlope						m_sSlope[E_NAV_TILE_FACE_TYPE_COUNT];
		bool							m_bLocked[E_NAV_TILE_FACE_TYPE_COUNT];
		bool							m_bFiltered[E_NAV_TILE_FACE_TYPE_COUNT];

	public:

		inline							CNavTile( void ) : m_bExported( false ), m_nHeight( 0 ), m_eDivideType( E_NAV_TILE_DIVIDE_TYPE_DOWN_LEFT ) { SetLock( false ); SetFilter( true ); }

		UInt32							Create( SNavTileCreateInfo & rsCreateInfo );

		inline ENavTileDivideType		GetDivideType( void ) const { return m_eDivideType; }
		inline void						SetDivideType( ENavTileDivideType eDivideType ) { m_eDivideType = eDivideType; }

		inline const SNavSlope &		GetSlope( ENavTileFaceType eFaceType ) const { return m_sSlope[eFaceType]; }

		inline Fixed32					GetHeight( void ) const { return m_nHeight; }

		inline bool						IsLocked( void ) const { return ( m_bLocked[E_NAV_TILE_FACE_TYPE_LEFT] | m_bLocked[E_NAV_TILE_FACE_TYPE_RIGHT] ); }
		inline void						SetLock( bool bLock ) { m_bLocked[E_NAV_TILE_FACE_TYPE_LEFT] = m_bLocked[E_NAV_TILE_FACE_TYPE_RIGHT] = bLock; }

		inline bool						IsLocked( ENavTileFaceType eFaceType ) const { return m_bLocked[eFaceType]; }
		inline void						SetLock( ENavTileFaceType eFaceType, bool bLock ) { m_bLocked[eFaceType] = bLock; }

		inline bool						IsFiltered( void ) const { return ( m_bFiltered[E_NAV_TILE_FACE_TYPE_LEFT] | m_bFiltered[E_NAV_TILE_FACE_TYPE_RIGHT] ); }
		inline void						SetFilter( bool bFilter ) { m_bFiltered[E_NAV_TILE_FACE_TYPE_LEFT] = m_bFiltered[E_NAV_TILE_FACE_TYPE_RIGHT] = bFilter; }

		inline bool						IsFiltered( ENavTileFaceType eFaceType ) const { return m_bFiltered[eFaceType]; }
		inline void						SetFilter( ENavTileFaceType eFaceType, bool bFilter ) { m_bFiltered[eFaceType] = bFilter; }

		inline void						SetExported( bool bExported = true ) { m_bExported = bExported; }
		inline bool						IsExported( void ) const { return m_bExported; }
};

//=============================================================================
// End of Class : CNavTile
//=============================================================================

END_NAMESPACE( navex )

#endif //___C_NAV_TILE_H___

//*****************************************************************************
//
// End of File : CNavTile.h
//
//*****************************************************************************