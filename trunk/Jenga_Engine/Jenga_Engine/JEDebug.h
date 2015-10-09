/*******************************************************************************
Copyright (C) 2015 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior 
written consent of DigiPen Institute of Technology is prohibited.
filename    JEDebug.h
author      Jaeheon Lee
email       jhoney4344@gmail.com
course      GAM250
assignment  Final Submission
due date    12/6/2015

Brief Description:
  This file is interface file for JEDebug.cpp file.

*******************************************************************************/
#ifndef JENGA_ENGINE_DEBUG_H
#define JENGA_ENGINE_DEBUG_H

#include "JESTL.h"//windows,iostream
#define JE_UNUSED_PARAM(p) (p)

#if (_DEBUG)
#define JECOUT(...) std::cout << (__VA_ARGS__) << std::endl
#define JEPRINTF(...) printf (__VA_ARGS__)
#else
#define JECOUT(...)
#define JEPRINTF(...)
#endif

namespace JE{
	namespace Debug
	{
		void ClearConsole();
		void CreateConsole();
		void CloseConsole();
	}
}

#endif