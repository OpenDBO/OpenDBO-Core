//*****************************************************************************
// File       : CNavConverter.h
// Desc       : 
// Author     : June C. Kim (cynis@hotmail.com)
// Update     : 2007. 1. 18
//*****************************************************************************

#ifndef ___C_NAV_CONVERTER_H___
#define ___C_NAV_CONVERTER_H___

//-----------------------------------------------------------------------------
// Pre-Compiler Option Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "NavBase.h"
#include "CNavPosition.h"
#include "CNavPosition2D.h"


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
// Name       : CNavConverter
// Desc       : 
// Author     : 
//=============================================================================

class CNavConverter
{
	public:

		inline							CNavConverter( void ) { Initialize(); }

		inline virtual					~CNavConverter( void ) { Destroy(); }

	private:

										CNavConverter( const CNavConverter & rcConverter );

		const CNavConverter &			operator =( const CNavConverter & rcConverter );

	public:

		virtual bool					ImportHeightMap( void * pvData = NULL );
		virtual bool					ExportHeightMap( void * pvData = NULL );

		virtual bool					ImportTerrain( void * pvData = NULL );
		virtual bool					ExportTerrain( void * pvData = NULL );

	protected:

		inline virtual void				Initialize( void ) {}
		inline virtual void				Destroy( void ) {}
};

//=============================================================================
// End of Class : CNavConverter
//=============================================================================

END_NAMESPACE( nav )

#endif //___C_NAV_CONVERTER_H___

//*****************************************************************************
//
// End of File : CNavConverter.h
//
//*****************************************************************************