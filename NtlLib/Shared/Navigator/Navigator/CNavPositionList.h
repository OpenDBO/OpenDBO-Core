//*****************************************************************************
// File       : CNavPositionList.h
// Desc       : 
// Author     : June C. Kim (cynis@hotmail.com)
// Update     : 2007. 1. 18
//*****************************************************************************

#ifndef ___C_NAV_POSITION_LIST_H___
#define ___C_NAV_POSITION_LIST_H___

//-----------------------------------------------------------------------------
// Pre-Compiler Option Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "NavBase.h"

#include "CNavPosition.h"


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


//-----------------------------------------------------------------------------
// Type Definition Table
//-----------------------------------------------------------------------------


//=============================================================================
// Name       : CNavPositionList
// Desc       : 
// Author     : 
//=============================================================================

class CNavPositionList
{
	protected:

		UInt32							m_nCount;
		CNavPosition *					m_pcPositionList;
		Fixed32 *						m_pnLengthList;

	public:

		inline							CNavPositionList( void ) : m_nCount( 0 ), m_pcPositionList( NULL ), m_pnLengthList( NULL ) {}
		inline							CNavPositionList( const CNavPositionList & rcList ) : m_nCount( 0 ), m_pcPositionList( NULL ), m_pnLengthList( NULL ) { Set( rcList ); }

		inline							~CNavPositionList( void ) { Destroy(); }

	public:

		inline UInt32					GetCount( void ) const { return m_nCount; }

		inline void						Alloc( UInt32 nCount ) { Destroy(); Create( nCount ); }
		void							Set( const CNavPositionList & rcList );

		inline void						Get( CNavPositionList & rcList ) const { rcList.Set( *this ); }

		const CNavPosition *			GetPosition( UInt32 nIndex ) const;
		Fixed32							GetLength( UInt32 nIndex ) const;

		inline Fixed32					GetTotalLength( void ) const { return GetLength( GetCount() - 1 ); }

		void							SetPosition( UInt32 nIndex, const CNavPosition & rcPosition );
		void							SetLength( UInt32 nIndex, Int32 nLength );

	protected:

		void							Create( UInt32 nCount );
		void							Destroy( void );
};

//=============================================================================
// End of Class : CNavPositionList
//=============================================================================

END_NAMESPACE( nav )

#endif //___C_NAV_POSITION_LIST_H___

//*****************************************************************************
//
// End of File : CNavPositionList.h
//
//*****************************************************************************