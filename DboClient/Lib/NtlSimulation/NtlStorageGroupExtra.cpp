#include "precomp_ntlsimulation.h"
#include "NtlStorageGroupExtra.h"

#include "NtlStorageMTContainer.h"
#include "NtlStorageBinarySerializer.h"

#include "NtlDebug.h"

CNtlStorageGroupExtra::CNtlStorageGroupExtra(void)
{
	SetStorageGroupType(eNTL_STORAGE_GROUP_EXTRA);
}

CNtlStorageGroupExtra::~CNtlStorageGroupExtra(void)
{
}

bool CNtlStorageGroupExtra::Create()
{
	CNtlStorageUnit* pDropFilter = NTL_NEW CNtlStorageUnit();
	if (!pDropFilter->Create(eNTL_STORAGE_EXTRAFILTER))
	{
		pDropFilter->Destroy();

		NTL_DELETE(pDropFilter);

		return false;
	}
	m_mapStorageUnit.insert(std::make_pair(eNTL_STORAGE_EXTRAFILTER, pDropFilter));

	return true;
}