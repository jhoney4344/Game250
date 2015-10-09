/*******************************************************************************
Copyright (C) 2015 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior 
written consent of DigiPen Institute of Technology is prohibited.
filename    JEDebug.cpp
author      Jaeheon Lee
email       jhoney4344@gmail.com
course      GAM250
assignment  Final Submission
due date    12/6/2015

Brief Description:
  This file control the debugging and non debugging situation.

*******************************************************************************/
#include "JEDebug.h"


#if defined(_DEBUG) | defined(JE_DEBUG)
namespace JE{
	namespace Debug{
/*******************************************************************************
   Function: CreateConsole

Description: Create console for debug.

     Inputs: None.

    Outputs: None.
*******************************************************************************/
void CreateConsole()
{
	FILE * conout;
	AllocConsole();
	freopen_s(&conout,"CONOUT$", "wt", stdout);
}

void ClearConsole()
{
	system("CLS");
}

void CloseConsole()
{
	FreeConsole();
}
} // Debug
} // JE
#else
namespace JE{
namespace Debug{
/*******************************************************************************
   Function: CreateConsole

Description: This is empty function for release mode.

     Inputs: None.

    Outputs: None.
*******************************************************************************/
void CreateConsole()
{
	//Do nothing.
}
void CloseConsole()
{
	//Do nothing.
}

void ClearConsole()
{
}
	} // Debug
} // JE
#endif

