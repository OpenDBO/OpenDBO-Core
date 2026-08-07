#pragma once

#include <string>

// sound
#include "fmod/fmod.hpp"
#include "fmod/fmod_common.h"

//#define SOUND_DEBUG_LOG						///< 디버그용 사운드 관련 로그 남기기

class CNtlSoundDSP;


// 占신몌옙 ////////////////////////////////////////////////////////////////////////////
#define MIN_DISTANCE_BETWEEN_MINMAX		1.f	///< 占쏙옙占쏙옙 占쏙옙占쏙옙 占쌍댐옙 占신몌옙占쏙옙 占쌍쇽옙 占신몌옙占쏙옙占쏙옙 占썩본占쏙옙占쏙옙占쏙옙 占쏙옙 커占쏙옙占싹댐옙 占썩본占쏙옙치


// 占시곤옙 ////////////////////////////////////////////////////////////////////////////
#define SOUND_FADEINOUT_INTERVAL_TIME	100		///< Fade In/Out 占시울옙 占쏙옙占쏙옙占쏙옙 占쏙옙화占쏙옙 占쌍댐옙 占시곤옙 (占쏙옙占쏙옙 :milisecond)
#define DELAY_EFFECT_SOUND_TIME			500		///< ���� �̸��� ����Ʈ ���尣 �÷��� ���� �ð�

#define dBGM_FADE_OUT_TIME				2000	///< 배경음이 사라지거나 교체될 때의 fade out 시간


// 占쏙옙占쏙옙 占쌘듸옙 ////////////////////////////////////////////////////////////////////////////
#define INVALID_SOUND_HANDLE				0xffffffff


// 채占쏙옙 占쏙옙占쏙옙 //////////////////////////////////////////////////////////////////////////
#define MAX_FMOD_CHANNELS				4093	///< Maximum channel (hardware + software) supported by FMOD
#define MAX_DBO_CHANNELS				2048	///< Number of channels used in Dbo (increased for x64)
#define MAX_EFFECT_CHANNELS				128		///< The number of all channels of the effect that can be played at maximum


// 占쏙옙치 占쏙옙占쏙옙 //////////////////////////////////////////////////////////////////////////
#define dNTLSOUND_VOLUME_DEFAULT				1.0f
#define dNTLSOUND_VOLUME_BACKGROUND_MAX			0.5f
#define dNTLSOUND_VOLUME_EVENT_MAX				0.7f

#define dNTLSOUND_PITCH_DEFAULT					1.0f
#define dNTLSOUND_PITCH_MINIMUM					0.0f
#define dNTLSOUND_PITCH_MAXIMUM					10.0f

// etc
#define NTL_SOUND_MEMORY_POOL			10
#define MAX_SOUND_DUPLICATION			3		///< The maximum number of times a sound of the same name can be played simultaneously

#define d2D_SOUND_MODE		(FMOD_2D)
#define d3D_SOUND_MODE		(FMOD_3D | FMOD_3D_LINEARROLLOFF | FMOD_3D_WORLDRELATIVE)



typedef unsigned int			SOUND_HANDLE;


enum eSoundResourceType
{
	SRT_CHANNEL,
	SRT_CHANNEL_GROUP,
};

enum eFMODPlayType
{
	FMODPLAYTYPE_3DMODE,						///< 3D 기본 사운드 모드
	FMODPLAYTYPE_3D_TOOLMODE,					///< 3D Tool 사운드 모드

	FMODPLAYTYPE_NUM
};

enum eSoundPlayState
{
	SPS_NONE,									///< 占쏙옙占쌍곤옙 占쏙옙占쏙옙占신놂옙 占쏙옙占쏙옙 占쏙옙占쏙옙占쏙옙 占쏙옙占쏙옙
	SPS_PLAYING,								///< 占쏙옙占쏙옙占쏙옙
	SPS_PLAY_FADE_IN,							///< Fade in 占쏙옙占싱댐옙
	SPS_PLAY_FADE_OUT,							///< Fade Out 占쏙옙占싱댐옙
	SPS_SLEEP,									///< 占쏙옙占쌍곤옙 占쏙옙占쏙옙占실억옙 占쏙옙占쏙옙占쏙옙占싱댐옙
};

enum eChannelGroupType
{
	CHANNEL_GROUP_FIRST = 0,

	CHANNEL_GROUP_UI_SOUND = CHANNEL_GROUP_FIRST,///< UI 효占쏙옙占쏙옙
	CHANNEL_GROUP_JINGLE_MUSIC,					///< (占싱븝옙트 占쌩삼옙占쏙옙)占쏙옙占쏙옙트 획占쏙옙, 占쏙옙占쏙옙트 占싹뤄옙 占쏙옙占쏙옙 짧占쏙옙 占쏙옙占쏙옙
	CHANNEL_GROUP_FLASH_MUSIC,					///< 플래쉬 배경음
	CHANNEL_GROUP_BGM,							///< DBO 메인 배경음악
	CHANNEL_GROUP_AVATAR_VOICE_SOUND,			///< 占싣뱄옙타占쏙옙 占쏙옙占쏙옙
	CHANNEL_GROUP_AVATAR_EFFECT_SOUND,			///< 占싣뱄옙타占쏙옙 占쏙옙占쏙옙트 占쏙옙占쏙옙
	CHANNEL_GROUP_VOICE_SOUND,					///< 占쏙옙占쏙옙占쏙옙 占쏙옙占쏙옙
	CHANNEL_GROUP_EFFECT_SOUND,					///< 占쏙옙占쏙옙트 占쏙옙占쏙옙
	CHANNEL_GROUP_OBJECT_MUSIC,					///< 占쏙옙占쏙옙占쏙옙트 占쏙옙占쏙옙, 占쌘듸옙占쏙옙 占쌀몌옙 占쏙옙(占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙 占쌀몌옙占쏙옙 占쏙옙占쏙옙 占쌍억옙占쏙옙占쏙옙占쏙옙 占쏙옙占쏙옙占쏙옙 占쌕억옙占쏙옙)
	CHANNEL_GROUP_AMBIENT_MUSIC,				///< 占쏙옙占쏙옙 환占쏙옙 占쏙옙占쏙옙(占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙 占쏙옙占쏙옙占쏙옙 占쏙옙占쏙옙占쏙옙占쏙옙 占쏙옙占쏙옙)
	CHANNEL_GROUP_WEATHER_EFFECT_SOUND,			///< ����ȿ����(���� ���� ���尡 ��� �÷��� �ȴ�)
	CHANNEL_GROUP_WEATHER_MUSIC,				///< 占쏙옙占쏙옙占쏙옙占쏙옙(占쏙옙占실뤄옙 占쏙옙占쏙옙 占쏙옙占쏙옙占쏙옙 占쏙옙占쏙옙 占쌥븝옙占쏙옙 占싫댐옙)
	CHANNEL_GROUP_JINGLE_MUSIC_WITHOUT_FADE,	///< CHANNEL_GROUP_JINGLE_MUSIC 占쏙옙 占쏙옙占쏙옙占쏙옙 BGM占쏙옙 占쏙옙占쏙옙占쏙옙 占쏙옙치占쏙옙 占십는댐옙

	NUM_CHANNEL_GROUP,

	INVALID_CHANNEL_GROUP
};

enum eBGMType
{
	BGM_TYPE_SHARE_THEME,						///< DBO Common Theme
	BGM_TYPE_LOCAL_THEME,						///< Local theme song
	BGM_TYPE_MAIN_THEME,						///< 占쏙옙 占쏙옙占쌈억옙占쏙옙 占싣댐옙 占쏙옙占쏙옙占쏙옙 占시뤄옙占쏙옙 占실댐옙 BGM
	BGM_TYPE_RANGE,								///< 占쏙옙占쏙옙 占쏙옙占쏙옙 占싫울옙占쏙옙 占시뤄옙占싱되댐옙 BGM
	BGM_TYPE_PVP,								///< 占시뤄옙占싱억옙 1:1 占쏙옙占쏙옙 占쌓몌옙占쏙옙
	BGM_TYPE_RANKBATTLE_BATTLE,					///< 占쏙옙크占쏙옙틀
	BGM_TYPE_THEME_BGM,							///< THEME BGM
	BGM_TYPE_SERVER_PLAYED,						///< 占쏙옙占쏙옙占쏙옙 占쏙옙청占쏙옙占쏙옙 Play占싹댐옙 BGM
	BGM_TYPE_CINEMATIC,							///< 占시네몌옙틱 BGM	
	BGM_TYPE_WAIT,

	NUM_BGM_TYPE,

	INVALID_BGM_TYPE
};

enum eFadeInOutType
{	
	FADE_IN,									///< Fade In 占쏙옙占쏙옙
	FADE_OUT									///< Fade Out 占쏙옙占쏙옙
};

enum eStoreResult
{
	STORE_READY_TO_PLAY,						///< 占쌕뤄옙 占시뤄옙占쏙옙 占싼댐옙
	STORE_LIST,									///< 占쏙옙占쏙옙트占쏙옙占쏙옙占싸몌옙 占쏙옙占쏙옙 占쏙옙占쏙옙占싼댐옙
	STORE_FAIL,									///< 占쏙옙占쏙옙 占쏙옙占쏙옙
};

struct sNtlVolume
{
	///< 占쏙옙占쏙옙 : 0.0f ~ 1.0f
	float		fMainVolume;
	float		fFadeVolume;
	float		fWhenMoviePlayVolume;			///< 占쏙옙占쏙옙占쏙옙占쏙옙 占시뤄옙占쏙옙 占쏙옙 占쏙옙占쏙옙 占쏙옙占쏙옙

	sNtlVolume()
	:fMainVolume(1.f)
	,fFadeVolume(1.f)
	,fWhenMoviePlayVolume(1.f) {}
};

struct sNtlPitch
{	
	float		fMainPitch;					///< 占쏙옙占쏙옙 : 0.0f ~ 10.f( default : 1.f )
	float		fPitchRate;					///< 占쏙옙占쏙옙 : 占쏙옙占쏙옙 占쏙옙占쏙옙
	float		fSlowMotionPitchRate;		///< 占쏙옙占쏙옙 : 占쏙옙占쏙옙 占쏙옙占쏙옙

	sNtlPitch() : fMainPitch(1.f), fPitchRate(1.f), fSlowMotionPitchRate(1.f) {}
};

struct sNtlMinMax
{
	float		fMin, fMax;

	sNtlMinMax() : fMin(1.f), fMax(1.f + MIN_DISTANCE_BETWEEN_MINMAX) {}
};

struct sNtlSoundPlayParameta
{
	// for 2D
	SOUND_HANDLE		hHandle;			///< Returns the handle of the created Channel.
	int					iChannelGroup;		
	char*				pcFileName;
	
	// extra parameta
	eBGMType			eBGM_Type;
	bool				bBGMReplay;
	bool				bLoop;
	float				fVolume;			///< 0.0 ~ 1.f
	float				fPitch;				///< 0.0 ~ 10.0f

	// for 3D
	float				fXPos;
	float				fYPos;
	float				fZPos;	
	float				fMinDistance;		///< Minimum distance at which volume is maximized
	float				fMaxDistance;		///< Maximum distance at which volume is minimum

	sNtlSoundPlayParameta()
	:hHandle(INVALID_SOUND_HANDLE)
	,iChannelGroup(INVALID_CHANNEL_GROUP)
	,pcFileName(NULL)
	,eBGM_Type(BGM_TYPE_LOCAL_THEME)
	,fXPos(0.f)
	,fYPos(0.f)
	,fZPos(0.f)
	,bBGMReplay(false)
	,bLoop(false)
	,fVolume(dNTLSOUND_VOLUME_DEFAULT)
	,fMinDistance(10.f)
	,fMaxDistance(50.f)
	,fPitch(dNTLSOUND_PITCH_DEFAULT)
	{ }
};

// Convert FMOD's left-hand coordinate system to the renderer's coordinate system
#define dCONVERT_COORDINATE_X(fX)		(-1.f * fX)
