//*****************************************************************************
// File       : NavBase.h
// Desc       : 
// Begin      :                                                          ()_()
// Copyright  :                                                          (o.o)
// Author     :                                                          (>.<)
// Update     : 
//*****************************************************************************

#ifndef ___NAV_BASE_H___
#define ___NAV_BASE_H___

//-----------------------------------------------------------------------------
// Pre-Compiler Option Table
//-----------------------------------------------------------------------------

#define __FAST_SQUARE_ROOT__					// Use Fast Integer Square Root Algorithm instead of sqrt()
#define __PATH_ENGINE_DLL__						// Use Path Engine DLL instead of static library
#define __PATH_ENGINE_TOK__						// Use TOK(Binary) Data Type instead of XML(Text) Data Type
#define __CHECK_PERFORMANCE__					// Check Performance


//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "BaseType.h"
//USE_NAMESPACE( bas )

#include "UserDefinedAssert.h"
#include "CLogMessageManager.h"
#include "CFixedString.h"
#include "CFixedStringList.h"
//USE_NAMESPACE( str )

#include <math.h>

#pragma warning(disable : 4100)
#include "i_pathEngine.h"
#pragma warning(default : 4100)


//-----------------------------------------------------------------------------
// Name Space Table
//-----------------------------------------------------------------------------

BEGIN_NAMESPACE( nav )


//-----------------------------------------------------------------------------
// Type Forward Definition Table
//-----------------------------------------------------------------------------

typedef UInt32					UId32;			// Unique ID
typedef signed long				Fixed32;		// Fixed-Point Real Number


//-----------------------------------------------------------------------------
// Constant Table
//-----------------------------------------------------------------------------

#define RATIO_FLOAT32_TO_FIXED32		( 100.0f )
#define RATIO_FIXED32_TO_FLOAT32		( 0.01f )			// = ( 1.0f / RATIO_FLOAT32_TO_FIXED32 )

#define RATIO_FLOAT32_TO_FIXED32_I		( 100L )			// = static_cast<Int32>(RATIO_FLOAT32_TO_FIXED32)

#define MAX_VAL_INT_8					( +127 )
#define MIN_VAL_INT_8					( -128 )
#define MAX_VAL_UINT_8					( +255 )

#define MAX_VAL_INT_16					( +32767 )
#define MIN_VAL_INT_16					( -32768 )
#define MAX_VAL_UINT_16					( +65535 )

#define MAX_VAL_INT_32					( +2147483647 )
#define MIN_VAL_INT_32					( -2147483647 )		// Not Use -2147483648 because of unary operation '-' problem (C4146)
#define MAX_VAL_UINT_32					( +4294967295 )

#define MAX_VAL_FIXED_32				MAX_VAL_INT_32
#define MIN_VAL_FIXED_32				MIN_VAL_INT_32

//#define SEARCH_FAIL					( 0xFFFFFFFF )
//#define INVALID_ID					SEARCH_FAIL
//#define INVALID_POSITION				SEARCH_FAIL
#define INVALID_ID						0xFFFFFFFF
#define INVALID_POSITION				0xFFFFFFFF


//-----------------------------------------------------------------------------
// Type Definition Table
//-----------------------------------------------------------------------------

typedef enum ENavEntityType
{
	E_NAV_ENTITY_TYPE_NONE				= 0x00,
	E_NAV_ENTITY_TYPE_NAVIGATOR			= 0x01,
	E_NAV_ENTITY_TYPE_WORLD				= 0x02,
	E_NAV_ENTITY_TYPE_ZONE				= 0x03,
	E_NAV_ENTITY_TYPE_SECTOR			= 0x04,
	E_NAV_ENTITY_TYPE_AREA				= 0x05,
	E_NAV_ENTITY_TYPE_SECTION			= 0x06,
	E_NAV_ENTITY_TYPE_TERRAIN			= 0x07,
	E_NAV_ENTITY_TYPE_HEIGHT_MAP		= 0x08,
	E_NAV_ENTITY_TYPE_PORTAL			= 0x09,
	E_NAV_ENTITY_TYPE_OBJECT			= 0x0A,
	E_NAV_ENTITY_TYPE_AGENT				= 0x0B,
	E_NAV_ENTITY_TYPE_PATH				= 0x0C,
	E_NAV_ENTITY_TYPE_COUNT
} ENavEntityType;


typedef enum ENavCreateType
{
	E_NAV_CREATE_TYPE_NONE,
	E_NAV_CREATE_TYPE_FILE,
	E_NAV_CREATE_TYPE_MEMORY,
	E_NAV_CREATE_TYPE_INSTANCE,
	E_NAV_CREATE_TYPE_EMPTY,
	E_NAV_CREATE_TYPE_COUNT
} ENavCreateType;


typedef enum ENavPathState
{
	E_NAV_PATH_STATE_NONE					= 0,
	E_NAV_PATH_STATE_PRE_PROCESS,
	E_NAV_PATH_STATE_READY_TO_FIND,
	E_NAV_PATH_STATE_IN_PROCESS,
	E_NAV_PATH_STATE_INVALID_PARAMETER,
	E_NAV_PATH_STATE_INVALID_DIRECT_PATH,
	E_NAV_PATH_STATE_INVALID_PATH,
	E_NAV_PATH_STATE_VALID_PATH,
	E_NAV_PATH_STATE_COUNT
} ENavPathState;


typedef enum ENavPathError
{
	E_NAV_PATH_ERROR_NONE							= 0,
	E_NAV_PATH_ERROR_NO_MATCH_WORLD					= 1,
	E_NAV_PATH_ERROR_NO_MATCH_ZONE					= 2,
	E_NAV_PATH_ERROR_NO_MATCH_SECTOR				= 3,
	E_NAV_PATH_ERROR_NO_TERRAIN						= 4,
	E_NAV_PATH_ERROR_NO_MESH						= 5,
	E_NAV_PATH_ERROR_NO_SOURCE_POSITION				= 6,
	E_NAV_PATH_ERROR_OBSTRUCT_SOURCE_POSITION		= 7,
	E_NAV_PATH_ERROR_NO_DESTINATION_POSITION		= 8,
	E_NAV_PATH_ERROR_OBSTRUCT_DESTINATION_POSITION	= 9,
	E_NAV_PATH_ERROR_NO_PATH						= 10,
	E_NAV_PATH_ERROR_NO_MATCH_SHAPE					= 11,
	E_NAV_PATH_ERROR_IN_PROCESS						= 12,
	E_NAV_PATH_ERROR_COUNT
} ENavPathError;


#define E_NAV_TILE_ATTRIBUTE_NORMAL					( 0x00000000 )
#define E_NAV_TILE_ATTRIBUTE_PC_UNMOVABLE			( 0x00000001 )
#define E_NAV_TILE_ATTRIBUTE_MOB_UNMOVABLE			( 0x00000002 )
#define E_NAV_TILE_ATTRIBUTE_ALL_UNMOVABLE			( 0x00000004 )
#define E_NAV_TILE_ATTRIBUTE_NON_PVP				( 0x00000008 )
#define E_NAV_TILE_ATTRIBUTE_NON_BATTLE				( 0x00000010 )
#define E_NAV_TILE_ATTRIBUTE_WATER					( 0x00000020 )
#define E_NAV_TILE_ATTRIBUTE_NONE					( 0xFFFFFFFF )


typedef enum ENavFileType
{
	E_NAV_FILE_TYPE_NONE				= 0,
	E_NAV_FILE_TYPE_XML					= 1,
	E_NAV_FILE_TYPE_TOK					= 2,
	E_NAV_FILE_TYPE_CPP					= 3,
	E_NAV_FILE_TYPE_PPP					= 4,
	E_NAV_FILE_TYPE_VFM					= 5,
	E_NAV_FILE_TYPE_COUNT
} ENavFileType;


typedef enum ENavMemoryType
{
	E_NAV_MEMORY_TYPE_NONE				= 0,
	E_NAV_MEMORY_TYPE_INSTANCE			= 1,
	E_NAV_MEMORY_TYPE_RESOURCE			= 2,
	E_NAV_MEMORY_TYPE_TYPE_COUNT
} ENavMemoryType;


typedef enum ENavNavigatorType
{
	E_NAV_NAVIGATOR_TYPE_NONE,
	E_NAV_NAVIGATOR_TYPE_CLIENT			= 0x00000001,
	E_NAV_NAVIGATOR_TYPE_SERVER			= 0x00000002,
	E_NAV_NAVIGATOR_TYPE_COUNT
} ENavNavigatorType;


typedef enum ENavWorldType
{
	E_NAV_WORLD_TYPE_NONE,
	E_NAV_WORLD_TYPE_INDOOR				= 0x00000001,
	E_NAV_WORLD_TYPE_OUTDOOR			= 0x00000002,
	E_NAV_WORLD_TYPE_COUNT
} ENavWorldType;


typedef enum ENavZoneType
{
	E_NAV_ZONE_TYPE_NONE,
	E_NAV_ZONE_TYPE_COUNT
} ENavZoneType;


typedef enum ENavSectorType
{
	E_NAV_SECTOR_TYPE_NONE,
	E_NAV_SECTOR_TYPE_FIELD				= 0x00000001,
	E_NAV_SECTOR_TYPE_TOWN				= 0x00000002,
	E_NAV_SECTOR_TYPE_DUNGEON			= 0x00000003,
	E_NAV_SECTOR_TYPE_COUNT
} ENavSectorType;


typedef enum ENavTerrainType
{
	E_NAV_TERRAIN_TYPE_NONE,
	E_NAV_TERRAIN_TYPE_WORLD,
	E_NAV_TERRAIN_TYPE_ZONE,
	E_NAV_TERRAIN_TYPE_SECTOR,
	E_NAV_TERRAIN_TYPE_COUNT
} ENavTerrainType;


typedef enum ENavPortalType
{
	E_NAV_PORTAL_TYPE_NONE,
	E_NAV_PORTAL_TYPE_FIELD				= 0x00000001,
	E_NAV_PORTAL_TYPE_DUNGEON			= 0x00000003,
	E_NAV_PORTAL_TYPE_COUNT
} ENavPortalType;


typedef enum ENavObjectType
{
	E_NAV_OBJECT_TYPE_NONE,
	E_NAV_OBJECT_TYPE_OBSTACLE			= 0x00000001,
	E_NAV_OBJECT_TYPE_PASSABLE			= 0x00000002,
	E_NAV_OBJECT_TYPE_COUNT
} ENavObjectType;


typedef enum ENavHeightMapType
{
	E_NAV_HEIGHT_MAP_TYPE_NONE,
	E_NAV_HEIGHT_MAP_TYPE_NORMAL		= 0x00000001,
	E_NAV_HEIGHT_MAP_TYPE_CUSTOM		= 0x00000002,
	E_NAV_HEIGHT_MAP_TYPE_COUNT
} ENavHeightMapType;


typedef union UNavEntitySubType
{
	ENavNavigatorType		eNavigatorType;
	ENavWorldType			eWorldType;
	ENavZoneType			eZoneType;
	ENavSectorType			eSectionType;
	ENavSectorType			eAreaType;
	ENavSectorType			eSectorType;
	ENavTerrainType			eTerrainType;
	ENavHeightMapType		eHeightMapType;
	ENavPortalType			ePortalType;
	ENavObjectType			eObjectType;

	UInt32					nValue;
} UNavEntitySubType;


typedef struct SNavSlope
{
	Fixed32						nX;
	Fixed32						nY;

	inline						SNavSlope( void ) : nX( 0 ), nY( 0 ) {}
	inline						SNavSlope( Fixed32 nDefaultX, Fixed32 nDefaultY ) : nX( nDefaultX ), nY( nDefaultY ) {}
	inline						SNavSlope( const SNavSlope &rsSlope ) : nX( rsSlope.GetX() ), nY( rsSlope.GetY() ) {}

	inline void					Set( Fixed32 nNewX, Fixed32 nNewY ) { nX = nNewX; nY = nNewY; }
	inline void					Set( const SNavSlope &rsSlope ) { nX = rsSlope.GetX(); nY = rsSlope.GetY(); }

	inline Fixed32				GetX( void ) const { return nX; }
	inline Fixed32				GetY( void ) const { return nY; }

	inline Float32				GetSlope( void ) const
	{
		Fixed32		nSquaredA = nX * nX + nY * nY;
		Fixed32		nSquaredB = RATIO_FLOAT32_TO_FIXED32_I * RATIO_FLOAT32_TO_FIXED32_I;

		return static_cast<Float32>(nSquaredA) / static_cast<Float32>(nSquaredB);
	}

	inline bool					IsFiltered( Float32 fThresholdSlope ) const { return ( ( GetSlope() > fThresholdSlope ) ? true : false ); }
} SNavSlope;


//-----------------------------------------------------------------------------
// Macro, Inline & Template Function Table
//-----------------------------------------------------------------------------

#ifndef Float32ToFixed32
inline Fixed32 Float32ToFixed32( Float32 fVal )
{
	return static_cast<Fixed32>(fVal * RATIO_FLOAT32_TO_FIXED32);
}
#endif //Float32ToFixed32


#ifndef Fixed32ToFloat32
inline Float32 Fixed32ToFloat32( Fixed32 nVal )
{
	return static_cast<Float32>(nVal * RATIO_FIXED32_TO_FLOAT32);
}
#endif //Float32ToFixed32


#ifndef GetFixed32Ratio
inline Fixed32 GetFixed32Ratio( Fixed32 nDividend, Fixed32 nDivisor )
{
	return nDividend * RATIO_FLOAT32_TO_FIXED32_I / nDivisor;
}
#endif //GetFixed32Ratio


//-----------------------------------------------------------------------------
// Name			: SqrtUInt32
// Desc			: Fast Square Root for Integer
//                (Modified from Paul Hsieh, http://www.azillionmonkeys.com/qed/sqroot.html)
// Privilege	: global
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

#ifdef __FAST_SQUARE_ROOT__
	inline UInt32 SqrtUInt32( UInt32 nValue )
	{
		register UInt32		nTemp, nResult;

		if( nValue >= 0x40000000 )
		{
			nResult = 0x8000; 
			nValue -= 0x40000000;
		}
		else
		{
			nResult = 0;
		}

	#define SQRT_INT_PART( nVAL )			\
		nTemp = (nResult << nVAL);			\
		nTemp += (1 << ((--nVAL) << 1));	\
		if( nValue >= nTemp )				\
		{									\
			nResult += (1 << nVAL);			\
			nValue -= nTemp;				\
		}

		register UInt8	nVAL = 15;
		SQRT_INT_PART( nVAL )
		SQRT_INT_PART( nVAL )
		SQRT_INT_PART( nVAL )
		SQRT_INT_PART( nVAL )
		SQRT_INT_PART( nVAL )
		SQRT_INT_PART( nVAL )
		SQRT_INT_PART( nVAL )
		SQRT_INT_PART( nVAL )
		SQRT_INT_PART( nVAL )
		SQRT_INT_PART( nVAL )
		SQRT_INT_PART( nVAL )
		SQRT_INT_PART( nVAL )
		SQRT_INT_PART( nVAL )
		SQRT_INT_PART( nVAL )
	#undef SQRT_INT_PART

		nTemp = ( nResult << 1 );
		if( nValue >= ++nTemp )
		{
			++nResult;
		}

		return nResult;
	}
#else //__FAST_SQUARE_ROOT__
	#define SqrtUInt32( nValue )		( static_cast<UInt32>(sqrtf( static_cast<Float32>(nValue) )) )
#endif //__FAST_SQUARE_ROOT__


//-----------------------------------------------------------------------------
// Name			: SqrtFixed32
// Desc			: Fixed32 Square Root
// Privilege	: global
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

inline Fixed32 SqrtFixed32( Fixed32 nValue )
{
	return static_cast<Fixed32>(SqrtUInt32( nValue * RATIO_FLOAT32_TO_FIXED32_I ));
}


END_NAMESPACE( nav )

#endif //___NAV_BASE_H___

//*****************************************************************************
//
// End of File : NavBase.h
//
//*****************************************************************************