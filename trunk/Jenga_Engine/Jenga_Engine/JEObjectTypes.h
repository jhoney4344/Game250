/*******************************************************************************
Copyright (C) 2015 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior 
written consent of DigiPen Institute of Technology is prohibited.
filename    JEObjectTypes.h
author      Jaeheon Lee
email       jhoney4344@gmail.com
course      GAM250
assignment  Final project submission
due date    12/6/2015

Brief Description:
  Organizing objects and body parts types.

*******************************************************************************/
#ifndef OBJECT_TYPES_H
#define OBJECT_TYPES_H
#define TEXT_OBJ_INDEX 20
namespace JE{

	enum DRAW_BASE
	{
		DRAW_TEXT,
		DRAW_BONE,
		DRAW_END
	};
	enum OBJECTS
	{
		OBJ_BEGINE,

		// object using texture
		OBJ_MEAT = 0,
		OBJ_BREAD = 1,
		OBJ_SHORT_TREE = 2,
		OBJ_LONG_TREE = 3,
		OBJ_ENEMY = 4,
		OBJ_STONE = 5,
		OBJ_BUG = 6,
		OBJ_MINERAL = 7,
		OBJ_SHOP = 8,
		OBJ_TOMB = 9,

		OBJ_EMPTY = 19,
		// under objects are use bone
		OBJ_ZOMBIE = 20,
		OBJ_DEER = 21,
		OBJ_END
	};
}
#endif