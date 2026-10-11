#include "precomp_dboclient.h"
#include "DynamicFieldSideIconGui.h"

// core
#include "NtlDebug.h"

// presentation
#include "NtlPLGuiManager.h"

// simulation
#include "NtlSLApi.h"
#include "NtlSLGlobal.h"
#include "NtlSLLogic.h"
#include "NtlSLEventFunc.h"
#include "NtlSobAvatar.h"
#include "DboTSCQAgency.h"
#include "DboTSCMain.h"

// dbo
#include "DboEvent.h"


namespace
{
	// CDboTSClickSideIcon::sitype used by the world boss quests
	const RwUInt8		DYNAMIC_FIELD_SIDEICON_TYPE = 1;

	// world boss quests started through the side icon (lowest level range first)
	const NTL_TS_T_ID	DYNAMIC_FIELD_QUEST_IDS[] = { 6860, 6861 };

	// used when the quest data could not be read
	const RwUInt8		DYNAMIC_FIELD_DEFAULT_MIN_LEVEL = 41;

	const RwReal		STAGE_ONE_MAX_PERCENT = 100.0f / 3.0f;
	const RwReal		STAGE_TWO_MAX_PERCENT = 200.0f / 3.0f;

	const char* const	STAGE_BUTTON_NAMES[CDynamicFieldSideIconGui::STAGE_COUNT] =
	{
		"btnStepOneSideIcon",
		"btnStepTwoSideIcon",
		"btnStepThreeSideIcon",
		"btnStepDisableSideIcon",
	};
}


CDynamicFieldSideIconGui::CDynamicFieldSideIconGui(const RwChar* pName)
	: CSideIconBase(pName)
	, m_eStage(STAGE_INVALID)
	, m_bReceivedCount(false)
	, m_bCanParticipate(false)
{
	for (int i = 0; i < STAGE_COUNT; ++i)
		m_apBtnStage[i] = NULL;
}

CDynamicFieldSideIconGui::~CDynamicFieldSideIconGui(void)
{
}

RwBool CDynamicFieldSideIconGui::Create()
{
	NTL_FUNCTION(__FUNCTION__);

	if (!CNtlPLGui::Create("gui\\Game.rsr", "gui\\DynamicFieldSideIcon.srf", "gui\\DynamicFieldSideIcon.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");

	for (int i = 0; i < STAGE_COUNT; ++i)
	{
		m_apBtnStage[i] = (gui::CButton*)GetComponent(STAGE_BUTTON_NAMES[i]);
		if (m_apBtnStage[i] == NULL)
			NTL_RETURN(FALSE);

		// all stage buttons share the same handlers. Only the button of the current stage is visible.
		m_aSlotClicked[i] = m_apBtnStage[i]->SigClicked().Connect(this, &CDynamicFieldSideIconGui::OnIconButtonClicked);
		m_aSlotMouseEnter[i] = m_apBtnStage[i]->SigMouseEnter().Connect(this, &CDynamicFieldSideIconGui::OnMouseEnter);
		m_aSlotMouseLeave[i] = m_apBtnStage[i]->SigMouseLeave().Connect(this, &CDynamicFieldSideIconGui::OnMouseLeave);
	}

	SetStage(STAGE_ONE);

	LinkMsg(g_EventDynamicFieldCount);
	LinkMsg(g_EventSobInfoUpdate);

	Show(false);

	NTL_RETURN(TRUE);
}

VOID CDynamicFieldSideIconGui::Destroy()
{
	UnLinkMsg(g_EventDynamicFieldCount);
	UnLinkMsg(g_EventSobInfoUpdate);

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();
	Show(false);
}

VOID CDynamicFieldSideIconGui::HandleEvents(RWS::CMsg& msg)
{
	if (msg.Id == g_EventDynamicFieldCount)
	{
		const SDboEventDynamicFieldCount* pData = reinterpret_cast<const SDboEventDynamicFieldCount*>(msg.pData);

		SetStage(CalcStage(*pData));

		m_bReceivedCount = true;
		UpdateVisibility();
	}
	else if (msg.Id == g_EventSobInfoUpdate)
	{
		const SNtlEventSobInfoUpdate* pUpdate = reinterpret_cast<const SNtlEventSobInfoUpdate*>(msg.pData);

		CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
		if (pAvatar && pUpdate->hSerialId == pAvatar->GetSerialID() && (pUpdate->uiUpdateType & EVENT_AIUT_ATTR_LEVELUP))
			UpdateVisibility();
	}
}

CDynamicFieldSideIconGui::eSTAGE CDynamicFieldSideIconGui::CalcStage(const SDboEventDynamicFieldCount& rCount)
{
	if (rCount.IsEventActive())
		return STAGE_BOSS;

	RwReal fPercent = rCount.GetProgressPercent();

	if (fPercent <= STAGE_ONE_MAX_PERCENT)
		return STAGE_ONE;
	else if (fPercent <= STAGE_TWO_MAX_PERCENT)
		return STAGE_TWO;

	return STAGE_THREE;
}

void CDynamicFieldSideIconGui::SetStage(eSTAGE eStage)
{
	if (m_eStage == eStage)
		return;

	m_eStage = eStage;

	for (int i = 0; i < STAGE_COUNT; ++i)
	{
		bool bActive = (i == eStage);

		m_apBtnStage[i]->Show(bActive);
		m_apBtnStage[i]->Enable(bActive);
	}
}

void CDynamicFieldSideIconGui::UpdateVisibility()
{
	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();

	m_bCanParticipate = pAvatar && Logic_GetLevel(pAvatar) >= GetMinParticipationLevel();

	Show(true);
}

RwUInt8 CDynamicFieldSideIconGui::GetMinParticipationLevel()
{
	CDboTSCQAgency* pAgency = API_GetQuestAgency();
	if (pAgency == NULL)
		return DYNAMIC_FIELD_DEFAULT_MIN_LEVEL;

	CDboTSCMain* pTSMain = (CDboTSCMain*)pAgency->GetParent();
	if (pTSMain == NULL)
		return DYNAMIC_FIELD_DEFAULT_MIN_LEVEL;

	int nMinLevel = INT_MAX;

	for (size_t i = 0; i < _countof(DYNAMIC_FIELD_QUEST_IDS); ++i)
	{
		CNtlTSTrigger* pTrig = pTSMain->FindQuestFromTS(DYNAMIC_FIELD_QUEST_IDS[i]);
		if (pTrig == NULL)
			continue;

		CNtlTSGroup* pGroup = pTrig->GetGroup(NTL_TS_MAIN_GROUP_ID);
		if (pGroup == NULL)
			continue;

		CNtlTSCont* pStartCont = pGroup->GetChildCont(START_CONTAINER_ID);
		if (pStartCont == NULL)
			continue;

		for (int nEntity = 0; nEntity < pStartCont->GetNumOfChildEntity(); ++nEntity)
		{
			CNtlTSEntity* pEntity = pStartCont->GetChildEntity(nEntity);
			if (pEntity && pEntity->GetEntityType() == DBO_COND_TYPE_ID_CHECK_LVL)
			{
				int nQuestMinLevel = static_cast<CDboTSCheckLvl*>(pEntity)->GetMinLvl();
				if (nQuestMinLevel < nMinLevel)
					nMinLevel = nQuestMinLevel;
			}
		}
	}

	if (nMinLevel == INT_MAX)
		return DYNAMIC_FIELD_DEFAULT_MIN_LEVEL;

	return (RwUInt8)nMinLevel;
}

VOID CDynamicFieldSideIconGui::OnIconButtonClicked(gui::CComponent* pComponent)
{
	CSideIconGui::GetInstance()->CloseSideView(SIDEVIEW_DYNAMIC_FIELD);

	// quest agency picks the world boss quest whose level range matches the avatar
	CNtlSLEventGenerator::TSClickSideIcon(DYNAMIC_FIELD_SIDEICON_TYPE);
}

VOID CDynamicFieldSideIconGui::OnSideViewClosed()
{
}

void CDynamicFieldSideIconGui::OnMouseEnter(gui::CComponent* pComponent)
{
	CSideIconGui::GetInstance()->OpenSideView(this, SIDEVIEW_DYNAMIC_FIELD, NULL);
}

void CDynamicFieldSideIconGui::OnMouseLeave(gui::CComponent* pComponent)
{
	CSideIconGui::GetInstance()->CloseSideView(SIDEVIEW_DYNAMIC_FIELD);
}

void CDynamicFieldSideIconGui::Show(bool bShow)
{
	// only visible for avatars that can participate, once we know the event status
	__super::Show(bShow && m_bReceivedCount && m_bCanParticipate);
}
