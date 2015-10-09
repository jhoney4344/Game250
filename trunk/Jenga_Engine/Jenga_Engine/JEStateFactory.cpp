/*******************************************************************************
Copyright (C) 2015 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior 
written consent of DigiPen Institute of Technology is prohibited.
filename    JEStateFactory.cpp
author      Jaeheon Lee
email       jhoney4344@gmail.com
course      GAM250
assignment  final project submission
due date    6/12/2015

Brief Description:
  This file managing the builder class in JEBuilder.cpp file.

*******************************************************************************/
#include "JEStateFactory.h"

namespace JE
{
/*******************************************************************************
   Function: ~StateFactory

Description: Destructor, release the resourses.

     Inputs: None.

    Outputs: None.
*******************************************************************************/
StateFactory::~StateFactory()
{
	FireBuilders();
}
/*******************************************************************************
   Function: HireBuilder

Description: Add the new builder.

     Inputs: type - the state.
	         newBuilder - the address for new bulder.

    Outputs: None.
*******************************************************************************/
void StateFactory::HireBuilder(STATES type, Builder* newBuilder)
{
	BuilderItor found = m_builders.find(type);

	if(found == m_builders.end())
		m_builders.insert(std::make_pair(type,newBuilder));
}
/*******************************************************************************
   Function: BuildState

Description: Call the builder so that create the state.

     Inputs: type - The flag for call the proper builder.

    Outputs: None.
*******************************************************************************/
States* StateFactory::BuildState(STATES type)
{
	BuilderItor itor = m_builders.find(type);
	return itor->second->Create();
}
/*******************************************************************************
   Function: FireBuilders

Description: Release the builder class memory.

     Inputs: None.

    Outputs: None.
*******************************************************************************/
void StateFactory::FireBuilders(void)
{
	BuilderItor begin = m_builders.begin();
	BuilderItor end   = m_builders.end();

	while(begin != end)
	{
		delete begin->second;
		begin->second = 0;
		++begin;
	}

	m_builders.clear();
}

}