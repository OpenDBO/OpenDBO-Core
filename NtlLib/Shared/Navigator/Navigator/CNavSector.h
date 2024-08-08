//*****************************************************************************
// File       : CNavSector.h
// Desc       : 
// Author     : June C. Kim (cynis@hotmail.com)
// Update     : 2007. 1. 18
//*****************************************************************************

#ifndef ___C_NAV_SECTOR_H___
#define ___C_NAV_SECTOR_H___

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

class CNavRenderer;
class CNavEntityVector;

class CNavTerrain;
class CNavHeightMap;
class CNavSection;
class CNavArea;


//-----------------------------------------------------------------------------
// Constant Table
//-----------------------------------------------------------------------------

#define TERRAIN_LIST_SIZE_DEFAULT		( 1 )


//-----------------------------------------------------------------------------
// Type Definition Table
//-----------------------------------------------------------------------------

typedef struct SNavSectorCreateInfo
{
	CFixedString			cPrefix;
	CFixedString			cPostfix;
	CFixedString			cSeparator;
	SNavCellPosition2D		sCellPosition;
	CNavBounding			cWorldBound;
	CNavPosition			cSectorSize;
	UId32					nSectionOrder;
	UId32					nAreaOrder;
} SNavSectorCreateInfo;


//=============================================================================
// Name       : CNavSector
// Desc       : 
// Author     : 
//=============================================================================

class CNavSector : public CNavEntity
{
	friend class						CNavEntityFactory;

	protected:

		CNavTerrain *					m_pcTerrain;
		CNavHeightMap *					m_pcHeightMap;

		CNavEntityVector *				m_pcAgents;

		UInt32							m_nSectionOrder;
		UInt32							m_nAreaOrder;

		SNavCellPosition2D				m_sCellPosition;

	protected:

		inline							CNavSector( UId32 nUniqueId ) : CNavEntity( nUniqueId ) { Initialize(); m_eType = E_NAV_ENTITY_TYPE_SECTOR; }

		inline							~CNavSector( void ) { Destroy(); }

	private:

										CNavSector( void );
										CNavSector( const CNavSector & rcSector );

		const CNavSector &				operator =( const CNavSector & rcSector );

	public:

		// Common

		bool							Create( SNavEntityCreateInfo & rsCreateInfo );
		bool							PostCreate( SNavEntityCreateInfo & rsCreateInfo );

		bool							InsertChild( CNavEntity * const pcEntity );
		bool							RemoveChild( CNavEntity * const pcEntity );

		CNavEntity * const				GetChildByUniqueId( UId32 nUniqueId ) const;
		CNavEntity * const				GetChildByPosition( const CNavPosition & rcPosition1 ) const;
		CNavEntity * const				GetChildByPosition( const CNavPosition & rcPosition1, const CNavPosition & rcPosition2 ) const;

		bool							GetHeight( CNavPosition & rcPosition ) const;
		DWORD							GetAttribute( CNavPosition & rcPosition ) const;

		Int32							Render( CNavRenderer * const pcRenderer ) const;

		inline CNavTerrain * const		GetTerrain( void ) const { return m_pcTerrain; }
		inline CNavHeightMap * const	GetHeightMap( void ) const { return m_pcHeightMap; }

		CNavSection * const				GetSection( void ) const;
		CNavArea * const				GetArea( void ) const;

		bool							DoIt( bool (*pfnToDo)( CNavEntity * pcThis, void * pvData ), void * pvData = NULL );
		bool							DoItAfter( bool (*pfnToDo)( CNavEntity * pcThis, void * pvData ), void * pvData = NULL );

		bool							GetHierarchy( CNavEntityHierarchy & rcHierarchy, const CNavPosition & rcPosition ) const;

		// Exclusive

		inline UInt32					GetCellPosition( void ) { return m_sCellPosition.GetIndex(); }
		inline UInt32					GetCellPosition( SNavCellPosition2D & rsCellPosition ) { rsCellPosition.Set( m_sCellPosition ); return m_sCellPosition.GetIndex(); }

	protected:

		// Common

		void							Initialize( void );
		void							Destroy( void );

		bool							CreateByDirect( SNavEntityCreateInfo & rsCreateInfo );
//		Int32							DestroyChild( void );

		// Exclusive

		bool							LinkHeightMap( CNavHeightMap * const pcHeightMap );
		bool							UnlinkHeightMap( CNavHeightMap * const pcHeightMap );

		bool							LinkTerrain( CNavTerrain * const pcTerrain );
		bool							UnlinkTerrain( CNavTerrain * const pcTerrain );
};

//=============================================================================
// End of Class : CNavSector
//=============================================================================

END_NAMESPACE( nav )

#endif //___C_NAV_SECTOR_H___

//*****************************************************************************
//
// End of File : CNavSector.h
//
//*****************************************************************************