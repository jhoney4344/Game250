/*******************************************************************************
Copyright (C) 2015 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior 
written consent of DigiPen Institute of Technology is prohibited.
filename    JEParts.h
author      Jaeheon Lee
email       jhoney4344@gmail.com
course      GAM250
assignment  Final project submission
due date    12/6/2015

Brief Description:
  This file is interface file for  JEParts.cpp file.

*******************************************************************************/
#ifndef JENGA_ENGIN_PARTS_H
#define  JENGA_ENGIN_PARTS_H

static const int MAX_STRING_SIZE = 20;
namespace JE{
#define NUMBER_OF_BODY 17
	enum BODY_PART
	{
		PART_HEAD,
		PART_BODY,
		PART_ARM,
		PART_LEG
	};
	enum PART_TYPE
	{
		BASIC_HEAD,BASIC_BODY,
		BASIC_ARM,BASIC_LEG,
		FEATHER_WING, CLAW_ARM,
		LANTERN_HEAD, DASH_LEG,
		BEAR_BODY, JUMP_LEG,
		RADIO_HEAD, GOOD_BODY, END_TYPE
	};
	enum PART_NAME
	{
		PN_HEAD = 0, PN_BODY = 1, PN_LEFTSHOULDER = 2, PN_RIGHTSHOULDER = 3,
		PN_LEFTARM1 = 4, PN_LEFTARM2 = 5, PN_RIGHTARM1 = 6, PN_RIGHTARM2 = 7,
		PN_LEFTLEG1 = 8, PN_LEFTLEG2 = 9, PN_RIGHTLEG1 = 10, PN_RIGHTLEG2 = 11,
		PN_LEFTHIP = 12, PN_RIGHTHIP = 13, PN_TAIL1 = 14, PN_TAIL2 = 15, PN_TAIL3 = 16
	};
	struct BODY_NAME{
		static char word[NUMBER_OF_BODY][MAX_STRING_SIZE];
	};

}
#endif