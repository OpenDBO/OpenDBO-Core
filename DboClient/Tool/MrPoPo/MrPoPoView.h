// MrPoPoView.h : interface of the CMrPoPoView class
//


#pragma once

#include <vector>

/**
 * \ingroup MrPoPo
 * \brief Hosts the tool panes as tabs (config, test, client cheat, graphic)
 */
class CMrPoPoView : public CView
{
protected: // create from serialization only
	CMrPoPoView();
	DECLARE_DYNCREATE(CMrPoPoView)

// Attributes
public:
	CMrPoPoDoc* GetDocument() const;

// Operations
public:

// Overrides
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// Implementation
public:
	virtual ~CMrPoPoView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	void	AddPane(CRuntimeClass* pViewClass, UINT nCaptionID);	///< creates a pane view and its tab
	void	ShowPane(int nIndex, BOOL bActivate = TRUE);
	void	LayoutPanes();

	CTabCtrl				m_tabPanes;
	std::vector<CView*>		m_vecPanes;		///< owned by their windows (views delete themselves)

// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTabSelChange(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in MrPoPoView.cpp
inline CMrPoPoDoc* CMrPoPoView::GetDocument() const
   { return reinterpret_cast<CMrPoPoDoc*>(m_pDocument); }
#endif
