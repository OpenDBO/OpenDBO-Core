// MrPoPoView.cpp : implementation of the CMrPoPoView class
//

#include "stdafx.h"
#include "MrPoPo.h"

#include "MrPoPoDoc.h"
#include "MrPoPoView.h"
#include "MPPDef.h"
#include "MPPConfigSetting.h"
#include "MPPSetItemPane.h"
#include "MPPTestPane.h"
#include "MPPClientCheat.h"
#include "MPPGraphicPane.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define IDC_TAB_PANES		0x6000
#define IDC_PANE_FIRST		0x6001


// CMrPoPoView

IMPLEMENT_DYNCREATE(CMrPoPoView, CView)

BEGIN_MESSAGE_MAP(CMrPoPoView, CView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_PANES, &CMrPoPoView::OnTabSelChange)
END_MESSAGE_MAP()

// CMrPoPoView construction/destruction

CMrPoPoView::CMrPoPoView()
{
}

CMrPoPoView::~CMrPoPoView()
{
}

BOOL CMrPoPoView::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style |= WS_CLIPCHILDREN;

	return CView::PreCreateWindow(cs);
}

int CMrPoPoView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_tabPanes.Create(WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | TCS_TABS | TCS_MULTILINE, CRect(0, 0, 0, 0), this, IDC_TAB_PANES))
		return -1;

	m_tabPanes.SetFont(CFont::FromHandle((HFONT)GetStockObject(DEFAULT_GUI_FONT)));

	// the panes the docking panes used to show, in the same order
	AddPane(RUNTIME_CLASS(CMPPConfigSetting), IDR_PANE_CONFIG);
#ifndef ONLY_GRAPHIC_PANE
	AddPane(RUNTIME_CLASS(CMPPSetItemPane), IDR_PANE_SETITEM);
	AddPane(RUNTIME_CLASS(CMPPTestPane), IDR_PANE_TEST);
	AddPane(RUNTIME_CLASS(CMPPClientCheat), IDR_PANE_CHEAT);
#endif
	AddPane(RUNTIME_CLASS(CMPPGraphicPane), IDR_PANE_GRAPHIC);

	// the frame is still being created, it activates its view itself
	ShowPane(0, FALSE);
	return 0;
}

void CMrPoPoView::AddPane(CRuntimeClass* pViewClass, UINT nCaptionID)
{
	CView* pView = (CView*)pViewClass->CreateObject();
	if (pView == NULL)
		return;

	// created like a frame's view, so it gets the document and OnInitialUpdate
	CCreateContext context;
	context.m_pCurrentDoc = GetDocument();
	context.m_pCurrentFrame = GetParentFrame();

	UINT nID = IDC_PANE_FIRST + (UINT)m_vecPanes.size();
	if (!pView->Create(NULL, NULL, WS_CHILD, CRect(0, 0, 0, 0), this, nID, &context))
		return;		// a view that failed to create deletes itself

	CString strCaption;
	strCaption.LoadString(nCaptionID);

	m_tabPanes.InsertItem((int)m_vecPanes.size(), strCaption);
	m_vecPanes.push_back(pView);
}

void CMrPoPoView::ShowPane(int nIndex, BOOL bActivate)
{
	for (int i = 0; i < (int)m_vecPanes.size(); ++i)
	{
		if (i == nIndex)
		{
			m_vecPanes[i]->SetWindowPos(&CWnd::wndTop, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);

			if (bActivate)
				GetParentFrame()->SetActiveView(m_vecPanes[i]);
		}
		else
		{
			m_vecPanes[i]->ShowWindow(SW_HIDE);
		}
	}

	m_tabPanes.SetCurSel(nIndex);
}

void CMrPoPoView::LayoutPanes()
{
	if (m_tabPanes.GetSafeHwnd() == NULL)
		return;

	CRect rcClient;
	GetClientRect(&rcClient);
	m_tabPanes.MoveWindow(&rcClient);

	CRect rcPane = rcClient;
	m_tabPanes.AdjustRect(FALSE, &rcPane);

	for (CView* pView : m_vecPanes)
		pView->MoveWindow(&rcPane);
}

void CMrPoPoView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	LayoutPanes();
}

BOOL CMrPoPoView::OnEraseBkgnd(CDC* pDC)
{
	// the tab control and the panes cover the whole view
	return TRUE;
}

void CMrPoPoView::OnTabSelChange(NMHDR* pNMHDR, LRESULT* pResult)
{
	ShowPane(m_tabPanes.GetCurSel());
	*pResult = 0;
}

// CMrPoPoView drawing

void CMrPoPoView::OnDraw(CDC* /*pDC*/)
{
}


// CMrPoPoView diagnostics

#ifdef _DEBUG
void CMrPoPoView::AssertValid() const
{
	CView::AssertValid();
}

void CMrPoPoView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMrPoPoDoc* CMrPoPoView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMrPoPoDoc)));
	return (CMrPoPoDoc*)m_pDocument;
}
#endif //_DEBUG
