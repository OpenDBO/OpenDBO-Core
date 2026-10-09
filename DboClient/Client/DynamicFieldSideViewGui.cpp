#include "precomp_dboclient.h"
#include "DynamicFieldSideViewGui.h"

// core
#include "NtlDebug.h"

// presentation
#include "NtlPLGuiManager.h"

// simulation
#include "NtlSLApi.h"
#include "DboTSCQAgency.h"

// dbo
#include "DisplayStringManager.h"
#include "DialogDefine.h"


namespace
{
	const NTL_TS_T_ID	DYNAMIC_FIELD_QUEST_IDS[] = { 6860, 6861 };

	const RwInt32		SIDEVIEW_WIDTH = 250;
	const RwInt32		SIDEVIEW_HEIGHT = 190;
	const RwInt32		SIDEVIEW_LINE_HEIGHT = 14;
}


CDynamicFieldSideViewGui::CDynamicFieldSideViewGui(const RwChar* pName)
	: CSideViewBase(pName)
	, m_pstbTitle(NULL)
	, m_pstbProgress(NULL)
	, m_pstbMapInfo(NULL)
	, m_pstbCount(NULL)
	, m_pstbInfo1(NULL)
	, m_pstbInfo2(NULL)
	, m_pstbQuest(NULL)
	, m_fRemainEventTime(0.0f)
	, m_nShownEventSecs(-1)
{
	memset(&m_sCount, 0, sizeof(m_sCount));
}

CDynamicFieldSideViewGui::~CDynamicFieldSideViewGui(void)
{
}

RwBool CDynamicFieldSideViewGui::Create()
{
	if (!CNtlPLGui::Create("", "gui\\DynamicFieldSideView.srf", "gui\\DynamicFieldSideView.frm"))
		return FALSE;

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");

	m_BackPanel.SetType(CWindowby3::WT_HORIZONTAL);
	m_BackPanel.SetSurface(0, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("DynamicFieldSideView.srf", "srfDialogBackUp"));
	m_BackPanel.SetSurface(1, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("DynamicFieldSideView.srf", "srfDialogBackCenter"));
	m_BackPanel.SetSurface(2, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("DynamicFieldSideView.srf", "srfDialogBackDown"));

	m_slotPaint = m_pThis->SigPaint().Connect(this, &CDynamicFieldSideViewGui::OnPaint);
	m_slotMove = m_pThis->SigMove().Connect(this, &CDynamicFieldSideViewGui::OnMove);

	m_pstbTitle = CreateStaticBox(15, COMP_TEXT_CENTER, DEFAULT_FONT_SIZE, INFOCOLOR_7);
	m_pstbTitle->SetText(GetDisplayStringManager()->GetString("DST_DYNAMIC_FIELD_SIDEVIEW_TITLE_01"));

	m_pstbProgress = CreateStaticBox(40, COMP_TEXT_CENTER, 115, INFOCOLOR_5);

	m_pstbMapInfo = CreateStaticBox(60, COMP_TEXT_CENTER, 80, INFOCOLOR_5);
	m_pstbMapInfo->SetText(GetDisplayStringManager()->GetString("DST_DYNAMIC_FIELD_SIDEVIEW_TITLE_04"));

	m_pstbCount = CreateStaticBox(80, COMP_TEXT_CENTER, DEFAULT_FONT_SIZE, INFOCOLOR_5);

	m_pstbInfo1 = CreateStaticBox(105, COMP_TEXT_LEFT, DEFAULT_FONT_SIZE, INFOCOLOR_0, 20);
	m_pstbInfo1->SetText(GetDisplayStringManager()->GetString("DST_DYNAMIC_FIELD_SIDEVIEW_CONTENT_01"));

	m_pstbInfo2 = CreateStaticBox(125, COMP_TEXT_LEFT, DEFAULT_FONT_SIZE, INFOCOLOR_0, 20);
	m_pstbInfo2->SetText(GetDisplayStringManager()->GetString("DST_DYNAMIC_FIELD_SIDEVIEW_CONTENT_06"));

	m_pstbQuest = CreateStaticBox(150, COMP_TEXT_CENTER, DEFAULT_FONT_SIZE, INFOCOLOR_14);

	LinkMsg(g_EventDynamicFieldCount);

	Show(false);

	return TRUE;
}

VOID CDynamicFieldSideViewGui::Destroy()
{
	GetNtlGuiManager()->RemoveUpdateFuncWithoutRefCheck(this);

	UnLinkMsg(g_EventDynamicFieldCount);

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();
}

gui::CStaticBox* CDynamicFieldSideViewGui::CreateStaticBox(RwInt32 nY, RwUInt32 uiTextStyle, RwInt32 nFontHeight, COLORREF color, RwInt32 nX /*= 0*/)
{
	CRectangle rect;
	rect.SetRectWH(nX, nY, SIDEVIEW_WIDTH - nX, SIDEVIEW_LINE_HEIGHT);

	// owned and deleted by m_pThis
	gui::CStaticBox* pStaticBox = NTL_NEW gui::CStaticBox(rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), uiTextStyle);
	pStaticBox->CreateFontStd(DEFAULT_FONT, nFontHeight, DEFAULT_FONT_ATTR);
	pStaticBox->SetTextColor(color);

	return pStaticBox;
}

VOID CDynamicFieldSideViewGui::HandleEvents(RWS::CMsg& msg)
{
	if (msg.Id == g_EventDynamicFieldCount)
	{
		m_sCount = *reinterpret_cast<const SDboEventDynamicFieldCount*>(msg.pData);

		m_fRemainEventTime = (RwReal)m_sCount.nEventTime;
		m_nShownEventSecs = -1;

		if (IsShow())
			RefreshProgressText();
	}
}

VOID CDynamicFieldSideViewGui::Update(RwReal fElapsed)
{
	if (!IsShow() || !m_sCount.IsEventActive())
		return;

	m_fRemainEventTime -= fElapsed;
	if (m_fRemainEventTime < 0.0f)
		m_fRemainEventTime = 0.0f;

	// only touch the text when the displayed second changes
	if ((RwInt32)m_fRemainEventTime != m_nShownEventSecs)
		RefreshProgressText();
}

VOID CDynamicFieldSideViewGui::RefreshProgressText()
{
	m_pstbProgress->Format(L"%.1f%%", m_sCount.GetProgressPercent());

	if (m_sCount.IsEventActive())
	{
		m_nShownEventSecs = (RwInt32)m_fRemainEventTime;

		m_pstbCount->Format(L"%02d:%02d:%02d", m_nShownEventSecs / 3600, (m_nShownEventSecs / 60) % 60, m_nShownEventSecs % 60);
	}
	else
	{
		m_pstbCount->Format(L"%u / %u", m_sCount.dwCurCount, m_sCount.dwMaxCount);
	}
}

bool CDynamicFieldSideViewGui::HasDynamicFieldQuest()
{
	CDboTSCQAgency* pAgency = API_GetQuestAgency();
	if (pAgency == NULL)
		return false;

	for (size_t i = 0; i < _countof(DYNAMIC_FIELD_QUEST_IDS); ++i)
	{
		if (pAgency->FindProgressTrigger(DYNAMIC_FIELD_QUEST_IDS[i]))
			return true;
	}

	return false;
}

VOID CDynamicFieldSideViewGui::RefreshQuestText()
{
	if (HasDynamicFieldQuest())
	{
		m_pstbQuest->SetTextColor(INFOCOLOR_9);
		m_pstbQuest->SetText(GetDisplayStringManager()->GetString("DST_DYNAMIC_FIELD_SIDEVIEW_CONTENT_05"));
	}
	else
	{
		m_pstbQuest->SetTextColor(INFOCOLOR_14);
		m_pstbQuest->SetText(GetDisplayStringManager()->GetString("DST_DYNAMIC_FIELD_SIDEVIEW_CONTENT_04"));
	}
}

VOID CDynamicFieldSideViewGui::OnSideViewOpen(const void* pData)
{
	RefreshProgressText();
	RefreshQuestText();

	GetNtlGuiManager()->AddUpdateFunc(this);

	Show(true);
}

VOID CDynamicFieldSideViewGui::OnSideViewClose()
{
	GetNtlGuiManager()->RemoveUpdateFuncWithoutRefCheck(this);

	Show(false);
}

VOID CDynamicFieldSideViewGui::OnSideViewLocate(const CRectangle& rectSideIcon)
{
	m_pThis->SetHeight(SIDEVIEW_HEIGHT);
	LocateComponent();
	m_pThis->SetPosition(rectSideIcon.left - m_pThis->GetWidth() + rectSideIcon.GetWidth(), rectSideIcon.top - SIDEVIEW_HEIGHT);
}

VOID CDynamicFieldSideViewGui::OnPressESC()
{
}

VOID CDynamicFieldSideViewGui::LocateComponent()
{
	m_BackPanel.SetRect(m_pThis->GetScreenRect());
}

VOID CDynamicFieldSideViewGui::OnMove(RwInt32 iOldX, RwInt32 iOldY)
{
	LocateComponent();
}

VOID CDynamicFieldSideViewGui::OnPaint()
{
	m_BackPanel.Render();
}
