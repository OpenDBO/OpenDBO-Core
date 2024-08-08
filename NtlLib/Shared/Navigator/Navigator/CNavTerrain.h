//*****************************************************************************
// File       : CNavTerrain.h
// Desc       : 
// Author     : June C. Kim (cynis@hotmail.com)
// Update     : 2007. 1. 18
//*****************************************************************************

#ifndef ___C_NAV_TERRAIN_H___
#define ___C_NAV_TERRAIN_H___

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

class CNavObject;
class CNavEntityVector;


//-----------------------------------------------------------------------------
// Constant Table
//-----------------------------------------------------------------------------

#define FACE_ID_INVALID					( 0xFFFFFFFF )


//-----------------------------------------------------------------------------
// Type Definition Table
//-----------------------------------------------------------------------------


//=============================================================================
// Name       : CNavTerrain
// Desc       : 
// Author     : 
//=============================================================================

class CNavTerrain : public CNavEntity
{
	friend class						CNavEntityFactory;

	protected:

		iMesh *							m_pcMesh;
		iCollisionContext *				m_pcContext;
		iObstacleSet *					m_pcObstacleSet;

		UInt32							m_nFaceCount;

		CNavEntityVector *				m_pcObjects;

		CFixedString					m_cExportName;

		EThreadPriority					m_ePriority;

	protected:

		inline							CNavTerrain( UInt32 nUniqueId ) : CNavEntity( nUniqueId ), m_ePriority( E_THREAD_PRIORITY_NORMAL ) { Initialize(); m_eType = E_NAV_ENTITY_TYPE_TERRAIN; }

		inline							~CNavTerrain( void ) { Destroy(); }

	private:

										CNavTerrain( void );
										CNavTerrain( const CNavTerrain & rcWorld );

		const CNavTerrain &				operator =( const CNavTerrain & rcWorld );

	public:

		// Common

		bool							Create( SNavEntityCreateInfo & rsCreateInfo );

		bool							InsertChild( CNavEntity * const pcEntity );
		bool							RemoveChild( CNavEntity * const pcEntity );

		CNavEntity * const				GetChildByUniqueId( UId32 nUniqueId ) const;
		CNavEntity * const				GetChildByPosition( const CNavPosition & rcPosition1 ) const;
		CNavEntity * const				GetChildByPosition( const CNavPosition & rcPosition1, const CNavPosition & rcPosition2 ) const;

		bool							PreProcess( SNavEntityPreProcessInfo & rsPreProcessInfo );
		bool							PostProcess( SNavEntityPreProcessInfo & rsPostProcessInfo );

		Int32							Render( CNavRenderer * const pcRenderer ) const;

		bool							DoIt( bool (*pfnToDo)( CNavEntity * pcThis, void * pvData ), void * pvData = NULL );
		bool							DoItAfter( bool (*pfnToDo)( CNavEntity * pcThis, void * pvData ), void * pvData = NULL );

		bool							IsValid( void ) const;

		// Exclusive

		bool							ConvertPosition( cPosition & rcResultPosition, const CNavPosition & rcSourcePosition ) const;

		inline const iMesh *			GetMesh( void ) const { return m_pcMesh; }
		inline iCollisionContext * const	GetContext( void ) const { return m_pcContext; }

		inline UInt32					GetFaceCount( void ) { return m_nFaceCount; }
		bool							GetVertex( UInt32 nFaceIndex, UInt32 nVertexIndex, CNavPosition & rcPosition ) const;
		UInt32							GetConnection( UInt32 nFaceIndex, UInt32 nEdgeIndex ) const;

		bool							FindUnblockedPosition( CNavPosition & rcSearchPosition, Fixed32 nDistance, UId32 nShapeId ) const;

		bool							ExportToFile( ENavFileType eExportFileType, CFixedString & rcExportFileName );

		bool							CreateFromFile( SNavEntityCreateInfo & rsCreateInfo );
		bool							ImportFromFile( SNavEntityCreateInfo & rsCreateInfo );

		inline void						SetMesh( iMesh * pcMesh ) { m_pcMesh = pcMesh; }

		inline EThreadPriority			GetPriority( void ) const { return m_ePriority; }
		inline void						SetPriority( EThreadPriority ePriority ) { m_ePriority = ePriority; }

	protected:

		// Common

		void							Initialize( void );
		void							Destroy( void );

//		Int32							DestroyChild( void );

		// Exclusive

		bool							LinkObject( CNavObject * const pcObject );
		bool							UnlinkObject( CNavObject * const pcObject );
};

//=============================================================================
// End of Class : CNavTerrain
//=============================================================================

END_NAMESPACE( nav )

#endif //___C_NAV_TERRAIN_H___

//*****************************************************************************
//
// End of File : CNavTerrain.h
//
//*****************************************************************************