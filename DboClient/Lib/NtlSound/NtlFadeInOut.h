/*****************************************************************************
*
* File			: NtlFadeInOut.h
* Author		: Hong SungBock
* Copyright		: (주)NTL
* Date			: 2006. 7. 12	
* Abstract		: Fade In/Out Effect
*****************************************************************************
* Desc			: Channel, ChannelGroup에 Effect를 적용할 수 있다
*
*				  FMOD System에서 Fade In/Out Effect를 제공하지 않아서
*				  따로 구현하였다.
*****************************************************************************/

#pragma once

#include <list>
#include "ceventhandler.h"
#include "fmod/fmod.hpp"

class CNtlSound;
class CNtlChannelGroup;

#define dFADE_STOP_FLAG_NOT_NOTIFY 0x01

class CNtlFadeInOut : public RWS::CEventHandler
{
public:
    static bool CreateInstance();
    static void DestroyInstance();
    static CNtlFadeInOut* GetInstance() { return m_pInstance; }

    CNtlFadeInOut();
    virtual ~CNtlFadeInOut();

    void FadeIn(CNtlSound* pSound, float fDestVolume, unsigned long ulTime);
    void FadeIn(CNtlChannelGroup* pChannelGroup, float fDestVolume, unsigned long ulTime);

    void FadeOut(CNtlSound* pSound, float fDestVolume, unsigned long ulTime);
    void FadeOut(CNtlChannelGroup* pChannelGroup, float fDestVolume, unsigned long ulTime);

    void StopImmediately(FMOD::Channel* m_pFMODChannel, RwUInt8 byFlag = 0);
    void StopImmediately(CNtlChannelGroup* pChannelGroup, RwUInt8 byFlag = 0);

protected:
    virtual VOID HandleEvents(RWS::CMsg& msg);

public:
    static CNtlFadeInOut* m_pInstance;
};

static CNtlFadeInOut* GetFadeInOut()
{
    return CNtlFadeInOut::GetInstance();
}