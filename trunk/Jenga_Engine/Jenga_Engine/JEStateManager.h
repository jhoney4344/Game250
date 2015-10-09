/*******************************************************************************
Copyright (C) 2015 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
filename    JEStateManager
author      Jaeheon Lee
email       jhoney4344@gmail.com
course      GAM250
assignment  Final project submission
due date    12/6/2015

Brief Description:
  This file is interface file for  JEStateManager.cpp file.

*******************************************************************************/
#ifndef JENGA_ENGINE_STATEMANAGER
#define JENGA_ENGINE_STATEMANAGER

#define NEW_STATE_MANAGER

#include "JEStates.h"
#include "JEFrameTimer.h"
#include "JEStateFactory.h"
#include "JEInput.h"
#include "JESound.h"
#include "JEObjectManager.h"
namespace JE
{

namespace StateManager
{

void Init();
void Update(HWND& hWnd);
void ShutDown();

void SetNextState(STATES);
void RestartState(void);
}

}
#endif