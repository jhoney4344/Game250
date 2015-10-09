/*******************************************************************************
Copyright (C) 2015 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
filename    JEParts.cpp
author      Jaeheon Lee
email       jhoney4344@gmail.com
course      GAM250
assignment  Final project submission
due date    12/6/2015

Brief Description:
This file is for parts of body.

*******************************************************************************/

#include "JEParts.h"
namespace JE{
	char BODY_NAME::word[NUMBER_OF_BODY][MAX_STRING_SIZE] 
	= {
		"head", "body", "leftshoulder", "rightshoulder",
		"leftarm1", "leftarm2", "rightarm1", "rightarm2",
		"leftleg1", "leftleg2", "rightleg1", "rightleg2",
		"lefthip", "righthip", "tail1", "tail2", "tail3"
	};
}