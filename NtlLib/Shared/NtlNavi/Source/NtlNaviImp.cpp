#include "precomp_navi.h"
#include "NtlNaviImp.h"
#include "NtlNaviLog.h"
#include "NtlNaviPathEngine.h"
#include "NtlNaviLoadingQueue.h"
#include "NtlNaviPEDataExportMng.h"
#include "NtlNaviPEDataImportMng.h"
#include "NtlNaviPEWorld.h"


#define REG_BASIG_ATTR_CHECK_FALG( group, basic ) m_aruiBasicAttributeCheckFlag[basic] |= 0x00000001 << group


CNtlNaviImp* CNtlNaviImp::GetInstance( void )
{
	static CNtlNaviImp g_clNaviImp;
	return &g_clNaviImp;
}

CNtlNaviImp::CNtlNaviImp( void )
{
	memset( m_aruiBasicAttributeCheckFlag, 0, sizeof( m_aruiBasicAttributeCheckFlag ) );

	m_pLoadingQueue = NULL;

	m_pNaviPEDataExporter = NULL;
	m_pNaviPEDataImporter = NULL;
}

CNtlNaviImp::~CNtlNaviImp( void )
{
}

bool CNtlNaviImp::Create( INtlNaviLog* pLog, const char* pPathDllName, bool bThreadLoading )
{
	Delete();

	memset( m_aruiBasicAttributeCheckFlag, 0, sizeof( m_aruiBasicAttributeCheckFlag ) );

	REG_BASIG_ATTR_CHECK_FALG( DBO_WORLD_ATTR_GROUP_TOWN, DBO_WORLD_ATTR_BASIC_FORBID_PC_BATTLE );
	REG_BASIG_ATTR_CHECK_FALG( DBO_WORLD_ATTR_GROUP_TOWN, DBO_WORLD_ATTR_BASIC_FORBID_VEHICLE );
	REG_BASIG_ATTR_CHECK_FALG( DBO_WORLD_ATTR_GROUP_TOWN, DBO_WORLD_ATTR_BASIC_FORBID_MOB_MOVE );
	REG_BASIG_ATTR_CHECK_FALG(DBO_WORLD_ATTR_GROUP_TOWN, DBO_WORLD_ATTR_BASIC_DRAGONBALL_SCRAMBLE_SAFE_ZONE);


	REG_BASIG_ATTR_CHECK_FALG( DBO_WORLD_ATTR_GROUP_SWIM, DBO_WORLD_ATTR_BASIC_FORBID_PC_BATTLE );
	REG_BASIG_ATTR_CHECK_FALG( DBO_WORLD_ATTR_GROUP_SWIM, DBO_WORLD_ATTR_BASIC_FORBID_PC_SHOP );
	REG_BASIG_ATTR_CHECK_FALG( DBO_WORLD_ATTR_GROUP_SWIM, DBO_WORLD_ATTR_BASIC_FORBID_MOB_MOVE );
	REG_BASIG_ATTR_CHECK_FALG( DBO_WORLD_ATTR_GROUP_SWIM, DBO_WORLD_ATTR_BASIC_ENABLE_SWIM );
	REG_BASIG_ATTR_CHECK_FALG(DBO_WORLD_ATTR_GROUP_SWIM, DBO_WORLD_ATTR_BASIC_DRAGONBALL_SCRAMBLE_SAFE_ZONE);

	REG_BASIG_ATTR_CHECK_FALG( DBO_WORLD_ATTR_GROUP_FORBID_PC_BATTLE, DBO_WORLD_ATTR_BASIC_FORBID_PC_BATTLE );
	REG_BASIG_ATTR_CHECK_FALG( DBO_WORLD_ATTR_GROUP_FORBID_PC_BATTLE, DBO_WORLD_ATTR_BASIC_FORBID_MOB_MOVE );
	REG_BASIG_ATTR_CHECK_FALG(DBO_WORLD_ATTR_GROUP_FORBID_PC_BATTLE, DBO_WORLD_ATTR_BASIC_DRAGONBALL_SCRAMBLE_SAFE_ZONE);

	REG_BASIG_ATTR_CHECK_FALG( DBO_WORLD_ATTR_GROUP_FORBID_PC_SHOP, DBO_WORLD_ATTR_BASIC_FORBID_PC_SHOP );

	REG_BASIG_ATTR_CHECK_FALG( DBO_WORLD_ATTR_GROUP_FORBID_ALL_MOVE, DBO_WORLD_ATTR_BASIC_FORBID_PC_MOVE );
	REG_BASIG_ATTR_CHECK_FALG( DBO_WORLD_ATTR_GROUP_FORBID_ALL_MOVE, DBO_WORLD_ATTR_BASIC_FORBID_NPC_MOVE );
	REG_BASIG_ATTR_CHECK_FALG( DBO_WORLD_ATTR_GROUP_FORBID_ALL_MOVE, DBO_WORLD_ATTR_BASIC_FORBID_MOB_MOVE );

	REG_BASIG_ATTR_CHECK_FALG( DBO_WORLD_ATTR_GROUP_FORBID_MOB_MOVE, DBO_WORLD_ATTR_BASIC_FORBID_MOB_MOVE );

	REG_BASIG_ATTR_CHECK_FALG(DBO_WORLD_ATTR_GROUP_LAVA, DBO_WORLD_ATTR_BASIC_FORBID_PC_SHOP);
	REG_BASIG_ATTR_CHECK_FALG(DBO_WORLD_ATTR_GROUP_LAVA, DBO_WORLD_ATTR_BASIC_DAMAGE_SMALL);

	REG_BASIG_ATTR_CHECK_FALG(DBO_WORLD_ATTR_GROUP_LAVA, DBO_WORLD_ATTR_BASIC_FORBID_MOB_MOVE);
	REG_BASIG_ATTR_CHECK_FALG(DBO_WORLD_ATTR_GROUP_LAVA, DBO_WORLD_ATTR_BASIC_FORBID_PC_BATTLE);
	REG_BASIG_ATTR_CHECK_FALG(DBO_WORLD_ATTR_GROUP_LAVA, DBO_WORLD_ATTR_BASIC_FORBID_PC_SHOP);
	REG_BASIG_ATTR_CHECK_FALG(DBO_WORLD_ATTR_GROUP_LAVA, DBO_WORLD_ATTR_BASIC_ENABLE_SWIM);
	REG_BASIG_ATTR_CHECK_FALG(DBO_WORLD_ATTR_GROUP_LAVA, DBO_WORLD_ATTR_BASIC_DAMAGE_LARGE);
	REG_BASIG_ATTR_CHECK_FALG(DBO_WORLD_ATTR_GROUP_LAVA, DBO_WORLD_ATTR_BASIC_DRAGONBALL_SCRAMBLE_SAFE_ZONE);

	REG_BASIG_ATTR_CHECK_FALG(DBO_WORLD_ATTR_GROUP_DRAGONBALL_SCRAMBLE_SAFE_ZONE, DBO_WORLD_ATTR_BASIC_DRAGONBALL_SCRAMBLE_SAFE_ZONE);

	REG_BASIG_ATTR_CHECK_FALG(DBO_WORLD_ATTR_GROUP_FREE_PVP_ZONE, DBO_WORLD_ATTR_BASIC_FREE_PVP_ZONE);
	REG_BASIG_ATTR_CHECK_FALG(DBO_WORLD_ATTR_GROUP_FREE_PVP_ZONE, DBO_WORLD_ATTR_BASIC_FORBID_VEHICLE);
	REG_BASIG_ATTR_CHECK_FALG(DBO_WORLD_ATTR_GROUP_FREE_PVP_ZONE, DBO_WORLD_ATTR_BASIC_FORBID_PC_SHOP);
	REG_BASIG_ATTR_CHECK_FALG(DBO_WORLD_ATTR_GROUP_FREE_PVP_ZONE, DBO_WORLD_ATTR_BASIC_FORBID_PC_BATTLE);
	REG_BASIG_ATTR_CHECK_FALG(DBO_WORLD_ATTR_GROUP_FREE_PVP_ZONE, DBO_WORLD_ATTR_BASIC_FORBID_MOB_MOVE);
	REG_BASIG_ATTR_CHECK_FALG(DBO_WORLD_ATTR_GROUP_FREE_PVP_ZONE, DBO_WORLD_ATTR_BASIC_DRAGONBALL_SCRAMBLE_SAFE_ZONE);

	REG_BASIG_ATTR_CHECK_FALG( DBO_WORLD_ATTR_GROUP_MINI_INDOOR_TILE, DBO_WORLD_ATTR_BASIC_MINI_INDOOR );

	REG_BASIG_ATTR_CHECK_FALG( DBO_WORLD_ATTR_GROUP_MINI_INDOOR_GATE, DBO_WORLD_ATTR_BASIC_MINI_INDOOR );

	CNtlNaviLog::GetInstance()->SetLog( pLog );

	if ( !CNtlNaviPathEngine::GetInstance()->Create( pPathDllName ) )
	{
		CNtlNaviLog::GetInstance()->Log( "[PATHENGINE] Creating path engine failed. %s", pPathDllName );

		return false;
	}

	if ( bThreadLoading )
	{
		m_pLoadingQueue = new CNtlLoadingQueue;

		if ( !m_pLoadingQueue->Create() )
		{
			CNtlNaviLog::GetInstance()->Log( "[PATHENGINE] Creating loading queue failed." );

			return false;
		}
	}
	else
	{
		m_pLoadingQueue = NULL;
	}

	return true;
}

void CNtlNaviImp::Delete( void )
{
	if ( m_pNaviPEDataExporter )
	{
		delete m_pNaviPEDataExporter;
		m_pNaviPEDataExporter = NULL;
	}

	if ( m_pNaviPEDataImporter )
	{
		delete m_pNaviPEDataImporter;
		m_pNaviPEDataImporter = NULL;
	}

	if ( m_pLoadingQueue )
	{
		delete m_pLoadingQueue;
		m_pLoadingQueue = NULL;
	}

	CNtlNaviPathEngine::GetInstance()->Delete();

	CNtlNaviLog::GetInstance()->SetLog( NULL );
}

bool CNtlNaviImp::ImportWorldToolData( const char* pRootFolder )
{
	// ������ CNtlNaviPEDataExportMng�� ������ ����
	if ( m_pNaviPEDataExporter )
	{
		delete m_pNaviPEDataExporter;
		m_pNaviPEDataExporter = NULL;
	}

	// ���ο� CNtlNaviPEDataExportMng ����
	m_pNaviPEDataExporter = new CNtlNaviPEDataExportMng;

	if ( !m_pNaviPEDataExporter->Create() )
	{
		CNtlNaviLog::GetInstance()->Log( "[IMPORT] Creating path engine data export manager failed." );

		delete m_pNaviPEDataExporter;
		m_pNaviPEDataExporter = NULL;

		return false;
	}

	if ( !m_pNaviPEDataExporter->ImportWorldAll( pRootFolder ) )
	{
		CNtlNaviLog::GetInstance()->Log( "[IMPORT] Import world tool data failed. [%s]", pRootFolder );

		delete m_pNaviPEDataExporter;
		m_pNaviPEDataExporter = NULL;

		return false;
	}

	return true;
}

void CNtlNaviImp::GetListImportedWorldIDList( vecdef_WorldIDList& vecOut )
{
	if( m_pNaviPEDataExporter == NULL )
	{
		// ���� Data Export manager�� �����Ǿ� ���� �ʴ�.
		return;
	}

	m_pNaviPEDataExporter->GetListImportedWorldIDList( vecOut );
}

bool CNtlNaviImp::ExportPathEngineData( const char* pRootFolder )
{
	if ( NULL == m_pNaviPEDataExporter )
	{
		CNtlNaviLog::GetInstance()->Log( "[EXPORT] path engine data to export does not exist. [%s]", pRootFolder );

		return false;
	}

	return m_pNaviPEDataExporter->ExportWorldAll( pRootFolder );
}

bool CNtlNaviImp::ExportPathEngineDataSelect( const char* pRootFolder, mapdef_ExportList& list )
{
	if ( NULL == m_pNaviPEDataExporter )
	{
		CNtlNaviLog::GetInstance()->Log( "[EXPORT] path engine data to export does not exist. [%s]", pRootFolder );

		return false;
	}

	return m_pNaviPEDataExporter->ExportWorldList( pRootFolder, list );
}

bool CNtlNaviImp::UpdateToolData( void )
{
	if ( m_pNaviPEDataExporter )
	{
		return m_pNaviPEDataExporter->UpdateToolData();
	}

	return true;
}

int CNtlNaviImp::GetWorldVertexCount( NAVI_INST_HANDLE hHandle )
{
	if( NULL == m_pNaviPEDataImporter )
		return -1;

	CNtlNaviPEWorld* pPEWorld = m_pNaviPEDataImporter->FindNaviWorld( hHandle );
	if( pPEWorld == NULL )
		return -1;

	return pPEWorld->GetGroundVertexCount();
}

int CNtlNaviImp::GetWorldVertexCount( unsigned int uiWorldID )
{
	if( NULL == m_pNaviPEDataImporter )
		return -1;

	CNtlNaviPEWorld* pPEWorld = m_pNaviPEDataImporter->FindNaviWorld( uiWorldID );
	if( pPEWorld == NULL )
		return -1;

	return pPEWorld->GetGroundVertexCount();
}

bool CNtlNaviImp::GetWorldVertexToBuffer( NAVI_INST_HANDLE hHandle, sNAVI_VERTEX* pBuffer )
{
	if( NULL == m_pNaviPEDataImporter )
		return false;

	CNtlNaviPEWorld* pPEWorld = m_pNaviPEDataImporter->FindNaviWorld( hHandle );
	if( pPEWorld == NULL )
		return false;

	return pPEWorld->GetGroundVertexToBuffer( pBuffer );
}

bool CNtlNaviImp::GetWorldVertexToBuffer( unsigned int uiWorldID, sNAVI_VERTEX* pBuffer )
{
	if( NULL == m_pNaviPEDataImporter )
		return false;

	CNtlNaviPEWorld* pPEWorld = m_pNaviPEDataImporter->FindNaviWorld( uiWorldID );
	if( pPEWorld == NULL )
		return false;

	return pPEWorld->GetGroundVertexToBuffer( pBuffer );
}

iMesh* CNtlNaviImp::GetNearestMesh( NAVI_INST_HANDLE hHandle, sNAVI_POS& vPos )
{
	if( NULL == m_pNaviPEDataImporter )
		return NULL;

	CNtlNaviPEWorld* pPEWorld = m_pNaviPEDataImporter->FindNaviWorld( hHandle );
	if( pPEWorld == NULL )
		return NULL;

	CNtlNaviVector3 vNaviPos( vPos.x, vPos.y, vPos.z );

	return pPEWorld->GetNearestMesh( vNaviPos );
}

/**
* \brief �н� ���� ����Ÿ�� �ε��Ѵ�.
* \param pRootFolder		�н����� Data�� ��� �ִ� ���� ����
* \param defWorldIDList		������ Resource ID list
* \param byLoadFlags		Load flags
* \return ��������
*/
bool CNtlNaviImp::LoadPathEngineData( const char* pRootFolder, vecdef_WorldIDList& defWorldIDList, unsigned char byLoadFlags )
{
	if ( m_pNaviPEDataImporter )
	{
		delete m_pNaviPEDataImporter;
		m_pNaviPEDataImporter = NULL;
	}

	m_pNaviPEDataImporter = new CNtlNaviPEDataImportMng;

	if ( !m_pNaviPEDataImporter->Create() )
	{
		CNtlNaviLog::GetInstance()->Log( "[IMPORT] Import path engine data failed. [%s, %d, %d]", pRootFolder, defWorldIDList.size(), byLoadFlags );

		delete m_pNaviPEDataImporter;
		m_pNaviPEDataImporter = NULL;

		return false;
	}

	if ( !m_pNaviPEDataImporter->ImportWorlds( pRootFolder, defWorldIDList, byLoadFlags ) )
	{
		CNtlNaviLog::GetInstance()->Log( "[IMPORT] Import path engine data failed. [%s, %d, %d]", pRootFolder, defWorldIDList.size(), byLoadFlags );

		delete m_pNaviPEDataImporter;
		m_pNaviPEDataImporter = NULL;

		return false;
	}

	return true;
}

bool CNtlNaviImp::WaitUntilLoadingFinish( unsigned int uiWaitTime, unsigned int& uiResult )
{
	if ( m_pNaviPEDataImporter )
	{
		return m_pNaviPEDataImporter->WaitUntilLoadingFinish( uiWaitTime, uiResult );
	}

	return true;
}

unsigned int CNtlNaviImp::GetLoadingTime( void )
{
	if ( m_pNaviPEDataImporter )
	{
		return m_pNaviPEDataImporter->GetLoadingTime();
	}

	return 0xffffffff;
}

unsigned long CNtlNaviImp::GetTotalMemory( void )
{
	return (unsigned long)CNtlNaviPathEngine::GetInstance()->GetPathEngine()->totalMemoryAllocated();
}

NAVI_INST_HANDLE CNtlNaviImp::CreateInstanceHandler( unsigned int uiWorldID )
{
	if ( m_pNaviPEDataImporter )
	{
		return m_pNaviPEDataImporter->CreateInstanceHandler( uiWorldID );
	}

	return NULL;
}

void CNtlNaviImp::DeleteInstanceHandler( NAVI_INST_HANDLE hHandle )
{
	if ( m_pNaviPEDataImporter )
	{
		m_pNaviPEDataImporter->DeleteInstanceHandler( hHandle );
	}
}

bool CNtlNaviImp::IsPropDataLoaded( unsigned int uiWorldID )
{
	if ( NULL == m_pNaviPEDataImporter )
	{
		return false;
	}

	CNtlNaviPEWorld* pPEWorld = m_pNaviPEDataImporter->FindNaviWorld( uiWorldID );

	if ( NULL == pPEWorld )
	{
		return false;
	}

	if ( pPEWorld->GetCurState() != CNtlNaviPEWorld::eNAVI_PE_STATE_COMPLETE )
	{
		return false;
	}

	if ( pPEWorld->GetLoadedFlags() & ePATH_DATA_LOAD_FLAG_LOAD_ONLY_PROPERTY )
	{
		return true;
	}

	return false;
}

bool CNtlNaviImp::IsPathDataLoaded( unsigned int uiWorldID )
{
	if ( NULL == m_pNaviPEDataImporter )
	{
		return false;
	}

	CNtlNaviPEWorld* pPEWorld = m_pNaviPEDataImporter->FindNaviWorld( uiWorldID );

	if ( NULL == pPEWorld )
	{
		return false;
	}

	if ( pPEWorld->GetCurState() != CNtlNaviPEWorld::eNAVI_PE_STATE_COMPLETE )
	{
		return false;
	}

	if ( pPEWorld->GetLoadedFlags() & ePATH_DATA_LOAD_FLAG_LOAD_ONLY_WORLD )
	{
		return true;
	}

	return false;
}

bool CNtlNaviImp::CanSupportAgent( NAVI_INST_HANDLE hHandle, float fAgentRadius )
{
	if ( NULL == m_pNaviPEDataImporter )
	{
		return false;
	}

	CNtlNaviPEWorld* pPEWorld = m_pNaviPEDataImporter->FindNaviWorld( hHandle );

	if ( NULL == pPEWorld )
	{
		return false;
	}

	if ( pPEWorld->GetCurState() != CNtlNaviPEWorld::eNAVI_PE_STATE_COMPLETE )
	{
		return false;
	}

	return pPEWorld->CanSupportAgent( fAgentRadius );
}

/**
* \brief World �� 
* \param x		World�� X ��ǥ
* \param z		World�� Z ��ǥ
* \return
*/
unsigned int CNtlNaviImp::GetTextAllIndex( NAVI_INST_HANDLE hHandle, float x, float z )
{
	if ( NULL == m_pNaviPEDataImporter )
	{
		return 0xffffffff;
	}

	CNtlNaviPEWorld* pPEWorld = m_pNaviPEDataImporter->FindNaviWorld( hHandle );

	if ( NULL == pPEWorld )
	{
		return 0xffffffff;
	}

	if ( pPEWorld->GetCurState() != CNtlNaviPEWorld::eNAVI_PE_STATE_COMPLETE )
	{
		return 0xffffffff;
	}

	return pPEWorld->GetTextAllIndex( x, z );
}

unsigned int CNtlNaviImp::GetZoneIndex( NAVI_INST_HANDLE hHandle, float x, float z )
{
	if ( NULL == m_pNaviPEDataImporter )
	{
		return 0xffffffff;
	}

	CNtlNaviPEWorld* pPEWorld = m_pNaviPEDataImporter->FindNaviWorld( hHandle );

	if ( NULL == pPEWorld )
	{
		return 0xffffffff;
	}

	if ( pPEWorld->GetCurState() != CNtlNaviPEWorld::eNAVI_PE_STATE_COMPLETE )
	{
		return 0xffffffff;
	}

	return pPEWorld->GetZoneIndex( x, z );
}

unsigned int CNtlNaviImp::GetAttribute( NAVI_INST_HANDLE hHandle, float x, float z )
{
	if ( NULL == m_pNaviPEDataImporter )
	{
		return 0;
	}

	CNtlNaviPEWorld* pPEWorld = m_pNaviPEDataImporter->FindNaviWorld( hHandle );

	if ( NULL == pPEWorld )
	{
		return 0;
	}

	if ( pPEWorld->GetCurState() != CNtlNaviPEWorld::eNAVI_PE_STATE_COMPLETE )
	{
		return 0;
	}

	return pPEWorld->GetAttribute( x, z );
}

bool CNtlNaviImp::IsBasicAttributeSet( NAVI_INST_HANDLE hHandle, float x, float z, eDBO_WORLD_ATTR_BASIC eAttrBasic )
{
	if ( DBO_WORLD_ATTR_BASIC_FIRST > eAttrBasic || DBO_WORLD_ATTR_BASIC_LAST < eAttrBasic )
	{
		return false;
	}

	unsigned int uiAttribute = GetAttribute( hHandle, x, z );

	if ( 0 == (uiAttribute & m_aruiBasicAttributeCheckFlag[eAttrBasic] ) )
	{
		return false;
	}

	return true;
}

float CNtlNaviImp::GetHeight( NAVI_INST_HANDLE hHandle, float x, float y, float z )
{
	if ( NULL == m_pNaviPEDataImporter )
	{
		return NAVI_FLT_MAX;
	}

	CNtlNaviPEWorld* pPEWorld = m_pNaviPEDataImporter->FindNaviWorld( hHandle );

	if ( NULL == pPEWorld )
	{
		return NAVI_FLT_MAX;
	}

	if ( pPEWorld->GetCurState() != CNtlNaviPEWorld::eNAVI_PE_STATE_COMPLETE )
	{
		return NAVI_FLT_MAX;
	}

	return pPEWorld->GetHeight( x, y, z );
}

float CNtlNaviImp::GetGuaranteedHeight( NAVI_INST_HANDLE hHandle, float x, float y, float z )
{
	if ( NULL == m_pNaviPEDataImporter )
	{
		return NAVI_FLT_MAX;
	}

	CNtlNaviPEWorld* pPEWorld = m_pNaviPEDataImporter->FindNaviWorld( hHandle );

	if ( NULL == pPEWorld )
	{
		return NAVI_FLT_MAX;
	}

	if ( pPEWorld->GetCurState() != CNtlNaviPEWorld::eNAVI_PE_STATE_COMPLETE )
	{
		return NAVI_FLT_MAX;
	}

	return pPEWorld->GetGuaranteedHeight( x, y, z );
}

bool CNtlNaviImp::FindNearestPos( NAVI_INST_HANDLE hHandle, float fAgentRadius, sNAVI_POS& sSrcPos, int nHorizRange, int nClosestRange )
{
	if ( NULL == m_pNaviPEDataImporter )
	{
		return false;
	}

	CNtlNaviPEWorld* pPEWorld = m_pNaviPEDataImporter->FindNaviWorld( hHandle );

	if ( NULL == pPEWorld )
	{
		return false;
	}

	if ( pPEWorld->GetCurState() != CNtlNaviPEWorld::eNAVI_PE_STATE_COMPLETE )
	{
		return false;
	}

	CNtlNaviVector3 vSrcPos( sSrcPos.x, sSrcPos.y, sSrcPos.z );

	bool bRet = pPEWorld->FindNearestPos( hHandle, fAgentRadius, vSrcPos, nHorizRange, nClosestRange );

	if ( bRet )
	{
		sSrcPos.x = vSrcPos.GetX();
		sSrcPos.y = vSrcPos.GetY();
		sSrcPos.z = vSrcPos.GetZ();
	}

	return bRet;
}

bool CNtlNaviImp::FindNearestPos( NAVI_INST_HANDLE hHandle, float fAgentRadius, sNAVI_POS& sSrcPos )
{
	return FindNearestPos(hHandle, fAgentRadius, sSrcPos, PATH_HORIZ_RANGE, PATH_CLOSEST_RANGE);
}

/**
* \brief Source position ���� Target position ���� �浹 �׽�Ʈ�� �Ѵ�.
* \remark �� �Լ��� �浹�� �Ǿ��� �ȵǾ��ĸ� �Ǵ��Ѵ�. �浹�� �� ���� ��ġ�� �ʿ��ϴٸ� CNtlNaviImp::FirstCollisionTest �Լ��� �����Ѵ�.
* \param hHandle WORLD�� ID�� iMesh Data�� ������ ����ü�� ( void* ) ���� ĳ�����ϱ� ���� ���Ǵ� Handle
* \param lAgentRadius Agent�� ������
* \param vSourcePos	���� ��ǥ
* \param vTargetPos �� ��ǥ
* \param nHorizRange Max horizontal range search
* \return �浹 ����� ��Ÿ���� eCOL_TEST_RESULT ������
*/
eCOL_TEST_RESULT CNtlNaviImp::CollisionTest( NAVI_INST_HANDLE hHandle, float fAgentRadius, sNAVI_POS& sSrcPos, sNAVI_POS& sDestPos, int nHorizRange )
{
	if ( NULL == m_pNaviPEDataImporter )
	{
		return eCOL_TEST_RESULT_FAILED;
	}

	CNtlNaviPEWorld* pPEWorld = m_pNaviPEDataImporter->FindNaviWorld(hHandle);

	if ( NULL == pPEWorld )
	{
		return eCOL_TEST_RESULT_FAILED;
	}

	if ( pPEWorld->GetCurState() != CNtlNaviPEWorld::eNAVI_PE_STATE_COMPLETE )
	{
		return eCOL_TEST_RESULT_FAILED;
	}

	CNtlNaviVector3 vSrcPos(sSrcPos.x, sSrcPos.y, sSrcPos.z);
	CNtlNaviVector3 vDestPos(sDestPos.x, sDestPos.y, sDestPos.z);

	return pPEWorld->CollisionTest(hHandle, fAgentRadius, vSrcPos, vDestPos, nHorizRange);
}

eCOL_TEST_RESULT CNtlNaviImp::CollisionTest( NAVI_INST_HANDLE hHandle, float fAgentRadius, sNAVI_POS& sSrcPos, sNAVI_POS& sDestPos )
{
	return CollisionTest(hHandle, fAgentRadius, sSrcPos, sDestPos, PATH_HORIZ_RANGE);
}

/**
* \brief Source position ���� Target position ���� �浹 �׽�Ʈ�� �Ͽ� �浹�� ���� position�� �˾Ƴ���.
* \remark ���� �� �Լ��� WORLD�� Y ��ǥ�� ��Ȯ���� �����Ƿ� Y�� ��Ȯ���� ������ �� ����.
* \param hHandle WORLD�� ID�� iMesh Data�� ������ ����ü�� ( void* ) ���� ĳ�����ϱ� ���� ���Ǵ� Handle
* \param lAgentRadius Agent�� ������
* \param sSrcPos ���� ��ǥ
* \param sDestPos �� ��ǥ
* \param sFirstCollisionPos �浹�� ���� ��ǥ�� ��µȴ�.
* \return �浹 ����� ��Ÿ���� eCOL_TEST_RESULT ������
*/
eCOL_TEST_RESULT CNtlNaviImp::FirstCollisionTest( NAVI_INST_HANDLE hHandle, float fAgentRadius, sNAVI_POS& sSrcPos, sNAVI_POS& sDestPos, sNAVI_POS& sFirstCollisionPos )
{
	if ( NULL == m_pNaviPEDataImporter )
	{
		return eCOL_TEST_RESULT_FAILED;
	}

	CNtlNaviPEWorld* pPEWorld = m_pNaviPEDataImporter->FindNaviWorld( hHandle );

	if ( NULL == pPEWorld )
	{
		return eCOL_TEST_RESULT_FAILED;
	}

	if ( pPEWorld->GetCurState() != CNtlNaviPEWorld::eNAVI_PE_STATE_COMPLETE )
	{
		return eCOL_TEST_RESULT_FAILED;
	}

	CNtlNaviVector3 vSrcPos( sSrcPos.x, sSrcPos.y, sSrcPos.z );
	CNtlNaviVector3 vDestPos( sDestPos.x, sDestPos.y, sDestPos.z );
	CNtlNaviVector3 vFirstCollisionPos;

	eCOL_TEST_RESULT eTestResult = pPEWorld->FirstCollisionTest( hHandle, fAgentRadius, vSrcPos, vDestPos, vFirstCollisionPos );
	sFirstCollisionPos.x = vFirstCollisionPos.GetX();
	sFirstCollisionPos.y = vFirstCollisionPos.GetY();
	sFirstCollisionPos.z = vFirstCollisionPos.GetZ();
	
	return eTestResult;
}

bool CNtlNaviImp::FindPath( NAVI_INST_HANDLE hHandle, float fAgentRadius, sNAVI_POS& sSrcPos, sNAVI_POS& sDestPos, vecdef_NaviPosList& defNaviPosList )
{
	if ( NULL == m_pNaviPEDataImporter )
	{
		return false;
	}

	CNtlNaviPEWorld* pPEWorld = m_pNaviPEDataImporter->FindNaviWorld( hHandle );

	if ( NULL == pPEWorld )
	{
		return false;
	}

	if ( pPEWorld->GetCurState() != CNtlNaviPEWorld::eNAVI_PE_STATE_COMPLETE )
	{
		return false;
	}

	CNtlNaviVector3 vSrcPos( sSrcPos.x, sSrcPos.y, sSrcPos.z );
	CNtlNaviVector3 vDestPos( sDestPos.x, sDestPos.y, sDestPos.z );

	return pPEWorld->FindPath( hHandle, fAgentRadius, vSrcPos, vDestPos, defNaviPosList );
}

bool CNtlNaviImp::IsValidPos(NAVI_INST_HANDLE hHandle, sNAVI_POS & sSrcPos)
{
	if (NULL == m_pNaviPEDataImporter)
	{
		return false;
	}

	CNtlNaviPEWorld* pPEWorld = m_pNaviPEDataImporter->FindNaviWorld(hHandle);

	if (NULL == pPEWorld)
	{
		return false;
	}

	return pPEWorld->IsValidPos(sSrcPos);
}
