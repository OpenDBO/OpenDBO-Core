#include "precomp_dboclient.h"
#include "HLShopGui.h"

// core
#include "NtlDebug.h"

// presentation
#include "NtlPLGuiManager.h"

//  simul
#include "NtlCameraController.h"
#include "NtlSLEventFunc.h"
#include "NtlSobManager.h"
#include "NtlSobItem.h"
#include "NtlSobItemAttr.h"
#include "NtlSLLogic.h"

// framework
#include "NtlApplication.h"


// dbo
#include "DisplayStringManager.h"
#include "DboEvent.h"
#include "IconMoveManager.h"
#include "DialogManager.h"
#include "DboLogic.h"
#include "InfoWndManager.h"
#include "AlarmManager.h"
#include "DboEventGenerator.h"
#include "DboGlobal.h"

// table
#include "HLSItemTable.h"
#include "SlotMachineTable.h"
#include "ItemTable.h"

// sound
#include "GUISoundDefine.h"


#define HLS_ITEMS_PER_PAGE 10


CHLShopGui::CHLShopGui(const RwChar * pName)
	:CNtlPLGui(pName),
	m_pStbTitle(NULL),
	m_pBtnClose(NULL),
	m_pipbHLSSearch(NULL),
	m_pbtnSearch(NULL),
	m_pbtnInitSearchList(NULL),
	m_pstbHaveCashTitle(NULL),
	m_pstbHaveCash(NULL),
	m_ppnlHaveCashMark(NULL),
	m_pstbHaveWaguCoinTitle(NULL),
	m_pstbHaveWaguCoin(NULL),
	m_ppnlHaveWaguCoinMark(NULL),
	m_pstbHaveEventCoinTitle(NULL),
	m_pstbHaveEventCoin(NULL),
	m_ppnlHaveEventCoinMark(NULL),
	m_pbtnYadrat(NULL),
	m_pbtnCashRecharge(NULL),
	m_pbtnProductFirstList(NULL),
	m_pbtnProductPrevList(NULL),
	m_pstbPage(NULL),
	m_pbtnProductNextList(NULL),
	m_pbtnProductLastList(NULL),
	m_pWaguFlash(NULL)
{
	m_nCurrentCategory = -1;
	m_nCurrentPage = 1;
	m_nMaxPage = 1;

	memset(&m_WaguInfo, 0, sizeof(m_WaguInfo));

	for (int i = 0; i < eHLS_CATEGORY_NUM; i++)
	{
		m_pBtnCategory[i] = NULL;
		m_vecProducts[i].clear();
	}

	m_vecProductsWagu[0].clear();
	m_vecProductsWagu[1].clear();
	m_vecVisibleProductsWagu.clear();

	m_bSearch = false;
	m_vecSearch.clear();
}

CHLShopGui::~CHLShopGui()
{
}

RwBool CHLShopGui::Create()
{
	NTL_FUNCTION("CHLShopGui::Create");

	if (!CNtlPLGui::Create("gui\\HLS.rsr", "gui\\HLS.srf", "gui\\HLS.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(GetNtlGuiManager()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");

	m_pStbTitle = (gui::CStaticBox*)GetComponent("stbHLSTitle");
	m_pStbTitle->SetText(GetDisplayStringManager()->GetString("DST_HLS_TITLE"));

	m_pipbHLSSearch = (gui::CInputBox*)GetComponent("ipbHLSSearch");
	m_pipbHLSSearch->SetMaxLength(NTL_MAX_LENGTH_ITEM_NAME_TEXT);
	m_pipbHLSSearch->SetText(L"");

	m_pbtnSearch = (gui::CButton*)GetComponent("btnSearch");
	m_pbtnSearch->SetText(GetDisplayStringManager()->GetString("DST_HLS_SEARCH"));
	m_slotClickedBtnSearch = m_pbtnSearch->SigClicked().Connect(this, &CHLShopGui::OnClickedBtnSearch);

	m_pbtnInitSearchList = (gui::CButton*)GetComponent("btnInitSearchList");
	m_pbtnInitSearchList->SetText(GetDisplayStringManager()->GetString("DST_HLS_INIT_SEARCH"));
	m_slotClickedBtnInitSearchList = m_pbtnInitSearchList->SigClicked().Connect(this, &CHLShopGui::OnClickedBtnInitSearchList);

	m_pBtnClose = (gui::CButton*)GetComponent("btnClose");
	m_slotClickedBtnClose = m_pBtnClose->SigClicked().Connect(this, &CHLShopGui::OnClickedBtnClose);

	m_pstbHaveCashTitle = (gui::CStaticBox*)GetComponent("stbHaveCashTitle");
	m_pstbHaveCashTitle->SetText(GetDisplayStringManager()->GetString("DST_HLS_HAVE_CASH"));

	m_pstbHaveCash = (gui::CStaticBox*)GetComponent("stbHaveCash");
	m_pstbHaveCash->SetText(0);

	m_ppnlHaveCashMark = (gui::CPanel*)GetComponent("pnlHaveCashMark");

	m_pstbHaveWaguCoinTitle = (gui::CStaticBox*)GetComponent("stbHaveWaguCoinTitle");
	m_pstbHaveWaguCoinTitle->SetText(GetDisplayStringManager()->GetString("DST_HLS_HAVE_COIN"));

	m_pstbHaveWaguCoin = (gui::CStaticBox*)GetComponent("stbHaveWaguCoin");
	m_pstbHaveWaguCoin->SetText(0);

	m_ppnlHaveWaguCoinMark = (gui::CPanel*)GetComponent("pnlHaveWaguCoinMark");

	m_pstbHaveEventCoinTitle = (gui::CStaticBox*)GetComponent("stbHaveEventCoinTitle");
	m_pstbHaveEventCoinTitle->SetText(GetDisplayStringManager()->GetString("DST_HLS_HAVE_EVENTCOIN"));
	m_pstbHaveEventCoinTitle->Show(false); // hide event coin by default

	m_pstbHaveEventCoin = (gui::CStaticBox*)GetComponent("stbHaveEventCoin");
	m_pstbHaveEventCoin->SetText(0);
	m_pstbHaveEventCoin->Show(false); // hide event coin by default

	m_ppnlHaveEventCoinMark = (gui::CPanel*)GetComponent("pnlHaveEventCoinMark");
	m_ppnlHaveEventCoinMark->Show(false); // hide event coin by default

	m_pbtnYadrat = (gui::CButton*)GetComponent("btnYadrat");
	m_pbtnYadrat->SetText(GetDisplayStringManager()->GetString("DST_HLS_REFRESH"));
	m_slotClickedBtnYadrat = m_pbtnYadrat->SigClicked().Connect(this, &CHLShopGui::OnClickedBtnYadrat);

	m_pbtnCashRecharge = (gui::CButton*)GetComponent("btnCashRecharge");
	m_pbtnCashRecharge->SetText(GetDisplayStringManager()->GetString("DST_HLS_CASHCHARGE"));
	m_slotClickedBtnCashRecharge = m_pbtnCashRecharge->SigClicked().Connect(this, &CHLShopGui::OnClickedBtnCashRecharge);

	m_pbtnProductFirstList = (gui::CButton*)GetComponent("btnProductFirstList");
	m_slotClickedBtnProductFirstList = m_pbtnProductFirstList->SigClicked().Connect(this, &CHLShopGui::OnClickedBtnProductFirstList);

	m_pbtnProductPrevList = (gui::CButton*)GetComponent("btnProductPrevList");
	m_slotClickedBtnProductPrevList = m_pbtnProductPrevList->SigClicked().Connect(this, &CHLShopGui::OnClickedBtnProductPrevList);

	m_pstbPage = (gui::CStaticBox*)GetComponent("stbPage");

	m_pbtnProductNextList = (gui::CButton*)GetComponent("btnProductNextList");
	m_slotClickedBtnProductNextList = m_pbtnProductNextList->SigClicked().Connect(this, &CHLShopGui::OnClickedBtnProductNextList);

	m_pbtnProductLastList = (gui::CButton*)GetComponent("btnProductLastList");
	m_slotClickedBtnProductLastList = m_pbtnProductLastList->SigClicked().Connect(this, &CHLShopGui::OnClickedBtnProductLastList);


	m_pWaguFlash = (gui::CFlash*)GetComponent("flsResult");
	if (m_pWaguFlash)
		m_slotWaguFlashEnd = m_pWaguFlash->SigMovieEnd().Connect(this, &CHLShopGui::OnWaguFlashEnd);

	//m_pdlgBanner = (gui::CDialog*)GetComponent("dlgBanner");
	m_slotMove = m_pThis->SigMove().Connect(this, &CHLShopGui::OnMove);
	m_slotPaint = m_pThis->SigPaint().Connect(this, &CHLShopGui::OnPaint);

	GetNtlGuiManager()->AddUpdateFunc(this);

	CreateCategoryButton();
	CreateItems();
	CreateWaguItem();
	SelectCategory(eHLS_CATEGORY_ALL);

	// Dialog Priority
	m_pThis->SetPriority(dDIALOGPRIORITY_HLSHOP);

	LinkMsg(g_EventHLShopEvent);
	LinkMsg(g_EventHLShopEventItemBuyRes);
	LinkMsg(g_EventHLShopEventItemGiftRes);
	LinkMsg(g_EventDialog);
	LinkMsg(g_EventHLShopEventSelect);
	LinkMsg(g_EventWaguMachineInfo);
	LinkMsg(g_EventHlsCoinUpdateInfo);
	LinkMsg(g_EventWaguExcuteRes);
	LinkMsg(g_EventMsgBoxResult);

	Show(false);

	NTL_RETURN(TRUE);
}

void CHLShopGui::Destroy()
{
	NTL_FUNCTION("CHLShopGui::Destroy");


	UnLinkMsg(g_EventHLShopEvent);
	UnLinkMsg(g_EventHLShopEventItemBuyRes);
	UnLinkMsg(g_EventHLShopEventItemGiftRes);
	UnLinkMsg(g_EventDialog);
	UnLinkMsg(g_EventHLShopEventSelect);
	UnLinkMsg(g_EventWaguMachineInfo);
	UnLinkMsg(g_EventHlsCoinUpdateInfo);
	UnLinkMsg(g_EventWaguExcuteRes);
	UnLinkMsg(g_EventMsgBoxResult);

	m_vecVisibleProducts.clear();
	m_vecVisibleProductsWagu.clear();

	m_bSearch = false;
	m_vecSearch.clear();

	for (int i = 0; i < eHLS_CATEGORY_NUM; i++)
	{
		NTL_DELETE(m_pBtnCategory[i]);

		for (std::vector<sHLS_PRODUCTS*>::iterator it = m_vecProducts[i].begin(); it != m_vecProducts[i].end(); )
		{
			sHLS_PRODUCTS* pProduct = *it;

			++it;

			pProduct->ItemSlot.Destroy();
			delete pProduct->ppnlItemSlot;
			delete pProduct->pstbItemName;
			delete pProduct->pstbPrice;
			delete pProduct->ppnlCoinMark;
			delete pProduct->pBtnBuy;
			delete pProduct->pBtnGift;
			delete pProduct->pDialog;
			delete pProduct;
		}

		m_vecProducts[i].clear();
	}

	for (int i = 0; i < 2; i++)
	{
		for (std::vector<sWAGU_PRODUCTS*>::iterator it = m_vecProductsWagu[i].begin(); it != m_vecProductsWagu[i].end(); )
		{
			sWAGU_PRODUCTS* pProduct = *it;

			++it;

			for (int j = 0; j < 10; j++)
				pProduct->ItemSlot[j].Destroy();

			delete pProduct->ppnlItemSlot;
			delete pProduct->btnPrev;
			delete pProduct->btnNext;
			delete pProduct->btnBunchInfo;
			delete pProduct->btnExcute;
			delete pProduct->btnWaguInfo;
			delete pProduct->stbLeftCapsule;
			delete pProduct->stbLeftCapsuleNum;
			delete pProduct->stbWaguTitle;
			delete pProduct->stbChampionItem;
			delete pProduct->stbChampionItemName;
			delete pProduct->stbNeedWaguCoin;
			delete pProduct->pProgressbar;
			delete pProduct->pnlEventCoinMarkSmall;
			delete pProduct->pnlEventMark;
			delete pProduct->pnlWaguMachine;
			delete pProduct->pDialog;
			delete pProduct;
		}

		m_vecProductsWagu[i].clear();
	}

	GetNtlGuiManager()->RemoveUpdateFunc(this);

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();

	NTL_RETURNVOID();
}

void CHLShopGui::Update(RwReal fElapsed)
{
	if (m_pWaguFlash && m_pWaguFlash->IsPlayMovie())
		m_pWaguFlash->Update(fElapsed);
}

void CHLShopGui::CreateCategoryButton()
{
	CTextTable* pMerchantTextTable = API_GetTableContainer()->GetTextAllTable()->GetMerchantTbl();

	CRectangle rect;
	RwInt32 iButtonY = 55;

	for (int i = 0; i < eHLS_CATEGORY_NUM; i++)
	{
		//check if category is active
		if (abIsCategoryActice[i] == false)
			continue;

		std::wstring wstrMerchantName = pMerchantTextTable->GetText(i + 1 + 100000); // category names start at ID 100001

		rect.SetRectWH(3, iButtonY, 97, 36);
		m_pBtnCategory[i] = NTL_NEW gui::CButton(rect, wstrMerchantName,
			GetNtlGuiManager()->GetSurfaceManager()->GetSurface("HLS.srf", "srfTabBtnUp"),
			GetNtlGuiManager()->GetSurfaceManager()->GetSurface("HLS.srf", "srfTabBtnDown"),
			GetNtlGuiManager()->GetSurfaceManager()->GetSurface("HLS.srf", "srfTabBtnDisable"),
			GetNtlGuiManager()->GetSurfaceManager()->GetSurface("HLS.srf", "srfTabBtnFocus"),
			NTL_BUTTON_UP_COLOR, NTL_BUTTON_UP_COLOR, NTL_BUTTON_FOCUS_COLOR, NTL_BUTTON_UP_COLOR,
			GUI_BUTTON_DOWN_COORD_X, GUI_BUTTON_DOWN_COORD_Y, m_pThis, GetNtlGuiManager()->GetSurfaceManager());

		m_slotClickedBtnCategory[i] = m_pBtnCategory[i]->SigClicked().Connect(this, &CHLShopGui::OnClickUpButtonCategory);

		iButtonY += 35;
	}
}

void CHLShopGui::CreateItems()
{
	CTextTable* pMerchantTextTable = API_GetTableContainer()->GetTextAllTable()->GetMerchantTbl();
	CTextTable* pItemTextTable = API_GetTableContainer()->GetTextAllTable()->GetItemTbl();

	CRectangle rect;

	int ai[eHLS_CATEGORY_NUM];
	int iDialogX[eHLS_CATEGORY_NUM]; //left/right
	int iDialogY[eHLS_CATEGORY_NUM]; //top/down

	for (int i = 0; i < eHLS_CATEGORY_NUM; i++)
	{
		ai[i] = 0;
		iDialogX[i] = 105;
		iDialogY[i] = 50;
	}

	CHLSItemTable* pHlsItemTable = API_GetTableContainer()->GetHLSItemTable();
	for (std::map<TBLIDX, sTBLDAT *>::iterator it = pHlsItemTable->Begin(); it != pHlsItemTable->End(); it++)
	{
		sHLS_ITEM_TBLDAT* pHlsItem = (sHLS_ITEM_TBLDAT*)it->second;

		// check if on sale
		if (pHlsItem->bOnSale == false)
			continue;

		// check if valid category
		if (pHlsItem->byCategory >= eHLS_CATEGORY_NUM)
			continue;

		//check if category is active
		if (abIsCategoryActice[pHlsItem->byCategory] == false)
			continue;

		// get item
		sITEM_TBLDAT* pItemTbldat = (sITEM_TBLDAT*)API_GetTableContainer()->GetItemTable()->FindData(pHlsItem->itemTblidx);

		// check if item exist
		if (pItemTbldat == NULL)
			continue;

		sHLS_PRODUCTS* pProduct = new sHLS_PRODUCTS;


		pProduct->hlsItemTblidx = pHlsItem->tblidx;

		if ((ai[pHlsItem->byCategory] % 2) == 0)
		{
			iDialogX[pHlsItem->byCategory] = 105;
		}
		else
		{
			iDialogX[pHlsItem->byCategory] = 105 + 233;
		}

		if (ai[pHlsItem->byCategory] == HLS_ITEMS_PER_PAGE)
		{
			ai[pHlsItem->byCategory] = 0;
			iDialogY[pHlsItem->byCategory] = 50;
		}

		rect.SetRectWH(iDialogX[pHlsItem->byCategory], iDialogY[pHlsItem->byCategory], 229, 86);
		pProduct->pDialog = NTL_NEW gui::CPanel(rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager());

		pProduct->nDialogX = iDialogX[pHlsItem->byCategory];
		pProduct->nDialogY = iDialogY[pHlsItem->byCategory];

		rect.SetRectWH(12, 18, NTL_ITEM_ICON_SIZE, NTL_ITEM_ICON_SIZE);
		pProduct->ppnlItemSlot = NTL_NEW gui::CPanel(rect, pProduct->pDialog, GetNtlGuiManager()->GetSurfaceManager());
		pProduct->slotMouseEnterItem = pProduct->ppnlItemSlot->SigMouseEnter().Connect(this, &CHLShopGui::OnMouseEnterItem);
		pProduct->slotMouseLeaveItem = pProduct->ppnlItemSlot->SigMouseLeave().Connect(this, &CHLShopGui::OnMouseLeaveItem);

		pProduct->ItemSlot.Create(pProduct->ppnlItemSlot, DIALOG_HLSHOP, REGULAR_SLOT_ITEM_TABLE, SDS_COUNT);
		pProduct->ItemSlot.SetSize(NTL_ITEM_ICON_SIZE);
		pProduct->ItemSlot.SetPosition_fromParent(0, 0);
		pProduct->ItemSlot.SetParentPosition(pProduct->ppnlItemSlot->GetScreenRect().left, pProduct->ppnlItemSlot->GetScreenRect().top);
		pProduct->ItemSlot.SetIcon(pItemTbldat->tblidx, pHlsItem->byStackCount);

		pProduct->mSurface.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("HLS.srf", "srfSlotBack"));

		rect.SetRectWH(50, 15, 165, 14);
		pProduct->pstbItemName = NTL_NEW gui::CStaticBox(rect, pProduct->pDialog, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_RIGHT);
		pProduct->pstbItemName->CreateFontStd(DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
		pProduct->pstbItemName->SetText(pItemTextTable->GetText(pItemTbldat->Name).c_str());
		pProduct->pstbItemName->Enable(false);
		pProduct->pstbItemName->SetColor(255, 218, 75); // INFOCOLOR_7

		rect.SetRectWH(168, 36, 30, 14);
		pProduct->pstbPrice = NTL_NEW gui::CStaticBox(rect, pProduct->pDialog, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_RIGHT);
		pProduct->pstbPrice->CreateFontStd(DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
		pProduct->pstbPrice->SetText(Logic_FormatZeni(pHlsItem->dwCash));
		pProduct->pstbPrice->Enable(false);
		pProduct->pstbPrice->SetColor(255, 168, 68); //INFOCOLOR_6

		rect.SetRectWH(203, 35, 16, 16);
		pProduct->ppnlCoinMark = NTL_NEW gui::CPanel(rect, pProduct->pDialog, GetNtlGuiManager()->GetSurfaceManager(), GetNtlGuiManager()->GetSurfaceManager()->GetSurface("HLS.srf", "srfCashMark"));
		pProduct->ppnlCoinMark->Enable(false);

		rect.SetRectWH(83, 58, 67, 22);
		pProduct->pBtnBuy = NTL_NEW gui::CButton(rect, GetDisplayStringManager()->GetString("DST_HLS_SLOT_BUY"),
			GetNtlGuiManager()->GetSurfaceManager()->GetSurface("HLS.srf", "srfHLSCommonBtnUp"),
			GetNtlGuiManager()->GetSurfaceManager()->GetSurface("HLS.srf", "srfHLSCommonBtnDown"),
			GetNtlGuiManager()->GetSurfaceManager()->GetSurface("HLS.srf", "srfHLSCommonBtnDown"),
			GetNtlGuiManager()->GetSurfaceManager()->GetSurface("HLS.srf", "srfHLSCommonBtnFocus"),
			NTL_BUTTON_UP_COLOR, NTL_BUTTON_UP_COLOR, NTL_BUTTON_FOCUS_COLOR, NTL_BUTTON_UP_COLOR,
			GUI_BUTTON_DOWN_COORD_X, GUI_BUTTON_DOWN_COORD_Y, pProduct->pDialog, GetNtlGuiManager()->GetSurfaceManager());
		pProduct->slotClickedBtnBuy = pProduct->pBtnBuy->SigClicked().Connect(this, &CHLShopGui::OnClickBuyButton);

		rect.SetRectWH(153, 58, 67, 22);
		pProduct->pBtnGift = NTL_NEW gui::CButton(rect, GetDisplayStringManager()->GetString("DST_HLS_SLOT_GIFT"),
			GetNtlGuiManager()->GetSurfaceManager()->GetSurface("HLS.srf", "srfHLSCommonBtnUp"),
			GetNtlGuiManager()->GetSurfaceManager()->GetSurface("HLS.srf", "srfHLSCommonBtnDown"),
			GetNtlGuiManager()->GetSurfaceManager()->GetSurface("HLS.srf", "srfHLSCommonBtnDown"),
			GetNtlGuiManager()->GetSurfaceManager()->GetSurface("HLS.srf", "srfHLSCommonBtnFocus"),
			NTL_BUTTON_UP_COLOR, NTL_BUTTON_UP_COLOR, NTL_BUTTON_FOCUS_COLOR, NTL_BUTTON_UP_COLOR,
			GUI_BUTTON_DOWN_COORD_X, GUI_BUTTON_DOWN_COORD_Y, pProduct->pDialog, GetNtlGuiManager()->GetSurfaceManager());
		pProduct->slotClickedBtnGift = pProduct->pBtnGift->SigClicked().Connect(this, &CHLShopGui::OnClickGiftButton);

		pProduct->pDialog->Show(false); // hide by default

		m_vecProducts[pHlsItem->byCategory].push_back(pProduct); // push into "category" list

		if ((ai[pHlsItem->byCategory] % 2) != 0)
		{
			iDialogY[pHlsItem->byCategory] += 90;
		}

		++ai[pHlsItem->byCategory];
	}
}

void CHLShopGui::CreateWaguItem()
{
	CTextTable* pItemTable = API_GetTableContainer()->GetTextAllTable()->GetItemTbl();
	CTextTable* pWaguTable = API_GetTableContainer()->GetTextAllTable()->GetETCTbl();

	CHLSItemTable* pHlsItemTable = API_GetTableContainer()->GetHLSItemTable();

	CRectangle rect;

	int ai[2];
	int iDialogX[2]; //left/right
	int iDialogY[2]; //top/down

	for (int i = 0; i < 2; i++)
	{
		ai[i] = 0;
		iDialogX[i] = 105;
		iDialogY[i] = 50;
	}

	CSlotMachineTable* pWaguMachineTable = API_GetTableContainer()->GetSlotMachineTable();
	if (!pWaguMachineTable)
		return;

	for (CTable::TABLEIT it = pWaguMachineTable->Begin(); it != pWaguMachineTable->End(); it++)
	{
		sHLS_SLOT_MACHINE_TBLDAT* pWaguItem = (sHLS_SLOT_MACHINE_TBLDAT*)it->second;

		if (!pWaguItem->bOnOff)
			continue;

		if (pWaguItem->byType >= 2)
			continue;

		sWAGU_PRODUCTS* pProduct = new sWAGU_PRODUCTS;

		for (int i = 0; i < 10; i++)
		{
			pProduct->ItemTblidx[i] = pWaguItem->aItemTblidx[i];
			pProduct->Stack[i] = pWaguItem->byStack[i];
		}

		if ((ai[pWaguItem->byType] % 2) == 0)
			iDialogX[pWaguItem->byType] = 105;
		else
			iDialogX[pWaguItem->byType] = 105 + 233;

		if (ai[pWaguItem->byType] == HLS_ITEMS_PER_PAGE)
		{
			ai[pWaguItem->byType] = 0;
			iDialogY[pWaguItem->byType] = 50;
		}

		pProduct->CurMachineIndex = pWaguItem->tblidx;
		pProduct->CurMachineType = pWaguItem->byType;
		pProduct->CurNeedCoin = pWaguItem->byCoin;
		pProduct->CurShowItem = 0;

		// item dialog
		rect.SetRectWH(iDialogX[pWaguItem->byType], iDialogY[pWaguItem->byType], 229, 224);
		pProduct->pDialog = NTL_NEW gui::CPanel(rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager());

		pProduct->nDialogX = iDialogX[pWaguItem->byType];
		pProduct->nDialogY = iDialogY[pWaguItem->byType];

		// item slot
		rect.SetRectWH(142, 75, NTL_ITEM_ICON_SIZE, NTL_ITEM_ICON_SIZE);
		pProduct->ppnlItemSlot = NTL_NEW gui::CPanel(rect, pProduct->pDialog, GetNtlGuiManager()->GetSurfaceManager());
		pProduct->slotMouseEnterItem = pProduct->ppnlItemSlot->SigMouseEnter().Connect(this, &CHLShopGui::OnMouseEnterWaguItem);
		pProduct->slotMouseLeaveItem = pProduct->ppnlItemSlot->SigMouseLeave().Connect(this, &CHLShopGui::OnMouseLeaveWaguItem);

		for (int i = 0; i < 10; i++)
		{
			sHLS_ITEM_TBLDAT* pHlsItem = (sHLS_ITEM_TBLDAT*)pHlsItemTable->FindData(pWaguItem->aItemTblidx[i]);

			// always create the slot so later unconditional loops (OnPaint/OnMove) over
			// all 10 slots never touch an un-initialized CRegularSlotGui
			pProduct->ItemSlot[i].Create(pProduct->ppnlItemSlot, DIALOG_HLSHOP, REGULAR_SLOT_ITEM_TABLE, SDS_COUNT);
			pProduct->ItemSlot[i].SetSize(NTL_ITEM_ICON_SIZE);
			pProduct->ItemSlot[i].SetPosition_fromParent(0, 0);
			pProduct->ItemSlot[i].SetParentPosition(pProduct->ppnlItemSlot->GetScreenRect().left, pProduct->ppnlItemSlot->GetScreenRect().top);

			if (pHlsItem)
			{
				pProduct->ItemSlot[i].SetIcon(pHlsItem->itemTblidx, 0);
				pProduct->hlsItemCount[i] = pHlsItem->byStackCount;
			}
			else
			{
				DBO_WARNING_MESSAGE("Wagu Item does not exist " << pWaguItem->aItemTblidx[i]);
				pProduct->hlsItemCount[i] = 0;
			}
		}

		// background
		pProduct->mSurface.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("HLS.srf", "srfWaguBack"));

		// prev button
		rect.SetRectWH(99, 80, 33, 21);
		pProduct->btnPrev = NTL_NEW gui::CButton(rect, "",
			GetNtlGuiManager()->GetSurfaceManager()->GetSurface("HLS.srf", "srfLeftBtnUp"),
			GetNtlGuiManager()->GetSurfaceManager()->GetSurface("HLS.srf", "srfLeftBtnDown"),
			GetNtlGuiManager()->GetSurfaceManager()->GetSurface("HLS.srf", "srfLeftBtnDown"),
			GetNtlGuiManager()->GetSurfaceManager()->GetSurface("HLS.srf", "srfLeftBtnFocus"),
			NTL_BUTTON_UP_COLOR, NTL_BUTTON_UP_COLOR, NTL_BUTTON_FOCUS_COLOR, NTL_BUTTON_UP_COLOR,
			GUI_BUTTON_DOWN_COORD_X, GUI_BUTTON_DOWN_COORD_Y, pProduct->pDialog, GetNtlGuiManager()->GetSurfaceManager());

		pProduct->slotClickPrev = pProduct->btnPrev->SigClicked().Connect(this, &CHLShopGui::OnClickedWaguPrev);

		// next button
		rect.SetRectWH(185, 80, 33, 21);
		pProduct->btnNext = NTL_NEW gui::CButton(rect, "",
			GetNtlGuiManager()->GetSurfaceManager()->GetSurface("HLS.srf", "srfRightBtnUp"),
			GetNtlGuiManager()->GetSurfaceManager()->GetSurface("HLS.srf", "srfRightBtnDown"),
			GetNtlGuiManager()->GetSurfaceManager()->GetSurface("HLS.srf", "srfRightBtnDown"),
			GetNtlGuiManager()->GetSurfaceManager()->GetSurface("HLS.srf", "srfRightBtnFocus"),
			NTL_BUTTON_UP_COLOR, NTL_BUTTON_UP_COLOR, NTL_BUTTON_FOCUS_COLOR, NTL_BUTTON_UP_COLOR,
			GUI_BUTTON_DOWN_COORD_X, GUI_BUTTON_DOWN_COORD_Y, pProduct->pDialog, GetNtlGuiManager()->GetSurfaceManager());

		pProduct->slotClickNext = pProduct->btnNext->SigClicked().Connect(this, &CHLShopGui::OnClickedWaguNext);

		// bunch info button
		rect.SetRectWH(193, 168, 29, 21);
		pProduct->btnBunchInfo = NTL_NEW gui::CButton(rect, "",
			GetNtlGuiManager()->GetSurfaceManager()->GetSurface("HLS.srf", "srfBunchBtnUp"),
			GetNtlGuiManager()->GetSurfaceManager()->GetSurface("HLS.srf", "srfBunchBtnDown"),
			GetNtlGuiManager()->GetSurfaceManager()->GetSurface("HLS.srf", "srfBunchBtnDown"),
			GetNtlGuiManager()->GetSurfaceManager()->GetSurface("HLS.srf", "srfBunchBtnFocus"),
			NTL_BUTTON_UP_COLOR, NTL_BUTTON_UP_COLOR, NTL_BUTTON_FOCUS_COLOR, NTL_BUTTON_UP_COLOR,
			GUI_BUTTON_DOWN_COORD_X, GUI_BUTTON_DOWN_COORD_Y, pProduct->pDialog, GetNtlGuiManager()->GetSurfaceManager());

		pProduct->slotBunchInfo = pProduct->btnBunchInfo->SigClicked().Connect(this, &CHLShopGui::OnClickedBtnBunchInfo);

		// left capsule text
		rect.SetRectWH(10, 174, 74, 18);
		pProduct->stbLeftCapsule = NTL_NEW gui::CStaticBox(rect, pProduct->pDialog, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_CENTER);
		pProduct->stbLeftCapsule->CreateFontStd(DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
		pProduct->stbLeftCapsule->SetText(GetDisplayStringManager()->GetString("DST_WAGU_ITEM_LEFT_CAPSULE"));
		pProduct->stbLeftCapsule->SetTextColor(RGB(153, 187, 238), true);
		pProduct->stbLeftCapsule->Enable(false);

		// left capsule count
		rect.SetRectWH(39, 197, 50, 20);
		pProduct->stbLeftCapsuleNum = NTL_NEW gui::CStaticBox(rect, pProduct->pDialog, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_LEFT);
		pProduct->stbLeftCapsuleNum->CreateFontStd(DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
		pProduct->stbLeftCapsuleNum->SetTextColor(RGB(153, 187, 238), true);
		pProduct->stbLeftCapsuleNum->Enable(false);

		// machine title
		rect.SetRectWH(96, 15, 125, 10);
		pProduct->stbWaguTitle = NTL_NEW gui::CStaticBox(rect, pProduct->pDialog, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_CENTER);
		pProduct->stbWaguTitle->CreateFontStd(DEFAULT_FONT, 100, DEFAULT_FONT_ATTR);
		pProduct->stbWaguTitle->SetText(pWaguTable->GetText(pWaguItem->dwName).c_str());
		pProduct->stbWaguTitle->Enable(false);

		// current champion item index
		rect.SetRectWH(123, 47, 76, 21);
		pProduct->stbChampionItem = NTL_NEW gui::CStaticBox(rect, pProduct->pDialog, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_CENTER);
		pProduct->stbChampionItem->CreateFontStd(DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
		WCHAR Buff[256];
		swprintf_s(Buff, 256, GetDisplayStringManager()->GetString("DST_WAGU_ITEM_CHAMPION_ITEM"), 1);
		pProduct->stbChampionItem->SetText(Buff);
		pProduct->stbChampionItem->SetTextColor(RGB(153, 187, 238), true);
		pProduct->stbChampionItem->Enable(false);

		// current champion item name
		rect.SetRectWH(98, 117, 119, 20);
		pProduct->stbChampionItemName = NTL_NEW gui::CStaticBox(rect, pProduct->pDialog, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_CENTER);
		pProduct->stbChampionItemName->CreateFontStd(DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
		if (pProduct->ItemSlot[0].GetItemTable())
		{
			std::wstring text = pItemTable->GetText(pProduct->ItemSlot[0].GetItemTable()->Name);
			pProduct->stbChampionItemName->SetText(text.c_str());
		}
		pProduct->stbChampionItemName->SetTextColor(RGB(255, 255, 255), true);
		pProduct->stbChampionItemName->Enable(false);

		// coin cost
		rect.SetRectWH(110, 143, 100, 20);
		pProduct->stbNeedWaguCoin = NTL_NEW gui::CStaticBox(rect, pProduct->pDialog, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_LEFT);
		pProduct->stbNeedWaguCoin->CreateFontStd(DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
		WCHAR Buff1[256];
		swprintf_s(Buff1, 256, GetDisplayStringManager()->GetString("DST_WAGU_ITEM_NEED_WAGU_COIN"), pWaguItem->byCoin);
		pProduct->stbNeedWaguCoin->SetText(Buff1);
		pProduct->stbNeedWaguCoin->SetTextColor(RGB(255, 221, 102), true);
		pProduct->stbNeedWaguCoin->Enable(false);

		// spin button
		rect.SetRectWH(125, 168, 67, 22);
		pProduct->btnExcute = NTL_NEW gui::CButton(rect, GetDisplayStringManager()->GetString("DST_WAGU_ITEM_EXCUTE_BUTTON"),
			GetNtlGuiManager()->GetSurfaceManager()->GetSurface("HLS.srf", "srfWaguMachineBtnUp"),
			GetNtlGuiManager()->GetSurfaceManager()->GetSurface("HLS.srf", "srfWaguMachineBtnDown"),
			GetNtlGuiManager()->GetSurfaceManager()->GetSurface("HLS.srf", "srfWaguMachineBtnDown"),
			GetNtlGuiManager()->GetSurfaceManager()->GetSurface("HLS.srf", "srfWaguMachineBtnFoc"),
			NTL_BUTTON_UP_COLOR, NTL_BUTTON_UP_COLOR, NTL_BUTTON_FOCUS_COLOR, NTL_BUTTON_UP_COLOR,
			GUI_BUTTON_DOWN_COORD_X, GUI_BUTTON_DOWN_COORD_Y, pProduct->pDialog, GetNtlGuiManager()->GetSurfaceManager());

		pProduct->slotClickExcute = pProduct->btnExcute->SigClicked().Connect(this, &CHLShopGui::OnClickedExcute);

		// winner list button
		rect.SetRectWH(125, 191, 67, 22);
		pProduct->btnWaguInfo = NTL_NEW gui::CButton(rect, GetDisplayStringManager()->GetString("DST_WAGU_ITEM_INFO_BUTTON"),
			GetNtlGuiManager()->GetSurfaceManager()->GetSurface("HLS.srf", "srfWaguMachineBtnUp"),
			GetNtlGuiManager()->GetSurfaceManager()->GetSurface("HLS.srf", "srfWaguMachineBtnDown"),
			GetNtlGuiManager()->GetSurfaceManager()->GetSurface("HLS.srf", "srfWaguMachineBtnDown"),
			GetNtlGuiManager()->GetSurfaceManager()->GetSurface("HLS.srf", "srfWaguMachineBtnFoc"),
			NTL_BUTTON_UP_COLOR, NTL_BUTTON_UP_COLOR, NTL_BUTTON_FOCUS_COLOR, NTL_BUTTON_UP_COLOR,
			GUI_BUTTON_DOWN_COORD_X, GUI_BUTTON_DOWN_COORD_Y, pProduct->pDialog, GetNtlGuiManager()->GetSurfaceManager());

		pProduct->slotClickWaguInfo = pProduct->btnWaguInfo->SigClicked().Connect(this, &CHLShopGui::OnclickedBtnWaguInfo);

		// event coin mark (small)
		rect.SetRectWH(199, 144, 17, 17);
		pProduct->pnlEventCoinMarkSmall = NTL_NEW gui::CPanel(rect, pProduct->pDialog, GetNtlGuiManager()->GetSurfaceManager(), GetNtlGuiManager()->GetSurfaceManager()->GetSurface("HLS.srf", "srfEventCoinMark"));

		// machine skin, keyed by tblidx % 100 (1-4)
		switch (pWaguItem->tblidx % 100)
		{
			case 1:
			{
				rect.SetRectWH(3, 1, 90, 150);
				pProduct->pnlWaguMachine = NTL_NEW gui::CPanel(rect, pProduct->pDialog, GetNtlGuiManager()->GetSurfaceManager(), GetNtlGuiManager()->GetSurfaceManager()->GetSurface("HLS.srf", "srfBuleMachine_4"));

				rect.SetRectWH(12, 164, 69, 5);
				pProduct->pProgressbar = NTL_NEW gui::CProgressBar(rect, pProduct->pDialog, GetNtlGuiManager()->GetSurfaceManager(), GetNtlGuiManager()->GetSurfaceManager()->GetSurface("HLS.srf", "srfBlueMachineProgress"));
				pProduct->pProgressbar->SetRange(0, 100);
				pProduct->pProgressbar->SetPos(100);
			}
			break;

			case 2:
			{
				rect.SetRectWH(3, 1, 90, 150);
				pProduct->pnlWaguMachine = NTL_NEW gui::CPanel(rect, pProduct->pDialog, GetNtlGuiManager()->GetSurfaceManager(), GetNtlGuiManager()->GetSurfaceManager()->GetSurface("HLS.srf", "srfYellowMachine_4"));

				rect.SetRectWH(12, 164, 69, 5);
				pProduct->pProgressbar = NTL_NEW gui::CProgressBar(rect, pProduct->pDialog, GetNtlGuiManager()->GetSurfaceManager(), GetNtlGuiManager()->GetSurfaceManager()->GetSurface("HLS.srf", "srfYellowMachineProgress"));
				pProduct->pProgressbar->SetRange(0, 100);
				pProduct->pProgressbar->SetPos(100);
			}
			break;

			case 3:
			{
				rect.SetRectWH(3, 1, 90, 150);
				pProduct->pnlWaguMachine = NTL_NEW gui::CPanel(rect, pProduct->pDialog, GetNtlGuiManager()->GetSurfaceManager(), GetNtlGuiManager()->GetSurfaceManager()->GetSurface("HLS.srf", "srfRedMachine_4"));

				rect.SetRectWH(12, 164, 69, 5);
				pProduct->pProgressbar = NTL_NEW gui::CProgressBar(rect, pProduct->pDialog, GetNtlGuiManager()->GetSurfaceManager(), GetNtlGuiManager()->GetSurfaceManager()->GetSurface("HLS.srf", "srfRedMachineProgress"));
				pProduct->pProgressbar->SetRange(0, 100);
				pProduct->pProgressbar->SetPos(100);
			}
			break;

			case 4:
			default:
			{
				rect.SetRectWH(3, 1, 90, 150);
				pProduct->pnlWaguMachine = NTL_NEW gui::CPanel(rect, pProduct->pDialog, GetNtlGuiManager()->GetSurfaceManager(), GetNtlGuiManager()->GetSurfaceManager()->GetSurface("HLS.srf", "srfGreenMachine_4"));

				rect.SetRectWH(12, 164, 69, 5);
				pProduct->pProgressbar = NTL_NEW gui::CProgressBar(rect, pProduct->pDialog, GetNtlGuiManager()->GetSurfaceManager(), GetNtlGuiManager()->GetSurfaceManager()->GetSurface("HLS.srf", "srfGreenMachineProgress"));
				pProduct->pProgressbar->SetRange(0, 100);
				pProduct->pProgressbar->SetPos(100);
			}
			break;
		}

		// EVENT badge overlay
		rect.SetRectWH(2, 91, 88, 59);
		pProduct->pnlEventMark = NTL_NEW gui::CPanel(rect, pProduct->pnlWaguMachine, GetNtlGuiManager()->GetSurfaceManager(), GetNtlGuiManager()->GetSurfaceManager()->GetSurface("HLS.srf", "srfEventMark"));

		if (pWaguItem->byType == HLS_MACHINE_TYPE_EVENT)
		{
			pProduct->pnlEventCoinMarkSmall->Show(true);
			pProduct->pnlEventMark->Show(true);
		}
		else
		{
			pProduct->pnlEventCoinMarkSmall->Show(false);
			pProduct->pnlEventMark->Show(false);
		}

		pProduct->pDialog->Show(false);

		m_vecProductsWagu[pWaguItem->byType].push_back(pProduct);

		if ((ai[pWaguItem->byType] % 2) != 0)
		{
			iDialogY[pWaguItem->byType] += 230;
		}

		++ai[pWaguItem->byType];
	}
}

void CHLShopGui::SelectCategory(int iCategory)
{
	if (m_nCurrentCategory == iCategory)
		return;

	// hide current active button
	if (m_nCurrentCategory != -1) // when we init the variable, its -1
	{
		m_pBtnCategory[m_nCurrentCategory]->SetDown(false);
	}

	// hide current shown items
	for (std::vector<sHLS_PRODUCTS*>::iterator it = m_vecVisibleProducts.begin(); it != m_vecVisibleProducts.end(); it++)
	{
		sHLS_PRODUCTS* pProduct = *it;

		pProduct->pDialog->Show(false);
	}
	m_vecVisibleProducts.clear();

	// hide current shown wagu machines
	for (std::vector<sWAGU_PRODUCTS*>::iterator it = m_vecVisibleProductsWagu.begin(); it != m_vecVisibleProductsWagu.end(); it++)
	{
		sWAGU_PRODUCTS* pProduct = *it;

		pProduct->pDialog->Show(false);
	}
	m_vecVisibleProductsWagu.clear();

	if (iCategory == eHLS_CATEGORY_WAGU_MACHINE)
	{
		// machine widgets get shown once RefreshWaguInfo receives the machine list from the server
		GetDboGlobal()->GetChatPacketGenerator()->SendLoadWaguMachineInfoReq(HLS_MACHINE_TYPE_WAGUWAGU);
	}
	else if (iCategory == eHLS_CATEGORY_EVENT_MACHINE)
	{
		GetDboGlobal()->GetChatPacketGenerator()->SendLoadWaguMachineInfoReq(HLS_MACHINE_TYPE_EVENT);
	}
	else
	{
		// show new items
		int i = 0;
		for (std::vector<sHLS_PRODUCTS*>::iterator it = m_vecProducts[iCategory].begin(); it != m_vecProducts[iCategory].end(); it++)
		{
			sHLS_PRODUCTS* pProduct = *it;

			pProduct->pDialog->Show(true);

			m_vecVisibleProducts.push_back(pProduct);

			if (++i >= HLS_ITEMS_PER_PAGE) // only show first 10 items
				break;
		}
	}

	m_nCurrentCategory = iCategory;
	m_pBtnCategory[m_nCurrentCategory]->SetDown(true);
	ShowCoin();

	if (iCategory == eHLS_CATEGORY_WAGU_MACHINE || iCategory == eHLS_CATEGORY_EVENT_MACHINE)
	{
		SetPage(1, 1);
	}
	else
	{
		// get max pages
		float fPages = ((float)m_vecProducts[m_nCurrentCategory].size() / (float)HLS_ITEMS_PER_PAGE) + 0.9;
		if (fPages < 1)
			fPages = 1;

		SetPage(1, (int)fPages);
	}
}

void CHLShopGui::ShowCoin()
{
	if (m_nCurrentCategory == eHLS_CATEGORY_EVENT_MACHINE)
	{
		m_pstbHaveEventCoinTitle->Show(true);
		m_pstbHaveEventCoin->Show(true);
		m_ppnlHaveEventCoinMark->Show(true);

		m_pstbHaveWaguCoinTitle->Show(false);
		m_pstbHaveWaguCoin->Show(false);
		m_ppnlHaveWaguCoinMark->Show(false);
	}
	else
	{
		m_pstbHaveEventCoinTitle->Show(false);
		m_pstbHaveEventCoin->Show(false);
		m_ppnlHaveEventCoinMark->Show(false);

		m_pstbHaveWaguCoinTitle->Show(true);
		m_pstbHaveWaguCoin->Show(true);
		m_ppnlHaveWaguCoinMark->Show(true);
	}
}

void CHLShopGui::SetPage(int nCurPage, int nMaxPage)
{
	m_nCurrentPage = nCurPage;
	m_nMaxPage = nMaxPage;

	m_pstbPage->Format(L"%d / %d", nCurPage, nMaxPage);

	//do onmove to refresh image position
	OnMove(0,0);
}

void CHLShopGui::ClearVisibleItems()
{
	// hide current shown items
	for (std::vector<sHLS_PRODUCTS*>::iterator it = m_vecVisibleProducts.begin(); it != m_vecVisibleProducts.end(); it++)
	{
		sHLS_PRODUCTS* pProduct = *it;

		pProduct->pDialog->Show(false);
	}
	m_vecVisibleProducts.clear();
}

void CHLShopGui::HandleEvents(RWS::CMsg & msg)
{
	NTL_FUNCTION("CHLShopGui::HandleEvents");

	if (msg.Id == g_EventHLShopEvent)
	{
		SDboEventHLShopEvent* pData = reinterpret_cast<SDboEventHLShopEvent*>(msg.pData);

		switch (pData->byEventType)
		{
			case eHLSHOP_EVENT_START:
			{
				Logic_SetHlsCash(pData->dwCash);
				m_pstbHaveCash->SetText(Logic_FormatZeni(pData->dwCash));

				m_pstbHaveWaguCoin->SetText(Logic_FormatZeni(Logic_GetWaguCoin()));
				m_pstbHaveEventCoin->SetText(Logic_FormatZeni(Logic_GetEventCoin()));

				// set storage position
				CRectangle rect = GetPosition();
				CNtlPLGui* pPLGui = GetDialogManager()->GetDialog(DIALOG_HLSHOP_WAREHOUSE);
				pPLGui->SetPosition(rect.left - pPLGui->GetWidth() - NTL_LINKED_DIALOG_GAP, rect.top);

				GetDialogManager()->OpenDialog(DIALOG_HLSHOP);

				// If a wagu/event category was selected via the side icon before this
				// response arrived, SelectCategory() already ran while the dialog was
				// still hidden and the widgets never rendered. Re-apply it now that the
				// dialog is actually visible.
				if (m_nCurrentCategory == eHLS_CATEGORY_WAGU_MACHINE || m_nCurrentCategory == eHLS_CATEGORY_EVENT_MACHINE)
				{
					int iPendingCategory = m_nCurrentCategory;
					m_nCurrentCategory = -1;
					SelectCategory(iPendingCategory);
				}
			}
			break;
			case eHLSHOP_EVENT_REFRESH:
			{
				Logic_SetHlsCash(pData->dwCash);
				m_pstbHaveCash->SetText(Logic_FormatZeni(pData->dwCash));
			}
			break;
			case eHLSHOP_EVENT_END:
			{
				GetDialogManager()->CloseDialog(DIALOG_HLSHOP_BUY);
				GetDialogManager()->CloseDialog(DIALOG_HLSHOP_BUY_CONFIRM);
				GetDialogManager()->CloseDialog(DIALOG_HLSHOP_GIFT);
				GetDialogManager()->CloseDialog(DIALOG_HLSHOP_GIFT_CONFIRM);
				GetDialogManager()->CloseDialog(DIALOG_HLSHOP_WAGU_INFO);
				GetDialogManager()->CloseDialog(DIALOG_HLSHOP);
			}
			break;
		}
	}
	else if (msg.Id == g_EventHLShopEventItemBuyRes)
	{
		SDboEventHLShopEventItemBuyRes* pData = reinterpret_cast<SDboEventHLShopEventItemBuyRes*>(msg.pData);

		Logic_SetHlsCash(pData->dwCash);
		m_pstbHaveCash->SetText(Logic_FormatZeni(pData->dwCash));
	}
	else if (msg.Id == g_EventHLShopEventItemGiftRes)
	{
		SDboEventHLShopEventItemGiftRes* pData = reinterpret_cast<SDboEventHLShopEventItemGiftRes*>(msg.pData);

		Logic_SetHlsCash(pData->dwCash);
		m_pstbHaveCash->SetText(Logic_FormatZeni(pData->dwCash));
	}
	else if (msg.Id == g_EventDialog)
	{
		SDboEventDialog* pData = reinterpret_cast<SDboEventDialog*>(msg.pData);
		if (pData->iType == DIALOGEVENT_NPC_BYEBYE && pData->iDestDialog == DIALOG_HLSHOP)
		{
			OnClickedBtnClose(NULL);
		}
	}
	else if (msg.Id == g_EventHLShopEventSelect)
	{
		SDboEventHLShopSelectCategory* pData = reinterpret_cast<SDboEventHLShopSelectCategory*>(msg.pData);
		SelectCategory(pData->Category);
	}
	else if (msg.Id == g_EventWaguMachineInfo)
	{
		SDboEventWaguMachineInfo* pEvent = (SDboEventWaguMachineInfo*)msg.pData;

		RefreshWaguInfo(pEvent->byType, pEvent->wCurrentCapsule, pEvent->wMaxCapsule, pEvent->wMachineIndex);
	}
	else if (msg.Id == g_EventHlsCoinUpdateInfo)
	{
		SDboEventHlsCoinUpdate* pData = reinterpret_cast<SDboEventHlsCoinUpdate*>(msg.pData);

		if (pData->Type == 0)
			m_pstbHaveWaguCoin->SetText(Logic_FormatZeni(pData->Coin));
		else
			m_pstbHaveEventCoin->SetText(Logic_FormatZeni(pData->Coin));
	}
	else if (msg.Id == g_EventMsgBoxResult)
	{
		SDboEventMsgBoxResult* pEvent = reinterpret_cast<SDboEventMsgBoxResult*>(msg.pData);

		if (pEvent->strID == "DST_WAGU_EXCUTE_RESULT" || pEvent->strID == "DST_WAGU_EXCUTE_RESULT_MORE_SET")
		{
			if (pEvent->eResult == MBR_OK)
			{
				for (int i = 0; i < 2; i++)
				{
					for (std::vector<sWAGU_PRODUCTS*>::iterator it = m_vecProductsWagu[i].begin(); it != m_vecProductsWagu[i].end(); it++)
					{
						sWAGU_PRODUCTS* pProduct = *it;

						pProduct->btnBunchInfo->ClickEnable(TRUE);
						pProduct->btnExcute->ClickEnable(TRUE);
						pProduct->btnNext->ClickEnable(TRUE);
						pProduct->btnPrev->ClickEnable(TRUE);
						pProduct->btnWaguInfo->ClickEnable(TRUE);
					}
				}
			}
		}
	}
	else if (msg.Id == g_EventWaguExcuteRes)
	{
		SDboEventWaguExcuteRes* pData = reinterpret_cast<SDboEventWaguExcuteRes*>(msg.pData);

		CHLSItemTable* pHlsItemTable = API_GetTableContainer()->GetHLSItemTable();
		for (int i = 0; i < pData->byReallyExtractCount; i++)
		{
			sHLS_ITEM_TBLDAT* pHlsItem = (sHLS_ITEM_TBLDAT*)pHlsItemTable->FindData(pData->ItemTblidx[i]);
			if (pHlsItem)
			{
				m_WaguInfo.byRanking[i] = pData->byRanking[i];
				m_WaguInfo.ItemTblidx[i] = pHlsItem->itemTblidx;
				m_WaguInfo.bySetCount[i] = pData->bySetCount[i];
				m_WaguInfo.byStackCount[i] = pData->byStackCount[i];
			}
		}
		m_WaguInfo.byReallyExtractCount = pData->byReallyExtractCount;
		m_WaguInfo.wMachineIndex = pData->wMachineIndex;
		m_WaguInfo.wNewWaguWaguPoints = pData->wNewWaguWaguPoints;

		if ((m_WaguInfo.wMachineIndex - 100) < 100)
		{
			GetDboGlobal()->GetChatPacketGenerator()->SendLoadWaguMachineInfoReq(HLS_MACHINE_TYPE_WAGUWAGU);
			m_WaguInfo.isEventType = false;
		}
		else
		{
			GetDboGlobal()->GetChatPacketGenerator()->SendLoadWaguMachineInfoReq(HLS_MACHINE_TYPE_EVENT);
			m_WaguInfo.isEventType = true;
		}

		if (m_pWaguFlash)
		{
			m_pWaguFlash->Raise();
			switch (pData->wMachineIndex % 100)
			{
				case 1: m_pWaguFlash->Load("Hls_SlotMachine1.swf"); m_pWaguFlash->PlayMovie(TRUE); break;
				case 2: m_pWaguFlash->Load("Hls_SlotMachine2.swf"); m_pWaguFlash->PlayMovie(TRUE); break;
				case 3: m_pWaguFlash->Load("Hls_SlotMachine3.swf"); m_pWaguFlash->PlayMovie(TRUE); break;
				case 4: m_pWaguFlash->Load("Hls_SlotMachine4.swf"); m_pWaguFlash->PlayMovie(TRUE); break;
			}

			sNtlSoundPlayParameta tSoundParam;
			tSoundParam.iChannelGroup = CHANNEL_GROUP_UI_SOUND;
			tSoundParam.pcFileName = GSD_WAGU_DRAW;
			GetSoundManager()->Play(&tSoundParam);

			// disable all machine buttons until the flash finishes
			for (int i = 0; i < 2; i++)
			{
				for (std::vector<sWAGU_PRODUCTS*>::iterator it = m_vecProductsWagu[i].begin(); it != m_vecProductsWagu[i].end(); it++)
				{
					sWAGU_PRODUCTS* pProduct = *it;

					pProduct->btnBunchInfo->ClickEnable(FALSE);
					pProduct->btnExcute->ClickEnable(FALSE);
					pProduct->btnNext->ClickEnable(FALSE);
					pProduct->btnPrev->ClickEnable(FALSE);
					pProduct->btnWaguInfo->ClickEnable(FALSE);
				}
			}
		}
		else
		{
			// no flash component available - skip straight to showing the result
			OnWaguFlashEnd(NULL);
		}
	}
}

RwInt32 CHLShopGui::SwitchDialog(bool bOpen)
{
	Show(bOpen);

	if (!bOpen)
	{
		if (GetInfoWndManager()->GetRequestGui() == DIALOG_HLSHOP)
			GetInfoWndManager()->ShowInfoWindow(FALSE);
	}
	else
	{
		
	}

	NTL_RETURN(TRUE);
}

void CHLShopGui::OnPaint()
{
	for (std::vector<sHLS_PRODUCTS*>::iterator it = m_vecVisibleProducts.begin(); it != m_vecVisibleProducts.end(); it++)
	{
		sHLS_PRODUCTS* pProduct = *it;

		pProduct->mSurface.Render();
		pProduct->ItemSlot.Paint();
	}

	for (std::vector<sWAGU_PRODUCTS*>::iterator it = m_vecVisibleProductsWagu.begin(); it != m_vecVisibleProductsWagu.end(); it++)
	{
		sWAGU_PRODUCTS* pProduct = *it;

		pProduct->mSurface.Render();
		pProduct->ItemSlot[pProduct->CurShowItem].Paint();
	}
}

void CHLShopGui::OnMove(RwInt32 iOldX, RwInt32 iOldY)
{
	for (std::vector<sHLS_PRODUCTS*>::iterator it = m_vecVisibleProducts.begin(); it != m_vecVisibleProducts.end(); it++)
	{
		sHLS_PRODUCTS* pProduct = *it;

		CRectangle rect = pProduct->ppnlItemSlot->GetScreenRect();

		pProduct->ItemSlot.SetParentPosition(rect.left, rect.top);

		CRectangle drect = pProduct->pDialog->GetScreenRect();

		pProduct->mSurface.SetPositionbyParent(drect.left, drect.top);
	}

	for (std::vector<sWAGU_PRODUCTS*>::iterator it = m_vecVisibleProductsWagu.begin(); it != m_vecVisibleProductsWagu.end(); it++)
	{
		sWAGU_PRODUCTS* pProduct = *it;

		CRectangle rect = pProduct->ppnlItemSlot->GetScreenRect();

		for (int i = 0; i < 10; i++)
			pProduct->ItemSlot[i].SetParentPosition(rect.left, rect.top);

		CRectangle drect = pProduct->pDialog->GetScreenRect();

		pProduct->mSurface.SetPositionbyParent(drect.left, drect.top);
	}

	CRectangle rtScreen = m_pThis->GetScreenRect();

	if(iOldX !=  0 && iOldY != 0)
		MoveLinkedPLGui(rtScreen.left - iOldX, rtScreen.top - iOldY);
}

void CHLShopGui::OnClickedBtnClose(gui::CComponent * pComponent)
{
	GetDboGlobal()->GetGamePacketGenerator()->SendCashItemHLShopEndReq();
}

void CHLShopGui::OnClickedBtnSearch(gui::CComponent * pComponent)
{
	const WCHAR* searchText = m_pipbHLSSearch->GetText();
	if (wcslen(searchText) == 0)
	{
		GetAlarmManager()->AlarmMessage("DST_HLS_SEARCH_GUIDE");
		return;
	}

	//init
	InitSearch();

	m_bSearch = true;

	CTextTable* pItemTextTable = API_GetTableContainer()->GetTextAllTable()->GetItemTbl();

	// hide current active button
	if (m_nCurrentCategory != -1) // when we init the variable, its -1
	{
		m_pBtnCategory[m_nCurrentCategory]->SetDown(false);
	}

	// hide current shown items
	for (std::vector<sHLS_PRODUCTS*>::iterator it = m_vecVisibleProducts.begin(); it != m_vecVisibleProducts.end(); it++)
	{
		sHLS_PRODUCTS* pProduct = *it;

		pProduct->pDialog->Show(false);
	}
	m_vecVisibleProducts.clear();


	// show new items
	int iDialogX = 105;
	int iDialogY = 50;

	int showCount = 0, totalCount = 0;

	for (int i = 0; i < eHLS_CATEGORY_NUM; i++)
	{
		for (std::vector<sHLS_PRODUCTS*>::iterator it = m_vecProducts[i].begin(); it != m_vecProducts[i].end(); it++)
		{
			sHLS_PRODUCTS* pProduct = *it;

			std::wstring wstrItemName = pItemTextTable->GetText(pProduct->ItemSlot.GetItemTable()->Name);

			if(wstrItemName.find(searchText) != string::npos) // search
			{
				if ((totalCount % 2) == 0)
				{
					iDialogX = 105;
				}
				else
				{
					iDialogX = 105 + 233;
				}

				if (totalCount == HLS_ITEMS_PER_PAGE)
				{
					totalCount = 0;
					iDialogY = 50;
				}

				CRectangle rect;
				rect.SetRectWH(iDialogX, iDialogY, 229, 86);

				pProduct->pDialog->SetPosition(rect);

				if (showCount++ < HLS_ITEMS_PER_PAGE) // only show first 10 items
				{
					pProduct->pDialog->Show(true);
					m_vecVisibleProducts.push_back(pProduct);
				}

				m_vecSearch.push_back(pProduct);

				if ((totalCount % 2) != 0)
				{
					iDialogY += 90;
				}

				++totalCount;
			}
		}
	}

	// get max pages
	float fPages = ((float)m_vecSearch.size() / (float)HLS_ITEMS_PER_PAGE) + 0.9;
	if (fPages < 1)
		fPages = 1;

	SetPage(1, (int)fPages);
}

void CHLShopGui::OnClickedBtnInitSearchList(gui::CComponent * pComponent)
{
	if (m_bSearch)
	{
		m_pipbHLSSearch->SetText(L"");
		InitSearch();
	}

	int curCat = m_nCurrentCategory;
	m_nCurrentCategory = -1;

	SelectCategory(curCat);
}

void CHLShopGui::OnClickedBtnYadrat(gui::CComponent * pComponent)
{
	GetDboGlobal()->GetGamePacketGenerator()->SendCashItemHLShopRefreshReq();
}

void CHLShopGui::OnClickedBtnCashRecharge(gui::CComponent * pComponent)
{
}

void CHLShopGui::OnClickedBtnProductFirstList(gui::CComponent * pComponent)
{
	if (m_nCurrentPage == 1)
		return;

	// hide current shown items
	ClearVisibleItems();

	// show new items
	int i = 0;

	if (m_bSearch)
	{
		for (std::vector<sHLS_PRODUCTS*>::iterator it = m_vecSearch.begin(); it != m_vecSearch.end(); it++)
		{
			sHLS_PRODUCTS* pProduct = *it;

			pProduct->pDialog->Show(true);

			m_vecVisibleProducts.push_back(pProduct);

			if (++i >= HLS_ITEMS_PER_PAGE) // only show first 10 items
				break;
		}
	}
	else
	{
		for (std::vector<sHLS_PRODUCTS*>::iterator it = m_vecProducts[m_nCurrentCategory].begin(); it != m_vecProducts[m_nCurrentCategory].end(); it++)
		{
			sHLS_PRODUCTS* pProduct = *it;

			pProduct->pDialog->Show(true);

			m_vecVisibleProducts.push_back(pProduct);

			if (++i >= HLS_ITEMS_PER_PAGE) // only show first 10 items
				break;
		}
	}

	SetPage(1, m_nMaxPage);
}

void CHLShopGui::OnClickedBtnProductPrevList(gui::CComponent * pComponent)
{
	if (m_nCurrentPage == 1)
		return;

	// hide current shown items
	ClearVisibleItems();

	int nToSkip = (m_nCurrentPage - 2) * HLS_ITEMS_PER_PAGE;
	if (nToSkip < HLS_ITEMS_PER_PAGE)
		nToSkip = 0;

	int i = 0;
	int nSkip = 0;

	if (m_bSearch)
	{
		for (std::vector<sHLS_PRODUCTS*>::iterator it = m_vecSearch.begin(); it != m_vecSearch.end(); it++)
		{
			sHLS_PRODUCTS* pProduct = *it;

			if (nSkip++ < nToSkip)
				continue;

			pProduct->pDialog->Show(true);

			m_vecVisibleProducts.push_back(pProduct);

			if (++i >= HLS_ITEMS_PER_PAGE) // only show first 10 items
				break;
		}
	}
	else
	{
		for (std::vector<sHLS_PRODUCTS*>::iterator it = m_vecProducts[m_nCurrentCategory].begin(); it != m_vecProducts[m_nCurrentCategory].end(); it++)
		{
			sHLS_PRODUCTS* pProduct = *it;

			if (nSkip++ < nToSkip)
				continue;

			pProduct->pDialog->Show(true);

			m_vecVisibleProducts.push_back(pProduct);

			if (++i >= HLS_ITEMS_PER_PAGE) // only show first 10 items
				break;
		}
	}


	SetPage(m_nCurrentPage - 1, m_nMaxPage);
}

void CHLShopGui::OnClickedBtnProductNextList(gui::CComponent * pComponent)
{
	if (m_nCurrentPage == m_nMaxPage)
		return;

	// hide current shown items
	ClearVisibleItems();

	int nToSkip = m_nCurrentPage * HLS_ITEMS_PER_PAGE;

	int i = 0;
	int nSkip = 0;

	if (m_bSearch)
	{
		for (std::vector<sHLS_PRODUCTS*>::iterator it = m_vecSearch.begin(); it != m_vecSearch.end(); it++)
		{
			sHLS_PRODUCTS* pProduct = *it;

			if (nSkip++ < nToSkip)
				continue;

			pProduct->pDialog->Show(true);

			m_vecVisibleProducts.push_back(pProduct);

			if (++i >= HLS_ITEMS_PER_PAGE) // only show first 10 items
				break;
		}
	}
	else
	{
		for (std::vector<sHLS_PRODUCTS*>::iterator it = m_vecProducts[m_nCurrentCategory].begin(); it != m_vecProducts[m_nCurrentCategory].end(); it++)
		{
			sHLS_PRODUCTS* pProduct = *it;

			if (nSkip++ < nToSkip)
				continue;

			pProduct->pDialog->Show(true);

			m_vecVisibleProducts.push_back(pProduct);

			if (++i >= HLS_ITEMS_PER_PAGE) // only show first 10 items
				break;
		}
	}

	SetPage(m_nCurrentPage + 1, m_nMaxPage);
}

void CHLShopGui::OnClickedBtnProductLastList(gui::CComponent * pComponent)
{
	if (m_nCurrentPage == m_nMaxPage)
		return;

	int nToSkip = (m_nMaxPage - 1) * HLS_ITEMS_PER_PAGE;
	if (nToSkip < 0)
		nToSkip = 0;

	// hide current shown items
	ClearVisibleItems();

	int i = 0;
	int nSkip = 0;

	if (m_bSearch)
	{
		for (std::vector<sHLS_PRODUCTS*>::iterator it = m_vecSearch.begin(); it != m_vecSearch.end(); it++)
		{
			sHLS_PRODUCTS* pProduct = *it;

			if (nSkip++ < nToSkip)
				continue;

			pProduct->pDialog->Show(true);

			m_vecVisibleProducts.push_back(pProduct);

			if (++i >= HLS_ITEMS_PER_PAGE) // only show first 10 items
				break;
		}
	}
	else
	{
		for (std::vector<sHLS_PRODUCTS*>::iterator it = m_vecProducts[m_nCurrentCategory].begin(); it != m_vecProducts[m_nCurrentCategory].end(); it++)
		{
			sHLS_PRODUCTS* pProduct = *it;

			if (nSkip++ < nToSkip)
				continue;

			pProduct->pDialog->Show(true);

			m_vecVisibleProducts.push_back(pProduct);

			if (++i >= HLS_ITEMS_PER_PAGE) // only show first 10 items
				break;
		}
	}

	SetPage(m_nMaxPage, m_nMaxPage);
}

void CHLShopGui::OnClickUpButtonCategory(gui::CComponent * pComponent)
{
	if (m_bSearch)
	{
		InitSearch();
		m_nCurrentCategory = -1;
	}

	for (int i = 0; i < eHLS_CATEGORY_NUM; i++)
	{
		if (m_pBtnCategory[i] == pComponent)
		{
			SelectCategory(i);
			break;
		}
	}
}

void CHLShopGui::OnClickBuyButton(gui::CComponent * pComponent)
{
	// buy window check
	if (GetDialogManager()->IsOpenDialog(DIALOG_HLSHOP_BUY) || GetDialogManager()->IsOpenDialog(DIALOG_HLSHOP_BUY_CONFIRM))
	{
		GetAlarmManager()->AlarmMessage("DST_HLS_ALREADY_CONFIRM");
		return;
	}
	// gift window check
	if (GetDialogManager()->IsOpenDialog(DIALOG_HLSHOP_GIFT) || GetDialogManager()->IsOpenDialog(DIALOG_HLSHOP_GIFT_CONFIRM))
	{
		GetAlarmManager()->AlarmMessage("DST_HLS_ALREADY_GIFT");
		return;
	}

	for (std::vector<sHLS_PRODUCTS*>::iterator it = m_vecVisibleProducts.begin(); it != m_vecVisibleProducts.end(); it++)
	{
		sHLS_PRODUCTS* pProduct = *it;

		if (pProduct->pBtnBuy == pComponent)
		{
			CDboEventGenerator::HLShopEventBuy(false, pProduct->hlsItemTblidx);

			break;
		}
	}
}

void CHLShopGui::OnClickGiftButton(gui::CComponent * pComponent)
{
	// buy window check
	if (GetDialogManager()->IsOpenDialog(DIALOG_HLSHOP_BUY) || GetDialogManager()->IsOpenDialog(DIALOG_HLSHOP_BUY_CONFIRM))
	{
		GetAlarmManager()->AlarmMessage("DST_HLS_ALREADY_CONFIRM");
		return;
	}
	// gift window check
	if (GetDialogManager()->IsOpenDialog(DIALOG_HLSHOP_GIFT) || GetDialogManager()->IsOpenDialog(DIALOG_HLSHOP_GIFT_CONFIRM))
	{
		GetAlarmManager()->AlarmMessage("DST_HLS_ALREADY_GIFT");
		return;
	}

	for (std::vector<sHLS_PRODUCTS*>::iterator it = m_vecVisibleProducts.begin(); it != m_vecVisibleProducts.end(); it++)
	{
		sHLS_PRODUCTS* pProduct = *it;

		if (pProduct->pBtnGift == pComponent)
		{
			CDboEventGenerator::HLShopEventGift(false, pProduct->hlsItemTblidx, L"");

			break;
		}
	}
}

void CHLShopGui::OnMouseEnterItem(gui::CComponent * pComponent)
{
	for (std::vector<sHLS_PRODUCTS*>::iterator it = m_vecVisibleProducts.begin(); it != m_vecVisibleProducts.end(); it++)
	{
		sHLS_PRODUCTS* pProduct = *it;

		if (pProduct->ppnlItemSlot == pComponent)
		{
			ShowItemInfoWindow(true, pProduct);

			break;
		}
	}
}

void CHLShopGui::OnMouseLeaveItem(gui::CComponent * pComponent)
{
	for (std::vector<sHLS_PRODUCTS*>::iterator it = m_vecVisibleProducts.begin(); it != m_vecVisibleProducts.end(); it++)
	{
		sHLS_PRODUCTS* pProduct = *it;

		if (pProduct->ppnlItemSlot == pComponent)
		{
			ShowItemInfoWindow(false, pProduct);

			break;
		}
	}
}

void CHLShopGui::ShowItemInfoWindow(bool bIsShow, sHLS_PRODUCTS* pProduct)
{
	if (bIsShow && pProduct->ItemSlot.GetItemTable() != NULL)
	{
		CRectangle rect = pProduct->ppnlItemSlot->GetScreenRect();

		GetInfoWndManager()->ShowInfoWindow(TRUE, CInfoWndManager::INFOWND_TABLE_ITEM, rect.left, rect.top, pProduct->ItemSlot.GetItemTable(), DIALOG_HLSHOP);
	}
	else
	{
		if (GetInfoWndManager()->GetRequestGui() == DIALOG_HLSHOP)
			GetInfoWndManager()->ShowInfoWindow(FALSE);
	}
}

void CHLShopGui::ShowItemInfoWindow(bool bIsShow, sWAGU_PRODUCTS* pProduct)
{
	if (bIsShow && pProduct->ItemSlot[pProduct->CurShowItem].GetItemTable() != NULL)
	{
		CRectangle rect = pProduct->ppnlItemSlot->GetScreenRect();

		GetInfoWndManager()->ShowInfoWindow(TRUE, CInfoWndManager::INFOWND_TABLE_ITEM, rect.left, rect.top, pProduct->ItemSlot[pProduct->CurShowItem].GetItemTable(), DIALOG_HLSHOP);
	}
	else
	{
		if (GetInfoWndManager()->GetRequestGui() == DIALOG_HLSHOP)
			GetInfoWndManager()->ShowInfoWindow(FALSE);
	}
}

void CHLShopGui::OnMouseEnterWaguItem(gui::CComponent* pComponent)
{
	for (std::vector<sWAGU_PRODUCTS*>::iterator it = m_vecVisibleProductsWagu.begin(); it != m_vecVisibleProductsWagu.end(); it++)
	{
		sWAGU_PRODUCTS* pProduct = *it;

		if (pProduct->ppnlItemSlot == pComponent)
		{
			ShowItemInfoWindow(true, pProduct);

			break;
		}
	}
}

void CHLShopGui::OnMouseLeaveWaguItem(gui::CComponent* pComponent)
{
	for (std::vector<sWAGU_PRODUCTS*>::iterator it = m_vecVisibleProductsWagu.begin(); it != m_vecVisibleProductsWagu.end(); it++)
	{
		sWAGU_PRODUCTS* pProduct = *it;

		if (pProduct->ppnlItemSlot == pComponent)
		{
			ShowItemInfoWindow(false, pProduct);

			break;
		}
	}
}

void CHLShopGui::OnClickedWaguPrev(gui::CComponent* pComponent)
{
	CTextTable* pItemTable = API_GetTableContainer()->GetTextAllTable()->GetItemTbl();

	for (std::vector<sWAGU_PRODUCTS*>::iterator it = m_vecVisibleProductsWagu.begin(); it != m_vecVisibleProductsWagu.end(); it++)
	{
		sWAGU_PRODUCTS* pProduct = *it;

		if (pProduct->btnPrev == pComponent)
		{
			if (pProduct->CurShowItem == 0)
				pProduct->CurShowItem = 9;
			else
				pProduct->CurShowItem--;

			WCHAR Buff[256];
			swprintf_s(Buff, 256, GetDisplayStringManager()->GetString("DST_WAGU_ITEM_CHAMPION_ITEM"), pProduct->CurShowItem + 1);
			pProduct->stbChampionItem->SetText(Buff);

			if (pProduct->ItemSlot[pProduct->CurShowItem].GetItemTable())
			{
				std::wstring text = pItemTable->GetText(pProduct->ItemSlot[pProduct->CurShowItem].GetItemTable()->Name);
				pProduct->stbChampionItemName->SetText(text.c_str());
			}

			break;
		}
	}
}

void CHLShopGui::OnClickedWaguNext(gui::CComponent* pComponent)
{
	CTextTable* pItemTable = API_GetTableContainer()->GetTextAllTable()->GetItemTbl();

	for (std::vector<sWAGU_PRODUCTS*>::iterator it = m_vecVisibleProductsWagu.begin(); it != m_vecVisibleProductsWagu.end(); it++)
	{
		sWAGU_PRODUCTS* pProduct = *it;

		if (pProduct->btnNext == pComponent)
		{
			if (pProduct->CurShowItem == 9)
				pProduct->CurShowItem = 0;
			else
				pProduct->CurShowItem++;

			WCHAR Buff[256];
			swprintf_s(Buff, 256, GetDisplayStringManager()->GetString("DST_WAGU_ITEM_CHAMPION_ITEM"), pProduct->CurShowItem + 1);
			pProduct->stbChampionItem->SetText(Buff);

			if (pProduct->ItemSlot[pProduct->CurShowItem].GetItemTable())
			{
				std::wstring text = pItemTable->GetText(pProduct->ItemSlot[pProduct->CurShowItem].GetItemTable()->Name);
				pProduct->stbChampionItemName->SetText(text.c_str());
			}

			break;
		}
	}
}

void CHLShopGui::OnclickedBtnWaguInfo(gui::CComponent* pComponent)
{
	for (std::vector<sWAGU_PRODUCTS*>::iterator it = m_vecVisibleProductsWagu.begin(); it != m_vecVisibleProductsWagu.end(); it++)
	{
		sWAGU_PRODUCTS* pProduct = *it;

		if (pProduct->btnWaguInfo == pComponent)
		{
			// position the winner-list dialog next to the shop
			CRectangle rect = GetPosition();
			CNtlPLGui* pPLGui = GetDialogManager()->GetDialog(DIALOG_HLSHOP_WAGU_INFO);
			pPLGui->SetPosition(rect.left + rect.GetWidth() + NTL_LINKED_DIALOG_GAP, rect.top);

			GetDboGlobal()->GetChatPacketGenerator()->SendLoadWaguMachineWinnerInfoReq(pProduct->CurMachineIndex);
			CDboEventGenerator::HLShopWaguEventInfo(pProduct->ItemTblidx, pProduct->stbWaguTitle->GetText(), pProduct->CurMachineIndex);

			break;
		}
	}
}

void CHLShopGui::OnClickedBtnBunchInfo(gui::CComponent* pComponent)
{
	for (std::vector<sWAGU_PRODUCTS*>::iterator it = m_vecVisibleProductsWagu.begin(); it != m_vecVisibleProductsWagu.end(); it++)
	{
		sWAGU_PRODUCTS* pProduct = *it;

		if (pProduct->btnBunchInfo == pComponent)
		{
			int x = pProduct->btnBunchInfo->GetScreenRect().left - 10;
			int y = pProduct->btnBunchInfo->GetScreenRect().top - 20;

			GetNtlSLGlobal()->GetSobAvatar()->SetWaguInfo(pProduct->CurMachineIndex, pProduct->CurNeedCoin, pProduct->CurMachineType);

			CDboEventGenerator::IconPopupShow(true, GetNtlSLGlobal()->GetSobAvatar()->GetSerialID(), PLACE_WAGU_ITEM_BUNCH, PLACE_NONE, x, y);

			break;
		}
	}
}

void CHLShopGui::OnClickedExcute(gui::CComponent* pComponent)
{
	for (std::vector<sWAGU_PRODUCTS*>::iterator it = m_vecVisibleProductsWagu.begin(); it != m_vecVisibleProductsWagu.end(); it++)
	{
		sWAGU_PRODUCTS* pProduct = *it;

		if (pProduct->btnExcute == pComponent)
		{
			CDboEventGenerator::WaguMachinesExcute(1, pProduct->CurMachineIndex, pProduct->CurMachineType, pProduct->CurNeedCoin);

			break;
		}
	}
}

void CHLShopGui::OnWaguFlashEnd(gui::CComponent* pComponent)
{
	if (m_pWaguFlash)
		m_pWaguFlash->PlayMovie(FALSE);

	CDboEventGenerator::WaguExcuteRes();

	if (!m_WaguInfo.isEventType)
	{
		int OldWaguPoints = m_WaguInfo.wNewWaguWaguPoints - Logic_GetWaguPoint();
		Logic_SetWaguPoint(m_WaguInfo.wNewWaguWaguPoints);
		CDboEventGenerator::GiftShopEvent(eGIFTSHOP_EVENT_WP_UPDATED, INVALID_SERIAL_ID);

		WCHAR Buff[256];
		swprintf_s(Buff, 256, GetDisplayStringManager()->GetString("DST_WP_ADD_POINT"), OldWaguPoints);
		GetAlarmManager()->AlarmMessage(Buff, 8);
	}

	// one dialog per item won, each showing that single item centered.
	// Called directly through MsgBoxShow (bAcceptDuplicate=TRUE) instead of
	// AlarmMessage/FormattedAlarmMessage, since the latter's "allow duplicate
	// window" flag is driven by local_msg_type.alm and depends on the client
	// having repacked lang0.pak — going straight to MsgBoxShow guarantees a
	// separate window per item regardless of that data file's state.
	for (int i = 0; i < m_WaguInfo.byReallyExtractCount; i++)
	{
		sMsgBoxData data;
		memset(&data, 0, sizeof(sMsgBoxData));
		data.sWaguInfo.byRanking[0] = m_WaguInfo.byRanking[i];
		data.sWaguInfo.ItemTblidx[0] = m_WaguInfo.ItemTblidx[i];
		data.sWaguInfo.bySetCount[0] = m_WaguInfo.bySetCount[i];
		data.sWaguInfo.byStackCount[0] = m_WaguInfo.byStackCount[i];
		data.sWaguInfo.byReallyExtractCount = 1;
		data.sWaguInfo.isWagu = true;

		const char* pStrKey;
		WCHAR Buff[256];
		if (m_WaguInfo.bySetCount[i] > 1)
		{
			pStrKey = "DST_WAGU_EXCUTE_RESULT_MORE_SET";
			swprintf_s(Buff, 256, GetDisplayStringManager()->GetString(pStrKey), m_WaguInfo.byRanking[i], Logic_GetItemName(m_WaguInfo.ItemTblidx[i]), m_WaguInfo.bySetCount[i]);
		}
		else
		{
			pStrKey = "DST_WAGU_EXCUTE_RESULT";
			swprintf_s(Buff, 256, GetDisplayStringManager()->GetString(pStrKey), m_WaguInfo.byRanking[i], Logic_GetItemName(m_WaguInfo.ItemTblidx[i]));
		}

		std::list<sMsgBoxCustomBtn> listCustomBtn;
		sMsgBoxCustomBtn btnOk;
		btnOk.bHasButton = TRUE;
		btnOk.strMessage = "DST_MSG_BTN_OK";
		listCustomBtn.push_back(btnOk);

		CDboEventGenerator::MsgBoxShow(pStrKey, Buff, TRUE, FALSE, 0.0f, &data, &listCustomBtn, TRUE, FALSE);
	}

	for (int i = 0; i < m_WaguInfo.byReallyExtractCount; i++)
	{
		if (m_WaguInfo.byRanking[i] == 1)
		{
			sNtlSoundPlayParameta tSoundParam;
			tSoundParam.iChannelGroup = CHANNEL_GROUP_UI_SOUND;
			tSoundParam.pcFileName = GSD_WAGU_FIRST_PRIZE;
			GetSoundManager()->Play(&tSoundParam);
			break;
		}
	}
}

void CHLShopGui::RefreshWaguInfo(BYTE WaguType, WORD* CurCap, WORD* MaxCap, TBLIDX* MachineIndex)
{
	// hide currently shown wagu machines
	for (std::vector<sWAGU_PRODUCTS*>::iterator it = m_vecVisibleProductsWagu.begin(); it != m_vecVisibleProductsWagu.end(); it++)
	{
		sWAGU_PRODUCTS* pProduct = *it;

		pProduct->pDialog->Show(false);
	}
	m_vecVisibleProductsWagu.clear();

	if (WaguType >= 2)
		return;

	int i = 0;
	for (std::vector<sWAGU_PRODUCTS*>::iterator it = m_vecProductsWagu[WaguType].begin(); it != m_vecProductsWagu[WaguType].end(); it++)
	{
		sWAGU_PRODUCTS* pProduct = *it;

		pProduct->CurCapNum = CurCap[i];
		pProduct->MaxCapNum = MaxCap[i];

		int CurWaguPercent = (MaxCap[i] > 0) ? (int)((float)CurCap[i] / (float)MaxCap[i] * 100) : 0;

		pProduct->pProgressbar->SetPos(CurWaguPercent);

		WCHAR Buff[64];
		swprintf_s(Buff, 64, L"%d / %d", CurCap[i], MaxCap[i]);
		pProduct->stbLeftCapsuleNum->SetText(Buff);

		const char* pSkinSuffix = (CurWaguPercent > 75) ? "_4" : (CurWaguPercent > 50) ? "_3" : (CurWaguPercent > 25) ? "_2" : "_1";
		const char* pSkinColor = NULL;
		switch (pProduct->CurMachineIndex % 100)
		{
			case 1: pSkinColor = "srfBuleMachine"; break;
			case 2: pSkinColor = "srfYellowMachine"; break;
			case 3: pSkinColor = "srfRedMachine"; break;
			default: pSkinColor = "srfGreenMachine"; break;
		}

		char szSurfaceName[64];
		sprintf_s(szSurfaceName, "%s%s", pSkinColor, pSkinSuffix);

		pProduct->pnlWaguMachine->ClearSurface();
		pProduct->pnlWaguMachine->AddSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("HLS.srf", szSurfaceName));

		m_vecVisibleProductsWagu.push_back(pProduct);
		pProduct->pDialog->Show(true);

		if (++i >= 4)
			break;
	}

	OnMove(0, 0);
}

void CHLShopGui::InitSearch()
{
	m_bSearch = false;

	// reset location
	for (std::vector<sHLS_PRODUCTS*>::iterator it = m_vecSearch.begin(); it != m_vecSearch.end(); it++)
	{
		sHLS_PRODUCTS* pProduct = *it;

		CRectangle rect;
		rect.SetRectWH(pProduct->nDialogX, pProduct->nDialogY, 229, 86);

		pProduct->pDialog->SetPosition(rect);
	}

	m_vecSearch.clear();
}
