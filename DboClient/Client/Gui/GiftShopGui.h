/******************************************************************************
* File			: GiftShopGui.h
* Abstract		:
*****************************************************************************
* Desc			: Gift Shop (Wagu Point Shop) 의 GUI 및 기능을 정의하는 Class
*****************************************************************************/

#ifndef __GIFTSHOP_GUI_H__
#define __GIFTSHOP_GUI_H__

#pragma once

// Core
#include "ceventhandler.h"

// Shared
#include "NtlItem.h"

// Gui
#include "eventtimer.h"

// Presentation
#include "NtlPLGui.h"

// Simulation
#include "NtlSLDef.h"

// Client
#include "SurfaceGui.h"
#include "SlotGui.h"
#include "Windowby3.h"

#define dGIFTSHOP_INVALID_INDEX		(-1)

class CGiftShopGui : public CNtlPLGui, public RWS::CEventHandler
{
#define dFIRST_PAGE			0
#define dMAX_PAGE			6			///< 한 탭 당 등록 수 있는 최대 페이지
#define dMAX_ITEM_PANEL		6			///< Shop의 아이템 패널 갯수
#define dGIFTSHOP_TAB_NUMS 4

public:
	CGiftShopGui(const RwChar* pName);
	virtual ~CGiftShopGui();

	struct ItemPanel
	{
		CRegularSlotGui			slot;			///< slot
		gui::CPanel*		pItemPanel;		///< Item panel
		gui::CStaticBox*	pItemName;		///< 이름
		gui::CStaticBox*	pPoint;			///< 필요한 포인트(가격)
	};

	struct ShopItem
	{
		SERIAL_HANDLE	hItem;
		std::wstring	wstrItemName;
		RwUInt32		uiPrice;
		sITEM_TBLDAT*	pITEM_DATA;
	};

	RwBool			Create();
	VOID			Destroy();

	RwInt32			SwitchDialog(bool bOpen);		///< DialogManager에서의 Open/Close

protected:
	CGiftShopGui() {}
	virtual VOID	HandleEvents( RWS::CMsg &msg );

	VOID			OpenShop();
	VOID			CloseShop();

	VOID			ClearShop();
	VOID			ClearPanels();

	VOID			UpdateTabContent(RwUInt8 byIndex);
	bool			SetPage(RwInt32 iPage);
	VOID			SetPanel(RwInt32 iPage);
	VOID			SetPageButton();

	VOID			SetZenny();							///< 자신의 소지 WP 정보를 업데이트 한다.

	RwUInt8			GetPageCount_of_CurTab();

	RwBool			IsFirstPage();
	RwBool			IsLastPage();

	RwInt32			PtinSlot(RwInt32 iX, RwInt32 iY);
	VOID			FocusEffect( RwBool bPush, RwInt32 iSlotIdx = -1);
	VOID			CheckInfoWindow();

	VOID			OnPaint();
	VOID			OnPostPaint();

	VOID			OnSelectChangeTabButton( INT nCurIndex, INT nPrevIndex );

	VOID			ClickedPrePageButton(gui::CComponent* pComponent);
	VOID			ClickedNextPageButton(gui::CComponent* pComponent);
	VOID			ClickedCloseButton(gui::CComponent* pComponent);
	VOID			ClickedHoipoiMixButton(gui::CComponent* pComponent);

	VOID			OnMouseDown(const CKey& key);
	VOID			OnMouseUp(const CKey& key);
	VOID			OnMove(RwInt32 iOldX, RwInt32 iOldY);
	VOID			OnMouseMove(RwInt32 nFlags, RwInt32 nX, RwInt32 nY);
	VOID			OnMouseLeave(gui::CComponent* pComponent);
	VOID			OnCaptureMouseDown(const CKey& key);

protected:
	gui::CSlot			m_slotMouseDown;
	gui::CSlot			m_slotMouseUp;
	gui::CSlot			m_slotMove;
	gui::CSlot			m_slotMouseMove;
	gui::CSlot			m_slotMouseLeave;
	gui::CSlot			m_slotTab;
	gui::CSlot			m_slotPrePageButton;
	gui::CSlot			m_slotNextPageButton;
	gui::CSlot			m_slotCloseButton;
	gui::CSlot			m_slotPaint;
	gui::CSlot			m_slotPostPaint;
	gui::CSlot			m_slotCaptureMouseDown;
	gui::CSlot			m_slotClickedHoipoiMix;

	RwInt32				m_iInfoWindowIndex;
	RwInt32				m_iMouseDownSlot;	///< 마우스로 눌린 슬롯의 인덱스
	RwInt32				m_iClickEffectedSlot;

	gui::CTabButton*	m_pTabButton;

	ItemPanel			m_ItemPanel[dMAX_ITEM_PANEL];

	CWindowby3			m_BackLineSurface;	///< 백라인

	CSurfaceGui			m_FocusEffect;
	CSurfaceGui			m_MoneyBackPanel;
	CSurfaceGui			m_PageBackPanel;

	gui::CButton*		m_pExitButton;
	gui::CButton*		m_pPrePageButton;
	gui::CButton*		m_pNextPageButton;
	gui::CButton*		m_pBtnHoipoiMixOpen;

	gui::CStaticBox*	m_pShopTitle;
	gui::CStaticBox*	m_pLargeBuyExplan;	///< 대량 구매 설명
	gui::CStaticBox*	m_pPocketMoneytitle;
	gui::CStaticBox*	m_pPocketMoney;
	gui::CStaticBox*	m_pCurrentPage;

	ShopItem			m_aShopItem[dGIFTSHOP_TAB_NUMS][NTL_MAX_MERCHANT_COUNT];

	RwInt32				m_iCurTab;
	RwInt32				m_iCurPage;

	RwBool				m_bFocus;

	unsigned int		m_adwGIFTSHOP_MERCHANT_TBLIDX[dGIFTSHOP_TAB_NUMS];
};

#endif
