////////////////////////////////////////////////////////////////////////////////
// File: NtlFlasherManager.h
// Desc: GFx Scaleform Integration Manager.
//
// 2006.07.13 Peessi@hitel.net   
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __NTL_FLASHER_MANAGER_H__
#define __NTL_FLASHER_MANAGER_H__

// GFx
#include "GFile.h"
#include "GFxPlayer.h"
#include "GFxLoader.h"
#include "GRendererD3D9.h"
#include "GFxFontLib.h"

#include "NtlDeviceRestoreHandler.h"

class CNtlFlasherManager
{
public:
	//! Enumuration

	//! Destructor
	~CNtlFlasherManager(VOID);

	//! Operation
	BOOL CreateInstance( HWND hWnd );
	VOID DeleteInstance(VOID);
	VOID ResetRenderer(VOID);

	VOID SetVideoMode(VOID);

	BOOL IsEnableRender(VOID) { return m_bRendererEnable; }

	VOID SetFontLibData( std::string& strLibFileName );
	VOID SetFontMapData( std::string& strMapName, std::string& strFontname, std::string& strStyle );

	GPtr<GFxFontLib> GetFontLib(VOID) { return m_pFontLib; }
	GPtr<GFxFontMap> GetFontMap(VOID) { return m_pFontMap; }

	VOID SetScreenSize( INT nWidth, INT nHeight ) { m_nScreenWidth = nWidth; m_nScreenHeight = nHeight; }
	INT	GetScreenWidth(VOID) { return m_nScreenWidth; }
	INT	GetScreenHeight(VOID) { return m_nScreenHeight; }

	//! Access
	static CNtlFlasherManager*			GetInstance(VOID);

	inline GRendererD3D9*				GetFlashRenderer(VOID);
	inline LPDIRECT3DSTATEBLOCK9		GetStateBlock(VOID);

	//! Callback
	INT DeviceRestoreCallback(VOID);
	INT DeviceReleaseCallback(VOID);

private:
	//! Implementation

	//! Variable
	HWND					m_hWnd;
	GPtr<GRendererD3D9>		m_pRenderer;
	D3DPRESENT_PARAMETERS	m_d3dpp;
	LPDIRECT3DSTATEBLOCK9	m_pStateBlock;

	BOOL					m_bRendererEnable;

	DEVICE_RESTORE_HANDLE	m_hDeviceRestore;
	DEVICE_RESTORE_HANDLE	m_hDeviceRelease;

	GPtr<GFxFontLib>		m_pFontLib;
	GPtr<GFxFontMap>		m_pFontMap;
	GPtr<GFxMovieDef>		m_pLibMovie;

	INT						m_nScreenWidth;
	INT						m_nScreenHeight;
	
	//! Prevented Constructor
	CNtlFlasherManager(VOID);
};

static CNtlFlasherManager* GetFlasherManager(VOID)
{
	return CNtlFlasherManager::GetInstance();
}

LPDIRECT3DSTATEBLOCK9 CNtlFlasherManager::GetStateBlock(VOID)
{
	return m_pStateBlock;
}

GRendererD3D9* CNtlFlasherManager::GetFlashRenderer(VOID)
{
	return m_pRenderer;
}

#endif//__NTL_FLASHER_MANAGER_H__