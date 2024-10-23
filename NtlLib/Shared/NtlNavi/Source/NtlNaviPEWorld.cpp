﻿#include "precomp_navi.h"

#include "NtlNaviPEWorld.h"
#include "NtlNaviLog.h"
#include "NtlNaviDataMng.h"
#include "NtlNaviUtility.h"
#include "NtlNaviPathEngine.h"
#include "NtlNaviPEImporter.h"


CNtlNaviPEWorld::CNtlNaviPEWorld( void )
{
	m_eCurState = eNAVI_PE_STATE_PRE_LOADING;

	m_byLoadedFlags = ePATH_DATA_LOAD_FLAG_NO_LOADING;

	m_pNaviDataMng = new CNtlNaviDataMng;
}

CNtlNaviPEWorld::~CNtlNaviPEWorld( void )
{
	Destroy();

	if ( m_pNaviDataMng )
	{
		delete m_pNaviDataMng;
		m_pNaviDataMng = NULL;
	}
}

CNtlNaviPEWorld::eNAVI_PE_STATE CNtlNaviPEWorld::GetCurState( void )
{
	m_csCurState.Lock();
	eNAVI_PE_STATE eState = m_eCurState;
	m_csCurState.Unlock();

	return eState;
}

void CNtlNaviPEWorld::SetCurState( eNAVI_PE_STATE eState )
{
	m_csCurState.Lock();
	m_eCurState = eState;
	m_csCurState.Unlock();
}

const char* CNtlNaviPEWorld::GetImportPath( void )
{
	return m_strImportPath.c_str();
}

unsigned char CNtlNaviPEWorld::GetLoadedFlags( void )
{
	return m_byLoadedFlags;
}

bool CNtlNaviPEWorld::ImportPathData( const char* pPath, unsigned char byLoadFlags )
{
	Destroy();

	m_strImportPath = pPath;

	if ( !ImportWorldInfo( m_strImportPath.c_str() ) )
	{
		CNtlNaviLog::GetInstance()->Log( "[IMPORT] Can not import world info data. [%s, %d]", m_strImportPath.c_str(), byLoadFlags );

		return false;
	}

	if ( byLoadFlags & ePATH_DATA_LOAD_FLAG_LOAD_ONLY_PROPERTY )
	{
		if ( ImportWorldProperty( m_strImportPath.c_str() ) )
		{
			m_byLoadedFlags |= ePATH_DATA_LOAD_FLAG_LOAD_ONLY_PROPERTY;
		}
		else
		{
			CNtlNaviLog::GetInstance()->Log( "[IMPORT] Can not import property data. [%s, %d]", m_strImportPath.c_str(), byLoadFlags );

			return false;
		}
	}

	if ( byLoadFlags & ePATH_DATA_LOAD_FLAG_LOAD_ONLY_WORLD )
	{
		// ÇØ´ç Æú´õ°¡ Á¸ÀçÇÏ´ÂÁö °Ë»çÇÑ´Ù
		bool bFindPathFolder = false;

		{
			WIN32_FIND_DATA FindFileData;
			HANDLE hFind = INVALID_HANDLE_VALUE;

			std::string strImportPath = m_strImportPath;
			AttachBackSlash( strImportPath );

			hFind = FindFirstFile( (strImportPath + "*.*").c_str(), &FindFileData );

			if ( hFind != INVALID_HANDLE_VALUE )
			{
				do
				{
					if ( (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) &&
						 (FindFileData.cFileName[0] != '.') )
					{
						if ( std::string( FindFileData.cFileName ) == PE_PATHENGINE_FOLDER )
						{
							bFindPathFolder = true;
							break;
						}
					}
				}
				while ( FindNextFile( hFind, &FindFileData ) != 0 );

				FindClose( hFind );
			}
		}

		// ÇØ´ç ÆÐ½º¸¦ ¸øÃ£À¸¸é ÆÐ½º µ¥ÀÌÅÍ¸¸ »ç¿ëÇÏÁö ¾Ê´Â °ÍÀ¸·Î Ã³¸®ÇÑ´Ù
		if ( !bFindPathFolder )
		{
			return true;
		}

		if ( ImportWorldGroup( m_strImportPath.c_str() ) )
		{
			m_byLoadedFlags |= ePATH_DATA_LOAD_FLAG_LOAD_ONLY_WORLD;
		}
		else
		{
			CNtlNaviLog::GetInstance()->Log( "[IMPORT] Can not import world group data. [%s, %d]", m_strImportPath.c_str(), byLoadFlags );

			return false;
		}
	}

	SetCurState( eNAVI_PE_STATE_LOADING );

	// Multi-thread ·ÎµùÀÌ ¾Æ´Ñ °æ¿ì
	if ( NULL == CNtlLoadingQueue::GetInstance() )
	{
		return CheckLoadComplete();
	}

	return true;
}

bool CNtlNaviPEWorld::CheckLoadComplete( void )
{
	switch ( m_eCurState )
	{
	case eNAVI_PE_STATE_LOADING:
		{
			return UpdateEntityToLoad();
		}
		break;
	}

	return true;
}

NAVI_INST_HANDLE CNtlNaviPEWorld::CreateInstanceHandler( unsigned int uiWorldID )
{
	sNAVI_INST_HANDLER* pInstHandler = new sNAVI_INST_HANDLER;

	sINNER_INST_HANDLER* pInnerInst = new sINNER_INST_HANDLER;

	mapdef_GroupDataList::iterator it = m_defGroupDataList.begin();
	for ( ; it != m_defGroupDataList.end(); ++it )
	{
		sGROUP_DATA& sGroupData = it->second;
		pInnerInst->defColContextList[it->first] = sGroupData.pGroundMesh->newContext();
	}

	pInstHandler->uiWorldID = uiWorldID;
	pInstHandler->pInnerData = pInnerInst;

	m_defInstHandlerList[pInstHandler] = pInstHandler;

	return pInstHandler;
}

void CNtlNaviPEWorld::DeleteInstanceHandler( NAVI_INST_HANDLE hHandle )
{
	mapdef_InstHandlerList::iterator itInst = m_defInstHandlerList.find( hHandle );
	if ( itInst == m_defInstHandlerList.end() ) return;

	sNAVI_INST_HANDLER* pInstHandler = itInst->second;

	sINNER_INST_HANDLER* pInnerInst = (sINNER_INST_HANDLER*)pInstHandler->pInnerData;

	mapdef_ColContextList::iterator itColContext = pInnerInst->defColContextList.begin();
	for ( ; itColContext != pInnerInst->defColContextList.end(); ++itColContext )
	{
		itColContext->second->destroy();
	}
	pInnerInst->defColContextList.clear();

	delete pInnerInst;

	delete pInstHandler;

	m_defInstHandlerList.erase( itInst );
}

void CNtlNaviPEWorld::DeleteAllInstanceHandler( void )
{
	mapdef_InstHandlerList::iterator itInst = m_defInstHandlerList.begin();
	for ( ; itInst != m_defInstHandlerList.end(); ++itInst )
	{
		sNAVI_INST_HANDLER* pInstHandler = itInst->second;

		sINNER_INST_HANDLER* pInnerInst = (sINNER_INST_HANDLER*)pInstHandler->pInnerData;

		mapdef_ColContextList::iterator itColContext = pInnerInst->defColContextList.begin();
		for ( ; itColContext != pInnerInst->defColContextList.end(); ++itColContext )
		{
			itColContext->second->destroy();
		}
		pInnerInst->defColContextList.clear();

		delete pInnerInst;

		delete pInstHandler;
	}

	m_defInstHandlerList.clear();
}

bool CNtlNaviPEWorld::CanSupportAgent( float fAgentRadius )
{
	if ( eNAVI_PE_STATE_COMPLETE != GetCurState() )
	{
		return false;
	}

	if ( m_defAgentList.find( fAgentRadius ) == m_defAgentList.end() )
	{
		return false;
	}

	return true;
}

unsigned int CNtlNaviPEWorld::GetAttribute( float x, float z )
{
	if ( eNAVI_PE_STATE_COMPLETE != GetCurState() )
	{
		return 0;
	}

	switch ( m_pNaviDataMng->GetLoadedWorld()->GetType() )
	{
	case eNAVI_INFO_WORLD_OUTDOOR:
		{
			CNtlNaviWorldOutDoorInfo* pODInfo = (CNtlNaviWorldOutDoorInfo*)m_pNaviDataMng->GetLoadedWorld();

			if ( pODInfo )
			{
				float fMinPosX, fMinPosZ;
				float fMaxPosX, fMaxPosZ;

				pODInfo->GetWorldMinPos( fMinPosX, fMinPosZ );
				pODInfo->GetWorldMaxPos( fMaxPosX, fMaxPosZ );

				// Field id °è»ê

				float fFieldSize = pODInfo->GetFieldSize();

				unsigned int uiCrossFieldCnt = (unsigned int)((fMaxPosX - fMinPosX) / fFieldSize);
				unsigned int uiFieldX = (unsigned int)((x - fMinPosX) / fFieldSize);
				unsigned int uiFieldZ = (unsigned int)((z - fMinPosZ) / fFieldSize);
				unsigned int uiFieldID = uiFieldX + uiFieldZ * uiCrossFieldCnt;

				mapdef_ODPropList::iterator it = m_defODPropList.find( uiFieldID );

				if ( it == m_defODPropList.end() )
				{
					return 0;
				}

				// Tile id °è»ê

				float fTileSize = pODInfo->GetTileSize();

				unsigned int uiTileCrossCnt = (int)(fFieldSize / fTileSize);
				unsigned int uiTileX = (int)(((x - fMinPosX) - uiFieldX * fFieldSize) / fTileSize);
				unsigned int uiTileZ = (int)(((z - fMinPosZ) - uiFieldZ * fFieldSize) / fTileSize);
				unsigned int uiTileID = uiTileX + uiTileZ * uiTileCrossCnt;

				CNtlNaviPropOutDoorInfo* pODPropInfo = it->second;

				return pODPropInfo->GetTileProp()[uiTileID];
			}
		}
		break;

	case eNAVI_INFO_WORLD_INDOOR:
		{
			CNtlNaviWorldInDoorInfo* pIDInfo = (CNtlNaviWorldInDoorInfo*)m_pNaviDataMng->GetLoadedWorld();

			if ( pIDInfo )
			{
				float fMinPosX, fMinPosZ;
				float fMaxPosX, fMaxPosZ;

				pIDInfo->GetWorldMinPos( fMinPosX, fMinPosZ );
				pIDInfo->GetWorldMaxPos( fMaxPosX, fMaxPosZ );

				// Block id °è»ê

				float fBlockSize = pIDInfo->GetBlockSize();

				unsigned int uiCrossBlockCnt = (unsigned int)((fMaxPosX - fMinPosX) / fBlockSize);
				unsigned int uiBlockX = (unsigned int)((x - fMinPosX) / fBlockSize);
				unsigned int uiBlockZ = (unsigned int)((z - fMinPosZ) / fBlockSize);
				unsigned int uiBlockID = uiBlockX + uiBlockZ * uiCrossBlockCnt;

				mapdef_IDPropList::iterator it = m_defIDPropList.find( uiBlockID );

				if ( it == m_defIDPropList.end() )
				{
					return 0;
				}

				CNtlNaviPropInDoorInfo* pIDPropInfo = it->second;

				unsigned int uiProperty = 0;

				int nEntityCnt = pIDPropInfo->GetEntityInfoCnt();
				for ( int i = 0; i < nEntityCnt; ++i )
				{
					CNtlNaviPropInDoorInfo::sENTITY_INFO* pEntityInfo = pIDPropInfo->GetEntityInfo( i );

					if ( CNtlNaviPropInDoorInfo::eENTITY_TYPE_SPHERE == pEntityInfo->eEntityType )
					{
						if ( IsColProp_Sphere( x,
											   z,
											   pEntityInfo->uEntityData.sSphere.fX,
											   pEntityInfo->uEntityData.sSphere.fZ,
											   pEntityInfo->uEntityData.sSphere.fRadius ) )
						{
							uiProperty |= pEntityInfo->uEntityData.sSphere.uiProperty;
						}
					}
					else if ( CNtlNaviPropInDoorInfo::eENTITY_TYPE_PLANE == pEntityInfo->eEntityType )
					{
						if ( IsColProp_Plane( x,
											  z,
											  pEntityInfo->uEntityData.sPlane.fX,
											  pEntityInfo->uEntityData.sPlane.fZ,
											  pEntityInfo->uEntityData.sPlane.fWidth,
											  pEntityInfo->uEntityData.sPlane.fDepth ) )
						{
							uiProperty |= pEntityInfo->uEntityData.sPlane.uiProperty;
						}
					}
				}

				return uiProperty;
			}
		}
		break;
	}

	return 0;
}

unsigned int CNtlNaviPEWorld::GetTextAllIndex( float x, float z )
{
	if ( eNAVI_PE_STATE_COMPLETE != GetCurState() )
	{
		return 0xffffffff;
	}

	switch ( m_pNaviDataMng->GetLoadedWorld()->GetType() )
	{
	case eNAVI_INFO_WORLD_OUTDOOR:
		{
			CNtlNaviWorldOutDoorInfo* pODInfo = (CNtlNaviWorldOutDoorInfo*)m_pNaviDataMng->GetLoadedWorld();

			if ( pODInfo )
			{
				float fMinPosX, fMinPosZ;
				float fMaxPosX, fMaxPosZ;

				pODInfo->GetWorldMinPos( fMinPosX, fMinPosZ );
				pODInfo->GetWorldMaxPos( fMaxPosX, fMaxPosZ );

				// Field id °è»ê

				float fFieldSize = pODInfo->GetFieldSize();

				unsigned int uiCrossFieldCnt = (unsigned int)((fMaxPosX - fMinPosX) / fFieldSize);
				unsigned int uiFieldX = (unsigned int)((x - fMinPosX) / fFieldSize);
				unsigned int uiFieldZ = (unsigned int)((z - fMinPosZ) / fFieldSize);
				unsigned int uiFieldID = uiFieldX + uiFieldZ * uiCrossFieldCnt;

				mapdef_ODPropList::iterator it = m_defODPropList.find( uiFieldID );

				if ( it == m_defODPropList.end() )
				{
					return 0xffffffff;
				}

				CNtlNaviPropOutDoorInfo* pODPropInfo = it->second;

				unsigned int uiFieldProp = pODPropInfo->GetFieldProp();

				return uiFieldProp == 0 ? 0xffffffff : uiFieldProp;
			}
		}
		break;

	case eNAVI_INFO_WORLD_INDOOR:
		{
			CNtlNaviWorldInDoorInfo* pIDInfo = (CNtlNaviWorldInDoorInfo*)m_pNaviDataMng->GetLoadedWorld();

			if ( pIDInfo )
			{
				float fMinPosX, fMinPosZ;
				float fMaxPosX, fMaxPosZ;

				pIDInfo->GetWorldMinPos( fMinPosX, fMinPosZ );
				pIDInfo->GetWorldMaxPos( fMaxPosX, fMaxPosZ );

				// Block id °è»ê

				float fBlockSize = pIDInfo->GetBlockSize();

				unsigned int uiCrossBlockCnt = (unsigned int)((fMaxPosX - fMinPosX) / fBlockSize);
				unsigned int uiBlockX = (unsigned int)((x - fMinPosX) / fBlockSize);
				unsigned int uiBlockZ = (unsigned int)((z - fMinPosZ) / fBlockSize);
				unsigned int uiBlockID = uiBlockX + uiBlockZ * uiCrossBlockCnt;

				mapdef_IDPropList::iterator it = m_defIDPropList.find( uiBlockID );

				if ( it == m_defIDPropList.end() )
				{
					return 0xffffffff;
				}

				CNtlNaviPropInDoorInfo* pIDPropInfo = it->second;

				unsigned int uiBlockProp = pIDPropInfo->GetBlockProp();

				return uiBlockProp == 0 ? 0xffffffff : uiBlockProp;
			}
		}
		break;
	}

	return 0xffffffff;
}

unsigned int CNtlNaviPEWorld::GetZoneIndex( float x, float z )
{
	if ( eNAVI_PE_STATE_COMPLETE != GetCurState() )
	{
		return 0xffffffff;
	}

	switch ( m_pNaviDataMng->GetLoadedWorld()->GetType() )
	{
	case eNAVI_INFO_WORLD_OUTDOOR:
		{
			CNtlNaviWorldOutDoorInfo* pODInfo = (CNtlNaviWorldOutDoorInfo*)m_pNaviDataMng->GetLoadedWorld();

			if ( pODInfo )
			{
				float fMinPosX, fMinPosZ;
				float fMaxPosX, fMaxPosZ;

				pODInfo->GetWorldMinPos( fMinPosX, fMinPosZ );
				pODInfo->GetWorldMaxPos( fMaxPosX, fMaxPosZ );

				// Field id °è»ê

				float fFieldSize = pODInfo->GetFieldSize();

				unsigned int uiCrossFieldCnt = (unsigned int)((fMaxPosX - fMinPosX) / fFieldSize);
				unsigned int uiFieldX = (unsigned int)((x - fMinPosX) / fFieldSize);
				unsigned int uiFieldZ = (unsigned int)((z - fMinPosZ) / fFieldSize);
				unsigned int uiFieldID = uiFieldX + uiFieldZ * uiCrossFieldCnt;

				mapdef_ODPropList::iterator it = m_defODPropList.find( uiFieldID );

				if ( it == m_defODPropList.end() )
				{
					return 0xffffffff;
				}

				CNtlNaviPropOutDoorInfo* pODPropInfo = it->second;

				unsigned int uiFieldProp = pODPropInfo->GetFieldProp();

				return uiFieldProp == 0 ? 0xffffffff : uiFieldProp / 1000;
			}
		}
		break;

	case eNAVI_INFO_WORLD_INDOOR:
		{
			CNtlNaviWorldInDoorInfo* pIDInfo = (CNtlNaviWorldInDoorInfo*)m_pNaviDataMng->GetLoadedWorld();

			if ( pIDInfo )
			{
				float fMinPosX, fMinPosZ;
				float fMaxPosX, fMaxPosZ;

				pIDInfo->GetWorldMinPos( fMinPosX, fMinPosZ );
				pIDInfo->GetWorldMaxPos( fMaxPosX, fMaxPosZ );

				// Block id °è»ê

				float fBlockSize = pIDInfo->GetBlockSize();

				unsigned int uiCrossBlockCnt = (unsigned int)((fMaxPosX - fMinPosX) / fBlockSize);
				unsigned int uiBlockX = (unsigned int)((x - fMinPosX) / fBlockSize);
				unsigned int uiBlockZ = (unsigned int)((z - fMinPosZ) / fBlockSize);
				unsigned int uiBlockID = uiBlockX + uiBlockZ * uiCrossBlockCnt;

				mapdef_IDPropList::iterator it = m_defIDPropList.find( uiBlockID );

				if ( it == m_defIDPropList.end() )
				{
					return 0xffffffff;
				}

				CNtlNaviPropInDoorInfo* pIDPropInfo = it->second;

				unsigned int uiBlockProp = pIDPropInfo->GetBlockProp();

				return uiBlockProp == 0 ? 0xffffffff : uiBlockProp / 1000;
			}
		}
		break;
	}

	return 0xffffffff;
}

float CNtlNaviPEWorld::GetHeight( float x, float y, float z )
{
	if ( eNAVI_PE_STATE_COMPLETE != GetCurState() )
	{
		return NAVI_FLT_MAX;
	}

	switch ( m_pNaviDataMng->GetLoadedWorld()->GetType() )
	{
	case eNAVI_INFO_WORLD_OUTDOOR:
		{
			CNtlNaviWorldOutDoorInfo* pODInfo = (CNtlNaviWorldOutDoorInfo*)m_pNaviDataMng->GetLoadedWorld();

			if ( pODInfo )
			{
				float fMinPosX, fMinPosZ;
				float fMaxPosX, fMaxPosZ;

				pODInfo->GetWorldMinPos( fMinPosX, fMinPosZ );
				pODInfo->GetWorldMaxPos( fMaxPosX, fMaxPosZ );

				// Field id °è»ê

				float fFieldSize = pODInfo->GetFieldSize();
				float fGroupSize = fFieldSize * pODInfo->GetCrossFieldCntOfGroup();

				unsigned int uiCrossGroupCnt = (unsigned int)((fMaxPosX - fMinPosX) / fGroupSize);
				unsigned int uiGroupX = (unsigned int)((x - fMinPosX) / fGroupSize);
				unsigned int uiGroupZ = (unsigned int)((z - fMinPosZ) / fGroupSize);
				unsigned int uiGroupID = uiGroupX + uiGroupZ * uiCrossGroupCnt;

				mapdef_GroupDataList::iterator it = m_defGroupDataList.find( uiGroupID );

				if ( it == m_defGroupDataList.end() )
				{
					return NAVI_FLT_MAX;
				}

				sGROUP_DATA& sGroupData = it->second;

				if ( NULL == sGroupData.pGroundMesh )
				{
					return NAVI_FLT_MAX;
				}

				tSigned32 arTemp[3];

				arTemp[0] = (tSigned32) WORLD_COORD_TO_PATH_COORD( x );
				arTemp[1] = (tSigned32) WORLD_COORD_TO_PATH_COORD( z );
				arTemp[2] = (tSigned32) WORLD_COORD_TO_PATH_COORD( y );

				cPosition cPoint = sGroupData.pGroundMesh->positionNear3DPoint( arTemp, PATH_HORIZ_RANGE, PATH_VERT_RANGE );

				if ( cPoint.cell == -1 )
				{
					return NAVI_FLT_MAX;
				}

				return (float)PATH_COORD_TO_WORLD_COORD( sGroupData.pGroundMesh->heightAtPosition( cPoint ) );
			}
		}
		break;

	case eNAVI_INFO_WORLD_INDOOR:
		{
			CNtlNaviWorldInDoorInfo* pIDInfo = (CNtlNaviWorldInDoorInfo*)m_pNaviDataMng->GetLoadedWorld();

			if ( pIDInfo )
			{
				// ÀÎµµ¿öÀÇ °æ¿ì ÇÏ³ªÀÇ navigation mesh ¸¸ Á¸ÀçÇÔ
				if ( m_defGroupDataList.size() != 1 )
				{
					return NAVI_FLT_MAX;
				}

				sGROUP_DATA& sGroupData = m_defGroupDataList.begin()->second;

				if ( NULL == sGroupData.pGroundMesh )
				{
					return NAVI_FLT_MAX;
				}

				tSigned32 arTemp[3];

				arTemp[0] = (tSigned32) WORLD_COORD_TO_PATH_COORD( x );
				arTemp[1] = (tSigned32) WORLD_COORD_TO_PATH_COORD( z );
				arTemp[2] = (tSigned32) WORLD_COORD_TO_PATH_COORD( y );

				cPosition cPoint = sGroupData.pGroundMesh->positionNear3DPoint( arTemp, PATH_HORIZ_RANGE, PATH_VERT_RANGE );

				if ( cPoint.cell == -1 )
				{
					return NAVI_FLT_MAX;
				}

				return (float)PATH_COORD_TO_WORLD_COORD( sGroupData.pGroundMesh->heightAtPosition( cPoint ) );
			}
		}
		break;
	}

	return NAVI_FLT_MAX;
}

float CNtlNaviPEWorld::GetGuaranteedHeight( float x, float y, float z )
{
	if ( eNAVI_PE_STATE_COMPLETE != GetCurState() )
	{
		return NAVI_FLT_MAX;
	}

	switch ( m_pNaviDataMng->GetLoadedWorld()->GetType() )
	{
	case eNAVI_INFO_WORLD_OUTDOOR:
		{
			CNtlNaviWorldOutDoorInfo* pODInfo = (CNtlNaviWorldOutDoorInfo*)m_pNaviDataMng->GetLoadedWorld();

			if ( pODInfo )
			{
				float fMinPosX, fMinPosZ;
				float fMaxPosX, fMaxPosZ;

				pODInfo->GetWorldMinPos( fMinPosX, fMinPosZ );
				pODInfo->GetWorldMaxPos( fMaxPosX, fMaxPosZ );

				// Field id °è»ê

				float fFieldSize = pODInfo->GetFieldSize();
				float fGroupSize = fFieldSize * pODInfo->GetCrossFieldCntOfGroup();

				unsigned int uiCrossGroupCnt = (unsigned int)((fMaxPosX - fMinPosX) / fGroupSize);
				unsigned int uiGroupX = (unsigned int)((x - fMinPosX) / fGroupSize);
				unsigned int uiGroupZ = (unsigned int)((z - fMinPosZ) / fGroupSize);
				unsigned int uiGroupID = uiGroupX + uiGroupZ * uiCrossGroupCnt;

				mapdef_GroupDataList::iterator it = m_defGroupDataList.find( uiGroupID );

				if ( it == m_defGroupDataList.end() )
				{
					return NAVI_FLT_MAX;
				}

				sGROUP_DATA& sGroupData = it->second;

				if ( NULL == sGroupData.pGroundMesh )
				{
					return NAVI_FLT_MAX;
				}

				tSigned32 arTemp[3];

				arTemp[0] = (tSigned32) WORLD_COORD_TO_PATH_COORD( x );
				arTemp[1] = (tSigned32) WORLD_COORD_TO_PATH_COORD( z );
				arTemp[2] = (tSigned32) WORLD_COORD_TO_PATH_COORD( y );

				cPosition cPoint = sGroupData.pGroundMesh->positionNear3DPoint( arTemp, PATH_HORIZ_RANGE, MAX_PATH_HEIGHT );

				if ( cPoint.cell == -1 )
				{
					return NAVI_FLT_MAX;
				}

				return (float)PATH_COORD_TO_WORLD_COORD( sGroupData.pGroundMesh->heightAtPosition( cPoint ) );
			}
		}
		break;

	case eNAVI_INFO_WORLD_INDOOR:
		{
			CNtlNaviWorldInDoorInfo* pIDInfo = (CNtlNaviWorldInDoorInfo*)m_pNaviDataMng->GetLoadedWorld();

			if ( pIDInfo )
			{
				// ÀÎµµ¿öÀÇ °æ¿ì ÇÏ³ªÀÇ navigation mesh ¸¸ Á¸ÀçÇÔ
				if ( m_defGroupDataList.size() != 1 )
				{
					return NAVI_FLT_MAX;
				}

				sGROUP_DATA& sGroupData = m_defGroupDataList.begin()->second;

				if ( NULL == sGroupData.pGroundMesh )
				{
					return NAVI_FLT_MAX;
				}

				tSigned32 arTemp[3];

				arTemp[0] = (tSigned32) WORLD_COORD_TO_PATH_COORD( x );
				arTemp[1] = (tSigned32) WORLD_COORD_TO_PATH_COORD( z );
				arTemp[2] = (tSigned32) WORLD_COORD_TO_PATH_COORD( y );

				cPosition cPoint = sGroupData.pGroundMesh->positionNear3DPoint( arTemp, PATH_HORIZ_RANGE, MAX_PATH_HEIGHT );

				if ( cPoint.cell == -1 )
				{
					return NAVI_FLT_MAX;
				}

				return (float)PATH_COORD_TO_WORLD_COORD( sGroupData.pGroundMesh->heightAtPosition( cPoint ) );
			}
		}
		break;
	}

	return NAVI_FLT_MAX;
}

bool CNtlNaviPEWorld::FindNearestPos(NAVI_INST_HANDLE hHandle, float fAgentRadius, CNtlNaviVector3& vSourcePos, int nHorizRange, int nClosestRange)
{
	if (eNAVI_PE_STATE_COMPLETE != GetCurState())
	{
		return false;
	}

	float x = vSourcePos.GetX();
	float z = vSourcePos.GetZ();

	switch (m_pNaviDataMng->GetLoadedWorld()->GetType())
	{
	case eNAVI_INFO_WORLD_OUTDOOR:
	{
		CNtlNaviWorldOutDoorInfo* pODInfo = (CNtlNaviWorldOutDoorInfo*)m_pNaviDataMng->GetLoadedWorld();

		if (pODInfo)
		{
			mapdef_AgentList::iterator itAgent = m_defAgentList.find(fAgentRadius);

			if (itAgent == m_defAgentList.end())
			{
				return false;
			}

			float fMinPosX, fMinPosZ;
			float fMaxPosX, fMaxPosZ;

			pODInfo->GetWorldMinPos(fMinPosX, fMinPosZ);
			pODInfo->GetWorldMaxPos(fMaxPosX, fMaxPosZ);

			// Field id °è»ê

			float fFieldSize = pODInfo->GetFieldSize();
			float fGroupSize = fFieldSize * pODInfo->GetCrossFieldCntOfGroup();

			unsigned int uiCrossGroupCnt = (unsigned int)((fMaxPosX - fMinPosX) / fGroupSize);
			unsigned int uiGroupX = (unsigned int)((x - fMinPosX) / fGroupSize);
			unsigned int uiGroupZ = (unsigned int)((z - fMinPosZ) / fGroupSize);
			unsigned int uiGroupID = uiGroupX + uiGroupZ * uiCrossGroupCnt;

			mapdef_GroupDataList::iterator itGroup = m_defGroupDataList.find(uiGroupID);

			if (itGroup == m_defGroupDataList.end())
			{
				return false;
			}

			sGROUP_DATA& sGroupData = itGroup->second;

			if (NULL == sGroupData.pGroundMesh)
			{
				return false;
			}

			tSigned32 arTemp[3];

			arTemp[0] = (tSigned32)WORLD_COORD_TO_PATH_COORD(x);
			arTemp[1] = (tSigned32)WORLD_COORD_TO_PATH_COORD(z);
			arTemp[2] = (tSigned32)WORLD_COORD_TO_PATH_COORD(vSourcePos.GetY());

			cPosition cNearPoint = sGroupData.pGroundMesh->positionNear3DPoint(arTemp, nHorizRange, PATH_VERT_RANGE);

			if (cNearPoint.cell == -1)
			{
				return false;
			}


			iCollisionContext* pContext = NULL;

			if (hHandle)
			{
				sNAVI_INST_HANDLER* pHandler = (sNAVI_INST_HANDLER*)hHandle;

				sINNER_INST_HANDLER* pInnerHandler = (sINNER_INST_HANDLER*)pHandler->pInnerData;

				if (pInnerHandler)
				{
					mapdef_ColContextList::iterator itColContext = pInnerHandler->defColContextList.find(uiGroupID);

					if (pInnerHandler->defColContextList.end() != itColContext)
					{
						pContext = itColContext->second;
					}
				}
			}

			cPosition cSourcePos = sGroupData.pGroundMesh->findClosestUnobstructedPosition(itAgent->second.pShape, pContext, cNearPoint, nClosestRange);

			if (cSourcePos.cell == -1)
			{
				return false;
			}

			vSourcePos.SetElem((float)PATH_COORD_TO_WORLD_COORD(cSourcePos.x),
				(float)PATH_COORD_TO_WORLD_COORD(sGroupData.pGroundMesh->heightAtPosition(cSourcePos)),
				(float)PATH_COORD_TO_WORLD_COORD(cSourcePos.y));

			return true;
		}
	}
	break;

	case eNAVI_INFO_WORLD_INDOOR:
	{
		CNtlNaviWorldInDoorInfo* pIDInfo = (CNtlNaviWorldInDoorInfo*)m_pNaviDataMng->GetLoadedWorld();

		if (pIDInfo)
		{
			mapdef_AgentList::iterator itAgent = m_defAgentList.find(fAgentRadius);

			if (itAgent == m_defAgentList.end())
			{
				return false;
			}

			// ÀÎµµ¿öÀÇ °æ¿ì ÇÏ³ªÀÇ navigation mesh ¸¸ Á¸ÀçÇÔ
			if (m_defGroupDataList.size() != 1)
			{
				return false;
			}

			sGROUP_DATA& sGroupData = m_defGroupDataList.begin()->second;

			if (NULL == sGroupData.pGroundMesh)
			{
				return false;
			}

			tSigned32 arTemp[3];

			arTemp[0] = (tSigned32)WORLD_COORD_TO_PATH_COORD(x);
			arTemp[1] = (tSigned32)WORLD_COORD_TO_PATH_COORD(z);
			arTemp[2] = (tSigned32)WORLD_COORD_TO_PATH_COORD(vSourcePos.GetY());

			cPosition cNearPoint = sGroupData.pGroundMesh->positionNear3DPoint(arTemp, nHorizRange, PATH_VERT_RANGE);

			if (cNearPoint.cell == -1)
			{
				return false;
			}

			iCollisionContext* pContext = NULL;

			if (hHandle)
			{
				sNAVI_INST_HANDLER* pHandler = (sNAVI_INST_HANDLER*)hHandle;

				sINNER_INST_HANDLER* pInnerHandler = (sINNER_INST_HANDLER*)pHandler->pInnerData;

				if (pInnerHandler)
				{
					mapdef_ColContextList::iterator itColContext = pInnerHandler->defColContextList.find(0);

					if (pInnerHandler->defColContextList.end() != itColContext)
					{
						pContext = itColContext->second;
					}
				}
			}

			cPosition cSourcePos = sGroupData.pGroundMesh->findClosestUnobstructedPosition(itAgent->second.pShape, NULL, cNearPoint, nClosestRange);

			if (cSourcePos.cell == -1)
			{
				return false;
			}

			vSourcePos.SetElem((float)PATH_COORD_TO_WORLD_COORD(cSourcePos.x),
				(float)PATH_COORD_TO_WORLD_COORD(sGroupData.pGroundMesh->heightAtPosition(cSourcePos)),
				(float)PATH_COORD_TO_WORLD_COORD(cSourcePos.y));

			return true;
		}
	}
	break;
	}

	return false;
}

bool CNtlNaviPEWorld::FindNearestPos(NAVI_INST_HANDLE hHandle, float fAgentRadius, CNtlNaviVector3& vSourcePos)
{
	return FindNearestPos(hHandle, fAgentRadius, vSourcePos, PATH_HORIZ_RANGE, PATH_CLOSEST_RANGE);
}

/**
* \brief 충돌 검사
* \remark Agent의 반지름을 굵기로 가지는 하나의 라인이 Obstacle이나 Navigation mesh에서 충돌이 되는지 안되는지의 여부를 Test 한다.
* \param nHandle WORLD의 ID와 iMesh Data를 가지는 구조체를 ( void* ) 으로 캐스팅하기 위해 사용되는 Handle
* \param fAgentRadius Agent의 반지름
* \param vSourcePos	시작 좌표
* \param vTargetPos 끝 좌표
* \param nHorizRange Max horizontal range search
* \return 충돌 결과를 나타내는 eCOL_TEST_RESULT 열거형
*/
eCOL_TEST_RESULT CNtlNaviPEWorld::CollisionTest( NAVI_INST_HANDLE hHandle, float fAgentRadius, CNtlNaviVector3& vSourcePos, CNtlNaviVector3& vTargetPos, int nHorizRange )
{
	// ÇöÀç PathEngine data°¡ ¿ÏÀüÈ÷ ·ÎµùµÇ¾î ÀÖÁö ¾Ê´Ù¸é ¿¬»êÀ» ¼öÇàÇÒ ¼ö ¾ø´Ù.
	if ( eNAVI_PE_STATE_COMPLETE != GetCurState() )
	{
		return eCOL_TEST_RESULT_FAILED;
	}

	float x = vSourcePos.GetX();
	float z = vSourcePos.GetZ();

	switch ( m_pNaviDataMng->GetLoadedWorld()->GetType() )
	{
	case eNAVI_INFO_WORLD_OUTDOOR:
		{
			CNtlNaviWorldOutDoorInfo* pODInfo = (CNtlNaviWorldOutDoorInfo*)m_pNaviDataMng->GetLoadedWorld();

			// OutDoor Á¤º¸¸¦ °¡Á®¿Í¼­ À¯È¿ÇÒ ¶§¸¸ °Ë»çÇÑ´Ù.
			if ( pODInfo )
			{
				// ÇöÀç Agent list¿¡¼­ À¯È¿ÇÑ ¹ÝÁö¸§ÀÇ Agent¸¦ ²¨³»¿Â´Ù. ¾ø´Ù¸é Ãæµ¹ ½ÇÆÐ¸¦ ¸®ÅÏÇÑ´Ù.
				mapdef_AgentList::iterator itAgent = m_defAgentList.find( fAgentRadius );
				if ( itAgent == m_defAgentList.end() )
				{
					return eCOL_TEST_RESULT_FAILED;
				}

				// ÇöÀç OutDoorÀÇ ÀüÃ¼ Å©±â °è»ê
				float fMinPosX, fMinPosZ;
				float fMaxPosX, fMaxPosZ;

				pODInfo->GetWorldMinPos( fMinPosX, fMinPosZ );
				pODInfo->GetWorldMaxPos( fMaxPosX, fMaxPosZ );

				// Field id °è»ê
				float fFieldSize = pODInfo->GetFieldSize();
				float fGroupSize = fFieldSize * pODInfo->GetCrossFieldCntOfGroup();

				unsigned int uiCrossGroupCnt = (unsigned int)((fMaxPosX - fMinPosX) / fGroupSize);	// GroupÀÇ Çà °è»ê
				unsigned int uiGroupX = (unsigned int)((x - fMinPosX) / fGroupSize);				// GroupÀÇ X °è»ê
				unsigned int uiGroupZ = (unsigned int)((z - fMinPosZ) / fGroupSize);				// GroupÀÇ Z °è»ê
				unsigned int uiGroupID = uiGroupX + uiGroupZ * uiCrossGroupCnt;						// Group ID

				// GroupÀÇ ID·Î ÇöÀç X Y °¡ ¼ÓÇØ ÀÖ´Â GroupÀ» ²¨³»¿Â´Ù.
				mapdef_GroupDataList::iterator itGroup = m_defGroupDataList.find( uiGroupID );
				if ( itGroup == m_defGroupDataList.end() )
				{
					return eCOL_TEST_RESULT_FAILED;
				}

				// Group Data¸¦ ²¨³»¿Â´Ù.
				sGROUP_DATA& sGroupData = itGroup->second;
				if ( NULL == sGroupData.pGroundMesh )
				{
					return eCOL_TEST_RESULT_FAILED;
				}

				tSigned32 arTemp[3];

				// SourceÀÇ ¿ùµå ÁÂÇ¥ -> ÆÐ½º¿£Áø ÁÂÇ¥
				arTemp[0] = (tSigned32) WORLD_COORD_TO_PATH_COORD( x );
				arTemp[1] = (tSigned32) WORLD_COORD_TO_PATH_COORD( z );
				arTemp[2] = (tSigned32) WORLD_COORD_TO_PATH_COORD( vSourcePos.GetY() );
				cPosition cSourcePos = sGroupData.pGroundMesh->positionNear3DPoint( arTemp, nHorizRange, PATH_VERT_RANGE );
				if ( cSourcePos.cell == -1 )
				{
					return eCOL_TEST_RESULT_INVALID_SRC_POS;
				}

				// TargetÀÇ ¿ùµå ÁÂÇ¥ -> ÆÐ½º¿£Áø ÁÂÇ¥
				arTemp[0] = (tSigned32) WORLD_COORD_TO_PATH_COORD( vTargetPos.GetX() );
				arTemp[1] = (tSigned32) WORLD_COORD_TO_PATH_COORD( vTargetPos.GetZ() );
				arTemp[2] = (tSigned32) WORLD_COORD_TO_PATH_COORD( vTargetPos.GetY() );
				cPosition cTargetPos = sGroupData.pGroundMesh->positionNear3DPoint( arTemp, nHorizRange, PATH_VERT_RANGE );
				if ( cTargetPos.cell == -1 )
				{
					return eCOL_TEST_RESULT_INVALID_DEST_POS;
				}

				// ÇöÀç GroupIDÀÇ ¾Ë¸Â´Â Collision context ¸¦ ²¨³»¿Â´Ù.
				iCollisionContext* pContext = NULL;
				if ( hHandle )
				{
					sNAVI_INST_HANDLER* pHandler = (sNAVI_INST_HANDLER*) hHandle;
					sINNER_INST_HANDLER* pInnerHandler = (sINNER_INST_HANDLER*)pHandler->pInnerData;

					if ( pInnerHandler )
					{
						mapdef_ColContextList::iterator itColContext = pInnerHandler->defColContextList.find( uiGroupID );
						if ( pInnerHandler->defColContextList.end() != itColContext )
						{
							pContext = itColContext->second;
						}
					}
				}

				// Source ¿Í Target À» ÀÕ´Â LineÀÌ Ãæµ¹µÇ´ÂÁö ¾ÈµÇ´ÂÁö Å×½ºÆ®ÇÑ´Ù.
				eCOL_TEST_RESULT eTestResult = eCOL_TEST_RESULT_FAILED;
				if( sGroupData.pGroundMesh->testLineCollision( itAgent->second.pShape, pContext, cSourcePos, cTargetPos ) )
					eTestResult = eCOL_TEST_RESULT_COL;
				else
					eTestResult = eCOL_TEST_RESULT_NO_COL;
				return  eTestResult;
			}
		}
		break;

	case eNAVI_INFO_WORLD_INDOOR:
		{
			CNtlNaviWorldInDoorInfo* pIDInfo = (CNtlNaviWorldInDoorInfo*)m_pNaviDataMng->GetLoadedWorld();

			if ( pIDInfo )
			{
				mapdef_AgentList::iterator itAgent = m_defAgentList.find( fAgentRadius );
				if ( itAgent == m_defAgentList.end() )
				{
					return eCOL_TEST_RESULT_FAILED;
				}

				// ÀÎµµ¿öÀÇ °æ¿ì ÇÏ³ªÀÇ navigation mesh ¸¸ Á¸ÀçÇÔ. ±×·¸Áö ¾Ê´Â °æ¿ìÀÇ Data°¡ Àß¸øµÈ °ÍÀÓ
				if ( m_defGroupDataList.size() != 1 )
				{
					return eCOL_TEST_RESULT_FAILED;
				}

				sGROUP_DATA& sGroupData = m_defGroupDataList.begin()->second;
				if ( NULL == sGroupData.pGroundMesh )
				{
					return eCOL_TEST_RESULT_FAILED;
				}

				tSigned32 arTemp[3];
				arTemp[0] = (tSigned32) WORLD_COORD_TO_PATH_COORD( x );
				arTemp[1] = (tSigned32) WORLD_COORD_TO_PATH_COORD( z );
				arTemp[2] = (tSigned32) WORLD_COORD_TO_PATH_COORD( vSourcePos.GetY() );

				// PathEngine»óÀÇ ÁÂÇ¥·Î º¯È¯
				cPosition cSourcePos = sGroupData.pGroundMesh->positionNear3DPoint( arTemp, nHorizRange, PATH_VERT_RANGE );
				if ( cSourcePos.cell == -1 )
				{
					return eCOL_TEST_RESULT_INVALID_SRC_POS;
				}

				arTemp[0] = (tSigned32) WORLD_COORD_TO_PATH_COORD( vTargetPos.GetX() );
				arTemp[1] = (tSigned32) WORLD_COORD_TO_PATH_COORD( vTargetPos.GetZ() );
				arTemp[2] = (tSigned32) WORLD_COORD_TO_PATH_COORD( vTargetPos.GetY() );
				cPosition cTargetPos = sGroupData.pGroundMesh->positionNear3DPoint( arTemp, nHorizRange, PATH_VERT_RANGE );

				if ( cTargetPos.cell == -1 )
				{
					return eCOL_TEST_RESULT_INVALID_DEST_POS;
				}

				iCollisionContext* pContext = NULL;
				if ( hHandle )
				{
					sNAVI_INST_HANDLER* pHandler = (sNAVI_INST_HANDLER*) hHandle;

					sINNER_INST_HANDLER* pInnerHandler = (sINNER_INST_HANDLER*)pHandler->pInnerData;
					if ( pInnerHandler )
					{
						mapdef_ColContextList::iterator itColContext = pInnerHandler->defColContextList.find( 0 );

						if ( pInnerHandler->defColContextList.end() != itColContext )
						{
							pContext = itColContext->second;
						}
					}
				}

				eCOL_TEST_RESULT eTestResult = eCOL_TEST_RESULT_FAILED;
				if( sGroupData.pGroundMesh->testLineCollision( itAgent->second.pShape, pContext, cSourcePos, cTargetPos ) )
					eTestResult = eCOL_TEST_RESULT_COL;
				else
					eTestResult = eCOL_TEST_RESULT_NO_COL;
				
				return eTestResult;
			}
		}
		break;
	}

	return eCOL_TEST_RESULT_FAILED;
}

eCOL_TEST_RESULT CNtlNaviPEWorld::CollisionTest(NAVI_INST_HANDLE hHandle, float fAgentRadius, CNtlNaviVector3& vSourcePos, CNtlNaviVector3& vTargetPos)
{
	return CollisionTest(hHandle, fAgentRadius, vSourcePos, vTargetPos, PATH_HORIZ_RANGE);
}

/**
* \brief 
*/
eCOL_TEST_RESULT CNtlNaviPEWorld::FirstCollisionTest( NAVI_INST_HANDLE hHandle, float fAgentRadius, CNtlNaviVector3& vSourcePos, CNtlNaviVector3& vTargetPos, CNtlNaviVector3& vFirstCollison )
{
	// 현재 PathEngine data가 완전히 로딩되어 있지 않다면 연산을 수행할 수 없다.
	if (eNAVI_PE_STATE_COMPLETE != GetCurState())
	{
		return eCOL_TEST_RESULT_FAILED;
	}

	float x = vSourcePos.GetX();
	float z = vSourcePos.GetZ();

	switch (m_pNaviDataMng->GetLoadedWorld()->GetType())
	{
	case eNAVI_INFO_WORLD_OUTDOOR:
	{
		CNtlNaviWorldOutDoorInfo* pODInfo = (CNtlNaviWorldOutDoorInfo*)m_pNaviDataMng->GetLoadedWorld();

		// OutDoor 정보를 가져와서 유효할 때만 검사한다.
		if (pODInfo)
		{
			// 현재 Agent list에서 유효한 반지름의 Agent를 꺼내온다. 없다면 충돌 실패를 리턴한다.
			mapdef_AgentList::iterator itAgent = m_defAgentList.find(fAgentRadius);
			if (itAgent == m_defAgentList.end())
			{
				return eCOL_TEST_RESULT_FAILED;
			}

			// 현재 OutDoor의 전체 크기 계산
			float fMinPosX, fMinPosZ;
			float fMaxPosX, fMaxPosZ;

			pODInfo->GetWorldMinPos(fMinPosX, fMinPosZ);
			pODInfo->GetWorldMaxPos(fMaxPosX, fMaxPosZ);

			// Field id 계산
			float fFieldSize = pODInfo->GetFieldSize();
			float fGroupSize = fFieldSize * pODInfo->GetCrossFieldCntOfGroup();

			unsigned int uiCrossGroupCnt = (unsigned int)((fMaxPosX - fMinPosX) / fGroupSize);	// Group의 행 계산
			unsigned int uiGroupX = (unsigned int)((x - fMinPosX) / fGroupSize);				// Group의 X 계산
			unsigned int uiGroupZ = (unsigned int)((z - fMinPosZ) / fGroupSize);				// Group의 Z 계산
			unsigned int uiGroupID = uiGroupX + uiGroupZ * uiCrossGroupCnt;						// Group ID

			// Group의 ID로 현재 X Y 가 속해 있는 Group을 꺼내온다.
			mapdef_GroupDataList::iterator itGroup = m_defGroupDataList.find(uiGroupID);
			if (itGroup == m_defGroupDataList.end())
			{
				return eCOL_TEST_RESULT_FAILED;
			}

			// Group Data를 꺼내온다.
			sGROUP_DATA& sGroupData = itGroup->second;
			if (NULL == sGroupData.pGroundMesh)
			{
				return eCOL_TEST_RESULT_FAILED;
			}

			tSigned32 arTemp[3];

			// Source의 월드 좌표 -> 패스엔진 좌표
			arTemp[0] = (tSigned32)WORLD_COORD_TO_PATH_COORD(x);
			arTemp[1] = (tSigned32)WORLD_COORD_TO_PATH_COORD(z);
			arTemp[2] = (tSigned32)WORLD_COORD_TO_PATH_COORD(vSourcePos.GetY());
			cPosition cSourcePos = sGroupData.pGroundMesh->positionNear3DPoint(arTemp, PATH_HORIZ_RANGE, PATH_VERT_RANGE);
			if (cSourcePos.cell == -1)
			{
				return eCOL_TEST_RESULT_INVALID_SRC_POS;
			}

			// Target의 월드 좌표 -> 패스엔진 좌표
			/*arTemp[0] = (tSigned32) WORLD_COORD_TO_PATH_COORD( vTargetPos.GetX() );
			arTemp[1] = (tSigned32) WORLD_COORD_TO_PATH_COORD( vTargetPos.GetZ() );
			arTemp[2] = (tSigned32) WORLD_COORD_TO_PATH_COORD( vTargetPos.GetY() );
			cPosition cTargetPos = sGroupData.pGroundMesh->positionNear3DPoint( arTemp, PATH_HORIZ_RANGE, PATH_VERT_RANGE );
			if ( cTargetPos.cell == -1 )
			{
				return eCOL_TEST_RESULT_INVALID_DEST_POS;
			}*/

			// FirstCollisionTest에서는 TargetPos가 Valid 할 필요가 없다.
			cPosition cTargetPos;
			cTargetPos.x = (tSigned32)WORLD_COORD_TO_PATH_COORD(vTargetPos.GetX());
			cTargetPos.y = (tSigned32)WORLD_COORD_TO_PATH_COORD(vTargetPos.GetZ());

			// 현재 GroupID의 알맞는 Collision context 를 꺼내온다.
			iCollisionContext* pContext = NULL;
			if (hHandle)
			{
				sNAVI_INST_HANDLER* pHandler = (sNAVI_INST_HANDLER*)hHandle;
				sINNER_INST_HANDLER* pInnerHandler = (sINNER_INST_HANDLER*)pHandler->pInnerData;

				if (pInnerHandler)
				{
					mapdef_ColContextList::iterator itColContext = pInnerHandler->defColContextList.find(uiGroupID);
					if (pInnerHandler->defColContextList.end() != itColContext)
					{
						pContext = itColContext->second;
					}
				}
			}

			eCOL_TEST_RESULT eTestResult = eCOL_TEST_RESULT_FAILED;
			iCollisionInfo* pCollisionInfo = sGroupData.pGroundMesh->firstCollision(itAgent->second.pShape, pContext, cSourcePos, cTargetPos.x, cTargetPos.y, cTargetPos.cell);
			if (pCollisionInfo)
			{
				// 충돌된 면의 Edge가 리턴된다.
				long lCollisionCoord[4];
				pCollisionInfo->getCollidingLine(lCollisionCoord);
				delete pCollisionInfo;

				// Source와 Dest의 라인 과 충돌된 면의 Edge의 교점을 구한다.
				float fSourceLineStartX = vSourcePos.GetX();
				float fSourceLineStartY = vSourcePos.GetZ();
				float fSourceLineEndX = vTargetPos.GetX();
				float fSourceLineEndY = vTargetPos.GetZ();

				float fTargetLineStartX = (float)PATH_COORD_TO_WORLD_COORD(lCollisionCoord[0]);
				float fTargetLineStartY = (float)PATH_COORD_TO_WORLD_COORD(lCollisionCoord[1]);
				float fTargetLineEndX = (float)PATH_COORD_TO_WORLD_COORD(lCollisionCoord[2]);
				float fTargetLineEndY = (float)PATH_COORD_TO_WORLD_COORD(lCollisionCoord[3]);

				float fCrossX, fCrossY;

				int nResult = LineToLineIntersect2D(
					fSourceLineStartX, fSourceLineStartY, fSourceLineEndX, fSourceLineEndY,
					fTargetLineStartX, fTargetLineStartY, fTargetLineEndX, fTargetLineEndY,
					&fCrossX, &fCrossY);

				vFirstCollison.SetElem(fCrossX, vSourcePos.GetY(), fCrossY);

				switch (nResult)
				{
				case LCTR_INTERSECT:
					eTestResult = eCOL_TEST_RESULT_COL;
					break;
				case LCTR_NO_COLLISION:
				case LCTR_PARALLEL:
				default:
					eTestResult = eCOL_TEST_RESULT_NO_COL;
					break;
				}
			}
			else
			{
				eTestResult = eCOL_TEST_RESULT_NO_COL;
			}

			return  eTestResult;
		}
	}
	break;

	case eNAVI_INFO_WORLD_INDOOR:
	{
		CNtlNaviWorldInDoorInfo* pIDInfo = (CNtlNaviWorldInDoorInfo*)m_pNaviDataMng->GetLoadedWorld();

		if (pIDInfo)
		{
			mapdef_AgentList::iterator itAgent = m_defAgentList.find(fAgentRadius);
			if (itAgent == m_defAgentList.end())
			{
				return eCOL_TEST_RESULT_FAILED;
			}

			// 인도워의 경우 하나의 navigation mesh 만 존재함. 그렇지 않는 경우의 Data가 잘못된 것임
			if (m_defGroupDataList.size() != 1)
			{
				return eCOL_TEST_RESULT_FAILED;
			}

			sGROUP_DATA& sGroupData = m_defGroupDataList.begin()->second;
			if (NULL == sGroupData.pGroundMesh)
			{
				return eCOL_TEST_RESULT_FAILED;
			}

			tSigned32 arTemp[3];
			arTemp[0] = (tSigned32)WORLD_COORD_TO_PATH_COORD(x);
			arTemp[1] = (tSigned32)WORLD_COORD_TO_PATH_COORD(z);
			arTemp[2] = (tSigned32)WORLD_COORD_TO_PATH_COORD(vSourcePos.GetY());

			// PathEngine상의 좌표로 변환
			cPosition cSourcePos = sGroupData.pGroundMesh->positionNear3DPoint(arTemp, PATH_HORIZ_RANGE, PATH_VERT_RANGE);
			if (cSourcePos.cell == -1)
			{
				return eCOL_TEST_RESULT_INVALID_SRC_POS;
			}

			/*arTemp[0] = (tSigned32) WORLD_COORD_TO_PATH_COORD( vTargetPos.GetX() );
			arTemp[1] = (tSigned32) WORLD_COORD_TO_PATH_COORD( vTargetPos.GetZ() );
			arTemp[2] = (tSigned32) WORLD_COORD_TO_PATH_COORD( vTargetPos.GetY() );
			cPosition cTargetPos = sGroupData.pGroundMesh->positionNear3DPoint( arTemp, PATH_HORIZ_RANGE, PATH_VERT_RANGE );

			if ( cTargetPos.cell == -1 )
			{
				return eCOL_TEST_RESULT_INVALID_DEST_POS;
			}*/

			// FirstCollisionTest에서는 TargetPos가 Valid 할 필요가 없다.
			cPosition cTargetPos;
			cTargetPos.x = (tSigned32)WORLD_COORD_TO_PATH_COORD(vTargetPos.GetX());
			cTargetPos.y = (tSigned32)WORLD_COORD_TO_PATH_COORD(vTargetPos.GetZ());

			iCollisionContext* pContext = NULL;
			if (hHandle)
			{
				sNAVI_INST_HANDLER* pHandler = (sNAVI_INST_HANDLER*)hHandle;

				sINNER_INST_HANDLER* pInnerHandler = (sINNER_INST_HANDLER*)pHandler->pInnerData;
				if (pInnerHandler)
				{
					mapdef_ColContextList::iterator itColContext = pInnerHandler->defColContextList.find(0);

					if (pInnerHandler->defColContextList.end() != itColContext)
					{
						pContext = itColContext->second;
					}
				}
			}

			eCOL_TEST_RESULT eTestResult = eCOL_TEST_RESULT_FAILED;
			iCollisionInfo* pCollisionInfo = sGroupData.pGroundMesh->firstCollision(itAgent->second.pShape, pContext, cSourcePos, cTargetPos.x, cTargetPos.y, cTargetPos.cell);
			if (pCollisionInfo)
			{
				long lCollisionCoord[4];
				pCollisionInfo->getCollidingLine(lCollisionCoord);
				delete pCollisionInfo;

				// Source와 Dest의 라인 과 충돌된 면의 Edge의 교점을 구한다.
				float fSourceLineStartX = vSourcePos.GetX();
				float fSourceLineStartY = vSourcePos.GetZ();
				float fSourceLineEndX = vTargetPos.GetX();
				float fSourceLineEndY = vTargetPos.GetZ();

				float fTargetLineStartX = (float)PATH_COORD_TO_WORLD_COORD(lCollisionCoord[0]);
				float fTargetLineStartY = (float)PATH_COORD_TO_WORLD_COORD(lCollisionCoord[1]);
				float fTargetLineEndX = (float)PATH_COORD_TO_WORLD_COORD(lCollisionCoord[2]);
				float fTargetLineEndY = (float)PATH_COORD_TO_WORLD_COORD(lCollisionCoord[3]);

				float fCrossX, fCrossY;

				int nResult = LineToLineIntersect2D(
					fSourceLineStartX, fSourceLineStartY, fSourceLineEndX, fSourceLineEndY,
					fTargetLineStartX, fTargetLineStartY, fTargetLineEndX, fTargetLineEndY,
					&fCrossX, &fCrossY);

				vFirstCollison.SetElem(fCrossX, vSourcePos.GetY(), fCrossY);

				switch (nResult)
				{
				case LCTR_INTERSECT:
					eTestResult = eCOL_TEST_RESULT_COL;
					break;
				case LCTR_NO_COLLISION:
				case LCTR_PARALLEL:
				default:
					eTestResult = eCOL_TEST_RESULT_NO_COL;
					break;
				}
			}
			else
			{
				eTestResult = eCOL_TEST_RESULT_NO_COL;
			}

			return eTestResult;
		}
	}
	break;
	}

	return eCOL_TEST_RESULT_FAILED;
}

bool CNtlNaviPEWorld::FindPath( NAVI_INST_HANDLE hHandle, float fAgentRadius, CNtlNaviVector3& vSourcePos, CNtlNaviVector3& vTargetPos, vecdef_NaviPosList& defNaviPosList )
{
	if ( eNAVI_PE_STATE_COMPLETE != GetCurState() )
	{
		return false;
	}

	float x = vSourcePos.GetX();
	float z = vSourcePos.GetZ();

	switch ( m_pNaviDataMng->GetLoadedWorld()->GetType() )
	{
	case eNAVI_INFO_WORLD_OUTDOOR:
		{
			CNtlNaviWorldOutDoorInfo* pODInfo = (CNtlNaviWorldOutDoorInfo*)m_pNaviDataMng->GetLoadedWorld();

			if ( pODInfo )
			{
				mapdef_AgentList::iterator itAgent = m_defAgentList.find( fAgentRadius );

				if ( itAgent == m_defAgentList.end() )
				{
					return false;
				}

				float fMinPosX, fMinPosZ;
				float fMaxPosX, fMaxPosZ;

				pODInfo->GetWorldMinPos( fMinPosX, fMinPosZ );
				pODInfo->GetWorldMaxPos( fMaxPosX, fMaxPosZ );

				// Field id °è»ê

				float fFieldSize = pODInfo->GetFieldSize();
				float fGroupSize = fFieldSize * pODInfo->GetCrossFieldCntOfGroup();

				unsigned int uiCrossGroupCnt = (unsigned int)((fMaxPosX - fMinPosX) / fGroupSize);
				unsigned int uiGroupX = (unsigned int)((x - fMinPosX) / fGroupSize);
				unsigned int uiGroupZ = (unsigned int)((z - fMinPosZ) / fGroupSize);
				unsigned int uiGroupID = uiGroupX + uiGroupZ * uiCrossGroupCnt;

				mapdef_GroupDataList::iterator itGroup = m_defGroupDataList.find( uiGroupID );

				if ( itGroup == m_defGroupDataList.end() )
				{
					return false;
				}

				sGROUP_DATA& sGroupData = itGroup->second;

				if ( NULL == sGroupData.pGroundMesh )
				{
					return false;
				}

				tSigned32 arTemp[3];

				arTemp[0] = (tSigned32) WORLD_COORD_TO_PATH_COORD( x );
				arTemp[1] = (tSigned32) WORLD_COORD_TO_PATH_COORD( z );
				arTemp[2] = (tSigned32) WORLD_COORD_TO_PATH_COORD( vSourcePos.GetY() );

				cPosition cSourcePos = sGroupData.pGroundMesh->positionNear3DPoint( arTemp, PATH_HORIZ_RANGE, PATH_VERT_RANGE );

				if ( cSourcePos.cell == -1 )
				{
					return false;
				}

				arTemp[0] = (tSigned32) WORLD_COORD_TO_PATH_COORD( vTargetPos.GetX() );
				arTemp[1] = (tSigned32) WORLD_COORD_TO_PATH_COORD( vTargetPos.GetZ() );
				arTemp[2] = (tSigned32) WORLD_COORD_TO_PATH_COORD( vTargetPos.GetY() );

				cPosition cTargetPos = sGroupData.pGroundMesh->positionNear3DPoint( arTemp, PATH_HORIZ_RANGE, PATH_VERT_RANGE );

				if ( cTargetPos.cell == -1 )
				{
					return false;
				}

				iCollisionContext* pContext = NULL;

				if ( hHandle )
				{
					sNAVI_INST_HANDLER* pHandler = (sNAVI_INST_HANDLER*) hHandle;

					sINNER_INST_HANDLER* pInnerHandler = (sINNER_INST_HANDLER*)pHandler->pInnerData;

					if ( pInnerHandler )
					{
						mapdef_ColContextList::iterator itColContext = pInnerHandler->defColContextList.find( uiGroupID );

						if ( pInnerHandler->defColContextList.end() != itColContext )
						{
							pContext = itColContext->second;
						}
					}
				}

				iPath* pPathRet = sGroupData.pGroundMesh->findShortestPath( itAgent->second.pShape, pContext, cSourcePos, cTargetPos );

				if ( pPathRet )
				{
					sNAVI_POS sPos;
					cPosition clPos;

					tUnsigned32 uiPathPosCnt = pPathRet->size();

					defNaviPosList.clear();
					defNaviPosList.reserve( uiPathPosCnt );

					for ( tUnsigned32 i = 0; i < uiPathPosCnt; ++i )
					{
						clPos = pPathRet->position( i );

						sPos.x = (float)PATH_COORD_TO_WORLD_COORD( clPos.x );
						sPos.z = (float)PATH_COORD_TO_WORLD_COORD( clPos.y );
						sPos.y = (float)PATH_COORD_TO_WORLD_COORD( sGroupData.pGroundMesh->heightAtPositionF( clPos ) );

						defNaviPosList.push_back( sPos );
					}

					pPathRet->destroy();
				}

				return pPathRet ? true : false;
			}
		}
		break;

	case eNAVI_INFO_WORLD_INDOOR:
		{
			CNtlNaviWorldInDoorInfo* pIDInfo = (CNtlNaviWorldInDoorInfo*)m_pNaviDataMng->GetLoadedWorld();

			if ( pIDInfo )
			{
				mapdef_AgentList::iterator itAgent = m_defAgentList.find( fAgentRadius );

				if ( itAgent == m_defAgentList.end() )
				{
					return false;
				}

				// ÀÎµµ¿öÀÇ °æ¿ì ÇÏ³ªÀÇ navigation mesh ¸¸ Á¸ÀçÇÔ
				if ( m_defGroupDataList.size() != 1 )
				{
					return false;
				}

				sGROUP_DATA& sGroupData = m_defGroupDataList.begin()->second;

				if ( NULL == sGroupData.pGroundMesh )
				{
					return false;
				}

				tSigned32 arTemp[3];

				arTemp[0] = (tSigned32) WORLD_COORD_TO_PATH_COORD( x );
				arTemp[1] = (tSigned32) WORLD_COORD_TO_PATH_COORD( z );
				arTemp[2] = (tSigned32) WORLD_COORD_TO_PATH_COORD( vSourcePos.GetY() );

				cPosition cSourcePos = sGroupData.pGroundMesh->positionNear3DPoint( arTemp, PATH_HORIZ_RANGE, PATH_VERT_RANGE );

				if ( cSourcePos.cell == -1 )
				{
					return false;
				}

				arTemp[0] = (tSigned32) WORLD_COORD_TO_PATH_COORD( vTargetPos.GetX() );
				arTemp[1] = (tSigned32) WORLD_COORD_TO_PATH_COORD( vTargetPos.GetZ() );
				arTemp[2] = (tSigned32) WORLD_COORD_TO_PATH_COORD( vTargetPos.GetY() );

				cPosition cTargetPos = sGroupData.pGroundMesh->positionNear3DPoint( arTemp, PATH_HORIZ_RANGE, PATH_VERT_RANGE );

				if ( cTargetPos.cell == -1 )
				{
					return false;
				}

				iCollisionContext* pContext = NULL;

				if ( hHandle )
				{
					sNAVI_INST_HANDLER* pHandler = (sNAVI_INST_HANDLER*) hHandle;

					sINNER_INST_HANDLER* pInnerHandler = (sINNER_INST_HANDLER*)pHandler->pInnerData;

					if ( pInnerHandler )
					{
						mapdef_ColContextList::iterator itColContext = pInnerHandler->defColContextList.find( 0 );

						if ( pInnerHandler->defColContextList.end() != itColContext )
						{
							pContext = itColContext->second;
						}
					}
				}

				iPath* pPathRet = sGroupData.pGroundMesh->findShortestPath( itAgent->second.pShape, pContext, cSourcePos, cTargetPos );

				if ( pPathRet )
				{
					sNAVI_POS sPos;
					cPosition clPos;

					tUnsigned32 uiPathPosCnt = pPathRet->size();

					defNaviPosList.clear();
					defNaviPosList.reserve( uiPathPosCnt );

					for ( tUnsigned32 i = 0; i < uiPathPosCnt; ++i )
					{
						clPos = pPathRet->position( i );

						sPos.x = (float)PATH_COORD_TO_WORLD_COORD( clPos.x );
						sPos.z = (float)PATH_COORD_TO_WORLD_COORD( clPos.y );
						sPos.y = (float)PATH_COORD_TO_WORLD_COORD( sGroupData.pGroundMesh->heightAtPositionF( clPos ) );

						defNaviPosList.push_back( sPos );
					}

					pPathRet->destroy();
				}

				return pPathRet ? true : false;
			}
		}
		break;
	}

	return eCOL_TEST_RESULT_FAILED;
}

bool CNtlNaviPEWorld::IsValidPos(sNAVI_POS& sSrcPos)
{
	if (GetCurState() != CNtlNaviPEWorld::eNAVI_PE_STATE_COMPLETE)
	{
		return false;
	}

	sGROUP_DATA sGroupData;
	GetGroupData(sGroupData, sSrcPos.x, sSrcPos.z);

	if (!sGroupData.pGroundMesh)
	{
		return false;
	}

	tSigned32 arPos[3];
	arPos[0] = static_cast<tSigned32>(WORLD_COORD_TO_PATH_COORD(sSrcPos.x));
	arPos[1] = static_cast<tSigned32>(WORLD_COORD_TO_PATH_COORD(sSrcPos.z));
	arPos[2] = static_cast<tSigned32>(WORLD_COORD_TO_PATH_COORD(sSrcPos.y));

	// Find the position in the 3D space and return whether it's valid or not.
	cPosition cPosition = sGroupData.pGroundMesh->positionNear3DPoint(arPos, PATH_HORIZ_RANGE, PATH_VERT_RANGE);
	return cPosition.cell != -1;
}

int CNtlNaviPEWorld::GetGroundVertexCount()
{
	int nFacesCount = 0;
	for each( std::pair< unsigned int, sGROUP_DATA > pair in m_defGroupDataList )
	{
		iMesh* pMesh = pair.second.pGroundMesh;
		nFacesCount += pMesh->getNumberOf3DFaces();
	}

	return nFacesCount * 3;
}

bool CNtlNaviPEWorld::GetGroundVertexToBuffer( sNAVI_VERTEX* pBuffer )
{
	int nVertexCount = 0;
	sNAVI_PE_VERTEX sPEVertex;
	for each( std::pair< unsigned int, sGROUP_DATA > pair in m_defGroupDataList )
	{
		iMesh* pMesh = pair.second.pGroundMesh;
		int nFaceCount = pMesh->getNumberOf3DFaces();

		for( int i=0; i < nFaceCount; ++i )
		{
			for( int j=0; j < 3; ++j )
			{
				pMesh->get3DFaceVertex( i, j, sPEVertex.x, sPEVertex.y, sPEVertex.z );

				// Path coordination to world coordination
				pBuffer[nVertexCount].x = (float)PATH_COORD_TO_WORLD_COORD( sPEVertex.x );
				pBuffer[nVertexCount].y = (float)PATH_COORD_TO_WORLD_COORD( sPEVertex.y );
				pBuffer[nVertexCount].z = (float)PATH_COORD_TO_WORLD_COORD( sPEVertex.z );
				
				nVertexCount++;
			}
		}	
	}

	return true;
}

iMesh* CNtlNaviPEWorld::GetNearestMesh( CNtlNaviVector3& vPos )
{
	if ( eNAVI_PE_STATE_COMPLETE != GetCurState() )
	{
		return false;
	}

	float x = vPos.GetX();
	float z = vPos.GetZ();

	switch ( m_pNaviDataMng->GetLoadedWorld()->GetType() )
	{
	case eNAVI_INFO_WORLD_OUTDOOR:
		{
			CNtlNaviWorldOutDoorInfo* pODInfo = (CNtlNaviWorldOutDoorInfo*)m_pNaviDataMng->GetLoadedWorld();

			if ( pODInfo )
			{
				/*mapdef_AgentList::iterator itAgent = m_defAgentList.find( lAgentRadius );

				if ( itAgent == m_defAgentList.end() )
				{
					return false;
				}*/

				float fMinPosX, fMinPosZ;
				float fMaxPosX, fMaxPosZ;

				pODInfo->GetWorldMinPos( fMinPosX, fMinPosZ );
				pODInfo->GetWorldMaxPos( fMaxPosX, fMaxPosZ );

				// Field id °è»ê

				float fFieldSize = pODInfo->GetFieldSize();
				float fGroupSize = fFieldSize * pODInfo->GetCrossFieldCntOfGroup();

				unsigned int uiCrossGroupCnt = (unsigned int)((fMaxPosX - fMinPosX) / fGroupSize);
				unsigned int uiGroupX = (unsigned int)((x - fMinPosX) / fGroupSize);
				unsigned int uiGroupZ = (unsigned int)((z - fMinPosZ) / fGroupSize);
				unsigned int uiGroupID = uiGroupX + uiGroupZ * uiCrossGroupCnt;

				mapdef_GroupDataList::iterator itGroup = m_defGroupDataList.find( uiGroupID );

				// ÇØ´çÇÏ´Â Group ID¸¦ Ã£Áö ¸øÇÔ
				if ( itGroup == m_defGroupDataList.end() )
				{
					return NULL;
				}

				sGROUP_DATA& sGroupData = itGroup->second;
				return sGroupData.pGroundMesh;
			}
		}
		break;

	case eNAVI_INFO_WORLD_INDOOR:
		{
			// ÀÎµµ¿öÀÇ °æ¿ì ÇÏ³ªÀÇ navigation mesh ¸¸ Á¸ÀçÇÔ
			if ( m_defGroupDataList.size() != 1 )
			{
				return NULL;
			}

			sGROUP_DATA& sGroupData = m_defGroupDataList.begin()->second;
			return sGroupData.pGroundMesh;
		}

		break;
	}

	return NULL;
}

void CNtlNaviPEWorld::Destroy( void )
{
	while ( !m_defLoadingEntityList.empty() )
	{
		if ( CNtlLoadingQueue::GetInstance() )
		{
			vecdef_LoadingEntityList::iterator it = m_defLoadingEntityList.begin();
			for ( ; it != m_defLoadingEntityList.end(); )
			{
				if ( CNtlLoadingQueue::GetInstance()->DetachEntityToLoad( *it ) ||
					CNtlLoadingQueue::GetInstance()->DetachEntityLoaded( *it ) )
				{
					it = m_defLoadingEntityList.erase( it );
				}
				else
				{
					++it;
				}
			}
		}
		else
		{
			vecdef_LoadingEntityList::iterator it = m_defLoadingEntityList.begin();
			for ( ; it != m_defLoadingEntityList.end(); ++it )
			{
				delete (*it);
			}
			m_defLoadingEntityList.clear();
		}
	}

	DestroyWorldProperty_ID();
	DestroyWorldProperty_OD();

	DestroyWorldGroup();

	if ( m_pNaviDataMng )
	{
		m_pNaviDataMng->Delete();
	}

	m_byLoadedFlags = ePATH_DATA_LOAD_FLAG_NO_LOADING;
}

bool CNtlNaviPEWorld::ImportWorldInfo( const char* pPath )
{
	// Navigation data manager »ý¼º
	if ( !m_pNaviDataMng->Create( pPath ) )
	{
		CNtlNaviLog::GetInstance()->Log( "[IMPORT] Creating the navi data manager failed. [%s]", pPath );

		return false;
	}

	// Navigation world info ·Îµù
	if ( NULL == m_pNaviDataMng->Load_World() )
	{
		CNtlNaviLog::GetInstance()->Log( "[IMPORT] Can not import world info data. [%s]", pPath );

		return false;
	}

	return true;
}

bool CNtlNaviPEWorld::ImportWorldProperty( const char* pPath )
{
	switch ( m_pNaviDataMng->GetLoadedWorld()->GetType() )
	{
	case eNAVI_INFO_WORLD_OUTDOOR:
		return ImportWorldProperty_OD( pPath );

	case eNAVI_INFO_WORLD_INDOOR:
		return ImportWorldProperty_ID( pPath );
	}

	return false;
}

bool CNtlNaviPEWorld::ImportWorldProperty_OD( const char* pPath )
{
	// Build import path
	std::string strWorldProp = pPath;
	AttachBackSlash( strWorldProp );

	strWorldProp += PE_PROPERTY_FOLDER;

	// Import world property
	CNtlNaviWorldOutDoorInfo* pWorldOD = (CNtlNaviWorldOutDoorInfo*)m_pNaviDataMng->GetLoadedWorld();

	float fMinPosX, fMinPosZ, fMaxPosX, fMaxPosZ;
	pWorldOD->GetWorldMinPos( fMinPosX, fMinPosZ );
	pWorldOD->GetWorldMaxPos( fMaxPosX, fMaxPosZ );

	float fFieldSize = pWorldOD->GetFieldSize();
	unsigned int uiFieldCntOfWorld = (unsigned int)( (fMaxPosX - fMinPosX) / fFieldSize );

	for ( float fZ = fMinPosZ; fZ < fMaxPosZ; fZ += fFieldSize )
	{
		for ( float fX = fMinPosX; fX < fMaxPosX; fX += fFieldSize )
		{
			unsigned int uiFieldID = (unsigned int) ( (fX - fMinPosX) / fFieldSize + (fZ - fMinPosZ) / fFieldSize * uiFieldCntOfWorld );

			AttachEntityToLoad( new CNtlNaviPE_ODProp( uiFieldID, m_pNaviDataMng, strWorldProp ) );
		}
	}

	return true;
}

void CNtlNaviPEWorld::DestroyWorldProperty_OD( void )
{
	mapdef_ODPropList::iterator it = m_defODPropList.begin();
	for ( ; it != m_defODPropList.end(); ++it )
	{
		CNtlNaviInfo* pNaviInfo = it->second;
		m_pNaviDataMng->Unload_NaviInfo( pNaviInfo );
	}
	m_defODPropList.clear();
}

bool CNtlNaviPEWorld::ImportWorldProperty_ID( const char* pPath )
{
	// Build import path
	std::string strWorldProp = pPath;
	AttachBackSlash( strWorldProp );

	strWorldProp += PE_PROPERTY_FOLDER;

	// Import world property
	CNtlNaviWorldInDoorInfo* pWorldID = (CNtlNaviWorldInDoorInfo*)m_pNaviDataMng->GetLoadedWorld();

	float fMinPosX, fMinPosZ, fMaxPosX, fMaxPosZ;
	pWorldID->GetWorldMinPos( fMinPosX, fMinPosZ );
	pWorldID->GetWorldMaxPos( fMaxPosX, fMaxPosZ );

	float fBlockSize = pWorldID->GetBlockSize();
	unsigned int uiBlockCntOfWorld = (unsigned int)( (fMaxPosX - fMinPosX) / fBlockSize );

	for ( float fZ = fMinPosZ; fZ < fMaxPosZ; fZ += fBlockSize )
	{
		for ( float fX = fMinPosX; fX < fMaxPosX; fX += fBlockSize )
		{
			unsigned int uiBlockID = (unsigned int) ( (fX - fMinPosX) / fBlockSize + (fZ - fMinPosZ) / fBlockSize * uiBlockCntOfWorld );

			AttachEntityToLoad( new CNtlNaviPE_IDProp( uiBlockID, m_pNaviDataMng, strWorldProp ) );
		}
	}

	return true;
}

void CNtlNaviPEWorld::DestroyWorldProperty_ID( void )
{
	mapdef_IDPropList::iterator it = m_defIDPropList.begin();
	for ( ; it != m_defIDPropList.end(); ++it )
	{
		CNtlNaviInfo* pNaviInfo = it->second;
		m_pNaviDataMng->Unload_NaviInfo( pNaviInfo );
	}
	m_defIDPropList.clear();
}

bool CNtlNaviPEWorld::ImportWorldGroup( const char* pPath )
{
	switch ( m_pNaviDataMng->GetLoadedWorld()->GetType() )
	{
	case eNAVI_INFO_WORLD_OUTDOOR:
		return ImportWorldGroup_OD( pPath );

	case eNAVI_INFO_WORLD_INDOOR:
		return ImportWorldGroup_ID( pPath );
	}

	return false;
}

bool CNtlNaviPEWorld::ImportWorldGroup_OD( const char* pPath )
{
	// Build import path
	std::string strImportPath = pPath;
	AttachBackSlash( strImportPath );

	strImportPath += PE_PATHENGINE_FOLDER;

	// Import world group
	CNtlNaviWorldOutDoorInfo* pWorldOD = (CNtlNaviWorldOutDoorInfo*) m_pNaviDataMng->GetLoadedWorld();

	float fRadius;
	tSigned32 arAgentCoord[NAVI_AGENT_COORD_COUNT];

	int nAgentCnt = pWorldOD->GetAgencyCnt();
	for ( int i = 0; i < nAgentCnt; ++i )
	{
		fRadius = pWorldOD->GetAgency( i );

		arAgentCoord[0] = (tSigned32) WORLD_COORD_TO_PATH_COORD( fRadius );
		arAgentCoord[1] = 0;

		arAgentCoord[2] = 0;
		arAgentCoord[3] = -(tSigned32) WORLD_COORD_TO_PATH_COORD( fRadius );

		arAgentCoord[4] = -(tSigned32) WORLD_COORD_TO_PATH_COORD( fRadius );
		arAgentCoord[5] = 0;

		arAgentCoord[6] = 0;
		arAgentCoord[7] = (tSigned32) WORLD_COORD_TO_PATH_COORD( fRadius );

		sAGENT_DATA sAgent;

		sAgent.fRadius = fRadius;
		sAgent.pShape = CNtlNaviPathEngine::GetInstance()->GetPathEngine()->newShape( sizeof( arAgentCoord ) / sizeof( *arAgentCoord ) / 2, arAgentCoord );

		if ( NULL == sAgent.pShape )
		{
			CNtlNaviLog::GetInstance()->Log( "[IMPORT] Creating shape of agent failed. [%s, %f]", pPath, fRadius );

			return false;
		}

		m_defAgentList[fRadius] = sAgent;
	}

	float fMinWorldX, fMinWorldZ;
	float fMaxWorldX, fMaxWorldZ;
	pWorldOD->GetWorldMinPos( fMinWorldX, fMinWorldZ );
	pWorldOD->GetWorldMaxPos( fMaxWorldX, fMaxWorldZ );

	unsigned int uiCrossFieldCntOfWorld = (unsigned int)((fMaxWorldX - fMinWorldX) / pWorldOD->GetFieldSize());
	unsigned int uiCrossGroupCntOfWorld = uiCrossFieldCntOfWorld / pWorldOD->GetCrossFieldCntOfGroup();

	for ( unsigned int z = 0; z < uiCrossGroupCntOfWorld; ++z )
	{
		for ( unsigned int x = 0; x < uiCrossGroupCntOfWorld; ++x )
		{
			unsigned int uiGroupID = x + z * uiCrossGroupCntOfWorld;

			CNtlNaviPE_ODGroup* pODGroup = new CNtlNaviPE_ODGroup( uiGroupID, strImportPath );

			mapdef_AgentList::iterator it = m_defAgentList.begin();
			for ( ; it != m_defAgentList.end(); ++it )
			{
				pODGroup->AttachAgent( it->second.fRadius, it->second.pShape );
			}

			AttachEntityToLoad( pODGroup );
		}
	}

	return true;
}

bool CNtlNaviPEWorld::ImportWorldGroup_ID( const char* pPath )
{
	// Build import path
	std::string strImportPath = pPath;
	AttachBackSlash( strImportPath );

	strImportPath += PE_PATHENGINE_FOLDER;

	// Import world group
	CNtlNaviWorldInDoorInfo* pWorldID = (CNtlNaviWorldInDoorInfo*) m_pNaviDataMng->GetLoadedWorld();

	float fRadius;
	tSigned32 arAgentCoord[NAVI_AGENT_COORD_COUNT];

	int nAgentCnt = pWorldID->GetAgencyCnt();
	for ( int i = 0; i < nAgentCnt; ++i )
	{
		fRadius = pWorldID->GetAgency( i );

		arAgentCoord[0] = (tSigned32) WORLD_COORD_TO_PATH_COORD( fRadius );
		arAgentCoord[1] = 0;

		arAgentCoord[2] = 0;
		arAgentCoord[3] = -(tSigned32) WORLD_COORD_TO_PATH_COORD( fRadius );

		arAgentCoord[4] = -(tSigned32) WORLD_COORD_TO_PATH_COORD( fRadius );
		arAgentCoord[5] = 0;

		arAgentCoord[6] = 0;
		arAgentCoord[7] = (tSigned32) WORLD_COORD_TO_PATH_COORD( fRadius );

		sAGENT_DATA sAgent;

		sAgent.fRadius = fRadius;
		sAgent.pShape = CNtlNaviPathEngine::GetInstance()->GetPathEngine()->newShape( sizeof( arAgentCoord ) / sizeof( *arAgentCoord ) / 2, arAgentCoord );

		if ( NULL == sAgent.pShape )
		{
			CNtlNaviLog::GetInstance()->Log( "[IMPORT] Creating shape of agent failed. [%s, %f]", pPath, fRadius );

			return false;
		}

		m_defAgentList[fRadius] = sAgent;
	}

	CNtlNaviPE_IDGroup* pIDGroup = new CNtlNaviPE_IDGroup( 0, strImportPath );

	mapdef_AgentList::iterator it = m_defAgentList.begin();
	for ( ; it != m_defAgentList.end(); ++it )
	{
		pIDGroup->AttachAgent( it->second.fRadius, it->second.pShape );
	}

	AttachEntityToLoad( pIDGroup );

	return true;
}

void CNtlNaviPEWorld::DestroyWorldGroup( void )
{
	mapdef_GroupDataList::iterator itGroupData = m_defGroupDataList.begin();
	for ( ; itGroupData != m_defGroupDataList.end(); ++itGroupData )
	{
		sGROUP_DATA& sGroupData = itGroupData->second;
		sGroupData.pGroundMesh->destroy();
	}
	m_defGroupDataList.clear();

	mapdef_AgentList::iterator itAgent = m_defAgentList.begin();
	for ( ; itAgent != m_defAgentList.end(); ++itAgent )
	{
		sAGENT_DATA& sAgentData = itAgent->second;
		sAgentData.pShape->destroy();
	}
	m_defAgentList.clear();
}

void CNtlNaviPEWorld::AttachEntityToLoad( CNtlNaviLoadingEntity* pEntity )
{
	CNtlNaviAutoCS clAuto( &m_csLoadingEntityList );

	m_defLoadingEntityList.push_back( pEntity );

	// Multi-threadÀ» »ç¿ëÇÑ ·Îµù
	if ( CNtlLoadingQueue::GetInstance() )
	{
		CNtlLoadingQueue::GetInstance()->AttachEntityToLoad( pEntity );
	}
	// Multi-threadÀ» »ç¿ëÇÏÁö ¾Ê´Â ·Îµù
	else
	{
		pEntity->RunMultiThread();
	}
}

bool CNtlNaviPEWorld::UpdateEntityToLoad( void )
{
	CNtlNaviAutoCS clAuto( &m_csLoadingEntityList );

	vecdef_LoadingEntityList::iterator it = m_defLoadingEntityList.begin();

	for ( ; it != m_defLoadingEntityList.end(); )
	{
		CNtlNaviLoadingEntity* pLoadingEntity = *it;

		if ( CNtlLoadingQueue::GetInstance() &&
			 !CNtlLoadingQueue::GetInstance()->DetachEntityLoaded( pLoadingEntity ) )
		{
			++it;

			continue;
		}

		if ( pLoadingEntity->IsError() )
		{
			delete pLoadingEntity;
			return false;
		}

		// Out door property
		CNtlNaviPE_ODProp* pODProp = dynamic_cast< CNtlNaviPE_ODProp* > ( pLoadingEntity );

		if ( pODProp )
		{
			if ( pODProp->m_pPropInfo )
			{
				m_defODPropList[pODProp->m_uiFieldID] = pODProp->m_pPropInfo;
			}

			delete pODProp;

			it = m_defLoadingEntityList.erase( it );

			continue;
		}

		// In door property
		CNtlNaviPE_IDProp* pIDProp = dynamic_cast< CNtlNaviPE_IDProp* > ( pLoadingEntity );

		if ( pIDProp )
		{
			if ( pIDProp->m_pPropInfo )
			{
				m_defIDPropList[pIDProp->m_uiBlockID] = pIDProp->m_pPropInfo;
			}

			delete pIDProp;

			it = m_defLoadingEntityList.erase( it );

			continue;
		}

		// Out door group
		CNtlNaviPE_ODGroup* pODGroup = dynamic_cast< CNtlNaviPE_ODGroup* > ( pLoadingEntity );

		if ( pODGroup )
		{
			if ( pODGroup->m_pMesh )
			{
				m_defGroupDataList[pODGroup->m_uiGroupID] = sGROUP_DATA( pODGroup->m_pMesh );
			}

			delete pODGroup;

			it = m_defLoadingEntityList.erase( it );

			continue;
		}

		// In door group
		CNtlNaviPE_IDGroup* pIDGroup = dynamic_cast< CNtlNaviPE_IDGroup* > ( pLoadingEntity );

		if ( pIDGroup )
		{
			if ( pIDGroup->m_pMesh )
			{
				m_defGroupDataList[pIDGroup->m_uiGroupID] = sGROUP_DATA( pIDGroup->m_pMesh );
			}

			delete pIDGroup;

			it = m_defLoadingEntityList.erase( it );

			continue;
		}

		CNtlNaviLog::GetInstance()->Log( "[IMPORT] Updating entity to load failed" );

		return false;
	}

	if ( NULL == CNtlLoadingQueue::GetInstance() )
	{
		SetCurState( eNAVI_PE_STATE_COMPLETE );
	}

	return true;
}

bool CNtlNaviPEWorld::IsColProp_Sphere( float x, float z, float fSX, float fSZ, float fSRadius )
{
	if ( (x - fSX) * (x - fSX) + (z - fSZ) * (z - fSZ) <= fSRadius * fSRadius )
	{
		return true;
	}

	return false;
}

bool CNtlNaviPEWorld::IsColProp_Plane( float x, float z, float fPX, float fPZ, float fPWidth, float fPDepth )
{
	float fMinX = fPX - fPWidth / 2.f;
	float fMaxX = fPX + fPWidth / 2.f;
	float fMinZ = fPZ - fPDepth / 2.f;
	float fMaxZ = fPZ + fPDepth / 2.f;

	if ( fMinX <= x &&
		 x <= fMaxX &&
		 fMinZ <= z &&
		 z <= fMaxZ )
	{
		return true;
	}

	return false;
}

unsigned int CNtlNaviPEWorld::GetGroupID(float x, float z)
{
	CNtlNaviWorldInfo* pWorldInfo = m_pNaviDataMng->GetLoadedWorld();
	if(pWorldInfo == NULL)
		return 0xFFFFFFFF;

	if (pWorldInfo->GetType() == eNAVI_INFO_WORLD_OUTDOOR)
	{
		CNtlNaviWorldOutDoorInfo* pODInfo = (CNtlNaviWorldOutDoorInfo*)m_pNaviDataMng->GetLoadedWorld();

		float fMinPosX, fMinPosZ;
		float fMaxPosX, fMaxPosZ;

		pWorldInfo->GetWorldMinPos(fMinPosX, fMinPosZ);
		pWorldInfo->GetWorldMaxPos(fMaxPosX, fMaxPosZ);

		float fGroupSize = pODInfo->GetFieldSize() * pODInfo->GetCrossFieldCntOfGroup();

		unsigned int uiCrossGroupCnt = (unsigned int)((fMaxPosX - fMinPosX) / fGroupSize);
		unsigned int uiGroupX = (unsigned int)((x - fMinPosX) / fGroupSize);
		unsigned int uiGroupZ = (unsigned int)((z - fMinPosZ) / fGroupSize);
		
		return uiGroupX + uiGroupZ * uiCrossGroupCnt;
	}

	return 0;
}

void CNtlNaviPEWorld::GetGroupData(sGROUP_DATA & result, float x, float z)
{
	CNtlNaviWorldInfo* pWorldInfo = m_pNaviDataMng->GetLoadedWorld();
	if (pWorldInfo == NULL)
		return;

	if (pWorldInfo->GetType() == eNAVI_INFO_WORLD_OUTDOOR)
	{
		CNtlNaviWorldOutDoorInfo* pODInfo = (CNtlNaviWorldOutDoorInfo*)m_pNaviDataMng->GetLoadedWorld();

		float fMinPosX, fMinPosZ;
		float fMaxPosX, fMaxPosZ;

		pWorldInfo->GetWorldMinPos(fMinPosX, fMinPosZ);
		pWorldInfo->GetWorldMaxPos(fMaxPosX, fMaxPosZ);

		float fGroupSize = pODInfo->GetFieldSize() * pODInfo->GetCrossFieldCntOfGroup();

		unsigned int uiCrossGroupCnt = (unsigned int)((fMaxPosX - fMinPosX) / fGroupSize);
		unsigned int uiGroupX = (unsigned int)((x - fMinPosX) / fGroupSize);
		unsigned int uiGroupZ = (unsigned int)((z - fMinPosZ) / fGroupSize);
		unsigned int uiGroupID = uiGroupX + uiGroupZ * uiCrossGroupCnt;

		mapdef_GroupDataList::iterator it = m_defGroupDataList.find(uiGroupID);
		if (it == m_defGroupDataList.end())
		{
			return;
		}

		result.pGroundMesh = it->second.pGroundMesh;
	}
	else if (pWorldInfo->GetType() == eNAVI_INFO_WORLD_INDOOR)
	{
		if (m_defGroupDataList.size())
		{
			result.pGroundMesh = m_defGroupDataList.begin()->second.pGroundMesh;
		}
	}
}
