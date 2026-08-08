/*****************************************************************************
*
* File			: NtlSoundManaager.h
* Author		: Hong SungBock
* Copyright		: (占쏙옙)NTL
* Date			: 2006. 7. 11	
* Abstract		: Ntl Sound Manager by Singleton
*****************************************************************************
* Desc: Background music is usually played only one song and is converted to another background music
* The previous background music is Fade Out and the new background music is Fade In
*				do.
*
* FMOD uses the left hand coordinate system.
*
* In fmod, the volume is a float value between 0.0 and 1.0. Entered volume value
* fVolume <0.0 fVolume = 0.0
* fVolume> 1.0 fVolume = 1.0
*
* FMOD :: sound has information about the sound source.
* FMOD :: Channel has current play information of sound source.
*
* The isPlaying function is called to see if the sound has finished playing.
* In FMOD, compare the length of the entire sound with the offset of the sound currently playing.
*, The current offset of the finished sound is 0
I can not figure it out.
*
* Background music is read in stream format and background loading
* When background music is in Loop mode When you play another background music
* The Loop mode argument to the Play (..) function does not apply.
* If you have PostMusic when background music changes, you can change PostMusic status
* To apply to new background music SetBackgroundMusicLoop ()
* Should be called.
*
* SoundManager interface for sounding situations
Control directly from the Sound Layer via event handlers rather than stretching
*				lets do it
*****************************************************************************/

#pragma once

#include "NtlSoundMessage.h"
#include "NtlSoundDefines.h"

#pragma comment(lib, "fmod.lib")


class CNtlChannelGroup;
class CNtlSoundDSP;
class CNtlSound;
class CNtlSoundSubSystem;

class CNtlSoundManager
{
protected:
	CNtlSoundManager();
	CNtlSoundManager(const CNtlSoundManager& soundManager);

public:
	virtual ~CNtlSoundManager();

	static CNtlSoundManager* GetInstance();

	RwBool		IsUsableSound();


	// System

	///< FMOD System 占십깍옙화, Channel Group 占십깍옙화
	///< fDopplerScale : 반향( 음원의 이동 속도, 소리를 듣는 주체의 이동 속도에 따른 음파의 변화)
	///< fDistanceFactor : �Ҹ��� �鸮�� ������ ����(ex : 1����, 1��Ƽ)
	///< fRollOffScale : 占쏙옙占쏙옙(占신몌옙占쏙옙 占쏙옙占쏙옙 占쌀뤄옙占쏙옙 크占쏙옙 占쏙옙占쏙옙
	void		Init(const char* pcPath, float fMasterVolume = 1.0, float fDopplerScale = 1.0,
					 float fDistacneFactor = 1.0, float fRollOffScale = 1.0);				

	void		Reset();
	void		Release();					///< FMOD 占시쏙옙占쏙옙 Shut down

	void		SetListenerPosition(float fXPos, float fYPos, float fZPos);	///< Lister 3D 占쏙옙치 占쏙옙占쏙옙

	void		SetListenerPosition(float fXPos, float fYPos, float fZPos, 
									float fXFoward, float fYFoward, float fZFoward, 
									float fXUp, float fYUp, float fZUp); 

	int			Play(sNtlSoundPlayParameta* pParameta);

	int			ReplayEnvironmentSound(CNtlSound* pSound);///< 占쏙옙占쏙옙占쏙옙트 占쏙옙占쏙옙 占쌕쏙옙 占시뤄옙占쏙옙 占싼댐옙.(占쌘듸옙占쏙옙 占쌕뀐옙占쏙옙 占십는댐옙)
	int			Replay(CNtlSound* pSound);		///< ������ �÷��̵ǰ� �ִ� ���带 �ٽ� �÷����Ѵ�.

	void		Update(float fElapsed);			///< 占쏙옙 ChannelGroup Update占쏙옙 FMOD::System->update()

	typedef void (F_CALL* FMOD_DEBUG_CALLBACK)(FMOD_DEBUG_FLAGS flags, const char* file, int line, const char* function, const char* message);


	//////////////////////////////////////////////////////////////////////////
	//				 Master Group
	//////////////////////////////////////////////////////////////////////////

	unsigned int	GetPlayingChannels();			///< 占쏙옙占쏙옙 占쏙옙占쌍되곤옙 sound channel占쏙옙 占쏙옙占쏙옙

	///< 占쏙옙占쏙옙
	void		SetMasterVolume(float fVolume);
	float		GetMasterVolume();

	// 효占쏙옙 占쏙옙占쏙옙
	void		SetMasterEffect(FMOD_DSP_TYPE eType);
	void		ReleaseMasterEffect(FMOD_DSP_TYPE eType);

	void		SetMute(bool bMute);					///< 占쏙옙체 占쏙옙占쏙옙占쏙옙 Mute 占쏙옙占쏙옙


	//////////////////////////////////////////////////////////////////////////
	//				 Channel Group
	//////////////////////////////////////////////////////////////////////////

	void		StopGroup(int iChannelGroup);	///< 채占쏙옙 占쌓뤄옙 占쏙옙체占쏙옙 占쏙각 Release 占쏙옙킨占쏙옙.

	CNtlChannelGroup*	GetChannelGroup(int iChannelGroup);
	char*		GetGroupName(int iChannelGroup);

	void		SetGroupVolume(int iChannelGroup, float fVolume);///< ChannelGroup占쏙옙 占쏙옙占쏙옙占쏙옙 占쏙옙占쏙옙占싼댐옙
	float		GetGroupVolume(int iChannelGroup);				///< ChannelGroup占쏙옙 占쏙옙占쏙옙占쏙옙 占쏙옙환占싼댐옙

	// 효占쏙옙 占쏙옙占쏙옙
	void		SetGroupEffect(int iChannelGroup, FMOD_DSP_TYPE eType);
	void		ReleaseGroupEffect(int iChannelGroup, FMOD_DSP_TYPE eType);

	void		SetMute(int iChannelGroup, bool bMute);	///< ChannelGroup占쏙옙 활占쏙옙화占쏙옙 占쏙옙占쏙옙 t : 占쏙옙占쏙옙, f : 占싼깍옙

	void		SetProhibition(int iChannelGroup, RwBool bProhibition);
	RwBool		IsProhibition(int iChannelGroup);

	void		SetValidGroupRange(int iChannelGroup, float fRange);	///< ä�� �׷캰 ���� ������ �Ÿ��� �����Ѵ�.
	float		GetValidGroupRange(int iChannelGroup);				///< ä�� �׷캰 ���� ������ �Ÿ��� �˾ƺ���.

	void		FadeIn(int iGroup, float fDestVolume, unsigned int ulTime);
	///< fVolume�� ���� �������� ũ�ų� ulTime�� 0 ���ϸ� ������� �ʴ´�

	void		FadeOut(int iGroup, float fDestVolume, unsigned int ulTime);
	///< fVolume�� ���� �������� �۰ų� ulTime�� 0 ���ϸ� ������� �ʴ´�

	bool		IsMute(int iChannelGroup);


	//////////////////////////////////////////////////////////////////////////
	//				 Channel
	//////////////////////////////////////////////////////////////////////////

	void		Stop(SOUND_HANDLE& rHandle);	///< ���带 Release ��Ų��.
	int			SetSoundPosition(SOUND_HANDLE hHandle, float fPosX, float fPosY, float fPosZ);	///< 채占쏙옙占쏙옙 3D 占쏙옙占쏙옙占쏙옙占쏙옙 占쏙옙치占쏙옙 占싱듸옙占쏙옙킨占쏙옙.

	const char*	GetSoundName(SOUND_HANDLE hHandle);	///< 占쏙옙占쏙옙占쏙옙 占싱몌옙占쏙옙 占쏙옙환占싼댐옙

	void		SetChannelVolume(SOUND_HANDLE hHandle, float fVolume);	///< Channel占쏙옙 占쏙옙占쏙옙占쏙옙 占쏙옙占쏙옙占싼댐옙
	float		GetChannelVolume(SOUND_HANDLE hHandle);					///< Channel占쏙옙 占쏙옙占쏙옙占쏙옙 占쏙옙환占싼댐옙

	// 효占쏙옙 占쏙옙占쏙옙
	void		SetChannelEffect(SOUND_HANDLE hHandle, FMOD_DSP_TYPE eType);
	void		ReleaseChannelEffect(SOUND_HANDLE hHandle, FMOD_DSP_TYPE eType);

	void		SetMinMaxDistance(SOUND_HANDLE hHandle, float fMinDistance, float fMaxDistance);
	void		GetMinMaxDistance(SOUND_HANDLE hHandle, float &fMinDistance, float &fMaxDistance);

	void		FadeIn(SOUND_HANDLE hHandle, float fDestVolume, unsigned int ulTime);
	///< fVolume�� ���� �������� ũ�ų� ulTime�� 0 ���ϸ� ������� �ʴ´�

	void		FadeOut(SOUND_HANDLE hHandle, float fDestVolume, unsigned int ulTime);
	///< fVolume�� ���� �������� �۰ų� ulTime�� 0 ���ϸ� ������� �ʴ´�

	CNtlSound*	GetSound(SOUND_HANDLE hHandle);			///< 占쏙옙占쏙옙 占쏙옙占쏙옙占쏙옙 占쏙옙환占싼댐옙.

protected:
	void		CreateChannelGroups();		///< Channel Group 占쏙옙 占십깍옙화占싼댐옙	

	int			CanPlay(int iChannelGroups, const char* pcName,
						float fPosX, float fPosY, float fPosZ);	///< ���ο� ���带 ������ �� �ִ��� ����

	int			CanPlay(sNtlSoundPlayParameta* pParameta);

	bool		IsValidGroupRange(int iChannelGroup, float fPosX, float fPosY, float fPosZ);
	///< ä�� �׷캰 ������ �� �ִ� �������� �������� �˻�

	float		LengthFromListenerToSound(float fPosX, float fPosY, float fPosZ);
	bool		IsExistGroup(int iGroup);	///< true : 占싯맞댐옙 Group index, false : 占쏙옙占쏙옙占쏙옙占쏙옙 占십댐옙 占쌓뤄옙 index


protected:
	FMOD::ChannelGroup*		m_pMasterChannelGroup;		///< Top Channel Group of Ntl Sound
	CNtlChannelGroup*		m_apChannelGroup[NUM_CHANNEL_GROUP];	///< Ntl Channel Group		
	CNtlSoundDSP*			m_pMasterDSP;
	
	CNtlSoundSubSystem*		m_pSubSystem;

private: 
	std::map<FMOD_DSP_TYPE, FMOD::DSP*> m_mapMasterDSP;
	std::map<int, std::map<FMOD_DSP_TYPE, FMOD::DSP*>> m_mapGroupDSP;

#ifdef _DEBUG
	int						m_iDebugFlag;				///< crtdbg占쏙옙
#endif
};

static CNtlSoundManager* GetSoundManager()
{
	return CNtlSoundManager::GetInstance();
}
