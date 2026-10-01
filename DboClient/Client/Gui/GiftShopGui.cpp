#include "precomp_dboclient.h"
#include "GiftShopGui.h"

// Core
#include "NtlDebug.h"

// Shared
#include "TableContainer.h"
#include "ItemTable.h"
#include "TextAllTable.h"
#include "MerchantTable.h"

// Presetation
#include "NtlPLGuiManager.h"
#include "NtlPLEvent.h"

// Simulation
#include "NtlSLEvent.h"
#include "NtlSLLogic.h"
#include "NtlSLGlobal.h"
#include "NtlSLApi.h"

// Dbo
#include "IconMoveManager.h"
#include "DboEvent.h"
#include "DboEventGenerator.h"
#include "DboGlobal.h"
#include "DboLogic.h"
#include "DisplayStringManager.h"
#include "InfoWndManager.h"
#include "DialogManager.h"

#define dGIFTSHOP_ITEM_NAME_START_X				68
#define dGIFTSHOP_ITEM_NAME_START_Y				64
#define dGIFTSHOP_CON_START_X						27
#define dGIFTSHOP_CON_START_Y						68
#define dGIFTSHOP_PRICE_START_X						204
#define dGIFTSHOP_PRICE_START_Y						86
#define dGIFTSHOP_SLOT_GAP_HORI						54

CGiftShopGui::CGiftShopGui(const RwChar* pName)
:CNtlPLGui(pName)
,m_iMouseDownSlot(dGIFTSHOP_INVALID_INDEX)
,m_iClickEffectedSlot(dGIFTSHOP_INVALID_INDEX)
,m_pExitButton(NULL)
,m_pPrePageButton(NULL)
,m_pNextPageButton(NULL)
,m_pBtnHoipoiMixOpen(NULL)
,m_pShopTitle(NULL)
,m_pLargeBuyExplan(NULL)
,m_pPocketMoneytitle(NULL)
,m_pPocketMoney(NULL)
,m_pCurrentPage(NULL)
,m_iCurTab(0)
,m_iCurPage(0)
,m_bFocus(false)
,m_iInfoWindowIndex(dGIFTSHOP_INVALID_INDEX)
,m_pTabButton(NULL)
{
}

CGiftShopGui::~CGiftShopGui()
{
}

RwBool CGiftShopGui::Create()
{
	NTL_FUNCTION( "CGiftShopGui::Create" );

	m_adwGIFTSHOP_MERCHANT_TBLIDX[0] = 2001;
	m_adwGIFTSHOP_MERCHANT_TBLIDX[1] = 2002;
	m_adwGIFTSHOP_MERCHANT_TBLIDX[2] = 2003;
	m_adwGIFTSHOP_MERCHANT_TBLIDX[3] = 2004;

	if(!CNtlPLGui::Create("", "gui\\GiftShopMainGui.srf", "gui\\GiftShopMainGui.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");

	CRectangle rect;

	// 상점 이름
	rect.SetRectWH(DBOGUI_DIALOG_TITLE_X, DBOGUI_DIALOG_TITLE_Y, 145, 14);
	m_pShopTitle = NTL_NEW gui::CStaticBox( rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_LEFT );
	m_pShopTitle->CreateFontStd( DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pShopTitle->Clear();
	m_pShopTitle->Enable(false);
	m_pShopTitle->SetText( GetDisplayStringManager()->GetString("DST_GIFT_SHOP_TITLE_NAME" ) );

	rect = GetPosition();

	// 백라인 (NetPy Shop의 공용 크롬 재사용)
	m_BackLineSurface.SetType(CWindowby3::WT_HORIZONTAL);
	m_BackLineSurface.SetSurface( 0, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "NetPyShopGui.srf", "srfBackLineTop" ) );
	m_BackLineSurface.SetSurface( 1, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "NetPyShopGui.srf", "srfBackLineCenter" ) );
	m_BackLineSurface.SetSurface( 2, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "NetPyShopGui.srf", "srfBackLineBottom" ) );
	m_BackLineSurface.SetSize(303, 351);
	m_BackLineSurface.SetPositionfromParent(9, 50);

	// 탭 버튼
	m_pTabButton = (gui::CTabButton*)GetComponent( "TabButton" );
	m_slotTab = m_pTabButton->SigSelectChanged().Connect( this, &CGiftShopGui::OnSelectChangeTabButton );

	// 슬롯 포커스 이펙트
	m_FocusEffect.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfSlotFocusEffect" ) );

	// 소지금 배경 (Gift Shop 고유)
	m_MoneyBackPanel.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GiftShopMainGui.srf", "MoneyBackPanel" ) );
	m_MoneyBackPanel.SetPositionfromParent(194, 439);

	// 페이지 배경 (NetPy Shop의 공용 크롬 재사용)
	m_PageBackPanel.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "NetPyShopGui.srf", "PageBackPanel" ) );
	m_PageBackPanel.SetPositionfromParent(203, 406);

	// Exit Button
	m_pExitButton = (gui::CButton*)GetComponent( "ExitButton" );
	m_slotCloseButton = m_pExitButton->SigClicked().Connect(this, &CGiftShopGui::ClickedCloseButton);

	// ItemPanel
	std::string fullName = "";
	char acPanelName[] = "ItemPanel";
	char acNum[3] = "";
	RwInt32	 iItemNamePosY = dGIFTSHOP_ITEM_NAME_START_Y;
	RwInt32	 iIconY = dGIFTSHOP_CON_START_Y;
	RwInt32	 iPricePosY = dGIFTSHOP_PRICE_START_Y;

	for( RwInt32 i = 0 ; i < dMAX_ITEM_PANEL ; ++i )
	{
		_itoa_s(i, acNum, sizeof(acNum), 10);

		fullName = acPanelName;
		fullName += acNum;
		m_ItemPanel[i].pItemPanel = (gui::CPanel*)GetComponent( fullName.c_str() );
		m_ItemPanel[i].pItemPanel->Enable(false);

		m_ItemPanel[i].slot.Create(m_pThis, DIALOG_GIFTSHOP, REGULAR_SLOT_ITEM_TABLE);
		m_ItemPanel[i].slot.SetPosition_fromParent(dGIFTSHOP_CON_START_X, iIconY);

		rect.SetRectWH( dGIFTSHOP_ITEM_NAME_START_X, iItemNamePosY, 200, 16);
		m_ItemPanel[i].pItemName = NTL_NEW gui::CStaticBox( rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_CENTER);
		m_ItemPanel[i].pItemName->CreateFontStd(DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
		m_ItemPanel[i].pItemName->Clear();
		m_ItemPanel[i].pItemName->Enable(false);

		rect.SetRectWH( dGIFTSHOP_PRICE_START_X, iPricePosY, 68, 16);
		m_ItemPanel[i].pPoint = NTL_NEW gui::CStaticBox( rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_RIGHT);
		m_ItemPanel[i].pPoint->CreateFontStd(DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
		m_ItemPanel[i].pPoint->SetTextColor( RGB(202, 202, 202) );
		m_ItemPanel[i].pPoint->Clear();
		m_ItemPanel[i].pPoint->Enable(false);

		iItemNamePosY += dGIFTSHOP_SLOT_GAP_HORI;
		iIconY += dGIFTSHOP_SLOT_GAP_HORI;
		iPricePosY += dGIFTSHOP_SLOT_GAP_HORI;
	}

	// 대량 구매 설명
	rect.SetRectWH(16, 379, 282, 20);
	m_pLargeBuyExplan = NTL_NEW gui::CStaticBox( rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_RIGHT);
	m_pLargeBuyExplan->CreateFontStd( "detail", 90, DEFAULT_FONT_ATTR);
	m_pLargeBuyExplan->SetText(GetDisplayStringManager()->GetString( "DST_NPCSHOP_LARGE_BUY_EXPLAIN" ));
	m_pLargeBuyExplan->Enable(false);

	// 나의 WP 소지금 제목
	rect.SetRectWH(135, 442, 63, 17);
	m_pPocketMoneytitle = NTL_NEW gui::CStaticBox( rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_CENTER);
	m_pPocketMoneytitle->CreateFontStd( DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pPocketMoneytitle->SetText(L"WP");
	m_pPocketMoneytitle->Enable(false);

	// 나의 WP 소지금
	rect.SetRectWH(185, 441, 90, 17);
	m_pPocketMoney = NTL_NEW gui::CStaticBox( rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_RIGHT );
	m_pPocketMoney->CreateFontStd( DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pPocketMoney->SetTextColor( RGB(202, 202, 202) );
	m_pPocketMoney->SetText("");
	m_pPocketMoney->Enable(false);

	// 현재 페이지 표시
	rect.SetRectWH(217, 409, 46, 16);
	m_pCurrentPage = NTL_NEW gui::CStaticBox( rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_CENTER );
	m_pCurrentPage->CreateFontStd( DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pCurrentPage->SetTextColor( RGB(255, 192, 0) );

	// PrePageButton
	m_pPrePageButton = (gui::CButton*)GetComponent( "PrePageButton" );
	m_slotPrePageButton = m_pPrePageButton->SigClicked().Connect(this, &CGiftShopGui::ClickedPrePageButton);

	// NextPageButton
	m_pNextPageButton = (gui::CButton*)GetComponent( "NextPageButton" );
	m_slotNextPageButton = m_pNextPageButton->SigClicked().Connect(this, &CGiftShopGui::ClickedNextPageButton);

	// Hoipoi Mix 바로가기 버튼
	m_pBtnHoipoiMixOpen = (gui::CButton*)GetComponent( "BtnHoipoiMixOpen" );
	m_slotClickedHoipoiMix = m_pBtnHoipoiMixOpen->SigClicked().Connect(this, &CGiftShopGui::ClickedHoipoiMixButton);

	// Sig
	m_slotMouseDown		= m_pThis->SigMouseDown().Connect( this, &CGiftShopGui::OnMouseDown );
	m_slotMouseUp		= m_pThis->SigMouseUp().Connect( this, &CGiftShopGui::OnMouseUp );
	m_slotMove			= m_pThis->SigMove().Connect( this, &CGiftShopGui::OnMove );
	m_slotMouseMove		= m_pThis->SigMouseMove().Connect( this, &CGiftShopGui::OnMouseMove );
	m_slotMouseLeave	= m_pThis->SigMouseLeave().Connect( this, &CGiftShopGui::OnMouseLeave );
	m_slotPaint			= m_pThis->SigPaint().Connect( this, &CGiftShopGui::OnPaint );
	m_slotPostPaint		= m_pNextPageButton->SigPaint().Connect( this, &CGiftShopGui::OnPostPaint );
	m_slotCaptureMouseDown = GetNtlGuiManager()->GetGuiManager()->SigCaptureMouseDown().Connect( this, &CGiftShopGui::OnCaptureMouseDown );

	LinkMsg( g_EventPickedUpHide );
	LinkMsg( g_EventGiftShopEvent );

	ClearShop();
	OnMove(100, 100);

	Show(false);

	NTL_RETURN(TRUE);
}

VOID CGiftShopGui::Destroy()
{
	NTL_FUNCTION( "CGiftShopGui::Destroy" );

	UnLinkMsg( g_EventPickedUpHide );
	UnLinkMsg( g_EventGiftShopEvent );

	CheckInfoWindow();

	for( RwInt32 i = 0 ; i < dMAX_ITEM_PANEL ; ++i )
		m_ItemPanel[i].slot.Destroy();

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();

	NTL_RETURNVOID();
}

RwInt32 CGiftShopGui::SwitchDialog(bool bOpen)
{
	if( bOpen )
	{
		Show(true);
		OpenShop();
	}
	else
	{
		Show(false);

		if( GetIconMoveManager()->GetSrcPlace() == PLACE_GIFTSHOP )
			GetIconMoveManager()->IconMoveEnd();

		CheckInfoWindow();

		CloseShop();
	}

	return 1;
}

VOID CGiftShopGui::HandleEvents( RWS::CMsg &msg )
{
	if( msg.Id == g_EventPickedUpHide )
	{
		RwInt32 nSlotIdx = (RwInt32)msg.pData;
		if( nSlotIdx != PLACE_GIFTSHOP )
			return;

		FocusEffect(false);
	}
	else if( msg.Id == g_EventGiftShopEvent )
	{
		SDboEventGiftShopEvent* pData = reinterpret_cast<SDboEventGiftShopEvent*>( msg.pData );

		if( pData->byEventType == eGIFTSHOP_EVENT_BUY_SUCCESS || pData->byEventType == eGIFTSHOP_EVENT_WP_UPDATED )
			SetZenny();
	}
}

VOID CGiftShopGui::OnSelectChangeTabButton( INT nCurIndex, INT nPrevIndex )
{
	if( GetIconMoveManager()->GetSrcPlace() == PLACE_GIFTSHOP )
		GetIconMoveManager()->IconMoveEnd();

	UpdateTabContent((RwUInt8)nCurIndex);
}

VOID CGiftShopGui::ClickedCloseButton(gui::CComponent* pComponent)
{
	GetDialogManager()->CloseDialog( DIALOG_GIFTSHOP );
	GetDialogManager()->CloseDialog( DIALOG_GIFTSHOP_TRADE );
}

VOID CGiftShopGui::ClickedHoipoiMixButton(gui::CComponent* pComponent)
{
	// set crafting position
	CNtlPLGui* pPLGuiMain = GetDialogManager()->GetDialog( DIALOG_HOIPOIMIX_RECIPE );
	CRectangle rect = pPLGuiMain->GetPosition();
	CNtlPLGui* pPLGui = GetDialogManager()->GetDialog( DIALOG_HOIPOIMIX_CRAFT );
	pPLGui->SetPosition( rect.right + NTL_LINKED_DIALOG_GAP, rect.top );

	if( GetDialogManager()->IsOpenDialog( DIALOG_HOIPOIMIX_RECIPE ) )
		GetDialogManager()->CloseDialog( DIALOG_HOIPOIMIX_RECIPE );
	else
		GetDialogManager()->OpenDialog( DIALOG_HOIPOIMIX_RECIPE, GetNtlSLGlobal()->GetSobAvatar()->GetSerialID() );
}

bool CGiftShopGui::SetPage(RwInt32 iPage)
{
	m_iCurPage = iPage;

	if(m_iCurPage < 0)
	{
		m_pCurrentPage->Clear();
		return false;
	}

	RwUInt8 byPageCount = GetPageCount_of_CurTab();
	m_pCurrentPage->Format(L"%d / %d", m_iCurPage+1, byPageCount);
	return true;
}

VOID CGiftShopGui::SetPanel(RwInt32 iPage)
{
	RwInt32 iIndex;
	CRectangle rtScreen = m_pThis->GetScreenRect();

	for( RwInt32 i = 0 ; i < dMAX_ITEM_PANEL ; ++i )
	{
		iIndex = (iPage * dMAX_ITEM_PANEL ) + i;

		m_ItemPanel[i].slot.SetParentPosition(rtScreen.left, rtScreen.top);

		if( m_ItemPanel[i].slot.SetIcon(m_aShopItem[m_iCurTab][iIndex].hItem) )
		{
			m_ItemPanel[i].slot.SetPrice(m_aShopItem[m_iCurTab][iIndex].uiPrice);
			m_ItemPanel[i].pItemName->SetText(m_aShopItem[m_iCurTab][iIndex].wstrItemName.c_str());
			m_ItemPanel[i].pItemPanel->Show(true);
			m_ItemPanel[i].pPoint->SetText(m_aShopItem[m_iCurTab][iIndex].uiPrice);
			m_ItemPanel[i].pItemPanel->SetPriority(1);
		}
	}

	SetPageButton();
}

VOID CGiftShopGui::SetPageButton()
{
	m_pPrePageButton->ClickEnable(!IsFirstPage());
	m_pNextPageButton->ClickEnable(!IsLastPage());
}

VOID CGiftShopGui::ClickedPrePageButton(gui::CComponent* pComponent)
{
	if( GetIconMoveManager()->GetSrcPlace() == PLACE_GIFTSHOP )
		GetIconMoveManager()->IconMoveEnd();

	if( IsFirstPage() )
		return;

	--m_iCurPage;
	ClearPanels();
	SetPage(m_iCurPage);
	SetPanel(m_iCurPage);
}

VOID CGiftShopGui::ClickedNextPageButton(gui::CComponent* pComponent)
{
	if( GetIconMoveManager()->GetSrcPlace() == PLACE_GIFTSHOP )
		GetIconMoveManager()->IconMoveEnd();

	if( IsLastPage() )
		return;

	++m_iCurPage;
	ClearPanels();
	SetPage(m_iCurPage);
	SetPanel(m_iCurPage);
}

VOID CGiftShopGui::SetZenny()
{
	m_pPocketMoney->SetText( Logic_FormatZeni((RwUInt32)Logic_GetWaguPoint()) );
}

RwUInt8 CGiftShopGui::GetPageCount_of_CurTab()
{
	RwUInt8 byLastIndex = 0;

	for( RwUInt8 i = 0 ; i < NTL_MAX_MERCHANT_COUNT ; ++i )
	{
		if( m_aShopItem[m_iCurTab][i].hItem != INVALID_SERIAL_ID )
			byLastIndex = i;
	}

	byLastIndex /= dMAX_ITEM_PANEL;

	return byLastIndex + 1;
}

RwBool CGiftShopGui::IsFirstPage()
{
	return dFIRST_PAGE == m_iCurPage;
}

RwBool CGiftShopGui::IsLastPage()
{
	RwInt32 iIndex;
	RwInt32 iNextPage = m_iCurPage + 1;

	if( m_iCurPage >= dMAX_PAGE -1 )
		return true;

	for( RwInt32 i = 0 ; i < dMAX_ITEM_PANEL ; ++i )
	{
		iIndex = (iNextPage * dMAX_ITEM_PANEL ) + i;
		if( m_aShopItem[m_iCurTab][iIndex].hItem != INVALID_SERIAL_ID )
			return false;
	}

	return true;
}

RwInt32 CGiftShopGui::PtinSlot(RwInt32 iX, RwInt32 iY)
{
	for( RwInt32 i = 0 ; i < dMAX_ITEM_PANEL ; ++i )
	{
		if( m_ItemPanel[i].slot.PtInRect(iX, iY) )
			return i;
	}

	return dGIFTSHOP_INVALID_INDEX;
}

VOID CGiftShopGui::FocusEffect( RwBool bPush, RwInt32 iSlotIdx /* = -1 */)
{
	if(bPush)
	{
		RwInt32 iY = dGIFTSHOP_CON_START_Y + iSlotIdx*dGIFTSHOP_SLOT_GAP_HORI;
		CRectangle rect = m_pThis->GetScreenRect();

		m_FocusEffect.SetRectWH(rect.left + dGIFTSHOP_CON_START_X, rect.top + iY, 32, 32);
		m_bFocus = true;
	}
	else
	{
		m_bFocus = false;
	}
}

VOID CGiftShopGui::CheckInfoWindow()
{
	if( GetInfoWndManager()->GetRequestGui() == DIALOG_GIFTSHOP )
	{
		m_iInfoWindowIndex = dGIFTSHOP_INVALID_INDEX;
		GetInfoWndManager()->ShowInfoWindow( FALSE );
	}
}

VOID CGiftShopGui::OnMouseDown( const CKey& key )
{
	gui::CGUIManager *pGuiMgr = CNtlPLGuiManager::GetInstance()->GetGuiManager();
	if( pGuiMgr->GetFocus() == m_pThis )
		RaiseLinked();

	if( GetIconMoveManager()->IsActive() )
		return;

	if( GetDialogManager()->GetMode() != DIALOGMODE_UNKNOWN )
		return;

	for( RwInt32 i = 0 ; i < dMAX_ITEM_PANEL ; ++i )
	{
		if( m_ItemPanel[i].slot.GetSerial() != INVALID_SERIAL_ID &&
			m_ItemPanel[i].slot.PtInRect((RwInt32)key.m_fX, (RwInt32)key.m_fY) )
		{
			m_iMouseDownSlot = i;
			m_pThis->CaptureMouse();

			m_iClickEffectedSlot = i;
			m_ItemPanel[m_iClickEffectedSlot].slot.ClickEffect(true);

			return;
		}
	}
}

VOID CGiftShopGui::OnMouseUp( const CKey& key )
{
	m_pThis->ReleaseMouse();

	if( m_iClickEffectedSlot != dGIFTSHOP_INVALID_INDEX )
	{
		m_ItemPanel[m_iClickEffectedSlot].slot.ClickEffect(false);
		m_iClickEffectedSlot = dGIFTSHOP_INVALID_INDEX;
	}

	if( !IsShow() )
	{
		m_iMouseDownSlot = dGIFTSHOP_INVALID_INDEX;
		return;
	}

	if( m_iMouseDownSlot < 0 || m_iMouseDownSlot >= dMAX_ITEM_PANEL )
		return;

	if( m_ItemPanel[m_iMouseDownSlot].slot.GetSerial() != INVALID_SERIAL_ID &&
		m_ItemPanel[m_iMouseDownSlot].slot.PtInRect((RwInt32)key.m_fX, (RwInt32)key.m_fY) )
	{
		if( key.m_nID == UD_LEFT_BUTTON )
		{
			RwInt32 iListIndex = (m_iCurPage*dMAX_ITEM_PANEL) + m_iMouseDownSlot;
			GetIconMoveManager()->IconMovePickUp(m_ItemPanel[m_iMouseDownSlot].slot.GetSerial(), PLACE_GIFTSHOP,
				iListIndex, 1, m_ItemPanel[m_iMouseDownSlot].slot.GetTexture(), m_iCurTab, m_ItemPanel[m_iMouseDownSlot].slot.GetPrice() );
		}
		else if( key.m_nID == UD_RIGHT_BUTTON )
		{
			RwInt32 iItemIndex = (m_iCurPage*dMAX_ITEM_PANEL) + m_iMouseDownSlot;

			if( key.m_dwVKey == UD_MK_CONTROL )
			{
				CDboEventGenerator::GiftShopEvent(eGIFTSHOP_EVENT_REG_ITEM_MAX,
					m_aShopItem[m_iCurTab][iItemIndex].hItem,
					m_aShopItem[m_iCurTab][iItemIndex].uiPrice,
					(wchar_t*)m_aShopItem[m_iCurTab][iItemIndex].wstrItemName.c_str(),
					m_iCurTab, iItemIndex, m_aShopItem[m_iCurTab][iItemIndex].pITEM_DATA->byMax_Stack);
			}
			else
			{
				CDboEventGenerator::GiftShopEvent(eGIFTSHOP_EVENT_REG_ITEM,
					m_aShopItem[m_iCurTab][iItemIndex].hItem,
					m_aShopItem[m_iCurTab][iItemIndex].uiPrice,
					(wchar_t*)m_aShopItem[m_iCurTab][iItemIndex].wstrItemName.c_str(),
					m_iCurTab, iItemIndex, 1);
			}
		}
	}

	m_iMouseDownSlot = dGIFTSHOP_INVALID_INDEX;
}

VOID CGiftShopGui::OnMove(RwInt32 iOldX, RwInt32 iOldY)
{
	CRectangle rtScreen = m_pThis->GetScreenRect();

	m_MoneyBackPanel.SetPositionbyParent(rtScreen.left, rtScreen.top);
	m_PageBackPanel.SetPositionbyParent(rtScreen.left, rtScreen.top);
	m_BackLineSurface.SetPositionbyParent(rtScreen.left, rtScreen.top);

	for( RwInt32 i = 0 ; i < dMAX_ITEM_PANEL ; ++i )
		m_ItemPanel[i].slot.SetParentPosition(rtScreen.left, rtScreen.top);

	MoveLinkedPLGui(rtScreen.left - iOldX, rtScreen.top - iOldY);

	CheckInfoWindow();
}

VOID CGiftShopGui::OnMouseMove(RwInt32 nFlags, RwInt32 nX, RwInt32 nY)
{
	RwInt32 iPtinSlot = PtinSlot(nX, nY);

	if( iPtinSlot != dGIFTSHOP_INVALID_INDEX )
	{
		if( m_iClickEffectedSlot != dGIFTSHOP_INVALID_INDEX )
		{
			if( m_iClickEffectedSlot == iPtinSlot )
				m_ItemPanel[m_iClickEffectedSlot].slot.ClickEffect(true);
			else
				m_ItemPanel[m_iClickEffectedSlot].slot.ClickEffect(false);
		}

		if( m_ItemPanel[iPtinSlot].slot.GetSerial() != INVALID_SERIAL_ID )
		{
			FocusEffect(true, iPtinSlot);

			if( m_iInfoWindowIndex != iPtinSlot )
			{
				CRectangle rtScreen = m_pThis->GetScreenRect();
				GetInfoWndManager()->ShowInfoWindow( TRUE, CInfoWndManager::INFOWND_TABLE_ITEM,
					rtScreen.left + m_ItemPanel[iPtinSlot].slot.GetX_fromParent(),
					rtScreen.top + m_ItemPanel[iPtinSlot].slot.GetY_fromParent(),
					m_aShopItem[m_iCurTab][m_iCurPage*dMAX_ITEM_PANEL + iPtinSlot].pITEM_DATA, DIALOG_GIFTSHOP );
				m_iInfoWindowIndex = iPtinSlot;
			}
		}
		else
		{
			m_iInfoWindowIndex = dGIFTSHOP_INVALID_INDEX;
			GetInfoWndManager()->ShowInfoWindow( FALSE );
		}
	}
	else
	{
		FocusEffect(false);

		if( m_iClickEffectedSlot != dGIFTSHOP_INVALID_INDEX )
			m_ItemPanel[m_iClickEffectedSlot].slot.ClickEffect(false);

		m_iInfoWindowIndex = dGIFTSHOP_INVALID_INDEX;
		GetInfoWndManager()->ShowInfoWindow( FALSE );
	}
}

VOID CGiftShopGui::OnMouseLeave(gui::CComponent* pComponent)
{
	FocusEffect(false);
	m_iInfoWindowIndex = dGIFTSHOP_INVALID_INDEX;
	GetInfoWndManager()->ShowInfoWindow( FALSE );
}

VOID CGiftShopGui::OnPaint()
{
	m_BackLineSurface.Render();
	m_MoneyBackPanel.Render();
	m_PageBackPanel.Render();
}

VOID CGiftShopGui::OnPostPaint()
{
	for( RwInt32 i = 0 ; i < dMAX_ITEM_PANEL ; ++i )
		m_ItemPanel[i].slot.Paint();

	if( m_bFocus )
		m_FocusEffect.Render();
}

VOID CGiftShopGui::OnCaptureMouseDown(const CKey& key)
{
	CAPTURE_MOUSEDOWN_RAISE(DIALOG_GIFTSHOP, key.m_fX, key.m_fY);
}

VOID CGiftShopGui::ClearShop()
{
	ClearPanels();

	m_pPocketMoney->Clear();
	m_pCurrentPage->Clear();

	for( RwInt32 i = 0 ; i < dGIFTSHOP_TAB_NUMS ; ++i )
	{
		for( RwInt32 j = 0 ; j < NTL_MAX_MERCHANT_COUNT ; ++j )
		{
			m_aShopItem[i][j].hItem		= INVALID_SERIAL_ID;
			m_aShopItem[i][j].wstrItemName.clear();
			m_aShopItem[i][j].pITEM_DATA	= NULL;
		}
	}

	m_pTabButton->ClearTab();

	m_iCurTab = 0;
	SetPage(-1);
}

VOID CGiftShopGui::OpenShop()
{
	// 카트를 연다
	CRectangle rect = GetPosition();
	CNtlPLGui* pPLGui = GetDialogManager()->GetDialog( DIALOG_GIFTSHOP_TRADE );
	pPLGui->SetPosition( rect.left + rect.GetWidth() + NTL_LINKED_DIALOG_GAP, rect.top );

	GetDialogManager()->OpenDialog( DIALOG_GIFTSHOP_TRADE );

	CTextTable* pMerchantTextTable = API_GetTableContainer()->GetTextAllTable()->GetMerchantTbl();
	CTextTable* pItemTextTable = API_GetTableContainer()->GetTextAllTable()->GetItemTbl();

	char acBuffer[256] = "";
	for(RwInt32 iTabIndex = 0 ; iTabIndex < dGIFTSHOP_TAB_NUMS; ++iTabIndex )
	{
		if( m_adwGIFTSHOP_MERCHANT_TBLIDX[iTabIndex] <= 0 )
			continue;

		sMERCHANT_TBLDAT* pMERCHANT_TBLDAT = Logic_GetMerchantDataFromTable(m_adwGIFTSHOP_MERCHANT_TBLIDX[iTabIndex]);
		if(!pMERCHANT_TBLDAT)
			continue;

		const wchar_t* pwcMerchantName = pMerchantTextTable->GetText(pMERCHANT_TBLDAT->Tab_Name).c_str();
		WideCharToMultiByte(GetACP(), 0, pwcMerchantName, -1, acBuffer, 256, NULL, NULL);
		std::string str = acBuffer;
		m_pTabButton->AddTab(str);

		sITEM_TBLDAT* pITEM_DATA;
		for( RwInt32 iMerchantIndex = 0 ; iMerchantIndex < NTL_MAX_MERCHANT_COUNT ; ++iMerchantIndex )
		{
			pITEM_DATA = Logic_GetItemDataFromTable(pMERCHANT_TBLDAT->aitem_Tblidx[iMerchantIndex]);
			if(!pITEM_DATA)
				continue;

			if( pMERCHANT_TBLDAT->aitem_Tblidx[iMerchantIndex] == 0 )
				m_aShopItem[iTabIndex][iMerchantIndex].hItem = INVALID_SERIAL_ID;
			else
				m_aShopItem[iTabIndex][iMerchantIndex].hItem = pMERCHANT_TBLDAT->aitem_Tblidx[iMerchantIndex];

			pItemTextTable->GetText(pITEM_DATA->Name, &m_aShopItem[iTabIndex][iMerchantIndex].wstrItemName);
			m_aShopItem[iTabIndex][iMerchantIndex].pITEM_DATA = pITEM_DATA;
			m_aShopItem[iTabIndex][iMerchantIndex].uiPrice = pMERCHANT_TBLDAT->adwNeedZenny[iMerchantIndex];
		}
	}

	SetZenny();

	m_pTabButton->SelectTab(0);
	UpdateTabContent(0);
}

VOID CGiftShopGui::CloseShop()
{
	if( GetDialogManager()->IsMode( DIALOGMODE_ITEM_REPAIR ) ||
		GetDialogManager()->IsMode( DIALOGMODE_NPCSHOP_ITEM_IDENTIFICATION ) )
		GetDialogManager()->OffMode();

	GetDialogManager()->CloseDialog( DIALOG_GIFTSHOP );
	GetDialogManager()->CloseDialog( DIALOG_GIFTSHOP_TRADE );

	ClearShop();
}

VOID CGiftShopGui::UpdateTabContent(RwUInt8 byIndex)
{
	m_iCurTab = byIndex;

	ClearPanels();
	SetPage(dFIRST_PAGE);
	SetPanel(dFIRST_PAGE);
}

VOID CGiftShopGui::ClearPanels()
{
	m_iMouseDownSlot = -1;

	for( RwInt32 i = 0 ; i < dMAX_ITEM_PANEL ; ++i )
	{
		m_ItemPanel[i].slot.Clear();
		m_ItemPanel[i].pItemName->Clear();
		m_ItemPanel[i].pPoint->Clear();
		m_ItemPanel[i].pItemPanel->Show(false);
	}
}
