/*******************************************************************************
Copyright (C) 2015 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
filename    JESpecPart.h
author      Jaeheon Lee
email       jhoney4344@gmail.com
course      GAM250
assignment  Final project submission
due date    12/6/2015

Brief Description:
This file is interface file for JESpecPart.cpp file.

*******************************************************************************/

#ifndef JENGA_ENGINE_SPECIAL_PART
#define JENGA_ENGINE_SPECIAL_PART

#include "JEBasePart.h"
#include "JEDayAndNight.h"

namespace JE{

	class FeatherWing : public BaseArm
	{
	public:
		FeatherWing(){
			SetPrice(40);
			SetConsumption(30);
			cost = 1; //???
		}
		~FeatherWing(){}
		void Flying();
	private:
	};

	class ClawArm : public BaseArm
	{
	public:
		ClawArm(){
			SetPrice(20);
			SetConsumption(20);
			cost = 1;
		}
		~ClawArm(){}
		void ClawAttack();
	private:
	};

	class LanternHead :public BaseHead
	{
	public:
		LanternHead()
		{
			SetPrice(30);
			SetConsumption(10);
		}
		~LanternHead(){}
		void LanternLight();
	private:
	};
	
	class RadioHead :public BaseHead
	{
	public:
		RadioHead()
		{
			SetPrice(20);
			SetConsumption(100);
		}
		~RadioHead(){}
		void Call();
	private:
	};

	class DashLeg : public BaseLeg
	{
	public:
		DashLeg()
		{
			SetPrice(10);
			SetConsumption(10);
		}
		~DashLeg(){}
		void Dash();
	private:
	};
	class JumpLeg : public BaseLeg
	{
	public:
		JumpLeg()
		{
			SetPrice(20);
			SetConsumption(20);
		}
		~JumpLeg(){}
		void CannonJump();
	private:
	};

	class BearBody : public BaseBody
	{
	public:
		BearBody(Resource* _resource = 0) : BaseBody(_resource)
		{
			SetMaxResource(Resource(400, 300, 50));
			SetCalorySpendRate(1);
			SetPrice(50);
		}
		~BearBody(){}
	private:
	};
	class GoodBody : public BaseBody
	{
	public:
		GoodBody(Resource* _resource = 0) : BaseBody(_resource)
		{
			SetMaxResource(Resource(250, 500, 50));
			SetCalorySpendRate(1);
			SetPrice(50);
		}
		~GoodBody(){}
	private:
	};
}
#endif