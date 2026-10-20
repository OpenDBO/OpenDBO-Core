/*****************************************************************************
*
* File			: BitmapSlider.h
* Abstract		: Owner drawn slider of the option dialog. The channel and the
*                 thumb are bitmaps; the area around the channel shows the
*                 parent dialog through CTransparentHelper.
*
*****************************************************************************/


#pragma once


#include "TransparentHelper.h"


#define WM_BITMAPSLIDER_REDRAW				( 0x1095 )


class CBitmapSlider : public CSliderCtrl, public CTransparentHelper
{
protected:
	CBitmap					m_bmThumb;
	CBitmap					m_bmChannel;

public:
	CBitmapSlider( void );
	virtual ~CBitmapSlider( void );

public:
	// Call after the control is created (DDX / OnInitDialog)
	void					InitSlider( UINT uiThumbBitmap, UINT uiChannelBitmap );

protected:
	BOOL					DrawThumb( CDC* pDC, const CRect& rcThumb, UINT uiItemState );
	BOOL					DrawChannel( CDC* pDC, const CRect& rcClient );

	afx_msg void			OnCustomDraw( NMHDR* pNMHDR, LRESULT* pResult );
	afx_msg LRESULT			OnRedraw( WPARAM wParam, LPARAM lParam );
	afx_msg BOOL			OnEraseBkgnd( CDC* pDC );

	DECLARE_MESSAGE_MAP()
};
