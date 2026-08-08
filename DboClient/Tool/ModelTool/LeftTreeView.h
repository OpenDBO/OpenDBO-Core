#pragma once
#include "afxcmn.h"

#include "LeftPropSheet.h"
#include "PagePC.h"
#include "PageNPC.h"
#include "PageMob.h"
#include "PageItem.h"
#include "PageGround.h"
#include "PageCLump.h"


// CLeftTreeView form view

class CLeftTreeView : public CFormView
{
	DECLARE_DYNCREATE(CLeftTreeView)

protected:
	CLeftTreeView();           // protected constructor used by dynamic creation
	virtual ~CLeftTreeView();

public:
	enum { IDD = IDD_LEFTTREEVIEW };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
public:
	virtual void OnInitialUpdate();

public:
	static CLeftTreeView* GetInstance() {return m_pInstance;};
protected:
	static CLeftTreeView* m_pInstance;	
public:
	CLeftPropSheet		  m_propSheet;					///< Property Sheet 객체
	CPagePC				  m_pagePC;						///< PC Property Page 객체
	CPageNPC			  m_pageNPC;					///< NPC Property Page 객체
	CPageMob			  m_pageMob;					///< Mob Property Page 객체
	CPageItem			  m_pageItem;					///< Item Property Page 객체
	CPageGround			  m_pageGround;					///< Ground Property Page 객체
	CPageClump			  m_pageClump;					///< Clump Property Page 객체
	
public:
    afx_msg void OnSize(UINT nType, int cx, int cy);
};


