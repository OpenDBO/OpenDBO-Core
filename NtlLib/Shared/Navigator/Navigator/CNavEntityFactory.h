//*****************************************************************************
// File       : CNavEntityFactory.h
// Desc       : 
// Author     : June C. Kim (cynis@hotmail.com)
// Update     : 2007. 1. 18
//*****************************************************************************

#ifndef ___C_NAV_ENTITY_FACTORY_H___
#define ___C_NAV_ENTITY_FACTORY_H___

//-----------------------------------------------------------------------------
// Pre-Compiler Option Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "NavBase.h"

#include "NavEntityBase.h"

#include "CNavEntityHashTable.h"


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

#define g_pEntityFactory				( CNavEntityFactory::GetInstance() )

#define ENTITY_REGISTER_TABLE_SIZE_DEFAULT	( 65536 )


//-----------------------------------------------------------------------------
// Type Definition Table
//-----------------------------------------------------------------------------


//=============================================================================
// Name       : CNavEntityFactory
// Desc       : 
// Author     : 
//=============================================================================

class CNavEntityFactory
{
	protected:

		CNavEntityHashTable				m_cRegisteredTable;

	protected:

		inline							CNavEntityFactory( void ) { Initialize(); }

		inline virtual					~CNavEntityFactory( void ) { Destroy(); }

	private:

										CNavEntityFactory( const CNavEntityFactory & rcFactory );

		const CNavEntityFactory &		operator =( const CNavEntityFactory & rcFactory );

	public:

		CNavEntity *					CreateEntity( SNavEntityCreateInfo & rsCreateInfo );
		void							DestroyEntity( CNavEntity * pcEntity );

		const CNavEntity &				GetEntity( const CFixedString & rcEntityName ) const;

	protected:

		virtual void					Initialize( void );
		virtual bool					Create( void );
		virtual void					Destroy( void );

		bool							RegisterEntity( CNavEntity * pcEntity );
		bool							UnregisterEntity( CNavEntity * pcEntity );

	private:

		static CNavEntityFactory *		m_pInstance;

	public:

		static CNavEntityFactory *		CreateInstance( void );
		static void						DestroyInstance( void );
		inline static CNavEntityFactory *	GetInstance( void ) { return m_pInstance; }
};

//=============================================================================
// End of Class : CNavEntityFactory
//=============================================================================

END_NAMESPACE( nav )

#endif //___C_NAV_ENTITY_FACTORY_H___

//*****************************************************************************
//
// End of File : CNavEntityFactory.h
//
//*****************************************************************************