/*******************************************************************************
Copyright (C) 2015 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
filename    JEAniType.h
author      Jaeheon Lee
email       jhoney4344@gmail.com
course      GAM250
assignment  Final Submission
due date    12/6/2015

Brief Description:
enum type for animation

*******************************************************************************/
#ifndef ANI_TYPE_H
#define ANI_TYPE_H

namespace JE{
	enum PLAYER_ANI_TYPE
	{
		ANI_STAND = 0,
		ANI_RUNNING = 1,
		ANI_FALL = 2,
		ANI_DIGGING = 3,
		ANI_CANNON_JUMP = 4,
		ANI_DASH = 5,
		ANI_SUMMON = 6,
		ANI_FLYING = 7,
		ANI_CLAW = 8,
		ANI_END
	};
	
}//JE

#endif