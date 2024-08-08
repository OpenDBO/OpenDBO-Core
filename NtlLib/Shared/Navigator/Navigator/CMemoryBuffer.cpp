//*****************************************************************************
// File       : CMemoryBuffer.cpp
// Desc       : 
// Author     : June C. Kim (cynis@hotmail.com)
// Update     : 2007. 1. 18
//*****************************************************************************

//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "StdAfx.h"

#include "CFixedString.h"

#include "CMemoryBuffer.h"


//-----------------------------------------------------------------------------
// Initialize Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Name			: ~CMemoryBuffer
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CMemoryBuffer::~CMemoryBuffer( void )
{
	Destroy();

	return;
}


//-----------------------------------------------------------------------------
// Name			: Clone
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CMemoryBuffer * CMemoryBuffer::Clone( void ) const
{
	return new CMemoryBuffer( *this );
}


//-----------------------------------------------------------------------------
// Name			: Create
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CMemoryBuffer::Create( const CMemoryBuffer & rcSource )
{
	Destroy();

	m_nLength = rcSource.GetLength();

	m_pvBuffer = new char[m_nLength];
	if( m_pvBuffer == NULL )
	{
		return false;
	}

	memcpy( m_pvBuffer, rcSource.GetBuffer(), m_nLength );

	return true;
}


//-----------------------------------------------------------------------------
// Name			: Create
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CMemoryBuffer::Create( const CFixedString & rcFileName )
{
	if( rcFileName.GetLength() == 0 )
	{
		return false;
	}

	Destroy();

	HANDLE	hFile = CreateFile( rcFileName.GetBuffer(), GENERIC_READ, FILE_SHARE_READ,
								NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
	if( hFile == INVALID_HANDLE_VALUE )
	{
		return false;
	}

	DWORD	dwFileSize;
	dwFileSize = GetFileSize( hFile, NULL );
	if( dwFileSize == 0L )
	{
		CloseHandle( hFile );

		return false;
	}

	m_pvBuffer = new char[dwFileSize];
	if( m_pvBuffer == NULL )
	{
		CloseHandle( hFile );

		return false;
	}

	DWORD	dwReadSize;
	if( ReadFile( hFile, m_pvBuffer, dwFileSize, &dwReadSize, NULL ) == false )
	{
		delete[] m_pvBuffer;

		CloseHandle( hFile );

		return false;
	}

	m_nLength = dwReadSize;

	CloseHandle( hFile );

	return true;
}


//-----------------------------------------------------------------------------
// Name			: Create
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CMemoryBuffer::Create( size_t nLength, const void * pvSourceBuffer )
{
	Destroy();
 
	if( nLength > 0 )
	{
		m_pvBuffer = new char[nLength];
		if( m_pvBuffer == NULL )
		{
			return false;
		}

		if( pvSourceBuffer != NULL )
		{
			memcpy( m_pvBuffer, pvSourceBuffer, nLength );
		}

		m_nLength	= nLength;
	}

	return true;
}


//-----------------------------------------------------------------------------
// Name			: Destroy
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CMemoryBuffer::Destroy( void )
{
	if( m_pvBuffer != NULL )
	{
		delete[] m_pvBuffer;
	}

	return;
}


//-----------------------------------------------------------------------------
// Name			: Dump
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

const char * const CMemoryBuffer::Dump( void ) const
{
	if( m_pvBuffer == NULL )
	{
		static const char	NULL_DUMP_MESSAGE[] = "(null)";

		return NULL_DUMP_MESSAGE;
	}

	return reinterpret_cast<const char *>(m_pvBuffer);
}


//-----------------------------------------------------------------------------
// Name			: CopySafe
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CMemoryBuffer::CopySafe( void * pvData, size_t nSize )
{
	if( m_nPosition + nSize >= m_nLength )
	{
		return false;
	}

	if( pvData == NULL )
	{
		return false;
	}

	if( m_pvBuffer == NULL )
	{
		return false;
	}

	Copy( pvData, nSize );

	return true;
}


//-----------------------------------------------------------------------------
// Name			: Read
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CMemoryBuffer::Read( void * pvData, size_t nReadSize )
{
	if( m_nPosition + nReadSize >= m_nLength )
	{
		return false;
	}

	Assert( pvData != NULL );

	UInt8 *		pnBuffer = static_cast<UInt8 *>(m_pvBuffer);

	memcpy( pvData, &pnBuffer[m_nPosition], nReadSize );

	m_nPosition += nReadSize;

	return true;
}


//*****************************************************************************
//
// End of File : CMemoryBuffer.cpp
//
//*****************************************************************************