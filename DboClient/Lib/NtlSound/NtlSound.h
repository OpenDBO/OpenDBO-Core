/*****************************************************************************
*
* File			: NtlSound.h
* Author		: Hong SungBock
* Copyright		: (주)NTL
* Date			: 2008. 4. 3
* Abstract		: 
*****************************************************************************
* Desc			: Dbo에서 쓰이는 개별 사운드를 다룬다
*****************************************************************************/

#pragma once

#include "NtlSoundDefines.h"

class CNtlSoundDSP;

class CNtlSound
{
public:
	static void*	operator new(size_t size);
	static void		operator delete(void *pObj);
	static void		Shutdown();

	CNtlSound(SOUND_HANDLE hHandle, char* pcFileName);
	virtual ~CNtlSound();

	void			Release();
	void			FreeMemoryData();

	void			SetVolume_FadeInit();

	void			SetMinMax(float fMin, float fMax);

public:
	static RwFreeList*	m_pFreeList;

	SOUND_HANDLE		m_hHandle;
	int					m_iChannelGroup;
	std::string			m_strName;
	eSoundPlayState		m_eState;
	bool				m_bLoop;
	bool				m_bCreatedfromSoundPool;	///< Sound pool에서 FMOD::Sound를 가지고 왔다
	sNtlVolume			m_tVolume;
	sNtlMinMax			m_tMinMax;
	float				m_fXPos;
	float				m_fYPos;
	float				m_fZPos;
	float				m_fMinDistance;
	float				m_fMaxDistance;
	FMOD::Sound*		m_pFMODSound;				///< 음원에 대한 정보
	FMOD::Channel*		m_pFMODChannel;				///< 음원의 현재 Play 정보를 가지고 있다
	CNtlSoundDSP*		m_pDSP;						///< FMOD 이펙트 컨테이너
	char*				pMemoryData;				///< Memory로 로드된 사운드 파일을 가리키는 포인터
};