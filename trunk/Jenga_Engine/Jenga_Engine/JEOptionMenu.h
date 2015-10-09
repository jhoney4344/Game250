/*******************************************************************************
Copyright (C) 2015 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
filename    JEOptionMenu.h
author      Jaeheon Lee
email       jhoney4344@gmail.com
course      GAM250
assignment  Final project submission
due date    12/6/2015

Brief Description:
This file is interface file for JEOptionMenu.cpp file.

*******************************************************************************/

#ifndef OPTION_MENU_H
#define OPTION_MENU_H

namespace JE{
	void OptionLoad(void);
	void OptionInit(void);
	void OptionUpdate(double mouseX, double mouseY, double padX, double PadY, double dt);
	void OptionClose();
	void OptionDraw(void);
	void OptionShutdown(void);
	void OptionUnload(void);
}

#endif