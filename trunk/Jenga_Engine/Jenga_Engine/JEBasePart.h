/*******************************************************************************
Copyright (C) 2015 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
filename    JEBasePart.h
author      Jaeheon Lee
email       jhoney4344@gmail.com
course      GAM250
assignment  Final project submission
due date    12/6/2015

Brief Description:
This file is for base parts of character.

*******************************************************************************/

#ifndef JENGA_ENGINE_PART_NEW
#define JENGA_ENGINE_PART_NEW
#include "JEResource.h"
#include "JEInput.h"
#include "JEResource.h"
#include "JEPad.h"
#include "JECharacterType.h"
#include "JEGraphicsNew.h"

namespace JE{
	class BodyPart
	{
	public:
		BodyPart() :price(0), cost(1){}
		virtual ~BodyPart(){}
		void SetPrice(int _price){ price = _price; }
		void SetConsumption(double _consumption){ consumption = _consumption; }
		int GetPrice(){ return price; }
		double GetConsumption(){ return consumption; }
		CHARACTER_ID playerID;
		int cost;
	private:
		int price; // purchase price
		double consumption; // mineral consumption
	};
	class BaseHead : public BodyPart
	{
	public:
		BaseHead(){}
		~BaseHead(){}
	private:
	};

	class BaseArm : public BodyPart
	{
	public:
		BaseArm(){}
		~BaseArm(){}
	private:
	};

	class BaseLeg : public BodyPart
	{
	public:
		BaseLeg(){}
		~BaseLeg(){}
	private:
	};

	class BaseBody : public BodyPart
	{
	public:
		BaseBody(Resource* _resource = 0) :resource(_resource){}
		~BaseBody(){}
		Resource& GetResource(){ return *resource; }
		void AddMeat(int meat = 1) 
		{ 
				resource->meat += meat; 
				if (resource->meat > max_resource.meat)
					resource->meat = max_resource.meat;
		}
		void AddBread(int bread = 1)
		{ 
			resource->bread += bread; 
			if (resource->bread > max_resource.bread)
				resource->bread = max_resource.bread;
		}
		void AddMineral(double mineral = 1.0)
		{
			resource->mineral += mineral; 
			if (resource->mineral > max_resource.mineral)
				resource->mineral = max_resource.mineral;
		}
		void SetResource(const Resource& _resource){ *resource = _resource; }
		void SetMaxResource(const Resource& maxResource){ max_resource = maxResource; }
		Resource& GetMaxResource(void){ return max_resource; }
		void SetCalorySpendRate(double spendRate = 1) { burningRate = spendRate; }
		void BurningCalory(double dt) 
		{ 
			resource->bread -= dt*burningRate*3;
			if (resource->bread <= 0)
				resource->bread = 0;
		}
	private:
		Resource* resource;
		Resource max_resource;
		double burningRate; // burning bread every time
	};
	class BasicBody : public BaseBody
	{
	public:
		BasicBody(Resource* _resource = 0) :BaseBody(_resource)
		{
			SetMaxResource(Resource(100,100,50));
			SetResource(Resource(100,0,0));
			SetCalorySpendRate(1);
		}
		~BasicBody(){}
	private:
	};
	class BasicHead : public BaseHead
	{
	public:
		~BasicHead(){}
	private:
	};

	class BasicArm : public BaseArm
	{
	public:
		~BasicArm(){}
	private:
	};

	class BasicLeg : public BaseLeg
	{
	public:
		~BasicLeg(){}
	private:
	};

}//JE
#endif