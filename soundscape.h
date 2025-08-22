#ifndef __SOUNDSCAPE_H
#define __SOUNDSCAPE_H

#include <mod/logger.h>

#define MAX_SOUNDSCAPES                 1024
#define MAX_SOUNDSCAPES_SOUNDS          6
#define MAX_SOUNDSCAPES_ACTIVE_SOUNDS   32
#define MAX_SOUNDSCAPES_SOUND_PATH      160

struct CSoundScape;

struct Pos3D
{
    float x, y, z;
};
enum eSoundScapeType : unsigned char
{
    SST_UNKNOWN = 0,
    SST_BOX,
    SST_SPHERE,
    SST_DOME,

    SST_MAXTYPES
};
struct CSoundSystem
{
    static bool Start();
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
    CSoundScape* m_pOwner;
    char m_szSoundPath[MAX_SOUNDSCAPES_SOUND_PATH];
    void* m_pSoundPtr;
    Pos3D m_vecCenter;
    float m_fVolume; // [0.0 - 1.0]
    unsigned int m_nFadeOutTime; // Appearing time (ms)
    unsigned int m_nFadeInTime; // Disappearing time (ms)
    bool m_bLoaded;
    bool m_bLooped;
    bool m_bActive;
    bool m_bPositional;
};

struct CSoundScape
{
    static bool StaticInit();
    static bool LoadDat(const char* filepath);
    static void UpdateCamera(Pos3D pos, Pos3D front, Pos3D top);

    static inline int m_nSoundsUsed = 0;
    static inline int m_nSoundScapes = 0;
    static inline Pos3D m_vecCameraPos;
    static inline Pos3D m_vecCameraFront;
    static inline Pos3D m_vecCameraTop;



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