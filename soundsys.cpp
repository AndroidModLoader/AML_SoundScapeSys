#include "soundscape.h"

#include <miniaudio/miniaudio.h>

ma_engine soundSys;
int g_nSoundsArrayUsed = 0;
ma_sound g_SoundsArray[MAX_SOUNDSCAPES * MAX_SOUNDSCAPES_SOUNDS];

bool CSoundSystem::Start()
{
    ma_result result = ma_engine_init(NULL, &soundSys);
    if (result != MA_SUCCESS)
    {
        logger->Error("Failed to start: audio engine init error %d", result);
        return false;
    }
    return true;
}

void CSoundSystem::SetListenerData()
{
    ma_engine_listener_set_position(&soundSys, 0,
        CSoundScape::m_vecCameraPos.x, CSoundScape::m_vecCameraPos.y, CSoundScape::m_vecCameraPos.z);
    ma_engine_listener_set_direction(&soundSys, 0,
        CSoundScape::m_vecCameraFront.x, CSoundScape::m_vecCameraFront.y, CSoundScape::m_vecCameraFront.z);
    ma_engine_listener_set_world_up(&soundSys, 0,
        CSoundScape::m_vecCameraTop.x, CSoundScape::m_vecCameraTop.y, CSoundScape::m_vecCameraTop.z);
}

void* CSoundSystem::GetNewSound()
{
    return (void*)&g_SoundsArray[g_nSoundsArrayUsed++];
}

bool CSoundSystem::InstantiateSound(void* precached, void* target, bool modifiedpitch)
{
    int flags = 0;
    if(!modifiedpitch) flags |= MA_SOUND_FLAG_NO_PITCH;
    ma_result result = ma_sound_init_copy(&soundSys, (ma_sound*)precached, flags, NULL, (ma_sound*)target);
    return (result == MA_SUCCESS);
}

bool CSoundSystem::LoadSound(void* ptr, const char* path, bool modifiedpitch, bool stream)
{
    int flags = 0;
    if(stream) flags |= MA_SOUND_FLAG_ASYNC;
    if(!modifiedpitch) flags |= MA_SOUND_FLAG_NO_PITCH;
    
    ma_result result = ma_sound_init_from_file(&soundSys, path, flags, NULL, NULL, (ma_sound*)ptr);
    return (result == MA_SUCCESS);
}

void CSoundSystem::SetPos(void* ptr, Pos3D pos)
{
    ma_sound_set_position((ma_sound*)ptr, pos.x, pos.y, pos.z);
}

void CSoundSystem::UnloadSound(void* ptr)
{
    ma_sound_uninit((ma_sound*)ptr);
}

void CSoundSystem::FadeIn(void* ptr, unsigned int timeMs, float volume)
{
    ma_sound_set_stop_time_in_milliseconds((ma_sound*)ptr, (ma_uint64)(ma_int64)-1);
    ma_sound_set_fade_in_milliseconds((ma_sound*)ptr, 0.0f, volume, timeMs);
}

void CSoundSystem::FadeOut(void* ptr, unsigned int timeMs)
{
    ma_sound_set_fade_in_milliseconds((ma_sound*)ptr, -1.0f, 0.0f, timeMs);
    ma_sound_set_stop_time_in_milliseconds((ma_sound*)ptr, ma_engine_get_time_in_milliseconds(&soundSys) + timeMs);
}

bool CSoundSystem::IsSoundPlaying(void* ptr)
{
    return ma_sound_is_playing((ma_sound*)ptr);
}

void CSoundSystem::SetDistance(void* ptr, float distance)
{
    ma_sound_set_max_distance((ma_sound*)ptr, distance);
}

void CSoundSystem::SetRolloff(void* ptr, float rolloff)
{
    ma_sound_set_rolloff((ma_sound*)ptr, rolloff);
}

void CSoundSystem::SetAttenuationModel(void* ptr, int model)
{
    switch(model)
    {
        case 1: return ma_sound_set_attenuation_model((ma_sound*)ptr, ma_attenuation_model_linear);
        case 2: return ma_sound_set_attenuation_model((ma_sound*)ptr, ma_attenuation_model_exponential);
        default: return ma_sound_set_attenuation_model((ma_sound*)ptr, ma_attenuation_model_inverse);
    }
}

void CSoundSystem::SetLooping(void* ptr, bool loop)
{
    ma_sound_set_looping((ma_sound*)ptr, loop);
}

void CSoundSystem::Start(void* ptr)
{
    ma_sound_start((ma_sound*)ptr);
}

void CSoundSystem::SetPan(void* ptr, float pan)
{
    ma_sound_set_pan((ma_sound*)ptr, pan);
}

void CSoundSystem::SetPitch(void* ptr, float pitch)
{
    ma_sound_set_pitch((ma_sound*)ptr, pitch);
}

void CSoundSystem::Stop(void* ptr)
{
    ma_sound_stop((ma_sound*)ptr);
}