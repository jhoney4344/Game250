/*******************************************************************************
Copyright (C) 2015 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
filename    JEAniType.cpp
author      Jaeheon Lee
email       jhoney4344@gmail.com
course      GAM250
assignment  Final Submission
due date    12/6/2015

Brief Description:
Ani system for running animation

*******************************************************************************/
#include "JEAniSystem.h"  
#include "JECharacterManager.h"
#include "JESTL.h"
//#define OLD_ANI_SYSTEM
namespace JE{
	namespace AniSystem{
namespace{
	struct BoneData{
		BoneData(JE::Bone* _bone = 0, bool* _acting = 0)
			: repeat(false), changeAni(false), firstPlay(false), acting(_acting)
		{
			bone = _bone;
			currAni = -1;
		}
		~BoneData()
		{
			bone = 0;
			acting = 0;
		}
		JE::Bone *bone;
		bool repeat;
		bool changeAni;
		bool firstPlay;
		bool *acting;
		int currAni;
		JE::Animator animator;
	};
	std::list<BoneData> bone_list;
	typedef std::list<BoneData>::iterator BL_Itor;
}

/*******************************************************************************
Function: AddAni

Description: pBone - the bone data from object
             acting - check animation running

Inputs: None.

Outputs: None.
*******************************************************************************/
void AddAni(Bone *pBone, bool &acting)
{
	bone_list.push_back(BoneData(pBone, &acting));
}
/*******************************************************************************
Function: Remove

Description: pBone - the bone data from object

Inputs: None.

Outputs: None.
*******************************************************************************/
void Remove(Bone *pBone)
{
	BL_Itor it = bone_list.begin();
	while (it != bone_list.end())
	{
		if ((*it).bone == pBone)
			break;
		it++;
	}
	if (it == bone_list.end())
		return;
	bone_list.erase(it);
}
/*******************************************************************************
Function: Init

Description: Do nothing

Inputs: None.

Outputs: None.
*******************************************************************************/
void Init(){}
/*******************************************************************************
Function: Update

Description: This function set the application option.

Inputs: None.

Outputs: None.
*******************************************************************************/
void Update()
{
	for (BL_Itor it = bone_list.begin(); it != bone_list.end(); it++)
	{
		if ((*it).changeAni == true)
		{
			BoneEndAnimate((*it).bone);
			(*it).animator.Set_Frame(0);
			(*it).changeAni = false;
		}

		if ((*it).firstPlay)
		{
			*(*it).acting = true;
			if (!BoneTriggerAnimate((*it).bone, ((*it).animator)++, (*it).currAni, (*it).firstPlay))
				(*it).animator.Set_Frame(0);
		}
		else if (!(*it).firstPlay && (*it).repeat == true)
		{
			if (!BonePressAnimate((*it).bone, ((*it).animator)++, (*it).currAni))
				(*it).animator.Set_Frame(0);
			//*acting[id] = false;
		}
	}//for
}//Update
/*******************************************************************************
Function: ShutDonw

Description: it should be done before erasing objects

Inputs: None.

Outputs: None.
*******************************************************************************/
void ShutDown()
{
	while (bone_list.size())
		bone_list.pop_back();
}//ShutDown
/*******************************************************************************
Function: StopAni

Description: Stop the current animation

Inputs: pBone - the bone data from object

Outputs: None.
*******************************************************************************/
void StopAni(Bone* pBone)
{
	BL_Itor it = bone_list.begin();
	while (it != bone_list.end())
	{
		if ((*it).bone == pBone)
			break;
		it++;
	}
	if (it == bone_list.end())
		return;
	(*it).firstPlay = false;
}// Stop ani
/*******************************************************************************
Function: IsFalling

Description: Check the falling object and running animation

Inputs: pBone - the bone data from object

Outputs: None.
*******************************************************************************/
bool IsFalling(Bone* pBone)
{
	BL_Itor it = bone_list.begin();
	while (it != bone_list.end())
	{
		if ((*it).bone == pBone)
			break;
		it++;
	}
	if (it == bone_list.end())
		return false;
	if ((*it).currAni == ANI_FALL)
		return false;
	else
		return true;
}// Is Falling
/*******************************************************************************
Function: none reverse search is only for characters.

Description: change the animaiton type

Inputs: pBone - the bone data from object
        type - animation type
		interative - repeating animation checker

Outputs: None.
*******************************************************************************/

void SetAni(Bone* pBone, int type, bool iterative)// Is that a proper position for default value?
{
	BL_Itor it = bone_list.begin();
	while (it != bone_list.end())
	{
		if ((*it).bone == pBone)
			break;
		it++;
	}
	if (it == bone_list.end())
		return;

	*(*it).acting = true; // make true and change into false in the update, for making idle state
	(*it).repeat = iterative;

	// not repeated ani
	if (!(*it).repeat)
	{
		if ((*it).currAni == type)// animation play once
		{
			return;
		}
		else if ((*it).currAni != type)
		{
			(*it).changeAni = true;
			(*it).currAni = type;
			(*it).firstPlay = true;
			return;
		}
	}
	else if ((*it).repeat) // repeatedly play
	{
		if ((*it).firstPlay)
			return;
		if ((*it).currAni == type) // keep current animation
		{
			return;
		}
		else if ((*it).currAni != type)// change new repeated animation
		{
 			(*it).changeAni = true;
			(*it).currAni = type;
			return;
		}
	}
}//SetAni


	}//ANI_SYSTEM
}//JE