//*****************************************************************************
// File       : CNavRenderer.h
// Desc       : 
// Author     : June C. Kim (cynis@hotmail.com)
// Update     : 2007. 1. 18
//*****************************************************************************

#ifndef ___C_NAV_RENDERER_H___
#define ___C_NAV_RENDERER_H___

//-----------------------------------------------------------------------------
// Pre-Compiler Option Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "NavBase.h"
#include "CNavPosition.h"
#include "CNavPosition2D.h"
#include "NavEntityBase.h"


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
// Name       : CNavRenderer
// Desc       : 
// Author     : 
//=============================================================================

class CNavRenderer
{
	protected:

		SNavEntityResultInfo			m_sResult;

	public:

		inline							CNavRenderer( void ) { Initialize(); }

		inline virtual					~CNavRenderer( void ) { Destroy(); }

	private:

										CNavRenderer( const CNavRenderer & rcRenderer );

		const CNavRenderer &			operator =( const CNavRenderer & rcRenderer );

	public:

		virtual bool					PreRender( void * pvData = NULL ) = 0;

		virtual bool					RenderLine( const CNavPosition & rcFromPosition, const CNavPosition & rcToPosition, UInt32 nColor, UInt32 nStyle ) = 0;
		inline virtual void				Skip( void ) { m_sResult.Skip(); }

		virtual bool					PostRender( void * pvData = NULL ) = 0;

	protected:

		inline virtual void				Initialize( void ) {}
		inline virtual void				Destroy( void ) {}
};

//=============================================================================
// End of Class : CNavRenderer
//=============================================================================

END_NAMESPACE( nav )

#endif //___C_NAV_RENDERER_H___

//*****************************************************************************
//
// End of File : CNavRenderer.h
//
//*****************************************************************************