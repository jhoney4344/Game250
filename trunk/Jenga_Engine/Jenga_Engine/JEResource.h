/*******************************************************************************
Copyright (C) 2015 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
filename    JEResource.h
author      Jaeheon Lee
email       jhoney4344@gmail.com
course      GAM250
assignment  Final project submission
due date    12/6/2015

Brief Description:
This file is interface file for JEResource.cpp file.

*******************************************************************************/

#ifndef JENGA_RESOURCE_H
#define JENGA_RESOURCE_H

namespace JE{

	struct Resource
	{
		Resource(double br = 0, double mi = 0, int me = 0)
			: bread(br), mineral(mi), meat(me){}

		double bread;
		int meat;
		double mineral;
	};
	struct ResourceID
	{
		int bread;
		int meat;
		int mineral;
	};
	namespace ResourceSystem{

		void Load(void);
		void Unload(void);
		ResourceID& GetResourceID(void);
		int GetCrystalID();
	}// RESOURCE
}//JE

#endif