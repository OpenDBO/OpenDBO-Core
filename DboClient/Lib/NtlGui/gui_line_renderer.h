/*****************************************************************************
* File			: gui_line_renderer.h
* Author		: HaeSung, Cho
* Copyright		: (주)NTL
* Date			: 2008. 11. 10
* Abstract		: 
*****************************************************************************
* Desc          : 
* 툴에서 쓰려고 만듬. client 용으로 쓸려면 진지하게 고려해보고 작업을 하여 써야함
* 현재 있는데로 그대로 쓰면 엄청 느림...
*****************************************************************************/

#ifndef __GUI_LINE_RENDERER_H__
#define __GUI_LINE_RENDERER_H__

START_GUI

#define GUI_UV_START	0.01f
#define GUI_UV_END		0.99f

struct SLineItem
{
	float		x;
	float		y;

	SLineItem()
	{
		x = 0; y = 0;
	}
};

/**
* \brief 이어지는 선을 그릴 수 있다.
*/
class CGuiLine
{
public:
	CGuiLine();
	~CGuiLine();

	// Particle Editor 툴 용
	VOID SetLine( SLineItem* pLineItem, int nItemNum );
	VOID SetLine( RwReal fPosX1, RwReal fPosY1, RwReal fPosX2, RwReal fPosY2 );
	VOID SetLineAt( RwUInt32 uiIndex, RwReal fPosX, RwReal fPosY );

	VOID SetColor( COLORREF color );
	VOID SetAlpha( RwUInt8 byAlpha );
	VOID Destroy();

	VOID Render(VOID);

	VOID SetLineMove( RwReal fX, RwReal fY );

protected:
	RwIm2DVertex*	m_p2dVertices;
	int				m_nVerticesNum;
};

END_GUI

#endif