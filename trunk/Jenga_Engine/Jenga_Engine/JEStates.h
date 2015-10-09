/*******************************************************************************
Copyright (C) 2015 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
filename    JEStates.h
author      Jaeheon Lee
email       jhoney4344@gmail.com
course      GAM250
assignment  Final project submission
due date    12/6/2015

Brief Description:
  This is header file about state classes.

*******************************************************************************/
#ifndef JENGA_ENGINE_STATES_H
#define JENGA_ENGINE_STATES_H

#include "JEStateTypes.h"
#include "JEIniFileManager.h"
#include "JECollisionManager.h"

namespace JE
{

class States 
{
public:
	virtual ~States(){}
	virtual void Load() = 0;
	virtual void Init()     = 0;
	virtual void Update(double, bool& fullScreen)   = 0;
	virtual void ShutDown() = 0;
	virtual void Unload() = 0;
private:
};

class Menu : public States
{
public:
	~Menu(){};
	void Load();
	void Init();
	void Update(double, bool& fullScreen);
	void ShutDown();
	void Unload();
private:
};

class State_OpenWorld : public States
{
public:
	~State_OpenWorld(){};
	void Load();
	void Init();
	void Update(double, bool& fullScreen);
	void ShutDown();
	void Unload();
private:
};

class Victory : public States
{
public:
	~Victory(){};
	void Load();
	void Init() ;
	void Update(double, bool& fullScreen);
	void ShutDown();
	void Unload();
private:
};


}
#endif