//*****************************************************************************
// File       : CNavOutputStream.h
// Desc       : 
// Author     : June C. Kim (cynis@hotmail.com)
// Update     : 2007. 1. 18
//*****************************************************************************

#ifndef ___C_NAV_OUTPUT_STREAM_H___
#define ___C_NAV_OUTPUT_STREAM_H___

//-----------------------------------------------------------------------------
// Pre-Compiler Option Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "NavBase.h"


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
// Name       : CNavOutputStream
// Desc       : 
// Author     : 
//=============================================================================

class CNavOutputStream : public iOutputStream
{
	protected:

		HANDLE							m_hFile;
		DWORD							m_nFlags;

	public:

		inline							CNavOutputStream( void ) { Initialize(); }

		inline virtual					~CNavOutputStream( void ) { Destroy(); }

	private:

										CNavOutputStream( const CNavOutputStream & rcStream );

		const CNavOutputStream &		operator =( const CNavOutputStream & rcStream );

	public:

		virtual bool					Create( const CFixedString & rcFileName, DWORD dwFlags = 0 );

		virtual void					put( const char * pchData, unsigned long nDataSize );

	protected:

		virtual void					Initialize( void );
		virtual void					Destroy( void );
};


//=============================================================================
// End of Class : CNavOutputStream
//=============================================================================

END_NAMESPACE( nav )

#endif //___C_NAV_OUTPUT_STREAM_H___

//*****************************************************************************
//
// End of File : CNavOutputStream.h
//
//*****************************************************************************