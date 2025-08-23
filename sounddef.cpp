#include "soundscape.h"

bool SoundDef::IsInRange()
{
	bool inverseLogic = false;
	if(m_nDistanceLogic < 0)
	{
		m_nDistanceLogic = (eSoundDistanceType)-m_nDistanceLogic;
		inverseLogic = true;
	}
	
	switch(m_nDistanceLogic)
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
