//***********************************************************************************
//	File		:	NtlPostEffectCamera.h
//	Desc		:	
//	Begin		:	2006. 2.13
//	Copyright	:	Ã¯Â¿Â½Ã¯Â¿Â½ 2006 by agebreak CO., Ltd
//	Author		:	agebreak
//	Update		:	
//***********************************************************************************

#ifndef _NTL_POST_EFFECT_CAMERA_
#define _NTL_POST_EFFECT_CAMERA_


#include <string>
#include "NtlCamera.h"
#include "NtlDeviceRestoreHandler.h"
#include "d3dx9.h"
#include "NtlInstancePostEffectSystem.h"
#include "NtlPLLensFlare.h"
#include "NtlPLRefractionEffect.h"


class CNtlPLHeatHaze;
struct RpWorld;

/**
*	post effect filter
*/
#define POST_EFFECT_FILTER_NONE		0
#define POST_EFFECT_FILTER_HDR		1
#define dMONO_POWER_DEFAULT (2.4f)
#define RES_MESH_CNT 20                                     ///< View ÃˆÂ­Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½ Mesh Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½ (Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½ Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½)
#define RES_VERTEX_CNT (RES_MESH_CNT * RES_MESH_CNT * 6)    ///< View ÃˆÂ­Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½ VertexÃ¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½ (Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½ Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½)

class CNtlPostEffectCamera : public CNtlCamera
{

public:
    enum EFFECT_PASS
    {
        EFFECT_PASS_NONE = 0,
        EFFECT_PASS_LASTDRAW,
        EFFECT_PASS_MONO_POWER,
        EFFECT_PASS_16BOX,

        EFFECT_PASS_MAX_COUNT,
    };

public:
	CNtlPostEffectCamera(void);
	~CNtlPostEffectCamera(void);

	void		Destroy();
	void		DestroyBlurCamera(RwCamera*& pCamera, RwTexture*& pTexture);

	RwBool		CreateBlurCamera(RwCamera*& pCamera, RwTexture*& pTexture, RwInt32 nWidth, RwInt32 nHeight, RwReal fNearClipPlane, RwReal fFarClipPlane);
	RwBool		CreateEffect();

	// Name tag / damage text pass: renders into a separate alpha texture using
	// the world camera's z-buffer (correct occlusion), composited after FXAA.
	RwCamera*	BeginNameCamera();
	void		EndNameCamera();

	virtual		RwCamera* Create( RwInt32 width,
						RwInt32 height, 
						RwBool zBuffer, 
						RwReal fNearClipPlane = 1.f, 
						RwReal fFarClipPlane = 1000.f);

    static CNtlPostEffectCamera*		GetInstance(void);
    static void		SetPostEffectFilters( RwUInt8 byPEFilters );
    static RwUInt8	GetPostEffectFilters(void);

    void		ApplySSAA();
    static void	SetSSAAScale(RwInt32 nScale);
    static RwInt32 GetSSAAScale();
    static RwReal GetRenderScale();

	// AA mode: 0 = off, 3 = FXAA, 4 = MSAA 2x, 5 = MSAA 4x, 6 = MSAA 8x
    static void	SetAAMode(RwInt32 nMode);
    static RwInt32 GetAAMode();
    static RwBool IsAAFilterEnabled();
    static RwBool IsMSAAMode();
    static RwInt32 GetMSAALevel();
    static void	SetCurrentMSAALevel(RwInt32 nLevel);
    static RwInt32 GetCurrentMSAALevel();
    static void	RequestMSAALevel(RwInt32 nLevel);
    static void	FlushMSAALevel();
    // Detaches the main camera from a world about to be destroyed (the world
    // plugin does not track attached cameras; the pointer would dangle).
    static void	RemoveMainCameraFromWorld(RpWorld* pWorld);

    virtual	void SetCameraSize(RwRect* rect);
    virtual void SetCalcFov(RwReal fFov);			

    RwCamera*	CameraClear(RwRGBA *rgba, RwInt32 uiClearMode);
    void		CameraShowRaster(HWND hWnd, RwUInt32 uiFlags);

    RwCamera*	PostCameraBeginUpdate(RwReal fElapsed);
    void		PostCameraEndUpdate(void);

    RwCamera*	MainCameraBeginUpdate(RwReal fElapsed);
    void		MainCameraEndUpdate(void);

	RwCamera*	GetMainCamera()	{ return m_pMainCamera; }

    virtual void		UpdatePostEffectFilter(void);
    virtual void		UpdateBlurFilter();
	virtual void		UpdateHeatHaze(RwReal fElapsedTime);
    virtual void        UpdateMotionBlur(RwReal fElapsedTime);
    virtual void        SetEnableBlur(RwBool bEnable, RwReal fFadeTime = 0.0f);
    virtual RwBool      GetEnableBlur() {return m_bEnableBlur;}

    RwV2d       GetScreenSize();

    // Post Effect System Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½ Ã¯Â¿Â½ÃžÂ¼Ã’ÂµÃ¯Â¿Â½
    void        AttachPostEffectSystem(CNtlInstancePostEffectSystem* pPostEffectSystem);
    void        DetachPostEffectSystem(CNtlInstancePostEffectSystem* pPostEffectSystem);

	// LensFlare Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½ Ã¯Â¿Â½ÃžÂ¼Ã’ÂµÃ¯Â¿Â½
	void		AttachLensFlare(CNtlPLLensFlare* pLensFlare);				///< LensFlare Ã¯Â¿Â½Ã¯Â¿Â½
	void		DetachLensFlare(CNtlPLLensFlare* pLensFlare);				///< LensFlare Ã¯Â¿Â½Ã¯Â¿Â½

protected:
    RwBool		IsEnablePostEffect(void);

	void		BuildVertex(RwInt32 width, RwInt32 height);
	void		SetRhwVertex(RwIm2DVertex* pVertex, RwInt32 nStartX, RwInt32 nStartY, RwInt32 width, RwInt32 height);	
	
	void		Update_Power_MonoPower();
	void		Update_FakeHDRFiltering();

	void		CreateReductionTexture(RwInt32 nTextureSize);

	void		DrawTexture();
	void		RenderPostEffectQuad(LPD3DXEFFECT pEffect, D3DXHANDLE hTechnique, D3DXHANDLE hRcpFrame, RwTexture* pTexture);
	void		RenderFXAAComposite();
	void		RenderSSAAComposite();
	void		RenderAAModeComposite();
	RwBool		CreateFSREffect();

	int			ReleaseHandler();
	int			RestoreHandler();

public:
    RwReal				m_fMonoPowerHeight;
    D3DXVECTOR4			m_vRGB2Y;

protected:
    static				CNtlPostEffectCamera*	m_pInstance;
    static				RwUInt8					m_byPEFilters;
    static				RwInt32					m_nSSAAScale;
    static				RwInt32					m_nAAMode;
    static				RwInt32					m_nCurrentMSAALevel;
    static				RwInt32					m_nPendingMSAALevel;
    static				RwBool					m_bMainCameraBegun;

    RwBool				m_bEnableCamera;
    std::string			m_strShaderName;

    RwRGBA				m_BackgroundColor;

    RwInt32				m_nBlurTextureWidth;
    RwInt32				m_nBlurTextureHeight;

    RwTexture*			m_pSourceTexture;

    RwCamera*			m_pTempCamera;
    RwTexture*			m_pTempTexture;

    RwTexture*			m_pNameTexture;
    RwRaster*			m_pNamePassWorldRaster;

    RwCamera*			m_pReductionCamera;
    RwTexture*			m_pReductionTexture;

    RwCamera*			m_pFakeHDRCamera;
    RwTexture*			m_pFakeHDRTexture;

	// Heat haze
	RwCamera*			m_pHeatHazeCamera;
	RwTexture*			m_pHeatHazeTexture;
	CNtlPLHeatHaze*		m_pHeatHaze;

    // Motion BlurÃ¯Â¿Â½Ã¯Â¿Â½ Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½ Ã„Â«Ã¯Â¿Â½ÃžÂ¶Ã¯Â¿Â½Ã¯Â¿Â½ Ã¯Â¿Â½Ã˜Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½ 
    RwCamera*			m_pMotionBlurCamera;
    RwTexture*			m_pMotionBlurTexture;

    RwCamera*			m_pMainCamera;
    RpWorld*			m_pMainCameraWorld; // world the main camera is attached to (MSAA lighting)

    RwIm3DVertex		m_3dVertex[4];
    RwIm2DVertex		m_RhwVertex[4];
    RwIm2DVertex		m_RhwReductionVertex[4];
    RwIm2DVertex        m_RhwBlurVertex[4];

    LPDIRECT3DDEVICE9	m_lpDevice;
    LPD3DXEFFECT		m_lpEffect;			// Effect File Handler
    D3DXHANDLE			m_hTechnique;

    LPD3DXEFFECT		m_lpFXAAEffect;		// FXAA Effect File Handler
    D3DXHANDLE			m_hFXAATechnique;
    D3DXHANDLE			m_hFXAARCPFrame;
    D3DXHANDLE			m_hGaussianTechnique;

    D3DXHANDLE			m_hMapWidth;
    D3DXHANDLE			m_hMapHeight;

    D3DXHANDLE			m_hMonoPowerHeight;
    D3DXHANDLE			m_hRGB2Y;


    RwInt32				m_nTextureReductionSize;

	DEVICE_RESTORE_HANDLE	m_handleRelease;
    DEVICE_RESTORE_HANDLE	m_handleRestore;

    // Help Draw Vertex
    RwIm2DVertex		m_SourceVertex[4];
    RwIm2DVertex		m_PowerVertex[4];
    RwIm2DVertex		m_FinalVertex[4];

    RwBool              m_bEnableBlur;                      ///< Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½ Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½ Enable Flag
    RwReal              m_fBlurAlpha;                       ///< Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½ Blur AlphaÃ¯Â¿Â½Ã¯Â¿Â½
    RwReal              m_fBlurFadeVelocity;                ///< BlurÃ¯Â¿Â½Ã¯Â¿Â½ Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã‡Â´Ã¯Â¿Â½ Ã¯Â¿Â½Ã“ÂµÃ¯Â¿Â½

    std::list<CNtlInstancePostEffectSystem*> m_listPostEffectSystem; ///< Ã„Â«Ã¯Â¿Â½ÃžÂ¶Ã¯Â¿Â½ Ã‡Â¥Ã¯Â¿Â½
	std::list<CNtlPLLensFlare*> m_listLensFlare;			///< Ã„Â«Ã¯Â¿Â½ÃžÂ¶Ã¯Â¿Â½ Ã‡Â¥Ã¯Â¿Â½

	// TEST
public:
	CNtlPLHeatHaze* GetHeatHaze() { return m_pHeatHaze; }
};

#endif
