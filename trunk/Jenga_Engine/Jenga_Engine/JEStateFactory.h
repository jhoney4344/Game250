/*******************************************************************************
Copyright (C) 2015 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
filename    JEStateFactory.h
author      Jaeheon Lee
email       jhoney4344@gmail.com
course      GAM250
assignment  Final project submission
due date    12/6/2015

Brief Description:
  This file is interface file for JEStateFactory.cpp file.
 
*******************************************************************************/
#ifndef JENGA_ENGINE_STATE_FACTORY_H
#define JENGA_ENGINE_STATE_FACTORY_H

#include "JEStates.h"
#include "JEBuilder.h"
#include "JEDebug.h"
#include <map>

namespace JE
{
	class StateFactory
	{
	public:
		StateFactory():stateCount(0) {}
		~StateFactory();
		void HireBuilder(STATES, Builder*);
		States* BuildState(STATES);
		void FireBuilders(void);
	private:
		typedef std::map<STATES, Builder*> BuilderMap;
		typedef BuilderMap::iterator       BuilderItor;

		BuilderMap m_builders;
		int stateCount;
	};

}

#endif