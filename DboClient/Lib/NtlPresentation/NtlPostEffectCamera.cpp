//***********************************************************************************
//	File		:	NtlBlurCamera.cpp
//	Desc		:	
//	Begin		:	2006. 2.13
//	Copyright	:
//	Author		:	agebreak
//	Update		:	
//***********************************************************************************

#include "precomp_ntlpresentation.h"
#include "NtlPostEffectCamera.h"
#include "NtlText.h"
#include "NtlPLRenderState.h"
#include "NtlEffectDefine.h"
#include "NtlPLWorldEntity.h"
#include "NtlPLVisualManager.h"
#include "NtlProfiler.h"
#include "NtlPLHeatHaze.h"

#include "NtlPLResourcePack.h"
#include "NtlPLResourceManager.h"


CNtlPostEffectCamera* CNtlPostEffectCamera::m_pInstance = NULL;
RwUInt8				  CNtlPostEffectCamera::m_byPEFilters = 0;
RwInt32				  CNtlPostEffectCamera::m_nSSAAScale = 1;
RwInt32				  CNtlPostEffectCamera::m_nAAMode = 0;
RwInt32				  CNtlPostEffectCamera::m_nCurrentMSAALevel = 1;
RwInt32				  CNtlPostEffectCamera::m_nPendingMSAALevel = 0;
RwBool				  CNtlPostEffectCamera::m_bMainCameraBegun = FALSE;
// FXAA 3.11 (quality preset) - Timothy Lottes, public domain.
// Tuned for toon-shaded art: high edge threshold (only clear jaggies are
// filtered) so the black outlines are not widened, plus the full 12-tap
// edge search that reduces smear on long diagonal edges.
// Also contains a CAS-style contrast-adaptive sharpener (technique "CAS")
// and a Gaussian 5x5 downscale for the 2x SSAA composite (technique "GAUSSIAN").
static const char* FXAA_SHADER_SOURCE =
"float2 RCPFRAME;\n"
"\n"
"static const float EDGE_THRESHOLD_MIN = 0.09375;\n"
"static const float EDGE_THRESHOLD_MAX = 0.125;\n"
"static const float MAX_BLEND_DIST    = 2.0;\n"
"\n"
"sampler SrcSamp : register(s0) = sampler_state\n"
"{\n"
"	MinFilter = LINEAR;\n"
"	MagFilter = LINEAR;\n"
"	MipFilter = NONE;\n"
"	AddressU = Clamp;\n"
"	AddressV = Clamp;\n"
"};\n"
"\n"
"float FxaaLuma(float3 rgb)\n"
"{\n"
"	return dot(rgb, float3(0.299, 0.587, 0.114));\n"
"}\n"
"\n"
"float4 PS_FXAA(float2 uv : TEXCOORD0) : COLOR\n"
"{\n"
"	float2 rcpFrame = RCPFRAME;\n"
"\n"
"	float3 rgbNW = tex2D(SrcSamp, uv + float2(-1.0, -1.0) * rcpFrame).xyz;\n"
"	float3 rgbNE = tex2D(SrcSamp, uv + float2( 1.0, -1.0) * rcpFrame).xyz;\n"
"	float3 rgbSW = tex2D(SrcSamp, uv + float2(-1.0,  1.0) * rcpFrame).xyz;\n"
"	float3 rgbSE = tex2D(SrcSamp, uv + float2( 1.0,  1.0) * rcpFrame).xyz;\n"
"	float3 rgbM  = tex2D(SrcSamp, uv).xyz;\n"
"\n"
"	float lumaNW = FxaaLuma(rgbNW);\n"
"	float lumaNE = FxaaLuma(rgbNE);\n"
"	float lumaSW = FxaaLuma(rgbSW);\n"
"	float lumaSE = FxaaLuma(rgbSE);\n"
"	float lumaM  = FxaaLuma(rgbM);\n"
"\n"
"	float lumaMin = min(lumaM, min(min(lumaNW, lumaNE), min(lumaSW, lumaSE)));\n"
"	float lumaMax = max(lumaM, max(max(lumaNW, lumaNE), max(lumaSW, lumaSE)));\n"
"\n"
"	// Faint edges (below the min threshold) are not filtered at all.\n"
"	if (lumaMax - lumaMin < EDGE_THRESHOLD_MIN)\n"
"		return float4(rgbM, 1.0);\n"
"\n"
"	// Edge direction from the 4-tap luma gradient.\n"
"	float2 dir;\n"
"	dir.x = -((lumaNW + lumaNE) - (lumaSW + lumaSE));\n"
"	dir.y =  ((lumaNW + lumaSW) - (lumaNE + lumaSE));\n"
"\n"
"	float dirReduce = max((lumaNW + lumaNE + lumaSW + lumaSE) * 0.03125, 0.0078125);\n"
"	float rcpDirMin = 1.0 / (min(abs(dir.x), abs(dir.y)) + dirReduce);\n"
"\n"
"	dir = min(float2(MAX_BLEND_DIST, MAX_BLEND_DIST),\n"
"			  max(float2(-MAX_BLEND_DIST, -MAX_BLEND_DIST), dir * rcpDirMin)) * rcpFrame;\n"
"\n"
"	// 2+2 tap blend along the edge.\n"
"	float3 rgbA = 0.5 * (\n"
"		tex2D(SrcSamp, uv + dir * (1.0 / 3.0 - 0.5)).xyz +\n"
"		tex2D(SrcSamp, uv + dir * (2.0 / 3.0 - 0.5)).xyz);\n"
"	float3 rgbB = rgbA * 0.5 + 0.25 * (\n"
"		tex2D(SrcSamp, uv + dir * -0.5).xyz +\n"
"		tex2D(SrcSamp, uv + dir * 0.5).xyz);\n"
"\n"
"	// If the blended result leaves the local luma range, the edge is too\n"
"	// strong - fall back to the lighter 2-tap blend (keeps outlines sharp).\n"
"	float lumaB = FxaaLuma(rgbB);\n"
"	if (lumaB < lumaMin || lumaB > lumaMax)\n"
"		return float4(rgbA, 1.0);\n"
"	return float4(rgbB, 1.0);\n"
"}\n"
"\n"
"// Gaussian 5x5 downscale for the 2x SSAA composite (sigma ~ 1.0).\n"
"float4 PS_GAUSSIAN(float2 uv : TEXCOORD0) : COLOR\n"
"{\n"
"	float2 rcpFrame = RCPFRAME;\n"
"	float3 result = 0.0;\n"
"\n"
"	float w[3];\n"
"	w[0] = 0.40261995;\n"
"	w[1] = 0.24420134;\n"
"	w[2] = 0.05448868;\n"
"\n"
"	for (int y = -2; y <= 2; ++y)\n"
"	{\n"
"		for (int x = -2; x <= 2; ++x)\n"
"		{\n"
"			float w2 = w[abs(x)] * w[abs(y)];\n"
"			result += tex2D(SrcSamp, uv + float2((float)x, (float)y) * rcpFrame).xyz * w2;\n"
"		}\n"
"	}\n"
"\n"
"	return float4(result, 1.0);\n"
"}\n"
"\n"
"technique FXAA\n"
"{\n"
"	pass P0\n"
"	{\n"
"		PixelShader = compile ps_3_0 PS_FXAA();\n"
"	}\n"
"}\n"
"\n"
"technique GAUSSIAN\n"
"{\n"
"	pass P0\n"
"	{\n"
"		PixelShader = compile ps_3_0 PS_GAUSSIAN();\n"
"	}\n"
"}\n";


// FSR-like upscaler: renders at reduced internal resolution and upscales
// with a Catmull-Rom bicubic kernel (edge-aware: blends toward bilinear on

CNtlPostEffectCamera::CNtlPostEffectCamera(void)
{
	m_pInstance				= this;

	m_bEnableCamera			= FALSE;
	m_strShaderName			= ".\\script\\Blur.fx";

	m_BackgroundColor.red	= 0;
	m_BackgroundColor.green	= 0;
	m_BackgroundColor.blue	= 0;
	m_BackgroundColor.alpha	= 0;

	m_pSourceTexture		= NULL;

	m_nBlurTextureWidth		= 128;
	m_nBlurTextureHeight	= 128;

	m_pMainCamera			= NULL;
	m_pMainCameraWorld		= NULL;

	m_pTempCamera			= NULL;
	m_pTempTexture			= NULL;

	m_pNameTexture			= NULL;
	m_pNamePassWorldRaster	= NULL;

	m_pReductionCamera		= NULL;
	m_pReductionTexture		= NULL;

	m_pFakeHDRCamera		= NULL;
	m_pFakeHDRTexture		= NULL;

	m_lpDevice				= NULL;

	m_lpEffect				= NULL;
	m_hTechnique			= NULL;

	m_lpFXAAEffect			= NULL;
	m_hFXAATechnique		= NULL;
	m_hFXAARCPFrame			= NULL;
	m_hGaussianTechnique	= NULL;

	m_hMapWidth				= NULL;
	m_hMapHeight			= NULL;

	m_hMonoPowerHeight		= NULL;
	m_hRGB2Y				= NULL;

	m_fMonoPowerHeight		= dMONO_POWER_DEFAULT;

	m_vRGB2Y				= D3DXVECTOR4(0.29900f, 0.4f, 0.11400f, 0.f);
//	m_vRGB2Y				= D3DXVECTOR4(0.29900f, 0.58700f, 0.11400f, 0.f);

	m_nTextureReductionSize	= 128;

    m_pMotionBlurCamera     = NULL;
    m_pMotionBlurTexture    = NULL;

    m_bEnableBlur           = FALSE;    
    m_fBlurAlpha            = EFFECT_BLUR_ALPHA;
    m_fBlurFadeVelocity     = 0.0f;

	m_handleRelease			= CNtlDeviceRestoreHandler::GetInstance()->LinkDeviceRelease(this, &CNtlPostEffectCamera::ReleaseHandler);
	m_handleRestore			= CNtlDeviceRestoreHandler::GetInstance()->LinkDeviceRestore(this, &CNtlPostEffectCamera::RestoreHandler);
	
	m_pHeatHazeCamera = NULL;
	m_pHeatHazeTexture = NULL;
	m_pHeatHaze = NULL;

	SetPostEffectFilters(POST_EFFECT_FILTER_NONE);
}


CNtlPostEffectCamera::~CNtlPostEffectCamera(void)
{
	CNtlDeviceRestoreHandler::GetInstance()->UnLinkDeviceRelease(m_handleRelease);
	CNtlDeviceRestoreHandler::GetInstance()->UnLinkDeviceRestore(m_handleRestore);

	m_pInstance		= NULL;
}

CNtlPostEffectCamera* CNtlPostEffectCamera::GetInstance(void)
{
	return m_pInstance;
}

//------------------------------------------------------------------
//	FuncName	: Destroy
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CNtlPostEffectCamera::Destroy()
{
	if (m_lpEffect != NULL)
	{
		m_lpEffect->Release();
		m_lpEffect = NULL;

		m_hTechnique			= NULL;
	}

	// Delete Camera
    DestroyBlurCamera(m_pMotionBlurCamera, m_pMotionBlurTexture);

	DestroyBlurCamera(m_pReductionCamera, m_pReductionTexture);

	DestroyBlurCamera(m_pFakeHDRCamera, m_pFakeHDRTexture);

	DestroyBlurCamera(m_pTempCamera, m_pTempTexture);

	if (m_pNameTexture != NULL)
	{
		RwTextureDestroy(m_pNameTexture);
		m_pNameTexture = NULL;
	}
	m_pNamePassWorldRaster = NULL;

	DestroyBlurCamera(m_pCamera, m_pSourceTexture);

	DestroyBlurCamera(m_pHeatHazeCamera, m_pHeatHazeTexture);

	m_pCamera = m_pMainCamera;
	m_pMainCamera = NULL;

	CNtlCamera::Destroy();

	if(m_pHeatHaze)
	{
		NTL_DELETE(m_pHeatHaze);
	}
}

void CNtlPostEffectCamera::DestroyBlurCamera(RwCamera*& pCamera, RwTexture*& pTexture)
{
	if(pCamera != NULL)
	{
		RwFrame *frame;
		RwRaster *raster;

		frame = RwCameraGetFrame(pCamera);
		RwCameraSetFrame(pCamera, NULL);
		RwFrameDestroy(frame);

		raster = RwCameraGetRaster(pCamera);
		RwCameraSetRaster(pCamera, NULL);
		RwRasterDestroy(raster);

		raster = RwCameraGetZRaster(pCamera);
		RwCameraSetZRaster(pCamera, NULL);
		RwRasterDestroy(raster);

		RwCameraDestroy(pCamera);
		pCamera = NULL;

		if (pTexture != NULL)
		{
			RwTextureSetRaster(pTexture, NULL);
			RwTextureDestroy(pTexture);
			pTexture = NULL;
		}
	}
}

//------------------------------------------------------------------
//	FuncName	: CreateBlurCamera
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
RwBool CNtlPostEffectCamera::CreateBlurCamera(RwCamera*& pCamera, RwTexture*& pTexture, RwInt32 nWidth, RwInt32 nHeight, RwReal fNearClipPlane, RwReal fFarClipPlane)
{
	// SSAA: the texture gets a full mip chain (AUTOGENMIPMAP) so the composite
	// quad downsamples the higher internal resolution cleanly through the
	// correct mip level (trilinear filtering).
	RwRaster *raster = RwRasterCreate(nWidth, nHeight, 0, rwRASTERTYPECAMERATEXTURE | rwRASTERFORMATMIPMAP | rwRASTERFORMATAUTOMIPMAP);
	if(raster != NULL)
	{
		RwRaster* zRaster = RwRasterCreate(nWidth, nHeight, 0, rwRASTERTYPEZBUFFER);
		if(zRaster != NULL)
		{
			RwFrame *frame = RwFrameCreate();
			if(frame != NULL)
			{
				pCamera = RwCameraCreate();
				if(pCamera != NULL)
				{
					//RwV2d vw;

					RwCameraSetRaster(pCamera, raster);
					RwCameraSetZRaster(pCamera, zRaster);

					RwCameraSetFrame(pCamera, frame);

					RwCameraSetNearClipPlane(pCamera, fNearClipPlane);
					RwCameraSetFarClipPlane(pCamera, fFarClipPlane);

					/*
					vw.x = vw.y = 0.4f;
					RwCameraSetViewWindow(pCamera, &vw);
					*/
                    CNtlCamera::SetCalcFov(pCamera, GetFov());
					
					pTexture = RwTextureCreate(raster);
					DBO_ASSERT(pTexture, "Texture create failed.");

					// Trilinear filtering + hardware-generated mip chain: the composite
					// quad downscales the Nx render target to screen size through the
					// correct mip level, giving a proper box-filtered downsample.
					RwTextureSetFilterMode(pTexture, rwFILTERLINEARMIPLINEAR);
					RwTextureSetAddressing(pTexture, rwTEXTUREADDRESSCLAMP);

					return TRUE;
				}
				RwFrameDestroy(frame);
			}
			RwRasterDestroy(zRaster);
		}
		RwRasterDestroy(raster);
	}
	return FALSE;
}


//------------------------------------------------------------------
//	FuncName	: Create
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
RwCamera* CNtlPostEffectCamera::Create(RwInt32 width, 
							 RwInt32 height, 
							 RwBool zBuffer, 
							 RwReal fNearClipPlane, 
							 RwReal fFarClipPlane)
{
	m_bEnableCamera = TRUE;
	m_nBlurTextureWidth = width;
	m_nBlurTextureHeight = height;

	m_lpDevice = (LPDIRECT3DDEVICE9)RwD3D9GetCurrentD3DDevice();

	if (!CreateEffect())
	{
		DBO_WARNING_MESSAGE("CreateEffect FAIL - continuing without post effects");
	}

	m_pMainCamera = CNtlCamera::Create(width, height, zBuffer, fNearClipPlane, fFarClipPlane);
	if (m_pMainCamera == NULL)
	{
		DBO_WARNING_MESSAGE("CNtlCamera::Create m_pMainCamera FAIL");
		return NULL;
	}

	// AA: render the world at a scaled internal resolution (SSAA 2x scales
	// the raster up; otherwise 1x).
	RwInt32 nSSAAScale = m_nSSAAScale;
	if (nSSAAScale < 1) nSSAAScale = 1;
	m_nSSAAScale = nSSAAScale;

	RwReal fRenderScale = GetRenderScale();
	RwInt32 nSSAAWidth = (RwInt32)(width * fRenderScale);
	RwInt32 nSSAAHeight = (RwInt32)(height * fRenderScale);
	if (nSSAAWidth < 1) nSSAAWidth = 1;
	if (nSSAAHeight < 1) nSSAAHeight = 1;

	if (!CreateBlurCamera(m_pCamera, m_pSourceTexture, nSSAAWidth, nSSAAHeight, fNearClipPlane, fFarClipPlane))
	{
		DBO_WARNING_MESSAGE("CreateBlurCamera m_pCamera FAIL");
		return NULL;
	}
	if (!CreateBlurCamera(m_pTempCamera, m_pTempTexture, nSSAAWidth, nSSAAHeight, fNearClipPlane, fFarClipPlane))
	{
		DBO_WARNING_MESSAGE("CreateBlurCamera m_pTempCamera FAIL");
		return NULL;
	}
    if(!CreateBlurCamera(m_pMotionBlurCamera, m_pMotionBlurTexture, EFFECT_BLUR_TEXTURE_WIDTH, EFFECT_BLUR_TEXTURE_HEIGHT, fNearClipPlane, fFarClipPlane))
    {
		DBO_WARNING_MESSAGE("CreateBlurCamera m_pMotionBlurCamera FAIL");
        return NULL;
    }
	if(!CreateBlurCamera(m_pHeatHazeCamera, m_pHeatHazeTexture, width, height, fNearClipPlane, fFarClipPlane))
	{
		DBO_WARNING_MESSAGE("m_pHeatHazeCamera m_pMotionBlurCamera FAIL");
		return NULL;
	}

	// Name tag / damage text texture: A8R8G8B8 so the tags can be alpha-blended
	// over the FXAA'd world. Rendered through the world camera itself (raster
	// swap) so it shares the world view and z-buffer exactly.
	{
		RwRaster* pNameRaster = RwRasterCreate(nSSAAWidth, nSSAAHeight, 0,
			rwRASTERTYPECAMERATEXTURE | rwRASTERFORMATMIPMAP | rwRASTERFORMATAUTOMIPMAP | rwRASTERFORMAT8888);
		if (pNameRaster != NULL)
		{
			m_pNameTexture = RwTextureCreate(pNameRaster);
			RwTextureSetFilterMode(m_pNameTexture, rwFILTERLINEARMIPLINEAR);
			RwTextureSetAddressing(m_pNameTexture, rwTEXTUREADDRESSCLAMP);
		}
	}

	CreateReductionTexture(m_nTextureReductionSize);

	BuildVertex(width, height);

	m_pHeatHaze = NTL_NEW CNtlPLHeatHaze(width, height);

	return m_pCamera;
}

void CNtlPostEffectCamera::SetSSAAScale(RwInt32 nScale)
{
	m_nSSAAScale = nScale;
	if (m_nSSAAScale < 1) m_nSSAAScale = 1;
	if (m_nSSAAScale > 8) m_nSSAAScale = 8;
}

RwInt32 CNtlPostEffectCamera::GetSSAAScale()
{
	return m_nSSAAScale;
}

RwReal CNtlPostEffectCamera::GetRenderScale()
{
	if (m_nSSAAScale > 1)
		return (RwReal)m_nSSAAScale;
	return 1.0f;
}

void CNtlPostEffectCamera::SetAAMode(RwInt32 nMode)
{
	if (nMode < 0) nMode = 0;
	if (nMode > 6) nMode = 6;
	if (nMode == 1 || nMode == 2) nMode = 0;	// not valid mode ids
	m_nAAMode = nMode;
}

RwInt32 CNtlPostEffectCamera::GetAAMode()
{
	return m_nAAMode;
}

RwBool CNtlPostEffectCamera::IsAAFilterEnabled()
{
	return (m_nAAMode == 3);
}

RwBool CNtlPostEffectCamera::IsMSAAMode()
{
	return (m_nAAMode >= 4);
}

RwInt32 CNtlPostEffectCamera::GetMSAALevel()
{
	switch (m_nAAMode)
	{
	case 4: return 2;
	case 5: return 4;
	case 6: return 8;
	}
	return 1;
}

void CNtlPostEffectCamera::SetCurrentMSAALevel(RwInt32 nLevel)
{
	m_nCurrentMSAALevel = nLevel;
}

RwInt32 CNtlPostEffectCamera::GetCurrentMSAALevel()
{
	return m_nCurrentMSAALevel;
}

// The device reset for an MSAA level change must happen BETWEEN frames
// (never inside a begun scene). The option apply only requests the level;
// DboApplication flushes it after the frame is presented.
void CNtlPostEffectCamera::RequestMSAALevel(RwInt32 nLevel)
{
	if (nLevel < 1) nLevel = 1;
	m_nPendingMSAALevel = nLevel;
}

void CNtlPostEffectCamera::FlushMSAALevel()
{
	if (m_nPendingMSAALevel == 0)
		return;
	if (m_nPendingMSAALevel == m_nCurrentMSAALevel)
	{
		m_nPendingMSAALevel = 0;
		return;
	}

	if (RwD3D9ChangeMultiSamplingLevels((RwUInt32)m_nPendingMSAALevel))
	{
		m_nCurrentMSAALevel = m_nPendingMSAALevel;
	}
	m_nPendingMSAALevel = 0;
}

void CNtlPostEffectCamera::RemoveMainCameraFromWorld(RpWorld* pWorld)
{
	if (m_pInstance != NULL && m_pInstance->m_pMainCamera != NULL &&
		m_pInstance->m_pMainCameraWorld == pWorld)
	{
		RpWorldRemoveCamera(pWorld, m_pInstance->m_pMainCamera);
		m_pInstance->m_pMainCameraWorld = NULL;
	}
}

RwCamera* CNtlPostEffectCamera::BeginNameCamera()
{
	// MSAA mode: name tags render in the world pass (multisampled by MSAA).
	if (IsMSAAMode())
		return NULL;

	if (!IsAAFilterEnabled() || m_pNameTexture == NULL || m_pCamera == NULL)
		return NULL;

	RwRaster* pNameRaster = RwTextureGetRaster(m_pNameTexture);
	if (pNameRaster == NULL)
		return NULL;

	// Render the tags through the world camera itself (temporary raster swap):
	// it shares the world view, projection and z-buffer exactly, so the tags
	// project identically to the world pass and are occluded by geometry.
	m_pNamePassWorldRaster = RwCameraGetRaster(m_pCamera);
	RwCameraSetRaster(m_pCamera, pNameRaster);

	// Image-only clear: the world depth stays in the z-buffer.
	RwCameraClear(m_pCamera, &m_BackgroundColor, rwCAMERACLEARIMAGE);

	return RwCameraBeginUpdate(m_pCamera);
}

void CNtlPostEffectCamera::EndNameCamera()
{
	if (m_pCamera == NULL)
		return;

	RwCameraEndUpdate(m_pCamera);

	if (m_pNamePassWorldRaster != NULL)
	{
		RwCameraSetRaster(m_pCamera, m_pNamePassWorldRaster);
		m_pNamePassWorldRaster = NULL;
	}
}
//------------------------------------------------------------------
//	FuncName	: RenderPostEffectQuad
//	Desc		: Renders a fullscreen quad with the given effect technique,
//				sampling the given texture at stage 0 (RCPFRAME updated
//				from the texture size).
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CNtlPostEffectCamera::RenderPostEffectQuad(LPD3DXEFFECT pEffect, D3DXHANDLE hTechnique, D3DXHANDLE hRcpFrame, RwTexture* pTexture)
{
	if (pEffect == NULL || hTechnique == NULL || pTexture == NULL)
		return;

	RwRaster* pRaster = RwTextureGetRaster(pTexture);
	if (pRaster && hRcpFrame)
	{
		D3DXVECTOR4 vRcpFrame;
		vRcpFrame.x = 1.0f / (RwReal)RwRasterGetWidth(pRaster);
		vRcpFrame.y = 1.0f / (RwReal)RwRasterGetHeight(pRaster);
		vRcpFrame.z = 0.0f;
		vRcpFrame.w = 0.0f;
		pEffect->SetVector(hRcpFrame, &vRcpFrame);
	}

	RwD3D9SetTexture(pTexture, 0);

	pEffect->SetTechnique(hTechnique);
	if (SUCCEEDED(pEffect->Begin(NULL, 0)))
	{
		if (SUCCEEDED(pEffect->BeginPass(0)))
		{
			pEffect->CommitChanges();

			RwIm2DRenderPrimitive(rwPRIMTYPETRISTRIP, m_RhwVertex, 4);

			pEffect->EndPass();
		}
		pEffect->End();
	}
}

//------------------------------------------------------------------
//	FuncName	: RenderFXAAComposite
//	Desc		: Fullscreen FXAA pass over the source texture.
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CNtlPostEffectCamera::RenderFXAAComposite()
{
	RenderPostEffectQuad(m_lpFXAAEffect, m_hFXAATechnique, m_hFXAARCPFrame, m_pSourceTexture);
}

//------------------------------------------------------------------
//	FuncName	: RenderSSAAComposite
//	Desc		: Gaussian 5x5 downscale of the 2x source texture to the
//				current render target.
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CNtlPostEffectCamera::RenderSSAAComposite()
{
	RenderPostEffectQuad(m_lpFXAAEffect, m_hGaussianTechnique, m_hFXAARCPFrame, m_pSourceTexture);
}

//------------------------------------------------------------------
void CNtlPostEffectCamera::RenderAAModeComposite()
{
	switch (m_nAAMode)
	{
	case 3:
		RenderFXAAComposite();
		break;
	}
}
void CNtlPostEffectCamera::ApplySSAA()
{
	RwInt32 nSSAAScale = m_nSSAAScale;
	if (nSSAAScale < 1) nSSAAScale = 1;
	if (nSSAAScale > 8) nSSAAScale = 8;
	m_nSSAAScale = nSSAAScale;

	RwReal fRenderScale = GetRenderScale();
	RwInt32 nWidth = (RwInt32)(m_nBlurTextureWidth * fRenderScale);
	RwInt32 nHeight = (RwInt32)(m_nBlurTextureHeight * fRenderScale);
	if (nWidth < 1) nWidth = 1;
	if (nHeight < 1) nHeight = 1;

	RwReal fNearClipPlane = RwCameraGetNearClipPlane(m_pCamera);
	RwReal fFarClipPlane = RwCameraGetFarClipPlane(m_pCamera);

	// Create the new rasters first. On failure the old camera stays intact.
	RwRaster* pNewRaster = RwRasterCreate(nWidth, nHeight, 0, rwRASTERTYPECAMERATEXTURE | rwRASTERFORMATMIPMAP | rwRASTERFORMATAUTOMIPMAP);
	if (pNewRaster == NULL)
	{
		DBO_WARNING_MESSAGE("ApplySSAA RwRasterCreate m_pCamera FAIL - falling back to 1x");
		nSSAAScale = 1;
		m_nSSAAScale = 1;
		nWidth = m_nBlurTextureWidth;
		nHeight = m_nBlurTextureHeight;
		pNewRaster = RwRasterCreate(nWidth, nHeight, 0, rwRASTERTYPECAMERATEXTURE | rwRASTERFORMATMIPMAP | rwRASTERFORMATAUTOMIPMAP);
		if (pNewRaster == NULL)
		{
			DBO_WARNING_MESSAGE("ApplySSAA fallback 1x RwRasterCreate FAIL");
			return;
		}
	}
	RwRaster* pNewZRaster = RwRasterCreate(nWidth, nHeight, 0, rwRASTERTYPEZBUFFER);
	if (pNewZRaster == NULL)
	{
		RwRasterDestroy(pNewRaster);
		DBO_WARNING_MESSAGE("ApplySSAA RwRasterCreate zraster FAIL - falling back to 1x");
		nSSAAScale = 1;
		m_nSSAAScale = 1;
		nWidth = m_nBlurTextureWidth;
		nHeight = m_nBlurTextureHeight;
		pNewRaster = RwRasterCreate(nWidth, nHeight, 0, rwRASTERTYPECAMERATEXTURE | rwRASTERFORMATMIPMAP | rwRASTERFORMATAUTOMIPMAP);
		pNewZRaster = RwRasterCreate(nWidth, nHeight, 0, rwRASTERTYPEZBUFFER);
		if (pNewRaster == NULL || pNewZRaster == NULL)
		{
			if (pNewRaster) RwRasterDestroy(pNewRaster);
			if (pNewZRaster) RwRasterDestroy(pNewZRaster);
			DBO_WARNING_MESSAGE("ApplySSAA fallback 1x raster create FAIL");
			return;
		}
	}

	// Swap the rasters on the EXISTING camera (keeps the frame/view matrix intact)
	RwRaster* pOldRaster = RwCameraGetRaster(m_pCamera);
	RwRaster* pOldZRaster = RwCameraGetZRaster(m_pCamera);

	RwCameraSetRaster(m_pCamera, NULL);
	RwCameraSetZRaster(m_pCamera, NULL);

	RwCameraSetRaster(m_pCamera, pNewRaster);
	RwCameraSetZRaster(m_pCamera, pNewZRaster);
	RwCameraSetNearClipPlane(m_pCamera, fNearClipPlane);
	RwCameraSetFarClipPlane(m_pCamera, fFarClipPlane);

	// Re-point the source texture to the new raster
	RwTextureSetRaster(m_pSourceTexture, pNewRaster);

	RwRasterDestroy(pOldRaster);
	RwRasterDestroy(pOldZRaster);

	// Same for the temp camera
	RwRaster* pNewTempRaster = RwRasterCreate(nWidth, nHeight, 0, rwRASTERTYPECAMERATEXTURE | rwRASTERFORMATMIPMAP | rwRASTERFORMATAUTOMIPMAP);
	RwRaster* pNewTempZRaster = RwRasterCreate(nWidth, nHeight, 0, rwRASTERTYPEZBUFFER);
	if (pNewTempRaster && pNewTempZRaster)
	{
		RwRaster* pOldTempRaster = RwCameraGetRaster(m_pTempCamera);
		RwRaster* pOldTempZRaster = RwCameraGetZRaster(m_pTempCamera);

		RwCameraSetRaster(m_pTempCamera, NULL);
		RwCameraSetZRaster(m_pTempCamera, NULL);

		RwCameraSetRaster(m_pTempCamera, pNewTempRaster);
		RwCameraSetZRaster(m_pTempCamera, pNewTempZRaster);

		RwTextureSetRaster(m_pTempTexture, pNewTempRaster);

		RwRasterDestroy(pOldTempRaster);
		RwRasterDestroy(pOldTempZRaster);
	}
	else
	{
		if (pNewTempRaster) RwRasterDestroy(pNewTempRaster);
		if (pNewTempZRaster) RwRasterDestroy(pNewTempZRaster);
		DBO_WARNING_MESSAGE("ApplySSAA temp raster create FAIL");
	}

	// Same for the name tag texture (A8R8G8B8 text overlay)
	{
		RwRaster* pNewNameRaster = RwRasterCreate(nWidth, nHeight, 0,
			rwRASTERTYPECAMERATEXTURE | rwRASTERFORMATMIPMAP | rwRASTERFORMATAUTOMIPMAP | rwRASTERFORMAT8888);
		if (pNewNameRaster != NULL)
		{
			RwTexture* pNewNameTexture = RwTextureCreate(pNewNameRaster);
			RwTextureSetFilterMode(pNewNameTexture, rwFILTERLINEARMIPLINEAR);
			RwTextureSetAddressing(pNewNameTexture, rwTEXTUREADDRESSCLAMP);

			if (m_pNameTexture != NULL)
			{
				RwTextureDestroy(m_pNameTexture);
			}
			m_pNameTexture = pNewNameTexture;
		}
		else
		{
			DBO_WARNING_MESSAGE("ApplySSAA name texture create FAIL");
		}
	}

	// Recalculate the view window for the new raster size (same aspect ratio)
	CNtlCamera::SetCalcFov(m_pCamera, GetFov());

	BuildVertex(m_nBlurTextureWidth, m_nBlurTextureHeight);

	if (m_pHeatHaze)
	{
		m_pHeatHaze->RestoreHandler(m_pCamera);
	}
}

void Set2DVertex(RwIm2DVertex* pVertex, RwReal fX, RwReal fY, RwReal fZ, RwReal fRecipZ, RwReal fU, RwReal fV)
{
    RwIm2DVertexSetScreenX(pVertex, fX);
    RwIm2DVertexSetScreenY(pVertex, fY);
    RwIm2DVertexSetScreenZ(pVertex, fZ);
    RwIm2DVertexSetRecipCameraZ(pVertex, fRecipZ);
    RwIm2DVertexSetU(pVertex, fU, NULL);
    RwIm2DVertexSetV(pVertex, fV, NULL);
    RwIm2DVertexSetIntRGBA(pVertex, 255, 255, 255, 255); 
}

//
void SetViewVertex(RwIm2DVertex* pVertex, RwInt32 nWidht, RwInt32 nHeight, RwCamera* pCamera)
{
    int nCount = 0;
    RwReal fXUnit = (RwReal)nWidht / RES_MESH_CNT;
    RwReal fYUnit = (RwReal)nHeight / RES_MESH_CNT;    
    RwReal fNearScreenZ = RwIm2DGetNearScreenZ();
    RwReal recipCameraZ = 1.0f / RwCameraGetNearClipPlane(pCamera);
    RwReal fPos[4], fUV[4];

    for(int nY = 0; nY < RES_MESH_CNT; ++nY)
    {
        for(int nX = 0; nX < RES_MESH_CNT; ++nX)
        {
            fPos[0] = fXUnit * nX;
            fPos[1] = fXUnit * (nX + 1);
            fPos[2] = fYUnit * nY;
            fPos[3] = fYUnit * (nY + 1);            

            fUV[0] = fPos[0] / nWidht;
            fUV[1] = fPos[1] / nWidht;
            fUV[2] = fPos[2] / nHeight;
            fUV[3] = fPos[3] / nHeight;

            Set2DVertex(&pVertex[nCount++], fPos[0], fPos[2], fNearScreenZ, recipCameraZ, fUV[0], fUV[2]);
            Set2DVertex(&pVertex[nCount++], fPos[0], fPos[3], fNearScreenZ, recipCameraZ, fUV[0], fUV[3]);
            Set2DVertex(&pVertex[nCount++], fPos[1], fPos[2], fNearScreenZ, recipCameraZ, fUV[1], fUV[2]);

            Set2DVertex(&pVertex[nCount++], fPos[1], fPos[2], fNearScreenZ, recipCameraZ, fUV[1], fUV[2]);
            Set2DVertex(&pVertex[nCount++], fPos[0], fPos[3], fNearScreenZ, recipCameraZ, fUV[0], fUV[3]);
            Set2DVertex(&pVertex[nCount++], fPos[1], fPos[3], fNearScreenZ, recipCameraZ, fUV[1], fUV[3]);
        }        
    }
}

void CNtlPostEffectCamera::BuildVertex(RwInt32 width, RwInt32 height)
{
	RwIm3DVertexSetPos(&m_3dVertex[0], -1, -1, 0.f);	RwIm3DVertexSetRGBA (&m_3dVertex[0], 255, 255, 255, 0);	RwIm3DVertexSetU(&m_3dVertex[0], 0);	RwIm3DVertexSetV(&m_3dVertex[0], 1);  
	RwIm3DVertexSetPos(&m_3dVertex[1], -1,  1, 0.f);	RwIm3DVertexSetRGBA (&m_3dVertex[1], 255, 255, 255, 0);	RwIm3DVertexSetU(&m_3dVertex[1], 0);  RwIm3DVertexSetV(&m_3dVertex[1], 0);  
	RwIm3DVertexSetPos(&m_3dVertex[2],  1, -1, 0.f);	RwIm3DVertexSetRGBA (&m_3dVertex[2], 255, 255, 255, 0);	RwIm3DVertexSetU(&m_3dVertex[2], 1);  RwIm3DVertexSetV(&m_3dVertex[2], 1);  
	RwIm3DVertexSetPos(&m_3dVertex[3],  1,  1, 0.f);	RwIm3DVertexSetRGBA (&m_3dVertex[3], 255, 255, 255, 0);	RwIm3DVertexSetU(&m_3dVertex[3], 1);  RwIm3DVertexSetV(&m_3dVertex[3], 0);  

	SetRhwVertex(m_RhwVertex, 0, 0, width, height);

	width /= 3;
	height /= 3;

	SetRhwVertex(m_SourceVertex, 0, 0, width, height);
	SetRhwVertex(m_PowerVertex, 0, height+10, width, height);
	SetRhwVertex(m_FinalVertex, 0, height+10+height+10, width, height);
    SetRhwVertex(m_RhwBlurVertex, 0, 0, EFFECT_BLUR_TEXTURE_WIDTH, EFFECT_BLUR_TEXTURE_HEIGHT);
}

void CNtlPostEffectCamera::SetRhwVertex(RwIm2DVertex* pVertex, RwInt32 nStartX, RwInt32 nStartY, RwInt32 width, RwInt32 height)
{
	if (m_pCamera == NULL) return;
	RwReal recipCameraZ = 1.0f / RwCameraGetNearClipPlane(m_pCamera);

    Set2DVertex(&pVertex[0], nStartX - 0.5f, nStartY - 0.5f, RwIm2DGetNearScreenZ(), recipCameraZ, 0.0f, 0.0f);
    Set2DVertex(&pVertex[1], nStartX - 0.5f, nStartY + height - 0.5f, RwIm2DGetNearScreenZ(), recipCameraZ, 0.0f, 1.0f);
    Set2DVertex(&pVertex[2], nStartX + width - 0.5f, nStartY - 0.5f, RwIm2DGetNearScreenZ(), recipCameraZ, 1.0f, 0.0f);
    Set2DVertex(&pVertex[3], nStartX + width - 0.5f, nStartY + height - 0.5f, RwIm2DGetNearScreenZ(), recipCameraZ, 1.0f, 1.0f);
}

//------------------------------------------------------------------
//	FuncName	: CreateEffect
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
RwBool CNtlPostEffectCamera::CreateEffect()
{
	const D3DCAPS9* d3dCaps = (const D3DCAPS9 *)RwD3D9GetCaps();
    if (((d3dCaps->VertexShaderVersion & 0xffff) >= 0x0101) && ((d3dCaps->PixelShaderVersion & 0xffff) >= 0x0200))
	{
		LPD3DXBUFFER pErr=NULL;
		if(GetNtlResourcePackManager()->GetActiveFlags() & NTL_PACK_TYPE_FLAG_SCRIPT)
		{
			BYTE*	pPackBuffer	= NULL;
			RwInt32	iPackSize	= 0;

			GetNtlResourcePackManager()->LoadScript(m_strShaderName.c_str(), (void**)&pPackBuffer, &iPackSize);
			if (pPackBuffer)
			{
				BYTE* pTempBuffer = NTL_NEW BYTE [iPackSize + 1];
				memcpy(pTempBuffer, pPackBuffer, iPackSize);
				pTempBuffer[iPackSize] = '\0';

				if (FAILED(D3DXCreateEffect(m_lpDevice, pTempBuffer, iPackSize + 1, NULL, NULL, 0, NULL, &m_lpEffect, &pErr)))
				{
					if (pErr)
					{
						RwChar* Error = (RwChar*)pErr->GetBufferPointer();
						DBO_WARNING_MESSAGE("Error " << Error);
						pErr->Release();
					}
					NTL_ARRAY_DELETE(pPackBuffer);
					NTL_ARRAY_DELETE(pTempBuffer);
					return FALSE;
                }

				NTL_ARRAY_DELETE(pPackBuffer);
				NTL_ARRAY_DELETE(pTempBuffer);
			}
			else
			{
				DBO_WARNING_MESSAGE("pPackBuffer == NULL");
			}
		}
		else
		{
			if (FAILED(D3DXCreateEffectFromFile(m_lpDevice, m_strShaderName.c_str(), NULL, NULL, 0, NULL, &m_lpEffect, &pErr)))
			{
				if (pErr)
				{
					RwChar* Error = (RwChar*)pErr->GetBufferPointer();
					DBO_WARNING_MESSAGE("Error " << Error);
					pErr->Release();
				}
				return FALSE;
			}
		}

		if ( NULL == m_lpEffect )
		{
			DBO_WARNING_MESSAGE("m_lpEffect == NULL");
			return FALSE;
		}

		m_hTechnique		= m_lpEffect->GetTechniqueByName("TShader");					NTL_ASSERTE(m_hTechnique);

		m_hMapWidth			= m_lpEffect->GetParameterByName(NULL, "MAP_WIDTH");			NTL_ASSERTE(m_hMapWidth);
		m_hMapHeight		= m_lpEffect->GetParameterByName(NULL, "MAP_HEIGHT");			NTL_ASSERTE(m_hMapHeight);

		m_hMonoPowerHeight	= m_lpEffect->GetParameterByName(NULL, "mono_power_height");	NTL_ASSERTE(m_hMonoPowerHeight);
		m_hRGB2Y			= m_lpEffect->GetParameterByName(NULL, "RGB2Y");				NTL_ASSERTE(m_hRGB2Y);

		m_lpEffect->SetFloat(m_hMapWidth, (RwReal)m_nBlurTextureWidth);
		m_lpEffect->SetFloat(m_hMapHeight, (RwReal)m_nBlurTextureHeight);

		m_lpEffect->SetFloat(m_hMonoPowerHeight, m_fMonoPowerHeight);

		m_lpEffect->SetVector(m_hRGB2Y, &m_vRGB2Y);
	}

	// FXAA effect - compiled from embedded source so no loose shader file is
	// needed (the game packs its .fx files). Failure is non-fatal: FXAA is
	// simply disabled.
	{
		LPD3DXBUFFER pErrFXAA = NULL;

		if (m_lpFXAAEffect != NULL)
		{
			m_lpFXAAEffect->Release();
			m_lpFXAAEffect = NULL;
		}
		m_hFXAATechnique = NULL;
		m_hFXAARCPFrame = NULL;
		m_hGaussianTechnique = NULL;

		if (SUCCEEDED(D3DXCreateEffect(m_lpDevice, FXAA_SHADER_SOURCE, (UINT)strlen(FXAA_SHADER_SOURCE), NULL, NULL, 0, NULL, &m_lpFXAAEffect, &pErrFXAA)))
		{
			m_hFXAATechnique = m_lpFXAAEffect->GetTechniqueByName("FXAA");
			m_hFXAARCPFrame = m_lpFXAAEffect->GetParameterByName(NULL, "RCPFRAME");
			m_hGaussianTechnique = m_lpFXAAEffect->GetTechniqueByName("GAUSSIAN");
		}
		else
		{
			DBO_WARNING_MESSAGE("FXAA effect compile FAILED");
			if (pErrFXAA)
			{
				RwChar* Error = (RwChar*)pErrFXAA->GetBufferPointer();
				DBO_WARNING_MESSAGE("FXAA effect error: " << Error);
				pErrFXAA->Release();
			}
			m_lpFXAAEffect = NULL;
		}
	}

	return TRUE;
}

//------------------------------------------------------------------
//	FuncName	: UpdateBlurFilter
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CNtlPostEffectCamera::UpdateBlurFilter()
{
	if(!IsEnablePostEffect())
		return;

	m_lpEffect->SetTechnique(m_hTechnique);
	m_lpEffect->Begin(NULL,0);	

	Update_Power_MonoPower();
	Update_FakeHDRFiltering();

	m_lpEffect->End();
}

//------------------------------------------------------------------
//	FuncName	: Update_Power_MonoPower
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CNtlPostEffectCamera::Update_Power_MonoPower()
{
	// apply current bloom effect mono factor
	m_fMonoPowerHeight = GetSceneManager()->GetActiveBloomFactor();

	if (SUCCEEDED(m_lpEffect->BeginPass(EFFECT_PASS_MONO_POWER)))
	{
		m_lpEffect->SetFloat(m_hMonoPowerHeight, m_fMonoPowerHeight);
		m_lpEffect->SetVector(m_hRGB2Y, &m_vRGB2Y);
		m_lpEffect->CommitChanges();

		if (RwCameraClear(m_pTempCamera, &m_BackgroundColor, rwCAMERACLEARZ|rwCAMERACLEARIMAGE))
		{
			if(RwCameraBeginUpdate(m_pTempCamera))
			{
				BeginPostEffectState(FALSE);

				RwD3D9SetTexture(m_pSourceTexture, 0);
				RwIm2DRenderPrimitive(rwPRIMTYPETRISTRIP, m_RhwVertex, 4);

				EndPostEffectState(FALSE);

				RwCameraEndUpdate(m_pTempCamera);
			}
		}

		m_lpEffect->EndPass();
	}
}

//------------------------------------------------------------------
//	FuncName	: Update_FakeHDRFiltering
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CNtlPostEffectCamera::Update_FakeHDRFiltering()
{
	// ==========================================
	//
	// ==========================================
	if (SUCCEEDED(m_lpEffect->BeginPass(EFFECT_PASS_NONE)))
	{
		m_lpEffect->CommitChanges();
		if (RwCameraClear(m_pReductionCamera, &m_BackgroundColor, rwCAMERACLEARZ|rwCAMERACLEARIMAGE))
		{
			if(RwCameraBeginUpdate(m_pReductionCamera))
			{
				// camera
				RwRenderStateSet(rwRENDERSTATECULLMODE,	(void *)rwCULLMODECULLNONE);

				BeginPostEffectState(FALSE);

				RwD3D9SetTexture(m_pTempTexture, 0);
				RwIm2DRenderPrimitive(rwPRIMTYPETRISTRIP, m_RhwReductionVertex, 4);

				EndPostEffectState(FALSE);

				RwRenderStateSet(rwRENDERSTATECULLMODE,	(void *)rwCULLMODECULLBACK);

				RwCameraEndUpdate(m_pReductionCamera);
			}
		}
		m_lpEffect->EndPass();	
	}

	// ==========================================
	//
	// ==========================================
	if (SUCCEEDED(m_lpEffect->BeginPass(EFFECT_PASS_16BOX)))
	{
		m_lpEffect->SetFloat(m_hMapWidth, (RwReal)m_nTextureReductionSize);
		m_lpEffect->SetFloat(m_hMapHeight, (RwReal)m_nTextureReductionSize);
		m_lpEffect->CommitChanges();
		if (RwCameraClear(m_pFakeHDRCamera, &m_BackgroundColor, rwCAMERACLEARZ|rwCAMERACLEARIMAGE))
		{
			if(RwCameraBeginUpdate(m_pFakeHDRCamera))
			{
				// camera
				RwRenderStateSet(rwRENDERSTATECULLMODE,	(void *)rwCULLMODECULLNONE);

				//programmable vertex Pipeline
				//Test

				// CzStateChange : Start
				// CzStateChange : Old
				//RwD3D9SetTextureStageState (0, D3DTSS_TEXCOORDINDEX, 0);
				//RwD3D9SetTextureStageState (1, D3DTSS_TEXCOORDINDEX, 1);

				// CzStateChange : New
				RwD3D9SetTextureStageState (0, D3DTSS_TEXCOORDINDEX, 0);
				RwD3D9SetTextureStageState (1, D3DTSS_TEXCOORDINDEX, 1);
				RwD3D9SetTextureStageState (2, D3DTSS_TEXCOORDINDEX, 2);
				RwD3D9SetTextureStageState (3, D3DTSS_TEXCOORDINDEX, 3);
				// CzStateChange : End

				RwD3D9SetTexture(m_pReductionTexture, 0);
				if(RwIm3DTransform(m_3dVertex, 4, NULL, rwIM3D_VERTEXXYZ|rwIM3D_VERTEXUV))
				{
					RwIm3DRenderPrimitive(rwPRIMTYPETRISTRIP);        
					RwIm3DEnd();
				}

				RwRenderStateSet(rwRENDERSTATECULLMODE,	(void *)rwCULLMODECULLBACK);

				RwCameraEndUpdate(m_pFakeHDRCamera);
			}
		}
		m_lpEffect->EndPass();
	}
	
	if (SUCCEEDED(m_lpEffect->BeginPass(EFFECT_PASS_16BOX)))
	{
		m_lpEffect->SetFloat(m_hMapWidth, (RwReal)m_nTextureReductionSize);
		m_lpEffect->SetFloat(m_hMapHeight, (RwReal)m_nTextureReductionSize);
		m_lpEffect->CommitChanges();
		if (RwCameraClear(m_pReductionCamera, &m_BackgroundColor, rwCAMERACLEARZ|rwCAMERACLEARIMAGE))
		{
			if(RwCameraBeginUpdate(m_pReductionCamera))
			{
				// camera
				RwRenderStateSet(rwRENDERSTATECULLMODE,	(void *)rwCULLMODECULLNONE);

				// programmable vertex Pipeline
				// Test

				// CzStateChange : Start
				// CzStateChange : Old
				//RwD3D9SetTextureStageState (0, D3DTSS_TEXCOORDINDEX, 0);
				//RwD3D9SetTextureStageState (1, D3DTSS_TEXCOORDINDEX, 1);

				// CzStateChange : New
				RwD3D9SetTextureStageState (0, D3DTSS_TEXCOORDINDEX, 0);
				RwD3D9SetTextureStageState (1, D3DTSS_TEXCOORDINDEX, 1);
				RwD3D9SetTextureStageState (2, D3DTSS_TEXCOORDINDEX, 2);
				RwD3D9SetTextureStageState (3, D3DTSS_TEXCOORDINDEX, 3);
				// CzStateChange : End

				RwD3D9SetTexture(m_pFakeHDRTexture, 0);
				if(RwIm3DTransform(m_3dVertex, 4, NULL, rwIM3D_VERTEXXYZ|rwIM3D_VERTEXUV))
				{
					RwIm3DRenderPrimitive(rwPRIMTYPETRISTRIP);        
					RwIm3DEnd();
				}

				RwRenderStateSet(rwRENDERSTATECULLMODE,	(void *)rwCULLMODECULLBACK);

				RwCameraEndUpdate(m_pReductionCamera);
			}
		}
		m_lpEffect->EndPass();
	}
	
	// ==========================================
	//
	// ==========================================
	if (SUCCEEDED(m_lpEffect->BeginPass(EFFECT_PASS_NONE)))
	{
		m_lpEffect->CommitChanges();
		if (RwCameraClear(m_pTempCamera, &m_BackgroundColor, rwCAMERACLEARZ|rwCAMERACLEARIMAGE))
		{
			if(RwCameraBeginUpdate(m_pTempCamera))
			{
				// camera
				RwRenderStateSet(rwRENDERSTATECULLMODE,	(void *)rwCULLMODECULLNONE);

				BeginPostEffectState(FALSE);

				RwD3D9SetTexture(m_pReductionTexture, 0);
				RwIm2DRenderPrimitive(rwPRIMTYPETRISTRIP, m_RhwVertex, 4);

				EndPostEffectState(FALSE);

				RwRenderStateSet(rwRENDERSTATECULLMODE,	(void *)rwCULLMODECULLBACK);

				RwCameraEndUpdate(m_pTempCamera);
			}
		}
		m_lpEffect->EndPass();
	}
}	


//-------------------------------------------------------------
// Name: CreateReductionTexture()
// Desc:
//-------------------------------------------------------------
void CNtlPostEffectCamera::CreateReductionTexture(RwInt32 nTextureSize)
{
	if (m_pReductionCamera != NULL)
	{
		DestroyBlurCamera(m_pReductionCamera, m_pReductionTexture);
	}
	if (m_pFakeHDRCamera != NULL)
	{
		DestroyBlurCamera(m_pFakeHDRCamera, m_pFakeHDRTexture);
	}

	SetRhwVertex(m_RhwReductionVertex, 0, 0, nTextureSize, nTextureSize);

	if (!CreateBlurCamera(m_pReductionCamera, m_pReductionTexture, nTextureSize, nTextureSize, 0.1f, 1000.f))
	{
		return;
	}
	if (!CreateBlurCamera(m_pFakeHDRCamera, m_pFakeHDRTexture, nTextureSize, nTextureSize, 0.1f, 1000.f))
	{
		return;
	}
}

//------------------------------------------------------------------
//	FuncName	: DrawTexture
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CNtlPostEffectCamera::DrawTexture()
{
	RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)TRUE);

	// Source Texture
	RwD3D9SetTexture(m_pSourceTexture, 0);
	RwIm2DRenderPrimitive(rwPRIMTYPETRISTRIP, m_SourceVertex, 4);

	// Source Texture
	RwD3D9SetTexture(m_pReductionTexture, 0);
	RwIm2DRenderPrimitive(rwPRIMTYPETRISTRIP, m_PowerVertex, 4);

	// Source Texture
	RwD3D9SetTexture(m_pTempTexture, 0);
	RwIm2DRenderPrimitive(rwPRIMTYPETRISTRIP, m_FinalVertex, 4);

	RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)FALSE);
}

void CNtlPostEffectCamera::SetCameraSize(RwRect* rect)
{
	if (rect != NULL)
	{
		m_nBlurTextureWidth = rect->w;
		m_nBlurTextureHeight= rect->h;
	}

	if (m_pTempCamera != NULL)
		CNtlCamera::SetCameraResize(m_pTempCamera, rect);
	if (m_pMainCamera != NULL)
		CNtlCamera::SetCameraResize(m_pMainCamera, rect);
	if (m_pCamera != NULL)
	{
		// SSAA/FSR: the world camera raster scales with the render scale.
		RwReal fRenderScale = GetRenderScale();

		if (rect != NULL && fRenderScale != 1.0f)
		{
			RwRect scaledRect;
			scaledRect.x = rect->x;
			scaledRect.y = rect->y;
			scaledRect.w = (RwInt32)(rect->w * fRenderScale);
			scaledRect.h = (RwInt32)(rect->h * fRenderScale);
			CNtlCamera::SetCameraResize(m_pCamera, &scaledRect);
		}
		else
		{
			CNtlCamera::SetCameraResize(m_pCamera, rect);
		}
	}
	if (m_pHeatHazeCamera != NULL)
		CNtlCamera::SetCameraResize(m_pHeatHazeCamera, rect);


	BuildVertex(m_nBlurTextureWidth, m_nBlurTextureHeight);

	if(m_pHeatHaze)
	{
		m_pHeatHaze->SetScrSize(rect);
	}

	RestoreHandler();
}

int CNtlPostEffectCamera::ReleaseHandler()
{
	if (m_lpEffect != NULL)
	{
		m_lpEffect->Release();
		m_lpEffect = NULL;

		m_hTechnique = NULL;
	}

	if (m_lpFXAAEffect != NULL)
	{
		m_lpFXAAEffect->Release();
		m_lpFXAAEffect = NULL;

		m_hFXAATechnique = NULL;
		m_hFXAARCPFrame = NULL;
		m_hGaussianTechnique = NULL;
	}


	if(m_pHeatHaze)
	{
		m_pHeatHaze->ReleaseHandler();
	}

	return TRUE;
}

int CNtlPostEffectCamera::RestoreHandler()
{
	if (!m_bEnableCamera) return TRUE;

	if (m_lpEffect != NULL)
	{
		m_lpEffect->Release();
		m_lpEffect = NULL;

		m_hTechnique = NULL;
	}

	if (m_lpFXAAEffect != NULL)
	{
		m_lpFXAAEffect->Release();
		m_lpFXAAEffect = NULL;

		m_hFXAATechnique = NULL;
		m_hFXAARCPFrame = NULL;
		m_hGaussianTechnique = NULL;
	}


	m_lpDevice = (LPDIRECT3DDEVICE9)RwD3D9GetCurrentD3DDevice();

	if (!CreateEffect())
	{
		return NULL;
	}

	if(m_pHeatHaze)
	{
		m_pHeatHaze->RestoreHandler(m_pCamera);
	}

	return TRUE;
}

RwBool CNtlPostEffectCamera::IsEnablePostEffect(void)
{
	if (IsMSAAMode())
		return FALSE;

	if(m_lpEffect == NULL)
		return FALSE;

	if(GetPostEffectFilters() == POST_EFFECT_FILTER_NONE)
		return FALSE;

	if(!GetSceneManager()->GetWorld())
		return FALSE;

	return TRUE;
}

RwCamera* CNtlPostEffectCamera::CameraClear(RwRGBA *rgba, RwInt32 uiClearMode)
{
	// MSAA mode: the world renders straight to the backbuffer.
	if (IsMSAAMode())
	{
		if (m_pMainCamera)
			return RwCameraClear(m_pMainCamera, rgba, uiClearMode);
		return NULL;
	}

	// image filteing camera
	if(m_pCamera)
		return RwCameraClear(m_pCamera, rgba, uiClearMode);
	return NULL;
}

void CNtlPostEffectCamera::CameraShowRaster(HWND hWnd, RwUInt32 uiFlags)
{
	// main
	RwCameraShowRaster(m_pMainCamera, hWnd, uiFlags);
}


RwCamera* CNtlPostEffectCamera::PostCameraBeginUpdate(RwReal fElapsed)
{
	// MSAA mode: begin the main (backbuffer) camera - the world renders
	// directly into the multisampled backbuffer.
	if (IsMSAAMode())
	{
		// The main camera must share the world camera's view so the world
		// projects correctly (no composite pass happens in MSAA mode).
		if (m_pCamera != NULL && m_pMainCamera != NULL)
		{
			RwFrame* pWorldFrame = RwCameraGetFrame(m_pCamera);
			RwFrame* pMainFrame = RwCameraGetFrame(m_pMainCamera);
			if (pWorldFrame && pMainFrame)
			{
				RwMatrix* pWorldLTM = RwFrameGetLTM(pWorldFrame);
				RwFrameTransform(pMainFrame, pWorldLTM, rwCOMBINEREPLACE);
			}

			// The game updates the FOV only on the world camera (the game
			// camera manager holds the base camera type and SetCalcFov is
			// not virtual), so the main camera's view window goes stale.
			// Mirror it so the world does not render zoomed in MSAA mode.
			const RwV2d* pViewWindow = RwCameraGetViewWindow(m_pCamera);
			if (pViewWindow != NULL)
			{
				RwV2d vw = *pViewWindow;
				RwCameraSetViewWindow(m_pMainCamera, &vw);
			}
		}

		// World lighting: the D3D9 driver derives the active D3D lights from
		// the camera's attached world (the world plugin's camera begin-update
		// sets the driver's current world). The stages only attach the world
		// camera, so attach the main camera too while it renders the world;
		// without this the lighting callback finds no world and non-prelit
		// atomics render black.
		if (m_pMainCamera != NULL && m_pMainCameraWorld != CNtlPLGlobal::m_pRpWorld)
		{
			if (m_pMainCameraWorld != NULL)
			{
				RpWorldRemoveCamera(m_pMainCameraWorld, m_pMainCamera);
			}
			if (CNtlPLGlobal::m_pRpWorld != NULL)
			{
				RpWorldAddCamera(CNtlPLGlobal::m_pRpWorld, m_pMainCamera);
			}
			m_pMainCameraWorld = CNtlPLGlobal::m_pRpWorld;
		}

		RwCamera* pResult = RwCameraBeginUpdate(m_pMainCamera);
		m_bMainCameraBegun = (pResult != NULL);

		// The driver never sets this state: without it the GPU renders only
		// to sample 0 and the MSAA resolve averages with the cleared samples
		// (black) - causing black fringes/buggy objects. Enable real
		// multisampling while MSAA mode is active.
		if (pResult != NULL)
		{
			RwD3D9SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, TRUE);
		}

		return pResult;
	}

	if(m_pCamera)
	{
		RwCamera* pResult = RwCameraBeginUpdate(m_pCamera);
		// Restore the default (no multisampling) when not in MSAA mode.
		RwD3D9SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, FALSE);
		return pResult;
	}
	else
	{
		RwCamera* pResult = RwCameraBeginUpdate(m_pMainCamera);
		RwD3D9SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, FALSE);
		return pResult;
	}
}

void CNtlPostEffectCamera::PostCameraEndUpdate(void)
{
	// MSAA mode: keep the camera begun until the UI is done (the end happens
	// in MainCameraEndUpdate).
	if (IsMSAAMode())
		return;

	if(m_pCamera)
		RwCameraEndUpdate(m_pCamera);
	else
		RwCameraEndUpdate(m_pMainCamera);
}

RwCamera* CNtlPostEffectCamera::MainCameraBeginUpdate(RwReal fElapsed)
{
	// MSAA mode: the main camera was already begun in PostCameraBeginUpdate
	// (the world rendered directly into the backbuffer). Just return it so
	// the UI renders on top; MainCameraEndUpdate performs the end.
	if (IsMSAAMode())
		return m_pMainCamera;

	// The main (backbuffer) camera must share the world camera's view so that
	// world-space billboards deferred past the composite (name tags, damage
	// text) project correctly.
	if (m_pCamera != NULL && m_pMainCamera != NULL)
	{
		RwFrame* pWorldFrame = RwCameraGetFrame(m_pCamera);
		RwFrame* pMainFrame = RwCameraGetFrame(m_pMainCamera);
		if (pWorldFrame && pMainFrame)
		{
			RwMatrix* pWorldLTM = RwFrameGetLTM(pWorldFrame);
			RwFrameTransform(pMainFrame, pWorldLTM, rwCOMBINEREPLACE);
		}
	}

	if(RwCameraBeginUpdate(m_pMainCamera))
	{
		if (IsEnablePostEffect())
		{
			if (IsAAFilterEnabled())
			{
				// AA filter replaces the fixed-function composite quad.
				BeginPostEffectState(TRUE);

				RenderAAModeComposite();

				EndPostEffectState(TRUE);
			}
			else
			{
			//m_lpEffect->SetTechnique(m_hTechnique);
			//m_lpEffect->Begin(NULL,0);	
			//m_lpEffect->BeginPass(EFFECT_PASS_LASTDRAW);
			//m_lpEffect->CommitChanges();
			
			// Test
			RwD3D9SetTextureStageState (0, D3DTSS_COLOROP,		D3DTOP_SELECTARG1);
			RwD3D9SetTextureStageState (0, D3DTSS_COLORARG1,	D3DTA_TEXTURE);
			
			RwD3D9SetTextureStageState (0, D3DTSS_ALPHAOP,		D3DTOP_SELECTARG1);
			RwD3D9SetTextureStageState (0, D3DTSS_ALPHAARG1,	D3DTA_TEXTURE);
			
			RwD3D9SetTextureStageState (1, D3DTSS_COLOROP,		D3DTOP_ADD);
			RwD3D9SetTextureStageState (1, D3DTSS_COLORARG1,	D3DTA_CURRENT); 
			RwD3D9SetTextureStageState (1, D3DTSS_COLORARG2,	D3DTA_TEXTURE);

			RwD3D9SetTextureStageState (1, D3DTSS_ALPHAOP,		D3DTOP_ADD);
			RwD3D9SetTextureStageState (1, D3DTSS_ALPHAARG1,	D3DTA_CURRENT); 
			RwD3D9SetTextureStageState (1, D3DTSS_ALPHAARG2,	D3DTA_TEXTURE);

			RwD3D9SetTextureStageState (1, D3DTSS_TEXCOORDINDEX, 0);

            RwD3D9SetTexture(m_pSourceTexture, 0);
			RwD3D9SetTexture(m_pTempTexture, 1);

			//BeginPostEffectState(TRUE);
	
			RwIm2DRenderPrimitive(rwPRIMTYPETRISTRIP, m_RhwVertex, 4);
			
			EndPostEffectState(TRUE);

			//RwD3D9SetTexture(NULL, 0);
			//RwD3D9SetTexture(NULL, 1);
			
			//m_lpEffect->EndPass();
			//m_lpEffect->End();
			}
		}
		else
		{
			BeginPostEffectState(FALSE);

			if (IsAAFilterEnabled())
			{
				RenderAAModeComposite();
			}
			else if (m_nSSAAScale > 1 && m_lpFXAAEffect && m_hGaussianTechnique)
			{
				// SSAA: Gaussian 5x5 downscale of the 2x source.
				RenderSSAAComposite();
			}
			else
			{
				RwD3D9SetTexture(m_pSourceTexture, 0);

				RwIm2DRenderPrimitive(rwPRIMTYPETRISTRIP, m_RhwVertex, 4);
			}

			EndPostEffectState(FALSE);
		}

		// Name tags / damage text overlay: alpha-blended over the (possibly
		// FXAA'd) world, with per-pixel alpha from the A8R8G8B8 texture.
		if (IsAAFilterEnabled() && m_pNameTexture != NULL)
		{
			RwD3D9SetTextureStageState (0, D3DTSS_COLOROP,		D3DTOP_SELECTARG1);
			RwD3D9SetTextureStageState (0, D3DTSS_COLORARG1,	D3DTA_TEXTURE);

			RwD3D9SetTextureStageState (0, D3DTSS_ALPHAOP,		D3DTOP_SELECTARG1);
			RwD3D9SetTextureStageState (0, D3DTSS_ALPHAARG1,	D3DTA_TEXTURE);

			RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)TRUE);
			RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)rwBLENDSRCALPHA);
			RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)rwBLENDINVSRCALPHA);
			RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void*)FALSE);
			RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void*)FALSE);

			RwD3D9SetTexture(m_pNameTexture, 0);
			RwIm2DRenderPrimitive(rwPRIMTYPETRISTRIP, m_RhwVertex, 4);

			RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)FALSE);
			RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void*)TRUE);
			RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void*)TRUE);
		}

		// LensFlare Render (by agebreak 2007.4.23)
		for each(CNtlPLLensFlare* pLensFlare in m_listLensFlare)
		{
			if(pLensFlare && pLensFlare->IsVisible())
			{
				pLensFlare->Render();
			}
		}		

		// Post Effect Render		
        for each(CNtlInstancePostEffectSystem* pPostEffectSystem in m_listPostEffectSystem)
        {
            if(pPostEffectSystem->GetComicState() != CNtlInstancePostEffectSystem::COMIC_STATE_IDLE)
            {
                pPostEffectSystem->RenderPostEffect();
            }
        }

		return m_pMainCamera;
	}

	return NULL;
}

void CNtlPostEffectCamera::MainCameraEndUpdate(void)
{
	// MSAA mode: only end if the camera was actually begun (the begin can
	// fail on device loss - ending an unbegun camera would crash the driver).
	if (IsMSAAMode())
	{
		if (m_bMainCameraBegun)
		{
			RwCameraEndUpdate(m_pMainCamera);
			m_bMainCameraBegun = FALSE;
		}
		return;
	}

	RwCameraEndUpdate(m_pMainCamera);
}

void CNtlPostEffectCamera::UpdatePostEffectFilter(void)
{
	// MSAA mode: no post-processing (the world is already in the backbuffer).
	if (IsMSAAMode())
		return;

	NTL_SPROFILE("CNtlPostEffectCamera::UpdatePostEffectFilter")

	UpdateBlurFilter();

	NTL_EPROFILE()
}

void CNtlPostEffectCamera::SetPostEffectFilters( RwUInt8 byPEFilters )
{
	m_byPEFilters = byPEFilters; 
}

RwUInt8	CNtlPostEffectCamera::GetPostEffectFilters(void)	
{ 
	return m_byPEFilters;
}

void CNtlPostEffectCamera::UpdateHeatHaze(RwReal fElapsedTime)
{
	// MSAA mode: no post-processing.
	if (IsMSAAMode())
		return;

	if(!m_pHeatHaze || !m_pHeatHaze->GetEnabled())
	{
		return;
	}

	if (m_pHeatHaze->Update(fElapsedTime, m_pHeatHazeCamera))
	{
		m_pHeatHaze->Render(m_pSourceTexture, m_pHeatHazeCamera);
	
		// take the frame
		RwIm2DVertex rhwVertex[4];
		memcpy(rhwVertex, m_RhwVertex, sizeof(RwIm2DVertex) * 4);
		for(int i = 0; i < 4; ++i)
		{
			RwIm2DVertexSetRealRGBA(&rhwVertex[i], 255, 255, 255, m_fBlurAlpha);
		}

		// SSAA: the world camera renders at Nx, so the overlay quad must cover
		// the full render target.
		{
			RwReal fRenderScale = GetRenderScale();
			RwInt32 nOverlayW = (RwInt32)(m_nBlurTextureWidth * fRenderScale);
			RwInt32 nOverlayH = (RwInt32)(m_nBlurTextureHeight * fRenderScale);

			RwIm2DVertexSetScreenX(&rhwVertex[0], -0.5f);
			RwIm2DVertexSetScreenY(&rhwVertex[0], -0.5f);
			RwIm2DVertexSetScreenX(&rhwVertex[1], -0.5f);
			RwIm2DVertexSetScreenY(&rhwVertex[1], (RwReal)(nOverlayH) - 0.5f);
			RwIm2DVertexSetScreenX(&rhwVertex[2], (RwReal)(nOverlayW) - 0.5f);
			RwIm2DVertexSetScreenY(&rhwVertex[2], -0.5f);
			RwIm2DVertexSetScreenX(&rhwVertex[3], (RwReal)(nOverlayW) - 0.5f);
			RwIm2DVertexSetScreenY(&rhwVertex[3], (RwReal)(nOverlayH) - 0.5f);
		}

		if(RwCameraBeginUpdate(m_pCamera))
		{
			RwD3D9SetTexture(m_pHeatHazeTexture, 0);
			RwIm2DRenderPrimitive(rwPRIMTYPETRISTRIP, rhwVertex, 4);

			EndBlurEffect();

			RwCameraEndUpdate(m_pCamera);       
		}
	}
}									

void CNtlPostEffectCamera::UpdateMotionBlur(RwReal fElapsedTime)
{
	// MSAA mode: no post-processing.
	if (IsMSAAMode())
		return;

	NTL_SPROFILE("CNtlPostEffectCamera::UpdateMotionBlur")

    if(!m_bEnableBlur || !m_pMotionBlurCamera || !m_pMotionBlurTexture)
	{
		NTL_RPROFILE_VOID()
	}

    //
    if(m_fBlurAlpha > EFFECT_BLUR_ALPHA)
    {
        m_fBlurAlpha -= (m_fBlurFadeVelocity * fElapsedTime);
        if(m_fBlurAlpha < EFFECT_BLUR_ALPHA)
        {
            m_fBlurAlpha = EFFECT_BLUR_ALPHA;
        }
    }

    //
    for(int i = 0; i < 4; ++i)
    {
        RwIm2DVertexSetRealRGBA(&m_RhwBlurVertex[i], 255, 255, 255, m_fBlurAlpha);
    }

    if(RwCameraBeginUpdate(m_pMotionBlurCamera))
    {
        BeginBlurEffect();

        RwD3D9SetTexture(m_pSourceTexture, 0);
        RwIm2DRenderPrimitive(rwPRIMTYPETRISTRIP, m_RhwBlurVertex, 4);

        RwCameraEndUpdate(m_pMotionBlurCamera);
    }

    //
    RwIm2DVertex rhwVertex[4];
    memcpy(rhwVertex, m_RhwVertex, sizeof(RwIm2DVertex) * 4);
    for(int i = 0; i < 4; ++i)
    {
        RwIm2DVertexSetRealRGBA(&rhwVertex[i], 255, 255, 255, m_fBlurAlpha);
    }

    // SSAA: the world camera renders at Nx, so the blur overlay quad must
    // cover the full render target.
    {
        RwReal fRenderScale = GetRenderScale();
        RwInt32 nOverlayW = (RwInt32)(m_nBlurTextureWidth * fRenderScale);
        RwInt32 nOverlayH = (RwInt32)(m_nBlurTextureHeight * fRenderScale);

        RwIm2DVertexSetScreenX(&rhwVertex[0], -0.5f);
        RwIm2DVertexSetScreenY(&rhwVertex[0], -0.5f);
        RwIm2DVertexSetScreenX(&rhwVertex[1], -0.5f);
        RwIm2DVertexSetScreenY(&rhwVertex[1], (RwReal)(nOverlayH) - 0.5f);
        RwIm2DVertexSetScreenX(&rhwVertex[2], (RwReal)(nOverlayW) - 0.5f);
        RwIm2DVertexSetScreenY(&rhwVertex[2], -0.5f);
        RwIm2DVertexSetScreenX(&rhwVertex[3], (RwReal)(nOverlayW) - 0.5f);
        RwIm2DVertexSetScreenY(&rhwVertex[3], (RwReal)(nOverlayH) - 0.5f);
    }

    if(RwCameraBeginUpdate(m_pCamera))
    {
        RwD3D9SetTexture(m_pMotionBlurTexture, 0);
        RwIm2DRenderPrimitive(rwPRIMTYPETRISTRIP, rhwVertex, 4);

        EndBlurEffect();

        RwCameraEndUpdate(m_pCamera);       
    }

	NTL_EPROFILE()
}

void CNtlPostEffectCamera::SetEnableBlur(RwBool bEnable, RwReal fFadeTime /* = 0.0f */)
{
    if(fFadeTime == 0.0f)
    {
        m_fBlurAlpha = EFFECT_BLUR_ALPHA;
        m_fBlurFadeVelocity = 0.0f;
    }
    else
    {
        m_fBlurAlpha = 255.0f;
        m_fBlurFadeVelocity = (255.0f - EFFECT_BLUR_ALPHA) / fFadeTime;
    }

    if(!m_bEnableBlur && bEnable)
    {
		if (RwCameraClear(m_pMotionBlurCamera, &m_BackgroundColor, rwCAMERACLEARZ|rwCAMERACLEARIMAGE))
		{
			//
			for(int i = 0; i < 4; ++i)
			{
				RwIm2DVertexSetRealRGBA(&m_RhwBlurVertex[i], 255, 255, 255, 255);
			}

			if(RwCameraBeginUpdate(m_pMotionBlurCamera))
			{
				BeginBlurEffect();

				RwD3D9SetTexture(m_pSourceTexture, 0);
				RwIm2DRenderPrimitive(rwPRIMTYPETRISTRIP, m_RhwBlurVertex, 4);

				EndBlurEffect();

				RwCameraEndUpdate(m_pMotionBlurCamera);
			}
		}
    }

    m_bEnableBlur = bEnable;
}

RwV2d CNtlPostEffectCamera::GetScreenSize() 
{
    RwV2d v2dSize;
    v2dSize.x = (RwReal)m_nBlurTextureWidth;
    v2dSize.y = (RwReal)m_nBlurTextureHeight;

    return v2dSize;
}

void CNtlPostEffectCamera::AttachPostEffectSystem( CNtlInstancePostEffectSystem* pPostEffectSystem ) 
{
	if(!pPostEffectSystem)
		return;

    m_listPostEffectSystem.push_back(pPostEffectSystem);
}

void CNtlPostEffectCamera::DetachPostEffectSystem( CNtlInstancePostEffectSystem* pPostEffectSystem ) 
{
	if(!pPostEffectSystem)
		return;

	m_listPostEffectSystem.remove(pPostEffectSystem);
}

void CNtlPostEffectCamera::AttachLensFlare( CNtlPLLensFlare* pLensFlare ) 
{
	if(!pLensFlare)
		return;

	m_listLensFlare.push_back(pLensFlare);
}

void CNtlPostEffectCamera::DetachLensFlare( CNtlPLLensFlare* pLensFlare ) 
{
	if(!pLensFlare)
		return;

	m_listLensFlare.remove(pLensFlare);
}

void CNtlPostEffectCamera::SetCalcFov( RwReal fFov ) 
{
    CNtlCamera::SetCalcFov(m_pCamera, fFov);
    CNtlCamera::SetCalcFov(m_pMainCamera, fFov);
    CNtlCamera::SetCalcFov(m_pTempCamera, fFov);
    CNtlCamera::SetCalcFov(m_pMotionBlurCamera, fFov);
}
