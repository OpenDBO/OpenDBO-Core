//*****************************************************************************
// File       : CNavIndexedFaceList.h
// Desc       : 
// Author     : June C. Kim (cynis@hotmail.com)
// Update     : 2007. 1. 18
//*****************************************************************************

#ifndef ___C_NAV_INDEXED_FACE_LIST_H___
#define ___C_NAV_INDEXED_FACE_LIST_H___

//-----------------------------------------------------------------------------
// Pre-Compiler Option Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "NavBase.h"

#include "CNavIndexedFace.h"


//-----------------------------------------------------------------------------
// Name Space Table
//-----------------------------------------------------------------------------

BEGIN_NAMESPACE( navex )


//-----------------------------------------------------------------------------
// Type Forward Definition Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Constant Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Type Definition Table
//-----------------------------------------------------------------------------


//=============================================================================
// Name       : CNavIndexedFaceList
// Desc       : 
// Author     : 
//=============================================================================

class CNavIndexedFaceList
{
	protected:

		UInt32							m_nCount;
		CNavIndexedFace *				m_pcFaceList;

	public:

		inline							CNavIndexedFaceList( void ) : m_nCount( 0 ), m_pcFaceList( NULL ) {}
		inline							CNavIndexedFaceList( const CNavIndexedFaceList & rcList ) : m_nCount( 0 ), m_pcFaceList( NULL ) { Set( rcList ); }

		inline							~CNavIndexedFaceList( void ) { Destroy(); }

	public:

		inline UInt32					GetCount( void ) const { return m_nCount; }

		inline void						Alloc( UInt32 nCount ) { Destroy(); Create( nCount ); }
		void							Set( const CNavIndexedFaceList & rcList );

		inline void						Get( CNavIndexedFaceList & rcList ) const { rcList.Set( *this ); }

		CNavIndexedFace *				GetFace( UInt32 nIndex ) const;

		void							SetFace( UInt32 nIndex, const CNavIndexedFace & rcFace );

	protected:

		void							Create( UInt32 nCount );
		void							Destroy( void );
};

//=============================================================================
// End of Class : CNavIndexedFaceList
//=============================================================================

END_NAMESPACE( navex )

#endif //___C_NAV_INDEXED_FACE_LIST_H___

//*****************************************************************************
//
// End of File : CNavIndexedFaceList.h
//
//*****************************************************************************