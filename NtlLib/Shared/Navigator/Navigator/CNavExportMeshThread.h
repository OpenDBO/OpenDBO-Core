//*****************************************************************************
// File       : CNavExportMeshThread.h
// Desc       : 
// Author     : June C. Kim (cynis@hotmail.com)
// Update     : 2007. 1. 18
//*****************************************************************************

#ifndef ___C_NAV_EXPORT_MESH_THREAD_H___
#define ___C_NAV_EXPORT_MESH_THREAD_H___

//-----------------------------------------------------------------------------
// Pre-Compiler Option Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "CBaseThread.h"
#include "NavEntityBase.h"


//-----------------------------------------------------------------------------
// Name Space Table
//-----------------------------------------------------------------------------

BEGIN_NAMESPACE( nav )


//-----------------------------------------------------------------------------
// Type Forward Definition Table
//-----------------------------------------------------------------------------

class CNavTerrain;


//-----------------------------------------------------------------------------
// Constant Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Type Definition Table
//-----------------------------------------------------------------------------


//=============================================================================
// Name       : CNavExportMeshThread
// Desc       : 
// Author     : 
//=============================================================================

class CNavExportMeshThread : public CBaseThread
{
	protected:

		SNavEntityCreateInfo 			m_sCreateInfo;

	public:

		inline							CNavExportMeshThread( void ) { Initialize(); }

		inline							~CNavExportMeshThread( void ) { Destroy(); }

	private:

										CNavExportMeshThread( const CNavExportMeshThread & rcThread );

		const CNavExportMeshThread &	operator=( const CNavExportMeshThread & rcThread );

	public:

		// Common

		// Exclusive

		void							SetCreateInfo( const SNavEntityCreateInfo & rsCreateInfo );
		inline const SNavEntityCreateInfo &		GetCreateInfo( void ) const { return m_sCreateInfo; }

	protected:

		void							Initialize( void );
		void							Destroy( void );

		UInt32							Begin( void );
		UInt32							Run( void );
		UInt32							End( void );
};

//=============================================================================
// End of Class : CNavExportMeshThread
//=============================================================================

END_NAMESPACE( nav )

#endif //___C_NAV_EXPORT_MESH_THREAD_H___

//*****************************************************************************
//
// End of File : CNavExportMeshThread.h
//
//*****************************************************************************