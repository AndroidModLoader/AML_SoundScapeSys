#include "soundscape.h"

#include <stdio.h>
#include <stdlib.h>

#include <rapidjson/include/rapidjson/document.h>
#include <rapidjson/include/rapidjson/filereadstream.h>

CSoundScape g_SoundScapes[MAX_SOUNDSCAPES] { 0 };
SoundDef g_SoundDefinitions[MAX_SOUNDSCAPES * MAX_SOUNDSCAPES_SOUNDS] { 0 };

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