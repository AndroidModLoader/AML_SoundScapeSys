#ifndef __SOUNDSCAPE_H
#define __SOUNDSCAPE_H

#define MAX_SOUNDSCAPES                 1024
#define MAX_SOUNDSCAPES_SOUNDS          6
#define MAX_SOUNDSCAPES_ACTIVE_SOUNDS   32
#define MAX_SOUNDSCAPES_SOUND_PATH      160

enum eSoundScapeType : unsigned char
{
    SST_UNKNOWN = 0,
    SST_BOX,
    SST_SPHERE,
    SST_DOME,

    SST_MAXTYPES
};

struct Pos3D
{
    float x, y, z;
};

struct SoundScapeBox
{
    Pos3D m_vecStart, m_vecEnd;
};
struct SoundScapeSphere
{
    Pos3D m_vecCenter;
    float m_fRadius;
};
struct SoundDef
{
    char m_szSoundPath[MAX_SOUNDSCAPES_SOUND_PATH];
    void* m_pSoundPtr;
    Pos3D m_vecCenter;
    float m_fVolume;
    unsigned int m_nFadeOutTime; // Appearing time
    unsigned int m_nFadeInTime; // Disappearing time
    bool m_bLooped;
    bool m_bActive;
};

struct CSoundScape
{
    static bool LoadDat(const char* filepath);
    static inline int m_nSoundScapes = 0;

    int m_nID;
    eSoundScapeType m_nType = SST_UNKNOWN;
    bool m_bActive;
    unsigned char m_nSounds;

    union m_Soundscape
    {
        SoundScapeBox m_box;
        SoundScapeSphere m_sphere;
    };
    SoundDef* m_SoundDef[MAX_SOUNDSCAPES_SOUNDS];
};

#endif // __SOUNDSCAPE_H