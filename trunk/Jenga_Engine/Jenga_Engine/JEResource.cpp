/*******************************************************************************
Copyright (C) 2015 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
filename    JEResource.cpp
author      Jaeheon Lee
email       jhoney4344@gmail.com
course      GAM250
assignment  Final project submission
due date    12/6/2015

Brief Description:
This file is for managing resource of game.

*******************************************************************************/

#include "JEResource.h"
#include "JEGraphicsNew.h"
#include "JELoadTextureNew.h"

namespace{
	JE::ResourceID resourceID;
	int crystalID;
}

namespace JE{
	namespace ResourceSystem{
		/*******************************************************************************
		Function: Load

		Description: Load for resource texture.


		Inputs: None.


		Outputs: None.
		*******************************************************************************/
		void Load(void)
		{
			resourceID.bread = GraphicLoadTexture("Texture//Resource_Bread.tga");
			resourceID.meat = GraphicLoadTexture("Texture//Resource_Meat.tga");
			resourceID.mineral = GraphicLoadTexture("Texture//Resource_Mineral.tga");
			crystalID = GraphicLoadTexture("Texture//Crystal.tga");
		}
		/*******************************************************************************
		Function: Unload

		Description: Unload the texture.


		Inputs: None.


		Outputs: None.
		*******************************************************************************/
		void Unload(void)
		{
			GraphicUnloadTexture(resourceID.bread);
			GraphicUnloadTexture(resourceID.meat);
			GraphicUnloadTexture(resourceID.mineral);
			GraphicUnloadTexture(crystalID);
		}
		/*******************************************************************************
		Function: GetResourceID

		Description: Return the resource ID.


		Inputs: None.


		Outputs: Resource ID.
		*******************************************************************************/
		ResourceID& GetResourceID(void)
		{
			return resourceID;
		}
		/*******************************************************************************
		Function: GetCrystalID

		Description: Return crystal ID.


		Inputs: None.


		Outputs: Crystal ID.
		*******************************************************************************/
		int GetCrystalID()
		{
			return crystalID;
		}
	}//HUD
}//JE