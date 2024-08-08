#include "precomp_ntlsimulation.h"
#include "NtlStorageGroupCharacter.h"
#include "NtlDebug.h"


CNtlStorageGroupCharacter::CNtlStorageGroupCharacter(void)
{
	SetStorageGroupType( eNTL_STORAGE_GROUP_CHARACTER );
}

CNtlStorageGroupCharacter::~CNtlStorageGroupCharacter(void)
{
}

bool CNtlStorageGroupCharacter::Create()
{
	CNtlStorageUnit* pCharacterUnit = NTL_NEW CNtlStorageUnit();
	if( !pCharacterUnit->Create( eNTL_STORAGE_CHARACTER ) )
	{
		pCharacterUnit->Destroy();
		NTL_DELETE( pCharacterUnit );
		return false;
	}
	m_mapStorageUnit.insert( std::make_pair( eNTL_STORAGE_CHARACTER, pCharacterUnit ) );

	return true;
}