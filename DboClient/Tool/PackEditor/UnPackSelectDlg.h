#pragma once
#include "afxwin.h"
#include "NtlPlResourcePack.h"


// CUnPackSelectDlg ��ȭ �����Դϴ�.

class CUnPackSelectDlg : public CDialog
{
	DECLARE_DYNAMIC(CUnPackSelectDlg)

public:
	CUnPackSelectDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CUnPackSelectDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_UNPACK_SELECT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
    CCheckListBox m_listPack;

    static int m_LoadPackType[MAX_NTL_PACK_TYPE];

public:    
    virtual BOOL OnInitDialog();
    afx_msg void OnBnClickedOk();
public:
    afx_msg void OnBnClickedBtAll();
};
