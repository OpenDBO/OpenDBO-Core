#include "precomp_ntlsound.h"
#include "NtlFadeInOut.h"

#include "NtlSoundEvent.h"
#include "NtlSoundEventGenerator.h"
#include "NtlSound.h"
#include "NtlChannelGroup.h"

#include "NtlSoundGlobal.h"

CNtlFadeInOut* CNtlFadeInOut::m_pInstance = NULL;

CNtlFadeInOut::CNtlFadeInOut()
{
}

CNtlFadeInOut::~CNtlFadeInOut()
{
}

bool CNtlFadeInOut::CreateInstance()
{
    NTL_FUNCTION("CNtlFadeInOut::CreateInstance");

    if (m_pInstance)
    {
        NTL_ASSERT(m_pInstance, "CNtlFadeInOut::CreateInstance, instance already exist");
        NTL_RETURN(FALSE);
    }

    m_pInstance = NTL_NEW CNtlFadeInOut;

    m_pInstance->LinkMsg(g_EventSoundReleaseSound);

    NTL_RETURN(TRUE);
}

void CNtlFadeInOut::DestroyInstance()
{
    NTL_FUNCTION("CNtlFadeInOut::DestroyInstance");

    if (!m_pInstance)
        NTL_RETURNVOID();

    m_pInstance->UnLinkMsg(g_EventSoundReleaseSound);

    NTL_DELETE(m_pInstance);

    NTL_RETURNVOID();
}

void CNtlFadeInOut::FadeIn(CNtlSound* pSound, float fDestVolume, unsigned long ulTime)
{
    if (!pSound)
        return;

    if (ulTime < 0)
        return;

    if (fDestVolume <= pSound->m_tVolume.fFadeVolume)
        return;

    if (pSound->m_pFMODChannel)
    {
        pSound->m_pFMODChannel->setVolumeRamp(true);

        unsigned long long parentclock;
        pSound->m_pFMODChannel->getDSPClock(NULL, &parentclock);


        int sampleRate;
        CNtlSoundGlobal::m_pFMODSystem->getSoftwareFormat(&sampleRate, 0, 0);
        unsigned long long fadeDuration = (ulTime * sampleRate) / 1000;

        // Schedule the volume ramp
        pSound->m_pFMODChannel->addFadePoint(parentclock, pSound->m_tVolume.fFadeVolume);
        pSound->m_pFMODChannel->addFadePoint(parentclock + fadeDuration, fDestVolume);

        pSound->m_eState = SPS_PLAY_FADE_IN;
        pSound->SetFadeVolume(fDestVolume);
        pSound->SetVolume_FadeInit();
    }
}

void CNtlFadeInOut::FadeIn(CNtlChannelGroup* pChannelGroup, float fDestVolume, unsigned long ulTime)
{
    if (!pChannelGroup)
        return;

    if (ulTime < 0)
        return;

    sNtlVolume* pNtlVolume = pChannelGroup->GetNtlVolume();
    if (fDestVolume <= pNtlVolume->fFadeVolume)
        return;

    FMOD::ChannelGroup* pFMODChannelGroup = pChannelGroup->GetFMODChannelGroup();
    if (pFMODChannelGroup)
    {
        pFMODChannelGroup->setVolumeRamp(true);

        unsigned long long parentclock;
        pFMODChannelGroup->getDSPClock(NULL, &parentclock);

        int sampleRate;
        CNtlSoundGlobal::m_pFMODSystem->getSoftwareFormat(&sampleRate, 0, 0);
        unsigned long long fadeDuration = (ulTime * sampleRate * 4) / 1000;

        // Schedule the volume ramp
        pFMODChannelGroup->addFadePoint(parentclock, pNtlVolume->fFadeVolume);
        pFMODChannelGroup->addFadePoint(parentclock + fadeDuration, fDestVolume);

        pChannelGroup->SetFadeVolume(fDestVolume);
        pChannelGroup->SetVolume_FadeInit();
    }
}


void CNtlFadeInOut::FadeOut(CNtlSound* pSound, float fDestVolume, unsigned long ulTime)
{
    if (!pSound)
        return;

    if (ulTime < 0)
        return;

    if (fDestVolume >= pSound->m_tVolume.fFadeVolume)
        return;

    if (pSound->m_pFMODChannel)
    {
        // Enable volume ramping
        pSound->m_pFMODChannel->setVolumeRamp(true);

        unsigned long long parentclock;
        FMOD_RESULT result = pSound->m_pFMODChannel->getDSPClock(NULL, &parentclock);

        int sampleRate;
        CNtlSoundGlobal::m_pFMODSystem->getSoftwareFormat(&sampleRate, 0, 0);
        unsigned long long fadeDuration = (ulTime * sampleRate) / 1000;

        // Schedule the volume ramp
        pSound->m_pFMODChannel->addFadePoint(parentclock, pSound->m_tVolume.fFadeVolume);
        pSound->m_pFMODChannel->addFadePoint(parentclock + fadeDuration, fDestVolume);

        pSound->m_eState = SPS_PLAY_FADE_OUT;
        pSound->SetFadeVolume(fDestVolume);
    }
}

void CNtlFadeInOut::FadeOut(CNtlChannelGroup* pChannelGroup, float fDestVolume, unsigned long ulTime)
{
    if (!pChannelGroup)
        return;

    if (ulTime < 0)
        return;

    sNtlVolume* pNtlVolume = pChannelGroup->GetNtlVolume();
    if (fDestVolume >= pNtlVolume->fFadeVolume)
        return;

    FMOD::ChannelGroup* pFMODChannelGroup = pChannelGroup->GetFMODChannelGroup();
    if (pFMODChannelGroup)
    {
        pFMODChannelGroup->setVolumeRamp(true);

        unsigned long long parentclock;
        FMOD_RESULT result = pFMODChannelGroup->getDSPClock(NULL, &parentclock);

        int sampleRate;
        CNtlSoundGlobal::m_pFMODSystem->getSoftwareFormat(&sampleRate, 0, 0);
        unsigned long long fadeDuration = (ulTime * sampleRate) / 1000;

        // Schedule the volume ramp
        pFMODChannelGroup->addFadePoint(parentclock, pNtlVolume->fFadeVolume);
        pFMODChannelGroup->addFadePoint(parentclock + fadeDuration, fDestVolume);
        pChannelGroup->SetFadeVolume(fDestVolume);
    }
}

void CNtlFadeInOut::StopImmediately(FMOD::Channel* m_pFMODChannel, RwUInt8 byFlag /* = 0 */)
{
    if (m_pFMODChannel)
    {
        m_pFMODChannel->setVolume(0.0f);
        m_pFMODChannel->stop();
    }
}

void CNtlFadeInOut::StopImmediately(CNtlChannelGroup* pChannelGroup, RwUInt8 byFlag /* = 0 */)
{
    FMOD::ChannelGroup* pFMODChannelGroup = pChannelGroup->GetFMODChannelGroup();
    if (pFMODChannelGroup)
    {
        pFMODChannelGroup->setVolume(0.0f);
        pFMODChannelGroup->stop();
    }
}

VOID CNtlFadeInOut::HandleEvents(RWS::CMsg& msg)
{
    NTL_FUNCTION("CNtlFadeInOut::HandleEvents");

    if (msg.Id == g_EventSoundReleaseSound)
    {
        CNtlSound* pReleasingSound = reinterpret_cast<CNtlSound*>(msg.pData);
        if (pReleasingSound && pReleasingSound->m_pFMODChannel)
        {
            pReleasingSound->m_pFMODChannel->setVolume(0.0f);
            pReleasingSound->m_pFMODChannel->stop();
        }
    }

    NTL_RETURNVOID();
}