/******************************************************************************
* File			: OptionFilter.h
* Author		: Santiago A.K.A Lothkru
* Date			: 2025. 5. 12
* Abstract		: Drop filter GUI for selecting item rarity
*****************************************************************************/
#ifndef _OPTIONFILTER_H_
#define _OPTIONFILTER_H_

#pragma once

#include "GuiLineTree.h"

// Option Filter List Size
#define dOPTIONFILTER_LIST_X			0
#define dOPTIONFILTER_LIST_Y			0
#define dOPTIONFILTER_LIST_WIDTH		288
#define dOPTIONFILTER_LIST_HEIGHT		304

// OptionFilterList ScrollBar Size
#define dOPTIONFILTER_LIST_SCROLL_WIDTH			12
#define dOPTIONFILTER_LIST_SCROLL_HEIGHT		19
#define dOPTIONFILTER_LIST_SLIDER_WIDTH			12
#define dOPTIONFILTER_LIST_SLIDER_HEIGHT		39

// CGuiLineTree Item Size
#define dOPTIONFILTER_LINE_HEIGHT				20
#define dOPTIONFILTER_LINE_MARGIN				4	
#define dOPTIONFILTER_CHILD_MARGIN_WIDTH		20

// Filter categories
#define dOPTIONFILTER_CATEGORY_RARITY			-2
#define dOPTIONFILTER_CATEGORY_STONES			-3
#define dOPTIONFILTER_CATEGORY_MISC             -4

enum eFILTEROPTIONID
{
    // Rarity
    eFILTER_RARITY_NORMAL = 0,
    eFILTER_RARITY_SUPERIOR,
    eFILTER_RARITY_EXCELLENT,
    eFILTER_RARITY_RARE,
    eFILTER_RARITY_LEGENDARY,

    // Stones
    eFILTER_STONE_BLUE,
    eFILTER_STONE_RED,
    eFILTER_STONE_GREEN,
    eFILTER_STONE_PURPLE,
    eFILTER_STONE_WHITE,
    eFILTER_STONE_ADOWN,
    eFILTER_STONE_WDOWN,
    eFILTER_MYSTERYITEM,
    eFILTER_CONSUMABLE,

    eFILTEROPTIONID_NUMS = eFILTER_CONSUMABLE,
    INVALID_FILTEROPTIONID = 0xFF
};

/**
* \ingroup client
*/
class COptionFilterCategoryNode : public CGuiLineTreeNode
{
public:
    COptionFilterCategoryNode(CGuiLineTree* pMgr, std::wstring strTitle, RwInt32 nID);
    virtual ~COptionFilterCategoryNode();

    virtual void ShowProc();				///< CGuiLineTreeNode override
    virtual void HideProc();				///< CGuiLineTreeNode override

    void OnClickBtnExpand(gui::CComponent* pComponent);		///< + button click handler
    void OnClickBtnReduce(gui::CComponent* pComponent);		///< - button click handler

protected:
    gui::CButton* m_pBtnExpand;				// + Button
    gui::CButton* m_pBtnReduce;				// - Button

    gui::CSlot m_slotClickedBtnExpand;		// + Button's signal
    gui::CSlot m_slotClickedBtnReduce;		// - Button's signal
};

/**
* \ingroup client
*/
class CCheckFilterNode : public CGuiLineTreeNode
{
public:
    CCheckFilterNode(CGuiLineTree* pMgr, RwInt32 nID, std::wstring strTitle, std::wstring strToolTip);
    virtual ~CCheckFilterNode();

    virtual void ShowProc();
    virtual void HideProc();

    void SetDown(RwBool bDown) { m_pBtnCheck->SetDown(B2b(bDown)); }
    void SetDown(bool bDown) { m_pBtnCheck->SetDown(bDown); }
    bool GetDown() { return m_pBtnCheck->IsDown(); }

    void OnToggledCheckFilter(gui::CComponent* pComponent, bool bDown);
protected:
    gui::CButton* m_pBtnCheck;
    gui::CStaticBox* m_pStbTitle;

    gui::CSlot		m_slotToggledBtnCheck;
};
/**
* \ingroup client
*/
class COptionFilterList : public CGuiLineTree
{
public:
    COptionFilterList();
    virtual ~COptionFilterList();

    RwBool	Create(CRectangle& rect, gui::CComponent* pParent, RwInt32 nLineHeight, RwInt32 nLineMargin, RwInt32 nChildXMargin, RwInt32 nScrollBarWidth, COptionFilter* pOptionFilter = NULL);

    void CreateTree();									///< Build the tree structure

    ///< Handle mouse wheel scrolling
    void OnCaptureWheelMove(RwInt32 nFlag, RwInt16 sDelta, CPos& pos);

protected:
    COptionFilter* m_pOptionFilter;

    gui::CSlot m_slotCaptureWheelMove;
};

/**
* \ingroup client
* COptionBase implementation for the Filter tab
*/
class COptionFilter : public COptionBase
{
public:
    COptionFilter();
    virtual ~COptionFilter();

    virtual RwBool		Create(COptionWindowGui* pOptionWindow);
    virtual void		Destroy();

    virtual void		Show();
    virtual void		Hide();

    virtual void		OnInit();
    virtual void		OnReset();
    virtual void		OnOk();
    virtual void		OnCancel();
    virtual void		OnHandleEvents(RWS::CMsg& pMsg);

    // Serialization
    VOID				LoadSerializer();
    VOID				SaveSerializer();
    VOID				DefaultOption();

protected:
    COptionFilterList* m_pOptionFilterList;		///< Main list
    gui::CDialog* m_pDlgFilterList;		///< Parent dialog
};

#endif