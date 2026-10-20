#include "stdafx.h"
#include "BitmapSlider.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CBitmapSlider::CBitmapSlider( void )
{
}

CBitmapSlider::~CBitmapSlider( void )
{
	m_bmChannel.DeleteObject();
	m_bmThumb.DeleteObject();
}

void CBitmapSlider::InitSlider( UINT uiThumbBitmap, UINT uiChannelBitmap )
{
	AttachParent( GetSafeHwnd() );

	m_bmChannel.DeleteObject();
	m_bmThumb.DeleteObject();

	m_bmChannel.LoadBitmap( uiChannelBitmap );
	m_bmThumb.LoadBitmap( uiThumbBitmap );
}

BOOL CBitmapSlider::DrawThumb( CDC* pDC, const CRect& rcThumb, UINT uiItemState )
{
	// The retail code has separate branches for CDIS_SELECTED and CDIS_HOT,
	// but all of them draw the same thumb bitmap.
	CDC memDC;
	memDC.CreateCompatibleDC( pDC );

	CBitmap* pOldBitmap = memDC.SelectObject( &m_bmThumb );

	BITMAP sBitmap;
	m_bmThumb.GetBitmap( &sBitmap );

	pDC->BitBlt( rcThumb.left, rcThumb.top, sBitmap.bmWidth, sBitmap.bmHeight, &memDC, 0, 0, SRCCOPY );

	memDC.SelectObject( pOldBitmap );

	return TRUE;
}

BOOL CBitmapSlider::DrawChannel( CDC* pDC, const CRect& rcClient )
{
	if ( m_hParentWnd )
	{
		DrawParentBackground( GetSafeHwnd(), pDC->GetSafeHdc() );
	}

	CDC memDC;
	memDC.CreateCompatibleDC( pDC );

	CBitmap* pOldBitmap = memDC.SelectObject( &m_bmChannel );

	BITMAP sBitmap;
	m_bmChannel.GetBitmap( &sBitmap );

	pDC->BitBlt( rcClient.left, rcClient.top, rcClient.Width(), rcClient.Height(), &memDC, 0, 0, SRCCOPY );

	memDC.SelectObject( pOldBitmap );

	return TRUE;
}

void CBitmapSlider::OnCustomDraw( NMHDR* pNMHDR, LRESULT* pResult )
{
	NMCUSTOMDRAW* pNMCD = reinterpret_cast<NMCUSTOMDRAW*>( pNMHDR );

	*pResult = CDRF_DODEFAULT;

	CDC* pDC = CDC::FromHandle( pNMCD->hdc );
	CRect rcItem( pNMCD->rc );

	switch ( pNMCD->dwDrawStage )
	{
	case CDDS_PREPAINT:
		{
			*pResult = CDRF_NOTIFYITEMDRAW;
		}
		break;

	case CDDS_ITEMPREPAINT:
		{
			switch ( pNMCD->dwItemSpec )
			{
			case TBCD_THUMB:
				{
					if ( DrawThumb( pDC, rcItem, pNMCD->uItemState ) )
					{
						*pResult = CDRF_SKIPDEFAULT;
					}
				}
				break;

			case TBCD_CHANNEL:
				{
					CRect rcClient;
					GetClientRect( &rcClient );

					if ( DrawChannel( pDC, rcClient ) )
					{
						*pResult = CDRF_SKIPDEFAULT;
					}
				}
				break;

			case TBCD_TICS:
				{
					*pResult = CDRF_SKIPDEFAULT;
				}
				break;
			}
		}
		break;
	}
}

LRESULT CBitmapSlider::OnRedraw( WPARAM wParam, LPARAM lParam )
{
	// Toggling the enable state twice forces the slider to repaint itself
	BOOL bEnabled = ::IsWindowEnabled( GetSafeHwnd() );

	::EnableWindow( GetSafeHwnd(), !bEnabled );
	::EnableWindow( GetSafeHwnd(), bEnabled );

	return 1;
}

BOOL CBitmapSlider::OnEraseBkgnd( CDC* pDC )
{
	return TRUE;
}

BEGIN_MESSAGE_MAP( CBitmapSlider, CSliderCtrl )
	ON_NOTIFY_REFLECT( NM_CUSTOMDRAW, &CBitmapSlider::OnCustomDraw )
	ON_MESSAGE( WM_BITMAPSLIDER_REDRAW, &CBitmapSlider::OnRedraw )
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()
