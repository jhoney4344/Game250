/*******************************************************************************
Copyright (C) 2015 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior 
written consent of DigiPen Institute of Technology is prohibited.
filename    JEBuilder.cpp
author      Jaeheon Lee
email       jhoney4344@gmail.com
course      GAM250
assignment  Final project submission
due date    12/6/2015

Brief Description:
  This classes are for create class.(to avoid use switch statement)

*******************************************************************************/
#include "JEBuilder.h"

namespace JE
{


/*******************************************************************************
Function: ~StateOpenWorld_Builder

Description: Release the resourses. Empty function yet.

Inputs: None.

Outputs: None.
*******************************************************************************/
StateOpenWorld_Builder::~StateOpenWorld_Builder(void)
{

}
/*******************************************************************************
Function: StateOpenWorld_Builder::Create

Description: create the class.

Inputs: None.

Outputs: new state class.
*******************************************************************************/
States* StateOpenWorld_Builder::Create(void) const
{
	return new State_OpenWorld;
}

/*******************************************************************************
   Function: ~Menu_Builder

Description: Release the resourses. Empty function yet.

     Inputs: None.

    Outputs: None.
*******************************************************************************/
Menu_Builder::~Menu_Builder(void)
{

}
/*******************************************************************************
   Function: Menu_Builder::Create

Description: create the class.

     Inputs: None.

    Outputs: new state class.
*******************************************************************************/
States* Menu_Builder::Create(void) const
{
	return new Menu;
}



}//JE

