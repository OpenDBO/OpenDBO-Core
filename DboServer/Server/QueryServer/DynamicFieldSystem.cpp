#include "stdafx.h"
#include "DynamicFieldSystem.h"
#include "QueryServer.h"
#include "Repository/DynamicFieldRepository.h"


CDynamicFieldSystem::CDynamicFieldSystem()
{
	Init();
}

CDynamicFieldSystem::~CDynamicFieldSystem()
{
	Destroy();
}



void CDynamicFieldSystem::Init()
{
	// defaults when no progress is stored yet (fresh database)
	m_dwCurCount = 0;
	m_dwMaxCount = 0;

	smart_ptr<QueryResult> item = g_pDynamicFieldRepository->GetCount();
	if (item)
	{
		Field* i = item->Fetch();
		if (i)
		{
			m_dwCurCount = i->GetDWORD();
		}
	}
}

void CDynamicFieldSystem::Destroy()
{
}



