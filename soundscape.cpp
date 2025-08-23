#include "soundscape.h"

#include <stdio.h>
#include <stdlib.h>

#include <rapidjson/include/rapidjson/document.h>
#include <rapidjson/include/rapidjson/filereadstream.h>

CSoundScape g_SoundScapes[MAX_SOUNDSCAPES] { 0 };
CSoundScape* g_ActiveSoundScapes[MAX_ACTIVE_SOUNDSCAPES] { NULL };
SoundDef g_SoundDefinitions[MAX_SOUNDSCAPES * MAX_SOUNDSCAPES_SOUNDS] { 0 };

// Static

bool CSoundScape::StaticInit()
{
    for(int i = 0; i < MAX_SOUNDSCAPES; ++i)
    {
        g_SoundScapes[i].m_nID = i;
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

    for (rapidjson::Value::ConstMemberIterator itr = doc.MemberBegin(); itr != doc.MemberEnd(); ++itr)
    {
        const rapidjson::Value& innerObj = itr->value;
        if(innerObj.IsObject()) // SoundScape definition (most likely.)
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

CSoundScape* CSoundScape::New()
{
	return &g_SoundScapes[m_nSoundScapes++];
}

// Member

SoundDef* CSoundScape::AttachSoundDef()
{
	if(m_nSounds >= MAX_SOUNDSCAPES_SOUNDS) return NULL;
	
	SoundDef* attachment = &g_SoundDefinitions[m_nSoundsUsed++];
	m_SoundDef[m_nSounds++] = attachment;
	attachment->m_pOwner = this;
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
		// TODO: Activate sound sources?
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
