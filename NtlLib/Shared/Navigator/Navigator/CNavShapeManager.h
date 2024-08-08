//*****************************************************************************
// File       : CNavShapeManager.h
// Desc       : 
// Author     : June C. Kim (cynis@hotmail.com)
// Update     : 2007. 1. 18
//*****************************************************************************

#ifndef ___C_NAV_SHAPE_MANAGER_H___
#define ___C_NAV_SHAPE_MANAGER_H___

//-----------------------------------------------------------------------------
// Pre-Compiler Option Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "NavEntityBase.h"


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

#define NAV_SHAPE_COUNT_MAX				( 1024 )


//-----------------------------------------------------------------------------
// Type Definition Table
//-----------------------------------------------------------------------------


//=============================================================================
// Name       : CNavShapeManager
// Desc       : 
// Author     : 
//=============================================================================

class CNavShapeManager
{
	protected:

		iShape *						m_apcShapeList[NAV_SHAPE_COUNT_MAX];
		UInt32							m_nShapeCount;

	public:

		inline							CNavShapeManager( void ) { Initialize(); }

		inline virtual					~CNavShapeManager( void ) { Destroy(); }

	private:

										CNavShapeManager( const CNavShapeManager & rcNavigator );

		const CNavShapeManager &		operator =( const CNavShapeManager & rcNavigator );

	public:

		UId32							Register( const CFixedString & rcShapeName, UInt32 nFlags = 0 );
		void							Unregister( UId32 nShapeId, UInt32 nFlags = 0 );
		iShape * const					GetByShapeId( UId32 nShapeId ) const;
		UId32							SearchByShape( const iShape * const pcShape ) const;

		inline UInt32					GetCount( void ) const { return m_nShapeCount; }

	protected:

		// Common

		void							Initialize( void );
		void							Destroy( void );

//		bool							ParseXML( const CFixedString & rcXMLName );

	public:

		static const SVersionInfo		VERSION;

	protected:

		static CFixedString				m_cPathName;

	public:

		inline static void				SetPathName( CFixedString & rcPathName ) { m_cPathName.Assign( rcPathName ); }
		inline static CFixedString &	GetPathName( void ) { return m_cPathName; }
};

//=============================================================================
// End of Class : CNavShapeManager
//=============================================================================

END_NAMESPACE( nav )

#endif //___C_NAV_SHAPE_MANAGER_H___

//*****************************************************************************
//
// End of File : CNavShapeManager.h
//
//*****************************************************************************