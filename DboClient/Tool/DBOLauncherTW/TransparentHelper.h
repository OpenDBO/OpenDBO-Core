/*****************************************************************************
*
* File			: TransparentHelper.h
* Abstract		: Lets a child control draw the part of its parent's
*                 background that lies behind it, so it can look transparent
*                 on top of a bitmap dialog.
*
*                 The parent is tagged with a window property. When a control
*                 needs its background it sends WM_TRANSPARENT_BK to the
*                 parent; a parent that does not answer it (returns 0) gets a
*                 WM_ERASEBKGND with the control's DC instead.
*
*****************************************************************************/


#pragma once


#define WM_TRANSPARENT_BK					( WM_USER + 0x3FD )		// 0x7FD
#define TRANSPARENT_CTRL_TAG				_T( "###tranparentctrl_tag###" )


class CTransparentHelper
{
protected:
	HWND					m_hParentWnd;

public:
	CTransparentHelper( void ) : m_hParentWnd( NULL ) { return; }
	virtual ~CTransparentHelper( void ) { return; }

public:
	virtual LRESULT			OnTransparentBk( HDC hDC, HWND hWnd, LPARAM lParam ) { return 0; }

	// Remembers the parent of hCtrlWnd and tags it as a transparent control host
	void					AttachParent( HWND hCtrlWnd );

	// Asks the parent to paint its background into hDC
	static void				DrawParentBackground( HWND hCtrlWnd, HDC hDC );

	HWND					GetParentWnd( void ) const { return m_hParentWnd; }
};
