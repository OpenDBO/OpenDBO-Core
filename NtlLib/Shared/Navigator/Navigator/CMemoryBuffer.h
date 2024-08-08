//*****************************************************************************
// File       : CMemoryBuffer.h
// Desc       : 
// Author     : June C. Kim (cynis@hotmail.com)
// Update     : 2007. 1. 18
//*****************************************************************************

#ifndef ___C_MEMORY_BUFFER_H___
#define ___C_MEMORY_BUFFER_H___

//-----------------------------------------------------------------------------
// Pre-Compiler Option Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "CFixedString.h"


//-----------------------------------------------------------------------------
// Name Space Table
//-----------------------------------------------------------------------------


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
// Name       : CMemoryBuffer
// Desc       : 
// Author     : 
//=============================================================================

class CMemoryBuffer
{
	protected:

		void *							m_pvBuffer;
		size_t							m_nLength;

		size_t							m_nPosition;

	public:

		inline							CMemoryBuffer( void ) : m_pvBuffer( NULL ), m_nLength( 0 ), m_nPosition( 0 ) {}
		inline							CMemoryBuffer( const CMemoryBuffer & rcBuffer ) : m_pvBuffer( NULL ), m_nLength( 0 ), m_nPosition( 0 ) { Create( rcBuffer ); }
		inline							CMemoryBuffer( const CFixedString & rcFileName ) : m_pvBuffer( NULL ), m_nLength( 0 ), m_nPosition( 0 ) { Create( rcFileName ); }
		inline							CMemoryBuffer( size_t nLength, const void * pvSourceBuffer = NULL ) : m_pvBuffer( NULL ), m_nLength( 0 ), m_nPosition( 0 ) { Create( nLength, pvSourceBuffer ); }

										~CMemoryBuffer( void );

	public:

		const CMemoryBuffer &			operator =( const CMemoryBuffer & rBuffer );

	public:

		bool							Create( const CMemoryBuffer & rcSource );
		bool							Create( const CFixedString & rcFileName );
		bool							Create( size_t nLength, const void * pvSourceBuffer = NULL );

		void							Destroy( void );

		CMemoryBuffer *					Clone( void ) const;

		inline void *					GetBuffer( void ) const { return m_pvBuffer; }
		inline size_t					GetLength( void ) const { return m_nLength; }

		inline bool						IsEnd( void ) const { return (m_nPosition >= m_nLength); }
		inline void *					GetAt( size_t nPosition, size_t nOffset = 1 ) const { return (BYTE *)m_pvBuffer + nPosition * nOffset; }
		inline void 					CopyAt( void * pvData, size_t nSize, size_t nPosition, size_t nOffset = 1 ) const { memcpy( pvData, ((BYTE *)m_pvBuffer + nPosition * nOffset), nSize ); }
		inline void 					Copy( void * pvData, size_t nSize ) { memcpy( pvData, ((BYTE *)m_pvBuffer + m_nPosition), nSize ); m_nPosition += nSize; }
		bool		 					CopySafe( void * pvData, size_t nSize );

		const char * const				Dump( void ) const;

		inline bool						SetPosition( size_t nPosition )
		{
			if( nPosition >= m_nLength )
			{
				return false;
			}

			m_nPosition = nPosition;
		}

		bool							Read( void * pvData, size_t nReadSize );

	protected:

		void							GenerateCRC( void );
};

//=============================================================================
// End of Class : CMemoryBuffer
//=============================================================================

#endif //___C_MEMORY_BUFFER_H___

//*****************************************************************************
//
// End of File : CMemoryBuffer.h
//
//*****************************************************************************