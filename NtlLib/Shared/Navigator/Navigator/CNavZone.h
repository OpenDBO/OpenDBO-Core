//*****************************************************************************
// File       : CNavZone.h
// Desc       : 
// Author     : June C. Kim (cynis@hotmail.com)
// Update     : 2007. 1. 18
//*****************************************************************************

#ifndef ___C_NAV_ZONE_H___
#define ___C_NAV_ZONE_H___

//-----------------------------------------------------------------------------
// Pre-Compiler Option Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "CNavEntity.h"


//-----------------------------------------------------------------------------
// Type Forward Definition Table
//-----------------------------------------------------------------------------

class CFixedStringTreeNodeW;


//-----------------------------------------------------------------------------
// Name Space Table
//-----------------------------------------------------------------------------

BEGIN_NAMESPACE( nav )


//-----------------------------------------------------------------------------
// Type Forward Definition Table
//-----------------------------------------------------------------------------

class CNavSection;
class CNavArea;
class CNavPortal;
class CNavEntityVector;
class CNavSector;
class CNavTerrain;


//-----------------------------------------------------------------------------
// Constant Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Type Definition Table
//-----------------------------------------------------------------------------


//=============================================================================
// Name       : CNavZone
// Desc       : 
// Author     : 
//=============================================================================

class CNavZone : public CNavEntity
{
	friend class						CNavEntityFactory;

	protected:

		CNavEntityVector *				m_pcSectors;
		CNavTerrain *					m_pcTerrain;

		SNavIndex2D						m_sSectorCount;
		UInt32							m_nSectorCount;

		ENavZoneType					m_eZoneType;

		CNavEntityVector				m_cSections;
		CNavEntityVector				m_cAreas;
		CNavEntityVector				m_cPortals;

		Float32							m_fComplexity;

	protected:

		inline							CNavZone( UId32 nUniqueId ) : CNavEntity( nUniqueId ), m_nSectorCount( 0 ), m_fComplexity( 1.0f ) { Initialize(); m_eType = E_NAV_ENTITY_TYPE_ZONE; }

		inline							~CNavZone( void ) { Destroy(); }

	private:

										CNavZone( void );
										CNavZone( const CNavZone & rcZone );

		const CNavZone &				operator =( const CNavZone & rcZone );

	public:

		// Common

		bool							Create( SNavEntityCreateInfo & rsCreateInfo );

		bool							InsertChild( CNavEntity * const pcEntity );
		bool							RemoveChild( CNavEntity * const pcEntity );

//		CNavEntity * const				GetChildByUniqueId( UId32 nChildId ) const;
//		CNavEntity * const				GetChildByPosition( const CNavPosition & rcPosition1 ) const;
//		CNavEntity * const				GetChildByPosition( const CNavPosition & rcPosition1, const CNavPosition & rcPosition2 ) const;

		bool							GetHeight( CNavPosition & rcPosition ) const;

		Int32							Render( CNavRenderer * const pcRenderer ) const;

		bool							PreProcess( SNavEntityPreProcessInfo & rsPreProcessInfo );
		bool							PostProcess( SNavEntityPreProcessInfo & rsPostProcessInfo );

		bool							DoIt( bool (*pfnToDo)( CNavEntity * pcThis, void * pvData ), void * pvData = NULL );
		bool							DoItAfter( bool (*pfnToDo)( CNavEntity * pcThis, void * pvData ), void * pvData = NULL );

		// Exclusive

		inline CNavTerrain	* const		GetTerrain( void ) const { return m_pcTerrain; }

		inline UInt32					GetSectorCount( void ) const { return m_nSectorCount; }
		inline const CNavEntityVector * const	GetSectors( void ) const { return m_pcSectors; }

		bool							FindUnblockedPosition( CNavPosition & rcSearchPosition, Fixed32 nDistance, UId32 nShapeId ) const;

		CNavSector * const				GetSector( UInt32 nSectorOrder ) const;
		CNavSector * const				GetSector( UInt32 nSectorOrderX, UInt32 nSectorOrderY ) const;

		CNavSection * const				GetSection( UInt32 nSectionOrder ) const;
		CNavArea * const				GetArea( UInt32 nAreaOrder ) const;
		CNavPortal * const				GetPortal( UInt32 nPortalOrder ) const;

		inline Float32					GetComplexity( void ) const { return m_fComplexity; }
		inline void						SetComplexity( Float32 fComplexity ) { m_fComplexity = fComplexity; }

	protected:

		// Common

		void							Initialize( void );
		void							Destroy( void );

		bool							CreateFromFile( SNavEntityCreateInfo & rsCreateInfo );
		bool							ImportFromFile( SNavEntityCreateInfo & rsCreateInfo );
//		bool							DestroyChild( void );

		// Exclusive

		bool							LinkSector( CNavSector * const pcSector );
		bool							UnlinkSector( CNavSector * const pcSector );

		bool							LinkTerrain( CNavTerrain * const pcTerrain );
		bool							UnlinkTerrain( CNavTerrain * const pcTerrain = NULL );

		bool							LinkSection( CNavSection * const pcSection );
		bool							UnlinkSection( CNavSection * const pcSection );

		bool							LinkArea( CNavArea * const pcArea );
		bool							UnlinkArea( CNavArea * const pcArea );

		bool							LinkPortal( CNavPortal * const pcPortal );
		bool							UnlinkPortal( CNavPortal * const pcPortal );

		bool							ParseHeader( CFixedStringTreeNodeW * pcHeader );
		bool							ParseBound( CFixedStringTreeNodeW * pcBound );
		bool							ParseTerrain( SNavEntityCreateInfo & rsCreateInfo, CFixedStringTreeNodeW * pcTerrain );
		bool							ParseSectionList( SNavEntityCreateInfo & rsCreateInfo, CFixedStringTreeNodeW * pcSectionList );
		bool							ParseAreaList( SNavEntityCreateInfo & rsCreateInfo, CFixedStringTreeNodeW * pcAreaList );
		bool							ParsePortalList( SNavEntityCreateInfo & rsCreateInfo, CFixedStringTreeNodeW * pcPortalList );
		bool							ParseSectorList( SNavEntityCreateInfo & rsCreateInfo, CFixedStringTreeNodeW * pcSectorList );

	protected:

		static CFixedString				m_cPathName;

	public:

		inline static void				SetPathName( CFixedString & rcPathName ) { m_cPathName.Assign( rcPathName ); }
		inline static CFixedString &	GetPathName( void ) { return m_cPathName; }
};

//=============================================================================
// End of Class : CNavZone
//=============================================================================

END_NAMESPACE( nav )

#endif //___C_NAV_ZONE_H___

//*****************************************************************************
//
// End of File : CNavZone.h
//
//*****************************************************************************