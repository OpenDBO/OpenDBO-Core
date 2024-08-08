//*****************************************************************************
// File       : CNavHeightMap.h
// Desc       : 
// Author     : June C. Kim (cynis@hotmail.com)
// Update     : 2006. 12. 5
//*****************************************************************************

#ifndef ___C_NAV_HEIGHT_MAP_H___
#define ___C_NAV_HEIGHT_MAP_H___

//-----------------------------------------------------------------------------
// Pre-Compiler Option Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Type Forward Definition Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "CNavEntity.h"
#include "CNavPosition2D.h"


//-----------------------------------------------------------------------------
// Name Space Table
//-----------------------------------------------------------------------------

BEGIN_NAMESPACE( nav )


//-----------------------------------------------------------------------------
// Constant Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Type Definition Table
//-----------------------------------------------------------------------------


//=============================================================================
// Name       : CNavHeightMap
// Desc       : 
// Author     : 
//=============================================================================

class CNavHeightMap : public CNavEntity
{
	friend class						CNavEntityFactory;

	protected:

		CNavPosition					m_cPrecision;

		Fixed32	*						m_pnHeights;

		CNavPosition2D *				m_pSlopes;

		SNavIndex2D						m_sHeightCount;
		UInt32							m_nHeightCount;

		UInt32 *						m_pnAttributes;

		SNavIndex2D						m_sAttributeCount;
		UInt32							m_nAttributeCount;

	protected:

		inline							CNavHeightMap( UId32 nUniqueId ) : CNavEntity( nUniqueId ), m_pSlopes( NULL ) { Initialize(); m_eType = E_NAV_ENTITY_TYPE_HEIGHT_MAP; }

		inline							~CNavHeightMap( void ) { Destroy(); }

	private:

										CNavHeightMap( void );
										CNavHeightMap( const CNavHeightMap & rcHeightMap );

		const CNavHeightMap &			operator =( const CNavHeightMap & rcHeightMap );

	public:

		// Common

		bool							Create( SNavEntityCreateInfo & rsCreateInfo );
		bool							PostCreate( SNavEntityCreateInfo & rsCreateInfo );

//		Int32							InsertChild( CNavEntity * const pcEntity );
//		Int32							RemoveChild( CNavEntity * const pcEntity );

//		CNavEntity * const				GetChildByPosition( const CNavPosition & rcPosition1 ) const;
//		CNavEntity * const				GetChildByPosition( const CNavPosition & rcPosition1, const CNavPosition & rcPosition2 ) const;

		Int32							Render( CNavRenderer * const pcRenderer ) const;

		// Exclusive

		bool							Reserve( const CNavBounding & rcBounding, const CNavPosition & rcPrecision );

		inline const CNavPosition &		GetPrecision( void ) const { return m_cPrecision; }

		inline const SNavIndex2D &		GetHeightCount( void ) const { return m_sHeightCount; }

		bool							GetHeight( CNavPosition & rcPosition ) const;
		UInt32							GetAttribute( CNavPosition & rcPosition ) const;

		bool							SetHeight( const CNavPosition & rcPosition );
		bool							SetAttribute( const CNavPosition & rcPosition, UInt32 nAttribute );

		bool							ExportToFile( const CFixedString & rcFullFileName, UInt32 nFlags = 0 ) const;

		bool							IsValid( void ) const;

	protected:

		// Common

		void							Initialize( void );
		void							Destroy( void );

		// Exclusive

		bool							CreateFromFile( SNavEntityCreateInfo & rsCreateInfo );
		bool							CreateEmpty( SNavEntityCreateInfo & rsCreateInfo );
		bool							ImportFromFile( SNavEntityCreateInfo & rsCreateInfo );

		bool							CreateBuffer( void );
		void							DestroyBuffer( void );

	protected:

		static CFixedString				m_cPathName;

	public:

		inline static void				SetPathName( CFixedString & rcPathName ) { m_cPathName.Assign( rcPathName ); }
		inline static const CFixedString &	GetPathName( void ) { return m_cPathName; }
};

//=============================================================================
// End of Class : CNavHeightMap
//=============================================================================

END_NAMESPACE( nav )

#endif //___C_NAV_HEIGHT_MAP_H___

//*****************************************************************************
//
// End of File : CNavHeightMap.h
//
//*****************************************************************************