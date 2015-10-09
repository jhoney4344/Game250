/*******************************************************************************
Copyright (C) 2015 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior 
written consent of DigiPen Institute of Technology is prohibited.
filename    JEFrameTimer.h
author      Jaeheon Lee
email       jhoney4344@gmail.com
course      GAM250
assignment  Final Submission
due date    6/12/2014

Brief Description:
  Timer checker for frame.

*******************************************************************************/
#ifndef JENGA_ENGINE_FRAMETIMER_H
#define JENGA_ENGINE_FRAMETIMER_H
#include <Windows.h>
#include "JEDebug.h"

namespace JE{
namespace FrameTimer{

void Init(void);
void StartTimer(void);
float EndTimer(void);

}//FrameTimer
}//JE

#endif