#include <mod/amlmod.h>
#include <mod/logger.h>
#include "soundscape.h"

MYMOD(net.rusjj.soundscapesys, SoundScapes, 1.0, RusJJ)

void *hGTASA, *hGTAVC;


extern "C" void OnModPreLoad()
{
    CGameModule::LoadModules();
    if(!CGameModule::Active())
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