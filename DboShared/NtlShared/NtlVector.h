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


#include <float.h>
#include <math.h>

const float MAX_REAL = FLT_MAX;
const float ZERO_TOLERANCE = 1e-06f;

class CNtlVector
{
public:

	CNtlVector();

	CNtlVector(float _x, float _y, float _z);

	CNtlVector (const float* _afTuple);

	CNtlVector (const CNtlVector& rhs);


public:


	CNtlVector& operator= (const CNtlVector& rhs);


	bool operator== (const CNtlVector& rhs) const;
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
	CNtlVector& operator*= (float fScalar);
	CNtlVector& operator/= (float fScalar);


	void				Reset();

	float				Length () const;
	float				SquaredLength () const;
	float				Dot (const CNtlVector& rhs) const;
	float				Normalize ();

	CNtlVector			Cross (const CNtlVector& rhs) const;
	CNtlVector			UnitCross (const CNtlVector& rhs) const;



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
inline bool CNtlVector::operator== (const CNtlVector& rhs) const
{
	return memcmp( afTuple, rhs.afTuple, sizeof(afTuple) ) == 0;
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

	if ( fLength > ZERO_TOLERANCE )
	{
		float fInvLength = ((float)1.0) / fLength;

		afTuple[0] *= fInvLength;
		afTuple[1] *= fInvLength;
		afTuple[2] *= fInvLength;
	}
	else
	{
		fLength = (float)0.0;
		afTuple[0] = (float)0.0;
		afTuple[1] = (float)0.0;
		afTuple[2] = (float)0.0;
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