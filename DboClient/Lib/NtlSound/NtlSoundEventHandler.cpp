#include "precomp_ntlsound.h"
#include "NtlSoundEventHandler.h"

#include "NtlSound.h"
#include "NtlSoundEvent.h"
#include "NtlSoundManager.h"
#include "NtlBGMGroup.h"
#include "NtlFadeInOut.h"
#include "NtlSoundGlobal.h"


CNtlSoundEventHandler::CNtlSoundEventHandler()
{
}

CNtlSoundEventHandler::~CNtlSoundEventHandler()
{
}


RwBool CNtlSoundEventHandler::Create(void)
{
	NTL_FUNCTION("CNtlSoundEventHandler::Create");

	RegisterMsg(g_EventSoundReleaseSound, "g_EventSoundReleaseSoundStr", "g_EventSoundReleaseSound");
	RegisterMsg(g_EventSoundFinishFade, "g_EventSoundFinishFadeStr", "g_EventSoundFinishFade");
	RegisterMsg(g_EventSoundEventMusic, "g_EventSoundEventMusic", "g_EventSoundEventMusic");
	RegisterMsg(g_EventSoundRestTime, "g_EventSoundRestTime", "g_EventSoundRestTime");
	RegisterMsg(g_EventSoundKnockdown, "g_EventSoundKnockdown", "g_EventSoundKnockdown");
	RegisterMsg(g_EventSoundMinMaxRate, "g_EventSoundMinMaxRate", "g_EventSoundMinMaxRate");
	RegisterMsg(g_EventSoundAddListRangeBGM, "g_EventSoundAddListRangeBGM", "g_EventSoundAddRangeBGM");
	RegisterMsg(g_EventSoundDelListRangeBGM, "g_EventSoundDelListRangeBGM", "g_EventSoundDelRangeBGM");
	RegisterMsg(g_EventSoundChangeListRangeBGM, "g_EventSoundChangeListRangeBGM", "g_EventSoundChangeRangeBGM");
	RegisterMsg(g_EventSoundAddListShareBGM, "g_EventSoundAddListShareBGM", "g_EventSoundAddListShareBGM");
	RegisterMsg(g_EventSoundDeleteAllListShareBGM, "g_EventSoundDeleteAllListShareBGM", "g_EventSoundDeleteAllListShareBGM");
	RegisterMsg(g_EventSoundShareBGMPlayRate, "g_EventSoundShareBGMPlayRate", "g_EventSoundShareBGMPlayRate");
	RegisterMsg(g_EventSoundReleaseObjectGroup, "g_EventSoundReleaseObjectGroup", "g_EventSoundReleaseObjectGroup");
	
	LinkMsg(g_EventSoundEventMusic);
	LinkMsg(g_EventSoundKnockdown);
	LinkMsg(g_EventSoundMinMaxRate);

	NTL_RETURN(TRUE);
}

void CNtlSoundEventHandler::Destroy(void)
{
	NTL_FUNCTION("CNtlSoundEventHandler::Destroy");

    UnLinkMsg(g_EventSoundEventMusic);
    UnLinkMsg(g_EventSoundKnockdown);
    UnLinkMsg(g_EventSoundMinMaxRate);

    UnRegisterMsg(g_EventSoundReleaseSound);
    UnRegisterMsg(g_EventSoundFinishFade);
    UnRegisterMsg(g_EventSoundEventMusic);
    UnRegisterMsg(g_EventSoundRestTime);
    UnRegisterMsg(g_EventSoundKnockdown);
    UnRegisterMsg(g_EventSoundMinMaxRate);
    UnRegisterMsg(g_EventSoundAddListRangeBGM);
    UnRegisterMsg(g_EventSoundDelListRangeBGM);
    UnRegisterMsg(g_EventSoundChangeListRangeBGM);
    UnRegisterMsg(g_EventSoundAddListShareBGM);
    UnRegisterMsg(g_EventSoundDeleteAllListShareBGM);
    UnRegisterMsg(g_EventSoundShareBGMPlayRate);
    UnRegisterMsg(g_EventSoundReleaseObjectGroup);

	NTL_RETURNVOID();
}

void CNtlSoundEventHandler::HandleEvents(RWS::CMsg& pMsg)
{
    NTL_FUNCTION("CNtlSoundEventHandler::HandleEvents");

    if (!GetSoundManager()->IsUsableSound())
        NTL_RETURNVOID();

    if (pMsg.Id == g_EventSoundEventMusic)
    {
        SNtlEeventMusic* pEvent = reinterpret_cast<SNtlEeventMusic*>(pMsg.pData);
        CNtlChannelGroup* pChannelGroup = GetSoundManager()->GetChannelGroup(CHANNEL_GROUP_BGM);
        CNtlFadeInOut* pFadeInOut = GetFadeInOut();

        switch (pEvent->byType)
        {
        case EVENT_MUSIC_EVENT_MUSIC_HAD_PLAY:
            pFadeInOut->StopImmediately(pChannelGroup, dFADE_STOP_FLAG_NOT_NOTIFY);
            pFadeInOut->FadeOut(pChannelGroup, 0.f, 3000);
            break;
        case EVENT_MUSIC_ALL_EVENT_MUSIC_FINISH:
            pFadeInOut->StopImmediately(pChannelGroup, dFADE_STOP_FLAG_NOT_NOTIFY);
            pFadeInOut->FadeIn(pChannelGroup, 1.f, 3000);
            break;
        case EVENT_MUSIC_VOLUME_DOWN_BECAUSE_MOVIE_PLAY:
            CNtlSoundGlobal::m_tMasterVolume.fWhenMoviePlayVolume = 0.f;
            GetSoundManager()->SetMasterVolume(CNtlSoundGlobal::m_tMasterVolume.fMainVolume);
            break;
        case EVENT_MUSIC_VOLUME_UP_BECAUSE_MOVIE_PLAY:
            CNtlSoundGlobal::m_tMasterVolume.fWhenMoviePlayVolume = 1.f;
            GetSoundManager()->SetMasterVolume(CNtlSoundGlobal::m_tMasterVolume.fMainVolume);
            break;
        }
    }
    else if (pMsg.Id == g_EventSoundKnockdown)
    {
        SNtlEventKnockdown* pEvent = reinterpret_cast<SNtlEventKnockdown*>(pMsg.pData);

        for (RwUInt8 i = 0; i < NUM_CHANNEL_GROUP; ++i)
        {
            if (pEvent->bKnockdown)
            {
                GetSoundManager()->GetChannelGroup(i)->SetPitch_SlowMotionRate(0.7f);
            }
            else
            {
                GetSoundManager()->GetChannelGroup(i)->SetPitch_InitSlowMotionRate();
            }
        }
    }
    else if (pMsg.Id == g_EventSoundMinMaxRate)
    {
        SNtlEventMinMaxRate* pEvent = reinterpret_cast<SNtlEventMinMaxRate*>(pMsg.pData);

        CNtlSoundGlobal::m_fMinMaxRate = pEvent->fMinMaxRate;

        for (auto hHandle : CNtlSoundGlobal::m_setPlayingSoundHandle)
        {
            CNtlSound* pSound = GetSoundManager()->GetSound(hHandle);
            if (!pSound)
                continue;

            pSound->SetMinMax(pSound->m_tMinMax.fMin, pSound->m_tMinMax.fMax);
        }
    }

    NTL_RETURNVOID();
}
