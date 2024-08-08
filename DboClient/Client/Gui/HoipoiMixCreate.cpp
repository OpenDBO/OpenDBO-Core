#include "precomp_dboclient.h"
#include "HoipoiMixCreate.h"

// Debug
#include "NtlDebug.h"

// Gui
#include "GuiUtil.h"

// Shared
#include "ItemRecipeTable.h"
#include "TableContainer.h"
#include "TextAllTable.h"

// Presentation
#include "NtlPLGuiManager.h"

// Simulation
#include "NtlSLApi.h"
#include "NtlSobItem.h"
#include "NtlSobItemAttr.h"

// Client
#include "HoipoiMixGui.h"
#include "DboEvent.h"
#include "DboLogic.h"
#include "DboEventGenerator.h"
#include "AlarmManager.h"
#include "InfoWndManager.h"
#include "DboGlobal.h"

#define dHOIPOIMIX_CREATE_TIME 2.0f
#define dHOIPOIMIX_HIT_RATE		0.02f
#define dHOIPOIMIX_NOTIFY_TIME	0.5f		///< COOL 같은 텍스트가 떠 있는 타임

CHoipoiMixCreate::CHoipoiMixCreate()
{
	m_pThis = NULL;
	m_pStbTitle = NULL;
	m_pStbSettingCategory = NULL;
	m_pStbZenny = NULL;
	m_pBtnMax = NULL;
	m_pBtnCalc = NULL;
	m_pStbPreViewCategory = NULL;
	m_pBtnCreate = NULL;
	m_pStbControlCategory = NULL;
	m_pPgbConcent = NULL;
	m_pStbConcentText = NULL;
	m_pOpbLog = NULL;
	m_idxEnableRecipe = INVALID_TBLIDX;
	m_bCreating = FALSE;
	m_nLeftCreate = 0;
	m_bStep = FALSE;
	m_fElapsedTime = 0.0f;
	m_nArrowCount = 0;
	m_nHitCount = 0;
	m_uiOpbLogCount = 0;
	m_pPnlNotify = NULL;
	m_pPbBox = NULL;

	::ZeroMemory( m_anArrowPosX, sizeof(RwInt32) * dHOIPOIMIX_ARROW_MAX_NUM );
	for( RwInt32 i=0; i < dHOIPOIMIX_ARROW_MAX_NUM; ++i )
	{
		m_abArrowHit[i] = FALSE;
	}
}

CHoipoiMixCreate::~CHoipoiMixCreate()
{

}

RwBool CHoipoiMixCreate::Create( CHoipoiMixCapsuleGui* pParentGui )
{
	m_pHoipoiMix = pParentGui;

	m_pThis = (gui::CDialog*)pParentGui->GetComponent( "dlgCreate" );
	m_slotMove = m_pThis->SigMove().Connect( this, &CHoipoiMixCreate::OnMove );
	m_slotMouseMove = m_pThis->SigMouseMove().Connect( this, &CHoipoiMixCreate::OnMouseMove );
	
	m_pStbTitle = (gui::CStaticBox*)pParentGui->GetComponent( "stbTitleCreate" );
	m_pStbSettingCategory = (gui::CStaticBox*)pParentGui->GetComponent( "stbSettingCategory" );
	m_pStbZenny = (gui::CStaticBox*)pParentGui->GetComponent( "stbSetZenny" );
	m_pBtnMax = (gui::CButton*)pParentGui->GetComponent( "btnMax" );
	m_pBtnCalc = (gui::CButton*)pParentGui->GetComponent( "btnCalc" );
	m_pStbPreViewCategory = (gui::CStaticBox*)pParentGui->GetComponent( "stbPreviewCategory" );
	m_pBtnCreate = (gui::CButton*)pParentGui->GetComponent( "btnCreate" );
	m_pBtnStop = (gui::CButton*)pParentGui->GetComponent( "btnStop" );
	m_pStbControlCategory = (gui::CStaticBox*)pParentGui->GetComponent( "stbControlCategory" );
	m_pPgbConcent = (gui::CProgressBar*)pParentGui->GetComponent( "pgbConcent" );
	m_pStbConcentText = (gui::CStaticBox*)pParentGui->GetComponent( "stbConcentText" );
	m_pOpbLog = (gui::COutputBox*)pParentGui->GetComponent( "opbLog" );
	m_pPnlNotify = (gui::CPanel*)pParentGui->GetComponent( "pnlNotify" );
	m_pFlaResult[ITEM_MIX_RESULT_TYPE_FAIL] = (gui::CFlash*)pParentGui->GetComponent( "flaFail" );
	m_pFlaResult[ITEM_MIX_RESULT_TYPE_SUCCESS] = (gui::CFlash*)pParentGui->GetComponent( "flaSuccess" );
	m_pFlaResult[ITEM_MIX_RESULT_TYPE_GREAT_SUCCESS] = (gui::CFlash*)pParentGui->GetComponent( "flaSuccess2" );
	
	for( int i=0; i < ITEM_MIX_RESULT_TYPE_NUMS; ++i )
	{
		m_slotMovieEnd[i] = m_pFlaResult[i]->SigMovieEnd().Connect( this, &CHoipoiMixCreate::OnMovieEnd );
	}
	

	m_pStbSettingCategory->SetText( GetDisplayStringManager()->GetString( DST_HOIPOIMIX_MATERIAL_CATEGORY ) );
	m_pBtnMax->SetToolTip( GetDisplayStringManager()->GetString( DST_HOIPOIMIX_TOOLTIP_MAX ) );
	m_pBtnCalc->SetToolTip( GetDisplayStringManager()->GetString( DST_HOIPOIMIX_TOOLTIP_CALC ) );
	m_pBtnCreate->SetToolTip( GetDisplayStringManager()->GetString( DST_HOIPOIMIX_TOOLTIP_CREATE ) );
	m_pStbPreViewCategory->SetText( GetDisplayStringManager()->GetString( DST_HOIPOIMIX_PREVIEW_CATEGORY ) );
	m_pStbControlCategory->SetText( GetDisplayStringManager()->GetString( DST_HOIPOIMIX_CONCENT_CATEGORY ) );
	m_pBtnCreate->SetText( GetDisplayStringManager()->GetString( DST_HOIPOIMIX_BUTTON_CREATE ) );
	m_pBtnStop->SetText( GetDisplayStringManager()->GetString( DST_HOIPOIMIX_BUTTON_STOP ) );
	m_pBtnStop->SetToolTip( GetDisplayStringManager()->GetString( DST_HOIPOIMIX_TOOLTIP_STOP ) );
	
	m_slotClickedBtnCalc = m_pBtnCalc->SigClicked().Connect( this, &CHoipoiMixCreate::OnClickedBtnCalc );
	m_slotClickedBtnMax = m_pBtnMax->SigClicked().Connect( this, &CHoipoiMixCreate::OnClickedBtnMax );
	m_slotClickedBtnCreate = m_pBtnCreate->SigClicked().Connect( this, &CHoipoiMixCreate::OnClickedBtnCreate );
	m_slotClickedBtnStop = m_pBtnStop->SigClicked().Connect( this, &CHoipoiMixCreate::OnClickedBtnStop );
	m_slotCaptureKeyDown = m_pHoipoiMix->GetDialog()->GetGuiManager()->SigCaptureKeyDown().Connect( this, &CHoipoiMixCreate::OnCaptureKeyDown );

	// Slot이 가장 나중에 그려지게 하기 위하여 아웃풋 박스의 Signal에 OnPaint를 연결한다.
	m_slotPaint = m_pOpbLog->SigPaint().Connect( this, &CHoipoiMixCreate::OnPaint );

	m_SlotRecipe.Create( m_pThis, DIALOG_HOIPOIMIX_CAPSULE, REGULAR_SLOT_ITEM_TABLE, SDS_NONE );
	m_SlotRecipe.SetPosition_fromParent( 21, 12 );

	CHAR acBuffer[32];
	CRectangle rtParent, rtChild;
	RwInt32 nPosX, nPosY;
	rtParent = m_pThis->GetScreenRect();
	CRectangle rect;
	for( RwInt32 i=0; i < dHOIPOIMIX_CREATE_MATERIAL_MAX_NUM; ++i )
	{
		sprintf_s( acBuffer, 32, "pnlStuffSlot%d", i );
		m_pPnlMaterialSlot[i] = (gui::CPanel*)pParentGui->GetComponent( acBuffer );
		m_SlotMaterial[i].Create( m_pThis, DIALOG_HOIPOIMIX_CAPSULE, REGULAR_SLOT_ITEM_TABLE, SDS_DISABLE);
		rtChild = m_pPnlMaterialSlot[i]->GetScreenRect();
		nPosX = rtChild.left - rtParent.left + 2;
		nPosY = rtChild.top - rtParent.top + 2;
		m_SlotMaterial[i].SetPosition_fromParent( nPosX, nPosY );

		rect.SetRectWH( nPosX, nPosY, DBOGUI_ICON_SIZE, DBOGUI_ICON_SIZE );
		m_apStbStackMaterial[i] = NTL_NEW gui::CStaticBox( &rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), DBOGUI_STACKNUM_ALIGN );
		m_apStbStackMaterial[i]->CreateFontStd( DBOGUI_STACKNUM_FONT, DBOGUI_STACKNUM_FONTHEIGHT, DBOGUI_STACKNUM_FONTATTR );
		m_apStbStackMaterial[i]->SetEffectMode( TE_OUTLINE );
		m_apStbStackMaterial[i]->Enable( false );
	}

	for( RwInt32 i=0; i < dHOIPOIMIX_CREATE_ITEM_MAX_NUM; ++i )
	{
		sprintf_s( acBuffer, 32, "pnlPreviewSlot%d", i );
		m_pPnlPreViewSlot[i] = (gui::CPanel*)pParentGui->GetComponent( acBuffer );
		m_SlotPreview[i].Create( m_pThis, DIALOG_HOIPOIMIX_CAPSULE, REGULAR_SLOT_ITEM_TABLE, SDS_NONE );
		rtChild = m_pPnlPreViewSlot[i]->GetScreenRect();
		nPosX = rtChild.left - rtParent.left + 2;
		nPosY = rtChild.top - rtParent.top + 2;
		m_SlotPreview[i].SetPosition_fromParent( nPosX, nPosY );
		rect.SetRectWH( nPosX, nPosY, DBOGUI_ICON_SIZE, DBOGUI_ICON_SIZE );
		
		m_apStbStackPreview[i] = NTL_NEW gui::CStaticBox( &rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), DBOGUI_STACKNUM_ALIGN );
		m_apStbStackPreview[i]->CreateFontStd( DBOGUI_STACKNUM_FONT, DBOGUI_STACKNUM_FONTHEIGHT, DBOGUI_STACKNUM_FONTATTR );
		m_apStbStackPreview[i]->SetEffectMode( TE_OUTLINE );
		m_apStbStackPreview[i]->Enable( false );
	}

	for( RwInt32 i=0; i < dHOIPOIMIX_ARROW_MAX_NUM; ++i )
	{
		sprintf_s( acBuffer, 32, "pnlArrow%d", i );
		m_pPnlEpArrow[i] = (gui::CPanel*)pParentGui->GetComponent( acBuffer );
	}

	for( RwInt32 i=0; i < dHOIPOIMIX_ARROW_MAX_NUM; ++i )
	{
		sprintf_s( acBuffer, 32, "pnlHitLine%d", i );
		m_pPnlHitLine[i] = (gui::CPanel*)pParentGui->GetComponent( acBuffer );
	}

	GetNtlGuiManager()->AddUpdateFunc( m_pHoipoiMix );

	m_pPbBox = NTL_NEW gui::CParticleBox( m_pThis, GetNtlGuiManager()->GetSurfaceManager() );
	m_pPbBox->Load( "CreateItemEffect" );

	OnMove(0, 0);

	return TRUE;
}

VOID CHoipoiMixCreate::Destroy()
{
	GetNtlGuiManager()->RemoveUpdateFunc( m_pHoipoiMix );

	m_SlotRecipe.Destroy();

	NTL_DELETE( m_pPbBox );

	for( RwInt32 i=0; i < dHOIPOIMIX_CREATE_MATERIAL_MAX_NUM; ++i )
	{
		m_SlotMaterial[i].Destroy();
		NTL_DELETE( m_apStbStackMaterial[i] );
	}

	for( RwInt32 i=0; i < dHOIPOIMIX_CREATE_ITEM_MAX_NUM; ++i )
	{
		m_SlotPreview[i].Destroy();
		NTL_DELETE( m_apStbStackPreview[i] );
	}
}

VOID CHoipoiMixCreate::Show( bool bShow )
{
	if( !bShow )
	{
		if( m_bCreating )
		{
			StopCreate();
		}
	}

	m_pThis->Show( bShow );

}

RwBool CHoipoiMixCreate::IsShow()
{
	return m_pThis->IsVisible();
}

VOID CHoipoiMixCreate::InitFlash()
{
	for( int i=0; i < ITEM_MIX_RESULT_TYPE_NUMS; ++i )
	{
		m_pFlaResult[i]->Show( FALSE );
	}
}

VOID CHoipoiMixCreate::InitGui()
{
	// 프로그레스바
	m_pPgbConcent->SetPos(0);
	m_uiOpbLogCount = 0;
	m_pOpbLog->Clear();
	SetOutputLog( GetDisplayStringManager()->GetString( DST_HOIPOIMIX_CREATE_FIRST_GUIDE ), INFOCOLOR_0 );

	// 화살표는 꺼준다. ( 히트라인도 꺼준다. )
	for( RwInt32 i =0 ; i < dHOIPOIMIX_ARROW_MAX_NUM; ++i )
	{
		m_pPnlEpArrow[i]->Show( false );
		m_pPnlHitLine[i]->Show( false );
	}

	m_pBtnCreate->Show( !B2b(m_bCreating) );
	m_pBtnStop->Show( B2b(m_bCreating) );
}

VOID CHoipoiMixCreate::SetRecipe( TBLIDX idxRecipeTbl )
{
	// 레시피 테이블 데이터를 읽어온다.
	sITEM_RECIPE_TBLDAT* pRecipeTblDat = (sITEM_RECIPE_TBLDAT*)API_GetTableContainer()->GetItemRecipeTable()->FindData( idxRecipeTbl );
	if( pRecipeTblDat == NULL )
	{
		DBO_FAIL( "Item recipe table is null." );
		return;
	}

	m_idxEnableRecipe = idxRecipeTbl;

	// 텍스트 테이블 데이타를 읽어온다.
	sTEXT_TBLDAT* pTextTblDat = (sTEXT_TBLDAT*)API_GetTableContainer()->GetTextAllTable()->GetItemTbl()->FindData( pRecipeTblDat->dwName );
	if( NULL == pTextTblDat )
	{
		WCHAR awcErrBuffer[32];
		swprintf_s( awcErrBuffer, 32, L"(ITEM_T_TBL)%d", pRecipeTblDat->dwName );
		m_pStbTitle->SetText( awcErrBuffer );
	}
	else
	{
		// 레시피 이름
		m_pStbTitle->SetText( pTextTblDat->wstrText.c_str() );
	}
	
	m_SlotRecipe.SetIcon( pRecipeTblDat->asCreateItemTblidx[0].itemTblidx );

	

	// 재료 셋팅
	for( RwInt32 i=0; i < dHOIPOIMIX_CREATE_MATERIAL_MAX_NUM; ++i )
	{
		if( pRecipeTblDat->asMaterial[i].materialTblidx == INVALID_TBLIDX )
		{
			m_SlotMaterial[i].Clear();
			continue;
		}
		
		m_SlotMaterial[i].SetIcon( pRecipeTblDat->asMaterial[i].materialTblidx );
	}
	
	// 미리보기 세팅
	for( RwInt32 i=0; i < dHOIPOIMIX_CREATE_ITEM_MAX_NUM; ++i )
	{
		if( pRecipeTblDat->asCreateItemTblidx[i].itemTblidx == INVALID_TBLIDX )
		{
			m_SlotPreview[i].Clear();
			continue;
		}

		m_SlotPreview[i].SetIcon( pRecipeTblDat->asCreateItemTblidx[i].itemTblidx );
	}

	// Default 갯수: 1개 이상 만들 수 있어야지 1로 세팅
	RwInt32 nMaxAmount =  Logic_AmountEnableCreateRecipe( m_idxEnableRecipe );
	if( nMaxAmount >= 1 )
		m_pStbZenny->SetText( 1 );
	else
		m_pStbZenny->SetText( 0 );

	// 재료 수량 체크
	UpdateMaterialCheck();
}

VOID CHoipoiMixCreate::HandleEvents( RWS::CMsg& msg )
{
	if( msg.Id == g_EventCalcPopupResult )
	{
		SDboEventCalcPopupResult* pData = reinterpret_cast<SDboEventCalcPopupResult*>( msg.pData );

		if( pData->nSrcPlace == PLACE_SUB_HOIPOIMIXCREATE )
		{
			if( pData->uiValue == 0 )
				return;

			m_pStbZenny->SetText( pData->uiValue );
		}		
	}
	else if( msg.Id == g_EventHoipoiMixItemMakeRes )
	{
		SDboEventHoipoiMixItemMakeRes* pResult = reinterpret_cast<SDboEventHoipoiMixItemMakeRes*>(msg.pData);

		if( pResult->wResultCode == GAME_SUCCESS )
		{
			CNtlSobItem* pItem = (CNtlSobItem*)GetNtlSobManager()->GetSobObject( pResult->hItem );
			if( pItem == NULL )
			{
				DBO_FAIL( "CHoipoiMixCreate" << pItem );
				return;
			}

			CNtlSobItemAttr* pItemAttr = (CNtlSobItemAttr*)pItem->GetSobAttr();
			if( pItemAttr == NULL )
			{
				DBO_FAIL( "CHoipoiMixCreate" << pItem );
				return;
			}
			
			COLORREF color = RGB(255,255,255);

			WCHAR awcBuffer[256];
			switch( pResult->bySuccessResult )
			{
			case ITEM_MIX_RESULT_TYPE_FAIL:
				{
					// 제조창용
					swprintf_s( awcBuffer, 256, GetDisplayStringManager()->GetString( DST_HOIPOIMIX_CREATE_FAILED ), Logic_GetItemName( pItemAttr->GetTblIdx() ) ); 
					color = INFOCOLOR_0;

					// Notify
					GetAlarmManager()->FormattedAlarmMessage( DST_HOIPOIMIX_CREATE_FAILED_NOTIFY, FALSE, NULL, Logic_GetItemName( pItemAttr->GetTblIdx() ) );
				}
				break;
			case ITEM_MIX_RESULT_TYPE_SUCCESS:
				{
					swprintf_s( awcBuffer, 256, GetDisplayStringManager()->GetString( DST_HOIPOIMIX_CREATE_SUCCESS_NORMAL ),Logic_GetItemName( pItemAttr->GetTblIdx() ) );
					color = INFOCOLOR_7;

					GetAlarmManager()->FormattedAlarmMessage( DST_HOIPOIMIX_CREATE_SUCCESS_NORMAL_NOTIFY, FALSE, NULL, Logic_GetItemName( pItemAttr->GetTblIdx() ) );
				}

				break;
			case ITEM_MIX_RESULT_TYPE_GREAT_SUCCESS:
				{
					swprintf_s( awcBuffer, 256, GetDisplayStringManager()->GetString( DST_HOIPOIMIX_CREATE_SUCCESS_GREAT ),Logic_GetItemName( pItemAttr->GetTblIdx() ) );
					color = INFOCOLOR_7;

										GetAlarmManager()->FormattedAlarmMessage( DST_HOIPOIMIX_CREATE_SUCCESS_GREAT_NOTIFY , FALSE, NULL, Logic_GetItemName( pItemAttr->GetTblIdx() ) );
				}
				break;
			default:
				DBO_FAIL( "CHoipoiMixCreate::HandleEvents - exits of success result" << pResult->bySuccessResult );
				break;
			}			
			SetOutputLog( awcBuffer, color );
			PlayResultMovie( pResult->bySuccessResult );
		}
		else
		{
			GetAlarmManager()->AlarmMessage( pResult->wResultCode, TRUE );
			StopCreate();
		}

		// 만약 계속해서 만들 것이 남아 있다면 만들어준다.
		if( m_bCreating )
			Step( TRUE );
	}
	else if( msg.Id == g_EventSobInfoUpdate )
	{
		SNtlEventSobInfoUpdate* pUpdate = reinterpret_cast<SNtlEventSobInfoUpdate*>( msg.pData );

		if( pUpdate->hSerialId != GetNtlSLGlobal()->GetSobAvatar()->GetSerialID() )
			return;

		if( pUpdate->uiUpdateType & EVENT_AIUT_ITEM )
		{
			if( IsShow() )
				UpdateMaterialCheck();
		}
	}
}

VOID CHoipoiMixCreate::OnPaint()
{
	m_SlotRecipe.Paint();

	for( RwInt32 i=0; i < dHOIPOIMIX_CREATE_MATERIAL_MAX_NUM; ++i )
	{
		m_SlotMaterial[i].Paint();
	}

	for( RwInt32 i=0; i < dHOIPOIMIX_CREATE_ITEM_MAX_NUM; ++i )
	{
		m_SlotPreview[i].Paint();
	}
}

VOID CHoipoiMixCreate::OnMove( int nOldX, int nOldY )
{
	CRectangle rect = m_pThis->GetScreenRect();

	m_SlotRecipe.SetParentPosition( rect.left, rect.top );

	for( RwInt32 i=0; i < dHOIPOIMIX_CREATE_MATERIAL_MAX_NUM; ++i )
	{
		m_SlotMaterial[i].SetParentPosition(rect.left, rect.top);
	}

	for( RwInt32 i=0; i < dHOIPOIMIX_CREATE_ITEM_MAX_NUM; ++i )
	{
		m_SlotPreview[i].SetParentPosition(rect.left, rect.top);
	}
}

/**
* \brief 만들 수 있는 최대의 갯수를 계산한다.
*/
VOID CHoipoiMixCreate::OnClickedBtnMax( gui::CComponent* pComponent )
{
	RwInt32 nAmount = Logic_AmountEnableCreateRecipe( m_idxEnableRecipe );
	if( nAmount == 0 )
	{
		GetAlarmManager()->AlarmMessage( DST_HOIPOIMIX_CREATE_NEED_MATERIAL );
		return;
	}

	m_pStbZenny->SetText( nAmount );
}

VOID CHoipoiMixCreate::OnClickedBtnCalc( gui::CComponent* pComponent )
{
	RwInt32 nMaxAmount = Logic_AmountEnableCreateRecipe( m_idxEnableRecipe );
	if( nMaxAmount == 0 )
	{
		GetAlarmManager()->AlarmMessage( DST_HOIPOIMIX_CREATE_NEED_MATERIAL );
		return;
	}
	CRectangle rect = pComponent->GetScreenRect();
	CDboEventGenerator::CalcPopupShow( TRUE, INVALID_SERIAL_ID, PLACE_SUB_HOIPOIMIXCREATE, rect.left, rect.top, nMaxAmount );
}

VOID CHoipoiMixCreate::OnClickedBtnCreate( gui::CComponent* pComponent )
{
	if( m_bCreating )
		StopCreate();

	RwInt32 nAmount = _wtoi( m_pStbZenny->GetText().c_str() );
	if( nAmount == 0 )
	{
		GetAlarmManager()->AlarmMessage( DST_HOIPOIMIX_CREATE_NEED_SET_NUMBER );
		return;
	}

	StartCreate( nAmount );
}

VOID CHoipoiMixCreate::StartCreate( RwInt32 nAmount )
{
	if( nAmount > 0 )
	{
		// 만드는 횟수 설정
		m_nLeftCreate = nAmount;
		SetCreating( TRUE );
	
		// UpdateFunc 등록
		
		Step( TRUE );
	}
}

VOID CHoipoiMixCreate::Step( RwBool bStep )
{
	if( bStep )
	{
		// 레시피 테이블 데이터를 읽어온다.
		sITEM_RECIPE_TBLDAT* pRecipeTblDat = (sITEM_RECIPE_TBLDAT*)API_GetTableContainer()->GetItemRecipeTable()->FindData( m_idxEnableRecipe );
		if( pRecipeTblDat == NULL )
		{
			DBO_FAIL( "CHoipoiMixCreate::SetRecipe - Out of item recipe table index" );
			return;
		}

		// 화살표 생성
		CreateArrow();
		m_pPnlNotify->ClearSurface();

		m_fElapsedTime = 0.0f;
		m_pPgbConcent->SetPos( 0 );
		m_nHitCount = 0;

		for( RwInt32 i=0; i < dHOIPOIMIX_ARROW_MAX_NUM; ++i )
		{
			m_abArrowHit[i] = FALSE;
			m_pPnlHitLine[i]->Show( false );
		}
	}
	else
	{
		m_nLeftCreate--;
		m_pStbZenny->SetText( m_nLeftCreate );
		m_fElapsedTime = 0.0f;

		if( m_nLeftCreate <= 0 )
			StopCreate();
	}

	m_bStep = bStep;
}

VOID CHoipoiMixCreate::Update( RwReal fElapsed )
{
	if( m_pPbBox )
		m_pPbBox->Update( fElapsed );

	if( !m_pThis->IsVisible() )
		return;

	for(int i=0; i < ITEM_MIX_RESULT_TYPE_NUMS; ++i )
	{
		if( m_pFlaResult[i]->IsVisible() )
			m_pFlaResult[i]->Update( fElapsed );
	}

	if( m_nLeftCreate > 0 )
	{
		if( m_bStep )
		{
			m_fElapsedTime += fElapsed;

			if( m_fNotifyTime >= 0.0f )
			{
				m_fNotifyTime -= fElapsed;
				if( m_fNotifyTime <= 0.0f )
				{
					//m_pStbNotify->Clear();
					m_pPnlNotify->ClearSurface();
				}
			}

			// Total Progress ( sec 2 )

			RwReal fPos = m_fElapsedTime / dHOIPOIMIX_CREATE_TIME ;

			if( fPos >= 1.0f )
			{
				fPos = 1.0f;
			
				RwBool bUseEp = TRUE;
				for( RwInt32 i=0; i < m_nArrowCount; ++i )
				{
					if( m_abArrowHit[i] == FALSE )
						bUseEp = FALSE;
				}

				if( m_bCreating )
				{
					GetDboGlobal()->GetGamePacketGenerator()->SendHoipoiMixItemMakeReq(
						m_pHoipoiMix->GetObject(),
						m_idxEnableRecipe,
						bUseEp );
				}

				Step( FALSE );
			}

			RwInt32 nPos = (RwInt32) ( fPos * dHOIPOIMIX_PROGRESS_BAR_WIDTH_F );
			m_pPgbConcent->SetPos( nPos );
		}
	}
}

VOID CHoipoiMixCreate::StopCreate()
{
	m_nLeftCreate = 0;
	m_pPgbConcent->SetPos(0);
	SetCreating( FALSE );

	// 화살표는 꺼준다. ( 히트라인도 꺼준다. )
	for( RwInt32 i =0 ; i < dHOIPOIMIX_ARROW_MAX_NUM; ++i )
	{
		m_pPnlEpArrow[i]->Show( false );
		m_pPnlHitLine[i]->Show( false );
	}

	m_pBtnCreate->Show( !B2b(m_bCreating) );
	m_pBtnStop->Show( B2b(m_bCreating) );
}

VOID CHoipoiMixCreate::PlayResultMovie( RwUInt8 byResultType )
{
	m_pFlaResult[byResultType]->Show( TRUE );
	m_pFlaResult[byResultType]->RestartMovie();
}

VOID CHoipoiMixCreate::SetCreating( RwBool bCreate )
{
	m_bCreating = bCreate;

	if( IsShow() )
	{
		m_pBtnCreate->Show( !B2b(m_bCreating) );
		m_pBtnStop->Show( B2b(m_bCreating) );
	}
}

VOID CHoipoiMixCreate::SetOutputLog( const WCHAR* pLog, COLORREF color )
{
	m_uiOpbLogCount++;
	
	WCHAR awcBuffer[128];
	swprintf_s( awcBuffer, 128, L"%u: %s", m_uiOpbLogCount, pLog );
	m_pOpbLog->AddText( awcBuffer, color );
}

VOID CHoipoiMixCreate::OnKeyDownFromConcent( const CKey& key )
{
	// 만드는 중이라면 EP를 일단 검사한다.
	if( m_bCreating && m_bStep )
	{
		// 사용 할 수 있는 EP 수치
		// 레시피 테이블 데이터를 읽어온다.
		sITEM_RECIPE_TBLDAT* pRecipeTblDat = (sITEM_RECIPE_TBLDAT*)API_GetTableContainer()->GetItemRecipeTable()->FindData( m_idxEnableRecipe );
		if( pRecipeTblDat == NULL )
		{
			DBO_FAIL( "CHoipoiMixCreate::SetRecipe - Out of item recipe table index" );
			return;
		}

		RwUInt32 uiEp = Logic_GetEp(GetNtlSLGlobal()->GetSobAvatar());
		RwUInt32 uiRequireEp = pRecipeTblDat->byNeedMixLevel * DBO_HOIPOIMIX_CHANGE_EP_RATE;

		// 일단 필요한 EP보다 아바타가 EP를 더 많이 가지고 있다면 사용 한다.
		if( uiEp >= uiRequireEp )
		{
			if( m_nHitCount >= m_nArrowCount )
				return;

			// 사용 보내주기
			GetDboGlobal()->GetGamePacketGenerator()->SendHoipoiMixItemMakeEpReq( m_idxEnableRecipe );

			// 정확도 체크
			for( RwInt32 i=0; i < m_nArrowCount; ++i )
			{
				// 프로그레스바의 RATE
				RwReal fPgbRate = (RwReal)m_pPgbConcent->GetPos() / dHOIPOIMIX_PROGRESS_BAR_WIDTH_F;
				RwReal fArrowRate = (RwReal)m_anArrowPosX[i] / (RwReal)m_pPgbConcent->GetWidth();

				// Panel 위치 옮기고 보여주기
				RwInt32 nX = (m_pPgbConcent->GetScreenRect().left - m_pThis->GetScreenRect().left ) + (RwInt32)(fPgbRate*(RwReal)m_pPgbConcent->GetWidth());
				RwInt32 nY = m_pPgbConcent->GetScreenRect().top - m_pThis->GetScreenRect().top;
				m_pPnlHitLine[m_nHitCount]->SetPosition( nX, nY );
				m_pPnlHitLine[m_nHitCount]->Show( true );

				//WCHAR awcBuffer[1024];
				//swprintf_s( awcBuffer, 1024, L"PGB = %f ARROW = %f, ARROW_PX = %d, EPSILON = %f", fPgbRate, fArrowRate, m_anArrowPosX[i], fPgbRate-fArrowRate );
				//CNtlSLEventGenerator::SysMsg( awcBuffer, SNtlEventSysStringMsg::ACTION_NOTIFY | SNtlEventSysStringMsg::TYPE_CHAT_NOTICE );

				//	히트 판정
				if( fabs( fPgbRate - fArrowRate ) <= dHOIPOIMIX_HIT_RATE )
				{
					m_pPnlNotify->ClearSurface();
					m_pPnlNotify->AddSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "HoipoiMix.srf", "srfSuccess" ) );
					m_fNotifyTime = dHOIPOIMIX_NOTIFY_TIME;
					m_abArrowHit[i] = TRUE;

					m_pPbBox->SetPosition( nX, nY+ ( m_pPgbConcent->GetScreenRect().GetHeight()/2 ) );
					m_pPbBox->Play( FALSE );

					break;
				}
				else
				{
					m_pPnlNotify->ClearSurface();
					m_pPnlNotify->AddSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "HoipoiMix.srf", "srfFailed" ) );
					m_fNotifyTime = dHOIPOIMIX_NOTIFY_TIME;
				}
			}

			m_nHitCount++;
		}
	}
}

VOID CHoipoiMixCreate::CreateArrow()
{
	// 영역 계산
	CRectangle rtParent = m_pThis->GetScreenRect();
	CRectangle rtPgb = m_pPgbConcent->GetScreenRect();
	RwInt32 nOffsetX = rtPgb.left - rtParent.left;
	RwInt32 nOffsetY = rtPgb.top - rtParent.top;

	// 3개로 끊어서 끊는 지점 2개
	RwInt32 nWidth = rtPgb.GetWidth()/3;

	m_pPnlEpArrow[0]->SetPosition( nOffsetX + nWidth, nOffsetY );
	m_anArrowPosX[0] = nWidth;
	m_pPnlEpArrow[1]->SetPosition( nOffsetX + nWidth + nWidth, nOffsetY );
	m_anArrowPosX[1] = nWidth+nWidth;

	m_nArrowCount = 2;
	for( RwInt32 i=0; i< m_nArrowCount; ++i )
	{
		m_pPnlEpArrow[i]->Show( true );
	}
}

VOID CHoipoiMixCreate::ClearArrow()
{
	m_nArrowCount = 0;
	for( RwInt32 i=0; i< dHOIPOIMIX_ARROW_MAX_NUM; ++i )
	{
		m_pPnlEpArrow[i]->Show( false );
	}
}

VOID CHoipoiMixCreate::OnMouseMove( RwInt32 nKey, RwInt32 nXPos, RwInt32 nYPos )
{
	CRectangle rect = m_pThis->GetScreenRect();

	RwBool bIn = FALSE;

	if( m_SlotRecipe.PtInRect( nXPos, nYPos ) )
	{
		sITEM_RECIPE_TBLDAT* pTblDat = (sITEM_RECIPE_TBLDAT*)API_GetTableContainer()->GetItemRecipeTable()->FindData( m_idxEnableRecipe );
		if( pTblDat )
		{
			GetInfoWndManager()->ShowInfoWindow(TRUE, CInfoWndManager::INFOWND_RECIPE, 
				rect.left + m_SlotRecipe.GetX_fromParent(), rect.top + m_SlotRecipe.GetY_fromParent(), 
				(VOID*)pTblDat, 
				DIALOG_HOIPOIMIX_CAPSULE);
		}
		else
		{
			WCHAR awcBuffer[128];
			swprintf_s( awcBuffer, 128, L"Invalid recipe table idx %d", m_idxEnableRecipe );
			GetInfoWndManager()->ShowInfoWindow(TRUE, CInfoWndManager::INFOWND_JUST_WTEXT, 
				rect.left + m_SlotRecipe.GetX_fromParent(), rect.top + m_SlotRecipe.GetY_fromParent(), 
				(VOID*)awcBuffer,
				DIALOG_HOIPOIMIX_CAPSULE);
		}

		bIn = TRUE;
	}

	for( RwInt32 i=0; i < dHOIPOIMIX_CREATE_MATERIAL_MAX_NUM; ++i )
	{
		if( m_SlotMaterial[i].PtInRect( nXPos, nYPos ) )
		{
			if( m_SlotMaterial[i].GetSerial() != INVALID_SERIAL_ID )
			{

				// Info Window
				GetInfoWndManager()->ShowInfoWindow( TRUE, 
					CInfoWndManager::INFOWND_TABLE_ITEM,
					m_SlotMaterial[i].GetX_fromParent() + rect.left,
					m_SlotMaterial[i].GetY_fromParent() + rect.top,
					m_SlotMaterial[i].GetItemTable(),
					DIALOG_HOIPOIMIX_CAPSULE );

				bIn = TRUE;

				break;
			}
		}
	}

	for( RwInt32 i=0; i < dHOIPOIMIX_CREATE_ITEM_MAX_NUM; ++i )
	{
		if( m_SlotPreview[i].PtInRect( nXPos, nYPos ) )
		{
			if( m_SlotPreview[i].GetSerial() != INVALID_SERIAL_ID )
			{

				// InfoWindow
				GetInfoWndManager()->ShowInfoWindow( TRUE,
					CInfoWndManager::INFOWND_TABLE_ITEM,
					m_SlotPreview[i].GetX_fromParent() + rect.left,
					m_SlotPreview[i].GetY_fromParent() + rect.top,
					m_SlotPreview[i].GetItemTable(),
					DIALOG_HOIPOIMIX_CAPSULE );

				bIn = TRUE;

				break;
			}
		}
	}

	if( !bIn )
	{
		if( GetInfoWndManager()->GetRequestGui() == DIALOG_HOIPOIMIX_CAPSULE )
			GetInfoWndManager()->ShowInfoWindow( FALSE );
	}
}

VOID CHoipoiMixCreate::OnCaptureKeyDown( gui::CComponent* pComponent, CInputDevice* pDevice, const CKey& key )
{
	if( pDevice->m_Type == CInputDevice::TYPE_KEYBOARD )
	{
		if( key.m_dwVKey == VK_SPACE )
			OnKeyDownFromConcent(key);
	}
}

VOID CHoipoiMixCreate::OnMovieEnd( gui::CComponent* pComponent )
{
	gui::CFlash* pFlash = (gui::CFlash*)pComponent;
	pFlash->PlayMovie( FALSE );
	pFlash->Show( FALSE );
}

VOID CHoipoiMixCreate::UpdateMaterialCheck()
{
	// 레시피 테이블 데이터를 읽어온다.
	sITEM_RECIPE_TBLDAT* pRecipeTblDat = (sITEM_RECIPE_TBLDAT*)API_GetTableContainer()->GetItemRecipeTable()->FindData( m_idxEnableRecipe );
	if( pRecipeTblDat == NULL )
	{
		DBO_FAIL( "CHoipoiMixCreate::SetRecipe - Out of item recipe table index" );
		return;
	}

	for( RwInt32 i=0; i < dHOIPOIMIX_CREATE_MATERIAL_MAX_NUM; ++i )
	{		
		if( pRecipeTblDat->asMaterial[i].materialTblidx == INVALID_SERIAL_ID )
		{
			m_apStbStackMaterial[i]->Clear();
			continue;
		}

		RwUInt32 uiCount = Logic_FindInventoryItemCountByTableId( pRecipeTblDat->asMaterial[i].materialTblidx );
		m_apStbStackMaterial[i]->Format( L"%u/%u", uiCount, pRecipeTblDat->asMaterial[i].byMaterialCount );

		if( uiCount >= pRecipeTblDat->asMaterial[i].byMaterialCount )
			m_SlotMaterial[i].ShowDisable( FALSE );
		else
			m_SlotMaterial[i].ShowDisable( TRUE );
	}
}

VOID CHoipoiMixCreate::OnClickedBtnStop( gui::CComponent* pComponent )
{
	StopCreate();
}