//*****************************************************************************
// File       : CNavPosition2D.h
// Desc       : 
// Author     : June C. Kim (cynis@hotmail.com)
// Update     : 2007. 1. 18
//*****************************************************************************

#ifndef ___C_NAV_POSITION_2D_H___
#define ___C_NAV_POSITION_2D_H___

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
// Name       : CNavPosition2D
// Desc       : 
// Author     : 
//=============================================================================

class CNavPosition2D
{
	protected:

		Fixed32							m_nX;
		Fixed32							m_nY;
		Int32							m_nW;

	public:

		inline							CNavPosition2D( void ) : m_nX( 0 ), m_nY( 0 ), m_nW( 0 ) {}
		inline							CNavPosition2D( Fixed32 nX, Fixed32 nY, Int32 nW = 0 ) : m_nX( nX ), m_nY( nY ), m_nW( nW ) {}
//		inline							CNavPosition2D( Float32 fX, Float32 fZ, Int32 nW = 0 ) : m_nW( nW ) { SetFloatX( fX ); SetFloatZ( fZ ); }
		inline							CNavPosition2D( const CNavPosition2D & rcPosition ) : m_nX( rcPosition.GetX() ), m_nY( rcPosition.GetY() ), m_nW( rcPosition.GetW() ) {}
		inline							CNavPosition2D( const CNavPosition & rcPosition ) : m_nX( rcPosition.GetX() ), m_nY( rcPosition.GetY() ), m_nW( rcPosition.GetW() ) {}

		inline							~CNavPosition2D( void ) {}

	public:

		inline const CNavPosition2D &	operator =( const CNavPosition2D & rcPosition ) { Set( rcPosition ); }

		inline bool						operator ==( const CNavPosition2D & rcPosition ) const { return IsEqual( rcPosition ); }
		inline bool						operator !=( const CNavPosition2D & rcPosition ) const { return !IsEqual( rcPosition ); }
		inline bool						operator >( const CNavPosition2D & rcPosition ) const { return IsLargerThan( rcPosition ); }
		inline bool						operator <( const CNavPosition2D & rcPosition ) const { return IsSmallerThan( rcPosition ); }
		inline bool						operator >=( const CNavPosition2D & rcPosition ) const { return !IsSmallerThan( rcPosition ); }
		inline bool						operator <=( const CNavPosition2D & rcPosition ) const { return !IsLargerThan( rcPosition ); }

	public:

		inline Fixed32					GetX( void ) const { return m_nX; }
		inline Fixed32					GetY( void ) const { return m_nY; }
		inline Int32					GetW( void ) const { return m_nW; }

		inline Float32					GetFloatX( void ) const { return Fixed32ToFloat32( m_nX ); }
		inline Float32					GetFloatY( void ) const { return Fixed32ToFloat32( m_nY ); }

		inline void						Get( CNavPosition2D & rcPosition ) const
		{
			rcPosition.SetX( m_nX );
			rcPosition.SetY( m_nY );
			rcPosition.SetW( m_nW );
		}

		inline void						Get( CNavPosition & rcPosition ) const
		{
			rcPosition.SetX( m_nX );
			rcPosition.SetY( m_nY );
			rcPosition.SetW( m_nW );
		}

		inline void						Get( long anPositionArray[2] ) const
		{
			anPositionArray[0] = GetX();
			anPositionArray[1] = GetY();
		}

		inline void						SetX( Fixed32 nX ) { m_nX = nX; }
		inline void						SetY( Fixed32 nY ) { m_nY = nY; }
		inline void						SetW( Int32 nW ) { m_nW = nW; }

		inline void						SetFloatX( Float32 fX ) { m_nX = Float32ToFixed32( fX ); }
		inline void						SetFloatY( Float32 fY ) { m_nY = Float32ToFixed32( fY ); }

		inline void						Set( const CNavPosition2D & rcPosition )
		{
			SetX( rcPosition.GetX() );
			SetY( rcPosition.GetY() );
			SetW( rcPosition.GetW() );
		}

		inline void						Set( const CNavPosition & rcPosition )
		{
			SetX( rcPosition.GetX() );
			SetY( rcPosition.GetY() );
			SetW( rcPosition.GetW() );
		}

		inline void						Set( Fixed32 nX, Fixed32 nY, Int32 nW = 0 )
		{
			SetX( nX );
			SetY( nY );
			SetW( nW );
		}

		inline void						SetFloat( Float32 fX, Float32 fY, Int32 nW = 0 )
		{
			SetFloatX( fX );
			SetFloatY( fY );
			SetW( nW );
		}

		inline void						Reset( void )
		{
			SetX( 0 );
			SetY( 0 );
			SetW( 0 );
		}

		inline bool						IsEqual( const CNavPosition2D & rcPosition ) const
		{
			if( GetX() != rcPosition.GetX() )
			{
				return false;
			}

			if( GetY() != rcPosition.GetY() )
			{
				return false;
			}

			if( GetW() != rcPosition.GetW() )
			{
				return false;
			}

			return true;
		}

		inline bool						IsLargerThan( const CNavPosition2D & rcPosition ) const
		{
			if( m_nX <= rcPosition.GetX() )
			{
				return false;
			}

			if( m_nY <= rcPosition.GetY() )
			{
				return false;
			}

			return true;
		}

		inline bool						IsSmallerThan( const CNavPosition2D & rcPosition ) const
		{
			if( m_nX >= rcPosition.GetX() )
			{
				return false;
			}

			if( m_nY >= rcPosition.GetY() )
			{
				return false;
			}

			return true;
		}

		inline CNavPosition2D *			Clone( void ) const
		{
			return new CNavPosition2D( *this );
		}

		inline CNavPosition2D &			Add( const CNavPosition2D & rcPosition )
		{
			m_nX += rcPosition.GetX();
			m_nY += rcPosition.GetY();

			return *this;
		}

		inline CNavPosition2D &			Add( Fixed32 nX, Fixed32 nY )
		{
			m_nX += nX;
			m_nY += nY;

			return *this;
		}

		inline CNavPosition2D &			Sub( const CNavPosition2D & rcPosition )
		{
			m_nX -= rcPosition.GetX();
			m_nY -= rcPosition.GetY();

			return *this;
		}

		inline CNavPosition2D &			Sub( Fixed32 nX, Fixed32 nY )
		{
			m_nX -= nX;
			m_nY -= nY;

			return *this;
		}

		inline CNavPosition2D &			Mul( const CNavPosition2D & rcPosition )
		{
			m_nX *= rcPosition.GetX();
			m_nY *= rcPosition.GetY();

			return *this;
		}

		inline CNavPosition2D &			Mul( Fixed32 nScale )
		{
			m_nX *= nScale;
			m_nY *= nScale;

			return *this;
		}

		inline CNavPosition2D &			MulFloat( Float32 fScale ) { return Mul( Float32ToFixed32( fScale ) ); }

		inline CNavPosition2D &			Scale( const CNavPosition2D & rcPosition )
		{
			m_nX = m_nX * rcPosition.GetX() / RATIO_FLOAT32_TO_FIXED32_I;
			m_nY = m_nY * rcPosition.GetY() / RATIO_FLOAT32_TO_FIXED32_I;

			return *this;
		}

		inline CNavPosition2D &			Scale( Fixed32 nScale )
		{
			m_nX = m_nX * nScale / RATIO_FLOAT32_TO_FIXED32_I;
			m_nY = m_nY * nScale / RATIO_FLOAT32_TO_FIXED32_I;

			return *this;
		}

		inline CNavPosition2D &			Div( const CNavPosition2D & rcPosition )
		{
			Assert( rcPosition.GetX() != 0 );
			Assert( rcPosition.GetY() != 0 );

			m_nX /= rcPosition.GetX();
			m_nY /= rcPosition.GetY();

			return *this;
		}

		inline CNavPosition2D &			Div( Fixed32 nScale )
		{
			Assert( nScale != 0 );

			m_nX /= nScale;
			m_nY /= nScale;

			return *this;
		}

		inline CNavPosition2D &			Rotate( Float32 fRotation )
		{
			Float32		fCosTheta = cos( fRotation );
			Float32		fSinTheta = sin( fRotation );
			Float32		fX = static_cast<Float32>(m_nX);
			Float32		fY = static_cast<Float32>(m_nY);

			m_nX = static_cast<Fixed32>( fCosTheta * fX - fSinTheta * fY );
			m_nY = static_cast<Fixed32>( fSinTheta * fX + fCosTheta * fY );

			return *this;
		}

		inline CNavPosition2D &			DivFloat( Float32 fScale ) { return Div( Float32ToFixed32( fScale ) ); }

		inline Fixed32					GetDistance( void ) const { return SqrtUInt32( m_nX * m_nX + m_nY * m_nY ); }

		inline Fixed32					GetDistanceFrom( const CNavPosition2D & rcPosition ) const { CNavPosition2D cPosition( *this ); cPosition.Sub( rcPosition ); return cPosition.GetDistance(); }
};

//=============================================================================
// End of Class : CNavPosition2D
//=============================================================================

END_NAMESPACE( nav )

#endif //___C_NAV_POSITION_2D_H___

//*****************************************************************************
//
// End of File : CNavPosition2D.h
//
//*****************************************************************************