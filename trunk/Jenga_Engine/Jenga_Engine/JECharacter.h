/*******************************************************************************
Copyright (C) 2015 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
filename    JECharacter.h
author      Jaeheon Lee, Nahye Park
email       jhoney4344@gmail.com, skqla12@gmail.com
course      GAM250
assignment  Final project submission
due date    12/6/2015

Brief Description:
This file is interface file for JECharacter.cpp file.
Jaehoen made functions about body parts of character.
Nahye made functions about collision of character and basic skill for character.

*******************************************************************************/

#ifndef JENGA_ENGINE_CHARACTER_NEW
#define JENGA_ENGINE_CHARACTER_NEW

#include "JEMath.h"
#include "JEGraphicsNew.h"
#include "JEIniFileManager.h"
#include "JEObjects.h"
#include "JESpecPart.h"
#include "JEParts.h"
#include "JEResource.h"
#include "JECharacterType.h"
#include "JEDayAndNight.h"


namespace JE{
	struct Property
	{
		bool autoPlay;

		float textureID;
		JEVector2D scale;
		JEPoint2D position;
		JEPoint2D movingDirection;
		
		bool isOnGround;
		double collisionDistance;
		double gravity;
		JEVector2D speed;
		JEPoint2D enemyVec;
		JEPoint2D wingInit;
		bool flip;
		bool acting;
		bool jump;
		bool knockBack;
		
		double time; // when player's position is in the air
		double action_time;
		double knockback_distance;
		double flying_distance;
		double auto_time;

		bool headAction;
		bool armAction;
		bool legAction;

		bool wingAction;

		bool blockmoving;

		bool blockCollision;
		Vector3f direction;
	};


	class Character
	{
	public:
		Character(CHARACTER_ID identity);
		~Character();
		void clear_part();
		void ChangeParts(BODY_PART,PART_TYPE);
		void UseSkill(BODY_PART);
		bool IsOnGround();
		void SetPosition(JEPoint2D newPos);
		void Jump();
		void Digging();
		
		void CharacterCollision(Character* player);
		void CollisionCheck(void);
		bool CheckTreeCollision(int tilePosX, int tilePosY);
		bool CheckEnemyCollision(int tilePosX, int tilePosY);
		void SkillUpdate();
		void AutoPlay(Character* player);

		void Character::SetBone(Bone* _bone);
		Bone* GetBone();
		BaseBody* GetBody();
		PART_TYPE GetPartType(BODY_PART);
		Property& GetProperty();
		JEPoint2D GetPosition();
		Resource& GetResource();
		void set_dt(double _dt);

		double get_dt() const;
		CHARACTER_ID get_id() const;
		static int get_character_num();
		static bool live1;
		static bool live2;
	private:

		void ChangeHead(PART_TYPE partType);
		void ChangeArm(PART_TYPE partType);
		void ChangeLeg(PART_TYPE partType);
		void ChangeBody(PART_TYPE partType);
		
		void HeadSkill();
		void ArmSkill();
		void LegSkill();

		
		Resource resource;
		Property property;
		double dt;
		CHARACTER_ID ID;
		
		static int character_num;
		

		template<typename T>
		struct PartData
		{
			PartData() : pPart(0){}
			T* pPart;
			PART_TYPE partType;
		};

		Bone* bone;
		PartData<BaseHead> head;
		PartData<BaseBody> body;
		PartData<BaseArm> arm;
		PartData<BaseLeg> leg;
		
	};


}

#endif
