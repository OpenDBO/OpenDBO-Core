#include "stdafx.h"
#include "DynamicFieldRepository.h"
#include "../QueryServer.h"


smart_ptr<QueryResult> CDynamicFieldRepository::GetCount()
{
	return GetLogDB.Query("SELECT count FROM dynamic_field_count WHERE server_index=0");
}

void CDynamicFieldRepository::ResetCount(DWORD serverFarmId)
{
	UpdateCount(0, serverFarmId);
}

void CDynamicFieldRepository::UpdateCount(DWORD totalCount, DWORD serverFarmId)
{
	// upsert: the row does not exist on a fresh database
	GetLogDB.Execute("INSERT INTO dynamic_field_count (server_index,count) VALUES (%u,%u) ON DUPLICATE KEY UPDATE count=VALUES(count)", serverFarmId, totalCount);
}
