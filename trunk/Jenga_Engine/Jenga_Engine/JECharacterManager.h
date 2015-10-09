/*******************************************************************************
Copyright (C) 2015 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
filename    JECharacterManager.h
author      Jaeheon Lee
email       jhoney4344@gmail.com
course      GAM250
assignment  Final project submission
due date    12/6/2015

Brief Description:
This file is interface file for JECharacterManager.cpp file.

*******************************************************************************/

#ifndef JENGA_ENGINE_CHARACTER_MANAGER_H
#define JENGA_ENGINE_CHARACTER_MANAGER_H
#include "JECharacter.h"
#include "JEMath.h"
#include "JEDayAndNight.h"
#include "JECharacter.h"


namespace JE
{
	namespace CharacterManager
	{
		void Load();
		void Init();
		void Update(double dt);
		void Draw();
		void Shutdown();
		void Unload();

		Character& GetPlayerByID(CHARACTER_ID id);

		void Movement(void);

		void SetReGenPosition(JEPoint2D newReGenPos);
		void CharacterRestart();
		int GetMaxPlayerNum();
		JEPoint2D GetPosByID(CHARACTER_ID);
	}
}//JE

#endif