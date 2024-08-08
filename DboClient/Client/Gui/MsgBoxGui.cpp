#include "precomp_dboclient.h"
#include "MsgBoxGui.h"

// core
#include "NtlDebug.h"
#include "NtlSysEvent.h"
#include "NtlStringUtil.h"

// presenation
#include "NtlPLGuiManager.h"

// simulation 
#include "NtlSLEvent.h"
#include "InputActionMap.h"

// shared
#include "NtlTeleport.h"

// dbo
#include "DboGlobal.h"
#include "DboEvent.h"
#include "DboEventGenerator.h"
#include "DboPacketGenerator.h"
#include "DialogManager.h"
#include "ChatGui.h"
#include "DisplayStringManager.h"
#include "MsgBoxManager.h"

#define MSGBOX_DISPLAYTEXT_YOFFSET	5
#define MSGBOX_HEIGHT_OFFSET	( 80 + 2 * MSGBOX_DISPLAYTEXT_YOFFSET ) 
#define MSGBOX_HEIGHT_OFFSET_FOR_INPUT		30
#define MSGBOX_DISPLAYTEXT_YPOS	( 33 + MSGBOX_DISPLAYTEXT_YOFFSET )	
#define MSGBOX_INPUT_YPOS_OFFSET_FROM_DISPLAY_BOTTOM	5	
#define MSGBOX_BTN_YPOS_OFFSET_FROM_DISPLAY_BOTTOM	( 9 + MSGBOX_DISPLAYTEXT_YOFFSET )

CMsgBoxGui::CMsgBoxGui()
{
	Init();	
}

CMsgBoxGui::CMsgBoxGui(const RwChar *pName)
:CNtlPLGui(pName)
{
	Init();
}

CMsgBoxGui::~CMsgBoxGui()
{
}

RwBool CMsgBoxGui::Create(void)
{
	NTL_FUNCTION("CMsgBoxGui::Create");

	if(!CNtlPLGui::Create("gui\\MsgBox.rsr", "gui\\MsgBox.srf", "gui\\MsgBox.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");
			
	m_pbtnOk = (gui::CButton*)GetComponent ("btnOk");
	m_pbtnCancel = (gui::CButton*)GetComponent ("btnCancel");
	m_phtmDisplay = (gui::CHtmlBox*)GetComponent ("htmlDisplay");
	m_pstbTitle = (gui::CStaticBox*)GetComponent( "stbTitle" );
	m_pInput = (gui::CInputBox*)GetComponent( "InputBox" );
	m_pInput->SetCaretSize(dINPUTBOX_CARET_WIDTH, dINPUTBOX_CARET_HEIGHT);
	
	m_slotReturn = m_pInput->SigReturnPressed().Connect(this, &CMsgBoxGui::OnReturn);
	m_slotGotFocus = m_pInput->SigGotFocus().Connect(this, &CMsgBoxGui::OnGotFocus);
	m_slotLostFocus = m_pInput->SigLostFocus().Connect(this, &CMsgBoxGui::OnLostFocus);
	m_slotOk = m_pbtnOk->SigClicked().Connect(this, &CMsgBoxGui::OnClickedOk);	
	m_slotCancel = m_pbtnCancel->SigClicked().Connect(this, &CMsgBoxGui::OnClickedCancel);	
	m_slotMove = m_pThis->SigMove().Connect( this, &CMsgBoxGui::OnMove );
	m_slotPaint = m_pThis->SigPaint().Connect( this, &CMsgBoxGui::OnPaint );

	m_rtOk = m_pbtnOk->GetPosition();
	m_rtCancel = m_pbtnCancel->GetPosition();

	m_srfBack.SetType(CWindowby3::WT_HORIZONTAL);
	m_srfBack.SetSurface(0, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "MsgBox.srf", "srfMsgBackUp" ) );
	m_srfBack.SetSurface(1, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "MsgBox.srf", "srfMsgBackMid" ) );
	m_srfBack.SetSurface(2, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "MsgBox.srf", "srfMsgBackDown" ) );

	m_srfBlackPanel.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "MsgBox.srf", "srfBlackPanel" ) );
	m_srfBlackPanel.Show( FALSE );
		
	m_pstbTitle->SetText( GetDisplayStringManager()->GetString( DST_MSG_BOX_TITLE ) );
	m_pbtnOk->SetText( GetDisplayStringManager()->GetString( DST_MSG_BTN_OK ) );
	m_pbtnCancel->SetText( GetDisplayStringManager()->GetString( DST_MSG_BTN_CANCEL ) );

	GetNtlGuiManager()->AddUpdateFunc( this );	
	
	NTL_RETURN(TRUE);
}

void CMsgBoxGui::Destroy(void)
{
	NTL_FUNCTION("CMsgBoxGui::Destroy");

	GetNtlGuiManager()->RemoveUpdateFuncWithoutRefCheck( this );
	DeleteCustomBtnAll();

	if( GetNtlGuiManager()->GetGuiManager()->GetFocus() == m_pInput )
		GetNtlGuiManager()->GetGuiManager()->SetFocus( GetNtlGuiManager()->GetGuiManager() );

	CNtlPLGui::DestroyComponents();
    CNtlPLGui::Destroy(); 

	NTL_RETURNVOID();
}

VOID CMsgBoxGui::Update( RwReal fElapsed )
{
	if( m_bHide )
	{	
		GetMsgBoxManager()->DeleteMsgBox( this );
	}
	else
	{
		if( m_fShowTime > 0.0f )
		{
			RwInt32 nTime = (RwInt32)fElapsed;
			m_fCurrentTime -= fElapsed;

			if( m_fCurrentTime <= 0.99f )
			{
				ProccessDefault();
			}
			else if( m_fShowTime > 0.0f && nTime != (RwInt32)m_fCurrentTime )
			{
				SetText();
			}		
		}	
	}
}

VOID CMsgBoxGui::HandleEvent_ItemDelete( RWS::CMsg& msg )
{
	if( m_eWorkId == MBW_ITEMDROP_CONFIRM )
	{
		SNtlEventSobItemDelete* pSobItemDelete = reinterpret_cast<SNtlEventSobItemDelete*>( msg.pData );

		if( m_Data.sItemDeleteInfo.hSerial == pSobItemDelete->hItemSerialId )
			ProccessDefault();
	}
}

VOID CMsgBoxGui::Show( bool bShow )
{
	CNtlPLGui::Show( bShow );

	if( bShow )
	{
		//GetNtlGuiManager()->AddUpdateFunc( this );	
	}
	else 
	{
		//GetNtlGuiManager()->RemoveUpdateFunc( this );

		if( GetNtlGuiManager()->GetGuiManager()->GetFocus() == m_pInput )
			GetNtlGuiManager()->GetGuiManager()->SetFocus( GetNtlGuiManager()->GetGuiManager() );
	}
}

VOID CMsgBoxGui::Popup( bool bPopup )
{
	CNtlPLGui::Popup( bPopup );

	if( bPopup )
		m_srfBlackPanel.Show( TRUE );
	else
		m_srfBlackPanel.Show( FALSE );	
}

VOID CMsgBoxGui::CenterAlign( RwUInt32 uiWidth, RwUInt32 uiHeight )
{
	CNtlPLGui::CenterAlign( uiWidth, uiHeight );
	m_srfBlackPanel.SetSize( uiWidth, uiHeight );
}

void CMsgBoxGui::SetBoxData( EMsgBoxTypeFlag flagType, RwBool bPopup, std::list<sMsgBoxCustomBtn>* plistCustomBtn, const WCHAR* szText,
							 RwInt32 eStringID, EMsgBoxWork eWorkID, sMsgBoxData* pData, RwReal fShowTime, RwBool bNetConnect /* = FALSE */)
{
	m_flagType = flagType;
	m_bPopup = bPopup;
	m_eWorkId = eWorkID;
	m_eStringID = eStringID;
	m_wstrDisplayText = szText;
	m_bNetConnect = bNetConnect;

	if( pData )
		memcpy_s( &m_Data, sizeof( sMsgBoxData ), pData, sizeof( sMsgBoxData ) );
	else
		memset( &m_Data, 0, sizeof( sMsgBoxData ) );

	// Btn Show 유무 판별.
	m_pInput->Show( false );
	m_pbtnOk->Show( false );
	m_pbtnCancel->Show( false );

	if( flagType & MBTF_CUSTOM )
	{
		std::list<sMsgBoxCustomBtn>::iterator iter;

		if( plistCustomBtn )
		{
			for( iter = plistCustomBtn->begin() ; iter != plistCustomBtn->end() ; ++iter )
			{
				sMsgBoxCustomBtn* pCustomBtn = &(*iter);
				AddCustomBtn( pCustomBtn );
			}
		}	
		else
		{
			NTL_ASSERTFAIL( "CustomData is Null" );
		}
	}

	if( flagType & MBTF_INPUT )
	{
		m_pInput->Clear();
		m_pInput->SetFocus();
		m_pInput->Show( true );
	}

	if( flagType & MBTF_OK )
	{
		m_pbtnOk->Show( true );
	}

	if( flagType & MBTF_CANCEL )
	{
		m_pbtnCancel->Show( true );
	}
	
	if( fShowTime > 0.0f )
	{
		m_fShowTime = fShowTime;
		m_fCurrentTime = fShowTime + 0.99f;		
	}	

	if( bPopup )
		m_pThis->SetPriority( dDIALOGPRIORITY_POPUP_MSGBOX );
	else
		m_pThis->SetPriority( dDIALOGPRIORITY_MSGBOX );

	RaiseTop();
	
	SetText();
	MsgBoxSizeProc( TRUE );
	ButtonPositionProc();

	Show( true );
}

VOID CMsgBoxGui::UpdateMsgBox( const WCHAR* szText, sMsgBoxData* pData )
{
	m_wstrDisplayText = szText;
	
	if( pData )
		memcpy_s( &m_Data, sizeof( sMsgBoxData ), pData, sizeof( sMsgBoxData ) );
	else
		memset( &m_Data, 0, sizeof( sMsgBoxData ) );

	RaiseTop();

	SetText();
	MsgBoxSizeProc( FALSE );
	ButtonPositionProc();

	if( !IsShow() )
		Show( true );
}

RwBool CMsgBoxGui::IsMsgBoxWorkID( EMsgBoxWork workID )
{
	if( m_eWorkId == workID )
		return TRUE;

	std::list<sMsgBoxCustomBtnData>::iterator iter;
	for( iter = m_listCustomBtn.begin() ; iter != m_listCustomBtn.end() ; ++iter )
	{
		sMsgBoxCustomBtnData* pData = &(*iter);
		if( pData->uiWorkID == (RwUInt32)workID )
			return TRUE;
	}

	return FALSE;
}

RwBool CMsgBoxGui::IsMsgBoxStringID( RwInt32 eStringID )
{
	return eStringID == m_eStringID ? TRUE : FALSE ;
}

VOID CMsgBoxGui::SendOK(VOID)
{
	OnClickedOk( NULL );
}

VOID CMsgBoxGui::SendCancel(VOID)
{
	OnClickedCancel( NULL );
}

VOID CMsgBoxGui::DoNotHide(VOID)
{
	m_bHide = FALSE;
}

VOID CMsgBoxGui::ProccessDefault()
{
	if( !m_bHide )
	{
		if( m_flagType & MBTF_CANCEL )
		{
			OnClickedCancel( NULL );
			return;
		}

		if( m_flagType & MBTF_OK )
		{
			OnClickedOk( NULL );
			return;
		}

		// Ok Cancel이 없는 경우.
		if( m_eWorkId == MBW_NONE )
			m_bHide = TRUE;
		else
			OnClickedOk( NULL );
	}	
}

VOID CMsgBoxGui::PositionAlign( RwInt32 nWidth, RwInt32 nHeight )
{
	SetPosition( (RwInt32)( nWidth * m_fPosXRate ), (RwInt32)( nHeight * m_fPosYRate ) );
	m_srfBlackPanel.SetSize( nWidth, nHeight );
}

VOID CMsgBoxGui::Init(VOID)
{
	m_pbtnOk = NULL;
	m_pbtnCancel = NULL;
	m_phtmDisplay = NULL;
	m_pstbTitle = NULL;

	m_flagType = MBTF_NONE;
	m_eWorkId = MBW_NONE;
	m_eStringID = INVALID_DWORD;

	m_fShowTime = 0.0f;
	m_fCurrentTime = 0.0f;
	m_bPopup = FALSE;

	m_bHide = FALSE;
}

VOID CMsgBoxGui::SetText(VOID)
{
	if( m_fShowTime == 0.0f )
	{
		m_phtmDisplay->SetHtmlFromMemory( m_wstrDisplayText.c_str(), m_wstrDisplayText.size() );
	}
	else
	{
		WCHAR buf[GUI_TEXT_BUFFER_SIZE] = { 0, };
		WCHAR bufTime[GUI_TEXT_BUFFER_SIZE] = { 0, };
		swprintf_s( bufTime, GUI_TEXT_BUFFER_SIZE, GetDisplayStringManager()->GetString( DST_MSG_REMAIN_TIME ), Logic_GetTimeString( m_fCurrentTime ) );
		swprintf_s( buf, GUI_TEXT_BUFFER_SIZE, L"%s%s", m_wstrDisplayText.c_str(), bufTime );
		m_phtmDisplay->SetHtmlFromMemory( buf, wcslen( buf ) );
	}
}

VOID CMsgBoxGui::MsgBoxSizeProc( RwBool bCenterAlign )
{
	CRectangle rect = m_pThis->GetPosition();
	RwInt32 nMessageHeight = 0;
	RwInt32 nMsgBoxHeight = 0;
	nMessageHeight = m_phtmDisplay->GetHeight();
	nMsgBoxHeight = nMessageHeight + MSGBOX_HEIGHT_OFFSET;
	
	if( m_flagType & MBTF_INPUT )	
	{
		nMsgBoxHeight += MSGBOX_HEIGHT_OFFSET_FOR_INPUT;		
		m_pInput->SetPosition( m_pInput->GetPosition().left, MSGBOX_DISPLAYTEXT_YPOS + nMessageHeight + MSGBOX_INPUT_YPOS_OFFSET_FROM_DISPLAY_BOTTOM + MSGBOX_DISPLAYTEXT_YOFFSET );		
	}

	rect.SetRectWH( rect.left, rect.top, rect.GetWidth(), nMsgBoxHeight );	
	SetPosition( rect );
	m_srfBack.SetSize( rect.GetWidth(), rect.GetHeight() );
	
	if( bCenterAlign )
		CenterAlign( GetDboGlobal()->GetScreenWidth(), GetDboGlobal()->GetScreenHeight() );	
}

VOID CMsgBoxGui::ButtonPositionProc(VOID)
{
	RwInt32 nButtonCount = 0;
	RwInt32 nCurrentIndex = 0;

	nButtonCount = (RwInt32)m_listCustomBtn.size();
	if( m_pbtnOk->IsVisible() )
		++nButtonCount;

	if( m_pbtnCancel->IsVisible() )
		++nButtonCount;

	//
	if( m_pbtnOk->IsVisible() )
	{
		SetButtonPosition( m_pbtnOk, nCurrentIndex, nButtonCount );
		++nCurrentIndex;
	}

	std::list<sMsgBoxCustomBtnData>::iterator iter;
	for( iter = m_listCustomBtn.begin() ; iter != m_listCustomBtn.end() ; ++iter )
	{
		gui::CComponent* pComponent = (*iter).m_pComponent;
		SetButtonPosition( pComponent, nCurrentIndex, nButtonCount );
		++nCurrentIndex;
	}

	if( m_pbtnCancel->IsVisible() )
	{
		SetButtonPosition( m_pbtnCancel, nCurrentIndex, nButtonCount );
		++nCurrentIndex;
	}
}

VOID CMsgBoxGui::SetButtonPosition( gui::CComponent* pComponent, RwInt32 nIndex, RwInt32 nTotalButtonCount )
{
	RwInt32 nWidth = GetWidth() / nTotalButtonCount;
	RwInt32 nXPos = 0;
	RwInt32 nYPos = m_phtmDisplay->GetHeight() + MSGBOX_DISPLAYTEXT_YPOS + MSGBOX_BTN_YPOS_OFFSET_FROM_DISPLAY_BOTTOM;

	if( m_flagType & MBTF_INPUT )
		nYPos += MSGBOX_HEIGHT_OFFSET_FOR_INPUT;

	nXPos = ( nWidth * nIndex ) + ( nWidth - pComponent->GetWidth() ) / 2;
	//nXPos = ( GetWidth() / ( nTotalButtonCount + 1 ) ) * ( nIndex + 1 ) - ( pComponent->GetWidth() / 2 );

	pComponent->SetPosition( nXPos, nYPos );
}

VOID CMsgBoxGui::CalcPosRate(VOID)
{
	CRectangle rect = GetPosition();
	m_fPosXRate = (RwReal)rect.left / (RwReal)GetDboGlobal()->GetScreenWidth();
	m_fPosYRate = (RwReal)rect.top / (RwReal)GetDboGlobal()->GetScreenHeight();
}

VOID CMsgBoxGui::AddCustomBtn( sMsgBoxCustomBtn* pCustomBtnData )
{
	gui::CButton* pButton = NULL;
	sMsgBoxCustomBtnData sData;
	CRectangle rect;
	rect.SetRectWH( 0, 0, m_pbtnOk->GetWidth(), m_pbtnOk->GetHeight() );

	pButton = NTL_NEW gui::CButton( rect, GetDisplayStringManager()->GetString( pCustomBtnData->uiStringID ), m_pThis, GetNtlGuiManager()->GetSurfaceManager() );
	pButton->AddSurfaceUp( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "MsgBox.srf", "srfCommonBtnUp" ) );
	pButton->AddSurfaceDown( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "MsgBox.srf", "srfCommonBtnDown" ) );
	pButton->AddSurfaceFocus( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "MsgBox.srf", "srfCommonBtnFoc" ) );	

	sData.uiStringID = pCustomBtnData->uiStringID;
	sData.uiWorkID = pCustomBtnData->uiWorkID;
	sData.m_slotClickCustomButton = pButton->SigClicked().Connect( this, &CMsgBoxGui::OnClickedCustomBtn );
	sData.m_pComponent = pButton;

	m_listCustomBtn.push_back( sData );	
}

VOID CMsgBoxGui::DeleteCustomBtnAll(VOID)
{
	std::list<sMsgBoxCustomBtnData>::iterator iter;
	for( iter = m_listCustomBtn.begin() ; iter != m_listCustomBtn.end() ; ++iter )
	{
		gui::CComponent* pComponent = (*iter).m_pComponent;
		NTL_DELETE( pComponent );
	}

	m_listCustomBtn.clear();
}

void CMsgBoxGui::OnClickedOk( gui::CComponent* pComponent )
{
	if( !m_bHide )
	{
		if( m_flagType & MBTF_INPUT )
		{		
			if( m_pInput->GetLength() == 0 )
			{
				// 아무것도 입력하지 않았습니다
				GetAlarmManager()->AlarmMessage(DST_MSG_MUST_INPUT_MESSAGE);
				return;
			}

			m_Data.pwcText = (const WCHAR*)m_pInput->GetText();
		}
		// ResultEvent에서 Hide취소를 할 수 있다. 순서 절대 바뀌면 안됨.
		m_bHide = TRUE;
		CDboEventGenerator::MsgBoxResult( TRUE, m_eWorkId, &m_Data );		
	}	
}

void CMsgBoxGui::OnClickedCancel( gui::CComponent* pComponent )
{
	if( !m_bHide )
	{
		m_bHide = TRUE;
		CDboEventGenerator::MsgBoxResult( FALSE, m_eWorkId, &m_Data );			
	}	
}

void CMsgBoxGui::OnClickedCustomBtn( gui::CComponent* pComponent )
{
	if( !m_bHide )
	{
		m_bHide = TRUE;
		std::list<sMsgBoxCustomBtnData>::iterator iter;
		for( iter = m_listCustomBtn.begin() ; iter != m_listCustomBtn.end() ; ++iter )
		{
			gui::CComponent* pCurComponent = (*iter).m_pComponent;
			if( pComponent == pCurComponent )
			{
				sMsgBoxCustomBtnData* pData = &(*iter);
				CDboEventGenerator::MsgBoxResult( TRUE, pData->uiWorkID, &m_Data );
			}		
		}
	}	
}

void  CMsgBoxGui::OnReturn()
{
	OnClickedOk(NULL);
}

void  CMsgBoxGui::OnGotFocus()
{
	GetDboGlobal()->GetGameData()->bChatMode = TRUE;
}

void  CMsgBoxGui::OnLostFocus()
{
	GetDboGlobal()->GetGameData()->bChatMode = FALSE;
}

VOID CMsgBoxGui::OnMove( RwInt32 iOldX, RwInt32 iOldY )
{
	CRectangle rtScreen = m_pThis->GetScreenRect();
	CRectangle rtRect;

	m_srfBack.SetPosition(rtScreen.left, rtScreen.top);	
	CalcPosRate();
}

VOID CMsgBoxGui::OnPaint(VOID)
{
	m_srfBack.Render();
	m_srfBlackPanel.Render();
}

VOID CMsgBoxGui::OnSetAlpha( RwUInt8 byAlpha )
{
	m_srfBack.SetAlpha( byAlpha );
}
