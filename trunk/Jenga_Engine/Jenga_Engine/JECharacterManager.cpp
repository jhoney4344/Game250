/*******************************************************************************
Copyright (C) 2015 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
filename    JECharacterManager.cpp
author      Jaeheon Lee, Nahye Park
email       jhoney4344@gmail.com, skqla12@gmail.com
course      GAM250
assignment  Final project submission
due date    12/6/2015

Brief Description:
This file is for managing the characters.
Nahye made character physics(collision, movement, gravity)
Jaehoen made rest of thins.

*******************************************************************************/

#include "JECharacterManager.h"
#include "JEInput.h"
#include "JEAniSystem.h"
#include "JELoadTextureNew.h"
#include "JEPad.h"
#include "JECollisionManager.h"
#include "JEOldParticle.h"

static const int CHARACTER_Y_ADJUST = 0;
static const int MAX_PLAYER_NUM = 2;
static const JEPoint2D START_POSITION = JEPoint2D(0, 0);


namespace{
	JE::Character *player[MAX_PLAYER_NUM];
	JEPoint2D reGenPosition;
	JE::Bone *player_bone[MAX_PLAYER_NUM];
	JE::Property* properties[MAX_PLAYER_NUM];
	JE::Animator ActAni[MAX_PLAYER_NUM];

	int chickenHeadID;
	int baseBodyID, baseArmID, baseLegID, baseHeadID;
	int baseFootID, baseHandID;

	int baseBody2ID, baseArm2ID, baseLeg2ID, baseHead2ID;
	int baseFoot2ID, baseHand2ID;

	int emptyID;
	int hungryID;
	bool once;
	bool jumpAni[2];
	particleSystem* particleSys = &getParticleSystem();

	bool bothAuto;

}

namespace JE{
	namespace CharacterManager
	{
		/*******************************************************************************
		Function: GetPosByID

		Description: Return the position by player ID.


		Inputs: ID - player ID.


		Outputs: position.
		*******************************************************************************/
		JEPoint2D GetPosByID(CHARACTER_ID ID)
		{
			return player[ID]->GetPosition();
		}
		/*******************************************************************************
		Function: Load

		Description: Load the textures.


		Inputs: None.


		Outputs: None.
		*******************************************************************************/
		void Load()
		{
			chickenHeadID = GraphicLoadTexture("Texture//Chicken_Head.tga");
			emptyID = GraphicLoadTexture("Texture//emptypart.tga");
			hungryID = GraphicLoadTexture("Texture//SpeechBubble_Hungry.tga");
			baseBodyID = GraphicLoadTexture("Texture//Base_Body.tga");
			baseHeadID = GraphicLoadTexture("Texture//Base_Head.tga");
			baseHandID = GraphicLoadTexture("Texture//Base_Hand.tga");
			baseArmID = GraphicLoadTexture("Texture//Base_Arm.tga");
			baseLegID = GraphicLoadTexture("Texture//Base_Leg.tga");
			baseFootID = GraphicLoadTexture("Texture//Base_Foot.tga");

			baseBody2ID = GraphicLoadTexture("Texture//Base_Body2.tga");
			baseArm2ID = GraphicLoadTexture("Texture//Base_Arm2.tga");
			baseLeg2ID = GraphicLoadTexture("Texture//Base_Leg2.tga");
			baseHead2ID = GraphicLoadTexture("Texture//Base_Head2.tga");
			baseFoot2ID = GraphicLoadTexture("Texture//Base_Foot2.tga");
			baseHand2ID = GraphicLoadTexture("Texture//Base_Hand2.tga");;
		}
		/*******************************************************************************
		Function: Init

		Description: Initialize for charater.


		Inputs: None.


		Outputs: None.
		*******************************************************************************/
		void Init()
		{
			jumpAni[0] = jumpAni[1] = false;
			for (CHARACTER_ID id = PLAYER_1ST; id < PLAYER_END; 
				id = static_cast<CHARACTER_ID>(static_cast<int>(id)+1))
			{
				player[id] = new Character(id);
				player_bone[id] = BoneInit(player_bone[id], "IniFile//Player.ini");
				player[id]->SetBone(player_bone[id]);

				if (id == PLAYER_1ST)
				{
					BoneSetTextureID(player_bone[id], BODY_NAME::word[PN_HEAD], baseHeadID);
					BoneSetTextureID(player_bone[id], BODY_NAME::word[PN_BODY], baseBodyID);
					BoneSetTextureID(player_bone[id], BODY_NAME::word[PN_LEFTSHOULDER], emptyID);
					BoneSetTextureID(player_bone[id], BODY_NAME::word[PN_RIGHTSHOULDER], emptyID);
					BoneSetTextureID(player_bone[id], BODY_NAME::word[PN_LEFTHIP], emptyID);
					BoneSetTextureID(player_bone[id], BODY_NAME::word[PN_RIGHTHIP], emptyID);
					BoneSetTextureID(player_bone[id], BODY_NAME::word[PN_LEFTARM1], baseArmID);
					BoneSetTextureID(player_bone[id], BODY_NAME::word[PN_LEFTARM2], baseHandID);
					BoneSetTextureID(player_bone[id], BODY_NAME::word[PN_RIGHTARM1], baseArmID);
					BoneSetTextureID(player_bone[id], BODY_NAME::word[PN_RIGHTARM2], baseHandID);
					BoneSetTextureID(player_bone[id], BODY_NAME::word[PN_LEFTLEG1], baseLegID);
					BoneSetTextureID(player_bone[id], BODY_NAME::word[PN_LEFTLEG2], baseFootID);
					BoneSetTextureID(player_bone[id], BODY_NAME::word[PN_RIGHTLEG1], baseLegID);
					BoneSetTextureID(player_bone[id], BODY_NAME::word[PN_RIGHTLEG2], baseFootID);
				}
				else
				{
					BoneSetTextureID(player_bone[id], BODY_NAME::word[PN_HEAD], baseHead2ID);
					BoneSetTextureID(player_bone[id], BODY_NAME::word[PN_BODY], baseBody2ID);
					BoneSetTextureID(player_bone[id], BODY_NAME::word[PN_LEFTSHOULDER], emptyID);
					BoneSetTextureID(player_bone[id], BODY_NAME::word[PN_RIGHTSHOULDER], emptyID);
					BoneSetTextureID(player_bone[id], BODY_NAME::word[PN_LEFTHIP], emptyID);
					BoneSetTextureID(player_bone[id], BODY_NAME::word[PN_RIGHTHIP], emptyID);
					BoneSetTextureID(player_bone[id], BODY_NAME::word[PN_LEFTARM1], baseArm2ID);
					BoneSetTextureID(player_bone[id], BODY_NAME::word[PN_LEFTARM2], baseHand2ID);
					BoneSetTextureID(player_bone[id], BODY_NAME::word[PN_RIGHTARM1], baseArm2ID);
					BoneSetTextureID(player_bone[id], BODY_NAME::word[PN_RIGHTARM2], baseHand2ID);
					BoneSetTextureID(player_bone[id], BODY_NAME::word[PN_LEFTLEG1], baseLeg2ID);
					BoneSetTextureID(player_bone[id], BODY_NAME::word[PN_LEFTLEG2], baseFoot2ID);
					BoneSetTextureID(player_bone[id], BODY_NAME::word[PN_RIGHTLEG1], baseLeg2ID);
					BoneSetTextureID(player_bone[id], BODY_NAME::word[PN_RIGHTLEG2], baseFoot2ID);
				}

				properties[id] = &player[id]->GetProperty();
				AniSystem::AddAni(player_bone[id], properties[id]->acting);
				Character::live1 = TRUE;
				Character::live2 = TRUE;
			}

			reGenPosition = START_POSITION;
			bothAuto = false;

		}
		/*******************************************************************************
		Function: Update

		Description: Update for character.


		Inputs: dt - dt for moving.


		Outputs: None.
		*******************************************************************************/
		void Update(double dt)
		{
			player[PLAYER_1ST]->set_dt(dt);
			player[PLAYER_2ND]->set_dt(dt);
			for (CHARACTER_ID id = PLAYER_1ST; id < PLAYER_END;
				id = static_cast<CHARACTER_ID>(static_cast<int>(id)+1))
				BoneUpdateTime(player_bone[id], dt);

			JEPoint2D initialPos[PLAYER_END];
			initialPos[PLAYER_1ST] = player[PLAYER_1ST]->GetPosition();
			initialPos[PLAYER_2ND] = player[PLAYER_2ND]->GetPosition();

			Movement();

			//if (!MyGamepad->Connected)
			//{
			//player[PLAYER_2ND]->GetBody()->
			//}

			if (g_Input->IsPressed(JE_CONTROL) && g_Input->IsTriggered(JE_F))
			{
				// fill all resource
				player[PLAYER_1ST]->GetBody()->AddBread(100);
				player[PLAYER_1ST]->GetBody()->AddMeat(100);
				player[PLAYER_1ST]->GetBody()->AddMineral(100);

				player[PLAYER_2ND]->GetBody()->AddBread(100);
				player[PLAYER_2ND]->GetBody()->AddMeat(100);
				player[PLAYER_2ND]->GetBody()->AddMineral(100);
			}
			if (g_Input->IsPressed(JE_CONTROL) && g_Input->IsTriggered(JE_D))
			{
				player[PLAYER_1ST]->GetBody()->AddBread(-10);
				player[PLAYER_2ND]->GetBody()->AddBread(-10);
			}

			//MyGamepad->GetButtonPressed(XButtons.Start);
			if (g_Input->IsPressed(JE_NUMPAD0))
			{
				BoneSetTextureID(player_bone[PLAYER_1ST], BODY_NAME::word[PN_LEFTSHOULDER], emptyID);
				BoneSetTextureID(player_bone[PLAYER_1ST], BODY_NAME::word[PN_RIGHTSHOULDER], emptyID);
				BoneSetTextureID(player_bone[PLAYER_1ST], BODY_NAME::word[PN_LEFTHIP], emptyID);
				BoneSetTextureID(player_bone[PLAYER_1ST], BODY_NAME::word[PN_RIGHTHIP], emptyID);
			}

			for (CHARACTER_ID id = PLAYER_1ST; id < PLAYER_END;
				id = static_cast<CHARACTER_ID>(static_cast<int>(id)+1))
			{
				if (Character::live1 == false && id == PLAYER_1ST)
					continue;
				else if (Character::live2 == false && id == PLAYER_2ND)
					continue;

				properties[id]->time += dt;

				bool live;
				// Dead Condition
				if (Character::live1 == true && id == PLAYER_1ST)
					live = true;
				else if (Character::live2 == true && id == PLAYER_2ND)
					live = true;
				else live = false;

				if (player[id]->GetResource().bread <= 0 && live == true)
				{
					ObjectManager::AddObject(OBJ_TOMB, new Tomb(player[id]->GetPosition()));
					if (id == PLAYER_1ST)
						Character::live1 = false;
					else if (id == PLAYER_2ND)
						Character::live2 = false;
				}

				if (properties[id]->jump)
					player[id]->Jump();


				if (player[id]->IsOnGround())
				{
					properties[id]->gravity = 0;
					if (properties[id]->autoPlay)
						properties[id]->jump = false;
					if (!AniSystem::IsFalling(player_bone[PLAYER_1ST]))
						AniSystem::StopAni(player_bone[PLAYER_1ST]);
				}
				else
					properties[id]->gravity = GRAVITY;

				JEPoint2D playerPos = player[id]->GetPosition();
				player[id]->SetPosition(JEPoint2D(playerPos.x, playerPos.y - (properties[id]->gravity*properties[id]->time*properties[id]->time) / 2));

				if (properties[id]->knockback_distance <= 0)
				{
					properties[id]->knockBack = false;
					properties[id]->blockCollision = false;
				}

				if (properties[id]->knockBack)
				{
					properties[id]->knockback_distance -= 0.1;
					g_Collision->KnockBack(properties[id]->position, properties[id]->enemyVec, properties[id]->knockback_distance);
				}



				player[id]->CollisionCheck();
				player[id]->CharacterCollision(player[static_cast<CHARACTER_ID>((static_cast<int>(id)+1) % MAX_PLAYER_NUM)]);
				player[id]->GetBody()->BurningCalory(dt);


			}
			///////////////////////AUTO PLAY///////////////////



			if (g_Input->IsPressed(JE_CONTROL) && g_Input->IsTriggered(JE_U))
			{
				if (!bothAuto)
					bothAuto = true;
				else
					bothAuto = false;
				
			}
			
			if (bothAuto)
			{
				properties[PLAYER_1ST]->autoPlay = true;
				properties[PLAYER_2ND]->autoPlay = true;
			}
			else
			{
				properties[PLAYER_1ST]->autoPlay = false;
				properties[PLAYER_2ND]->autoPlay = false;
			}

			if (properties[PLAYER_1ST]->autoPlay)
				player[PLAYER_1ST]->AutoPlay(player[PLAYER_2ND]);


			if (!MyGamepad->Connected() && !properties[PLAYER_1ST]->autoPlay)
				properties[PLAYER_2ND]->autoPlay = true;
			else if (MyGamepad->Connected() && !properties[PLAYER_1ST]->autoPlay)
				properties[PLAYER_2ND]->autoPlay = false;

			if (properties[PLAYER_2ND]->autoPlay)
				player[PLAYER_2ND]->AutoPlay(player[PLAYER_1ST]);


			///////////////////////............. SKILL............////////////////////
			if (player[PLAYER_1ST]->live1)
			{


				if (g_Input->IsTriggered(JE_A))
				{
					player[PLAYER_1ST]->UseSkill(PART_HEAD);
				}

				if (g_Input->IsTriggered(JE_S) && !properties[PLAYER_1ST]->armAction)
				{
					player[PLAYER_1ST]->UseSkill(PART_ARM);
				}

				if (g_Input->IsTriggered(JE_D))
				{
					player[PLAYER_1ST]->UseSkill(PART_LEG);
				}
				player[PLAYER_1ST]->SkillUpdate();
			}

			if (player[PLAYER_2ND]->live2)
			{
				if (MyGamepad->GetButtonDown(XButtons.X))
				{
					player[PLAYER_2ND]->UseSkill(PART_LEG);
				}

				if (MyGamepad->GetButtonDown(XButtons.Y))
				{
					player[PLAYER_2ND]->UseSkill(PART_ARM);
				}


				if (MyGamepad->GetButtonDown(XButtons.B))
				{
					player[PLAYER_2ND]->UseSkill(PART_HEAD);
				}

				
				player[PLAYER_2ND]->SkillUpdate();
			}

			//moving direction
			properties[PLAYER_1ST]->movingDirection = player[PLAYER_1ST]->GetPosition() - initialPos[PLAYER_1ST];
			properties[PLAYER_2ND]->movingDirection = player[PLAYER_2ND]->GetPosition() - initialPos[PLAYER_2ND];

		}
		/*******************************************************************************
		Function: Draw

		Description: Draw function for character.


		Inputs: None.


		Outputs: None.
		*******************************************************************************/
		void Draw()
		{
			for (CHARACTER_ID id = PLAYER_1ST; id < PLAYER_END;
				id = static_cast<CHARACTER_ID>(static_cast<int>(id)+1))
			{
				if (Character::live1 && PLAYER_1ST == id)
				{
					//check hungry
					if (player[id]->GetResource().bread < player[id]->GetBody()->GetMaxResource().bread * 0.1)
					{
						GraphicSetTexture(hungryID);
						GraphicSetFilpTex(FLIP_IDLE, 1);
						GraphicSetAnimaTex(1, 0);
						GraphicSetPersTransform(properties[id]->position.x + 10, properties[id]->position.y + 10, 0,
							10, 10,// scale
							0);//rotation
					}
					BoneDraw(player_bone[id], properties[id]->position.x,
						properties[id]->position.y + CHARACTER_Y_ADJUST, 0);
				}
				else if (Character::live2 && PLAYER_2ND == id)
				{
					//check hungry
					if (player[id]->GetResource().bread < player[id]->GetBody()->GetMaxResource().bread * 0.1)
					{
						GraphicSetTexture(hungryID);
						GraphicSetFilpTex(FLIP_IDLE, 1);
						GraphicSetAnimaTex(1, 0);
						GraphicSetPersTransform(properties[id]->position.x + 10, properties[id]->position.y + 10, 0,
							10, 10,// scale
							0);//rotation
					}
					BoneDraw(player_bone[id], properties[id]->position.x,
						properties[id]->position.y + CHARACTER_Y_ADJUST, 0);
				}
			}

		}
		/*******************************************************************************
		Function: Shutdown

		Description: Shutdown.


		Inputs: None.


		Outputs: None.
		*******************************************************************************/
		void Shutdown()
		{
			
			for (int i = 0; i < Character::get_character_num(); i++)
			{
				BoneFreeTree(player_bone[i]);
				delete player[i];
			}
		}
		/*******************************************************************************
		Function: Unload

		Description: Unload the textures.


		Inputs: None.


		Outputs: None.
		*******************************************************************************/
		void Unload()
		{
			GraphicUnloadTexture(chickenHeadID);

			GraphicUnloadTexture(emptyID);
			GraphicUnloadTexture(hungryID);
			GraphicUnloadTexture(baseBodyID);
			GraphicUnloadTexture(baseHeadID);
			GraphicUnloadTexture(baseHandID);
			GraphicUnloadTexture(baseArmID);
			GraphicUnloadTexture(baseLegID);
			GraphicUnloadTexture(baseFootID);

			GraphicUnloadTexture(baseBody2ID);
			GraphicUnloadTexture(baseArm2ID);
			GraphicUnloadTexture(baseLeg2ID);
			GraphicUnloadTexture(baseHead2ID);
			GraphicUnloadTexture(baseFoot2ID);
			GraphicUnloadTexture(baseHand2ID);

		}
		/*******************************************************************************
		Function: SetReGenPosition

		Description: Set regenerated position.


		Inputs: newReGenPos - new position for character.


		Outputs: None.
		*******************************************************************************/
		void SetReGenPosition(JEPoint2D newReGenPos)
		{
			reGenPosition = newReGenPos;
		}
		/*******************************************************************************
		Function: CharacterRestart

		Description: Set character for restart.


		Inputs: None.


		Outputs: None.
		*******************************************************************************/
		void CharacterRestart()
		{
			for (CHARACTER_ID id = PLAYER_1ST; id < PLAYER_END;
				id = static_cast<CHARACTER_ID>(static_cast<int>(id)+1))
			{
				player[id]->ChangeParts(PART_HEAD, BASIC_HEAD);
				player[id]->ChangeParts(PART_BODY, BASIC_BODY);
				player[id]->ChangeParts(PART_ARM, BASIC_ARM);
				player[id]->ChangeParts(PART_LEG, BASIC_LEG);
			}
		}
		/*******************************************************************************
		Function: GetPlayerByID

		Description: Return pointer to player by player ID.


		Inputs: id - player id.


		Outputs: pointer to character.
		*******************************************************************************/
		Character& GetPlayerByID(CHARACTER_ID id)
		{
			return *player[id];
		}
		/*******************************************************************************
		Function: GetMaxPlayerNum

		Description: Return the max player number.


		Inputs: None.


		Outputs: max player number.
		*******************************************************************************/
		int GetMaxPlayerNum()
		{
			return MAX_PLAYER_NUM;
		}
		/*******************************************************************************
		Function: Movement

		Description: Movement for character.


		Inputs: None.


		Outputs: None.
		*******************************************************************************/
		void Movement(void)
		{
			// 1st Player's movement
			if (player[PLAYER_1ST]->live1)
			{
				if (!properties[PLAYER_1ST]->acting && !properties[PLAYER_1ST]->jump)
					AniSystem::SetAni(player_bone[PLAYER_1ST], ANI_STAND, true);

				properties[PLAYER_1ST]->acting = false;

				if (!properties[PLAYER_1ST]->blockmoving && !properties[PLAYER_1ST]->autoPlay)
				{
					if (g_Input->IsPressed(JE_ARROW_RIGHT))
						//if (MyGamepad->Connected() && !MyGamepad->LStick_InDeadzone() && MyGamepad->LeftStick_X() > 0.0f)
					{
						properties[PLAYER_1ST]->direction = JE::Vector3f(1.f, 0.f, 0.f);

						if (properties[PLAYER_1ST]->flip == true)
						{
							properties[PLAYER_1ST]->flip = false;
							BoneReverse(player_bone[PLAYER_1ST]);
						}
						if (!properties[PLAYER_1ST]->jump) 
							AniSystem::SetAni(player_bone[PLAYER_1ST], ANI_RUNNING, true);
						player[0]->SetPosition(player[0]->GetPosition()
							+ JEPoint2D(player[PLAYER_1ST]->get_dt()*player[PLAYER_1ST]->GetProperty().speed.x, 0));
					}
					else if (g_Input->IsPressed(JE_ARROW_LEFT))
						//else if (MyGamepad->Connected() && !MyGamepad->LStick_InDeadzone() && MyGamepad->LeftStick_X() < 0.0f)
					{
						properties[PLAYER_1ST]->direction = JE::Vector3f(-1.f, 0.f, 0.f);

						if (properties[PLAYER_1ST]->flip == false)
						{
							properties[PLAYER_1ST]->flip = true;
							BoneReverse(player_bone[PLAYER_1ST]);
						}
						if (!properties[PLAYER_1ST]->jump)
							AniSystem::SetAni(player_bone[PLAYER_1ST], ANI_RUNNING, true);
						player[PLAYER_1ST]->SetPosition(player[PLAYER_1ST]->GetPosition()
							+ JEPoint2D(-player[PLAYER_1ST]->get_dt()*player[PLAYER_1ST]->GetProperty().speed.x, 0));
					}

					if (player[PLAYER_1ST]->IsOnGround())
					{
						if (jumpAni[PLAYER_1ST])
						{
							AniSystem::StopAni(player_bone[PLAYER_1ST]);
							jumpAni[PLAYER_1ST] = false;
						}
						if (g_Input->IsTriggered(JE_SPACE) && !properties[PLAYER_1ST]->jump)
						{
							SOUNDMANAGER()->Play("jump1");

							JEPoint2D save = player[PLAYER_1ST]->GetPosition();

							Vector3f emitPosition = Vector3f(float(save.x), float(save.y), 0.f);

							particleSys->addParticleEmit(PJUMP, emitPosition, 30, EMPTY_EMITTER_ID, properties[PLAYER_1ST]->direction);
							AniSystem::SetAni(player_bone[PLAYER_1ST], ANI_FALL, true);
							properties[PLAYER_1ST]->time = 0.5;
							properties[PLAYER_1ST]->jump = true;
						}
						
					}
					//else if ( player[PLAYER_1ST]-> && g_Input->IsTriggered(JE_A))

					properties[PLAYER_1ST]->direction = JE::Vector3f(0.f, 0.f, 0.f);
				}
				
				if (g_Input->IsTriggered(JE_F))
				{
					AniSystem::SetAni(player_bone[PLAYER_1ST], ANI_DIGGING, false);
					player[0]->Digging();
				}
				else if (g_Input->IsTriggered(JE_D))
				{
					PART_TYPE type = player[PLAYER_1ST]->GetPartType(PART_LEG);
					if (type == DASH_LEG){AniSystem::SetAni(player_bone[PLAYER_1ST], ANI_DASH, false);}
					else if (type == JUMP_LEG && player[PLAYER_1ST]->IsOnGround())
					{
						AniSystem::SetAni(player_bone[PLAYER_1ST], ANI_CANNON_JUMP, false);
						jumpAni[PLAYER_1ST] = true;
					}
				}
				else if (g_Input->IsTriggered(JE_S))
				{
					PART_TYPE type = player[PLAYER_1ST]->GetPartType(PART_ARM);
					if (type == FEATHER_WING) AniSystem::SetAni(player_bone[PLAYER_1ST], ANI_FLYING, false);
					else if (type == CLAW_ARM) AniSystem::SetAni(player_bone[PLAYER_1ST], ANI_DIGGING, false);
				}
				else if (g_Input->IsTriggered(JE_A))
				{
					PART_TYPE type = player[PLAYER_1ST]->GetPartType(PART_HEAD);
					if (type == RADIO_HEAD) AniSystem::SetAni(player_bone[PLAYER_1ST], ANI_SUMMON, false);
				}

			}

			

			// 2nd Player's movement
			if (player[PLAYER_2ND]->live2)
			{

				if (!properties[PLAYER_2ND]->acting  && !properties[PLAYER_2ND]->jump)
					AniSystem::SetAni(player_bone[PLAYER_2ND], ANI_STAND, true);

				properties[PLAYER_2ND]->acting = false;

				if (!properties[PLAYER_2ND]->blockmoving && !properties[PLAYER_2ND]->autoPlay)
				{
					if (MyGamepad->Connected() && ((!MyGamepad->LStick_InDeadzone() && MyGamepad->LeftStick_X() > 1.0f) || MyGamepad->GetButtonPressed(XButtons.DPad_Right)))
					{
						properties[PLAYER_2ND]->direction = JE::Vector3f(1.f, 0.f, 0.f);
						
						if (properties[PLAYER_2ND]->flip == true)
						{
							properties[PLAYER_2ND]->flip = false;
							BoneReverse(player_bone[PLAYER_2ND]);
						}
						if (!properties[PLAYER_2ND]->jump)
							AniSystem::SetAni(player_bone[PLAYER_2ND], ANI_RUNNING, true);
						player[PLAYER_2ND]->SetPosition(player[PLAYER_2ND]->GetPosition()
							+ JEPoint2D(player[PLAYER_2ND]->get_dt()*player[PLAYER_2ND]->GetProperty().speed.x, 0));
					}
					else if (MyGamepad->Connected() && ((!MyGamepad->LStick_InDeadzone() && MyGamepad->LeftStick_X() < -1.0f) || MyGamepad->GetButtonPressed(XButtons.DPad_Left)))
					{

						properties[PLAYER_2ND]->direction = JE::Vector3f(-1.f, 0.f, 0.f);

						if (properties[PLAYER_2ND]->flip == false)
						{
							properties[PLAYER_2ND]->flip = true;
							BoneReverse(player_bone[PLAYER_2ND]);
						}
						if (!properties[PLAYER_2ND]->jump)
							AniSystem::SetAni(player_bone[PLAYER_2ND], ANI_RUNNING, true);
						player[PLAYER_2ND]->SetPosition(player[PLAYER_2ND]->GetPosition()
							+ JEPoint2D(-player[PLAYER_2ND]->get_dt()*player[PLAYER_2ND]->GetProperty().speed.x, 0));
					}
					if (player[PLAYER_2ND]->IsOnGround())
					{
						if (jumpAni[PLAYER_2ND])
						{
							AniSystem::StopAni(player_bone[PLAYER_2ND]);
							jumpAni[PLAYER_2ND] = false;
						}
						if (MyGamepad->Connected() && MyGamepad->GetButtonDown(XButtons.A) && !properties[PLAYER_2ND]->jump)
						{
							SOUNDMANAGER()->Play("jump1");
							JEPoint2D savePosition = player[PLAYER_2ND]->GetPosition();
							Vector3f emitPosition = Vector3f(float(savePosition.x), float(savePosition.y), 0.f);
							particleSys->addParticleEmit(PJUMP, emitPosition, 12, EMPTY_EMITTER_ID, properties[PLAYER_2ND]->direction);

							AniSystem::SetAni(player_bone[PLAYER_2ND], ANI_FALL, true);
							properties[PLAYER_2ND]->time = 0.5;
							properties[PLAYER_2ND]->jump = true;
						}
					}
					properties[PLAYER_2ND]->direction = JE::Vector3f(0.f, 0.f, 0.f);
				}

				if (MyGamepad->Connected())
				{
					if (MyGamepad->GetButtonDown(XButtons.R_Shoulder))
					{
						AniSystem::SetAni(player_bone[PLAYER_2ND], ANI_DIGGING, false);
						player[PLAYER_2ND]->Digging();
					}
					else if (MyGamepad->GetButtonDown(XButtons.X))
					{
						PART_TYPE type = player[PLAYER_2ND]->GetPartType(PART_LEG);
						if (type == DASH_LEG) AniSystem::SetAni(player_bone[PLAYER_2ND], ANI_DASH, false);
						else if (type == JUMP_LEG && player[PLAYER_2ND]->IsOnGround())
						{
							AniSystem::SetAni(player_bone[PLAYER_2ND], ANI_CANNON_JUMP, false);
							jumpAni[PLAYER_2ND] = true;
						}
					}
					else if (MyGamepad->GetButtonDown(XButtons.Y))
					{
						PART_TYPE type = player[PLAYER_2ND]->GetPartType(PART_ARM);
						if (type == FEATHER_WING) AniSystem::SetAni(player_bone[PLAYER_2ND], ANI_FLYING, false);
						else if (type == CLAW_ARM) AniSystem::SetAni(player_bone[PLAYER_2ND], ANI_DIGGING, false);
					}
					else if (MyGamepad->GetButtonDown(XButtons.B))
					{
						PART_TYPE type = player[PLAYER_2ND]->GetPartType(PART_HEAD);
						if (type == RADIO_HEAD) AniSystem::SetAni(player_bone[PLAYER_2ND], ANI_SUMMON, false);
					}
				}

				if (  MyGamepad->GetButtonPressed(XButtons.R_Shoulder))
					MyGamepad->Rumble(1.f, 1.f);
				else
					MyGamepad->Rumble(0.0f, 0.0f);
			}

		}
		
	}//Character Manager
}//JE
