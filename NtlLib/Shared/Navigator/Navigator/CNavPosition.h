//*****************************************************************************
// File       : CNavPosition.h
// Desc       : 
// Author     : June C. Kim (cynis@hotmail.com)
// Update     : 2007. 1. 18
//*****************************************************************************

#ifndef ___C_NAV_POSITION_H___
#define ___C_NAV_POSITION_H___

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
// Name       : CNavPosition
// Desc       : 
// Author     : 
//=============================================================================

class CNavPosition
{
	protected:

		Fixed32							m_nX;
		Fixed32							m_nY;
		Fixed32							m_nZ;
		Int32							m_nW;

	public:

		inline							CNavPosition( void ) : m_nX( 0L ), m_nY( 0L ), m_nZ( 0L ), m_nW( 0L ) {}
		inline							CNavPosition( Fixed32 nX, Fixed32 nY, Fixed32 nZ, Int32 nW = 0L ) : m_nX( nX ), m_nY( nY ), m_nZ( nZ ), m_nW( nW ) {}
//		inline							CNavPosition( Float32 fX, Float32 fY, Float32 fZ, Int32 nW = 0L ) : m_nW( nW ) { SetFloatX( fX ); SetFloatY( fY ); SetFloatZ( fZ ); }
		inline							CNavPosition( const CNavPosition & rcPosition ) : m_nX( rcPosition.GetX() ), m_nY( rcPosition.GetY() ), m_nZ( rcPosition.GetZ() ), m_nW( rcPosition.GetW() ) {}

		inline							~CNavPosition( void ) {}

	public:

		inline const CNavPosition &		operator =( const CNavPosition & rcPosition ) { Set( rcPosition ); return *this; }

		inline bool						operator ==( const CNavPosition & rcPosition ) const { return IsEqual( rcPosition ); }
		inline bool						operator !=( const CNavPosition & rcPosition ) const { return !IsEqual( rcPosition ); }
		inline bool						operator >( const CNavPosition & rcPosition ) const { return IsLargerThan( rcPosition ); }
		inline bool						operator <( const CNavPosition & rcPosition ) const { return IsSmallerThan( rcPosition ); }
		inline bool						operator >=( const CNavPosition & rcPosition ) const { return !IsSmallerThan( rcPosition ); }
		inline bool						operator <=( const CNavPosition & rcPosition ) const { return !IsLargerThan( rcPosition ); }

	public:

		inline Fixed32					GetX( void ) const { return m_nX; }
		inline Fixed32					GetY( void ) const { return m_nY; }
		inline Fixed32					GetZ( void ) const { return m_nZ; }
		inline Int32					GetW( void ) const { return m_nW; }

		inline Float32					GetFloatX( void ) const { return Fixed32ToFloat32( m_nX ); }
		inline Float32					GetFloatY( void ) const { return Fixed32ToFloat32( m_nZ ); }
		inline Float32					GetFloatZ( void ) const { return Fixed32ToFloat32( m_nY ); }

		inline void						Get( CNavPosition & rcPosition ) const
		{
			rcPosition.SetX( m_nX );
			rcPosition.SetY( m_nY );
			rcPosition.SetZ( m_nZ );
			rcPosition.SetW( m_nW );
		}

		inline void						Get( long anPositionArray[3] ) const
		{
			anPositionArray[0] = GetX();
			anPositionArray[1] = GetY();
			anPositionArray[2] = GetZ();
		}

		inline void						SetX( Fixed32 nX ) { m_nX = nX; }
		inline void						SetY( Fixed32 nY ) { m_nY = nY; }
		inline void						SetZ( Fixed32 nZ ) { m_nZ = nZ; }
		inline void						SetW( Int32 nW ) { m_nW = nW; }

		inline void						SetFloatX( Float32 fX ) { m_nX = Float32ToFixed32( fX ); }
		inline void						SetFloatY( Float32 fY ) { m_nZ = Float32ToFixed32( fY ); }
		inline void						SetFloatZ( Float32 fZ ) { m_nY = Float32ToFixed32( fZ ); }

		inline void						Set( const CNavPosition & rcPosition )
		{
			SetX( rcPosition.GetX() );
			SetY( rcPosition.GetY() );
			SetZ( rcPosition.GetZ() );
			SetW( rcPosition.GetW() );
		}

		inline void						Set( Fixed32 nX, Fixed32 nY, Fixed32 nZ, Int32 nW = 0L )
		{
			SetX( nX );
			SetY( nY );
			SetZ( nZ );
			SetW( nW );
		}

		inline void						SetFloat( Float32 fX, Float32 fY, Float32 fZ, Int32 nW = 0L )
		{
			SetFloatX( fX );
			SetFloatY( fY );
			SetFloatZ( fZ );
			SetW( nW );
		}

		inline void						Reset( void )
		{
			SetX( 0L );
			SetY( 0L );
			SetZ( 0L );
			SetW( 0L );
		}

		inline bool						IsEqual( const CNavPosition & rcPosition ) const
		{
			if( GetX() != rcPosition.GetX() )
			{
				return false;
			}

			if( GetY() != rcPosition.GetY() )
			{
				return false;
			}

			if( GetZ() != rcPosition.GetZ() )
			{
				return false;
			}

			if( GetW() != rcPosition.GetW() )
			{
				return false;
			}

			return true;
		}

		inline bool						IsLargerThan( const CNavPosition & rcPosition ) const
		{
			if( m_nX <= rcPosition.GetX() )
			{
				return false;
			}

			if( m_nY <= rcPosition.GetY() )
			{
				return false;
			}

			if( m_nZ <= rcPosition.GetZ() )
			{
				return false;
			}

			return true;
		}

		inline bool						IsSmallerThan( const CNavPosition & rcPosition ) const
		{
			if( m_nX >= rcPosition.GetX() )
			{
				return false;
			}

			if( m_nY >= rcPosition.GetY() )
			{
				return false;
			}

			if( m_nZ >= rcPosition.GetZ() )
			{
				return false;
			}

			return true;
		}

		inline CNavPosition *			Clone( void ) const
		{
			return new CNavPosition( *this );
		}

		inline CNavPosition &			Add( const CNavPosition & rcPosition )
		{
			m_nX += rcPosition.GetX();
			m_nY += rcPosition.GetY();
			m_nZ += rcPosition.GetZ();

			return *this;
		}

		inline CNavPosition &			Sub( const CNavPosition & rcPosition )
		{
			m_nX -= rcPosition.GetX();
			m_nY -= rcPosition.GetY();
			m_nZ -= rcPosition.GetZ();

			return *this;
		}

		inline CNavPosition &			Mul( const CNavPosition & rcPosition )
		{
			m_nX *= rcPosition.GetX();
			m_nY *= rcPosition.GetY();
			m_nZ *= rcPosition.GetZ();

			return *this;
		}

		inline CNavPosition &			Mul( Fixed32 nScale )
		{
			m_nX *= nScale;
			m_nY *= nScale;
			m_nZ *= nScale;

			return *this;
		}

		inline CNavPosition &			MulFloat( Float32 fScale ) { return Mul( Float32ToFixed32( fScale ) ); }

		inline CNavPosition &			Scale( const CNavPosition & rcPosition )
		{
			m_nX = m_nX * rcPosition.GetX() / RATIO_FLOAT32_TO_FIXED32_I;
			m_nY = m_nY * rcPosition.GetY() / RATIO_FLOAT32_TO_FIXED32_I;
			m_nZ = m_nZ * rcPosition.GetZ() / RATIO_FLOAT32_TO_FIXED32_I;

			return *this;
		}

		inline CNavPosition &			Scale( Fixed32 nScale )
		{
			m_nX = m_nX * nScale / RATIO_FLOAT32_TO_FIXED32_I;
			m_nY = m_nY * nScale / RATIO_FLOAT32_TO_FIXED32_I;
			m_nZ = m_nZ * nScale / RATIO_FLOAT32_TO_FIXED32_I;

			return *this;
		}

		inline CNavPosition &			Div( const CNavPosition & rcPosition )
		{
			Assert( rcPosition.GetX() != 0L );
			Assert( rcPosition.GetY() != 0L );
			Assert( rcPosition.GetZ() != 0L );

			m_nX /= rcPosition.GetX();
			m_nY /= rcPosition.GetY();
			m_nZ /= rcPosition.GetZ();

			return *this;
		}

		inline CNavPosition &			Div( Fixed32 nScale )
		{
			Assert( nScale != 0L );

			m_nX /= nScale;
			m_nY /= nScale;
			m_nZ /= nScale;

			return *this;
		}

		inline CNavPosition &			DivFloat( Float32 fScale ) { return Div( Float32ToFixed32( fScale ) ); }

		inline Fixed32					GetDistance2D( void ) const { return SqrtUInt32( m_nX * m_nX + m_nY * m_nY ); }
		inline Fixed32					GetDistance( void ) const { return SqrtUInt32( m_nX * m_nX + m_nY * m_nY + m_nZ * m_nZ ); }

		inline Fixed32					GetDistance2DFrom( const CNavPosition & rcPosition ) const { CNavPosition cPosition( *this ); cPosition.Sub( rcPosition ); return cPosition.GetDistance2D(); }
		inline Fixed32					GetDistanceFrom( const CNavPosition & rcPosition ) const { CNavPosition cPosition( *this ); cPosition.Sub( rcPosition ); return cPosition.GetDistance(); }
};

//=============================================================================
// End of Class : CNavPosition
//=============================================================================

inline void CrossProduct( CNavPosition & rcResult, const CNavPosition & rcVector1, const CNavPosition & rcVector2 )
{
	rcResult.SetX( rcVector1.GetY() * rcVector2.GetZ() - rcVector1.GetZ() * rcVector2.GetY() );
	rcResult.SetY( rcVector1.GetZ() * rcVector2.GetX() - rcVector1.GetX() * rcVector2.GetZ() );
	rcResult.SetZ( rcVector1.GetX() * rcVector2.GetY() - rcVector1.GetY() * rcVector2.GetX() );
	rcResult.SetW( rcVector1.GetW() );

	return;
}

END_NAMESPACE( nav )

#endif //___C_NAV_POSITION_H___

//*****************************************************************************
//
// End of File : CNavPosition.h
//
//*****************************************************************************