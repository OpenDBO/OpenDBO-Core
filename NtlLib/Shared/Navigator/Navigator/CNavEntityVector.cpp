//*****************************************************************************
// File       : CNavEntityVector.cpp
// Desc       : 
// Author     : June C. Kim (cynis@hotmail.com)
// Update     : 2007. 1. 18
//*****************************************************************************

//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "StdAfx.h"

#include "CNavEntity.h"

#include "CNavEntityVector.h"


//-----------------------------------------------------------------------------
// Name Space Table
//-----------------------------------------------------------------------------

USE_NAMESPACE( nav )


//-----------------------------------------------------------------------------
// Initialize Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Name			: Initialize
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CNavEntityVector::Initialize( void )
{
	m_ppcVector			= NULL;
	m_nSize				= 0L;
	m_nCount			= 0L;
	m_nFirstFreeOrder	= 0L;

	return;
}


//-----------------------------------------------------------------------------
// Name			: Destroy
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CNavEntityVector::Destroy( bool bDestroyEntity )
{
	if( m_ppcVector == NULL )
	{
		return;
	}

	if( bDestroyEntity == true )
	{
		for( UInt32 nOrder = 0; nOrder < m_nSize; ++nOrder )
		{
			if( m_ppcVector[nOrder] != NULL )
			{
				CNavEntityFactory::GetInstance()->DestroyEntity( m_ppcVector[nOrder] );
			}
		}
	}

	delete[] m_ppcVector;
	m_ppcVector = NULL;

	return;
}


//-----------------------------------------------------------------------------
// Name			: Create
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CNavEntityVector::Create( UInt32 nReservedSize )
{
	Destroy( m_bDestroyEntityWhenDestroyed );
	Initialize();

	if( nReservedSize > 0 )
	{
		m_ppcVector = new CNavEntity *[nReservedSize];
		Assert( m_ppcVector != NULL );
		ZeroMemory( m_ppcVector, sizeof( CNavEntity * ) * nReservedSize );

		m_nSize = nReservedSize;
	}

	return true;
}


//-----------------------------------------------------------------------------
// Name			: Insert
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CNavEntityVector::Insert( CNavEntity * const pcEntity )
{
	Assert( pcEntity != NULL );
	Assert( m_nSize >= m_nCount );

	if( m_nSize == m_nCount )
	{
		if( m_ppcVector == NULL )
		{
			Assert( m_nCount == 0L );

			m_nSize = NAV_ENTITY_VECTOR_SIZE_DEFAULT;

			m_ppcVector = new CNavEntity *[m_nSize];
			Assert( m_ppcVector != NULL );

			ZeroMemory( m_ppcVector, sizeof( CNavEntity * ) * m_nSize );

			m_nFirstFreeOrder = 0L;
		}
		else
		{
			Assert( m_nCount != 0L );

			m_nSize = m_nSize << 1;

			CNavEntity **	ppcNewVector = new CNavEntity *[m_nSize];
			Assert( ppcNewVector != NULL );

			memcpy( ppcNewVector, m_ppcVector, sizeof( CNavEntity * ) * m_nCount );
			ZeroMemory( &(ppcNewVector[m_nCount]), sizeof( CNavEntity * ) * ( m_nSize - m_nCount ) );

			Destroy( false );

			m_ppcVector = ppcNewVector;

			m_nFirstFreeOrder = m_nCount;
		}
	}

	while( m_nFirstFreeOrder < m_nSize )
	{
		if( m_ppcVector[m_nFirstFreeOrder] == NULL )
		{
			m_ppcVector[m_nFirstFreeOrder] = pcEntity;

			++m_nCount;
			++m_nFirstFreeOrder;

			return true;
		}

		++m_nFirstFreeOrder;
	}

	Assert( !"Invalid m_nFirstFreeOrder" );

	return false;
}


//-----------------------------------------------------------------------------
// Name			: Remove
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CNavEntityVector::Remove( CNavEntity * const pcEntity )
{
	Assert( pcEntity != NULL );

	if( m_ppcVector == NULL )
	{
		Assert( !"Empty Vector" );

		return false;
	}

	for( UInt32 nOrder = 0; nOrder < m_nSize; ++nOrder )
	{
		if( m_ppcVector[nOrder] == pcEntity &&
			m_bDestroyEntityWhenDestroyed == true )
		{
			CNavEntityFactory::GetInstance()->DestroyEntity( m_ppcVector[nOrder] );
			m_ppcVector[nOrder] = NULL;

			m_nFirstFreeOrder = Min( nOrder, m_nFirstFreeOrder );

			--m_nCount;

			return true;
		}
	}

	Assert( !"No Match Entity" );

	return false;
}


//-----------------------------------------------------------------------------
// Name			: InsertAt
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CNavEntityVector::InsertAt( CNavEntity * const pcEntity, UInt32 nOrder )
{
	Assert( pcEntity != NULL );
	Assert( m_nSize >= m_nCount );

	if( m_nSize <= nOrder )
	{
		if( m_ppcVector == NULL )
		{
			Assert( m_nCount == 0L );

			m_nSize = NAV_ENTITY_VECTOR_SIZE_DEFAULT;

			while( m_nSize <= nOrder )
			{
				m_nSize <<= 1;
			}

			m_ppcVector = new CNavEntity *[m_nSize];
			Assert( m_ppcVector != NULL );

			ZeroMemory( m_ppcVector, sizeof( CNavEntity * ) * m_nSize );

			m_nFirstFreeOrder = 0L;
		}
		else
		{
			UInt32	nNewSize = NAV_ENTITY_VECTOR_SIZE_DEFAULT;

			while( nNewSize <= nOrder )
			{
				nNewSize <<= 1;
			}

			CNavEntity **	ppcNewVector = new CNavEntity *[nNewSize];
			Assert( ppcNewVector != NULL );

			memcpy( ppcNewVector, m_ppcVector, sizeof( CNavEntity * ) * m_nCount );
			ZeroMemory( &(ppcNewVector[m_nCount]), sizeof( CNavEntity * ) * ( nNewSize - m_nSize ) );

			Destroy( false );

			m_ppcVector = ppcNewVector;

			m_nFirstFreeOrder = m_nCount;
			m_nSize = nNewSize;
		}
	}

	if( m_ppcVector[nOrder] != NULL &&
		m_bDestroyEntityWhenDestroyed == true )
	{
		CNavEntityFactory::GetInstance()->DestroyEntity( m_ppcVector[nOrder] );
	}

	m_ppcVector[nOrder] = pcEntity;

	return true;
}


//-----------------------------------------------------------------------------
// Name			: InsertAt
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CNavEntityVector::RemoveAt( UInt32 nOrder )
{
	if( m_ppcVector == NULL )
	{
		Assert( !"Empty Vector" );

		return false;
	}

	if( m_nSize >= nOrder )
	{
		Assert( !"Out of Range" );

		return false;
	}

	if( m_ppcVector[nOrder] == NULL )
	{
		Assert( !"Empty Slot" );

		return false;
	}


	Assert( !"No Match Entity" );

	return true;
}


//-----------------------------------------------------------------------------
// Name			: ReplaceAt
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CNavEntityVector::ReplaceAt( CNavEntity * const pcEntity, UInt32 nOrder )
{
	if( RemoveAt( nOrder ) == false )
	{
		return false;
	}

	return InsertAt( pcEntity, nOrder );
}


//-----------------------------------------------------------------------------
// Name			: GetAt
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CNavEntity * const CNavEntityVector::GetAt( UInt32 nOrder ) const
{
	if( m_ppcVector == NULL )
	{
		return NULL;
	}

	if( nOrder >= m_nSize )
	{
		return NULL;
	}

	return m_ppcVector[nOrder];
}


//-----------------------------------------------------------------------------
// Name			: SearchChildByUniqueId
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

UInt32 CNavEntityVector::SearchChildByUniqueId( UId32 nChildId ) const
{
	if( m_ppcVector == NULL )
	{
		return INVALID_POSITION;
	}

	for( UInt32 nOrder = 0; nOrder < m_nSize; ++nOrder )
	{
		if( m_ppcVector[nOrder] != NULL &&
			m_ppcVector[nOrder]->GetUniqueId() == nChildId )
		{
			return nOrder;
		}
	}

	return INVALID_POSITION;
}


//-----------------------------------------------------------------------------
// Name			: SearchChildByPosition
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

UInt32 CNavEntityVector::SearchChildByPosition( const CNavPosition & rcPosition1 ) const
{
	if( m_ppcVector == NULL )
	{
		return INVALID_POSITION;
	}

	for( UInt32 nOrder = 0; nOrder < m_nSize; ++nOrder )
	{
		if( m_ppcVector[nOrder] != NULL &&
			m_ppcVector[nOrder]->IsInBound( rcPosition1 ) == true )
		{
			return nOrder;
		}
	}

	return INVALID_POSITION;
}


//-----------------------------------------------------------------------------
// Name			: SearchChildByPosition
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

UInt32 CNavEntityVector::SearchChildByPosition( const CNavPosition & rcPosition1, const CNavPosition & rcPosition2 ) const
{
	if( m_ppcVector == NULL )
	{
		return INVALID_POSITION;
	}

	if( rcPosition1.GetW() != rcPosition2.GetW() )
	{
		return INVALID_POSITION;
	}

	for( UInt32 nOrder = 0; nOrder < m_nSize; ++nOrder )
	{
		if( m_ppcVector[nOrder] != NULL &&
			m_ppcVector[nOrder]->IsInBound( rcPosition1 ) == true )
		{
			return nOrder;
		}
	}

	return INVALID_POSITION;
}


//-----------------------------------------------------------------------------
// Name			: PreProcess
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CNavEntityVector::PreProcess( SNavEntityPreProcessInfo & rsPreProcessInfo )
{
	bool	bResult = true;

	if( m_ppcVector != NULL )
	{
		for( UInt32 nOrder = 0; nOrder < m_nSize; ++nOrder )
		{
			if( m_ppcVector[nOrder] != NULL )
			{
				bResult &= m_ppcVector[nOrder]->PreProcess( rsPreProcessInfo );
			}
		}
	}

	return bResult;
}


//-----------------------------------------------------------------------------
// Name			: PostProcess
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CNavEntityVector::PostProcess( SNavEntityPreProcessInfo & rsPostProcessInfo )
{
	bool	bResult = true;

	if( m_ppcVector != NULL )
	{
		for( UInt32 nOrder = 0L; nOrder < m_nSize; ++nOrder )
		{
			if( m_ppcVector[nOrder] != NULL )
			{
				bResult &= m_ppcVector[nOrder]->PostProcess( rsPostProcessInfo );
			}
		}
	}

	return bResult;
}


//-----------------------------------------------------------------------------
// Name			: Render
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

Int32 CNavEntityVector::Render( CNavRenderer * const pcRenderer ) const
{
	if( m_ppcVector == NULL )
	{
		return 0L;
	}

	UInt32	nRenderCount = 0L;

	for( UInt32 nOrder = 0L; nOrder < m_nSize; ++nOrder )
	{
		if( m_ppcVector[nOrder] != NULL )
		{
			nRenderCount += m_ppcVector[nOrder]->Render( pcRenderer );
		}
	}

	return nRenderCount;
}


//-----------------------------------------------------------------------------
// Name			: DoIt
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CNavEntityVector::DoIt( bool (*pfnToDo)( CNavEntity * pcThis, void * pvData ), void * pvData )
{
	if( m_ppcVector == NULL )
	{
		return false;
	}

	bool	bResult = true;

	for( UInt32 nOrder = 0L; nOrder < m_nSize; ++nOrder )
	{
		if( m_ppcVector[nOrder] != NULL )
		{
			bResult &= m_ppcVector[nOrder]->DoIt( pfnToDo, pvData );
		}
	}

	return bResult;
}


//-----------------------------------------------------------------------------
// Name			: DoItAfter
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CNavEntityVector::DoItAfter( bool (*pfnToDo)( CNavEntity * pcThis, void * pvData ), void * pvData )
{
	if( m_ppcVector == NULL )
	{
		return false;
	}

	bool	bResult = true;

	for( UInt32 nOrder = m_nSize; nOrder > 0; --nOrder )
	{
		if( m_ppcVector[nOrder - 1] != NULL )
		{
			bResult &= m_ppcVector[nOrder - 1]->DoItAfter( pfnToDo, pvData );
		}
	}

	return bResult;
}


//*****************************************************************************
//
// End of File : CNavEntityVector.cpp
//
//*****************************************************************************