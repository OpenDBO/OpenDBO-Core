//*****************************************************************************
// File       : CNavPath.h
// Desc       : 
// Author     : 
// Update     : 
//*****************************************************************************

#ifndef ___C_NAV_PATH_H___
#define ___C_NAV_PATH_H___

//-----------------------------------------------------------------------------
// Pre-Compiler Option Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "NavBase.h"

#include "CNavPosition.h"
#include "CNavPositionList.h"
#include "CNavBounding.h"


//-----------------------------------------------------------------------------
// Name Space Table
//-----------------------------------------------------------------------------

BEGIN_NAMESPACE( nav )


//-----------------------------------------------------------------------------
// Type Forward Definition Table
//-----------------------------------------------------------------------------

class CNavRenderer;
class CNavWorld;
class CNavTerrain;


//-----------------------------------------------------------------------------
// Constant Table
//-----------------------------------------------------------------------------

#define WORLD_ID_DEFAULT				( 0 )		// or 0xFFFFFFFF? (For Marking Invalid Value)
#define SHAPE_ID_DEFAULT				( 0 )

#define INTERNAL_CELL_INDEX_RESET		( -1000 )
#define INTERNAL_CELL_INDEX_NOT_EXIST	( -1 )


//-----------------------------------------------------------------------------
// Type Definition Table
//-----------------------------------------------------------------------------

typedef struct SNavPathInternalParameter
{
	const iMesh *		pcMesh;
	iShape *			pcShape;
	iCollisionContext *	pcPeContext;

	long				nSourceCell;
	long				nDestinationCell;

	cPosition			cSourcePosition;
	cPosition			cDestinationPosition;

	Float32				fComplexity;

	inline				SNavPathInternalParameter( void ) { Reset(); }
	inline				SNavPathInternalParameter( const SNavPathInternalParameter & rsParameter ) { Set( rsParameter ); }

	inline void			Reset( void ) { pcMesh = NULL; pcShape = NULL; nSourceCell = INTERNAL_CELL_INDEX_RESET; nDestinationCell = INTERNAL_CELL_INDEX_RESET; fComplexity = 1.0f; }
	inline void			Set( const SNavPathInternalParameter & rsParameter ) { memcpy( this, &rsParameter, sizeof( SNavPathInternalParameter ) ); }
} SNavPathInternalParameter;


//=============================================================================
// Name       : CNavPath
// Desc       : 
// Author     : 
//=============================================================================

class CNavPath
{
	private:

		ENavPathState					m_eState;
		ENavPathError					m_eError;
		bool							m_bResult;

		iQueryCallBack *				m_pcQueryCallback;

	protected:

		UInt32							m_nFlags;
		UInt32							m_nShapeId;

		CNavPosition					m_cSourcePosition;
		CNavPosition					m_cDestinationPosition;

		SNavPathInternalParameter		m_sInternalParameter;

		CNavPositionList				m_cWayPointList;
		UInt32							m_nMinimalWayPointCount;

	public:

										CNavPath( void );
										CNavPath( const CNavPosition & rcSourcePoint, const CNavPosition & rcDestinationPoint, UInt32 nShapeID );
										CNavPath( const CNavPath & rcPath );

										~CNavPath( void );

	private:

		const CNavPath &				operator =( const CNavPath & rcPath ) { Set( rcPath ); }

	public:

		CNavPath *						Clone( void ) const;

		void							Set( const CNavPath & rcPath );
		void							Set( const CNavPosition & rcSourcePoint, const CNavPosition & rcDestinationPoint, UId32 nShapeId = 0 );

		void							SetSourcePosition( const CNavPosition & rcSourcePosition );
		void							SetDestinationPosition( const CNavPosition & rcDestinationPosition );
		void							SetShape( UId32 nShapeId );

		inline void						Get( CNavPath & rcPath ) const { rcPath.Set( *this ); }

		inline UInt32					GetShapeId( void ) const { return m_nShapeId; }
		inline const CNavPosition &		GetSourcePosition( void ) const { return m_cSourcePosition; }
		inline const CNavPosition &		GetDestinationPosition( void )  const { return m_cDestinationPosition; }
		Fixed32							GetLength( UInt32 nPositionIndex = 0xFFFFFFFF ) const;

		inline const SNavPathInternalParameter &	GetInternalParameter( void ) const { return m_sInternalParameter; }

		inline const CNavPositionList &	GetWayPointList( void ) const { return m_cWayPointList; }
		inline UInt32					GetWayPointCount( void ) const { return m_cWayPointList.GetCount(); }
		inline const CNavPosition *		GetWayPoint( UId32 nPositionId ) const { return m_cWayPointList.GetPosition( nPositionId ); }
		inline UInt32					GetMinimalWayPointCount( void ) const { return m_nMinimalWayPointCount; }

		bool							GetPositionByMoveLength( Float32 fMoveLength, CNavPosition & rcResultPosition ) const;

		inline ENavPathState			GetState( void ) const { return m_eState; }
		inline ENavPathError			GetError( void ) const { return m_eError; }
		inline UInt32					GetFlags( void ) const { return m_nFlags; }

		inline bool						HasPath( void ) const { return ( ( m_eState == E_NAV_PATH_STATE_VALID_PATH ) ? true : false ); }
		inline bool						HasDirectPath( void ) const { return ( ( HasPath() == true && m_nMinimalWayPointCount == 2 ) ? true : false ); }

		bool							FindPath( UInt32 nFlags = 0 );
		bool							FindPath( const CNavBounding & rcSearchBound, UInt32 nFlags = 0 );

		bool							FindPathFast( UInt32 nFlags = 0 );
		bool							FindPathFast( const CNavBounding & rcSearchBound, UInt32 nFlags = 0 );

		bool							FindDirectPath( UInt32 nFlags = 0 );

		bool							Render( CNavRenderer * pcRenderer );

	protected:

		void							Initialize( void );
		void							Destroy( void );

		inline void						SetState( ENavPathState eState ) { m_eState = eState; }
		inline void						SetError( ENavPathError eError ) { m_eError = eError; }

		inline bool						GetResult( void ) const { return m_bResult; }
		inline void						SetResult( bool bResult ) { m_bResult = bResult; }

		UInt32							CalculatePathFindThreshold( void ) const;
		bool							DetectStateForPath( UInt32 nFlags );
		bool							FindMeshForPath( void );
		bool							CreatePathMap( const iPath * pcPePath, UInt32 nFlags = 0 );
		bool							CreateDirectPathMap( UInt32 nFlags = 0 );
};

//=============================================================================
// End of Class : CNavPath
//=============================================================================

END_NAMESPACE( nav )

#endif //___C_NAV_PATH_H___

//*****************************************************************************
//
// End of File : CNavPath.h
//
//*****************************************************************************