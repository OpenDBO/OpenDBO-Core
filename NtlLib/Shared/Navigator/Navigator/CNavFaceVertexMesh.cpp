//*****************************************************************************
// File       : CNavFaceVertexMesh.cpp
// Desc       : 
// Author     : June C. Kim (cynis@hotmail.com)
// Update     : 2007. 1. 18
//*****************************************************************************

//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "StdAfx.h"

#include "CNavigator.h"
#include "CNavPathEngine.h"
#include "CNavEntityFactory.h"

#include "CNavFaceVertexMesh.h"


//-----------------------------------------------------------------------------
// Name Space Table
//-----------------------------------------------------------------------------

USE_NAMESPACE( nav )


//-----------------------------------------------------------------------------
// Initialize Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Name			: Initialize
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CNavFaceVertexMesh::Initialize( void )
{
	m_nVertexCount			= 0;
	m_nVertexBufferSize		= 0;
	m_pcVertexPositions		= NULL;

	m_nFaceCount			= 0;
	m_nFaceBufferSize		= 0;
	m_psFaceVertices		= NULL;
	m_psFaceAttributes		= NULL;

	m_cBound.Invalidate();

	return;
}


//-----------------------------------------------------------------------------
// Name			: Destroy
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CNavFaceVertexMesh::Destroy( void )
{
	if( m_psFaceAttributes != NULL )
	{
		delete[] m_psFaceAttributes;
	}

	if( m_psFaceVertices != NULL )
	{
		delete[] m_psFaceVertices;
	}

	if( m_pcVertexPositions != NULL )
	{
		delete[] m_pcVertexPositions;
	}

	Initialize();

	return;
}


//-----------------------------------------------------------------------------
// Name			: ExpandVertexBuffer
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CNavFaceVertexMesh::ExpandVertexBuffer( UInt32 nBufferSize )
{
	if( m_nVertexBufferSize == 0 )
	{
		nBufferSize = Max( nBufferSize, static_cast<UInt32>(NAV_FACE_VERTEX_MESH_VERTEX_BUFFER_SIZE_DEFAULT) );

		m_pcVertexPositions = new CNavPosition[nBufferSize];
		Assert( m_pcVertexPositions != NULL );
	}
	else
	{
		nBufferSize = Max( nBufferSize, static_cast<UInt32>(m_nVertexBufferSize << 1) );

		CNavPosition *	pcVertexPositions = new CNavPosition[nBufferSize];
		Assert( pcVertexPositions != NULL );
		memcpy( pcVertexPositions, m_pcVertexPositions, sizeof( CNavPosition ) * m_nVertexCount );
		delete[] m_pcVertexPositions;
		m_pcVertexPositions = pcVertexPositions;
	}

	m_nVertexBufferSize = nBufferSize;

	return;
}


//-----------------------------------------------------------------------------
// Name			: ExpandFaceBuffer
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CNavFaceVertexMesh::ExpandFaceBuffer( UInt32 nBufferSize )
{
	if( m_nFaceBufferSize == 0 )
	{
		nBufferSize = Max( nBufferSize, static_cast<UInt32>(NAV_FACE_VERTEX_MESH_FACE_BUFFER_SIZE_DEFAULT) );

		m_psFaceVertices = new SNavIndexedTriangle[nBufferSize];
		Assert( m_psFaceVertices != NULL );

		m_psFaceAttributes = new SNavFaceAttribute[nBufferSize];
		Assert( m_psFaceAttributes != NULL );
	}
	else
	{
		nBufferSize = Max( nBufferSize, static_cast<UInt32>(m_nFaceBufferSize << 1) );

		SNavIndexedTriangle *	psFaceVertices = new SNavIndexedTriangle[nBufferSize];
		Assert( psFaceVertices != NULL );
		memcpy( psFaceVertices, m_psFaceVertices, sizeof( SNavIndexedTriangle ) * m_nFaceCount );
		delete[] m_psFaceVertices;
		m_psFaceVertices = psFaceVertices;

		SNavFaceAttribute *		psFaceAttributes = new SNavFaceAttribute[nBufferSize];
		Assert( psFaceAttributes != NULL );
		memcpy( psFaceAttributes, m_psFaceAttributes, sizeof( SNavFaceAttribute ) * m_nFaceCount );
		delete[] m_psFaceAttributes;
		m_psFaceAttributes = psFaceAttributes;
	}

	m_nFaceBufferSize = nBufferSize;

	return;
}


//-----------------------------------------------------------------------------
// Name			:RegisterVertex
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

Int32 CNavFaceVertexMesh::RegisterVertex( const CNavPosition & rcVertexPosition )
{
	// Check Vertex Position is Registered

	Int32	nVertexIndex;
	for( nVertexIndex = 0; nVertexIndex < m_nVertexCount; ++nVertexIndex )
	{
		Assert( m_pcVertexPositions != NULL );

		if( m_pcVertexPositions[nVertexIndex].IsEqual( rcVertexPosition ) == true )
		{
			return nVertexIndex;
		}
	}

	// Register New Vertex Position

	Assert( m_nVertexCount <= m_nVertexBufferSize );
	if( m_nVertexCount == m_nVertexBufferSize )
	{
		ExpandVertexBuffer( m_nVertexBufferSize << 1 );
	}

	m_pcVertexPositions[nVertexIndex].Set( rcVertexPosition );

	//TCHAR	szBuf[1024];
	//_stprintf_s( szBuf, 1024, _T( "V(%d) : %d %d %d" ), m_nVertexCount,
	//			rcVertexPosition.GetX(), rcVertexPosition.GetY(), rcVertexPosition.GetZ() );
	//g_Log( szBuf );

	++m_nVertexCount;

	m_cBound.ExpandToContainPosition( rcVertexPosition );

	return nVertexIndex;
}


//-----------------------------------------------------------------------------
// Name			:RegisterVertexForce
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

Int32 CNavFaceVertexMesh::RegisterVertexForce( const CNavPosition & rcVertexPosition )
{
	// Register New Vertex Position

	Assert( m_nVertexCount <= m_nVertexBufferSize );
	if( m_nVertexCount == m_nVertexBufferSize )
	{
		ExpandVertexBuffer( m_nVertexBufferSize << 1 );
	}

	m_pcVertexPositions[m_nVertexCount].Set( rcVertexPosition );
	++m_nVertexCount;

	m_cBound.ExpandToContainPosition( rcVertexPosition );

	return m_nVertexCount - 1;
}


//-----------------------------------------------------------------------------
// Name			: RegisterFace
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

Int32 CNavFaceVertexMesh::RegisterFace( const SNavIndexedTriangle & rsIndex, SNavFaceAttribute * psFaceAttribute )
{
	// Check Face Vertex Index is Registered

	Int32	nFaceIndex;

	for( nFaceIndex = 0; nFaceIndex < m_nFaceCount; ++nFaceIndex )
	{
		if( m_psFaceVertices[nFaceIndex].IsEqual( rsIndex ) == true )
		{
			return nFaceIndex;
		}
	}

	// Register New Face Vertex Index

	Assert( m_nFaceCount <= m_nFaceBufferSize );
	if( m_nFaceCount == m_nFaceBufferSize )
	{
		ExpandFaceBuffer( m_nFaceBufferSize << 1 );
	}

	m_psFaceVertices[nFaceIndex].Set( rsIndex );

	if( psFaceAttribute != NULL )
	{
		m_psFaceAttributes[nFaceIndex].Set( *psFaceAttribute );
	}
	else
	{
		m_psFaceAttributes[nFaceIndex].Reset();
	}

	//TCHAR	szBuf[1024];
	//_stprintf_s( szBuf, 1024, _T( "F(%d) : %d %d %d" ), m_nFaceCount,
	//			rsIndex.GetIndex( 0 ), rsIndex.GetIndex( 1 ), rsIndex.GetIndex( 2 ) );
	//g_Log( szBuf );

	++m_nFaceCount;

	return nFaceIndex;
}


//-----------------------------------------------------------------------------
// Name			: RegisterFaceForce
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

Int32 CNavFaceVertexMesh::RegisterFaceForce( const SNavIndexedTriangle & rsIndex, SNavFaceAttribute * psFaceAttribute )
{
	// Register New Face Vertex Index

	Assert( m_nFaceCount <= m_nFaceBufferSize );
	if( m_nFaceCount == m_nFaceBufferSize )
	{
		ExpandFaceBuffer( m_nFaceBufferSize << 1 );
	}

	m_psFaceVertices[m_nFaceCount].Set( rsIndex );

	if( psFaceAttribute != NULL )
	{
		m_psFaceAttributes[m_nFaceCount].Set( *psFaceAttribute );
	}
	else
	{
		m_psFaceAttributes[m_nFaceCount].Reset();
	}

	Assert( !"NOOOOO" );

	++m_nFaceCount;

	return m_nFaceCount - 1;
}


//-----------------------------------------------------------------------------
// Name			: Export
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------
/*
bool CNavFaceVertexMesh::Export( ENavFileType eExportFileType, CFixedString & rcExportFileName )
{
	CNavEntity	*		pcNavTerrain;

	switch( eExportFileType )
	{
		case E_NAV_FILE_TYPE_XML:
		case E_NAV_FILE_TYPE_TOK:
		{
			pcNavTerrain = CreateTerrain( rcExportFileName );

			break;
		}

		default:
		{
			pcNavTerrain = NULL;
		}
	}

	if( pcNavTerrain == NULL )
	{
		return false;
	}

	CFixedString	cExportFilePath;

	pcNavTerrain->Export( eExportFileType, cExportFilePath );

	CNavEntityFactory::GetInstance()->DestroyEntity( &pcNavTerrain );

	return true;
}*/


//-----------------------------------------------------------------------------
// Name			: CreateTerrain
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CNavEntity * CNavFaceVertexMesh::CreateTerrain( const CFixedString & rcName, Float32 fFilterSlope, Float32 fOptimizeThreshold )
{
	static const CFixedStringA	KEYWORD_FILTER_SLOPE( "filterFacesWithSlopeAbove" );
	static const CFixedStringA	KEYWORD_OPTIMIZE_THRESHOLD( "optimiseWithThreshold" );
	CFixedStringA	PARAMETER_FILTER_SLOPE;
	CFixedStringA	PARAMETER_OPTIMIZE_THRESHOLD;
	CFixedString	cEngineName;
	CFixedString	cSessionName;
//	CNavigator *	pcNavigator = CNavigator::CreateInstance( cEngineName, cSessionName );
	iPathEngine *	pcPathEngine = CNavPathEngine::GetInstance()->GetPathEngine();


	UInt32		nAttributeCount = 0;
	char *		pachAttributeList[5] = { "\0", };

	if( fFilterSlope != 0.0f )
	{
		PARAMETER_FILTER_SLOPE.Format( "%f", fFilterSlope );

		pachAttributeList[nAttributeCount] = (LPSTR)KEYWORD_FILTER_SLOPE.GetBuffer();
		pachAttributeList[++nAttributeCount] = (LPSTR)PARAMETER_FILTER_SLOPE.GetBuffer();
	}

	if( fOptimizeThreshold != 0.0f )
	{
		PARAMETER_OPTIMIZE_THRESHOLD.Format( "%f", fOptimizeThreshold );

		pachAttributeList[++nAttributeCount] = (LPSTR)KEYWORD_OPTIMIZE_THRESHOLD.GetBuffer();
		pachAttributeList[++nAttributeCount] = (LPSTR)PARAMETER_OPTIMIZE_THRESHOLD.GetBuffer();
	}

	const iFaceVertexMesh *		pcFaceVertexMesh = this;
	iMesh *		pcMesh = pcPathEngine->buildMeshFromContent( &pcFaceVertexMesh, 1, pachAttributeList );

	if( pcMesh == NULL )
	{
		return NULL;
	}

	SNavEntityCreateInfo	sCreateInfo;
	sCreateInfo.nUniqueId		= 0;
	sCreateInfo.eEntityType		= E_NAV_ENTITY_TYPE_TERRAIN;
	sCreateInfo.eCreateType		= E_NAV_CREATE_TYPE_INSTANCE;
	sCreateInfo.pvUserData		= pcMesh;
	sCreateInfo.nUserDataSize	= sizeof( iMesh * );
	sCreateInfo.pcParent		= NULL;
	sCreateInfo.cName.Assign( rcName );
	sCreateInfo.cBounding.Set( m_cBound );

	CNavEntity *	pcNavTerrain = g_pEntityFactory->CreateEntity( sCreateInfo );

	return pcNavTerrain;
}


//*****************************************************************************
//
// End of File : CNavFaceVertexMesh.cpp
//
//*****************************************************************************