//*****************************************************************************
// File       : CNavErrorHandler.cpp
// Desc       : 
// Author     : June C. Kim (cynis@hotmail.com)
// Update     : 2007. 1. 18
//*****************************************************************************

//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "StdAfx.h"

#include "CLogMessageManager.h"

#include "CFixedString.h"

#include "CNavErrorHandler.h"


//-----------------------------------------------------------------------------
// Name Space Table
//-----------------------------------------------------------------------------

USE_NAMESPACE( nav )


//-----------------------------------------------------------------------------
// Initialize Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Name			: handle
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

iErrorHandler::eAction CNavErrorHandler::handle( const char * pErrorType, const char * pErrorString, const char * const * pAttributes )
{
	CFixedString	cErrorCategory( pErrorType );
	CFixedString	cErrorDescription( pErrorString );
	cErrorDescription.Append( "\n" );

	if( pAttributes != NULL )
	{
		UInt32	nAttIndex = 0;
		while( pAttributes[nAttIndex] != NULL )
		{
			cErrorDescription.Append( pAttributes[nAttIndex] );
			cErrorDescription.Append( "\n" );
			++nAttIndex;
		}
	}

	MessageBox( NULL, cErrorDescription.GetBuffer(), cErrorCategory.GetBuffer(), MB_OK );
	g_Error( (TCHAR *)cErrorDescription.GetBuffer() );

	return iErrorHandler::CONTINUE;
}


//*****************************************************************************
//
// End of File : CNavErrorHandler.cpp
//
//*****************************************************************************