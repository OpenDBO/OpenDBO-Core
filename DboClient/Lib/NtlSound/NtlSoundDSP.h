/*****************************************************************************
*
* File			: NtlSoundDSP.h
* Author		: Hong SungBock
* Copyright		: (주)NTL
* Date			: 2008. 4. 3
* Abstract		: FMOD의 DSP(Disital Signal Processor)를 관리한다
*****************************************************************************
* Desc			: FMOD의 시스템, 채널그룹, 채널에 효과를 적용하기 위한 클래스
*				  효과를 적용한 대상에게 중복해서 같은 효과를 적용할 수 있으나
*				  소리가 좋지 못하고 각 효과에 인자를 주어 조절할 수 있어
*				  중복해서 같은 효과를 적용하지 못하도록 하였다.
*				  그러나, 시스템(마스터 그룹), 채널그룹, 채널에 같은 효과를
*				  주어 중복해서 효과를 줄 수는 있으나 권장하지 않는다.
*
*				  주의) 개별 효과의 파라메타의 범위는 모두 다르다. 현재는
*				  고정된 파라메타 값을 사용한다.
*****************************************************************************/

#pragma once

#include <map>

class CNtlSoundDSP
{
public:
	typedef std::map<RwUInt8, FMOD::DSP*>				MAP_DSP;
	typedef std::map<RwUInt8, FMOD::DSP*>::iterator		MAP_DSP_ITER;

	CNtlSoundDSP() {}
	~CNtlSoundDSP();

	VOID		Destroy();

	FMOD::DSP*	CreateDSP(FMOD_DSP_TYPE eType);
	VOID		ReleaseDSP(FMOD_DSP_TYPE eType);

	VOID		SetParameta(FMOD_DSP_TYPE eType, RwUInt8 byParametaType, RwReal fValue);
	RwReal		GetParameta(FMOD_DSP_TYPE eType, RwUInt8 byParametaType);

	RwBool		IsExistDSP(FMOD_DSP_TYPE eType);

protected:
	VOID		SetDetailParameta(FMOD_DSP_TYPE eType, FMOD::DSP* pDSP);

protected:
	MAP_DSP		m_mapDSP;
};