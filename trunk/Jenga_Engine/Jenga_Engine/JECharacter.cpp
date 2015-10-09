/*******************************************************************************
Copyright (C) 2015 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
filename    JECharacter.cpp
author      Jaeheon Lee, Nahye Park
email       jhoney4344@gmail.com, skqla12@gmail.com
course      GAM250
assignment  Final project submission
due date    12/6/2015

Brief Description:
The functions that managing characters of player.
Jaehoen made functions about body parts of character.
Nahye made functions about collision of character and basic skill for character.

*******************************************************************************/

#include "JECharacter.h"
#include "JEGraphicsNew.h"
#include "JELoadTextureNew.h"
#include "JEMath.h"
#include "JETileMap.h"
#include "JEInput.h"
#include "JEPad.h"
#include "JEDebug.h"
#include "JECollisionManager.h"
#include "JEOldParticle.h"
#include "JESound.h"
#include "JEShop.h"


namespace
{
	particleSystem* particleSys = &getParticleSystem();
	JE::Vector3f particleDirtPosition;
	JE::Vector3f particleDirtEndPosition;
	JE::DayNight* pDayNight;
}

namespace JE{
	bool Character::live1 = true;
	bool Character::live2 = true;
	int Character::character_num = 0;

	/*******************************************************************************
	Function: Character

	Description: Constructor for character.


	Inputs: identity - identity of character (player1 or player2).


	Outputs: None.
	*******************************************************************************/
	Character::Character(CHARACTER_ID identity) : ID(identity)
	{
		property.flip = false;
		property.time = 0;
		property.jump = false;
		property.acting = false;
		property.speed = JEPoint2D(50, 110);
		property.collisionDistance = 5.0f;
		property.isOnGround = false;
		property.action_time = 0;
		property.headAction = false;
		property.armAction = false;
		property.legAction = false;
		property.blockmoving = false;
		property.gravity = GRAVITY;
		property.knockBack = false;
		property.knockback_distance = 0;
		property.wingAction = false;
		property.blockCollision = false;
		property.autoPlay = false;
		property.auto_time = 0;

		

		ChangeHead(BASIC_HEAD);
		ChangeBody(BASIC_BODY);
		ChangeArm(BASIC_ARM);
		ChangeLeg(BASIC_LEG);

		character_num++;
	}

	/*******************************************************************************
	Function: ~Character

	Description: Destructor for character.


	Inputs: None.


	Outputs: None.
	*******************************************************************************/
	Character::~Character()
	{
		delete head.pPart;
		delete body.pPart;
		delete arm.pPart;
		delete leg.pPart;

		character_num--;
	}

	/*******************************************************************************
	Function: ChangeParts

	Description: Change the parts of character.


	Inputs: body_part - kind of body part.
			type - type of part


	Outputs: None.
	*******************************************************************************/
	void Character::ChangeParts(BODY_PART body_part, PART_TYPE type)
	{
		switch (body_part)
		{
		case PART_HEAD:
			ChangeHead(type);
			break;
		case PART_BODY:
			ChangeBody(type);
			break;
		case PART_ARM:
			ChangeArm(type);
			break;
		case PART_LEG:
			ChangeLeg(type);
			break;
		}
	}

	/*******************************************************************************
	Function: UseSkill

	Description: Use skill.


	Inputs: body_part - part of body.


	Outputs: None.
	*******************************************************************************/
	void Character::UseSkill(BODY_PART body_part)
	{
		switch (body_part)
		{
		case PART_HEAD:
			HeadSkill();
			break;
		case PART_ARM:
			ArmSkill();
			break;
		case PART_LEG:
			LegSkill();
			break;
		}
	}

	/*******************************************************************************
	Function: GetBody

	Description: Return the part of body.


	Inputs: None.


	Outputs: None.
	*******************************************************************************/
	BaseBody* Character::GetBody() { return body.pPart; }

	/*******************************************************************************
	Function: GetPartType

	Description: Return the type of part

	Inputs: which part

	Outputs: type of parts
	*******************************************************************************/
	PART_TYPE Character::GetPartType(BODY_PART part)
	{
		PART_TYPE type;

		switch (part)
		{
		case PART_HEAD: type = head.partType; break;
		case PART_BODY: type = body.partType; break;
		case PART_ARM:  type = arm.partType;  break;
		case PART_LEG:  type = leg.partType;  break;

		default: type = END_TYPE; break;
		}

		return type;
	}

/////////////////////////////////////////////////////////////////////
///////////////      CHANGE PART            /////////////////////////
/////////////////////////////////////////////////////////////////////

	/*******************************************************************************
	Function: ChangeHead

	Description: Change the head part.


	Inputs: partType - type of head.


	Outputs: None.
	*******************************************************************************/
	void Character::ChangeHead(PART_TYPE partType)
	{
		if (head.pPart)
			delete head.pPart;

		switch (partType)
		{
		case BASIC_HEAD:
			head.pPart = new BasicHead;
			head.partType = BASIC_ARM;
			break;
		case LANTERN_HEAD:
			head.pPart = new LanternHead;
			head.partType = LANTERN_HEAD;
			break;
		case RADIO_HEAD:
			head.pPart = new RadioHead;
			head.partType = RADIO_HEAD;
			break;
		default:
			head.pPart = new BasicHead;
			head.partType = BASIC_ARM;
			break;
		}
		head.pPart->playerID = ID;
	}

	/*******************************************************************************
	Function: ChangeArm

	Description: Change the part of arm.


	Inputs: partType - type of arm.


	Outputs: None.
	*******************************************************************************/
	void Character::ChangeArm(PART_TYPE partType)
	{
		if (arm.pPart)
			delete arm.pPart;
		switch (partType)
		{
		case BASIC_ARM:
			arm.pPart = new BasicArm;
			arm.partType = BASIC_ARM;
			break;
		case CLAW_ARM:
			arm.pPart = new ClawArm;
			arm.partType = CLAW_ARM;
			break;
		case FEATHER_WING:
			arm.pPart = new FeatherWing;
			arm.partType = FEATHER_WING;
			break;
		default:
			arm.pPart = new BasicArm;
			arm.partType = BASIC_ARM;
			break;
		}
		arm.pPart->playerID = ID;
	}

	/*******************************************************************************
	Function: ChangeLeg

	Description: Change the leg part.


	Inputs: partType - type of leg.


	Outputs: None.
	*******************************************************************************/
	void Character::ChangeLeg(PART_TYPE partType)
	{
		if (leg.pPart)
			delete leg.pPart;
		switch (partType)
		{
		case BASIC_LEG:
			leg.pPart = new BasicLeg;
			leg.partType = BASIC_LEG;
			break;
		case DASH_LEG:
			leg.pPart = new DashLeg;
			leg.partType = DASH_LEG;
			break;
		case JUMP_LEG:
			leg.pPart = new JumpLeg;
			leg.partType = JUMP_LEG;
			break;
		default:
			leg.pPart = new BasicLeg;
			leg.partType = BASIC_LEG;
			break;
		}
		leg.pPart->playerID = ID;
	}

	/*******************************************************************************
	Function: ChangeBody

	Description: Change the body part.


	Inputs: partType - type of body.


	Outputs: None.
	*******************************************************************************/
	void Character::ChangeBody(PART_TYPE partType)
	{
		if (body.pPart)
			delete body.pPart;
		switch (partType)
		{
		case BASIC_BODY:
			body.pPart = new BasicBody(&resource);
			body.partType = BASIC_BODY;
			break;
		case BEAR_BODY:
			body.pPart = new BearBody(&resource);
			body.partType = BEAR_BODY;
			break;
		case GOOD_BODY:
			body.pPart = new GoodBody(&resource);
			body.partType = GOOD_BODY;
			break;

		default:
			body.pPart = new BasicBody(&resource);
			body.partType = BASIC_BODY;
			break;
		}
		body.pPart->playerID = ID;
	}



/////////////////////////////////////////////////////////////////////
///////////////          ACTION             /////////////////////////
/////////////////////////////////////////////////////////////////////

	/*******************************************************************************
	Function: HeadSkill

	Description: Use head skill.


	Inputs: None.


	Outputs: None.
	*******************************************************************************/
	void Character::HeadSkill()
	{

		
		switch (head.partType)
		{
		case BASIC_HEAD:
			//dynamic_cast<BASIC_HEAD*>(head.pPart)->BasicAction();
			break;
		case LANTERN_HEAD:
			if (!property.headAction)
				property.headAction = true;
			else
				property.headAction = false;
			
			static_cast<LanternHead*>(head.pPart)->LanternLight();
			SOUNDMANAGER()->Play("tick");
			break;
		case RADIO_HEAD:
			if (body.pPart->GetResource().mineral >= head.pPart->GetPrice())
				body.pPart->GetResource().mineral -= head.pPart->GetPrice();
			else
				break;

			property.headAction = true;
			static_cast<RadioHead*>(head.pPart)->Call();
			SOUNDMANAGER()->Play("teleport");
			break;
		}
	}

	/*******************************************************************************
	Function: ArmSkill

	Description: Use arm skill.


	Inputs: None.


	Outputs: None.
	*******************************************************************************/
	void Character::ArmSkill()
	{
		if (body.pPart->GetResource().mineral >= arm.pPart->GetPrice() && arm.partType != FEATHER_WING)
			body.pPart->GetResource().mineral -= arm.pPart->GetPrice();
		else if (body.pPart->GetResource().mineral < arm.pPart->GetPrice() && arm.partType != FEATHER_WING)
			return;
		else if (arm.partType == FEATHER_WING && body.pPart->GetResource().mineral <= 0)
			return;
		switch (arm.partType)
		{
		case BASIC_ARM:
			// dynamic_cast<BASIC_ARM*>(arm.pPart)->BasicAction();
			break;
		case CLAW_ARM:
			//???
			property.armAction = true;
			property.action_time = 0;
			//AniSystem::SetAni(GetBone(), ANI_CLAW, false);
			dynamic_cast<ClawArm*>(arm.pPart)->ClawAttack();
			break;
		case FEATHER_WING:
			if (!property.jump)
			{
				property.wingAction = true;
				property.gravity = GRAVITY_FLY / 2;
				property.time = 0;
				property.wingInit = property.position;
				//property.flying_distance = 2;
				dynamic_cast<FeatherWing*>(arm.pPart)->Flying();
				SOUNDMANAGER()->Play("jump2");
			}
			break;
		}
	}

	/*******************************************************************************
	Function: LegSkill

	Description: Use leg skill.


	Inputs: None.


	Outputs: None.
	*******************************************************************************/
	void Character::LegSkill()
	{
		
		switch (leg.partType)
		{
		case BASIC_LEG:
			break;
		case DASH_LEG:
			if (body.pPart->GetResource().mineral >= leg.pPart->GetPrice())
				body.pPart->GetResource().mineral -= leg.pPart->GetPrice();
			else
				return;
			property.legAction = true;
			dynamic_cast<DashLeg*>(leg.pPart)->Dash();
			SOUNDMANAGER()->Play("cat");
			break;
		case JUMP_LEG:
			if (body.pPart->GetResource().mineral >= leg.pPart->GetPrice() && !property.jump)
				body.pPart->GetResource().mineral -= leg.pPart->GetPrice();
			else
				return;
			property.legAction = true;
			dynamic_cast<JumpLeg*>(leg.pPart)->CannonJump();
			SOUNDMANAGER()->Play("jump2");
			break;
		}
	}

	/*******************************************************************************
	Function: Digging

	Description: Degging tree or ground, attack the enemy.


	Inputs: None.


	Outputs: None.
	*******************************************************************************/
	void Character::Digging()
	{
		TileMapManager* tileMapManager = &GetTileMapManager();
		Contents currTile[9];
		TilePosition character;
		JEPoint2D worldCharacter;
		JEPoint2D tile[9];
		TilePosition pos[9];
		TILE_POSITION tilePos[9];
		int tileSize = 3;
		int direction = 0;
		bool ground = false;

		if (get_id() == PLAYER_1ST)
			character = tileMapManager->Get1stCharacterTilePosition();
		else if (get_id() == PLAYER_2ND)
			character = tileMapManager->Get2ndCharacterTilePosition();

		tileMapManager->GetCollisionArea(character, currTile, tileSize);

		worldCharacter = ConvertTileToWorldPosition(character);
		worldCharacter.x -= TILE_SPACE / 2;
		worldCharacter.y -= TILE_SPACE / 2;

		g_Collision->CollisionAreaPos(tile, tilePos, worldCharacter, tileSize);

		TilePosition result;

		if ((get_id() == PLAYER_1ST && g_Input->IsPressed(JE_ARROW_LEFT))
			|| get_id() == PLAYER_2ND && ((!MyGamepad->LStick_InDeadzone() && MyGamepad->LeftStick_X() < -2.0f) || MyGamepad->GetButtonPressed(XButtons.DPad_Left)))
		{
			direction = 3;
			result.x = character.x - 1;
			result.y = character.y;
			ground = true;

			particleDirtPosition = JE::Vector3f(float(property.position.x - 5), float(property.position.y), 0);
			particleDirtEndPosition = JE::Vector3f(float(property.position.x - 10), float(property.position.y), 0);
		}
		else if ((get_id() == PLAYER_1ST &&g_Input->IsPressed(JE_ARROW_RIGHT))
			|| get_id() == PLAYER_2ND && ((!MyGamepad->LStick_InDeadzone() && MyGamepad->LeftStick_X() > 2.0f) || MyGamepad->GetButtonPressed(XButtons.DPad_Right)))
		{
			direction = 5;
			result.x = character.x + 1;
			result.y = character.y;
			ground = true;
		
			particleDirtPosition = JE::Vector3f(float(property.position.x + 5), float(property.position.y), 0);
			particleDirtEndPosition = JE::Vector3f(float(property.position.x + 10), float(property.position.y), 0);
		}
		else if ((get_id() == PLAYER_1ST &&g_Input->IsPressed(JE_ARROW_DOWN))
			|| get_id() == PLAYER_2ND && !MyGamepad->LStick_InDeadzone() && (MyGamepad->LeftStick_Y() < -2.0f || MyGamepad->GetButtonPressed(XButtons.DPad_Down)))
		{
			direction = 7;
			result.x = character.x;
			result.y = character.y - 1;
			ground = true;

			particleDirtPosition = JE::Vector3f(float(property.position.x), float(property.position.y - 5), 0);
			particleDirtEndPosition = JE::Vector3f(float(property.position.x), float(property.position.y - 10), 0);
		}
		else if ((get_id() == PLAYER_1ST &&	g_Input->IsPressed(JE_ARROW_UP))
			|| get_id() == PLAYER_2ND &&  !MyGamepad->LStick_InDeadzone() && (MyGamepad->LeftStick_Y() > 2.0f || MyGamepad->GetButtonPressed(XButtons.DPad_Up)))
		{
			direction = 1;
			result.x = character.x;
			result.y = character.y + 1;
			ground = true;

			particleDirtPosition = JE::Vector3f(float(property.position.x), float(property.position.y + 5), 0);
			particleDirtEndPosition = JE::Vector3f(float(property.position.x), float(property.position.y + 10), 0);
		}
		else if (get_id() == PLAYER_2ND &&property.autoPlay && property.flip)
		{
			direction = 3;
			result.x = character.x - 1;
			result.y = character.y;
			ground = true;

			particleDirtPosition = JE::Vector3f(float(property.position.x - 5), float(property.position.y), 0);
			particleDirtEndPosition = JE::Vector3f(float(property.position.x - 10), float(property.position.y), 0);
		}
		else if (get_id() == PLAYER_2ND &&property.autoPlay && !property.flip)
		{
			direction = 5;
			result.x = character.x + 1;
			result.y = character.y;
			ground = true;

			particleDirtPosition = JE::Vector3f(float(property.position.x + 5), float(property.position.y), 0);
			particleDirtEndPosition = JE::Vector3f(float(property.position.x + 10), float(property.position.y), 0);
		}

		if (ground)
		{
			if (currTile[direction].tileIDs == tileMapManager->tileIDs.dirtID
				&& g_Collision->IntersectionCircleRect(property.position, property.collisionDistance, tile[direction], TILE_SPACE, TILE_SPACE))
			{

				Contents* contectTile = &tileMapManager->tiles.find(result)->second;

				contectTile->tileHP--;

				//test for particle
				if (contectTile->tileHP > 0)
				{
					SOUNDMANAGER()->Play("dirt");
					particleSys->addParticleEmit(SMOKE, particleDirtPosition, 4);
				}
				else if (contectTile->tileHP == 0)
				{
					SOUNDMANAGER()->Play("dirt");
					particleSys->addParticleEmit(SMOKE, particleDirtEndPosition, 12);
				}


				if (contectTile->tileHP <= 0)
				{
					SOUNDMANAGER()->Play("shovel");
					int wormPosibility = RandValue(-1, 3);
					if (contectTile->mineral)
					{
						ObjectManager::AddObject(OBJ_MINERAL, new Mineral(tile[direction]));
					}
					else if (!wormPosibility)
					{
						// worm!!!
						ObjectManager::AddObject(OBJ_BUG, new Bug(JEPoint2D(tile[direction])));
					}
					tileMapManager->SetTileType(result, tileMapManager->tileIDs.emptyDirtID);
				}

			}
		}
		else
		{


			////////////////ENEMY
			if (CheckEnemyCollision(character.x, character.y))
				return;
			if (CheckEnemyCollision(character.x, character.y+1))
				return;
			if (CheckEnemyCollision(character.x, character.y-1))
				return;

			if (property.flip)//left enemy
			{
				if(CheckEnemyCollision(character.x - 1, character.y))
					return;
			}
			else if (!property.flip)//right enemy
			{
				if(CheckEnemyCollision(character.x + 1, character.y))
					return;
			}

			///////////CHECK TREE////////////
			//center
			bool centerTree =
				CheckTreeCollision(character.x, character.y);
			bool centerDownTree =
				CheckTreeCollision(character.x, character.y - 1);

			if (property.flip)//left enemy
			{
				//left
				if (!centerTree && !centerDownTree)
				{
					CheckTreeCollision(character.x - 1, character.y);
					CheckTreeCollision(character.x - 1, character.y - 1);
				}
			}
			else
			{
				//right
				if (!centerTree && !centerDownTree)
				{
					CheckTreeCollision(character.x + 1, character.y);
					CheckTreeCollision(character.x + 1, character.y - 1);
				}
			}

			
		}
	}

	/*******************************************************************************
	Function: Jump

	Description: Jump character.


	Inputs: None.


	Outputs: None.
	*******************************************************************************/
	void Character::Jump()
	{
		property.position.y += property.speed.y*dt;
	}

	/*******************************************************************************
	Function: GetPosition

	Description: Return the position of character.


	Inputs: None.


	Outputs: None.
	*******************************************************************************/
	JEPoint2D Character::GetPosition()
	{
		return property.position;
	}

	/*******************************************************************************
	Function: GetProperty

	Description: Return the property of character.


	Inputs: None.


	Outputs: None.
	*******************************************************************************/
	Property& Character::GetProperty()
	{
		return property;
	}

	/*******************************************************************************
	Function: IsOnGround

	Description: If character is on ground.


	Inputs: None.


	Outputs: If character is on groud, return true.
			 Else, return false.
	*******************************************************************************/
	bool Character::IsOnGround()
	{
		return property.isOnGround;
	}

	/*******************************************************************************
	Function: SetPosition

	Description: Set character position.


	Inputs: newPos - new position of character.


	Outputs: None.
	*******************************************************************************/
	void Character::SetPosition(JEPoint2D newPos)
	{
		property.position = newPos;
	}

	/*******************************************************************************
	Function: GetResource

	Description: Return the resource of character.


	Inputs: None.


	Outputs: None.
	*******************************************************************************/
	Resource& Character::GetResource()
	{
		return resource;
	}

	/*******************************************************************************
	Function: set_dt

	Description: Set dt for moving.


	Inputs: _dt - dt for moving.


	Outputs: None.
	*******************************************************************************/
	void Character::set_dt(double _dt)
	{
		dt = _dt;
	}

	/*******************************************************************************
	Function: get_dt

	Description: Return the dt of character.


	Inputs: None.


	Outputs: None.
	*******************************************************************************/
	double Character::get_dt() const
	{
		return dt;
	}

	/*******************************************************************************
	Function: get_id

	Description: Return the id of character.


	Inputs: None.


	Outputs: None.
	*******************************************************************************/
	CHARACTER_ID Character::get_id() const
	{
		return ID;
	}

	/*******************************************************************************
	Function: get_character_num

	Description: Return the character number.


	Inputs: None.


	Outputs: None.
	*******************************************************************************/
	int Character::get_character_num()
	{
		return character_num;
	}


	/*******************************************************************************
	Function: CollisionCheck

	Description: Collision check for charater


	Inputs: None.


	Outputs: None.
	*******************************************************************************/
	void Character::CollisionCheck(void)
	{
		//TILE_SPACE(10) : tile size
		TileMapManager* tileMapManager = &GetTileMapManager();
		Contents currTile[49];
		TilePosition character = ConvertWorldToTilePosition(property.position);
		JEPoint2D worldCharacter;
		JEPoint2D tile[49];
		TILE_POSITION tilePos[49];
		bool isCollided = false;
		bool down = false;

		tileMapManager->GetCollisionArea(character, currTile, 7);

		worldCharacter = ConvertTileToWorldPosition(character);
		worldCharacter.x -= TILE_SPACE / 2;
		worldCharacter.y -= TILE_SPACE / 2;

		g_Collision->CollisionAreaPos(tile, tilePos, worldCharacter, 7);


		if ((currTile[31].tileIDs == tileMapManager->tileIDs.dirtID || currTile[31].tileIDs == tileMapManager->tileIDs.stoneID)
			&& g_Collision->IntersectionCircleRect(property.position, property.collisionDistance, tile[31], TILE_SPACE, TILE_SPACE))
			down = true;
		else
			property.isOnGround = false;



		for (int i = 0; i < 49; ++i)
		{

			//ground
			if (currTile[i].tileIDs == tileMapManager->tileIDs.dirtID || currTile[i].tileIDs == tileMapManager->tileIDs.stoneID
				|| currTile[i].tileIDs == tileMapManager->tileIDs.unbreakableTileID)
			{
				if (g_Collision->IntersectionCircleRect(property.position, property.collisionDistance, tile[i], TILE_SPACE, TILE_SPACE))
				{

					if (property.wingAction && (tilePos[i] == TL_LEFTDOWN || tilePos[i] == TL_RIGHTDOWN))
						continue;

					if (down && (tilePos[i] == TL_LEFTDOWN || tilePos[i] == TL_RIGHTDOWN))
						continue;

					

					g_Collision->CollisionResponseWithTile(property.position, property.collisionDistance,
															tile[i], tilePos[i]);

					

					switch(tilePos[i])
					{
					case TL_DOWN:
						if (!property.autoPlay)
							property.jump = false;
						property.time = 0.5;
						property.isOnGround = true;
						property.wingAction = false;
						break;
					case TL_UP:
						property.time = 0.2;
						property.jump = false;
						property.wingAction = false;
						break;
					case TL_RIGHT:
						property.legAction = false;
						if (property.autoPlay)
							property.jump = true;
						break;
					case TL_LEFT:
						property.legAction = false;
						if (property.autoPlay)
							property.jump = true;
						break;
					case TL_LEFTDOWN:

						break;
					case TL_RIGHTDOWN:
						break;
					case TL_LEFTUP:
						break;
					case TL_RIGHTUP:
						break;
					}


					isCollided = true;
				}
			}
			//meat
			for (ObjVecItor it = currTile[i].objectData.begin(); it != currTile[i].objectData.end(); it++)
			{

				if (it->objectType == OBJ_DEER)
				{
					Deer* deer = dynamic_cast<Deer*>(it->object);
					if (g_Collision->IntersectionCircleRect(property.position, property.collisionDistance, deer->collisionPos, it->object->collisionDistance*1.5, it->object->collisionDistance*0.8)
						&&!property.blockCollision)
					{
						SOUNDMANAGER()->Play("enepunch");
						GetBody()->AddBread(-1);
						particleSys->addParticleEmit(BLOOD, JE::Vector3f(float(property.position.x), float(property.position.y), 0.f), 1);
						property.knockBack = true;
						property.blockCollision = true;
						property.knockback_distance = 1.5;
						property.enemyVec = g_Math->Normalize(JEPoint2D(property.position.x - it->object->position.x, property.position.y - it->object->position.y));
						MyGamepad->Rumble(1.f, 1.f);
						
					}
				}
				if (g_Collision->IntersectionCircleRect(property.position, property.collisionDistance, it->object->position, it->object->collisionDistance, it->object->collisionDistance))
				{
					
					if (it->objectType == OBJ_BUG || it->objectType == OBJ_MEAT)
					{
						if (!dynamic_cast<Eatable*>(it->object)->settedWhoEat)
						{
							dynamic_cast<Eatable*>(it->object)->whoEat = ID;
							dynamic_cast<Eatable*>(it->object)->settedWhoEat = true;
						}
						if (!dynamic_cast<Eatable*>(it->object)->eaten)
						{
							dynamic_cast<Eatable*>(it->object)->SetEaten(true, ID);
							body.pPart->AddMeat(5);
							it->object->applyGravity = false;
						}
					}
					else if (it->objectType == OBJ_BREAD)
					{
						if (!dynamic_cast<Eatable*>(it->object)->settedWhoEat)
						{
							dynamic_cast<Eatable*>(it->object)->whoEat = ID;
							dynamic_cast<Eatable*>(it->object)->settedWhoEat = true;
						}					
						if (!dynamic_cast<Eatable*>(it->object)->eaten)
						{
							dynamic_cast<Eatable*>(it->object)->SetEaten(true,ID);
							body.pPart->AddBread(20);
							it->object->applyGravity = false;
						}
						
					}
					else if (it->objectType == OBJ_MINERAL )
					{
						body.pPart->AddMineral(5);
						SOUNDMANAGER()->Play("mineral");
						it->object->remove = true;
					}
					if (it->objectType == OBJ_ZOMBIE  &&!property.blockCollision)
					{
						SOUNDMANAGER()->Play("enepunch");
						GetBody()->AddBread(-1);
						particleSys->addParticleEmit(BLOOD, JE::Vector3f(float(property.position.x), float(property.position.y), 0.f), 1);
						property.knockBack = true;
						property.blockCollision = true;
						property.knockback_distance = 1.5;
						property.enemyVec = g_Math->Normalize(JEPoint2D(property.position.x - it->object->position.x, property.position.y - it->object->position.y));
						MyGamepad->Rumble(1.f, 1.f);
						
					}
					

				}

			}

		}

		if (!isCollided)
			property.isOnGround = false;

	}

	/*******************************************************************************
	Function: SkillUpdate

	Description: Update the skill of character.


	Inputs: None.


	Outputs: None.
	*******************************************************************************/
	void Character::SkillUpdate()
	{
		//head;
		switch (head.partType)
		{
		case BASIC_HEAD:
			break;
		case LANTERN_HEAD:
			static_cast<LanternHead*>(head.pPart)->LanternLight();
			break;
		}

		//arm
		switch (arm.partType)
		{
		case BASIC_ARM:
			break;
		case CLAW_ARM:
			dynamic_cast<ClawArm*>(arm.pPart)->ClawAttack();
			break;
		case FEATHER_WING:
			dynamic_cast<FeatherWing*>(arm.pPart)->Flying();
			break;
		}

		//leg
		switch (leg.partType)
		{
		case BASIC_LEG:
			break;
		case DASH_LEG:
			dynamic_cast<DashLeg*>(leg.pPart)->Dash();
			break;
		case JUMP_LEG:
			dynamic_cast<JumpLeg*>(leg.pPart)->CannonJump();
			break;
		}
	}


	/*******************************************************************************
	Function: CharacterCollision

	Description: Collision check between characters.


	Inputs: player - pointer to other character.


	Outputs: None.
	*******************************************************************************/
	void Character::CharacterCollision(Character* player)
	{
		JEPoint2D lineStart(player->property.position.x - property.collisionDistance*0.4, player->property.position.y + property.collisionDistance);
		JEPoint2D lineEnd(player->property.position.x + property.collisionDistance*0.4, player->property.position.y + property.collisionDistance);
		if (g_Collision->IntersectionCircleLine(property.position, property.collisionDistance, lineStart, lineEnd)
			&& property.position.y > player->property.position.y + property.collisionDistance)
		{
			property.position.y += property.collisionDistance - g_Math->DistancePointLine(property.position,
				lineStart,
				lineEnd);
			property.time = 0.5;
			property.jump = false;
			property.isOnGround = true;
		}
	}

	/*******************************************************************************
	Function: CheckTreeCollision

	Description: Check colision with tree.


	Inputs: tilePosX - x position of tile.
	tilePosY - y position of tile.


	Outputs: None.
	*******************************************************************************/
	bool Character::CheckTreeCollision(int tilePosX, int tilePosY)
	{
		TileMapManager* tileMapManager = &GetTileMapManager();
		bool result = false;
		TilePosition treeTile;
		treeTile.x = tilePosX;
		treeTile.y = tilePosY;

		Contents* contectTile = &tileMapManager->tiles.find(treeTile)->second;
		for (ObjVecItor objit = contectTile->objectData.begin(); objit != contectTile->objectData.end(); ++objit)
		{
			//OBJ_SHORT_TREE
			//OBJ_LONG_TREE
			if (objit->objectType == OBJ_SHORT_TREE || objit->objectType == OBJ_LONG_TREE)
			{

				//object type
				JEPoint2D objPos;

				if (objit->objectType == OBJ_SHORT_TREE)
					objPos = JEPoint2D(objit->object->position.x, objit->object->position.y + TILE_SPACE / 2);

				else if (objit->objectType == OBJ_LONG_TREE)
					objPos = JEPoint2D(objit->object->position.x, objit->object->position.y + TILE_SPACE );

				if (g_Collision->IntersectionCircleRect(property.position, property.collisionDistance*2,
					objPos, objit->object->scale.x, objit->object->scale.y))
				{
					dynamic_cast<Living*>(objit->object)->SetHP(dynamic_cast<Living*>(objit->object)->GetHP() - 1);
					
					/*if (property.autoPlay)
						property.speed.x = 0;*/

					if (dynamic_cast<Living*>(objit->object)->GetHP() < 0)
					{
						SOUNDMANAGER()->Play("wood");
						objit->object->remove = true;
					}
					else
						SOUNDMANAGER()->Play("woodcut");

					result = true;
					
				}
			}

			if (objit->objectType == OBJ_SHOP && !property.autoPlay)
			{
				JEPoint2D objPos;

				objPos = JEPoint2D(objit->object->position.x, objit->object->position.y);
				if (g_Collision->IntersectionCircleRect(property.position, property.collisionDistance * 2,
					objPos, objit->object->scale.x, objit->object->scale.y))
				{
					if (property.flip)
					{
						property.flip = false;
						BoneReverse(bone);
					}
					if (ID == PLAYER_1ST && live1)
						if (live2 == false)
						{
							live2 = true;
						}
					if (ID == PLAYER_2ND && live2)
						if (live1 == false)
						{
							live1 = true;
						}
					SOUNDMANAGER()->Play("shop");
					SetShopOn(true);
					
				}
			}

		}
		return result;
	}

	/*******************************************************************************
	Function: CheckEnemyCollision

	Description: Check collision with enenmy.


	Inputs: tilePosX - x position of tile.
	tilePosY - y position of tile.


	Outputs: None.
	*******************************************************************************/
	bool Character::CheckEnemyCollision(int tilePosX, int tilePosY)
	{
		TileMapManager* tileMapManager = &GetTileMapManager();
		bool result = false;
		TilePosition enemyTile;
		enemyTile.x = tilePosX;
		enemyTile.y = tilePosY;

		Contents* contectTile = &tileMapManager->tiles.find(enemyTile)->second;
		for (ObjVecItor objit = contectTile->objectData.begin(); objit != contectTile->objectData.end(); ++objit)
		{
			//OBJ_SHORT_TREE
			//OBJ_LONG_TREE
			if (objit->objectType == OBJ_ZOMBIE)//OBJ_ENEMY)
			{

				if (g_Collision->IntersectionCircleRect(property.position, property.collisionDistance*2 ,
					objit->object->position, objit->object->collisionDistance, objit->object->collisionDistance))
				{
					/*if (property.autoPlay)
						property.speed.x = 0;*/

					if (arm.partType == CLAW_ARM)
					{
						SOUNDMANAGER()->Play("powerpunch");
						dynamic_cast<Living*>(objit->object)->SetHP(-1);
					}
					else
					{
						//static_cast<Living*>(objit->object)->SetHP(static_cast<Living*>(objit->object)->GetHP() - 1);
						SOUNDMANAGER()->Play("punch");
						dynamic_cast<Living*>(objit->object)->SetHP(dynamic_cast<Living*>(objit->object)->GetHP() - 1);
						dynamic_cast<Zombie*>(objit->object)->knockBack = true;
						dynamic_cast<Zombie*>(objit->object)->knockback_distance = 2;
						dynamic_cast<Zombie*>(objit->object)->knockVec = JEPoint2D(g_Math->Normalize(JEPoint2D(objit->object->position.x - property.position.x,
							objit->object->position.y - property.position.y)));
						//objit->object->position.y += 2;
					}
					particleSys->addParticleEmit(BLOOD, JE::Vector3f(float(objit->object->position.x), float(objit->object->position.y), 0.f), 1);
					if (dynamic_cast<Living*>(objit->object)->GetHP() < 0)
					{
						objit->object->remove = true;
					}

					result = true;

				}
			}
			if (objit->objectType == OBJ_DEER)
			{
				Deer* deer = dynamic_cast<Deer*>(objit->object);
				if (g_Collision->IntersectionCircleRect(property.position, property.collisionDistance*3,
					deer->collisionPos, objit->object->collisionDistance*4, objit->object->collisionDistance*2))
				{
					/*if (property.autoPlay)
						property.speed.x = 0;*/

					if (arm.partType == CLAW_ARM)
					{
						SOUNDMANAGER()->Play("powerpunch");
						dynamic_cast<Living*>(objit->object)->SetHP(-1);
					}
					else
					{
						SOUNDMANAGER()->Play("punch");
						dynamic_cast<Living*>(objit->object)->SetHP(dynamic_cast<Living*>(objit->object)->GetHP() - 1);
						deer->knockBack = true;
						deer->knockback_distance = 1.5;
						deer->knockVec = JEPoint2D(g_Math->Normalize(JEPoint2D(deer->collisionPos.x - property.position.x,
							deer->collisionPos.y - property.position.y)));
						std::cout << "Deer knockBack" << std::endl;
					}
					//objit->object->position.y += 2;
					particleSys->addParticleEmit(BLOOD, JE::Vector3f(float(objit->object->position.x), float(objit->object->position.y), 0.f), 1);

					if (dynamic_cast<Living*>(objit->object)->GetHP() < 0)
					{
						objit->object->remove = true;
					}

					result = true;

				}
			}
		}
		return result;
	}

	/*******************************************************************************
	Function: GetBone

	Description: Return the pointer to character bone.


	Inputs: None.


	Outputs: None.
	*******************************************************************************/
	Bone* Character::GetBone()
	{
		return bone;
	}

	/*******************************************************************************
	Function: SetBone

	Description: Set the bone of character.


	Inputs: _bone - pointer to new bone.


	Outputs: None.
	*******************************************************************************/
	void Character::SetBone(Bone* _bone)
	{
		bone = _bone;
	}

	/*******************************************************************************
	Function: AutoPlay

	Description: Auto play for unconnected player.


	Inputs: None.


	Outputs: None.
	*******************************************************************************/
	void Character::AutoPlay(Character* player)
	{
		if (!property.autoPlay)
			return;


		int ID = 0;
		if (get_id() == PLAYER_1ST)
			ID = PLAYER_1ST;
		else
			ID = PLAYER_2ND;


		if (property.auto_time > 2)
		{
			if (!player->property.autoPlay)
			{
				JEPoint2D direction = g_Math->Normalize(JEPoint2D((player->property.position.x - property.position.x), 0));
				property.speed = JEPoint2D(direction.x *player->property.speed.x, property.speed.y);
			}

			else
			{
				if (ID != PLAYER_1ST)
				{
					JEPoint2D direction = g_Math->Normalize(JEPoint2D((player->property.position.x - property.position.x), 0));
					property.speed = JEPoint2D(direction.x *player->property.speed.x, property.speed.y);
				}
			}

			property.auto_time = 0;
		}

		property.position.x += property.speed.x*get_dt()*0.8;

		TileMapManager* tileMapManager = &GetTileMapManager();
		Contents currTile[9];
		TilePosition character;
		JEPoint2D worldCharacter;
		JEPoint2D tile[9];
		TilePosition pos[9];
		TILE_POSITION tilePos[9];
		int tileSize = 3;

		if (ID == PLAYER_1ST)
			character = tileMapManager->Get1stCharacterTilePosition();
		else if (ID == PLAYER_2ND)
			character = tileMapManager->Get2ndCharacterTilePosition();

		tileMapManager->GetCollisionArea(character, currTile, tileSize);

		worldCharacter = ConvertTileToWorldPosition(character);
		worldCharacter.x -= TILE_SPACE / 2;
		worldCharacter.y -= TILE_SPACE / 2;

		g_Collision->CollisionAreaPos(tile, tilePos, worldCharacter, tileSize);

		////////////////ENEMY
		if (CheckEnemyCollision(character.x, character.y))
			return;
		if (CheckEnemyCollision(character.x, character.y + 1))
			return;
		if (CheckEnemyCollision(character.x, character.y - 1))
			return;

		if (property.flip)//left enemy
		{
			if (CheckEnemyCollision(character.x - 1, character.y))
				return;
		}
		else if (!property.flip)//right enemy
		{
			if (CheckEnemyCollision(character.x + 1, character.y))
				return;
		}

		///////////CHECK TREE////////////
		//center
		bool centerTree =
			CheckTreeCollision(character.x, character.y);
		bool centerDownTree =
			CheckTreeCollision(character.x, character.y - 1);

		if (property.flip)//left enemy
		{
			//left
			if (!centerTree && !centerDownTree)
			{
				CheckTreeCollision(character.x - 1, character.y);
				CheckTreeCollision(character.x - 1, character.y - 1);
			}
		}
		else
		{
			//right
			if (!centerTree && !centerDownTree)
			{
				CheckTreeCollision(character.x + 1, character.y);
				CheckTreeCollision(character.x + 1, character.y - 1);
			}
		}



		property.auto_time += get_dt();


	}
}