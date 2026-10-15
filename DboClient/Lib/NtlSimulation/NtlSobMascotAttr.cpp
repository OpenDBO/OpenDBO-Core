#include "precomp_ntlsimulation.h"
#include "NtlSobMascotAttr.h"

// shared
#include "TableContainer.h"
#include "MascotStatusTable.h"
#include "ItemTable.h"
#include "TextAllTable.h"

// core
#include "NtlDebug.h"

// simulation
#include "NtlSLApi.h"
#include "NtlSLLogicDef.h"

DEFINITION_MEMORY_POOL(CNtlSobMascotAttr)

CNtlSobMascotAttr::CNtlSobMascotAttr()
{
	m_mascotTblidx = INVALID_TBLIDX;
	m_pMascotTbl = NULL;
}

RwBool CNtlSobMascotAttr::SetMascotTblidx(TBLIDX mascotTblidx)
{
	m_mascotTblidx = mascotTblidx;
	m_pMascotTbl = (sMASCOT_STATUS_TBLDAT*)API_GetTableContainer()->GetMascotStatusTable()->FindData(mascotTblidx);
	if (m_pMascotTbl == NULL)
	{
		DBO_WARNING_MESSAGE("Mascot status table data not found: " << mascotTblidx);
		return FALSE;
	}

	SetModelName(m_pMascotTbl->szModel);
	SetLevel(m_pMascotTbl->byRank);
	SetRadius(0.5f);
	SetNameColor(NTL_NPC_NAME_COLOR);

	// mascots are registered from an item with the same index, its text is the mascot name
	sITEM_TBLDAT* pItemTbl = (sITEM_TBLDAT*)API_GetTableContainer()->GetItemTable()->FindData(mascotTblidx);
	if (pItemTbl)
		SetName(API_GetTableContainer()->GetTextAllTable()->GetItemTbl()->GetText(pItemTbl->Name).c_str());
	else
		SetName(m_pMascotTbl->wszNameText);

	return TRUE;
}
