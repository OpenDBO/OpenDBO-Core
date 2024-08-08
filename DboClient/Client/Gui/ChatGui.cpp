#include "precomp_dboclient.h"
#include "ChatGui.h"

// core
#include "NtlDebug.h"
#include "NtlMath.h"

// presentation
#include "NtlPLGuiManager.h"
#include "NtlPLEvent.h"

// simulation
#include "InputActionMap.h"
#include "NtlSobManager.h"
#include "NtlSobAvatar.h"
#include "NtlSobPlayerAttr.h"
#include "NtlSLGlobal.h"
#include "NtlSLEvent.h"

// dbo
#include "DboDef.h"
#include "DboGlobal.h"
#include "DboEvent.h"
#include "DboEventGenerator.h"
#include "DialogManager.h"
#include "ChatOptionGui.h"
#include "ChatDisplayGui.h"
#include "ChatReceiver.h"
#include "ChatSender.h"
#include "OptionWindowGui.h"


CChatGui::CChatGui( const RwChar* pName )
: CNtlPLGui( pName )
,m_bChatModeEndReturn( FALSE )
,m_eChatType( CHAT_TYPE_GENERAL )
,m_pChatOptionGui( NULL )
,m_pChatReceiver( NULL )
,m_pSender( NULL )
,m_bRenderBackground(FALSE)
,m_bInitializedOption(FALSE)
{

}

CChatGui::~CChatGui(VOID)
{

}

RwBool CChatGui::Create(VOID)
{
	NTL_FUNCTION( "CChatGui::Create" );

	if( !CNtlPLGui::Create( "", "gui\\Chat.srf", "gui\\Chat.frm" ) )
		NTL_RETURN( FALSE );

	CNtlPLGui::CreateComponents( CNtlPLGuiManager::GetInstance()->GetGuiManager() );

	m_pThis		= (gui::CDialog*)GetComponent( "dlgMain" );
	m_pManagerDialog	= (gui::CDialog*)GetComponent( "dlgManager" );
	m_pSelBtn	= (gui::CButton*)GetComponent( "btnModeSel" );
	m_pOptionBtn= (gui::CButton*)GetComponent( "btnOption" );
	m_pIME		= (gui::CStaticBox*)GetComponent( "stbIME" );
	m_pInput	= (gui::CInputBox*)GetComponent( "inbChat" );

	m_pSelBtn->SetToolTip( std::wstring( GetDisplayStringManager()->GetString( DST_CHAT_FILTER ) ) );
	m_pOptionBtn->SetToolTip( GetDisplayStringManager()->GetString( DST_OPTION_CONTROL_ACTION_WINDOW_OPTION ) );

	m_pInput->SetCaretSize(dINPUTBOX_CARET_WIDTH, dINPUTBOX_CARET_HEIGHT);
	m_pInput->SetMaxLength(NTL_MAX_LENGTH_OF_CHAT_MESSAGE_UNICODE);

	m_pChatTypeButton[CHAT_TYPE_GENERAL]	= (gui::CButton*)GetComponent( "btnGeneral" );
	m_pChatTypeButton[CHAT_TYPE_TRADE]		= (gui::CButton*)GetComponent( "btnTrade" );
	m_pChatTypeButton[CHAT_TYPE_GUILD]		= (gui::CButton*)GetComponent( "btnGuild" );
	m_pChatTypeButton[CHAT_TYPE_PARTY]		= (gui::CButton*)GetComponent( "btnParty" );
	m_pChatTypeButton[CHAT_TYPE_WHISPER]	= (gui::CButton*)GetComponent( "btnWhisper" );
	m_pChatTypeButton[CHAT_TYPE_SHOUT]		= (gui::CButton*)GetComponent( "btnShout" );

	m_pChatTypeButton[CHAT_TYPE_GENERAL]->SetToolTip( GetDisplayStringManager()->GetString(DST_CHAT_MODE_GENERAL) );
	m_pChatTypeButton[CHAT_TYPE_TRADE]	->SetToolTip( GetDisplayStringManager()->GetString(DST_CHAT_MODE_TRADE) );
	m_pChatTypeButton[CHAT_TYPE_GUILD]	->SetToolTip( GetDisplayStringManager()->GetString(DST_CHAT_MODE_GUILD) );
	m_pChatTypeButton[CHAT_TYPE_PARTY]	->SetToolTip( GetDisplayStringManager()->GetString(DST_CHAT_MODE_PARTY) );
	m_pChatTypeButton[CHAT_TYPE_WHISPER]->SetToolTip( GetDisplayStringManager()->GetString(DST_CHAT_MODE_WHISPER) );
	m_pChatTypeButton[CHAT_TYPE_SHOUT]	->SetToolTip( GetDisplayStringManager()->GetString(DST_CHAT_MODE_SHOUT) );

	for( RwUInt8 i = 0 ; i < NUM_NET_CHAT_TYPE ; ++i )
		m_slotChatButton[i] = m_pChatTypeButton[i]->SigClicked().Connect(this, &CChatGui::OnChatTypeButton);

	m_pChatTypePanel[CHAT_TYPE_GENERAL]		= (gui::CPanel*)GetComponent( "pnlGeneral" );
	m_pChatTypePanel[CHAT_TYPE_TRADE]		= (gui::CPanel*)GetComponent( "pnlTrade" );
	m_pChatTypePanel[CHAT_TYPE_GUILD]		= (gui::CPanel*)GetComponent( "pnlGuild" );
	m_pChatTypePanel[CHAT_TYPE_PARTY]		= (gui::CPanel*)GetComponent( "pnlParty" );
	m_pChatTypePanel[CHAT_TYPE_WHISPER]		= (gui::CPanel*)GetComponent( "pnlWhisper" );
	m_pChatTypePanel[CHAT_TYPE_SHOUT]		= (gui::CPanel*)GetComponent( "pnlShout" );

	m_pChatTypePanel[CHAT_TYPE_GENERAL]	->SetToolTip( GetDisplayStringManager()->GetString(DST_CHAT_MODE_GENERAL) );
	m_pChatTypePanel[CHAT_TYPE_TRADE]	->SetToolTip( GetDisplayStringManager()->GetString(DST_CHAT_MODE_TRADE) );
	m_pChatTypePanel[CHAT_TYPE_GUILD]	->SetToolTip( GetDisplayStringManager()->GetString(DST_CHAT_MODE_GUILD) );
	m_pChatTypePanel[CHAT_TYPE_PARTY]	->SetToolTip( GetDisplayStringManager()->GetString(DST_CHAT_MODE_PARTY) );
	m_pChatTypePanel[CHAT_TYPE_WHISPER]	->SetToolTip( GetDisplayStringManager()->GetString(DST_CHAT_MODE_WHISPER) );
	m_pChatTypePanel[CHAT_TYPE_SHOUT]	->SetToolTip( GetDisplayStringManager()->GetString(DST_CHAT_MODE_SHOUT) );

	m_pThis->SetPriority(dDIALOGPRIORITY_CHATTING);
		
	m_InputBackground.SetSurface(0, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "Chat.srf", "srfInputBackLeft" ) );
	m_InputBackground.SetSurface(1, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "Chat.srf", "srfInputBackCenter" ) );
	m_InputBackground.SetSurface(2, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "Chat.srf", "srfInputBackRight" ) );
	m_InputBackground.SetSize(312, 24);

	m_pChatOptionGui = NTL_NEW CChatOptionGui("ChatOption");
	if( !m_pChatOptionGui->Create(this) )
	{
		m_pChatOptionGui->Destroy();
		NTL_DELETE(m_pChatOptionGui);
		NTL_RETURN( FALSE );
	}	

	m_pChatReceiver = NTL_NEW CChatReceiver;
	if( !m_pChatReceiver->Create() )
	{
		m_pChatReceiver->Destroy();
		NTL_DELETE(m_pChatReceiver);
		NTL_RETURN( FALSE );
	}

	m_pSender = NTL_NEW CChatSender;
	if( !m_pSender->Create(this) )
	{
		m_pSender->Destroy();
		NTL_DELETE(m_pSender);
		NTL_RETURN( FALSE );
	}

	if( !AddDisplayGui() )
		return FALSE;	

	// CallBack
	m_slotGotFocus			= m_pInput->SigGotFocus().Connect( this, &CChatGui::OnGotFocus );
	m_slotLostFocus			= m_pInput->SigLostFocus().Connect( this, &CChatGui::OnLostFocus );
	m_slotReturn			= m_pInput->SigReturnPressed().Connect( this, &CChatGui::OnInputReturn );	
	m_slotModeSelBtnClick	= m_pSelBtn->SigClicked().Connect( this, &CChatGui::OnModeSelBtnClick );
	m_slotOption			= m_pOptionBtn->SigClicked().Connect( this, &CChatGui::OnOptionButton );
	m_slotMove				= m_pThis->SigMove().Connect( this, &CChatGui::OnMove );
	m_slotResize			= m_pThis->SigResize().Connect( this, &CChatGui::OnResize );
	m_slotIMEChange			= m_pInput->SigIMEChanged().Connect( this, &CChatGui::OnIMEChange );
	m_slotESCKeyUp			= m_pInput->SigEscKeyUp().Connect( this, &CChatGui::OnESCKeyUp );
	m_slotPaint				= m_pThis->SigPaint().Connect( this, &CChatGui::OnPaint );	

	// Initial setting
	//gui::CInputBox::SetNativeMode();
	gui::CInputBox::SetNativeModeDefault();
	OnIMEChange( m_pInput->GetCurrentIMEState() );

	m_tPositionInfo.iOldScreenHeight	= 0;

	if( GetNtlWorldConcept()->GetWorldConceptController(WORLD_PLAY_TUTORIAL) )
	{
		m_bRenderBackground = TRUE;
	}
	else if( !m_ChatOption.bAlwaysInput )
	{
		ShowChatGui(false);
	}

	m_pThis->SetPosition(dDIALOG_CLEINT_EDGE_GAP, GetDboGlobal()->GetScreenHeight() - m_pThis->GetHeight() - dDIALOG_CLEINT_EDGE_GAP);


	// Event link.
	LinkMsg( g_EventChatGuiMode, 0 );
	LinkMsg( g_EventSetTextToChatInput, 0 );
	LinkMsg( g_EventPartyUpdateValue, 0 );
	LinkMsg( g_EventNotifyGuild, 0 );
	LinkMsg( g_EventGameServerChangeOut, 0 );	
	LinkMsg( g_EventGameChatOption, 0 );
	LinkMsg( g_EventChangeWorldConceptState, 0 );
	LinkMsg( g_EventSobInfoUpdate, 0 );	
	LinkMsg( g_EventResize, 0 );

	GetInputActionMap()->LinkTabKey( this, &CChatGui::ActionMapTabKey );
	GetInputActionMap()->LinkChatPageChange( this, &CChatGui::ActionMapChangeTab );
	
	NTL_RETURN( TRUE );
}

VOID CChatGui::Destroy(VOID)
{
	NTL_FUNCTION( "CChatGui::Destroy" );

	// Event unlink
	UnLinkMsg( g_EventChatGuiMode );
	UnLinkMsg( g_EventSetTextToChatInput );
	UnLinkMsg( g_EventPartyUpdateValue );
	UnLinkMsg( g_EventNotifyGuild );
	UnLinkMsg( g_EventGameServerChangeOut );
	UnLinkMsg( g_EventGameChatOption );
	UnLinkMsg( g_EventChangeWorldConceptState );
	UnLinkMsg( g_EventSobInfoUpdate );
	UnLinkMsg( g_EventResize );	

	GetInputActionMap()->UnLinkTabKey();
	GetInputActionMap()->UnLinkChatPageChane();

	if( m_pSender )
	{
		m_pSender->Destroy();
		NTL_DELETE( m_pSender );
	}

	if( m_pChatReceiver )
	{
		m_pChatReceiver->Destroy();
		NTL_DELETE( m_pChatReceiver );
	}

	if( m_pChatOptionGui )
	{
		m_pChatOptionGui->Destroy();
		NTL_DELETE( m_pChatOptionGui );
	}

	for each( CChatDisplayGui* pDisplayGui in m_listDisplayGui )
	{
		pDisplayGui->Destroy();
		NTL_DELETE( pDisplayGui );
	}
	m_listDisplayGui.clear();

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy(); 

	NTL_RETURNVOID();
}

RwInt32 CChatGui::ActionMapTabKey(VOID)
{
	CNtlSobAvatar* pSobAvatar = GetNtlSLGlobal()->GetSobAvatar(); 
	if( !pSobAvatar )
	{
		DBO_FAIL("Not exist avatar instance");
		return 1;
	}

	eChatType eType = m_eChatType;

	while( TRUE )
	{
		eType = (eChatType)(eType + 1);

		if( eType == NUM_NET_CHAT_TYPE )
			eType = CHAT_TYPE_GENERAL;

		if( eType == CHAT_TYPE_TRADE )
		{
		
		}
		else if( eType == CHAT_TYPE_PARTY )
		{
			if( pSobAvatar->GetParty()->IsHaveGroup() )
				break;
		}
		else if( eType == CHAT_TYPE_GUILD )
		{
			if( pSobAvatar->GetGuild()->IsHaveGroup() )
				break;
		}
		else if( eType == CHAT_TYPE_WHISPER )
		{
			if( GetLastRecieveWhisperName().empty() )
			{
				GetAlarmManager()->AlarmMessage(DST_CHAT_HAVE_NO_USER_TO_REPLY);
				break;
			}
		}		
		else
		{
			break;
		}
	}

	SetChatType( eType );
	return 1;
}

RwInt32 CChatGui::ActionMapChangeTab(RwUInt32 uiOption)
{
	for each( CChatDisplayGui* pDisplayGui in m_listDisplayGui )
	{
		if( uiOption )
			pDisplayGui->PageDown();			
		else
			pDisplayGui->PageUp();
	}

	return 1;
}

RwBool CChatGui::AddDisplayGui(VOID)
{
	CChatDisplayGui* pDisplayGui = NTL_NEW CChatDisplayGui("ChatDisplay");
	if( !pDisplayGui->Create(this) )
	{
		pDisplayGui->Destroy();
		NTL_DELETE(pDisplayGui);
		return FALSE;
	}

	RwInt8 byIndex = (RwInt8)m_listDisplayGui.size();

	pDisplayGui->SetIndex(byIndex);
	pDisplayGui->Lock(m_ChatOption.bLockChatDisplay);
	pDisplayGui->SetAlphaWeight(m_ChatOption.byAlpha);
	
	m_pChatReceiver->RegisterNotify(pDisplayGui);
	m_listDisplayGui.push_back(pDisplayGui);
	return TRUE;
}

VOID CChatGui::RemoveDisplayGui(RwUInt8 byDisplayIndex)
{
	LIST_DISPLAY_GUI::iterator it_displayGui = m_listDisplayGui.begin();
	for( ; it_displayGui != m_listDisplayGui.end() ; ++it_displayGui )
	{
		CChatDisplayGui* pDisplayGui = *it_displayGui;

		if( pDisplayGui->GetIndex() == byDisplayIndex )
		{
			m_pChatReceiver->UnregisterNotify(pDisplayGui);
			pDisplayGui->Destroy();
			NTL_DELETE(pDisplayGui);
			m_listDisplayGui.erase(it_displayGui);
			break;
		}
	}

	RwInt8 byCount = 0;
	it_displayGui = m_listDisplayGui.begin();
	for each( CChatDisplayGui* pDisplayGui in m_listDisplayGui )
	{
		pDisplayGui->SetIndex(byCount);
		++byCount;
	}
}

VOID CChatGui::SetDismovableDisplayPositioin(RwBool bForce /* = FALSE */)
{
	CRectangle rtScreen = m_pThis->GetScreenRect();
	RwInt32 iHeight = rtScreen.top;
	for each( CChatDisplayGui* pDisplayGui in m_listDisplayGui )
	{
		// 첫 번째 디스플레이창은 항상 붙어다닌다
		iHeight -= pDisplayGui->GetHeight();
		pDisplayGui->SetPosition(rtScreen.left, iHeight);

		if( !bForce )
			return;
	}
}

VOID CChatGui::ShowChatGui(bool bShow)
{
	CNtlWorldConceptController* pWorldConcept = GetNtlWorldConcept()->GetWorldConceptController(WORLD_PLAY_TUTORIAL);
	if( pWorldConcept )
	{
		if( pWorldConcept->GetState() != WORLD_STATE_EXIT )
			return;
	}

	m_bRenderBackground = bShow;
	m_pManagerDialog->Show(bShow);
}

RwBool CChatGui::IsFocused(VOID)
{
	if( GetNtlGuiManager()->GetGuiManager()->GetFocus() == m_pInput )
		return TRUE;
	else 
		return FALSE;
}

VOID CChatGui::FocusToInput(VOID)
{
	m_pInput->SetFocus();
}

VOID CChatGui::ShowChatButton(eChatType eType, bool bShow)
{
	if( eType >= NUM_NET_CHAT_TYPE )
		return;

	m_pChatTypeButton[eType]->Show(bShow);

	if( bShow )
	{
		if( eType == m_eChatType )
			m_pChatTypePanel[eType]->Show(true);
	}
	else
	{
		m_pChatTypePanel[eType]->Show(false);
	}

	SetComponentPosition();	
}

VOID CChatGui::SetChatType( eChatType eType )
{
	if( eType >= NUM_NET_CHAT_TYPE )
		return;

	// avooo's comment : 거래 채팅에 대한 기획이 명확하지 않다
	if( eType == CHAT_TYPE_TRADE )
		eType = CHAT_TYPE_GENERAL;

	if( eType == CHAT_TYPE_WHISPER )
	{
		if( GetLastRecieveWhisperName().empty() )
			return;

		m_pChatOptionGui->EnableCatting(CHAT_TYPE_WHISPER, true);
	}
	else
	{
		m_pChatOptionGui->EnableCatting(CHAT_TYPE_WHISPER, false);
	}

	
	RwUInt32 uiColor;

	m_eChatType = eType;

	switch( m_eChatType )
	{
	case CHAT_TYPE_GENERAL:		uiColor = CHATGUI_COLOR_GLOBAL;		break;
	case CHAT_TYPE_WHISPER:		uiColor = CHATGUI_COLOR_WHISPER;	break;
	case CHAT_TYPE_PARTY:		uiColor = CHATGUI_COLOR_PARTY;		break;
	case CHAT_TYPE_GUILD:		uiColor = CHATGUI_COLOR_GUILD;		break;
	case CHAT_TYPE_TRADE:		uiColor = CHATGUI_COLOR_GLOBAL;		break;
	case CHAT_TYPE_SHOUT:		uiColor = CHATGUI_COLOR_SHOUT;		break;
	default:
		{
			uiColor = CHATGUI_COLOR_GLOBAL;
			DBO_FAIL("Invalid chat type");
			break;
		}
	}
	

	std::wstring strText = m_pInput->GetText();
	m_pInput->Clear();
	m_pInput->SetTextColor( uiColor );
	m_pInput->SetText( strText.c_str() );

	for( RwUInt8 i = 0 ; i < NUM_NET_CHAT_TYPE ; ++i )
	{
		m_pChatTypePanel[i]->Show(false);
	}

	if( m_pChatTypeButton[m_eChatType]->IsVisible() )
		m_pChatTypePanel[m_eChatType]->Show(true);
}

eChatType CChatGui::GetChatType()
{
	return m_eChatType;
}

VOID CChatGui::SetLastRecieveWhisperName(WCHAR* pwcName)
{
	if( !pwcName )
		return;

	if( wcscmp(Logic_GetAvatarName(), pwcName) == 0 )
		return;

	m_wstrLastRecieveWhisterName = pwcName;
}

VOID CChatGui::SetComponentPosition()
{
	CRectangle rect = m_pOptionBtn->GetPosition();
	RwInt32 iX = rect.right;

	for( RwInt8 i = 0 ; i < NUM_NET_CHAT_TYPE ; ++i )
	{
		if( m_pChatTypeButton[i]->IsVisible() )
		{
			m_pChatTypeButton[i]->SetPosition(iX, rect.top);
			m_pChatTypePanel[i]->SetPosition(iX, rect.top);
			iX += m_pChatTypeButton[i]->GetWidth();
		}
	}

	iX -= 2;
	rect = m_pIME->GetPosition();
	m_pIME->SetPosition(iX, rect.top);

	iX = m_pIME->GetPosition().right - 2;
	rect = m_pInput->GetPosition();
	rect.left = iX;
	rect.right = GetWidth() - 3;
	m_pInput->SetPosition(rect);
}

VOID CChatGui::Option_AlwaysInput(bool bEnable)
{
	if( m_bInitializedOption )
	{
		if( B2b(m_ChatOption.bAlwaysInput) == bEnable )
			return;
	}

	m_ChatOption.bAlwaysInput = bEnable;

	CNtlWorldConceptController* pWorldConcept = GetNtlWorldConcept()->GetWorldConceptController(WORLD_PLAY_TUTORIAL);
	if( pWorldConcept )
	{
		if( pWorldConcept->GetState() != WORLD_STATE_EXIT )
			return;
	}

	if( m_ChatOption.bAlwaysInput )
	{
		ShowChatGui(true);
	}
	else if( GetDboGlobal()->GetGameData()->bChatMode == FALSE )
	{
		ShowChatGui(false);
	}
}

VOID CChatGui::Option_LockChatDisplay(bool bEnable)
{
	if( m_bInitializedOption )
	{
		if( m_ChatOption.bLockChatDisplay == bEnable )
			return;
	}

	m_ChatOption.bLockChatDisplay = bEnable;

	for each(CChatDisplayGui* pDisplayGui in m_listDisplayGui)
		pDisplayGui->Lock(m_ChatOption.bLockChatDisplay);
}

VOID CChatGui::Option_MoreDisplayGui()
{
	if( m_listDisplayGui.size() == 2 )
		return;

	AddDisplayGui();
	SetDismovableDisplayPositioin(TRUE);
}

VOID CChatGui::Option_LessDisplayGui()
{
	if( m_listDisplayGui.size() == 1 )
		return;

	LIST_DISPLAY_GUI::reverse_iterator rit_displayGui = m_listDisplayGui.rbegin();
	CChatDisplayGui* pDisplayGui = *rit_displayGui;

	RemoveDisplayGui(pDisplayGui->GetIndex());

	if( GetDialogManager()->IsDialogMovable() == FALSE )
		SetDismovableDisplayPositioin();
}

VOID CChatGui::Option_Alpha(RwUInt8 byAlpha)
{
	if( m_bInitializedOption )
	{
		if( m_ChatOption.byAlpha == byAlpha )
			return;
	}

	m_ChatOption.byAlpha = byAlpha;

	for each(CChatDisplayGui* pDisplayGui in m_listDisplayGui)
		pDisplayGui->SetAlphaWeight(m_ChatOption.byAlpha);
}

VOID CChatGui::Option_DisplayChatLogType(RwUInt8 byDisplayIndex, eChatType eType, RwBool bEnable)
{
	for each( CChatDisplayGui* pDisplayGui in m_listDisplayGui )
	{
		if( pDisplayGui->GetIndex() == byDisplayIndex )
		{
			pDisplayGui->EnableLogType(eType, bEnable);
			break;
		}
	}
}

VOID CChatGui::Option_RefreshDisplayChatLog(RwUInt8 byDisplayIndex)
{
	for each( CChatDisplayGui* pDisplayGui in m_listDisplayGui )
	{
		if( pDisplayGui->GetIndex() == byDisplayIndex )
		{
			pDisplayGui->Refresh();
			break;
		}
	}

	m_bInitializedOption = TRUE;
}

VOID CChatGui::OnGotFocus(VOID)
{
	GetDboGlobal()->GetGameData()->bChatMode = TRUE;
	ShowChatGui(true);

	if( GetChatType() == CHAT_TYPE_WHISPER && m_pInput->GetLength() == 0 )
	{
		WCHAR awcBuffer[64] = L"";
		std::wstring& WhisperName = GetLastRecieveWhisperName();

		swprintf_s(awcBuffer, 64, L"%s ", WhisperName.c_str());
		m_pInput->SetText( awcBuffer );
	}
}

VOID CChatGui::OnLostFocus(VOID)
{
	GetDboGlobal()->GetGameData()->bChatMode = FALSE;	
}

VOID CChatGui::OnInputReturn(VOID)
{
	if( m_pInput->GetLength() )
	{
		m_pSender->InputText( m_pInput->GetText() );
		m_pInput->Clear();

		if( m_ChatOption.bAlwaysInput )
		{
			if( GetChatType() == CHAT_TYPE_WHISPER )
			{
				WCHAR awcBuffer[64] = L"";
				std::wstring& WhisperName = GetLastRecieveWhisperName();

				swprintf_s(awcBuffer, 64, L"%s ", WhisperName.c_str());
				m_pInput->SetText( awcBuffer );
			}
		}
		else
		{
			GetNtlGuiManager()->GetGuiManager()->SetFocus( GetNtlGuiManager()->GetGuiManager() );
			m_bChatModeEndReturn = TRUE;
			ShowChatGui(false);
		}
	}
	else
	{
		GetNtlGuiManager()->GetGuiManager()->SetFocus( GetNtlGuiManager()->GetGuiManager() );
		m_bChatModeEndReturn = TRUE;		

		if( !m_ChatOption.bAlwaysInput )
			ShowChatGui(false);
	}
}

VOID CChatGui::OnModeSelBtnClick( gui::CComponent* pComponent )
{
	if( m_pChatOptionGui->IsShow() )
	{
		m_pChatOptionGui->SwitchDialog(false);
	}
	else
	{
		m_pChatOptionGui->SwitchDialog(true);
	}
}

VOID CChatGui::OnOptionButton( gui::CComponent* pComponent )
{
	if( GetDialogManager()->IsOpenDialog(DIALOG_OPTIONWND) )
	{
		if( !Logic_CanMouseInput_in_Tutorial( ETL_MOUSE_INPUT_TYPE_MAINMENU_OPTION_CLOSE ) )
			return;

		GetDialogManager()->CloseDialog( DIALOG_OPTIONWND );
	}
	else
	{
		if( !Logic_CanMouseInput_in_Tutorial( ETL_MOUSE_INPUT_TYPE_MAINMENU_OPTION_OPEN ) )
			return;

		COptionWindowGui* pOptionGui = reinterpret_cast<COptionWindowGui*>( GetDialogManager()->GetDialog(DIALOG_OPTIONWND) );
		if( !pOptionGui )
			return;

		GetDialogManager()->OpenDialog( DIALOG_OPTIONWND );
		pOptionGui->SelectOptionTab( COptionWindowGui::GAME_CHATTING );
	}
}

VOID CChatGui::OnChatTypeButton( gui::CComponent* pComponent )
{
	for( RwInt8 i = 0 ; i < NUM_NET_CHAT_TYPE ; ++i )
	{
		if( m_pChatTypeButton[i] == pComponent )
		{
			SetChatType((eChatType)i);
			break;
		}
	}
}

VOID CChatGui::OnMove( RwInt32 nOldX, RwInt32 nOldY )
{
	CRectangle rtScreen = m_pThis->GetScreenRect();	

	SetDismovableDisplayPositioin();	

	m_InputBackground.SetRect(rtScreen);
	m_pChatOptionGui->SetPosition(rtScreen.left, rtScreen.top - m_pChatOptionGui->GetHeight());

	SetComponentPosition();

	m_tPositionInfo.iOldScreenHeight = GetDboGlobal()->GetScreenHeight();
}

VOID CChatGui::OnResize( RwInt32 iOldW, RwInt32 iOldH )
{
	CRectangle rtScreen = m_pThis->GetScreenRect();
	m_pManagerDialog->SetWidth( rtScreen.GetWidth() );

	m_InputBackground.SetRect(rtScreen);
	m_pChatOptionGui->SetPosition(rtScreen.left, rtScreen.top - m_pChatOptionGui->GetHeight());

	SetComponentPosition();
}

VOID CChatGui::OnIMEChange( INT eIMEState )
{
	if( eIMEState == 0 )
	{
		m_pIME->SetText( GetDisplayStringManager()->GetString( DST_CHAT_IME_ENG ) );
	}
	else
	{
		m_pIME->SetText( GetDisplayStringManager()->GetString( DST_CHAT_IME_KOR ) );
	}
}

VOID CChatGui::OnESCKeyUp(VOID)
{
	GetNtlGuiManager()->GetGuiManager()->SetFocus( GetNtlGuiManager()->GetGuiManager() );
	m_bChatModeEndReturn = TRUE;

	// dialog manager 을 일회 cancel process disable setting
	CDboEventGenerator::DialogEvent(DIALOGEVENT_INPUTEDITBOX_LOST_FOCUS_WITH_ESC_KEY);
}

VOID CChatGui::OnPaint(VOID)
{
	if( m_bRenderBackground )
		m_InputBackground.Render();
}

RwInt32 CChatGui::SwitchDialog( bool bOpen )
{
	Show( bOpen );

	for each( CChatDisplayGui* pDisplayGui in m_listDisplayGui )
		pDisplayGui->Show( bOpen );

	if( !bOpen )
	{
		eDialogMode eMode = GetDialogManager()->GetMode();

		if( eMode == DIALOGMODE_CHATTING_RESIZE_HORI || eMode == DIALOGMODE_CHATTING_RESIZE_RIGHTUP ||
			eMode == DIALOGMODE_CHATTING_RESIZE_VERT || eMode == DIALOGMODE_CHATTING_RESIZE_RIGHTDOWN )
			GetDialogManager()->OffMode();

		m_pChatOptionGui->SwitchDialog(false);
	}

	return 1;
}

VOID CChatGui::HandleEvents( RWS::CMsg& msg )
{
	if( msg.Id == g_EventChatGuiMode )
	{
		if( m_bChatModeEndReturn )
		{
			// EventChatGuiMode는 GUI키보드처리 루틴을 거치고 들어오기 때문에
			// 이미 포커스가 바뀌어 또 다시 인풋으로 들어가게 되는 걸 방지
			// m_bChatModeEndReturn == TRUE 이면 이미 포커스는 해제된 상태.
			m_bChatModeEndReturn = FALSE;	
		}
		else
		{
			SDboEventChatGuiMode* pData = reinterpret_cast<SDboEventChatGuiMode*>( msg.pData );

			if( pData->bMode )
			{
				if( GetDialogManager()->IsMode( DIALOGMODE_NARRATION ) )
					return;

				if( GetNtlGuiManager()->GetGuiManager()->GetFocus() != m_pInput )
					m_pInput->SetFocus();			
			}
			else
			{
				if( GetNtlGuiManager()->GetGuiManager()->GetFocus() == m_pInput )
					GetNtlGuiManager()->GetGuiManager()->SetFocus( GetNtlGuiManager()->GetGuiManager() );
			}			 
		}
	}
	else if( msg.Id == g_EventSetTextToChatInput )
	{
		CDboEventGenerator::ChatGuiMode(TRUE);
		m_pInput->SetText( (WCHAR*)msg.pData );		
	}
	else if( msg.Id == g_EventPartyUpdateValue )
	{
		SNtlEventPartyUpdate* pEvent = reinterpret_cast<SNtlEventPartyUpdate*>(msg.pData);

		if( pEvent->iMessage == PMT_PARTY_LEAVE )
		{
			if( m_eChatType == CHAT_TYPE_PARTY )
				SetChatType(CHAT_TYPE_GENERAL);
		}
	}
	else if( msg.Id == g_EventNotifyGuild )
	{
		SNtlEventNotifyGuild* pEvent = reinterpret_cast<SNtlEventNotifyGuild*>( msg.pData );

		if( pEvent->iMessage == SLGE_DISBAND )
		{
			if( m_eChatType == CHAT_TYPE_GUILD )
				SetChatType(CHAT_TYPE_GENERAL);
		}
	}
	else if( msg.Id == g_EventGameServerChangeOut )
	{
		m_pSender->UnregAllSocialAction();

		if( m_eChatType == CHAT_TYPE_PARTY ||
			m_eChatType == CHAT_TYPE_GUILD )
			SetChatType(CHAT_TYPE_GENERAL);
	}
	else if( msg.Id == g_EventGameChatOption )
	{
		SNtlEventGameChatOption* pEvent = reinterpret_cast<SNtlEventGameChatOption*>( msg.pData );

		switch(pEvent->byData1)
		{
		case OPTION_CHAT_INPUTMODE:
			{
				Option_AlwaysInput(B2b(pEvent->byData2));
				break;
			}
		case OPTION_CHAT_WINDOWLOCK:
			{
				Option_LockChatDisplay(B2b(pEvent->byData2));
				break;
			}
		case OPTION_CHAT_ADDEDWINDOW:
			{
				if( pEvent->byData2 )
					Option_MoreDisplayGui();
				else
					Option_LessDisplayGui();
				break;
			}
		case OPTION_CHAT_WINDOWOPACITY:
			{
				Option_Alpha(pEvent->byData2);
				break;
			}
		case OPTION_CHAT_NORMAL:
			{
				Option_DisplayChatLogType(pEvent->byData2, CHAT_TYPE_GENERAL, pEvent->byData3);
				break;
			}
		case OPTION_CHAT_WISPHER:
			{
				Option_DisplayChatLogType(pEvent->byData2, CHAT_TYPE_WHISPER, pEvent->byData3);
				break;
			}
		case OPTION_CHAT_PARTY:
			{
				Option_DisplayChatLogType(pEvent->byData2, CHAT_TYPE_PARTY, pEvent->byData3);
				break;
			}
		case OPTION_CHAT_GUILD:
			{
				Option_DisplayChatLogType(pEvent->byData2, CHAT_TYPE_GUILD, pEvent->byData3);
				break;
			}
		case OPTION_CHAT_TRADE:
			{
				Option_DisplayChatLogType(pEvent->byData2, CHAT_TYPE_TRADE, pEvent->byData3);
				break;
			}
		case OPTION_CHAT_SHOUT:
			{
				Option_DisplayChatLogType(pEvent->byData2, CHAT_TYPE_SHOUT, pEvent->byData3);
				break;
			}
		case OPTION_CHAT_SYSTEM:
			{
				Option_DisplayChatLogType(pEvent->byData2, CHAT_TYPE_SYSTEM, pEvent->byData3);
				Option_DisplayChatLogType(pEvent->byData2, CHAT_TYPE_NOTIFY, pEvent->byData3);
				break;
			}
		case OPTION_CHAT_REFRESH:
			{
				for( RwUInt8 i = 0 ; i < m_listDisplayGui.size() ; ++i )
					Option_RefreshDisplayChatLog(i);

				break;
			}
		}
	}
	else if( msg.Id == g_EventChangeWorldConceptState )
	{
		SNtlEventWorldConceptState* pEvent = reinterpret_cast<SNtlEventWorldConceptState*>( msg.pData );

		if( pEvent->eWorldConcept == WORLD_PLAY_TUTORIAL )
		{
			if( pEvent->uiState == WORLD_STATE_EXIT )
			{
				ShowChatGui(B2b(m_ChatOption.bAlwaysInput));
			}
		}
	}
	else if( msg.Id == g_EventSobInfoUpdate )
	{
		if( Logic_IsUpdateType(EVENT_AIUT_SKILL, msg.pData) == FALSE )
			NTL_RETURNVOID();

		CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
		if( !pAvatar )
			NTL_RETURNVOID();

		CNtlSkillContainer* pSkillContainer = pAvatar->GetSkillContainer();
		CNtlSobActionSkill* pSobActionSkill = NULL;

		for( RwInt32 i = 0 ; i < NTL_MAX_PC_HAVE_ACTION_SKILL ; ++i )
		{
			pSobActionSkill = pSkillContainer->GetActionSkill(i);
			if( pSobActionSkill )
			{
				m_pSender->RegSocialAction(pSobActionSkill);
			}			
		}
	}
	else if( msg.Id == g_EventResize )
	{
		RwInt32 iNewX, iNewY;
		CRectangle rtScreen = m_pThis->GetScreenRect();
		RwInt32 iCleintWidth = GetDboGlobal()->GetScreenWidth();
		RwInt32 iCleintHeight = GetDboGlobal()->GetScreenHeight();

		if( abs(rtScreen.left) < 100 )
		{
			iNewX = dDIALOG_CLEINT_EDGE_GAP;
		}
		else
		{
			if( rtScreen.right >= iCleintWidth )
			{
				iNewX = iCleintWidth - 150;
			}
			else
			{
				iNewX = rtScreen.left;
			}
		}

		iNewY = rtScreen.top + (iCleintHeight - m_tPositionInfo.iOldScreenHeight);
		
		if( iNewX != rtScreen.left || iNewY != rtScreen.top )
		{
			m_pThis->SetPosition(iNewX, iNewY);
		}
	}
}