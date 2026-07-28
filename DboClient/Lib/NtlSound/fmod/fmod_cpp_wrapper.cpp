/*
 * fmod_cpp_wrapper.cpp
 *
 * Thin forwarding implementations for FMOD C++ class methods that were
 * previously linked from the static fmod_vc.lib. Each method simply
 * calls the corresponding C API function from fmod.dll.
 *
 * This file is needed because fmod.hpp declares these methods as
 * non-inline (implementations lived in the static lib), but we now
 * link dynamically against the DLL which only exports the C API.
 */

#include "fmod.hpp"

using namespace FMOD;

// ==================== System ====================

FMOD_RESULT System::init(int maxchannels, FMOD_INITFLAGS flags, void *extradriverdata)
{
    return FMOD_System_Init((FMOD_SYSTEM *)this, maxchannels, flags, extradriverdata);
}

FMOD_RESULT System::close()
{
    return FMOD_System_Close((FMOD_SYSTEM *)this);
}

FMOD_RESULT System::update()
{
    return FMOD_System_Update((FMOD_SYSTEM *)this);
}

FMOD_RESULT System::release()
{
    return FMOD_System_Release((FMOD_SYSTEM *)this);
}

FMOD_RESULT System::getVersion(unsigned int *version)
{
    return FMOD_System_GetVersion((FMOD_SYSTEM *)this, version);
}

FMOD_RESULT System::getChannelsPlaying(int *channels, int *realchannels)
{
    return FMOD_System_GetChannelsPlaying((FMOD_SYSTEM *)this, channels, realchannels);
}

FMOD_RESULT System::getMasterChannelGroup(ChannelGroup **channelgroup)
{
    return FMOD_System_GetMasterChannelGroup((FMOD_SYSTEM *)this, (FMOD_CHANNELGROUP **)channelgroup);
}

FMOD_RESULT System::getSoftwareFormat(int *samplerate, FMOD_SPEAKERMODE *speakermode, int *numrawchannels)
{
    return FMOD_System_GetSoftwareFormat((FMOD_SYSTEM *)this, samplerate, speakermode, numrawchannels);
}

FMOD_RESULT System::get3DListenerAttributes(int listener, FMOD_VECTOR *pos, FMOD_VECTOR *vel, FMOD_VECTOR *forward, FMOD_VECTOR *up)
{
    return FMOD_System_Get3DListenerAttributes((FMOD_SYSTEM *)this, listener, pos, vel, forward, up);
}

FMOD_RESULT System::set3DListenerAttributes(int listener, const FMOD_VECTOR *pos, const FMOD_VECTOR *vel, const FMOD_VECTOR *forward, const FMOD_VECTOR *up)
{
    return FMOD_System_Set3DListenerAttributes((FMOD_SYSTEM *)this, listener, pos, vel, forward, up);
}

FMOD_RESULT System::set3DSettings(float dopplerscale, float distancefactor, float rolloffscale)
{
    return FMOD_System_Set3DSettings((FMOD_SYSTEM *)this, dopplerscale, distancefactor, rolloffscale);
}

FMOD_RESULT System::setStreamBufferSize(unsigned int filebuffersize, FMOD_TIMEUNIT filebuffersizetype)
{
    return FMOD_System_SetStreamBufferSize((FMOD_SYSTEM *)this, filebuffersize, filebuffersizetype);
}

FMOD_RESULT System::createSound(const char *name_or_data, FMOD_MODE mode, FMOD_CREATESOUNDEXINFO *exinfo, Sound **sound)
{
    return FMOD_System_CreateSound((FMOD_SYSTEM *)this, name_or_data, mode, exinfo, (FMOD_SOUND **)sound);
}

FMOD_RESULT System::createStream(const char *name_or_data, FMOD_MODE mode, FMOD_CREATESOUNDEXINFO *exinfo, Sound **sound)
{
    return FMOD_System_CreateStream((FMOD_SYSTEM *)this, name_or_data, mode, exinfo, (FMOD_SOUND **)sound);
}

FMOD_RESULT System::createDSPByType(FMOD_DSP_TYPE type, DSP **dsp)
{
    return FMOD_System_CreateDSPByType((FMOD_SYSTEM *)this, type, (FMOD_DSP **)dsp);
}

FMOD_RESULT System::createChannelGroup(const char *name, ChannelGroup **channelgroup)
{
    return FMOD_System_CreateChannelGroup((FMOD_SYSTEM *)this, name, (FMOD_CHANNELGROUP **)channelgroup);
}

FMOD_RESULT System::playSound(Sound *sound, ChannelGroup *channelgroup, bool paused, Channel **channel)
{
    return FMOD_System_PlaySound((FMOD_SYSTEM *)this, (FMOD_SOUND *)sound, (FMOD_CHANNELGROUP *)channelgroup, paused, (FMOD_CHANNEL **)channel);
}

// ==================== Sound ====================

FMOD_RESULT Sound::getMode(FMOD_MODE *mode)
{
    return FMOD_Sound_GetMode((FMOD_SOUND *)this, mode);
}

FMOD_RESULT Sound::setMode(FMOD_MODE mode)
{
    return FMOD_Sound_SetMode((FMOD_SOUND *)this, mode);
}

FMOD_RESULT Sound::release()
{
    return FMOD_Sound_Release((FMOD_SOUND *)this);
}

// ==================== Channel ====================

FMOD_RESULT Channel::getPosition(unsigned int *position, FMOD_TIMEUNIT postype)
{
    return FMOD_Channel_GetPosition((FMOD_CHANNEL *)this, position, postype);
}

FMOD_RESULT Channel::setChannelGroup(ChannelGroup *channelgroup)
{
    return FMOD_Channel_SetChannelGroup((FMOD_CHANNEL *)this, (FMOD_CHANNELGROUP *)channelgroup);
}

// ==================== ChannelControl ====================
// All ChannelControl C++ methods forward to FMOD_Channel_* C API.

FMOD_RESULT ChannelControl::stop()
{
    return FMOD_Channel_Stop((FMOD_CHANNEL *)this);
}

FMOD_RESULT ChannelControl::setVolume(float volume)
{
    return FMOD_Channel_SetVolume((FMOD_CHANNEL *)this, volume);
}

FMOD_RESULT ChannelControl::setVolumeRamp(bool ramp)
{
    return FMOD_Channel_SetVolumeRamp((FMOD_CHANNEL *)this, ramp ? 1 : 0);
}

FMOD_RESULT ChannelControl::setPitch(float pitch)
{
    return FMOD_Channel_SetPitch((FMOD_CHANNEL *)this, pitch);
}

FMOD_RESULT ChannelControl::setMute(bool mute)
{
    return FMOD_Channel_SetMute((FMOD_CHANNEL *)this, mute ? 1 : 0);
}

FMOD_RESULT ChannelControl::setPaused(bool paused)
{
    return FMOD_Channel_SetPaused((FMOD_CHANNEL *)this, paused ? 1 : 0);
}

FMOD_RESULT ChannelControl::getMute(bool *mute)
{
    FMOD_BOOL m = 0;
    FMOD_RESULT r = FMOD_Channel_GetMute((FMOD_CHANNEL *)this, &m);
    *mute = m != 0;
    return r;
}

FMOD_RESULT ChannelControl::isPlaying(bool *isplaying)
{
    FMOD_BOOL p = 0;
    FMOD_RESULT r = FMOD_Channel_IsPlaying((FMOD_CHANNEL *)this, &p);
    *isplaying = p != 0;
    return r;
}

FMOD_RESULT ChannelControl::getMode(unsigned int *mode)
{
    return FMOD_Channel_GetMode((FMOD_CHANNEL *)this, (FMOD_MODE *)mode);
}

FMOD_RESULT ChannelControl::getDSPClock(unsigned long long *dspclock, unsigned long long *parentclock)
{
    return FMOD_Channel_GetDSPClock((FMOD_CHANNEL *)this, dspclock, parentclock);
}

FMOD_RESULT ChannelControl::addFadePoint(unsigned long long dspclock, float volume)
{
    return FMOD_Channel_AddFadePoint((FMOD_CHANNEL *)this, dspclock, volume);
}

FMOD_RESULT ChannelControl::get3DAttributes(FMOD_VECTOR *pos, FMOD_VECTOR *vel)
{
    return FMOD_Channel_Get3DAttributes((FMOD_CHANNEL *)this, pos, vel);
}

FMOD_RESULT ChannelControl::set3DAttributes(const FMOD_VECTOR *pos, const FMOD_VECTOR *vel)
{
    return FMOD_Channel_Set3DAttributes((FMOD_CHANNEL *)this, pos, vel);
}

FMOD_RESULT ChannelControl::get3DMinMaxDistance(float *min, float *max)
{
    return FMOD_Channel_Get3DMinMaxDistance((FMOD_CHANNEL *)this, min, max);
}

FMOD_RESULT ChannelControl::set3DMinMaxDistance(float min, float max)
{
    return FMOD_Channel_Set3DMinMaxDistance((FMOD_CHANNEL *)this, min, max);
}

FMOD_RESULT ChannelControl::getNumDSPs(int *numdsps)
{
    return FMOD_Channel_GetNumDSPs((FMOD_CHANNEL *)this, numdsps);
}

FMOD_RESULT ChannelControl::addDSP(int index, DSP *dsp)
{
    return FMOD_Channel_AddDSP((FMOD_CHANNEL *)this, index, (FMOD_DSP *)dsp);
}

FMOD_RESULT ChannelControl::getDSP(int index, DSP **dsp)
{
    return FMOD_Channel_GetDSP((FMOD_CHANNEL *)this, index, (FMOD_DSP **)dsp);
}

FMOD_RESULT ChannelControl::removeDSP(DSP *dsp)
{
    return FMOD_Channel_RemoveDSP((FMOD_CHANNEL *)this, (FMOD_DSP *)dsp);
}

// ==================== ChannelGroup ====================

FMOD_RESULT ChannelGroup::getNumChannels(int *numchannels)
{
    return FMOD_ChannelGroup_GetNumChannels((FMOD_CHANNELGROUP *)this, numchannels);
}

FMOD_RESULT ChannelGroup::getName(char *name, int namelen)
{
    return FMOD_ChannelGroup_GetName((FMOD_CHANNELGROUP *)this, name, namelen);
}

FMOD_RESULT ChannelGroup::release()
{
    return FMOD_ChannelGroup_Release((FMOD_CHANNELGROUP *)this);
}

FMOD_RESULT ChannelGroup::addGroup(ChannelGroup *group, bool propagatedspclock, DSPConnection **connection)
{
    return FMOD_ChannelGroup_AddGroup((FMOD_CHANNELGROUP *)this, (FMOD_CHANNELGROUP *)group, propagatedspclock ? 1 : 0, (FMOD_DSPCONNECTION **)connection);
}

// ==================== DSP ====================

FMOD_RESULT DSP::getType(FMOD_DSP_TYPE *type)
{
    return FMOD_DSP_GetType((FMOD_DSP *)this, type);
}

FMOD_RESULT DSP::release()
{
    return FMOD_DSP_Release((FMOD_DSP *)this);
}

FMOD_RESULT DSP::setActive(bool active)
{
    return FMOD_DSP_SetActive((FMOD_DSP *)this, active ? 1 : 0);
}
