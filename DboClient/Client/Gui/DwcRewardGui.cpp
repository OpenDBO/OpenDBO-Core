#include "precomp_dboclient.h"
#include "DwcRewardGui.h"

// core
#include "NtlDebug.h"

// shared
#include "CharTitleTable.h"
#include "DwcMissionTable.h"
#include "TableContainer.h"
#include "TextAllTable.h"

// presentation
#include "NtlPLGuiManager.h"
#include "NtlPLEvent.h"

// simulation
#include "NtlSLApi.h"

// dbo
#include "DboEvent.h"
#include "DboGlobal.h"
#include "DboLogic.h"
#include "DialogManager.h"
#include "DisplayStringManager.h"
#include "InfoWndManager.h"

#define dDWC_REWARD_FLASH_BG			"DWC_result_bg.swf"
#define dDWC_REWARD_FLASH_RESULT		"DWC_result.swf"
#define dDWC_REWARD_FLASH_CARD			"DWC_rotate_drop.swf"
#define dDWC_REWARD_FLASH_CAPSULE		"TMQ_result_Capsule.swf"

// tw layout, offsets from the middle of the screen
#define dDWC_REWARD_FLASH_CARD_X		-417
#define dDWC_REWARD_FLASH_CARD_Y		-183
#define dDWC_REWARD_CARD_X				-325
#define dDWC_REWARD_CARD_Y				-104
#define dDWC_REWARD_CONFIRM_X			288
#define dDWC_REWARD_CONFIRM_Y			49
#define dDWC_REWARD_TEXT_X				48
#define dDWC_REWARD_MISSION_Y			-89
#define dDWC_REWARD_TITLE_Y				-42

// tw: two rows of 4 slots, basic rewards on top and repeat rewards below
#define dDWC_REWARD_SLOT_COLUMN			4
#define dDWC_REWARD_SLOT_X				40
#define dDWC_REWARD_SLOT_GAP			44
#define dDWC_REWARD_SLOT_TOP_Y			19
#define dDWC_REWARD_SLOT_BOTTOM_Y		66
#define dDWC_REWARD_SLOT_ICON_GAP		2


CDwcRewardGui::CDwcRewardGui(const RwChar* pName)
: CNtlPLGui(pName)
{
	m_pflaResultBg = NULL;
	m_pflaResult = NULL;
	m_pflaCard = NULL;
	m_pflaCapsule = NULL;
	m_ppnlCard = NULL;
	m_ppnlCardGrade = NULL;
	m_pstbMission = NULL;
	m_pstbCharTitle = NULL;
	m_pbtnConfirm = NULL;
	m_pCardTexture = NULL;
	m_iRevealCount = 0;
	m_iFocusSlot = -1;
}

CDwcRewardGui::~CDwcRewardGui(VOID)
{
}

RwBool CDwcRewardGui::Create(VOID)
{
	NTL_FUNCTION("CDwcRewardGui::Create");

	if (!CNtlPLGui::Create("gui\\DWC.rsr", "gui\\DWC.srf", "gui\\DWC_Reward.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(GetNtlGuiManager()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");

	m_pflaResult = (gui::CFlash*)GetComponent("flaResult");
	m_pflaResultBg = (gui::CFlash*)GetComponent("flaResultBg");
	m_pflaResultBg->SetResizeType(gui::CFlash::EXACTFIT);
	m_pflaCard = (gui::CFlash*)GetComponent("flaCard");
	m_pflaCapsule = (gui::CFlash*)GetComponent("flaCapsule");

	// the challenge flash is not used
	GetComponent("flaChaanllenge")->Show(false);

	m_ppnlCard = (gui::CPanel*)GetComponent("pnlCard");
	m_ppnlCardGrade = (gui::CPanel*)GetComponent("pnlCardGrade");

	m_pstbMission = (gui::CStaticBox*)GetComponent("stbMission");
	m_pstbCharTitle = (gui::CStaticBox*)GetComponent("stbCharTitle");

	m_pbtnConfirm = (gui::CButton*)GetComponent("btnConfirm");
	m_pbtnConfirm->SetText(GetDisplayStringManager()->GetString("DST_LOBBY_OK"));

	for (RwInt32 i = 0; i < dDWC_REWARD_SLOT_COUNT; ++i)
	{
		m_asrfSlotBack[i].SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("GameCommon.srf", "srfSlot"));
		m_aSlot[i].Create(m_pThis, DIALOG_DWC_REWARD, REGULAR_SLOT_ITEM_TABLE, SDS_COUNT);
	}

	m_slotClickConfirm = m_pbtnConfirm->SigClicked().Connect(this, &CDwcRewardGui::OnClickConfirm);
	m_slotMouseMove = m_pThis->SigMouseMove().Connect(this, &CDwcRewardGui::OnMouseMove);
	m_slotMouseLeave = m_pThis->SigMouseLeave().Connect(this, &CDwcRewardGui::OnMouseLeave);
	m_slotPaintSlot = m_pflaResultBg->SigPaint().Connect(this, &CDwcRewardGui::OnPaintSlot);
	m_slotCardEnd = m_pflaCard->SigMovieEnd().Connect(this, &CDwcRewardGui::OnMovieEnd);
	m_slotCapsuleEnd = m_pflaCapsule->SigMovieEnd().Connect(this, &CDwcRewardGui::OnMovieEnd);

	m_pThis->SetPriority(dDIALOGPRIORITY_RESULT);

	LinkMsg(g_EventDwcNotify);
	LinkMsg(g_EventResize);

	Show(false);

	NTL_RETURN(TRUE);
}

VOID CDwcRewardGui::Destroy(VOID)
{
	NTL_FUNCTION("CDwcRewardGui::Destroy");

	GetNtlGuiManager()->RemoveUpdateFuncWithoutRefCheck(this);

	UnLinkMsg(g_EventDwcNotify);
	UnLinkMsg(g_EventResize);

	ClearReward();

	for (RwInt32 i = 0; i < dDWC_REWARD_SLOT_COUNT; ++i)
		m_aSlot[i].Destroy();

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();

	NTL_RETURNVOID();
}

VOID CDwcRewardGui::Update(RwReal fElapsed)
{
	if (!IsShow())
		return;

	m_pflaResultBg->Update(fElapsed);
	m_pflaResult->Update(fElapsed);
	m_pflaCard->Update(fElapsed);

	if (m_pflaCapsule->IsVisible())
		m_pflaCapsule->Update(fElapsed);
}

RwInt32 CDwcRewardGui::SwitchDialog(bool bOpen)
{
	if (bOpen)
	{
		LocateComponent();
		Show(true);

		m_pflaResultBg->PlayMovie(TRUE);
		m_pflaCard->PlayMovie(TRUE);
		m_pflaResult->PlayMovie(TRUE);

		GetNtlGuiManager()->AddUpdateFunc(this);
	}
	else
	{
		Show(false);

		GetNtlGuiManager()->RemoveUpdateFunc(this);

		if (GetInfoWndManager()->GetRequestGui() == DIALOG_DWC_REWARD)
			GetInfoWndManager()->ShowInfoWindow(FALSE);

		m_pflaResultBg->Unload();
		m_pflaResult->Unload();
		m_pflaCard->Unload();
		m_pflaCapsule->Unload();
		ClearReward();
	}

	return TRUE;
}

VOID CDwcRewardGui::HandleEvents(RWS::CMsg& msg)
{
	if (msg.Id == g_EventDwcNotify)
	{
		SDboEventDwcNotify* pData = reinterpret_cast<SDboEventDwcNotify*>(msg.pData);
		if (pData->byType != DWC_EVENT_MISSION_REWARD)
			return;

		sGU_DWC_MISSION_REWARD_NFY* pReward = reinterpret_cast<sGU_DWC_MISSION_REWARD_NFY*>(pData->pData);

		if (GetDialogManager()->IsOpenDialog(DIALOG_DWC_REWARD))
			GetDialogManager()->CloseDialog(DIALOG_DWC_REWARD);

		LoadFlash(m_pflaResultBg, dDWC_REWARD_FLASH_BG);
		LoadFlash(m_pflaResult, dDWC_REWARD_FLASH_RESULT);
		LoadFlash(m_pflaCard, dDWC_REWARD_FLASH_CARD);
		LoadFlash(m_pflaCapsule, dDWC_REWARD_FLASH_CAPSULE);
		m_pflaCapsule->Show(false);

		SetReward(pReward->dwcMissionTblidx, pReward->bIsBasicReward, pReward->byRewardGrade);

		GetDialogManager()->OpenDialog(DIALOG_DWC_REWARD);
	}
	else if (msg.Id == g_EventResize)
	{
		if (IsShow())
			LocateComponent();
	}
}

VOID CDwcRewardGui::SetReward(RwUInt32 dwcMissionTblidx, RwBool bIsBasicReward, RwUInt8 byRewardGrade)
{
	ClearReward();

	sDWCMISSION_TBLDAT* pMissionTbldat = reinterpret_cast<sDWCMISSION_TBLDAT*>(API_GetTableContainer()->GetDwcMissionTable()->FindData(dwcMissionTblidx));
	if (pMissionTbldat == NULL || byRewardGrade >= MAX_DWC_REWARD_GRADE_COUNT)
		return;

	sDWCMISSION_REWARD& rReward = pMissionTbldat->asReward[byRewardGrade];

	// tw: the card texture and the grade go on the panels
	std::string strTexture = Logic_GetDwcCardTexture(pMissionTbldat, byRewardGrade);
	m_pCardTexture = Logic_CreateTexture((RwChar*)strTexture.c_str(), TEXTYPE_NONE);
	if (m_pCardTexture)
	{
		gui::CSurface surface;
		surface.m_pTexture = m_pCardTexture;
		surface.m_Original.rtRect.SetRect(0, 0, m_ppnlCard->GetWidth(), m_ppnlCard->GetHeight());
		surface.m_SnapShot.rtRect = surface.m_Original.rtRect;
		m_ppnlCard->AddSurface(surface);
	}

	static const char* s_apcGrade[MAX_DWC_REWARD_GRADE_COUNT] = { "srfSmallNormal", "srfSmallSuperior", "srfSmallExcellent", "srfSmallRare", "srfSmallLegendary" };
	m_ppnlCardGrade->AddSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("DWC.srf", s_apcGrade[byRewardGrade]));

	if (CTextTable* pDwcText = API_GetTableContainer()->GetTextAllTable()->GetDWCTbl())
		m_pstbMission->SetText(pDwcText->GetText(rReward.cardNameTblidx).c_str());

	// tw: a new card gives the basic reward in the top row, a repeated clear the repeat reward in the bottom row
	sDWCMISSION_REWARD_ENTITY* pEntity = bIsBasicReward ? rReward.asBasicReward : rReward.asRepeatReward;
	RwInt32 iSlot = bIsBasicReward ? 0 : dDWC_REWARD_SLOT_COLUMN;

	CCharTitleTable* pTitleTable = API_GetTableContainer()->GetCharTitleTable();
	CTextTable* pTitleText = API_GetTableContainer()->GetTextAllTable()->GetCharTitleTbl();

	for (RwInt32 i = 0; i < MAX_DWC_REWARD_COUNT; ++i, ++iSlot)
	{
		if (pEntity[i].tblidx == INVALID_TBLIDX)
			continue;

		if (pEntity[i].byType == DWC_REWARD_ITEM)
		{
			m_aSlot[iSlot].SetIcon(pEntity[i].tblidx, pEntity[i].byValue);
		}
		else if (pEntity[i].byType == DWC_REWARD_TITLE)
		{
			sCHARTITLE_TBLDAT* pTitleTbldat = reinterpret_cast<sCHARTITLE_TBLDAT*>(pTitleTable->FindData(pEntity[i].tblidx));
			if (pTitleTbldat && pTitleText)
				m_pstbCharTitle->SetText(pTitleText->GetText(pTitleTbldat->tblNameIndex).c_str());
		}
	}
}

VOID CDwcRewardGui::ClearReward(VOID)
{
	m_pstbMission->Clear();
	m_pstbCharTitle->Clear();
	m_ppnlCard->ClearSurface();
	m_ppnlCardGrade->ClearSurface();

	if (m_pCardTexture)
	{
		Logic_DeleteTexture(m_pCardTexture);
		m_pCardTexture = NULL;
	}

	for (RwInt32 i = 0; i < dDWC_REWARD_SLOT_COUNT; ++i)
		m_aSlot[i].Clear();

	m_iRevealCount = 0;
	m_iFocusSlot = -1;
}

VOID CDwcRewardGui::LoadFlash(gui::CFlash* pFlash, const char* pcFileName)
{
	if (!pFlash->Load(pcFileName))
	{
		DBO_WARNING_MESSAGE("CDwcRewardGui::LoadFlash, can not load " << pcFileName);
		return;
	}

	CRectangle rtRect = pFlash->GetFrameResolution();
	pFlash->SetSize(rtRect.GetWidth(), rtRect.GetHeight());
	pFlash->AutoRewind(FALSE);
	pFlash->SetFrameCallback(TRUE);	// SigMovieEnd only fires with it
	pFlash->PlayMovie(FALSE);
}

VOID CDwcRewardGui::LocateComponent(VOID)
{
	RwInt32 iScreenWidth = (RwInt32)GetDboGlobal()->GetScreenWidth();
	RwInt32 iScreenHeight = (RwInt32)GetDboGlobal()->GetScreenHeight();
	RwInt32 iX = iScreenWidth / 2;
	RwInt32 iY = iScreenHeight / 2;

	m_pThis->SetPosition(CRectangle(0, 0, iScreenWidth, iScreenHeight));

	m_pflaCard->SetPosition(iX + dDWC_REWARD_FLASH_CARD_X, iY + dDWC_REWARD_FLASH_CARD_Y);
	m_ppnlCard->SetPosition(iX + dDWC_REWARD_CARD_X, iY + dDWC_REWARD_CARD_Y);
	m_pflaResultBg->SetPosition(iX - m_pflaResultBg->GetWidth() / 2, iY - m_pflaResultBg->GetHeight() / 2);
	m_pflaResult->SetPosition(iX - m_pflaResult->GetWidth() / 2, iY - m_pflaResult->GetHeight() / 2);

	m_pbtnConfirm->SetPosition(iX + dDWC_REWARD_CONFIRM_X, iY + dDWC_REWARD_CONFIRM_Y);
	m_pstbMission->SetPosition(iX + dDWC_REWARD_TEXT_X, iY + dDWC_REWARD_MISSION_Y);
	m_pstbCharTitle->SetPosition(iX + dDWC_REWARD_TEXT_X, iY + dDWC_REWARD_TITLE_Y);

	for (RwInt32 i = 0; i < dDWC_REWARD_SLOT_COUNT; ++i)
	{
		RwInt32 iSlotX = iX + dDWC_REWARD_SLOT_X + (i % dDWC_REWARD_SLOT_COLUMN) * dDWC_REWARD_SLOT_GAP;
		RwInt32 iSlotY = iY + (i < dDWC_REWARD_SLOT_COLUMN ? dDWC_REWARD_SLOT_TOP_Y : dDWC_REWARD_SLOT_BOTTOM_Y);

		m_asrfSlotBack[i].SetPosition(iSlotX, iSlotY);
		m_aSlot[i].SetParentPosition(0, 0);
		m_aSlot[i].SetPosition_fromParent(iSlotX + dDWC_REWARD_SLOT_ICON_GAP, iSlotY + dDWC_REWARD_SLOT_ICON_GAP);
	}
}

VOID CDwcRewardGui::RevealNextSlot(VOID)
{
	// tw: the capsule plays over each reward slot in turn
	while (m_iRevealCount < dDWC_REWARD_SLOT_COUNT)
	{
		RwInt32 iSlot = m_iRevealCount++;
		if (m_aSlot[iSlot].GetSerial() == INVALID_SERIAL_ID)
			continue;

		RwInt32 iX = m_aSlot[iSlot].GetX_fromParent() + m_aSlot[iSlot].GetWidth() / 2;
		RwInt32 iY = m_aSlot[iSlot].GetY_fromParent() + m_aSlot[iSlot].GetHeight() / 2;

		m_pflaCapsule->SetPosition(iX - m_pflaCapsule->GetWidth() / 2, iY - m_pflaCapsule->GetHeight() / 2);
		m_pflaCapsule->Show(true);
		m_pflaCapsule->RestartMovie();
		return;
	}

	m_pflaCapsule->Show(false);
}

VOID CDwcRewardGui::OnClickConfirm(gui::CComponent* pComponent)
{
	GetDialogManager()->CloseDialog(DIALOG_DWC_REWARD);
}

VOID CDwcRewardGui::OnPaintSlot(VOID)
{
	for (RwInt32 i = 0; i < dDWC_REWARD_SLOT_COUNT; ++i)
	{
		if (m_aSlot[i].GetSerial() == INVALID_SERIAL_ID)
			continue;

		// the capsule shows the slot it is playing over
		if (i >= m_iRevealCount)
			continue;

		m_asrfSlotBack[i].Render();
		m_aSlot[i].Paint();
	}
}

VOID CDwcRewardGui::OnMouseMove(RwInt32 nFlags, RwInt32 nX, RwInt32 nY)
{
	RwInt32 iFocus = -1;

	for (RwInt32 i = 0; i < dDWC_REWARD_SLOT_COUNT && i < m_iRevealCount; ++i)
	{
		if (m_aSlot[i].GetSerial() != INVALID_SERIAL_ID && m_aSlot[i].PtInRect(nX, nY))
		{
			iFocus = i;
			break;
		}
	}

	if (iFocus == m_iFocusSlot)
		return;

	m_iFocusSlot = iFocus;

	if (iFocus < 0)
	{
		if (GetInfoWndManager()->GetRequestGui() == DIALOG_DWC_REWARD)
			GetInfoWndManager()->ShowInfoWindow(FALSE);

		return;
	}

	GetInfoWndManager()->ShowInfoWindow(TRUE, CInfoWndManager::INFOWND_TABLE_ITEM,
		m_aSlot[iFocus].GetX_fromParent(), m_aSlot[iFocus].GetY_fromParent(), m_aSlot[iFocus].GetItemTable(), DIALOG_DWC_REWARD);
}

VOID CDwcRewardGui::OnMouseLeave(gui::CComponent* pComponent)
{
	m_iFocusSlot = -1;

	if (GetInfoWndManager()->GetRequestGui() == DIALOG_DWC_REWARD)
		GetInfoWndManager()->ShowInfoWindow(FALSE);
}

VOID CDwcRewardGui::OnMovieEnd(gui::CComponent* pComponent)
{
	// the card dropped or a capsule opened, the next reward comes out
	RevealNextSlot();
}
