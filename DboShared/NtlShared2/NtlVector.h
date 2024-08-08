//***********************************************************************************
//
//	File		:	NtlVector.h
//
//	Begin		:	2006-03-03
//
//	Copyright	:	¨Ï NTL-Inc Co., Ltd
//
//	Author		:	Hyun Woo, Koo   ( zeroera@ntl-inc.com )
//
//	Desc		:	Basic Vector Class
//
//***********************************************************************************

#pragma once

#pragma warning(disable : 4201)

#include "NtlSharedType.h"

#include <float.h>
#include <math.h>
#include <crtdbg.h>


//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
#define FLOAT_EPSILON 0.0001
#define FLOAT_EQ(a, b)	( ( ( b - FLOAT_EPSILON) < a ) && ( a < ( b + FLOAT_EPSILON ) ) )
//-----------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
const float MAX_REAL = FLT_MAX;
const float ZERO_TOLERANCE = 1e-06f;
//-----------------------------------------------------------------------------------


struct sVECTOR2
{
	float x;
	float z;
};

struct sVECTOR3
{
	float x;
	float y;
	float z;
};


class CNtlVector
{
public:

	CNtlVector();

	CNtlVector(sVECTOR3 & rVector);

	CNtlVector(sVECTOR2 & rVector);

	CNtlVector(float _x, float _y, float _z);

	CNtlVector (const float* _afTuple);

	CNtlVector (const CNtlVector& rhs);


public:

	void			CopyFrom(float _x, float _y, float _z);

	void			CopyFrom(sVECTOR3 * pVector);

	void			CopyFrom(sVECTOR3 & rVector);

	void			CopyFrom(sVECTOR2 * pVector);

	void			CopyFrom(sVECTOR2 & rVector);


	void			CopyTo(float & rX, float &rY, float &rZ);

	void			CopyTo(sVECTOR3 * pVector);

	void			CopyTo(sVECTOR2 * pVector);


	void			CopyTo(sVECTOR3 & rVector);

	void			CopyTo(sVECTOR2 & rVector);



public:


	CNtlVector& operator= (const CNtlVector& rhs);
	CNtlVector& operator= (const sVECTOR3& rhs);
	CNtlVector& operator= (const sVECTOR2& rhs);

	bool operator== (const CNtlVector& rhs) const;
	bool operator== (const sVECTOR3& rhs) const;
	bool operator!= (const CNtlVector& rhs) const;
	bool operator<  (const CNtlVector& rhs) const;
	bool operator<= (const CNtlVector& rhs) const;
	bool operator>  (const CNtlVector& rhs) const;
	bool operator>= (const CNtlVector& rhs) const;


	CNtlVector operator+ (const CNtlVector& rhs) const;
	CNtlVector operator- (const CNtlVector& rhs) const;
	CNtlVector operator+ (float fScalar) const;
	CNtlVector operator- (float fScalar) const;
	CNtlVector operator* (float fScalar) const;
	CNtlVector operator/ (float fScalar) const;
	CNtlVector operator- () const;

	CNtlVector& operator+= (const CNtlVector& rhs);
	CNtlVector& operator-= (const CNtlVector& rhs);
	CNtlVector& operator+= (float fScalar);
	CNtlVector& operator-= (float fScalar);
	CNtlVector& operator*= (float fScalar);
	CNtlVector& operator/= (float fScalar);


	void				Reset();

	float				Length () const;
	float				SquaredLength () const;
	float				Dot (const CNtlVector& rhs) const;
	float				Normalize ();
	bool				SafeNormalize();
	float				Normalize (float fLength);

	CNtlVector			Cross (const CNtlVector& rhs) const;
	CNtlVector			UnitCross (const CNtlVector& rhs) const;

	bool				IsZero() const;
	bool				IsEqual(CNtlVector &rhs) const;
	bool				IsInvalid(bool bCheckY = true) const;



public:

	union {
		struct {
			float		x;
			float		y;
			float		z;
		};
		float			afTuple[3];		
	};


public:

	// special vectors
	static CNtlVector				ZERO;
	static CNtlVector				UNIT_X;
	static CNtlVector				UNIT_Y;
	static CNtlVector				UNIT_Z;
	static CNtlVector				INVALID;
	static CNtlVector				INVALID_XZ;

};


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
inline CNtlVector::CNtlVector()
:x(0.0f), y(0.0f), z(0.0f)
{
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
inline CNtlVector::CNtlVector(sVECTOR3 & rVector)
:x(rVector.x), y(rVector.y), z(rVector.z)
{
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
inline CNtlVector::CNtlVector(sVECTOR2 & rVector)
:x(rVector.x), y(0.0f), z(rVector.z)
{
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
inline CNtlVector::CNtlVector(float _x, float _y, float _z)
:x(_x), y(_y), z(_z)
{
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
inline CNtlVector::CNtlVector(const float* _afTuple)
{
	afTuple[0] = _afTuple[0];
	afTuple[1] = _afTuple[1];
	afTuple[2] = _afTuple[2];
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
inline CNtlVector::CNtlVector(const CNtlVector& rhs)
:x(rhs.x), y(rhs.y), z(rhs.z)
{
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
inline void CNtlVector::CopyFrom(float _x, float _y, float _z)
{
	x = _x;
	y = _y;
	z = _z;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
inline void CNtlVector::CopyFrom(sVECTOR3 * pVector)
{
	x = pVector->x;
	y = pVector->y;
	z = pVector->z;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
inline void CNtlVector::CopyFrom(sVECTOR3 & rVector)
{
	x = rVector.x;
	y = rVector.y;
	z = rVector.z;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
inline void CNtlVector::CopyFrom(sVECTOR2 * pVector)
{
	x = pVector->x;
	y = 0.0f;
	z = pVector->z;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
inline void CNtlVector::CopyFrom(sVECTOR2 & rVector)
{
	x = rVector.x;
	y = 0.0f;
	z = rVector.z;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
inline void CNtlVector::CopyTo(float & rX, float & rY, float &rZ)
{
	rX = x;
	rY = y;
	rZ = z;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
inline void CNtlVector::CopyTo(sVECTOR3 * pVector)
{
	pVector->x = x;
	pVector->y = y;
	pVector->z = z;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
inline void CNtlVector::CopyTo(sVECTOR2 * pVector)
{
	pVector->x = x;
	pVector->z = z;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
inline void CNtlVector::CopyTo(sVECTOR3 & rVector)
{
	rVector.x = x;
	rVector.y = y;
	rVector.z = z;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
inline void CNtlVector::CopyTo(sVECTOR2 & rVector)
{
	rVector.x = x;
	rVector.z = z;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
inline CNtlVector& CNtlVector::operator= (const CNtlVector& rhs)
{
	x = rhs.x;
	y = rhs.y;
	z = rhs.z;

	return *this;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
inline CNtlVector& CNtlVector::operator= (const sVECTOR3& rhs)
{
	x = rhs.x;
	y = rhs.y;
	z = rhs.z;

	return *this;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
inline CNtlVector& CNtlVector::operator= (const sVECTOR2& rhs)
{
	x = rhs.x;
	y = 0.0f;
	z = rhs.z;

	return *this;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
inline bool CNtlVector::operator== (const CNtlVector& rhs) const
{
	return memcmp( afTuple, rhs.afTuple, sizeof(afTuple) ) == 0;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
inline bool CNtlVector::operator== (const sVECTOR3& rhs) const
{
	return memcmp( afTuple, &rhs, sizeof(afTuple) ) == 0;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
inline bool CNtlVector::operator!= (const CNtlVector& rhs) const
{
	return memcmp( afTuple, rhs.afTuple, sizeof(afTuple) ) != 0;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
inline bool CNtlVector::operator<  (const CNtlVector& rhs) const
{
	return memcmp( afTuple, rhs.afTuple, sizeof(afTuple) ) < 0;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
inline bool CNtlVector::operator<= (const CNtlVector& rhs) const
{
	return memcmp( afTuple, rhs.afTuple, sizeof(afTuple) ) <= 0;
}

//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
inline bool CNtlVector::operator>  (const CNtlVector& rhs) const
{
	return memcmp( afTuple, rhs.afTuple, sizeof(afTuple) ) > 0;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
inline bool CNtlVector::operator>= (const CNtlVector& rhs) const
{
	return memcmp( afTuple, rhs.afTuple, sizeof(afTuple) ) >= 0;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
inline CNtlVector CNtlVector::operator+ (const CNtlVector& rhs) const
{
	return CNtlVector( x + rhs.x , y + rhs.y, z + rhs.z );
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
inline CNtlVector CNtlVector::operator- (const CNtlVector& rhs) const
{
	return CNtlVector( x - rhs.x , y - rhs.y, z - rhs.z );
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
inline CNtlVector CNtlVector::operator+ (float fScalar) const
{
	return CNtlVector( x + fScalar , y + fScalar, z + fScalar );
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
inline CNtlVector CNtlVector::operator- (float fScalar) const
{
	return CNtlVector( x - fScalar , y - fScalar, z - fScalar );
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
inline CNtlVector CNtlVector::operator* (float fScalar) const
{
	return CNtlVector( x * fScalar , y * fScalar, z * fScalar );
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
inline CNtlVector CNtlVector::operator/ (float fScalar) const
{
	return CNtlVector( x / fScalar , y / fScalar, z / fScalar );
}

//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
inline CNtlVector CNtlVector::operator- () const
{
	return CNtlVector( -x , -y, -z );
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
inline CNtlVector& CNtlVector::operator+= (const CNtlVector& rhs)
{
	x += rhs.x;
	y += rhs.y;
	z += rhs.z;

	return *this;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
inline CNtlVector& CNtlVector::operator-= (const CNtlVector& rhs)
{
	x -= rhs.x;
	y -= rhs.y;
	z -= rhs.z;

	return *this;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
inline CNtlVector& CNtlVector::operator+= (float fScalar)
{
	x += fScalar;
	y += fScalar;
	z += fScalar;

	return *this;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
inline CNtlVector& CNtlVector::operator-= (float fScalar)
{
	x -= fScalar;
	y -= fScalar;
	z -= fScalar;

	return *this;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
inline CNtlVector& CNtlVector::operator*= (float fScalar)
{
	x *= fScalar;
	y *= fScalar;
	z *= fScalar;

	return *this;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
inline CNtlVector& CNtlVector::operator/= (float fScalar)
{
	if( fScalar != (float) 0.0f ) 
	{
		float fInvScalar = ((float)1.0f) / fScalar;

		x *= fInvScalar;
		y *= fInvScalar;
		z *= fInvScalar;
	}
	else
	{
		x = MAX_REAL;
		y = MAX_REAL;
		z = MAX_REAL;
	}


	return *this;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
inline void CNtlVector::Reset()
{
	x = y = z = 0.0f;
}

//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
inline float CNtlVector::Length () const
{
	return ::sqrt( afTuple[0] * afTuple[0] + afTuple[1] * afTuple[1] + afTuple[2] * afTuple[2] );
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
inline float CNtlVector::SquaredLength () const
{
	return afTuple[0] * afTuple[0] + afTuple[1] * afTuple[1] + afTuple[2] * afTuple[2];

}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
inline float CNtlVector::Dot (const CNtlVector& rhs) const
{
	return afTuple[0] * rhs.afTuple[0] + afTuple[1] * rhs.afTuple[1] + afTuple[2] * rhs.afTuple[2];
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
inline float CNtlVector::Normalize ()
{
	float fLength = Length();

	return Normalize( fLength );
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
inline bool CNtlVector::SafeNormalize()
{
	float fLength = Length();

	if (ZERO_TOLERANCE > fLength)
	{
		return false;
	}

	Normalize(fLength);

	return true;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
inline float CNtlVector::Normalize(float fLength)
{
	if ( fLength > ZERO_TOLERANCE )
	{
		float fInvLength = ( 1.0f ) / fLength;

		afTuple[0] *= fInvLength;
		afTuple[1] *= fInvLength;
		afTuple[2] *= fInvLength;
	}
	else
	{
		fLength = 0.0f;
		afTuple[0] = 0.0f;
		afTuple[1] = 0.0f;
		afTuple[2] = 0.0f;
	}


	if( Length() < 0.8f )
	{
		//_ASSERT( 0 );
	}


	return fLength;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
inline CNtlVector CNtlVector::Cross (const CNtlVector& rhs) const
{
	return CNtlVector(	afTuple[1]*rhs.afTuple[2] - afTuple[2]*rhs.afTuple[1],
						afTuple[2]*rhs.afTuple[0] - afTuple[0]*rhs.afTuple[2],
						afTuple[0]*rhs.afTuple[1] - afTuple[1]*rhs.afTuple[0] );

}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
inline CNtlVector CNtlVector::UnitCross (const CNtlVector& rhs) const
{
	CNtlVector kCross(	afTuple[1]*rhs.afTuple[2] - afTuple[2]*rhs.afTuple[1],
						afTuple[2]*rhs.afTuple[0] - afTuple[0]*rhs.afTuple[2],
						afTuple[0]*rhs.afTuple[1] - afTuple[1]*rhs.afTuple[0]);

	kCross.Normalize();

	return kCross;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
inline bool CNtlVector::IsZero() const
{
	if( afTuple[0] || afTuple[1] || afTuple[2] )
	{
		return false;
	}

	return true;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
inline bool CNtlVector::IsInvalid(bool bCheckY) const
{
	if( false == bCheckY )
	{
		if( INVALID_FLOAT == afTuple[0] && INVALID_FLOAT == afTuple[2] )
		{
			return true;
		}
	}
	else
	{
		if( INVALID_FLOAT == afTuple[0] && INVALID_FLOAT == afTuple[1] && INVALID_FLOAT == afTuple[2] )
		{
			return true;
		}
	}

	return false;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
inline bool CNtlVector::IsEqual(CNtlVector &rhs) const
{
	if( FLOAT_EQ( afTuple[ 0 ], rhs.afTuple[ 0 ] ) &&
		FLOAT_EQ( afTuple[ 1 ], rhs.afTuple[ 1 ] ) &&
		FLOAT_EQ( afTuple[ 2 ], rhs.afTuple[ 2 ] ) )
	{
		return true;
	}

	return false;
}