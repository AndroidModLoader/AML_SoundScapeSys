#ifndef __SOUNDSCAPE_H
#define __SOUNDSCAPE_H

#include <mod/logger.h>
#include <math.h>

#define MAX_SOUNDSCAPES                 1024
#define MAX_ACTIVE_SOUNDSCAPES          10
#define MAX_SOUNDSCAPES_SOUNDS          6
#define MAX_SOUNDSCAPES_ACTIVE_SOUNDS   (2 * MAX_ACTIVE_SOUNDSCAPES * MAX_SOUNDSCAPES_SOUNDS)
#define MAX_SOUNDSCAPES_SOUND_PATH      40
#define MAX_PRECACHED_SOUNDS            128

struct CSoundScape;

struct Pos3D
{
    float x, y, z;
	
	inline float SqDistTo(Pos3D& v)
	{
		float fx = v.x - x;
		float fy = v.y - y;
		float fz = v.z - z;
		return (x*x + y*y + z*z);
	}
	inline float SqDistTo2D(Pos3D& v)
	{
		float fx = v.x - x;
		float fy = v.y - y;
		return (x*x + y*y);
	}
	inline float DistTo(Pos3D& v)
	{
		return sqrtf(SqDistTo(v));
	}
	inline float DistTo2D(Pos3D& v)
	{
		return sqrtf(SqDistTo2D(v));
	}
};
enum eSoundScapeType : unsigned char
{
    SST_UNKNOWN = 0,
    SST_BOX,
    SST_SPHERE,
    SST_DOME,
	SST_INVDOME,

    SST_MAXTYPES
};
enum eSoundDistanceType : char
{
    SDT_EVERYWHERE = 0,
    SDT_SPHERICAL,
    SDT_HEIGHT, // AXISZ
    SDT_AXISX,
    SDT_AXISY,
	SDT_ALONGXY, // XZ, YZ?

    SDT_MAXTYPES
};
struct CSoundSystem
{
    static bool Start();
	static void Precache(void* ptr, const char* path);
	static void* IsPrecached(const char* path);
	static void SetListenerData();
	static void* GetNewSound();
	static bool LoadSound(void* ptr, const char* path, bool modifiedpitch = false, bool stream = false);
	static void SetPos(void* ptr, Pos3D pos);
};
struct SoundScapeBox
{
    Pos3D m_vecStart, m_vecEnd;
};
struct SoundScapeSphere
{
    Pos3D m_vecCenter;
    float m_fRadius;
	float m_fSqRadius; // for faster math
};
struct SoundDef
{
	bool IsInRange();
	
    CSoundScape* m_pOwner;
    char m_szSoundPath[MAX_SOUNDSCAPES_SOUND_PATH];
    void* m_pSoundPtr;
    Pos3D m_vecCenter;
    float m_fVolume; // [0.0 - 1.0]
	float m_fDistance;
	float m_fRollOff;
    unsigned int m_nFadeInTime; // Appearing time (ms)
    unsigned int m_nFadeOutTime; // Disappearing time (ms)
    eSoundDistanceType m_nDistanceLogic; // Might be negative to inverse logic
	struct
	{
		char m_bLoaded : 1;
    	char m_bLooped : 1;
    	char m_bActive : 1;
		char m_bStream : 1;
	};
	unsigned char m_nRequiredSpecialFlag;
	unsigned char m_nAttenuationModel;
};

struct CSoundScape
{
	// Static
    static bool StaticInit();
    static bool LoadDat(const char* filepath);
    static void UpdateCamera(Pos3D pos, Pos3D front, Pos3D top);
	static void UpdateWorldId(int worldId);
	static void UpdateWorldTime(unsigned int timeValue);
	static bool HasSpecialFlag(unsigned char flagNum);
	static CSoundScape* New();
	static void UpdateAll();
	static void PrecacheAudio(const char* filepath);

	static inline char m_szPathPrefix[256] = "";
	static inline bool m_bSplitUpdates = true; // Unloads CPU.
    static inline int m_nSoundsUsed = 0;
    static inline int m_nSoundScapes = 0;
	static inline int m_nActiveSoundScapes = 0;
    static inline Pos3D m_vecCameraPos;
    static inline Pos3D m_vecCameraFront;
    static inline Pos3D m_vecCameraTop;
	static inline int m_nWorldID = -1;
	static inline unsigned int m_nWorldTime = 0;
	static inline long long m_nSpecialAudioFlags = 0; // Something you can set in the code [1-63]



	// Member
	SoundDef* AttachSoundDef();
	int GetActiveListPos();
	bool InActiveList();
	bool Activate();
	bool Deactivate();
	bool IsInRange();
	bool IsTimed();
	bool IsActiveAtTime();
	void UpdateInactive();
	void UpdateActive();
	
    int m_nID;
	int m_nTargetWorldID;
	unsigned int m_nTimeStart;
	unsigned int m_nTimeEnd;
    eSoundScapeType m_nType = SST_UNKNOWN;
    bool m_bActive;
    unsigned char m_nSounds;

    union
    {
        SoundScapeBox m_box;
        SoundScapeSphere m_sphere;
    };
    SoundDef* m_SoundDef[MAX_SOUNDSCAPES_SOUNDS];
};

#endif // __SOUNDSCAPE_H
