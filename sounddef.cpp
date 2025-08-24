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
		case SDT_ALONGXY:
		{
			if(m_fSensitiveAxisCoord != CSoundScape::m_vecCameraPos.z)
			{
				m_fSensitiveAxisCoord = CSoundScape::m_vecCameraPos.z;
				m_vecCenter.z = m_fSensitiveAxisCoord;
				CSoundSystem::SetPos(m_pSoundPtr, m_vecCenter);
			}
			break;
		}
	}
}
