//*****************************************************************************
// File       : CNavPolygon.h
// Desc       : 
// Author     : June C. Kim (cynis@hotmail.com)
// Update     : 2007. 1. 18
//*****************************************************************************

#ifndef ___C_NAV_POLYGON_H___
#define ___C_NAV_POLYGON_H___

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
#include "CNavPosition2D.h"
#include "CNavPositionList.h"
#include "CNavIndexedFaceList.h"


//-----------------------------------------------------------------------------
// Name Space Table
//-----------------------------------------------------------------------------

BEGIN_NAMESPACE( navex )


//-----------------------------------------------------------------------------
// Constant Table
//-----------------------------------------------------------------------------

#define MINIMUM_VERTEX_COUNT_FOR_POLYGON	( 3 )


//-----------------------------------------------------------------------------
// Type Definition Table
//-----------------------------------------------------------------------------


//=============================================================================
// Name       : CNavPolygon
// Desc       : 
// Author     : 
//=============================================================================

class CNavPolygon
{
	protected:

		CNavPositionList				m_cVertexList;
		CNavPosition					m_cCenter;

	public:

		inline							CNavPolygon( void ) { Initialize(); }

		inline							~CNavPolygon( void ) { Destroy(); }

	private:

										CNavPolygon( const CNavPolygon & rcPolygon );

		inline const CNavPolygon &		operator=( const CNavPolygon & rcPolygon );

	public:

		bool							Create( const CNavPositionList & rcVertexList, const CNavIndexedFaceList & rcFaceList );

		UInt32							GetCount( void ) const { return m_cVertexList.GetCount(); }
		const CNavPosition *			GetPosition( UInt32 nIndex ) const { return m_cVertexList.GetPosition( nIndex ); }

		bool							Convert( Fixed32 * pnResultBuffer, CNavPosition2D & rcPosition, Float32 & rfDirection, CNavPosition2D & rcScale );

	protected:

		void							Initialize( void );
		void							Destroy( void );

		bool							SortByOutline( const CNavPositionList & rcVertexList, const CNavIndexedFaceList & rcFaceList );
		bool							SortByClockwise( void );
		bool							CalculateCenter( void );
};

//=============================================================================
// End of Class : CNavPolygon
//=============================================================================

END_NAMESPACE( navex )

#endif //___C_NAV_POLYGON_H___

//*****************************************************************************
//
// End of File : CNavPolygon.h
//
//*****************************************************************************