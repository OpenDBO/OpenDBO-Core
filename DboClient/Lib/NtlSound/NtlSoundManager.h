/*****************************************************************************
*
* File			: NtlSoundManaager.h
* Author		: Hong SungBock
* Copyright		: (��)NTL
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

	///< FMOD System �ʱ�ȭ, Channel Group �ʱ�ȭ
	///< fDopplerScale : ����( ������ �̵� �ӵ�, �Ҹ��� ��� ��ü�� �̵� �ӵ��� ���� ������ ��ȭ)
	///< fDistanceFactor : �Ҹ��� �鸮�� ������ ����(ex : 1����, 1��Ƽ)
	///< fRollOffScale : ����(�Ÿ��� ���� �ҷ��� ũ�� ����
	void		Init(const char* pcPath, float fMasterVolume = 1.0, float fDopplerScale = 1.0,
					 float fDistacneFactor = 1.0, float fRollOffScale = 1.0);				

	void		Reset();
	void		Release();					///< FMOD �ý��� Shut down

	void		SetListenerPosition(float fXPos, float fYPos, float fZPos);	///< Lister 3D ��ġ ����

	void		SetListenerPosition(float fXPos, float fYPos, float fZPos, 
									float fXFoward, float fYFoward, float fZFoward, 
									float fXUp, float fYUp, float fZUp); 

	int			Play(sNtlSoundPlayParameta* pParameta);

	int			ReplayEnvironmentSound(CNtlSound* pSound);///< ������Ʈ ���� �ٽ� �÷��� �Ѵ�.(�ڵ��� �ٲ��� �ʴ´�)
	int			Replay(CNtlSound* pSound);		///< ������ �÷��̵ǰ� �ִ� ���带 �ٽ� �÷����Ѵ�.

	void		Update(float fElapsed);			///< �� ChannelGroup Update�� FMOD::System->update()

	typedef void (F_CALL* FMOD_DEBUG_CALLBACK)(FMOD_DEBUG_FLAGS flags, const char* file, int line, const char* function, const char* message);


	//////////////////////////////////////////////////////////////////////////
	//				 Master Group
	//////////////////////////////////////////////////////////////////////////

	unsigned int	GetPlayingChannels();			///< ���� ���ֵǰ� sound channel�� ����

	///< ����
	void		SetMasterVolume(float fVolume);
	float		GetMasterVolume();

	// ȿ�� ����
	void		SetMasterEffect(FMOD_DSP_TYPE eType);
	void		ReleaseMasterEffect(FMOD_DSP_TYPE eType);

	void		SetMute(bool bMute);					///< ��ü ������ Mute ����


	//////////////////////////////////////////////////////////////////////////
	//				 Channel Group
	//////////////////////////////////////////////////////////////////////////

	void		StopGroup(int iChannelGroup);	///< ä�� �׷� ��ü�� �ﰢ Release ��Ų��.

	CNtlChannelGroup*	GetChannelGroup(int iChannelGroup);
	char*		GetGroupName(int iChannelGroup);

	void		SetGroupVolume(int iChannelGroup, float fVolume);///< ChannelGroup�� ������ �����Ѵ�
	float		GetGroupVolume(int iChannelGroup);				///< ChannelGroup�� ������ ��ȯ�Ѵ�

	// ȿ�� ����
	void		SetGroupEffect(int iChannelGroup, FMOD_DSP_TYPE eType);
	void		ReleaseGroupEffect(int iChannelGroup, FMOD_DSP_TYPE eType);

	void		SetMute(int iChannelGroup, bool bMute);	///< ChannelGroup�� Ȱ��ȭ�� ���� t : ����, f : �ѱ�

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
	int			SetSoundPosition(SOUND_HANDLE hHandle, float fPosX, float fPosY, float fPosZ);	///< ä���� 3D �������� ��ġ�� �̵���Ų��.

	const char*	GetSoundName(SOUND_HANDLE hHandle);	///< ������ �̸��� ��ȯ�Ѵ�

	void		SetChannelVolume(SOUND_HANDLE hHandle, float fVolume);	///< Channel�� ������ �����Ѵ�
	float		GetChannelVolume(SOUND_HANDLE hHandle);					///< Channel�� ������ ��ȯ�Ѵ�

	// ȿ�� ����
	void		SetChannelEffect(SOUND_HANDLE hHandle, FMOD_DSP_TYPE eType);
	void		ReleaseChannelEffect(SOUND_HANDLE hHandle, FMOD_DSP_TYPE eType);

	void		SetMinMaxDistance(SOUND_HANDLE hHandle, float fMinDistance, float fMaxDistance);
	void		GetMinMaxDistance(SOUND_HANDLE hHandle, float &fMinDistance, float &fMaxDistance);

	void		FadeIn(SOUND_HANDLE hHandle, float fDestVolume, unsigned int ulTime);
	///< fVolume�� ���� �������� ũ�ų� ulTime�� 0 ���ϸ� ������� �ʴ´�

	void		FadeOut(SOUND_HANDLE hHandle, float fDestVolume, unsigned int ulTime);
	///< fVolume�� ���� �������� �۰ų� ulTime�� 0 ���ϸ� ������� �ʴ´�

	CNtlSound*	GetSound(SOUND_HANDLE hHandle);			///< ���� ������ ��ȯ�Ѵ�.

protected:
	void		CreateChannelGroups();		///< Channel Group �� �ʱ�ȭ�Ѵ�	

	int			CanPlay(int iChannelGroups, const char* pcName,
						float fPosX, float fPosY, float fPosZ);	///< ���ο� ���带 ������ �� �ִ��� ����

	int			CanPlay(sNtlSoundPlayParameta* pParameta);

	bool		IsValidGroupRange(int iChannelGroup, float fPosX, float fPosY, float fPosZ);
	///< ä�� �׷캰 ������ �� �ִ� �������� �������� �˻�

	float		LengthFromListenerToSound(float fPosX, float fPosY, float fPosZ);
	bool		IsExistGroup(int iGroup);	///< true : �˸´� Group index, false : �������� �ʴ� �׷� index


protected:
	FMOD::ChannelGroup*		m_pMasterChannelGroup;		///< Top Channel Group of Ntl Sound
	CNtlChannelGroup*		m_apChannelGroup[NUM_CHANNEL_GROUP];	///< Ntl Channel Group		
	CNtlSoundDSP*			m_pMasterDSP;
	
	CNtlSoundSubSystem*		m_pSubSystem;

private: 
	std::map<FMOD_DSP_TYPE, FMOD::DSP*> m_mapMasterDSP;
	std::map<int, std::map<FMOD_DSP_TYPE, FMOD::DSP*>> m_mapGroupDSP;

#ifdef _DEBUG
	int						m_iDebugFlag;				///< crtdbg��
#endif
};

static CNtlSoundManager* GetSoundManager()
{
	return CNtlSoundManager::GetInstance();
}