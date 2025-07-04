// ItemPreviewWnd.h : header file

#pragma once

class CItemPreviewWnd : public CDockablePane
{
public:
    CItemPreviewWnd();
    virtual ~CItemPreviewWnd();

    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

    void SetIconImage(const CString& strIconName);

protected:
    CStatic m_wndImage;
    CImage m_image;

    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    DECLARE_MESSAGE_MAP()
};
