#include <windows.h>
#define DLL_EXPORTS
#include "fmod.h"

// Auto-generated x86 shim: forwards decorated stdcall C API to the custom fmod.dll exports.
extern "C" {
static HMODULE g_hFmod = NULL;
static void EnsureFmod() { if (!g_hFmod) g_hFmod = LoadLibraryA("fmod.dll"); }

typedef FMOD_RESULT (F_CALL *PFN_FMOD_System_Create)(FMOD_SYSTEM **system, unsigned int headerversion);
static PFN_FMOD_System_Create g_pFMOD_System_Create = NULL;
static void InitFMOD_System_Create() { if (!g_pFMOD_System_Create) { EnsureFmod(); g_pFMOD_System_Create = (PFN_FMOD_System_Create)GetProcAddress(g_hFmod, "FMOD_System_Create"); } }
__declspec(dllexport) FMOD_RESULT F_API FMOD_System_Create(FMOD_SYSTEM **system, unsigned int headerversion)
{
    if (!g_pFMOD_System_Create) InitFMOD_System_Create();
    return g_pFMOD_System_Create(system, headerversion);
}

typedef FMOD_RESULT (F_CALL *PFN_FMOD_System_Init)(FMOD_SYSTEM *system, int maxchannels, FMOD_INITFLAGS flags, void *extradriverdata);
static PFN_FMOD_System_Init g_pFMOD_System_Init = NULL;
static void InitFMOD_System_Init() { if (!g_pFMOD_System_Init) { EnsureFmod(); g_pFMOD_System_Init = (PFN_FMOD_System_Init)GetProcAddress(g_hFmod, "FMOD_System_Init"); } }
__declspec(dllexport) FMOD_RESULT F_API FMOD_System_Init(FMOD_SYSTEM *system, int maxchannels, FMOD_INITFLAGS flags, void *extradriverdata)
{
    if (!g_pFMOD_System_Init) InitFMOD_System_Init();
    return g_pFMOD_System_Init(system, maxchannels, flags, extradriverdata);
}

typedef FMOD_RESULT (F_CALL *PFN_FMOD_System_Close)(FMOD_SYSTEM *system);
static PFN_FMOD_System_Close g_pFMOD_System_Close = NULL;
static void InitFMOD_System_Close() { if (!g_pFMOD_System_Close) { EnsureFmod(); g_pFMOD_System_Close = (PFN_FMOD_System_Close)GetProcAddress(g_hFmod, "FMOD_System_Close"); } }
__declspec(dllexport) FMOD_RESULT F_API FMOD_System_Close(FMOD_SYSTEM *system)
{
    if (!g_pFMOD_System_Close) InitFMOD_System_Close();
    return g_pFMOD_System_Close(system);
}

typedef FMOD_RESULT (F_CALL *PFN_FMOD_System_Update)(FMOD_SYSTEM *system);
static PFN_FMOD_System_Update g_pFMOD_System_Update = NULL;
static void InitFMOD_System_Update() { if (!g_pFMOD_System_Update) { EnsureFmod(); g_pFMOD_System_Update = (PFN_FMOD_System_Update)GetProcAddress(g_hFmod, "FMOD_System_Update"); } }
__declspec(dllexport) FMOD_RESULT F_API FMOD_System_Update(FMOD_SYSTEM *system)
{
    if (!g_pFMOD_System_Update) InitFMOD_System_Update();
    return g_pFMOD_System_Update(system);
}

typedef FMOD_RESULT (F_CALL *PFN_FMOD_System_Release)(FMOD_SYSTEM *system);
static PFN_FMOD_System_Release g_pFMOD_System_Release = NULL;
static void InitFMOD_System_Release() { if (!g_pFMOD_System_Release) { EnsureFmod(); g_pFMOD_System_Release = (PFN_FMOD_System_Release)GetProcAddress(g_hFmod, "FMOD_System_Release"); } }
__declspec(dllexport) FMOD_RESULT F_API FMOD_System_Release(FMOD_SYSTEM *system)
{
    if (!g_pFMOD_System_Release) InitFMOD_System_Release();
    return g_pFMOD_System_Release(system);
}

typedef FMOD_RESULT (F_CALL *PFN_FMOD_System_GetVersion)(FMOD_SYSTEM *system, unsigned int *version);
static PFN_FMOD_System_GetVersion g_pFMOD_System_GetVersion = NULL;
static void InitFMOD_System_GetVersion() { if (!g_pFMOD_System_GetVersion) { EnsureFmod(); g_pFMOD_System_GetVersion = (PFN_FMOD_System_GetVersion)GetProcAddress(g_hFmod, "FMOD_System_GetVersion"); } }
__declspec(dllexport) FMOD_RESULT F_API FMOD_System_GetVersion(FMOD_SYSTEM *system, unsigned int *version)
{
    if (!g_pFMOD_System_GetVersion) InitFMOD_System_GetVersion();
    return g_pFMOD_System_GetVersion(system, version);
}

typedef FMOD_RESULT (F_CALL *PFN_FMOD_System_GetChannelsPlaying)(FMOD_SYSTEM *system, int *channels, int *realchannels);
static PFN_FMOD_System_GetChannelsPlaying g_pFMOD_System_GetChannelsPlaying = NULL;
static void InitFMOD_System_GetChannelsPlaying() { if (!g_pFMOD_System_GetChannelsPlaying) { EnsureFmod(); g_pFMOD_System_GetChannelsPlaying = (PFN_FMOD_System_GetChannelsPlaying)GetProcAddress(g_hFmod, "FMOD_System_GetChannelsPlaying"); } }
__declspec(dllexport) FMOD_RESULT F_API FMOD_System_GetChannelsPlaying(FMOD_SYSTEM *system, int *channels, int *realchannels)
{
    if (!g_pFMOD_System_GetChannelsPlaying) InitFMOD_System_GetChannelsPlaying();
    return g_pFMOD_System_GetChannelsPlaying(system, channels, realchannels);
}

typedef FMOD_RESULT (F_CALL *PFN_FMOD_System_GetMasterChannelGroup)(FMOD_SYSTEM *system, FMOD_CHANNELGROUP **channelgroup);
static PFN_FMOD_System_GetMasterChannelGroup g_pFMOD_System_GetMasterChannelGroup = NULL;
static void InitFMOD_System_GetMasterChannelGroup() { if (!g_pFMOD_System_GetMasterChannelGroup) { EnsureFmod(); g_pFMOD_System_GetMasterChannelGroup = (PFN_FMOD_System_GetMasterChannelGroup)GetProcAddress(g_hFmod, "FMOD_System_GetMasterChannelGroup"); } }
__declspec(dllexport) FMOD_RESULT F_API FMOD_System_GetMasterChannelGroup(FMOD_SYSTEM *system, FMOD_CHANNELGROUP **channelgroup)
{
    if (!g_pFMOD_System_GetMasterChannelGroup) InitFMOD_System_GetMasterChannelGroup();
    return g_pFMOD_System_GetMasterChannelGroup(system, channelgroup);
}

typedef FMOD_RESULT (F_CALL *PFN_FMOD_System_GetSoftwareFormat)(FMOD_SYSTEM *system, int *samplerate, FMOD_SPEAKERMODE *speakermode, int *numrawspeakers);
static PFN_FMOD_System_GetSoftwareFormat g_pFMOD_System_GetSoftwareFormat = NULL;
static void InitFMOD_System_GetSoftwareFormat() { if (!g_pFMOD_System_GetSoftwareFormat) { EnsureFmod(); g_pFMOD_System_GetSoftwareFormat = (PFN_FMOD_System_GetSoftwareFormat)GetProcAddress(g_hFmod, "FMOD_System_GetSoftwareFormat"); } }
__declspec(dllexport) FMOD_RESULT F_API FMOD_System_GetSoftwareFormat(FMOD_SYSTEM *system, int *samplerate, FMOD_SPEAKERMODE *speakermode, int *numrawspeakers)
{
    if (!g_pFMOD_System_GetSoftwareFormat) InitFMOD_System_GetSoftwareFormat();
    return g_pFMOD_System_GetSoftwareFormat(system, samplerate, speakermode, numrawspeakers);
}

typedef FMOD_RESULT (F_CALL *PFN_FMOD_System_Get3DListenerAttributes)(FMOD_SYSTEM *system, int listener, FMOD_VECTOR *pos, FMOD_VECTOR *vel, FMOD_VECTOR *forward, FMOD_VECTOR *up);
static PFN_FMOD_System_Get3DListenerAttributes g_pFMOD_System_Get3DListenerAttributes = NULL;
static void InitFMOD_System_Get3DListenerAttributes() { if (!g_pFMOD_System_Get3DListenerAttributes) { EnsureFmod(); g_pFMOD_System_Get3DListenerAttributes = (PFN_FMOD_System_Get3DListenerAttributes)GetProcAddress(g_hFmod, "FMOD_System_Get3DListenerAttributes"); } }
__declspec(dllexport) FMOD_RESULT F_API FMOD_System_Get3DListenerAttributes(FMOD_SYSTEM *system, int listener, FMOD_VECTOR *pos, FMOD_VECTOR *vel, FMOD_VECTOR *forward, FMOD_VECTOR *up)
{
    if (!g_pFMOD_System_Get3DListenerAttributes) InitFMOD_System_Get3DListenerAttributes();
    return g_pFMOD_System_Get3DListenerAttributes(system, listener, pos, vel, forward, up);
}

typedef FMOD_RESULT (F_CALL *PFN_FMOD_System_Set3DListenerAttributes)(FMOD_SYSTEM *system, int listener, const FMOD_VECTOR *pos, const FMOD_VECTOR *vel, const FMOD_VECTOR *forward, const FMOD_VECTOR *up);
static PFN_FMOD_System_Set3DListenerAttributes g_pFMOD_System_Set3DListenerAttributes = NULL;
static void InitFMOD_System_Set3DListenerAttributes() { if (!g_pFMOD_System_Set3DListenerAttributes) { EnsureFmod(); g_pFMOD_System_Set3DListenerAttributes = (PFN_FMOD_System_Set3DListenerAttributes)GetProcAddress(g_hFmod, "FMOD_System_Set3DListenerAttributes"); } }
__declspec(dllexport) FMOD_RESULT F_API FMOD_System_Set3DListenerAttributes(FMOD_SYSTEM *system, int listener, const FMOD_VECTOR *pos, const FMOD_VECTOR *vel, const FMOD_VECTOR *forward, const FMOD_VECTOR *up)
{
    if (!g_pFMOD_System_Set3DListenerAttributes) InitFMOD_System_Set3DListenerAttributes();
    return g_pFMOD_System_Set3DListenerAttributes(system, listener, pos, vel, forward, up);
}

typedef FMOD_RESULT (F_CALL *PFN_FMOD_System_Set3DSettings)(FMOD_SYSTEM *system, float dopplerscale, float distancefactor, float rolloffscale);
static PFN_FMOD_System_Set3DSettings g_pFMOD_System_Set3DSettings = NULL;
static void InitFMOD_System_Set3DSettings() { if (!g_pFMOD_System_Set3DSettings) { EnsureFmod(); g_pFMOD_System_Set3DSettings = (PFN_FMOD_System_Set3DSettings)GetProcAddress(g_hFmod, "FMOD_System_Set3DSettings"); } }
__declspec(dllexport) FMOD_RESULT F_API FMOD_System_Set3DSettings(FMOD_SYSTEM *system, float dopplerscale, float distancefactor, float rolloffscale)
{
    if (!g_pFMOD_System_Set3DSettings) InitFMOD_System_Set3DSettings();
    return g_pFMOD_System_Set3DSettings(system, dopplerscale, distancefactor, rolloffscale);
}

typedef FMOD_RESULT (F_CALL *PFN_FMOD_System_SetStreamBufferSize)(FMOD_SYSTEM *system, unsigned int filebuffersize, FMOD_TIMEUNIT filebuffersizetype);
static PFN_FMOD_System_SetStreamBufferSize g_pFMOD_System_SetStreamBufferSize = NULL;
static void InitFMOD_System_SetStreamBufferSize() { if (!g_pFMOD_System_SetStreamBufferSize) { EnsureFmod(); g_pFMOD_System_SetStreamBufferSize = (PFN_FMOD_System_SetStreamBufferSize)GetProcAddress(g_hFmod, "FMOD_System_SetStreamBufferSize"); } }
__declspec(dllexport) FMOD_RESULT F_API FMOD_System_SetStreamBufferSize(FMOD_SYSTEM *system, unsigned int filebuffersize, FMOD_TIMEUNIT filebuffersizetype)
{
    if (!g_pFMOD_System_SetStreamBufferSize) InitFMOD_System_SetStreamBufferSize();
    return g_pFMOD_System_SetStreamBufferSize(system, filebuffersize, filebuffersizetype);
}

typedef FMOD_RESULT (F_CALL *PFN_FMOD_System_CreateSound)(FMOD_SYSTEM *system, const char *name_or_data, FMOD_MODE mode, FMOD_CREATESOUNDEXINFO *exinfo, FMOD_SOUND **sound);
static PFN_FMOD_System_CreateSound g_pFMOD_System_CreateSound = NULL;
static void InitFMOD_System_CreateSound() { if (!g_pFMOD_System_CreateSound) { EnsureFmod(); g_pFMOD_System_CreateSound = (PFN_FMOD_System_CreateSound)GetProcAddress(g_hFmod, "FMOD_System_CreateSound"); } }
__declspec(dllexport) FMOD_RESULT F_API FMOD_System_CreateSound(FMOD_SYSTEM *system, const char *name_or_data, FMOD_MODE mode, FMOD_CREATESOUNDEXINFO *exinfo, FMOD_SOUND **sound)
{
    if (!g_pFMOD_System_CreateSound) InitFMOD_System_CreateSound();
    return g_pFMOD_System_CreateSound(system, name_or_data, mode, exinfo, sound);
}

typedef FMOD_RESULT (F_CALL *PFN_FMOD_System_CreateStream)(FMOD_SYSTEM *system, const char *name_or_data, FMOD_MODE mode, FMOD_CREATESOUNDEXINFO *exinfo, FMOD_SOUND **sound);
static PFN_FMOD_System_CreateStream g_pFMOD_System_CreateStream = NULL;
static void InitFMOD_System_CreateStream() { if (!g_pFMOD_System_CreateStream) { EnsureFmod(); g_pFMOD_System_CreateStream = (PFN_FMOD_System_CreateStream)GetProcAddress(g_hFmod, "FMOD_System_CreateStream"); } }
__declspec(dllexport) FMOD_RESULT F_API FMOD_System_CreateStream(FMOD_SYSTEM *system, const char *name_or_data, FMOD_MODE mode, FMOD_CREATESOUNDEXINFO *exinfo, FMOD_SOUND **sound)
{
    if (!g_pFMOD_System_CreateStream) InitFMOD_System_CreateStream();
    return g_pFMOD_System_CreateStream(system, name_or_data, mode, exinfo, sound);
}

typedef FMOD_RESULT (F_CALL *PFN_FMOD_System_CreateDSPByType)(FMOD_SYSTEM *system, FMOD_DSP_TYPE type, FMOD_DSP **dsp);
static PFN_FMOD_System_CreateDSPByType g_pFMOD_System_CreateDSPByType = NULL;
static void InitFMOD_System_CreateDSPByType() { if (!g_pFMOD_System_CreateDSPByType) { EnsureFmod(); g_pFMOD_System_CreateDSPByType = (PFN_FMOD_System_CreateDSPByType)GetProcAddress(g_hFmod, "FMOD_System_CreateDSPByType"); } }
__declspec(dllexport) FMOD_RESULT F_API FMOD_System_CreateDSPByType(FMOD_SYSTEM *system, FMOD_DSP_TYPE type, FMOD_DSP **dsp)
{
    if (!g_pFMOD_System_CreateDSPByType) InitFMOD_System_CreateDSPByType();
    return g_pFMOD_System_CreateDSPByType(system, type, dsp);
}

typedef FMOD_RESULT (F_CALL *PFN_FMOD_System_CreateChannelGroup)(FMOD_SYSTEM *system, const char *name, FMOD_CHANNELGROUP **channelgroup);
static PFN_FMOD_System_CreateChannelGroup g_pFMOD_System_CreateChannelGroup = NULL;
static void InitFMOD_System_CreateChannelGroup() { if (!g_pFMOD_System_CreateChannelGroup) { EnsureFmod(); g_pFMOD_System_CreateChannelGroup = (PFN_FMOD_System_CreateChannelGroup)GetProcAddress(g_hFmod, "FMOD_System_CreateChannelGroup"); } }
__declspec(dllexport) FMOD_RESULT F_API FMOD_System_CreateChannelGroup(FMOD_SYSTEM *system, const char *name, FMOD_CHANNELGROUP **channelgroup)
{
    if (!g_pFMOD_System_CreateChannelGroup) InitFMOD_System_CreateChannelGroup();
    return g_pFMOD_System_CreateChannelGroup(system, name, channelgroup);
}

typedef FMOD_RESULT (F_CALL *PFN_FMOD_System_PlaySound)(FMOD_SYSTEM *system, FMOD_SOUND *sound, FMOD_CHANNELGROUP *channelgroup, FMOD_BOOL paused, FMOD_CHANNEL **channel);
static PFN_FMOD_System_PlaySound g_pFMOD_System_PlaySound = NULL;
static void InitFMOD_System_PlaySound() { if (!g_pFMOD_System_PlaySound) { EnsureFmod(); g_pFMOD_System_PlaySound = (PFN_FMOD_System_PlaySound)GetProcAddress(g_hFmod, "FMOD_System_PlaySound"); } }
__declspec(dllexport) FMOD_RESULT F_API FMOD_System_PlaySound(FMOD_SYSTEM *system, FMOD_SOUND *sound, FMOD_CHANNELGROUP *channelgroup, FMOD_BOOL paused, FMOD_CHANNEL **channel)
{
    if (!g_pFMOD_System_PlaySound) InitFMOD_System_PlaySound();
    return g_pFMOD_System_PlaySound(system, sound, channelgroup, paused, channel);
}

typedef FMOD_RESULT (F_CALL *PFN_FMOD_Sound_GetMode)(FMOD_SOUND *sound, FMOD_MODE *mode);
static PFN_FMOD_Sound_GetMode g_pFMOD_Sound_GetMode = NULL;
static void InitFMOD_Sound_GetMode() { if (!g_pFMOD_Sound_GetMode) { EnsureFmod(); g_pFMOD_Sound_GetMode = (PFN_FMOD_Sound_GetMode)GetProcAddress(g_hFmod, "FMOD_Sound_GetMode"); } }
__declspec(dllexport) FMOD_RESULT F_API FMOD_Sound_GetMode(FMOD_SOUND *sound, FMOD_MODE *mode)
{
    if (!g_pFMOD_Sound_GetMode) InitFMOD_Sound_GetMode();
    return g_pFMOD_Sound_GetMode(sound, mode);
}

typedef FMOD_RESULT (F_CALL *PFN_FMOD_Sound_SetMode)(FMOD_SOUND *sound, FMOD_MODE mode);
static PFN_FMOD_Sound_SetMode g_pFMOD_Sound_SetMode = NULL;
static void InitFMOD_Sound_SetMode() { if (!g_pFMOD_Sound_SetMode) { EnsureFmod(); g_pFMOD_Sound_SetMode = (PFN_FMOD_Sound_SetMode)GetProcAddress(g_hFmod, "FMOD_Sound_SetMode"); } }
__declspec(dllexport) FMOD_RESULT F_API FMOD_Sound_SetMode(FMOD_SOUND *sound, FMOD_MODE mode)
{
    if (!g_pFMOD_Sound_SetMode) InitFMOD_Sound_SetMode();
    return g_pFMOD_Sound_SetMode(sound, mode);
}

typedef FMOD_RESULT (F_CALL *PFN_FMOD_Sound_Release)(FMOD_SOUND *sound);
static PFN_FMOD_Sound_Release g_pFMOD_Sound_Release = NULL;
static void InitFMOD_Sound_Release() { if (!g_pFMOD_Sound_Release) { EnsureFmod(); g_pFMOD_Sound_Release = (PFN_FMOD_Sound_Release)GetProcAddress(g_hFmod, "FMOD_Sound_Release"); } }
__declspec(dllexport) FMOD_RESULT F_API FMOD_Sound_Release(FMOD_SOUND *sound)
{
    if (!g_pFMOD_Sound_Release) InitFMOD_Sound_Release();
    return g_pFMOD_Sound_Release(sound);
}

typedef FMOD_RESULT (F_CALL *PFN_FMOD_Channel_GetPosition)(FMOD_CHANNEL *channel, unsigned int *position, FMOD_TIMEUNIT postype);
static PFN_FMOD_Channel_GetPosition g_pFMOD_Channel_GetPosition = NULL;
static void InitFMOD_Channel_GetPosition() { if (!g_pFMOD_Channel_GetPosition) { EnsureFmod(); g_pFMOD_Channel_GetPosition = (PFN_FMOD_Channel_GetPosition)GetProcAddress(g_hFmod, "FMOD_Channel_GetPosition"); } }
__declspec(dllexport) FMOD_RESULT F_API FMOD_Channel_GetPosition(FMOD_CHANNEL *channel, unsigned int *position, FMOD_TIMEUNIT postype)
{
    if (!g_pFMOD_Channel_GetPosition) InitFMOD_Channel_GetPosition();
    return g_pFMOD_Channel_GetPosition(channel, position, postype);
}

typedef FMOD_RESULT (F_CALL *PFN_FMOD_Channel_SetChannelGroup)(FMOD_CHANNEL *channel, FMOD_CHANNELGROUP *channelgroup);
static PFN_FMOD_Channel_SetChannelGroup g_pFMOD_Channel_SetChannelGroup = NULL;
static void InitFMOD_Channel_SetChannelGroup() { if (!g_pFMOD_Channel_SetChannelGroup) { EnsureFmod(); g_pFMOD_Channel_SetChannelGroup = (PFN_FMOD_Channel_SetChannelGroup)GetProcAddress(g_hFmod, "FMOD_Channel_SetChannelGroup"); } }
__declspec(dllexport) FMOD_RESULT F_API FMOD_Channel_SetChannelGroup(FMOD_CHANNEL *channel, FMOD_CHANNELGROUP *channelgroup)
{
    if (!g_pFMOD_Channel_SetChannelGroup) InitFMOD_Channel_SetChannelGroup();
    return g_pFMOD_Channel_SetChannelGroup(channel, channelgroup);
}

typedef FMOD_RESULT (F_CALL *PFN_FMOD_Channel_Stop)(FMOD_CHANNEL *channel);
static PFN_FMOD_Channel_Stop g_pFMOD_Channel_Stop = NULL;
static void InitFMOD_Channel_Stop() { if (!g_pFMOD_Channel_Stop) { EnsureFmod(); g_pFMOD_Channel_Stop = (PFN_FMOD_Channel_Stop)GetProcAddress(g_hFmod, "FMOD_Channel_Stop"); } }
__declspec(dllexport) FMOD_RESULT F_API FMOD_Channel_Stop(FMOD_CHANNEL *channel)
{
    if (!g_pFMOD_Channel_Stop) InitFMOD_Channel_Stop();
    return g_pFMOD_Channel_Stop(channel);
}

typedef FMOD_RESULT (F_CALL *PFN_FMOD_Channel_SetVolume)(FMOD_CHANNEL *channel, float volume);
static PFN_FMOD_Channel_SetVolume g_pFMOD_Channel_SetVolume = NULL;
static void InitFMOD_Channel_SetVolume() { if (!g_pFMOD_Channel_SetVolume) { EnsureFmod(); g_pFMOD_Channel_SetVolume = (PFN_FMOD_Channel_SetVolume)GetProcAddress(g_hFmod, "FMOD_Channel_SetVolume"); } }
__declspec(dllexport) FMOD_RESULT F_API FMOD_Channel_SetVolume(FMOD_CHANNEL *channel, float volume)
{
    if (!g_pFMOD_Channel_SetVolume) InitFMOD_Channel_SetVolume();
    return g_pFMOD_Channel_SetVolume(channel, volume);
}

typedef FMOD_RESULT (F_CALL *PFN_FMOD_Channel_SetVolumeRamp)(FMOD_CHANNEL *channel, FMOD_BOOL ramp);
static PFN_FMOD_Channel_SetVolumeRamp g_pFMOD_Channel_SetVolumeRamp = NULL;
static void InitFMOD_Channel_SetVolumeRamp() { if (!g_pFMOD_Channel_SetVolumeRamp) { EnsureFmod(); g_pFMOD_Channel_SetVolumeRamp = (PFN_FMOD_Channel_SetVolumeRamp)GetProcAddress(g_hFmod, "FMOD_Channel_SetVolumeRamp"); } }
__declspec(dllexport) FMOD_RESULT F_API FMOD_Channel_SetVolumeRamp(FMOD_CHANNEL *channel, FMOD_BOOL ramp)
{
    if (!g_pFMOD_Channel_SetVolumeRamp) InitFMOD_Channel_SetVolumeRamp();
    return g_pFMOD_Channel_SetVolumeRamp(channel, ramp);
}

typedef FMOD_RESULT (F_CALL *PFN_FMOD_Channel_SetPitch)(FMOD_CHANNEL *channel, float pitch);
static PFN_FMOD_Channel_SetPitch g_pFMOD_Channel_SetPitch = NULL;
static void InitFMOD_Channel_SetPitch() { if (!g_pFMOD_Channel_SetPitch) { EnsureFmod(); g_pFMOD_Channel_SetPitch = (PFN_FMOD_Channel_SetPitch)GetProcAddress(g_hFmod, "FMOD_Channel_SetPitch"); } }
__declspec(dllexport) FMOD_RESULT F_API FMOD_Channel_SetPitch(FMOD_CHANNEL *channel, float pitch)
{
    if (!g_pFMOD_Channel_SetPitch) InitFMOD_Channel_SetPitch();
    return g_pFMOD_Channel_SetPitch(channel, pitch);
}

typedef FMOD_RESULT (F_CALL *PFN_FMOD_Channel_SetMute)(FMOD_CHANNEL *channel, FMOD_BOOL mute);
static PFN_FMOD_Channel_SetMute g_pFMOD_Channel_SetMute = NULL;
static void InitFMOD_Channel_SetMute() { if (!g_pFMOD_Channel_SetMute) { EnsureFmod(); g_pFMOD_Channel_SetMute = (PFN_FMOD_Channel_SetMute)GetProcAddress(g_hFmod, "FMOD_Channel_SetMute"); } }
__declspec(dllexport) FMOD_RESULT F_API FMOD_Channel_SetMute(FMOD_CHANNEL *channel, FMOD_BOOL mute)
{
    if (!g_pFMOD_Channel_SetMute) InitFMOD_Channel_SetMute();
    return g_pFMOD_Channel_SetMute(channel, mute);
}

typedef FMOD_RESULT (F_CALL *PFN_FMOD_Channel_SetPaused)(FMOD_CHANNEL *channel, FMOD_BOOL paused);
static PFN_FMOD_Channel_SetPaused g_pFMOD_Channel_SetPaused = NULL;
static void InitFMOD_Channel_SetPaused() { if (!g_pFMOD_Channel_SetPaused) { EnsureFmod(); g_pFMOD_Channel_SetPaused = (PFN_FMOD_Channel_SetPaused)GetProcAddress(g_hFmod, "FMOD_Channel_SetPaused"); } }
__declspec(dllexport) FMOD_RESULT F_API FMOD_Channel_SetPaused(FMOD_CHANNEL *channel, FMOD_BOOL paused)
{
    if (!g_pFMOD_Channel_SetPaused) InitFMOD_Channel_SetPaused();
    return g_pFMOD_Channel_SetPaused(channel, paused);
}

typedef FMOD_RESULT (F_CALL *PFN_FMOD_Channel_GetMute)(FMOD_CHANNEL *channel, FMOD_BOOL *mute);
static PFN_FMOD_Channel_GetMute g_pFMOD_Channel_GetMute = NULL;
static void InitFMOD_Channel_GetMute() { if (!g_pFMOD_Channel_GetMute) { EnsureFmod(); g_pFMOD_Channel_GetMute = (PFN_FMOD_Channel_GetMute)GetProcAddress(g_hFmod, "FMOD_Channel_GetMute"); } }
__declspec(dllexport) FMOD_RESULT F_API FMOD_Channel_GetMute(FMOD_CHANNEL *channel, FMOD_BOOL *mute)
{
    if (!g_pFMOD_Channel_GetMute) InitFMOD_Channel_GetMute();
    return g_pFMOD_Channel_GetMute(channel, mute);
}

typedef FMOD_RESULT (F_CALL *PFN_FMOD_Channel_IsPlaying)(FMOD_CHANNEL *channel, FMOD_BOOL *isplaying);
static PFN_FMOD_Channel_IsPlaying g_pFMOD_Channel_IsPlaying = NULL;
static void InitFMOD_Channel_IsPlaying() { if (!g_pFMOD_Channel_IsPlaying) { EnsureFmod(); g_pFMOD_Channel_IsPlaying = (PFN_FMOD_Channel_IsPlaying)GetProcAddress(g_hFmod, "FMOD_Channel_IsPlaying"); } }
__declspec(dllexport) FMOD_RESULT F_API FMOD_Channel_IsPlaying(FMOD_CHANNEL *channel, FMOD_BOOL *isplaying)
{
    if (!g_pFMOD_Channel_IsPlaying) InitFMOD_Channel_IsPlaying();
    return g_pFMOD_Channel_IsPlaying(channel, isplaying);
}

typedef FMOD_RESULT (F_CALL *PFN_FMOD_Channel_GetMode)(FMOD_CHANNEL *channel, FMOD_MODE *mode);
static PFN_FMOD_Channel_GetMode g_pFMOD_Channel_GetMode = NULL;
static void InitFMOD_Channel_GetMode() { if (!g_pFMOD_Channel_GetMode) { EnsureFmod(); g_pFMOD_Channel_GetMode = (PFN_FMOD_Channel_GetMode)GetProcAddress(g_hFmod, "FMOD_Channel_GetMode"); } }
__declspec(dllexport) FMOD_RESULT F_API FMOD_Channel_GetMode(FMOD_CHANNEL *channel, FMOD_MODE *mode)
{
    if (!g_pFMOD_Channel_GetMode) InitFMOD_Channel_GetMode();
    return g_pFMOD_Channel_GetMode(channel, mode);
}

typedef FMOD_RESULT (F_CALL *PFN_FMOD_Channel_GetDSPClock)(FMOD_CHANNEL *channel, unsigned long long *dspclock, unsigned long long *parentclock);
static PFN_FMOD_Channel_GetDSPClock g_pFMOD_Channel_GetDSPClock = NULL;
static void InitFMOD_Channel_GetDSPClock() { if (!g_pFMOD_Channel_GetDSPClock) { EnsureFmod(); g_pFMOD_Channel_GetDSPClock = (PFN_FMOD_Channel_GetDSPClock)GetProcAddress(g_hFmod, "FMOD_Channel_GetDSPClock"); } }
__declspec(dllexport) FMOD_RESULT F_API FMOD_Channel_GetDSPClock(FMOD_CHANNEL *channel, unsigned long long *dspclock, unsigned long long *parentclock)
{
    if (!g_pFMOD_Channel_GetDSPClock) InitFMOD_Channel_GetDSPClock();
    return g_pFMOD_Channel_GetDSPClock(channel, dspclock, parentclock);
}

typedef FMOD_RESULT (F_CALL *PFN_FMOD_Channel_AddFadePoint)(FMOD_CHANNEL *channel, unsigned long long dspclock, float volume);
static PFN_FMOD_Channel_AddFadePoint g_pFMOD_Channel_AddFadePoint = NULL;
static void InitFMOD_Channel_AddFadePoint() { if (!g_pFMOD_Channel_AddFadePoint) { EnsureFmod(); g_pFMOD_Channel_AddFadePoint = (PFN_FMOD_Channel_AddFadePoint)GetProcAddress(g_hFmod, "FMOD_Channel_AddFadePoint"); } }
__declspec(dllexport) FMOD_RESULT F_API FMOD_Channel_AddFadePoint(FMOD_CHANNEL *channel, unsigned long long dspclock, float volume)
{
    if (!g_pFMOD_Channel_AddFadePoint) InitFMOD_Channel_AddFadePoint();
    return g_pFMOD_Channel_AddFadePoint(channel, dspclock, volume);
}

typedef FMOD_RESULT (F_CALL *PFN_FMOD_Channel_Get3DAttributes)(FMOD_CHANNEL *channel, FMOD_VECTOR *pos, FMOD_VECTOR *vel);
static PFN_FMOD_Channel_Get3DAttributes g_pFMOD_Channel_Get3DAttributes = NULL;
static void InitFMOD_Channel_Get3DAttributes() { if (!g_pFMOD_Channel_Get3DAttributes) { EnsureFmod(); g_pFMOD_Channel_Get3DAttributes = (PFN_FMOD_Channel_Get3DAttributes)GetProcAddress(g_hFmod, "FMOD_Channel_Get3DAttributes"); } }
__declspec(dllexport) FMOD_RESULT F_API FMOD_Channel_Get3DAttributes(FMOD_CHANNEL *channel, FMOD_VECTOR *pos, FMOD_VECTOR *vel)
{
    if (!g_pFMOD_Channel_Get3DAttributes) InitFMOD_Channel_Get3DAttributes();
    return g_pFMOD_Channel_Get3DAttributes(channel, pos, vel);
}

typedef FMOD_RESULT (F_CALL *PFN_FMOD_Channel_Set3DAttributes)(FMOD_CHANNEL *channel, const FMOD_VECTOR *pos, const FMOD_VECTOR *vel);
static PFN_FMOD_Channel_Set3DAttributes g_pFMOD_Channel_Set3DAttributes = NULL;
static void InitFMOD_Channel_Set3DAttributes() { if (!g_pFMOD_Channel_Set3DAttributes) { EnsureFmod(); g_pFMOD_Channel_Set3DAttributes = (PFN_FMOD_Channel_Set3DAttributes)GetProcAddress(g_hFmod, "FMOD_Channel_Set3DAttributes"); } }
__declspec(dllexport) FMOD_RESULT F_API FMOD_Channel_Set3DAttributes(FMOD_CHANNEL *channel, const FMOD_VECTOR *pos, const FMOD_VECTOR *vel)
{
    if (!g_pFMOD_Channel_Set3DAttributes) InitFMOD_Channel_Set3DAttributes();
    return g_pFMOD_Channel_Set3DAttributes(channel, pos, vel);
}

typedef FMOD_RESULT (F_CALL *PFN_FMOD_Channel_Get3DMinMaxDistance)(FMOD_CHANNEL *channel, float *mindistance, float *maxdistance);
static PFN_FMOD_Channel_Get3DMinMaxDistance g_pFMOD_Channel_Get3DMinMaxDistance = NULL;
static void InitFMOD_Channel_Get3DMinMaxDistance() { if (!g_pFMOD_Channel_Get3DMinMaxDistance) { EnsureFmod(); g_pFMOD_Channel_Get3DMinMaxDistance = (PFN_FMOD_Channel_Get3DMinMaxDistance)GetProcAddress(g_hFmod, "FMOD_Channel_Get3DMinMaxDistance"); } }
__declspec(dllexport) FMOD_RESULT F_API FMOD_Channel_Get3DMinMaxDistance(FMOD_CHANNEL *channel, float *mindistance, float *maxdistance)
{
    if (!g_pFMOD_Channel_Get3DMinMaxDistance) InitFMOD_Channel_Get3DMinMaxDistance();
    return g_pFMOD_Channel_Get3DMinMaxDistance(channel, mindistance, maxdistance);
}

typedef FMOD_RESULT (F_CALL *PFN_FMOD_Channel_Set3DMinMaxDistance)(FMOD_CHANNEL *channel, float mindistance, float maxdistance);
static PFN_FMOD_Channel_Set3DMinMaxDistance g_pFMOD_Channel_Set3DMinMaxDistance = NULL;
static void InitFMOD_Channel_Set3DMinMaxDistance() { if (!g_pFMOD_Channel_Set3DMinMaxDistance) { EnsureFmod(); g_pFMOD_Channel_Set3DMinMaxDistance = (PFN_FMOD_Channel_Set3DMinMaxDistance)GetProcAddress(g_hFmod, "FMOD_Channel_Set3DMinMaxDistance"); } }
__declspec(dllexport) FMOD_RESULT F_API FMOD_Channel_Set3DMinMaxDistance(FMOD_CHANNEL *channel, float mindistance, float maxdistance)
{
    if (!g_pFMOD_Channel_Set3DMinMaxDistance) InitFMOD_Channel_Set3DMinMaxDistance();
    return g_pFMOD_Channel_Set3DMinMaxDistance(channel, mindistance, maxdistance);
}

typedef FMOD_RESULT (F_CALL *PFN_FMOD_Channel_GetNumDSPs)(FMOD_CHANNEL *channel, int *numdsps);
static PFN_FMOD_Channel_GetNumDSPs g_pFMOD_Channel_GetNumDSPs = NULL;
static void InitFMOD_Channel_GetNumDSPs() { if (!g_pFMOD_Channel_GetNumDSPs) { EnsureFmod(); g_pFMOD_Channel_GetNumDSPs = (PFN_FMOD_Channel_GetNumDSPs)GetProcAddress(g_hFmod, "FMOD_Channel_GetNumDSPs"); } }
__declspec(dllexport) FMOD_RESULT F_API FMOD_Channel_GetNumDSPs(FMOD_CHANNEL *channel, int *numdsps)
{
    if (!g_pFMOD_Channel_GetNumDSPs) InitFMOD_Channel_GetNumDSPs();
    return g_pFMOD_Channel_GetNumDSPs(channel, numdsps);
}

typedef FMOD_RESULT (F_CALL *PFN_FMOD_Channel_AddDSP)(FMOD_CHANNEL *channel, int index, FMOD_DSP *dsp);
static PFN_FMOD_Channel_AddDSP g_pFMOD_Channel_AddDSP = NULL;
static void InitFMOD_Channel_AddDSP() { if (!g_pFMOD_Channel_AddDSP) { EnsureFmod(); g_pFMOD_Channel_AddDSP = (PFN_FMOD_Channel_AddDSP)GetProcAddress(g_hFmod, "FMOD_Channel_AddDSP"); } }
__declspec(dllexport) FMOD_RESULT F_API FMOD_Channel_AddDSP(FMOD_CHANNEL *channel, int index, FMOD_DSP *dsp)
{
    if (!g_pFMOD_Channel_AddDSP) InitFMOD_Channel_AddDSP();
    return g_pFMOD_Channel_AddDSP(channel, index, dsp);
}

typedef FMOD_RESULT (F_CALL *PFN_FMOD_Channel_GetDSP)(FMOD_CHANNEL *channel, int index, FMOD_DSP **dsp);
static PFN_FMOD_Channel_GetDSP g_pFMOD_Channel_GetDSP = NULL;
static void InitFMOD_Channel_GetDSP() { if (!g_pFMOD_Channel_GetDSP) { EnsureFmod(); g_pFMOD_Channel_GetDSP = (PFN_FMOD_Channel_GetDSP)GetProcAddress(g_hFmod, "FMOD_Channel_GetDSP"); } }
__declspec(dllexport) FMOD_RESULT F_API FMOD_Channel_GetDSP(FMOD_CHANNEL *channel, int index, FMOD_DSP **dsp)
{
    if (!g_pFMOD_Channel_GetDSP) InitFMOD_Channel_GetDSP();
    return g_pFMOD_Channel_GetDSP(channel, index, dsp);
}

typedef FMOD_RESULT (F_CALL *PFN_FMOD_Channel_RemoveDSP)(FMOD_CHANNEL *channel, FMOD_DSP *dsp);
static PFN_FMOD_Channel_RemoveDSP g_pFMOD_Channel_RemoveDSP = NULL;
static void InitFMOD_Channel_RemoveDSP() { if (!g_pFMOD_Channel_RemoveDSP) { EnsureFmod(); g_pFMOD_Channel_RemoveDSP = (PFN_FMOD_Channel_RemoveDSP)GetProcAddress(g_hFmod, "FMOD_Channel_RemoveDSP"); } }
__declspec(dllexport) FMOD_RESULT F_API FMOD_Channel_RemoveDSP(FMOD_CHANNEL *channel, FMOD_DSP *dsp)
{
    if (!g_pFMOD_Channel_RemoveDSP) InitFMOD_Channel_RemoveDSP();
    return g_pFMOD_Channel_RemoveDSP(channel, dsp);
}

typedef FMOD_RESULT (F_CALL *PFN_FMOD_ChannelGroup_GetNumChannels)(FMOD_CHANNELGROUP *channelgroup, int *numchannels);
static PFN_FMOD_ChannelGroup_GetNumChannels g_pFMOD_ChannelGroup_GetNumChannels = NULL;
static void InitFMOD_ChannelGroup_GetNumChannels() { if (!g_pFMOD_ChannelGroup_GetNumChannels) { EnsureFmod(); g_pFMOD_ChannelGroup_GetNumChannels = (PFN_FMOD_ChannelGroup_GetNumChannels)GetProcAddress(g_hFmod, "FMOD_ChannelGroup_GetNumChannels"); } }
__declspec(dllexport) FMOD_RESULT F_API FMOD_ChannelGroup_GetNumChannels(FMOD_CHANNELGROUP *channelgroup, int *numchannels)
{
    if (!g_pFMOD_ChannelGroup_GetNumChannels) InitFMOD_ChannelGroup_GetNumChannels();
    return g_pFMOD_ChannelGroup_GetNumChannels(channelgroup, numchannels);
}

typedef FMOD_RESULT (F_CALL *PFN_FMOD_ChannelGroup_GetName)(FMOD_CHANNELGROUP *channelgroup, char *name, int namelen);
static PFN_FMOD_ChannelGroup_GetName g_pFMOD_ChannelGroup_GetName = NULL;
static void InitFMOD_ChannelGroup_GetName() { if (!g_pFMOD_ChannelGroup_GetName) { EnsureFmod(); g_pFMOD_ChannelGroup_GetName = (PFN_FMOD_ChannelGroup_GetName)GetProcAddress(g_hFmod, "FMOD_ChannelGroup_GetName"); } }
__declspec(dllexport) FMOD_RESULT F_API FMOD_ChannelGroup_GetName(FMOD_CHANNELGROUP *channelgroup, char *name, int namelen)
{
    if (!g_pFMOD_ChannelGroup_GetName) InitFMOD_ChannelGroup_GetName();
    return g_pFMOD_ChannelGroup_GetName(channelgroup, name, namelen);
}

typedef FMOD_RESULT (F_CALL *PFN_FMOD_ChannelGroup_Release)(FMOD_CHANNELGROUP *channelgroup);
static PFN_FMOD_ChannelGroup_Release g_pFMOD_ChannelGroup_Release = NULL;
static void InitFMOD_ChannelGroup_Release() { if (!g_pFMOD_ChannelGroup_Release) { EnsureFmod(); g_pFMOD_ChannelGroup_Release = (PFN_FMOD_ChannelGroup_Release)GetProcAddress(g_hFmod, "FMOD_ChannelGroup_Release"); } }
__declspec(dllexport) FMOD_RESULT F_API FMOD_ChannelGroup_Release(FMOD_CHANNELGROUP *channelgroup)
{
    if (!g_pFMOD_ChannelGroup_Release) InitFMOD_ChannelGroup_Release();
    return g_pFMOD_ChannelGroup_Release(channelgroup);
}

typedef FMOD_RESULT (F_CALL *PFN_FMOD_ChannelGroup_AddGroup)(FMOD_CHANNELGROUP *channelgroup, FMOD_CHANNELGROUP *group, FMOD_BOOL propagatedspclock, FMOD_DSPCONNECTION **connection);
static PFN_FMOD_ChannelGroup_AddGroup g_pFMOD_ChannelGroup_AddGroup = NULL;
static void InitFMOD_ChannelGroup_AddGroup() { if (!g_pFMOD_ChannelGroup_AddGroup) { EnsureFmod(); g_pFMOD_ChannelGroup_AddGroup = (PFN_FMOD_ChannelGroup_AddGroup)GetProcAddress(g_hFmod, "FMOD_ChannelGroup_AddGroup"); } }
__declspec(dllexport) FMOD_RESULT F_API FMOD_ChannelGroup_AddGroup(FMOD_CHANNELGROUP *channelgroup, FMOD_CHANNELGROUP *group, FMOD_BOOL propagatedspclock, FMOD_DSPCONNECTION **connection)
{
    if (!g_pFMOD_ChannelGroup_AddGroup) InitFMOD_ChannelGroup_AddGroup();
    return g_pFMOD_ChannelGroup_AddGroup(channelgroup, group, propagatedspclock, connection);
}

typedef FMOD_RESULT (F_CALL *PFN_FMOD_DSP_GetType)(FMOD_DSP *dsp, FMOD_DSP_TYPE *type);
static PFN_FMOD_DSP_GetType g_pFMOD_DSP_GetType = NULL;
static void InitFMOD_DSP_GetType() { if (!g_pFMOD_DSP_GetType) { EnsureFmod(); g_pFMOD_DSP_GetType = (PFN_FMOD_DSP_GetType)GetProcAddress(g_hFmod, "FMOD_DSP_GetType"); } }
__declspec(dllexport) FMOD_RESULT F_API FMOD_DSP_GetType(FMOD_DSP *dsp, FMOD_DSP_TYPE *type)
{
    if (!g_pFMOD_DSP_GetType) InitFMOD_DSP_GetType();
    return g_pFMOD_DSP_GetType(dsp, type);
}

typedef FMOD_RESULT (F_CALL *PFN_FMOD_DSP_Release)(FMOD_DSP *dsp);
static PFN_FMOD_DSP_Release g_pFMOD_DSP_Release = NULL;
static void InitFMOD_DSP_Release() { if (!g_pFMOD_DSP_Release) { EnsureFmod(); g_pFMOD_DSP_Release = (PFN_FMOD_DSP_Release)GetProcAddress(g_hFmod, "FMOD_DSP_Release"); } }
__declspec(dllexport) FMOD_RESULT F_API FMOD_DSP_Release(FMOD_DSP *dsp)
{
    if (!g_pFMOD_DSP_Release) InitFMOD_DSP_Release();
    return g_pFMOD_DSP_Release(dsp);
}

typedef FMOD_RESULT (F_CALL *PFN_FMOD_DSP_SetActive)(FMOD_DSP *dsp, FMOD_BOOL active);
static PFN_FMOD_DSP_SetActive g_pFMOD_DSP_SetActive = NULL;
static void InitFMOD_DSP_SetActive() { if (!g_pFMOD_DSP_SetActive) { EnsureFmod(); g_pFMOD_DSP_SetActive = (PFN_FMOD_DSP_SetActive)GetProcAddress(g_hFmod, "FMOD_DSP_SetActive"); } }
__declspec(dllexport) FMOD_RESULT F_API FMOD_DSP_SetActive(FMOD_DSP *dsp, FMOD_BOOL active)
{
    if (!g_pFMOD_DSP_SetActive) InitFMOD_DSP_SetActive();
    return g_pFMOD_DSP_SetActive(dsp, active);
}

} // extern "C"
