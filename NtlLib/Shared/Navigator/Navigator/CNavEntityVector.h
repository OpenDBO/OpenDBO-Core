//*****************************************************************************
// File       : CNavEntityVector.h
// Desc       : 
// Author     : June C. Kim (cynis@hotmail.com)
// Update     : 2007. 1. 18
//*****************************************************************************

#ifndef ___C_NAV_ENTITY_VECTOR_H___
#define ___C_NAV_ENTITY_VECTOR_H___

//-----------------------------------------------------------------------------
// Pre-Compiler Option Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "NavEntityBase.h"

#include "CNavEntityFactory.h"


//-----------------------------------------------------------------------------
// Name Space Table
//-----------------------------------------------------------------------------

BEGIN_NAMESPACE( nav )


//-----------------------------------------------------------------------------
// Type Forward Definition Table
//-----------------------------------------------------------------------------

class CNavEntity;
class CNavRenderer;
struct SNavEntityPreProcessInfo;


//-----------------------------------------------------------------------------
// Constant Table
//-----------------------------------------------------------------------------

#define NAV_ENTITY_VECTOR_SIZE_DEFAULT	( 1 )


//-----------------------------------------------------------------------------
// Type Definition Table
//-----------------------------------------------------------------------------


//=============================================================================
// Name       : CNavEntityVector
// Desc       : 
// Author     : 
//=============================================================================

class CNavEntityVector
{
	friend class						CNavEntityFactory;

	protected:

		CNavEntity **					m_ppcVector;
		UInt32							m_nSize;
		UInt32							m_nCount;
		UInt32							m_nFirstFreeOrder;
		bool							m_bDestroyEntityWhenDestroyed;

	public:

		inline							CNavEntityVector( bool bDestroyEntityWhenDestroyed = false ) : m_bDestroyEntityWhenDestroyed( bDestroyEntityWhenDestroyed ) { Initialize(); }

		inline							~CNavEntityVector( void ) { Destroy( m_bDestroyEntityWhenDestroyed ); }

	private:

										CNavEntityVector( const CNavEntityVector & rcEntity );

		const CNavEntityVector &		operator =( const CNavEntityVector & rcEntity );

	public:

		bool							Create( UInt32 nReserveSize );
		void							Destroy( bool bDestroyEntity );

		bool							Insert( CNavEntity * const pcEntity );
		bool							Remove( CNavEntity * const pcEntity );

		bool							InsertAt( CNavEntity * const pcEntity, UInt32 nOrder );
		bool							RemoveAt( UInt32 nOrder );
		bool							ReplaceAt( CNavEntity * const pcEntity, UInt32 nOrder );

		UInt32							GetCount( void ) const { return m_nCount; }
		UInt32							GetSize( void ) const { return m_nSize; }
		UInt32							GetFirstFreeOrder( void ) const { return m_nFirstFreeOrder; }

		CNavEntity * const				GetAt( UInt32 nOrder ) const;

		UInt32							SearchChildByUniqueId( UId32 nChildId ) const;
		UInt32							SearchChildByPosition( const CNavPosition & rcPosition1 ) const;
		UInt32							SearchChildByPosition( const CNavPosition & rcPosition1, const CNavPosition & rcPosition2 ) const;

		bool							PreProcess( SNavEntityPreProcessInfo & rsPreProcessInfo );
		bool							PostProcess( SNavEntityPreProcessInfo & rsPostProcessInfo );

		Int32							Render( CNavRenderer * const pcRenderer ) const;

		bool							DoIt( bool (*pfnToDo)( CNavEntity * pcThis, void * pvData ), void * pvData = NULL );
		bool							DoItAfter( bool (*pfnToDo)( CNavEntity * pcThis, void * pvData ), void * pvData = NULL );

	protected:

		void							Initialize( void );

	protected:

		static UInt32					m_nLastErrorCode;

	public:

		static void						SetLastError( UInt32 nLastErrorCode = ERROR_SUCCESS ) { m_nLastErrorCode = nLastErrorCode; }
		static UInt32					GetLastError( void ) { return m_nLastErrorCode; }
};

//=============================================================================
// End of Class : CNavEntityVector
//=============================================================================

END_NAMESPACE( nav )

#endif //___C_NAV_ENTITY_VECTOR_H___

//*****************************************************************************
//
// End of File : CNavEntityVector.h
//
//*****************************************************************************