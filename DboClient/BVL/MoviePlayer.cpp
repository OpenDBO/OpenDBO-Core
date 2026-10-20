#include "precomp_dboclient.h"
#include "MoviePlayer.h"

#include <D3D9.h>
#include <mmsystem.h>
#include <vector>

// core
#include "NtlDebug.h"

// sound
#include "NtlSoundLogic.h"

// client
#include "DboDef.h"
#include "DboApplication.h"

// The movie files are avi containers with an MPEG-1 video stream and an MP2 audio stream.
// BandiVideo only ships a 32-bit dll, so the streams are decoded with pl_mpeg instead.
#define PLM_NO_STDIO
#define PL_MPEG_IMPLEMENTATION
#include "pl_mpeg.h"


#define D3DFVF_CUSTOMVERTEX ( D3DFVF_XYZRHW | D3DFVF_TEX1 )

#define dMOVIE_AUDIO_BUFFER_COUNT	24				///< about 0.6 second of 44.1khz audio queued ahead
#define dMOVIE_AVI_LOAD_SIZE		(32 * 1024)		///< bytes moved from the avi into a decoder buffer per load

struct Vertex
{
	RwReal x, y, z, w;
	RwReal tu, tv;
};


CMoviePlayer* CMoviePlayer::m_pInstance = NULL;

UINT WM_MOVIE_NOTIFY_EOF = ::RegisterWindowMessage(_T("WM_DBO_MOVIE_NOTIFY_EOF"));

////////////////////////////////////////////////////////////////////////////////////////////////////
// avi reading
////////////////////////////////////////////////////////////////////////////////////////////////////

struct sAviStreamReader
{
	FILE*					pFile;
	__int64					iPos;
	__int64					iEnd;
	bool					bVideo;
	bool					bEnded;
	std::vector<uint8_t>	vecChunk;

	sAviStreamReader() : pFile(NULL), iPos(0), iEnd(0), bVideo(false), bEnded(false) {}
	sAviStreamReader(const sAviStreamReader&) = delete;
	sAviStreamReader& operator=(const sAviStreamReader&) = delete;

	bool Open(const char* pcFileName, bool bVideoStream);
	void Close();
};

static bool Avi_IsStreamChunk(DWORD dwID, bool bVideo)
{
	// chunk ids are "##dc" or "##db" for video and "##wb" for audio, ## being the stream number
	char c2 = (char)((dwID >> 16) & 0xFF);
	char c3 = (char)((dwID >> 24) & 0xFF);

	if( bVideo )
		return c2 == 'd' && (c3 == 'c' || c3 == 'b');

	return c2 == 'w' && c3 == 'b';
}

static bool Avi_FindMovi(FILE* pFile, __int64& iStart, __int64& iEnd)
{
	DWORD adwRiff[3];
	if( fread(adwRiff, sizeof(adwRiff), 1, pFile) != 1 ||
		adwRiff[0] != mmioFOURCC('R', 'I', 'F', 'F') ||
		adwRiff[2] != mmioFOURCC('A', 'V', 'I', ' ') )
		return false;

	__int64 iPos		= 12;
	__int64 iRiffEnd	= 8 + (__int64)adwRiff[1];

	while( iPos + 12 <= iRiffEnd )
	{
		DWORD adwChunk[3];
		if( _fseeki64(pFile, iPos, SEEK_SET) != 0 || fread(adwChunk, sizeof(adwChunk), 1, pFile) != 1 )
			return false;

		if( adwChunk[0] == mmioFOURCC('L', 'I', 'S', 'T') && adwChunk[2] == mmioFOURCC('m', 'o', 'v', 'i') )
		{
			iStart	= iPos + 12;
			iEnd	= iPos + 8 + (__int64)adwChunk[1];
			return true;
		}

		iPos += 8 + (__int64)adwChunk[1] + (adwChunk[1] & 1);
	}

	return false;
}

bool sAviStreamReader::Open(const char* pcFileName, bool bVideoStream)
{
	if( fopen_s(&pFile, pcFileName, "rb") != 0 )
	{
		pFile = NULL;
		return false;
	}

	if( !Avi_FindMovi(pFile, iPos, iEnd) )
		return false;

	bVideo = bVideoStream;
	return true;
}

void sAviStreamReader::Close()
{
	if( pFile )
	{
		fclose(pFile);
		pFile = NULL;
	}
}

// pl_mpeg calls this when a decoder buffer runs dry; it is fed the next chunks of its stream
static void Avi_LoadCallback(plm_buffer_t* pBuffer, void* pUser)
{
	sAviStreamReader* pReader = reinterpret_cast<sAviStreamReader*>(pUser);
	if( pReader->bEnded )
		return;

	size_t uiWritten = 0;
	while( uiWritten < dMOVIE_AVI_LOAD_SIZE )
	{
		DWORD adwHeader[2];
		if( pReader->iPos + 8 > pReader->iEnd ||
			_fseeki64(pReader->pFile, pReader->iPos, SEEK_SET) != 0 ||
			fread(adwHeader, sizeof(adwHeader), 1, pReader->pFile) != 1 )
		{
			break;
		}

		DWORD dwID		= adwHeader[0];
		DWORD dwSize	= adwHeader[1];

		if( dwID == mmioFOURCC('L', 'I', 'S', 'T') )
		{
			// "rec " lists group the chunks of one frame, step inside
			pReader->iPos += 12;
			continue;
		}

		__int64 iData	= pReader->iPos + 8;
		pReader->iPos	= iData + dwSize + (dwSize & 1);

		if( dwSize == 0 || !Avi_IsStreamChunk(dwID, pReader->bVideo) )
			continue;

		if( iData + dwSize > pReader->iEnd )
			break;

		pReader->vecChunk.resize(dwSize);
		if( fread(&pReader->vecChunk[0], 1, dwSize, pReader->pFile) != dwSize )
			break;

		plm_buffer_write(pBuffer, &pReader->vecChunk[0], dwSize);
		uiWritten += dwSize;
	}

	if( uiWritten < dMOVIE_AVI_LOAD_SIZE )
	{
		pReader->bEnded = true;
		plm_buffer_signal_end(pBuffer);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// decoder
////////////////////////////////////////////////////////////////////////////////////////////////////

struct sMovieDecoder
{
	sAviStreamReader		tVideoReader;
	sAviStreamReader		tAudioReader;

	plm_video_t*			pVideo;
	plm_audio_t*			pAudio;

	IDirect3DTexture9*		pTexture;
	RwInt32					iWidth;
	RwInt32					iHeight;
	double					dFrameDuration;
	double					dNextFrameTime;
	bool					bVideoEnded;

	HWAVEOUT				hWaveOut;
	WAVEHDR					atWaveHdr[dMOVIE_AUDIO_BUFFER_COUNT];
	short					asPCM[dMOVIE_AUDIO_BUFFER_COUNT][PLM_AUDIO_SAMPLES_PER_FRAME * 2];
	RwInt32					iSampleRate;
	bool					bAudioDecodeEnded;
	bool					bAudioDrained;

	// play clock, follows the audio device while it plays and the performance counter after that
	LARGE_INTEGER			liFrequency;
	LARGE_INTEGER			liClockTick;
	double					dClockBase;

	bool					bEOFPosted;

	sMovieDecoder();
	sMovieDecoder(const sMovieDecoder&) = delete;
	sMovieDecoder& operator=(const sMovieDecoder&) = delete;

	bool					Open(const char* pcFileName);
	bool					OpenAudio(const char* pcFileName);
	void					Close();
};

sMovieDecoder::sMovieDecoder()
:pVideo(NULL)
,pAudio(NULL)
,pTexture(NULL)
,iWidth(0)
,iHeight(0)
,dFrameDuration(1.0 / 30.0)
,dNextFrameTime(0.0)
,bVideoEnded(false)
,hWaveOut(NULL)
,iSampleRate(0)
,bAudioDecodeEnded(false)
,bAudioDrained(false)
,dClockBase(0.0)
,bEOFPosted(false)
{
	memset(atWaveHdr, 0, sizeof(atWaveHdr));
	QueryPerformanceFrequency(&liFrequency);
	liClockTick.QuadPart = 0;
}

bool sMovieDecoder::Open(const char* pcFileName)
{
	if( !tVideoReader.Open(pcFileName, true) )
		return false;

	plm_buffer_t* pVideoBuffer = plm_buffer_create_with_capacity(PLM_BUFFER_DEFAULT_SIZE);
	plm_buffer_set_load_callback(pVideoBuffer, Avi_LoadCallback, &tVideoReader);
	pVideo = plm_video_create_with_buffer(pVideoBuffer, TRUE);

	if( !plm_video_has_header(pVideo) )
		return false;

	iWidth	= plm_video_get_width(pVideo);
	iHeight	= plm_video_get_height(pVideo);

	double dFrameRate = plm_video_get_framerate(pVideo);
	if( dFrameRate > 0.0 )
		dFrameDuration = 1.0 / dFrameRate;

	LPDIRECT3DDEVICE9 pD3DDevice = static_cast<LPDIRECT3DDEVICE9>( RwD3D9GetCurrentD3DDevice() );
	if( FAILED(pD3DDevice->CreateTexture(iWidth, iHeight, 1, 0, D3DFMT_X8R8G8B8, D3DPOOL_MANAGED, &pTexture, NULL)) )
	{
		pTexture = NULL;
		return false;
	}

	// a movie without playable audio still shows its video, timed by the performance counter
	if( !OpenAudio(pcFileName) )
		bAudioDrained = true;

	return true;
}

bool sMovieDecoder::OpenAudio(const char* pcFileName)
{
	if( !tAudioReader.Open(pcFileName, false) )
		return false;

	plm_buffer_t* pAudioBuffer = plm_buffer_create_with_capacity(PLM_BUFFER_DEFAULT_SIZE);
	plm_buffer_set_load_callback(pAudioBuffer, Avi_LoadCallback, &tAudioReader);
	pAudio = plm_audio_create_with_buffer(pAudioBuffer, TRUE);

	if( !plm_audio_has_header(pAudio) )
		return false;

	iSampleRate = plm_audio_get_samplerate(pAudio);
	if( iSampleRate <= 0 )
		return false;

	// pl_mpeg always hands out interleaved stereo samples
	WAVEFORMATEX tFormat;
	memset(&tFormat, 0, sizeof(tFormat));
	tFormat.wFormatTag		= WAVE_FORMAT_PCM;
	tFormat.nChannels		= 2;
	tFormat.nSamplesPerSec	= iSampleRate;
	tFormat.wBitsPerSample	= 16;
	tFormat.nBlockAlign		= tFormat.nChannels * tFormat.wBitsPerSample / 8;
	tFormat.nAvgBytesPerSec	= tFormat.nSamplesPerSec * tFormat.nBlockAlign;

	if( waveOutOpen(&hWaveOut, WAVE_MAPPER, &tFormat, 0, 0, CALLBACK_NULL) != MMSYSERR_NOERROR )
	{
		hWaveOut = NULL;
		return false;
	}

	// held until the first buffers are queued so the clock starts with the picture
	waveOutPause(hWaveOut);

	for( RwInt32 i = 0; i < dMOVIE_AUDIO_BUFFER_COUNT; ++i )
	{
		atWaveHdr[i].lpData			= reinterpret_cast<LPSTR>(asPCM[i]);
		atWaveHdr[i].dwBufferLength	= sizeof(asPCM[i]);
		waveOutPrepareHeader(hWaveOut, &atWaveHdr[i], sizeof(WAVEHDR));
	}

	return true;
}

void sMovieDecoder::Close()
{
	if( hWaveOut )
	{
		waveOutReset(hWaveOut);

		for( RwInt32 i = 0; i < dMOVIE_AUDIO_BUFFER_COUNT; ++i )
		{
			if( atWaveHdr[i].dwFlags & WHDR_PREPARED )
				waveOutUnprepareHeader(hWaveOut, &atWaveHdr[i], sizeof(WAVEHDR));
		}

		waveOutClose(hWaveOut);
		hWaveOut = NULL;
	}

	if( pAudio )
	{
		plm_audio_destroy(pAudio);
		pAudio = NULL;
	}

	if( pVideo )
	{
		plm_video_destroy(pVideo);
		pVideo = NULL;
	}

	tAudioReader.Close();
	tVideoReader.Close();

	if( pTexture )
	{
		pTexture->Release();
		pTexture = NULL;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// CMoviePlayer
////////////////////////////////////////////////////////////////////////////////////////////////////

RwBool CMoviePlayer::CreateInstance()
{
	NTL_FUNCTION("CMoviePlayer::CreateInstance");

	if( m_pInstance )
	{
		DBO_FAIL("CMoviePlayer instance already exist");
		NTL_RETURN(FALSE);
	}

	m_pInstance = NTL_NEW CMoviePlayer;
	if( !m_pInstance )
	{
		DBO_FAIL("CMoviePlayer::CreateInstance : Instance Allocate Fail");
		NTL_RETURN(FALSE);
	}

	NTL_RETURN(TRUE);
}

VOID CMoviePlayer::DestoryInstance()
{
	NTL_FUNCTION("CMoviePlayer::DestoryInstance");

	NTL_DELETE(m_pInstance);

	NTL_RETURNVOID();
}

CMoviePlayer* CMoviePlayer::GetInstance()
{
	return m_pInstance;
}

CMoviePlayer::CMoviePlayer()
:m_pCallback(NULL)
,m_pDecoder(NULL)
,m_pVertexBuffer(NULL)
,m_eSize(MOVIE_SIZE_ORIGINAL)
,m_bPlay(FALSE)
,m_bRelease(FALSE)
,m_fVolume(1.f)
{

}

CMoviePlayer::~CMoviePlayer()
{
	Stop();
}

RwBool CMoviePlayer::Play(const char* pcFileName, eMovieSizeType eSizeType)
{
	if( !pcFileName )
		return FALSE;

	if( IsPlay() )
		return FALSE;

	if( false == ReadyMovie(pcFileName, eSizeType) )
		return FALSE;

	m_handleReleaseHandler = CNtlDeviceRestoreHandler::GetInstance()->LinkDeviceRelease(this, &CMoviePlayer::ReleaseHandler);

	m_strMovieFileName		= pcFileName;
	m_eSize					= eSizeType;
	m_bPlay					= TRUE;

	SetVolume(m_fVolume);

	// queue the first audio frames, then start the device and the clock together
	FeedAudio();

	if( m_pDecoder->hWaveOut )
		waveOutRestart(m_pDecoder->hWaveOut);

	QueryPerformanceCounter(&m_pDecoder->liClockTick);

	return TRUE;
}

RwBool CMoviePlayer::ReadyMovie(const char* pcFileName, eMovieSizeType eSizeType)
{
	if( !pcFileName )
	{
		DBO_FAIL("Invalid movie file name");
		return FALSE;
	}

	if( strlen(pcFileName) == 0 )
	{
		DBO_FAIL("movie file name is empty");
		return FALSE;
	}

	m_pDecoder = NTL_NEW sMovieDecoder;

	if( !m_pDecoder->Open(pcFileName) )
	{
		DBO_FAIL("Can not open movie file of name " << pcFileName);
		StopMovie();
		return FALSE;
	}

	LPDIRECT3DDEVICE9 pD3DDevice = static_cast<LPDIRECT3DDEVICE9>( RwD3D9GetCurrentD3DDevice() );

	RwInt32 iScreenWidth		= (RwInt32)GetDboGlobal()->GetScreenWidth();
	RwInt32 iScreenHeight		= (RwInt32)GetDboGlobal()->GetScreenHeight();
	RwInt32 iMovieWidth			= m_pDecoder->iWidth;
	RwInt32 iMovieHeight		= m_pDecoder->iHeight;

	Vertex SurfaceVertices[4];

	switch(eSizeType)
	{
	case MOVIE_SIZE_ORIGINAL:
		{
			// keep the movie resolution and center it on the screen
			RwReal fMovieTop	= (RwReal)((iScreenHeight - iMovieHeight) / 2.f);
			RwReal fMovieBottom	= fMovieTop + iMovieHeight;
			RwReal fMovieLeft	= (RwReal)((iScreenWidth - iMovieWidth) / 2.f);
			RwReal fMovieRight	= fMovieLeft + (RwReal)iMovieWidth;

			Vertex vertices[4] =
			{
				{fMovieLeft,	fMovieTop,		0.0f,	1.f,	0.0f,	0.0f },
				{fMovieLeft,	fMovieBottom,	0.0f,	1.f,	0.0f,	1.0f },
				{fMovieRight,	fMovieTop,		0.0f,	1.f,	1.0f,	0.0f },
				{fMovieRight,	fMovieBottom,	0.0f,	1.f,	1.0f,	1.0f },
			};

			memcpy(SurfaceVertices, vertices, sizeof(SurfaceVertices));
			break;
		}
	case MOVIE_SIZE_RATIO_FULL:
		{
			// stretch to the screen width and scale the height by the same ratio
			RwReal fAdjustMovieHight	= ((RwReal)iScreenWidth / (RwReal)iMovieWidth) * (RwReal)iMovieHeight;
			RwReal fMovieTop			= ((RwReal)iScreenHeight - fAdjustMovieHight) / 2.f;
			RwReal fMovieBottom			= fMovieTop + fAdjustMovieHight;

			Vertex vertices[4] =
			{
				{0.f,					fMovieTop,		0.0f,	1.f,	0.0f,	0.0f },
				{0.f,					fMovieBottom,	0.0f,	1.f,	0.0f,	1.0f },
				{(RwReal)iScreenWidth,	fMovieTop,		0.0f,	1.f,	1.0f,	0.0f },
				{(RwReal)iScreenWidth,	fMovieBottom,	0.0f,	1.f,	1.0f,	1.0f },
			};

			memcpy(SurfaceVertices, vertices, sizeof(SurfaceVertices));
			break;
		}
	case MOVIE_SIZE_FULL:
		{
			// scale by the larger of the width/height ratios so the movie fills the screen
			RwReal fScaleWidth, fScaleHeight;
			RwReal fMovieTop, fMovieBottom, fMovieLeft, fMovieRight;

			fScaleWidth		= (RwReal)iScreenWidth  / (RwReal)iMovieWidth;
			fScaleHeight	= (RwReal)iScreenHeight / (RwReal)iMovieHeight;

			if( fScaleWidth > fScaleHeight )
			{
				fMovieTop	= 0.f;
				fMovieBottom= fScaleWidth * (RwReal)iMovieHeight;
				fMovieLeft	= 0.f;
				fMovieRight	= (RwReal)iScreenWidth;
			}
			else
			{
				fMovieTop	= 0.f;
				fMovieBottom= (RwReal)iScreenHeight;
				fMovieLeft	= 0.f;
				fMovieRight	= fScaleHeight * (RwReal)iMovieWidth;
			}

			Vertex vertices[4] =
			{
				{0.f,					fMovieTop,		0.0f,	1.f,	0.0f,	0.0f },
				{0.f,					fMovieBottom,	0.0f,	1.f,	0.0f,	1.0f },
				{(RwReal)iScreenWidth,	fMovieTop,		0.0f,	1.f,	1.0f,	0.0f },
				{(RwReal)iScreenWidth,	fMovieBottom,	0.0f,	1.f,	1.0f,	1.0f },
			};

			memcpy(SurfaceVertices, vertices, sizeof(SurfaceVertices));
			break;
		}
	default:
		{
			DBO_FAIL("Invalid movie size type");
			StopMovie();
			return FALSE;
		}
	}

	if( FAILED(pD3DDevice->CreateVertexBuffer(sizeof(SurfaceVertices), 0, D3DFVF_CUSTOMVERTEX,
		D3DPOOL_MANAGED, &m_pVertexBuffer, NULL)) )
	{
		m_pVertexBuffer = NULL;
		DBO_FAIL("Can not create movie vertex buffer");
		StopMovie();
		return FALSE;
	}

	void* pVertices = NULL;
	m_pVertexBuffer->Lock( 0, sizeof(SurfaceVertices), (void**)&pVertices, 0 );
	memcpy( pVertices, SurfaceVertices, sizeof(SurfaceVertices) );
	m_pVertexBuffer->Unlock();

	return TRUE;
}

RwBool CMoviePlayer::Stop()
{
	RwBool bResult = false;

	if( m_bPlay )
	{
		m_bPlay = FALSE;

		CNtlDeviceRestoreHandler::GetInstance()->UnLinkDeviceRelease(m_handleReleaseHandler);

		m_strMovieFileName.clear();

		StopMovie();
		bResult = true;
	}

	if(m_pCallback)
	{
		m_pCallback->Call();
		NTL_DELETE(m_pCallback);
	}

	return bResult;
}

VOID CMoviePlayer::StopMovie()
{
	if( m_pDecoder )
	{
		m_pDecoder->Close();
		NTL_DELETE(m_pDecoder);
	}

	if( m_pVertexBuffer )
	{
		m_pVertexBuffer->Release();
		m_pVertexBuffer = NULL;
	}
}

VOID CMoviePlayer::SetVolume(RwReal fVolume)
{
	// applied to the samples as they are queued, waveOutSetVolume would change the volume of
	// the whole application session and with it the FMOD output
	m_fVolume = Logic_GetFMODValidVolume(fVolume);
}

RwReal CMoviePlayer::GetVolume()
{
	return m_fVolume;
}

RwBool CMoviePlayer::IsPlay()
{
	return m_bPlay;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
///         D3D 공간에 사각 평면을 생성하고 거기에 동영상 텍스처를 입힘
/// @param
/// @return
/// @date   Thursday, February 05, 2009  11:08:42 AM
////////////////////////////////////////////////////////////////////////////////////////////////////
void CMoviePlayer::RenderD3D(IDirect3DTexture9 *texture)
{
	LPDIRECT3DDEVICE9 pD3DDevice = static_cast<LPDIRECT3DDEVICE9>( RwD3D9GetCurrentD3DDevice() );
	HWND hWnd = CNtlApplication::GetInstance()->GetHWnd();

	// RenderWare leaves the render target of the last camera bound. In game that is one of the
	// post effect / supersampling rasters, so draw to the back buffer and give the target back.
	IDirect3DSurface9* pOldRenderTarget = NULL;
	IDirect3DSurface9* pOldDepthStencil = NULL;
	D3DVIEWPORT9 tOldViewport;

	pD3DDevice->GetRenderTarget(0, &pOldRenderTarget);
	pD3DDevice->GetDepthStencilSurface(&pOldDepthStencil);
	pD3DDevice->GetViewport(&tOldViewport);

	IDirect3DSurface9* pBackBuffer = NULL;
	if( SUCCEEDED(pD3DDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer)) )
	{
		D3DSURFACE_DESC tDesc;
		pBackBuffer->GetDesc(&tDesc);

		pD3DDevice->SetRenderTarget(0, pBackBuffer);
		pD3DDevice->SetDepthStencilSurface(NULL);

		D3DVIEWPORT9 tViewport = { 0, 0, tDesc.Width, tDesc.Height, 0.0f, 1.0f };
		pD3DDevice->SetViewport(&tViewport);
	}

	pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_COLORVALUE(0.0f,0.0f,0.0f,1.0f), 1.0f, 0 );

	// Every state is set through RenderWare and then on the device itself. The flash ui and
	// parts of the world renderer touch the device directly, so RenderWare's cache can claim
	// a value the device no longer has and skip the call (a leftover pixel shader, fog or a
	// diffuse color stage then shows the quad as plain white).
	RwD3D9SetVertexShader(NULL);
	RwD3D9SetPixelShader(NULL);
	pD3DDevice->SetVertexShader(NULL);
	pD3DDevice->SetPixelShader(NULL);

	const DWORD aRenderStates[][2] =
	{
		{ D3DRS_LIGHTING,				FALSE },
		{ D3DRS_ZENABLE,				FALSE },
		{ D3DRS_ZWRITEENABLE,			FALSE },
		{ D3DRS_CULLMODE,				D3DCULL_NONE },
		{ D3DRS_FILLMODE,				D3DFILL_SOLID },
		{ D3DRS_ALPHABLENDENABLE,		FALSE },
		{ D3DRS_ALPHATESTENABLE,		FALSE },
		{ D3DRS_FOGENABLE,				FALSE },
		{ D3DRS_SPECULARENABLE,			FALSE },
		{ D3DRS_STENCILENABLE,			FALSE },
		{ D3DRS_SCISSORTESTENABLE,		FALSE },
		{ D3DRS_SRGBWRITEENABLE,		FALSE },
		{ D3DRS_COLORWRITEENABLE,		0x0000000F },
	};

	for( RwInt32 i = 0; i < (RwInt32)(sizeof(aRenderStates) / sizeof(aRenderStates[0])); ++i )
	{
		RwD3D9SetRenderState(aRenderStates[i][0], aRenderStates[i][1]);
		pD3DDevice->SetRenderState((D3DRENDERSTATETYPE)aRenderStates[i][0], aRenderStates[i][1]);
	}

	const DWORD aStageStates[][3] =
	{
		{ 0, D3DTSS_COLOROP,				D3DTOP_SELECTARG1 },
		{ 0, D3DTSS_COLORARG1,				D3DTA_TEXTURE },
		{ 0, D3DTSS_ALPHAOP,				D3DTOP_SELECTARG1 },
		{ 0, D3DTSS_ALPHAARG1,				D3DTA_TEXTURE },
		{ 0, D3DTSS_TEXCOORDINDEX,			0 },
		{ 0, D3DTSS_TEXTURETRANSFORMFLAGS,	D3DTTFF_DISABLE },
		{ 1, D3DTSS_COLOROP,				D3DTOP_DISABLE },
		{ 1, D3DTSS_ALPHAOP,				D3DTOP_DISABLE },
	};

	for( RwInt32 i = 0; i < (RwInt32)(sizeof(aStageStates) / sizeof(aStageStates[0])); ++i )
	{
		RwD3D9SetTextureStageState(aStageStates[i][0], aStageStates[i][1], aStageStates[i][2]);
		pD3DDevice->SetTextureStageState(aStageStates[i][0], (D3DTEXTURESTAGESTATETYPE)aStageStates[i][1], aStageStates[i][2]);
	}

	const DWORD aSamplerStates[][2] =
	{
		{ D3DSAMP_MINFILTER,		D3DTEXF_LINEAR },
		{ D3DSAMP_MAGFILTER,		D3DTEXF_LINEAR },
		{ D3DSAMP_MIPFILTER,		D3DTEXF_NONE },
		{ D3DSAMP_ADDRESSU,			D3DTADDRESS_CLAMP },
		{ D3DSAMP_ADDRESSV,			D3DTADDRESS_CLAMP },
		{ D3DSAMP_SRGBTEXTURE,		FALSE },
	};

	for( RwInt32 i = 0; i < (RwInt32)(sizeof(aSamplerStates) / sizeof(aSamplerStates[0])); ++i )
	{
		RwD3D9SetSamplerState(0, aSamplerStates[i][0], aSamplerStates[i][1]);
		pD3DDevice->SetSamplerState(0, (D3DSAMPLERSTATETYPE)aSamplerStates[i][0], aSamplerStates[i][1]);
	}

	// clear RenderWare's cached stage 0 texture, the movie texture is not a RwTexture
	RwD3D9SetTexture(NULL, 0);

	RwD3D9SetStreamSource(0, m_pVertexBuffer, 0, sizeof(Vertex));
	RwD3D9SetFVF(D3DFVF_CUSTOMVERTEX);
	pD3DDevice->SetStreamSource(0, m_pVertexBuffer, 0, sizeof(Vertex));
	pD3DDevice->SetFVF(D3DFVF_CUSTOMVERTEX);

	pD3DDevice->BeginScene();
	pD3DDevice->SetTexture( 0, texture);
	pD3DDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP,  0, 2 );
	pD3DDevice->SetTexture( 0, NULL);
	pD3DDevice->EndScene();

	// the vertex buffer is released with the movie, do not leave it bound
	RwD3D9SetStreamSource(0, NULL, 0, 0);
	pD3DDevice->SetStreamSource(0, NULL, 0, 0);

	pD3DDevice->Present( NULL, NULL, hWnd, NULL );

	if( pOldRenderTarget )
	{
		pD3DDevice->SetRenderTarget(0, pOldRenderTarget);
		pOldRenderTarget->Release();
	}

	pD3DDevice->SetDepthStencilSurface(pOldDepthStencil);
	if( pOldDepthStencil )
		pOldDepthStencil->Release();

	pD3DDevice->SetViewport(&tOldViewport);

	if( pBackBuffer )
		pBackBuffer->Release();
}

VOID CMoviePlayer::FeedAudio()
{
	if( !m_pDecoder || !m_pDecoder->hWaveOut || m_pDecoder->bAudioDrained )
		return;

	RwBool bQueued = FALSE;

	for( RwInt32 i = 0; i < dMOVIE_AUDIO_BUFFER_COUNT; ++i )
	{
		WAVEHDR& tHeader = m_pDecoder->atWaveHdr[i];

		if( tHeader.dwFlags & WHDR_INQUEUE )
		{
			bQueued = TRUE;
			continue;
		}

		if( m_pDecoder->bAudioDecodeEnded )
			continue;

		plm_samples_t* pSamples = plm_audio_decode(m_pDecoder->pAudio);
		if( !pSamples )
		{
			m_pDecoder->bAudioDecodeEnded = true;
			continue;
		}

		short* pPCM = m_pDecoder->asPCM[i];
		RwUInt32 uiCount = pSamples->count * 2;

		for( RwUInt32 k = 0; k < uiCount; ++k )
		{
			float fSample = pSamples->interleaved[k] * m_fVolume;

			if( fSample > 1.f )
				fSample = 1.f;
			else if( fSample < -1.f )
				fSample = -1.f;

			pPCM[k] = (short)(fSample * 32767.f);
		}

		tHeader.dwBufferLength = (DWORD)(uiCount * sizeof(short));
		waveOutWrite(m_pDecoder->hWaveOut, &tHeader, sizeof(WAVEHDR));
		bQueued = TRUE;
	}

	if( m_pDecoder->bAudioDecodeEnded && !bQueued )
		m_pDecoder->bAudioDrained = true;
}

double CMoviePlayer::GetClock()
{
	LARGE_INTEGER liNow;
	QueryPerformanceCounter(&liNow);

	if( m_pDecoder->hWaveOut && !m_pDecoder->bAudioDrained )
	{
		MMTIME tTime;
		tTime.wType = TIME_SAMPLES;

		if( waveOutGetPosition(m_pDecoder->hWaveOut, &tTime, sizeof(tTime)) == MMSYSERR_NOERROR )
		{
			double dSamples = -1.0;

			if( tTime.wType == TIME_SAMPLES )
				dSamples = (double)tTime.u.sample;
			else if( tTime.wType == TIME_BYTES )
				dSamples = (double)(tTime.u.cb / 4);

			if( dSamples >= 0.0 )
			{
				m_pDecoder->dClockBase	= dSamples / (double)m_pDecoder->iSampleRate;
				m_pDecoder->liClockTick	= liNow;
				return m_pDecoder->dClockBase;
			}
		}
	}

	return m_pDecoder->dClockBase +
		(double)(liNow.QuadPart - m_pDecoder->liClockTick.QuadPart) / (double)m_pDecoder->liFrequency.QuadPart;
}

VOID CMoviePlayer::Render()
{
	if( m_bRelease )
	{
		Release();
		return;
	}

	if( false == IsPlay() || !m_pDecoder )
		return;

	FeedAudio();

	double dClock = GetClock();

	// decode up to the current play time, only the newest frame is shown
	plm_frame_t* pFrame = NULL;
	while( !m_pDecoder->bVideoEnded && m_pDecoder->dNextFrameTime <= dClock )
	{
		plm_frame_t* pDecoded = plm_video_decode(m_pDecoder->pVideo);
		if( !pDecoded )
		{
			m_pDecoder->bVideoEnded = true;
			break;
		}

		pFrame = pDecoded;
		m_pDecoder->dNextFrameTime = pDecoded->time + m_pDecoder->dFrameDuration;
	}

	if( pFrame )
	{
		D3DLOCKED_RECT tLocked;
		HRESULT hr = m_pDecoder->pTexture->LockRect(0, &tLocked, NULL, 0);
		if( SUCCEEDED(hr) )
		{
			plm_frame_to_bgra(pFrame, reinterpret_cast<uint8_t*>(tLocked.pBits), tLocked.Pitch);
			m_pDecoder->pTexture->UnlockRect(0);
		}
		else
		{
			static bool bLogged = false;
			if( !bLogged )
			{
				bLogged = true;
				DBO_WARNING_MESSAGE("CMoviePlayer::Render: can not lock the movie texture, hr: " << hr);
			}
		}

		RenderD3D(m_pDecoder->pTexture);
	}

	// stopped from the message handler, like the key and mouse skips
	if( m_pDecoder->bVideoEnded && m_pDecoder->bAudioDrained && !m_pDecoder->bEOFPosted )
	{
		m_pDecoder->bEOFPosted = true;
		::PostMessage(CNtlApplication::GetInstance()->GetHWnd(), WM_MOVIE_NOTIFY_EOF, 0, 0);
	}
}

RwInt32 CMoviePlayer::ReleaseHandler()
{
	if( IsPlay() )
		m_bRelease = TRUE;

	return 1;
}

VOID CMoviePlayer::Release()
{
	m_bRelease = FALSE;

	Stop();
}

eMOVIE_HANDLE_EVENT_RESULT CMoviePlayer::HandleEvent(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if( hWnd != CNtlApplication::GetInstance()->GetHWnd() )
		return MOVIE_HANDLE_EVENT_OK;

	switch(message)
	{
	case WM_KEYDOWN:
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_MBUTTONDOWN:
		{
			if ( IsPlay() )
			{
				Stop();
				return MOVIE_HANDLE_EVENT_STOP_MOVIE;
			}
		}
		break;

	case WM_ACTIVATE:
		{
			// Minimize 또는 Active 비활성화
			if ( LOWORD( wParam ) == WA_INACTIVE || ::IsIconic( hWnd ) )
			{
				if ( IsPlay() )
				{
					Stop();
					return MOVIE_HANDLE_EVENT_STOP_MOVIE;
				}
			}
		}
		break;
	}

	if ( WM_MOVIE_NOTIFY_EOF == message )
	{
		if ( Stop() )
		{
			return MOVIE_HANDLE_EVENT_STOP_MOVIE;
		}
	}

	return MOVIE_HANDLE_EVENT_OK;
}
