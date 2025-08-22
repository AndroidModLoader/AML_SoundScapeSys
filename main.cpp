#include <mod/amlmod.h>
#include <mod/logger.h>
#include "soundscape.h"

MYMOD(net.rusjj.soundscapesys, SoundScapes, 1.0, RusJJ)

void *hGTASA, *hGTAVC;


extern "C" void OnModPreLoad()
{
    hGTASA = aml->GetLibHandle("libGTASA.so");
    hGTAVC = aml->GetLibHandle("libGTAVC.so");

    if(!hGTASA && !hGTAVC)
    {
        logger->Error("Failed to start: no supported game was found.");
        return;
    }
    if(!CSoundScape::StaticInit())
    {
        // Failed.
        return;
    }
}