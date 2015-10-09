/*******************************************************************************
Copyright (C) 2015 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior 
written consent of DigiPen Institute of Technology is prohibited.
filename    JEObjects.h
author      Jaeheon Lee
email       jhoney4344@gmail.com
course      GAM250
assignment  Final project submission
due date    12/6/2015

Brief Description:
  This file is interface file for JEObjects.cpp file.

*******************************************************************************/
#ifndef JENGA_ENGIN_OBJECT_H
#define JENGA_ENGIN_OBJECT_H

//#define OBJECT_WITH_BONE
//#define OBJECT_WITHOUT_BONE

#include "JESTL.h"
#include "JEMath.h"
#include "JEObjectTypes.h"
#include "JEParts.h"
#include "JETileMapStructs.h"
#include "JEIniFileManager.h"
#include "JEGraphicsNew.h"
#include "JEResource.h"
#include "JEAniSystem.h"
#include "JEOldParticle.h"

namespace JE{
	void SetCharacters_Obj();
	struct BaseObject
	{
		BaseObject()
			: gravity(0), remove(false), drawBase(DRAW_END), applyGravity(false)
			  ,collisionDistance(0), isOnGround(false), time(0), type(OBJ_BEGINE){}
		virtual ~BaseObject(){};
		void CheckCollision(void);

		JEPoint2D position;
		JEPoint2D imageGap;
		JEPoint2D speed;
		JEVector2D scale;

		//physics
		double gravity;
		double time;
		bool applyGravity;
		double collisionDistance;
		bool isOnGround;

		bool remove;
		DRAW_BASE drawBase;

		TilePosition prevTilePosition;

		OBJECTS type;
	};

	struct BoneBase : virtual BaseObject
	{
		BoneBase()
		{
			acting = false;
			drawBase = DRAW_BONE;
			flip = false;
		}
		Bone * bone;
		bool acting;
		bool flip;
	};

	struct TextBase : virtual BaseObject
	{
		TextBase()
		{
			drawBase = DRAW_TEXT;
		}
		struct AniData
		{
			AniData(){
				animationNum = 1;
				aniRateOfChange = 0;
				aniSecPerFrame = 0;
				aniTime = 0;
				aniTimeStack = 0;
				
			}
			float animationNum;
			float aniRateOfChange;
			float aniSecPerFrame;
			float aniTime;
			float aniTimeStack;
			
		};
		
		AniData aniData;
		int textureID;
	};

	struct Living
	{
		Living(int newHP = 0) : HP(newHP){}
		int GetHP(){ return HP; }
		void SetHP(int newHP){ HP = newHP; }
		int HP;
	};

	struct Eatable
	{
		Eatable() : eaten(false), protectTime(1), whoEat(-1), playerTouch(false),
			settedWhoEat(false){}
		void SetEaten(bool _eaten, int player_id)
		{
			eaten = _eaten;
			whoEat = player_id;
		}
		~Eatable(){};
		Resource resource;
		double protectTime;
		bool eaten;
		bool settedWhoEat;
		int whoEat;
		bool playerTouch;
	};

	struct Bug : Eatable, Living , virtual TextBase
	{
		Bug(JEPoint2D pos = JEPoint2D(0, 0));
		~Bug(){}
	};

	struct Meat : Eatable, virtual TextBase
	{
		Meat(JEPoint2D pos = JEPoint2D(0, 0));
		~Meat(){}
	};
	struct Bread : Eatable, virtual TextBase
	{
		Bread(JEPoint2D pos = JEPoint2D(0, 0));
		~Bread(){}
	};
	struct Mineral : Eatable, virtual TextBase
	{
		Mineral(JEPoint2D pos = JEPoint2D(0, 0));
		~Mineral(){}
	};

	struct ShortTree : Living, virtual TextBase
	{
		ShortTree(JEPoint2D pos = JEPoint2D(0, 0));
		~ShortTree();
	};

	struct LongTree : Living , virtual TextBase
	{
		LongTree(JEPoint2D pos = JEPoint2D(0, 0));
		~LongTree();
	};

	struct Stone : TextBase
	{
		Stone(JEPoint2D pos = JEPoint2D(0, 0));
		~Stone(){}
	};
	void SetEnemyID(int newID);

	struct Enemy : Living , virtual BoneBase
	{
		Enemy(JEPoint2D pos = JEPoint2D(0, 0));
		~Enemy(){}

		float enemyTime;
		float enemyFrame;
	};
	struct Shop : virtual TextBase
	{
		Shop(JEPoint2D pos = JEPoint2D(0, 0));
		~Shop(){}
	};
	struct Tomb :virtual TextBase
	{
		Tomb(JEPoint2D pos = JEPoint2D(0, 0));
		~Tomb();

		bool active;
		//ask Seho
		emitter* m_flyEmit;
	};
	struct Zombie : Living, virtual BoneBase
	{
		Zombie(JEPoint2D pos = JEPoint2D(0, 0));
		~Zombie(){}

		JEPoint2D knockVec;
		bool knockBack;
		double knockback_distance;

		//Moving
		void DefaultMoving(BaseObject* pSelf, double dt);
		JEPoint2D playerPos;
		double checkTime;

		bool jump;
		void Jump(double dt);
	};
	struct Deer : Living, virtual BoneBase
	{
		Deer(JEPoint2D pos = JEPoint2D(0, 0));
		~Deer();

		JEPoint2D collisionPos;

		JEPoint2D knockVec;
		bool knockBack;
		double knockback_distance;
		void DefaultMoving(BaseObject* pSelf, double dt);
		JEPoint2D playerPos;
		double checkTime;

		bool jump;
		void Jump(double dt);
	};

}// JE
#endif // JE_OBJECT_H