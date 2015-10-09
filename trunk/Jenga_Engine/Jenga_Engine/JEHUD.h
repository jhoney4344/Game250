/*******************************************************************************
Copyright (C) 2015 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
filename    JEHUD.h
author      Jaeheon Lee
email       jhoney4344@gmail.com
course      GAM250
assignment  Final project submission
due date    12/6/2015

Brief Description:
This file is interface file for HUD of game.

*******************************************************************************/

#ifndef JENGA_NUMBER_H
#define JENGA_NUMBER_H
#include "JEMath.h"

namespace JE{
	namespace HUD{
		void Load(void);
		void Init(void);
		void DrawNumber(JEPoint2D pos, double scale, int number);
		void Shutdown(void);
		void Unload(void);
	}
}

#endif