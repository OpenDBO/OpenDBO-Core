/*****************************************************************************
 *
 * File			: NtlPLPlayerName.h
 * Author		: HyungSuk Jang
 * Copyright	: (주)NTL
 * Date			: 2005. 11. 24	
 * Abstract		: Presentation layer player name entity class
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __NTL_PLPLAYERNAME_H__
#define __NTL_PLPLAYERNAME_H__

#include "NtlPLAttach.h"
#include "NtlPLGlobal.h"
#include "NtlPLEmblemMaker.h"


#include "gui_font.h"
#include "gui_texture.h"

// Texture의 1 Size에 가장 뚜렷하게 보이는 폴리곤 사이즈
// Textrue Size에 곱해주면 가장 뚜렷하게 보이는 폴리곤 사이즈를 내놓는다.
// ex) 512 * PLAYERNAME_BOX_BASIS_CX = 2.0f
#define PLAYERNAME_BOX_BASIS_CX			0.00390625f
#define PLAYERNAME_BOX_BASIS_CY			0.00625f

#define PLAYERNAME_EMBLEM_BASIS_CX		0.005f
#define PLAYERNAME_EMBLEM_BASIS_CY		0.006f

#define PLAYERNAME_BOX_OFFSET			0.02f;
#define PLAYERNAME_NICK_OFFSET			0.08f;

#define PLAYERNAME_VERTEX_COUNT			4

// Player 이름을 출력 할 때 거리에 따라서 컬링한다.
#define PLAYERNAME_DIST							40.0f
#define PLAYERNAME_CAMERA__BOUNDING_RADIUS		0.1f

#define PLAYERNAME_LINE_INTERVAL_Y				5

// FONT SIZE
//#define dNTL_PLAYERNAME_FONTSIZE_NAME	230 
//#define dNTL_PLAYERNAME_FONTSIZE_GUILD	180
// modified by kell ( 2008. 7. 1 기획팀요청 )
#define dNTL_PLAYERNAME_FONTSIZE_NAME	200 
#define dNTL_PLAYERNAME_FONTSIZE_GUILD	170
#define dNTL_PLAYERNAME_FONTSIZE_NICK	190

// Flag
#define dNTL_PL_FLAG_NAME		0x00000001
#define dNTL_PL_FLAG_GUILD		0x00000002
#define dNTL_PL_FLAG_EMBLEM		0x00000004
#define dNTL_PL_FLAG_NICK		0x00000008

/**
* \brief PlayerNameEntity의 자료구조
*/
struct SPLPlayerNameCreateParam : public SPLEntityCreateParam
{
	const WCHAR		*pName;
	COLORREF		Color;
	RwReal			fCullDist;
	const WCHAR		*pGuildName;
	COLORREF		ColorGuild;
	sEmblemFactor	*pEmblemFactor;

	SPLPlayerNameCreateParam()
		:pName(NULL)
		,Color(RGB(255,255,255))
		,fCullDist(PLAYERNAME_DIST)
		,pGuildName(NULL)
		,ColorGuild(RGB(255,255,255))
		,pEmblemFactor(NULL)
	{
	}
};

/**
* \ingroup presentation
*/
class CNtlPLPlayerName : public CNtlPLAttach
{
public:
	CNtlPLPlayerName();
	virtual ~CNtlPLPlayerName();

	RwBool	Create( const SPLEntityCreateParam * pParam = NULL );
	void	Destroy(void);
	RwBool	Update(RwReal fElapsed);
	RwBool	Render(void);
	RwBool	SetProperty(const CNtlPLProperty *pData) { return TRUE;}

	void    SetAlpha(RwUInt8 byValue);
	__inline RwUInt8 GetAlpha() {return m_byAlpha;}

	void*	operator new(size_t size);
	void	operator delete(void *pObj);

	RwBool	IsExistEmblemTexture();

	void	SetVisible(RwBool bVisible);
	void	EnablePlayerNameVisible(RwBool bVisible);
	RwBool	IsEnablePlayerNameVisible();
	void	SetCameraCullin(RwBool bCullin);

protected:
	void	AllVertexInit(void);
	void	VertexAssign(void);

	void	NameVertexAssign(void);
	void	EmblemVertexAssign(void);
	void	NickNameVertexAssign(void);

	RwReal	GetAdjustPolygonWidth(RwInt32 nSize);
	RwReal	GetAdjustPolygonHeight(RwInt32 nSize);

	RwBool	CreateNameTexture(const WCHAR* pName);
	RwBool	CreateGuildNameTexture(const WCHAR* pGuildName);
	RwBool	CreateGuildEmblemTexture(sEmblemFactor* pEmblemFactor);
	RwBool	CreateNickNameTexture(const WCHAR* pNickName);

	RwBool	RenderName(void);
	RwBool	RenderEmblem(void);
	RwBool	RenderNick(void);

public:
	void	SetPosition(const RwV3d *pPos);
	RwV3d	GetPosition(void);

	void	SetNameColor(const WCHAR* pwcName, COLORREF color);
	void	SetNameColor(const WCHAR* pwcName, COLORREF color, const WCHAR* pwcGuildName, COLORREF guildcolor
		, sEmblemFactor* pEmblemFactor );
	void	SetTitleColor(const WCHAR* pwcTitle, COLORREF color );

protected:
	RwBool	m_bCameraCullIn;				///< 카메라에 컬링되는가
	RwReal	m_fCullDist;					///< 컬링 거리

	RwBool m_bNameVisible;					///< PlayerName의 Visible 을 할 수 있나 없나의 여부
	
	gui::CGuiFont *m_pFont;					///< Name Font
	gui::CGuiFont *m_pGuildFont;			///< Guild Name Font
	gui::CGuiFont *m_pNickFont;				///< Nick Font

	gui::CTexture *m_pTexture;				///< Name & Guild Texture 
	RwTexture *m_pEmblemTexture;			///< Emblem Texture
	gui::CTexture *m_pNickTexture;			///< Nick Texture

	CPos	m_posNameSize;					///< 이름의 픽셀 크기
	CPos	m_posGuildNameSize;				///< 길드 이름의 픽셀 크기
	CPos	m_posNickNameSize;				///< 닉의 픽셀 크기

	RwV3d			m_v3WorldPosition;		///< PlayerName Entity의 월드 포지션
	
	RwV3d			m_v3Pos[PLAYERNAME_VERTEX_COUNT];				///< 이름과 길드 이름의 로컬 좌표
	RwIm3DVertex	m_3dVertices[PLAYERNAME_VERTEX_COUNT];			///< 실제 오브젝트를 그릴 때 사용될 버텍스 버퍼

	RwV3d			m_v3EmblemPos[PLAYERNAME_VERTEX_COUNT];			///< 길드 엠블렘의 로컬 좌표
	RwIm3DVertex	m_3dEmblemVertices[PLAYERNAME_VERTEX_COUNT];	///< 엠블렘 오브젝트를 그릴 때 사용될 버텍스 버퍼

	RwV3d			m_v3NickPos[PLAYERNAME_VERTEX_COUNT];
	RwIm3DVertex	m_3dNickVertices[PLAYERNAME_VERTEX_COUNT];

	RwMatrix		m_matWorld;				///< 카메라의 역행렬과 스케일 값으로 구한 월드 행렬
    RwUInt8			m_byAlpha;				///< PlayerName의 Alpha

	RwUInt32		m_byFlag;				///< 출력되거나 로컬 좌표를 계산하는 상태 태그
};

#endif
