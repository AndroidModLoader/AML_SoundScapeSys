#include "soundscape.h"
#include <mod/amlmod.h>

static bool g_bDisableAmbientSoundtrack = false;
static char g_szDatPath[512] { 0 };
static char g_szAudioPath[512] { 0 };
static int g_nAudioPathOffset = 0;

struct CGTASAModule : CGameModule
{
    CGTASAModule(const char* lib) : CGameModule(lib) {}

    virtual bool IsSupported() { return true; }
    virtual bool LoadedData(rapidjson::Document& doc)
    {
        if(doc.HasMember("disable_ambient_soundtrack"))
        {
            g_bDisableAmbientSoundtrack = doc["disable_ambient_soundtrack"].GetBool();
        }
        return true;
    }
    virtual void LateActivated()
    {
        snprintf(g_szDatPath, sizeof(g_szDatPath), "%ssoundscape/soundscapes.dat", aml->GetAndroidDataPath());
        snprintf(g_szAudioPath, sizeof(g_szAudioPath), "%ssoundscape/", aml->GetAndroidDataPath());
        g_nAudioPathOffset = strlen(g_szAudioPath);
    }
    virtual const char* GetFullAudioPath(const char* filepath)
    {
        strcpy(g_szAudioPath + g_nAudioPathOffset, filepath);
        return g_szAudioPath;
    }
};
//CGTASAModule gtasaModule("libGTASA.so"); // Activate a module