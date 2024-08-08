#include "precomp_dboclient.h"
//// core
//#include "NtlDebug.h"
//
//// presentation
//#include "NtlPLGui.h"
//#include "NtlPLGuiManager.h"
//
//// simulation
//#include "InputActionMap.h"
//#include "NtlSobManager.h"
//#include "NtlSobAvatar.h"
//#include "NtlSobAvatarAttr.h"
//#include "NtlSLGlobal.h"
//#include "NtlSkillContainer.h"
//#include "NtlSobSkill.h"
//#include "NtlSobSkillAttr.h"
//#include "NtlSobSkillIcon.h"
//#include "NtlSobHTBSkill.h"
//#include "NtlSobHTBSkillAttr.h"
//#include "NtlSobHTBSkillIcon.h"
//#include "NtlSobActionSkill.h"
//#include "NtlSobActionSkillAttr.h"
//#include "NtlSobActionSkillIcon.h"
//#include "NtlSobNpc.h"
//#include "NtlSobNpcAttr.h"
//#include "NtlSLEvent.h"
//#include "NtlSLEventFunc.h"
//#include "NtlSLApi.h"
//#include "NtlCameraController.h"
//#include "NtlWorldConceptNPCCommu.h"
//
//// Table
//#include "SkillTable.h"
//#include "HTBSetTable.h"
//#include "NPCTable.h"
//#include "PCTable.h"
//#include "MerchantTable.h"
//#include "ActionTable.h"
//#include "ChatCommandTable.h"
//#include "TextAllTable.h"
//#include "TableContainer.h"
//
//// dbo
//#include "DboDef.h"
//#include "DboGlobal.h"
//#include "DboLogic.h"
//#include "DboEvent.h"
//#include "DboEventGenerator.h"
//#include "DumpCommand.h"
//#include "SurfaceGui.h"
//#include "IconMoveManager.h"
//#include "InfoWndManager.h"
//#include "DisplayStringManager.h"
//#include "DisplayStringDef.h"
//#include "DialogManager.h"
//#include "ChatBalloonManager.h"
//
//// etc
//#include "NtlMath.h"
//#include "GUISoundDefine.h"
//
//#include "SkillWindowGui.h"
//////////////////////////////////////////////////////////////////////////////////
//// CSkillWindowItem
//CSkillWindowItemDlg::CSkillWindowItemDlg( CSkillWindowGui* pSkillWnd, gui::CTexture* pUnknownTexture )
//: m_pText( NULL ), m_pGauge( NULL ), m_pItem( NULL ), m_bLClickIcon( FALSE ), m_bRClickIcon( FALSE ),
//  m_pUnknownTexture( pUnknownTexture ), m_eMouseOn( MOUSE_NONE ), m_bPushDownIcon( FALSE ), m_bShowPickedUp( FALSE ),
//  m_pBtnUpgrade( NULL ), m_pBtnLearn( NULL ), m_bShowRQ( FALSE ), m_pTitle( NULL ), m_pbtnRPMode( NULL ),
//  m_ucColorRed( 255 ), m_ucColorGreen( 255 ), m_ucColorBlue( 255 ), m_ucAlpha( 255 ), m_fRightClickTime( 0.0f )
//{
//	gui::CSurfaceManager* pSurfaceManager = CNtlPLGuiManager::GetInstance()->GetSurfaceManager();
//	m_pSkillWnd = pSkillWnd;
//
//	// Dialog Setting
//	CRectangle rect;
//	rect.SetRectWH( 0, 0, SKILLWND_ITEM_WIDTH, SKILLWND_ITEM_HEIGHT );
//	m_pThis = NTL_NEW gui::CDialog( rect, pSkillWnd->GetDialog(), pSurfaceManager );
//	m_pThis->AddSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "SkillWnd.srf", "srfSkillDlgBack" ) );
//	NTL_ASSERT( m_pThis, "CSkillWindowItem::CSkillWindowItem, Dialog Allocate Fail" );
//
//	// Title Setting
//	rect.SetRectWH( SKILLWND_TEXT_NAME_X, SKILLWND_TEXT_NAME_Y, SKILLWND_TEXT_NAME_WIDTH, SKILLWND_TEXT_NAME_HEIGHT );
//	m_pTitle = NTL_NEW gui::CStaticBox( rect, m_pThis, pSurfaceManager, COMP_TEXT_CENTER );
//	m_pTitle->CreateFontStd( DEFAULT_FONT, DEFAULT_FONT_SIZE, 0 );
//
//	// Grade
//	rect.SetRectWH( SKILLWND_TEXT_GRADE_X, SKILLWND_TEXT_GRADE_Y, SKILLWND_TEXT_GRADE_WIDTH, SKILLWND_TEXT_GRADE_HEIGHT );
//	m_pGrade = NTL_NEW gui::CStaticBox( rect, m_pThis, pSurfaceManager, COMP_TEXT_CENTER );
//	m_pGrade->CreateFontStd( DEFAULT_FONT, DEFAULT_FONT_SIZE, 0 );
//		
//	// Icon Setting
//	m_rtIcon.SetRectWH( SKILLWND_ITEM_ICON_X, SKILLWND_ITEM_ICON_Y, SKILLWND_ITEM_ICON_WIDTH, SKILLWND_ITEM_ICON_HEIGHT );
//	m_surIcon.SetRect( m_rtIcon );
//
//	// Text Setting
//	rect.SetRectWH( SKILLWND_ITEM_TEXT_X, SKILLWND_ITEM_TEXT_Y, SKILLWND_ITEM_TEXT_WIDTH, SKILLWND_ITEM_TEXT_HEIGHT );
//	m_pText = NTL_NEW gui::CMDStaticBox( rect, m_pThis, pSurfaceManager );
//	m_pText->AddFont( "detail", DEFAULT_FONT_SIZE );
//	m_pText->Enable( false );
//	
//	// ProgressBar Back Setting
//	rect.SetRectWH( SKILLWND_ITEM_BAR_X, SKILLWND_ITEM_BAR_Y, SKILLWND_ITEM_BAR_WIDTH, SKILLWND_ITEM_BAR_HEIGHT );
//	m_pGaugeBack = NTL_NEW gui::CPanel( rect, m_pThis, pSurfaceManager, pSurfaceManager->GetSurface( SKILLWND_ITEM_SURFACE_FILE, SKILLWND_ITEM_BARBACK_SURFACE )  );
//	m_pGaugeBack->AddSurface( pSurfaceManager->GetSurface( SKILLWND_ITEM_SURFACE_FILE, SKILLWND_ITEM_BAR_TITLE ) );
//	m_pGaugeBack->Show( false );
//
//	// ProgressBar Setting
//	m_pGauge = NTL_NEW gui::CProgressBar( rect, m_pThis, pSurfaceManager, pSurfaceManager->GetSurface( SKILLWND_ITEM_SURFACE_FILE, SKILLWND_ITEM_BAR_SURFACE )  );
//	m_pGauge->SetRange( 0, 1000 );
//	m_pGauge->SetPos( 1000 );
//	m_pGauge->Show( false );
//	
//	// ProgressBar Number Setting
//	rect.SetRectWH( SKILLWND_ITEM_TEXT_EXP_X, SKILLWND_ITEM_TEXT_EXP_Y, SKILLWND_ITEM_TEXT_EXP_WIDTH, SKILLWND_ITEM_TEXT_EXP_HEIGHT );
//	m_pGaugeNumber = NTL_NEW gui::CStaticBox( rect, m_pThis, pSurfaceManager );
//	m_pGaugeNumber->CreateFontStd( "detail", SKILLWND_EXP_FONT_SIZE, 0 );
//	m_pGaugeNumber->Show( false );
//
//	// RequireImage Setting
//	m_rtRQ.SetRectWH( SKILLWND_ITEM_RQ_X, SKILLWND_ITEM_RQ_Y, SKILLWND_ITEM_RQ_WIDTH, SKILLWND_ITEM_RQ_HEIGHT );
//
//	// Skill Master용 Button Setting
//	rect.SetRectWH( SKILLWND_ITEM_BUTTON_X, SKILLWND_ITEM_BUTTON_Y, SKILLWND_ITEM_BUTTON_WIDTH, SKILLWND_ITEM_BUTTON_HEIGHT );
//	gui::CSurface srfUpgradeUp = pSurfaceManager->GetSurface( SKILLWND_ITEM_SURFACE_FILE, "srfUpgradeBtnUp" );
//	gui::CSurface srfUpgradeDown = pSurfaceManager->GetSurface( SKILLWND_ITEM_SURFACE_FILE, "srfUpgradeBtnDown" );
//	gui::CSurface srfUpgradeFoc = pSurfaceManager->GetSurface( SKILLWND_ITEM_SURFACE_FILE, "srfUpgradeBtnFoc" );
//	gui::CSurface srfLearnUp = pSurfaceManager->GetSurface( SKILLWND_ITEM_SURFACE_FILE, "srfLearnBtnUp" );
//	gui::CSurface srfLearnDown = pSurfaceManager->GetSurface( SKILLWND_ITEM_SURFACE_FILE, "srfLearnBtnDown" );
//	gui::CSurface srfLearnFoc = pSurfaceManager->GetSurface( SKILLWND_ITEM_SURFACE_FILE, "srfLearnBtnFoc" );
//
//	m_pBtnUpgrade = NTL_NEW gui::CButton( rect, "", m_pThis, pSurfaceManager );
//	m_pBtnLearn = NTL_NEW gui::CButton( rect, "", m_pThis, pSurfaceManager );
//
//	m_pBtnUpgrade->AddSurfaceUp( srfUpgradeUp );
//	m_pBtnUpgrade->AddSurfaceFocus( srfUpgradeFoc );
//	m_pBtnUpgrade->AddSurfaceDown( srfUpgradeDown );
//	m_pBtnUpgrade->Show( false );
//	m_pBtnLearn->AddSurfaceUp( srfLearnUp );
//	m_pBtnLearn->AddSurfaceFocus( srfLearnFoc );
//	m_pBtnLearn->AddSurfaceDown( srfLearnDown );
//	m_pBtnLearn->Show( false );	
//
//	// Help Text
//	rect.SetRectWH( SKILLWND_TEXT_HELP_X, SKILLWND_TEXT_HELP_Y, SKILLWND_TEXT_HELP_WIDTH, SKILLWND_TEXT_HELP_HEIGHT );
//	m_pHelpText = NTL_NEW gui::CStaticBox( rect, m_pThis, pSurfaceManager, COMP_TEXT_CENTER );
//	m_pHelpText->CreateFontStd( "detail", DEFAULT_FONT_SIZE, 0 );
//	m_pHelpText->Show( true );
//
//	// Consume Title
//	rect.SetRectWH( SKILLWND_TEXT_CONSUME_TEXT_X, SKILLWND_TEXT_CONSUME_TEXT_Y, SKILLWND_TEXT_CONSUME_TEXT_WIDTH, SKILLWND_TEXT_CONSUME_TEXT_HEIGHT );
//	m_pConsumeTitle = NTL_NEW gui::CStaticBox( rect, m_pThis, pSurfaceManager, COMP_TEXT_RIGHT );
//	m_pConsumeTitle->CreateFontStd( "detail", DEFAULT_FONT_SIZE, 0 );
//	m_pConsumeTitle->Show( true );
//
//	// RPMode 버튼
//	rect.SetRectWH( SKILLWND_ITEM_RPMODE_BUTTON_X, SKILLWND_ITEM_RPMODE_BUTTON_Y, SKILLWND_ITEM_RPMODE_BUTTON_WIDTH, SKILLWND_ITEM_RPMODE_BUTTON_HEIGHT );
//	m_pbtnRPMode = NTL_NEW gui::CButton( rect, "", m_pThis, pSurfaceManager );
//	m_pbtnRPMode->AddSurfaceUp( pSurfaceManager->GetSurface( SKILLWND_ITEM_SURFACE_FILE, "srfRPModeBtnUp" ) );
//	m_pbtnRPMode->AddSurfaceFocus( pSurfaceManager->GetSurface( SKILLWND_ITEM_SURFACE_FILE, "srfRPModeBtnFocus" ) );
//	m_pbtnRPMode->AddSurfaceDown( pSurfaceManager->GetSurface( SKILLWND_ITEM_SURFACE_FILE, "srfRPModeBtnDown" ) );
//	m_pbtnRPMode->SetToolTip( GetDisplayStringManager()->GetString( DST_SKILL_RPMODE ) );
//	m_pbtnRPMode->Show( false );
//
//	// RPType
//	m_surRPType.SetRect( 0, 0, SKILLWND_ITEM_RPTYPE_WIDTH, SKILLWND_ITEM_RPTYPE_HEIGHT );
//	m_surRPType.SetPositionfromParent( SKILLWND_ITEM_RPTYPE_X, SKILLWND_ITEM_RPTYPE_Y );
//
//	// Effect
//	m_CoolTimeEffect.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfSlotGrayedEffect" ) );
//	m_CoolTimeEffect.SetPosition( m_rtIcon.left, m_rtIcon.top );
//	m_surFocusEffect.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfSlotFocusEffect" ) );
//	m_surFocusEffect.SetRect( m_rtIcon.left, m_rtIcon.top, m_rtIcon.right, m_rtIcon.bottom );
//
//	// Surface Setting
//	m_pThis->Show( false );
//
//	//
//	m_surPickedUp.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfSlotGrayedEffect" ) );
//	m_surPickedUp.SetPosition( m_rtIcon.left, m_rtIcon.top );
//	
//	// CallBack
//	m_slotMouseDown = m_pThis->SigMouseDown().Connect( this, &CSkillWindowItemDlg::OnMouseDown );
//	m_slotMouseUp	= m_pThis->SigMouseUp().Connect( this, &CSkillWindowItemDlg::OnMouseUp );
//	m_slotMouseMove	= m_pThis->SigMouseMove().Connect( this, &CSkillWindowItemDlg::OnMouseMove );
//	m_slotPaint		= m_pThis->SigPaint().Connect( this, &CSkillWindowItemDlg::OnPaint );
//	m_slotMove		= m_pThis->SigMove().Connect( this, &CSkillWindowItemDlg::OnMove );
//	m_slotMouseLeave= m_pThis->SigMouseLeave().Connect( this, &CSkillWindowItemDlg::OnMouseLeave );
//	
//	m_slotLearnButtonEnter = m_pBtnLearn->SigMouseEnter().Connect( this, &CSkillWindowItemDlg::OnLearnButtonEnter );
//	m_slotLearnButtonLeave = m_pBtnLearn->SigMouseLeave().Connect( this, &CSkillWindowItemDlg::OnLearnButtonLeave );
//	m_slotUpgradeButtonEnter = m_pBtnUpgrade->SigMouseEnter().Connect( this, &CSkillWindowItemDlg::OnUpgradeButtonEnter );
//	m_slotUpgradeButtonLeave = m_pBtnUpgrade->SigMouseLeave().Connect( this, &CSkillWindowItemDlg::OnUpgradeButtonLeave );
//	m_slotLearnButtonClick = m_pBtnLearn->SigClicked().Connect( this, &CSkillWindowItemDlg::OnLearnButtonClick );
//	m_slotUpgradeButtonClick = m_pBtnUpgrade->SigClicked().Connect( this, &CSkillWindowItemDlg::OnUpgradeButtonClick );
//	m_slotRPModeClick = m_pbtnRPMode->SigClicked().Connect( this, &CSkillWindowItemDlg::OnRPModeClick );
//}
//
//CSkillWindowItemDlg::~CSkillWindowItemDlg(VOID)
//{
//	if( m_pThis )
//	{
//		NTL_DELETE( m_pThis );
//	}
//}
//
//VOID CSkillWindowItemDlg::Update( RwReal fElapsedTime )
//{
//	CNtlSobIcon* pIcon = m_pItem->GetIcon();
//	if( pIcon->GetIconState() == CNtlSobIcon::ICON_STATE_COOLING )
//	{
//		m_CoolTimeEffect.Update( pIcon->GetCoolingTime() );
//	}					
//
//	if( m_bRClickIcon )
//	{
//		m_fRightClickTime += fElapsedTime;
//
//		if( fElapsedTime > SKILL_RPBONUS_KEYTIME )
//		{
//			if( Logic_UseProcRpBonusSkill( m_pItem->GetIcon()->GetSobObj()->GetSerialID() ) )
//				m_bRClickIcon = FALSE;
//
//			m_fRightClickTime = 0.0f;
//		}
//	}
//}
//
//VOID CSkillWindowItemDlg::UpdateData( CSkillGuiItem* pItem )
//{
//	switch( pItem->GetType() )
//	{
//	case CSkillGuiItem::SKILL: // Battle Skill
//		UpdateSkillData( pItem );
//		break;
//	case CSkillGuiItem::HTB: // HTB Skill
//		UpdateHTBData( pItem );
//		break;
//	case CSkillGuiItem::ACTION: // Action Skill
//		UpdateActionData( pItem );
//		break;
//	}
//
//	if( m_pHelpText->GetFitWidthToString() > m_pHelpText->GetWidth() )
//		m_pHelpText->SetToolTip( m_pHelpText->GetText() );
//	else
//		m_pHelpText->DeleteToolTip();
//}
//
//VOID CSkillWindowItemDlg::SetColor( RwUInt8 ucRed, RwUInt8 ucGreen, RwUInt8 ucBlue )
//{
//	m_ucColorRed = ucRed;
//	m_ucColorGreen = ucGreen;
//	m_ucColorBlue = ucBlue;
//
//	m_pThis->SetColor( ucRed, ucGreen, ucBlue );
//
//	m_surIcon.SetColorOnly( ucRed, ucGreen, ucBlue );
//}
//
//VOID CSkillWindowItemDlg::SetAlpha( RwUInt8 ucAlpha )
//{
//	m_ucAlpha = ucAlpha;
//
//	m_pThis->SetAlpha( ucAlpha );
//
//	m_surIcon.SetAlpha( ucAlpha );
//}
//
//VOID CSkillWindowItemDlg::StartCoolTime(VOID)
//{
//	if( m_pItem->IsUseable() )
//		m_CoolTimeEffect.StartProc( m_pItem->GetIcon()->GetMaxCoolingTime() );
//}
//
//VOID CSkillWindowItemDlg::EndCoolTime(VOID)
//{
//	m_CoolTimeEffect.EndProc();
//}
//
//VOID CSkillWindowItemDlg::UpdateSkillData( CSkillGuiItem* pItem )
//{
//	RwUInt32 uHelpColor;
//	RwUInt8  ucRed, ucGreen, ucBlue, ucAlpha;
//	WCHAR*	 pText = NULL;
//	WCHAR*	 pString = NULL;
//
//	CRectangle rtScreen = m_pThis->GetScreenRect();
//
//	m_pItem = pItem;
//	m_flagRQState = SRQ_FLAG_INITIAL;
//
//	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
//	if( !pAvatar )
//		return;
//	CNtlSobAvatarAttr* pAvatarAttr = reinterpret_cast<CNtlSobAvatarAttr*>( pAvatar->GetSobAttr() );
//	CNtlSkillContainer* pSkillContainer = pAvatar->GetSkillContainer();
//	sSKILL_TBLDAT* pSkillData = reinterpret_cast<sSKILL_TBLDAT*>( m_pItem->GetData() );
//
//	m_pGauge->Show( false );	
//	m_pGaugeBack->Show( false );
//	m_pGaugeNumber->Show( false );
//
//	m_pBtnLearn->Show( false );
//	m_pBtnUpgrade->Show( false );
//	m_pbtnRPMode->Show( false );
//
//	// Active Skill Update
//	if( pItem->IsUseable() )
//	{
//		CNtlSobSkillAttr* pSkillAttr = reinterpret_cast<CNtlSobSkillAttr*>( pItem->GetIcon()->GetSobObj()->GetSobAttr() );
//		
//		m_pGauge->SetRange( 0, pSkillAttr->m_uiMaxExp );
//		m_pGauge->SetPos( pSkillAttr->m_uiExp );
//
//		m_surIcon.UnsetTexture();
//		m_surIcon.SetTexture( reinterpret_cast<gui::CTexture*>( pItem->GetIcon()->GetImage() ) );
//
//		// RPMode버튼
//		for( RwInt32 i = 0 ; i < DBO_MAX_RP_BONUS_COUNT_PER_SKILL ; ++i )
//		{
//			if( pSkillData->abyRpEffect[i] != INVALID_BYTE )
//			{
//				m_pbtnRPMode->Show( true );
//				break;
//			}
//		}
//
//		// RPType
//		SetRPTypeImage( pSkillAttr->m_bRPBonusAutoMode, pSkillAttr->m_byRPBonusType );		
//
//		// RQ State
//		if( pSkillData->dwNextSkillTblidx != INVALID_TBLIDX )
//		{
//			RwReal	fValue = (RwReal)pSkillAttr->m_uiExp / pSkillAttr->m_uiMaxExp * 100;
//			RwBool	bNextRQLevel = FALSE;
//			RwBool	bRQSKillExp = FALSE;
//
//			CSkillTable* pSkillTable = API_GetTableContainer()->GetSkillTable();
//			sSKILL_TBLDAT* pNextSkillData = reinterpret_cast<sSKILL_TBLDAT*>( pSkillTable->FindData( pSkillData->dwNextSkillTblidx ) );
//
//			if( pAvatarAttr->GetLevel() >= pNextSkillData->byRequire_Train_Level )
//				bNextRQLevel = TRUE;
//			else
//			{
//				bNextRQLevel = FALSE;
//				m_flagRQState |= SRQ_FLAG_NOT_UPGRADE_LEVEL;
//			}
//
//			if( fValue >= 100.0f )
//				bRQSKillExp = TRUE;	
//			else
//			{
//				bRQSKillExp = FALSE;
//				m_flagRQState |= SRQ_FLAG_NOT_UPGRADE_SKILLEXP;
//			}
//
//			if( bRQSKillExp && bNextRQLevel )
//			{
//				pText = const_cast<WCHAR*>( GetDisplayStringManager()->GetString( DST_SKILL_READY_UPGRADE ) );
//				m_flagRQState = SRQ_FLAG_CAN_UPGRADE;
//			}
//			else
//			{
//				pText = L"";
//				m_pGaugeNumber->Format( "%.0f%%", fValue );
//				m_pGaugeNumber->Show( true );
//				m_pGauge->Show( true );
//				m_pGaugeBack->Show( true );
//			}
//			uHelpColor = SKILLWND_COLOR_CANUPGRADE;			
//		}
//		else
//		{
//			pText = const_cast<WCHAR*>( GetDisplayStringManager()->GetString( DST_SKILL_MASTERED ) );
//			uHelpColor = SKILLWND_COLOR_MASTERED;
//			m_flagRQState = SRQ_FLAG_MASTER;			
//		}
//
//		// 쿨타임 검사
//		if( m_pItem->GetIcon()->GetIconState() == CNtlSobIcon::ICON_STATE_COOLING )
//			StartCoolTime();
//		else
//			EndCoolTime();
//
//		ucRed = SKILLWND_ITEM_LEARN_COLOR_RED;
//		ucGreen = SKILLWND_ITEM_LEARN_COLOR_GREEN;
//		ucBlue = SKILLWND_ITEM_LEARN_COLOR_BLUE;
//		ucAlpha = SKILLWND_ITEM_LEARN_ALPHA;		
//	}
//	// InActive Skill Update
//	else
//	{
//		SetRPTypeImage( FALSE );
//
//		m_surIcon.UnsetTexture();
//		m_surIcon.SetTexture( m_pUnknownTexture );
//
//		// RQState
//		if( CanLearnSKill( pAvatarAttr->GetLevel() ) )
//		{
//			pText = const_cast<WCHAR*>( GetDisplayStringManager()->GetString( DST_SKILL_READY_LEARN ) );
//			uHelpColor = SKILLWND_COLOR_CANLEARN;
//			
//			ucRed = SKILLWND_ITEM_LEARN_COLOR_RED;
//			ucGreen = SKILLWND_ITEM_LEARN_COLOR_GREEN;
//			ucBlue = SKILLWND_ITEM_LEARN_COLOR_BLUE;
//			ucAlpha = SKILLWND_ITEM_LEARN_ALPHA;		
//		}
//		else
//		{
//			pText = const_cast<WCHAR*>( GetDisplayStringManager()->GetString( DST_SKILL_NOT_LEARN ) );
//			uHelpColor = SKILLWND_COLOR_NOTLEARN;
//
//			ucRed = SKILLWND_ITEM_NOT_LEARN_COLOR_RED;
//			ucGreen = SKILLWND_ITEM_NOT_LEARN_COLOR_GREEN;
//			ucBlue = SKILLWND_ITEM_NOT_LEARN_COLOR_BLUE;
//			ucAlpha = SKILLWND_ITEM_NOT_LEARN_ALPHA;		
//		}
//
//		// 쿨타임
//		EndCoolTime();
//	}
//
//	// InfoText 출력
//	m_pTitle->SetText( API_GetTableContainer()->GetTextAllTable()->GetSkillTbl()->GetText( pSkillData->Skill_Name ).c_str() );
//	
//	pString = const_cast<WCHAR*>( GetDisplayStringManager()->GetString( DST_SKILL_GRADE ) );
//	m_pGrade->Format( pString, pSkillData->bySkill_Grade );
//
//	m_pConsumeTitle->Show( true );
//	m_pConsumeTitle->SetText( GetDisplayStringManager()->GetString( DST_SKILL_CONSUME_TEXT ) );
//
//	m_pText->Clear();
//
//	m_pText->SetItemXY( "EP", "EP", 0, SKILLWND_TEXT_REQUIRE1_TEXT_X, SKILLWND_TEXT_REQUIRE_Y, 0, SKILLWND_COLOR_EP );
//	m_pText->SetItemXY( pSkillData->wRequire_EP, "EPValue", 0, SKILLWND_TEXT_REQUIRE1_VALUE_X, SKILLWND_TEXT_REQUIRE_Y, 0, SKILLWND_COLOR_CONSUME_TEXT );
//
//	if( pSkillData->wRequire_LP )
//	{
//		m_pText->SetItemXY( "LP", "LP", 0, SKILLWND_TEXT_REQUIRE2_TEXT_X, SKILLWND_TEXT_REQUIRE_Y, 0, SKILLWND_COLOR_LP );
//		m_pText->SetItemXY( pSkillData->wRequire_LP, "LPValue", 0, SKILLWND_TEXT_REQUIRE2_VALUE_X, SKILLWND_TEXT_REQUIRE_Y, 0, SKILLWND_COLOR_CONSUME_TEXT );		
//	}
//	
//	m_bShowRQ = CanRQDisplay();
//
//	//
//	if( !m_bShowRQ )
//	{
//		if( m_flagRQState == SRQ_FLAG_CAN_LEARN_BY_SELF )
//			m_pBtnLearn->Show( true );
//		else if( m_flagRQState == SRQ_FLAG_CAN_UPGRADE )
//			m_pBtnUpgrade->Show( true );		
//	}
//	
//	m_pHelpText->SetTextColor( uHelpColor );
//	m_pHelpText->SetText( pText );
//			
//	m_pText->DrawItem();
//
//	SetColor( ucRed, ucGreen, ucBlue );
//	SetAlpha( ucAlpha );	
//	SetRQSurface( FALSE );
//
//	// InfoWndUpdate
//	if( m_eMouseOn == MOUSE_ONRQ && GetInfoWndManager()->GetInfoWndState() == CInfoWndManager::INFOWND_SKILLRQ )
//	{
//		GetInfoWndManager()->ShowInfoWindow( TRUE, CInfoWndManager::INFOWND_SKILLRQ, m_rtRQ.left + rtScreen.left, m_rtRQ.top + rtScreen.top, (VOID*)m_flagRQState, DIALOG_SKILL );
//	}
//
//	if(  m_eMouseOn == MOUSE_ONICON &&
//		( GetInfoWndManager()->GetInfoWndState() == CInfoWndManager::INFOWND_SKILL ||
//		  GetInfoWndManager()->GetInfoWndState() == CInfoWndManager::INFOWND_NOTLEARN_SKILL ) )
//	{
//		if( m_pItem->IsUseable() )
//		{
//			GetInfoWndManager()->ShowInfoWindow( TRUE, CInfoWndManager::INFOWND_SKILL, m_rtIcon.left + rtScreen.left, m_rtIcon.top + rtScreen.top, m_pItem->GetIcon()->GetSobObj(), DIALOG_SKILL );
//		}
//		else
//		{
//			GetInfoWndManager()->ShowInfoWindow( TRUE, CInfoWndManager::INFOWND_NOTLEARN_SKILL, m_rtIcon.left + rtScreen.left, m_rtIcon.top + rtScreen.top, pSkillData, DIALOG_SKILL );
//		}
//	}
//}
//
//VOID CSkillWindowItemDlg::UpdateHTBData( CSkillGuiItem* pItem )
//{
//	RwUInt32 uHelpColor;
//	RwUInt8	 ucRed, ucGreen, ucBlue, ucAlpha;
//	WCHAR*	 pText = NULL;
//	WCHAR*	 pString = NULL;
//
//	CRectangle rtScreen = m_pThis->GetScreenRect();
//
//	m_pItem = pItem;
//	m_flagRQState = SRQ_FLAG_INITIAL;
//
//	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
//	if( !pAvatar )
//		return;
//	CNtlSobAvatarAttr* pAvatarAttr = reinterpret_cast<CNtlSobAvatarAttr*>( pAvatar->GetSobAttr() );
//	CNtlSkillContainer* pSkillContainer = pAvatar->GetSkillContainer();
//	sHTB_SET_TBLDAT* pHTBData = reinterpret_cast<sHTB_SET_TBLDAT*>( m_pItem->GetData() );
//
//	m_pGauge->Show( false );	
//	m_pGaugeBack->Show( false );
//	m_pGaugeNumber->Show( false );
//
//	m_pBtnLearn->Show( false );
//	m_pBtnUpgrade->Show( false );
//	m_pbtnRPMode->Show( false );
//
//	SetRPTypeImage( FALSE );
//	
//	// Active Skill Update
//	if( pItem->IsUseable() )
//	{
//		CNtlSobHTBSkillAttr* pSkillAttr = reinterpret_cast<CNtlSobHTBSkillAttr*>( pItem->GetIcon()->GetSobObj()->GetSobAttr() );
//
//		m_surIcon.UnsetTexture();
//		m_surIcon.SetTexture( reinterpret_cast<gui::CTexture*>( pItem->GetIcon()->GetImage() ) );
//
//		// RQ State
//		pText = const_cast<WCHAR*>( GetDisplayStringManager()->GetString( DST_SKILL_MASTERED ) );
//		uHelpColor = SKILLWND_COLOR_MASTERED;
//		m_flagRQState = SRQ_FLAG_MASTER;
//
//		// 쿨타임 검사
//		if( m_pItem->GetIcon()->GetIconState() == CNtlSobIcon::ICON_STATE_COOLING )
//			StartCoolTime();
//		else
//			EndCoolTime();
//
//		ucRed = SKILLWND_ITEM_LEARN_COLOR_RED;
//		ucGreen = SKILLWND_ITEM_LEARN_COLOR_GREEN;
//		ucBlue = SKILLWND_ITEM_LEARN_COLOR_BLUE;
//		ucAlpha = SKILLWND_ITEM_LEARN_ALPHA;		
//	}
//	// InActive Skill Update
//	else
//	{
//		m_surIcon.UnsetTexture();
//		m_surIcon.SetTexture( m_pUnknownTexture );
//
//		// RQState
//		if( CanLearnHTBSkill( pAvatarAttr->GetLevel() ) )
//		{
//			pText = const_cast<WCHAR*>( GetDisplayStringManager()->GetString( DST_SKILL_READY_LEARN ) );
//			uHelpColor = SKILLWND_COLOR_CANLEARN;
//			
//			ucRed = SKILLWND_ITEM_LEARN_COLOR_RED;
//			ucGreen = SKILLWND_ITEM_LEARN_COLOR_GREEN;
//			ucBlue = SKILLWND_ITEM_LEARN_COLOR_BLUE;
//			ucAlpha = SKILLWND_ITEM_LEARN_ALPHA;		
//		}
//		else
//		{
//			pText = const_cast<WCHAR*>( GetDisplayStringManager()->GetString( DST_SKILL_NOT_LEARN ) );
//			uHelpColor = SKILLWND_COLOR_NOTLEARN;
//
//			ucRed = SKILLWND_ITEM_NOT_LEARN_COLOR_RED;
//			ucGreen = SKILLWND_ITEM_NOT_LEARN_COLOR_GREEN;
//			ucBlue = SKILLWND_ITEM_NOT_LEARN_COLOR_BLUE;
//			ucAlpha = SKILLWND_ITEM_NOT_LEARN_ALPHA;		
//		}
//
//		// 쿨타임
//		EndCoolTime();		
//	}
//
//	// InfoText 출력
//	m_pTitle->SetText( API_GetTableContainer()->GetTextAllTable()->GetHTBSetTbl()->GetText( pHTBData->HTB_Skill_Name ).c_str() );
//
//	m_pGrade->SetText( GetDisplayStringManager()->GetString( DST_SKILL_TYPE_HTB ) );
//
//	m_pConsumeTitle->Show( true );
//	m_pConsumeTitle->SetText( GetDisplayStringManager()->GetString( DST_SKILL_CONSUME_TEXT ) );
//
//	m_pText->Clear();
//
//	m_pText->SetItemXY( "RP", "RP", 0, SKILLWND_TEXT_REQUIRE1_TEXT_X, SKILLWND_TEXT_REQUIRE_Y, 0, SKILLWND_COLOR_RP );
//	//m_pText->SetItemXY( pHTBData->wNeed_EP, "EPValue", 0, SKILLWND_TEXT_REQUIRE1_VALUE_X, SKILLWND_TEXT_REQUIRE_Y, 0, SKILLWND_COLOR_CONSUME_TEXT );
//
//	m_bShowRQ = CanRQDisplay();
//
//	if( !m_bShowRQ )
//	{
//		m_pBtnLearn->Show( true );	
//	}
//	
//	m_pHelpText->SetTextColor( uHelpColor );
//	m_pHelpText->SetText( pText );		
//	
//	m_pText->DrawItem();
//
//	SetColor( ucRed, ucGreen, ucBlue );
//	SetAlpha( ucAlpha );			
//	SetRQSurface( FALSE );
//
//	// InfoWndUpdate
//	if( m_eMouseOn == MOUSE_ONRQ && GetInfoWndManager()->GetInfoWndState() == CInfoWndManager::INFOWND_SKILLRQ )
//	{
//		GetInfoWndManager()->ShowInfoWindow( TRUE,CInfoWndManager::INFOWND_SKILLRQ, m_rtRQ.left + rtScreen.left, m_rtRQ.top + rtScreen.top, (VOID*)m_flagRQState, DIALOG_SKILL );
//	}
//
//	if( m_eMouseOn == MOUSE_ONICON &&
//		( GetInfoWndManager()->GetInfoWndState() == CInfoWndManager::INFOWND_HTBSKILL ||
//		  GetInfoWndManager()->GetInfoWndState() == CInfoWndManager::INFOWND_NOTLEARN_HTBSKILL ) )
//	{
//		if( m_pItem->IsUseable() )
//		{
//			GetInfoWndManager()->ShowInfoWindow( TRUE, CInfoWndManager::INFOWND_HTBSKILL, m_rtIcon.left + rtScreen.left, m_rtIcon.top + rtScreen.top, m_pItem->GetIcon()->GetSobObj(), DIALOG_SKILL );
//		}
//		else
//		{
//			GetInfoWndManager()->ShowInfoWindow( TRUE, CInfoWndManager::INFOWND_NOTLEARN_HTBSKILL, m_rtIcon.left + rtScreen.left, m_rtIcon.top + rtScreen.top, m_pItem->GetData(), DIALOG_SKILL );
//		}
//	}
//}
//
//VOID CSkillWindowItemDlg::UpdateActionData( CSkillGuiItem* pItem )
//{
//	WCHAR*	 pText = NULL;
//	WCHAR*	 pString = NULL;
//
//	CRectangle rtScreen = m_pThis->GetScreenRect();
//
//	m_pItem = pItem;
//	m_flagRQState = SRQ_FLAG_INITIAL;
//
//	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
//	if( !pAvatar )
//		return;
//
//	sACTION_TBLDAT* pActionData = reinterpret_cast<sACTION_TBLDAT*>( m_pItem->GetData() );	
//	CChatCommandTable* pChatCommandTable = API_GetTableContainer()->GetChatCommandTable();
//	
//	m_pGauge->Show( false );	
//	m_pGaugeBack->Show( false );
//	m_pGaugeNumber->Show( false );
//
//	m_pBtnLearn->Show( false );
//	m_pBtnUpgrade->Show( false );
//	m_pbtnRPMode->Show( false );
//
//	m_pConsumeTitle->Show( false );
//
//	SetRPTypeImage( FALSE );
//	
//	m_bShowRQ = FALSE;
//
//	// Active Skill Update
//	m_surIcon.UnsetTexture();
//	m_surIcon.SetTexture( reinterpret_cast<gui::CTexture*>( pItem->GetIcon()->GetImage() ) );
//	
//	// 쿨타임 검사
//	if( m_pItem->GetIcon()->GetIconState() == CNtlSobIcon::ICON_STATE_COOLING )
//		StartCoolTime();
//	else
//		EndCoolTime();
//
//	SetColor( SKILLWND_ITEM_LEARN_COLOR_RED, SKILLWND_ITEM_LEARN_COLOR_GREEN, SKILLWND_ITEM_LEARN_COLOR_BLUE );
//	SetAlpha( SKILLWND_ITEM_LEARN_ALPHA );
//	
//	// InfoText 출력
//	CTextTable* pActionText = API_GetTableContainer()->GetTextAllTable()->GetActionTbl();
//	CTextTable* pChatCommandText = API_GetTableContainer()->GetTextAllTable()->GetChatCommandTbl();
//
//	m_pTitle->SetText( pActionText->GetText( pActionData->Action_Name ).c_str() );
//
//	m_pText->Clear();
//	
//	if( pActionData->byAction_Type == ACTION_TYPE_SOCIAL_ACTION )
//	{
//		sCHAT_COMMAND_TBLDAT* pChatCommandData = reinterpret_cast<sCHAT_COMMAND_TBLDAT*>( pChatCommandTable->FindData( pActionData->chat_Command_Index ) );
//
//		std::wstring strBuf;
//		std::wstring wstrChatCommand;
//		RwInt32 i;
//		for( i = 0 ; i < 3 ; ++i )	
//		{
//			wstrChatCommand = pChatCommandText->GetText( pChatCommandData->aChat_Command[i] );
//			if( wstrChatCommand.empty() )
//				break;
//
//			if( i > 0 ) 
//				strBuf += L", ";
//
//			strBuf += L"/";
//			strBuf += wstrChatCommand;
//		}	
//		if( !wstrChatCommand.empty() )
//			strBuf += L"...";
//
//		m_pText->SetItemXY( strBuf.c_str(), "ChatCommand", 0, SKILLWND_TEXT_ACTIONCOMMAND_X, SKILLWND_TEXT_REQUIRE_Y, 0, SKILLWND_COLOR_ACTION_COMMANDS );		
//		pString = const_cast<WCHAR*>( GetDisplayStringManager()->GetString( DST_SKILL_TEXT_EMOTION ) );
//	}
//	else if( pActionData->byAction_Type == ACTION_TYPE_NORMAL_ACTION )
//		pString = const_cast<WCHAR*>( GetDisplayStringManager()->GetString( DST_SKILL_TEXT_ACTION ) );
//
//	m_pGrade->SetText( pString );
//		
//	m_pHelpText->SetTextColor( SKILLWND_COLOR_ACTION_TEXT );
//	m_pHelpText->SetText( pActionText->GetText( pActionData->Note ).c_str() );
//	
//	m_pText->DrawItem();
//
//	// InfoWndUpdate
//	if( m_eMouseOn == MOUSE_ONICON && GetInfoWndManager()->GetInfoWndState() == CInfoWndManager::INFOWND_ACTIONSKILL )
//	{
//		GetInfoWndManager()->ShowInfoWindow( TRUE, CInfoWndManager::INFOWND_ACTIONSKILL, m_rtIcon.left + rtScreen.left, m_rtIcon.top + rtScreen.top, m_pItem->GetIcon()->GetSobObj(), DIALOG_SKILL );
//	}
//}
//
//VOID CSkillWindowItemDlg::SetRQSurface( RwBool bFocus )
//{
//	if( bFocus )
//	{
//		if( m_flagRQState & SRQ_FLAG_NOT_LEARN_LEVEL ||	m_flagRQState & SRQ_FLAG_NOT_LEARN_SKILL || m_flagRQState & SRQ_FLAG_NOT_LEARN_HTB )
//			m_surRQ.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "SkillWnd.srf", "srfIconSkillLearnConditionFoc" ) );
//		else if( m_flagRQState & SRQ_FLAG_CAN_LEARN_BY_SKILLMASTER )
//			m_surRQ.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "SkillWnd.srf", "srfIconSkillLearnFoc" ) );
//		else if( m_flagRQState & SRQ_FLAG_NOT_UPGRADE_LEVEL || m_flagRQState & SRQ_FLAG_NOT_UPGRADE_SKILLEXP )
//			m_surRQ.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "SkillWnd.srf", "srfIconSkillUpgradeConditionFoc" ) );
//		else if( m_flagRQState & SRQ_FLAG_MASTER )
//			m_surRQ.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "SkillWnd.srf", "srfIconSkillMasterFoc" ) );
//		else
//			return;
//	}
//	else
//	{
//		if( m_flagRQState & SRQ_FLAG_NOT_LEARN_LEVEL ||	m_flagRQState & SRQ_FLAG_NOT_LEARN_SKILL || m_flagRQState & SRQ_FLAG_NOT_LEARN_HTB )
//			m_surRQ.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "SkillWnd.srf", "srfIconSkillLearnCondition" ) );
//		else if( m_flagRQState & SRQ_FLAG_CAN_LEARN_BY_SKILLMASTER )
//			m_surRQ.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "SkillWnd.srf", "srfIconSkillLearn" ) );
//		else if( m_flagRQState & SRQ_FLAG_NOT_UPGRADE_LEVEL || m_flagRQState & SRQ_FLAG_NOT_UPGRADE_SKILLEXP )
//			m_surRQ.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "SkillWnd.srf", "srfIconSkillUpgradeCondition" ) );
//		else if( m_flagRQState & SRQ_FLAG_MASTER )
//			m_surRQ.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "SkillWnd.srf", "srfIconSkillMaster" ) );
//		else
//			return;
//	}
//	
//	CRectangle rtScreen = m_pThis->GetScreenRect();
//	m_surRQ.SetPosition( rtScreen.left + m_rtRQ.left, rtScreen.top + m_rtRQ.top );
//	m_surRQ.SetColor( m_ucColorRed, m_ucColorGreen, m_ucColorBlue, m_ucAlpha );	
//}
//
//VOID CSkillWindowItemDlg::SetRPTypeImage( RwBool bAutoMode, RwInt32 eRPType )
//{
//	if( bAutoMode )
//	{
//		m_surRPType.SetSurface( Logic_GetSmallRPTypeMark( eRPType ) );
//		m_surRPType.Show( TRUE );
//
//		CRectangle rect = m_pThis->GetScreenRect();
//		m_surRPType.SetPositionbyParent( rect.left, rect.top );
//	}
//	else
//	{
//		m_surRPType.Show( FALSE );
//	}
//}
//
//RwBool CSkillWindowItemDlg::CanLearnSKill( RwUInt8 ucAvatarLevel )
//{
//	sSKILL_TBLDAT* pData = reinterpret_cast<sSKILL_TBLDAT*>( m_pItem->GetData() );
//
//	if( ucAvatarLevel < pData->byRequire_Train_Level )
//		m_flagRQState |= SRQ_FLAG_NOT_LEARN_LEVEL;
//
//	if( pData->uiRequire_Skill_Tblidx_Min_1 == INVALID_TBLIDX ||
//		pData->uiRequire_Skill_Tblidx_Max_1 == INVALID_TBLIDX )
//	{
//		if( !( m_flagRQState & SRQ_FLAG_NOT_LEARN_LEVEL ) )
//		{
//			if( pData->bSelfTrain )
//				m_flagRQState = SRQ_FLAG_CAN_LEARN_BY_SELF;
//			else
//				m_flagRQState = SRQ_FLAG_CAN_LEARN_BY_SKILLMASTER;
//
//			return TRUE;
//		}
//		else
//			return FALSE;
//	}
//
//	CNtlSkillContainer* pSkillContainer = GetNtlSLGlobal()->GetSobAvatar()->GetSkillContainer();
//
//	for( RwInt32 i = 0 ; i < NTL_MAX_PC_HAVE_SKILL ; ++i )
//	{
//		CNtlSobSkill* pSkill = pSkillContainer->GetSkill( i );
//		if( !pSkill )
//			continue;
//
//		CNtlSobSkillAttr* pSkillAttr = reinterpret_cast<CNtlSobSkillAttr*>( pSkill->GetSobAttr() );
//		
//		if( pData->uiRequire_Skill_Tblidx_Min_1 >= pSkillAttr->GetSkillTbl()->tblidx &&
//			pData->uiRequire_Skill_Tblidx_Max_1 <= pSkillAttr->GetSkillTbl()->tblidx )
//		{
//			if( !( m_flagRQState & SRQ_FLAG_NOT_LEARN_LEVEL ) )
//			{
//				if( pData->bSelfTrain )
//					m_flagRQState = SRQ_FLAG_CAN_LEARN_BY_SELF;
//				else
//					m_flagRQState = SRQ_FLAG_CAN_LEARN_BY_SKILLMASTER;
//
//				return TRUE;
//			}
//			else
//				return FALSE;
//		}
//	}
//
//	m_flagRQState |= SRQ_FLAG_NOT_LEARN_SKILL;
//	return FALSE;
//}
//
//RwBool CSkillWindowItemDlg::CanLearnHTBSkill( RwUInt8 ucAvatarLevel )
//{
//	sHTB_SET_TBLDAT* pData = reinterpret_cast<sHTB_SET_TBLDAT*>( m_pItem->GetData() );
//	
//	if( ucAvatarLevel < pData->byRequire_Train_Level )	
//	{
//		m_flagRQState = SRQ_FLAG_NOT_LEARN_LEVEL;
//		m_flagRQState |= SRQ_FLAG_NOT_LEARN_HTB;
//		return FALSE;
//	}
//
//	m_flagRQState = SRQ_FLAG_NOT_LEARN_HTB;
//	return FALSE;
//}
//
//RwBool CSkillWindowItemDlg::CanRQDisplay(VOID)
//{
//	if( m_flagRQState == SRQ_FLAG_CAN_LEARN_BY_SELF ||
//		m_flagRQState == SRQ_FLAG_CAN_UPGRADE )
//			return FALSE;
//	
//	return TRUE;
//}
//
////RwBool CSkillWindowItemDlg::CanLearnFromThisSkillMaster(VOID)
////{
////	if( m_eRQState & SRQ_CAN_UPGRADE )
////	{
////		// HTB는 업그레이드가 없음.
////		sSKILL_TBLDAT* pSkillData = reinterpret_cast<sSKILL_TBLDAT*>( m_pItem->GetData() );
////		if( FindSkillFromTableIdx( pSkillData->dwNextSkillTblidx ) )
////			return TRUE;
////		else
////			m_eRQState = SRQ_CAN_UPGRADE_BUT_NOT_THIS_MASTER;
////	}
////	else if( m_eRQState & SRQ_CAN_LEARN )
////	{
////		if( FindSkillFromTableIdx( m_pItem->GetData()->tblidx ) )
////			return TRUE;
////		else
////			m_eRQState = SRQ_CAN_LEARN_BUT_NOT_THIS_MASTER;
////	}
////
////	return FALSE;
////}
//
////RwBool CSkillWindowItemDlg::FindSkillFromTableIdx( RwUInt32 tblidx )
////{
////	CNtlSobNpc* pSkillMaster = m_pSkillWnd->GetSkillMaster();
////	CNtlSobNpcAttr* pSkillMasterAttr = reinterpret_cast<CNtlSobNpcAttr*>( pSkillMaster->GetSobAttr() );
////	sNPC_TBLDAT* pSkillMasterData = pSkillMasterAttr->GetNpcTbl();
////	CMerchantTable* pSkillMasterItemTable = API_GetTableContainer()->GetMerchantTable();
////
////	for( RwInt32 i = 0 ; i < NTL_MAX_MERCHANT_TAB_COUNT ; ++i )
////	{
////		if( pSkillMasterData->amerchant_Tblidx[i] == INVALID_TBLIDX )
////			continue;
////
////		sMERCHANT_TBLDAT* pSkillMasterItemData = reinterpret_cast<sMERCHANT_TBLDAT*>( pSkillMasterItemTable->FindData( pSkillMasterData->amerchant_Tblidx[i] ) );
////		NTL_ASSERT( pSkillMasterItemData, "CSkillWindowItemDlg::FindSkillFromTableIdx : Invalid Skill Merchant Idx" );
////
////		for( RwInt32 j = 0 ; j < NTL_MAX_MERCHANT_COUNT ; ++j )
////		{
////			if( tblidx / 10 == pSkillMasterItemData->aitem_Tblidx[j] / 10 )
////			{
////				m_sMerchantInfo.m_byMerchantTab = i;
////				m_sMerchantInfo.m_byPos = j;
////				return TRUE;
////			}
////		}
////	}
////
////	return FALSE;
////}
//
//VOID CSkillWindowItemDlg::ClickEffect( RwBool bPush, RwBool bStillPush /* = TRUE  */)
//{
//	CRectangle rtScreen = m_pThis->GetScreenRect();
//
//	if( bPush )
//	{
//		m_surIcon.SetRect( rtScreen.left + m_rtIcon.left + ICONPUSH_SIZEDIFF, rtScreen.top + m_rtIcon.top + ICONPUSH_SIZEDIFF,
//						   rtScreen.left + m_rtIcon.right - ICONPUSH_SIZEDIFF, rtScreen.top + m_rtIcon.bottom - ICONPUSH_SIZEDIFF );
//	}
//	else 
//	{
//		m_surIcon.SetRect( rtScreen.left + m_rtIcon.left, rtScreen.top + m_rtIcon.top,
//						   rtScreen.left + m_rtIcon.right, rtScreen.top + m_rtIcon.bottom );
//	}	
//
//	m_bPushDownIcon = bStillPush;
//}
//
//VOID CSkillWindowItemDlg::ShowPickedUp( RwBool bShow )
//{
//	if( bShow )
//	{
//		m_bShowPickedUp = TRUE;
//	}
//	else
//	{
//		m_bShowPickedUp = FALSE;
//	}
//}
//
//VOID CSkillWindowItemDlg::OnMouseDown( const CKey& key )
//{
//	RwBool bClickIcon = m_rtIcon.PtInRect( (RwInt32)key.m_fX, (RwInt32)key.m_fY );
//	if( bClickIcon && !GetIconMoveManager()->IsActive() )
//		ClickEffect( TRUE );
//
//	if( key.m_nID == UD_RIGHT_BUTTON )
//	{
//		if( bClickIcon && m_pItem->IsUseable() ) 
//		{
//			if( m_pItem->GetIcon()->IsUsePossible() )
//				m_bRClickIcon = TRUE;							
//		}	
//	}
//	else if( key.m_nID == UD_LEFT_BUTTON )
//	{
//		if( bClickIcon && m_pItem->IsUseable() )
//		{
//			if( m_pItem->GetIcon()->IsUsePossible() )
//				m_bLClickIcon = TRUE;
//		}
//	}
//
//	m_pThis->CaptureMouse();
//
//	if( m_bRClickIcon && m_bLClickIcon )
//	{
//		m_bLClickIcon = FALSE;
//		m_bRClickIcon = FALSE;
//		ClickEffect( FALSE, FALSE );
//		m_pThis->ReleaseMouse();
//	}
//}
//
//VOID CSkillWindowItemDlg::OnMouseUp( const CKey& key )
//{
//	RwBool bClickIcon = m_rtIcon.PtInRect( (RwInt32)key.m_fX, (RwInt32)key.m_fY );
//	ClickEffect( FALSE, FALSE );
//	m_pThis->ReleaseMouse();
//
//	if( key.m_nID == UD_LEFT_BUTTON )
//	{
//		if( bClickIcon )
//		{
//			if( m_bLClickIcon )
//			{
//				if( GetIconMoveManager()->IsActive() )
//				{
//					if( GetIconMoveManager()->GetSrcSerial() == m_pItem->GetIcon()->GetSobObj()->GetSerialID() )
//						GetIconMoveManager()->IconMoveEnd();
//				}
//				else
//				{
//					if( GetIconMoveManager()->IconMovePickUp( m_pItem->GetIcon()->GetSobObj()->GetSerialID(), PLACE_SKILL, 0, 0, m_pItem->GetIcon()->GetImage() ) )
//						ShowPickedUp( TRUE );
//				}
//			}
//		}
//	
//		m_bLClickIcon = FALSE;
//	}
//	else if( key.m_nID == UD_RIGHT_BUTTON )
//	{
//		if( m_bRClickIcon )
//		{
//			if( !GetDialogManager()->IsOpenDialog( DIALOG_SKILL_RPBONUS ) )
//				Logic_UseProc( m_pItem->GetIcon()->GetSobObj()->GetSerialID() );					
//		}
//				
//		m_bRClickIcon = FALSE;		
//	}
//}
//
//VOID CSkillWindowItemDlg::OnMouseMove( RwInt32 nKey, RwInt32 nXPos, RwInt32 nYPos )
//{
//	if( m_rtIcon.PtInRect( nXPos, nYPos ) )
//	{
//		CRectangle rtScreen = m_pThis->GetScreenRect();
//
//		if( m_eMouseOn != MOUSE_ONICON )
//		{
//			CInfoWndManager::eINFOWNDSTATE eInfoState = CInfoWndManager::INFOWND_NOTSHOW;
//			
//			if( m_pItem->IsUseable() )
//			{
//				switch( m_pItem->GetType() )
//				{
//				case CSkillGuiItem::SKILL:  eInfoState = CInfoWndManager::INFOWND_SKILL; break;
//				case CSkillGuiItem::HTB:	eInfoState = CInfoWndManager::INFOWND_HTBSKILL; break;
//				case CSkillGuiItem::ACTION: eInfoState = CInfoWndManager::INFOWND_ACTIONSKILL; break;
//				}
//
//				GetInfoWndManager()->ShowInfoWindow( TRUE, eInfoState, m_rtIcon.left + rtScreen.left, m_rtIcon.top + rtScreen.top, m_pItem->GetIcon()->GetSobObj(), DIALOG_SKILL );
//			}
//			else
//			{
//				switch( m_pItem->GetType() )
//				{
//				case CSkillGuiItem::SKILL: eInfoState = CInfoWndManager::INFOWND_NOTLEARN_SKILL; break;
//				case CSkillGuiItem::HTB:   eInfoState = CInfoWndManager::INFOWND_NOTLEARN_HTBSKILL; break;
//				}
//
//				GetInfoWndManager()->ShowInfoWindow( TRUE, eInfoState, m_rtIcon.left + rtScreen.left, m_rtIcon.top + rtScreen.top, m_pItem->GetData(), DIALOG_SKILL );
//			}
//
//			if( m_eMouseOn == MOUSE_ONRQ )
//				SetRQSurface( FALSE );
//
//			m_eMouseOn = MOUSE_ONICON;
//
//			if( m_bPushDownIcon )
//				ClickEffect( TRUE );
//		}
//		return;
//	}
//	else if( m_rtRQ.PtInRect( nXPos, nYPos ) )
//	{
//		if( !m_bShowRQ )
//			return;
//
//		CRectangle rtScreen = m_pThis->GetScreenRect();
//
//		if( m_eMouseOn != MOUSE_ONRQ )
//		{
//			GetInfoWndManager()->ShowInfoWindow( TRUE, CInfoWndManager::INFOWND_SKILLRQ, m_rtRQ.left + rtScreen.left, m_rtRQ.top + rtScreen.top, (VOID*)m_flagRQState, DIALOG_SKILL );
//			m_eMouseOn = MOUSE_ONRQ;
//
//			SetRQSurface( TRUE );
//
//			if( m_bPushDownIcon )
//				ClickEffect( FALSE, m_bPushDownIcon );
//		}
//		return;
//	}
//
//	if( m_eMouseOn != MOUSE_NONE )
//	{
//		GetInfoWndManager()->ShowInfoWindow( FALSE );
//		
//		if( m_bPushDownIcon )
//			ClickEffect( FALSE, m_bPushDownIcon );
//
//		if( m_eMouseOn == MOUSE_ONRQ )
//			SetRQSurface( FALSE );
//
//		m_eMouseOn = MOUSE_NONE;
//	}
//}
//
//VOID CSkillWindowItemDlg::OnMouseLeave( gui::CComponent* pComponent )
//{
//	if( m_eMouseOn != MOUSE_NONE )
//	{
//		GetInfoWndManager()->ShowInfoWindow( FALSE );
//		
//		if( m_bPushDownIcon )
//			ClickEffect( FALSE, m_bPushDownIcon );
//
//		if( m_eMouseOn == MOUSE_ONRQ )
//			SetRQSurface( FALSE );
//
//		m_eMouseOn = MOUSE_NONE;
//	}
//}
//
//VOID CSkillWindowItemDlg::OnPaint(VOID)
//{
//	m_surIcon.Render();
//
//	m_surRPType.Render();
//
//	if( m_bShowPickedUp )
//		m_surPickedUp.Render();
//
//	if( m_eMouseOn == MOUSE_ONICON )
//		m_surFocusEffect.Render();
//
//	m_CoolTimeEffect.Render();
//
//	if( m_bShowRQ )
//		m_surRQ.Render();
//}
//
//VOID CSkillWindowItemDlg::OnMove( RwInt32 nOldX, RwInt32 nOldY )
//{
//	CRectangle rtScreen = m_pThis->GetScreenRect();
//	m_surIcon.SetPosition( rtScreen.left + m_rtIcon.left, rtScreen.top + m_rtIcon.top );
//	m_surRPType.SetPositionbyParent( rtScreen.left, rtScreen.top );
//	m_surFocusEffect.SetPosition( rtScreen.left + m_rtIcon.left, rtScreen.top + m_rtIcon.top );
//	m_CoolTimeEffect.SetPosition( rtScreen.left + m_rtIcon.left, rtScreen.top + m_rtIcon.top );
//	m_surRQ.SetPosition( rtScreen.left + m_rtRQ.left, rtScreen.top + m_rtRQ.top );
//	m_surPickedUp.SetPosition( rtScreen.left + m_rtIcon.left, rtScreen.top + m_rtIcon.top );
//}
//
//VOID CSkillWindowItemDlg::OnLearnButtonEnter( gui::CComponent* pComponent )
//{
//	CRectangle rtScreen = m_pBtnLearn->GetScreenRect();
//	WCHAR* pString = const_cast<WCHAR*>( GetDisplayStringManager()->GetString( DST_SKILL_READY_LEARN ) );
//	GetInfoWndManager()->ShowInfoWindow( TRUE, CInfoWndManager::INFOWND_JUST_WTEXT, rtScreen.left, rtScreen.top, (VOID*)pString, DIALOG_SKILL );
//}
//
//VOID CSkillWindowItemDlg::OnLearnButtonLeave( gui::CComponent* pComponent )
//{
//	GetInfoWndManager()->ShowInfoWindow( FALSE );
//}
//
//VOID CSkillWindowItemDlg::OnUpgradeButtonEnter( gui::CComponent* pComponent )
//{
//	CRectangle rtScreen = m_pBtnUpgrade->GetScreenRect();
//	WCHAR* pString = const_cast<WCHAR*>( GetDisplayStringManager()->GetString( DST_SKILL_READY_UPGRADE ) );
//	GetInfoWndManager()->ShowInfoWindow( TRUE, CInfoWndManager::INFOWND_JUST_WTEXT, rtScreen.left, rtScreen.top, (VOID*)pString, DIALOG_SKILL );
//}
//
//VOID CSkillWindowItemDlg::OnUpgradeButtonLeave( gui::CComponent* pComponent )
//{
//	GetInfoWndManager()->ShowInfoWindow( FALSE );
//}
//
//VOID CSkillWindowItemDlg::OnLearnButtonClick( gui::CComponent* pComponent )
//{
//	if( m_pItem->GetType() == CSkillGuiItem::SKILL )
//		GetDboGlobal()->GetGamePacketGenerator()->SendSkillLearnReq( m_pItem->GetData()->tblidx );		
//	else
//		NTL_ASSERTFAIL( "CSkillWindowItemDlg::OnLearnButtonClick : Invalid Skill type" );
//}
//
//VOID CSkillWindowItemDlg::OnUpgradeButtonClick( gui::CComponent* pComponent )
//{
//	CNtlSkillContainer* pSkillContainer = GetNtlSLGlobal()->GetSobAvatar()->GetSkillContainer();
//	RwInt32 nSlotIdx = pSkillContainer->GetSkillSlotIdx( m_pItem->GetIcon()->GetSobObj()->GetSerialID() );
//	NTL_ASSERT( nSlotIdx >= 0, "CSkillWindowItemDlg::OnUpgradeButtonClick : Invalid Skill Object" );
//
//	GetDboGlobal()->GetGamePacketGenerator()->SendSkillUpgradeReq( (RwUInt8)nSlotIdx );	
//}
//
//VOID CSkillWindowItemDlg::OnRPModeClick( gui::CComponent* pComponent )
//{
//	CRectangle rtScreen = m_pbtnRPMode->GetScreenRect();
//	CDboEventGenerator::RpBonusSetup( rtScreen.right, rtScreen.bottom, m_pItem->GetIcon() );
//}
//
//////////////////////////////////////////////////////////////////////////////////
//// CSkillWindowGui
//
//CSkillWindowGui::CSkillWindowGui(VOID)
//{
//	Init();
//}
//
//CSkillWindowGui::CSkillWindowGui( const RwChar *pName )
//: CNtlPLGui( pName )
//{
//	Init();
//}
//
//CSkillWindowGui::~CSkillWindowGui(VOID)
//{
//
//}
//
//RwBool CSkillWindowGui::Create(VOID)
//{
//	NTL_FUNCTION( "CSkillWindowGui::Create" );
//
//	if( !CNtlPLGui::Create( "", "gui\\SkillWnd.srf", "gui\\SkillWnd.frm" ) )
//		NTL_RETURN( FALSE );
//
//	CNtlPLGui::CreateComponents( GetNtlGuiManager()->GetGuiManager() );
//
//	m_pThis = (gui::CDialog*)GetComponent( "dlgMain" );
//
//	m_pTitle = reinterpret_cast<gui::CStaticBox*>( GetComponent( "stbTitle" ) );
//	m_pScrollBar = (gui::CScrollBar*)GetComponent( "scbScroll" );
//	m_pTabButton = (gui::CTabButton*)GetComponent( "tabSkill" );
//	m_pCloseButton = (gui::CButton*)GetComponent( "btnClose" );
//
//	m_pFilter[0] = (gui::CButton*)GetComponent( "btnFilter1" );
//	m_pFilter[1] = (gui::CButton*)GetComponent( "btnFilter2" );
//	m_pFilter[2] = (gui::CButton*)GetComponent( "btnFilter3" );
//	m_pFilter[3] = (gui::CButton*)GetComponent( "btnFilter4" );
//
//	m_pBtnHelp = (gui::CButton*)GetComponent( "btnHelp" );
//	m_slotHelp = m_pBtnHelp->SigClicked().Connect( this, &CSkillWindowGui::OnClickedBtnHelp );
//
//	m_slotTabChanged			= m_pTabButton->SigSelectChanged().Connect( this, &CSkillWindowGui::OnTabChanged );
//	m_slotCloseButtonClicked	= m_pCloseButton->SigClicked().Connect( this, &CSkillWindowGui::OnCloseButtonClicked );
//	m_slotScrollChanged			= m_pScrollBar->SigValueChanged().Connect( this, &CSkillWindowGui::OnScrollChanged );
//	m_slotScrollSliderMoved		= m_pScrollBar->SigSliderMoved().Connect( this, &CSkillWindowGui::OnScrollChanged );
//
//	m_slotFilterBtnClicked[0]	= m_pFilter[0]->SigToggled().Connect( this, &CSkillWindowGui::OnFilterBtn0Clicked );
//	m_slotFilterBtnClicked[1]	= m_pFilter[1]->SigToggled().Connect( this, &CSkillWindowGui::OnFilterBtn1Clicked );
//	m_slotFilterBtnClicked[2]	= m_pFilter[2]->SigToggled().Connect( this, &CSkillWindowGui::OnFilterBtn2Clicked );
//	m_slotFilterBtnClicked[3]	= m_pFilter[3]->SigToggled().Connect( this, &CSkillWindowGui::OnFilterBtn3Clicked );
//
//	m_slotCaptureWheelMove = GetNtlGuiManager()->GetGuiManager()->SigCaptureWheelMove().Connect( this, &CSkillWindowGui::OnCaptureWheelMove );
//	m_slotCaptureMouseDown = GetNtlGuiManager()->GetGuiManager()->SigCaptureMouseDown().Connect( this, &CSkillWindowGui::OnCaptureMouseDown );
//
//	m_pUnknownTexture = Logic_CreateTexture( SKILLWND_UNKNOWN_TEXNAME );
//
//	// Tab Setting
//	m_pTitle->SetText( GetDisplayStringManager()->GetString( DST_SKILL_WINDOW_TITLE ) );
//	m_pTabButton->AddTab( std::wstring( GetDisplayStringManager()->GetString( DST_SKILL_TAB_BATTLE_SKILL ) ) );
//	m_pTabButton->AddTab( std::wstring( GetDisplayStringManager()->GetString( DST_SKILL_TAB_SUB_SKILL ) ) );
//	m_pTabButton->AddTab( std::wstring( GetDisplayStringManager()->GetString( DST_SKILL_TAB_ACTION_SKILL ) ) );
//
//	// Filter Button Setting
//	FilterButtonSetting();
//	m_pFilter[0]->SetText( std::wstring( GetDisplayStringManager()->GetString( DST_SKILL_FILTER_ALL ) ) );		// 변하지 않는 녀석. 변한다면 FilterButtonSetting에 넣자.
//
//	// Initial Setting
//	m_pThis->Show( false );
//
//	// serialize 
//	EnableSerialize(TRUE);
//
//	// Update 연결
//	GetNtlGuiManager()->AddUpdateFunc( this );
//
//	// Event연결
//	LinkMsg( g_EventSobInfoUpdate );
//	LinkMsg( g_EventSobCooling );
//	LinkMsg( g_EventPickedUpHide );
//	LinkMsg( g_EventSobConvertClass );
//	LinkMsg( g_EventTLNotifyLockUnlock );	
//	LinkMsg( g_EventChangeWorldConceptState );
//	LinkMsg( g_EventGameServerChangeOut );
//
//	NTL_RETURN( TRUE );
//}
//
//VOID CSkillWindowGui::Destroy(VOID)
//{
//	ClearItems();
//	ClearItemDialogs();
//
//	Logic_DeleteTexture( m_pUnknownTexture );
//		
//	UnLinkMsg( g_EventSobInfoUpdate );
//	UnLinkMsg( g_EventSobCooling );
//	UnLinkMsg( g_EventPickedUpHide );
//	UnLinkMsg( g_EventSobConvertClass );
//	UnLinkMsg( g_EventTLNotifyLockUnlock );
//	UnLinkMsg( g_EventChangeWorldConceptState );
//	UnLinkMsg( g_EventGameServerChangeOut );
//
//	GetNtlGuiManager()->RemoveUpdateFunc( this );
//
//	CNtlPLGui::DestroyComponents();
//	CNtlPLGui::Destroy();
//}
//
//VOID CSkillWindowGui::Update( RwReal fElapsed )
//{
//	for( RwInt32 i = 0 ; i < MAX_VIEWABLE_ITEM ; ++i )
//	{
//		if( m_arrViewSet[i] && m_arrViewSet[i]->IsShow() )
//		{
//			CSkillGuiItem* pItem = m_arrViewSet[i]->GetSkillItem();
//
//			if( pItem->IsUseable() )
//			{
//				m_arrViewSet[i]->Update( fElapsed );				
//			}
//		}
//	}
//}
//
//VOID CSkillWindowGui::Init(VOID)
//{
//	RwInt32 i;
//
//	for( i = 0 ; i < NUM_TAB ; ++i )
//	{
//		m_nTabScrollOffset[i] = 0;
//	}
//
//	for( i = 0 ; i < NUM_BATTLE_FILTER ; ++i )
//	{
//		m_nBattleFilterScrollOffset[i] = 0;
//	}
//
//	for( i = 0 ; i < NUM_ASSIST_FILTER ; ++i )
//	{
//		m_nAssistFilterScrollOffset[i] = 0;
//	}
//
//	for( i = 0 ; i < NUM_ACTION_FILTER ; ++i )
//	{
//		m_nActionFilterScrollOffset[i] = 0;
//	}
//
//	for( i = 0 ; i < MAX_VIEWABLE_ITEM ; ++i )
//	{
//		m_arrViewSet[i] = NULL;
//	}
//
//	m_eCurrentTab = BATTLE_TAB;
//	m_eCurrentBattleFilter = NO_BATTLE_FILTER;
//	m_eCurrentAssistFilter = NO_ASSIST_FILTER;
//	m_eCurrentActionFilter = NO_ACTION_FILTER;
//		
//	m_pScrollBar = NULL;
//	m_pTabButton = NULL;
//	m_pCloseButton = NULL;
//	m_pTitle = NULL;
//
//	for( i = 0 ; i < 4 ; ++i )
//	{
//		m_pFilter[i] = NULL;
//
//	}
//
//	m_pUnknownTexture = NULL;
//}
//
//VOID CSkillWindowGui::ClearItems(VOID)
//{
//	SKILLMAP_IT it;
//	RwInt32 i;
//
//	for( it = m_mapBattle.begin() ; it != m_mapBattle.end() ; ++it )
//	{
//		if( (*it).second )
//			NTL_DELETE( (*it).second );
//	}
//	for( it = m_mapAssist.begin() ; it != m_mapAssist.end() ; ++it )
//	{
//		if( (*it).second )
//			NTL_DELETE( (*it).second );
//	}
//	for( it = m_mapAction.begin() ; it != m_mapAction.end() ; ++it )
//	{
//		if( (*it).second )
//			NTL_DELETE( (*it).second );
//	}
//	m_mapBattle.clear();
//	m_mapAssist.clear();
//	m_mapAction.clear();	
//
//	for( i = 0 ; i < NUM_TAB ; ++i )
//	{
//		m_nTabScrollOffset[i] = 0;
//	}
//
//	for( i = 0 ; i < NUM_BATTLE_FILTER ; ++i )
//	{
//		m_nBattleFilterScrollOffset[i] = 0;
//		m_vecBattleFilter[i].clear();
//	}
//
//	for( i = 0 ; i < NUM_ASSIST_FILTER ; ++i )
//	{
//		m_nAssistFilterScrollOffset[i] = 0;
//		m_vecAssistFilter[i].clear();
//	}
//
//	for( i = 0 ; i < NUM_ACTION_FILTER ; ++i )
//	{
//		m_nActionFilterScrollOffset[i] = 0;
//		m_vecActionFilter[i].clear();
//	}
//
//	m_eCurrentTab = BATTLE_TAB;
//	m_eCurrentBattleFilter = NO_BATTLE_FILTER;
//	m_eCurrentAssistFilter = NO_ASSIST_FILTER;
//	m_eCurrentActionFilter = NO_ACTION_FILTER;
//}
//
//VOID CSkillWindowGui::ClearItemDialogs(VOID)
//{
//	for( RwInt32 i = 0 ; i < MAX_VIEWABLE_ITEM ; ++i )
//	{
//		if( m_arrViewSet[i] )
//			NTL_DELETE( m_arrViewSet[i] );
//	}
//}
//
//RwBool CSkillWindowGui::CreateItems(VOID)
//{
//	NTL_FUNCTION( "CSkillWindowGui::CreateItems" );
//		
//	CPCTable* pPCTable = API_GetTableContainer()->GetPcTable();
//	CSkillTable* pSkillTable = API_GetTableContainer()->GetSkillTable();
//	CHTBSetTable* pHTBTable = API_GetTableContainer()->GetHTBSetTable();
//	CActionTable* pActionTable = API_GetTableContainer()->GetActionTable();
//	SAvatarInfo* pAvatarInfo = GetNtlSLGlobal()->GetAvatarInfo();
//
//	sPC_TBLDAT* pPCData = reinterpret_cast<sPC_TBLDAT*>( pPCTable->FindData( pAvatarInfo->sCharPf.tblidx ) );
//
//	CTable::TABLEIT	it;
//	SKILLMAP_IT sit;
//	
//	//Skill
//	for( it = pSkillTable->Begin() ; it != pSkillTable->End() ; ++it )
//	{
//		sSKILL_TBLDAT* pSkillData = reinterpret_cast<sSKILL_TBLDAT*>( (*it).second );
//
//		if( pSkillData->bySkill_Class == NTL_SKILL_CLASS_HTB )
//			continue;
//
//		if( pSkillData->dwPC_Class_Bit_Flag & pPCData->dwClass_Bit_Flag )
//		{
//			CSkillGuiItem* pItem = NULL;
//
//			if( pSkillData->bySkill_Grade == 1 &&
//				pSkillData->bDefaultDisplayOff == FALSE )
//			{
//				pItem = NTL_NEW CSkillGuiItem( NULL, pSkillData, CSkillGuiItem::SKILL );
//				NTL_ASSERT( pItem, "Faile To Allocate SkillWindowItem" );
//				
//				if( pSkillData->bySkill_Class == NTL_SKILL_CLASS_ACTIVE )
//				{
//					if( pSkillData->bySkill_Type == NTL_SKILL_TYPE_PHYSICAL ||
//						pSkillData->bySkill_Type == NTL_SKILL_TYPE_ENERGY )
//					{
//						AddSkillMapData( &m_mapBattle, pItem );
//					}
//					else // NTL_SKILL_TYPE_NONE || NTL_SKILL_TYPE_STATE )
//					{
//						AddSkillMapData( &m_mapAssist, pItem );		
//					}					
//				}
//				else // NTL_SKILL_CLASS_PASSIVE
//				{
//					AddSkillMapData( &m_mapAssist, pItem );					
//				}								
//			}
//		}
//	}
//
//	// HTB Skill
//	for( it = pHTBTable->Begin() ; it != pHTBTable->End() ; ++it )
//	{
//		sHTB_SET_TBLDAT* pHTBData = reinterpret_cast<sHTB_SET_TBLDAT*>( (*it).second );
//
//		if( pHTBData->dwPC_Class_Bit_Flag & pPCData->dwClass_Bit_Flag )
//		{
//			CSkillGuiItem* pItem = NULL;
//
//			pItem = NTL_NEW CSkillGuiItem( NULL, pHTBData, CSkillGuiItem::HTB );
//			NTL_ASSERT( pItem, "Faile To Allocate SkillWindowItem" );
//
//			AddSkillMapData( &m_mapBattle, pItem );			
//		}
//	}
//
//	// Social Action
//	for( it = pActionTable->Begin() ; it != pActionTable->End() ; ++it )
//	{
//		sACTION_TBLDAT* pActionData = reinterpret_cast<sACTION_TBLDAT*>( (*it).second );
//
//		CSkillGuiItem* pItem = NULL;
//
//		pItem = NTL_NEW CSkillGuiItem( NULL, pActionData, CSkillGuiItem::ACTION );
//		NTL_ASSERT( pItem, "Failed to allocate skillwindowitem" );
//
//		m_mapAction[pActionData->tblidx] = pItem;
//	}
//
//	// 초기 SkillItemDlg 생성.
//	for( RwInt32 i = 0 ; i < MAX_VIEWABLE_ITEM ; ++i )
//	{
//		m_arrViewSet[i] = NTL_NEW CSkillWindowItemDlg( this, m_pUnknownTexture );
//		NTL_ASSERT( m_arrViewSet[i], "CSkillWindowGui::CreateItems, Failed to Allocate SkillWindowItemDlg" );
//	}
//
//	// Filter 생성
//	SetFilterItems();
//
//	NTL_RETURN( TRUE );
//}
//
//VOID CSkillWindowGui::AddItemsByClass( RwUInt8 byClass )
//{
//	CSkillTable* pSkillTable = API_GetTableContainer()->GetSkillTable();
//	CHTBSetTable* pHTBTable = API_GetTableContainer()->GetHTBSetTable();
//	SAvatarInfo* pAvatarInfo = GetNtlSLGlobal()->GetAvatarInfo();
//	RwInt32 flagClass = Logic_GetExplicitPCClassFlagFromPCClass( byClass );
//
//	CTable::TABLEIT	it;
//
//	// Skill
//	for( it = pSkillTable->Begin() ; it != pSkillTable->End() ; ++it )
//	{
//		sSKILL_TBLDAT* pSkillData = reinterpret_cast<sSKILL_TBLDAT*>( (*it).second );
//
//		if( pSkillData->bySkill_Class == NTL_SKILL_CLASS_HTB )
//			continue;
//
//		if( pSkillData->dwPC_Class_Bit_Flag == flagClass )
//		{
//			CSkillGuiItem* pItem = NULL;
//
//			if( pSkillData->bySkill_Grade == 1 &&
//				pSkillData->bDefaultDisplayOff == FALSE )
//			{
//				pItem = NTL_NEW CSkillGuiItem( NULL, pSkillData, CSkillGuiItem::SKILL );
//				NTL_ASSERT( pItem, "Faile To Allocate SkillWindowItem" );
//
//				if( pSkillData->bySkill_Class == NTL_SKILL_CLASS_ACTIVE )
//				{
//					if( pSkillData->bySkill_Type == NTL_SKILL_TYPE_PHYSICAL ||
//						pSkillData->bySkill_Type == NTL_SKILL_TYPE_ENERGY )
//					{
//						AddSkillMapData( &m_mapBattle, pItem );
//					}
//					else // NTL_SKILL_TYPE_NONE || NTL_SKILL_TYPE_STATE )
//					{
//						AddSkillMapData( &m_mapAssist, pItem );						
//					}					
//				}
//				else // NTL_SKILL_CLASS_PASSIVE
//				{
//					AddSkillMapData( &m_mapAssist, pItem );							
//				}					
//			}
//		}
//	}
//
//	// HTB Skill
//	for( it = pHTBTable->Begin() ; it != pHTBTable->End() ; ++it )
//	{
//		sHTB_SET_TBLDAT* pHTBData = reinterpret_cast<sHTB_SET_TBLDAT*>( (*it).second );
//
//		if( pHTBData->dwPC_Class_Bit_Flag == flagClass )
//		{
//			CSkillGuiItem* pItem = NULL;
//
//			pItem = NTL_NEW CSkillGuiItem( NULL, pHTBData, CSkillGuiItem::HTB );
//			NTL_ASSERT( pItem, "Faile To Allocate SkillWindowItem" );
//
//			AddSkillMapData( &m_mapBattle, pItem );
//		}
//	}
//
//	// Filter Setting
//	SetFilterItems();
//
//	// 추가분 갱신
//	SetScrollValue();
//	ShowItemDetect();
//}
//
//VOID CSkillWindowGui::FindAddedSkill( RwUInt32 uiSlot )
//{
//	RwBool bAdded = FALSE;
//
//	// 바로 추가된 스킬
//	CNtlSkillContainer* pSkillContainer = GetNtlSLGlobal()->GetSobAvatar()->GetSkillContainer();
//	CNtlSobSkill* pSobSkill = pSkillContainer->GetSkill( uiSlot );
//	SKILLMAP_IT it;
//
//	if( pSobSkill )
//	{
//		CNtlSobSkillAttr* pSobSkillAttr = reinterpret_cast<CNtlSobSkillAttr*>( pSobSkill->GetSobAttr() );
//		CNtlSobSkillIcon* pSobSkillIcon = reinterpret_cast<CNtlSobSkillIcon*>( pSobSkill->GetIcon() );
//		sSKILL_TBLDAT* pSkillData = pSobSkillAttr->GetSkillTbl();
//
//		CSkillGuiItem* pItem = NULL;
//
//		// 현재 있는 스킬인지 검색
//		if( pSkillData->bySkill_Class == NTL_SKILL_CLASS_ACTIVE )
//		{
//			if( pSkillData->bySkill_Type == NTL_SKILL_TYPE_PHYSICAL ||
//				pSkillData->bySkill_Type == NTL_SKILL_TYPE_ENERGY )
//			{
//				pItem = GetSkillMapData( &m_mapBattle, pSkillData->bySlot_Index );
//				
//				if( !pItem )
//				{
//					AddSkillMapData( &m_mapBattle, NTL_NEW CSkillGuiItem( pSobSkillIcon, pSkillData, CSkillGuiItem::SKILL ) );					
//					bAdded = TRUE;
//				}
//
//			}
//			else // NTL_SKILL_TYPE_STATE || NTL_SKILL_TYPE_NONE
//			{
//				pItem = GetSkillMapData( &m_mapAssist, pSkillData->bySlot_Index );
//
//				if( !pItem )
//				{
//					AddSkillMapData( &m_mapAssist, NTL_NEW CSkillGuiItem( pSobSkillIcon, pSkillData, CSkillGuiItem::SKILL ) );					
//					bAdded = TRUE;
//				}
//			}
//		}
//		else // NTL_SKILL_CLASS_PASSIVE
//		{	
//			pItem = GetSkillMapData( &m_mapAssist, pSkillData->bySlot_Index );
//
//			if( !pItem )
//			{
//				AddSkillMapData( &m_mapAssist, NTL_NEW CSkillGuiItem( pSobSkillIcon, pSkillData, CSkillGuiItem::SKILL ) );					
//				bAdded = TRUE;
//			}
//		}									
//
//		//// 새롭게 보이는 스킬
//		//if( pSkillData->dwUpperSkillTblidx != INVALID_TBLIDX )
//		//{
//		//	sSKILL_TBLDAT* pUpperSkillData = reinterpret_cast<sSKILL_TBLDAT*>( API_GetTableContainer()->GetSkillTable()->FindData( pSkillData->dwUpperSkillTblidx ) );
//		//	CNtlSobAvatarAttr* pAvatarAttr = reinterpret_cast<CNtlSobAvatarAttr*>( GetNtlSLGlobal()->GetSobAvatar()->GetSobAttr() );
//		//	
//		//	if( pUpperSkillData->dwPC_Class_Bit_Flag & pAvatarAttr->GetPcTbl()->dwClass_Bit_Flag )
//		//	{
//		//		if( pUpperSkillData->bySkill_Class == NTL_SKILL_CLASS_ACTIVE )
//		//		{
//		//			if( pUpperSkillData->bySkill_Type == NTL_SKILL_TYPE_PHYSICAL ||
//		//				pUpperSkillData->bySkill_Type == NTL_SKILL_TYPE_ENERGY )
//		//			{
//		//				pItem = GetSkillMapData( &m_mapBattle, pUpperSkillData->bySlot_Index );
//
//		//				if( !pItem )
//		//				{
//		//					AddSkillMapData( &m_mapBattle, NTL_NEW CSkillGuiItem( NULL, pUpperSkillData, CSkillGuiItem::SKILL ) );					
//		//					bAdded = TRUE;
//		//				}
//		//			}
//		//			else // NTL_SKILL_TYPE_STATE || NTL_SKILL_TYPE_NONE
//		//			{
//		//				pItem = GetSkillMapData( &m_mapAssist, pUpperSkillData->bySlot_Index );
//
//		//				if( !pItem )
//		//				{
//		//					AddSkillMapData( &m_mapAssist, NTL_NEW CSkillGuiItem( NULL, pUpperSkillData, CSkillGuiItem::SKILL ) );					
//		//					bAdded = TRUE;
//		//				}					
//		//			}
//		//		}
//		//		else // NTL_SKILL_CLASS_PASSIVE
//		//		{	
//		//			pItem = GetSkillMapData( &m_mapAssist, pUpperSkillData->bySlot_Index );
//
//		//			if( !pItem )
//		//			{
//		//				AddSkillMapData( &m_mapAssist, NTL_NEW CSkillGuiItem( NULL, pUpperSkillData, CSkillGuiItem::SKILL ) );					
//		//				bAdded = TRUE;
//		//			}
//		//		}				
//		//	}
//		//}
//	}
//
//	if( bAdded )
//	{
//		// Filter Setting
//		SetFilterItems();
//
//		// 추가분 갱신
//		SetScrollValue();
//		ShowItemDetect();
//	}
//}
//
//VOID CSkillWindowGui::AddSkillMapData( SKILLMAP* pMap, CSkillGuiItem* pItem )
//{
//	// 맵에 중복된 데이터는 자동으로 지우고 갱신.
//	SKILLMAP_IT it;
//
//	if( pItem->GetType() == CSkillGuiItem::SKILL )
//	{
//		sSKILL_TBLDAT* pSkillData = reinterpret_cast<sSKILL_TBLDAT*>( pItem->GetData() );
//
//		it = pMap->find( pSkillData->bySlot_Index );
//
//		if( it == pMap->end() )
//		{
//			(*pMap)[pSkillData->bySlot_Index] = pItem;
//		}
//		else
//		{
//			CSkillGuiItem* pEraseItem = (*it).second;
//			NTL_DELETE( pEraseItem );			
//			(*it).second = pItem;			
//		}
//	}
//	else if( pItem->GetType() == CSkillGuiItem::HTB )
//	{
//		sHTB_SET_TBLDAT* pHTBData = reinterpret_cast<sHTB_SET_TBLDAT*>( pItem->GetData() );
//
//		it = pMap->find( pHTBData->bySlot_Index );
//
//		if( it == pMap->end() )
//		{
//			(*pMap)[pHTBData->bySlot_Index] = pItem;
//		}
//		else
//		{
//			CSkillGuiItem* pEraseItem = (*it).second;
//			NTL_DELETE( pEraseItem );
//			(*it).second = pItem;						
//		}
//	}
//	
//	// 액션은 중복될 일이 없으므로 제외.
//}
//
//CSkillGuiItem* CSkillWindowGui::GetSkillMapData( SKILLMAP* pMap, RwUInt32 idx )
//{
//	SKILLMAP_IT it;
//
//	it = pMap->find( idx );
//
//	if( it == pMap->end() )
//		return NULL;
//	else
//		return (*it).second;			
//}
//
//VOID CSkillWindowGui::SetFilterItems(VOID)
//{
//	// Filter 비우기
//	RwInt32 i;
//
//	for( i = 0 ; i < NUM_BATTLE_FILTER ; ++i )
//		m_vecBattleFilter[i].clear();
//
//	for( i = 0 ; i < NUM_ASSIST_FILTER ; ++i )
//		m_vecAssistFilter[i].clear();
//
//	for( i = 0 ; i < NUM_ACTION_FILTER ; ++i )
//		m_vecActionFilter[i].clear();
//
//	// Filter 입력.
//	SKILLMAP_IT sit;
//
//	for( sit = m_mapBattle.begin() ; sit != m_mapBattle.end() ; ++sit )
//	{
//		CSkillGuiItem* pItem = (*sit).second;
//		NTL_ASSERT( pItem, "CSkillWindowGui::CreateItem : Wrong Skill Item Inserted" );
//
//		if( pItem->GetType() == CSkillGuiItem::SKILL )
//		{
//			m_vecBattleFilter[BATTLE_FILTER].push_back( pItem );			
//		}
//		else if( pItem->GetType() == CSkillGuiItem::HTB )
//		{
//			m_vecBattleFilter[HTB_FILTER].push_back( pItem );
//		}
//		else
//		{
//			NTL_ASSERTFAIL( "CSKillWindowGui::CreateItem() : Unavilable Type in Battle Tab" );
//			continue;
//		}
//	}
//
//	for( sit = m_mapAssist.begin() ; sit != m_mapAssist.end() ; ++sit )
//	{
//		CSkillGuiItem* pItem = (*sit).second;
//		NTL_ASSERT( pItem, "CSkillWindowGui::CreateItem : Wrong Skill Item Inserted" );
//		sSKILL_TBLDAT* pData = reinterpret_cast<sSKILL_TBLDAT*>( pItem->GetData() );
//
//		if( pData->bySkill_Class == NTL_SKILL_CLASS_PASSIVE )
//		{
//			m_vecAssistFilter[PASSIVE_FILTER].push_back( pItem );
//		}
//		else // NTL_SKILL_CLASS_ACTIVE and NTL_SKILL_TYPE_STATE
//		{
//			m_vecAssistFilter[ASSIST_FILTER].push_back( pItem );
//		}		
//	}
//
//	for( sit = m_mapAction.begin() ; sit != m_mapAction.end() ; ++sit )
//	{
//		CSkillGuiItem* pItem = (*sit).second;
//		NTL_ASSERT( pItem, "CSkillWindowGui::CreateItem : Wrong Skill Item Inserted" );
//		sACTION_TBLDAT* pData = reinterpret_cast<sACTION_TBLDAT*>( pItem->GetData() );
//
//		if( pData->byAction_Type == ACTION_TYPE_SOCIAL_ACTION )
//		{
//			m_vecActionFilter[SOCIAL_FILTER].push_back( pItem );
//		}
//		else
//		{
//			m_vecActionFilter[ETC_FILTER].push_back( pItem );
//		}
//	}
//}
//
//VOID CSkillWindowGui::ShowItemDetect(VOID)
//{ 
//	RwInt32 i;
//
//	for( i = 0 ; i < MAX_VIEWABLE_ITEM ; ++i )
//	{
//		if( m_arrViewSet[i] )
//			m_arrViewSet[i]->Show( false );
//	}
//
//	if( m_eCurrentTab == BATTLE_TAB )
//	{
//		if( m_eCurrentBattleFilter == NO_BATTLE_FILTER )
//		{
//			SKILLMAP_IT it;
//			for( i = 0, it = m_mapBattle.begin() ; i < m_nTabScrollOffset[m_eCurrentTab] ; ++i )
//			{
//				++it;
//			}
//
//			for( i = 0 ; it != m_mapBattle.end() ; ++it, ++i )
//			{
//				if( i >= MAX_VIEWABLE_ITEM )
//					break;
//
//				m_arrViewSet[i]->UpdateData( (*it).second );
//				m_arrViewSet[i]->Show( true );
//				ItemPositionSet( i, m_arrViewSet[i] );
//			}
//		}
//		else
//		{
//			RwInt32 nMaxItemCount = m_vecBattleFilter[m_eCurrentBattleFilter].size();
//
//			for( i = 0 ; i < MAX_VIEWABLE_ITEM ; ++i )
//			{
//				if( m_nBattleFilterScrollOffset[m_eCurrentBattleFilter] + i >= nMaxItemCount )
//					break;
//
//				m_arrViewSet[i]->UpdateData( m_vecBattleFilter[m_eCurrentBattleFilter][m_nBattleFilterScrollOffset[m_eCurrentBattleFilter] + i] );
//				m_arrViewSet[i]->Show( true );
//				ItemPositionSet( i, m_arrViewSet[i] );				
//			}
//		}
//	}
//	else if( m_eCurrentTab == ASSIST_TAB )
//	{
//		if( m_eCurrentAssistFilter == NO_ASSIST_FILTER )
//		{
//			SKILLMAP_IT it;
//			for( i = 0, it = m_mapAssist.begin() ; i < m_nTabScrollOffset[m_eCurrentTab] ; ++i )
//			{
//				++it;
//			}
//
//			for( i = 0 ; it != m_mapAssist.end() ; ++it, ++i )
//			{
//				if( i >= MAX_VIEWABLE_ITEM )
//					break;
//
//				m_arrViewSet[i]->UpdateData( (*it).second );
//				m_arrViewSet[i]->Show( true );
//				ItemPositionSet( i, m_arrViewSet[i] );
//			}	
//		}
//		else
//		{
//			RwInt32 nMaxItemCount = m_vecAssistFilter[m_eCurrentAssistFilter].size();
//
//			for( i = 0 ; i < MAX_VIEWABLE_ITEM ; ++i )
//			{
//				if( m_nAssistFilterScrollOffset[m_eCurrentAssistFilter] + i >= nMaxItemCount )
//					break;
//
//				m_arrViewSet[i]->UpdateData( m_vecAssistFilter[m_eCurrentAssistFilter][m_nAssistFilterScrollOffset[m_eCurrentAssistFilter] + i] );
//				m_arrViewSet[i]->Show( true );
//				ItemPositionSet( i, m_arrViewSet[i] );				
//			}
//		}
//	}
//	else if( m_eCurrentTab == ACTION_TAB )
//	{
//		if( m_eCurrentActionFilter == NO_ASSIST_FILTER )
//		{
//			SKILLMAP_IT it;
//			for( i = 0, it = m_mapAction.begin() ; i < m_nTabScrollOffset[m_eCurrentTab] ; ++i )
//			{
//				++it;
//			}
//
//			for( i = 0 ; it != m_mapAction.end() ; ++it, ++i )
//			{
//				if( i >= MAX_VIEWABLE_ITEM )
//					break;
//
//				m_arrViewSet[i]->UpdateData( (*it).second );
//				m_arrViewSet[i]->Show( true );
//				ItemPositionSet( i, m_arrViewSet[i] );
//			}
//		}
//		else
//		{
//			RwInt32 nMaxItemCount = m_vecActionFilter[m_eCurrentActionFilter].size();
//
//			for( i = 0 ; i < MAX_VIEWABLE_ITEM ; ++i )
//			{
//				if( m_nActionFilterScrollOffset[m_eCurrentActionFilter] + i >= nMaxItemCount )
//					break;
//
//				m_arrViewSet[i]->UpdateData( m_vecActionFilter[m_eCurrentActionFilter][m_nActionFilterScrollOffset[m_eCurrentActionFilter] + i] );
//				m_arrViewSet[i]->Show( true );
//				ItemPositionSet( i, m_arrViewSet[i] );
//			}
//		}	
//	}
//}
//
//VOID CSkillWindowGui::FilterButtonSetting(VOID)
//{
//	switch( m_eCurrentTab )
//	{
//	case BATTLE_TAB: 
//		m_pFilter[0]->Show( true );
//		m_pFilter[1]->Show( true );
//		m_pFilter[2]->Show( true );
//		m_pFilter[3]->Show( false );
//
//		m_pFilter[1]->SetText( std::wstring( GetDisplayStringManager()->GetString( DST_SKILL_FILTER_ACTION ) ) );
//		m_pFilter[2]->SetText( std::wstring( GetDisplayStringManager()->GetString( DST_SKILL_FILTER_HTB ) ) );		
//		break;
//	case ASSIST_TAB:
//		m_pFilter[0]->Show( true );
//		m_pFilter[1]->Show( true );
//		m_pFilter[2]->Show( true );
//		m_pFilter[3]->Show( false );
//
//		m_pFilter[1]->SetText( std::wstring( GetDisplayStringManager()->GetString( DST_SKILL_FILTER_SUB ) ) );
//		m_pFilter[2]->SetText( std::wstring( GetDisplayStringManager()->GetString( DST_SKILL_FILTER_PASSIVE ) ) );		
//		break;
//	case ACTION_TAB:
//		m_pFilter[0]->Show( true );
//		m_pFilter[1]->Show( true );
//		m_pFilter[2]->Show( true );
//		m_pFilter[3]->Show( false );
//
//		m_pFilter[1]->SetText( std::wstring( GetDisplayStringManager()->GetString( DST_SKILL_FILTER_SOCIAL ) ) );
//		m_pFilter[2]->SetText( std::wstring( GetDisplayStringManager()->GetString( DST_SKILL_FILTER_ETC ) ) );		
//		break;
//	}
//}
//
//VOID CSkillWindowGui::SetFilterMode( RwInt32 nIndex )
//{
//	for( RwInt32 i = 0 ; i < 4 ; ++i )
//	{
//		if( nIndex == i )
//		{
//			m_pFilter[i]->Enable( false );
//		}
//		else
//		{
//			m_pFilter[i]->Enable( true );
//			m_pFilter[i]->SetDown( false );		
//		}
//	}
//
//	switch( m_eCurrentTab )
//	{
//	case BATTLE_TAB:
//		m_eCurrentBattleFilter = (eBATTLESKILLFILTER)( nIndex - 1 );		// NO_BATTLE_TAB = -1.
//		break;
//	case ASSIST_TAB:
//		m_eCurrentAssistFilter = (eASSISTSKILLFILTER)( nIndex - 1 );
//		break;
//	case ACTION_TAB:
//		m_eCurrentActionFilter = (eACTIONSKILLFILTER)( nIndex - 1 );
//		break;
//	}
//
//	SetScrollValue();
//	ShowItemDetect();
//}
//
//VOID CSkillWindowGui::ItemPositionSet( RwInt32 nIndex, CSkillWindowItemDlg* pItemDlg )
//{
//	pItemDlg->SetPosition( 16, ( nIndex * 75 ) + 91 );
//}
//
//VOID CSkillWindowGui::SetScrollValue(VOID)
//{
//	if( m_eCurrentTab == BATTLE_TAB )
//	{
//		if( m_eCurrentBattleFilter == NO_BATTLE_FILTER )
//		{
//			m_pScrollBar->SetRange( 0, m_mapBattle.size() - MAX_VIEWABLE_ITEM );
//			m_pScrollBar->SetValue(	m_nTabScrollOffset[m_eCurrentTab] );
//		}
//		else
//		{
//			m_pScrollBar->SetRange( 0, m_vecBattleFilter[m_eCurrentBattleFilter].size() - MAX_VIEWABLE_ITEM );
//			m_pScrollBar->SetValue(	m_nBattleFilterScrollOffset[m_eCurrentBattleFilter] );
//		}
//	}
//	else if( m_eCurrentTab == ASSIST_TAB )
//	{
//		if( m_eCurrentAssistFilter == NO_BATTLE_FILTER )
//		{
//			m_pScrollBar->SetRange( 0, m_mapAssist.size() - MAX_VIEWABLE_ITEM );
//			m_pScrollBar->SetValue( m_nTabScrollOffset[m_eCurrentTab] );
//		}
//		else
//		{
//			m_pScrollBar->SetRange( 0, m_vecAssistFilter[m_eCurrentAssistFilter].size() - MAX_VIEWABLE_ITEM );
//			m_pScrollBar->SetValue(	m_nAssistFilterScrollOffset[m_eCurrentAssistFilter] );
//		}		
//	}	
//	else if( m_eCurrentTab == ACTION_TAB )
//	{
//		if( m_eCurrentActionFilter == NO_ACTION_FILTER )
//		{
//			m_pScrollBar->SetRange( 0, m_mapAction.size() - MAX_VIEWABLE_ITEM );
//			m_pScrollBar->SetValue( m_nTabScrollOffset[m_eCurrentTab] );
//		}
//		else
//		{
//			m_pScrollBar->SetRange( 0, m_vecActionFilter[m_eCurrentActionFilter].size() - MAX_VIEWABLE_ITEM );
//			m_pScrollBar->SetValue( m_nActionFilterScrollOffset[m_eCurrentActionFilter] );
//		}
//	}
//}
//
//VOID CSkillWindowGui::UpdateAllViewItems(VOID)
//{
//	for( RwInt32 i = 0 ; i < MAX_VIEWABLE_ITEM ; ++i )
//	{
//		if( m_arrViewSet[i]->IsShow() )
//			m_arrViewSet[i]->UpdateData( m_arrViewSet[i]->GetSkillItem() );
//	}
//}
//
//VOID CSkillWindowGui::UpdateActiveSkillItem( RwInt32 nSlot )
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
//
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
//		NTL_ASSERT( pItem, "CSkillWindowGui::UpdateActiveSkillItem : No SkillItem Data. ERROR" );
//		pItem->SetIcon( pSobSkillIcon );
//		pItem->SetData( pSkillTable );
//		
//		for( RwInt32 i = 0 ; i < MAX_VIEWABLE_ITEM ; ++i )
//		{
//			if( m_arrViewSet[i]->GetSkillItem() == pItem )
//			{
//				m_arrViewSet[i]->UpdateData( pItem );
//				break;
//			}
//		}
//	}
//}
//
///**
//* \brief 스킬 경험치가 증가되었을 경우 업그레이드 가능한지 판단하고 업그레이드가 가능하면 SideIcon으로 Notify를 보낸다.
//*/
//VOID CSkillWindowGui::UpdateSkillCanUpgrade( RwInt32 nSlot )
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
//		RwReal	fValue = (RwReal)pSobSkillAttr->m_uiExp / pSobSkillAttr->m_uiMaxExp * 100;
//
//		if( fValue >= 100.0f )
//		{
//			if( pSkillTable->dwNextSkillTblidx != INVALID_DWORD )
//			{
//				WCHAR awcBuffer[256];
//				swprintf_s( awcBuffer, 256, GetDisplayStringManager()->GetString( DST_SIDEICON_HINT_SKILL_UPGRADE_TEXT ), pSkillTable->wszNameText );
//
//				CNtlSLEventGenerator::PopoNotify( GetDisplayStringManager()->GetString( DST_SIDEICON_HINT_SKILL_UPGRADE_TITLE ), awcBuffer, TRUE, 5.0f );
//			}
//		}
//	}
//}
//
//VOID CSkillWindowGui::UpdateActiveHTBItem( RwInt32 nSlot )
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
//		NTL_ASSERT( pItem, "CSkillWindowGui::UpdateActiveHTBItem : No SkillItem Data. ERROR" );
//
//		pItem->SetIcon( pSobHTBSkillIcon );
//		pItem->SetData( pHTBTable );
//
//		for( RwInt32 i = 0 ; i < MAX_VIEWABLE_ITEM ; ++i )
//		{
//			if( m_arrViewSet[i]->GetSkillItem() == pItem )
//			{
//				m_arrViewSet[i]->UpdateData( pItem );
//				break;
//			}
//		}
//	}
//}
//
//VOID CSkillWindowGui::UpdateActiveActionSkillItem( RwInt32 nSlot )
//{
//	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
//	CNtlSkillContainer* pSkillContainer = pAvatar->GetSkillContainer();
//
//	CNtlSobActionSkill* pSobActionSkill = pSkillContainer->GetActionSkill( nSlot );
//
//	if( pSobActionSkill )
//	{
//		CNtlSobActionSkillAttr* pSobActionSkillAttr = reinterpret_cast<CNtlSobActionSkillAttr*>( pSobActionSkill->GetSobAttr() );
//		CNtlSobActionSkillIcon* pSobActionSkillIcon = reinterpret_cast<CNtlSobActionSkillIcon*>( pSobActionSkill->GetIcon() );
//		sACTION_TBLDAT* pActionTable = pSobActionSkillAttr->GetActionSkillTbl();
//
//		CSkillGuiItem* pItem = NULL;
//
//		pItem = GetSkillMapData( &m_mapAction, pActionTable->tblidx );
//		NTL_ASSERT( pItem, "CSkillWindowGui::UpdateActiveActionSkillItem : No SkillItem Data. ERROR" );
//
//		pItem->SetIcon( pSobActionSkillIcon );
//		pItem->SetData( pActionTable );
//
//		for( RwInt32 i = 0 ; i < MAX_VIEWABLE_ITEM ; ++i )
//		{
//			if( m_arrViewSet[i]->GetSkillItem() == pItem )
//			{
//				m_arrViewSet[i]->UpdateData( pItem );
//				break;
//			}
//		}
//	}
//}
//
//VOID CSkillWindowGui::HandleEvents( RWS::CMsg& msg )
//{
//	if( msg.Id == g_EventSobInfoUpdate )
//	{
//		SNtlEventSobInfoUpdate* pUpdate = reinterpret_cast<SNtlEventSobInfoUpdate*>( msg.pData );
//		RwUInt32 nSlot = pUpdate->uiParam1;
//
//		if( pUpdate->hSerialId != GetNtlSLGlobal()->GetSobAvatar()->GetSerialID() )
//			return;
//		
//		if( pUpdate->uiUpdateType & ( EVENT_AIUT_SKILL_LEVELUP | EVENT_AIUT_SKILL_LEARN ) )
//		{
//			FindAddedSkill( nSlot );
//			UpdateActiveSkillItem( nSlot );
//			UpdateAllViewItems();
//		}
//		else if( pUpdate->uiUpdateType & EVENT_AIUT_ATTR_LEVELUP )
//		{
//			UpdateAllViewItems();
//		}
//		else if( pUpdate->uiUpdateType & EVENT_AIUT_SKILL )
//		{
//			for( RwInt32 i = 0 ; i < NTL_MAX_PC_HAVE_SKILL ; ++i )
//			{
//				FindAddedSkill( i );
//				UpdateActiveSkillItem( i );
//			}
//			for( RwInt32 i = 0 ; i < NTL_HTB_MAX_PC_HAVE_HTB_SKILL ; ++i )
//			{
//				UpdateActiveHTBItem( i );
//			}
//			for( RwInt32 i = 0 ; i < NTL_MAX_PC_HAVE_ACTION_SKILL ; ++i )
//			{
//				UpdateActiveActionSkillItem( i );
//			}
//		}
//		else if( pUpdate->uiUpdateType & EVENT_AIUT_HTBSKILL_LEARN )
//		{
//			UpdateActiveHTBItem( nSlot );
//		}
//		else if( pUpdate->uiUpdateType & EVENT_AIUT_SKILL_INIT )
//		{
//			CreateItems();
//			SetFilterMode( 0 );
//		}
//	}
//	else if( msg.Id == g_EventIconPopupResult )
//	{
//		//SDboEventIconPopupResult* pData = reinterpret_cast<SDboEventIconPopupResult*>( msg.pData );
//		//
//		//if( pData->nSrcPlace != PLACE_SKILL )
//		//	return;
//
//		//if( pData->nWorkId == PMW_USE )
//		//{
//		//	//스킬 사용.
//		//	Logic_UseProc( pData->uiSerial );
//		//}
//		//else if( pData->nWorkId == PMW_PICKUP )
//		//{
//		//	CNtlSob* pSkill = GetNtlSobManager()->GetSobObject( pData->uiSerial );
//		//	GetIconMoveManager()->IconMovePickUp( pData->uiSerial, PLACE_SKILL, 0, 0, pSkill->GetIcon()->GetImage() );
//		//}		
//	}
//	else if( msg.Id == g_EventSobCooling )
//	{
//		SNtlEventSobCooling* pData = reinterpret_cast<SNtlEventSobCooling*>( msg.pData );
//
//		CoolingEffectProc( pData->hSerialId, pData->bStartOrEnd );
//	}
//	else if( msg.Id == g_EventPickedUpHide )
//	{
//		RwInt32 nSlotIdx = (RwInt32)msg.pData;
//		if( nSlotIdx != PLACE_SKILL )
//			return;
//
//		for( RwInt32 i = 0 ; i < MAX_VIEWABLE_ITEM ; ++i )
//		{
//			m_arrViewSet[i]->ShowPickedUp( FALSE );
//		}
//	}
//	else if( msg.Id == g_EventSobConvertClass )
//	{
//		SNtlEventSobConvertClass* pData = reinterpret_cast<SNtlEventSobConvertClass*>( msg.pData );
//
//		if( pData->hSerialId == GetNtlSLGlobal()->GetSobAvatar()->GetSerialID() )
//		{
//			AddItemsByClass( pData->byClass );
//		}
//	}
//	else if( msg.Id == g_EventTLNotifyLockUnlock )
//	{
//		SNtlEventTLLockUnlcok* pEvent = reinterpret_cast<SNtlEventTLLockUnlcok*>( msg.pData );
//
//		if( pEvent->byLockType != TLLOCK_MOUSE )
//			return;
//
//		if( pEvent->byIndex != ETL_MOUSE_INPUT_TYPE_SKILL_WND_TAB && 
//			pEvent->byIndex != ETL_MOUSE_INPUT_TYPE_ALL )
//			return;
//
//		for( RwUInt8 i = 0 ; i < m_pTabButton->GetButtonCount() ; ++i )
//			m_pTabButton->EnableTab(i, !pEvent->bLock);
//
//		for( RwUInt8 i = 0 ; i < 4 ; ++i )
//			m_pFilter[i]->ClickEnable(!pEvent->bLock);
//	}
//	else if( msg.Id == g_EventChangeWorldConceptState )
//	{
//		SNtlEventWorldConceptState* pEvent = reinterpret_cast<SNtlEventWorldConceptState*>( msg.pData );
//
//		if( pEvent->eWorldConcept == WORLD_PLAY_TUTORIAL )
//		{
//			if( pEvent->uiState == WORLD_STATE_EXIT )
//			{
//				for( RwUInt8 i = 0 ; i < m_pTabButton->GetButtonCount() ; ++i )
//					m_pTabButton->EnableTab(i, true);
//
//				for( RwUInt8 i = 0 ; i < 4 ; ++i )
//					m_pFilter[i]->ClickEnable(true);
//			}
//		}
//	}
//	else if( msg.Id == g_EventGameServerChangeOut )
//	{
//		ClearItems();
//		ClearItemDialogs();
//	}
//}
//
//VOID CSkillWindowGui::CoolingEffectProc( SERIAL_HANDLE hSerial, BOOL bStart )
//{
//	for( RwInt32 i = 0 ; i < MAX_VIEWABLE_ITEM ; ++i )
//	{
//		if( m_arrViewSet[i]->IsShow() )
//		{
//			CSkillGuiItem* pItem = m_arrViewSet[i]->GetSkillItem();
//
//			if( pItem->IsUseable() )
//			{
//				if( pItem->GetIcon()->GetSobObj()->GetSerialID() == hSerial )
//				{
//					if( bStart )
//						m_arrViewSet[i]->StartCoolTime();
//					else
//						m_arrViewSet[i]->EndCoolTime();
//
//					break;
//				}	
//			}			
//		}
//	}
//}
//
//VOID CSkillWindowGui::ShowSkillWindow(VOID)
//{
//	RaiseTop();
//	Show( true );
//	m_pTabButton->SelectTab( BATTLE_TAB );
//	m_eCurrentTab = BATTLE_TAB;
//	SetFilterMode( 0 );
//	FilterButtonSetting();
//}
//
//VOID CSkillWindowGui::HideSkillWindow(VOID)
//{
//	if( GetInfoWndManager()->GetRequestGui() == DIALOG_SKILL )
//	{
//		GetInfoWndManager()->ShowInfoWindow( FALSE );
//	}
//
//	Show( false );	
//}
//
//RwInt32 CSkillWindowGui::SwitchDialog( bool bOpen )
//{
//	if( bOpen )
//	{
//		ShowSkillWindow();
//		Logic_SendTutorialCondition(ETL_CONDITION_TYPE_SKILL_OPEN);
//	}
//	else
//	{
//		HideSkillWindow();
//		Logic_SendTutorialCondition(ETL_CONDITION_TYPE_SKILL_CLOSE);
//	}
//	
//	return 1;
//}
//
//VOID CSkillWindowGui::OnTabChanged( RwInt32 nIndex, RwInt32 nOldIndex )
//{
//	m_eCurrentTab = (eTAB)nIndex;
//	FilterButtonSetting();
//
//	if( m_eCurrentTab == BATTLE_TAB )
//	{
//		SetFilterMode( m_eCurrentBattleFilter + 1 );	// -1부터 시작.
//	}
//	else if( m_eCurrentTab == ASSIST_TAB )
//	{
//		SetFilterMode( m_eCurrentAssistFilter + 1 );
//	}
//	else // ACTION_TAB
//	{
//		SetFilterMode( m_eCurrentActionFilter + 1 );
//	}	
//}
//
//VOID CSkillWindowGui::OnCloseButtonClicked( gui::CComponent* pComponent )
//{
//	if( !Logic_CanMouseInput_in_Tutorial( ETL_MOUSE_INPUT_TYPE_SKILL_WND_CLOSE_BTN ) )
//		return;
//
//	GetDialogManager()->CloseDialog( DIALOG_SKILL );			
//}
//
//VOID CSkillWindowGui::OnScrollChanged( RwInt32 nNewOffset )
//{
//	if( m_eCurrentTab == BATTLE_TAB )
//	{
//		if( m_eCurrentBattleFilter == NO_BATTLE_FILTER )
//		{
//			m_nTabScrollOffset[m_eCurrentTab] = nNewOffset;
//		}
//		else
//		{
//			m_nBattleFilterScrollOffset[m_eCurrentBattleFilter] = nNewOffset;
//		}
//	}
//	else if( m_eCurrentTab == ASSIST_TAB )
//	{
//		if( m_eCurrentAssistFilter == NO_ASSIST_FILTER )
//		{
//			m_nTabScrollOffset[m_eCurrentTab] = nNewOffset;
//		}
//		else
//		{
//			m_nAssistFilterScrollOffset[m_eCurrentAssistFilter] = nNewOffset;
//		}
//	}
//	else // Action Tab
//	{
//		if( m_eCurrentActionFilter == NO_ASSIST_FILTER )
//		{
//			m_nTabScrollOffset[m_eCurrentTab] = nNewOffset;	
//		}
//		else
//		{
//			m_nActionFilterScrollOffset[m_eCurrentActionFilter] = nNewOffset;
//		}
//	}
//
//	ShowItemDetect();
//}
//
//VOID CSkillWindowGui::OnFilterBtn0Clicked( gui::CComponent* pComponent, bool bDown )
//{
//	SetFilterMode( 0 );
//}
//
//VOID CSkillWindowGui::OnFilterBtn1Clicked( gui::CComponent* pComponent, bool bDown )
//{
//	SetFilterMode( 1 );
//}
//
//VOID CSkillWindowGui::OnFilterBtn2Clicked( gui::CComponent* pComponent, bool bDown )
//{
//	SetFilterMode( 2 );
//}
//
//VOID CSkillWindowGui::OnFilterBtn3Clicked( gui::CComponent* pComponent, bool bDown )
//{
//	SetFilterMode( 3 );
//}
//
//VOID CSkillWindowGui::OnCaptureWheelMove( RwInt32 nFlag, RwInt16 sDelta, CPos& pos )
//{
//	CAPTURE_MOUSEWHEELMOVE_SCROLLBAR( IsShow(), m_pThis, m_pScrollBar, sDelta, pos );
//}
//
//VOID CSkillWindowGui::OnCaptureMouseDown( const CKey& key )
//{
//	CAPTURE_MOUSEDOWN_RAISE( DIALOG_SKILL, key.m_fX, key.m_fY );
//}
//
//VOID CSkillWindowGui::OnClickedBtnHelp( gui::CComponent* pComponent )
//{
//	CDboEventGenerator::OpenHelpContent( DIALOG_SKILL );
//}
////RwBool CSkillWindowGui::IsEnableShowSkillMasterWindow( CNtlSobNpc* pNPC )
////{
////	CNtlSobNpcAttr* pSkillMasterAttr = reinterpret_cast<CNtlSobNpcAttr*>( pNPC->GetSobAttr() );
////	sNPC_TBLDAT* pSkillMasterData = pSkillMasterAttr->GetNpcTbl();
////	CMerchantTable* pSkillMasterItemTable = API_GetTableContainer()->GetMerchantTable();
////
////	for( RwInt32 i = 0 ; i < NTL_MAX_MERCHANT_TAB_COUNT ; ++i )
////	{
////		if( pSkillMasterData->amerchant_Tblidx[i] == INVALID_TBLIDX )
////			continue;
////
////		sMERCHANT_TBLDAT* pSkillMasterItemData = reinterpret_cast<sMERCHANT_TBLDAT*>( pSkillMasterItemTable->FindData( pSkillMasterData->amerchant_Tblidx[i] ) );
////		NTL_ASSERT( pSkillMasterItemData, "CSkillWindowItemDlg::FindSkillFromTableIdx : Invalid Skill Merchant Idx:" << pSkillMasterData->amerchant_Tblidx[i]);
////
////		SKILLMAP_IT it;
////
////		for( it = m_mapBattle.begin() ; it != m_mapBattle.end() ; ++it )
////		{
////			CSkillWindowItem* pItem = (*it).second;
////
////			for( RwInt32 i = 0 ; i < NTL_MAX_MERCHANT_COUNT ; ++i )
////			{
////				if( pSkillMasterItemData->aitem_Tblidx[i] == INVALID_TBLIDX )
////					break;
////
////				if( pItem->GetData()->tblidx == pSkillMasterItemData->aitem_Tblidx[i] )
////					return TRUE;
////			}
////		}
////
////		for( it = m_mapAssist.begin() ; it != m_mapAssist.end() ; ++it )
////		{
////			CSkillWindowItem* pItem = (*it).second;
////
////			for( RwInt32 i = 0 ; i < NTL_MAX_MERCHANT_COUNT ; ++i )
////			{
////				if( pSkillMasterItemData->aitem_Tblidx[i] == INVALID_TBLIDX )
////					break;
////
////				if( pItem->GetData()->tblidx == pSkillMasterItemData->aitem_Tblidx[i] )
////					return TRUE;
////			}
////		}
////	}
////
////	return FALSE;
////}
//
