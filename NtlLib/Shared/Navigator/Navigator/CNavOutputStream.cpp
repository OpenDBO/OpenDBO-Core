//*****************************************************************************
// File       : CNavOutputStream.cpp
// Desc       : 
// Author     : June C. Kim (cynis@hotmail.com)
// Update     : 2007. 1. 18
//*****************************************************************************

//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "StdAfx.h"

#include "CNavOutputStream.h"


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

void CNavOutputStream::Initialize( void )
{
	m_hFile		= INVALID_HANDLE_VALUE;
	m_nFlags	= 0L;

	return;
}


//-----------------------------------------------------------------------------
// Name			: Destroy
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CNavOutputStream::Destroy( void )
{
	if( m_hFile != INVALID_HANDLE_VALUE )
	{
		CloseHandle( m_hFile );
	}

	Initialize();

	return;
}


//-----------------------------------------------------------------------------
// Name			: Create
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CNavOutputStream::Create( const CFixedString & rcFileName, DWORD dwFlags )
{
	Destroy();

	m_hFile =  CreateFileSafe( rcFileName.GetBuffer(), GENERIC_WRITE, FILE_SHARE_WRITE,
								NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );

	if( m_hFile == INVALID_HANDLE_VALUE )
	{
		return false;
	}

	m_nFlags = dwFlags;

	return true;
}


//-----------------------------------------------------------------------------
// Name			: put
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CNavOutputStream::put( const char * pchData, unsigned long nDataSize )
{
	if( m_hFile == INVALID_HANDLE_VALUE )
	{
		return;
	}

	DWORD	nWrittenSize;

	WriteFile( m_hFile, pchData, nDataSize, &nWrittenSize, NULL );

	return;
}


//*****************************************************************************
//
// End of File : CNavOutputStream.cpp
//
//*****************************************************************************