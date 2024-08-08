//*****************************************************************************
// File       : CNavIndexedFace.h
// Desc       : 
// Author     : June C. Kim (cynis@hotmail.com)
// Update     : 2007. 1. 18
//*****************************************************************************

#ifndef ___C_INDEXED_FACE_H___
#define ___C_INDEXED_FACE_H___

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


//-----------------------------------------------------------------------------
// Name Space Table
//-----------------------------------------------------------------------------

BEGIN_NAMESPACE( navex )


//-----------------------------------------------------------------------------
// Constant Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Type Definition Table
//-----------------------------------------------------------------------------

#define NAV_EDGE_ATTRIBUTE_NONE			( 0x00000000 )
#define NAV_EDGE_ATTRIBUTE_EXTERNAL		( 0x00000001 )


//=============================================================================
// Name       : CNavIndexedTriangle
// Desc       : 
// Author     : 
//=============================================================================

class CNavIndexedFace
{
	protected:

		Int32							m_nVertexIndex[3];
		UInt32							m_nEdgeFlags[3];

	public:

		inline							CNavIndexedFace( void ) { Reset(); }
		inline							CNavIndexedFace( const CNavIndexedFace & rcFace ) { Set( rcFace ); }

		inline							~CNavIndexedFace( void ) {}

	public:

		inline const CNavIndexedFace &	operator=( const CNavIndexedFace & rcFace ) { Set( rcFace ); return *this; }

	public:

		inline void						SetIndex( Int32 nIndex0, Int32 nIndex1, Int32 nIndex2 ) { m_nVertexIndex[0] = nIndex0; m_nVertexIndex[1] = nIndex1; m_nVertexIndex[2] = nIndex2; }
		inline void						SetIndex( UInt32 nPosition, Int32 nIndex ) { if( nPosition < 3 ) { m_nVertexIndex[nPosition] = nIndex; } }

		inline Int32					GetIndex( UInt32 nPosition ) const { if( nPosition < 3 ) { return m_nVertexIndex[nPosition]; } else { return -1; } }

		inline void						SetFlags( Int32 nFlags0, Int32 nFlags1, Int32 nFlags2 ) { m_nEdgeFlags[0] = nFlags0; m_nEdgeFlags[1] = nFlags1; m_nEdgeFlags[2] = nFlags2; }
		inline void						SetFlags( UInt32 nPosition, Int32 nFlags ) { if( nPosition < 3 ) { m_nEdgeFlags[nPosition] = nFlags; } }

		inline Int32					GetFlags( UInt32 nPosition ) const { if( nPosition < 3 ) { return m_nEdgeFlags[nPosition]; } else { return -1; } }

		inline void						Set( const CNavIndexedFace & rcFace ) { memcpy( this, &rcFace, sizeof( CNavIndexedFace ) ); }

		inline void						Reset( void ) { m_nVertexIndex[0] = -1; m_nVertexIndex[1] = -1; m_nVertexIndex[2] = -1;																\
														m_nEdgeFlags[0] = NAV_EDGE_ATTRIBUTE_NONE; m_nEdgeFlags[1] = NAV_EDGE_ATTRIBUTE_NONE; m_nEdgeFlags[2] = NAV_EDGE_ATTRIBUTE_NONE; }
};

//=============================================================================
// End of Class : CNavIndexedFace
//=============================================================================

END_NAMESPACE( navex )

#endif //___C_INDEXED_FACE_H___

//*****************************************************************************
//
// End of File : CNavIndexedFace.h
//
//*****************************************************************************