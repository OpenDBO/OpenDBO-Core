#include "precomp_ntlpresentation.h"
#include "NtlPLResourceScheduling.h"

// core
#include "NtlDebug.h"
#include "NtlCoreUtil.h"

// framework
#include "NtlTimer.h"

// presentation
#include "NtlPLDef.h"
#include "NtlPLEntity.h"
#include "NtlPLResourceManager.h"

// Performance Check ( Develop )
//#define USE_RESOURCE_SCHEDULING_PERFORMANCE_CHECK

#define MAX_LOAD_CHARACTER_SCHEDULING_TIME			4.0f
#define MAX_LOAD_OBJECT_SCHEDULING_TIME				3.0f

#define MAX_DELETE_SCHEDULING_TIME					0.1f

// 이 시간을 넘기면 rest time 을 뻥튀기 한다.
#define MAX_LOAD_CHARACTER_OVER_WEIGHT				0.07f
#define MAX_LOAD_OBJECT_OVER_WEIGHT					0.05f

#define DEFAULT_LOAD_OBJECT_REST_TIME					0.01f
#define DEFAULT_LOAD_CHARACTER_REST_TIME				0.02f

RwBool	g_bLoadSeamless				= FALSE;
RwReal	g_fLoadObjectTime			= 0.08f;
RwReal	g_fLoadObjectSeamlessTime	= 0.05f;
RwReal	g_fLoadCharacterSeamlessTime = 0.06f;

/**
* \brief Construction
*/
CNtlResourceScheduleUnit::CNtlResourceScheduleUnit()
{
	m_byEntityType			= 0;

	m_fLoadCurrTime			= 0.0f;
	m_fLoadSchedulingTime	= g_fLoadObjectTime;
}

/**
* \brief Destruction
*/
CNtlResourceScheduleUnit::~CNtlResourceScheduleUnit()
{
	// Delete Schedule占쏙옙 占쌍댐옙 clump 占쏙옙 占쏙옙占쏙옙占싼댐옙.
	CNtlPLResource *pResource;
	ListScheduling::iterator itDelete;
	for(itDelete = m_listClumpDeleteSchedule.begin(); itDelete != m_listClumpDeleteSchedule.end(); itDelete++)
	{
		pResource = (*itDelete);
		GetNtlResourceManager()->UnLoad(pResource);
	}
	m_listClumpDeleteSchedule.clear();

	// Load Schedule占쏙옙 占쌍댐옙 clump 占쏙옙占쏙옙트占쏙옙 占쏙옙占쏙옙占싼댐옙.
	MapScheduling::iterator it;
	for(it = m_mapClumpLoadSchedule.begin(); it != m_mapClumpLoadSchedule.end(); it++)
	{
		CNtlResourceScheduleManager::FreeListFree((void*)(*it).second);
	}
	m_mapClumpLoadSchedule.clear();
}

/**
* \brief Obejct ���� Clump���� �����층 �ε� ( ���� ���������� ���� ���߿� �ε��Ѵ�. )
* \param fElapsed	(RwReal) ���� ������Ʈ���� ��� �ð�
*/
void CNtlResourceScheduleUnit::UpdateObjectLoadScheduling(RwReal fElapsed)
{
	RwReal fSum = 0.0f;
	DWORD dwTime;
	
	SResourceScheduleNode *pNode;
	CNtlPLResource *pResource;
	CNtlPLEntity *pPLEntity;
	
	MapScheduling::iterator it;

#ifdef USE_RESOURCE_SCHEDULING_PERFORMANCE_CHECK
	DWORD dwCount = 0;
#endif
	
	// The map is rotated for a certain period of time and is loaded unconditionally. (Improving the loading speed of the data that was read.)
	for(it = m_mapClumpLoadSchedule.begin(); it != m_mapClumpLoadSchedule.end(); )
	{
		dwTime = GetTickCount();

		pNode = (*it).second;
		pPLEntity = (*it).first;
		if(pPLEntity)
			pPLEntity->CallPreSchedulingResource();

		pResource = GetNtlResourceManager()->LoadClump(pNode->chFileName, pNode->chResourcePath);

		if(pPLEntity)
		{	
			pPLEntity->CallSchedulingResource(pResource);
			pPLEntity->CallSchedulingResourceOnly();
		}

		it = m_mapClumpLoadSchedule.erase(it);
		CNtlResourceScheduleManager::FreeListFree((void*)pNode);

#ifdef USE_RESOURCE_SCHEDULING_PERFORMANCE_CHECK
		dwCount++;
#endif

		fSum += (RwReal)(GetTickCount() - dwTime)/1000.0f;
		if(fSum > g_fLoadObjectTime)
			break;
	}

#ifdef USE_RESOURCE_SCHEDULING_PERFORMANCE_CHECK
	RwReal fRealTime = fSum;
#endif
	
	if( fSum > MAX_LOAD_OBJECT_OVER_WEIGHT )
	{
		fSum *= 5.0f;
		if( fSum > MAX_LOAD_OBJECT_SCHEDULING_TIME )
			fSum = MAX_LOAD_OBJECT_SCHEDULING_TIME;
	}

	m_fLoadSchedulingTime = DEFAULT_LOAD_OBJECT_REST_TIME + fSum;

#ifdef USE_RESOURCE_SCHEDULING_PERFORMANCE_CHECK
	NtlLogFilePrintArgs( "[OBJECT] : %d loading for one tick, time is %3.3f sec and after rest time %3.3f sec", dwCount, fRealTime, m_fLoadSchedulingTime);
#endif
}

/**
* \brief Object 占쏙옙占쏙옙 clump占쏙옙占쏙옙 Seamless 占싸듸옙 ( 占쏙옙占쏙옙 占쏙옙占쏙옙 占심몌옙占쏙옙 占쏙옙占쏙옙占쏙옙 占싸듸옙 )
* \param fElapsed	(RwReal) ���� ������Ʈ���� ����� �ð�
*/
void CNtlResourceScheduleUnit::UpdateObjectLoadSeamlessScheduling(RwReal fElapsed)
{
	RwReal fSum = 0.0f;
	DWORD dwTime;
	
	SResourceScheduleNode *pNode;
	CNtlPLResource *pResource;
	CNtlPLEntity *pPLEntity;
	
	MapScheduling::iterator it;

#ifdef USE_RESOURCE_SCHEDULING_PERFORMANCE_CHECK
	DWORD dwCount = 0;
#endif
	
	for(it = m_mapClumpLoadSchedule.begin(); it != m_mapClumpLoadSchedule.end(); )
	{
		dwTime = GetTickCount();

		pNode = (*it).second;
		pPLEntity = (*it).first;
		if(pPLEntity)
			pPLEntity->CallPreSchedulingResource();

		pResource = GetNtlResourceManager()->LoadClump(pNode->chFileName, pNode->chResourcePath);

		if(pPLEntity)
		{	
			pPLEntity->CallSchedulingResource(pResource);
			pPLEntity->CallSchedulingResourceOnly();
		}

		it = m_mapClumpLoadSchedule.erase(it);
		CNtlResourceScheduleManager::FreeListFree((void*)pNode);

#ifdef USE_RESOURCE_SCHEDULING_PERFORMANCE_CHECK
		dwCount++;
#endif

		// 占십댐옙 20 frm
		fSum += (RwReal)(GetTickCount() - dwTime)/1000.0f;
		if(fSum > g_fLoadObjectSeamlessTime)
			break;
	}

#ifdef USE_RESOURCE_SCHEDULING_PERFORMANCE_CHECK
	RwReal fRealtime = fSum;
#endif

	if( fSum > MAX_LOAD_OBJECT_OVER_WEIGHT )
	{
		fSum *= 5.0f;
		if( fSum > MAX_LOAD_OBJECT_SCHEDULING_TIME )
			fSum = MAX_LOAD_OBJECT_SCHEDULING_TIME;
	}

	m_fLoadSchedulingTime = DEFAULT_LOAD_OBJECT_REST_TIME + fSum;

#ifdef USE_RESOURCE_SCHEDULING_PERFORMANCE_CHECK
	NtlLogFilePrintArgs( "[OBJECTSEAMLESS] : %d loading for one tick, time is %3.3f sec and after rest time %3.3f sec", dwCount, fRealtime, m_fLoadSchedulingTime);
#endif
}

/**
* \brief �����층 ������Ʈ�� �ε��Ѵ�.
* \param fElapsed	(RwReal) ���� ������Ʈ���� ����� �ð�
*/
void CNtlResourceScheduleUnit::UpdateLoadScheduling(RwReal fElapsed)
{
	m_fLoadCurrTime += fElapsed;
	if(m_fLoadCurrTime <= m_fLoadSchedulingTime)
		return;

	m_fLoadCurrTime -= m_fLoadSchedulingTime;

	if(m_mapClumpLoadSchedule.empty())
		return;

	// 占쏙옙占쏙옙占쏙옙트 占쏙옙 占쏙옙占쏙옙占쏙옙트 占싼댐옙.
	UpdateObjectLoadScheduling(fElapsed);
}

/**
* \brief �ɸ��� ������� �����췯�� ������Ʈ �Ѵ�.
*/
void CNtlResourceScheduleUnit::UpdateLoadSeamlessScheduling(RwReal fElapsed)
{
	if(m_mapClumpLoadSchedule.empty())
		return;
	
	UpdateObjectLoadSeamlessScheduling(fElapsed);
}

/**
* \brief ������ ����� �����층�Ѵ�.
*/
void CNtlResourceScheduleUnit::UpdateDeleteScheduling(RwReal fElapsed)
{
	if(m_listClumpDeleteSchedule.empty())
		return;

	RwReal fSumTime = 0.0f;
	DWORD dwTime;
	RwReal fFps		= ( 1.0f / CNtlTimer::GetFps() ) * 0.1f;
	if(fFps > 0.1f)
		fFps = 0.1f;

	CNtlPLResource *pResource;
	ListScheduling::iterator it = m_listClumpDeleteSchedule.begin();

	dwTime = GetTickCount();
	
	while(1)
	{

		pResource = (*it);
		GetNtlResourceManager()->UnLoad(pResource);

		it = m_listClumpDeleteSchedule.erase(it);

		if(m_listClumpDeleteSchedule.empty())
		{
			break;
		}

		fSumTime = (GetTickCount() - dwTime)/1000.0f;

		if(fSumTime > fFps)
		{
			break;
		}
	}
}

/**
* \brief Unit�� �����층 �ε��� Entity�� Type�� ����
* \param byEntityType	(RwUInt8) 占쏙옙티티占쏙옙 타占쏙옙
*/
void CNtlResourceScheduleUnit::SetEntityType(RwUInt8 byEntityType)
{
	m_byEntityType = byEntityType;
}

/**
* \brief
*/
void CNtlResourceScheduleUnit::Update(RwReal fElapsed)
{
	if(g_bLoadSeamless)
		UpdateLoadSeamlessScheduling(fElapsed);
	else
		UpdateLoadScheduling(fElapsed);

	UpdateDeleteScheduling(fElapsed);
}

void CNtlResourceScheduleUnit::AddSchedulingNode(CNtlPLEntity *pPLEntity, SResourceScheduleNode *pNode)
{
	m_mapClumpLoadSchedule[pPLEntity] = pNode;
}

void CNtlResourceScheduleUnit::UnLoadScheduling(CNtlPLEntity *pPLEntity, CNtlPLResource *pResource)
{
	if(pResource == NULL)
	{
		MapScheduling::iterator it = m_mapClumpLoadSchedule.find(pPLEntity);
		if(it != m_mapClumpLoadSchedule.end())
		{
			CNtlResourceScheduleManager::FreeListFree((void*)(*it).second);
			m_mapClumpLoadSchedule.erase(it);
		}
	}
	else
	{
		m_listClumpDeleteSchedule.push_back(pResource);
	}
}

RwBool CNtlResourceScheduleUnit::IsEmptyLoadScheduling(void)
{
	if(m_mapClumpLoadSchedule.empty())
		return TRUE;

	return FALSE;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////

/**
* \brief Construction
*/
CNtlResourceScheduleCharUnit::CNtlResourceScheduleCharUnit() 
{
}

/**
* \brief Destruction
*/
CNtlResourceScheduleCharUnit::~CNtlResourceScheduleCharUnit() 
{
	ListCharScheduling::iterator it;
	for(it = m_listCharClumpLoadSchedule.begin(); it != m_listCharClumpLoadSchedule.end(); it++)
	{
		CNtlResourceScheduleManager::FreeListFree((void*)(*it).pNode);
	}
	m_listCharClumpLoadSchedule.clear();
}

/**
* \brief Update
* \param fElapsed	(RwReal) 占쏙옙占쏙옙 占쏙옙占쏙옙占쌈울옙占쏙옙占쏙옙 占쏙옙占쏙옙占쏙옙트 Time
*/
void CNtlResourceScheduleCharUnit::Update( RwReal fElapsed ) 
{
	//UpdateCharacterLoadScheduling( fElapsed );
	UpdateCharacterLoadSeamlessScheduling( fElapsed );

	CNtlResourceScheduleUnit::UpdateDeleteScheduling( fElapsed );
}

/**
* \brief �����층 �ε��� Node�� ����Ѵ�.
* \param pPLEntity	(CNtlPLEntity*) Entity占쏙옙 占쌍쇽옙
* \param pNode		(SResourceScheduleNode*) ResourceNode
*/
void CNtlResourceScheduleCharUnit::AddSchedulingNode(CNtlPLEntity *pPLEntity, SResourceScheduleNode *pNode ) 
{
	// ����Ʈ�� ��ϵ� ����ü
	SResourceScheduleCharUnit sCharUnit;
	sCharUnit.pPLEntity = pPLEntity;
	sCharUnit.pNode = pNode;

	// 占쏙옙占쏙옙트占쏙옙 占쌩곤옙占싼댐옙.
	m_listCharClumpLoadSchedule.push_back( sCharUnit );
}

/**
* \brief �����층 �ε����� Node�� �����Ѵ�.
* \param pPLEntity	(CNtlPLEntity*) 占쏙옙占쏙옙占쏙옙 Entity占쏙옙 占쏙옙占쏙옙占쏙옙
* \param pResource	(CNtlPLResource*) 占쏙옙占쏙옙占쏙옙 占쏙옙占쌀쏙옙占쏙옙 占쏙옙占쏙옙占쏙옙
*/
void CNtlResourceScheduleCharUnit::UnLoadScheduling( CNtlPLEntity *pPLEntity, CNtlPLResource *pResource ) 
{
	// pResource占쏙옙 NULL占쏙옙 占쏙옙占승다몌옙 占쏙옙占쏙옙트占쏙옙占쏙옙 占쏙옙占쏙옙占쌔억옙 占싼댐옙.
	// multimap으로 자료구조를 바꿔야함
	if(pResource == NULL)
	{
		ListCharScheduling::iterator it = m_listCharClumpLoadSchedule.begin();
		while(it != m_listCharClumpLoadSchedule.end())
		{
			// 占쏙옙占쏙옙트占쏙옙占쏙옙 占싯삼옙占싹울옙 占쏙옙占쏙옙
			if( (*it).pPLEntity == pPLEntity )
			{
				CNtlResourceScheduleManager::FreeListFree((void*)(*it).pNode);
				m_listCharClumpLoadSchedule.erase(it);
				return;
			}
			else
			{
				it++;
			}
		}
	}
	else
	{
		// 삭제할 리스트를 등록한다.
		m_listClumpDeleteSchedule.push_back(pResource);
	}
}

/**
* \brief �����층 �ε� �� ����Ʈ�� ����ִ°�?
* \returns 비어있는지 여부
*/
RwBool CNtlResourceScheduleCharUnit::IsEmptyLoadScheduling( void ) 
{
	if( m_listCharClumpLoadSchedule.empty() )
		return TRUE;

	return FALSE;
}

/**
* \brief ĳ������ �����층�� ��� �ִ� clump�� �ε��Ѵ�.
* \param fElapsed	(RwReal) ����ð�
*/
void CNtlResourceScheduleCharUnit::UpdateCharacterLoadScheduling(RwReal fElapsed)
{
	m_fLoadCurrTime += fElapsed;

	// 占쏙옙占쏙옙占쏙옙 占심렸댐옙 占시곤옙占쏙옙큼 占쏙옙占쏙옙占쏙옙 占십았다몌옙 占쏙옙占쏙옙占싼댐옙.
	if(m_fLoadCurrTime <= m_fLoadSchedulingTime)
		return;

	m_fLoadCurrTime = 0.0f;

	// 占쏙옙占쌕몌옙 占쏙옙占쏙옙
	if( IsEmptyLoadScheduling() )
		return;

	SResourceScheduleNode *pNode = NULL;
	CNtlPLResource *pResource = NULL;
	CNtlPLEntity *pPLEntity = NULL;

	ListCharScheduling::iterator it = m_listCharClumpLoadSchedule.begin(); 
		
	DWORD dwTime = GetTickCount();

	pPLEntity = (*it).pPLEntity;
	pNode = (*it).pNode;

	// Entity占쏙옙占쏙옙 Resource占쏙옙 占싸듸옙占싹깍옙 占쏙옙占쏙옙 占싯뤄옙占쌔댐옙.
	if(pPLEntity)
		pPLEntity->CallPreSchedulingResource();

	// 占쏙옙占쌀쏙옙占쏙옙 占쏙옙占쏙옙占쏙옙 占싶쇽옙
	pResource = GetNtlResourceManager()->LoadClump(pNode->chFileName, pNode->chResourcePath);

	// 占쏙옙占쌀쏙옙占쏙옙 占싸듸옙
	if(pPLEntity)
	{	
		pPLEntity->CallSchedulingResource(pResource);
		pPLEntity->CallSchedulingResourceOnly();
	}

	m_listCharClumpLoadSchedule.erase(it);
	// 占쏙옙占쌀쏙옙占쏙옙 占싸듸옙占쏙옙占식울옙占쏙옙 占쌥븝옙占쌘몌옙 占쏙옙占쏙옙占쌔댐옙.
	CNtlResourceScheduleManager::FreeListFree((void*)pNode);

	dwTime = GetTickCount() - dwTime;

	m_fLoadSchedulingTime = g_fCharScheduleTime;

}

/**
* \brief Cash 占쏙옙 占쏙옙占쏙옙占쏙옙占쏙옙 占싸듸옙 占쌈듸옙占쏙옙 占쏙옙占쏙옙키占쏙옙 占쏙옙占쏙옙 占쏙옙占쏙옙占쏙옙트 占쌉쇽옙
* \param fElapsed	(RwReal) ���� ������Ʈ���� ����� �ð�
*/
void CNtlResourceScheduleCharUnit::UpdateCharacterLoadSeamlessScheduling( RwReal fElapsed ) 
{
	m_fLoadCurrTime += fElapsed;

	// 占쏙옙占쏙옙占쏙옙 占심렸댐옙 占시곤옙占쏙옙큼 占쏙옙占쏙옙占쏙옙 占십았다몌옙 占쏙옙占쏙옙占싼댐옙.
	if(m_fLoadCurrTime <= m_fLoadSchedulingTime)
		return;

	m_fLoadCurrTime = 0.0f;

	// 占쏙옙占쌕몌옙 占쏙옙占쏙옙
	if( IsEmptyLoadScheduling() )
		return;

	DWORD dwTime = 0;
	RwReal fSum = 0.0f;

	SResourceScheduleNode *pNode = NULL;
	CNtlPLResource *pResource = NULL;
	CNtlPLEntity *pPLEntity = NULL;

	ListCharScheduling::iterator it;

#ifdef USE_RESOURCE_SCHEDULING_PERFORMANCE_CHECK
	DWORD dwCount = 0;
#endif

	for( it = m_listCharClumpLoadSchedule.begin(); it != m_listCharClumpLoadSchedule.end(); )
	{
		dwTime = GetTickCount();

		pPLEntity = (*it).pPLEntity;
		pNode = (*it).pNode;

		// Entity占쏙옙占쏙옙 Resource占쏙옙 占싸듸옙占싹깍옙 占쏙옙占쏙옙 占싯뤄옙占쌔댐옙.
		if(pPLEntity)
			pPLEntity->CallPreSchedulingResource();

		// 占쏙옙占쌀쏙옙占쏙옙 占쏙옙占쏙옙占쏙옙 占싶쇽옙
		pResource = GetNtlResourceManager()->LoadClump(pNode->chFileName, pNode->chResourcePath);

		// 占쏙옙占쌀쏙옙占쏙옙 占싸듸옙
		if(pPLEntity)
		{	
			pPLEntity->CallSchedulingResource(pResource);
			pPLEntity->CallSchedulingResourceOnly();
		}

		it = m_listCharClumpLoadSchedule.erase(it);
		CNtlResourceScheduleManager::FreeListFree((void*)pNode);

		dwTime = GetTickCount() - dwTime;

#ifdef USE_RESOURCE_SCHEDULING_PERFORMANCE_CHECK
		dwCount++;
#endif

		fSum += ((RwReal)dwTime/1000.0f);
		if( fSum > g_fLoadCharacterSeamlessTime )
			break;
	}

#ifdef USE_RESOURCE_SCHEDULING_PERFORMANCE_CHECK
	RwReal fRealTime = fSum;
#endif

	if( fSum > MAX_LOAD_CHARACTER_OVER_WEIGHT )
	{
		fSum *= 5.0f;
		if( fSum > MAX_LOAD_CHARACTER_SCHEDULING_TIME )
			fSum = MAX_LOAD_CHARACTER_SCHEDULING_TIME;
	}

	m_fLoadSchedulingTime = g_fCharScheduleTime + fSum;

#ifdef USE_RESOURCE_SCHEDULING_PERFORMANCE_CHECK
	NtlLogFilePrintArgs( "[CHARACTER] : %d loading for one tick, time is %3.3f sec and after rest time %3.3f sec", dwCount, fRealTime, m_fLoadSchedulingTime);
#endif
}
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////

RwFreeList* CNtlResourceScheduleManager::m_pScheduleNodeFreeList = NULL;		/* scheduling node memory pool **/


RwBool CNtlResourceScheduleManager::AllocFreeList(void)
{
	NTL_FUNCTION("CNtlReourceScheduleManager::AllocFreeList");

	RwUInt32 iBlockSize = 500;
	m_pScheduleNodeFreeList = RwFreeListCreate(sizeof(SResourceScheduleNode), iBlockSize, 16, rwMEMHINTDUR_GLOBAL);
    RWS_ASSERT(m_pScheduleNodeFreeList, "CNtlReourceScheduleManager::AllocFreeList");

	NTL_RETURN(TRUE);
}

void CNtlResourceScheduleManager::DeAllocFreeList(void)
{
	NTL_FUNCTION("CNtlReourceScheduleManager::DeAllocFreeList");

	if (m_pScheduleNodeFreeList)
    {
        RwFreeListDestroy(m_pScheduleNodeFreeList);
        m_pScheduleNodeFreeList = 0;
    }

	NTL_RETURNVOID();
}

void CNtlResourceScheduleManager::FreeListFree(void *pData)
{
	RwFreeListFree(m_pScheduleNodeFreeList, pData);
}

CNtlResourceScheduleManager::~CNtlResourceScheduleManager()
{
	CNtlResourceScheduleUnit *pUnit;
	MapScheduleUnit::iterator it;

	for(it = m_mapUnit.begin(); it != m_mapUnit.end(); it++)
	{
		pUnit = (*it).second;
		NTL_DELETE(pUnit);
	}

	m_mapUnit.clear();
}

void CNtlResourceScheduleManager::Update(RwReal fElapsed)
{
	// Kell
	static RwBool bCz = FALSE;
	if (bCz)
	{
		return;
	}

	MapScheduleUnit::iterator it;
	for(it = m_mapUnit.begin(); it != m_mapUnit.end(); it++)
	{
		(*it).second->Update(fElapsed);
	}
}

CNtlResourceScheduleUnit* CNtlResourceScheduleManager::FindUnit(RwUInt8 byEntityType)
{
	MapScheduleUnit::iterator it = m_mapUnit.find(byEntityType);
	if(it == m_mapUnit.end())
		return NULL;

	return (*it).second;
}

void CNtlResourceScheduleManager::AddSchedulingClump(const char *pStrName, const char * pResourcePath, CNtlPLEntity *pEntity)
{
	SResourceScheduleNode *pNode = (SResourceScheduleNode*)RwFreeListAlloc(m_pScheduleNodeFreeList, rwMEMHINTDUR_GLOBAL);
	pNode->uiResType = rwID_CLUMP;
	strcpy_s(pNode->chResourcePath, pResourcePath);
	strcpy_s(pNode->chFileName, pStrName);

	RwUInt8 byEntityType = (RwUInt8)pEntity->GetClassType();

	// 확占쏙옙 by Kell
	// ĳ���Ϳ� �������� �ٸ� ����� �����층 �ε��� ����Ѵ�.
	CNtlResourceScheduleUnit *pUnit = FindUnit(byEntityType);
	if(pUnit == NULL)
	{
		if( byEntityType == PLENTITY_CHARACTER || byEntityType == PLENTITY_ITEM )
		{
			pUnit = NTL_NEW CNtlResourceScheduleCharUnit;
		}
		else
		{
			pUnit = NTL_NEW CNtlResourceScheduleUnit;
		}

		pUnit->SetEntityType(byEntityType);
		m_mapUnit[byEntityType] = pUnit;
	}

	pUnit->AddSchedulingNode(pEntity, pNode);
}

void CNtlResourceScheduleManager::UnLoadScheduling(CNtlPLEntity *pPLEntity, CNtlPLResource *pResource)
{
	if(pPLEntity == NULL)
		return;

	CNtlResourceScheduleUnit *pUnit = FindUnit((RwUInt8)pPLEntity->GetClassType());

	if(pUnit == NULL)
		return;

	pUnit->UnLoadScheduling(pPLEntity, pResource);
}

RwBool CNtlResourceScheduleManager::IsEmptyLoadScheduling(RwUInt8 byEntityType)
{
	CNtlResourceScheduleUnit *pUnit = FindUnit(byEntityType);
	if(pUnit == NULL)
		return TRUE;

	return pUnit->IsEmptyLoadScheduling();
}

RwBool GetLoadObjectSeamlessScheduling(void)
{
	return g_bLoadSeamless;
}

void SetLoadObjectSeamlessScheduling(RwBool bEnable)
{
	g_bLoadSeamless = bEnable;
}

void SetLoadObjectSchedulingOneTickTime(RwReal fTime)
{
	g_fLoadObjectTime = fTime;
}

void SetLoadObjectSeamlessSchedulingOneTickTime(RwReal fTime)
{
	g_fLoadObjectSeamlessTime = fTime;
}
