#include "precomp_ntlsound.h"
#include "NtlChannelGroup.h"

#include <mmsystem.h>

// sound
#include "NtlSoundGlobal.h"
#include "NtlSoundEvent.h"
#include "NtlSound.h"

CNtlChannelGroup::CNtlChannelGroup(eChannelGroupType eGroup)
:m_eGroup(eGroup)
,m_uiSoundDuplication(0)
,m_bProhibition(FALSE)
,m_fValidRange(50.0f)
,m_pMasterLayer(NULL)
{
	m_mapGroup.clear();	
	m_mapDSP.clear();
}

CNtlChannelGroup::~CNtlChannelGroup()
{
	Destory();
}

bool CNtlChannelGroup::Create(FMOD::ChannelGroup* pChannelGroup, unsigned int uiSoundDuplication /* = 0 */)
{
	NTL_FUNCTION( "CNtlChannelGroup::Create" );

	m_pMasterLayer = pChannelGroup;
	m_uiSoundDuplication = uiSoundDuplication;

	NTL_RETURN(TRUE);
}

void CNtlChannelGroup::Reset()
{

}

void CNtlChannelGroup::Update(float fElapsed)
{	
	ReleaseFinishedSound(fElapsed);
}

void CNtlChannelGroup::Destory()
{
	NTL_FUNCTION( "CNtlChannelGroup::Destory" );

	MAP_DSP_ITER dsp_it = m_mapDSP.begin();
	for( ; dsp_it != m_mapDSP.end() ; ++dsp_it )
	{
		FMOD::DSP* pDSP = dsp_it->second;
		pDSP->release();
	}
	m_mapDSP.clear();	

	SOUND_ITER sound_it = m_mapGroup.begin();
	for( ; sound_it != m_mapGroup.end() ; ++sound_it )
	{
		CNtlSound* pSound = sound_it->second;
		pSound->Release();
	}
	m_mapGroup.clear();

	if(m_pMasterLayer)
	{
		m_pMasterLayer->release();
		m_pMasterLayer = NULL;
	}

	NTL_RETURNVOID();
}

eStoreResult CNtlChannelGroup::StoreSound(CNtlSound* pSound, sNtlSoundPlayParameta* pParameta)
{
	m_mapGroup.insert( SOUND_VALUE(pSound->m_hHandle, pSound) );

	// register the channel with this group's channel group
	if( pSound->m_pFMODChannel )
		pSound->m_pFMODChannel->setChannelGroup(m_pMasterLayer);

	return STORE_READY_TO_PLAY;
}

bool CNtlChannelGroup::Stop(SOUND_HANDLE hHandle)
{
	SOUND_ITER it = m_mapGroup.find(hHandle);
	if( it != m_mapGroup.end() )
	{
		CNtlSound* pSound = it->second;		
		pSound->Release();

		m_mapGroup.erase(it);
		return true;
	}

	return false;
}

void CNtlChannelGroup::StopGroup()
{
	SOUND_ITER it = m_mapGroup.begin();
	for( ; it != m_mapGroup.end() ; ++it )
	{
		CNtlSound* pSound = it->second;
		pSound->Release();
	}

	m_mapGroup.clear();
}

CNtlSound* CNtlChannelGroup::GetSound(SOUND_HANDLE hHandle)
{
	SOUND_ITER it = m_mapGroup.find(hHandle);
	if( it != m_mapGroup.end() )
		return it->second;

	return NULL;
}

void CNtlChannelGroup::AddEffect(FMOD_DSP_TYPE eType)
{
	MAP_DSP_ITER it = m_mapDSP.find(eType);
	if( it != m_mapDSP.end() )
		return;

	FMOD::DSP* pDSP;

	CNtlSoundGlobal::m_pFMODSystem->createDSPByType(eType, &pDSP);
	m_pMasterLayer->addDSP((int)m_mapDSP.size(), pDSP);

	m_mapDSP[eType] = pDSP;
}

void  CNtlChannelGroup::RemoveEffect(FMOD_DSP_TYPE eType)
{
	MAP_DSP_ITER it = m_mapDSP.find(eType);
	if( it != m_mapDSP.end() )
		return;

	FMOD::DSP* pDSP = it->second;
	pDSP->release();
}

void  CNtlChannelGroup::RemoveAllEffect()
{
	FMOD::DSP* pDSP;
	MAP_DSP_ITER it = m_mapDSP.begin();
	for( ; it != m_mapDSP.end() ; ++it )
	{
		pDSP = it->second;
		pDSP->release();
	}

	m_mapDSP.clear();
}

bool CNtlChannelGroup::ReleaseLowRankChannel()
{
	// release the oldest channel in this group to free a slot
	SOUND_ITER it = m_mapGroup.begin();

	CNtlSound* pSound = it->second;
	pSound->Release();

	m_mapGroup.erase(it);

	return true;
}

int CNtlChannelGroup::CanPlay(const char* pcName)
{
	// if m_uiSoundDuplication is 0 there's no limit on playing the same sound repeatedly
	if( m_uiSoundDuplication > 0 )
	{
		unsigned int uiCount = 0;
		unsigned int uiMS = 0;

		if(pcName)
		{
			SOUND_ITER it = m_mapGroup.begin();
			for( ; it != m_mapGroup.end() ; ++it )
			{
				CNtlSound* pSound = it->second;
				if( pSound && pSound->m_strName.c_str() )
				{
					if( strcmp(pSound->m_strName.c_str(), pcName) == 0 )
					{
						++uiCount;

						if( m_eGroup == CHANNEL_GROUP_AVATAR_VOICE_SOUND ||
							m_eGroup == CHANNEL_GROUP_AVATAR_EFFECT_SOUND ||
							m_eGroup == CHANNEL_GROUP_VOICE_SOUND ||
							m_eGroup == CHANNEL_GROUP_EFFECT_SOUND )
						{
							unsigned int uiTempMS;
							pSound->m_pFMODChannel->getPosition(&uiTempMS, FMOD_TIMEUNIT_MS);

							if( uiTempMS > uiMS )
								uiMS = uiTempMS;
						}						
					}
				}
			}
		}

		if(m_uiSoundDuplication > 1)
		{
			// exceeded the maximum duplicate play count
			if( uiCount > m_uiSoundDuplication )
			{
#ifdef SOUND_DEBUG_LOG
				Logic_NtlSoundLog("CNtlChannelGroup::CanPlay, Too many number than duplication of the same name file", m_eGroup, pcName);
#endif
				return SOUNDRESULT_FULL_DUPLICATION;
			}

			// not enough time has passed since the same-named effect sound started playing
			if( uiMS >= DELAY_EFFECT_SOUND_TIME )
				return SOUNDRESULT_MORE_WAIT_EFFECT_SOUND;
		}
		else
		{
			// a sound with the same name is already playing
			if( uiCount == 1 )
#ifdef SOUND_DEBUG_LOG
				Logic_NtlSoundLog("CNtlChannelGroup::CanPlay, Already play same name sound", m_eGroup, pcName);
#endif
				return SOUNDRESULT_ALREADY_PLAY_SAME_SOUND;
		}
	}

	return SOUNDRESULT_OK;
}

void CNtlChannelGroup::SetMute(bool bMute)
{
	m_pMasterLayer->setMute(bMute);
}

bool CNtlChannelGroup::IsMute()
{
	bool bMute;
	m_pMasterLayer->getMute(&bMute);
	return bMute;
}

void CNtlChannelGroup::SetProhibition(RwBool bProhibition)
{
	m_bProhibition = bProhibition;
}

RwBool CNtlChannelGroup::IsProhibition()
{
	return m_bProhibition;
}

void CNtlChannelGroup::SetValidGroupRange(float fRange)
{
	m_fValidRange = fRange;
}

float  CNtlChannelGroup::GetValidGroupRange()
{
	return m_fValidRange;
}

void CNtlChannelGroup::SetGroupVolume(float fVolume)
{
	m_tVolume.fMainVolume = Logic_GetFMODValidVolume(fVolume);
	m_pMasterLayer->setVolume( Logic_CalcPlayVolume(&m_tVolume) );
}

bool CNtlChannelGroup::SetChannelVolume(SOUND_HANDLE hHandle, float fVolume)
{
	CNtlSound* pSound = GetSound(hHandle);
	if( !pSound )
		return false;

	if( !pSound->m_pFMODChannel )
		return false;
	

	fVolume = Logic_GetFMODValidVolume(fVolume);
	pSound->m_pFMODChannel->setVolume(fVolume);
	return true;
}

void CNtlChannelGroup::SetVolume_FadeInit()
{
	m_tVolume.fFadeVolume = Logic_GetFMODValidVolume(1.f);

	if( m_pMasterLayer )
		m_pMasterLayer->setVolume( Logic_CalcPlayVolume(&m_tVolume) );
}

void CNtlChannelGroup::SetFadeVolume(float volume) {
	m_tVolume.fFadeVolume = Logic_GetFMODValidVolume(volume);
}

float CNtlChannelGroup::GetGroupVolume()
{
	return m_tVolume.fMainVolume;
}

sNtlVolume* CNtlChannelGroup::GetNtlVolume()
{
	return &m_tVolume;
}

void CNtlChannelGroup::SetGroupPitch(float fPitch)
{
	m_tPitch.fMainPitch = Logic_GetFMODValidPitch(fPitch);
	m_pMasterLayer->setPitch( Logic_CalcPlayPitch(&m_tPitch) );
}

void CNtlChannelGroup::SetPitch_SlowMotionRate(float fPitchRate)
{
	m_tPitch.fSlowMotionPitchRate = fPitchRate;
	m_pMasterLayer->setPitch( Logic_CalcPlayPitch(&m_tPitch) );
}

void CNtlChannelGroup::SetPitch_InitSlowMotionRate()
{
	m_tPitch.fSlowMotionPitchRate = 1.f;
	m_pMasterLayer->setPitch( Logic_CalcPlayPitch(&m_tPitch) );
}

float CNtlChannelGroup::GetGroupPitch()
{
	return m_tPitch.fMainPitch;
}

eChannelGroupType CNtlChannelGroup::GetGroupType()
{
	return m_eGroup;
}

unsigned int CNtlChannelGroup::GetPlayingChannels()
{
	int iChannels = 0;
	m_pMasterLayer->getNumChannels(&iChannels);
	return iChannels;
}

unsigned int CNtlChannelGroup::GetSoundDuplication() 
{
	return m_uiSoundDuplication;
}


FMOD::ChannelGroup* CNtlChannelGroup::GetFMODChannelGroup()
{
	return m_pMasterLayer;
}

bool CNtlChannelGroup::IsExistSound(SOUND_HANDLE handle)
{
	SOUND_ITER it = m_mapGroup.find(handle);
	if( it != m_mapGroup.end() )
		return true;

	return false;
}

void CNtlChannelGroup::ReleaseFinishedSound(float fElapsed)
{	
	FMOD_MODE mode;

	SOUND_ITER it = m_mapGroup.begin();
	while( it != m_mapGroup.end() )
	{
		CNtlSound* pSound = it->second;
		if( pSound->m_pFMODChannel && pSound->m_pFMODSound )
		{			
			pSound->m_pFMODChannel->getMode(&mode);

			if( !(mode & FMOD_LOOP_NORMAL) )
			{	// when not looping infinitely
				bool bPlaying;

				// isPlaying still returns true when paused
				pSound->m_pFMODChannel->isPlaying(&bPlaying);

				if(!bPlaying)
				{					
					pSound->Release();
					it = m_mapGroup.erase(it);

					continue;
				}

			}			
		}

		++it;
	}		
}