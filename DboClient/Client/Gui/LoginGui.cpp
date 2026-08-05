#include "precomp_dboclient.h"
#include "LoginGui.h"

// shared
#include "NtlResultCode.h"

// core
#include "NtlDebug.h"
#include "NtlEventQueue.h"
#include "NtlSystemInfo.h"

// sound 
#include "NtlSoundEventGenerator.h"

// presentation
#include "NtlPLGuiManager.h"
#include "NtlPLEvent.h"

// Simulation
#include "InputActionMap.h"
#include "NtlStorageManager.h"

// dbo
#include "DboDef.h"
#include "DboGlobal.h"
#include "DboEvent.h"
#include "DboEventGenerator.h"
#include "AlarmManager.h"
#include "LogInStageState.h"

#include "DboApplication.h"
#include "MoviePlayer.h"
#include "DialogDefine.h"

#include "VirtualKeyboardGui.h"
#include "BugTrap.h"

CLogInGui::CLogInGui(const RwChar* pName)
	:CNtlPLGui(pName)
{
}

CLogInGui::~CLogInGui()
{
}

RwBool CLogInGui::Create()
{
	NTL_FUNCTION("CLogInGui::Create");

	if (GetDboGlobal()->GetDBOUIConfig()->GetLoginTerrain()->bIsEnable)
	{
		if (!CNtlPLGui::Create("gui\\Login.rsr", "gui\\Login.srf", "gui\\LoginWorld.frm"))
			NTL_RETURN(FALSE);
	}
	else
	{
		if (!CNtlPLGui::Create("gui\\Login.rsr", "gui\\Login.srf", "gui\\Login.frm"))
			NTL_RETURN(FALSE);
	}

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager()); // this lags a bit because of flaCredit

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");

	CRectangle rect;

	m_pFrame = (gui::CFrame*)GetComponent("frmParent");

	// flash背景
	m_pFlashBackground = (gui::CFlash*)GetComponent("flaBackground");

	m_pFlashCredit = (gui::CFlash*)GetComponent("flaCredit");
	m_pFlashCredit->Show(false);


	// moive电影按钮
	m_pCinemaButton = (gui::CButton*)GetComponent("CinemaButton");
	m_pCinemaButton->SetTextFont(DEFAULT_FONT, 105, DEFAULT_FONT_ATTR);
	m_pCinemaButton->SetTextFocusColor(INFOCOLOR_LOBBY_FOC);
	m_pCinemaButton->SetTextDownColor(INFOCOLOR_LOBBY_DOWN);
	m_pCinemaButton->SetText(GetDisplayStringManager()->GetString("DST_LOGIN_PLAY_MOVIE"));
	m_slotCinemaButton = m_pCinemaButton->SigClicked().Connect(this, &CLogInGui::ClickedCinemaButton);

	// 制作组按钮
	m_pCreditButton = (gui::CButton*)GetComponent("CreditButton");
	m_pCreditButton->SetTextFont(DEFAULT_FONT, 105, DEFAULT_FONT_ATTR);
	m_pCreditButton->SetTextFocusColor(INFOCOLOR_LOBBY_FOC);
	m_pCreditButton->SetTextDownColor(INFOCOLOR_LOBBY_DOWN);
	m_pCreditButton->SetText(GetDisplayStringManager()->GetString("DST_LOGIN_CREDIT"));
	m_slotCreditButton = m_pCreditButton->SigClicked().Connect(this, &CLogInGui::ClickedCreditButton);

	// 游戏政策按钮
	m_pGamePolicyButton = (gui::CButton*)GetComponent("GamePolicyButton");
	m_pGamePolicyButton->SetTextFont(DEFAULT_FONT, 105, DEFAULT_FONT_ATTR);
	m_pGamePolicyButton->SetTextFocusColor(INFOCOLOR_LOBBY_FOC);
	m_pGamePolicyButton->SetTextDownColor(INFOCOLOR_LOBBY_DOWN);
	m_pGamePolicyButton->SetText(GetDisplayStringManager()->GetString("DST_LOGIN_CONTRACT"));
	m_slotGamePolicyButton = m_pGamePolicyButton->SigClicked().Connect(this, &CLogInGui::ClickedGamePolicyButton);

	// 创建新账号按钮
	m_pNewAccountButton = (gui::CButton*)GetComponent("NewAccountButton");
	m_pNewAccountButton->SetTextFont(DEFAULT_FONT, 105, DEFAULT_FONT_ATTR);
	m_pNewAccountButton->SetTextFocusColor(INFOCOLOR_LOBBY_FOC);
	m_pNewAccountButton->SetTextDownColor(INFOCOLOR_LOBBY_DOWN);
	m_pNewAccountButton->SetText(GetDisplayStringManager()->GetString("DST_LOGIN_NEW_ACCOUNT"));
	m_slotNewAccountButton = m_pNewAccountButton->SigClicked().Connect(this, &CLogInGui::ClickedNewAccountButton);

	// 登录按钮
	m_pLoginButton = (gui::CButton*)GetComponent("LoginButton");
	m_pLoginButton->SetTextFont(DEFAULT_FONT, 105, DEFAULT_FONT_ATTR);
	m_pLoginButton->SetTextFocusColor(INFOCOLOR_LOBBY_FOC);
	m_pLoginButton->SetTextDownColor(INFOCOLOR_LOBBY_DOWN);
	m_pLoginButton->SetText(GetDisplayStringManager()->GetString("DST_LOGIN"));
	m_slotLoginButton = m_pLoginButton->SigClicked().Connect(this, &CLogInGui::ClickedLoginButton);

	// 退出按钮
	m_pExitButton = (gui::CButton*)GetComponent("ExitButton");
	m_pExitButton->SetTextFont(DEFAULT_FONT, 105, DEFAULT_FONT_ATTR);
	m_pExitButton->SetTextFocusColor(INFOCOLOR_LOBBY_FOC);
	m_pExitButton->SetTextDownColor(INFOCOLOR_LOBBY_DOWN);
	m_pExitButton->SetText(GetDisplayStringManager()->GetString("DST_LOBBY_EXIT"));
	m_slotExitButton = m_pExitButton->SigClicked().Connect(this, &CLogInGui::ClickedExitButton);

	// 虚拟键盘按钮
	m_pVirtualKeyButton = (gui::CButton*)GetComponent("VirtualKeyButton");
	m_slotVirtualKeyButton = m_pVirtualKeyButton->SigClicked().Connect(this, &CLogInGui::ClickedVirtualKeyButton);
	m_pVirtualKeyButton->Enable(false);

	if (GetDboGlobal()->GetDBOUIConfig()->GetLoginTerrain()->bIsEnable)
	{
		m_srfTitle.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Login.srf", "srfTitle"));
	}

	// Input box 背景
	m_srfInputDialogBack.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Login.srf", "srfTWInputDialogBack"));

	// 未知 下划线?
	m_NoticeUnderLine.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Login.srf", "underLine"));

	// 账号ID文本
	rect.SetRectWH(417, 588, 60, 30);
	m_pAccountName = NTL_NEW gui::CStaticBox(rect, m_pFrame, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_LEFT);
	m_pAccountName->CreateFontStd(DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pAccountName->SetText(GetDisplayStringManager()->GetString("DST_LOGIN_ID"));
	m_pAccountName->Enable(false);

	// 密码文本
	rect.SetRectWH(417, 616, 60, 30);
	m_pPassward = NTL_NEW gui::CStaticBox(rect, m_pFrame, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_LEFT);
	m_pPassward->CreateFontStd(DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pPassward->SetText(GetDisplayStringManager()->GetString("DST_LOGIN_PASSWARD"));
	m_pPassward->Enable(false);

	// 提示/通知文本
	rect.SetRectWH(773, 436, 220, 30);
	m_pNotive = NTL_NEW gui::CStaticBox(rect, m_pFrame, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_CENTER);
	m_pNotive->CreateFontStd(DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pNotive->SetTextColor(RGB(255, 255, 255));
	m_pNotive->Enable(false);

	// Username Input
	m_pAccountInput = (gui::CInputBox*)GetComponent("AccountInput");
	m_pAccountInput->SetCaretSize(dINPUTBOX_CARET_WIDTH, dINPUTBOX_CARET_HEIGHT);
	m_pAccountInput->SetMaxLength(NTL_MAX_SIZE_USERID);
	m_slotEnterAccount = m_pAccountInput->SigReturnPressed().Connect(this, &CLogInGui::PressEnder_in_AccountBox);
	m_slotAccountInputGotFocus = m_pAccountInput->SigGotFocus().Connect(this, &CLogInGui::OnAccountInput_GotFocus);

	// Password Input
	m_pPasswardInput = (gui::CInputBox*)GetComponent("PasswardInput");
	m_pPasswardInput->SetCaretSize(dINPUTBOX_CARET_WIDTH, dINPUTBOX_CARET_HEIGHT);
	m_pPasswardInput->SetMaxLength(NTL_MAX_SIZE_USERPW);
	m_pPasswardInput->SetPasswordMode(TRUE);
	m_slotEnterPassward = m_pPasswardInput->SigReturnPressed().Connect(this, &CLogInGui::PressEnder_in_PasswarsBox);
	m_slotPasswarsInputGotFocus = m_pPasswardInput->SigGotFocus().Connect(this, &CLogInGui::OnPasswarsInput_GotFocus);


	m_pVirtualKeyboard = NTL_NEW CVirtualKeyboardGui("VirtualKeyboardGui");
	if (!m_pVirtualKeyboard->Create()) // 如果创建ui成功
	{
		if (m_pVirtualKeyboard)
		{
			m_pVirtualKeyboard->Destroy();
			NTL_DELETE(m_pVirtualKeyboard);
			m_pVirtualKeyboard = NULL;
		}
	}

	// Locate Component
	LocateComponent(GetDboGlobal()->GetScreenWidth(), GetDboGlobal()->GetScreenHeight());

	// sig
	m_slotMove = m_pThis->SigMove().Connect(this, &CLogInGui::OnMove);
	m_slotPaint = m_pFlashBackground->SigPaint().Connect(this, &CLogInGui::OnPaint);
	m_creditPaint = m_pFlashCredit->SigMovieEnd().Connect(this, &CLogInGui::OnCreditPaintEnd);


	GetNtlGuiManager()->AddUpdateFunc(this);

	// Input Handle
	GetInputActionMap()->SetActive(FALSE);
	GetInputActionMap()->LinkTabKey(this, &CLogInGui::TabButtonHandle);

	m_handleKeyDown = CInputHandler::GetInstance()->LinkKeyDown(this, &CLogInGui::KeyboardDownHandler);

	// event 殿废.
	LinkMsg(g_EventLoginGuiEnable, 0);
	LinkMsg(g_EventLogInStageStateEnter, 0);
	LinkMsg(g_EventLogInStageStateExit, 0);
	LinkMsg(g_EventLogInStageTimeOut, 0);
	LinkMsg(g_EventResize, 0);
	LinkMsg(g_EventLoginSuccess, 0);
	LinkMsg(g_EventVirtualKeyboard, 0);

	Show(false);

	NTL_RETURN(TRUE);
}


VOID CLogInGui::Destroy()
{
	NTL_FUNCTION("CLogInGui::Destroy");

	m_pVirtualKeyboard->Destroy();

	NTL_DELETE(m_pAccountName);
	NTL_DELETE(m_pPassward);
	NTL_DELETE(m_pNotive);
	NTL_DELETE(m_pVirtualKeyboard);

	m_pFlashBackground->Unload();
	m_pFlashCredit->Unload();

	GetNtlGuiManager()->RemoveUpdateFunc(this);

	GetInputActionMap()->SetActive(TRUE);
	GetInputActionMap()->UnLinkTabKey();

	CInputHandler::GetInstance()->UnLinkKeyDown(m_handleKeyDown);

	UnLinkMsg(g_EventLoginGuiEnable);
	UnLinkMsg(g_EventLogInStageStateEnter);
	UnLinkMsg(g_EventLogInStageStateExit);
	UnLinkMsg(g_EventLogInStageTimeOut);
	UnLinkMsg(g_EventResize);
	UnLinkMsg(g_EventLoginSuccess);
	UnLinkMsg(g_EventVirtualKeyboard);

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();

	NTL_RETURNVOID();
}

VOID CLogInGui::Update(RwReal fElapsed)
{
	if (IsShow() == FALSE)
		return;

	if (m_pFlashCredit->IsVisible())
		m_pFlashCredit->Update(fElapsed);
	else
		m_pFlashBackground->Update(fElapsed);
}

VOID CLogInGui::LocateComponent(RwInt32 iWidth, RwInt32 iHeight)
{
	CRectangle rect;

	rect.left = 0;
	rect.top = 0;
	rect.right = iWidth;
	rect.bottom = iHeight;

	m_pThis->SetPosition(rect);

	if (GetDboGlobal()->GetDBOUIConfig()->GetLoginTerrain()->bIsEnable)
		m_srfTitle.SetPosition((rect.GetWidth() - m_srfTitle.GetWidth()) / 2, (rect.GetHeight() - m_srfTitle.GetHeight()) / 2 - 270);


	m_pFrame->SetPosition(rect);
	m_pFlashBackground->SetPosition(rect);
	m_pFlashCredit->SetPosition(rect);

	rect = m_pCinemaButton->GetPosition();

	m_pCinemaButton->SetPosition(rect.left, iHeight - 196);
	m_pCreditButton->SetPosition(rect.left, iHeight - 156);
	m_pGamePolicyButton->SetPosition(rect.left, iHeight - 116);
	m_pNewAccountButton->SetPosition(rect.left, iHeight - 70);

	m_pExitButton->SetPosition(iWidth - 176, iHeight - 116);

	m_srfInputDialogBack.SetPosition(iWidth / 2 - m_srfInputDialogBack.GetWidth() / 2, iHeight - 300);
	m_srfInputDialogBack.GetRect(rect);

	if (m_pVirtualKeyboard != NULL)
		m_pVirtualKeyboard->GetDialog()->SetPosition(rect.left, rect.top + 130);

	m_pVirtualKeyButton->SetPosition(rect.left + 39, rect.top + 86);

	m_pLoginButton->SetPosition(rect.left + 132, rect.top + 86);

	m_pAccountName->SetPosition(rect.left + 40, rect.top + 20);
	m_pPassward->SetPosition(rect.left + 40, rect.top + 49);

	m_pAccountInput->SetPosition(rect.left + 139, rect.top + 30);
	m_pPasswardInput->SetPosition(rect.left + 139, rect.top + 58);


	m_pNotive->SetPosition(iWidth - 251, iHeight - 332);
	m_NoticeUnderLine.SetPosition(iWidth - 251, iHeight - 311);



	if (IsShow())
	{
		if (GetNtlGuiManager()->GetGuiManager()->GetFocus() == m_pAccountInput)
		{
			// 如果您再次将焦点放在同一个组件上，它将返回并编写快捷方式  
			//m_pPasswardInput->SetFocus();
			m_pAccountInput->SetFocus();
		}
		else if (GetNtlGuiManager()->GetGuiManager()->GetFocus() == m_pPasswardInput)
		{
			//m_pAccountInput->SetFocus();
			m_pPasswardInput->SetFocus();
		}
	}
}

VOID CLogInGui::SwitchEnableButtons(bool bEnable)
{

}
// 点击观看视频按钮
VOID CLogInGui::ClickedCinemaButton(gui::CComponent* pComponent)
{
	// 观看视频

	if (GetMoviePlayer()->Play(dOPENING_MOVIE, MOVIE_SIZE_RATIO_FULL))
	{
		Logic_SetOptionMoviePlay();
		GetMoviePlayer()->SetCallBack(this, &CLogInGui::CallBackMoviePlayStop);
		CNtlSoundEventGenerator::SendEventMusic(EVENT_MUSIC_IMMEDIATELY_END_MAIN_THEME);
	}
}

int CLogInGui::CallBackMoviePlayStop()
{
	Logic_SetOptionMovieStop();
	CNtlSoundEventGenerator::SendEventMusic(EVENT_MUSIC_START_MAIN_THEME);
	return TRUE;
}

VOID CLogInGui::ClickedCreditButton(gui::CComponent* pComponent)
{
	m_pFlashCredit->Load("DBO_Credit.swf");

	m_pFlashCredit->RestartMovie();
	m_pFlashCredit->Show(true);

	m_pFrame->Show(false);

	if (m_pVirtualKeyboard)
		m_pVirtualKeyboard->Show(false);
}

VOID CLogInGui::ClickedGamePolicyButton(gui::CComponent* pComponent)
{
	// 显示游戏政策
	CDboEventGenerator::LoginEvent(LOGIN_EVENT_SHOW_CONTRACT_PAGE, true);

}

VOID CLogInGui::ClickedNewAccountButton(gui::CComponent* pComponent)
{
	//新建账号弹出注册官网
	ShellExecute(NULL, _T("open"), _T("explorer.exe"), Ntl_WC2MB(GetDisplayStringManager()->GetString("DST_PAY_REGISTER")), NULL, SW_SHOW);
}

// 点击登录按钮
VOID CLogInGui::ClickedLoginButton(gui::CComponent* pComponent)
{
	// Sign in

	if (m_pAccountInput->GetLength() == 0)
	{
		GetAlarmManager()->AlarmMessage("DST_INPUT_YOUR_ID");
		return;
	}

	if (m_pPasswardInput->GetLength() == 0)
	{
		GetAlarmManager()->AlarmMessage("DST_INPUT_YOUR_PASSWORD");
		return;
	}

	SwitchEnableButtons(false);

	// Do not erase it (Fluorite)
	//请求登录到服务器
	const WCHAR* pID = m_pAccountInput->GetText();
	// TODO: We need to use hash md5 encryption here to send directly to the server encrypted.
	const WCHAR* pPW = m_pPasswardInput->GetText();
	SUserData* pUserData = GetDboGlobal()->GetUserData();
	wcscpy_s(pUserData->wchUserID, NTL_MAX_SIZE_USERID + 1, pID);
	wcscpy_s(pUserData->wchPassword, NTL_MAX_SIZE_USERPW + 1, pPW);

	const BYTE* pMacAddress = GetSystemMacAddress();

	if (pMacAddress)
	{

		if (GetDboGlobal()->GetContractVersion() == (RwUInt32)GetNtlStorageManager()->GetIntData(dSTORAGE_SYSTEM_ETC_CONTRACT)) //检查版本
			GetDboGlobal()->GetLoginPacketGenerator()->SendLonInReq(pID, pPW, CLIENT_LVER, CLIENT_RVER, pMacAddress);
	}
	else
	{
		GetAlarmManager()->AlarmMessage(L"读取MAC地址失败", CAlarmManager::ALARM_TYPE_LOBBY_MSGBOX);
	}
}

VOID CLogInGui::ClickedExitButton(gui::CComponent* pComponent)
{
	// Leave game
	PostQuitMessage(0);
}

VOID CLogInGui::PressEnder_in_AccountBox()
{
	if (!IsShow())
		return;

	// When you press Enter on the account input, the focus moves to the password input
	m_pPasswardInput->SetFocus();
}

VOID CLogInGui::PressEnder_in_PasswarsBox()
{
	if (!IsShow())
		return;

	// Enter in the password input is the same as pressing the login button
	ClickedLoginButton(NULL);
}

VOID CLogInGui::OnPasswarsInput_GotFocus()
{
	if (m_pVirtualKeyboard)
	{
		m_pVirtualKeyButton->Enable(true);
		m_pVirtualKeyboard->GetDialog()->Show(true);
	}
}


VOID CLogInGui::OnAccountInput_GotFocus()
{
	if (m_pVirtualKeyboard)
	{
		m_pVirtualKeyboard->GetDialog()->Show(false);
		m_pVirtualKeyButton->Enable(false);
	}
}

VOID CLogInGui::ClickedVirtualKeyButton(gui::CComponent* pComponent)
{
	if (m_pVirtualKeyboard)
	{
		if (m_pVirtualKeyboard->GetDialog()->IsVisible())
			m_pVirtualKeyboard->GetDialog()->Show(false);
		else
			m_pVirtualKeyboard->GetDialog()->Show(true);
	}
}

RwInt32 CLogInGui::TabButtonHandle()
{
	if (!IsShow())
		return 1;

	if (GetNtlGuiManager()->GetGuiManager()->GetFocus() == m_pAccountInput)
	{
		m_pPasswardInput->SetFocus();
	}
	else if (GetNtlGuiManager()->GetGuiManager()->GetFocus() == m_pPasswardInput)
	{
		m_pAccountInput->SetFocus();
	}

	return 1;
}

VOID CLogInGui::OnMove(RwInt32 iOldX, RwInt32 iOldY)
{

}

VOID CLogInGui::OnPaint()
{
	m_srfInputDialogBack.Render();
	m_NoticeUnderLine.Render();
	m_srfTitle.Render();
}

VOID CLogInGui::OnCreditPaintEnd(gui::CComponent* pComponent)
{
	m_pFlashCredit->Show(false);
	m_pFrame->Show(true);
}

VOID CLogInGui::HandleEvents(RWS::CMsg& msg)
{
	NTL_FUNCTION("CLogInGui::HandleEvents");

	if (msg.Id == g_EventLoginGuiEnable)
	{
		SwitchEnableButtons(true);
	}
	else if (msg.Id == g_EventLogInStageStateEnter)
	{
		LogInStageEnterEventHandler(msg);
	}
	else if (msg.Id == g_EventLogInStageStateExit)
	{
		LogInStageExitEventHandler(msg);
	}
	else if (msg.Id == g_EventLogInStageTimeOut)
	{
		LogInStageTimeOutEventHandler(msg);
	}
	else if (msg.Id == g_EventResize)
	{
		ResizeEventHandler(msg);
	}
	else if (msg.Id == g_EventLoginSuccess)
	{
		// event 关闭登录按钮模板
		CDboEventGenerator::LoginGuiEnable();
	}
	else if (msg.Id == g_EventVirtualKeyboard)
	{
		SDboEventKeyboard* Data = reinterpret_cast<SDboEventKeyboard*>(msg.pData);

		if (Data->key == "51") // 删除一个
		{
			std::string text = ws2s(m_pPasswardInput->GetText());
			text.pop_back();
			m_pPasswardInput->SetText(text);
		}
		else if (Data->key == "52") // 全部删除
		{
			m_pPasswardInput->SetText("");
		}
		else
		{
			std::string text = ws2s(m_pPasswardInput->GetText());
			text += Data->key;
			m_pPasswardInput->SetText(text);
		}

	}

	NTL_RETURNVOID();
}

VOID CLogInGui::LogInStageEnterEventHandler(RWS::CMsg& msg)
{
	SDboEventLogInStageStateEnter* pStageStateEnter = reinterpret_cast<SDboEventLogInStageStateEnter*>(msg.pData);

	ELogInStageState eState = (ELogInStageState)pStageStateEnter->byState;
	switch (eState)
	{
	case LOGIN_STATE_LOGO:
		m_pLoginButton->ClickEnable(false);
		break;
	case LOGIN_STATE_SERVER_CONNECT:
		CNtlSoundEventGenerator::SendEventMusic(EVENT_MUSIC_START_MAIN_THEME);
		m_pLoginButton->ClickEnable(false);
		m_pAccountInput->SetFocus();
		Show(true);
		break;
	case LOGIN_STATE_SERVER_CONNECT_FAIL:
		GetAlarmManager()->AlarmMessage("DST_CHAR_SERVER_CONNECT_FAIL");
		break;
	case LOGIN_STATE_IDLE:
		m_pLoginButton->ClickEnable(true);
		break;
	case LOGIN_STATE_CONTRACT:
	case LOGIN_STATE_LOGINREQ:
	case LOGIN_STATE_CHAR_SERVER_CONNECT:
	case LOGIN_STATE_CHAR_SERVER_LOGIN:
		m_pLoginButton->ClickEnable(false);
		break;
	}
}

VOID CLogInGui::LogInStageExitEventHandler(RWS::CMsg& msg)
{
	SDboEventLogInStageStateExit* pStageStateExit = reinterpret_cast<SDboEventLogInStageStateExit*>(msg.pData);

	ELogInStageState eState = (ELogInStageState)pStageStateExit->byState;
	//DBO_WARNING_MESSAGE("State: " << eState);
	switch (eState)
	{
	case LOGIN_STATE_LOGO:

		m_pLoginButton->ClickEnable(true);
		break;
	case LOGIN_STATE_SERVER_CONNECT:
		m_pLoginButton->ClickEnable(true);
		CNtlSoundEventGenerator::SendEventMusic(EVENT_MUSIC_START_MAIN_THEME);
		break;
	case LOGIN_STATE_SERVER_CONNECT_FAIL:
		break;
	case LOGIN_STATE_IDLE:
		break;
	case LOGIN_STATE_LOGINREQ:
		m_pLoginButton->ClickEnable(true);
		break;
	case LOGIN_STATE_CHAR_SERVER_CONNECT:
		m_pLoginButton->ClickEnable(true);
		break;
	case LOGIN_STATE_CHAR_SERVER_LOGIN:
		break;
	}
}

VOID CLogInGui::LogInStageTimeOutEventHandler(RWS::CMsg& msg)
{
	SDboEventLogInStageTimeOut* pStageTimeOut = reinterpret_cast<SDboEventLogInStageTimeOut*>(msg.pData);

	ELogInStageState eState = (ELogInStageState)pStageTimeOut->byState;
	switch (eState)
	{
	case LOGIN_STATE_LOGO:
		break;
	case LOGIN_STATE_SERVER_CONNECT:
		break;
	case LOGIN_STATE_SERVER_CONNECT_FAIL:
		break;
	case LOGIN_STATE_IDLE:
		break;
	case LOGIN_STATE_LOGINREQ:
		break;
	case LOGIN_STATE_CHAR_SERVER_CONNECT:
		break;
	case LOGIN_STATE_CHAR_SERVER_LOGIN:
		break;
	}
}

VOID CLogInGui::ResizeEventHandler(RWS::CMsg& msg)
{
	SNtlPLEventResize* pPacket = reinterpret_cast<SNtlPLEventResize*>(msg.pData);
	LocateComponent(pPacket->iWidht, pPacket->iHeight);
}

int CLogInGui::KeyboardDownHandler(unsigned int uiKeyData)
{
	SKeyData* pData = (SKeyData*)uiKeyData;

	if (pData->uiChar == NTL_KEY_ESCAPE)
	{
		if (m_pFlashCredit->IsVisible())
		{
			m_pFlashCredit->Unload();
			m_pFlashCredit->Show(false);
			m_pFrame->Show(true);
		}
	}

	return TRUE;
}

