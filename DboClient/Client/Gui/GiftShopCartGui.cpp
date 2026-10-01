#include "precomp_dboclient.h"
#include "GiftShopCartGui.h"

// core
#include "NtlDebug.h"

// shared
#include "ItemTable.h"

// presentation
#include "NtlPLDef.h"
#include "NtlPLGuiManager.h"

// simulation
#include "NtlSLEvent.h"
#include "NtlSLGlobal.h"
#include "NtlSLLogic.h"

// dbo
#include "IconMoveManager.h"
#include "DisplayStringManager.h"
#include "DboLogic.h"
#include "InfoWndManager.h"
#include "DialogManager.h"
#include "DboGlobal.h"
#include "DboEvent.h"
#include "DboPacketGenerator.h"
#include "DboEventGenerator.h"

namespace
{
#define dGIFTSHOP_MAX_TRADE_OVERLAP_COUNT		20	///< 한 슬롯당 거래할 수 있는 최대 아이템 갯수

#define dGIFTSHOP_GUI_SLOT_HORI_GAP			42
#define dGIFTSHOP_GUI_SLOT_VERT_GAP			57
#define dGIFTSHOP_GUI_BUTTON_HORI_GAP			19
}

CGiftShopCartGui::CGiftShopCartGui(const RwChar* pName)
:CNtlPLGui(pName)
,m_pBuyButton(NULL)
,m_pTotalBuyMoney(NULL)
,m_uiTotalBuyPrice(0)
,m_bFocus(FALSE)
,m_byInfoWindowIndex(INVALID_BYTE)
,m_iMouseDownSlot(dGIFTSHOP_CART_INVALID_INDEX)
,m_pDialogName(NULL)
{
	for(RwInt32 j = 0 ; j < MAX_SLOT ; ++j )
	{
		m_pUpButton[j] = NULL;
		m_pDownButton[j] = NULL;
	}
}

CGiftShopCartGui::~CGiftShopCartGui()
{
	NTL_FUNCTION("CGiftShopCartGui::~CGiftShopCartGui");

	Destroy();

	NTL_RETURNVOID();
}

RwBool CGiftShopCartGui::Create()
{
	NTL_FUNCTION( "CGiftShopCartGui::Create" );

	char acSurfaceName[64];

	if(!CNtlPLGui::Create("gui\\GiftShopGui.rsr", "gui\\GiftShopCartGui.srf", "gui\\GiftShopCartGui.frm"))
		NTL_RETURN(FALSE);

	sprintf_s(acSurfaceName, 64, "GiftShopCartGui.srf");

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");

	CRectangle rect;

	// 다이얼로그 이름 스태틱
	rect.SetRectWH(DBOGUI_DIALOG_TITLE_X, DBOGUI_DIALOG_TITLE_Y, 130, 14);
	m_pDialogName = NTL_NEW gui::CStaticBox( rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_LEFT );
	m_pDialogName->CreateFontStd(DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pDialogName->Enable(false);

	// 구입 버튼
	m_pBuyButton = (gui::CButton*)GetComponent( "BuyButton" );
	if( m_pBuyButton )
		m_slotClickedBuy = m_pBuyButton->SigClicked().Connect(this, &CGiftShopCartGui::ClickedBuyButton);

	// 아이템 갯수 더하기/빼기 버튼 및 슬롯
	RwInt32 iButtonX;
	RwInt32 iButtonY = 93;
	RwInt32 iSlotX;
	RwInt32 iSlotY = 57;

	for( RwInt32 j = 0 ; j < MAX_SLOT ; ++j )
	{
		if( (j%2) == 0 )
		{
			iButtonX = 16;
			iSlotX = 18;
		}
		else
		{
			iButtonX = 16 + dGIFTSHOP_GUI_SLOT_HORI_GAP;
			iSlotX = 18 + dGIFTSHOP_GUI_SLOT_HORI_GAP;
		}

		// 아이템 갯수 더하기 버튼
		rect.SetRectWH(iButtonX, iButtonY, 18, 15);
		m_pUpButton[j] = NTL_NEW gui::CButton(rect, "",
			GetNtlGuiManager()->GetSurfaceManager()->GetSurface( acSurfaceName, "srfUpButtonUp" ),
			GetNtlGuiManager()->GetSurfaceManager()->GetSurface( acSurfaceName, "srfUpButtonDown" ),
			GetNtlGuiManager()->GetSurfaceManager()->GetSurface( acSurfaceName, "srfUpButtonDis" ),
			GetNtlGuiManager()->GetSurfaceManager()->GetSurface( acSurfaceName, "srfUpButtonFocus" ),
			NTL_BUTTON_UP_COLOR, NTL_BUTTON_DOWN_COLOR, NTL_BUTTON_FOCUS_COLOR, NTL_BUTTON_UP_COLOR,
			GUI_BUTTON_DOWN_COORD_X, GUI_BUTTON_DOWN_COORD_Y, m_pThis, GetNtlGuiManager()->GetSurfaceManager() );
		m_slotUpButton[j] = m_pUpButton[j]->SigClicked().Connect(this, &CGiftShopCartGui::ClickUpButton);

		// 아이템 갯수 빼기 버튼
		rect.SetRectWH(iButtonX + dGIFTSHOP_GUI_BUTTON_HORI_GAP, iButtonY, 18, 15);
		m_pDownButton[j] = NTL_NEW gui::CButton(rect, "",
			GetNtlGuiManager()->GetSurfaceManager()->GetSurface( acSurfaceName, "srfDownButtonUp" ),
			GetNtlGuiManager()->GetSurfaceManager()->GetSurface( acSurfaceName, "srfDownButtonDown" ),
			GetNtlGuiManager()->GetSurfaceManager()->GetSurface( acSurfaceName, "srfDownButtonDis" ),
			GetNtlGuiManager()->GetSurfaceManager()->GetSurface( acSurfaceName, "srfDownButtonFocus" ),
			NTL_BUTTON_UP_COLOR, NTL_BUTTON_DOWN_COLOR, NTL_BUTTON_FOCUS_COLOR, NTL_BUTTON_UP_COLOR,
			GUI_BUTTON_DOWN_COORD_X, GUI_BUTTON_DOWN_COORD_Y, m_pThis, GetNtlGuiManager()->GetSurfaceManager() );
		m_slotDownButton[j] = m_pDownButton[j]->SigClicked().Connect(this, &CGiftShopCartGui::ClickDownButton);

		m_BuySlotInfo[j].slot.Create(m_pThis, DIALOG_SHOPING_CART, REGULAR_SLOT_ITEM_TABLE, SDS_COUNT);
		m_BuySlotInfo[j].slot.SetPosition_fromParent(iSlotX, iSlotY);

		if( (j%2) != 0 )
		{
			iButtonY += dGIFTSHOP_GUI_SLOT_VERT_GAP;
			iSlotY += dGIFTSHOP_GUI_SLOT_VERT_GAP;
		}
	}

	// 총 구입 금액
	rect.SetRectWH( 14, 411, 61, 16);
	m_pTotalBuyMoney = NTL_NEW gui::CStaticBox( rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_RIGHT );
	m_pTotalBuyMoney->CreateFontStd( DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pTotalBuyMoney->SetText( "0");
	m_pTotalBuyMoney->Enable(false);

	// 슬롯 포커스 이펙트
	m_FocusEffect.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfSlotFocusEffect" ) );

	// Sig
	m_slotMouseDown		= m_pThis->SigMouseDown().Connect( this, &CGiftShopCartGui::OnMouseDown );
	m_slotMouseUp		= m_pThis->SigMouseUp().Connect(this, &CGiftShopCartGui::OnMouseUp);
	m_slotMove			= m_pThis->SigMove().Connect( this, &CGiftShopCartGui::OnMove );
	m_slotMouseMove		= m_pThis->SigMouseMove().Connect( this, &CGiftShopCartGui::OnMouseMove );
	m_slotMouseLeave	= m_pThis->SigMouseLeave().Connect( this, &CGiftShopCartGui::OnMouseLeave );
	m_slotPaint			= m_pThis->SigPaint().Connect( this, &CGiftShopCartGui::OnPaint );
	m_slotCaptureWheelMove = GetNtlGuiManager()->GetGuiManager()->SigCaptureWheelMove().Connect( this, &CGiftShopCartGui::OnCaptureWheelMove );
	m_slotCaptureMouseDown = GetNtlGuiManager()->GetGuiManager()->SigCaptureMouseDown().Connect( this, &CGiftShopCartGui::OnCaptureMouseDown );

	LinkMsg( g_EventGiftShopEvent );

	Show(false);

	Clear();

	NTL_RETURN(TRUE);
}

VOID CGiftShopCartGui::Destroy()
{
	NTL_FUNCTION( "CGiftShopCartGui::Destroy" );

	CheckInfoWindow();

	for( RwInt32 i = 0 ; i < MAX_SLOT ; ++i )
		m_BuySlotInfo[i].slot.Destroy();

	UnLinkMsg( g_EventGiftShopEvent );

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();

	NTL_RETURNVOID();
}

VOID CGiftShopCartGui::Clear()
{
	m_iMouseDownSlot = dGIFTSHOP_CART_INVALID_INDEX;

	for( RwInt32 i = 0 ; i < MAX_SLOT ; ++i )
		ClearSlot(i);

	CalcTotalBuyPrice();
}

VOID CGiftShopCartGui::ClearSlot(RwInt32 iSlot)
{
	m_BuySlotInfo[iSlot].slot.Clear();
	memset((void*)&m_BuySlotInfo[iSlot].GiftShopBuyInfo, 0, sizeof(sSHOP_BUY_CART) );
	m_BuySlotInfo[iSlot].GiftShopBuyInfo.byItemPos = INVALID_BYTE;

	m_iMouseDownSlot = dGIFTSHOP_CART_INVALID_INDEX;
}

VOID CGiftShopCartGui::OnCaptureWheelMove(RwInt32 iFlag, RwInt16 sDelta, CPos& pos)
{
	if( !IsShow() )
		return;

	if( m_pThis->GetParent()->GetChildComponentReverseAt( pos.x, pos.y ) != m_pThis )
		return;

	if( m_pThis->PosInRect( pos.x, pos.y ) != gui::CComponent::INRECT )
		return;

	RwInt32 iSlot;
	CRectangle rtScreen = m_pThis->GetScreenRect();

	iSlot = PtinSlot(pos.x - rtScreen.left, pos.y - rtScreen.top);
	if( iSlot != dGIFTSHOP_CART_INVALID_INDEX )
	{
		if( sDelta > 0 )
			AddItem(iSlot, 1);
		else
			SubItem(iSlot, 1);
	}
}

VOID CGiftShopCartGui::OnCaptureMouseDown(const CKey& key)
{
	CAPTURE_MOUSEDOWN_RAISE_TOP_LINKED(DIALOG_GIFTSHOP_TRADE, key.m_fX, key.m_fY);
}

VOID CGiftShopCartGui::ClickedBuyButton(gui::CComponent* pComponent)
{
	sSHOP_BUY_CART aBuyCart[MAX_SLOT];
	RwUInt8 byCount = 0;
	for( RwInt32 i = 0 ; i < MAX_SLOT ; ++i )
	{
		if( m_BuySlotInfo[i].GiftShopBuyInfo.byItemPos != INVALID_BYTE )
		{
			memcpy( &aBuyCart[byCount], &m_BuySlotInfo[i].GiftShopBuyInfo, sizeof( sSHOP_BUY_CART ) );
			byCount++;
		}
	}

	if( byCount == 0 )
		return;

	GetDboGlobal()->GetGamePacketGenerator()->SendShopGiftItemBuyReq( byCount, aBuyCart );
}

VOID CGiftShopCartGui::AddItemCount(RwInt32 iSlot, RwInt32 iCount)
{
	RwInt32 iResult = m_BuySlotInfo[iSlot].GiftShopBuyInfo.byStack + iCount;

	if( iResult > dGIFTSHOP_MAX_TRADE_OVERLAP_COUNT )
		iResult = dGIFTSHOP_MAX_TRADE_OVERLAP_COUNT;

	if( iResult <= 0 )
	{
		ClearSlot(iSlot);
		return;
	}

	m_BuySlotInfo[iSlot].GiftShopBuyInfo.byStack = (RwUInt8)iResult;
	m_BuySlotInfo[iSlot].slot.SetCount(iResult);
}

VOID CGiftShopCartGui::CalcTotalBuyPrice()
{
	RwInt32 iTotalPrice = 0;
	for( RwInt32 i = 0 ; i < MAX_SLOT ; ++i )
	{
		if( m_BuySlotInfo[i].slot.GetSerial() != INVALID_SERIAL_ID )
			iTotalPrice += m_BuySlotInfo[i].slot.GetPrice() * m_BuySlotInfo[i].GiftShopBuyInfo.byStack;
	}

	m_uiTotalBuyPrice = iTotalPrice;
	if( m_pTotalBuyMoney )
		m_pTotalBuyMoney->SetText(Logic_FormatZeni((RwUInt32)m_uiTotalBuyPrice));

	if( m_pBuyButton )
		m_pBuyButton->ClickEnable(m_uiTotalBuyPrice != 0);
}

VOID CGiftShopCartGui::ClickUpButton(gui::CComponent* pComponent)
{
	for( RwUInt8 j = 0 ; j < MAX_SLOT ; ++j )
	{
		if( m_pUpButton[j] == pComponent )
		{
			AddItem(j, 1);
			break;
		}
	}
}

VOID CGiftShopCartGui::ClickDownButton(gui::CComponent* pComponent)
{
	for( RwUInt8 j = 0 ; j < MAX_SLOT ; ++j )
	{
		if( m_pDownButton[j] == pComponent )
		{
			SubItem(j, 1);
			break;
		}
	}
}

VOID CGiftShopCartGui::OnMouseDown(const CKey& key)
{
	m_iMouseDownSlot = dGIFTSHOP_CART_INVALID_INDEX;

	if( GetDialogManager()->GetMode() != DIALOGMODE_UNKNOWN )
		return;

	for( RwInt32 i = 0 ; i < MAX_SLOT ; ++i )
	{
		if( m_BuySlotInfo[i].slot.PtInRect((RwInt32)key.m_fX, (RwInt32)key.m_fY) )
		{
			m_iMouseDownSlot = i;
			m_pThis->CaptureMouse();
			return;
		}
	}
}

VOID CGiftShopCartGui::OnMouseUp(const CKey& key)
{
	m_pThis->ReleaseMouse();

	if( !IsShow() )
	{
		m_iMouseDownSlot = dGIFTSHOP_CART_INVALID_INDEX;
		return;
	}

	for(RwInt32 i = 0 ; i < MAX_SLOT ; ++i )
	{
		if( m_BuySlotInfo[i].slot.PtInRect((RwInt32)key.m_fX, (RwInt32)key.m_fY) )
		{
			if( key.m_nID == UD_LEFT_BUTTON )
			{
				if( !GetIconMoveManager()->IsActive() )
					break;

				if( GetIconMoveManager()->GetSrcPlace() == PLACE_GIFTSHOP )
				{
					RegBuyItemByDrag(i);
					GetIconMoveManager()->IconMoveEnd();
				}
			}
			else if( key.m_nID == UD_RIGHT_BUTTON )
			{
				if( GetIconMoveManager()->IsActive() )
					break;

				if( m_iMouseDownSlot == i )
				{
					ClearSlot(i);
					CalcTotalBuyPrice();
				}
			}

			break;
		}
	}

	m_iMouseDownSlot = dGIFTSHOP_CART_INVALID_INDEX;
}

VOID CGiftShopCartGui::OnMove( RwInt32 iOldX, RwInt32 iOldY )
{
	CRectangle rect = GetPosition();

	for( RwInt32 i = 0 ; i < MAX_SLOT ; ++i )
		m_BuySlotInfo[i].slot.SetParentPosition(rect.left, rect.top);

	m_bFocus = FALSE;

	CheckInfoWindow();
}

VOID CGiftShopCartGui::OnMouseMove(RwInt32 nFlags, RwInt32 nX, RwInt32 nY)
{
	FocusEffect(FALSE);

	for( RwInt32 i = 0 ; i < MAX_SLOT ; ++i )
	{
		if( m_BuySlotInfo[i].slot.PtInRect(nX, nY) )
		{
			FocusEffect(TRUE, i);

			if( GetIconMoveManager()->IsActive() )
				return;

			if( m_BuySlotInfo[i].slot.GetSerial() != INVALID_SERIAL_ID )
			{
				if( m_byInfoWindowIndex != i )
				{
					CRectangle rtScreen = m_pThis->GetScreenRect();
					GetInfoWndManager()->ShowInfoWindow(TRUE, CInfoWndManager::INFOWND_TABLE_ITEM,
						rtScreen.left + m_BuySlotInfo[i].slot.GetX_fromParent(),
						rtScreen.top + m_BuySlotInfo[i].slot.GetY_fromParent(),
						m_BuySlotInfo[i].slot.GetItemTable(), DIALOG_SHOPING_CART );
					m_byInfoWindowIndex = (RwInt8)i;
				}
			}

			return;
		}
	}

	m_byInfoWindowIndex = INVALID_BYTE;
	GetInfoWndManager()->ShowInfoWindow( FALSE );
}

VOID CGiftShopCartGui::OnMouseLeave(gui::CComponent* pComponent)
{
	FocusEffect(FALSE);
	m_byInfoWindowIndex = INVALID_BYTE;
	GetInfoWndManager()->ShowInfoWindow( FALSE );
}

VOID CGiftShopCartGui::FocusEffect( RwBool bPush, RwInt32 iSlotIdx /* = -1 */)
{
	if( bPush)
	{
		RwInt32 iX = 18;
		RwInt32 iY = 57 + (iSlotIdx/2) * dGIFTSHOP_GUI_SLOT_VERT_GAP;
		CRectangle rect = m_pThis->GetScreenRect();

		if( (iSlotIdx%2) != 0 )
			iX += dGIFTSHOP_GUI_SLOT_HORI_GAP;

		m_FocusEffect.SetRectWH(rect.left + iX, rect.top + iY, 32, 32);
		m_bFocus = TRUE;
	}
	else
	{
		m_bFocus = FALSE;
	}
}

VOID CGiftShopCartGui::AddItem(RwInt32 iSlotY, RwInt32 iCount)
{
	if( iSlotY < 0 || iSlotY >= MAX_SLOT )
	{
		NTL_ASSERT(false, "CGiftShopCartGui::AddItem, Unknown index " << iSlotY);
		return;
	}

	if( m_BuySlotInfo[iSlotY].slot.GetSerial() != INVALID_SERIAL_ID )
	{
		if( m_BuySlotInfo[iSlotY].slot.GetItemTable()->byMax_Stack >= m_BuySlotInfo[iSlotY].slot.GetCount() + iCount )
		{
			AddItemCount(iSlotY, iCount);
			CalcTotalBuyPrice();
		}
	}
}

VOID CGiftShopCartGui::SubItem(RwInt32 iSlotY, RwInt32 iCount)
{
	if( iSlotY < 0 || iSlotY >= MAX_SLOT )
	{
		NTL_ASSERT(false, "CGiftShopCartGui::SubItem, Unknown index " << iSlotY);
		return;
	}

	if( m_BuySlotInfo[iSlotY].slot.GetSerial() != INVALID_SERIAL_ID )
	{
		AddItemCount(iSlotY, -iCount);
		CalcTotalBuyPrice();
	}
}

VOID CGiftShopCartGui::RegBuyItemByDrag(RwInt32 iSlot)
{
	sITEM_TBLDAT* pITEM_DATA = Logic_GetItemDataFromTable( GetIconMoveManager()->GetSrcSerial() );
	if(!pITEM_DATA)
		return;

	RwUInt8 byMerchantTab	= (BYTE)GetIconMoveManager()->GetEXData1();
	RwUInt8 byItemPos		= (BYTE)GetIconMoveManager()->GetSrcSlotIdx();

	if( eDURATIONTYPE_FLATSUM == pITEM_DATA->byDurationType ||
		eDURATIONTYPE_METERRATE == pITEM_DATA->byDurationType )
	{
		// 기간제 아이템은 선물 상점에서 취급하지 않는다.
		return;
	}

	if( m_BuySlotInfo[iSlot].slot.GetCount() <= 0 )
	{
		RwInt32 iCount = GetIconMoveManager()->GetStackCount();
		RwInt32 iPrice = GetIconMoveManager()->GetEXData2();

		CRectangle rect = m_pThis->GetScreenRect();
		m_BuySlotInfo[iSlot].slot.SetParentPosition(rect.left, rect.top);
		m_BuySlotInfo[iSlot].slot.SetIcon(GetIconMoveManager()->GetSrcSerial(), iCount);
		m_BuySlotInfo[iSlot].slot.SetPrice(iPrice);

		m_BuySlotInfo[iSlot].GiftShopBuyInfo.byMerchantTab	= byMerchantTab;
		m_BuySlotInfo[iSlot].GiftShopBuyInfo.byItemPos		= byItemPos;

		AddItemCount(iSlot, iCount);
		CalcTotalBuyPrice();
	}
	else if( m_BuySlotInfo[iSlot].slot.GetSerial() == GetIconMoveManager()->GetSrcSerial() )
	{
		if( pITEM_DATA->byMax_Stack > m_BuySlotInfo[iSlot].GiftShopBuyInfo.byStack)
			AddItem(iSlot, 1);
	}
}

VOID CGiftShopCartGui::RegBuyItemByEvent(RwInt32 iSlot, SDboEventGiftShopEvent& BuyInfo)
{
	sITEM_TBLDAT* pITEM_DATA = Logic_GetItemDataFromTable( BuyInfo.uiSerial );
	if( !pITEM_DATA )
	{
		DBO_FAIL("Not exist item table of index : " << BuyInfo.uiSerial);
		return;
	}

	CRectangle rect = m_pThis->GetScreenRect();

	m_BuySlotInfo[iSlot].slot.SetSerialType(REGULAR_SLOT_ITEM_TABLE);
	m_BuySlotInfo[iSlot].slot.SetParentPosition(rect.left, rect.top);
	m_BuySlotInfo[iSlot].slot.SetIcon(BuyInfo.uiSerial, BuyInfo.nOverlapCount);
	m_BuySlotInfo[iSlot].slot.SetPrice(BuyInfo.ulPrice);

	m_BuySlotInfo[iSlot].GiftShopBuyInfo.byMerchantTab = (RwUInt8)BuyInfo.nPlace;
	m_BuySlotInfo[iSlot].GiftShopBuyInfo.byItemPos = (RwUInt8)BuyInfo.nPosition;

	AddItemCount(iSlot, BuyInfo.nOverlapCount);
	CalcTotalBuyPrice();
}

RwInt32 CGiftShopCartGui::FindEmptySlot()
{
	for( RwInt32 i = 0 ; i < MAX_SLOT ; ++i )
	{
		if( m_BuySlotInfo[i].slot.GetCount() <= 0 )
			return i;
	}

	return dGIFTSHOP_CART_INVALID_INDEX;
}

RwInt32	 CGiftShopCartGui::PtinSlot(RwInt32 iX, RwInt32 iY)
{
	for( RwInt32 i = 0 ; i < MAX_SLOT ; ++i )
	{
		if( m_BuySlotInfo[i].slot.PtInRect(iX, iY)  )
			return i;
	}

	return dGIFTSHOP_CART_INVALID_INDEX;
}

VOID CGiftShopCartGui::CheckInfoWindow()
{
	if( GetInfoWndManager()->GetRequestGui() == DIALOG_SHOPING_CART )
	{
		m_byInfoWindowIndex = INVALID_BYTE;
		GetInfoWndManager()->ShowInfoWindow( FALSE );
	}
}

RwInt32 CGiftShopCartGui::SwitchDialog(bool bOpen)
{
	if( bOpen )
	{
		if( m_pBuyButton )
		{
			m_pBuyButton->SetText(GetDisplayStringManager()->GetString("DST_TRADECART_BUY"));
			m_pBuyButton->ClickEnable(false);
		}

		for( RwUInt8 i = 0 ; i < MAX_SLOT ; ++i )
		{
			if( m_pUpButton[i] )
				m_pUpButton[i]->Show(true);
			if( m_pDownButton[i] )
				m_pDownButton[i]->Show(true);
		}

		SetMovable(false);

		Show(true);
	}
	else
	{
		Show(false);
		CheckInfoWindow();

		Clear();

		m_uiTotalBuyPrice = 0;
	}

	return 1;
}

VOID CGiftShopCartGui::OnPaint()
{
	for( RwInt32 i = 0 ; i< MAX_SLOT ; ++i )
		m_BuySlotInfo[i].slot.Paint();

	if( m_bFocus )
		m_FocusEffect.Render();
}

VOID CGiftShopCartGui::HandleEvents( RWS::CMsg &msg )
{
	NTL_FUNCTION( "CGiftShopCartGui::HandleEvents" );

	if( msg.Id == g_EventGiftShopEvent )
	{
		SDboEventGiftShopEvent* pData = reinterpret_cast<SDboEventGiftShopEvent*>( msg.pData );

		switch( pData->byEventType )
		{
		case eGIFTSHOP_EVENT_REG_ITEM:
			{
				RwInt32 iSlot = dGIFTSHOP_CART_INVALID_INDEX;

				for( RwInt32 i = 0 ; i < MAX_SLOT ; ++i )
				{
					if( m_BuySlotInfo[i].slot.GetSerial() == pData->uiSerial )
					{
						if(m_BuySlotInfo[i].slot.GetItemTable()->byMax_Stack > m_BuySlotInfo[i].GiftShopBuyInfo.byStack)
						{
							AddItem(i, 1);
							iSlot = i;
							break;
						}
					}
				}

				if(iSlot == dGIFTSHOP_CART_INVALID_INDEX)
				{
					iSlot = FindEmptySlot();

					if( iSlot >= 0 )
						RegBuyItemByEvent(iSlot, *pData);
					else
						GetAlarmManager()->AlarmMessage("DST_TRADECART_NO_MORE_SLOT");
				}
			}
			break;
		case eGIFTSHOP_EVENT_REG_ITEM_MAX:
			{
				RwInt32 iSlot = FindEmptySlot();
				if( iSlot >= 0 )
				{
					RegBuyItemByEvent(iSlot, *pData);
				}
				else
				{
					for( RwInt32 i = 0 ; i < MAX_SLOT ; ++i )
					{
						if( m_BuySlotInfo[i].slot.GetSerial() == pData->uiSerial )
						{
							if(m_BuySlotInfo[i].slot.GetItemTable()->byMax_Stack > m_BuySlotInfo[i].slot.GetCount())
							{
								AddItem(i, m_BuySlotInfo[i].slot.GetItemTable()->byMax_Stack - m_BuySlotInfo[i].slot.GetCount());
								break;
							}
						}
					}
				}
			}
			break;
		case eGIFTSHOP_EVENT_BUY_SUCCESS:
			{
				for(RwInt32 i = 0 ; i < MAX_SLOT ; ++i )
					ClearSlot(i);

				CalcTotalBuyPrice();

				CheckInfoWindow();
			}
			break;
		default:
			break;
		}
	}

	NTL_RETURNVOID();
}
