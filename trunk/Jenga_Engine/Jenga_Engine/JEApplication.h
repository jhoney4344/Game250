/*******************************************************************************
Copyright (C) 2015 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior 
written consent of DigiPen Institute of Technology is prohibited.
filename    JEApplication.h
author      Jaeheon Lee
email       jhoney4344@gmail.com
course      GAM250
assignment  Final Submission
due date    12/6/2015

Brief Description:
  This file is interface file for JEApplication.cpp file.

*******************************************************************************/
#ifndef JENGA_ENGINE_APPLICATION
#define JENGA_ENGINE_APPLICATION

#include "JESTL.h"//windows,windowsx
#include "JEMath.h"
#include "JEInput.h"
#include "JEPad.h"
#include "JEFrameTimer.h"
#include "JEStateManager.h"
#include "JEDebug.h"

#include "JEGraphicsNew.h"

struct APPLICATION_DATA
{
	unsigned int width;
	unsigned int height;
	LPCTSTR title;
	HINSTANCE hInstance;
};

class JE_APPLICATION
{
public:
	JE_APPLICATION(void); // This constructor exist for initialize 'data' values.
	void Init(int nCmdShow);
	void Update(void);
	void ShutDown(void);

	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
	APPLICATION_DATA data;

//private:
	WNDCLASS WndClass;
	//HWND hWnd;
};


namespace JE{
namespace APP{

void FullScreen(bool);

} // APP
} // JE

void MessagePump(void);
HDC GetHDC();

#endif