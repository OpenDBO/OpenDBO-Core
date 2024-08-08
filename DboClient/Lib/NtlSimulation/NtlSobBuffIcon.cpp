#include "precomp_ntlsimulation.h"
#include "NtlSobBuffIcon.h"

// core
#include "NtlDebug.h"

// shared
#include "NtlObject.h"
#include "SkillTable.h"
#include "ItemTable.h"
#include "UseItemTable.h"
#include "TableContainer.h"

// simulation
#include "NtlSLApi.h"
#include "NtlSLEvent.h"
#include "NtlSobBuff.h"
#include "NtlSobBuffAttr.h"

DEFINITION_MEMORY_POOL(CNtlSobBuffIcon)

CNtlSobBuffIcon::CNtlSobBuffIcon()
{
}

CNtlSobBuffIcon::~CNtlSobBuffIcon()
{
}

void CNtlSobBuffIcon::HandleEvents(RWS::CMsg &pMsg)
{
	if(pMsg.Id == g_EventSobCreate)
	{
		CNtlEventSobBuffCreate *pSobItemCreate = reinterpret_cast<CNtlEventSobBuffCreate*>(pMsg.pData);
		{
			CNtlSobBuff *pSobBuff = reinterpret_cast<CNtlSobBuff*>(m_pSobObj);
			CNtlSobBuffAttr *pSobBuffAttr = reinterpret_cast<CNtlSobBuffAttr*>(pSobBuff->GetSobAttr());

			if (DBO_OBJECT_SOURCE_SKILL == pSobBuffAttr->GetBuffType())
			{
				sSKILL_TBLDAT* pSkillTbl = pSobBuffAttr->GetSkillTbl();
				CreateImage(pSkillTbl->szIcon_Name);
				m_fMaxCoolTime = pSobItemCreate->uiKeepTime / 1000.0f;				
			}
			else if (DBO_OBJECT_SOURCE_ITEM == pSobBuffAttr->GetBuffType())
			{
				sITEM_TBLDAT* pItemTbl = pSobBuffAttr->GetItemTbl();
				CreateImage(pItemTbl->szIcon_Name);	

				sUSE_ITEM_TBLDAT* pUseItemData = reinterpret_cast<sUSE_ITEM_TBLDAT*>( API_GetTableContainer()->GetUseItemTable()->FindData( pItemTbl->Use_Item_Tblidx ) );
				NTL_ASSERT( pUseItemData, "UseItemTbl Index" << pItemTbl->Use_Item_Tblidx << "is Invalid" );

				m_fMaxCoolTime = (RwReal)pUseItemData->wKeep_Time;
			}

			if( pSobItemCreate->uiRemainTime > 0 )
			{
				SetIconState( ICON_STATE_COOLING, m_fMaxCoolTime - ( pSobItemCreate->uiRemainTime / 1000.0f ) );
			}			
		}
	}
}

RwBool CNtlSobBuffIcon::IsDragAndDropPossible(void)
{
	return FALSE;
}

RwBool CNtlSobBuffIcon::IsUsePossible(void)
{
	return FALSE;
}
