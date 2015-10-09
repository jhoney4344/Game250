/*******************************************************************************
Copyright (C) 2015 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior 
written consent of DigiPen Institute of Technology is prohibited.
filename    JEObjects.cpp
author      Jaeheon Lee, Nahye Park
email       jhoney4344@gmail.com, skqla12@gmail.com
course      GAM250
assignment  Final project submission
due date    12/6/2015

Brief Description:
The functions that managing objects.
Jaehoen made functions about basic objects.
Nahye made functions about enemy AI.

*******************************************************************************/
#include "JEObjects.h"
#include "JEObjectManager.h"
#include "JEInput.h"
#include "JESound.h"
#include "JETileMap.h"
#include "JEGraphicsNew.h"
#include "JECollisionManager.h"
#include "JEOldParticle.h"

#include "JECharacterManager.h"
#include "JECharacter.h"
namespace{
	int *objID = JE::ObjectManager::GetObjID();
	particleSystem* particleSys = &getParticleSystem();
	JE::Character* player1;
	JE::Character* player2;
}

namespace JE{
	/*******************************************************************************
	Function: SetCharacters_Obj

	Description: Set character player pointer.


	Inputs: None.


	Outputs: None.
	*******************************************************************************/
	void SetCharacters_Obj()
	{
		player1 = &CharacterManager::GetPlayerByID(PLAYER_1ST);
		player2 = &CharacterManager::GetPlayerByID(PLAYER_2ND);
	}
	/*******************************************************************************
	Function: CheckCollision

	Description: Collision check for objects.


	Inputs: None.


	Outputs: None.
	*******************************************************************************/
	void BaseObject::CheckCollision(void)
	{
		if (!applyGravity)
			return;

		//TILE_SPACE(10) : tile size
		TileMapManager* tileMapManager = &GetTileMapManager();
		Contents currTile[9];
		TilePosition object = ConvertWorldToTilePosition(position);
		JEPoint2D worldCharacter;
		JEPoint2D tile[9];
		TILE_POSITION tilePos[9];
		bool isCollided = false;

		tileMapManager->GetCollisionArea(object, currTile, 3);

		worldCharacter = ConvertTileToWorldPosition(object);
		worldCharacter.x -= TILE_SPACE / 2;
		worldCharacter.y -= TILE_SPACE / 2;

		g_Collision->CollisionAreaPos(tile, tilePos, worldCharacter, 3);

		for (int i = 0; i < 9; ++i)
		{

			//ground
			if (currTile[i].tileIDs == tileMapManager->tileIDs.dirtID || currTile[i].tileIDs == tileMapManager->tileIDs.stoneID
				|| currTile[i].tileIDs == tileMapManager->tileIDs.unbreakableTileID)
			{
				if (g_Collision->IntersectionCircleRect(position, collisionDistance, tile[i], TILE_SPACE, TILE_SPACE))
				{

					g_Collision->CollisionResponseWithTile(position, collisionDistance,
						tile[i], tilePos[i]);

					if (type == OBJ_ZOMBIE)
					{
						if (tilePos[i] == TL_RIGHT || tilePos[i] == TL_LEFT)
						{
							dynamic_cast <Zombie*>(this)->jump = true;

						}
					}

					if (type == OBJ_DEER)
					{
						if (tilePos[i] == TL_RIGHT || tilePos[i] == TL_LEFT)
						{
							speed.x *= -1;
						}
					}

					if (tilePos[i] == TL_DOWN)
					{
						isCollided = true;

						isOnGround = true;
						time = 0;
					}
				}
			}

		}

		if (!isCollided)
		{
			isOnGround = false;
		}
	}
	/*******************************************************************************
	Function: Bug

	Description: Constructor for bug.


	Inputs: pos - initioal position.


	Outputs: None.
	*******************************************************************************/
	Bug::Bug(JEPoint2D pos)
	{
		aniData.animationNum = 2;
		aniData.aniSecPerFrame = 0.5f;
		aniData.aniRateOfChange = 0.5f;
		aniData.aniTime = 0.5f;
		textureID = objID[OBJ_BUG];
		position = pos;
		imageGap = JEPoint2D(0, 0);
		scale.x = 10;
		scale.y = 10;
		applyGravity = true;
		collisionDistance = 5.0;
		type = OBJ_BUG;
	}
	/*******************************************************************************
	Function: Meat

	Description: Constructor for meat.


	Inputs: pos - initioal position.


	Outputs: None.
	*******************************************************************************/
	Meat::Meat(JEPoint2D pos)
	{

		textureID = objID[OBJ_MEAT];
		position = pos;
		imageGap = JEPoint2D(0, 0);
		scale.x = 7;
		scale.y = 7;
		applyGravity = true;
		collisionDistance = 5.0;
		type = OBJ_MEAT;

	}
	/*******************************************************************************
	Function: Mineral

	Description: Constructor for mineral.


	Inputs: pos - initioal position.


	Outputs: None.
	*******************************************************************************/
	Mineral::Mineral(JEPoint2D pos)
	{
		textureID = objID[OBJ_MINERAL];
		position = pos;
		imageGap = JEPoint2D(0, 0);
		scale.x = 10;
		scale.y = 10;
		applyGravity = true;
		collisionDistance = 5.0;
		type = OBJ_MINERAL;
	}
	/*******************************************************************************
	Function: Bread

	Description: Constructor for bread.


	Inputs: pos - initail position.


	Outputs: None.
	*******************************************************************************/
	Bread::Bread(JEPoint2D pos)
	{
		textureID = objID[OBJ_BREAD];
		position = pos;
		imageGap = JEPoint2D(0, 0);
		scale.x = 10;
		scale.y = 10;
		applyGravity = true;
		collisionDistance = 5.0;
		type = OBJ_BREAD;
	}
	/*******************************************************************************
	Function: ShortTree

	Description: Constructor for short tree.


	Inputs: pos - initail position.


	Outputs: None.
	*******************************************************************************/
	ShortTree::ShortTree(JEPoint2D pos)
	{
		textureID = objID[OBJ_SHORT_TREE];
		position = pos;

		imageGap = JEPoint2D();
		if (position.x > -10.f)
		{
			imageGap.x += TILE_SPACE;
			position.x -= TILE_SPACE;
		}

		if (position.y > -10.f)
		{
			imageGap.y += TILE_SPACE;
			position.y -= TILE_SPACE;
		}

		imageGap.x -= TILE_SPACE / 2;
		scale.x = TILE_SPACE;
		scale.y = TILE_SPACE * 2;
		HP = 3;
		type = OBJ_SHORT_TREE;
	}
	/*******************************************************************************
	Function: ~ShortTree

	Description: Destructor.


	Inputs: None.


	Outputs: None.
	*******************************************************************************/
	ShortTree::~ShortTree()
	{
		ObjectManager::AddObject(OBJ_BREAD, new Bread(position));
	}
	/*******************************************************************************
	Function: LongTree

	Description: Constructor for long tree.


	Inputs: pos - initial position.


	Outputs: None.
	*******************************************************************************/
	LongTree::LongTree(JEPoint2D pos)
	{
		textureID = objID[OBJ_LONG_TREE];
		position = pos;

		imageGap = JEPoint2D();
		if (position.x > -10.f)
		{
			imageGap.x += TILE_SPACE;
			position.x -= TILE_SPACE;
		}

		if (position.y > -10.f)
		{
			imageGap.y += TILE_SPACE;
			position.y -= TILE_SPACE;
		}

		imageGap.x -= TILE_SPACE / 2;
		imageGap.y += TILE_SPACE / 2;
		scale.x = TILE_SPACE;
		scale.y = TILE_SPACE * 4;
		HP = 5;
		type = OBJ_LONG_TREE;
	}
	/*******************************************************************************
	Function: ~LongTree

	Description: Destructor.


	Inputs: None.


	Outputs: None.
	*******************************************************************************/
	LongTree::~LongTree()
	{
		ObjectManager::AddObject(OBJ_BREAD, new Bread(position));
	}
	/*******************************************************************************
	Function: Stone

	Description: Constructor for stone.


	Inputs: pos - initail position.


	Outputs: None.
	*******************************************************************************/
	Stone::Stone(JEPoint2D pos)
	{
		textureID = 1;
		position = pos;
		imageGap = JEPoint2D(0, 0);
		type = OBJ_STONE;
	}
	/*******************************************************************************
	Function: Enemy

	Description: Constructor for Enemy.


	Inputs: pos - initial position.


	Outputs: None.
	*******************************************************************************/
	Enemy::Enemy(JEPoint2D pos)
	{
		position = pos;
		imageGap = JEPoint2D(0, 0);
		enemyTime = 0;
		enemyFrame = 0;
		speed.x = 1.5;
		speed.y = 1.5;
		HP = 3;
		type = OBJ_ENEMY;
	}
	/*******************************************************************************
	Function: Shop

	Description: Constructor for shop.


	Inputs: pos - initial position.


	Outputs: None.
	*******************************************************************************/
	Shop::Shop(JEPoint2D pos)
	{
		textureID = objID[OBJ_SHOP];
		position = pos;

		imageGap = JEPoint2D();
		if (position.x > -10.f)
		{
			imageGap.x += TILE_SPACE;
			position.x -= TILE_SPACE;
		}
		imageGap.x -= TILE_SPACE / 2;
		imageGap.y += TILE_SPACE / 5;

		scale.x = 18;
		scale.y = 18;
		applyGravity = true;
		collisionDistance = 5.0;
		type = OBJ_SHOP;
		
	}
	/*******************************************************************************
	Function: Tomb

	Description: Constructor for tomb.


	Inputs: pos - initial position.


	Outputs: None.
	*******************************************************************************/
	Tomb::Tomb(JEPoint2D pos)
	{
		active = true;
		textureID = objID[OBJ_TOMB];
		position = pos;
		imageGap = JEPoint2D(0, 0);
		scale.x = 10;
		scale.y = 10;
		type = OBJ_TOMB;
		applyGravity = true;
		collisionDistance = 5.0;

		m_flyEmit = 0;
	}
	/*******************************************************************************
	Function: ~Tomb

	Description: Denstructor for tomb.


	Inputs: None.


	Outputs: None.
	*******************************************************************************/
	Tomb::~Tomb()
	{
		ObjectManager::AddObject(OBJ_ZOMBIE,new Zombie(position));
	}
	/*******************************************************************************
	Function: Zombie

	Description: Constructor for zombie.


	Inputs: pos - initial position.


	Outputs: None.
	*******************************************************************************/
	Zombie::Zombie(JEPoint2D pos)
	{
		int* boneID = ObjectManager::GetBoneID(OBJ_ZOMBIE);
		position = pos;
		bone = BoneInit(bone, "IniFile//Zombie.ini");
		BoneSetTextureID(bone, BODY_NAME::word[PN_HEAD], boneID[PN_HEAD]);
		BoneSetTextureID(bone, BODY_NAME::word[PN_BODY], boneID[PN_BODY]);
		BoneSetTextureID(bone, BODY_NAME::word[PN_LEFTSHOULDER], objID[OBJ_EMPTY]);
		BoneSetTextureID(bone, BODY_NAME::word[PN_RIGHTSHOULDER], objID[OBJ_EMPTY]);
		BoneSetTextureID(bone, BODY_NAME::word[PN_LEFTHIP], objID[OBJ_EMPTY]);
		BoneSetTextureID(bone, BODY_NAME::word[PN_RIGHTHIP], objID[OBJ_EMPTY]);
		BoneSetTextureID(bone, BODY_NAME::word[PN_LEFTARM1], boneID[PN_LEFTARM1]);
		BoneSetTextureID(bone, BODY_NAME::word[PN_LEFTARM2], boneID[PN_LEFTARM2]);
		BoneSetTextureID(bone, BODY_NAME::word[PN_RIGHTARM1], boneID[PN_RIGHTARM1]);
		BoneSetTextureID(bone, BODY_NAME::word[PN_RIGHTARM2], boneID[PN_RIGHTARM2]);
		BoneSetTextureID(bone, BODY_NAME::word[PN_LEFTLEG1], boneID[PN_LEFTLEG1]);
		BoneSetTextureID(bone, BODY_NAME::word[PN_LEFTLEG2], boneID[PN_LEFTLEG2]);
		BoneSetTextureID(bone, BODY_NAME::word[PN_RIGHTLEG1], boneID[PN_RIGHTLEG1]);
		BoneSetTextureID(bone, BODY_NAME::word[PN_RIGHTLEG2], boneID[PN_RIGHTLEG2]);
		AniSystem::AddAni(bone, acting);
		AniSystem::SetAni(bone, 0, true);
		applyGravity = true;
		collisionDistance = 4.0;
		type = OBJ_ZOMBIE;
		checkTime = 0;
		jump = false;

		if (!player1->live1 && player2->live2)
			playerPos = player2->GetPosition();
		else if (!player2->live2 && player1->live1)
			playerPos = player1->GetPosition();
		else
		{

			if (g_Math->DistancePointPoint(JEPoint2D(position.x, 0), JEPoint2D(player1->GetProperty().position.x, 0))
				< g_Math->DistancePointPoint(JEPoint2D(position.x, 0), JEPoint2D(player2->GetProperty().position.x, 0)))
				playerPos = player1->GetPosition();
			else
				playerPos = player2->GetPosition();
		}
		JEPoint2D direction = g_Math->Normalize(JEPoint2D((playerPos.x - position.x), 0));

		speed = JEPoint2D(direction.x * 15, 40);

		HP = 5;
	}
	/*******************************************************************************
	Function: DefaultMoving

	Description: Default movement for zombie.


	Inputs: pSelf - pointer to base object.
			dt - for moving.


	Outputs: None.
	*******************************************************************************/
	void Zombie::DefaultMoving(BaseObject* pSelf, double dt)
	{
		checkTime += dt;
		JEPoint2D playerPosition;
		if (speed.x > 0 && flip == true)
		{
			flip = false;
			BoneReverse(bone);
		}
		else if (speed.x < 0 && flip == false)
		{
			flip = true;
			BoneReverse(bone);
		}
		if (checkTime > 1)
		{
			if (!player1->live1 && player2->live2)
				playerPosition = player2->GetPosition();
			else if (!player2->live2 && player1->live1)
				playerPosition = player1->GetPosition();
			else
			{

				if (g_Math->DistancePointPoint(JEPoint2D(position.x, 0), JEPoint2D(player1->GetProperty().position.x, 0))
					< g_Math->DistancePointPoint(JEPoint2D(position.x, 0), JEPoint2D(player2->GetProperty().position.x, 0)))
					playerPosition = player1->GetPosition();
				else
					playerPosition = player2->GetPosition();
			}

			JEPoint2D direction = g_Math->Normalize(JEPoint2D((playerPosition.x - pSelf->position.x), 0));

			speed = JEPoint2D(direction.x * 15, speed.y);

			checkTime = 0;
		}
		pSelf->position.x += speed.x*dt;

	}
	/*******************************************************************************
	Function: Jump

	Description: Jump.


	Inputs: dt - for moving.


	Outputs: None.
	*******************************************************************************/
	void Zombie::Jump(double dt)
	{
		position.y += speed.y*dt;
	}
	/*******************************************************************************
	Function: Deer

	Description: Constructor for deer.


	Inputs: pos - initial position.


	Outputs: None.
	*******************************************************************************/
	Deer::Deer(JEPoint2D pos)
	{
		int* boneID = ObjectManager::GetBoneID(OBJ_DEER);
		position = pos;
		bone = BoneInit(bone, "IniFile//Deer.ini");
		BoneSetTextureID(bone, BODY_NAME::word[PN_HEAD], boneID[PN_HEAD]);
		BoneSetTextureID(bone, BODY_NAME::word[PN_BODY], boneID[PN_BODY]);
		BoneSetTextureID(bone, BODY_NAME::word[PN_LEFTSHOULDER], objID[OBJ_EMPTY]);
		BoneSetTextureID(bone, BODY_NAME::word[PN_RIGHTSHOULDER], objID[OBJ_EMPTY]);
		BoneSetTextureID(bone, BODY_NAME::word[PN_LEFTHIP], objID[OBJ_EMPTY]);
		BoneSetTextureID(bone, BODY_NAME::word[PN_RIGHTHIP], objID[OBJ_EMPTY]);
		BoneSetTextureID(bone, BODY_NAME::word[PN_LEFTARM1], boneID[PN_LEFTLEG1]);
		BoneSetTextureID(bone, BODY_NAME::word[PN_LEFTARM2], boneID[PN_LEFTLEG2]);
		BoneSetTextureID(bone, BODY_NAME::word[PN_RIGHTARM1], boneID[PN_LEFTLEG1]);
		BoneSetTextureID(bone, BODY_NAME::word[PN_RIGHTARM2], boneID[PN_LEFTLEG2]);
		BoneSetTextureID(bone, BODY_NAME::word[PN_LEFTLEG1], boneID[PN_LEFTLEG1]);
		BoneSetTextureID(bone, BODY_NAME::word[PN_LEFTLEG2], boneID[PN_LEFTLEG2]);
		BoneSetTextureID(bone, BODY_NAME::word[PN_RIGHTLEG1], boneID[PN_LEFTLEG1]);
		BoneSetTextureID(bone, BODY_NAME::word[PN_RIGHTLEG2], boneID[PN_LEFTLEG2]);
		BoneSetTextureID(bone, BODY_NAME::word[PN_TAIL1], boneID[PN_TAIL1]);
		BoneSetTextureID(bone, BODY_NAME::word[PN_TAIL2], objID[OBJ_EMPTY]);
		BoneSetTextureID(bone, BODY_NAME::word[PN_TAIL3], objID[OBJ_EMPTY]);
		AniSystem::AddAni(bone, acting);
		AniSystem::SetAni(bone,0,true);
		applyGravity = true;
		collisionDistance = 5.0;
		type = OBJ_DEER;
		HP = 2;
		checkTime = 0;

		if (flip) //left
			collisionPos = JEPoint2D(position.x - collisionDistance / 2, position.y);
		else //right
			collisionPos = JEPoint2D(position.x + collisionDistance / 2, position.y);

		speed = JEPoint2D((playerPos.x - position.x) * 0.01, 50);
		int random = rand() % 2;
		if (random == 0)
			speed.x = 10 ;
		else
			speed.x = -10;
	}
	/*******************************************************************************
	Function: DefaultMoving

	Description: DefaultMoving for deer.


	Inputs: pSelf - pointer to base object.
	        dt - for moving


	Outputs: None.
	*******************************************************************************/
	void Deer::DefaultMoving(BaseObject* pSelf,double dt)
	{
		if (speed.x > 0 && flip == true)
		{
			flip = false;
			BoneReverse(bone);
		}
		else if (speed.x < 0 && flip == false)
		{
			flip = true;
			BoneReverse(bone);
		}
		checkTime += dt;
		JEPoint2D playerPosition;
		
		if (checkTime > 0.2)
		{
			jump = true;
			checkTime = 0;
		}

		pSelf->position.x += speed.x*dt;

	}
	/*******************************************************************************
	Function: ~Deer

	Description: Destructor.


	Inputs: None.


	Outputs: None.
	*******************************************************************************/
	Deer::~Deer()
	{
		if (HP < 1)
		{
			ObjectManager::AddObject(OBJ_TOMB, new Tomb(position));
			ObjectManager::AddObject(OBJ_MEAT, new Meat(position));
		}
	}
	/*******************************************************************************
	Function: Jump

	Description: Jump.


	Inputs: dt - for moving.


	Outputs: None.
	*******************************************************************************/
	void Deer::Jump(double dt)
	{
		position.y += speed.y*dt;
	}
	
	
}//JE

