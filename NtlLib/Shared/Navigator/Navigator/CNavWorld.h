//*****************************************************************************
// File       : CNavWorld.h
// Desc       : 
// Author     : June C. Kim (cynis@hotmail.com)
// Update     : 2007. 1. 18
//*****************************************************************************

#ifndef ___C_NAV_WORLD_H___
#define ___C_NAV_WORLD_H___

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

class CNavEntityVector;
class CNavZone;
class CNavSector;


//-----------------------------------------------------------------------------
// Constant Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Type Definition Table
//-----------------------------------------------------------------------------


//=============================================================================
// Name       : CNavWorld
// Desc       : 
// Author     : 
//=============================================================================

class CNavWorld : public CNavEntity
{
	friend class						CNavEntityFactory;

	protected:

		CNavEntityVector *				m_pcZones;
		UInt32							m_nActiveZoneCount;

		CNavEntityVector *				m_pcSectors;
		SNavIndex2D						m_sSectorCount;
		UInt32							m_nActiveSectorCount;

		CNavPosition					m_cSectorSize;
		CNavPosition					m_cOverlapSize;

		ENavWorldType					m_eWorldType;

	protected:

		inline							CNavWorld( UId32 nUniqueId ) : CNavEntity( nUniqueId ) { Initialize(); m_eType = E_NAV_ENTITY_TYPE_WORLD; }

		inline							~CNavWorld( void ) { Destroy(); }

	private:

										CNavWorld( void );
										CNavWorld( const CNavWorld & rcWorld );

		const CNavWorld &				operator =( const CNavWorld & rcWorld );

	public:

		// Common

		bool							Create( SNavEntityCreateInfo & rsCreateInfo );

		bool							InsertChild( CNavEntity * const pcChild );
		bool							RemoveChild( CNavEntity * const pcChild );

		CNavEntity * const				GetChildByUniqueId( UId32 nUniqueId ) const;
		CNavEntity * const				GetChildByPosition( const CNavPosition & rcPosition1 ) const;
		CNavEntity * const				GetChildByPosition( const CNavPosition & rcPosition1, const CNavPosition & rcPosition2 ) const;

		bool							GetHeight( CNavPosition & rcPosition ) const;
		DWORD							GetAttribute( CNavPosition & rcPosition ) const;

		bool							PreProcess( SNavEntityPreProcessInfo & rsPreProcessInfo );
		bool							PostProcess( SNavEntityPreProcessInfo & rsPostProcessInfo );

		Int32							Render( CNavRenderer * const pcRenderer ) const;

		bool							DoIt( bool (*pfnToDo)( CNavEntity * pcThis, void * pvData ), void * pvData = NULL );
		bool							DoItAfter( bool (*pfnToDo)( CNavEntity * pcThis, void * pvData ), void * pvData = NULL );

		bool							GetHierarchy( CNavEntityHierarchy & rcHierarchy, const CNavPosition & rcPosition ) const;

		// Exclusive

		UInt32							GetActiveZoneCount( void ) const { m_nActiveZoneCount; }

		UInt32							GetActiveSectorCount( void ) const { return m_nActiveSectorCount; }

		CNavSector * const				GetSectorByUniqueId( UId32 nUniqueId ) const;
		CNavSector * const				GetSectorByPosition( const CNavPosition & rcPosition1 ) const;
		CNavSector * const				GetSectorByPosition( const CNavPosition & rcPosition1, const CNavPosition & rcPosition2 ) const;

		const SNavIndex2D &				GetSectorCount( void ) const { return m_sSectorCount; }
		const CNavPosition &			GetSectorSize( void ) const { return m_cSectorSize; }
		const CNavPosition &			GetOverlapSize( void ) const { return m_cOverlapSize; }

		bool							FindUnblockedPosition( CNavPosition & rcSearchPosition, Fixed32 nDistance, UId32 nShapeId ) const;

	protected:

		// Common

		void							Initialize( void );
		void							Destroy( void );

		bool							CreateFromFile( SNavEntityCreateInfo & rsCreateInfo );
//		Int32							DestroyChild( void );

		// Exclusive

		bool							LinkZone( CNavZone * const pcZone );
		bool							UnlinkZone( CNavZone * const pcZone );

		CNavZone * const				GetZone( UInt32 nOrder ) const;

		bool							LinkSector( CNavSector * const pcSector );
		bool							UnlinkSector( CNavSector * const pcSector );

		CNavSector * const				GetSector( UInt32 nSectorOrder ) const;
		CNavSector * const				GetSector( const SNavIndex2D & rsSectorOrder ) const;
		CNavSector * const				GetSector( const CNavPosition & rcPosition ) const;

		UInt32							GetSectorIndex( const CNavPosition & rcPosition ) const;

		bool							ParseHeader( CFixedStringTreeNodeW * pcHeader );
		bool							ParseBound( CFixedStringTreeNodeW * pcBound );
		bool							ParseSector( CFixedStringTreeNodeW * pcSector );
		bool							ParseZoneList( SNavEntityCreateInfo & rsCreateInfo, CFixedStringTreeNodeW * pcZoneList );

		bool							DetectLineCollisionWithMultipleZone( const CNavPosition & rcBeginPosition, const CNavPosition & rcEndPosiiton, UId32 nShapeId ) const;
};

//=============================================================================
// End of Class : CNavWorld
//=============================================================================

END_NAMESPACE( nav )

#endif //___C_NAV_WORLD_H___

//*****************************************************************************
//
// End of File : CNavWorld.h
//
//*****************************************************************************