#include "precomp_dboclient.h"
#include "EventRewardMainGui.h"

// core
#include "NtlDebug.h"

// shared
#include "NtlEventDef.h"
#include "EventSystemTable.h"
#include "TableContainer.h"
#include "TextAllTable.h"

// presentation
#include "NtlPLGuiManager.h"

// simulation
#include "NtlSLApi.h"

// dbo
#include "AlarmManager.h"
#include "DboEvent.h"
#include "DboEventGenerator.h"
#include "DboGlobal.h"
#include "DboLogic.h"
#include "DialogManager.h"
#include "DisplayStringManager.h"

#define dEVENTREWARD_DISABLE_COLOR		RGB(183, 191, 195)


sEVENT_SYSTEM_TBLDAT* Logic_FindEventRewardTbldat(TBLIDX eventTblidx)
{
	CEventSystemTable* pEventTable = API_GetTableContainer()->GetEventSystemTable();
	if (pEventTable == NULL)
		return NULL;

	return static_cast<sEVENT_SYSTEM_TBLDAT*>(pEventTable->FindData(eventTblidx));
}


CEventRewardMainGui::CEventRewardMainGui(const RwChar* pName)
: CNtlPLGui(pName)
{
	m_pstbTitle = NULL;
	m_pbtnPageLeft = NULL;
	m_pstbPage = NULL;
	m_pbtnPageRight = NULL;
	m_pbtnClose = NULL;

	for (RwInt32 i = 0; i < dEVENTREWARD_PAGE_COUNT; ++i)
		m_apbtnEvent[i] = NULL;

	m_hNPCSerial = INVALID_SERIAL_ID;
	m_iCurPage = 1;
	m_iPageCount = 1;
}

CEventRewardMainGui::~CEventRewardMainGui(VOID)
{
}

RwBool CEventRewardMainGui::Create(VOID)
{
	NTL_FUNCTION("CEventRewardMainGui::Create");

	if (!CNtlPLGui::Create("gui\\EventReward.rsr", "gui\\EventReward.srf", "gui\\eventrewardmaingui.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(GetNtlGuiManager()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");
	m_pThis->SetPriority(dDIALOGPRIORITY_DEFAULT);

	m_pstbTitle = (gui::CStaticBox*)GetComponent("StTitle");
	m_pstbTitle->SetText(GetDisplayStringManager()->GetString("DST_REWARDLIST"));

	for (RwInt32 i = 0; i < dEVENTREWARD_PAGE_COUNT; ++i)
	{
		RwChar acName[32];
		sprintf_s(acName, 32, "BtnEvent%d", i + 1);

		m_apbtnEvent[i] = (gui::CButton*)GetComponent(acName);
		m_apbtnEvent[i]->SetTextFont(DEFAULT_FONT, 90, DEFAULT_FONT_ATTR);
		m_apbtnEvent[i]->SetTextStyle(COMP_TEXT_CENTER);
		m_apbtnEvent[i]->SetTextUpColor(RGB(255, 255, 255));
		m_apbtnEvent[i]->SetTextFocusColor(RGB(255, 255, 255));
		m_apbtnEvent[i]->SetTextDownColor(RGB(255, 255, 255));
		m_apbtnEvent[i]->SetTextDisableColor(dEVENTREWARD_DISABLE_COLOR);
		m_apbtnEvent[i]->ApplyText();
		m_aslotClickEvent[i] = m_apbtnEvent[i]->SigClicked().Connect(this, &CEventRewardMainGui::OnClickEvent);
	}

	m_pbtnPageLeft = (gui::CButton*)GetComponent("BtnPageLeft");
	m_slotClickPageLeft = m_pbtnPageLeft->SigClicked().Connect(this, &CEventRewardMainGui::OnClickPageLeft);

	m_pstbPage = (gui::CStaticBox*)GetComponent("StPageCenter");

	m_pbtnPageRight = (gui::CButton*)GetComponent("BtnPageRight");
	m_slotClickPageRight = m_pbtnPageRight->SigClicked().Connect(this, &CEventRewardMainGui::OnClickPageRight);

	m_pbtnClose = (gui::CButton*)GetComponent("btnClose");
	m_slotClickClose = m_pbtnClose->SigClicked().Connect(this, &CEventRewardMainGui::OnClickClose);

	LinkMsg(g_EventNPCDialogOpen);
	LinkMsg(g_EventDialog);
	LinkMsg(g_EventNpcReward);

	Show(false);

	NTL_RETURN(TRUE);
}

VOID CEventRewardMainGui::Destroy(VOID)
{
	NTL_FUNCTION("CEventRewardMainGui::Destroy");

	UnLinkMsg(g_EventNPCDialogOpen);
	UnLinkMsg(g_EventDialog);
	UnLinkMsg(g_EventNpcReward);

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();

	NTL_RETURNVOID();
}

RwInt32 CEventRewardMainGui::SwitchDialog(bool bOpen)
{
	if (!bOpen)
	{
		ClearList();
		Logic_CancelNpcFacing();

		GetDialogManager()->CloseDialog(DIALOG_EVENTREWARD_SUB);

		m_hNPCSerial = INVALID_SERIAL_ID;
	}

	Show(bOpen);
	return TRUE;
}

VOID CEventRewardMainGui::HandleEvents(RWS::CMsg& msg)
{
	if (msg.Id == g_EventNPCDialogOpen)
	{
		SDboNpcDialogOpen* pData = reinterpret_cast<SDboNpcDialogOpen*>(msg.pData);
		if (pData->eDialog != DIALOG_EVENTREWARD_MAIN)
			return;

		if (!GetDialogManager()->OpenDialog(DIALOG_EVENTREWARD_MAIN, pData->hSerialId))
		{
			Logic_CancelNpcFacing();
			return;
		}

		// tw: the list opens empty and fills once the server answered
		m_hNPCSerial = pData->hSerialId;
		ClearList();
		EnableList(false);

		GetDboGlobal()->GetGamePacketGenerator()->SendEventRewardLoadReq(m_hNPCSerial);
	}
	else if (msg.Id == g_EventDialog)
	{
		SDboEventDialog* pData = reinterpret_cast<SDboEventDialog*>(msg.pData);
		if (pData->iDestDialog != DIALOG_EVENTREWARD_MAIN)
			return;

		if (pData->iType == DIALOGEVENT_NPC_BYEBYE || pData->iType == DIALOGEVENT_OPEN_FAIL_NPC_DIALOG)
			GetDialogManager()->CloseDialog(DIALOG_EVENTREWARD_MAIN);
	}
	else if (msg.Id == g_EventNpcReward)
	{
		if (!IsShow())
			return;

		SDboEventNpcReward* pData = reinterpret_cast<SDboEventNpcReward*>(msg.pData);

		switch (pData->byType)
		{
			case NPC_REWARD_EVENT_LOAD_INFO:
			{
				sGU_EVENT_REWARD_LOAD_INFO* pInfo = reinterpret_cast<sGU_EVENT_REWARD_LOAD_INFO*>(pData->pData);

				for (int i = 0; i < pInfo->nCount && i < NTL_MAX_EVENT_REWARD_COUNT_IN_PACKET; ++i)
				{
					sREWARD sReward;
					sReward.eventTblidx = pInfo->aInfo[i].eventTblidx;
					sReward.charId = pInfo->aInfo[i].charId;
					sReward.bReceived = FALSE;

					m_vecReward.push_back(sReward);
				}
			}
			break;
			case NPC_REWARD_EVENT_LOAD:
			{
				sGU_EVENT_REWARD_LOAD_RES* pResult = reinterpret_cast<sGU_EVENT_REWARD_LOAD_RES*>(pData->pData);

				for (BYTE i = 0; i < pResult->byCount && i < NTL_MAX_EVENT_REWARD_COUNT_IN_PACKET; ++i)
				{
					WCHAR awchName[NTL_MAX_SIZE_CHAR_NAME + 1];
					wcsncpy_s(awchName, NTL_MAX_SIZE_CHAR_NAME + 1, pResult->asNameInfo[i].awchName, _TRUNCATE);

					m_mapName[pResult->asNameInfo[i].charId] = awchName;
				}

				RwInt32 iCount = (RwInt32)m_vecReward.size();
				m_iPageCount = (iCount + dEVENTREWARD_PAGE_COUNT - 1) / dEVENTREWARD_PAGE_COUNT;
				if (m_iPageCount < 1)
					m_iPageCount = 1;

				EnableList(true);
				SetPage(1);
			}
			break;
			case NPC_REWARD_EVENT_SELECT:
			{
				// an account has a reward once
				for (std::vector<sREWARD>::iterator it = m_vecReward.begin(); it != m_vecReward.end(); ++it)
				{
					if (it->eventTblidx == pData->eventTblidx)
						it->bReceived = TRUE;
				}

				UpdatePage();
			}
			break;
		}
	}
}

VOID CEventRewardMainGui::ClearList(VOID)
{
	m_vecReward.clear();
	m_mapName.clear();

	m_iCurPage = 1;
	m_iPageCount = 1;
}

VOID CEventRewardMainGui::EnableList(bool bEnable)
{
	for (RwInt32 i = 0; i < dEVENTREWARD_PAGE_COUNT; ++i)
	{
		if (!bEnable)
			m_apbtnEvent[i]->SetText(L"");

		m_apbtnEvent[i]->ClickEnable(bEnable);
	}

	m_pbtnPageLeft->ClickEnable(bEnable);
	m_pbtnPageRight->ClickEnable(bEnable);

	if (!bEnable)
		m_pstbPage->SetText(L"");
}

VOID CEventRewardMainGui::SetPage(RwInt32 iPage)
{
	if (iPage < 1 || iPage > m_iPageCount)
		return;

	m_iCurPage = iPage;

	WCHAR awcBuffer[32];
	swprintf_s(awcBuffer, 32, L"%d / %d", m_iCurPage, m_iPageCount);
	m_pstbPage->SetText(awcBuffer);

	UpdatePage();
}

VOID CEventRewardMainGui::UpdatePage(VOID)
{
	CTextTable* pTextTable = API_GetTableContainer()->GetTextAllTable()->GetETCTbl();

	for (RwInt32 i = 0; i < dEVENTREWARD_PAGE_COUNT; ++i)
	{
		RwInt32 iIndex = (m_iCurPage - 1) * dEVENTREWARD_PAGE_COUNT + i;
		if (iIndex >= (RwInt32)m_vecReward.size())
		{
			m_apbtnEvent[i]->SetText(L"");
			m_apbtnEvent[i]->ClickEnable(false);
			continue;
		}

		const sREWARD& rReward = m_vecReward[iIndex];

		sEVENT_SYSTEM_TBLDAT* pEventTbldat = Logic_FindEventRewardTbldat(rReward.eventTblidx);
		if (pEventTbldat == NULL)
		{
			m_apbtnEvent[i]->SetText(GetDisplayStringManager()->GetString("DST_REWARD_TABLE_ERROR"));
			m_apbtnEvent[i]->ClickEnable(false);
			continue;
		}

		if (pEventTbldat->byType != EVENT_SYSTEM_TYPE_NPC_REWARD)
		{
			m_apbtnEvent[i]->SetText(GetDisplayStringManager()->GetString("DST_REWARD_INDEX_ERROR"));
			m_apbtnEvent[i]->ClickEnable(false);
			continue;
		}

		std::wstring wstrName;
		if (pTextTable)
			pTextTable->GetText(dEVENTREWARD_TEXT_NAME(pEventTbldat->tblidx), &wstrName);

		m_apbtnEvent[i]->SetText(wstrName);
		m_apbtnEvent[i]->ClickEnable(rReward.bReceived ? false : true);
	}
}

VOID CEventRewardMainGui::OnClickEvent(gui::CComponent* pComponent)
{
	for (RwInt32 i = 0; i < dEVENTREWARD_PAGE_COUNT; ++i)
	{
		if (m_apbtnEvent[i] != pComponent)
			continue;

		RwInt32 iIndex = (m_iCurPage - 1) * dEVENTREWARD_PAGE_COUNT + i;
		if (iIndex >= (RwInt32)m_vecReward.size())
			return;

		const sREWARD& rReward = m_vecReward[iIndex];

		sEVENT_SYSTEM_TBLDAT* pEventTbldat = Logic_FindEventRewardTbldat(rReward.eventTblidx);
		if (pEventTbldat == NULL)
		{
			GetAlarmManager()->AlarmMessage("DST_REWARD_NOT_TABLE");
			return;
		}

		if (rReward.charId == INVALID_CHARACTERID)
		{
			// a reward of the whole account, any character takes it once. tw refuses one of a character reward
			// (DST_REWARD_NOT_CHAR_CHARID), but the gm commands give every reward to the whole account
			GetDialogManager()->OpenDialog(DIALOG_EVENTREWARD_SUB, m_hNPCSerial);
			CDboEventGenerator::NpcReward(NPC_REWARD_EVENT_DATA, NULL, rReward.eventTblidx, INVALID_CHARACTERID, L"");
			return;
		}

		std::map<CHARACTERID, std::wstring>::iterator itName = m_mapName.find(rReward.charId);
		if (itName == m_mapName.end())
		{
			GetAlarmManager()->AlarmMessage("DST_REWARD_NOT_CHARID");
			return;
		}

		// tw: an account reward never names a receiver
		if (pEventTbldat->adwSetting[0] == dEVENTREWARD_RECEIVER_ACCOUNT)
		{
			GetAlarmManager()->AlarmMessage("DST_REWARD_NOT_ACCOUNT_CHARID");
			return;
		}

		GetDialogManager()->OpenDialog(DIALOG_EVENTREWARD_SUB, m_hNPCSerial);
		CDboEventGenerator::NpcReward(NPC_REWARD_EVENT_DATA, NULL, rReward.eventTblidx, rReward.charId, itName->second.c_str());
		return;
	}
}

VOID CEventRewardMainGui::OnClickPageLeft(gui::CComponent* pComponent)
{
	SetPage(m_iCurPage - 1);
}

VOID CEventRewardMainGui::OnClickPageRight(gui::CComponent* pComponent)
{
	SetPage(m_iCurPage + 1);
}

VOID CEventRewardMainGui::OnClickClose(gui::CComponent* pComponent)
{
	GetDialogManager()->CloseDialog(DIALOG_EVENTREWARD_MAIN);
}
