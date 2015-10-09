/*******************************************************************************
Copyright (C) 2015 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior 
written consent of DigiPen Institute of Technology is prohibited.
filename    main.cpp
author      Jaeheon Lee
email       jhoney4344@gmail.com
course      GAM250
assignment  Final project submission
due date    12/6/2015

Brief Description:
  This header include header for game data for entire state.
 
*******************************************************************************/
#ifndef JE_GAME_DATA_H
#define JE_GAME_DATA_H

#include "JEMath.h"

namespace JE{

struct GameData
{
	bool windowInactive;
	int screenWidth;
	int screenHeight;
	bool fullScreen;
	int DayNight;
	bool pause;
	bool muteBGM;
	bool muteEFT;
	JEPoint2D background[3];

	bool padConnected;
	double padTime;
	float padAlpha;
};
}
#endif