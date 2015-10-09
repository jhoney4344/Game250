/*******************************************************************************
Copyright (C) 2015 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
filename    JENumber.cpp
author      Jaeheon Lee
email       jhoney4344@gmail.com
course      GAM250
assignment  Final project submission
due date    12/6/2015

Brief Description:
This file is interface file for HUD.

*******************************************************************************/

#include "JEHUD.h"
#include "JEGraphicsNew.h"
#include "JELoadTextureNew.h"


namespace{
	int numbersID;
	float numberAniRate;
	float numberAtAni[3];
}


namespace JE{
namespace HUD{

void Load(void)
{
	numbersID = JE::GraphicLoadTexture("Texture//Numbers.tga");
}

void Init(void)
{
	numberAniRate = 0.1f; // 0 to 10
	for (int i = 0; i < 3; i++)
		numberAtAni[i] = numberAniRate;
}

void DrawNumber(JEPoint2D pos, double scale = 0.1, int number = 0)
{
	
	int firstDigit  = (int)(number / 100);
	int secondDigit = (int)((number % 100) / 10);
	int thirdDigit  = (int)(number % 10);	

	numberAtAni[0] = firstDigit * numberAniRate;
	numberAtAni[1] = secondDigit * numberAniRate;
	numberAtAni[2] = thirdDigit * numberAniRate;

	for (int i = 0; i < 3; i++)
	{
		
		GraphicSetTexture(numbersID);
		GraphicSetFilpTex(JE::FLIP_IDLE, 10);
		GraphicSetAnimaTex(10, numberAtAni[i]);
		GraphicSetOrthogTransform(pos.x+scale*0.7*i, pos.y, 0.f, //x,y coordinate z order
			scale, scale*1.25f,		//scale x,y
			0.f);
	}
}

void Shutdown(void)
{

}

void Unload(void)
{
	GraphicUnloadTexture(numbersID);
}

}//HUD
}//JE