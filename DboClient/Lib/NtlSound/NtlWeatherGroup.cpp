#include "precomp_ntlsound.h"
#include "NtlWeatherGroup.h"

#include "NtlSoundEventGenerator.h"
#include "NtlSoundGlobal.h"
#include "NtlFadeInOut.h"
#include "NtlSound.h"

CNtlWeatherGroup::CNtlWeatherGroup(eChannelGroupType eGroup)
:CNtlChannelGroup(eGroup)
,m_hRecoverySoundHandle(INVALID_SOUND_HANDLE)
{
	LinkMsg(g_EventSoundFinishFade);
}

CNtlWeatherGroup::~CNtlWeatherGroup()
{
	UnLinkMsg(g_EventSoundFinishFade);
	Destory();
}

eStoreResult CNtlWeatherGroup::StoreSound(CNtlSound* pSound, sNtlSoundPlayParameta* pParameta)
{
	GetFadeInOut()->FadeIn(pSound, 1.0f, dBGM_FADE_OUT_TIME);
	return CNtlChannelGroup::StoreSound(pSound, pParameta);
}

bool CNtlWeatherGroup::Stop(SOUND_HANDLE hHandle)
{
	SOUND_ITER it = m_mapGroup.find(hHandle);
	if( it != m_mapGroup.end() )
	{
		CNtlSound* pSound = it->second;
		GetFadeInOut()->FadeOut(pSound, 0.0f, dBGM_FADE_OUT_TIME);
		return true;
	}

	return false;
}

void CNtlWeatherGroup::StopGroup()
{
	SOUND_ITER it = m_mapGroup.begin();
	for( ; it != m_mapGroup.end() ; ++it )
	{
		CNtlSound* pSound = it->second;
		GetFadeInOut()->FadeOut(pSound, 0.0f, dBGM_FADE_OUT_TIME);
	}
}

int CNtlWeatherGroup::CanPlay(const char* pcName)
{
	if(!pcName)
		return SOUNDRESULT_EMPTY_FILENAME;

	SOUND_ITER it = m_mapGroup.begin();
	for( ; it != m_mapGroup.end() ; ++it )
	{
		CNtlSound* pSound = it->second;
		if( pSound && pSound->m_strName.c_str() )
		{
			if( strcmp(pSound->m_strName.c_str(), pcName) == 0 )
			{
				// Fade out되어 사라지는 중이면 다시 Fade in 시킨다
				if( pSound->m_eState == SPS_PLAY_FADE_OUT )
				{
					GetFadeInOut()->StopImmediately(pSound->m_pFMODChannel, dFADE_STOP_FLAG_NOT_NOTIFY);
					GetFadeInOut()->FadeIn(pSound, 1.0f, dBGM_FADE_OUT_TIME);

					m_hRecoverySoundHandle = pSound->m_hHandle;

					return SOUNDRESULT_RECOVERY_FROM_FADE_OUT;
				}
				else
				{
					return SOUNDRESULT_ALREADY_PLAY_SAME_SOUND;
				}
			}
		}
	}

	return SOUNDRESULT_OK;
}

SOUND_HANDLE CNtlWeatherGroup::GetRecoverySoundHandle()
{
	return m_hRecoverySoundHandle;
}

VOID CNtlWeatherGroup::HandleEvents(RWS::CMsg& msg)
{
	NTL_FUNCTION("CNtlWeatherGroup::HandleEvents");

	if (msg.Id == g_EventSoundFinishFade)
	{
		// Handle the event based on the new logic
		// Since sFadeInOut is no longer used, we need to adapt the logic accordingly
		// Assuming we have a way to identify the sound/channel that finished fading

		// Example logic (you may need to adapt this based on your actual requirements):
		CNtlSound* pSound = reinterpret_cast<CNtlSound*>(msg.pData);

		if (pSound)
		{
			SOUND_ITER it = m_mapGroup.begin();
			for (; it != m_mapGroup.end(); ++it)
			{
				if (pSound->m_hHandle == (*it->second).m_hHandle)
				{
					if (pSound->m_hHandle == m_hRecoverySoundHandle)
					{
						m_hRecoverySoundHandle = INVALID_SOUND_HANDLE;
						break;
					}
					else
					{
						if (pSound->m_eState == SPS_PLAY_FADE_OUT)
							CNtlChannelGroup::Stop(it->first);

						break;
					}
				}
			}
		}
	}
}