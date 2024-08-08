//*****************************************************************************
// File       : NavEntityBase.h
// Desc       : 
// Author     : June C. Kim (cynis@hotmail.com)
// Update     : 2007. 1. 18
//*****************************************************************************

#ifndef ___NAV_ENTITY_BASE_H___
#define ___NAV_ENTITY_BASE_H___

//-----------------------------------------------------------------------------
// Pre-Compiler Option Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "NavBase.h"
#include "CNavStringTable.h"

#include "CNavPosition.h"
#include "CNavBounding.h"
#include "CNavPath.h"
#include "CBaseThreadManager.h"


//-----------------------------------------------------------------------------
// Name Space Table
//-----------------------------------------------------------------------------

BEGIN_NAMESPACE( nav )


//-----------------------------------------------------------------------------
// Type Forward Definition Table
//-----------------------------------------------------------------------------

class CNavConverter;
class CNavRenderer;

class CNavEntity;


//-----------------------------------------------------------------------------
// Constant Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Type Definition Table
//-----------------------------------------------------------------------------


//=============================================================================
// Name       : SNavEntityResultInfo
// Desc       : 
// Author     : 
//=============================================================================

typedef struct SNavEntityResultInfo
{
	UInt32					nSuccess;
	UInt32					nFailure;
	UInt32					nSkip;
	UInt32					nError;

public:

	inline					SNavEntityResultInfo( void ) : nSuccess( 0 ), nFailure( 0 ), nSkip( 0 ), nError( 0 ) {}

	inline void				Success( UInt32 nCount = 1 ) { nSuccess += nCount; }
	inline void				Failure( UInt32 nCount = 1 ) { nFailure += nCount; }
	inline void				Skip( UInt32 nCount = 1 ) { nSkip += nCount; }
	inline void				Error( UInt32 nCount = 1 ) { nError += nCount; }

	inline void				Reset( void ) { nSuccess = 0; nFailure = 0; nSkip = 0; nError = 0; }
} SNavEntityResultInfo;

//=============================================================================
// End of Struct : SNavEntityResultInfo
//=============================================================================


//=============================================================================
// Name       : SNavEntityExternalProcess
// Desc       : 
// Author     : 
//=============================================================================

typedef struct SNavEntityExternalProcess
{
	CNavConverter *			pcImporter;
	CNavConverter *			pcExporter;
	CNavRenderer *			pcRenderer;

public:

	inline					SNavEntityExternalProcess( void ) : pcImporter( NULL ), pcExporter( NULL ), pcRenderer( NULL ) {}

	// Log
	// Error Handler
	// Monitor ...
} SNavEntityExternalProcess;

//=============================================================================
// End of Struct : SNavEntityExternalProcess
//=============================================================================


//=============================================================================
// Name       : SNavEntityCreateInfo
// Desc       : 
// Author     : 
//=============================================================================

typedef struct SNavEntityCreateInfo
{
	UId32					nUniqueId;
	ENavEntityType			eEntityType;
	ENavCreateType			eCreateType;
	UNavEntitySubType		uSubType;
	CNavEntity *			pcParent;
	UInt32					nFlags;
	bool					bExport;
	CFixedString			cName;
	CFixedString			cPath;
	CFixedString			cTitle;
	CNavPosition			cPosition;
	CNavBounding			cBounding;
	UInt32					nUserDataSize;
	void *					pvUserData;

	CBaseThreadManager *	pcThreadManager;

	SNavEntityResultInfo *	psResultInfo;

	inline					SNavEntityCreateInfo( void ) { Reset(); }
	inline					SNavEntityCreateInfo( const SNavEntityCreateInfo & rsCreateInfo ) { CopyFrom( rsCreateInfo ); }

	inline SNavEntityCreateInfo *	Clone( void ) const
	{
		SNavEntityCreateInfo * psClone = new SNavEntityCreateInfo( *this );

		return psClone;
	}

	inline void	CopyFrom( const SNavEntityCreateInfo & rsFrom )
	{
		nUniqueId		= rsFrom.nUniqueId;
		eEntityType		= rsFrom.eEntityType;
		eCreateType		= rsFrom.eCreateType;
		uSubType		= rsFrom.uSubType;
		pcParent		= rsFrom.pcParent;
		nFlags			= rsFrom.nFlags;
		bExport			= rsFrom.bExport;
		nUserDataSize	= rsFrom.nUserDataSize;
		pvUserData		= rsFrom.pvUserData;
		cName			= rsFrom.cName;
		cPath			= rsFrom.cPath;
		cTitle			= rsFrom.cTitle;
		cPosition		= rsFrom.cPosition;
		cBounding		= rsFrom.cBounding;

		pcThreadManager	= rsFrom.pcThreadManager;
		psResultInfo	= rsFrom.psResultInfo;
	}

	inline void	CopyTo( SNavEntityCreateInfo & rsTo ) const
	{
		rsTo.CopyFrom( *this );
	}

	inline void Reset( void )
	{
		nUniqueId		= 0;
		eEntityType		= E_NAV_ENTITY_TYPE_NONE;
		eCreateType		= E_NAV_CREATE_TYPE_NONE;
		uSubType.nValue	= 0;
		pcParent		= NULL;
		nFlags			= 0;
		bExport			= false;
		nUserDataSize	= 0;
		pvUserData		= NULL;

		cName.Clear();
		cPath.Clear();
		cTitle.Clear();
		cPosition.Reset();
		cBounding.Reset();

		pcThreadManager	= NULL;
		psResultInfo	= NULL;
	}
} SNavEntityCreateInfo;

#define E_NAV_ENTITY_CREATE_INFO_FLAG_CLIENT_MODE	( 0x10000000 )
#define E_NAV_ENTITY_CREATE_INFO_FLAG_SERVER_MODE	( 0x20000000 )
#define E_NAV_ENTITY_CREATE_INFO_FLAG_EXPORT_MODE	( 0x40000000 )
#define E_NAV_ENTITY_CREATE_INFO_FLAG_EDITOR_MODE	( 0x80000000 )
#define E_NAV_ENTITY_CREATE_INFO_FLAG_MULTI_THREAD	( 0x01000000 )

#define E_NAV_ENTITY_CREATE_INFO_FLAG_DEFAULT		( E_NAV_ENTITY_CREATE_INFO_FLAG_SERVER_MODE )

inline UInt32 MakeFlags( UInt32 nGlobalFlags, UInt32 nLocalFlags = 0 )
{
	return ( ( nGlobalFlags & 0xFFFF0000 ) | ( nLocalFlags & 0x0000FFFF ) );
}

inline UInt32 InsertFlags( UInt32 nOriginalFlags, UInt32 nInsertedFlags )
{
	return ( nOriginalFlags | nInsertedFlags );
}

inline UInt32 RemoveFlags( UInt32 nOriginalFlags, UInt32 nRemovedFlags )
{
	return ( nOriginalFlags & ~nRemovedFlags );
}

inline bool IsInFlags( UInt32 nFlags, UInt32 nKey )
{
	return ( ( nFlags & nKey ) == nKey ) ? true : false;
}

inline UInt32 GetGlobalFlags( UInt32 nGlobalFlags )
{
	return ( ( nGlobalFlags & 0xFFFF0000 ) >> 16 );
}

inline UInt32 GetLocalFlags( UInt32 nGlobalFlags )
{
	return ( nGlobalFlags & 0x0000FFFF );
}

//=============================================================================
// End of Struct : SNavEntityCreateInfo
//=============================================================================


//=============================================================================
// Name       : SNavEntityProcessInfo
// Desc       : 
// Author     : 
//=============================================================================

typedef struct SNavEntityProcessInfo
{
	UInt32		nPreviousTime;
	UInt32		nCurrentTime;
	UInt32		nFlags;
	void *		pvCallBack;	// or Listener

	CBaseThreadManager *	pcThreadManager;

	SNavEntityResultInfo	sResultInfo;

	inline		SNavEntityProcessInfo( void ) : nPreviousTime( 0 ), nCurrentTime( 0 ), nFlags( 0 ), pvCallBack( NULL ) {}
} SNavEntityProcessInfo;

//=============================================================================
// End of Struct : SNavEntityProcessInfo
//=============================================================================


//=============================================================================
// Name       : SNavEntityPreProcessInfo
// Desc       : 
// Author     : 
//=============================================================================

typedef struct SNavEntityPreProcessInfo
{
	bool					bPreProcess;
	UId32					nShapeId;
	CFixedString			cPath;
	UInt32					nFlags;

	CBaseThreadManager *	pcThreadManager;

	SNavEntityResultInfo *	psResultInfo;

public:

	inline					SNavEntityPreProcessInfo( bool bIsProcess = true ) : bPreProcess( bIsProcess ), nShapeId( 0 ), nFlags( 0 ), pcThreadManager( NULL ), psResultInfo( NULL ) {}
	inline					SNavEntityPreProcessInfo( const SNavEntityPreProcessInfo & rcInfo ) { CopyFrom( rcInfo ); }

	inline SNavEntityPreProcessInfo *	Clone( void ) const
	{
		SNavEntityPreProcessInfo * psClone = new SNavEntityPreProcessInfo( *this );

		return psClone;
	}

	inline void	CopyFrom( const SNavEntityPreProcessInfo & rsFrom )
	{
		bPreProcess		= rsFrom.bPreProcess;
		nShapeId		= rsFrom.nShapeId;
		nFlags			= rsFrom.nFlags;
		cPath			= rsFrom.cPath;

		pcThreadManager	= rsFrom.pcThreadManager;
		psResultInfo	= rsFrom.psResultInfo;
	}

	inline void	CopyTo( SNavEntityPreProcessInfo & rsTo ) const
	{
		rsTo.CopyFrom( *this );
	}

	inline void Reset( void )
	{
		bPreProcess		= false;
		nShapeId		= 0;
		nFlags			= 0;

		cPath.Clear();

		pcThreadManager	= NULL;
		psResultInfo	= NULL;
	}
} SNavEntityPreProcessInfo;

//=============================================================================
// End of Struct : SNavEntityPreProcessInfo
//=============================================================================


//=============================================================================
// Name       : SNavIndex2D
// Desc       : 
// Author     : 
//=============================================================================

typedef struct SNavIndex2D
{
	UInt32					nX;
	UInt32					nY;

public:

	inline					SNavIndex2D( void ) { Reset(); }
	inline					SNavIndex2D( const SNavIndex2D & rsIndex ) { Set( rsIndex ); }
	inline					SNavIndex2D( const CNavPosition & rcPosition, const CNavPosition & rcCellSize ) { Set( rcPosition, rcCellSize ); }
	inline					SNavIndex2D( UInt32 nNewX, UInt32 nNewY ) : nX( nNewX ), nY( nNewY ) {}

	inline void				Reset( void ) { nX = nY = 0; }
	inline void				SetX( UInt32 nNewX ) { nX = nNewX; }
	inline void				SetY( UInt32 nNewY ) { nY = nNewY; }
	inline UInt32			GetX( void ) const { return nX; }
	inline UInt32			GetY( void ) const { return nY; }
	inline void				Set( const SNavIndex2D & rsIndex ) { nX = rsIndex.GetX(); nY = rsIndex.GetY(); }
	inline void				Set( UInt32 nNewX, UInt32 nNewY ) { nX = nNewX; nY = nNewY; }
	inline void				Set( const CNavPosition & rcPosition, const CNavPosition & rcCellSize ) { SetX( rcPosition.GetX() / rcCellSize.GetX() ); SetY( rcPosition.GetY() / rcCellSize.GetY() ); }

	inline void				IncX( void ) { ++nX; }
	inline void				IncY( void ) { ++nY; }
	inline void				DecX( void ) { --nX; }
	inline void				DecY( void ) { --nY; }

	inline UInt32			GetArea( void ) const { return nX * nY; }
	inline UInt32			GetIndex( const UInt32 nWidth ) const { return nX + nWidth * nY; }
	inline UInt32			GetIndex( const SNavIndex2D & rsSize ) const { return GetIndex( rsSize.GetX() ); }
} SNavIndex2D;

//=============================================================================
// End of Struct : SNavIndex2D
//=============================================================================


//=============================================================================
// Name       : SNavCellPosition2D
// Desc       : 
// Author     : 
//=============================================================================

typedef struct SNavCellPosition2D
{
	SNavIndex2D				sIndex;
	SNavIndex2D				sSize;

public:

							SNavCellPosition2D( void ) : sIndex( 0, 0 ), sSize( 1, 1 ) {}

	inline void				Set( const SNavCellPosition2D & rsCellPosition )
	{
		memcpy( this, &rsCellPosition, sizeof( SNavCellPosition2D ) );
	}

	inline void				SetIndex( const SNavIndex2D & rsIndex )
	{
		sIndex.Set( rsIndex );
	}

	inline void				SetIndex( UInt32 nX, UInt32 nY )
	{
		sIndex.Set( nX, nY );
	}

	inline void				SetIndexX( UInt32 nX )
	{
		sIndex.SetX( nX );
	}

	inline void				SetIndexY( UInt32 nY )
	{
		sIndex.SetY( nY );
	}

	inline void				SetSize( UInt32 nX, UInt32 nY )
	{
		sSize.Set( nX, nY );
	}

	inline void				SetSize( const SNavIndex2D & rsSize )
	{
		sSize.Set( rsSize );
	}

	inline UInt32			GetIndex( void ) const
	{
		return sIndex.nY * sSize.nX + sIndex.nX;
	}

	inline UInt32			GetIndexX( void ) const { return sIndex.GetX(); }
	inline UInt32			GetIndexY( void ) const { return sIndex.GetY(); }

} SNavCellPosition2D;

//=============================================================================
// End of Struct : SNavCellPosition2D
//=============================================================================

END_NAMESPACE( nav )

#endif //___NAV_ENTITY_BASE_H___

//*****************************************************************************
//
// End of File : NavEntityBase.h
//
//*****************************************************************************