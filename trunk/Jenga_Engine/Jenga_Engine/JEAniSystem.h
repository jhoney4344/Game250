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
  Ani system for running animation

*******************************************************************************/
#ifndef JENGA_ENGINE_ANIMATION_SYSTEM
#define JENGA_ENGINE_ANIMATION_SYSTEM

//#define OLD_ANI_SYSTEM

#include "JECharacter.h"
#include "JEGraphicsNew.h"
//#include "JEPartsNew.h"
#include "JEAniType.h"

namespace JE{

class Animator
{
public:
	Animator() :n_th_frame(0) {}

	void Set_Frame(int n_th)
	{
		n_th_frame = n_th;
	}

	int& operator++(void)
	{
		n_th_frame++;
		return n_th_frame;
	}

	int operator++(int)
	{
		int oldFrame = n_th_frame;
		operator++();
		return oldFrame;
	}

private:
	int n_th_frame;
};



namespace AniSystem{
	void AddAni(Bone *pBone, bool &acting);
	void Init();
	void Update();
	void ShutDown();
	void SetAni(Bone* id, int type, bool iterative);
	int GetAniType(Bone* id);
	void StopAni(Bone* id);
	bool IsFalling(Bone* id);
}//ANI_SYSTEM

}//JE

#endif