/*******************************************************************************
Copyright (C) 2015 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
filename    JEDayAndNight.cpp
author      Jaeheon Lee - entire day and night program
            Seho.joo - Made singlton, and spot light
email       jhoney4344@gmail.com
			jsh2958@gmail.com
course      GAM250
assignment  Final Submission
due date    12/6/2015

Brief Description:
Change the Day and Night.

*******************************************************************************/
#include "JEDayAndNight.h"
#include "JEGraphicsNew.h"
#include "JEOldParticle.h"
#include "JEGameData.h"

extern JE::GameData gd;
static const float DAY_CONSTANT = 2.f;
extern JE::GameData gd;

namespace
{
	int intensitySpeed = 100;
	float maxmumColor = 1.f;
	float minimumColor = .05f;

	JE::DayNight dayNight;
	particleSystem* pParticleSys = &getParticleSystem();
	emitter* pEmitter;
}

namespace JE{


	void DayNight::ChangeState(DAY_NIGHT& currState, Vector3f cameraPos)
	{
		if (currState == DAY)
		{
			if (gd.muteBGM == false)
			{
				SOUNDMANAGER()->Play("night");
				SOUNDMANAGER()->BGMDayMute();
				SOUNDMANAGER()->BGMNightMute();
			}
			currState = NIGHT;
			for (int characterNum = 0; characterNum < NUMBER_OF_PLAYER; ++characterNum)
			{
				if (m_spotDead[characterNum] == false)
					m_spotAmbientIntensity[characterNum] = 0.85f;
			}
			pEmitter = pParticleSys->addParticleEmit(FIREFLY, cameraPos, 500, NIGHT_FIREFLY);
		}
		else
		{
			if (gd.muteBGM == false)
			{
				SOUNDMANAGER()->Play("morning");
				SOUNDMANAGER()->BGMDayMute();
				SOUNDMANAGER()->BGMNightMute();
			}
			currState = DAY;
			for (int characterNum = 0; characterNum < NUMBER_OF_PLAYER; ++characterNum)
			{
				if (m_spotDead[characterNum] == false)
					m_spotAmbientIntensity[characterNum] = 0.f;
			}
		}
	}

	DayNight::DayNight()
	{
		m_timeStop = true;
		m_period = 20;
		m_currState = DAY;
		m_ambientIntensity = 1.f;//1
		m_color.m_x = 1.f;
		m_color.m_y = 1.f;
		m_color.m_z = 1.f;
		m_time = 0;
	}

	DayNight::DayNight(const double& periodSec, DAY_NIGHT dayOrNight)
	{
		m_timeStop = true;
		m_period = periodSec;
		m_currState = dayOrNight;
		m_ambientIntensity = JEGetDirectionalAmbient();//1
		m_color = JEGetDirectionalColor();
		m_time = 0;
	}

	void DayNight::InitDayNight(void)
	{
		m_timeStop = true;
		m_period = 20;
		m_currState = DAY;
		m_ambientIntensity = 1.f;//1
		m_color.m_x = 1.f;
		m_color.m_y = 1.f;
		m_color.m_z = 1.f;
		m_time = 0;
	}

	void DayNight::UpdateDayNight(const double& dt, Vector3f cameraPos)
	{
		JEControlLight(m_color, m_ambientIntensity);
		m_time += dt;
		if (m_timeStop)
		{
			if (m_time <= m_period)
			{
				if (m_currState == NIGHT)
					pEmitter->changeEmitterPos(cameraPos);
				return;

			}
			else
			{
				m_timeStop = false;
				m_time = 0;
			}
		}
		
		if (m_time <= m_period)
		{
			if (m_currState == DAY)
			{
				if (m_color.m_x >= minimumColor)
				{
					m_color.m_x = static_cast<float>(maxmumColor - m_time / m_period);
					m_color.m_y = static_cast<float>(maxmumColor - m_time / m_period);
					m_color.m_z = static_cast<float>(maxmumColor - m_time / m_period);
				}

				for (int characterNum = 0; characterNum < NUMBER_OF_PLAYER; ++characterNum)
				{
					if (m_spotAmbientIntensity[characterNum] <= .85f && m_spotDead[characterNum] == false)
						m_spotAmbientIntensity[characterNum] += float(m_time / intensitySpeed);
				}

				//background
				for (int i = 0; i < 3; i++)
					gd.background[i].y -= 6 * dt;
			}
			else if (m_currState == NIGHT)
			{
				if (pEmitter != 0 && m_time <= m_period - 2)
				{
					//pParticleSys->deleteParticleEmit(NIGHT_FIREFLY);
					pEmitter->setNumberOfParticle(0);
					pEmitter = 0;
				}

				if (m_color.m_x <= maxmumColor)
				{
					m_color.m_x = static_cast<float>(minimumColor + m_time / m_period);
					m_color.m_y = static_cast<float>(minimumColor + m_time / m_period);
					m_color.m_z = static_cast<float>(minimumColor + m_time / m_period);
				}

				for (int characterNum = 0; characterNum < NUMBER_OF_PLAYER; ++characterNum)
				{
					if (m_spotAmbientIntensity[characterNum] >= 0.0f && m_spotDead[characterNum] == false)
						m_spotAmbientIntensity[characterNum] -= float(m_time / intensitySpeed);
				}

				//background
				for (int i = 0; i < 3; i++)
					gd.background[i].y += 4 * dt;
			}
		}
		else
		{
			m_time = 0;
			ChangeState(m_currState, cameraPos);
			m_timeStop = true;
		}
		
	}
	DAY_NIGHT DayNight::GetCurrState()
	{
		return m_currState;
	}
	DayNight::~DayNight()
	{
		m_period = 0;
		m_time = 0;
		m_ambientIntensity = 0;
		m_color.m_x = 0.01f;
		m_color.m_y = 0.01f;
		m_color.m_z = 0.01f;
		
	}

	void DayNight::InitSpotLight(int playerIndex, int spotID, JE::Vector3f position, float ambient, float angle)
	{
		m_angle[playerIndex] = angle;
		m_spotAmbientIntensity[playerIndex] = ambient;
		m_lightSkill[playerIndex] = false;
		m_spotID[playerIndex] = spotID;
		m_spotDead[playerIndex] = false;
		m_spotColor[playerIndex] = Vector3f(1.f, 1.f, 1.f);

		JE::JEAddSpotLight(spotID, position, m_spotColor[playerIndex], angle);
	}
	void DayNight::ChangeSpotLight(int playerIndex, bool lightDead, JE::Vector3f position)
	{
		if (m_lightSkill[playerIndex] == true)
		{
			m_angle[playerIndex] = 100;
			m_spotColor[playerIndex] = Vector3f(1.f, 1.f, 1.f);
		}
		else
		{
			m_angle[playerIndex] = 50;
			m_spotColor[playerIndex] = Vector3f(1.f, 1.f, 1.f);
		}

		if (lightDead == true)
		{
			m_spotDead[playerIndex] = false;
		}
		else
		{
			m_spotDead[playerIndex] = true;
			m_spotAmbientIntensity[playerIndex] = 0.f;
		}
		JEChangeSpotLight(m_spotID[playerIndex], position, m_spotColor[playerIndex], m_spotAmbientIntensity[playerIndex], m_angle[playerIndex]);
	}

	void DayNight::SetLightSkill(int playerIndex,bool lightSwitch)
	{
		m_lightSkill[playerIndex] = lightSwitch;
	}

	bool DayNight::GetLightSkill(int playerIndex)
	{
		return m_lightSkill[playerIndex];
	}

	float DayNight::GetSpotAngle(int playerIndex)
	{
		return m_angle[playerIndex];
	}

	float DayNight::GetSpotAmbient(int playerIndex)
	{
		return m_spotAmbientIntensity[playerIndex];
	}

	DayNight& getDayNight()
	{
		return dayNight;
	}
}