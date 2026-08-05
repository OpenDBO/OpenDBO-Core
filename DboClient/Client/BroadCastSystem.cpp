#include "precomp_dboclient.h"
#include "BroadCastSystem.h"

// core
#include "NtlDebug.h"

// gui library
#include "gui_renderer.h"
#include "GuiUtil.h"

// simulation layer
#include "NtlSLEvent.h"
#include "NtlPLEvent.h"
#include "NtlSLEventFunc.h"
#include "NtlSLLogic.h"

// presentation
#include "NtlPLGuiManager.h"

// Dbo
#include "DboGlobal.h"
#include "DialogManager.h"
#include "DialogPriority.h"

CBroadCastSystem::CBroadCastSystem(const RwChar* pName)
	: CNtlPLGui(pName)
{
	m_iTextValues = 1;
	m_bIsPaused = FALSE;
}

CBroadCastSystem::~CBroadCastSystem()
{

}

RwBool CBroadCastSystem::Create()
{
	NTL_FUNCTION("CBroadCastSystem::Create");

	if (!CNtlPLGui::Create("", "gui\\Commercial.srf", "gui\\Commercial.frm"))
		NTL_RETURN(FALSE);


	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");
	m_pThis->SetPriority(dDIALOGPRIORITY_MSGBOX);
	m_pThis->SetClippingMode(true);
	m_stBackGround = (gui::CStaticBox*)GetComponent("stBackGround");
	m_stBackGround->SetClippingMode(true);
	m_stString = (gui::CStaticBox*)GetComponent("stString");
	m_stString->SetClippingMode(true);
	m_stString->SetText("");
	m_stSecondString = (gui::CStaticBox*)GetComponent("stSecondString");
	m_stSecondString->SetClippingMode(true);
	m_stSecondString->SetText("");

	m_slotMouseEnter = m_pThis->SigMouseEnter().Connect(this, &CBroadCastSystem::OnMouseEnter);
	m_slotMouseLeave = m_pThis->SigMouseLeave().Connect(this, &CBroadCastSystem::OnMouseLeave);

	LocateComponent(GetDboGlobal()->GetScreenWidth(), GetDboGlobal()->GetScreenHeight());

	GetNtlGuiManager()->AddUpdateFunc(this);

	Show(false);

	LinkMsg(g_EventBroadCastNfy, 0);
	LinkMsg(g_EventResize, 0);
	InitStringBoxFirst();
	InitStringBoxSecond();
	NTL_RETURN(TRUE);
}

VOID CBroadCastSystem::Destroy()
{
	NTL_FUNCTION("CBroadCastSystem::Destroy");

	UnLinkMsg(g_EventBroadCastNfy);
	UnLinkMsg(g_EventResize);

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();
}

VOID CBroadCastSystem::LocateComponent(RwInt32 iWidth, RwInt32 iHeight)
{
	m_pThis->SetPosition((iWidth - m_pThis->GetWidth()) / 2, iHeight - 170);
}

VOID CBroadCastSystem::ResizeEventHandler(RWS::CMsg& msg)
{
	SNtlPLEventResize* pPacket = reinterpret_cast<SNtlPLEventResize*>(msg.pData);
	LocateComponent(pPacket->iWidht, pPacket->iHeight);
}

VOID CBroadCastSystem::OnMouseEnter(gui::CComponent* pComponent)
{
	m_bIsPaused = TRUE;
}

VOID CBroadCastSystem::OnMouseLeave(gui::CComponent* pComponent)
{
	m_bIsPaused = FALSE;
}

VOID CBroadCastSystem::HandleEvents(RWS::CMsg& msg)
{

	SDboEventBroadCastNfy* pNotify = reinterpret_cast<SDboEventBroadCastNfy*>(msg.pData);

	if (msg.Id == g_EventBroadCastNfy) 
	{
		WCHAR Text[500];

		if (pNotify->MsgType == DBO_BROADCASTING_MSG_TYPE_ITEMUPGRADE) 
		{
			swprintf_s(Text, 500, GetDisplayStringManager()->GetString("DST_COMMERCIAL_ITEMUPGRADE"), pNotify->pData.sItemUpgrade.wszName, Logic_GetItemName(pNotify->pData.sItemUpgrade.tblidx), pNotify->pData.sItemUpgrade.byGrade);
			if (m_iTextValues % 2 == 0)
				m_sTextTwo.push_back(Text);
			else
				m_sTextOne.push_back(Text);
			
			Show(true);
			m_iTextValues++;
		}
		else if (pNotify->MsgType == DBO_BROADCASTING_MSG_TYPE_ITEMMIX) 
		{
		}
		else if (pNotify->MsgType == DBO_BROADCASTING_MSG_TYPE_TMQ) 
		{
		}
		else if (pNotify->MsgType == DBO_BROADCASTING_MSG_TYPE_BUDOKAI) 
		{
			std::wstring winner;
			if (pNotify->pData.sBudokaiRecord.byRank == 0) {
				winner = GetDisplayStringManager()->GetString("DST_BUDOKAI_PC_STATE_FINAL_WINNER");
			}
			else if (pNotify->pData.sBudokaiRecord.byRank == 1) {
				winner = GetDisplayStringManager()->GetString("DST_BUDOKAI_PC_STATE_SEMIFINAL_WINNER");
			}
			else if (pNotify->pData.sBudokaiRecord.byRank == 2) {
				winner = GetDisplayStringManager()->GetString("DST_BUDOKAI_PC_STATE_PRIZE_WINNER");
			}

			if (pNotify->pData.sBudokaiRecord.byMatchType == BUDOKAI_MATCH_TYPE_INDIVIDIAUL)
			{
				if (pNotify->pData.sBudokaiRecord.byBudokaiType == BUDOKAI_TYPE_JUNIOR)
				{
					swprintf_s(Text, 500, GetDisplayStringManager()->GetString("DST_COMMERCIAL_BUDOKAI"),
						pNotify->pData.sBudokaiRecord.wSeason,
						GetDisplayStringManager()->GetString("DST_BUDOKAI_TYPE_CHILD_TITLE"),
						GetDisplayStringManager()->GetString("DST_BUDOKAI_NOTICE_SOLO"),
						pNotify->pData.sBudokaiRecord.awszMember[0],
						winner.c_str());

					if (m_iTextValues % 2 == 0)
						m_sTextTwo.push_back(Text);
					else
						m_sTextOne.push_back(Text);

					Show(true);
					m_iTextValues++;
				}
				else 
				{
					swprintf_s(Text, 500, GetDisplayStringManager()->GetString("DST_COMMERCIAL_BUDOKAI"),
						pNotify->pData.sBudokaiRecord.wSeason,
						GetDisplayStringManager()->GetString("DST_BUDOKAI_TYPE_ADULT_TITLE"),
						GetDisplayStringManager()->GetString("DST_BUDOKAI_NOTICE_SOLO"),
						pNotify->pData.sBudokaiRecord.awszMember[0],
						winner.c_str());

					if (m_iTextValues % 2 == 0)
						m_sTextTwo.push_back(Text);
					else
						m_sTextOne.push_back(Text);

					Show(true);
					m_iTextValues++;
				}
			}
			else {
				
				std::wstring CharName;

				for (int i = 0; i < pNotify->pData.sBudokaiRecord.byMemberCount; i++) {
					CharName += pNotify->pData.sBudokaiRecord.awszMember[i];
					if (i != pNotify->pData.sBudokaiRecord.byMemberCount - 1) {
						CharName += L", ";
					}
				}

				if (pNotify->pData.sBudokaiRecord.byBudokaiType == BUDOKAI_TYPE_JUNIOR)
				{
					swprintf_s(Text, 500, GetDisplayStringManager()->GetString("DST_COMMERCIAL_BUDOKAI"),
						pNotify->pData.sBudokaiRecord.wSeason,
						GetDisplayStringManager()->GetString("DST_BUDOKAI_TYPE_CHILD_TITLE"),
						GetDisplayStringManager()->GetString("DST_BUDOKAI_NOTICE_PARTY"),
						CharName.c_str(), 
						winner.c_str());
				
					if (m_iTextValues % 2 == 0)
						m_sTextTwo.push_back(Text);
					else
						m_sTextOne.push_back(Text);

					Show(true);
					m_iTextValues++;
				}
				else 
				{
					swprintf_s(Text, 500, GetDisplayStringManager()->GetString("DST_COMMERCIAL_BUDOKAI"),
						pNotify->pData.sBudokaiRecord.wSeason,
						GetDisplayStringManager()->GetString("DST_BUDOKAI_TYPE_ADULT_TITLE"),
						GetDisplayStringManager()->GetString("DST_BUDOKAI_NOTICE_PARTY"),
						CharName.c_str(),
						winner.c_str());

					if (m_iTextValues % 2 == 0)
						m_sTextTwo.push_back(Text);
					else
						m_sTextOne.push_back(Text);

					Show(true);
					m_iTextValues++;
				}

			}

		}
		else if (pNotify->MsgType == DBO_BROADCASTING_MSG_TYPE_WAGUWAGU_1ST) 
		{
			/* TODO: Uncomment when wagu system is done
			swprintf_s(Text, 500, GetDisplayStringManager()->GetString("DST_WAGUWAGU_BROADINFO"), pNotify->pData.sWaguWagu1st.wszName, Logic_GetItemName(pNotify->pData.sWaguWagu1st.itemTblidx));

			if (m_iTextValues % 2 == 0)
				m_sTextTwo.push_back(Text);
			else
				m_sTextOne.push_back(Text);

			Show(true);
			m_iTextValues++;
			*/
		}
		else if (pNotify->MsgType == DBO_BROADCASTING_MSG_TYPE_CCBD) 
		{
			std::wstring CharName;

			for (int i = 0; i < pNotify->pData.sCCBD.byMemberCount;i++) {
				CharName += pNotify->pData.sCCBD.awszMember[i];
				if (i != pNotify->pData.sCCBD.byMemberCount - 1) {
					CharName += L", ";
				}
			}

			swprintf_s(Text, 500, GetDisplayStringManager()->GetString("DST_BROADCAST_CCBD_CLEAR"), CharName.c_str(), pNotify->pData.sCCBD.byStage);
			if (m_iTextValues % 2 == 0)
				m_sTextTwo.push_back(Text);
			else
				m_sTextOne.push_back(Text);

			Show(true);
			m_iTextValues++;
		}
	}
	else if (msg.Id == g_EventResize)
		ResizeEventHandler(msg);

}


void CBroadCastSystem::Update(RwReal fElapsed)
{
	if (!m_bIsPaused)
	{
	if (!m_sTextOne.empty())
	{
		if(m_bIsTwoTextOver || m_stString->GetScreenRect().right < m_stSecondString->GetScreenRect().left)
		{
			m_fBoxPresentPosX -= (fElapsed * (RwReal)g_nTestVal);
			m_stString->SetPosition(m_fBoxPresentPosX, 19);
			if (m_stString->GetText().empty())
				m_stString->SetText(m_sTextOne.front().c_str());

			if (m_stString->GetScreenRect().right < m_stBackGround->GetScreenRect().right)
				m_bIsOneTextOver = true;
			else 
				m_bIsOneTextOver = false;

			if (m_stString->GetScreenRect().right < m_stBackGround->GetScreenRect().left)
			{
				InitStringBoxFirst();
				m_sTextOne.erase(m_sTextOne.begin());
			}
		}
	}
			
	if (!m_sTextTwo.empty()) 
	{
		if(m_bIsOneTextOver || m_stSecondString->GetScreenRect().right < m_stString->GetScreenRect().left)
		{
			m_fBoxPresentPosXTwo -= (fElapsed * (RwReal)g_nTestVal);
			m_stSecondString->SetPosition(m_fBoxPresentPosXTwo, 19);
			if (m_stSecondString->GetText().empty())
				m_stSecondString->SetText(m_sTextTwo.front().c_str());

			if (m_stSecondString ->GetScreenRect().right < m_stBackGround->GetScreenRect().right)
				m_bIsTwoTextOver = true;
			else
				m_bIsTwoTextOver = false;

			if (m_stSecondString->GetScreenRect().right < m_stBackGround->GetScreenRect().left)
			{
				InitStringBoxSecond();
				m_sTextTwo.erase(m_sTextTwo.begin());
			}
		}
	}
	}

	if (m_sTextOne.empty() && m_sTextTwo.empty())
	{
		if (m_pThis->IsVisible())
		{
			m_iTextValues = 1;
			Show(false);
		}	
	}
}

RwInt32 CBroadCastSystem::SwitchDialog(bool bOpen)
{
	Show(bOpen);
	return TRUE;
}

void CBroadCastSystem::InitStringBox(gui::CStaticBox* pStringBox, RwBool& bIsOver, RwReal& fBoxPresentPosX)
{
	CRectangle rtScreen = m_stBackGround->GetScreenRect();
	bIsOver = true;
	fBoxPresentPosX = (RwReal)rtScreen.GetWidth() + 15;
	pStringBox->Clear();
	pStringBox->SetPosition(fBoxPresentPosX, 19);
}

void CBroadCastSystem::InitStringBoxFirst()
{
	InitStringBox(m_stString, m_bIsOneTextOver, m_fBoxPresentPosX);
}

void CBroadCastSystem::InitStringBoxSecond()
{
	InitStringBox(m_stSecondString, m_bIsTwoTextOver, m_fBoxPresentPosXTwo);
}