#include <mod/amlmod.h>
#include "soundscape.h"

static CGameModule* g_aModules[128];
static int g_nModules = 0;

void CGameModule::LoadModules()
{
    for(int i = 0; i < g_nModules; ++i)
    {
        g_aModules[i]->m_hLibrary = aml->GetLibHandle(g_aModules[i]->m_szLib);
        g_aModules[i]->m_pAddress = aml->GetLib(g_aModules[i]->m_szLib);
        if(g_aModules[i]->IsSupported())
        {
            m_pActiveModule = g_aModules[i];
            return;
        }
    }
}

CGameModule::CGameModule(const char* libName)
{
    this->m_szLib = libName;
    g_aModules[g_nModules++] = this;
}

bool CGameModule::IsSupported()
{
    return false;
}

void CGameModule::PreActivated()
{

}

void CGameModule::PostActivated()
{

}

void CGameModule::LateActivated()
{

}

bool CGameModule::LoadedData(rapidjson::Document& doc)
{
    return true; // false to stop next stages (post wont be called)
}

void CGameModule::PostLoadedData(rapidjson::Document& doc)
{
    
}

const char* CGameModule::GetFullAudioPath(const char* filepath)
{
    return filepath;
}