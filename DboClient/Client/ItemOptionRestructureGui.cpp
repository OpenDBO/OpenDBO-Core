#include "precomp_dboclient.h"
#include <math.h>

// core
#include "ItemOptionRestructureGui.h"
#include "NtlDebug.h"

// shared

// presentation
#include "NtlPLGuiManager.h"

// simulation 
#include "NtlSLEvent.h"
#include "NtlSobNpc.h"
#include "NtlSobManager.h"
#include "NtlSobAvatar.h"
#include "NtlSLGlobal.h"
#include "NtlSobItem.h"
#include "NtlSobItemAttr.h"
#include "NtlSobItemIcon.h"

// dbo
#include "DboGlobal.h"
#include "IconMoveManager.h"
#include "DialogManager.h"
#include "InfoWndManager.h"

//table
#include "SystemEffectTable.h"
#include "ItemOptionTable.h"
#include "ItemEnchantTable.h"


// process effect of the tw client: everything blinks for 3s, then the request is sent
#define OPTION_RESTRUCTURE_PROCESS_TIME		3.0f
#define OPTION_RESTRUCTURE_SLOT_BLINK		1.0f	// item icon fades out and back in, 3 times
#define OPTION_RESTRUCTURE_OPTION_BLINK		0.5f	// current options fade out and back in, 6 times
#define OPTION_RESTRUCTURE_ARROW_BLINK		1.0f	// each arrow fades in and back out, red -> yellow -> blue
#define OPTION_RESTRUCTURE_NAME_COLOR		RGB(255, 255, 255)

// alpha of one fade out and back in, fTime is the time since the blink started
static RwUInt8 GetBlinkAlpha(RwReal fTime, RwReal fPeriod)
{
	RwReal fRatio = fmodf(fTime, fPeriod) / (fPeriod * 0.5f);

	if (fRatio < 1.0f)
		return (RwUInt8)(255.0f * (1.0f - fRatio));

	fRatio -= 1.0f;
	if (fRatio > 1.0f)
		fRatio = 1.0f;

	return (RwUInt8)(255.0f * fRatio);
}


CItemOptionRestructureGui::CItemOptionRestructureGui(const RwChar *pName)
	:CNtlPLGui(pName)
{
	Init();
}

CItemOptionRestructureGui::~CItemOptionRestructureGui()
{
	Init();
}

void CItemOptionRestructureGui::Init()
{
	m_pItem = NULL;
	m_pBoxItem = NULL;
	m_nBoxStackNum = 0;

	m_nLSelectedSlotIdx = -1;
	m_nRSelectedSlotIdx = -1;

	for (RwInt32 i = 0; i < NUM_SLOT; ++i)
	{
		m_anFocusEffect[i] = SLOT_FOCUS_NONE;
	}

	m_pstbStackCount = NULL;

	m_bCanSelectOption = false;

	m_eState = STATE_NONE;

	m_wOptionIndex = INVALID_WORD;
	m_bAfterIsPreview = false;

	m_fEffectTime = 0.0f;
}

RwBool CItemOptionRestructureGui::Create()
{
	NTL_FUNCTION("CItemOptionRestructureGui::Create");

	if (!CNtlPLGui::Create("gui\\OptionRestructure.rsr", "gui\\OptionRestructure.srf", "gui\\OptionRestructure.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");
	m_pbtnClose = (gui::CButton*)GetComponent("btnClose");
	m_pbtnBuyItem = (gui::CButton*)GetComponent("btnBuyItem");
	m_pbtnRestructure = (gui::CButton*)GetComponent("btnRestructure");
	m_pbtnCloseWide = (gui::CButton*)GetComponent("btnCloseWide");

	m_pstbTitle = (gui::CStaticBox*)GetComponent("stbTitle");
	m_pstbTargetItemTitle = (gui::CStaticBox*)GetComponent("stbTargetItemTitle");
	m_pstbOptionKitTitle = (gui::CStaticBox*)GetComponent("stbOptionKitTitle");
	m_pstbTargetOptionTitle = (gui::CStaticBox*)GetComponent("stbTargetOptionTitle");
	m_pstbRestructureOptionTitle = (gui::CStaticBox*)GetComponent("stbRestructureOptionTitle");

	
	m_pcbbSelectOption = (gui::CComboBox*)GetComponent("cbbSelectOption");
	m_ppnlOptionKitItemSlot = (gui::CPanel*)GetComponent("pnlOptionKitItemSlot");
	m_ppnlTargetItemSlot = (gui::CPanel*)GetComponent("pnlTargetItemSlot");


	m_ppnlArrow[ARROW_BLUE] = (gui::CPanel*)GetComponent("pnlArrowBlue");
	m_ppnlArrow[ARROW_YELLOW] = (gui::CPanel*)GetComponent("pnlArrowYellow");
	m_ppnlArrow[ARROW_RED] = (gui::CPanel*)GetComponent("pnlArrowRed");
	m_ppnlStarCircle = (gui::CPanel*)GetComponent("pnlStarCircle");

	RwChar buf[64];
	RwChar buf2[64];
	RwChar buf3[64];
	RwChar buf4[64];

	for (int i = 0; i < ITEM_OPTION_MAX_COUNT; i++)
	{
		sprintf_s(buf, 64, "stbAfterOptionName_0%d", i);
		sprintf_s(buf2, 64, "stbAfterOptionValue_0%d", i);

		sprintf_s(buf3, 64, "stbBeforeOptionName_0%d", i);
		sprintf_s(buf4, 64, "stbBeforeOptionValue_0%d", i);

		m_pstbAfterOptionName[i] = (gui::CStaticBox*)GetComponent(buf);
		m_pstbAfterOptionValue[i] = (gui::CStaticBox*)GetComponent(buf2);

		m_pstbBeforeOptionName[i] = (gui::CStaticBox*)GetComponent(buf3);
		m_pstbBeforeOptionValue[i] = (gui::CStaticBox*)GetComponent(buf4);

		m_pstbBeforeOptionName[i]->SetText(L"");
		m_pstbBeforeOptionValue[i]->SetText(L"");

		m_pstbAfterOptionName[i]->SetText(L"");
		m_pstbAfterOptionValue[i]->SetText(L"");
	}

	m_slotPaint = m_pThis->SigPaint().Connect(this, &CItemOptionRestructureGui::OnPaint);
	m_slotMouseUp = m_pThis->SigMouseUp().Connect(this, &CItemOptionRestructureGui::OnMouseUp);
	m_slotMove = m_pThis->SigMove().Connect(this, &CItemOptionRestructureGui::OnMove);


	m_slotSelectedOption = m_pcbbSelectOption->SigSelected().Connect(this, &CItemOptionRestructureGui::OnSelectedOption);

	m_slotCloseBtnClick = m_pbtnClose->SigClicked().Connect(this, &CItemOptionRestructureGui::OnClickedBtnClose);
	m_slotBuyItemBtnClick = m_pbtnBuyItem->SigClicked().Connect(this, &CItemOptionRestructureGui::OnClickedBtnBuy);
	m_slotRestructureBtnClick = m_pbtnRestructure->SigClicked().Connect(this, &CItemOptionRestructureGui::OnClickedBtnRestructure);
	m_slotCloseWideBtnClick = m_pbtnCloseWide->SigClicked().Connect(this, &CItemOptionRestructureGui::OnClickedBtnCloseWide);

	m_slotItemIconMouseEnter = m_ppnlTargetItemSlot->SigMouseEnter().Connect(this, &CItemOptionRestructureGui::OnItemIconMouseEnter);
	m_slotItemIconMouseLeave = m_ppnlTargetItemSlot->SigMouseLeave().Connect(this, &CItemOptionRestructureGui::OnItemIconMouseLeave);
	m_slotItemIconMouseDown = m_ppnlTargetItemSlot->SigMouseDown().Connect(this, &CItemOptionRestructureGui::OnItemIconMouseDown);

	m_slotBoxItemIconMouseEnter = m_ppnlOptionKitItemSlot->SigMouseEnter().Connect(this, &CItemOptionRestructureGui::OnBoxItemIconMouseEnter);
	m_slotBoxItemIconMouseLeave = m_ppnlOptionKitItemSlot->SigMouseLeave().Connect(this, &CItemOptionRestructureGui::OnBoxItemIconMouseLeave);
	m_slotBoxItemIconMouseDown = m_ppnlOptionKitItemSlot->SigMouseDown().Connect(this, &CItemOptionRestructureGui::OnBoxItemIconMouseDown);


	m_pbtnBuyItem->SetText(GetDisplayStringManager()->GetString("DST_ITEM_OPTION_RESTRUCTURE_BUY"));
	m_pbtnRestructure->SetText(GetDisplayStringManager()->GetString("DST_ITEM_OPTION_RESTRUCTURE_START"));
	m_pbtnCloseWide->SetText(GetDisplayStringManager()->GetString("DST_ITEM_OPTION_RESTRUCTURE_CLOSE"));

	m_pstbTitle->SetText(GetDisplayStringManager()->GetString("DST_ITEM_OPTION_RESTRUCTURE_TITLE"));
	m_pstbTargetItemTitle->SetText(GetDisplayStringManager()->GetString("DST_ITEM_OPTION_RESTRUCTURE_TARGET_ITEM_TITLE"));
	m_pstbOptionKitTitle->SetText(GetDisplayStringManager()->GetString("DST_ITEM_OPTION_RESTRUCTURE_OPTION_KIT_TITLE"));
	m_pstbTargetOptionTitle->SetText(GetDisplayStringManager()->GetString("DST_ITEM_OPTION_RESTRUCTURE_TARGET_OPTION_TITLE"));
	m_pstbRestructureOptionTitle->SetText(GetDisplayStringManager()->GetString("DST_ITEM_OPTION_RESTRUCTURE_RESTRUCTURE_OPTION_TITLE"));

	m_pcbbSelectOption->Raise();

	SetSlotRectHardCode();

	ShowArrows(false);

	Show(false);

	GetNtlGuiManager()->AddUpdateFunc(this);

	// event register.
	LinkMsg(g_EventRegOptionReconstructItem);
	LinkMsg(g_EventIconMoveClick);
	LinkMsg(g_EventItemChangeOptionConfirmation);
	LinkMsg(g_EventItemChangeOptionResult);
	LinkMsg(g_EventGameServerChangeOut);

	NTL_RETURN(TRUE);
}

void CItemOptionRestructureGui::Destroy()
{
	NTL_FUNCTION("CItemOptionRestructureGui::Destroy");

	GetNtlGuiManager()->RemoveUpdateFunc(this);

	UnLinkMsg(g_EventRegOptionReconstructItem);
	UnLinkMsg(g_EventIconMoveClick);
	UnLinkMsg(g_EventItemChangeOptionConfirmation);
	UnLinkMsg(g_EventItemChangeOptionResult);
	UnLinkMsg(g_EventGameServerChangeOut);

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();

	NTL_RETURNVOID();
}

RwInt32 CItemOptionRestructureGui::SwitchDialog(bool bOpen)
{
	if (bOpen)
	{
		CenterAlign(GetDboGlobal()->GetScreenWidth(), GetDboGlobal()->GetScreenHeight());

		DeleteItem();
		DeleteBoxItem();

		SetState(STATE_NONE);

		Show(bOpen);
	}
	else
	{
		if (m_eState == STATE_PROCESS || m_eState == STATE_PACKETWAIT)
			return -1;

		if (GetIconMoveManager()->GetSrcPlace() == PLACE_ITEM_OPTIONRECONSTRUCTION)
			GetIconMoveManager()->IconMoveEnd();

		Show(bOpen);

		DeleteItem();
		DeleteBoxItem();
	}

	return 1;
}

void CItemOptionRestructureGui::Update(RwReal fElapsed)
{
	if (!IsShow())
		return;

	UpdateProcessEffect(fElapsed);
}

void CItemOptionRestructureGui::HandleEvents(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CItemOptionRestructureGui::HandleEvents");

	if (pMsg.Id == g_EventRegOptionReconstructItem)
	{
		SDboEventRegMailAttachItem* pData = (SDboEventRegMailAttachItem*)pMsg.pData;

		CNtlSobItem* pItem = reinterpret_cast<CNtlSobItem*>(GetNtlSobManager()->GetSobObject(pData->hHandle));
		CNtlSobItemAttr* pItemAttr = reinterpret_cast<CNtlSobItemAttr*>(pItem->GetSobAttr());

		sITEM_TBLDAT* pItemTbl = pItemAttr->GetItemTbl();

		if (IsValidBoxItem(pItemAttr))
		{
			if (IsShow() == FALSE)
			{
				GetDialogManager()->OpenDialog(DIALOG_ITEM_OPTIONRESTRUCTURE);
			}

			SetItemSlot(pData->hHandle, PLACE_BAG, (RwUInt8)pData->uiPos, SLOT_BOX, FALSE);
		}
		else if(IsValidItem(pItemAttr))
		{
			SetItemSlot(pData->hHandle, pData->uiPlace, (RwUInt8)pData->uiPos, SLOT_ITEM, FALSE);
		}
	}
	else if (pMsg.Id == g_EventIconMoveClick)
	{
		RwBool bPickUpState = (RwBool)pMsg.pData;
		ShowIconDestination(bPickUpState);
	}
	else if (pMsg.Id == g_EventItemChangeOptionConfirmation)
	{
		if (m_eState == STATE_READY)
			SetState(STATE_PROCESS);
	}
	else if (pMsg.Id == g_EventItemChangeOptionResult)
	{
		if (!GetDialogManager()->IsOpenDialog(DIALOG_ITEM_OPTIONRESTRUCTURE))
			NTL_RETURNVOID();

		SDboItemChangeOptionResult* pData = reinterpret_cast<SDboItemChangeOptionResult*>(pMsg.pData);

		SetResult(pData->pItemOptionSet, pData->byBoxCount);
	}
	else if (pMsg.Id == g_EventGameServerChangeOut)
	{
		if (m_eState == STATE_PROCESS) // request was not sent yet, drop it
			SetState(STATE_NONE);

		GetDialogManager()->CloseDialog(DIALOG_ITEM_OPTIONRESTRUCTURE);
	}

	NTL_RETURNVOID();
}




void CItemOptionRestructureGui::SetSlotRectHardCode()
{
	CRectangle rtItemScreen = m_ppnlTargetItemSlot->GetScreenRect();

	m_surSlot[SLOT_ITEM].SetRectWH(rtItemScreen.left, rtItemScreen.top, DBOGUI_ICON_SIZE, DBOGUI_ICON_SIZE);
	m_asurFocus[SLOT_ITEM].SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("GameCommon.srf", "srfSlotFocusEffect"));
	m_asurFocus[SLOT_ITEM].SetRectWH(rtItemScreen.left, rtItemScreen.top, DBOGUI_ICON_SIZE, DBOGUI_ICON_SIZE);
	m_asurDisableSlot[SLOT_ITEM].SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("GameCommon.srf", "srfSlotDisableEffect"));
	m_asurDisableSlot[SLOT_ITEM].SetRectWH(rtItemScreen.left, rtItemScreen.top, DBOGUI_ICON_SIZE, DBOGUI_ICON_SIZE);


	CRectangle rtBoxItemScreen = m_ppnlOptionKitItemSlot->GetScreenRect();

	m_surSlot[SLOT_BOX].SetRectWH(rtBoxItemScreen.left, rtBoxItemScreen.top, DBOGUI_ICON_SIZE, DBOGUI_ICON_SIZE);
	m_asurFocus[SLOT_BOX].SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("GameCommon.srf", "srfSlotFocusEffect"));
	m_asurFocus[SLOT_BOX].SetRectWH(rtBoxItemScreen.left, rtBoxItemScreen.top, DBOGUI_ICON_SIZE, DBOGUI_ICON_SIZE);
	m_asurDisableSlot[SLOT_BOX].SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("GameCommon.srf", "srfSlotDisableEffect"));
	m_asurDisableSlot[SLOT_BOX].SetRectWH(rtBoxItemScreen.left, rtBoxItemScreen.top, DBOGUI_ICON_SIZE, DBOGUI_ICON_SIZE);
}

void CItemOptionRestructureGui::SetItemCurrentStats()
{
	if (m_pItem)
	{
		CNtlSobItemAttr* pItemAttr = reinterpret_cast<CNtlSobItemAttr*>(m_pItem->GetSobAttr());

		SetOptionRows(m_pstbBeforeOptionName, m_pstbBeforeOptionValue, pItemAttr->GetOptionSet());
	}
}

void CItemOptionRestructureGui::SetOptionRows(gui::CStaticBox** ppstbName, gui::CStaticBox** ppstbValue, sITEM_OPTION_SET* pOptionSet)
{
	ClearOptionRows(ppstbName, ppstbValue);

	if (pOptionSet == NULL)
		return;

	CItemEnchantTable* pItemEnchantTable = API_GetTableContainer()->GetItemEnchantTable();
	CItemOptionTable* pItemOptionTable = API_GetTableContainer()->GetItemOptionTable();
	CTextTable* pSystemEffectText = API_GetTableContainer()->GetTextAllTable()->GetSystemEffectTbl();

	int nRow = 0;

	// fixed options
	for (int i = 0; i < NTL_MAX_OPTION_IN_ITEM && nRow < ITEM_OPTION_MAX_COUNT; i++)
	{
		if (pOptionSet->aOptionTblidx[i] == INVALID_TBLIDX)
			continue;

		sITEM_OPTION_TBLDAT* pItemOptionData = reinterpret_cast<sITEM_OPTION_TBLDAT*>(pItemOptionTable->FindData(pOptionSet->aOptionTblidx[i]));
		if (pItemOptionData == NULL)
			continue;

		for (int ii = 0; ii < NTL_MAX_SYSTEM_EFFECT_COUNT && nRow < ITEM_OPTION_MAX_COUNT; ii++)
		{
			if (pItemOptionData->system_Effect[ii] == INVALID_TBLIDX)
				break;

			std::wstring wstrName = pSystemEffectText->GetText((pItemOptionData->system_Effect[ii] * 100) + 1);

			ppstbName[nRow]->SetText(wstrName.c_str());
			ppstbValue[nRow]->SetText(pItemOptionData->nValue[ii]);
			++nRow;
		}
	}

	// random options
	for (int i = 0; i < NTL_MAX_SYSTEM_EFFECT_COUNT && nRow < ITEM_OPTION_MAX_COUNT; i++)
	{
		if (pOptionSet->GetRandomOptionIndex(i) == INVALID_WORD)
			break;

		sITEM_ENCHANT_TBLDAT* pItemEnchantData = reinterpret_cast<sITEM_ENCHANT_TBLDAT*>(pItemEnchantTable->FindData(pOptionSet->GetRandomOptionIndex(i)));
		if (pItemEnchantData == NULL)
			continue;

		std::wstring wstrName = pSystemEffectText->GetText((pItemEnchantData->seTblidx * 100) + 1);

		ppstbName[nRow]->SetText(wstrName.c_str());
		ppstbValue[nRow]->SetText(pOptionSet->GetRandomOptionValue(i));
		++nRow;
	}
}

void CItemOptionRestructureGui::ClearOptionRows(gui::CStaticBox** ppstbName, gui::CStaticBox** ppstbValue)
{
	for (int i = 0; i < ITEM_OPTION_MAX_COUNT; i++)
	{
		ppstbName[i]->SetTextColor(OPTION_RESTRUCTURE_NAME_COLOR);
		ppstbName[i]->SetText(L"");
		ppstbValue[i]->SetText(L"");
	}

	if (ppstbName == m_pstbAfterOptionName)
		m_bAfterIsPreview = false;
}

void CItemOptionRestructureGui::EnableSelectOption(bool bFlag)
{
	m_bCanSelectOption = bFlag;

	m_pcbbSelectOption->SetText(L"");

	m_wOptionIndex = INVALID_WORD;

	if (m_bAfterIsPreview)
		ClearOptionRows(m_pstbAfterOptionName, m_pstbAfterOptionValue);

	if (bFlag && m_pItem)
	{
		m_pcbbSelectOption->Enable(true);
		m_pcbbSelectOption->ClearAllItems();

		CItemEnchantTable* pItemEnchantTable = API_GetTableContainer()->GetItemEnchantTable();

		CNtlSobItemAttr* pItemAttr = reinterpret_cast<CNtlSobItemAttr*>(m_pItem->GetSobAttr());

		sITEM_TBLDAT* pItemTbl = pItemAttr->GetItemTbl();

		std::vector<sITEM_ENCHANT_TBLDAT*> vec;

		if (pItemEnchantTable->GetVectorRandomOptionEnchantData(pItemTbl, pItemAttr->GetRank(), &vec))
		{
			for (std::vector<sITEM_ENCHANT_TBLDAT*>::iterator it = vec.begin(); it != vec.end(); it++)
			{
				TBLIDX seTblidx = (*it)->seTblidx;

				std::wstring wstrName = API_GetTableContainer()->GetTextAllTable()->GetSystemEffectTbl()->GetText((seTblidx * 100) + 1);

				m_pcbbSelectOption->AddItem(wstrName.c_str(), (*it)->tblidx);
			}
		}
	}
	else
	{
		m_pcbbSelectOption->Enable(false);
	}
}

bool CItemOptionRestructureGui::CreateStackNumber(RwInt32 nValue)
{
	NTL_FUNCTION("CItemOptionRestructureGui::CreateStackNumber");

	// check if already exist.. update text
	if (m_pstbStackCount)
	{
		m_pstbStackCount->SetText(nValue);
		return true;
	}

	CRectangle rtScreen = m_ppnlOptionKitItemSlot->GetScreenRect();

	CRectangle rect;
	rect.SetRectWH(1, 0, DBOGUI_STACKNUM_WIDTH, DBOGUI_STACKNUM_HEIGHT);

	m_pstbStackCount = NTL_NEW gui::CStaticBox(rect, m_ppnlOptionKitItemSlot, GetNtlGuiManager()->GetSurfaceManager(), DBOGUI_STACKNUM_ALIGN);

	if (!m_pstbStackCount)
		return false;

	m_pstbStackCount->CreateFontStd(DBOGUI_STACKNUM_FONT, DBOGUI_STACKNUM_FONTHEIGHT, DBOGUI_STACKNUM_FONTATTR);
	m_pstbStackCount->SetEffectMode(DBOGUI_STACKNUM_FONTEFFECTMODE);
	m_pstbStackCount->SetText(nValue);
	m_pstbStackCount->Enable(false);

	return true;
}

void CItemOptionRestructureGui::DeleteStackNumber()
{
	if (m_pstbStackCount)
		NTL_DELETE(m_pstbStackCount);
}

void CItemOptionRestructureGui::SetResult(void * pOptionSet, BYTE byBoxCount)
{
	// failed. error message is shown by the packet handler and the kit is not consumed
	if (pOptionSet == NULL)
	{
		CheckReady();
		return;
	}

	GetAlarmManager()->AlarmMessage("DST_ITEM_OPTION_RESTRUCTURE_COMPLETE");

	// set before item bonus. the item still holds its old options here, the inventory is updated after this event
	SetItemCurrentStats();

	// set after item bonus
	SetOptionRows(m_pstbAfterOptionName, m_pstbAfterOptionValue, (sITEM_OPTION_SET*)pOptionSet);

	// remove the kit item
	if (byBoxCount == 0)
		DeleteBoxItem();
	else
		CreateStackNumber(byBoxCount);

	CheckReady();
}

void CItemOptionRestructureGui::ShowArrows(bool bShow)
{
	// the arrows are only seen during the process, they start invisible and blink one after another
	for (int i = 0; i < NUM_ARROW; i++)
	{
		m_ppnlArrow[i]->SetAlpha(0);
		m_ppnlArrow[i]->Show(bShow);
	}
}

void CItemOptionRestructureGui::UpdateProcessEffect(RwReal fElapsed)
{
	if (m_eState != STATE_PROCESS)
		return;

	m_fEffectTime += fElapsed;

	// the next state puts the item, its options and the arrows back at rest
	if (m_fEffectTime >= OPTION_RESTRUCTURE_PROCESS_TIME)
	{
		SendPacket();
		return;
	}

	SetBeforeAlpha(GetBlinkAlpha(m_fEffectTime, OPTION_RESTRUCTURE_SLOT_BLINK), GetBlinkAlpha(m_fEffectTime, OPTION_RESTRUCTURE_OPTION_BLINK));

	// arrows fade in and back out instead
	RwInt32 nArrow = (RwInt32)(m_fEffectTime / OPTION_RESTRUCTURE_ARROW_BLINK);

	for (RwInt32 i = 0; i < NUM_ARROW; i++)
		m_ppnlArrow[i]->SetAlpha(i == nArrow ? (RwUInt8)(255 - GetBlinkAlpha(m_fEffectTime, OPTION_RESTRUCTURE_ARROW_BLINK)) : 0);
}

void CItemOptionRestructureGui::SetBeforeAlpha(RwUInt8 bySlotAlpha, RwUInt8 byOptionAlpha)
{
	m_surSlot[SLOT_ITEM].SetAlpha(bySlotAlpha);

	for (int i = 0; i < ITEM_OPTION_MAX_COUNT; i++)
	{
		m_pstbBeforeOptionName[i]->SetAlpha(byOptionAlpha);
		m_pstbBeforeOptionValue[i]->SetAlpha(byOptionAlpha);
	}
}

bool CItemOptionRestructureGui::SetItemSlot(RwUInt32 hSerialID, RwUInt32 eSrcplace, RwUInt32 uiSrcSlotIdx, RwUInt32 uiDestSlotIdx, RwBool bNeedToIconMoveEnd)
{
	if (m_eState == STATE_PACKETWAIT || m_eState == STATE_PROCESS || m_eState == STATE_RESULT)
	{
		GetAlarmManager()->AlarmMessage("DST_ITEMUPGRADE_NOTIFY_CANNOT_CONTROL_STATE");
		return false;
	}

	CNtlSobItem* pItem = reinterpret_cast<CNtlSobItem*>(GetNtlSobManager()->GetSobObject(hSerialID));
	NTL_ASSERT(pItem, "CItemOptionRestructureGui::SetItemSlot : pItem is must be present!");

	CNtlSobItemAttr* pItemAttr = reinterpret_cast<CNtlSobItemAttr*>(pItem->GetSobAttr());

	if (eSrcplace == PLACE_ITEM_OPTIONRECONSTRUCTION)
	{
		if (uiSrcSlotIdx == uiDestSlotIdx)
		{
			if (bNeedToIconMoveEnd)
				GetIconMoveManager()->IconMoveEnd();

			return true;
		}
	}
	else if (eSrcplace == PLACE_BAG)
	{
		if (m_eState == STATE_NONE || m_eState == STATE_READY)
		{
			if (uiDestSlotIdx == SLOT_ITEM)
			{
				if (!IsValidItem(pItemAttr, true))
					return false;

				SetItem(pItem);

				CheckReady();
			}
			else if (uiDestSlotIdx == SLOT_BOX)
			{
				if (!IsValidBoxItem(pItemAttr, true))
				{
					return false;
				}

				SetBoxItem(pItemAttr->GetStackNum(), pItem);

				CheckReady();
			}
			else
			{
				// something wrong
				return false;
			}

			// disable item
			CDboEventGenerator::DialogEvent(DIALOGEVENT_BEGIN_ITEM_IN_OPTIONRECONSTRUCTION, PLACE_ITEM_OPTIONRECONSTRUCTION, PLACE_BAG, pItem->GetParentItemSlotIdx(), pItem->GetItemSlotIdx());
		}
	}
	else
	{
		return false;
	}

	// 아이콘 무브 완료
	if (bNeedToIconMoveEnd)
		GetIconMoveManager()->IconMoveEnd();

	return true;
}

void CItemOptionRestructureGui::UpdateSlotIcon()
{
	if (m_pBoxItem)
	{
		CNtlSobItemAttr* pAttr = reinterpret_cast<CNtlSobItemAttr*>(m_pBoxItem->GetSobAttr());
		CreateStackNumber(pAttr->GetStackNum());
	}
}

void CItemOptionRestructureGui::SetItem(CNtlSobItem * pItem)
{
	if (!pItem)
	{
		NTL_ASSERTFAIL("CItemOptionRestructureGui::SetItem : CNtlSobItem null");
		return;
	}

	if (m_pItem)
		DeleteItem(true);

	m_pItem = pItem;
	m_surSlot[SLOT_ITEM].SetTexture((gui::CTexture*)pItem->GetIcon()->GetImage());

	SetItemCurrentStats();

	if (m_pBoxItem)
	{
		CNtlSobItemAttr* pItemAttr = reinterpret_cast<CNtlSobItemAttr*>(m_pBoxItem->GetSobAttr());

		sITEM_TBLDAT* pItemTbl = pItemAttr->GetItemTbl();

		EnableSelectOption(pItemTbl->tblidx == 11120093 ? true : false);//11120093 = special equipment box that allow us to select options
	}
}

void CItemOptionRestructureGui::SetBoxItem(RwInt32 nStackCount, CNtlSobItem * pItem)
{
	if (!pItem)
	{
		NTL_ASSERTFAIL("CItemOptionRestructureGui::SetBoxItem : CNtlSobItem null");
		return;
	}

	if (m_pBoxItem)
		DeleteBoxItem(true);

	m_pBoxItem = pItem;
	m_nBoxStackNum = nStackCount;
	m_surSlot[SLOT_BOX].SetTexture((gui::CTexture*)pItem->GetIcon()->GetImage());
	CreateStackNumber(nStackCount);

	CNtlSobItemAttr* pItemAttr = reinterpret_cast<CNtlSobItemAttr*>(m_pBoxItem->GetSobAttr());

	sITEM_TBLDAT* pItemTbl = pItemAttr->GetItemTbl();

	EnableSelectOption(pItemTbl->tblidx == 11120093 ? true : false);//11120093 = special equipment box that allow us to select options
}

void CItemOptionRestructureGui::DeleteItem(bool bReplace)
{
	if (m_pItem)
	{
		ClearOptionRows(m_pstbBeforeOptionName, m_pstbBeforeOptionValue);
		ClearOptionRows(m_pstbAfterOptionName, m_pstbAfterOptionValue);

		CDboEventGenerator::DialogEvent(DIALOGEVENT_END_ITEM_IN_OPTIONRECONSTRUCTION, PLACE_ITEM_OPTIONRECONSTRUCTION, PLACE_BAG,
			m_pItem->GetParentItemSlotIdx(), m_pItem->GetItemSlotIdx());

		m_pItem = NULL;
		m_surSlot[SLOT_ITEM].SetTexture((gui::CTexture*)NULL);

		if (bReplace == false)
		{
			EnableSelectOption(false);
			SetState(STATE_NONE);
		}
	}
}

void CItemOptionRestructureGui::DeleteBoxItem(bool bReplace)
{
	if (m_pBoxItem)
	{
		CDboEventGenerator::DialogEvent(DIALOGEVENT_END_ITEM_IN_OPTIONRECONSTRUCTION, PLACE_ITEM_OPTIONRECONSTRUCTION, PLACE_BAG,
			m_pBoxItem->GetParentItemSlotIdx(), m_pBoxItem->GetItemSlotIdx());

		m_pBoxItem = NULL;
		m_surSlot[SLOT_BOX].SetTexture((gui::CTexture*)NULL);

		if (bReplace == false)
		{
			EnableSelectOption(false);
			DeleteStackNumber();
			SetState(STATE_NONE);
		}
	}
}

void CItemOptionRestructureGui::CheckReady()
{
	if (m_pItem && m_pBoxItem)
	{
		if (m_bCanSelectOption == false ||
			m_bCanSelectOption == true && m_wOptionIndex != INVALID_WORD)
		{
			SetState(STATE_READY);
			return;
		}
	}

	SetState(STATE_NONE);
}

void CItemOptionRestructureGui::SetState(STATE eState)
{
	m_pbtnClose->ClickEnable(TRUE);
	m_pbtnCloseWide->ClickEnable(TRUE);
	m_pbtnRestructure->ClickEnable(FALSE);
	m_pcbbSelectOption->Enable(m_bCanSelectOption && m_pItem);

	DisableSlot(FALSE);

	if (eState != STATE_PROCESS)
	{
		SetBeforeAlpha(255, 255);
		ShowArrows(false);
	}

	switch (eState)
	{
		case STATE_NONE:
		{
			if (GetIconMoveManager()->IsActive())
				ShowIconDestination(TRUE);
		}
		break;
		case STATE_READY:
		{
			if (m_pBoxItem == NULL)
			{
				SetState(STATE_NONE);
				return;
			}

			m_pbtnRestructure->ClickEnable(TRUE);
		}
		break;
		case STATE_PROCESS: // play the process effect, request is sent once it is done (see UpdateProcessEffect)
		{
			// slots are not greyed out, input is blocked by the state
			m_pcbbSelectOption->Enable(false);

			// Exit button disabled !!!
			m_pbtnClose->ClickEnable(FALSE);
			m_pbtnCloseWide->ClickEnable(FALSE);

			m_fEffectTime = 0.0f;

			ShowArrows(true);

			// new roll: current list shows what the item has now (last result) and the new list is emptied
			SetItemCurrentStats();
			ClearOptionRows(m_pstbAfterOptionName, m_pstbAfterOptionValue);
		}
		break;
		case STATE_PACKETWAIT: // effect is done, wait for the result
		{
			m_pcbbSelectOption->Enable(false);

			// Exit button disabled !!!
			m_pbtnClose->ClickEnable(FALSE);
			m_pbtnCloseWide->ClickEnable(FALSE);
		}
		break;
		case STATE_RESULT:
		{
			DisableSlot(TRUE);
		}
		break;
	}

	m_eState = eState;
}

void CItemOptionRestructureGui::ShowIconDestination(BOOL isPick)
{
	if (isPick)
	{
		if (GetIconMoveManager()->GetSrcPlace() == PLACE_BAG)
		{
			if (m_eState == STATE_PROCESS || m_eState == STATE_PACKETWAIT || m_eState == STATE_RESULT)
				return;
			else
			{
				CNtlSobItem* pSobItem = reinterpret_cast<CNtlSobItem*>(GetNtlSobManager()->GetSobObject(GetIconMoveManager()->GetSrcSerial()));
				NTL_ASSERT(pSobItem, "CItemOptionRestructureGui::ShowIconDestination : pSobItem is must be present!");

				CNtlSobItemAttr* pSobItemAttr = reinterpret_cast<CNtlSobItemAttr*>(pSobItem->GetSobAttr());
				NTL_ASSERT(pSobItemAttr, "CItemOptionRestructureGui::ShowIconDestination : pSobItemAttr is must be present!");

				if (IsValidItem(pSobItemAttr))
				{
					if (m_pItem == NULL)
						m_anFocusEffect[SLOT_ITEM] = m_anFocusEffect[SLOT_ITEM] |= SLOT_FOCUS_CAN_MOVE;
				}
				else if (IsValidBoxItem(pSobItemAttr))
				{
					if (m_pBoxItem == NULL)
						m_anFocusEffect[SLOT_BOX] = m_anFocusEffect[SLOT_BOX] |= SLOT_FOCUS_CAN_MOVE;
				}
			}
		}
		else if (GetIconMoveManager()->GetSrcPlace() == PLACE_ITEM_OPTIONRECONSTRUCTION)
		{
			if (m_eState == STATE_PROCESS || m_eState == STATE_PACKETWAIT || m_eState == STATE_RESULT)
				return;
			else
			{

			}
		}
	}
	else
	{
		for (RwInt32 i = 0; i < NUM_SLOT; ++i)
		{
			m_anFocusEffect[i] = m_anFocusEffect[i] & ~SLOT_FOCUS_CAN_MOVE;
		}
	}
}

void CItemOptionRestructureGui::DisableSlot(RwBool bDisable)
{
	for (RwInt32 i = 0; i < NUM_SLOT; ++i)
	{
		m_asurDisableSlot[i].Show(bDisable);
	}
}

bool CItemOptionRestructureGui::IsItem(CNtlSobItemAttr * pItemAttr)
{
	if (pItemAttr->IsNeedToIdentify())
		return false;

	sITEM_TBLDAT* pData = pItemAttr->GetItemTbl();

	if (!pData)
		return false;

	if (CanChangeOption(pData->byEquip_Type, pItemAttr->GetRank(), pItemAttr->GetRestrictState(), pData->byNeed_Min_Level) == false)
		return false;

	return true;
}

bool CItemOptionRestructureGui::IsBoxItem(CNtlSobItemAttr * pItemAttr)
{
	if (pItemAttr->IsNeedToIdentify())
		return false;

	sITEM_TBLDAT* pData = pItemAttr->GetItemTbl();

	if (!pData)
		return false;

	if (pData->byItem_Type == ITEM_TYPE_CHANGE_OPTION_KIT)
	{
		return true;
	}

	return false;
}

bool CItemOptionRestructureGui::IsValidItem(CNtlSobItemAttr * pItemAttr, bool bSendError)
{
	if (!IsItem(pItemAttr))
	{
		if (bSendError)
			GetAlarmManager()->AlarmMessage("DST_ITEM_OPTION_RESTRUCTURE_CANNOT_RESTRUCTURE");

		return false;
	}

	return true;
}

bool CItemOptionRestructureGui::IsValidBoxItem(CNtlSobItemAttr * pItemAttr, bool bSendError)
{
	if (!IsBoxItem(pItemAttr))
	{
		if (bSendError)
			GetAlarmManager()->AlarmMessage("DST_ITEM_OPTION_RESTRUCTURE_CANNOT_MOVE");

		return false;
	}

	return true;
}

bool CItemOptionRestructureGui::IsValidIconPickup(RwInt32 nClickIdx)
{
	if (m_eState == STATE_PROCESS ||
		m_eState == STATE_PACKETWAIT ||
		m_eState == STATE_RESULT)
	{
		GetAlarmManager()->AlarmMessage("DST_ITEMUPGRADE_NOTIFY_CANNOT_CONTROL_STATE");
		return false;
	}

	return true;
}

bool CItemOptionRestructureGui::IsValidRightBtnProc(RwInt32 nClickIdx)
{
	if (m_eState == STATE_PROCESS ||
		m_eState == STATE_PACKETWAIT ||
		m_eState == STATE_RESULT)
	{
		GetAlarmManager()->AlarmMessage("DST_ITEMUPGRADE_NOTIFY_CANNOT_CONTROL_STATE");
		return false;
	}

	return true;
}

void CItemOptionRestructureGui::SendPacket()
{
	if (m_pItem == NULL || m_pBoxItem == NULL)
	{
		CheckReady();
		return;
	}

	RwUInt8 byItemPlace = m_pItem->GetParentItemSlotIdx() + CONTAINER_TYPE_BAG1;
	RwUInt8 byItemPos = m_pItem->GetItemSlotIdx();

	RwUInt8 byBoxPlace = m_pBoxItem->GetParentItemSlotIdx() + CONTAINER_TYPE_BAG1;
	RwUInt8 byBoxPos = m_pBoxItem->GetItemSlotIdx();

	GetDboGlobal()->GetGamePacketGenerator()->SendItemChangeOptionReq(byItemPlace, byItemPos, byBoxPlace, byBoxPos, m_wOptionIndex);

	SetState(STATE_PACKETWAIT);
}


void CItemOptionRestructureGui::OnPaint()
{
	if (m_pItem)
		m_surSlot[SLOT_ITEM].Render();

	if (m_pBoxItem)
		m_surSlot[SLOT_BOX].Render();

	for (RwInt32 i = 0; i < NUM_SLOT; ++i)
	{
		if (m_anFocusEffect[i])
			m_asurFocus[i].Render();

		m_asurDisableSlot[i].Render();
	}
}

void CItemOptionRestructureGui::OnMouseUp(const CKey & key)
{
	m_pThis->ReleaseMouse();

	if (!IsShow())
	{
		m_nLSelectedSlotIdx = -1;
		m_nRSelectedSlotIdx = -1;
		return;
	}

	if (key.m_nID == UD_LEFT_BUTTON)
	{
		if (GetIconMoveManager()->IsActive())
		{
			if (GetIconMoveManager()->GetSrcSerial() != INVALID_SERIAL_ID)
			{
				SetItemSlot(GetIconMoveManager()->GetSrcSerial(),
					GetIconMoveManager()->GetSrcPlace(),
					GetIconMoveManager()->GetSrcSlotIdx(),
					m_nLSelectedSlotIdx, TRUE);
			}
		}

		m_nLSelectedSlotIdx = -1;
	}
	else if (key.m_nID == UD_RIGHT_BUTTON)
	{
		if (!GetIconMoveManager()->IsActive())
		{
			switch (m_nRSelectedSlotIdx)
			{
				case SLOT_ITEM:
				{
					DeleteItem();
				}
				break;
				case SLOT_BOX:
				{
					DeleteBoxItem();
				}
				break;

				default: break;
			}
		}

		m_nRSelectedSlotIdx = -1;
	}
}

void CItemOptionRestructureGui::OnMove(RwInt32 nXPos, RwInt32 nYPos)
{
	CRectangle rtItemScreen = m_ppnlTargetItemSlot->GetScreenRect();

	m_surSlot[SLOT_ITEM].SetPosition(rtItemScreen.left, rtItemScreen.top);
	m_asurFocus[SLOT_ITEM].SetPosition(rtItemScreen.left, rtItemScreen.top);
	m_asurDisableSlot[SLOT_ITEM].SetPosition(rtItemScreen.left, rtItemScreen.top);

	CRectangle rtBoxItemScreen = m_ppnlOptionKitItemSlot->GetScreenRect();

	m_surSlot[SLOT_BOX].SetPosition(rtBoxItemScreen.left, rtBoxItemScreen.top);
	m_asurFocus[SLOT_BOX].SetPosition(rtBoxItemScreen.left, rtBoxItemScreen.top);
	m_asurDisableSlot[SLOT_BOX].SetPosition(rtBoxItemScreen.left, rtBoxItemScreen.top);
}

void CItemOptionRestructureGui::OnItemIconMouseEnter(gui::CComponent * pComponent)
{
	CRectangle rtScreen = m_ppnlTargetItemSlot->GetScreenRect();

	if (m_pItem)
	{
		GetInfoWndManager()->ShowInfoWindow(TRUE, CInfoWndManager::INFOWND_ITEM, rtScreen.left, rtScreen.top,
			(VOID*)m_pItem, DIALOG_ITEM_OPTIONRESTRUCTURE);
	}
	else
	{
		GetInfoWndManager()->ShowInfoWindow(TRUE, CInfoWndManager::INFOWND_JUST_WTEXT, rtScreen.left, rtScreen.top,
			(VOID*)GetDisplayStringManager()->GetString("DST_RESTRUCTURE_ITEM_EMPTY_TOOLTIP"), DIALOG_ITEM_OPTIONRESTRUCTURE);
	}

	m_anFocusEffect[SLOT_ITEM] = m_anFocusEffect[SLOT_ITEM] | SLOT_FOCUS;
}

void CItemOptionRestructureGui::OnItemIconMouseLeave(gui::CComponent * pComponent)
{
	if (GetInfoWndManager()->GetRequestGui() == DIALOG_ITEM_OPTIONRESTRUCTURE)
	{
		GetInfoWndManager()->ShowInfoWindow(FALSE);

		m_anFocusEffect[SLOT_ITEM] = m_anFocusEffect[SLOT_ITEM] & ~SLOT_FOCUS;
	}
}

void CItemOptionRestructureGui::OnItemIconMouseDown(const CKey & key)
{
	if (m_asurDisableSlot[SLOT_ITEM].IsShow())
		return;

	if (key.m_nID == UD_LEFT_BUTTON)
	{
		if (GetIconMoveManager()->IsActive())
		{
			m_nLSelectedSlotIdx = SLOT_ITEM;
		}
		else
		{
			if (IsValidIconPickup(SLOT_ITEM))
				m_nLSelectedSlotIdx = SLOT_ITEM;
		}
	}
	else if (key.m_nID == UD_RIGHT_BUTTON)
	{
		if (!GetIconMoveManager()->IsActive())
		{
			if (IsValidRightBtnProc(SLOT_ITEM))
			{
				if (m_pItem)
					m_nRSelectedSlotIdx = SLOT_ITEM;
			}
		}
	}

	m_pThis->CaptureMouse();

	if (m_nLSelectedSlotIdx >= 0 && m_nRSelectedSlotIdx >= 0)
	{
		m_nLSelectedSlotIdx = -1;
		m_nRSelectedSlotIdx = -1;
		m_pThis->ReleaseMouse();
	}
}

void CItemOptionRestructureGui::OnBoxItemIconMouseEnter(gui::CComponent * pComponent)
{
	CRectangle rtScreen = m_ppnlOptionKitItemSlot->GetScreenRect();

	if (m_pBoxItem)
	{
		GetInfoWndManager()->ShowInfoWindow(TRUE, CInfoWndManager::INFOWND_ITEM, rtScreen.left, rtScreen.top,
			(VOID*)m_pBoxItem, DIALOG_ITEM_OPTIONRESTRUCTURE);
	}
	else
	{
		GetInfoWndManager()->ShowInfoWindow(TRUE, CInfoWndManager::INFOWND_JUST_WTEXT, rtScreen.left, rtScreen.top,
			(VOID*)GetDisplayStringManager()->GetString("DST_OPTION_KIT_ITEM_EMPTY_TOOLTIP"), DIALOG_ITEM_OPTIONRESTRUCTURE);
	}

	m_anFocusEffect[SLOT_BOX] = m_anFocusEffect[SLOT_BOX] | SLOT_FOCUS;
}

void CItemOptionRestructureGui::OnBoxItemIconMouseLeave(gui::CComponent * pComponent)
{
	if (GetInfoWndManager()->GetRequestGui() == DIALOG_ITEM_OPTIONRESTRUCTURE)
	{
		GetInfoWndManager()->ShowInfoWindow(FALSE);

		m_anFocusEffect[SLOT_BOX] = m_anFocusEffect[SLOT_BOX] & ~SLOT_FOCUS;
	}
}

void CItemOptionRestructureGui::OnBoxItemIconMouseDown(const CKey & key)
{
	if (m_asurDisableSlot[SLOT_BOX].IsShow())
		return;

	if (key.m_nID == UD_LEFT_BUTTON)
	{
		if (GetIconMoveManager()->IsActive())
		{
			m_nLSelectedSlotIdx = SLOT_BOX;
		}
		else
		{
			if (IsValidIconPickup(SLOT_BOX))
				m_nLSelectedSlotIdx = SLOT_BOX;
		}
	}
	else if (key.m_nID == UD_RIGHT_BUTTON)
	{
		if (!GetIconMoveManager()->IsActive())
		{
			if (IsValidRightBtnProc(SLOT_BOX))
			{
				if (m_pBoxItem)
					m_nRSelectedSlotIdx = SLOT_BOX;
			}
		}
	}

	m_pThis->CaptureMouse();

	if (m_nLSelectedSlotIdx >= 0 && m_nRSelectedSlotIdx >= 0)
	{
		m_nLSelectedSlotIdx = -1;
		m_nRSelectedSlotIdx = -1;
		m_pThis->ReleaseMouse();
	}
}

void CItemOptionRestructureGui::OnClickedBtnClose(gui::CComponent * pComponent)
{
	if (m_eState == STATE_PACKETWAIT)
		return;

	GetDialogManager()->CloseDialog(DIALOG_ITEM_OPTIONRESTRUCTURE);
}

void CItemOptionRestructureGui::OnClickedBtnBuy(gui::CComponent * pComponent)
{
	if (GetDialogManager()->IsOpenDialog(DIALOG_HLSHOP) == FALSE)
		GetDboGlobal()->GetGamePacketGenerator()->SendCashItemHLShopStartReq();
}

void CItemOptionRestructureGui::OnClickedBtnRestructure(gui::CComponent * pComponent)
{
	GetAlarmManager()->AlarmMessage("DST_ITEM_OPTION_RESTRUCTURE_CONFIRM_MESSAGE");
}

void CItemOptionRestructureGui::OnClickedBtnCloseWide(gui::CComponent * pComponent)
{
	if (m_eState == STATE_PACKETWAIT)
		return;

	GetDialogManager()->CloseDialog(DIALOG_ITEM_OPTIONRESTRUCTURE);
}

void CItemOptionRestructureGui::OnSelectedOption(INT nSelectIndex)
{
	if (m_eState != STATE_NONE && m_eState != STATE_READY)
		return;

	CItemEnchantTable* pItemEnchantTable = API_GetTableContainer()->GetItemEnchantTable();

	TBLIDX enchantIdx = (TBLIDX)m_pcbbSelectOption->GetItemData(nSelectIndex);

	sITEM_ENCHANT_TBLDAT* pItemEnchantData = reinterpret_cast<sITEM_ENCHANT_TBLDAT*>(pItemEnchantTable->FindData(enchantIdx));
	if (pItemEnchantData)
	{
		m_wOptionIndex = (WORD)enchantIdx;

		std::wstring wstrName = API_GetTableContainer()->GetTextAllTable()->GetSystemEffectTbl()->GetText((pItemEnchantData->seTblidx * 100) + 1);

		// preview the selected option in the after column
		ClearOptionRows(m_pstbAfterOptionName, m_pstbAfterOptionValue);

		m_pstbAfterOptionName[0]->SetTextColor(INFOCOLOR_9);
		m_pstbAfterOptionName[0]->SetText(wstrName.c_str());
		m_bAfterIsPreview = true;

		if (m_wOptionIndex != INVALID_WORD)
			CheckReady();
	}
}

