/*******************************************************************************
Copyright (C) 2015 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior 
written consent of DigiPen Institute of Technology is prohibited.
filename    JEFrameTimer.cpp
author      Jaeheon Lee
email       jhoney4344@gmail.com
course      GAM250F14kr
assignment  Final Submission
due date    12/6/2014

Brief Description:
  Time counter for frame time.

*******************************************************************************/
#include "JEFrameTimer.h"

namespace{

LARGE_INTEGER freq, start, end;

}
namespace JE{
namespace FrameTimer{
/*******************************************************************************
   Function: Init

Description: Initialize the timer's variables.

     Inputs: None.

    Outputs: None.
*******************************************************************************/
void Init(void) 
{
	JEPRINTF("Frame timer Initialize\n");
	
	QueryPerformanceFrequency(&freq); // Get system frequency
	start.QuadPart = 0;
	end.QuadPart = 0;
}
/*******************************************************************************
   Function: StartTimer

Description: Start count time.

     Inputs: None.

    Outputs: None.
*******************************************************************************/
void StartTimer(void)
{
    QueryPerformanceCounter(&start); // Save pre-frequency value
}
/*******************************************************************************
   Function: EndTimer

Description: Get time slit from start time to now.

     Inputs: None.

    Outputs: None.
*******************************************************************************/
float EndTimer(void)
{
    QueryPerformanceCounter(&end); // Save later-frequency value

    return (float)(end.QuadPart-start.QuadPart)/ freq.QuadPart;
}

}
}