//*****************************************************************************
// File       : CNavErrorHandler.h
// Desc       : 
// Author     : June C. Kim (cynis@hotmail.com)
// Update     : 2007. 1. 18
//*****************************************************************************

#ifndef __C_NAV_ERROR_HANDLER_H__
#define __C_NAV_ERROR_HANDLER_H__

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

#define g_pErrorHandler					( CNavErrorHandler::GetInstance() )


//-----------------------------------------------------------------------------
// Type Definition Table
//-----------------------------------------------------------------------------


//=============================================================================
// Name       : CNavErrorHandler
// Desc       : 
// Author     : 
//=============================================================================

class CNavErrorHandler : public iErrorHandler
{
	public:

		inline							CNavErrorHandler() {}

		inline virtual					~CNavErrorHandler() {}

	private:

										CNavErrorHandler( const CNavErrorHandler & rErrorHandler );

		const CNavErrorHandler &		operator=( const CNavErrorHandler & rErrorHandler );

	public:

		eAction							handle( const char * pErrorType, const char * pErrorString, const char * const * pAttributes );

		inline static CNavErrorHandler &	refInstance() { return *GetInstance(); }

	protected:

		static CNavErrorHandler *		m_pInstance;

	public:

		static CNavErrorHandler *		CreateInstance( void );
		static void						DestroyInstance( void );
		inline static CNavErrorHandler *	GetInstance( void ) { return m_pInstance; }
};

//=============================================================================
// End of Class : CNavErrorHandler
//=============================================================================

END_NAMESPACE( nav )

#endif //__C_NAV_ERROR_HANDLER_H__

//*****************************************************************************
//
// End of File : CNavErrorHandler.h
//
//*****************************************************************************