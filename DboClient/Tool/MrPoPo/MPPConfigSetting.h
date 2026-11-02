#pragma once
#include "afxwin.h"
#include "NtlScriptEncrypter.h"

enum EItemID
{
    ID_FILE_LOAD = 1,
    ID_OP_IP,
    ID_OP_PORT,
    ID_BUGTRAP_IP,
    ID_BUGTRAP_PORT,
    ID_BUGTRAP_FOLDER,
    ID_LOCALIZE,
};

/**
 * \ingroup MrPoPo
 * \brief   Config 파일 설정 뷰 클래스 (edits the client ConfigOptions.xml)
 *
 * \date 2008-07-31
 * \author agebreak
 */
class CMPPConfigSetting : public CFormView
{
	DECLARE_DYNCREATE(CMPPConfigSetting)

protected:
	CMPPConfigSetting();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CMPPConfigSetting();

public:
	enum { IDD = IDD_MPPCONFIGSETTING };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

public:
    virtual void OnInitialUpdate();
    afx_msg void OnBnClickedBtnSavexml();

protected:
    DECLARE_MESSAGE_MAP()
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

    afx_msg LRESULT OnPropertyChanged(WPARAM wParam, LPARAM lParam);
    afx_msg void OnSize(UINT nType, int cx, int cy);

    void    UpdateConfigData();                         ///< 컨피그 데이터를 업데이트합니다.
    void    LayoutControls();

    CMFCPropertyGridProperty*   AddTextItem(CMFCPropertyGridProperty* pGroup, LPCTSTR szName, EItemID eID);

protected:
    CButton m_btnSaveXML;

    CMFCPropertyGridCtrl            m_property;
    CMFCPropertyGridFileProperty*   m_pFileLoad;
    CMFCPropertyGridProperty*       m_pOpIp;
    CMFCPropertyGridProperty*       m_pOpPort;
    CMFCPropertyGridProperty*       m_pBugTrapIp;
    CMFCPropertyGridProperty*       m_pBugTrapPort;
    CMFCPropertyGridProperty*       m_pBugTrapFolder;
    CMFCPropertyGridProperty*       m_pLocalize;

    SConfigData             m_configData;               ///< 컨피그 정보 데이터
};
