/*******************************************************************************
Copyright (C) 2015 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
filename    JEStateManager.cpp
author      Jaeheon Lee
email       jhoney4344@gmail.com
course      GAM250
assignment  Final project submission
due date    12/6/2015

Brief Description:
  This file managing state factory and make state. Control the states.

*******************************************************************************/
#include "JEStateManager.h"
#include "JEShop.h"
#include "JEGameData.h"
#include "JEApplication.h"
#define JE_30_FRAMES 0.033f
#define JE_60_FRAMES 0.016f
extern JE::GameData gd;
extern JE_APPLICATION JEApp;

//Unnamed namespace for static data
namespace{
	
	struct SMData
	{
		JE::STATES currState;
		JE::STATES nextState;
		JE::States *pState;
		bool restart;
	};
	
	// Global variables for this file(state manager).
	SMData smData;
	JE::StateFactory factory;

	double dt;
	bool fullScreen = false;
}

namespace JE{ 
namespace StateManager
{
/*******************************************************************************
   Function: Init

Description: Initialize the state manager variable.
             Hire the builder for call those classes.

     Inputs: None.

    Outputs: None.
*******************************************************************************/
void Init()
{
	JEPRINTF("State manager init\n");

	smData.pState = 0;
	smData.restart = false;
	gd.muteBGM = false;
	gd.muteEFT = false;
	smData.currState = smData.nextState = MENU;

	factory.HireBuilder(MENU, new Menu_Builder);
	factory.HireBuilder(STATE_OPENWORLD, new StateOpenWorld_Builder);

	FrameTimer::Init();
	FrameTimer::StartTimer();
	ObjectManager::Init();
	ShopLoad();
	
}
/*******************************************************************************
   Function: Update

Description: Managing the current state. Run the game and exit the game.

     Inputs: None.

    Outputs: None.
*******************************************************************************/
void Update(HWND& hWnd)
{

	hWnd;
	MSG Message;
	ZeroMemory(&Message, sizeof(Message));// initialize message variable as 0.
	JEPRINTF("State manager update\n");

	APP::FullScreen(true);

	while(smData.nextState<END_STATE )
	{
	// If restart game is true, we keep our state.
	if(smData.restart)
	{
		smData.restart = false;
	}
	// Else if we change our state, delete and change state pointer.
	else
	{
		delete smData.pState;
		smData.pState = factory.BuildState(smData.currState);
		JEPRINTF("make new state\n");
	}
	
	// not pointing yet
	 if(smData.pState == 0)
		 JEPRINTF("bad pointer\n");
	 else
	 {
		// Load,Unload should be done only when start new state, not for restart.
		smData.pState->Load();
	    smData.pState->Init();

		while(smData.currState == smData.nextState && smData.restart == false)
	    {
		  if(PeekMessage(&Message, NULL, 0, 0,PM_REMOVE) )
		  {
		    TranslateMessage(&Message);
			DispatchMessage(&Message);
		  }

		 // predt = FrameTimer::EndTimer();
		  //dt = (predt > JE_30_FRAMES ? JE_30_FRAMES : predt);
		  dt = FrameTimer::EndTimer();
		  if( dt >=  JE_60_FRAMES)
		  {
			 MyGamepad->Update();
			 smData.pState->Update(dt, fullScreen);
			 g_Input->Reset();
			 MyGamepad->RefreshState();
			  
			 FrameTimer::StartTimer();
		   }
		}
	    smData.pState->ShutDown();
		smData.pState->Unload();
	}
	 smData.currState = smData.nextState;
	}
}
/*******************************************************************************
   Function: ShutDown

Description: Release the state manager resource.

     Inputs: None.

    Outputs: None.
*******************************************************************************/
void ShutDown()
{
	JEPRINTF("State manager shut down");
	delete smData.pState;
	smData.pState = 0;
	ShopUnload();
	ObjectManager::ShutDown();
}
/*******************************************************************************
   Function: SetNextState

Description: Get next state and change next state flag.

     Inputs: nextState - get next state.

    Outputs: None.
*******************************************************************************/
void SetNextState(STATES nextState)
{
	smData.nextState = nextState;
}
/*******************************************************************************
   Function: RestartState

Description: change current restart variable state.

     Inputs: None.

    Outputs: None.
*******************************************************************************/
void RestartState(void)
{
	smData.restart = true;
}

}}