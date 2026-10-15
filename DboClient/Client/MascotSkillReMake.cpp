#include "precomp_dboclient.h"
#include "MascotSkillReMake.h"

// Core
#include "NtlDebug.h"

// Shared
#include "ItemTable.h"
#include "SkillTable.h"
#include "TextAllTable.h"
#include "TableContainer.h"

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
#include "DboEventGenerator.h"
#include "DboGlobal.h"
#include "DboLogic.h"
#include "DisplayStringManager.h"
#include "DialogManager.h"
#include "IconMoveManager.h"
#include "AlarmManager.h"

namespace
{
	const WCHAR* GetSkillName(TBLIDX skillTblidx)
	{
		sSKILL_TBLDAT* pSkillTbl = (sSKILL_TBLDAT*)API_GetTableContainer()->GetSkillTable()->FindData(skillTblidx);
		if (pSkillTbl == NULL)
			return GetDisplayStringManager()->GetString("DST_NOTTING");

		static std::wstring s_wstrName;
		s_wstrName = API_GetTableContainer()->GetTextAllTable()->GetSkillTbl()->GetText(pSkillTbl->Skill_Name);
		return s_wstrName.c_str();
	}
}

CMascotSkillReMake::CMascotSkillReMake(const RwChar* pName)
	:CNtlPLGui(pName)
{
	Init();
}

CMascotSkillReMake::~CMascotSkillReMake()
{
}

VOID CMascotSkillReMake::Init()
{
	for (RwInt32 i = 0; i < REMAKE_SLOT_COUNT; ++i)
		m_pPanel[i] = NULL;

	m_byMascotIndex = INVALID_BYTE;
	m_bySkillSlot = INVALID_BYTE;
	m_oldSkillTblidx = INVALID_TBLIDX;
	m_newSkillTblidx = INVALID_TBLIDX;
	m_hItem = INVALID_SERIAL_ID;
	m_iFocusSlot = -1;

	m_bFilling = FALSE;
	m_fFillTime = 0.0f;
	m_bResultArrived = FALSE;
}

RwBool CMascotSkillReMake::Create()
{
	NTL_FUNCTION("CMascotSkillReMake::Create");

	if (!CNtlPLGui::Create("gui\\MascotEx.rsr", "gui\\MascotEx.srf", "gui\\MascotSkillReMake.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");

	m_Stb_Title = (gui::CStaticBox*)GetComponent("Stb_Title");
	m_Stb_OldSkillName = (gui::CStaticBox*)GetComponent("Stb_OldSkillName");
	m_Stb_NewSkillName = (gui::CStaticBox*)GetComponent("Stb_NewSkillName");
	m_Stb_TypeOld = (gui::CStaticBox*)GetComponent("Stb_TypeOld");
	m_Stb_TypeItem = (gui::CStaticBox*)GetComponent("Stb_TypeItem");
	m_Stb_TypeNew = (gui::CStaticBox*)GetComponent("Stb_TypeNew");
	m_Stb_TypeBefore = (gui::CStaticBox*)GetComponent("Stb_TypeBefore");
	m_Stb_TypeAfter = (gui::CStaticBox*)GetComponent("Stb_TypeAfter");
	m_Btn_Close = (gui::CButton*)GetComponent("Btn_Close");
	m_Btn_MascotSkillRemake = (gui::CButton*)GetComponent("Btn_MascotSkillRemake");
	m_Pgb_LeftGague = (gui::CProgressBar*)GetComponent("Pgb_LeftGague");
	m_Pgb_RightGague = (gui::CProgressBar*)GetComponent("Pgb_RightGague");
	m_HtmlBox_Explan = (gui::CHtmlBox*)GetComponent("HtmlBox_Explan");

	m_Pgb_LeftGague->SetRange(0, 100);
	m_Pgb_RightGague->SetRange(0, 100);

	m_Stb_TypeOld->SetText(GetDisplayStringManager()->GetString("DST_MASCOTEX_ROLL_HAVESKILL"));
	m_Stb_TypeItem->SetText(GetDisplayStringManager()->GetString("DST_MASCOTEX_ROLL_ROLLITEM"));
	m_Stb_TypeNew->SetText(GetDisplayStringManager()->GetString("DST_MASCOTEX_ROLL_ROLLSKILL"));
	m_Stb_TypeBefore->SetText(GetDisplayStringManager()->GetString("DST_MASCOTEX_ROLL_HAVESKILL_TITLE"));
	m_Stb_TypeAfter->SetText(GetDisplayStringManager()->GetString("DST_MASCOTEX_ROLL_ROLLSKILL_TITLE"));

	static const char* s_apcPanel[REMAKE_SLOT_COUNT] = { "Pn_OldSkill", "Pn_Item", "Pn_NewSkill" };
	static const eREGULAR_SLOT_TYPE s_aeSlotType[REMAKE_SLOT_COUNT] = { REGULAR_SLOT_SKILL_TABLE, REGULAR_SLOT_ITEM_SOB, REGULAR_SLOT_SKILL_TABLE };

	for (RwInt32 i = 0; i < REMAKE_SLOT_COUNT; ++i)
	{
		m_pPanel[i] = (gui::CPanel*)GetComponent(s_apcPanel[i]);

		m_slotEnter[i] = m_pPanel[i]->SigMouseEnter().Connect(this, &CMascotSkillReMake::OnMouseEnter);
		m_slotLeave[i] = m_pPanel[i]->SigMouseLeave().Connect(this, &CMascotSkillReMake::OnMouseLeave);
		m_slotUp[i] = m_pPanel[i]->SigMouseUp().Connect(this, &CMascotSkillReMake::OnMouseUp);

		const CRectangle& rect = m_pPanel[i]->GetPosition();
		m_Slot[i].Create(m_pThis, DIALOG_MASCOT_SKILL_REMAKE, s_aeSlotType[i]);
		m_Slot[i].SetPosition_fromParent(rect.left, rect.top);
	}

	m_pPanel[REMAKE_SLOT_ITEM]->SetToolTip(GetDisplayStringManager()->GetString("DST_MASCOTEX_SKILLROLL_LEFTCLICK"));

	m_surFocus.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("GameCommon.srf", "srfSlotFocusEffect"));
	m_surFocus.Show(FALSE);

	m_slotCloseButton = m_Btn_Close->SigClicked().Connect(this, &CMascotSkillReMake::ClickedCloseButton);
	m_slotRemakeButton = m_Btn_MascotSkillRemake->SigClicked().Connect(this, &CMascotSkillReMake::ClickedRemakeButton);
	m_slotPaint = m_pThis->SigPaint().Connect(this, &CMascotSkillReMake::OnPaint);
	m_slotMove = m_pThis->SigMove().Connect(this, &CMascotSkillReMake::OnMove);

	LinkMsg(g_EventMascotUpdate);
	LinkMsg(g_EventMascotUnSummon);

	GetNtlGuiManager()->AddUpdateFunc(this);

	Show(false);

	NTL_RETURN(TRUE);
}

VOID CMascotSkillReMake::Destroy()
{
	UnLinkMsg(g_EventMascotUpdate);
	UnLinkMsg(g_EventMascotUnSummon);

	GetNtlGuiManager()->RemoveUpdateFunc(this);

	for (RwInt32 i = 0; i < REMAKE_SLOT_COUNT; ++i)
		m_Slot[i].Destroy();

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();
}

RwInt32 CMascotSkillReMake::SwitchDialog(bool bOpen)
{
	if (bOpen)
	{
		CRectangle rtScreen = m_pThis->GetScreenRect();
		OnMove(rtScreen.left, rtScreen.top);
		Refresh();
	}
	else
	{
		// a pending result is shown by the mascot window instead
		if (m_bFilling)
			CDboEventGenerator::MascotUpdate(m_byMascotIndex, MASCOT_UPDATE_SKILL_SHOWN, m_bySkillSlot);

		StopFill();
		ClearItem();
		m_byMascotIndex = INVALID_BYTE;
		m_bySkillSlot = INVALID_BYTE;
		m_oldSkillTblidx = INVALID_TBLIDX;
		m_newSkillTblidx = INVALID_TBLIDX;
	}

	Show(bOpen);
	return 1;
}

VOID CMascotSkillReMake::OpenSlot(BYTE byMascotIndex, BYTE bySkillSlot)
{
	sMASCOT_DATA_EX* pData = GetNtlSLGlobal()->GetAvatarMascotInfo()->Find(byMascotIndex);
	if (pData == NULL || bySkillSlot >= DBO_MASCOT_MAX_SKILL_COUNT)
		return;

	m_byMascotIndex = byMascotIndex;
	m_bySkillSlot = bySkillSlot;
	m_oldSkillTblidx = pData->skillTblidx[bySkillSlot];
	m_newSkillTblidx = INVALID_TBLIDX;

	if (GetDialogManager()->IsOpenDialog(DIALOG_MASCOT_SKILL_REMAKE))
		Refresh();
	else
		GetDialogManager()->OpenDialog(DIALOG_MASCOT_SKILL_REMAKE);
}

VOID CMascotSkillReMake::OpenWithItem(SERIAL_HANDLE hItem)
{
	// already open on a skill, the item just goes into its slot
	if (GetDialogManager()->IsOpenDialog(DIALOG_MASCOT_SKILL_REMAKE))
	{
		if (m_hItem != hItem)
			SetItem(hItem);
		return;
	}

	SMascotInfo* pInfo = GetNtlSLGlobal()->GetAvatarMascotInfo();
	sMASCOT_DATA_EX* pData = pInfo->GetSummoned();
	if (pData == NULL)
	{
		GetAlarmManager()->AlarmMessage("DST_MASCOTEX_SKILLGRADEUP_DONT_SUMMON");
		return;
	}

	// start on the first learned skill, another one is picked with its + button
	BYTE bySlot = INVALID_BYTE;
	for (BYTE i = 0; i < DBO_MASCOT_MAX_SKILL_COUNT; ++i)
	{
		if (pData->skillTblidx[i] != INVALID_TBLIDX)
		{
			bySlot = i;
			break;
		}
	}

	if (bySlot == INVALID_BYTE)
	{
		GetAlarmManager()->AlarmMessage("DST_MASCOTEX_SKILLROLL_NOT_SKILL");
		return;
	}

	if (!GetDialogManager()->IsOpenDialog(DIALOG_MASCOT))
		GetDialogManager()->OpenDialog(DIALOG_MASCOT);

	CRectangle rect = GetDialogManager()->GetDialog(DIALOG_MASCOT)->GetPosition();
	SetPosition(rect.left + rect.GetWidth() + NTL_LINKED_DIALOG_GAP, rect.top);

	OpenSlot(pData->byIndex, bySlot);
	SetItem(hItem);
}

RwBool CMascotSkillReMake::IsCreateMode(VOID)
{
	return m_oldSkillTblidx == INVALID_TBLIDX;
}

VOID CMascotSkillReMake::HandleEvents(RWS::CMsg& msg)
{
	if (msg.Id == g_EventMascotUpdate)
	{
		SDboEventMascotUpdate* pData = reinterpret_cast<SDboEventMascotUpdate*>(msg.pData);
		if (pData->byType != MASCOT_UPDATE_SKILL || pData->byIndex != m_byMascotIndex || pData->bySlot != m_bySkillSlot)
			return;

		if (!pData->bSuccess)
		{
			StopFill();
			Refresh();
			return;
		}

		// shown once the gauges are full
		m_bResultArrived = TRUE;
		if (!m_bFilling)
			RevealResult();
	}
	else if (msg.Id == g_EventMascotUnSummon)
	{
		GetDialogManager()->CloseDialog(DIALOG_MASCOT_SKILL_REMAKE);
	}
}

VOID CMascotSkillReMake::Refresh(VOID)
{
	RwBool bCreate = IsCreateMode() && m_newSkillTblidx == INVALID_TBLIDX;

	if (bCreate)
	{
		m_Stb_Title->SetText(GetDisplayStringManager()->GetString("DST_MASCOTEX_SKILL_CREATE_TITLE"));
		m_Btn_MascotSkillRemake->SetText(GetDisplayStringManager()->GetString("DST_MASCOTEX_SKILL_CREATE_BUTTON"));

		const WCHAR* wchHtmlMsg = GetDisplayStringManager()->GetString("DST_MASCOTEX_SKILL_CREATE_EXPALANATION");
		m_HtmlBox_Explan->SetHtmlFromMemory(wchHtmlMsg, wcslen(wchHtmlMsg));
	}
	else
	{
		m_Stb_Title->SetText(GetDisplayStringManager()->GetString("DST_MASCOTEX_ROLL_TITLE"));
		m_Btn_MascotSkillRemake->SetText(GetDisplayStringManager()->GetString("DST_MASCOTEX_SKILLROLL_BUTTON"));

		const WCHAR* wchHtmlMsg = GetDisplayStringManager()->GetString("DST_MASCOTEX_ROLL_EXPALANATION");
		m_HtmlBox_Explan->SetHtmlFromMemory(wchHtmlMsg, wcslen(wchHtmlMsg));
	}

	// a freshly learned skill can be re-rolled right away, show it as the current one
	TBLIDX oldSkill = m_oldSkillTblidx;
	if (oldSkill == INVALID_TBLIDX)
		oldSkill = m_newSkillTblidx;

	if (oldSkill != INVALID_TBLIDX)
		m_Slot[REMAKE_SLOT_OLD].SetIcon(oldSkill);
	else
		m_Slot[REMAKE_SLOT_OLD].Clear();

	if (m_newSkillTblidx != INVALID_TBLIDX)
		m_Slot[REMAKE_SLOT_NEW].SetIcon(m_newSkillTblidx);
	else
		m_Slot[REMAKE_SLOT_NEW].Clear();

	if (m_hItem != INVALID_SERIAL_ID)
		m_Slot[REMAKE_SLOT_ITEM].SetIcon(m_hItem);
	else
		m_Slot[REMAKE_SLOT_ITEM].Clear();

	m_Stb_OldSkillName->SetText(GetSkillName(oldSkill));
	m_Stb_NewSkillName->SetText(GetSkillName(m_newSkillTblidx));

	// the gauges only fill while a request is answered
	if (!m_bFilling)
	{
		m_Pgb_LeftGague->SetPos(0);
		m_Pgb_RightGague->SetPos(0);
	}

	m_Btn_MascotSkillRemake->ClickEnable(!m_bFilling);
}

RwBool CMascotSkillReMake::IsRevealPending(BYTE byMascotIndex, BYTE bySkillSlot) const
{
	return m_bFilling && m_byMascotIndex == byMascotIndex && m_bySkillSlot == bySkillSlot;
}

VOID CMascotSkillReMake::StartFill(VOID)
{
	m_bFilling = TRUE;
	m_fFillTime = 0.0f;
	m_bResultArrived = FALSE;

	m_Pgb_LeftGague->SetPos(0);
	m_Pgb_RightGague->SetPos(0);
	m_Btn_MascotSkillRemake->ClickEnable(false);
}

VOID CMascotSkillReMake::StopFill(VOID)
{
	m_bFilling = FALSE;
	m_fFillTime = 0.0f;
	m_bResultArrived = FALSE;

	m_Pgb_LeftGague->SetPos(0);
	m_Pgb_RightGague->SetPos(0);
	m_Btn_MascotSkillRemake->ClickEnable(true);
}

VOID CMascotSkillReMake::RevealResult(VOID)
{
	sMASCOT_DATA_EX* pMascot = GetNtlSLGlobal()->GetAvatarMascotInfo()->Find(m_byMascotIndex);

	StopFill();

	if (pMascot == NULL)
		return;

	m_newSkillTblidx = pMascot->skillTblidx[m_bySkillSlot];

	// a re-roll consumed the item, the server updates its stack on its own
	if (m_hItem != INVALID_SERIAL_ID && GetNtlSobManager()->GetSobObject(m_hItem) == NULL)
		m_hItem = INVALID_SERIAL_ID;

	Refresh();

	// the mascot window held its skill icon back until now
	CDboEventGenerator::MascotUpdate(m_byMascotIndex, MASCOT_UPDATE_SKILL_SHOWN, m_bySkillSlot);
}

VOID CMascotSkillReMake::Update(RwReal fElapsed)
{
	if (!m_bFilling)
		return;

	// both arrows fill together, then the skill appears
	const RwReal fFillLength = 1.6f;
	const RwReal fTimeout = 10.0f;

	m_fFillTime += fElapsed;

	RwInt32 iFill = (RwInt32)(min(1.0f, m_fFillTime / fFillLength) * 100.0f);
	m_Pgb_LeftGague->SetPos(iFill);
	m_Pgb_RightGague->SetPos(iFill);

	if (m_fFillTime < fFillLength)
		return;

	if (m_bResultArrived)
		RevealResult();
	else if (m_fFillTime > fTimeout)
	{
		// no answer from the server
		StopFill();
		Refresh();
	}
}

VOID CMascotSkillReMake::SetItem(SERIAL_HANDLE hItem)
{
	CNtlSobItem* pItem = reinterpret_cast<CNtlSobItem*>(GetNtlSobManager()->GetSobObject(hItem));
	if (pItem == NULL)
		return;

	sITEM_TBLDAT* pItemTbl = reinterpret_cast<CNtlSobItemAttr*>(pItem->GetSobAttr())->GetItemTbl();
	if (pItemTbl == NULL || pItemTbl->byItem_Type != ITEM_TYPE_EVENT_MASCOT_CHANGE_SKILL)
	{
		GetAlarmManager()->AlarmMessage("DST_MASCOTEX_SKILLROLL_NOT_ITEM");
		return;
	}

	if (m_hItem != INVALID_SERIAL_ID && m_hItem != hItem)
	{
		GetAlarmManager()->AlarmMessage("DST_MASCOTEX_SKILLROLL_ITEM_AGAINE");
		return;
	}

	m_hItem = hItem;
	Logic_EnableBagItemIcon(m_hItem, FALSE);
	Refresh();
}

VOID CMascotSkillReMake::ClearItem(VOID)
{
	if (m_hItem != INVALID_SERIAL_ID)
		Logic_EnableBagItemIcon(m_hItem, TRUE);

	m_hItem = INVALID_SERIAL_ID;
	m_Slot[REMAKE_SLOT_ITEM].Clear();
}

VOID CMascotSkillReMake::ClickedCloseButton(gui::CComponent* pComponent)
{
	GetDialogManager()->CloseDialog(DIALOG_MASCOT_SKILL_REMAKE);
}

VOID CMascotSkillReMake::ClickedRemakeButton(gui::CComponent* pComponent)
{
	SMascotInfo* pInfo = GetNtlSLGlobal()->GetAvatarMascotInfo();
	sMASCOT_DATA_EX* pData = pInfo->Find(m_byMascotIndex);
	if (m_bFilling || pData == NULL || m_bySkillSlot >= DBO_MASCOT_MAX_SKILL_COUNT)
		return;

	if (pInfo->bySummonIndex != m_byMascotIndex)
	{
		GetAlarmManager()->AlarmMessage("DST_MASCOTEX_SKILLGRADEUP_DONT_SUMMON");
		return;
	}

	if (pData->skillTblidx[m_bySkillSlot] == INVALID_TBLIDX)
	{
		if (GetDboGlobal()->GetGamePacketGenerator()->SendMascotSkillAddReq(m_bySkillSlot))
			StartFill();
		return;
	}

	RwUInt8 byItemPlace, byItemPos;
	if (!Logic_GetItemContainerPosFromHandle(m_hItem, byItemPlace, byItemPos))
	{
		GetAlarmManager()->AlarmMessage("DST_MASCOTEX_SKILLROLL_ITEM_NO");
		return;
	}

	// the slot shows what is re-rolled now, the result appears on the right
	m_oldSkillTblidx = pData->skillTblidx[m_bySkillSlot];

	if (GetDboGlobal()->GetGamePacketGenerator()->SendMascotSkillUpdateReq(m_bySkillSlot, byItemPlace, byItemPos))
	{
		// the new skill appears on the right once the gauges are full
		m_newSkillTblidx = INVALID_TBLIDX;
		Refresh();
		StartFill();
	}
}

VOID CMascotSkillReMake::OnMouseEnter(gui::CComponent* pComponent)
{
	m_iFocusSlot = -1;
	for (RwInt32 i = 0; i < REMAKE_SLOT_COUNT; ++i)
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

VOID CMascotSkillReMake::OnMouseLeave(gui::CComponent* pComponent)
{
	m_iFocusSlot = -1;
	m_surFocus.Show(FALSE);
}

VOID CMascotSkillReMake::OnMouseUp(const CKey& key)
{
	if (m_iFocusSlot != REMAKE_SLOT_ITEM)
		return;

	if (GetIconMoveManager()->IsActive())
	{
		RwBool bFromBag = GetIconMoveManager()->GetSrcPlace() == PLACE_BAG;
		SERIAL_HANDLE hItem = GetIconMoveManager()->GetSrcSerial();
		GetIconMoveManager()->IconMoveEnd();

		if (bFromBag)
			SetItem(hItem);
	}
	else if (key.m_nID == UD_RIGHT_BUTTON)
	{
		ClearItem();
		Refresh();
	}
}

VOID CMascotSkillReMake::OnPaint()
{
	for (RwInt32 i = 0; i < REMAKE_SLOT_COUNT; ++i)
		m_Slot[i].Paint();

	m_surFocus.Render();
}

VOID CMascotSkillReMake::OnMove(RwInt32 iOldX, RwInt32 iOldY)
{
	CRectangle rtScreen = m_pThis->GetScreenRect();

	for (RwInt32 i = 0; i < REMAKE_SLOT_COUNT; ++i)
		m_Slot[i].SetParentPosition(rtScreen.left, rtScreen.top);
}
