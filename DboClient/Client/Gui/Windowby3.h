/******************************************************************************
* File			: CWindowby3.h
* Author		: Hong SungBock
* Copyright		: (��)NTL
* Date			: 2006. 12. 8
* Abstract		: 
* Update		: 
*****************************************************************************
* Desc			: ū �������� ���ҽ��� ���� Ȥ�� �������� 3����� ���� ǥ���Ѵ�
*				  ��ũ��Ʈ�� �̸� ����� �������� �ʾҴٸ� SetRect, SetRectWH,
*				  SetSize���� ȣ���� �ڿ� SetPositionfromParent�Լ��� ȣ������
*****************************************************************************/

#pragma once

#include "gui_Surface.h"
#include "SurfaceGui.h"

class CWindowby3
{
public:
	enum
	{
		WINDOWS_NUM = 3
	};

	enum eWinType
	{
		WT_VERTICAL,
		WT_HORIZONTAL
	};

	CWindowby3() : m_byWinType(WT_VERTICAL), m_bSetSize(false) {}
	CWindowby3(const CWindowby3& windows) {}
	virtual ~CWindowby3() {}

	VOID			SetType(RwUInt8 byWinType);

	VOID			SetRect(const CRectangle& rect);
	VOID			SetRectWH(RwInt32 iPosX, RwInt32 iPosY, RwInt32 iWidth, RwInt32 iHeight);
	VOID			SetSize(RwUInt32 uiWidth, RwUInt32 uiHeight);
	VOID			SetAlpha(RwUInt8 byAlpha);
	VOID			SetSurface(RwInt8 byIndex, gui::CSurface& surface);	///< �����̽� ���
	VOID			SetPosition(RwInt32 iPosX, RwInt32 iPosY);			///< �������� ���¸� ������ü �ű��
	VOID			SetPositionfromParent(RwInt32 iPosX, RwInt32 iPosY);
	VOID			SetPositionbyParent(RwInt32 iParentPosX, RwInt32 iParentPosY);

	RwInt32			GetPositionX();
	RwInt32			GetPositionY();
	RwInt32			GetWidth();
	RwInt32			GetHeight();
	CPos			GetPosfromParent();

	VOID			Show(bool bShow);
	VOID			Render(bool bTopRender = false);
	VOID			SetClippingRect(CRectangle* parentRtClipping);

	RwBool			PtInRect(int iPosX, int iPosY);

protected:
	CSurfaceGui			m_Window[WINDOWS_NUM];
	RwUInt8				m_byWinType;
	RwBool				m_bSetSize;
};