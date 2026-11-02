// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "MrPoPo.h"

#include "MainFrm.h"
#include "MrPoPoDoc.h"
#include "MrPoPoView.h"
#include "MPPLogView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// height of the log view under the pane tabs
#define LOGVIEW_HEIGHT		150


// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
}

CMainFrame::~CMainFrame()
{
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	return 0;
}

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	if (!m_wndSplitter.CreateStatic(this, 2, 1))
		return FALSE;

	int nPaneHeight = max(lpcs->cy - LOGVIEW_HEIGHT, LOGVIEW_HEIGHT);

	if (!m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(CMrPoPoView), CSize(0, nPaneHeight), pContext) ||
		!m_wndSplitter.CreateView(1, 0, RUNTIME_CLASS(CMPPLogView), CSize(0, LOGVIEW_HEIGHT), pContext))
	{
		m_wndSplitter.DestroyWindow();
		return FALSE;
	}

	SetActiveView((CView*)m_wndSplitter.GetPane(0, 0));
	return TRUE;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;

	cs.style |= WS_CLIPCHILDREN|WS_CLIPSIBLINGS;

	// the title is just the app name, no document name
	cs.style &= ~FWS_ADDTOTITLE;
	return TRUE;
}


// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG
