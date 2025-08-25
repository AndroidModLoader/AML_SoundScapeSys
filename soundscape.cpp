#include "soundscape.h"

#include <stdio.h>
#include <stdlib.h>

#include <rapidjson/include/rapidjson/document.h>
#include <rapidjson/include/rapidjson/filereadstream.h>

CSoundScape g_SoundScapes[MAX_SOUNDSCAPES] { 0 };
CSoundScape* g_ActiveSoundScapes[MAX_ACTIVE_SOUNDSCAPES] { NULL };
SoundDef g_SoundDefinitions[MAX_SOUNDSCAPES * MAX_SOUNDSCAPES_SOUNDS] { 0 };
SoundDef g_SoundPrecaches[MAX_PRECACHED_SOUNDS] { 0 };

// Static

bool CSoundScape::StaticInit()
{
    for(int i = 0; i < MAX_SOUNDSCAPES; ++i)
    {
        g_SoundScapes[i].m_nID = i;
        g_SoundScapes[i].m_nTargetWorldID = -1;
        g_SoundScapes[i].m_nTimeStart = 0xFFFFFFFF;
        g_SoundScapes[i].m_nTimeEnd = 0xFFFFFFFF;
    }
    if(!CSoundSystem::Start()) return false;
    return true;
}

bool CSoundScape::LoadDat(const char* filepath)
{
    FILE* jsonFile = fopen(filepath, "rb");
    if(!jsonFile) return false;

    fseek(jsonFile, 0, SEEK_END);
    long len = ftell(jsonFile);
    fseek(jsonFile, 0, SEEK_SET);

    char* buf = new char[len + 1];
    if(!buf)
    {
        fclose(jsonFile);
        return false;
    }

    rapidjson::FileReadStream is(jsonFile, buf, len+1);
    rapidjson::Document doc;
    doc.ParseStream(is);
    delete[] buf;

    if(doc.HasParseError() || !doc.IsObject())
    {
        fclose(jsonFile);
        return false;
    }

    // TODO: Load important keys first!
    for (rapidjson::Value::ConstMemberIterator itr = doc.MemberBegin(); itr != doc.MemberEnd(); ++itr)
    {
        const rapidjson::Value& innerObj = itr->value;
        if(innerObj.IsObject()) // SoundScape definition (most likely.)
        {

        }
        else if(innerObj.IsArray()) // Some special data (precache table for example)
        {

        }
        else // Config or some sort of stuff
        {

        }
    }

    fclose(jsonFile);
    return true;
}

void CSoundScape::UpdateCamera(Pos3D pos, Pos3D front, Pos3D top)
{
    m_vecCameraPos = pos;
    m_vecCameraFront = front;
    m_vecCameraTop = top;
}

void CSoundScape::UpdateWorldId(int worldId)
{
    if(worldId != m_nWorldID)
    {
        // TODO: Callback?
        m_nWorldID = worldId;
    }
}

void CSoundScape::UpdateWorldTime(unsigned int timeValue)
{
    m_nWorldTime = timeValue;
}

bool CSoundScape::HasSpecialFlag(unsigned char flagNum)
{
    return ((m_nSpecialAudioFlags >> flagNum) & 0x1) != 0;
}

CSoundScape* CSoundScape::New()
{
    if(m_nSoundScapes >= MAX_SOUNDSCAPES) return NULL;
    return &g_SoundScapes[m_nSoundScapes++];
}

static int g_nUpdateCounter = 0, g_nUpdateOffset = 0;
void CSoundScape::UpdateAll()
{
    CSoundSystem::SetListenerData();
    
    CSoundScape* sscape;
    if(!m_bSplitUpdates || m_nSoundScapes < 40)
    {
        for(int i = 0; i < m_nActiveSoundScapes; ++i)
        {
            g_ActiveSoundScapes[i]->UpdateActive();
        }
        
        for(int i = 0; i < m_nSoundScapes; ++i)
        {
            sscape = &g_SoundScapes[i];
            if(!sscape->m_bActive)
            {
                sscape->UpdateInactive();
            }
        }
        g_nUpdateCounter = 0;
    }
    else
    {
        if(g_nUpdateCounter == 0)
        {
            for(int i = 0; i < m_nActiveSoundScapes; ++i)
            {
                g_ActiveSoundScapes[i]->UpdateActive();
            }
            
            g_nUpdateOffset = ((m_nSoundScapes - 1) / 8) + 1;
        }
        int start = g_nUpdateCounter * g_nUpdateOffset;
        int end = (g_nUpdateCounter + 1) * g_nUpdateOffset;
        for(int i = start; i < end && i < m_nSoundScapes; ++i)
        {
            sscape = &g_SoundScapes[i];
            if(!sscape->m_bActive)
            {
                sscape->UpdateInactive();
            }
        }
        g_nUpdateCounter = (g_nUpdateCounter + 1) % 8;
    }
}

void CSoundScape::PrecacheAudio(const char* filepath)
{
    if(m_nSoundsPrecached >= MAX_PRECACHED_SOUNDS-1) return;
    if(GetPrecached(filepath) != NULL) return;
    
    SoundDef* attachment = &g_SoundPrecaches[m_nSoundsPrecached++];
    attachment->m_pOwner = NULL;
    attachment->m_pSoundPtr = CSoundSystem::GetNewSound();
    attachment->m_bPrecached = true;
    strncpy(attachment->m_szSoundPath, filepath, sizeof(attachment->m_szSoundPath)-1);
    CSoundSystem::LoadSound(attachment->m_pSoundPtr, filepath);
}

SoundDef* CSoundScape::GetPrecached(const char* filepath)
{
    for(int i = 0; i < m_nSoundsPrecached; ++i)
    {
        if(!strcmp(filepath, g_SoundPrecaches[i].m_szSoundPath))
        {
            return &g_SoundPrecaches[i];
        }
    }
    return NULL;
}

// Member

SoundDef* CSoundScape::AttachSoundDef(const char* filepath)
{
    if(m_nSounds >= MAX_SOUNDSCAPES_SOUNDS) return NULL;
    SoundDef* precache = GetPrecached(filepath);
    
    SoundDef* attachment = &g_SoundDefinitions[m_nSoundsUsed++];
    m_SoundDef[m_nSounds++] = attachment;
    attachment->m_pOwner = this;
    if(precache)
    {
        attachment->m_pPrecache = precache;
        attachment->m_bUsesPrecached = true;
    }
    else
    {
        attachment->m_pSoundPtr = CSoundSystem::GetNewSound();
    }
    strncpy(attachment->m_szSoundPath, filepath, sizeof(attachment->m_szSoundPath)-1);
    return attachment;
}

int CSoundScape::GetActiveListPos()
{
    for(int i = 0; i < m_nActiveSoundScapes; ++i)
    {
        if(g_ActiveSoundScapes[i] == this) return i;
    }
    return -1;
}

bool CSoundScape::InActiveList()
{
    return (GetActiveListPos() != -1);
}

bool CSoundScape::Activate()
{
    if(m_nActiveSoundScapes >= MAX_ACTIVE_SOUNDSCAPES || InActiveList()) return false;
    
    m_bActive = true;
    g_ActiveSoundScapes[m_nActiveSoundScapes++] = this;
    for(int i = 0; i < m_nSounds; ++i)
    {
        if(m_SoundDef[i]->Load())
        {
            m_SoundDef[i]->FadeIn();
        }
    }
    return true;
}

bool CSoundScape::Deactivate()
{
    m_bActive = false;
    int listPos = GetActiveListPos();
    if(listPos == -1) return false;
    
    if(m_nActiveSoundScapes > 1)
    {
        g_ActiveSoundScapes[listPos] = g_ActiveSoundScapes[--m_nActiveSoundScapes];
    }
    else
    {
        g_ActiveSoundScapes[0] = NULL;
        m_nActiveSoundScapes = 0;
    }
    
    for(int i = 0; i < m_nSounds; ++i)
    {
        // TODO: Deactivate sound sources?
    }
    return true;
}

bool CSoundScape::IsInRange()
{
    if(m_nWorldID != -1 && m_nTargetWorldID != -1 && m_nTargetWorldID != m_nWorldID)
    {
        return false;
    }
    if(!IsActiveAtTime())
    {
        return false;
    }
    
    switch(m_nType)
    {
        case SST_BOX:
        {
            bool vmn = (m_box.m_vecStart.x < m_vecCameraPos.x &&
                        m_box.m_vecStart.y < m_vecCameraPos.y &&
                        m_box.m_vecStart.z < m_vecCameraPos.z);
            bool vmx = (m_box.m_vecEnd.x > m_vecCameraPos.x &&
                        m_box.m_vecEnd.y > m_vecCameraPos.y &&
                        m_box.m_vecEnd.z > m_vecCameraPos.z);
            return (vmn && vmx);
        }
        case SST_SPHERE:
        {
            return (m_sphere.m_vecCenter.SqDistTo(m_vecCameraPos) < m_sphere.m_fSqRadius);
        }
        case SST_DOME:
        {
            if(m_sphere.m_vecCenter.z > m_vecCameraPos.z) return false;
            return (m_sphere.m_vecCenter.SqDistTo(m_vecCameraPos) < m_sphere.m_fSqRadius);
        }
        case SST_INVDOME:
        {
            if(m_sphere.m_vecCenter.z < m_vecCameraPos.z) return false;
            return (m_sphere.m_vecCenter.SqDistTo(m_vecCameraPos) < m_sphere.m_fSqRadius);
        }

        default: return false;
    }
    return false;
}

bool CSoundScape::IsTimed()
{
    return (m_nTimeStart != 0xFFFFFFFF && m_nTimeEnd != 0xFFFFFFFF);
}

bool CSoundScape::IsActiveAtTime()
{
    if(!IsTimed()) return true;
    if(m_nTimeStart < m_nTimeEnd)
    {
        return (m_nTimeStart <= m_nWorldTime && m_nWorldTime < m_nTimeEnd);
    }
    else
    {
        return (m_nTimeStart <= m_nWorldTime || m_nWorldTime < m_nTimeEnd);
    }
}

void CSoundScape::UpdateInactive()
{
    if(IsActiveAtTime() && IsInRange())
    {
        Activate();
    }
}

void CSoundScape::UpdateActive()
{
    if(!IsActiveAtTime() || !IsInRange())
    {
        Deactivate();
        return;
    }
    
    for(int i = 0; i < m_nSounds; ++i)
    {
        m_SoundDef[i]->UpdatePos();
        // TODO: position, fading etc
    }
}
