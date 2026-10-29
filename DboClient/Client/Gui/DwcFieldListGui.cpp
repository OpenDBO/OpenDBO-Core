#include "precomp_dboclient.h"
#include "DwcFieldListGui.h"

// core
#include "NtlDebug.h"

// shared
#include "DwcTable.h"
#include "TableContainer.h"
#include "TextAllTable.h"

// presentation
#include "NtlPLGuiManager.h"
#include "NtlPLEvent.h"

// simulation
#include "NtlSLApi.h"
#include "NtlSLGlobal.h"

// dbo
#include "AlarmManager.h"
#include "DboEvent.h"
#include "DboGlobal.h"
#include "DboLogic.h"
#include "DialogManager.h"
#include "DisplayStringManager.h"
#include "MsgBoxManager.h"

// tw layout of the list items
#define dDWC_FIELDLIST_ITEM_WIDTH			215
#define dDWC_FIELDLIST_ITEM_HEIGHT			19
#define dDWC_FIELDLIST_ITEM_TOP				1
#define dDWC_FIELDLIST_ITEM_GAP				21
#define dDWC_FIELDLIST_ITEM_LIST_HEIGHT		22		///< tw: the list grows by 22 per item
#define dDWC_FIELDLIST_VISIBLE_ITEM			5
#define dDWC_FIELDLIST_WHEEL_STEP			18

#define dDWC_FIELDLIST_METER_X				117
#define dDWC_FIELDLIST_METER_Y				4
#define dDWC_FIELDLIST_METER_GAP			19
#define dDWC_FIELDLIST_METER_WIDTH			18
#define dDWC_FIELDLIST_METER_HEIGHT			11

// a full meter icon per 20 % of the admission count, a half one for the rest from 10 %
#define dDWC_FIELDLIST_METER_FULL_STEP		20
#define dDWC_FIELDLIST_METER_HALF_STEP		10

// a click opens the question once a second (tw needs a double click)
#define dDWC_FIELDLIST_CLICK_DELAY			1000

#define dDWC_FIELDLIST_DISABLE_COLOR		RGB(183, 191, 195)


CDwcFieldListGui::CDwcFieldListGui(const RwChar* pName)
: CNtlPLGui(pName)
{
	m_pstbTitle = NULL;
	m_ppnlFieldList = NULL;
	m_pscbScroll = NULL;
	m_pbtnExit = NULL;
	m_hNPCSerial = INVALID_SERIAL_ID;
	m_byRemainCount = 0;
	m_iListTop = 0;
}

CDwcFieldListGui::~CDwcFieldListGui(VOID)
{
}

RwBool CDwcFieldListGui::Create(VOID)
{
	NTL_FUNCTION("CDwcFieldListGui::Create");

	if (!CNtlPLGui::Create("gui\\DWC.rsr", "gui\\DWC.srf", "gui\\DWC_FieldList.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(GetNtlGuiManager()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");
	m_pThis->SetPriority(dDIALOGPRIORITY_DEFAULT);

	m_pstbTitle = (gui::CStaticBox*)GetComponent("stbTitle");
	m_pstbTitle->SetText(GetDisplayStringManager()->GetString("DST_DWC_FIELD_ENTER"));

	m_ppnlFieldList = (gui::CPanel*)GetComponent("pnlFieldList");
	m_iListTop = m_ppnlFieldList->GetPosition().top;
	m_ppnlFieldList->SetClippingMode(true);

	m_pscbScroll = (gui::CScrollBar*)GetComponent("scbScroll");
	m_slotScrollChanged = m_pscbScroll->SigValueChanged().Connect(this, &CDwcFieldListGui::OnScrollChanged);
	m_slotScrollMoved = m_pscbScroll->SigSliderMoved().Connect(this, &CDwcFieldListGui::OnScrollChanged);

	m_pbtnExit = (gui::CButton*)GetComponent("btnExit");
	m_pbtnExit->SetText(GetDisplayStringManager()->GetString("DST_LOBBY_EXIT"));
	m_slotClickExit = m_pbtnExit->SigClicked().Connect(this, &CDwcFieldListGui::OnClickExit);

	m_slotMouseWheel = GetNtlGuiManager()->GetGuiManager()->SigCaptureWheelMove().Connect(this, &CDwcFieldListGui::OnMouseWheel);

	LinkMsg(g_EventNPCDialogOpen);
	LinkMsg(g_EventDialog);
	LinkMsg(g_EventDwcNotify);
	LinkMsg(g_EventResize);

	Show(false);

	NTL_RETURN(TRUE);
}

VOID CDwcFieldListGui::Destroy(VOID)
{
	NTL_FUNCTION("CDwcFieldListGui::Destroy");

	UnLinkMsg(g_EventNPCDialogOpen);
	UnLinkMsg(g_EventDialog);
	UnLinkMsg(g_EventDwcNotify);
	UnLinkMsg(g_EventResize);

	ClearFieldList();

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();

	NTL_RETURNVOID();
}

RwInt32 CDwcFieldListGui::SwitchDialog(bool bOpen)
{
	if (bOpen)
	{
		PositionAlign();
	}
	else
	{
		ClearFieldList();
		Logic_CancelNpcFacing();

		m_hNPCSerial = INVALID_SERIAL_ID;
	}

	Show(bOpen);
	return TRUE;
}

VOID CDwcFieldListGui::HandleEvents(RWS::CMsg& msg)
{
	if (msg.Id == g_EventNPCDialogOpen)
	{
		SDboNpcDialogOpen* pData = reinterpret_cast<SDboNpcDialogOpen*>(msg.pData);
		if (pData->eDialog != DIALOG_DWC_FIELDLIST)
			return;

		// tw: the command only asks the server, the answer opens the list
		m_hNPCSerial = pData->hSerialId;
		GetDboGlobal()->GetGamePacketGenerator()->SendDwcWorldCountInfoReq(m_hNPCSerial);
	}
	else if (msg.Id == g_EventDialog)
	{
		SDboEventDialog* pData = reinterpret_cast<SDboEventDialog*>(msg.pData);
		if (pData->iDestDialog != DIALOG_DWC_FIELDLIST)
			return;

		if (pData->iType == DIALOGEVENT_OPEN_FAIL_NPC_DIALOG)
		{
			Logic_CancelNpcFacing();
			GetDialogManager()->CloseDialog(DIALOG_DWC_FIELDLIST);
		}
		else if (pData->iType == DIALOGEVENT_NPC_BYEBYE)
		{
			GetDialogManager()->CloseDialog(DIALOG_DWC_FIELDLIST);
		}
	}
	else if (msg.Id == g_EventDwcNotify)
	{
		SDboEventDwcNotify* pData = reinterpret_cast<SDboEventDwcNotify*>(msg.pData);

		switch (pData->byType)
		{
			case DWC_EVENT_WORLDCOUNT_INFO:
			{
				sGU_DWC_WORLDCOUNT_INFO_RES* pResult = reinterpret_cast<sGU_DWC_WORLDCOUNT_INFO_RES*>(pData->pData);

				m_hNPCSerial = pResult->hNpc;
				m_byRemainCount = pResult->byLimitCount;
				SetFieldList(pResult->byWorldCount, pResult->asWorldCountInfo);

				if (!GetDialogManager()->IsOpenDialog(DIALOG_DWC_FIELDLIST))
					GetDialogManager()->OpenDialog(DIALOG_DWC_FIELDLIST, m_hNPCSerial);
			}
			break;
			case DWC_EVENT_LIMIT_COUNT:
			{
				m_byRemainCount = (RwUInt8)pData->uiValue;
			}
			break;
			case DWC_EVENT_ENTER_CONFIRM:
			case DWC_EVENT_ENTER_DIRECTION:
			{
				// tw: the list closes once the entry was answered
				if (GetDialogManager()->IsOpenDialog(DIALOG_DWC_FIELDLIST))
					GetDialogManager()->CloseDialog(DIALOG_DWC_FIELDLIST);
			}
			break;
		}
	}
	else if (msg.Id == g_EventResize)
	{
		if (IsShow())
			PositionAlign();
	}
}

VOID CDwcFieldListGui::SetFieldList(RwUInt8 byWorldCount, const sDWC_WORLD_COUNT_INFO* pInfo)
{
	ClearFieldList();

	CDwcTable* pDwcTable = API_GetTableContainer()->GetDwcTable();
	CTextTable* pTextTable = API_GetTableContainer()->GetTextAllTable()->GetDWCTbl();
	gui::CSurfaceManager* pSurfaceManager = GetNtlGuiManager()->GetSurfaceManager();

	for (RwUInt8 i = 0; i < byWorldCount && i < MAX_DWC_COUNT; ++i)
	{
		sDWC_TBLDAT* pDwcTbldat = reinterpret_cast<sDWC_TBLDAT*>(pDwcTable->FindData(pInfo[i].tblidx));
		if (pDwcTbldat == NULL)
			continue;

		sFIELD_ITEM* pItem = NTL_NEW sFIELD_ITEM;
		pItem->dwcTblidx = pInfo[i].tblidx;
		pItem->uiLastClickTime = 0;

		CRectangle rect;
		rect.SetRectWH(0, dDWC_FIELDLIST_ITEM_TOP + (RwInt32)m_vecItem.size() * dDWC_FIELDLIST_ITEM_GAP, dDWC_FIELDLIST_ITEM_WIDTH, dDWC_FIELDLIST_ITEM_HEIGHT);
		pItem->pButton = NTL_NEW gui::CButton(rect, m_ppnlFieldList, pSurfaceManager);
		pItem->pButton->AddSurfaceUp(pSurfaceManager->GetSurface("DWC.srf", "srfFieldItem_Normal"));
		pItem->pButton->AddSurfaceDown(pSurfaceManager->GetSurface("DWC.srf", "srfFieldItem_Down"));
		pItem->pButton->AddSurfaceDisabled(pSurfaceManager->GetSurface("DWC.srf", "srfFieldItem_Normal"));
		pItem->pButton->AddSurfaceFocus(pSurfaceManager->GetSurface("DWC.srf", "srfFieldItem_Up"));
		pItem->pButton->SetTextFont(DEFAULT_FONT, 80, DEFAULT_FONT_ATTR);
		pItem->pButton->SetTextStyle(COMP_TEXT_LEFT);
		pItem->pButton->SetTextCoord(5, 4);
		pItem->pButton->SetTextDownCoordDiff(0, 0);
		// tw: a dwc the avatar can not enter yet (level, admission conditions) is greyed and can not be clicked
		pItem->pButton->SetTextUpColor(RGB(255, 255, 255));
		pItem->pButton->SetTextFocusColor(RGB(255, 255, 255));
		pItem->pButton->SetTextDownColor(RGB(255, 255, 255));
		pItem->pButton->SetTextDisableColor(dDWC_FIELDLIST_DISABLE_COLOR);
		pItem->pButton->Enable(Logic_IsDwcEnterable(pDwcTbldat) ? true : false);
		if (pTextTable)
			pItem->pButton->SetText(pTextTable->GetText(pDwcTbldat->tblNameIndex));
		pItem->pButton->ApplyText();
		pItem->pButton->SetClippingMode(true);
		pItem->slotClicked = pItem->pButton->SigClicked().Connect(this, &CDwcFieldListGui::OnClickItem);

		// tw: a half and a full icon per meter slot, the busy rate picks which one shows
		for (RwInt32 j = 0; j < dDWC_FIELDLIST_METER_COUNT; ++j)
		{
			CRectangle rtMeter;
			rtMeter.SetRectWH(dDWC_FIELDLIST_METER_X + j * dDWC_FIELDLIST_METER_GAP, dDWC_FIELDLIST_METER_Y, dDWC_FIELDLIST_METER_WIDTH, dDWC_FIELDLIST_METER_HEIGHT);

			pItem->apHalfMeter[j] = NTL_NEW gui::CPanel(rtMeter, pItem->pButton, pSurfaceManager, pSurfaceManager->GetSurface("DWC.srf", "srfFieldStateLevel1"));
			pItem->apFullMeter[j] = NTL_NEW gui::CPanel(rtMeter, pItem->pButton, pSurfaceManager, pSurfaceManager->GetSurface("DWC.srf", "srfFieldStateLevel2"));
			pItem->apHalfMeter[j]->Enable(false);
			pItem->apFullMeter[j]->Enable(false);
			pItem->apHalfMeter[j]->SetClippingMode(true);
			pItem->apFullMeter[j]->SetClippingMode(true);
		}

		SetMeter(pItem, pInfo[i].wUseCount);

		m_vecItem.push_back(pItem);
	}

	// tw: the list scrolls by pixel once more than 5 dwcs are listed
	RwInt32 iCount = (RwInt32)m_vecItem.size();
	m_ppnlFieldList->SetHeight(iCount * dDWC_FIELDLIST_ITEM_LIST_HEIGHT);
	// SetHeight keeps the old clipping rect, a move recalculates it for the panel and every child
	m_ppnlFieldList->SetPosition(m_ppnlFieldList->GetPosition().left, m_iListTop + 1);
	m_ppnlFieldList->SetPosition(m_ppnlFieldList->GetPosition().left, m_iListTop);

	if (iCount > dDWC_FIELDLIST_VISIBLE_ITEM)
	{
		m_pscbScroll->SetRange(0, (iCount - dDWC_FIELDLIST_VISIBLE_ITEM) * dDWC_FIELDLIST_ITEM_LIST_HEIGHT);
		m_pscbScroll->SetValue(0, FALSE);
		m_pscbScroll->Show(true);
	}
	else
	{
		m_pscbScroll->SetRange(0, 0);
		m_pscbScroll->SetValue(0, FALSE);
		m_pscbScroll->Show(false);
	}
}

VOID CDwcFieldListGui::SetMeter(sFIELD_ITEM* pItem, RwUInt16 wUseCount)
{
	RwInt32 iFull = wUseCount / dDWC_FIELDLIST_METER_FULL_STEP;
	RwBool bHalf = (wUseCount % dDWC_FIELDLIST_METER_FULL_STEP) >= dDWC_FIELDLIST_METER_HALF_STEP;

	for (RwInt32 j = 0; j < dDWC_FIELDLIST_METER_COUNT; ++j)
	{
		pItem->apFullMeter[j]->Show(j < iFull);
		pItem->apHalfMeter[j]->Show(j == iFull && bHalf);
	}
}

VOID CDwcFieldListGui::ClearFieldList(VOID)
{
	for (std::vector<sFIELD_ITEM*>::iterator it = m_vecItem.begin(); it != m_vecItem.end(); ++it)
	{
		sFIELD_ITEM* pItem = *it;

		for (RwInt32 j = 0; j < dDWC_FIELDLIST_METER_COUNT; ++j)
		{
			NTL_DELETE(pItem->apHalfMeter[j]);
			NTL_DELETE(pItem->apFullMeter[j]);
		}

		NTL_DELETE(pItem->pButton);
		NTL_DELETE(pItem);
	}

	m_vecItem.clear();
}

VOID CDwcFieldListGui::PositionAlign(VOID)
{
	// tw: the list sits in the middle of the screen
	RwInt32 iX = (RwInt32)GetDboGlobal()->GetScreenWidth() / 2 - m_pThis->GetWidth() / 2;
	RwInt32 iY = (RwInt32)GetDboGlobal()->GetScreenHeight() / 2 - m_pThis->GetHeight() / 2;

	m_pThis->SetPosition(iX, iY);
}

VOID CDwcFieldListGui::OnClickItem(gui::CComponent* pComponent)
{
	for (std::vector<sFIELD_ITEM*>::iterator it = m_vecItem.begin(); it != m_vecItem.end(); ++it)
	{
		sFIELD_ITEM* pItem = *it;
		if (pItem->pButton != pComponent)
			continue;

		RwUInt32 uiTime = GetTickCount();
		if (pItem->uiLastClickTime != 0 && uiTime - pItem->uiLastClickTime < dDWC_FIELDLIST_CLICK_DELAY)
			return;

		pItem->uiLastClickTime = uiTime;

		if (GetMsgBoxManager()->GetMsgBox("DST_DWC_ENTER_QUESTION"))
			return;

		sMsgBoxData sData;
		sData.sDwcEnter.hNpc = m_hNPCSerial;
		sData.sDwcEnter.dwcTblidx = pItem->dwcTblidx;

		GetAlarmManager()->FormattedAlarmMessage("DST_DWC_ENTER_QUESTION", FALSE, &sData, m_byRemainCount);
		return;
	}
}

VOID CDwcFieldListGui::OnClickExit(gui::CComponent* pComponent)
{
	GetDialogManager()->CloseDialog(DIALOG_DWC_FIELDLIST);
}

VOID CDwcFieldListGui::OnScrollChanged(RwInt32 iValue)
{
	m_ppnlFieldList->SetPosition(m_ppnlFieldList->GetPosition().left, m_iListTop - iValue);
}

VOID CDwcFieldListGui::OnMouseWheel(RwInt32 nFlag, RwInt16 sDelta, CPos& pos)
{
	if (!IsShow() || !m_pscbScroll->IsVisible())
		return;

	if (m_pThis->GetParent()->GetChildComponentReverseAt(pos.x, pos.y) != m_pThis)
		return;

	if (m_pThis->PosInRect(pos.x, pos.y) != gui::CComponent::INRECT)
		return;

	// tw: 18 pixels per wheel notch
	RwInt32 iValue = m_pscbScroll->GetValue() - sDelta * dDWC_FIELDLIST_WHEEL_STEP / GUI_MOUSE_WHEEL_DELTA;
	if (iValue < 0)
		iValue = 0;
	else if (iValue > m_pscbScroll->GetMaxValue())
		iValue = m_pscbScroll->GetMaxValue();

	m_pscbScroll->SetValue(iValue);
}
