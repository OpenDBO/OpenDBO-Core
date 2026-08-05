#pragma once

#include <string>

// sound
#include "fmod/fmod.hpp"
#include "fmod/fmod_common.h"

//#define SOUND_DEBUG_LOG						///< ����׿� ���� ���� �α� �����

class CNtlSoundDSP;


// �Ÿ� ////////////////////////////////////////////////////////////////////////////
#define MIN_DISTANCE_BETWEEN_MINMAX		1.f	///< ���� ���� �ִ� �Ÿ��� �ּ� �Ÿ����� �⺻������ �� Ŀ���ϴ� �⺻��ġ


// �ð� ////////////////////////////////////////////////////////////////////////////
#define SOUND_FADEINOUT_INTERVAL_TIME	100		///< Fade In/Out �ÿ� ������ ��ȭ�� �ִ� �ð� (���� :milisecond)
#define DELAY_EFFECT_SOUND_TIME			500		///< ���� �̸��� ����Ʈ ���尣 �÷��� ���� �ð�

#define dBGM_FADE_OUT_TIME				2000	///< ������� ������ų� ��ü�� ���� fade out �ð�


// ���� �ڵ� ////////////////////////////////////////////////////////////////////////////
#define INVALID_SOUND_HANDLE				0xffffffff


// ä�� ���� //////////////////////////////////////////////////////////////////////////
#define MAX_FMOD_CHANNELS				4093	///< Maximum channel (hardware + software) supported by FMOD
#define MAX_DBO_CHANNELS				2048	///< Number of channels used in Dbo (increased for x64)
#define MAX_EFFECT_CHANNELS				128		///< The number of all channels of the effect that can be played at maximum


// ��ġ ���� //////////////////////////////////////////////////////////////////////////
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
	FMODPLAYTYPE_3DMODE,						///< 3D �⺻ ���� ���
	FMODPLAYTYPE_3D_TOOLMODE,					///< 3D Tool ���� ���

	FMODPLAYTYPE_NUM
};

enum eSoundPlayState
{
	SPS_NONE,									///< ���ְ� �����ų� ���� ������ ����
	SPS_PLAYING,								///< ������
	SPS_PLAY_FADE_IN,							///< Fade in ���̴�
	SPS_PLAY_FADE_OUT,							///< Fade Out ���̴�
	SPS_SLEEP,									///< ���ְ� �����Ǿ� �������̴�
};

enum eChannelGroupType
{
	CHANNEL_GROUP_FIRST = 0,

	CHANNEL_GROUP_UI_SOUND = CHANNEL_GROUP_FIRST,///< UI ȿ����
	CHANNEL_GROUP_JINGLE_MUSIC,					///< (�̺�Ʈ �߻���)����Ʈ ȹ��, ����Ʈ �Ϸ� ���� ª�� ����
	CHANNEL_GROUP_FLASH_MUSIC,					///< �÷��� �����
	CHANNEL_GROUP_BGM,							///< DBO ���� �������
	CHANNEL_GROUP_AVATAR_VOICE_SOUND,			///< �ƹ�Ÿ�� ����
	CHANNEL_GROUP_AVATAR_EFFECT_SOUND,			///< �ƹ�Ÿ�� ����Ʈ ����
	CHANNEL_GROUP_VOICE_SOUND,					///< ������ ����
	CHANNEL_GROUP_EFFECT_SOUND,					///< ����Ʈ ����
	CHANNEL_GROUP_OBJECT_MUSIC,					///< ������Ʈ ����, �ڵ��� �Ҹ� ��(�������������� �Ҹ��� ���� �־������� ������ �پ���)
	CHANNEL_GROUP_AMBIENT_MUSIC,				///< ���� ȯ�� ����(������������ ������ �������� ����)
	CHANNEL_GROUP_WEATHER_EFFECT_SOUND,			///< ����ȿ����(���� ���� ���尡 ��� �÷��� �ȴ�)
	CHANNEL_GROUP_WEATHER_MUSIC,				///< ��������(���Ƿ� ���� ������ ���� �ݺ��� �ȴ�)
	CHANNEL_GROUP_JINGLE_MUSIC_WITHOUT_FADE,	///< CHANNEL_GROUP_JINGLE_MUSIC �� ������ BGM�� ������ ��ġ�� �ʴ´�

	NUM_CHANNEL_GROUP,

	INVALID_CHANNEL_GROUP
};

enum eBGMType
{
	BGM_TYPE_SHARE_THEME,						///< DBO Common Theme
	BGM_TYPE_LOCAL_THEME,						///< Local theme song
	BGM_TYPE_MAIN_THEME,						///< �� ���Ӿ��� �ƴ� ������ �÷��� �Ǵ� BGM
	BGM_TYPE_RANGE,								///< ���� ���� �ȿ��� �÷��̵Ǵ� BGM
	BGM_TYPE_PVP,								///< �÷��̾� 1:1 ���� �׸���
	BGM_TYPE_RANKBATTLE_BATTLE,					///< ��ũ��Ʋ
	BGM_TYPE_THEME_BGM,							///< THEME BGM
	BGM_TYPE_SERVER_PLAYED,						///< ������ ��û���� Play�ϴ� BGM
	BGM_TYPE_CINEMATIC,							///< �ó׸�ƽ BGM	
	BGM_TYPE_WAIT,

	NUM_BGM_TYPE,

	INVALID_BGM_TYPE
};

enum eFadeInOutType
{	
	FADE_IN,									///< Fade In ����
	FADE_OUT									///< Fade Out ����
};

enum eStoreResult
{
	STORE_READY_TO_PLAY,						///< �ٷ� �÷��� �Ѵ�
	STORE_LIST,									///< ����Ʈ�����θ� ���� �����Ѵ�
	STORE_FAIL,									///< ���� ����
};

struct sNtlVolume
{
	///< ���� : 0.0f ~ 1.0f
	float		fMainVolume;
	float		fFadeVolume;
	float		fWhenMoviePlayVolume;			///< �������� �÷��� �� ���� ����

	sNtlVolume()
	:fMainVolume(1.f)
	,fFadeVolume(1.f)
	,fWhenMoviePlayVolume(1.f) {}
};

struct sNtlPitch
{	
	float		fMainPitch;					///< ���� : 0.0f ~ 10.f( default : 1.f )
	float		fPitchRate;					///< ���� : ���� ����
	float		fSlowMotionPitchRate;		///< ���� : ���� ����

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