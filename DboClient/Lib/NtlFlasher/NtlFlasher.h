////////////////////////////////////////////////////////////////////////////////
// File: NtlFlasher.h
// Desc: GFx Scaleform Integration Class. 
//
// 2006.07.13 Peessi@hitel.net   
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __NTL_FLASHER_H__
#define __NTL_FLASHER_H__

// GFx
#include "GFile.h"
#include "GFxPlayer.h"
#include "GFxLoader.h"
#include "GRendererD3D9.h"
#include "GFxFontLib.h"

#include "NtlDebug.h"
#include "NtlCallbackWidget.h"

typedef void (*CallFlashResourcePack)( const char* pFileName, void** pData, int* iSize );
extern CallFlashResourcePack g_fnCallFlashPack;

void LinkFlashResourceLoad( CallFlashResourcePack fn );
void UnLinkFlashResourceLoad(void);

class FlasherMemoryFile : public GMemoryFile
{
public:
	FlasherMemoryFile( const char* pFilename, const BYTE* pBuffer, INT sizeBuffer ) 
	: GMemoryFile( pFilename, pBuffer, sizeBuffer )
	{
		m_pDataPointerForDelete = pBuffer;
	}
	
	~FlasherMemoryFile(void)
	{
		NTL_ARRAY_DELETE( m_pDataPointerForDelete );
	}

	const BYTE*	m_pDataPointerForDelete;
};

class FlasherFileOpener : public GFxFileOpener
{
public:
	virtual GFile* OpenFile( const char* pFilename );
};

class FlasherCommandHandler : public GFxFSCommandHandler
{
public:
	FlasherCommandHandler() : GFxFSCommandHandler(), m_pFSCallBack( NULL ) {}

	virtual void Callback( GFxMovieView* pMovie, const char* command, const char* args );

	CNtlCallbackVPParam2*	m_pFSCallBack;

	template<class Callbackclass>	
	void LinkFSCallBack( Callbackclass* cbclass, INT (Callbackclass::*callback)( void* command, void* args ) )
	{
		UnLinkFSCallBack();
		m_pFSCallBack = NTL_NEW CNtlCallbackVPWidget2<Callbackclass>( cbclass, callback );
	}
	void UnLinkFSCallBack(void)
	{
		if( m_pFSCallBack )
			NTL_DELETE( m_pFSCallBack );
	}
};

class CNtlFlasher
{
public:
	//! Constructor & Destructor
	CNtlFlasher(VOID);
	~CNtlFlasher(VOID);

	//! Operation
	BOOL Create( CONST CHAR* szFileName );
	VOID Destroy(VOID);

	BOOL Update( FLOAT fElapsed );			// return FALSE -> 한타임 종료됨.
	VOID Render(VOID);

	INT	 GetResolutionWidth(VOID) { return m_nResolutionWidth; }
	INT	 GetResolutionHeight(VOID) { return m_nResolutionHeight; }
	INT	 GetFrameCount(VOID);
	FLOAT GetFrameRate(VOID);

	DWORD GetCurrentFrame(VOID);
	VOID GotoFrame( DWORD dwFrame );

	VOID SetPosition( INT nScreenX, INT nScreenY, INT nWidth, INT nHeight, INT nClippingX, INT nClippingY, INT nClippingWidth, INT nClippingHeight, BOOL bClipping = FALSE );
	VOID SetRotate( INT nDegree );			// + : clock, - : counter clock
	VOID SetAlpha( CHAR ucAlpha );

	VOID SetResizeType( GFxMovieView::ScaleModeType eResizeType );
	VOID SetUseFont( BOOL bUseFont ) { m_bUseFont = bUseFont; }

	BOOL IsPlay(VOID);
	VOID Play( BOOL bPlay );

	BOOL HasFrameCallback(VOID) { return m_bFrameCallback; }
	VOID SetFrameCallback( BOOL bUse ) { m_bFrameCallback = bUse; }

	VOID RestartMovie(VOID);

	BOOL IsAutoRewind(VOID) { return m_bAutoRewind; }
	VOID AutoRewind( BOOL bAutoRewind ) { m_bAutoRewind = bAutoRewind; }

	//! Control
	VOID OnMouseMove( INT nX, INT nY );
	VOID OnMouseButton( UINT button, BOOL bDown, INT nX, INT nY );
	VOID OnMouseWheel( INT nDelta, INT nX, INT nY );
	VOID OnKeyEvent( UINT key, UINT info, BOOL bDown );

	GFxMovieView*	GetMovie(VOID) { return m_pMovie.GetPtr(); }
	GFxMovieDef*	GetMovieDef(VOID) { return m_pMovieDef.GetPtr(); }
	FlasherCommandHandler*	GetCommandHandler(VOID) { return m_pCommandHandler.GetPtr(); }
				
protected:
	//! Implementation
	VOID SetViewPortRect( INT nX, INT nY, INT nWidth, INT nHeight, INT nClippingX, INT nClippingY, INT nClippingWidth, INT nClippingHeight, BOOL bClipping );
		
	//! Variable	
	GPtr<GFxMovieDef>		m_pMovieDef;
	GPtr<GFxMovieView>		m_pMovie;	
	
	GPtr<FlasherFileOpener>		m_pFileOpener;
	GPtr<FlasherCommandHandler>	m_pCommandHandler;
	GPtr<GFxImageCreator>		m_pImageCreator;
	GPtr<GFxRenderConfig>		m_pRenderConfig;

	INT		m_nScreenX;
	INT		m_nScreenY;
	INT		m_nWidth;
	INT		m_nHeight;	
	INT		m_nResolutionWidth;
	INT		m_nResolutionHeight;

	BOOL	m_bPlay;			
	BOOL	m_bAutoRewind;
	BOOL	m_bFrameCallback;	
	BOOL	m_bUseFont;
	BOOL	m_bOutofScreen;								
	GFxMovieView::ScaleModeType	m_eScaleModeType;	

	std::string				m_strFileName;
};

#endif//__NTL_FLASHER_H__