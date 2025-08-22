#include <mod/amlmod.h>
#include <mod/logger.h>
#include <miniaudio/miniaudio.h>

MYMOD(net.rusjj.soundscapesys, SoundScapes, 1.0, RusJJ)

void *hGTASA, *hGTAVC;

ma_engine engine;
extern "C" void OnModPreLoad()
{
    hGTASA = aml->GetLibHandle("libGTASA.so");
    hGTAVC = aml->GetLibHandle("libGTAVC.so");

    if(!hGTASA && !hGTAVC)
    {
        logger->Error("Failed to start: no supported game was found.");
        return;
    }

    ma_result result = ma_engine_init(NULL, &engine);
    if (result != MA_SUCCESS)
    {
        logger->Error("Failed to start: audio engine init error %d", result);
        return;
    }
}