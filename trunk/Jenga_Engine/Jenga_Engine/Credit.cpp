/*******************************************************************************
Copyright (C) 2014 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior 
written consent of DigiPen Institute of Technology is prohibited.
filename    Creadit.cpp
author      Nahye Park
email       skqla12@gmail.com
course      GAM200
due date    12/12/2014

Brief Description:
  This file is for creadit in main menu.

*******************************************************************************/

#include "JEStates.h"
#include "JEInput.h"
#include "JEDebug.h"
#include "JELoadTextureNew.h"
#include "JEGraphicsNew.h"
#include "JEStateManager.h"


namespace{

	int creditBackgroundID;

}

namespace JE
{
	void Credit::Load()
	{}
/*******************************************************************************
   Function: Init

Description: Init function for credit.

     Inputs: None.

    Outputs: None.
*******************************************************************************/
void Credit::Init()
{
	GraphicInitCamera();


	SOUNDMANAGER()->LoadSound();
	SOUNDMANAGER()->PlaySound("main");

	creditBackgroundID = JE::GraphicLoadTexture("Texture//Credit.tga");

	JE::GraphicCameraZoom(false, 40.f);
	JEPRINTF("Credit Init");
}

/*******************************************************************************
   Function: Update

Description: Update function for credit.

     Inputs: dt - for time.

    Outputs: None.
*******************************************************************************/
void Credit::Update(double dt, bool &fullScreen)
{
	JE_UNUSED_PARAM(fullScreen);
	JE_UNUSED_PARAM(dt);


	if(g_Input->IsTriggered(JE_BACKSPACE) ||g_Input->IsTriggered(JE_ESCAPE))
		StateManager::SetNextState(MENU);


	
	GraphicSetTexture(creditBackgroundID);
	GraphicSetFilpTex(FLIP_IDLE, 1);
	GraphicSetAnimaTex(1,0);
	GraphicSetPersTransform(0.f,4.f,0.f,		//x,y coordinate z order
		100,100,			//scale x,y
		0);


}

/*******************************************************************************
   Function: ShutDown

Description: Shtu down.

     Inputs: None.

    Outputs: None.
*******************************************************************************/
void Credit::ShutDown()
{
	GraphicUnloadTexture(creditBackgroundID);

	SOUNDMANAGER()->DeleteSound();
	JEPRINTF("Credit ShutDown\n");
}

void Credit::Unload()
{}
}//Jenga Engine