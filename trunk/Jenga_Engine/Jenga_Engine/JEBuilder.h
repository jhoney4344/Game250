/*******************************************************************************
Copyright (C) 2015 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
filename    JEBuilder.h
author      Jaeheon Lee
email       jhoney4344@gmail.com
course      GAM250
assignment  Final project submission
due date    12/6/2015

Brief Description:
  This file is interface file for JEBuilder.cpp file.

*******************************************************************************/
#ifndef JENGA_ENGINE_BUILDER_H
#define JENGA_ENGINE_BUILDER_H

#include "JEStates.h"

namespace JE
{

class Builder
{
public:
	virtual ~Builder(void) {}
	virtual States* Create(void) const = 0;
private:

};
///////////////////////////////////
///////////////////////////////////
class Menu_Builder : public Builder
{
public:
	~Menu_Builder(void);
	States* Create(void) const;
private:
};
///////////////////////////////////
///////////////////////////////////

class StateOpenWorld_Builder : public Builder
{
public:
	~StateOpenWorld_Builder(void);
	States* Create(void) const;
private:
};





}//JE


#endif