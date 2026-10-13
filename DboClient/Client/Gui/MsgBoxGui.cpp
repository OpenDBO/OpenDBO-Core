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
#include "InfoWndManager.h"


namespace
{
	// dojo war boxes whose custom buttons are plain ok / cancel buttons
	bool IsDojoScrambleMsgBox(const std::string& strStringID)
	{
		return strStringID == "DST_DOJO_SCRAMBLE_ASK_CHALLENGE" ||
			strStringID == "DST_DOJO_SCRAMBLE_ACCEPT_CHALLENGE" ||
			strStringID == "DST_DOJO_SCRAMBLE_ASK_TELEPORT";
	}

	// custom cancel buttons are DST_MSG_BTN_CANCEL or named ..._CANCEL_BTN
	bool IsCancelButton(const std::string& strButton)
	{
		static const std::string strCancelSuffix = "_CANCEL_BTN";

		if (strButton == "DST_MSG_BTN_CANCEL")
			return true;

		return strButton.size() > strCancelSuffix.size() &&
			strButton.compare(strButton.size() - strCancelSuffix.size(), strCancelSuffix.size(), strCancelSuffix) == 0;
	}
}

#define MSGBOX_DISPLAYTEXT_YOFFSET	5
#define MSGBOX_HEIGHT_OFFSET	( 80 + 2 * MSGBOX_DISPLAYTEXT_YOFFSET )
#define MSGBOX_HEIGHT_OFFSET_FOR_INPUT		30
#define MSGBOX_DISPLAYTEXT_YPOS	( 33 + MSGBOX_DISPLAYTEXT_YOFFSET )
#define MSGBOX_INPUT_YPOS_OFFSET_FROM_DISPLAY_BOTTOM	5
#define MSGBOX_BTN_YPOS_OFFSET_FROM_DISPLAY_BOTTOM	( 9 + MSGBOX_DISPLAYTEXT_YOFFSET )
#define MSGBOX_HEIGHT_OFFSET_FOR_WAGU		55

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

RwBool CMsgBoxGui::Create(const char* StringID)
{
	NTL_FUNCTION("CMsgBoxGui::Create");

	if(!CNtlPLGui::Create("gui\\MsgBox.rsr", "gui\\MsgBox.srf", "gui\\MsgBox.frm"))
		NTL_RETURN(FALSE);

	m_strStringID = StringID;

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

	m_srfSlotBack.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("MsgBox.srf", "srfSlotBack"));
	m_srfSlotBack.Show(FALSE);
		
	m_pstbTitle->SetText( GetDisplayStringManager()->GetString( "DST_MSG_BOX_TITLE" ) );
	m_pbtnOk->SetText( GetDisplayStringManager()->GetString( "DST_MSG_BTN_OK" ) );
	m_pbtnCancel->SetText( GetDisplayStringManager()->GetString( "DST_MSG_BTN_CANCEL" ) );

	CRectangle rect;
	CRectangle rect2;
	int x = 5;
	int y = 60;
	for (int i = 0 ; i < dMSG_BOX_SLOTS_MAX; i++)
	{
		if ( i % 5 == 0)
		{
			y += 50;
			x = 5;
		}
		rect.SetRectWH(x += 50, y, 36, 36);
		m_pItemIcon[i] = NTL_NEW gui::CPanel(rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager());
		m_pItemIcon[i]->AddSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("MsgBox.srf", "srfSlotItem"));
		slotMouseEnterItem[i] = m_pItemIcon[i]->SigMouseEnter().Connect(this, &CMsgBoxGui::OnMouseEnterWaguItem);
		slotMouseLeaveItem[i] = m_pItemIcon[i]->SigMouseLeave().Connect(this, &CMsgBoxGui::OnMouseLeaveWaguItem);
		m_pItemIcon[i]->Show(false);
		m_pItemIcon[i]->Enable(false);

		rect2.SetRectWH(rect.left + 3 ,rect.top,34,34);
		m_pItemCount[i] = NTL_NEW gui::CStaticBox(rect2, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), DBOGUI_STACKNUM_ALIGN);
		m_pItemCount[i]->CreateFontStd(DBOGUI_STACKNUM_FONT, DBOGUI_STACKNUM_FONTHEIGHT, DBOGUI_STACKNUM_FONTATTR);
		m_pItemCount[i]->SetEffectMode(DBOGUI_STACKNUM_FONTEFFECTMODE);
		m_pItemCount[i]->SetText(0);
		m_pItemCount[i]->Show(false);
		m_pItemCount[i]->Enable(false);

		m_slotWaguPaint[i] = m_pItemIcon[i]->SigPaint().Connect(this, &CMsgBoxGui::OnWaguPaint);
	}

	CRectangle rect1;
	rect1.SetRectWH(0, 0, 32, 32);
	m_apflaEffect = NTL_NEW gui::CFlash(rect1, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), "Skill_action.swf");
	m_apflaEffect->Show(false);
	m_apflaEffect->Enable(false);

	GetNtlGuiManager()->AddUpdateFunc( this );

	NTL_RETURN(TRUE);
}

VOID CMsgBoxGui::Destroy(VOID)
{
	NTL_FUNCTION("CMsgBoxGui::Destroy");

	GetNtlGuiManager()->RemoveUpdateFuncWithoutRefCheck( this );
	DeleteCustomBtnAll();

	if( GetNtlGuiManager()->GetGuiManager()->GetFocus() == m_pInput )
		GetNtlGuiManager()->GetGuiManager()->SetFocus( GetNtlGuiManager()->GetGuiManager() );

	for (int i = 0; i < dMSG_BOX_SLOTS_MAX; i++)
	{
		NTL_DELETE(m_pItemIcon[i]);
		m_pItemIcon[i] = NULL;

		ItemSlot[i].Destroy();
	}

	NTL_DELETE(m_apflaEffect);
	m_apflaEffect = NULL;

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
		if (m_apflaEffect->IsVisible())
		{
			m_apflaEffect->Update(fElapsed);
		}
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

void CMsgBoxGui::SetBoxData( RwBool bPopup, RwBool bHasInput, std::list<sMsgBoxCustomBtn>* plistCustomBtn, const WCHAR* szText,
	const char* strStringID, sMsgBoxData* pData, RwReal fShowTime)
{
	m_bPopup = bPopup;
	m_strStringID = strStringID;
	m_wstrDisplayText = szText;
	memset(&m_Data, 0, sizeof(sMsgBoxData));

	if( pData )
		memcpy_s( &m_Data, sizeof( sMsgBoxData ), pData, sizeof( sMsgBoxData ) );

	// Btn Show 유무 판별.
	m_pInput->Show( false );
	m_pbtnOk->Show( false );
	m_pbtnCancel->Show( false );


	if( plistCustomBtn )
	{
		std::list<sMsgBoxCustomBtn>::iterator iter;
		for( iter = plistCustomBtn->begin() ; iter != plistCustomBtn->end() ; ++iter )
		{
			sMsgBoxCustomBtn* pCustomBtn = &(*iter);
			
			//DBO_WARNING_MESSAGE("has button: " << pCustomBtn->bHasButton << " name: " << pCustomBtn->strMessage);

			if (pCustomBtn->bHasButton)
			{
				if (pCustomBtn->strMessage == "DST_MSG_BTN_CANCEL")
					m_pbtnCancel->Show(true);
				else if (pCustomBtn->strMessage == "DST_MSG_BTN_OK")
					m_pbtnOk->Show(true);
				else
				{
					AddCustomBtn(pCustomBtn);
				}

				if (m_strStringID == "DST_WAGU_EXCUTE_RESULT" || m_strStringID == "DST_WAGU_EXCUTE_RESULT_MORE_SET")
				{
					for (int i = 0; i < dMSG_BOX_SLOTS_MAX; i++)
					{
						m_pItemCount[i]->Show(false);

						if (i < m_Data.sWaguInfo.byReallyExtractCount)
						{
							m_pItemIcon[i]->Show(true);
							m_pItemIcon[i]->Enable(true);
						}
						else
						{
							m_pItemIcon[i]->Show(false);
							m_pItemIcon[i]->Enable(false);
						}

						ItemSlot[i].Destroy();
					}

					for (int i = 0; i < m_Data.sWaguInfo.byReallyExtractCount; i++)
					{
						ItemSlot[i].Create(m_pItemIcon[i], DIALOG_HLSHOP, REGULAR_SLOT_ITEM_TABLE, SDS_COUNT);
						ItemSlot[i].SetSize(NTL_ITEM_ICON_SIZE);
						ItemSlot[i].SetPosition_fromParent(2, 2);
						ItemSlot[i].SetParentPosition(m_pItemIcon[i]->GetScreenRect().left, m_pItemIcon[i]->GetScreenRect().top);
						ItemSlot[i].SetIcon(m_Data.sWaguInfo.ItemTblidx[i],0);

						if (m_Data.sWaguInfo.byStackCount[i] > 1)
						{
							m_pItemCount[i]->Show(true);
							m_pItemCount[i]->SetText(m_Data.sWaguInfo.byStackCount[i]);
							m_pItemCount[i]->Raise();
						}

						if (m_Data.sWaguInfo.byRanking[i] == 1)
						{
							CRectangle rect;
							rect.SetRectWH(m_pItemIcon[i]->GetScreenRect().left + 1, m_pItemIcon[i]->GetScreenRect().top + 1, 32, 32);
							m_apflaEffect->SetPosition(rect);
							m_apflaEffect->PlayMovie(TRUE);
							m_apflaEffect->Show(true);
							m_apflaEffect->Raise();
						}
					}
				}
			}
		}
	}

	if(bHasInput)
	{
		m_pInput->Clear();
		m_pInput->SetFocus();
		m_pInput->Show( true );
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

	// a single item won gets its own dialog: center it under the now-final
	// message text instead of leaving it in the old top-left grid slot
	if (m_Data.sWaguInfo.byReallyExtractCount == 1 && m_pItemIcon[0]->IsVisible())
	{
		CRectangle rect = m_pItemIcon[0]->GetPosition();
		RwInt32 nCenteredX = (GetWidth() - rect.GetWidth()) / 2;
		RwInt32 nY = MSGBOX_DISPLAYTEXT_YPOS + m_phtmDisplay->GetHeight() + 10;
		m_pItemIcon[0]->SetPosition(nCenteredX, nY);
		m_pItemCount[0]->SetPosition(nCenteredX + 3, nY);

		ItemSlot[0].SetParentPosition(m_pItemIcon[0]->GetScreenRect().left, m_pItemIcon[0]->GetScreenRect().top);

		if (m_Data.sWaguInfo.byRanking[0] == 1)
		{
			CRectangle rectFx;
			rectFx.SetRectWH(m_pItemIcon[0]->GetScreenRect().left + 1, m_pItemIcon[0]->GetScreenRect().top + 1, 32, 32);
			m_apflaEffect->SetPosition(rectFx);
		}
	}

	MsgBoxSizeProc( TRUE );
	ButtonPositionProc();

	Show( true );
}

VOID CMsgBoxGui::UpdateMsgBox( const WCHAR* szText, sMsgBoxData* pData )
{
	m_wstrDisplayText = szText;

	memset(&m_Data, 0, sizeof(sMsgBoxData));
	
	if( pData )
		memcpy_s( &m_Data, sizeof( sMsgBoxData ), pData, sizeof( sMsgBoxData ) );

	RaiseTop();

	SetText();
	MsgBoxSizeProc( FALSE );
	ButtonPositionProc();

	if( !IsShow() )
		Show( true );
}

RwBool CMsgBoxGui::IsMsgBoxStringID(const char* strStringID )
{
	return strStringID == m_strStringID ? TRUE : FALSE ;
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
		if (m_pbtnCancel->IsVisible())
		{
			OnClickedCancel( NULL );
			return;
		}

		if (m_pbtnOk->IsVisible())
		{
			OnClickedOk( NULL );
			return;
		}

		// Ok Cancel이 없는 경우.
		if(m_pbtnCancel->IsVisible() == false && m_pbtnOk->IsVisible() == false)
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

	m_strStringID = "";

	m_fShowTime = 0.0f;
	m_fCurrentTime = 0.0f;
	m_bPopup = FALSE;
	m_apflaEffect = NULL;

	m_bHide = FALSE;

	memset(&m_Data, NULL, sizeof(sMsgBoxData));
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
		swprintf_s( bufTime, GUI_TEXT_BUFFER_SIZE, GetDisplayStringManager()->GetString( "DST_MSG_REMAIN_TIME" ), Logic_GetTimeString( m_fCurrentTime ) );
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
	
	if(m_pInput->IsVisible())
	{
		nMsgBoxHeight += MSGBOX_HEIGHT_OFFSET_FOR_INPUT;
		m_pInput->SetPosition( m_pInput->GetPosition().left, MSGBOX_DISPLAYTEXT_YPOS + nMessageHeight + MSGBOX_INPUT_YPOS_OFFSET_FROM_DISPLAY_BOTTOM + MSGBOX_DISPLAYTEXT_YOFFSET );
	}
	if (m_pItemIcon[0]->IsVisible())
		nMsgBoxHeight += MSGBOX_HEIGHT_OFFSET_FOR_WAGU;

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

	if(m_pInput->IsVisible())
		nYPos += MSGBOX_HEIGHT_OFFSET_FOR_INPUT;

	if (m_pItemIcon[0]->IsVisible())
		nYPos += MSGBOX_HEIGHT_OFFSET_FOR_WAGU;

	//nXPos = ( nWidth * nIndex ) + ( nWidth - pComponent->GetWidth() ) / 2;// disabled by daneos
	nXPos = ( GetWidth() / ( nTotalButtonCount + 1 ) ) * ( nIndex + 1 ) - ( pComponent->GetWidth() / 2 ); // enabled by daneos

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

	pButton = NTL_NEW gui::CButton( rect, GetDisplayStringManager()->GetString( pCustomBtnData->strMessage ), m_pThis, GetNtlGuiManager()->GetSurfaceManager() );
	pButton->AddSurfaceUp( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "MsgBox.srf", "srfCommonBtnUp" ) );
	pButton->AddSurfaceDown( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "MsgBox.srf", "srfCommonBtnDown" ) );
	pButton->AddSurfaceFocus( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "MsgBox.srf", "srfCommonBtnFoc" ) );	

	sData.strMessage = pCustomBtnData->strMessage;
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
		m_Data.pwcText = NULL;

		if(m_pInput->IsVisible())
		{		
			if( m_pInput->GetLength() == 0 )
			{
				// I have not entered anything.
				GetAlarmManager()->AlarmMessage("DST_MSG_MUST_INPUT_MESSAGE");
				return;
			}

			m_Data.pwcText = (const WCHAR*)m_pInput->GetText();
		}

		if (m_pItemIcon[0]->IsVisible())
		{
			m_apflaEffect->Show(false);
			m_apflaEffect->Enable(false);
			m_apflaEffect->PlayMovie(false);
		}

		m_bHide = TRUE;
		CDboEventGenerator::MsgBoxResult(MBR_OK, m_strStringID.c_str(), &m_Data );
	}
}

void CMsgBoxGui::OnClickedCancel( gui::CComponent* pComponent )
{
	if( !m_bHide )
	{
		m_bHide = TRUE;

		CDboEventGenerator::MsgBoxResult(MBR_CANCEL, m_strStringID.c_str(), &m_Data);
	}	
}

void CMsgBoxGui::OnClickedCustomBtn( gui::CComponent* pComponent )
{
	if( !m_bHide )
	{
		std::list<sMsgBoxCustomBtnData>::iterator iter;
		for( iter = m_listCustomBtn.begin() ; iter != m_listCustomBtn.end() ; ++iter )
		{
			gui::CComponent* pCurComponent = (*iter).m_pComponent;
			if( pComponent == pCurComponent )
			{
				sMsgBoxCustomBtnData* pData = &(*iter);

				m_Data.pwcText = Ntl_MB2WC(pData->strMessage.c_str());
			//	DBO_WARNING_MESSAGE("pData->strMessage: " << pData->strMessage.c_str());

				EMsgBoxResult eResult = MBR_OK;

				if (IsDojoScrambleMsgBox(m_strStringID))
				{
					// dojo war boxes only use their custom buttons as ok / cancel
					m_bHide = TRUE;

					if (IsCancelButton(pData->strMessage))
						eResult = MBR_CANCEL;
				}
				else if (m_strStringID == "DST_COMMERCIAL_MB_USE_TERM_LIMETED_ITEM_MSG")
					m_bHide = TRUE;
				else if (m_strStringID == "DST_TELEPORT_COMMON_CONFIRM_MSG")
					m_bHide = TRUE;
				else if (m_strStringID == "DST_TELEPORT_FINAL_CONFIRM_MSG")
					m_bHide = TRUE;
				else if (m_strStringID == "DST_COMMERCIAL_MB_DELETE_TERM_LIMETED_ITEM_MSG")
					m_bHide = TRUE;
				else if (m_strStringID == "DST_COMMERCIAL_MB_UPDATE_TERM_LIMETED_ITEM_MSG")
					m_bHide = TRUE;

				CDboEventGenerator::MsgBoxResult(eResult, m_strStringID.c_str(), &m_Data );

				Ntl_CleanUpHeapStringW(m_Data.pwcText); // clean memory

				break;
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

	if (m_pItemIcon[0]->IsVisible())
	{
		for (int i = 0; i < dMSG_BOX_SLOTS_MAX; i++)
		{
			ItemSlot[i].SetParentPosition(m_pItemIcon[i]->GetScreenRect().left, m_pItemIcon[i]->GetScreenRect().top);
		}
	}

	m_srfBack.SetPosition(rtScreen.left, rtScreen.top);
	CalcPosRate();
}

VOID CMsgBoxGui::OnPaint(VOID)
{
	m_srfBack.Render();
	m_srfBlackPanel.Render();
}

VOID CMsgBoxGui::OnWaguPaint(VOID)
{
	for (int i = 0 ; i < dMSG_BOX_SLOTS_MAX; i++)
	{
		ItemSlot[i].Paint();
	}
}

VOID CMsgBoxGui::OnSetAlpha( RwUInt8 byAlpha )
{
	m_srfBack.SetAlpha( byAlpha );
}

VOID CMsgBoxGui::OnMouseEnterWaguItem(gui::CComponent* pComponent)
{
	for (int i = 0; i < dMSG_BOX_SLOTS_MAX; i++)
	{
		if (m_pItemIcon[i] == pComponent)
		{
			ShowItemInfoWindow(true, i);
		}
	}
}

VOID CMsgBoxGui::OnMouseLeaveWaguItem(gui::CComponent* pComponent)
{
	ShowItemInfoWindow(false, 0);
}

VOID CMsgBoxGui::ShowItemInfoWindow(RwBool isShow, BYTE i)
{
	if (isShow)
	{
		CRectangle rect = m_pItemIcon[i]->GetScreenRect();

		GetInfoWndManager()->ShowInfoWindow(TRUE, CInfoWndManager::INFOWND_TABLE_ITEM, rect.left, rect.top, ItemSlot[i].GetItemTable(), DIALOG_HLSHOP);
	}
	else
	{
		if (GetInfoWndManager()->GetRequestGui() == DIALOG_HLSHOP)
			GetInfoWndManager()->ShowInfoWindow(FALSE);
	}
}
