/*****************************************************************************
*
* File			: OutputBox.h
* Author		:
* Copyright		: (주)NTL
* Date			:
* Abstract		: 
*****************************************************************************
* Desc          :
*
*****************************************************************************/


#pragma once


//////////////////////////////////////////////////////////////////////////
//
// COutputBox
//
//////////////////////////////////////////////////////////////////////////


class COutputBox : public CStatic
{
public:
	COLORREF			m_clrText;			// 글씨 색 (default white)

public:
	COutputBox( void );
    virtual ~COutputBox( void );

public:
    void				EraseBack( CDialog* pDlg );		// 글씨영역을 지운다.

protected:
	afx_msg HBRUSH		CtlColor( CDC* pDC, UINT nCtlColor );

	DECLARE_MESSAGE_MAP()
};
