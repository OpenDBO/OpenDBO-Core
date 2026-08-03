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

	/*if (!CNtlPLGui::Create("", "gui\\BroadCastSystem.srf", "gui\\BroadCastSystem.frm"))
		NTL_RETURN(FALSE);
		*/
	if (!CNtlPLGui::Create("", "gui\\Commercial.srf", "gui\\Commercial.frm"))
		NTL_RETURN(FALSE);


	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");
	m_pThis->SetPriority(dDIALOGPRIORITY_MSGBOX); // 设置成优先级
	m_pThis->SetClippingMode(true);
	m_stBackGround = (gui::CStaticBox*)GetComponent("stBackGround");
	m_stBackGround->SetClippingMode(true);
	m_stString = (gui::CStaticBox*)GetComponent("stString"); // 第一个文字
	m_stString->SetClippingMode(true);
	m_stString->SetText("");// 初始化一下不然会崩溃
	m_stSecoundString = (gui::CStaticBox*)GetComponent("stSecoundString");	// 第二个文字
	m_stSecoundString->SetClippingMode(true);
	m_stSecoundString->SetText("");

	m_slotMouseEnter = m_pThis->SigMouseEnter().Connect(this, &CBroadCastSystem::OnMouseEnter);
	m_slotMouseLeave = m_pThis->SigMouseLeave().Connect(this, &CBroadCastSystem::OnMouseLeave);

	/*m_pnlBackGround = (gui::CPanel*)GetComponent("backPanelSystem");
	m_paStbInfoBox = (gui::CStaticBox*)GetComponent("StbInfoBox");*/

	//m_pThis->GetClientRect();
	// 等调用show的时候在设置定位
	
	// 设置强化框的位置
	LocateComponent(GetDboGlobal()->GetScreenWidth(), GetDboGlobal()->GetScreenHeight());

	GetNtlGuiManager()->AddUpdateFunc(this);

	Show(false);

	LinkMsg(g_EventBroadCastNfy, 0);
	LinkMsg(g_EventResize, 0); // 调整窗口时 重定位
	InitStringBoxFirst();
	InitStringBoxSecound();
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

		// 强化的聊天框
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

			// 设置获胜者的排行文字
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

			if (pNotify->pData.sBudokaiRecord.byMatchType == BUDOKAI_MATCH_TYPE_INDIVIDIAUL) // 个人武道会
			{
				if (pNotify->pData.sBudokaiRecord.byBudokaiType == BUDOKAI_TYPE_JUNIOR) // 判断是成人还是少年
				{
					// 设置文字
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
					// 设置文字
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
			else {// 否则就是团队武道会
				
				std::wstring CharName;

				// 循环增加玩家名称
				for (int i = 0; i < pNotify->pData.sBudokaiRecord.byMemberCount; i++) {
					CharName += pNotify->pData.sBudokaiRecord.awszMember[i];
					if (i != pNotify->pData.sBudokaiRecord.byMemberCount - 1) {
						CharName += L", ";
					}
				}

				if (pNotify->pData.sBudokaiRecord.byBudokaiType == BUDOKAI_TYPE_JUNIOR) // 判断是成人还是少年
				{
					// 设置文字
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
					// 设置文字
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
			// 设置文字
			swprintf_s(Text, 500, GetDisplayStringManager()->GetString("DST_WAGUWAGU_BROADINFO"), pNotify->pData.sWaguWagu1st.wszName, Logic_GetItemName(pNotify->pData.sWaguWagu1st.itemTblidx));

			if (m_iTextValues % 2 == 0)
				m_sTextTwo.push_back(Text);
			else
				m_sTextOne.push_back(Text);

			Show(true);
			m_iTextValues++;
		}
		else if (pNotify->MsgType == DBO_BROADCASTING_MSG_TYPE_CCBD) 
		{
			std::wstring CharName;
			// 循环增加玩家名称
			for (int i = 0; i < pNotify->pData.sCCBD.byMemberCount;i++) {
				CharName += pNotify->pData.sCCBD.awszMember[i];
				if (i != pNotify->pData.sCCBD.byMemberCount - 1) {
					CharName += L", ";
				}
			}
			// 设置文字
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
		if(m_bIsTwoTextOver || m_stString->GetScreenRect().right < m_stSecoundString->GetScreenRect().left)// 这里不至于结束后无法播放
		{
			// 第一个文字
			m_fBoxPresentPosX -= (fElapsed * (RwReal)g_nTestVal);
			m_stString->SetPosition(m_fBoxPresentPosX, 19);
			if (m_stString->GetText().empty())
				m_stString->SetText(m_sTextOne.front().c_str()); // 获取第一个元素并给第一个文本框赋值

			if (m_stString->GetScreenRect().right < m_stBackGround->GetScreenRect().right)
				m_bIsOneTextOver = true;
			else 
				m_bIsOneTextOver = false;

			if (m_stString->GetScreenRect().right < m_stBackGround->GetScreenRect().left)
			{
				InitStringBoxFirst();//初始化窗口位置
				m_sTextOne.erase(m_sTextOne.begin()); // 弹出文本
			}
		}
	}
			
	if (!m_sTextTwo.empty()) 
	{
		if(m_bIsOneTextOver || m_stSecoundString->GetScreenRect().right < m_stString->GetScreenRect().left)// 这里不至于结束后无法播放
		{
			m_fBoxPresentPosXTwo -= (fElapsed * (RwReal)g_nTestVal);
			m_stSecoundString->SetPosition(m_fBoxPresentPosXTwo, 19);
			if (m_stSecoundString->GetText().empty())
				m_stSecoundString->SetText(m_sTextTwo.front().c_str());

			if (m_stSecoundString ->GetScreenRect().right < m_stBackGround->GetScreenRect().right)
				m_bIsTwoTextOver = true;
			else
				m_bIsTwoTextOver = false;

			if (m_stSecoundString->GetScreenRect().right < m_stBackGround->GetScreenRect().left)
			{
				InitStringBoxSecound();
				m_sTextTwo.erase(m_sTextTwo.begin());
			}
		}
	}
	}

	if (m_sTextOne.empty() && m_sTextTwo.empty()) // 如果容器不为空则滚动文字
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

void CBroadCastSystem::InitStringBoxFirst()
{
	CRectangle rtScreen = m_stBackGround->GetScreenRect();
	m_bIsOneTextOver = true;
	m_fBoxPresentPosX = (RwReal)rtScreen.GetWidth() + 15;
	m_stString->Clear(); // 初始化文字
	m_stString->SetPosition(m_fBoxPresentPosX, 19); // 初始化定位
}

void CBroadCastSystem::InitStringBoxSecound()
{
	CRectangle rtScreen = m_stBackGround->GetScreenRect();
	m_bIsTwoTextOver = true;
	m_fBoxPresentPosXTwo = (RwReal)rtScreen.GetWidth() + 15;
	m_stSecoundString->Clear();
	m_stSecoundString->SetPosition(m_fBoxPresentPosX, 19);
}