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
