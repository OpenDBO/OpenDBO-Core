#include "precomp_dboclient.h"
#include "MascotFusion.h"

// Core
#include "NtlDebug.h"

// Shared
#include "ItemTable.h"

// Presetation
#include "NtlPLGuiManager.h"

// Simulation
#include "NtlSLGlobal.h"
#include "NtlSLApi.h"
#include "NtlSobManager.h"
#include "NtlSobItem.h"
#include "NtlSobItemAttr.h"

// Dbo
#include "DboEvent.h"
#include "DboGlobal.h"
#include "DboLogic.h"
#include "DisplayStringManager.h"
#include "DialogManager.h"
#include "IconMoveManager.h"
#include "AlarmManager.h"

namespace
{
	// both arrows fill towards the middle together, then the evolved mascot appears
	const RwReal FUSION_FILL_TIME		= 2.0f;
	const RwReal FUSION_TIMEOUT			= 10.0f;
	const RwInt32 FUSION_GAUGE_MAX		= 100;
}

CMascotFusion::CMascotFusion(const RwChar* pName)
	:CNtlPLGui(pName)
{
	Init();
}

CMascotFusion::~CMascotFusion()
{
}

VOID CMascotFusion::Init()
{
	m_Stb_Title = NULL;
	m_HtmlBox_Explan = NULL;
	m_Btn_MascotFusion = NULL;
	m_Btn_Close = NULL;
	m_Pgb_LGague = NULL;
	m_Pgb_RGague = NULL;
	m_pCircle = NULL;

	for (RwInt32 i = 0; i < FUSION_SLOT_COUNT; ++i)
		m_pPanel[i] = NULL;

	m_byMainIndex = INVALID_BYTE;
	m_byOfferingIndex = INVALID_BYTE;
	m_hItem = INVALID_SERIAL_ID;
	m_iFocusSlot = -1;
	m_bFused = FALSE;

	m_bFilling = FALSE;
	m_fFillTime = 0.0f;
	m_bRequestPending = FALSE;
	m_bResultArrived = FALSE;
	m_bResultSuccess = FALSE;
	memset(&m_sResult, 0, sizeof(m_sResult));
}

RwBool CMascotFusion::Create()
{
	NTL_FUNCTION("CMascotFusion::Create");

	if (!CNtlPLGui::Create("gui\\MascotEx.rsr", "gui\\MascotEx.srf", "gui\\MascotFusion.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");

	m_Stb_Title = (gui::CStaticBox*)GetComponent("Stb_Title");
	m_Stb_Title->SetText(GetDisplayStringManager()->GetString("DST_MASCOTEX_FUTIONTITEL"));

	m_HtmlBox_Explan = (gui::CHtmlBox*)GetComponent("HtmlBox_Explan");
	const WCHAR* wchHtmlMsg = GetDisplayStringManager()->GetString("DST_MASCOTEX_FUSION_EXPLANATION");
	m_HtmlBox_Explan->SetHtmlFromMemory(wchHtmlMsg, wcslen(wchHtmlMsg));

	m_Btn_MascotFusion = (gui::CButton*)GetComponent("Btn_MascotFusion");
	m_Btn_MascotFusion->SetText(GetDisplayStringManager()->GetString("DST_MASCOTEX_FUSIONBUTTON"));
	m_Btn_Close = (gui::CButton*)GetComponent("Btn_Close");

	// the green arrows and circle only light up during and after a fusion
	m_Pgb_LGague = (gui::CProgressBar*)GetComponent("Pgb_LGague");
	m_Pgb_RGague = (gui::CProgressBar*)GetComponent("Pgb_RGague");
	m_Pgb_LGague->SetRange(0, FUSION_GAUGE_MAX);
	m_Pgb_RGague->SetRange(0, FUSION_GAUGE_MAX);
	m_pCircle = (gui::CPanel*)GetComponent("Pn_Circle");
	SetFused(FALSE);

	for (RwInt32 i = 0; i < FUSION_SLOT_COUNT; ++i)
	{
		char acName[32];
		sprintf_s(acName, "Pn_IvnIcon%d", i);
		m_pPanel[i] = (gui::CPanel*)GetComponent(acName);

		m_slotEnter[i] = m_pPanel[i]->SigMouseEnter().Connect(this, &CMascotFusion::OnMouseEnter);
		m_slotLeave[i] = m_pPanel[i]->SigMouseLeave().Connect(this, &CMascotFusion::OnMouseLeave);
		m_slotUp[i] = m_pPanel[i]->SigMouseUp().Connect(this, &CMascotFusion::OnMouseUp);

		CRectangle rect = m_pPanel[i]->GetPosition();
		m_Slot[i].Create(m_pThis, DIALOG_MASCOT_FUSION, i == FUSION_SLOT_ITEM ? REGULAR_SLOT_ITEM_SOB : REGULAR_SLOT_ITEM_TABLE);
		m_Slot[i].SetPosition_fromParent(rect.left, rect.top);
	}

	m_pPanel[FUSION_SLOT_OFFERING]->SetToolTip(GetDisplayStringManager()->GetString("DST_MASCOTEX_FUSION_SUBMASCOT_REGISTER"));
	m_pPanel[FUSION_SLOT_MAIN]->SetToolTip(GetDisplayStringManager()->GetString("DST_MASCOTEX_FUSION_TOOLTIP"));
	m_pPanel[FUSION_SLOT_ITEM]->SetToolTip(GetDisplayStringManager()->GetString("DST_MASCOTEX_FUSION_DEFENCEITEM_LEFTCLICK"));

	m_surFocus.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("GameCommon.srf", "srfSlotFocusEffect"));
	m_surFocus.Show(FALSE);

	m_slotCloseButton = m_Btn_Close->SigClicked().Connect(this, &CMascotFusion::ClickedCloseButton);
	m_slotFusionButton = m_Btn_MascotFusion->SigClicked().Connect(this, &CMascotFusion::ClickedFusionButton);
	m_slotPaint = m_pThis->SigPaint().Connect(this, &CMascotFusion::OnPaint);
	m_slotMove = m_pThis->SigMove().Connect(this, &CMascotFusion::OnMove);

	LinkMsg(g_EventMascotUpdate);
	LinkMsg(g_EventMascotDelete);
	LinkMsg(g_EventMascotSummon);

	GetNtlGuiManager()->AddUpdateFunc(this);

	Show(false);

	NTL_RETURN(TRUE);
}

VOID CMascotFusion::Destroy()
{
	UnLinkMsg(g_EventMascotUpdate);
	UnLinkMsg(g_EventMascotDelete);
	UnLinkMsg(g_EventMascotSummon);

	GetNtlGuiManager()->RemoveUpdateFunc(this);

	for (RwInt32 i = 0; i < FUSION_SLOT_COUNT; ++i)
		m_Slot[i].Destroy();

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();
}

RwInt32 CMascotFusion::SwitchDialog(bool bOpen)
{
	if (bOpen)
	{
		CRectangle rtScreen = m_pThis->GetScreenRect();
		OnMove(rtScreen.left, rtScreen.top);
		Refresh();
	}
	else
	{
		// closed mid animation, apply the outcome now (or once the server answers)
		if (m_bFilling && m_bResultArrived)
		{
			SMascotFusionResult sResult = m_sResult;
			StopFill();
			Logic_ApplyMascotFusion(sResult);
			ShowResultMessages(sResult.bSuccess);
		}

		StopFill();
		SetFused(FALSE);
		m_byMainIndex = INVALID_BYTE;
		m_byOfferingIndex = INVALID_BYTE;
		ClearItem();
	}

	Show(bOpen);
	return 1;
}

VOID CMascotFusion::SetMainMascot(BYTE byIndex)
{
	if (m_bFilling)
		return;

	m_byMainIndex = byIndex;

	if (m_byOfferingIndex == byIndex)
		m_byOfferingIndex = INVALID_BYTE;

	SetFused(FALSE);
	Refresh();
}

VOID CMascotFusion::SetOfferingMascot(BYTE byIndex)
{
	if (m_bFilling)
		return;

	if (byIndex == m_byMainIndex)
	{
		GetAlarmManager()->AlarmMessage("DST_MASCOTEX_FUSION_SUBMASCOT_MAINMASCOT");
		return;
	}

	if (GetNtlSLGlobal()->GetAvatarMascotInfo()->bySummonIndex == byIndex)
	{
		GetAlarmManager()->AlarmMessage("DST_MASCOTEX_FUSION_SUBMASCOT_SUMMONING");
		return;
	}

	m_byOfferingIndex = byIndex;
	SetFused(FALSE);
	Refresh();
}

VOID CMascotFusion::HandleEvents(RWS::CMsg& msg)
{
	if (msg.Id == g_EventMascotUpdate)
	{
		SDboEventMascotUpdate* pData = reinterpret_cast<SDboEventMascotUpdate*>(msg.pData);
		if (pData->byType != MASCOT_UPDATE_FUSION || !m_bRequestPending)
			return;

		m_bRequestPending = FALSE;

		// refused by the server, nothing was consumed
		if (pData->byIndex == INVALID_BYTE)
		{
			StopFill();
			Refresh();
			return;
		}

		// the window was closed during the animation and the result went straight to the list
		ShowResultMessages(pData->bSuccess);
	}
	else if (msg.Id == g_EventMascotDelete)
	{
		SDboEventMascotOperate* pData = reinterpret_cast<SDboEventMascotOperate*>(msg.pData);

		if (pData->index == m_byMainIndex)
			GetDialogManager()->CloseDialog(DIALOG_MASCOT_FUSION);
		else if (pData->index == m_byOfferingIndex && !m_bFilling)
		{
			// a fusion in progress keeps showing its sacrifice until the result appears
			m_byOfferingIndex = INVALID_BYTE;
			Refresh();
		}
	}
	else if (msg.Id == g_EventMascotSummon)
	{
		// fusions need every mascot dismissed
		if (IsShow())
			GetDialogManager()->CloseDialog(DIALOG_MASCOT_FUSION);
	}
}

VOID CMascotFusion::SetFusionResult(const SMascotFusionResult& sResult)
{
	m_sResult = sResult;
	m_bResultArrived = TRUE;
	m_bResultSuccess = sResult.bSuccess;
	m_bRequestPending = FALSE;

	if (m_fFillTime >= FUSION_FILL_TIME)
		RevealResult();
}

VOID CMascotFusion::Update(RwReal fElapsed)
{
	if (!m_bFilling)
		return;

	m_fFillTime += fElapsed;

	RwInt32 iFill = (RwInt32)(min(1.0f, m_fFillTime / FUSION_FILL_TIME) * (RwReal)FUSION_GAUGE_MAX);
	m_Pgb_LGague->SetPos(iFill);
	m_Pgb_RGague->SetPos(iFill);

	if (m_fFillTime < FUSION_FILL_TIME)
		return;

	if (m_bResultArrived)
		RevealResult();
	else if (m_fFillTime > FUSION_TIMEOUT)
	{
		// no answer from the server
		m_bRequestPending = FALSE;
		StopFill();
		Refresh();
	}
}

VOID CMascotFusion::Refresh(VOID)
{
	SMascotInfo* pInfo = GetNtlSLGlobal()->GetAvatarMascotInfo();

	// after a successful fusion the middle mascot already carries its new rank
	sMASCOT_DATA_EX* pMain = pInfo->Find(m_byMainIndex);
	const RwChar* pcMainIcon = pMain ? Logic_GetMascotIconName(pMain->tblidx) : NULL;
	if (pcMainIcon)
		m_Slot[FUSION_SLOT_MAIN].SetIcon(pcMainIcon);
	else
		m_Slot[FUSION_SLOT_MAIN].Clear();

	sMASCOT_DATA_EX* pOffering = pInfo->Find(m_byOfferingIndex);
	const RwChar* pcOfferingIcon = pOffering ? Logic_GetMascotIconName(pOffering->tblidx) : NULL;
	if (pOffering == NULL)
		m_byOfferingIndex = INVALID_BYTE;

	if (pcOfferingIcon)
		m_Slot[FUSION_SLOT_OFFERING].SetIcon(pcOfferingIcon);
	else
		m_Slot[FUSION_SLOT_OFFERING].Clear();

	if (m_hItem != INVALID_SERIAL_ID && GetNtlSobManager()->GetSobObject(m_hItem))
		m_Slot[FUSION_SLOT_ITEM].SetIcon(m_hItem);
	else
		ClearItem();

	if (!m_bFilling)
	{
		RwInt32 iFill = m_bFused ? FUSION_GAUGE_MAX : 0;
		m_Pgb_LGague->SetPos(iFill);
		m_Pgb_RGague->SetPos(iFill);
	}

	m_Btn_MascotFusion->ClickEnable(!m_bFilling && pMain != NULL && pOffering != NULL);
}

VOID CMascotFusion::SetFused(RwBool bFused)
{
	m_bFused = bFused;

	RwInt32 iFill = bFused ? FUSION_GAUGE_MAX : 0;
	m_Pgb_LGague->SetPos(iFill);
	m_Pgb_RGague->SetPos(iFill);
	m_pCircle->Show(bFused ? true : false);
}

VOID CMascotFusion::StartFill(VOID)
{
	SetFused(FALSE);

	m_bFilling = TRUE;
	m_fFillTime = 0.0f;
	m_bResultArrived = FALSE;
	m_bResultSuccess = FALSE;

	m_Btn_MascotFusion->ClickEnable(false);
}

VOID CMascotFusion::StopFill(VOID)
{
	m_bFilling = FALSE;
	m_fFillTime = 0.0f;
	m_bResultArrived = FALSE;

	SetFused(m_bFused);
}

VOID CMascotFusion::RevealResult(VOID)
{
	RwBool bSuccess = m_bResultSuccess;

	m_bFilling = FALSE;
	m_fFillTime = 0.0f;
	m_bResultArrived = FALSE;

	// only now the mascot list loses the sacrifice and shows the evolved mascot
	Logic_ApplyMascotFusion(m_sResult);
	ShowResultMessages(bSuccess);

	// the sacrifice and the item are used up whatever the outcome
	m_byOfferingIndex = INVALID_BYTE;
	ClearItem();

	// a success keeps the arrows and the circle lit around the evolved mascot
	SetFused(bSuccess);
	Refresh();
}

VOID CMascotFusion::ShowResultMessages(RwBool bSuccess)
{
	GetAlarmManager()->AlarmMessage("DST_MASCOTEX_FUSION_SUBMASCOT_DELETED");

	if (!bSuccess)
		GetAlarmManager()->AlarmMessage("MASCOT_FUSION_FAIL", TRUE);
}

VOID CMascotFusion::ClearItem(VOID)
{
	if (m_hItem != INVALID_SERIAL_ID)
		Logic_EnableBagItemIcon(m_hItem, TRUE);

	m_hItem = INVALID_SERIAL_ID;
	m_Slot[FUSION_SLOT_ITEM].Clear();
}

VOID CMascotFusion::DropItem(VOID)
{
	if (GetIconMoveManager()->GetSrcPlace() != PLACE_BAG)
	{
		GetIconMoveManager()->IconMoveEnd();
		return;
	}

	SERIAL_HANDLE hItem = GetIconMoveManager()->GetSrcSerial();
	GetIconMoveManager()->IconMoveEnd();

	SetItem(hItem);
}

VOID CMascotFusion::SetItem(SERIAL_HANDLE hItem)
{
	if (m_bFilling || hItem == m_hItem)
		return;

	CNtlSobItem* pItem = reinterpret_cast<CNtlSobItem*>(GetNtlSobManager()->GetSobObject(hItem));
	if (pItem == NULL)
		return;

	sITEM_TBLDAT* pItemTbl = reinterpret_cast<CNtlSobItemAttr*>(pItem->GetSobAttr())->GetItemTbl();
	if (pItemTbl == NULL || pItemTbl->byItem_Type != ITEM_TYPE_EVENT_MASCOT_LEVELUP)
	{
		GetAlarmManager()->AlarmMessage("DST_MASCOTEX_FUSION_DEFENCEITEM_NO");
		return;
	}

	if (m_hItem != INVALID_SERIAL_ID)
	{
		GetAlarmManager()->AlarmMessage("DST_MASCOTEX_FUSION_DEFENCEITEM_AGAINE");
		return;
	}

	m_hItem = hItem;
	Logic_EnableBagItemIcon(m_hItem, FALSE);
	SetFused(FALSE);
	Refresh();
}

VOID CMascotFusion::ClickedCloseButton(gui::CComponent* pComponent)
{
	GetDialogManager()->CloseDialog(DIALOG_MASCOT_FUSION);
}

VOID CMascotFusion::ClickedFusionButton(gui::CComponent* pComponent)
{
	if (m_bFilling || m_bRequestPending)
		return;

	SMascotInfo* pInfo = GetNtlSLGlobal()->GetAvatarMascotInfo();

	// rank and EXP of the middle mascot, the server refuses it otherwise
	if (!Logic_CheckMascotFusion(pInfo->Find(m_byMainIndex)))
		return;

	if (pInfo->Find(m_byOfferingIndex) == NULL)
	{
		GetAlarmManager()->AlarmMessage("DST_MASCOTEX_FUSION_SUBMASCOT_REGISTER");
		return;
	}

	if (pInfo->bySummonIndex != INVALID_BYTE)
	{
		GetAlarmManager()->AlarmMessage("DST_MASCOTEX_FUSION_MAINMASCOT_SUMMONING");
		return;
	}

	// without an item the place stays invalid and the server rolls the success chance
	RwUInt8 byPlace = INVALID_BYTE;
	RwUInt8 byPos = INVALID_BYTE;
	Logic_GetItemContainerPosFromHandle(m_hItem, byPlace, byPos);

	if (GetDboGlobal()->GetGamePacketGenerator()->SendMascotFusionReq(byPlace, byPos, m_byMainIndex, m_byOfferingIndex))
	{
		m_bRequestPending = TRUE;
		StartFill();
	}
}

VOID CMascotFusion::OnMouseEnter(gui::CComponent* pComponent)
{
	m_iFocusSlot = -1;
	for (RwInt32 i = 0; i < FUSION_SLOT_COUNT; ++i)
	{
		if (m_pPanel[i] == pComponent)
			m_iFocusSlot = i;
	}

	if (m_iFocusSlot < 0)
		return;

	CRectangle rtScreen = m_pThis->GetScreenRect();
	m_surFocus.SetRectWH(rtScreen.left + m_Slot[m_iFocusSlot].GetX_fromParent(), rtScreen.top + m_Slot[m_iFocusSlot].GetY_fromParent(), 32, 32);
	m_surFocus.Show(TRUE);
}

VOID CMascotFusion::OnMouseLeave(gui::CComponent* pComponent)
{
	m_iFocusSlot = -1;
	m_surFocus.Show(FALSE);
}

VOID CMascotFusion::OnMouseUp(const CKey& key)
{
	if (m_bFilling)
	{
		if (GetIconMoveManager()->IsActive())
			GetIconMoveManager()->IconMoveEnd();
		return;
	}

	switch (m_iFocusSlot)
	{
	case FUSION_SLOT_ITEM:
		{
			if (GetIconMoveManager()->IsActive())
				DropItem();
			else if (key.m_nID == UD_RIGHT_BUTTON)
			{
				ClearItem();
				SetFused(FALSE);
				Refresh();
			}
			break;
		}
	case FUSION_SLOT_OFFERING:
		{
			if (key.m_nID == UD_RIGHT_BUTTON)
			{
				m_byOfferingIndex = INVALID_BYTE;
				SetFused(FALSE);
				Refresh();
			}
			else if (m_byOfferingIndex == INVALID_BYTE)
				GetAlarmManager()->AlarmMessage("DST_MASCOTEX_FUSION_SUBMASCOT_REGISTER");
			break;
		}
	default:
		break;
	}
}

VOID CMascotFusion::OnPaint()
{
	for (RwInt32 i = 0; i < FUSION_SLOT_COUNT; ++i)
		m_Slot[i].Paint();

	m_surFocus.Render();
}

VOID CMascotFusion::OnMove(RwInt32 iOldX, RwInt32 iOldY)
{
	CRectangle rtScreen = m_pThis->GetScreenRect();

	for (RwInt32 i = 0; i < FUSION_SLOT_COUNT; ++i)
		m_Slot[i].SetParentPosition(rtScreen.left, rtScreen.top);
}
