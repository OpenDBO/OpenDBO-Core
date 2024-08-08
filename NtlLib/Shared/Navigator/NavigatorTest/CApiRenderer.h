//*****************************************************************************
// File       : CApiRenderer.h
// Desc       : 
// Begin      : 
// Copyright  : 
// Author     : 
// Update     : 
//*****************************************************************************

#ifndef ___C_API_RENDERER_H___
#define ___C_API_RENDERER_H___

//-----------------------------------------------------------------------------
// Pre-Compiler Option Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Type Forward Definition Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "NavigatorTest.h"


//-----------------------------------------------------------------------------
// Constant Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Type Definition Table
//-----------------------------------------------------------------------------


//=============================================================================
// Name       : CApiRenderer
// Desc       : 
// Author     : 
//=============================================================================

class CApiRenderer : public CNavRenderer
{
	protected:

		HDC								m_hScreenDC;
		CNavPosition2D					m_cScreenPosition;
		CNavPosition2D					m_cScreenSize;

		CNavPosition2D					m_cCameraPosition;
		Fixed32							m_nMeterPerPixel;

		CNavBounding2D					m_cVisibleBound;

		bool							m_bRedraw;

	public:

		inline							CApiRenderer( void ) { Initialize(); }

		inline							~CApiRenderer( void ) { Destroy(); }

	private:

										CApiRenderer( const CApiRenderer & rcRenderer );

		const CApiRenderer &			operator =( const CApiRenderer & rcRenderer );

	public:

		void							SetScreen( const RECT & rsRect, HDC hDC = NULL );
		void							SetCamera( const CNavPosition2D & rcPosition, Fixed32 nMeterPerPixel );

		void							MoveCamera( const CNavPosition2D & rcMovement, UInt32 nFlags = 0L );

		const CNavPosition2D &			GetCameraPosition( void ) const { return m_cCameraPosition; }
		const CNavPosition2D &			GetScreenPosition( void ) const { return m_cScreenPosition; }

		Fixed32							GetMeterPerPixel( void ) const { return m_nMeterPerPixel; }
		void							SetMeterPerPixel( Fixed32 nMeterPerPixel ) { m_nMeterPerPixel = nMeterPerPixel; m_bRedraw = true; }

		bool							RenderLine( const CNavPosition & rcFromPosition, const CNavPosition & rcToPosition, UInt32 nColor, UInt32 nStyle );

		bool							PreRender( void * pvData = NULL );
		bool							PostRender( void * pvData = NULL );

		void							Invalidate( void ) { m_bRedraw = true; }

		void							ProjectPosition( CNavPosition2D & rcResult, const CNavPosition & rcOriginal );
		void							UnprojectPosition( CNavPosition & rcResult, const CNavPosition2D & rcOriginal );

	protected:

		void							Initialize( void );
		inline void						Destroy( void ) {}

		bool							IsVisiblePosition( const CNavPosition2D & rcPosition ) const;
};

//=============================================================================
// End of Class : CApiRenderer
//=============================================================================

#endif //___C_API_RENDERER_H___

//*****************************************************************************
//
// End of File : CApiRenderer.h
//
//*****************************************************************************