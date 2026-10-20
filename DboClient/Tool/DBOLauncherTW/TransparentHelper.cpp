#include "stdafx.h"
#include "TransparentHelper.h"


void CTransparentHelper::AttachParent( HWND hCtrlWnd )
{
	m_hParentWnd = ::GetParent( hCtrlWnd );

	if ( m_hParentWnd && NULL == ::GetProp( m_hParentWnd, TRANSPARENT_CTRL_TAG ) )
	{
		::SetProp( m_hParentWnd, TRANSPARENT_CTRL_TAG, (HANDLE)m_hParentWnd );
	}
}

void CTransparentHelper::DrawParentBackground( HWND hCtrlWnd, HDC hDC )
{
	ASSERT( hDC );
	ASSERT( hCtrlWnd );

	HWND hParent = ::GetParent( hCtrlWnd );

	if ( 0 == ::SendMessage( hParent, WM_TRANSPARENT_BK, (WPARAM)hDC, (LPARAM)hCtrlWnd ) )
	{
		::SendMessage( hParent, WM_ERASEBKGND, (WPARAM)hDC, 0 );
	}
}
