#include "soundscape.h"

bool SoundDef::IsInRange()
{
    bool inverseLogic = false;
    eSoundDistanceType logic = m_nDistanceLogic;
    if(logic < 0)
    {
        logic = (eSoundDistanceType)-m_nDistanceLogic;
        inverseLogic = true;
    }
    
    switch(logic)
    {
        case SDT_EVERYWHERE: return true;
        case SDT_SPHERICAL:
        {
            if(inverseLogic)
            {
                return (m_vecCenter.SqDistTo(CSoundScape::m_vecCameraPos) > m_fDistance*m_fDistance);
            }
            else
            {
                return (m_vecCenter.SqDistTo(CSoundScape::m_vecCameraPos) < m_fDistance*m_fDistance);
            }
        }
        case SDT_HEIGHT:
        {
            float h = CSoundScape::m_vecCameraPos.z - m_vecCenter.z;
            if(inverseLogic) h = -h;
            return (h > 0.0f && h < m_fDistance);
        }
        case SDT_AXISX:
        {
            float x = CSoundScape::m_vecCameraPos.x - m_vecCenter.x;
            if(inverseLogic) x = -x;
            return (x > 0.0f && x < m_fDistance);
        }
        case SDT_AXISY:
        {
            float y = CSoundScape::m_vecCameraPos.y - m_vecCenter.y;
            if(inverseLogic) y = -y;
            return (y > 0.0f && y < m_fDistance);
        }
        case SDT_ALONGXY:
        {
            if(inverseLogic)
            {
                return (m_vecCenter.SqDistTo2D(CSoundScape::m_vecCameraPos) > m_fDistance*m_fDistance);
            }
            else
            {
                return (m_vecCenter.SqDistTo2D(CSoundScape::m_vecCameraPos) < m_fDistance*m_fDistance);
            }
        }
        
        default: return false;
    }
    return false;
}

void SoundDef::UpdatePos()
{
    if(!m_pSoundPtr) return;
    
    eSoundDistanceType logic = m_nDistanceLogic;
    if(logic < 0)
    {
        logic = (eSoundDistanceType)-m_nDistanceLogic;
    }
    switch(logic)
    {
        case SDT_EVERYWHERE:
            return;

        case SDT_ALONGXY:
        {
            if(m_fSensitiveAxisCoord != CSoundScape::m_vecCameraPos.z)
            {
                m_fSensitiveAxisCoord = CSoundScape::m_vecCameraPos.z;
                m_vecCenter.z = m_fSensitiveAxisCoord;
                SetPos(m_vecCenter);
            }
            break;
        }

        default: return;
    }
}

bool SoundDef::Load()
{
    if(m_bLoaded)
    {
        return false;
    }

    if(m_bUsesPrecached && m_pPrecache)
    {
        if(CSoundSystem::InstantiateSound(m_pPrecache->m_pSoundPtr, m_pSoundPtr, m_bHasPitchMod))
        {
            m_bLoaded = true;
        }
    }

    // Not precached/instantiate failed
    if(!m_bLoaded)
    {
        m_bLoaded = CSoundSystem::LoadSound(m_pSoundPtr, m_szSoundPath, m_bHasPitchMod, m_bStream);
    }

    if(m_bLoaded)
    {
        m_bLoaded = true;
        CSoundSystem::SetDistance(m_pSoundPtr, m_fDistance);
        CSoundSystem::SetRolloff(m_pSoundPtr, m_fRollOff);
        CSoundSystem::SetAttenuationModel(m_pSoundPtr, m_nAttenuationModel);
        CSoundSystem::SetLooping(m_pSoundPtr, m_bLooped);
        if(m_bHasPitchMod)
        {
            CSoundSystem::SetPan(m_pSoundPtr, m_fPan);
            CSoundSystem::SetPitch(m_pSoundPtr, m_fPitch);
        }
        UpdatePos();
        return true;
    }

    return false;
}

void SoundDef::Unload()
{
    if(!m_bLoaded) return;
    
    CSoundSystem::UnloadSound(m_pSoundPtr);
    m_bLoaded = false;
}

void SoundDef::SetPos(Pos3D pos)
{
    CSoundSystem::SetPos(m_pSoundPtr, pos);
}

void SoundDef::FadeIn()
{
    if(!m_bLoaded) return;

    CSoundSystem::FadeIn(m_pSoundPtr, m_nFadeInTime, m_fVolume);
    CSoundSystem::Start(m_pSoundPtr);
    m_bFadingOut = false;
}

void SoundDef::FadeOut()
{
    CSoundSystem::FadeIn(m_pSoundPtr, m_nFadeOutTime);
    m_bFadingOut = true;
}

bool SoundDef::IsActive()
{
    return m_bLoaded;
}

bool SoundDef::IsFadingOut()
{
    return (IsPlaying() && m_bFadingOut);
}

bool SoundDef::IsPlaying()
{
    return (IsActive() && CSoundSystem::IsSoundPlaying(m_pSoundPtr));
}

void SoundDef::Stop()
{
    CSoundSystem::Stop(m_pSoundPtr);
}