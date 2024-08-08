//*****************************************************************************
// File       : NtlWeBase.h
// Desc       : 
// Author     : June C. Kim (cynis@hotmail.com)
// Update     : 2007. 1. 18
//*****************************************************************************

#ifndef ___NTL_WE_BASE_H___
#define ___NTL_WE_BASE_H___

//-----------------------------------------------------------------------------
// Pre-Compiler Option Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "NavExporter.h"


//-----------------------------------------------------------------------------
// Name Space Table
//-----------------------------------------------------------------------------

BEGIN_NAMESPACE( navex )


//-----------------------------------------------------------------------------
// Type Forward Definition Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Constant Table
//-----------------------------------------------------------------------------

#ifndef rwTEXTUREBASENAMELENGTH
	#define rwTEXTUREBASENAMELENGTH     ( 32 )
#endif //rwTEXTUREBASENAMELENGTH


#ifndef dNTL_SPLAT_LAYER_NUM
	#define dNTL_SPLAT_LAYER_NUM		( 5 )
#endif //dNTL_SPLAT_LAYER_NUM


#define PI								(3.141592654f)


//-----------------------------------------------------------------------------
// Type Definition Table
//-----------------------------------------------------------------------------

#ifndef RwChar
	typedef char		RwChar;		// TCHAR?
#endif RwChar;


#ifndef RwUInt8
	typedef BYTE		RwUInt8;
#endif RwUInt8;


#ifndef RwInt32
	typedef long		RwInt32;
#endif //RwInt32


#ifndef RwBool
	typedef RwInt32		RwBool;
#endif //RwBool


#ifndef RwReal
	typedef float		RwReal;
#endif //RwReal


#ifndef RwV3d
	typedef struct RwV3d
	{
		RwReal		x;
		RwReal		y;
		RwReal		z;
	} RwV3d;
#endif //RwV3d


#ifndef D3DXMATRIX
typedef struct D3DXMATRIX
{
	float			m[4][4];
} D3DXMATRIX;
#endif //D3DXMATRIX

/*
typedef struct NAVEXPORTER_API SNtlWeWorldInfo
{
	RwInt32		WorldHeightfieldNum;
	RwInt32		WorldSectorTileSize;
	RwInt32		WorldSectorTileNum;
	RwInt32		WorldSectorVertNum;
	RwInt32		WorldSectorSize;
	RwInt32		WorldSectorNum;
	RwInt32		WorldSectorHalfNum;
	RwInt32		WorldSectorPolyNum;
	RwInt32		WorldFieldSectorNum;
	RwInt32		WorldFieldSize;
	RwInt32		WorldFieldNum;
	RwInt32		WorldFieldHalfNum;
	RwInt32		WorldSize;
	RwInt32		WorldSizeHalf;
	RwInt32		WorldValueMax;
	RwInt32		WorldValueMin;
	RwInt32		WorldTexAddr;
	RwInt32		WorldPolyCnt;

	RwReal		WorldNearPlane;
	RwReal		WorldFarPlane;
	RwReal		WorldCamSpeedPerSec;

	void		Reset( void ) { ZeroMemory( this, sizeof( SNtlWeWorldInfo ) ); }
} SNtlWeWorldInfo;
*/

typedef struct NAVEXPORTER_API SNtlWeSectorInfo
{
	RwInt32		nSizeF;
	RwInt32		nSizeX;
	RwInt32		nSizeZ;
	RwV3d *		psVertexList;

	SNtlWeSectorInfo( void ) { ZeroMemory( this, sizeof( SNtlWeSectorInfo ) ); }
	~SNtlWeSectorInfo( void ) { Destroy(); }

	inline void		Destroy( void )
	{
		if( psVertexList != NULL )
		{
			delete[] psVertexList;
		}

		ZeroMemory( this, sizeof( SNtlWeSectorInfo ) );
	}

	inline void		Create( RwInt32 nNewSizeF, RwInt32 nNewSizeX, RwInt32 nNewSizeZ )
	{
		Destroy();

		nSizeF			= nNewSizeF;
		nSizeX			= nNewSizeX;
		nSizeZ			= nNewSizeZ;
		psVertexList	= new RwV3d[nSizeF * nSizeX * nSizeZ];
	}

	inline RwV3d *	GetField( RwInt32 nIndexF )
	{
		Assert( psVertexList != NULL );
		Assert( nIndexF >= 0 );
		Assert( nIndexF < nSizeF );

		return &psVertexList[nSizeX * nSizeZ * nIndexF];
	}


	inline const RwV3d *	GetVertex( RwInt32 nIndexF, RwInt32 nIndexX, RwInt32 nIndexZ ) const
	{
		Assert( psVertexList != NULL );
		Assert( nIndexF >= 0 );
		Assert( nIndexF < nSizeF );
		Assert( nIndexX >= 0 );
		Assert( nIndexX < nSizeX );
		Assert( nIndexZ >= 0 );
		Assert( nIndexZ < nSizeZ );

		return &psVertexList[nSizeX * nSizeZ * nIndexF + nSizeX * nIndexZ + nIndexX];
	}

	inline const void	SetVertex( RwInt32 nIndexF, RwInt32 nIndexX, RwInt32 nIndexZ, const RwV3d * psVertex )
	{
		Assert( psVertexList != NULL );
		Assert( nIndexF >= 0 );
		Assert( nIndexF < nSizeF );
		Assert( nIndexX >= 0 );
		Assert( nIndexX < nSizeX );
		Assert( nIndexZ >= 0 );
		Assert( nIndexZ < nSizeZ );

		memcpy( &psVertexList[nSizeX * nSizeZ * nIndexF + nSizeX * nIndexZ + nIndexX], psVertex, sizeof( RwV3d ) );
	}
} SNtlWeSectorInfo;


typedef struct NAVEXPORTER_API SNtlWeSectorExportReference
{
	RwV3d		sPosition;
	RwV3d		sSize;
	bool		bUse;
} SNtlWeSectorExportReference;


//-----------------------------------------------------------------------------
// Macro, Inline & Template Function Table
//-----------------------------------------------------------------------------

END_NAMESPACE( navex )

#endif //___NTL_WE_BASE_H___

//*****************************************************************************
//
// End of File : NtlWeBase.h
//
//*****************************************************************************