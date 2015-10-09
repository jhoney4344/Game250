/*******************************************************************************
Copyright (C) 2015 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
filename    JEDayAndNight.h
author      Jaeheon Lee
email       jhoney4344@gmail.com
course      GAM250
assignment  Final Submission
due date    12/6/2015

Brief Description:
Change the Day and Night.

*******************************************************************************/
#ifndef DAY_AND_NIGHT_H
#define DAY_AND_NIGHT_H
#include "JEOGLMathHelper.h"
#include "JESound.h"


#define NUMBER_OF_PLAYER 2

namespace JE{

	enum DAY_NIGHT{
		DAY_START,

		DAY, NIGHT,

		DAY_END

	};

	class DayNight
	{
	public:
		DayNight();
		DayNight(const double& period, DAY_NIGHT dayOrNight);
		~DayNight(void);
		void ChangeState(DAY_NIGHT& currState, Vector3f cameraPos);
		void UpdateDayNight(const double& dt, Vector3f cameraPos);
	
		void InitDayNight(void);

		void InitSpotLight(int playerIndex, int spotID, JE::Vector3f position, float ambient, float angle);
		void ChangeSpotLight(int playerIndex, bool lightDead, JE::Vector3f position);
		void SetLightSkill(int playerIndex, bool lightSwitch);
		bool GetLightSkill(int playerIndex);
		float GetSpotAngle(int playerIndex);
		float GetSpotAmbient(int playerIndex);

		DAY_NIGHT GetCurrState();
	private:
		double    m_period;
		double    m_time;
		float     m_ambientIntensity;
		Vector3f  m_color;
		DAY_NIGHT m_currState;
		bool      m_timeStop;

		float     m_angle[NUMBER_OF_PLAYER];
		float     m_spotAmbientIntensity[NUMBER_OF_PLAYER];
		int       m_spotID[NUMBER_OF_PLAYER];
		bool      m_lightSkill[NUMBER_OF_PLAYER];
		bool      m_spotDead[NUMBER_OF_PLAYER];
		Vector3f  m_spotColor[NUMBER_OF_PLAYER];


	};
	
	DayNight& getDayNight();
}
#endif