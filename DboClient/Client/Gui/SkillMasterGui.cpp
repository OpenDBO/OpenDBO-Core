#include "precomp_dboclient.h"
#include "SkillMasterGui.h"

// core
#include "NtlDebug.h"

// presentation
#include "NtlPLGui.h"
#include "NtlPLGuiManager.h"

// simulation
#include "InputActionMap.h"
#include "NtlSobManager.h"
#include "NtlSobAvatar.h"
#include "NtlSobAvatarAttr.h"
#include "NtlSLGlobal.h"
#include "NtlSkillContainer.h"
#include "NtlSobSkill.h"
#include "NtlSobSkillAttr.h"
#include "NtlSobSkillIcon.h"
#include "NtlSobHTBSkill.h"
#include "NtlSobHTBSkillAttr.h"
#include "NtlSobHTBSkillIcon.h"
#include "NtlSobActionSkill.h"
#include "NtlSobActionSkillAttr.h"
#include "NtlSobActionSkillIcon.h"
#include "NtlSobNpc.h"
#include "NtlSobNpcAttr.h"
#include "NtlSLEvent.h"
#include "NtlSLEventFunc.h"
#include "NtlSLApi.h"
#include "NtlCameraController.h"
#include "NtlWorldConceptNPCCommu.h"

// Table
#include "SkillTable.h"
#include "HTBSetTable.h"
#include "NPCTable.h"
#include "PCTable.h"
#include "MerchantTable.h"
#include "ActionTable.h"
#include "ChatCommandTable.h"
#include "TextAllTable.h"
#include "TableContainer.h"

// dbo
#include "DboDef.h"
#include "DboGlobal.h"
#include "DboLogic.h"
#include "DboEvent.h"
#include "DboEventGenerator.h"
#include "DumpCommand.h"
#include "SurfaceGui.h"
#include "IconMoveManager.h"
#include "InfoWndManager.h"
#include "DisplayStringManager.h"
#include "DisplayStringDef.h"
#include "DialogManager.h"
#include "ChatBalloonManager.h"

// etc
#include "NtlMath.h"
#include "GUISoundDefine.h"

////////////////////////////////////////////////////////////////////////////////
// CSkillMasterItem
CSkillMasterItemDlg::CSkillMasterItemDlg( CSkillMasterGui* pSkillWnd )
: m_pEffectText( NULL ), m_pZennySpText( NULL ), m_pItem( NULL ), m_pbtnLearn( NULL ), m_ppnlZennyMark( NULL ), m_pTitle( NULL ), m_pHelpText( NULL ),
  m_ucColorRed( 255 ), m_ucColorGreen( 255 ), m_ucColorBlue( 255 ), m_ucAlpha( 255 ),
  m_pSkillMasterWnd( pSkillWnd ), m_flagRQState( SRQ_FLAG_INITIAL ), m_eMouseOn( MOUSE_NONE )
{
	gui::CSurfaceManager* pSurfaceManager = CNtlPLGuiManager::GetInstance()->GetSurfaceManager();
	
	// Dialog Setting
	CRectangle rect;
	rect.SetRectWH( 0, 0, SKILLWND_ITEM_WIDTH, SKILLWND_ITEM_HEIGHT );
	m_pThis = NTL_NEW gui::CDialog( rect, pSkillWnd->GetDialog(), pSurfaceManager );
	m_pThis->AddSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "SkillMasterWnd.srf", "srfSkillDlgBack" ) );
	NTL_ASSERT( m_pThis, "CSkillMasterItem::CSkillMasterItem, Dialog Allocate Fail" );

	// Title Setting
	rect.SetRectWH( SKILLWND_TEXT_NAME_X, SKILLWND_TEXT_NAME_Y, SKILLWND_TEXT_NAME_WIDTH, SKILLWND_TEXT_NAME_HEIGHT );
	m_pTitle = NTL_NEW gui::CStaticBox( rect, m_pThis, pSurfaceManager, COMP_TEXT_LEFT | COMP_TEXT_DOWN );
	m_pTitle->CreateFontStd( DEFAULT_FONT, 105, 0 );

	// Icon Setting
	m_rtIcon.SetRectWH( SKILLWND_ITEM_ICON_X, SKILLWND_ITEM_ICON_Y, SKILLWND_ITEM_ICON_WIDTH, SKILLWND_ITEM_ICON_HEIGHT );
	m_surIcon.SetRect( m_rtIcon );

	// Effect Text
	rect.SetRectWH( SKILLWND_TEXT_EFFECT_X, SKILLWND_TEXT_EFFECT_Y, SKILLWND_TEXT_EFFECT_WIDTH, SKILLWND_TEXT_EFFECT_HEIGHT );
	m_pEffectText = NTL_NEW gui::CStaticBox( rect, m_pThis, pSurfaceManager, COMP_TEXT_LEFT );
	m_pEffectText->CreateFontStd( DEFAULT_FONT, DEFAULT_FONT_SIZE, 0 );
	m_pEffectText->Show( true );

	// Zenny SP Text Setting
	rect.SetRectWH( SKILLWND_ITEM_ZENNYMARK_X, SKILLWND_ITEM_ZENNYMARK_Y, SKILLWND_TEXT_HELP_WIDTH, SKILLWND_TEXT_HELP_HEIGHT );
	m_pZennySpText = NTL_NEW gui::CMDStaticBox( rect, m_pThis, pSurfaceManager );
	m_pZennySpText->AddFont( DEFAULT_FONT, DEFAULT_FONT_SIZE );
	m_pZennySpText->Enable( false );

	// 배우기 버튼
	rect.SetRectWH( SKILLWND_ITEM_BUTTON_X, SKILLWND_ITEM_BUTTON_Y, SKILLWND_ITEM_BUTTON_WIDTH, SKILLWND_ITEM_BUTTON_HEIGHT );
	gui::CSurface srfUp = pSurfaceManager->GetSurface( SKILLWND_ITEM_SURFACE_FILE, "srfLearnBtnUp" );
	gui::CSurface srfDown = pSurfaceManager->GetSurface( SKILLWND_ITEM_SURFACE_FILE, "srfLearnBtnDown" );
	gui::CSurface srfFoc = pSurfaceManager->GetSurface( SKILLWND_ITEM_SURFACE_FILE, "srfLearnBtnFoc" );
	gui::CSurface srfDisable = pSurfaceManager->GetSurface( SKILLWND_ITEM_SURFACE_FILE, "srfLearnBtnDisable" );

	m_pbtnLearn = NTL_NEW gui::CButton( rect, "", m_pThis, pSurfaceManager );

	m_pbtnLearn->AddSurfaceUp( srfUp );
	m_pbtnLearn->AddSurfaceFocus( srfFoc );
	m_pbtnLearn->AddSurfaceDown( srfDown );
	m_pbtnLearn->AddSurfaceDisabled( srfDisable );
	m_pbtnLearn->Show( true );	

	// Skill Master용 Zenny Mark
	rect.SetRectWH( SKILLWND_ITEM_ZENNYMARK_X, SKILLWND_ITEM_ZENNYMARK_Y, SKILLWND_ITEM_ZENNYMARK_WIDTH, SKILLWND_ITEM_ZENNYMARK_HEIGHT );
	m_ppnlZennyMark = NTL_NEW gui::CPanel( rect, m_pThis, pSurfaceManager, pSurfaceManager->GetSurface( "GameCommon.srf", "srfZennyMark" ) );
	m_ppnlZennyMark->Show( true );	

	// Help Text
	rect.SetRectWH( SKILLWND_TEXT_HELP_X, SKILLWND_TEXT_HELP_Y, SKILLWND_TEXT_HELP_WIDTH, SKILLWND_TEXT_HELP_HEIGHT );
	m_pHelpText = NTL_NEW gui::CStaticBox( rect, m_pThis, pSurfaceManager, COMP_TEXT_LEFT );
	m_pHelpText->CreateFontStd( DEFAULT_FONT, DEFAULT_FONT_SIZE, 0 );
	m_pHelpText->Show( false );

	// Consume Title
	//rect.SetRectWH( SKILLWND_TEXT_CONSUME_TEXT_X, SKILLWND_TEXT_CONSUME_TEXT_Y, SKILLWND_TEXT_CONSUME_TEXT_WIDTH, SKILLWND_TEXT_CONSUME_TEXT_HEIGHT );
	//m_pConsumeTitle = NTL_NEW gui::CStaticBox( rect, m_pThis, pSurfaceManager, COMP_TEXT_RIGHT );
	//m_pConsumeTitle->CreateFontStd( DEFAULT_FONT, DEFAULT_FONT_SIZE, 0 );
	//m_pConsumeTitle->Show( true );

	// Effect
	m_surFocusEffect.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfSlotFocusEffect" ) );
	m_surFocusEffect.SetRect( m_rtIcon.left, m_rtIcon.top, m_rtIcon.right, m_rtIcon.bottom );

	// Surface Setting
	m_pThis->Show( false );

	// CallBack
	m_slotMouseMove	= m_pThis->SigMouseMove().Connect( this, &CSkillMasterItemDlg::OnMouseMove );
	m_slotPaint		= m_pThis->SigPaint().Connect( this, &CSkillMasterItemDlg::OnPaint );
	m_slotMove		= m_pThis->SigMove().Connect( this, &CSkillMasterItemDlg::OnMove );
	m_slotMouseLeave= m_pThis->SigMouseLeave().Connect( this, &CSkillMasterItemDlg::OnMouseLeave );

	m_slotLearnButtonEnter = m_pbtnLearn->SigMouseEnter().Connect( this, &CSkillMasterItemDlg::OnLearnButtonEnter );
	m_slotLearnButtonLeave = m_pbtnLearn->SigMouseLeave().Connect( this, &CSkillMasterItemDlg::OnLearnButtonLeave );
	m_slotLearnButtonClick = m_pbtnLearn->SigClicked().Connect( this, &CSkillMasterItemDlg::OnLearnButtonClick );	
}

CSkillMasterItemDlg::~CSkillMasterItemDlg(VOID)
{
	if( m_pThis )
	{
		NTL_DELETE( m_pThis );
	}

	m_surIcon.UnsetTexture();
}

VOID CSkillMasterItemDlg::UpdateData( CSkillGuiItem* pItem )
{
	switch( pItem->GetType() )
	{
	case CSkillGuiItem::SKILL:	// Battle Skill
		UpdateSkillData( pItem );
		break;
	case CSkillGuiItem::HTB:	// HTB Skill
		UpdateHTBData( pItem );
		break;	
	}

	if( m_pHelpText->GetFitWidthToString() > m_pHelpText->GetWidth() )
		m_pHelpText->SetToolTip( m_pHelpText->GetText() );
	else
		m_pHelpText->DeleteToolTip();
}

VOID CSkillMasterItemDlg::SetColor( RwUInt8 ucRed, RwUInt8 ucGreen, RwUInt8 ucBlue )
{
	m_ucColorRed = ucRed;
	m_ucColorGreen = ucGreen;
	m_ucColorBlue = ucBlue;

	m_pThis->SetColor( ucRed, ucGreen, ucBlue );

	m_surIcon.SetColorOnly( ucRed, ucGreen, ucBlue );
}

VOID CSkillMasterItemDlg::SetAlpha( RwUInt8 ucAlpha )
{
	m_ucAlpha = ucAlpha;

	m_pThis->SetAlpha( ucAlpha );

	m_surIcon.SetAlpha( ucAlpha );
}

VOID CSkillMasterItemDlg::UpdateSkillData( CSkillGuiItem* pItem )
{
	RwUInt8  ucRed, ucGreen, ucBlue, ucAlpha;
	WCHAR*	 pString = NULL;

	m_pItem = pItem;
	m_flagRQState = SkillCommonLogic::ExmineRQBySkill( m_pItem->GetData() );

	CRectangle rtScreen = m_pThis->GetScreenRect();
	sSKILL_TBLDAT* pSkillData = reinterpret_cast<sSKILL_TBLDAT*>( m_pItem->GetData() );
		
	// Texture 입력
	m_surIcon.UnsetTexture();
	if( m_pItem->IsUseable() )
		m_surIcon.SetTexture( reinterpret_cast<gui::CTexture*>( m_pItem->GetIcon()->GetImage() ) );
	else
		m_surIcon.SetTexture( Logic_CreateTexture( pSkillData->szIcon_Name, TEXTYPE_ICON ), TRUE );
		
	// Infomation 출력
	m_pTitle->SetText( API_GetTableContainer()->GetTextAllTable()->GetSkillTbl()->GetText( pSkillData->Skill_Name ).c_str() );

	//m_pConsumeTitle->SetText( GetDisplayStringManager()->GetString( DST_SKILL_CONSUME_TEXT ) );
	RwInt32 nNotShowedEffectCount = 0;
	for( RwInt32 i = 0 ; i < NTL_MAX_EFFECT_IN_SKILL ; ++i )
	{
		std::wstring wstrText;
		RwUInt32 uiColor = RGB( 255, 255, 255 );	

		if( Logic_GetSystemEffectText( pSkillData->skill_Effect[i], pSkillData->fSkill_Effect_Value[i], wstrText, pSkillData->bySkill_Effect_Type[i], pSkillData, &uiColor ) )
		{
			if( i == 0 )
			{
				m_pEffectText->SetTextColor( uiColor );
				m_pEffectText->SetText( wstrText.c_str() );
			}			
			else
			{
				++nNotShowedEffectCount;
			}
		}
	}

	if( nNotShowedEffectCount > 0 )
	{
		WCHAR buf[256];
		swprintf_s( buf, 256, GetDisplayStringManager()->GetString( DST_SKILL_HAVE_MORE_EFFECT ), nNotShowedEffectCount );
		m_pEffectText->AddText( buf );
	}

	if( m_flagRQState == SRQ_FLAG_CAN_LEARN_BY_SKILLMASTER )
	{
		m_pbtnLearn->ClickEnable( TRUE );

		ucRed = SKILLWND_ITEM_LEARN_COLOR_RED;
		ucGreen = SKILLWND_ITEM_LEARN_COLOR_GREEN;
		ucBlue = SKILLWND_ITEM_LEARN_COLOR_BLUE;
		ucAlpha = SKILLWND_ITEM_LEARN_ALPHA;	

		m_pZennySpText->Clear();

		if( pSkillData->dwRequire_Zenny > 0 )
		{
			RwInt32 nOffsetZenny = SKILLWND_ITEM_ZENNYMARK_WIDTH + SKILLWND_TEXT_ZENNY_OFFSET;
			m_pZennySpText->SetItem( pSkillData->dwRequire_Zenny, "RequireZenny", 0, COMP_TEXT_LEFT, SKILLWND_COLOR_COST, nOffsetZenny );
			m_ppnlZennyMark->Show( true );
		}
		else
		{
			m_ppnlZennyMark->Show( false );
		}

		if( pSkillData->wRequireSP > 0 )
		{
			RwInt32 nOffsetSP = 0;
			RwBool bPrevLine = TRUE;

			if( m_ppnlZennyMark->IsVisible() )
			{
				nOffsetSP = SKILLWND_TEXT_SP_OFFSET;
				bPrevLine = TRUE;
			}
			else
			{
				nOffsetSP = 0;
				bPrevLine = FALSE;
			}

			m_pZennySpText->Format( "RequiredSP", 0, COMP_TEXT_LEFT, SKILLWND_COLOR_SP, nOffsetSP, bPrevLine, GetDisplayStringManager()->GetString( DST_SKILL_SP ), pSkillData->wRequireSP );
		}

		m_pZennySpText->DrawItem();
		m_pZennySpText->Show( true );		
		m_pHelpText->Show( false );
	}
	else
	{
		m_pbtnLearn->ClickEnable( FALSE );

		if( m_flagRQState & SRQ_FLAG_LEARNED )
			pString = const_cast<WCHAR*>( GetDisplayStringManager()->GetString( DST_SKILL_ALREADY_LEARNED ) );
		else if( m_flagRQState & SRQ_FLAG_NOT_LEARN_CLASS )
			pString = const_cast<WCHAR*>( GetDisplayStringManager()->GetString( DST_SKILL_CANT_LEARN_FOREVER ) );
		else
			pString = const_cast<WCHAR*>( GetDisplayStringManager()->GetString( DST_SKILL_CANT_LEARN ) );
		
		ucRed = SKILLWND_ITEM_NOT_LEARN_COLOR_RED;
		ucGreen = SKILLWND_ITEM_NOT_LEARN_COLOR_GREEN;
		ucBlue = SKILLWND_ITEM_NOT_LEARN_COLOR_BLUE;
		ucAlpha = SKILLWND_ITEM_NOT_LEARN_ALPHA;	

		m_pHelpText->SetText( pString );
		
		m_pZennySpText->Show( false );
		m_ppnlZennyMark->Show( false );
		m_pHelpText->Show( true );
	}		

	// Dialog색 적용
	SetColor( ucRed, ucGreen, ucBlue );
	SetAlpha( ucAlpha );

	// InfoWndUpdate
	RwInt32 eInfoWndState = GetInfoWndManager()->GetInfoWndState();
	if( ( m_eMouseOn == MOUSE_ONRQ && eInfoWndState == CInfoWndManager::INFOWND_SKILLRQ ) ||
		( m_eMouseOn == MOUSE_ONRQ && eInfoWndState == CInfoWndManager::INFOWND_JUST_WTEXT ) )
	{
		CRectangle rtRect = m_pbtnLearn->GetScreenRect();
		GetInfoWndManager()->ShowInfoWindow( TRUE, CInfoWndManager::INFOWND_SKILLRQ, rtRect.left, rtRect.top, (VOID*)m_flagRQState, DIALOG_SKILL_TRAINER );
	}

	if(  m_eMouseOn == MOUSE_ONICON &&
		( eInfoWndState == CInfoWndManager::INFOWND_SKILL || eInfoWndState == CInfoWndManager::INFOWND_NOTLEARN_SKILL ) )
	{
		if( m_pItem->IsUseable() )
			GetInfoWndManager()->ShowInfoWindow( TRUE, CInfoWndManager::INFOWND_SKILL, m_rtIcon.left + rtScreen.left, m_rtIcon.top + rtScreen.top, m_pItem->GetIcon()->GetSobObj(), DIALOG_SKILL_TRAINER );
		else
			GetInfoWndManager()->ShowInfoWindow( TRUE, CInfoWndManager::INFOWND_NOTLEARN_SKILL, m_rtIcon.left + rtScreen.left, m_rtIcon.top + rtScreen.top, pSkillData, DIALOG_SKILL_TRAINER );		
	}
}

VOID CSkillMasterItemDlg::UpdateHTBData( CSkillGuiItem* pItem )
{
	RwUInt8  ucRed, ucGreen, ucBlue, ucAlpha;
	WCHAR*	 pString = NULL;

	m_pItem = pItem;
	m_flagRQState = SkillCommonLogic::ExmineRQByHTB( m_pItem->GetData() );

	CRectangle rtScreen = m_pThis->GetScreenRect();
	sHTB_SET_TBLDAT* pHTBData = reinterpret_cast<sHTB_SET_TBLDAT*>( m_pItem->GetData() );

	// Texture 입력
	m_surIcon.UnsetTexture();
	if( m_pItem->IsUseable() )
		m_surIcon.SetTexture( reinterpret_cast<gui::CTexture*>( m_pItem->GetIcon()->GetImage() ) );
	else
		m_surIcon.SetTexture( Logic_CreateTexture( pHTBData->szIcon_Name, TEXTYPE_ICON ), TRUE );

	// Infomation 출력
	m_pTitle->SetText( API_GetTableContainer()->GetTextAllTable()->GetHTBSetTbl()->GetText( pHTBData->HTB_Skill_Name ).c_str() );

	m_pEffectText->SetTextColor( INFOCOLOR_7 );
	m_pEffectText->Format( GetDisplayStringManager()->GetString( DST_SKILL_TOTAL_DAMAGE ), (RwInt32)Logic_GetHTBTotalPower( pHTBData ) );

	if( m_flagRQState == SRQ_FLAG_CAN_LEARN_BY_SKILLMASTER )
	{
		m_pbtnLearn->ClickEnable( TRUE );

		ucRed = SKILLWND_ITEM_LEARN_COLOR_RED;
		ucGreen = SKILLWND_ITEM_LEARN_COLOR_GREEN;
		ucBlue = SKILLWND_ITEM_LEARN_COLOR_BLUE;
		ucAlpha = SKILLWND_ITEM_LEARN_ALPHA;		
	
		m_pZennySpText->Clear();

		if( pHTBData->dwRequire_Zenny > 0 )
		{
			RwInt32 nOffsetZenny = SKILLWND_ITEM_ZENNYMARK_WIDTH + SKILLWND_TEXT_ZENNY_OFFSET;
			m_pZennySpText->SetItem( pHTBData->dwRequire_Zenny, "RequireZenny", 0, COMP_TEXT_LEFT, SKILLWND_COLOR_COST, nOffsetZenny );
			m_ppnlZennyMark->Show( true );
		}
		else
		{
			m_ppnlZennyMark->Show( false );
		}

		if( pHTBData->wRequireSP > 0 )
		{
			RwInt32 nOffsetSP = 0;
			RwBool bPrevLine = TRUE;

			if( m_ppnlZennyMark->IsVisible() )
			{
				nOffsetSP = SKILLWND_TEXT_SP_OFFSET;
				bPrevLine = TRUE;
			}
			else
			{
				nOffsetSP = 0;
				bPrevLine = FALSE;
			}

			m_pZennySpText->Format( "RequiredSP", 0, COMP_TEXT_LEFT, SKILLWND_COLOR_SP, nOffsetSP, bPrevLine, GetDisplayStringManager()->GetString( DST_SKILL_SP ), pHTBData->wRequireSP );
		}

		m_pZennySpText->DrawItem();

		m_pZennySpText->Show( true );
		m_ppnlZennyMark->Show( true );
		m_pHelpText->Show( false );
	}
	else
	{
		m_pbtnLearn->ClickEnable( FALSE );

		if( m_flagRQState & SRQ_FLAG_LEARNED )
			pString = const_cast<WCHAR*>( GetDisplayStringManager()->GetString( DST_SKILL_ALREADY_LEARNED ) );
		else if( m_flagRQState & SRQ_FLAG_NOT_LEARN_CLASS )
			pString = const_cast<WCHAR*>( GetDisplayStringManager()->GetString( DST_SKILL_CANT_LEARN_FOREVER ) );
		else
			pString = const_cast<WCHAR*>( GetDisplayStringManager()->GetString( DST_SKILL_CANT_LEARN ) );

		ucRed = SKILLWND_ITEM_NOT_LEARN_COLOR_RED;
		ucGreen = SKILLWND_ITEM_NOT_LEARN_COLOR_GREEN;
		ucBlue = SKILLWND_ITEM_NOT_LEARN_COLOR_BLUE;
		ucAlpha = SKILLWND_ITEM_NOT_LEARN_ALPHA;	

		m_pHelpText->SetText( pString );

		m_pZennySpText->Show( false );
		m_ppnlZennyMark->Show( false );
		m_pHelpText->Show( true );
	}

	// Dialog색 적용
	SetColor( ucRed, ucGreen, ucBlue );
	SetAlpha( ucAlpha );		

	// InfoWndUpdate
	RwInt32 eInfoWndState = GetInfoWndManager()->GetInfoWndState();
	if( ( m_eMouseOn == MOUSE_ONRQ && eInfoWndState == CInfoWndManager::INFOWND_SKILLRQ ) ||
		( m_eMouseOn == MOUSE_ONRQ && eInfoWndState == CInfoWndManager::INFOWND_JUST_WTEXT ) )
	{
		CRectangle rtRect = m_pbtnLearn->GetScreenRect();
		GetInfoWndManager()->ShowInfoWindow( TRUE,CInfoWndManager::INFOWND_SKILLRQ, rtRect.left, rtRect.top, (VOID*)m_flagRQState, DIALOG_SKILL_TRAINER );
	}

	if( m_eMouseOn == MOUSE_ONICON &&
		( eInfoWndState == CInfoWndManager::INFOWND_HTBSKILL || eInfoWndState == CInfoWndManager::INFOWND_NOTLEARN_HTBSKILL ) )
	{
		if( m_pItem->IsUseable() )
		{
			GetInfoWndManager()->ShowInfoWindow( TRUE, CInfoWndManager::INFOWND_HTBSKILL, m_rtIcon.left + rtScreen.left, m_rtIcon.top + rtScreen.top, m_pItem->GetIcon()->GetSobObj(), DIALOG_SKILL_TRAINER );
		}
		else
		{
			GetInfoWndManager()->ShowInfoWindow( TRUE, CInfoWndManager::INFOWND_NOTLEARN_HTBSKILL, m_rtIcon.left + rtScreen.left, m_rtIcon.top + rtScreen.top, m_pItem->GetData(), DIALOG_SKILL_TRAINER );
		}
	}
}

VOID CSkillMasterItemDlg::OnMouseMove( RwInt32 nKey, RwInt32 nXPos, RwInt32 nYPos )
{
	if( m_rtIcon.PtInRect( nXPos, nYPos ) )
	{
		CRectangle rtScreen = m_pThis->GetScreenRect();

		if( m_eMouseOn != MOUSE_ONICON )
		{
			CInfoWndManager::eINFOWNDSTATE eInfoState = CInfoWndManager::INFOWND_NOTSHOW;

			if( m_pItem->IsUseable() )
			{
				switch( m_pItem->GetType() )
				{
				case CSkillGuiItem::SKILL : eInfoState = CInfoWndManager::INFOWND_SKILL; break;
				case CSkillGuiItem::HTB : eInfoState = CInfoWndManager::INFOWND_HTBSKILL; break;				
				}

				GetInfoWndManager()->ShowInfoWindow( TRUE, eInfoState, m_rtIcon.left + rtScreen.left, m_rtIcon.top + rtScreen.top, m_pItem->GetIcon()->GetSobObj(), DIALOG_SKILL_TRAINER );
			}
			else
			{
				switch( m_pItem->GetType() )
				{
				case CSkillGuiItem::SKILL: eInfoState = CInfoWndManager::INFOWND_NOTLEARN_SKILL; break;
				case CSkillGuiItem::HTB: eInfoState = CInfoWndManager::INFOWND_NOTLEARN_HTBSKILL; break;
				}

				GetInfoWndManager()->ShowInfoWindow( TRUE, eInfoState, m_rtIcon.left + rtScreen.left, m_rtIcon.top + rtScreen.top, m_pItem->GetData(), DIALOG_SKILL_TRAINER );
			}

			m_eMouseOn = MOUSE_ONICON;
		}

		return;
	}
	
	if( m_eMouseOn != MOUSE_NONE )
	{
		GetInfoWndManager()->ShowInfoWindow( FALSE );
		m_eMouseOn = MOUSE_NONE;
	}
}

VOID CSkillMasterItemDlg::OnMouseLeave( gui::CComponent* pComponent )
{
	if( m_eMouseOn != MOUSE_NONE )
	{
		GetInfoWndManager()->ShowInfoWindow( FALSE );
		m_eMouseOn = MOUSE_NONE;
	}
}

VOID CSkillMasterItemDlg::OnPaint(VOID)
{
	m_surIcon.Render();

	if( m_eMouseOn == MOUSE_ONICON )
		m_surFocusEffect.Render();	
}

VOID CSkillMasterItemDlg::OnMove( RwInt32 nOldX, RwInt32 nOldY )
{
	CRectangle rtScreen = m_pThis->GetScreenRect();
	m_surIcon.SetPosition( rtScreen.left + m_rtIcon.left, rtScreen.top + m_rtIcon.top );
	m_surFocusEffect.SetPosition( rtScreen.left + m_rtIcon.left, rtScreen.top + m_rtIcon.top );	
}

VOID CSkillMasterItemDlg::OnLearnButtonEnter( gui::CComponent* pComponent )
{
	m_eMouseOn = MOUSE_ONRQ;

	CRectangle rtScreen = m_pbtnLearn->GetScreenRect();

	if( m_flagRQState & SRQ_FLAG_CAN_LEARN_BY_SKILLMASTER )
	{
		WCHAR* pString = const_cast<WCHAR*>( GetDisplayStringManager()->GetString( DST_SKILL_READY_LEARN ) );
		GetInfoWndManager()->ShowInfoWindow( TRUE, CInfoWndManager::INFOWND_JUST_WTEXT, rtScreen.left, rtScreen.top, (VOID*)pString, DIALOG_SKILL_TRAINER );
	}
	else
		GetInfoWndManager()->ShowInfoWindow( TRUE, CInfoWndManager::INFOWND_SKILLRQ, rtScreen.left, rtScreen.top, (VOID*)m_flagRQState, DIALOG_SKILL_TRAINER );
}

VOID CSkillMasterItemDlg::OnLearnButtonLeave( gui::CComponent* pComponent )
{
	m_eMouseOn = MOUSE_NONE;
	GetInfoWndManager()->ShowInfoWindow( FALSE );
}

VOID CSkillMasterItemDlg::OnLearnButtonClick( gui::CComponent* pComponent )
{
	CNtlSobNpc* pSkillMaster = m_pSkillMasterWnd->GetSkillMaster();
	CNtlSobNpcAttr* pSkillMasterAttr = reinterpret_cast<CNtlSobNpcAttr*>( pSkillMaster->GetSobAttr() );
	sNPC_TBLDAT* pSkillMasterData = pSkillMasterAttr->GetNpcTbl();
	CMerchantTable* pSkillMasterItemTable = API_GetTableContainer()->GetMerchantTable();

	for( RwUInt8 i = 0 ; i < NTL_MAX_MERCHANT_TAB_COUNT ; ++i )
	{
		if( pSkillMasterData->amerchant_Tblidx[i] == INVALID_TBLIDX )
			continue;

		sMERCHANT_TBLDAT* pSkillMasterItemData = reinterpret_cast<sMERCHANT_TBLDAT*>( pSkillMasterItemTable->FindData( pSkillMasterData->amerchant_Tblidx[i] ) );
		NTL_ASSERT( pSkillMasterItemData, "CSkillMasterItemDlg::FindSkillFromTableIdx : Invalid Skill Merchant Idx" );

		for( RwUInt8 j = 0 ; j < NTL_MAX_MERCHANT_COUNT ; ++j )
		{
			if( m_pItem->GetData()->tblidx == pSkillMasterItemData->aitem_Tblidx[j] )
			{
				GetDboGlobal()->GetGamePacketGenerator()->SendShopSkillBuyReq( pSkillMaster->GetSerialID(), i, j );								
				return;
			}			
		}
	}		
}

////////////////////////////////////////////////////////////////////////////////
// CSkillMasterGui

CSkillMasterGui::CSkillMasterGui(VOID)
{
	Init();
}

CSkillMasterGui::CSkillMasterGui( const RwChar *pName )
: CNtlPLGui( pName )
{
	Init();
}

CSkillMasterGui::~CSkillMasterGui(VOID)
{

}

RwBool CSkillMasterGui::Create(VOID)
{
	NTL_FUNCTION( "CSkillMasterGui::Create" );

	if( !CNtlPLGui::Create( "", "gui\\SkillMasterWnd.srf", "gui\\SkillMasterWnd.frm" ) )
		NTL_RETURN( FALSE );

	CNtlPLGui::CreateComponents( GetNtlGuiManager()->GetGuiManager() );

	m_pThis = (gui::CDialog*)GetComponent( "dlgMain" );

	m_pTitle = reinterpret_cast<gui::CStaticBox*>( GetComponent( "stbTitle" ) );
	m_pSP = reinterpret_cast<gui::CStaticBox*>( GetComponent( "stbSP" ) );
	m_pScrollBar = (gui::CScrollBar*)GetComponent( "scbScroll" );
	m_pTabButton = (gui::CTabButton*)GetComponent( "tabSkill" );
	m_pCloseButton = (gui::CButton*)GetComponent( "btnClose" );

	m_pFilter[0] = (gui::CButton*)GetComponent( "btnFilter1" );
	m_pFilter[1] = (gui::CButton*)GetComponent( "btnFilter2" );
	m_pFilter[2] = (gui::CButton*)GetComponent( "btnFilter3" );
	m_pFilter[3] = (gui::CButton*)GetComponent( "btnFilter4" );

	m_slotTabChanged			= m_pTabButton->SigSelectChanged().Connect( this, &CSkillMasterGui::OnTabChanged );
	m_slotCloseButtonClicked	= m_pCloseButton->SigClicked().Connect( this, &CSkillMasterGui::OnCloseButtonClicked );
	m_slotScrollChanged			= m_pScrollBar->SigValueChanged().Connect( this, &CSkillMasterGui::OnScrollChanged );
	m_slotScrollSliderMoved		= m_pScrollBar->SigSliderMoved().Connect( this, &CSkillMasterGui::OnScrollChanged );

	m_slotFilterBtnClicked[0]	= m_pFilter[0]->SigToggled().Connect( this, &CSkillMasterGui::OnFilterBtn0Clicked );
	m_slotFilterBtnClicked[1]	= m_pFilter[1]->SigToggled().Connect( this, &CSkillMasterGui::OnFilterBtn1Clicked );
	m_slotFilterBtnClicked[2]	= m_pFilter[2]->SigToggled().Connect( this, &CSkillMasterGui::OnFilterBtn2Clicked );
	m_slotFilterBtnClicked[3]	= m_pFilter[3]->SigToggled().Connect( this, &CSkillMasterGui::OnFilterBtn3Clicked );

	m_slotCaptureWheelMove = GetNtlGuiManager()->GetGuiManager()->SigCaptureWheelMove().Connect( this, &CSkillMasterGui::OnCaptureWheelMove );
	m_slotCaptureMouseDown = GetNtlGuiManager()->GetGuiManager()->SigCaptureMouseDown().Connect( this, &CSkillMasterGui::OnCaptureMouseDown );

	// Tab Setting
	m_pTabButton->AddTab( std::wstring( GetDisplayStringManager()->GetString( DST_SKILL_TAB_BATTLE_SKILL ) ) );
	m_pTabButton->AddTab( std::wstring( GetDisplayStringManager()->GetString( DST_SKILL_TAB_SUB_SKILL ) ) );
	
	// Filter Button Setting
	FilterButtonSetting();
	m_pFilter[0]->SetText( std::wstring( GetDisplayStringManager()->GetString( DST_SKILL_FILTER_ALL ) ) );		// 변하지 않는 녀석. 변한다면 FilterButtonSetting에 넣자.

	// 초기 SkillItemDlg 생성.
	for( RwInt32 i = 0 ; i < MAX_VIEWABLE_ITEM ; ++i )
	{
		m_arrViewSet[i] = NTL_NEW CSkillMasterItemDlg( this );
		NTL_ASSERT( m_arrViewSet[i], "CSkillMasterGui::CreateItems, Failed to Allocate SkillMasterItemDlg" );
	}

	// Initial Setting
	m_pThis->Show( false );
	SetFilterMode( 0 );

	// serialize 
	EnableSerialize(TRUE);

	// Event연결
	LinkMsg( g_EventSobInfoUpdate );
	LinkMsg( g_EventNPCDialogOpen );
	LinkMsg( g_EventDialog );
	LinkMsg( g_EventCharObjDelete );
	LinkMsg( g_EventChangeClassAuthorityChangedNfy, 0, 0x7000 );
	LinkMsg( g_EventSobConvertClass, 0, 0x7000 );
		
	NTL_RETURN( TRUE );
}

VOID CSkillMasterGui::Destroy(VOID)
{
	ClearItems();
	ClearItemDialogs();

	UnLinkMsg( g_EventSobInfoUpdate );
	UnLinkMsg( g_EventNPCDialogOpen );
	UnLinkMsg( g_EventDialog );
	UnLinkMsg( g_EventCharObjDelete );
	UnLinkMsg( g_EventChangeClassAuthorityChangedNfy );
	UnLinkMsg( g_EventSobConvertClass );
		
	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();
}

VOID CSkillMasterGui::Init(VOID)
{
	RwInt32 i;

	for( i = 0 ; i < NUM_TAB ; ++i )
	{
		m_nTabScrollOffset[i] = 0;
	}

	for( i = 0 ; i < NUM_BATTLE_FILTER ; ++i )
	{
		m_nBattleFilterScrollOffset[i] = 0;
	}

	for( i = 0 ; i < NUM_ASSIST_FILTER ; ++i )
	{
		m_nAssistFilterScrollOffset[i] = 0;
	}

	for( i = 0 ; i < MAX_VIEWABLE_ITEM ; ++i )
	{
		m_arrViewSet[i] = NULL;
	}

	m_eCurrentTab = BATTLE_TAB;
	m_eCurrentBattleFilter = NO_BATTLE_FILTER;
	m_eCurrentAssistFilter = NO_ASSIST_FILTER;
	
	m_pScrollBar = NULL;
	m_pTabButton = NULL;
	m_pCloseButton = NULL;
	m_pTitle = NULL;

	for( i = 0 ; i < 4 ; ++i )
	{
		m_pFilter[i] = NULL;
	}
	
	m_pSkillMaster = NULL;
}

VOID CSkillMasterGui::SetItems( RwUInt32 hNPCSerial )
{
	ClearItems();

	CNtlSobNpc* pNPC = reinterpret_cast<CNtlSobNpc*>( GetNtlSobManager()->GetSobObject( hNPCSerial ) );
	NTL_ASSERT( pNPC, "CSkillMasterGui::SetItem : Invalid NPC Handle" );
	CNtlSobNpcAttr* pNPCAttr = reinterpret_cast<CNtlSobNpcAttr*>( pNPC->GetSobAttr() );
	sNPC_TBLDAT* pNPCData = pNPCAttr->GetNpcTbl();

	m_pSkillMaster = pNPC;

	CSkillTable* pSkillTable = API_GetTableContainer()->GetSkillTable();
	CHTBSetTable* pHTBTable = API_GetTableContainer()->GetHTBSetTable();
	CMerchantTable* pSkillMasterItemTable = API_GetTableContainer()->GetMerchantTable();

	// UI Name 적용
	m_pTitle->SetText( Logic_GetNPCJobName( pNPCData->byJob ) );
	
	// 스킬 아이템 생성
	for( RwInt32 i = 0 ; i < NTL_MAX_MERCHANT_TAB_COUNT ; ++i )
	{
		if( pNPCData->amerchant_Tblidx[i] == INVALID_TBLIDX )
			continue;

		sMERCHANT_TBLDAT* pMerchantData = reinterpret_cast<sMERCHANT_TBLDAT*>( pSkillMasterItemTable->FindData( pNPCData->amerchant_Tblidx[i] ) );
		if( !pMerchantData )
		{
			if( Logic_IsUIDevInfoVisible() )
				GetBalloonManager()->AddNPCBalloon( pNPC, L"Invalid Skill Merchant Idx", BALLOON_NPC_DISPTIME );
			
			continue;
			//NTL_ASSERT( pMerchantData, "CSkillMasterItemDlg::FindSkillFromTableIdx : Invalid Skill Merchant Idx" );
		}

		if( pMerchantData->bySell_Type == MERCHANT_SELL_TYPE_SKILL )
		{
			for( RwInt32 j = 0 ; j < NTL_MAX_MERCHANT_COUNT ; ++j )
			{
				if( pMerchantData->aitem_Tblidx[j] == INVALID_TBLIDX )
					break;

				sSKILL_TBLDAT* pSkillData = reinterpret_cast<sSKILL_TBLDAT*>( pSkillTable->FindData( pMerchantData->aitem_Tblidx[j] ) );

				// 형석 임시 코드
				if(pSkillData == NULL)
					continue;
								
				CSkillGuiItem* pItem = NTL_NEW CSkillGuiItem( NULL, pSkillData, CSkillGuiItem::SKILL );
				
				if( pSkillData->bySkill_Class == NTL_SKILL_CLASS_ACTIVE )
				{
					if( pSkillData->bySkill_Type == NTL_SKILL_TYPE_PHYSICAL ||
						pSkillData->bySkill_Type == NTL_SKILL_TYPE_ENERGY )
					{
						AddSkillMapData( &m_mapBattle, pItem );						
					}
					else // NTL_SKILL_TYPE_NONE || NTL_SKILL_TYPE_STATE )
					{
						AddSkillMapData( &m_mapAssist, pItem );						
					}					
				}
				else // NTL_SKILL_CLASS_PASSIVE
				{
					AddSkillMapData( &m_mapAssist, pItem );						
				}	
			}
		}
		else if( pMerchantData->bySell_Type == MERCHANT_SELL_TYPE_HTB_SKILL )
		{
			for( RwInt32 j = 0 ; j < NTL_MAX_MERCHANT_COUNT ; ++j )
			{
				if( pMerchantData->aitem_Tblidx[j] == INVALID_TBLIDX )
					break;

				sHTB_SET_TBLDAT* pHTBData = reinterpret_cast<sHTB_SET_TBLDAT*>( pHTBTable->FindData( pMerchantData->aitem_Tblidx[j] ) );
				CSkillGuiItem* pItem = NTL_NEW CSkillGuiItem( NULL, pHTBData, CSkillGuiItem::HTB );

				AddSkillMapData( &m_mapBattle, pItem );				
			}
		}		
	}		

	// Filter 생성
	SetFilterItems();
}

VOID CSkillMasterGui::ClearItems(VOID)
{
	SKILLMAP_IT it;
	RwInt32 i = 0;

	for( it = m_mapBattle.begin() ; it != m_mapBattle.end() ; ++it )
	{
		if( (*it).second )
			NTL_DELETE( (*it).second );
	}
	for( it = m_mapAssist.begin() ; it != m_mapAssist.end() ; ++it )
	{
		if( (*it).second )
			NTL_DELETE( (*it).second );
	}

	m_mapBattle.clear();
	m_mapAssist.clear();

	for( i = 0 ; i < NUM_TAB ; ++i )
	{
		m_nTabScrollOffset[i] = 0;
	}

	for( i = 0 ; i < NUM_BATTLE_FILTER ; ++i )
	{
		m_nBattleFilterScrollOffset[i] = 0;
		m_vecBattleFilter[i].clear();
	}

	for( i = 0 ; i < NUM_ASSIST_FILTER ; ++i )
	{
		m_nAssistFilterScrollOffset[i] = 0;
		m_vecAssistFilter[i].clear();
	}	

	m_eCurrentTab = BATTLE_TAB;
	m_eCurrentBattleFilter = NO_BATTLE_FILTER;
	m_eCurrentAssistFilter = NO_ASSIST_FILTER;
}

VOID CSkillMasterGui::ClearItemDialogs(VOID)
{
	for( RwInt32 i = 0 ; i < MAX_VIEWABLE_ITEM ; ++i )
	{
		if( m_arrViewSet[i] )
			NTL_DELETE( m_arrViewSet[i] );
	}
}

VOID CSkillMasterGui::AddSkillMapData( SKILLMAP* pMap, CSkillGuiItem* pItem )
{
	// 맵에 중복된 데이터는 자동으로 지우고 갱신.
	SKILLMAP_IT it;

	if( pItem->GetType() == CSkillGuiItem::SKILL )
	{
		sSKILL_TBLDAT* pSkillData = reinterpret_cast<sSKILL_TBLDAT*>( pItem->GetData() );

		it = pMap->find( pSkillData->bySlot_Index );

		if( it == pMap->end() )
		{
			(*pMap)[pSkillData->bySlot_Index] = pItem;
		}
		else
		{
			CSkillGuiItem* pEraseItem = (*it).second;
			NTL_DELETE( pEraseItem );
			(*it).second = pItem;						
		}
	}
	else if( pItem->GetType() == CSkillGuiItem::HTB )
	{
		sHTB_SET_TBLDAT* pHTBData = reinterpret_cast<sHTB_SET_TBLDAT*>( pItem->GetData() );

		it = pMap->find( pHTBData->bySlot_Index );

		if( it == pMap->end() )
		{
			(*pMap)[pHTBData->bySlot_Index] = pItem;
		}
		else
		{
			CSkillGuiItem* pEraseItem = (*it).second;
			NTL_DELETE( pEraseItem );
			(*it).second = pItem;					
		}
	}
}

CSkillGuiItem* CSkillMasterGui::GetSkillMapData( SKILLMAP* pMap, RwUInt32 idx )
{
	SKILLMAP_IT it;

	it = pMap->find( idx );

	if( it == pMap->end() )
		return NULL;
	else
		return (*it).second;			
}

VOID CSkillMasterGui::SetFilterItems(VOID)
{
	// Filter 비우기	
	for(RwInt32 i = 0 ; i < NUM_BATTLE_FILTER ; ++i )
		m_vecBattleFilter[i].clear();

	for( RwInt32 i = 0 ; i < NUM_ASSIST_FILTER ; ++i )
		m_vecAssistFilter[i].clear();

	// Filter 입력.
	SKILLMAP_IT sit;

	for( sit = m_mapBattle.begin() ; sit != m_mapBattle.end() ; ++sit )
	{
		CSkillGuiItem* pItem = (*sit).second;
		NTL_ASSERT( pItem, "CSkillMasterGui::CreateItem : Wrong Skill Item Inserted" );

		if( pItem->GetType() == CSkillGuiItem::SKILL )
		{
			m_vecBattleFilter[BATTLE_FILTER].push_back( pItem );			
		}
		else if( pItem->GetType() == CSkillGuiItem::HTB )
		{
			m_vecBattleFilter[HTB_FILTER].push_back( pItem );
		}
		else
		{
			NTL_ASSERTFAIL( "CSKillMasterGui::CreateItem() : Unavilable Type in Battle Tab" );
			continue;
		}
	}

	for( sit = m_mapAssist.begin() ; sit != m_mapAssist.end() ; ++sit )
	{
		CSkillGuiItem* pItem = (*sit).second;
		NTL_ASSERT( pItem, "CSkillMasterGui::CreateItem : Wrong Skill Item Inserted" );
		sSKILL_TBLDAT* pData = reinterpret_cast<sSKILL_TBLDAT*>( pItem->GetData() );

		if( pData->bySkill_Class == NTL_SKILL_CLASS_PASSIVE )
		{
			m_vecAssistFilter[PASSIVE_FILTER].push_back( pItem );
		}
		else // NTL_SKILL_CLASS_ACTIVE and NTL_SKILL_TYPE_STATE
		{
			m_vecAssistFilter[ASSIST_FILTER].push_back( pItem );
		}		
	}
}

VOID CSkillMasterGui::ShowItemDetect(VOID)
{ 
	RwInt32 i;

	for( i = 0 ; i < MAX_VIEWABLE_ITEM ; ++i )
	{
		if( m_arrViewSet[i] )
			m_arrViewSet[i]->Show( false );
	}

	if( m_eCurrentTab == BATTLE_TAB )
	{
		if( m_mapBattle.size() <= 0 )
			return;

		if( m_eCurrentBattleFilter == NO_BATTLE_FILTER )
		{
			SKILLMAP_IT it;
			for( i = 0, it = m_mapBattle.begin() ; i < m_nTabScrollOffset[m_eCurrentTab] && it != m_mapBattle.end() ; ++i, ++it );			

			for( i = 0 ; it != m_mapBattle.end() ; ++it, ++i )
			{
				if( i >= MAX_VIEWABLE_ITEM )
					break;

				m_arrViewSet[i]->UpdateData( (*it).second );
				m_arrViewSet[i]->Show( true );
				ItemPositionSet( i, m_arrViewSet[i] );
			}
		}
		else
		{
			RwInt32 nMaxItemCount = m_vecBattleFilter[m_eCurrentBattleFilter].size();

			for( i = 0 ; i < MAX_VIEWABLE_ITEM ; ++i )
			{
				if( m_nBattleFilterScrollOffset[m_eCurrentBattleFilter] + i >= nMaxItemCount )
					break;

				m_arrViewSet[i]->UpdateData( m_vecBattleFilter[m_eCurrentBattleFilter][m_nBattleFilterScrollOffset[m_eCurrentBattleFilter] + i] );
				m_arrViewSet[i]->Show( true );
				ItemPositionSet( i, m_arrViewSet[i] );				
			}
		}
	}
	else if( m_eCurrentTab == ASSIST_TAB )
	{
		if( m_mapAssist.size() <= 0 )
			return;

		if( m_eCurrentAssistFilter == NO_ASSIST_FILTER )
		{
			SKILLMAP_IT it;
			for( i = 0, it = m_mapAssist.begin() ; i < m_nTabScrollOffset[m_eCurrentTab] && it != m_mapAssist.end() ; ++i, ++it );
			
			for( i = 0 ; it != m_mapAssist.end() ; ++it, ++i )
			{
				if( i >= MAX_VIEWABLE_ITEM )
					break;

				m_arrViewSet[i]->UpdateData( (*it).second );
				m_arrViewSet[i]->Show( true );
				ItemPositionSet( i, m_arrViewSet[i] );
			}	
		}
		else
		{
			RwInt32 nMaxItemCount = m_vecAssistFilter[m_eCurrentAssistFilter].size();

			for( i = 0 ; i < MAX_VIEWABLE_ITEM ; ++i )
			{
				if( m_nAssistFilterScrollOffset[m_eCurrentAssistFilter] + i >= nMaxItemCount )
					break;

				m_arrViewSet[i]->UpdateData( m_vecAssistFilter[m_eCurrentAssistFilter][m_nAssistFilterScrollOffset[m_eCurrentAssistFilter] + i] );
				m_arrViewSet[i]->Show( true );
				ItemPositionSet( i, m_arrViewSet[i] );				
			}
		}
	}	
}

VOID CSkillMasterGui::FilterButtonSetting(VOID)
{
	switch( m_eCurrentTab )
	{
	case BATTLE_TAB: 
		m_pFilter[0]->Show( true );
		m_pFilter[1]->Show( true );
		m_pFilter[2]->Show( true );
		m_pFilter[3]->Show( false );

		m_pFilter[1]->SetText( std::wstring( GetDisplayStringManager()->GetString( DST_SKILL_FILTER_ACTION ) ) );
		m_pFilter[2]->SetText( std::wstring( GetDisplayStringManager()->GetString( DST_SKILL_FILTER_HTB ) ) );		
		break;
	case ASSIST_TAB:
		m_pFilter[0]->Show( true );
		m_pFilter[1]->Show( true );
		m_pFilter[2]->Show( true );
		m_pFilter[3]->Show( false );

		m_pFilter[1]->SetText( std::wstring( GetDisplayStringManager()->GetString( DST_SKILL_FILTER_SUB ) ) );
		m_pFilter[2]->SetText( std::wstring( GetDisplayStringManager()->GetString( DST_SKILL_FILTER_PASSIVE ) ) );		
		break;
	}
}

VOID CSkillMasterGui::SetFilterMode( RwInt32 nIndex )
{
	for( RwInt32 i = 0 ; i < 4 ; ++i )
	{
		if( nIndex == i )
		{
			m_pFilter[i]->Enable( false );
		}
		else
		{
			m_pFilter[i]->Enable( true );
			m_pFilter[i]->SetDown( false );		
		}
	}

	switch( m_eCurrentTab )
	{
	case BATTLE_TAB:
		m_eCurrentBattleFilter = (eBATTLESKILLFILTER)( nIndex - 1 );		// NO_BATTLE_TAB = -1.
		break;
	case ASSIST_TAB:
		m_eCurrentAssistFilter = (eASSISTSKILLFILTER)( nIndex - 1 );
		break;	
	}

	SetScrollValue();
	ShowItemDetect();
}

VOID CSkillMasterGui::ItemPositionSet( RwInt32 nIndex, CSkillMasterItemDlg* pItemDlg )
{
	pItemDlg->SetPosition( 16, ( nIndex * 75 ) + 91 );
}

VOID CSkillMasterGui::SetScrollValue(VOID)
{
	if( m_eCurrentTab == BATTLE_TAB )
	{
		if( m_eCurrentBattleFilter == NO_BATTLE_FILTER )
		{
			m_pScrollBar->SetRange( 0, m_mapBattle.size() - MAX_VIEWABLE_ITEM );
			m_pScrollBar->SetValue(	m_nTabScrollOffset[m_eCurrentTab] );
		}
		else
		{
			m_pScrollBar->SetRange( 0, m_vecBattleFilter[m_eCurrentBattleFilter].size() - MAX_VIEWABLE_ITEM );
			m_pScrollBar->SetValue(	m_nBattleFilterScrollOffset[m_eCurrentBattleFilter] );
		}
	}
	else if( m_eCurrentTab == ASSIST_TAB )
	{
		if( m_eCurrentAssistFilter == NO_BATTLE_FILTER )
		{
			m_pScrollBar->SetRange( 0, m_mapAssist.size() - MAX_VIEWABLE_ITEM );
			m_pScrollBar->SetValue( m_nTabScrollOffset[m_eCurrentTab] );
		}
		else
		{
			m_pScrollBar->SetRange( 0, m_vecAssistFilter[m_eCurrentAssistFilter].size() - MAX_VIEWABLE_ITEM );
			m_pScrollBar->SetValue(	m_nAssistFilterScrollOffset[m_eCurrentAssistFilter] );
		}		
	}	
}

VOID CSkillMasterGui::UpdateAllViewItems(VOID)
{
	for( RwInt32 i = 0 ; i < MAX_VIEWABLE_ITEM ; ++i )
	{
		if( m_arrViewSet[i]->IsShow() )
			m_arrViewSet[i]->UpdateData( m_arrViewSet[i]->GetSkillItem() );
	}
}

//VOID CSkillMasterGui::UpdateActiveSkillItem( RwInt32 nSlot )
//{
//	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
//	CNtlSkillContainer* pSkillContainer = pAvatar->GetSkillContainer();
//
//	CNtlSobSkill* pSobSkill = pSkillContainer->GetSkill( nSlot );
//
//	if( pSobSkill )
//	{
//		CNtlSobSkillAttr* pSobSkillAttr = reinterpret_cast<CNtlSobSkillAttr*>( pSobSkill->GetSobAttr() );
//		CNtlSobSkillIcon* pSobSkillIcon = reinterpret_cast<CNtlSobSkillIcon*>( pSobSkill->GetIcon() );
//		sSKILL_TBLDAT* pSkillTable = pSobSkillAttr->GetSkillTbl();
//
//		CSkillGuiItem* pItem = NULL;
//
//		if( pSkillTable->bySkill_Class == NTL_SKILL_CLASS_ACTIVE )
//		{
//			if( pSkillTable->bySkill_Type == NTL_SKILL_TYPE_PHYSICAL ||
//				pSkillTable->bySkill_Type == NTL_SKILL_TYPE_ENERGY )
//			{
//				pItem = GetSkillMapData( &m_mapBattle, pSkillTable->bySlot_Index );				
//			}
//			else // NTL_SKILL_TYPE_STATE || NTL_SKILL_TYPE_NONE
//			{
//				pItem = GetSkillMapData( &m_mapAssist, pSkillTable->bySlot_Index );				
//			}
//		}
//		else // NTL_SKILL_CLASS_PASSIVE
//		{	
//			pItem = GetSkillMapData( &m_mapAssist, pSkillTable->bySlot_Index );				
//		}		
//
//		if( pItem )
//		{
//			pItem->SetIcon( pSobSkillIcon );
//			pItem->SetData( pSkillTable );
//
//			for( RwInt32 i = 0 ; i < MAX_VIEWABLE_ITEM ; ++i )
//			{
//				if( m_arrViewSet[i]->GetSkillItem() == pItem )
//				{
//					m_arrViewSet[i]->UpdateData( pItem );
//					break;
//				}
//			}
//		}			
//	}
//}
//
//VOID CSkillMasterGui::UpdateActiveHTBItem( RwInt32 nSlot )
//{
//	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
//	CNtlSkillContainer* pSkillContainer = pAvatar->GetSkillContainer();
//
//	CNtlSobHTBSkill* pSobHTBSkill = pSkillContainer->GetHTBSkill( nSlot );
//
//	if( pSobHTBSkill )
//	{
//		CNtlSobHTBSkillAttr* pSobHTBSkillAttr = reinterpret_cast<CNtlSobHTBSkillAttr*>( pSobHTBSkill->GetSobAttr() );
//		CNtlSobHTBSkillIcon* pSobHTBSkillIcon = reinterpret_cast<CNtlSobHTBSkillIcon*>( pSobHTBSkill->GetIcon() );
//		sHTB_SET_TBLDAT* pHTBTable = pSobHTBSkillAttr->GetHTBSkillTbl();
//
//		CSkillGuiItem* pItem = NULL;
//
//		pItem = GetSkillMapData( &m_mapBattle, pHTBTable->bySlot_Index );
//		
//		if( pItem )
//		{
//			pItem->SetIcon( pSobHTBSkillIcon );
//			pItem->SetData( pHTBTable );
//
//			for( RwInt32 i = 0 ; i < MAX_VIEWABLE_ITEM ; ++i )
//			{
//				if( m_arrViewSet[i]->GetSkillItem() == pItem )
//				{
//					m_arrViewSet[i]->UpdateData( pItem );
//					break;
//				}
//			}
//		}	
//	}
//}

VOID CSkillMasterGui::HandleEvents( RWS::CMsg& msg )
{
	if( msg.Id == g_EventSobInfoUpdate )
	{
		if( IsShow() )
		{
			SNtlEventSobInfoUpdate* pUpdate = reinterpret_cast<SNtlEventSobInfoUpdate*>( msg.pData );			

			if( pUpdate->hSerialId != GetNtlSLGlobal()->GetSobAvatar()->GetSerialID() )
				return;

			if( pUpdate->uiUpdateType & ( EVENT_AIUT_SKILL_LEVELUP | EVENT_AIUT_SKILL_LEARN | EVENT_AIUT_HTBSKILL_LEARN | EVENT_AIUT_ATTR_LEVELUP ) )
			{
				UpdateAllViewItems();
			}
			else if( pUpdate->uiUpdateType & EVENT_AIUT_SKILL_SP )
			{
				SetSP();
			}
		}		
	}
	else if( msg.Id == g_EventNPCDialogOpen )
	{
		SDboNpcDialogOpen* pData = reinterpret_cast<SDboNpcDialogOpen*>( msg.pData );

		if( pData->eDialog != DIALOG_SKILL_TRAINER )
			return;

		SetItems( pData->hSerialId );

		CNtlWorldConceptNPCCOMMU* pWorldConceptController = reinterpret_cast<CNtlWorldConceptNPCCOMMU*>( GetNtlWorldConcept()->GetWorldConceptController(WORLD_PLAY_NPC_COMMU) );
		pWorldConceptController->ChangeState(WORLD_NPC_TRAINER);

		GetDialogManager()->OpenDialog( DIALOG_SKILL_TRAINER, GetNtlSLGlobal()->GetSobAvatar()->GetSerialID() );		
	}	
	else if( msg.Id == g_EventDialog )
	{
		SDboEventDialog* pPacket = (SDboEventDialog*)msg.pData;
		if( pPacket->iType == DIALOGEVENT_NPC_BYEBYE &&
			pPacket->iDestDialog == DIALOG_SKILL_TRAINER )
		{
			OnCloseButtonClicked( NULL );
		}
	}
	else if( msg.Id == g_EventCharObjDelete )
	{
		if( m_pSkillMaster )
		{
			RwUInt32* pHandle = (RwUInt32*)msg.pData;
			if( m_pSkillMaster->GetSerialID() == *pHandle )
				OnCloseButtonClicked( NULL );
		}
	}		
	else if( msg.Id == g_EventChangeClassAuthorityChangedNfy || msg.Id == g_EventSobConvertClass )
	{
		UpdateAllViewItems();
	}
}

VOID CSkillMasterGui::ShowSkillMaster(VOID)
{
	RaiseTop();
	Show( true );
	m_pTabButton->SelectTab( BATTLE_TAB );
	m_eCurrentTab = BATTLE_TAB;
	SetFilterMode( 0 );
	FilterButtonSetting();
	SetSP();
}

VOID CSkillMasterGui::HideSkillMaster(VOID)
{
	CNtlSLEventGenerator::CameraControlDelete( CAMERA_CONTROL_NPC );
	GetNtlWorldConcept()->RemoveWorldPlayConcept( WORLD_PLAY_NPC_COMMU );
	
	if( GetInfoWndManager()->GetRequestGui() == DIALOG_SKILL_TRAINER )
	{
		GetInfoWndManager()->ShowInfoWindow( FALSE );
	}

	Show( false );
	m_pSkillMaster = NULL;
}

VOID CSkillMasterGui::SetSP(VOID)
{
	m_pSP->Format( GetDisplayStringManager()->GetString( DST_SKILL_SP ), Logic_GetSp() );
}

RwInt32 CSkillMasterGui::SwitchDialog( bool bOpen )
{
	if( bOpen )
	{
		ShowSkillMaster();
	}
	else
	{
		HideSkillMaster();		
	}

	return 1;
}

VOID CSkillMasterGui::OnTabChanged( RwInt32 nIndex, RwInt32 nOldIndex )
{
	m_eCurrentTab = (eTAB)nIndex;
	FilterButtonSetting();

	if( m_eCurrentTab == BATTLE_TAB )
	{
		SetFilterMode( m_eCurrentBattleFilter + 1 );	// -1부터 시작.
	}
	else if( m_eCurrentTab == ASSIST_TAB )
	{
		SetFilterMode( m_eCurrentAssistFilter + 1 );
	}	
}

VOID CSkillMasterGui::OnCloseButtonClicked( gui::CComponent* pComponent )
{
	m_pSkillMaster = NULL;

	GetDialogManager()->CloseDialog( DIALOG_SKILL_TRAINER );			
}

VOID CSkillMasterGui::OnScrollChanged( RwInt32 nNewOffset )
{
	if( m_eCurrentTab == BATTLE_TAB )
	{
		if( m_eCurrentBattleFilter == NO_BATTLE_FILTER )
		{
			m_nTabScrollOffset[m_eCurrentTab] = nNewOffset;
		}
		else
		{
			m_nBattleFilterScrollOffset[m_eCurrentBattleFilter] = nNewOffset;
		}
	}
	else if( m_eCurrentTab == ASSIST_TAB )
	{
		if( m_eCurrentAssistFilter == NO_ASSIST_FILTER )
		{
			m_nTabScrollOffset[m_eCurrentTab] = nNewOffset;
		}
		else
		{
			m_nAssistFilterScrollOffset[m_eCurrentAssistFilter] = nNewOffset;
		}
	}

	ShowItemDetect();
}

VOID CSkillMasterGui::OnFilterBtn0Clicked( gui::CComponent* pComponent, bool bDown )
{
	SetFilterMode( 0 );
}

VOID CSkillMasterGui::OnFilterBtn1Clicked( gui::CComponent* pComponent, bool bDown )
{
	SetFilterMode( 1 );
}

VOID CSkillMasterGui::OnFilterBtn2Clicked( gui::CComponent* pComponent, bool bDown )
{
	SetFilterMode( 2 );
}

VOID CSkillMasterGui::OnFilterBtn3Clicked( gui::CComponent* pComponent, bool bDown )
{
	SetFilterMode( 3 );
}

VOID CSkillMasterGui::OnCaptureWheelMove( RwInt32 nFlag, RwInt16 sDelta, CPos& pos )
{
	CAPTURE_MOUSEWHEELMOVE_SCROLLBAR( IsShow(), m_pThis, m_pThis, m_pScrollBar, sDelta, pos );
}

VOID CSkillMasterGui::OnCaptureMouseDown( const CKey& key )
{
	CAPTURE_MOUSEDOWN_RAISE( DIALOG_SKILL_TRAINER, key.m_fX, key.m_fY );
}
