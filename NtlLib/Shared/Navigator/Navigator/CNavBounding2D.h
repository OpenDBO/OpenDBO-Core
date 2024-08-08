//*****************************************************************************
// File       : CNavBounding2D.h
// Desc       : 
// Author     : June C. Kim (cynis@hotmail.com)
// Update     : 2007. 1. 18
//*****************************************************************************

#ifndef ___C_NAV_BOUNDING_2D_H___
#define ___C_NAV_BOUNDING_2D_H___

//-----------------------------------------------------------------------------
// Pre-Compiler Option Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "NavBase.h"

#include "CNavPosition2D.h"

#include "CNavBounding.h"


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
// Name       : CNavBounding2D
// Desc       : 
// Author     : 
//=============================================================================

class CNavBounding2D
{
	protected:

		CNavPosition2D					m_cMin;
		CNavPosition2D					m_cMax;

	public:

		inline							CNavBounding2D( void ) { Reset(); }
		inline							CNavBounding2D( const CNavBounding2D & rcBounding ) : m_cMin( rcBounding.GetMin() ), m_cMax( rcBounding.GetMax() ) {}
		inline							CNavBounding2D( const CNavBounding & rcBounding ) : m_cMin( rcBounding.GetMin() ), m_cMax( rcBounding.GetMax() ) {}
		inline							CNavBounding2D( const CNavPosition2D & rcMin, const CNavPosition2D & rcMax ) : m_cMin( rcMin ), m_cMax( rcMax ) {}
		inline							CNavBounding2D( Fixed32 nMinX, Fixed32 nMinY, Fixed32 nMaxX, Fixed32 nMaxY, Int32 nW = 0 ) :
														m_cMin( nMinX, nMinY, nW ), m_cMax( nMaxX, nMaxY, nW ) {}

		inline							~CNavBounding2D( void ) {}

	private:

		inline const CNavBounding2D &	operator =( const CNavBounding2D & rcBounding ) { Set( rcBounding ); }
		inline bool						operator ==( const CNavBounding2D & rcBounding ) const { return IsEqual( rcBounding ); }

	public:

		inline const CNavPosition2D &	GetMin( void ) const { return m_cMin; }
		inline const CNavPosition2D &	GetMax( void ) const { return m_cMax; }

		inline Fixed32					GetCenterX( void ) const { return ( ( m_cMin.GetX() + m_cMax.GetX() ) >> 1 ); }
		inline Fixed32					GetCenterY( void ) const { return ( ( m_cMin.GetY() + m_cMax.GetY() ) >> 1 ); }

		inline Fixed32					GetSizeX( void ) const { return ( m_cMax.GetX() - m_cMin.GetX() ); }
		inline Fixed32					GetSizeY( void ) const { return ( m_cMax.GetY() - m_cMin.GetY() ); }

		inline void						Get( CNavBounding2D & rcBounding ) const
		{
			rcBounding.SetMin( m_cMin );
			rcBounding.SetMax( m_cMax );
		}

		inline void						GetCenter( CNavPosition2D & rcCenter ) const
		{
			rcCenter.SetX( GetCenterX() );
			rcCenter.SetY( GetCenterY() );
			rcCenter.SetW( GetW() );
		}

		inline void						GetSize( CNavPosition2D & rcSize ) const
		{
			rcSize.SetX( GetSizeX() );
			rcSize.SetY( GetSizeY() );
			rcSize.SetW( GetW() );
		}

		inline Int32					GetW( void ) const
		{
			Assert( m_cMin.GetW() == m_cMax.GetW() );

			return m_cMin.GetW();
		}

		inline void						SetMin( const CNavPosition2D & rcMin ) { m_cMin.Set( rcMin ); }
		inline void						SetMin( Fixed32 nMinX, Fixed32 nMinY, Int32 nWorld = 0L ) { m_cMin.Set( nMinX, nMinY, nWorld ); }

		inline void						SetMax( const CNavPosition2D & rcMax ) { m_cMax.Set( rcMax ); }
		inline void						SetMax( Fixed32 nMaxX, Fixed32 nMaxY, Int32 nWorld = 0L ) { m_cMax.Set( nMaxX, nMaxY, nWorld ); }

		inline void						Set( const CNavBounding2D & rcBound )
		{
			m_cMin.Set( rcBound.GetMin() );
			m_cMax.Set( rcBound.GetMax() );
		}

		inline void						Set( const CNavPosition2D & rcMin, const CNavPosition2D & rcMax )
		{
			m_cMin.Set( rcMin );
			m_cMax.Set( rcMax );
		}

		inline void						Set( Fixed32 nMinX, Fixed32 nMinY, Fixed32 nMaxX, Fixed32 nMaxY, Int32 nWorld = 0L )
		{
			m_cMin.Set( nMinX, nMinY, nWorld );
			m_cMax.Set( nMaxX, nMaxY, nWorld );
		}

		inline void						SetWorld( Int32 nW )
		{
			m_cMin.SetW( nW );
			m_cMax.SetW( nW );
		}

		inline void						Reset( void )
		{
			m_cMin.Reset();
			m_cMax.Reset();
		}

		inline bool						IsValid( void ) const
		{
			if( m_cMin.GetX() > m_cMax.GetX() )
			{
				return false;
			}

			if( m_cMin.GetY() > m_cMax.GetY() )
			{
				return false;
			}

			if( m_cMin.GetW() != m_cMax.GetW() )
			{
				return false;
			}

			return true;
		}

		inline void						Invalidate( void )
		{
			m_cMin.Set( MAX_VAL_INT_32, MAX_VAL_INT_32 );
			m_cMax.Set( MIN_VAL_INT_32, MIN_VAL_INT_32 );
		}

		inline void						Validate( void )
		{
			if( m_cMin.GetX() > m_cMax.GetX() )
			{
				Fixed32		nTempToSwap = m_cMin.GetX();
				m_cMin.SetX( m_cMax.GetX() );
				m_cMax.SetX( nTempToSwap );
			}

			if( m_cMin.GetY() > m_cMax.GetY() )
			{
				Fixed32		nTempToSwap = m_cMin.GetY();
				m_cMin.SetY( m_cMax.GetY() );
				m_cMax.SetY( nTempToSwap );
			}

			if( m_cMin.GetW() != m_cMax.GetW() )
			{
				m_cMax.SetW( m_cMin.GetW() );
			}

			return;
		}

		inline void						ExpandBySize( const CNavPosition2D & rcSize )
		{
			m_cMin.Sub( rcSize );
			m_cMax.Add( rcSize );
		}

		inline void						ExpandToContainPosition( const CNavPosition2D & rcPosition )
		{
			if( rcPosition.GetX() < m_cMin.GetX() )
			{
				m_cMin.SetX( rcPosition.GetX() );
			}
			if( rcPosition.GetX() > m_cMax.GetX() )
			{
				m_cMax.SetX( rcPosition.GetX() );
			}

			if( rcPosition.GetY() < m_cMin.GetY() )
			{
				m_cMin.SetY( rcPosition.GetY() );
			}
			if( rcPosition.GetY() > m_cMax.GetY() )
			{
				m_cMax.SetY( rcPosition.GetY() );
			}
		}

		inline bool						IsEqual( const CNavBounding2D & rcBounding ) const
		{
			if( m_cMin.IsEqual( rcBounding.GetMin() ) == false )
			{
				return false;
			}

			if( m_cMax.IsEqual( rcBounding.GetMax() ) == false )
			{
				return false;
			}

			return true;
		}

		inline bool						IsContain( const CNavPosition2D & rcPosition ) const
		{
			if( m_cMin > rcPosition )
			{
				return false;
			}

			if( m_cMax < rcPosition )
			{
				return false;
			}

			if( m_cMin.GetW() != rcPosition.GetW() )
			{
				return false;
			}

			return true;
		}

		inline bool						IsIntersect( const CNavBounding2D & rcBounding ) const
		{
			if( m_cMin.GetX() > rcBounding.GetMax().GetX() ||
				m_cMax.GetX() < rcBounding.GetMin().GetX() )
			{
				return false;
			}

			if( m_cMin.GetY() > rcBounding.GetMax().GetY() ||
				m_cMax.GetY() < rcBounding.GetMin().GetY() )
			{
				return false;
			}

			if( m_cMin.GetW() != rcBounding.GetW() )
			{
				return false;
			}

			return true;
		}

		inline CNavBounding2D *			Clone( void ) const
		{
			return new CNavBounding2D( *this );
		}
};

//=============================================================================
// End of Class : CNavBounding2D
//=============================================================================

END_NAMESPACE( nav )

#endif //___C_NAV_BOUNDING_2D_H___

//*****************************************************************************
//
// End of File : CNavBounding2D.h
//
//*****************************************************************************