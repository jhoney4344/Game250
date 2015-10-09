/*******************************************************************************
Copyright (C) 2015 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
filename    JESpecPart.cpp
author      Jaeheon Lee, Nahye Park
email       jhoney4344@gmail.com, skqla12@gmail.com
course      GAM250
assignment  Final project submission
due date    12/6/2015

Brief Description:
This file is for managing specific parts.
Jaehoen made outline of functions.
Nahye implemented the skills for each parts.

*******************************************************************************/

#include "JESpecPart.h"
#include "JEAniSystem.h"
#include "JESound.h"
#include "JECharacterManager.h"
#include "JECollisionManager.h"
#include "JETileMap.h"
#include "JEGraphicsNew.h"
#include "JEDayAndNight.h"
#include "JESound.h"


namespace
{
	bool turnOn = false;
	JE::DayNight* pDayNight;
}

namespace JE{
	void ClawArm::ClawAttack()
	{
		Character* player = &CharacterManager::GetPlayerByID(playerID);
		Property* property = &player->GetProperty();
		JEPRINTF("Claw attack !!!\n");

		if (!property->armAction)
		{
			return;
		}

		property->blockCollision = true;
		
		if (property->action_time < 1.3)
		{

			TileMapManager* tileMapManager = &GetTileMapManager();
			Contents currTile[9];
			TilePosition character;
			JEPoint2D worldCharacter;
			JEPoint2D tile[9];
			TilePosition pos[9];
			TILE_POSITION tilePos[9];
			int tileSize = 3;

			if (player->get_id() == PLAYER_1ST)
				character = tileMapManager->Get1stCharacterTilePosition();
			else if (player->get_id() == PLAYER_2ND)
				character = tileMapManager->Get2ndCharacterTilePosition();

			tileMapManager->GetCollisionArea(character, currTile, tileSize);

			worldCharacter = ConvertTileToWorldPosition(character);
			worldCharacter.x -= TILE_SPACE / 2;
			worldCharacter.y -= TILE_SPACE / 2;

			g_Collision->CollisionAreaPos(tile, tilePos, worldCharacter, tileSize);

			player->CheckEnemyCollision(character.x, character.y);
			player->CheckEnemyCollision(character.x, character.y + 1);
			player->CheckEnemyCollision(character.x, character.y - 1);
			

			if (property->flip)//left enemy
			{
				player->CheckEnemyCollision(character.x - 1, character.y);
			

			}
			else if (!property->flip)//right enemy
			{
				player->CheckEnemyCollision(character.x + 1, character.y);
					
			}
			

		}
		else
		{
			property->armAction = false;
			property->blockCollision = false;
			property->action_time = 0;

			return;
		}

		
		property->action_time += player->get_dt();
		
		
	}

	void DashLeg::Dash()
	{
		Character* player = &CharacterManager::GetPlayerByID(playerID);
		Property* property = &player->GetProperty();

		if (!property->legAction)
		{
			return;
		}


		if (property->action_time < 0.4)
		{

			if ((g_Input->IsPressed(JE_ARROW_LEFT) && playerID == PLAYER_1ST)
				|| (!MyGamepad->LStick_InDeadzone() && (MyGamepad->LeftStick_X() < -2.0f || MyGamepad->GetButtonPressed(XButtons.DPad_Left)) && playerID == PLAYER_2ND))//left
				property->position.x -= property->speed.x*player->get_dt()*property->time + (property->time*property->time) / 2;

			else if ((g_Input->IsPressed(JE_ARROW_RIGHT) && playerID == PLAYER_1ST)
				|| (!MyGamepad->LStick_InDeadzone() && (MyGamepad->LeftStick_X() > 2.0f || MyGamepad->GetButtonPressed(XButtons.DPad_Right)) && playerID == PLAYER_2ND))//right
				property->position.x += property->speed.x*player->get_dt()*property->time + (property->time*property->time) / 2;


		}
		else
		{
			property->action_time = 0;
			property->legAction = false;
			return;
		}

		property->action_time += player->get_dt();

	}

	void JumpLeg::CannonJump()
	{
		Character* player = &CharacterManager::GetPlayerByID(playerID);
		Property* property = &player->GetProperty();
		JEPoint2D initPos(0, 0);

		if (!property->legAction)
		{
			property->blockmoving = false;
			property->action_time = 0;
			initPos = property->position;
			return;
		}
		if (property->jump)
		{
			property->legAction = false;
			return;
		}

		if (property->action_time > 0.1 && player->IsOnGround())
		{
			property->legAction = false;
			property->blockmoving = false;
			return;
		}

		property->blockmoving = true;

		if (property->flip) //left
			property->position.x -= property->speed.x*1.5*player->get_dt();
		else //right
			property->position.x += property->speed.x*1.5 * player->get_dt();

		property->position.y += property->speed.y * 2 * player->get_dt();

		property->action_time += player->get_dt();

	}
	void FeatherWing::Flying()
	{

		Character* player = &CharacterManager::GetPlayerByID(playerID);
		Property* property = &player->GetProperty();

		if (!property->wingAction)
		{
			property->gravity = GRAVITY;
			return;
		}


		if (player->GetBody()->GetResource().mineral <= 0)
		{
			property->wingAction = false;
			property->gravity = GRAVITY;
			return;
		}

		if (property->position.y > property->wingInit.y + 20.0)
		{

			property->wingAction = false;
			property->action_time = 0;
			property->gravity = GRAVITY;
			return;
		}

		if (property->movingDirection.y > 0)
			player->GetBody()->AddMineral(-0.1);


		property->gravity = GRAVITY_FLY / 2;
		property->position.y += property->speed.y*0.4*player->get_dt();

		
	}
	void LanternHead::LanternLight()
	{

		pDayNight = &(getDayNight());
		Character* player = &CharacterManager::GetPlayerByID(playerID);
		Property* property = &player->GetProperty();

		if (property->headAction && pDayNight->GetCurrState() == DAY)
		{
			SOUNDMANAGER()->Play("tick");
			property->headAction = false;
		}


		if (!property->headAction)
		{
			pDayNight->SetLightSkill(player->get_id(), property->headAction);
			return;
		}

		player->GetBody()->AddMineral(-0.01);
		pDayNight->SetLightSkill(player->get_id(), property->headAction);


		
	}
	void RadioHead::Call()
	{
		Character* player = &CharacterManager::GetPlayerByID(playerID);
		Character* otherPlayer = &CharacterManager::GetPlayerByID(CHARACTER_ID((static_cast<int>(playerID)+1) % 2));
		Property* property1 = &player->GetProperty();
		Property* property2 = &otherPlayer->GetProperty();
		
		if (!property1->headAction)
			return;
		
		if (g_Collision->IntersectionCircleCircle(player->GetPosition(), 10, property2->position, property2->collisionDistance))
			property1->headAction = false;

		TileMapManager* tileMapManager = &GetTileMapManager();
		Contents currTile[9];
		TilePosition character = ConvertWorldToTilePosition(player->GetPosition());
		JEPoint2D worldCharacter;
		JEPoint2D tile[9];
		TILE_POSITION tilePos[9];

		tileMapManager->GetCollisionArea(character, currTile, 3);

		worldCharacter = ConvertTileToWorldPosition(character);
		worldCharacter.x -= TILE_SPACE / 2;
		worldCharacter.y -= TILE_SPACE / 2;

		g_Collision->CollisionAreaPos(tile, tilePos, worldCharacter, 3);

		int i = 0;
		for (; i < 9; ++i)
		{
			if ((currTile[i].tileIDs == tileMapManager->tileIDs.emptyDirtID
				|| currTile[i].tileIDs == tileMapManager->tileIDs.emptyID) 
				&& (tile[i].x != worldCharacter.x && tile[i].y != worldCharacter.y))
				break;
		}

		property2->position = tile[i];
		


	}
}