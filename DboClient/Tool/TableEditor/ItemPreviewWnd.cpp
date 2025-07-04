//Cpp file for the CItemPreviewWnd class, which is a dockable pane that displays an item preview image.
// It includes methods to create the pane, handle resizing, and set an icon image.

#include "pch.h"
#include "ItemPreviewWnd.h"


BEGIN_MESSAGE_MAP(CItemPreviewWnd, CDockablePane)
    ON_WM_CREATE()
    ON_WM_SIZE()
END_MESSAGE_MAP()


BOOL CItemPreviewWnd::PreCreateWindow(CREATESTRUCT& cs)
{
    cs.dwExStyle |= WS_EX_COMPOSITED; // Reduz flickering/rastro
    return CDockablePane::PreCreateWindow(cs);
}

CItemPreviewWnd::CItemPreviewWnd()

{

}

CItemPreviewWnd::~CItemPreviewWnd()
{
    m_image.Destroy();
}

int CItemPreviewWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CDockablePane::OnCreate(lpCreateStruct) == -1)
        return -1;

    if (!m_wndImage.Create(_T(""), WS_CHILD | WS_VISIBLE | SS_BITMAP, CRect(0, 0, 100, 100), this))
        return -1;

    return 0;
}

void CItemPreviewWnd::OnSize(UINT nType, int cx, int cy)
{
    CDockablePane::OnSize(nType, cx, cy);
    m_wndImage.SetWindowPos(nullptr, 0, 0, cx, cy, SWP_NOZORDER);
}

void CItemPreviewWnd::SetIconImage(const CString& strIconName)
{
    CString strPath = _T("./texture/gui/icon/");
    strPath += strIconName;

    m_image.Destroy();

    if (m_image.Load(strPath) == S_OK)
    {
        CRect rect;
        m_wndImage.GetClientRect(&rect);

        // Criar uma imagem esticada do mesmo tamanho do controle
        CImage stretchedImage;
        stretchedImage.Create(rect.Width(), rect.Height(), m_image.GetBPP());

        // Esticar a imagem original para preencher a nova
        m_image.StretchBlt(
            stretchedImage.GetDC(),
            0, 0, rect.Width(), rect.Height(),
            SRCCOPY
        );
        stretchedImage.ReleaseDC();

        // Atualizar o controle
        m_wndImage.SetBitmap((HBITMAP)stretchedImage.Detach());

        // Destruir imagem original, se quiser liberar
        m_image.Destroy();
    }
    else
    {
        m_wndImage.SetWindowText(_T("Image not found"));
    }
}