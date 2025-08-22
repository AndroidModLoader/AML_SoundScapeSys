#include "soundscape.h"

#include <miniaudio/miniaudio.h>

ma_engine soundSys;
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