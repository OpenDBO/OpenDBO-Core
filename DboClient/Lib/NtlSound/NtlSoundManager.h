/*****************************************************************************
*
* File			: NtlSoundManaager.h
* Author		: Hong SungBock
* Copyright		: (주)NTL
* Date			: 2006. 7. 11	
* Abstract		: Ntl Sound Manager by Singleton
*****************************************************************************
* Desc			: 배경음악은 보통 1곡만이 연주되고 있으며 다른 배경음악으로 전환될
*				시 이전 배경음악은 Fade Out이 되고 새로운 배경음악은 Fade In
*				된다. 
*				
*				FMOD는 왼손 좌표계를 쓴다.
*
*				fmod에서 볼륨은 0.0 ~ 1.0 까지의 float 값이다. 입력된 볼륨값
*				fVolume < 0.0 일 경우 fVolume = 0.0
*				fVolume > 1.0 일 경우 fVolume = 1.0
*
*				FMOD::sound는 음원에 대한 정보를 가지고 있다.
*				FMOD::Channel은 음원의 현재 Play 정보를 가지고 있다.
*
*				사운드가 플레이가 끝난지 알아보기 위하여 isPlaying 함수를 썻다.
*				FMOD에서 전체 사운드의 길이와 현재 플레이중인 사운드의 오프셋을 비교하게
*				되면 플레이가 끝난 사운드의 현재 오프셋이 0이 되어 플레이가
*				끝난지 제대로 알 수가 없다.
*
*				배경음악은 stream 방식으로 읽어들여 background loading을 한다
*				배경음악이 Loop 모드일 경우 다른 배경음악을 연주하게 될 때
*				Play(..) 함수에 Loop 모드 인자를 주어도 적용되지 않는다.
*				배경음악이 바뀔 때 PostMusic이 있다면 PostMusic의 상태를
*				새로운 배경음악에 적용하기에 이 때는 SetBackgroundMusicLoop()
*				을 호출해야 한다.
*
*				상황에 따른 사운드 연출을 위해서 SoundManager의 인터페이스를
*				늘리기 보다는 이벤트 핸들을 통해서 Sound Layer에서 직접 컨트롤
*				하자
*****************************************************************************/

#pragma once

#include "NtlSoundMessage.h"
#include "NtlSoundDefines.h"

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

	///< FMOD System 초기화, Channel Group 초기화
	///< fDopplerScale : 반향( 음원의 이동 속도, 소리를 듣는 주체의 이동 속도에 따른 음파의 변화)
	///< fDistanceFactor : 소리가 들리는 길이의 비율(ex : 1미터, 1센티)
	///< fRollOffScale : 감쇠(거리에 따른 불륨의 크기 감소
	void		Init(const char* pcPath, float fMasterVolume = 1.0, float fDopplerScale = 1.0,
					 float fDistacneFactor = 1.0, float fRollOffScale = 1.0);				

	void		Reset();
	void		Release();					///< FMOD 시스템 Shut down

	void		SetListenerPosition(float fXPos, float fYPos, float fZPos);	///< Lister 3D 위치 설정

	void		SetListenerPosition(float fXPos, float fYPos, float fZPos, 
									float fXFoward, float fYFoward, float fZFoward, 
									float fXUp, float fYUp, float fZUp); 

	int			Play(sNtlSoundPlayParameta* pParameta);

	int			ReplayEnvironmentSound(CNtlSound* pSound);///< 오브젝트 뮤직 다시 플레이 한다.(핸들이 바뀌지 않는다)
	int			Replay(CNtlSound* pSound);		///< 기존에 플레이되고 있는 사운드를 다시 플레이한다.

	void		Update(float fElapsed);			///< 각 ChannelGroup Update와 FMOD::System->update()



	//////////////////////////////////////////////////////////////////////////
	//				 Master Group
	//////////////////////////////////////////////////////////////////////////

	unsigned int	GetPlayingChannels();			///< 현재 연주되고 sound channel의 갯수

	///< 볼륨
	void		SetMasterVolume(float fVolume);
	float		GetMasterVolume();

	// 효과 적용
	void		SetMasterEffect(FMOD_DSP_TYPE eType);
	void		ReleaseMasterEffect(FMOD_DSP_TYPE eType);

	void		SetMute(bool bMute);					///< 전체 사운드의 Mute 설정


	//////////////////////////////////////////////////////////////////////////
	//				 Channel Group
	//////////////////////////////////////////////////////////////////////////

	void		StopGroup(int iChannelGroup);	///< 채널 그룹 전체를 즉각 Release 시킨다.

	CNtlChannelGroup*	GetChannelGroup(int iChannelGroup);
	char*		GetGroupName(int iChannelGroup);

	void		SetGroupVolume(int iChannelGroup, float fVolume);///< ChannelGroup의 볼륨을 설정한다
	float		GetGroupVolume(int iChannelGroup);				///< ChannelGroup의 볼륨을 반환한다

	// 효과 적용
	void		SetGroupEffect(int iChannelGroup, FMOD_DSP_TYPE eType);
	void		ReleaseGroupEffect(int iChannelGroup, FMOD_DSP_TYPE eType);

	void		SetMute(int iChannelGroup, bool bMute);	///< ChannelGroup의 활성화를 설정 t : 끄기, f : 켜기

	void		SetProhibition(int iChannelGroup, RwBool bProhibition);
	RwBool		IsProhibition(int iChannelGroup);

	void		SetValidGroupRange(int iChannelGroup, float fRange);	///< 채널 그룹별 연주 가능한 거리를 설정한다.
	float		GetValidGroupRange(int iChannelGroup);				///< 채널 그룹별 연주 가능한 거리를 알아본다.

	void		FadeIn(int iGroup, float fDestVolume, unsigned int ulTime);
	///< fVolume이 현재 볼륨보다 크거나 ulTime이 0 이하면 실행되지 않는다

	void		FadeOut(int iGroup, float fDestVolume, unsigned int ulTime);
	///< fVolume이 현재 볼륨보다 작거나 ulTime이 0 이하면 실행되지 않는다

	bool		IsMute(int iChannelGroup);


	//////////////////////////////////////////////////////////////////////////
	//				 Channel
	//////////////////////////////////////////////////////////////////////////

	void		Stop(SOUND_HANDLE& rHandle);	///< 사운드를 Release 시킨다.
	int			SetSoundPosition(SOUND_HANDLE hHandle, float fPosX, float fPosY, float fPosZ);	///< 채널의 3D 공간상의 위치를 이동시킨다.

	const char*	GetSoundName(SOUND_HANDLE hHandle);	///< 사운드의 이름을 반환한다

	void		SetChannelVolume(SOUND_HANDLE hHandle, float fVolume);	///< Channel의 볼륨을 설정한다
	float		GetChannelVolume(SOUND_HANDLE hHandle);					///< Channel의 볼륨을 반환한다

	// 효과 적용
	void		SetChannelEffect(SOUND_HANDLE hHandle, FMOD_DSP_TYPE eType);
	void		ReleaseChannelEffect(SOUND_HANDLE hHandle, FMOD_DSP_TYPE eType);

	void		SetMinMaxDistance(SOUND_HANDLE hHandle, float fMinDistance, float fMaxDistance);
	void		GetMinMaxDistance(SOUND_HANDLE hHandle, float &fMinDistance, float &fMaxDistance);

	void		FadeIn(SOUND_HANDLE hHandle, float fDestVolume, unsigned int ulTime);
	///< fVolume이 현재 볼륨보다 크거나 ulTime이 0 이하면 실행되지 않는다

	void		FadeOut(SOUND_HANDLE hHandle, float fDestVolume, unsigned int ulTime);
	///< fVolume이 현재 볼륨보다 작거나 ulTime이 0 이하면 실행되지 않는다

	CNtlSound*	GetSound(SOUND_HANDLE hHandle);			///< 사운드 정보를 반환한다.

protected:
	void		CreateChannelGroups();		///< Channel Group 을 초기화한다	

	int			CanPlay(int iChannelGroups, const char* pcName,
						float fPosX, float fPosY, float fPosZ);	///< 새로운 사운드를 연주할 수 있는지 여부

	int			CanPlay(sNtlSoundPlayParameta* pParameta);

	bool		IsValidGroupRange(int iChannelGroup, float fPosX, float fPosY, float fPosZ);
	///< 채널 그룹별 연주할 수 있는 범위내에 사운드인지 검사

	float		LengthFromListenerToSound(float fPosX, float fPosY, float fPosZ);
	bool		IsExistGroup(int iGroup);	///< true : 알맞는 Group index, false : 존재하지 않는 그룹 index


protected:
	FMOD::ChannelGroup*		m_pMasterChannelGroup;		///< Ntl Sound의 최상위 Channel Group
	CNtlChannelGroup*		m_apChannelGroup[NUM_CHANNEL_GROUP];	///< Ntl Channel Group		
	CNtlSoundDSP*			m_pMasterDSP;
	
	CNtlSoundSubSystem*		m_pSubSystem;

#ifdef _DEBUG
	int						m_iDebugFlag;				///< crtdbg용
#endif
};

static CNtlSoundManager* GetSoundManager()
{
	return CNtlSoundManager::GetInstance();
}