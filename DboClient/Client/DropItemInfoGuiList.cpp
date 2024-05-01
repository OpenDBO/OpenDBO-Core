#include "precomp_dboclient.h"
#include "DropItemInfoGuiList.h"


#include "DropItemInfoGui.h"

#include "DBoEvent.h"

// simulation
#include "NtlSLEvent.h"

// presentation
#include "NtlPLGuiManager.h"
#include "NtlPLGlobal.h"

#include "NtlSobManager.h"
#include "DialogManager.h"

CDropItemInfoGuiList::CDropItemInfoGuiList(const RwChar* pName)
	: CNtlPLGui(pName)

{
	Init();
}

CDropItemInfoGuiList::~CDropItemInfoGuiList(VOID)
{
}

VOID CDropItemInfoGuiList::Init()
{

}

RwBool CDropItemInfoGuiList::Create()
{
	NTL_FUNCTION("CDropItemInfoGuiList::Create");

	// DropItemInfo
	m_pMainDropItemInfoGui = NTL_NEW CDropItemInfoGui("DropItemInfoGui");
	if (!m_pMainDropItemInfoGui->Create())
	{
		m_pMainDropItemInfoGui->Destroy();
		NTL_DELETE(m_pMainDropItemInfoGui);
	}

	LinkMsg(g_EventSobCreateDropItemInfo);
	LinkMsg(g_EventCharObjDelete);

	NTL_RETURN(TRUE);
}

VOID CDropItemInfoGuiList::Destroy(VOID)
{
	NTL_FUNCTION("CDropItemInfoGuiList::Destroy");

	m_pMainDropItemInfoGui->Destroy();
	NTL_DELETE(m_pMainDropItemInfoGui);

	for (auto elem : m_ListDropItemGui) {
		elem->Destroy();
		NTL_DELETE(elem);
	}

	m_ListDropItemGui.clear();

	UnLinkMsg(g_EventSobCreateDropItemInfo);
	UnLinkMsg(g_EventCharObjDelete);

	NTL_RETURNVOID();
}


VOID CDropItemInfoGuiList::HandleEvents(RWS::CMsg& msg) {

	bool isOptionsShowAllItemsEnabled = CNtlPLGlobal::m_bItemDropDisplay;

	if (msg.Id == g_EventSobCreateDropItemInfo) {
	SNtlEventSobDropItemInfoCreate* pData = reinterpret_cast<SNtlEventSobDropItemInfoCreate*>(msg.pData);
		CDropItemInfoGui* m_pDropItemInfoGui = NTL_NEW CDropItemInfoGui(pData->hSerialId, "DropItemInfoGui");

		if (!m_pDropItemInfoGui->Create())
		{
			m_pDropItemInfoGui->Destroy();
			NTL_DELETE(m_pDropItemInfoGui);
		}

		m_ListDropItemGui.emplace(m_pDropItemInfoGui);
	}
	else if (msg.Id == g_EventCharObjDelete) {
		SNtlEventSobDelete* pData = reinterpret_cast<SNtlEventSobDelete*>(msg.pData);

		if (pData->hSerialId == INVALID_SERIAL_ID)
			return;

		for (auto elem : m_ListDropItemGui) {

			if (elem->m_uiDropItemSerial == pData->hSerialId) {

				//remove from list
				m_ListDropItemGui.erase(elem);
				elem->HandleEvents(msg);

				//then destroy
				elem->Destroy();
				NTL_DELETE(elem);
				break;
			}
		}
	}
}