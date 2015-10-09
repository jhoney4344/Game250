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
  This file run the game application.

*******************************************************************************/
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include <Windows.h>
#include "JEApplication.h"
#include "JEInput.h"
#include "JEMath.h"
#include "JEFrameTimer.h"
#include "JEGameData.h"
/////////////////CHECK MEMORY LEAK////////////////////////////
#if defined(DEBUG) | defined(_DEBUG) 
#define CRTDBG_MAP_ALLOC 
#define RESOLUTION_TYPE 5

#include <cstdlib>  
#include <crtdbg.h>
#endif 

JE::GameData gd;
JE_APPLICATION JEApp;
/*******************************************************************************
   Function: WinMain

Description: The main function for win32 project.

     Inputs: hInstance - The handle for this window.
	         lpCmd - The command message (no use).
			 nCmdShow - The state how to show.

    Outputs: int.
*******************************************************************************/
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR lpCmd, int nCmdShow)
{
#if defined(DEBUG) | defined(_DEBUG)
	/* This will call _CrtDumpMemoryLeaks() on program exit.*/
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	/*To show memory leaks in the output window */
	//_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
	//_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);

	/*To print to console*/
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDERR);

	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR);

	/* If you have a leak, there is a number in curly braces next to the error.
	Put that number in this function and check the call stack to see when and
	where the allocation happened. Set it to -1 to have it not break.*/
	_CrtSetBreakAlloc(-1);
#endif

	JE_UNUSED_PARAM(lpCmd);
	_CrtSetBreakAlloc(87);

	JEApp.data.width  = GetSystemMetrics(SM_CXSCREEN);
	JEApp.data.height = GetSystemMetrics(SM_CYSCREEN);
	gd.pause = false;
	gd.windowInactive = false;

	JEApp.data.title  = "Adaptation";
	JEApp.data.hInstance = hInstance;

	JEApp.Init(nCmdShow);
	JEApp.Update();
	JEApp.ShutDown();

	return 0;
}