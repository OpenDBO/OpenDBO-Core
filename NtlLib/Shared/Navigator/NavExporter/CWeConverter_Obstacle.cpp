//*****************************************************************************
// File       : CWeConverter_Obstacle.cpp
// Desc       : 
// Author     : June C. Kim (cynis@hotmail.com)
// Update     : 2007. 2. 22
//*****************************************************************************

//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "StdAfx.h"

#include "CNavPosition2D.h"

#include "CWeConverter.h"


//-----------------------------------------------------------------------------
// Name Space Table
//-----------------------------------------------------------------------------

USE_NAMESPACE( navex )


//-----------------------------------------------------------------------------
// Initialize Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Name			: ConvertObstacles
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

UInt32 CWeConverter::ConvertObstacles( iMesh * pcMesh, const CNavEntityVector * const pcSectors ) const
{
	Assert( pcMesh != NULL );

	UInt32		nConvertCount = 0;
	
	iPathEngine *	pcPathEngine = CNavPathEngine::GetInstance()->GetPathEngine();
	Assert( pcPathEngine != NULL );

	iCollisionContext *		pcContext		= pcMesh->newContext();
	Assert( pcContext != NULL );

	iObstacleSet *			pcObstacleSet	= pcMesh->newObstacleSet();
	Assert( pcObstacleSet != NULL );

	//	pcContext->addObstacleSet( pcObstacleSet );

	CNavSector *		pcSector;

	for( UInt32 nOrder = 0; nOrder < pcSectors->GetCount(); ++nOrder )
	{
		pcSector = down_cast<CNavSector *>(pcSectors->GetAt( nOrder ));
		Assert( pcSector != NULL );

		SNavCellPosition2D	sCellPosition;
		pcSector->GetCellPosition( sCellPosition );

		CNavFilePath	cFilePath( GetObjectPath() );
		CFixedString	cSectorId;
		cSectorId.Format( _T( "%d" ), pcSector->GetCellPosition() );

		CFixedString	cFullFileName;
		cFilePath.GetFullPathName( cFullFileName, cSectorId );

		CMemoryBuffer	cBuffer( cFullFileName );

		for( UInt32 nWeFieldId = 0; nWeFieldId < m_nSectorsInField; ++nWeFieldId )
		{
			UInt32		nObstacleCount;
			cBuffer.Copy( &nObstacleCount, sizeof( UInt32 ) );

			if( nObstacleCount == 0 )
			{
				continue;
			}

			for( UInt32 nObstacleIndex = 0; nObstacleIndex < nObstacleCount; ++nObstacleIndex )
			{
				RwChar		szName[128];
				cBuffer.Copy( szName, sizeof( RwChar ) * 128 );

				RwV3d			sScale, sDirection, sPosition;
				CNavPosition2D	cScale;
				Float32			fDirection;

				cBuffer.Copy( &sScale, sizeof( RwV3d ) );
				cScale.SetFloat( sScale.x, sScale.z );

				cBuffer.Copy( &sDirection, sizeof( RwV3d ) );
				fDirection = PI - ( PI * sDirection.y / 180.0f );

				cBuffer.Copy( &sPosition, sizeof( RwV3d ) );
				CNavPosition2D	cNavPosition;
				cNavPosition.SetFloat( sPosition.x, sPosition.z );

				CFixedString	cModelName( szName );
				const SWeModelInfo &	rsModelInfo = GetModelInfo( cModelName );

				TCHAR	szLog[1024];
				_stprintf_s( szLog, 1024, _T( "Obstacle : Position( %.1f, %.1f ) Direction( %.1f, %.1f, %.1f ), Scale( %.1f, %.1f )" ),
							sPosition.x, sPosition.z,
							sDirection.x, sDirection.y, sDirection.z,
							sScale.x, sScale.z );
				g_Log( szLog );

				_stprintf_s( szLog, 1024, _T( "\tModel Name : %s" ),
							cModelName.GetBuffer() );
				g_Log( szLog );

				SWePolygonNode *	psPolygon = rsModelInfo.psPolygon;

				for( UInt32 nPolygonIndex = 0; nPolygonIndex < rsModelInfo.nFlags; ++nPolygonIndex )
				{
					if( psPolygon == NULL )
					{
						break;
					}

					_stprintf_s( szLog, 1024, _T( "\tPart : %d of %d" ),
								nPolygonIndex + 1, rsModelInfo.nFlags );
					g_Log( szLog );

					UInt32		nVertexCount = psPolygon->cPolygon.GetCount();
					Assert( nVertexCount != 0 );

					CNavPosition2D *	pcVertexBuffer = new CNavPosition2D[nVertexCount];
					Assert( pcVertexBuffer != NULL );

					Fixed32	*	pnResultBuffer = new Fixed32[nVertexCount * 2];
					Assert( pnResultBuffer != NULL );

					// Transform

					CNavPosition2D	cTransformedPosition( cNavPosition );
					psPolygon->cPolygon.Convert( pnResultBuffer, cTransformedPosition, fDirection, cScale );

					iShape *	pcShape = pcPathEngine->newShape( nVertexCount, pnResultBuffer );
					if( pcShape == NULL )
					{
						Assert( !"pcPathEngine->newShape() Fail" );

						continue;
					}

					long	anPosition[3];
					anPosition[0] = cTransformedPosition.GetX();
					anPosition[1] = cTransformedPosition.GetY();
					anPosition[2] = 0;

					cPosition	cPePosition = pcMesh->positionFor3DPoint( anPosition );
					if( cPePosition.cell == -1 )
					{
						g_Log( "\tResult : Failure" );

						continue;
					}

					iAgent *	pcAgent = pcMesh->placeAgent( pcShape, cPePosition );
					if( pcAgent == NULL )
					{
						Assert( !"pcMesh->placeAgent() Fail" );

						continue;
					}

					pcContext->addAgent( pcAgent );
					pcObstacleSet->addAgent( pcAgent );

					++nConvertCount;

					g_Log( "\tResult : Success" );

					delete[] pcVertexBuffer;
					delete[] pnResultBuffer;

					psPolygon = psPolygon->psNext;
				}	
			}
		}
	}

	pcMesh->burnContextIntoMesh( pcContext );

	return nConvertCount;
}


//-----------------------------------------------------------------------------
// Name			: ConvertPosition
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CWeConverter::ConvertPosition( cPosition * pcResultPosition, CNavPosition * pcPosition, iMesh * pcMesh ) const
{
	Assert( pcMesh != NULL );

	long	anVertex[3];

	anVertex[0] = static_cast<long>(pcPosition->GetX());
	anVertex[1] = static_cast<long>(pcPosition->GetY());
	anVertex[2] = static_cast<long>(pcPosition->GetZ());

	*pcResultPosition = pcMesh->positionFor3DPoint( reinterpret_cast<const long *>(anVertex) );

	return (pcResultPosition->cell >= 0 );
}


//-----------------------------------------------------------------------------
// Name			: CreateModelInfoes
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CWeConverter::CreateModelInfoes( CFixedString & rcModelListFileName )
{
	rcModelListFileName;

	CFixedString	cModelListFileName;

	m_cModelPath.GetFullPathName( cModelListFileName );

	CXMLParserW		cParser;
	if( cParser.Create( cModelListFileName ) == false )
	{
		return false;
	}


	CFixedStringTreeNodeW *	pcRoot = cParser.GetRoot();
	Assert( pcRoot != NULL );

	CFixedStringTreeNodeW *	pcObjectProperty = pcRoot->FindChild( KEY__OBJECT_PROPERTY );
	Assert( pcObjectProperty != NULL );

	CFixedStringTreeNodeW *	pcObjectElement = pcObjectProperty->FindChild( KEY__OBJECT_ELEMENT );
	Assert( pcObjectElement != NULL );

	UInt32		nElementCount = pcObjectElement->GetChildCount();

	CFixedStringTreeNodeW *	pcValue;

	for( UInt32 nElementIndex = 0; nElementIndex < nElementCount; ++nElementIndex )
	{
		CFixedStringTreeNodeW * pcElement = pcObjectElement->FindChild( nElementIndex );
		Assert( pcElement != NULL );

		CFixedStringTreeNodeW * pcElementName = pcElement->FindChild( KEY__NAME );
		Assert( pcElementName != NULL );

		pcValue = pcElementName->FindChild();
		Assert( pcValue != NULL );

		RegisterModelInfo( pcValue->GetString() );
	}

	return true;
}


//-----------------------------------------------------------------------------
// Name			: RegisterModelInfo
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

const SWeModelInfo & CWeConverter::RegisterModelInfo( const CFixedStringW & rcModelName )
{
	SWeModelingInfoNode *	psNode = m_psModels;
	SWeModelingInfoNode *	psPrev = m_psModels;

	// Get Pure Name (Discard ".xml")

	CFixedStringW	cPureModelName;
	cPureModelName.CreateSubStringFrom( rcModelName, 0, rcModelName.GetLength() - 5 );

	while( psNode != NULL )
	{
		if( psNode->sInfo.cName == cPureModelName )
		{
			return psNode->sInfo;
		}

		psPrev = psNode;
		psNode = psNode->psNext;
	}

	psNode = new SWeModelInfoNode;
	if( psPrev == NULL )
	{
		m_psModels  = psNode;
	}
	else
	{
		psPrev->psNext = psNode;
	}

	++m_nModelCount;

	psNode->sInfo.cName.Assign( cPureModelName );

	if( LoadModelInfo( psNode->sInfo, cPureModelName ) == false )
	{
		Assert( !"LoadModelInfo() Fail" );
	}

	return psNode->sInfo;
}


//-----------------------------------------------------------------------------
// Name			: GetModelInfo
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

const SWeModelInfo & CWeConverter::GetModelInfo( const CFixedStringW & rcModelName ) const
{
	SWeModelingInfoNode *	psNode = m_psModels;
	SWeModelingInfoNode *	psPrev = m_psModels;

	while( psNode != NULL )
	{
		if( psNode->sInfo.cName == rcModelName )
		{
			return psNode->sInfo;
		}

		psPrev = psNode;
		psNode = psNode->psNext;
	}

	AfxMessageBox( "Unregistered Model" );

	static const SWeModelInfo	sError;

	return sError;
}


//-----------------------------------------------------------------------------
// Name			: LoadModelInfo
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CWeConverter::LoadModelInfo( SWeModelInfo & rcModelInfo, const CFixedStringW & rcModelName ) const
{
	CNavFilePath	cModelFilePath( m_cModelPath );
	cModelFilePath.SetName( rcModelName );
	CFixedString	cModelFileName;
	cModelFilePath.GetFullPathName( cModelFileName );

	CXMLParserW		cParser;
	if( cParser.Create( cModelFileName ) == false )
	{
		return false;
	}

	// Keyword List

	CFixedStringTreeNodeW *	pcRoot = cParser.GetRoot();
	Assert( pcRoot != NULL );

	CFixedStringTreeNodeW *	pcObjectScript = pcRoot->FindChild( KEY__OBJECT_SCRIPT );
	Assert( pcObjectScript != NULL );

	CFixedStringTreeNodeW *	pcBody = pcObjectScript->FindChild( KEY__BODY );
	Assert( pcBody != NULL );

	CFixedStringTreeNodeW *	pcCollision = pcBody->FindChild( KEY__PATH_COLLISION );
	if( pcCollision == NULL )
	{
		rcModelInfo.nFlags = 0;

		return true;
	}

	CFixedStringTreeNodeW *	pcCollisionValue = pcCollision->FindChild();
	Assert( pcCollisionValue != NULL );
	rcModelInfo.nFlags = static_cast<UInt32>(pcCollisionValue->GetString().ConvertToInt32());

	if( rcModelInfo.nFlags == 0 )
	{
		return true;
	}

	CFixedString	cPolygonName;

	CFixedStringTreeNodeW *	pcCollisionMeshName = pcBody->FindChild( KEY__COLLISION_MESH );
	if( pcCollisionMeshName != NULL && pcCollisionMeshName->GetChildCount() != 0 )
	{
		RemoveFileExt( cPolygonName, pcCollisionMeshName->FindChild()->GetString() );
	}
	else
	{
		CFixedStringTreeNodeW *	pcHeader = pcObjectScript->FindChild( KEY__HEADER );
		Assert( pcHeader != NULL );

		CFixedStringTreeNodeW *	pcObjectName = pcHeader->FindChild( KEY__NAME );
		Assert( pcObjectName != NULL );

		cPolygonName.Assign( COLLISION_PREFIX );
		cPolygonName.Append( pcObjectName->FindChild()->GetString() );
	}

	return LoadCollisionInfo( rcModelInfo, cPolygonName );
}


//-----------------------------------------------------------------------------
// Name			: LoadCollisionInfo
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CWeConverter::LoadCollisionInfo( SWeModelInfo & rcModelInfo, const CFixedStringW & rcCollisionName ) const
{
	CNavFilePath	cCollisionFilePath( m_cModelPath );
	cCollisionFilePath.SetName( rcCollisionName );
	cCollisionFilePath.SetExt( KEY_ASE );

	CFixedString	cCollisiionFileName;
	cCollisionFilePath.GetFullPathName( cCollisiionFileName );

	CFixedStringList	cStringList;
	if( cStringList.CreateFromFile( cCollisiionFileName ) == false )
	{
		rcModelInfo.nFlags = 0;

		return true;
	}

	rcModelInfo.nFlags = ParsePolygon( &(rcModelInfo.psPolygon), cStringList, 0 );

	return true;
}


//-----------------------------------------------------------------------------
// Name			: ParsePolygon
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

UInt32 CWeConverter::ParsePolygon( SWePolygonNode ** ppsPolygon, const CFixedStringList & rcStringList, UInt32 nPosition ) const
{
	nPosition = rcStringList.FindEqual( KEY__GEOMOBJECT, nPosition );
	if( nPosition == FIXED_STRING_SEARCH_FAIL )
	{
		return 0;
	}


	// Find Offset

	CNavPosition	cOffset;

	nPosition = rcStringList.FindEqual( KEY__NODE_TM, nPosition );
	if( nPosition != FIXED_STRING_SEARCH_FAIL )
	{
		nPosition = rcStringList.FindEqual( KEY__TM_POS, nPosition );
		if( nPosition != FIXED_STRING_SEARCH_FAIL )
		{
			cOffset.SetX( static_cast<Int32>(rcStringList.GetAt( ++nPosition )->ConvertToFloat32()) );
			cOffset.SetZ( static_cast<Int32>(rcStringList.GetAt( ++nPosition )->ConvertToFloat32()) );
			cOffset.SetY( static_cast<Int32>(rcStringList.GetAt( ++nPosition )->ConvertToFloat32()) );
			cOffset.SetW( 0 );
		}
		else
		{
			cOffset.Reset();
		}
	}
	else
	{
		cOffset.Reset();
	}


	// Find Vertices

	nPosition = rcStringList.FindEqual( KEY__MESH, nPosition );
	if( nPosition == FIXED_STRING_SEARCH_FAIL )
	{
		Assert( !"KEY__MESH" );

		return 0;
	}

	nPosition = rcStringList.FindEqual( KEY__MESH_NUMVERTEX, nPosition );
	if( nPosition == FIXED_STRING_SEARCH_FAIL )
	{
		Assert( !"KEY__MESH_NUMVERTEX" );

		return 0;
	}

	UInt32	nVertices = rcStringList.GetAt( ++nPosition )->ConvertToInt32();
	if( nVertices == 0 )
	{
		Assert( !"nVertices == 0" );

		return 0;
	}

	nPosition = rcStringList.FindEqual( KEY__MESH_NUMFACES, nPosition );
	if( nPosition == FIXED_STRING_SEARCH_FAIL )
	{
		Assert( !"KEY__MESH_NUMFACES" );

		return 0;
	}

	UInt32	nFaces = rcStringList.GetAt( ++nPosition )->ConvertToInt32();
	if( nFaces == 0 )
	{
		Assert( !"nFaces == 0" );

		return 0;
	}


	// Parse Vertex List

	nPosition = rcStringList.FindEqual( KEY__MESH_VERTEX_LIST, nPosition );
	if( nPosition == FIXED_STRING_SEARCH_FAIL )
	{
		Assert( !"KEY__MESH_VERTEX_LIST" );

		return 0;
	}

	CNavPositionList	cOriginalVertexList;
	cOriginalVertexList.Alloc( nVertices );

	CNavPosition		cVertex;
	CNavPosition		cCenter;

	for( UInt32 nIndex = 0; nIndex < nVertices; ++nIndex )
	{
		nPosition = rcStringList.FindEqual( KEY__MESH_VERTEX, nPosition );
		if( nPosition == FIXED_STRING_SEARCH_FAIL )
		{
			Assert( !"KEY__MESH_VERTEX" );

			return 0;
		}

		if( static_cast<UInt32>(rcStringList.GetAt( ++nPosition )->ConvertToInt32()) != nIndex )
		{
			Assert( !"nIndex" );

			return 0;
		}

		cVertex.SetX( static_cast<Int32>(rcStringList.GetAt( ++nPosition )->ConvertToFloat32()) );
		cVertex.SetZ( static_cast<Int32>(rcStringList.GetAt( ++nPosition )->ConvertToFloat32()) );
		cVertex.SetY( static_cast<Int32>(rcStringList.GetAt( ++nPosition )->ConvertToFloat32()) );
		cVertex.SetW( 0 );

		cVertex.Sub( cOffset );
		cCenter.Add( cVertex );

		cVertex.SetX( -cVertex.GetX() );
		cOriginalVertexList.SetPosition( nIndex, cVertex );
	}

	if( nVertices >= 3 )
	{
		cCenter.Div( nVertices );
	}


	// Parse Face List

	nPosition = rcStringList.FindEqual( KEY__MESH_FACE_LIST, nPosition );
	if( nPosition == FIXED_STRING_SEARCH_FAIL )
	{
		Assert( !"KEY__MESH_FACE_LIST" );

		return 0;
	}

	CNavIndexedFaceList		cOriginalFaceList;
	cOriginalFaceList.Alloc( nFaces );

	CNavIndexedFace		cFace;

	for( UInt32 nIndex = 0; nIndex < nFaces; ++nIndex )
	{
		nPosition = rcStringList.FindEqual( KEY__MESH_FACE, nPosition );
		if( nPosition == FIXED_STRING_SEARCH_FAIL )
		{
			Assert( !"KEY__MESH_FACE" );

			return 0;
		}

		nPosition += 3;
		cFace.SetIndex( 0, rcStringList.GetAt( nPosition )->ConvertToInt32() );

		nPosition += 2;
		cFace.SetIndex( 1, rcStringList.GetAt( nPosition )->ConvertToInt32() );

		nPosition += 2;
		cFace.SetIndex( 2, rcStringList.GetAt( nPosition )->ConvertToInt32() );

		nPosition += 2;
		cFace.SetFlags( 0, rcStringList.GetAt( nPosition )->ConvertToInt32() );

		nPosition += 2;
		cFace.SetFlags( 1, rcStringList.GetAt( nPosition )->ConvertToInt32() );

		nPosition += 2;
		cFace.SetFlags( 2, rcStringList.GetAt( nPosition )->ConvertToInt32() );

		cOriginalFaceList.SetFace( nIndex, cFace );
	}


	// Normalize

	SWePolygonNode *	psThisNode = new SWePolygonNode;
	Assert( psThisNode != NULL );

	psThisNode->cPolygon.Create( cOriginalVertexList, cOriginalFaceList );


	// Parse Next Polygon

	*ppsPolygon = psThisNode;

	return ParsePolygon( &(psThisNode->psNext), rcStringList, nPosition ) + 1;
}


//-----------------------------------------------------------------------------
// Name			: DestroyModelInfoes
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CWeConverter::DestroyModelInfoes( void )
{
	SWeModelInfoNode *	psNode = m_psModels;
	SWeModelInfoNode *	psTemp;

	while( psNode != NULL )
	{
		psTemp = psNode;
		psNode = psNode->psNext;

		delete psTemp;
	}

	m_nModelCount = 0;

	return;
}


//*****************************************************************************
//
// End of File : CWeConverter_Obstacle.cpp
//
//*****************************************************************************