//*****************************************************************************
// File       : CNavBounding.h
// Desc       : 
// Author     : June C. Kim (cynis@hotmail.com)
// Update     : 2007. 1. 18
//*****************************************************************************

#ifndef ___C_NAV_BOUNDING_H___
#define ___C_NAV_BOUNDING_H___

//-----------------------------------------------------------------------------
// Pre-Compiler Option Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "NavBase.h"

#include "CNavPosition.h"


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
// Name       : CNavBounding
// Desc       : 
// Author     : 
//=============================================================================

class CNavBounding
{
	protected:

		CNavPosition					m_cMin;
		CNavPosition					m_cMax;

	public:

		inline							CNavBounding( void ) { Reset(); }
		inline							CNavBounding( const CNavBounding & rcBounding ) : m_cMin( rcBounding.GetMin() ), m_cMax( rcBounding.GetMax() ) {}
		inline							CNavBounding( const CNavPosition & rcMin, const CNavPosition & rcMax ) : m_cMin( rcMin ), m_cMax( rcMax ) {}
		inline							CNavBounding( Fixed32 nMinX, Fixed32 nMinY, Fixed32 nMinZ, Fixed32 nMaxX, Fixed32 nMaxY, Fixed32 nMaxZ, Int32 nW = 0L ) :
													m_cMin( nMinX, nMinY, nMinZ, nW ), m_cMax( nMaxX, nMaxY, nMaxZ, nW ) {}

		inline							~CNavBounding( void ) {}

	public:

		inline const CNavBounding &		operator =( const CNavBounding & rcBounding ) { Set( rcBounding ); return *this; }
		inline bool						operator ==( const CNavBounding & rcBounding ) const { return IsEqual( rcBounding ); }

	public:

		inline const CNavPosition &		GetMin( void ) const { return m_cMin; }
		inline const CNavPosition &		GetMax( void ) const { return m_cMax; }

		inline Fixed32					GetCenterX( void ) const { return ( ( m_cMin.GetX() + m_cMax.GetX() ) >> 1 ); }
		inline Fixed32					GetCenterY( void ) const { return ( ( m_cMin.GetY() + m_cMax.GetY() ) >> 1 ); }
		inline Fixed32					GetCenterZ( void ) const { return ( ( m_cMin.GetZ() + m_cMax.GetZ() ) >> 1 ); }

		inline Fixed32					GetSizeX( void ) const { return ( m_cMax.GetX() - m_cMin.GetX() ); }
		inline Fixed32					GetSizeY( void ) const { return ( m_cMax.GetY() - m_cMin.GetY() ); }
		inline Fixed32					GetSizeZ( void ) const { return ( m_cMax.GetZ() - m_cMin.GetZ() ); }

		inline void						Get( CNavBounding & rcBounding ) const
		{
			rcBounding.SetMin( m_cMin );
			rcBounding.SetMax( m_cMax );
		}

		inline void						GetCenter( CNavPosition & rcCenter ) const
		{
			rcCenter.SetX( GetCenterX() );
			rcCenter.SetY( GetCenterY() );
			rcCenter.SetZ( GetCenterZ() );
			rcCenter.SetW( GetW() );
		}

		inline void						GetSize( CNavPosition & rcSize ) const
		{
			rcSize.SetX( GetSizeX() );
			rcSize.SetY( GetSizeY() );
			rcSize.SetZ( GetSizeZ() );
			rcSize.SetW( GetW() );
		}

		inline Int32					GetW( void ) const
		{
			Assert( m_cMin.GetW() == m_cMax.GetW() );

			return m_cMin.GetW();
		}

		inline void						SetMinX( Fixed32 nMinX ) { m_cMin.SetX( nMinX ); }
		inline void						SetMinY( Fixed32 nMinY ) { m_cMin.SetY( nMinY ); }
		inline void						SetMinZ( Fixed32 nMinZ ) { m_cMin.SetZ( nMinZ ); }

		inline void						SetMaxX( Fixed32 nMaxX ) { m_cMax.SetX( nMaxX ); }
		inline void						SetMaxY( Fixed32 nMaxY ) { m_cMax.SetY( nMaxY ); }
		inline void						SetMaxZ( Fixed32 nMaxZ ) { m_cMax.SetZ( nMaxZ ); }

		inline void						SetFloatMinX( Float32 fMinX ) { m_cMin.SetFloatX( fMinX ); }
		inline void						SetFloatMinY( Float32 fMinY ) { m_cMin.SetFloatY( fMinY ); }
		inline void						SetFloatMinZ( Float32 fMinZ ) { m_cMin.SetFloatZ( fMinZ ); }

		inline void						SetFloatMaxX( Float32 fMaxX ) { m_cMax.SetFloatX( fMaxX ); }
		inline void						SetFloatMaxY( Float32 fMaxY ) { m_cMax.SetFloatY( fMaxY ); }
		inline void						SetFloatMaxZ( Float32 fMaxZ ) { m_cMax.SetFloatZ( fMaxZ ); }

		inline void						SetMin( const CNavPosition & rcMin ) { m_cMin.Set( rcMin ); }
		inline void						SetMin( Fixed32 nMinX, Fixed32 nMinY, Fixed32 nMinZ, Int32 nWorld = 0L ) { m_cMin.Set( nMinX, nMinY, nMinZ, nWorld ); }
		inline void						SetMinFloat( Float32 fMinX, Float32 fMinY, Float32 fMinZ, Int32 nWorld = 0L ) { m_cMin.SetFloat( fMinX, fMinY, fMinZ, nWorld ); }

		inline void						SetMax( const CNavPosition & rcMax ) { m_cMax.Set( rcMax ); }
		inline void						SetMax( Fixed32 nMaxX, Fixed32 nMaxY, Fixed32 nMaxZ, Int32 nWorld = 0L ) { m_cMax.Set( nMaxX, nMaxY, nMaxZ, nWorld ); }
		inline void						SetMaxFloat( Float32 fMaxX, Float32 fMaxY, Float32 fMaxZ, Int32 nWorld = 0L ) { m_cMax.SetFloat( fMaxX, fMaxY, fMaxZ, nWorld ); }

		inline void						Set( const CNavBounding & rcBound )
		{
			m_cMin.Set( rcBound.GetMin() );
			m_cMax.Set( rcBound.GetMax() );
		}

		inline void						Set( const CNavPosition & rcMin, const CNavPosition & rcMax )
		{
			m_cMin.Set( rcMin );
			m_cMax.Set( rcMax );
		}

		inline void						Set( Fixed32 nMinX, Fixed32 nMinY, Fixed32 nMinZ, Fixed32 nMaxX, Fixed32 nMaxY, Fixed32 nMaxZ, Int32 nWorld = 0L )
		{
			m_cMin.Set( nMinX, nMinY, nMinZ, nWorld );
			m_cMax.Set( nMaxX, nMaxY, nMaxZ, nWorld );
		}

		inline void						SetFloat( Float32 fMinX, Float32 fMinY, Float32 fMinZ, Float32 fMaxX, Float32 fMaxY, Float32 fMaxZ, Int32 nWorld = 0L )
		{
			m_cMin.SetFloat( fMinX, fMinY, fMinZ, nWorld );
			m_cMax.SetFloat( fMaxX, fMaxY, fMaxZ, nWorld );
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

			if( m_cMin.GetZ() > m_cMax.GetZ() )
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
			m_cMin.Set( MAX_VAL_INT_32, MAX_VAL_INT_32, MAX_VAL_INT_32 );
			m_cMax.Set( MIN_VAL_INT_32, MIN_VAL_INT_32, MIN_VAL_INT_32 );
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

			if( m_cMin.GetZ() > m_cMax.GetZ() )
			{
				Fixed32		nTempToSwap = m_cMin.GetZ();
				m_cMin.SetZ( m_cMax.GetZ() );
				m_cMax.SetZ( nTempToSwap );
			}

			if( m_cMin.GetW() != m_cMax.GetW() )
			{
				m_cMax.SetW( m_cMin.GetW() );
			}

			return;
		}

		inline void						ExpandBySize( const CNavPosition & rcSize )
		{
			m_cMin.Sub( rcSize );
			m_cMax.Add( rcSize );
		}

		inline void						ExpandToContainPosition( const CNavPosition & rcPosition )
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

			if( rcPosition.GetZ() < m_cMin.GetZ() )
			{
				m_cMin.SetZ( rcPosition.GetZ() );
			}
			if( rcPosition.GetZ() > m_cMax.GetZ() )
			{
				m_cMax.SetZ( rcPosition.GetZ() );
			}
		}

		inline bool						IsEqual( const CNavBounding & rcBounding ) const
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

		inline bool						IsContain( const CNavPosition & rcPosition ) const
		{
			if( m_cMin.GetX() > rcPosition.GetX() || m_cMin.GetY() > rcPosition.GetY() || m_cMin.GetZ() > rcPosition.GetZ() )
			{
				return false;
			}

			if( m_cMax.GetX() < rcPosition.GetX() || m_cMax.GetY() < rcPosition.GetY() || m_cMax.GetZ() < rcPosition.GetZ() )
			{
				return false;
			}

			if( m_cMin.GetW() != rcPosition.GetW() )
			{
				return false;
			}

			return true;
		}

		inline bool						IsIntersect( const CNavBounding & rcBounding ) const
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

			if( m_cMin.GetZ() > rcBounding.GetMax().GetZ() ||
				m_cMax.GetZ() < rcBounding.GetMin().GetZ() )
			{
				return false;
			}

			if( m_cMin.GetW() != rcBounding.GetW() )
			{
				return false;
			}

			return true;
		}

		inline CNavBounding *			Clone( void ) const
		{
			return new CNavBounding( *this );
		}
};

//=============================================================================
// End of Class : CNavBounding
//=============================================================================

END_NAMESPACE( nav )

#endif //___C_NAV_BOUNDING_H___

//*****************************************************************************
//
// End of File : CNavBounding.h
//
//*****************************************************************************