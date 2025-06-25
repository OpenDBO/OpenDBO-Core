#include "precomp_dboclient.h"
// core
#include "NtlDebug.h"
#include "CEventHandler.h"

// presentation
#include "NtlPLGui.h"
#include "NtlPLGuiManager.h"

// framework
#include "NtlApplication.h"

// Simulation
#include "InputActionMap.h"
#include "NtlSLEvent.h"
#include "NtlStorageManager.h"
#include "NtlStorageMTContainer.h"

// dbo
#include "DialogManager.h"
#include "DisplayStringManager.h"

// Option
#include "OptionWindowGui.h"
#include "OptionBase.h"
#include "OptionFilter.h"


/////////////////////////////////////////////////////////////////////////////
// class : COptionFilterCategoryNode

COptionFilterCategoryNode::COptionFilterCategoryNode(CGuiLineTree* pMgr, std::wstring strTitle, RwInt32 nID)
    : CGuiLineTreeNode(pMgr, nID)
    , m_pBtnExpand(NULL)
    , m_pBtnReduce(NULL)
{
    // + Button
    CRectangle rect;
    rect.SetRectWH(0, 0, 140, 20);
    m_pBtnExpand = NTL_NEW gui::CButton(rect, std::string(), pMgr->GetParentGui(), GetNtlGuiManager()->GetSurfaceManager());
    m_pBtnExpand->AddSurfaceUp(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("QuestList.srf", "srfExpandBtnUp"));
    m_pBtnExpand->AddSurfaceFocus(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("QuestList.srf", "srfExpandBtnFoc"));
    m_pBtnExpand->AddSurfaceDown(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("QuestList.srf", "srfExpandBtnDown"));
    m_pBtnExpand->SetTextStyle(COMP_TEXT_LEFT);
    m_pBtnExpand->SetTextCoord(25, -2);
    m_pBtnExpand->SetText(strTitle.c_str());

    // - Button
    m_pBtnReduce = NTL_NEW gui::CButton(rect, std::string(), pMgr->GetParentGui(), GetNtlGuiManager()->GetSurfaceManager());
    m_pBtnReduce->AddSurfaceUp(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("QuestList.srf", "srfReduceBtnUp"));
    m_pBtnReduce->AddSurfaceFocus(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("QuestList.srf", "srfReduceBtnFoc"));
    m_pBtnReduce->AddSurfaceDown(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("QuestList.srf", "srfReduceBtnDown"));
    m_pBtnReduce->SetTextStyle(COMP_TEXT_LEFT);
    m_pBtnReduce->SetTextCoord(25, -2);
    m_pBtnReduce->SetText(strTitle.c_str());

    // Button signals
    m_slotClickedBtnExpand = m_pBtnExpand->SigClicked().Connect(this, &COptionFilterCategoryNode::OnClickBtnExpand);
    m_slotClickedBtnReduce = m_pBtnReduce->SigClicked().Connect(this, &COptionFilterCategoryNode::OnClickBtnReduce);
}

COptionFilterCategoryNode::~COptionFilterCategoryNode()
{
    NTL_DELETE(m_pBtnReduce);
    NTL_DELETE(m_pBtnExpand);
}

void COptionFilterCategoryNode::ShowProc()
{
    CGuiLineTreeNode::ShowProc();

    if (IsExpand())
    {
        m_pBtnReduce->Show(true);
        m_pBtnExpand->Show(false);
    }
    else
    {
        m_pBtnExpand->Show(true);
        m_pBtnReduce->Show(false);
    }

    m_pBtnExpand->SetPosition(m_nPosX, m_nPosY);
    m_pBtnReduce->SetPosition(m_nPosX, m_nPosY);
}

void COptionFilterCategoryNode::HideProc()
{
    CGuiLineTreeNode::HideProc();

    m_pBtnExpand->Show(false);
    m_pBtnReduce->Show(false);
}

void COptionFilterCategoryNode::OnClickBtnExpand(gui::CComponent* pComponent)
{
    Expand(TRUE);
}

void COptionFilterCategoryNode::OnClickBtnReduce(gui::CComponent* pComponent)
{
    Expand(FALSE);
}



/////////////////////////////////////////////////////////////////////////////
// class : CCheckFilterNode

// CheckBoxFilter dimensions
#define dOPTIONCHATTING_CHECKNODE_CHECKBOX_X		5
#define dOPTIONCHATTING_CHECKNODE_CHECKBOX_Y		5
#define dOPTIONCHATTING_CHECKNODE_CHECKBOX_WIDTH	16
#define dOPTIONCHATTING_CHECKNODE_CHECKBOX_HEIGHT	16

// CheckBoxFilter table dimensions
#define dOPTIONCHATTING_CHECKNODE_TITLE_X			30
#define dOPTIONCHATTING_CHECKNODE_TITLE_Y			0
#define dOPTIONCHATTING_CHECKNODE_TITLE_WIDTH		200
#define dOPTIONCHATTING_CHECKNODE_TITLE_HEGIHT		20
#define dOPTIONCHATTING_CHECKNODE_TITLE_COLOR		RGB(255, 255, 255)

CCheckFilterNode::CCheckFilterNode(CGuiLineTree* pMgr, RwInt32 nID, std::wstring strTitle, std::wstring strToolTip)
    : CGuiLineTreeNode(pMgr, nID)
    , m_pBtnCheck(NULL)
    , m_pStbTitle(NULL)
{
    // Check Box
    CRectangle rect;
    rect.SetRectWH(dOPTIONCHATTING_CHECKNODE_CHECKBOX_X,
        dOPTIONCHATTING_CHECKNODE_CHECKBOX_Y,
        dOPTIONCHATTING_CHECKNODE_CHECKBOX_WIDTH,
        dOPTIONCHATTING_CHECKNODE_CHECKBOX_HEIGHT);
    m_pBtnCheck = NTL_NEW gui::CButton(rect, std::string(), pMgr->GetParentGui(), GetNtlGuiManager()->GetSurfaceManager());
    m_pBtnCheck->AddSurfaceUp(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("OptionWnd.srf", "srfSmallCheckBoxOff"));
    m_pBtnCheck->AddSurfaceFocus(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("OptionWnd.srf", "srfSmallCheckBoxFocus"));
    m_pBtnCheck->AddSurfaceDown(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("OptionWnd.srf", "srfSmallCheckBoxOn"));
    m_pBtnCheck->AddSurfaceDisabled(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("OptionWnd.srF", "srfSmallCheckBoxOn"));
    m_pBtnCheck->SetToggleMode(true);

    // Signals.
    m_slotToggledBtnCheck = m_pBtnCheck->SigToggled().Connect(this, &CCheckFilterNode::OnToggledCheckFilter);


    // Title
    rect.SetRectWH(dOPTIONCHATTING_CHECKNODE_TITLE_X,
        dOPTIONCHATTING_CHECKNODE_TITLE_Y,
        dOPTIONCHATTING_CHECKNODE_TITLE_WIDTH,
        dOPTIONCHATTING_CHECKNODE_TITLE_HEGIHT);
    m_pStbTitle = NTL_NEW gui::CStaticBox(rect, pMgr->GetParentGui(), GetNtlGuiManager()->GetSurfaceManager());
    m_pStbTitle->CreateFontStd(DETAIL_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
    m_pStbTitle->SetTextStyle(COMP_TEXT_LEFT);
    m_pStbTitle->SetText(strTitle.c_str());
    m_pStbTitle->SetToolTip(strToolTip.c_str());
}

CCheckFilterNode::~CCheckFilterNode()
{
    NTL_DELETE(m_pStbTitle);
    NTL_DELETE(m_pBtnCheck);
}

void CCheckFilterNode::ShowProc()
{
    CGuiLineTreeNode::ShowProc();

    m_pBtnCheck->Show(true);
    m_pStbTitle->Show(true);

    m_pBtnCheck->SetPosition(m_nPosX + dOPTIONCHATTING_CHECKNODE_CHECKBOX_X,
        m_nPosY + dOPTIONCHATTING_CHECKNODE_CHECKBOX_Y);
    m_pStbTitle->SetPosition(m_nPosX + dOPTIONCHATTING_CHECKNODE_TITLE_X,
        m_nPosY + dOPTIONCHATTING_CHECKNODE_TITLE_Y);
}

void CCheckFilterNode::HideProc()
{
    CGuiLineTreeNode::HideProc();

    m_pBtnCheck->Show(false);
    m_pStbTitle->Show(false);
}

void CCheckFilterNode::OnToggledCheckFilter(gui::CComponent* pComponent, bool bDown)
{

}

/////////////////////////////////////////////////////////////////////////////
// class : COptionFilterList

COptionFilterList::COptionFilterList()
{
}

COptionFilterList::~COptionFilterList()
{
}

RwBool COptionFilterList::Create(CRectangle& rect, gui::CComponent* pParent, RwInt32 nLineHeight, RwInt32 nLineMargin, RwInt32 nChildXMargin, RwInt32 nScrollBarWidth, COptionFilter* pOptionFilter)
{
    if (!CGuiLineTree::Create(rect, pParent, nLineHeight, nLineMargin, nChildXMargin, nScrollBarWidth))
        return FALSE;

    m_pScroll->AddSurfaceLayout(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("GameCommon.srf", "srfScrollLayout"));
    m_pScroll->AddSurfaceSlider(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("GameCommon.srf", "srfScrollMedium"));
    m_pScroll->GetDecreaseButton()->AddSurfaceUp(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("GameCommon.srf", "srfScrollPrevBtnUp"));
    m_pScroll->GetDecreaseButton()->AddSurfaceDown(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("GameCommon.srf", "srfScrollPrevBtnDown"));
    m_pScroll->GetDecreaseButton()->AddSurfaceFocus(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("GameCommon.srf", "srfScrollPrevBtnFoc"));
    m_pScroll->GetIncreaseButton()->AddSurfaceUp(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("GameCommon.srf", "srfScrollNextBtnUp"));
    m_pScroll->GetIncreaseButton()->AddSurfaceDown(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("GameCommon.srf", "srfScrollNextBtnDown"));
    m_pScroll->GetIncreaseButton()->AddSurfaceFocus(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("GameCommon.srf", "srfScrollNextBtnUp"));
    m_pScroll->SetButtonSize(dOPTIONFILTER_LIST_SCROLL_WIDTH, dOPTIONFILTER_LIST_SCROLL_HEIGHT);
    m_pScroll->SetSliderSize(dOPTIONFILTER_LIST_SLIDER_WIDTH, dOPTIONFILTER_LIST_SLIDER_HEIGHT);

    m_slotCaptureWheelMove = GetNtlGuiManager()->GetGuiManager()->SigCaptureWheelMove().Connect(this, &COptionFilterList::OnCaptureWheelMove);
    m_pOptionFilter = pOptionFilter;

    return TRUE;
}

void COptionFilterList::CreateTree()
{
#define OCLGDS(stringid) GetDisplayStringManager()->GetString(stringid)

    // --------------Rarity category----------------------
    CGuiLineTreeNode* pNode = NTL_NEW COptionFilterCategoryNode(this,
        OCLGDS("DST_OPTION_FILTER_RARITY"),
        dOPTIONFILTER_CATEGORY_RARITY);
    CGuiLineTree::AddNode(pNode, GUILINETREE_ROOTNODE_ID);
    pNode->Expand(true);

    // 1. Normal
    pNode = NTL_NEW CCheckFilterNode(
        this,
        eFILTER_RARITY_NORMAL,
        OCLGDS("DST_OPTION_FILTER_RARITY_NORMAL"),
        OCLGDS("DST_OPTION_TOOLTIP_FILTER_NORMAL")
    );
    CGuiLineTree::AddNode(pNode, dOPTIONFILTER_CATEGORY_RARITY);

    // 2. Superior
    pNode = NTL_NEW CCheckFilterNode(
        this,
        eFILTER_RARITY_SUPERIOR,
        OCLGDS("DST_OPTION_FILTER_RARITY_SUPERIOR"),
        OCLGDS("DST_OPTION_TOOLTIP_FILTER_SUPERIOR")
    );
    CGuiLineTree::AddNode(pNode, dOPTIONFILTER_CATEGORY_RARITY);

    // 3. Excellent
    pNode = NTL_NEW CCheckFilterNode(
        this,
        eFILTER_RARITY_EXCELLENT,
        OCLGDS("DST_OPTION_FILTER_RARITY_EXCELLENT"),
        OCLGDS("DST_OPTION_TOOLTIP_FILTER_EXCELLENT")
    );
    CGuiLineTree::AddNode(pNode, dOPTIONFILTER_CATEGORY_RARITY);

    // 4. Rare
    pNode = NTL_NEW CCheckFilterNode(
        this,
        eFILTER_RARITY_RARE,
        OCLGDS("DST_OPTION_FILTER_RARITY_RARE"),
        OCLGDS("DST_OPTION_TOOLTIP_FILTER_RARE")
    );
    CGuiLineTree::AddNode(pNode, dOPTIONFILTER_CATEGORY_RARITY);

    // 5. Legendary
    pNode = NTL_NEW CCheckFilterNode(
        this,
        eFILTER_RARITY_LEGENDARY,
        OCLGDS("DST_OPTION_FILTER_RARITY_LEGENDARY"),
        OCLGDS("DST_OPTION_TOOLTIP_FILTER_LEGENDARY")
    );
    CGuiLineTree::AddNode(pNode, dOPTIONFILTER_CATEGORY_RARITY);

    // ------------Stones category-------------------
    pNode = NTL_NEW COptionFilterCategoryNode(this,
        GetDisplayStringManager()->GetString("DST_OPTION_FILTER_STONES"),
        dOPTIONFILTER_CATEGORY_STONES);
    CGuiLineTree::AddNode(pNode, GUILINETREE_ROOTNODE_ID);
    pNode->Expand(false);

    // 1. Blue Stones
    pNode = NTL_NEW CCheckFilterNode(
        this,
        eFILTER_STONE_BLUE,
        OCLGDS("DST_OPTION_FILTER_STONE_BLUE"),
        OCLGDS("DST_OPTION_TOOLTIP_FILTER_STONE_BLUE")
    );
    CGuiLineTree::AddNode(pNode, dOPTIONFILTER_CATEGORY_STONES);

    // 2. Red Stones
    pNode = NTL_NEW CCheckFilterNode(
        this,
        eFILTER_STONE_RED,
        OCLGDS("DST_OPTION_FILTER_STONE_RED"),
        OCLGDS("DST_OPTION_TOOLTIP_FILTER_STONE_RED")
    );
    CGuiLineTree::AddNode(pNode, dOPTIONFILTER_CATEGORY_STONES);

    // 3. Green Stones
    pNode = NTL_NEW CCheckFilterNode(
        this,
        eFILTER_STONE_GREEN,
        OCLGDS("DST_OPTION_FILTER_STONE_GREEN"),
        OCLGDS("DST_OPTION_TOOLTIP_FILTER_STONE_GREEN")
    );
    CGuiLineTree::AddNode(pNode, dOPTIONFILTER_CATEGORY_STONES);

    // 4. Purple Stones
    pNode = NTL_NEW CCheckFilterNode(
        this,
        eFILTER_STONE_PURPLE,
        OCLGDS("DST_OPTION_FILTER_STONE_PURPLE"),
        OCLGDS("DST_OPTION_TOOLTIP_FILTER_STONE_PURPLE")
    );
    CGuiLineTree::AddNode(pNode, dOPTIONFILTER_CATEGORY_STONES);

    // 5. White Stones
    pNode = NTL_NEW CCheckFilterNode(
        this,
        eFILTER_STONE_WHITE,
        OCLGDS("DST_OPTION_FILTER_STONE_WHITE"),
        OCLGDS("DST_OPTION_TOOLTIP_FILTER_STONE_WHITE")
    );
    CGuiLineTree::AddNode(pNode, dOPTIONFILTER_CATEGORY_STONES);

    // 6. Armor Downgrade Stones
    pNode = NTL_NEW CCheckFilterNode(
        this,
        eFILTER_STONE_ADOWN,
        OCLGDS("DST_OPTION_FILTER_STONE_ADOWN"),
        OCLGDS("DST_OPTION_TOOLTIP_FILTER_STONE_ADOWN")
    );
    CGuiLineTree::AddNode(pNode, dOPTIONFILTER_CATEGORY_STONES);

    // 7. Weapon Downgrade Stones
    pNode = NTL_NEW CCheckFilterNode(
        this,
        eFILTER_STONE_WDOWN,
        OCLGDS("DST_OPTION_FILTER_STONE_WDOWN"),
        OCLGDS("DST_OPTION_TOOLTIP_FILTER_STONE_WDOWN")
    );
    CGuiLineTree::AddNode(pNode, dOPTIONFILTER_CATEGORY_STONES);

    // ------------Misc category-------------------
    pNode = NTL_NEW COptionFilterCategoryNode(this,
        GetDisplayStringManager()->GetString("DST_OPTION_FILTER_MISC"),
        dOPTIONFILTER_CATEGORY_MISC);
    CGuiLineTree::AddNode(pNode, GUILINETREE_ROOTNODE_ID);
    pNode->Expand(false);

    // 1. Mystery Item
    pNode = NTL_NEW CCheckFilterNode(
        this,
        eFILTER_MYSTERYITEM,
        OCLGDS("DST_OPTION_FILTER_MYSTERYITEM"),
        OCLGDS("DST_OPTION_TOOLTIP_FILTER_MYSTERYITEM")
    );
    CGuiLineTree::AddNode(pNode, dOPTIONFILTER_CATEGORY_MISC);

    // 2. Consumables
    pNode = NTL_NEW CCheckFilterNode(
        this,
        eFILTER_CONSUMABLE,
        OCLGDS("DST_OPTION_FILTER_CONSUMABLE"),
        OCLGDS("DST_OPTION_TOOLTIP_FILTER_CONSUMABLE")
    );
    CGuiLineTree::AddNode(pNode, dOPTIONFILTER_CATEGORY_MISC);

}


void COptionFilterList::OnCaptureWheelMove(RwInt32 nFlag, RwInt16 sDelta, CPos& pos)
{
    if (!GetParentGui()->IsVisible())
        return;

    if (GetParentGui()->PosInRect(pos.x, pos.y) != gui::CComponent::INRECT)
        return;

    RwInt32 nValue = static_cast<gui::CScrollBar*>(CGuiLineTree::GetScrollBar())->GetValue();
    RwInt32 nMaxValue = static_cast<gui::CScrollBar*>(CGuiLineTree::GetScrollBar())->GetMaxValue();
    RwInt32 nDelta = nValue - sDelta / GUI_MOUSE_WHEEL_DELTA;

    if (nDelta < 0)
        nDelta = 0;
    else if (nDelta > nMaxValue)
        nDelta = nMaxValue;

    static_cast<gui::CScrollBar*>(CGuiLineTree::GetScrollBar())->SetValue(nDelta);
}

/////////////////////////////////////////////////////////////////////////////
// class : COptionFilter

COptionFilter::COptionFilter()
{
    m_pOptionFilterList = NULL;
    m_pDlgFilterList = NULL;
}

COptionFilter::~COptionFilter()
{
}

RwBool COptionFilter::Create(COptionWindowGui* pOptionWindow)
{
    NTL_FUNCTION("COptionFilter::Create");

    COptionBase::Create(pOptionWindow);

    // Creation of m_pDlgFilterList for the dialog of the drop filter
    m_pDlgFilterList = (gui::CDialog*)GetComponent("dlgFilterList");

    CRectangle rect;
    rect.SetRectWH(dOPTIONFILTER_LIST_X,
        dOPTIONFILTER_LIST_Y,
        dOPTIONFILTER_LIST_WIDTH,
        dOPTIONFILTER_LIST_HEIGHT);

    m_pOptionFilterList = NTL_NEW COptionFilterList;
    if (!m_pOptionFilterList->Create(rect, m_pDlgFilterList,
        dOPTIONFILTER_LINE_HEIGHT,
        dOPTIONFILTER_LINE_MARGIN,
        dOPTIONFILTER_CHILD_MARGIN_WIDTH,
        dOPTIONFILTER_LIST_SLIDER_WIDTH, this))
        return FALSE;

    m_pOptionFilterList->CreateTree();

    NTL_RETURN(TRUE);
}

void COptionFilter::Destroy()
{
    NTL_FUNCTION("COptionFilter::Destroy");

    if (m_pOptionFilterList)
    {
        m_pOptionFilterList->Destroy();
        NTL_DELETE(m_pOptionFilterList);
    }

    NTL_RETURNVOID();
}

void COptionFilter::Show()
{
    COptionBase::Show();

    m_pScrollBar->Show(false);

    m_pDlgFilterList->Show(true);
}

void COptionFilter::Hide()
{
    COptionBase::Hide();

    m_pScrollBar->Show(true);

    m_pDlgFilterList->Show(false);
}

void COptionFilter::OnInit()
{
    LoadSerializer();
}

void COptionFilter::OnReset()
{
    DefaultOption();
}

void COptionFilter::OnOk()
{
    SaveSerializer();
    GetNtlStorageManager()->ApplyStorageType(eNTL_STORAGE_EXTRAFILTER, eNTL_STORAGE_APPLY_ALL);
    //DBO_WARNING_MESSAGE("DBO Warning message here: EXTRA TAB -> The apply button was pressed")
}

void COptionFilter::OnCancel()
{
}

void COptionFilter::OnHandleEvents(RWS::CMsg& pMsg)
{
}

VOID COptionFilter::LoadSerializer()
{
    bool value;

    // Category Rarity
    CCheckFilterNode* pNode = static_cast<CCheckFilterNode*>(m_pOptionFilterList->FindNode(eFILTER_RARITY_NORMAL));
    value = GetNtlStorageManager()->GetBoolData(dSTORAGE_FILTER_RARITY_NORMAL);
    //DBO_WARNING_MESSAGE(("Loading dSTORAGE_FILTER_RARITY_NORMAL: " + std::to_string(value)).c_str());
    pNode->SetDown(value);

    pNode = static_cast<CCheckFilterNode*>(m_pOptionFilterList->FindNode(eFILTER_RARITY_SUPERIOR));
    value = GetNtlStorageManager()->GetBoolData(dSTORAGE_FILTER_RARITY_SUPERIOR);
    //DBO_WARNING_MESSAGE(("Loading dSTORAGE_FILTER_RARITY_SUPERIOR: " + std::to_string(value)).c_str());
    pNode->SetDown(value);

    pNode = static_cast<CCheckFilterNode*>(m_pOptionFilterList->FindNode(eFILTER_RARITY_EXCELLENT));
    value = GetNtlStorageManager()->GetBoolData(dSTORAGE_FILTER_RARITY_EXCELLENT);
    //DBO_WARNING_MESSAGE(("Loading dSTORAGE_FILTER_RARITY_EXCELLENT: " + std::to_string(value)).c_str());
    pNode->SetDown(value);

    pNode = static_cast<CCheckFilterNode*>(m_pOptionFilterList->FindNode(eFILTER_RARITY_RARE));
    value = GetNtlStorageManager()->GetBoolData(dSTORAGE_FILTER_RARITY_RARE);
    //DBO_WARNING_MESSAGE(("Loading dSTORAGE_FILTER_RARITY_RARE: " + std::to_string(value)).c_str());
    pNode->SetDown(value);

    pNode = static_cast<CCheckFilterNode*>(m_pOptionFilterList->FindNode(eFILTER_RARITY_LEGENDARY));
    value = GetNtlStorageManager()->GetBoolData(dSTORAGE_FILTER_RARITY_LEGENDARY);
    //DBO_WARNING_MESSAGE(("Loading dSTORAGE_FILTER_RARITY_LEGENDARY: " + std::to_string(value)).c_str());
    pNode->SetDown(value);


    // Category Stones
    pNode = static_cast<CCheckFilterNode*>(m_pOptionFilterList->FindNode(eFILTER_STONE_BLUE));
    value = GetNtlStorageManager()->GetBoolData(dSTORAGE_FILTER_STONE_BLUE);
    //DBO_WARNING_MESSAGE(("Loading dSTORAGE_FILTER_STONE_BLUE: " + std::to_string(value)).c_str());
    pNode->SetDown(value);

    pNode = static_cast<CCheckFilterNode*>(m_pOptionFilterList->FindNode(eFILTER_STONE_RED));
    value = GetNtlStorageManager()->GetBoolData(dSTORAGE_FILTER_STONE_RED);
    //DBO_WARNING_MESSAGE(("Loading dSTORAGE_FILTER_STONE_BLUE: " + std::to_string(value)).c_str());
    pNode->SetDown(value);

    pNode = static_cast<CCheckFilterNode*>(m_pOptionFilterList->FindNode(eFILTER_STONE_GREEN));
    value = GetNtlStorageManager()->GetBoolData(dSTORAGE_FILTER_STONE_GREEN);
    //DBO_WARNING_MESSAGE(("Loading dSTORAGE_FILTER_STONE_GREEN: " + std::to_string(value)).c_str());
    pNode->SetDown(value);

    pNode = static_cast<CCheckFilterNode*>(m_pOptionFilterList->FindNode(eFILTER_STONE_PURPLE));
    value = GetNtlStorageManager()->GetBoolData(dSTORAGE_FILTER_STONE_PURPLE);
    //DBO_WARNING_MESSAGE(("Loading dSTORAGE_FILTER_STONE_PURPLE: " + std::to_string(value)).c_str());
    pNode->SetDown(value);

    pNode = static_cast<CCheckFilterNode*>(m_pOptionFilterList->FindNode(eFILTER_STONE_WHITE));
    value = GetNtlStorageManager()->GetBoolData(dSTORAGE_FILTER_STONE_WHITE);
    //DBO_WARNING_MESSAGE(("Loading dSTORAGE_FILTER_STONE_WHITE: " + std::to_string(value)).c_str());
    pNode->SetDown(value);

    pNode = static_cast<CCheckFilterNode*>(m_pOptionFilterList->FindNode(eFILTER_STONE_ADOWN));
    value = GetNtlStorageManager()->GetBoolData(dSTORAGE_FILTER_STONE_ADOWN);
    //DBO_WARNING_MESSAGE(("Loading dSTORAGE_FILTER_STONE_ADOWN: " + std::to_string(value)).c_str());
    pNode->SetDown(value);

    pNode = static_cast<CCheckFilterNode*>(m_pOptionFilterList->FindNode(eFILTER_STONE_WDOWN));
    value = GetNtlStorageManager()->GetBoolData(dSTORAGE_FILTER_STONE_WDOWN);
    //DBO_WARNING_MESSAGE(("Loading dSTORAGE_FILTER_STONE_WDOWN: " + std::to_string(value)).c_str());
    pNode->SetDown(value);


    // Category Misc (mystery items, LP and EP recovery items)
    pNode = static_cast<CCheckFilterNode*>(m_pOptionFilterList->FindNode(eFILTER_MYSTERYITEM));
    value = GetNtlStorageManager()->GetBoolData(dSTORAGE_FILTER_MYSTERYITEM);
    //DBO_WARNING_MESSAGE(("Loading dSTORAGE_FILTER_STONE_WDOWN: " + std::to_string(value)).c_str());
    pNode->SetDown(value);

    pNode = static_cast<CCheckFilterNode*>(m_pOptionFilterList->FindNode(eFILTER_CONSUMABLE));
    value = GetNtlStorageManager()->GetBoolData(dSTORAGE_FILTER_CONSUMABLE);
    //DBO_WARNING_MESSAGE(("Loading dSTORAGE_FILTER_CONSUMABLE: " + std::to_string(value)).c_str());
    pNode->SetDown(value);


}
VOID COptionFilter::SaveSerializer()
{
    // Category Rarity
    CCheckFilterNode* pNode = static_cast<CCheckFilterNode*>(m_pOptionFilterList->FindNode(eFILTER_RARITY_NORMAL));
    bool value = pNode->GetDown();
    //DBO_WARNING_MESSAGE(("Saving dSTORAGE_FILTER_RARITY_NORMAL: " + std::to_string(value)).c_str());
    GetNtlStorageManager()->SetData(dSTORAGE_FILTER_RARITY_NORMAL, value);

    pNode = static_cast<CCheckFilterNode*>(m_pOptionFilterList->FindNode(eFILTER_RARITY_SUPERIOR));
    value = pNode->GetDown();
    //DBO_WARNING_MESSAGE(("Saving dSTORAGE_FILTER_RARITY_SUPERIOR: " + std::to_string(value)).c_str());
    GetNtlStorageManager()->SetData(dSTORAGE_FILTER_RARITY_SUPERIOR, value);

    pNode = static_cast<CCheckFilterNode*>(m_pOptionFilterList->FindNode(eFILTER_RARITY_EXCELLENT));
    value = pNode->GetDown();
    //DBO_WARNING_MESSAGE(("Saving dSTORAGE_FILTER_RARITY_EXCELLENT: " + std::to_string(value)).c_str());
    GetNtlStorageManager()->SetData(dSTORAGE_FILTER_RARITY_EXCELLENT, value);

    pNode = static_cast<CCheckFilterNode*>(m_pOptionFilterList->FindNode(eFILTER_RARITY_RARE));
    value = pNode->GetDown();
    //DBO_WARNING_MESSAGE(("Saving dSTORAGE_FILTER_RARITY_RARE: " + std::to_string(value)).c_str());
    GetNtlStorageManager()->SetData(dSTORAGE_FILTER_RARITY_RARE, value);

    pNode = static_cast<CCheckFilterNode*>(m_pOptionFilterList->FindNode(eFILTER_RARITY_LEGENDARY));
    value = pNode->GetDown();
    //DBO_WARNING_MESSAGE(("Saving dSTORAGE_FILTER_RARITY_LEGENDARY: " + std::to_string(value)).c_str());
    GetNtlStorageManager()->SetData(dSTORAGE_FILTER_RARITY_LEGENDARY, value);


    // Category Stones
    pNode = static_cast<CCheckFilterNode*>(m_pOptionFilterList->FindNode(eFILTER_STONE_BLUE));
    value = pNode->GetDown();
    //DBO_WARNING_MESSAGE(("Saving dSTORAGE_FILTER_STONE_BLUE: " + std::to_string(value)).c_str());
    GetNtlStorageManager()->SetData(dSTORAGE_FILTER_STONE_BLUE, value);

    pNode = static_cast<CCheckFilterNode*>(m_pOptionFilterList->FindNode(eFILTER_STONE_RED));
    value = pNode->GetDown();
    //DBO_WARNING_MESSAGE(("Saving dSTORAGE_FILTER_STONE_RED: " + std::to_string(value)).c_str());
    GetNtlStorageManager()->SetData(dSTORAGE_FILTER_STONE_RED, value);

    pNode = static_cast<CCheckFilterNode*>(m_pOptionFilterList->FindNode(eFILTER_STONE_GREEN));
    value = pNode->GetDown();
    //DBO_WARNING_MESSAGE(("Saving dSTORAGE_FILTER_STONE_GREEN: " + std::to_string(value)).c_str());
    GetNtlStorageManager()->SetData(dSTORAGE_FILTER_STONE_GREEN, value);

    pNode = static_cast<CCheckFilterNode*>(m_pOptionFilterList->FindNode(eFILTER_STONE_PURPLE));
    value = pNode->GetDown();
    //DBO_WARNING_MESSAGE(("Saving dSTORAGE_FILTER_STONE_PURPLE: " + std::to_string(value)).c_str());
    GetNtlStorageManager()->SetData(dSTORAGE_FILTER_STONE_PURPLE, value);

    pNode = static_cast<CCheckFilterNode*>(m_pOptionFilterList->FindNode(eFILTER_STONE_WHITE));
    value = pNode->GetDown();
    //DBO_WARNING_MESSAGE(("Saving dSTORAGE_FILTER_STONE_WHITE: " + std::to_string(value)).c_str());
    GetNtlStorageManager()->SetData(dSTORAGE_FILTER_STONE_WHITE, value);

    pNode = static_cast<CCheckFilterNode*>(m_pOptionFilterList->FindNode(eFILTER_STONE_ADOWN));
    value = pNode->GetDown();
    //DBO_WARNING_MESSAGE(("Saving dSTORAGE_FILTER_STONE_ADOWN: " + std::to_string(value)).c_str());
    GetNtlStorageManager()->SetData(dSTORAGE_FILTER_STONE_ADOWN, value);

    pNode = static_cast<CCheckFilterNode*>(m_pOptionFilterList->FindNode(eFILTER_STONE_WDOWN));
    value = pNode->GetDown();
    //DBO_WARNING_MESSAGE(("Saving dSTORAGE_FILTER_STONE_WDOWN: " + std::to_string(value)).c_str());
    GetNtlStorageManager()->SetData(dSTORAGE_FILTER_STONE_WDOWN, value);


    // Category Misc (mystery items, LP and EP recovery items)
    pNode = static_cast<CCheckFilterNode*>(m_pOptionFilterList->FindNode(eFILTER_MYSTERYITEM));
    value = pNode->GetDown();
    //DBO_WARNING_MESSAGE(("Saving dSTORAGE_FILTER_MYSTERYITEM: " + std::to_string(value)).c_str());
    GetNtlStorageManager()->SetData(dSTORAGE_FILTER_MYSTERYITEM, value);

    pNode = static_cast<CCheckFilterNode*>(m_pOptionFilterList->FindNode(eFILTER_CONSUMABLE));
    value = pNode->GetDown();
    //DBO_WARNING_MESSAGE(("Saving dSTORAGE_FILTER_CONSUMABLE: " + std::to_string(value)).c_str());
    GetNtlStorageManager()->SetData(dSTORAGE_FILTER_CONSUMABLE, value);

}


VOID COptionFilter::DefaultOption()
{
    // Category Rarity
    CCheckFilterNode* pNode = static_cast<CCheckFilterNode*>(m_pOptionFilterList->FindNode(eFILTER_RARITY_NORMAL));
    pNode->SetDown(GetNtlStorageMTContainer()->GetDefaultBool(dSTORAGE_FILTER_RARITY_NORMAL));

    pNode = static_cast<CCheckFilterNode*>(m_pOptionFilterList->FindNode(eFILTER_RARITY_SUPERIOR));
    pNode->SetDown(GetNtlStorageMTContainer()->GetDefaultBool(dSTORAGE_FILTER_RARITY_SUPERIOR));

    pNode = static_cast<CCheckFilterNode*>(m_pOptionFilterList->FindNode(eFILTER_RARITY_EXCELLENT));
    pNode->SetDown(GetNtlStorageMTContainer()->GetDefaultBool(dSTORAGE_FILTER_RARITY_EXCELLENT));

    pNode = static_cast<CCheckFilterNode*>(m_pOptionFilterList->FindNode(eFILTER_RARITY_RARE));
    pNode->SetDown(GetNtlStorageMTContainer()->GetDefaultBool(dSTORAGE_FILTER_RARITY_RARE));

    pNode = static_cast<CCheckFilterNode*>(m_pOptionFilterList->FindNode(eFILTER_RARITY_LEGENDARY));
    pNode->SetDown(GetNtlStorageMTContainer()->GetDefaultBool(dSTORAGE_FILTER_RARITY_LEGENDARY));


    // Category Stones
    pNode = static_cast<CCheckFilterNode*>(m_pOptionFilterList->FindNode(eFILTER_STONE_BLUE));
    pNode->SetDown(GetNtlStorageMTContainer()->GetDefaultBool(dSTORAGE_FILTER_STONE_BLUE));

    pNode = static_cast<CCheckFilterNode*>(m_pOptionFilterList->FindNode(eFILTER_STONE_RED));
    pNode->SetDown(GetNtlStorageMTContainer()->GetDefaultBool(dSTORAGE_FILTER_STONE_RED));

    pNode = static_cast<CCheckFilterNode*>(m_pOptionFilterList->FindNode(eFILTER_STONE_GREEN));
    pNode->SetDown(GetNtlStorageMTContainer()->GetDefaultBool(dSTORAGE_FILTER_STONE_GREEN));

    pNode = static_cast<CCheckFilterNode*>(m_pOptionFilterList->FindNode(eFILTER_STONE_PURPLE));
    pNode->SetDown(GetNtlStorageMTContainer()->GetDefaultBool(dSTORAGE_FILTER_STONE_PURPLE));

    pNode = static_cast<CCheckFilterNode*>(m_pOptionFilterList->FindNode(eFILTER_STONE_WHITE));
    pNode->SetDown(GetNtlStorageMTContainer()->GetDefaultBool(dSTORAGE_FILTER_STONE_WHITE));

    pNode = static_cast<CCheckFilterNode*>(m_pOptionFilterList->FindNode(eFILTER_STONE_ADOWN));
    pNode->SetDown(GetNtlStorageMTContainer()->GetDefaultBool(dSTORAGE_FILTER_STONE_ADOWN));

    pNode = static_cast<CCheckFilterNode*>(m_pOptionFilterList->FindNode(eFILTER_STONE_WDOWN));
    pNode->SetDown(GetNtlStorageMTContainer()->GetDefaultBool(dSTORAGE_FILTER_STONE_WDOWN));

    // Category Misc (mystery items, LP and EP recovery items)
    pNode = static_cast<CCheckFilterNode*>(m_pOptionFilterList->FindNode(eFILTER_MYSTERYITEM));
    pNode->SetDown(GetNtlStorageMTContainer()->GetDefaultBool(dSTORAGE_FILTER_MYSTERYITEM));

    pNode = static_cast<CCheckFilterNode*>(m_pOptionFilterList->FindNode(eFILTER_CONSUMABLE));
    pNode->SetDown(GetNtlStorageMTContainer()->GetDefaultBool(dSTORAGE_FILTER_CONSUMABLE));

}