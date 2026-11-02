// MPPConfigSetting.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MrPoPo.h"
#include "MPPConfigSetting.h"
#include "MPPDef.h"
#include "MPPLogView.h"

#define IDC_PROPERTY_GRID               0x6100
#define CONFIG_MARGIN                   6

// CMPPConfigSetting

IMPLEMENT_DYNCREATE(CMPPConfigSetting, CFormView)

CMPPConfigSetting::CMPPConfigSetting()
	: CFormView(CMPPConfigSetting::IDD)
	, m_pFileLoad(NULL)
	, m_pOpIp(NULL)
	, m_pOpPort(NULL)
	, m_pBugTrapIp(NULL)
	, m_pBugTrapPort(NULL)
	, m_pBugTrapFolder(NULL)
	, m_pLocalize(NULL)
{
    // the values of the ConfigOptions.xml the client ships with
    m_configData.strAddr = L"127.0.0.1";
    m_configData.dwPort = 20200;
    m_configData.strBugTrapServerIP = L"127.0.0.1";
    m_configData.dwBugTrapServerPort = 9999;
    m_configData.strLocalize = L"english";
}

CMPPConfigSetting::~CMPPConfigSetting()
{
}

void CMPPConfigSetting::DoDataExchange(CDataExchange* pDX)
{
    CFormView::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_BTN_SAVEXML, m_btnSaveXML);
}

BEGIN_MESSAGE_MAP(CMPPConfigSetting, CFormView)
    ON_REGISTERED_MESSAGE(AFX_WM_PROPERTY_CHANGED, &CMPPConfigSetting::OnPropertyChanged)
    ON_BN_CLICKED(IDC_BTN_SAVEXML, &CMPPConfigSetting::OnBnClickedBtnSavexml)
    ON_WM_SIZE()
END_MESSAGE_MAP()


// CMPPConfigSetting 진단입니다.

#ifdef _DEBUG
void CMPPConfigSetting::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CMPPConfigSetting::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMPPConfigSetting 메시지 처리기입니다.

void CMPPConfigSetting::OnInitialUpdate()
{
    CFormView::OnInitialUpdate();

    // the grid follows the view size, no scroll bars
    SetScrollSizes(MM_TEXT, CSize(1, 1));

    if(!m_property.Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER, CRect(0, 0, 0, 0), this, IDC_PROPERTY_GRID))
        return;

    m_property.EnableHeaderCtrl(FALSE);
    m_property.EnableDescriptionArea(FALSE);
    m_property.SetVSDotNetLook(TRUE);

    CMFCPropertyGridProperty* pCatConfig = new CMFCPropertyGridProperty(L"DBO Config Options");

    m_pFileLoad = new CMFCPropertyGridFileProperty(L"Load Config File", TRUE, L"", L"xml", OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, FILTER_CONFIG, NULL, ID_FILE_LOAD);
    pCatConfig->AddSubItem(m_pFileLoad);

    m_pOpIp             = AddTextItem(pCatConfig, L"Server IP", ID_OP_IP);
    m_pOpPort           = AddTextItem(pCatConfig, L"Server Port", ID_OP_PORT);
    m_pBugTrapIp        = AddTextItem(pCatConfig, L"BugTrap Server IP", ID_BUGTRAP_IP);
    m_pBugTrapPort      = AddTextItem(pCatConfig, L"BugTrap Server Port", ID_BUGTRAP_PORT);
    m_pBugTrapFolder    = AddTextItem(pCatConfig, L"BugTrap Folder", ID_BUGTRAP_FOLDER);
    m_pLocalize         = AddTextItem(pCatConfig, L"Localize Folder", ID_LOCALIZE);

    m_property.AddProperty(pCatConfig);
    pCatConfig->Expand();

    UpdateConfigData();
    LayoutControls();
}

CMFCPropertyGridProperty* CMPPConfigSetting::AddTextItem(CMFCPropertyGridProperty* pGroup, LPCTSTR szName, EItemID eID)
{
    CMFCPropertyGridProperty* pItem = new CMFCPropertyGridProperty(szName, (_variant_t)L"", NULL, eID);
    pGroup->AddSubItem(pItem);
    return pItem;
}

void CMPPConfigSetting::LayoutControls()
{
    if(m_property.GetSafeHwnd() == NULL || m_btnSaveXML.GetSafeHwnd() == NULL)
        return;

    CRect rcClient;
    GetClientRect(&rcClient);

    CRect rcButton;
    m_btnSaveXML.GetWindowRect(&rcButton);

    // the save button under the grid, the grid takes the rest
    int nButtonY = rcClient.bottom - CONFIG_MARGIN - rcButton.Height();
    m_btnSaveXML.SetWindowPos(NULL, CONFIG_MARGIN, nButtonY, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

    m_property.MoveWindow(0, 0, rcClient.Width(), max(nButtonY - CONFIG_MARGIN, 0));
}

void CMPPConfigSetting::OnSize(UINT nType, int cx, int cy)
{
    CFormView::OnSize(nType, cx, cy);

    LayoutControls();
}

LRESULT CMPPConfigSetting::OnPropertyChanged(WPARAM wParam, LPARAM lParam)
{
    USES_CONVERSION;

    CMFCPropertyGridProperty* pItem = (CMFCPropertyGridProperty*)lParam;
    if(pItem == NULL)
        return 0;

    CString strValue = pItem->GetValue().vt == VT_BSTR ? CString(pItem->GetValue().bstrVal) : CString();

    switch(pItem->GetData())
    {
    case ID_FILE_LOAD:
        if(CNtlScriptEncrypter::LoadConfigOption(&m_configData, W2A(strValue)))
        {
            UpdateConfigData();

            CString strLog;
            strLog = L"Success Config File Loading - ";
            strLog += strValue;
            AddLogView(strLog);
        }
        else
        {
            MessageBox(L"Config File Load Fail!", NULL, MB_ICONERROR);
        }
        break;
    case ID_OP_IP:
        m_configData.strAddr = strValue;
        break;
    case ID_OP_PORT:
        m_configData.dwPort = (DWORD)_wtoi(strValue);
        break;
    case ID_BUGTRAP_IP:
        m_configData.strBugTrapServerIP = strValue;
        break;
    case ID_BUGTRAP_PORT:
        m_configData.dwBugTrapServerPort = (DWORD)_wtoi(strValue);
        break;
    case ID_BUGTRAP_FOLDER:
        m_configData.strBugTrapFolder = strValue;
        break;
    case ID_LOCALIZE:
        m_configData.strLocalize = strValue;
        break;
    }

    return 0;
}

void CMPPConfigSetting::UpdateConfigData()
{
    WCHAR buf[64] = {0,};

    m_pOpIp->SetValue((_variant_t)(LPCTSTR)m_configData.strAddr);
    swprintf_s(buf, L"%u", m_configData.dwPort);
    m_pOpPort->SetValue((_variant_t)buf);

    m_pBugTrapIp->SetValue((_variant_t)(LPCTSTR)m_configData.strBugTrapServerIP);
    swprintf_s(buf, L"%u", m_configData.dwBugTrapServerPort);
    m_pBugTrapPort->SetValue((_variant_t)buf);
    m_pBugTrapFolder->SetValue((_variant_t)(LPCTSTR)m_configData.strBugTrapFolder);

    m_pLocalize->SetValue((_variant_t)(LPCTSTR)m_configData.strLocalize);
}

void CMPPConfigSetting::OnBnClickedBtnSavexml()
{
    USES_CONVERSION;

    CFileDialog dlg(FALSE, L"xml", L"ConfigOptions.xml", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, FILTER_CONFIG, this);
    if(dlg.DoModal() != IDOK)
        return;

    if(CNtlScriptEncrypter::SaveConfigOption(&m_configData, W2A(dlg.GetPathName())))
    {
        CString strLog;
        strLog = L"Success Config File Save - ";
        strLog += dlg.GetPathName();
        AddLogView(strLog);
    }
    else
    {
        MessageBox(L"Config File Save Fail!", NULL, MB_ICONERROR);
    }
}
