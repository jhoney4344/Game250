/*******************************************************************************
Copyright (C) 2015 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
filename    JEShop.cpp
author      Jaeheon Lee
            Sunghak Jin
email       jhoney4344@gmail.com
            shjin920@gmail.com
course      GAM250
assignment  Final submission
due date    6/12/2015

Brief Description:
This file control the shop contents. Buying parts and show the parta menu.

*******************************************************************************/
#include "JEShop.h"	
#include "JEGameData.h"
#include "JEGraphicsNew.h"
#include "JEResource.h"
#include "JEResourceGauge.h"

//#include "JEIniFileManager.h"

static const float HALF_SIDE_LENGTH = 1.5f;
static const float PARTS_IMAGE_SCALE = 1;
static const float CUSTOM_BOX_SCALE = 1.f;
static const int MAX_PLAYER_NUM = 2;
extern JE::GameData gd;

namespace{
	int shopIconID, shopBoardID;
	bool shopOn = false;
	JE::ResourceID resourceID;

	JEPoint2D orthoMouse; ////////////////////////
	JEPoint2D padOrthoMouse; /////////////////////
	int mousePointID;     ////////////////////////
	int tempID;           ////////////////////////
	bool ShopChk = true;  ////////////////////////
	int padPointID;       ////////////////////////
	int padTempID;        ////////////////////////
	bool PadShopChk = true;  /////////////////////

	int connectedID;
	int disConnectedID;
	int rectangleID;
	int crystalID;
	int radioHeadID, radioHeadPartID, lightHeadID, lightHeadPartID, bearBodyID, bearBodyPartID, goodBodyID, goodBodyPartID, clawID, wingID, catLegID, duckLegID;
	int clawLeft1ID, clawLeft2ID, clawRight1ID, clawRight2ID, wingLeft1ID, wingLeft2ID, wingRight1ID, wingRight2ID;
	int catFoot1ID, catFoot2ID, duckFoot1ID, duckFoot2ID;
	int angelWing1ID, angelWing2ID;
	int angelWing_PDA, claw_PDA, Lantern_PDA, Radio_PDA, good_PDA, bear_PDA, cat_PDA, duck_PDA;

	int shop_Head_1, shop_Body_1, shop_Arm_1, shop_Foot_1;
	int shop_Head_2, shop_Body_2, shop_Arm_2, shop_Foot_2;
	int quesMark;

	JEPoint2D partsMenuBox[8];
	JEPoint2D partsMenuBoxImage[8];
	JEPoint2D selectBox[5]; // The black rectangles on the right side of shop
	JEPoint2D selectBoxImage[5];
	JEPoint2D characterBox[6];

	JE::BODY_PART PartIndex;
	JE::BODY_PART PadPartIndex;
	JE::Character* player[MAX_PLAYER_NUM];
	JE::Bone* player_bone[MAX_PLAYER_NUM];

	JE::PART_TYPE partID;
	int pID;
	int pID2;
	int pID3; 
	int pID4;

	bool Select = false;;
}

namespace JE{

	void SetShopOn(bool _shopOn)
	{
		shopOn = _shopOn;
	}
	bool& GetShopOn()
	{
		return shopOn;
	}

	/*******************************************************************************
	Function: ShopCheck
	Description: Check and show whether the part has been clicked

	Inputs: ID - ID of the part

	Outputs: None.
	*******************************************************************************/
	void ShopCheck(int ID)
	{
		if (ShopChk == true)
		{
			mousePointID = ID;
			ShopChk = false;
		}

		else if (ShopChk == false)
		{
			mousePointID = tempID;
			ShopChk = true;
		}
	}

	void PadShopCheck(int ID)
	{
		if (PadShopChk == true)
		{
			padPointID = ID;
			PadShopChk = false;
		}

		else if (PadShopChk == false)
		{
			padPointID = padTempID;
			PadShopChk = true;
		}
	}

	/*******************************************************************************
	Function: ShopLoad

	Description: Load the texture and resources.

	Inputs: None.

	Outputs: None.
	*******************************************************************************/
	void ShopLoad()
	{
		mousePointID = GraphicLoadTexture("Texture//mouse.tga"); /////////////////////////////////////
		tempID = mousePointID; ////////////////////////////////////
		padPointID = GraphicLoadTexture("Texture//pad.tga"); /////////////////////////////////////
		padTempID = padPointID; ////////////////////////////////////

		connectedID = GraphicLoadTexture("Texture//HUD_Connected.tga");
		disConnectedID = GraphicLoadTexture("Texture//HUD_Disconnected.tga");

		radioHeadPartID = GraphicLoadTexture("Texture//Radio_Head.tga");
		radioHeadID = GraphicLoadTexture("Texture//Radio_Head_Mouse.tga");
		lightHeadID = GraphicLoadTexture("Texture//Light_Head_Mouse.tga");
		lightHeadPartID = GraphicLoadTexture("Texture//Light_Head.tga");
		bearBodyID = GraphicLoadTexture("Texture//Bear_Body_Mouse.tga");
		bearBodyPartID = GraphicLoadTexture("Texture//Bear_Body.tga");
		goodBodyPartID = GraphicLoadTexture("Texture//Good_Body.tga");
		goodBodyID = GraphicLoadTexture("Texture//Good_Body_Mouse.tga");
		clawID = GraphicLoadTexture("Texture//Claw_Right.tga");
		wingID = GraphicLoadTexture("Texture//Angel_Wing.tga");
		catLegID = GraphicLoadTexture("Texture//Cat_Foot_Mouse.tga");
		duckLegID = GraphicLoadTexture("Texture//Duck_Foot_Mouse.tga");

		crystalID = GraphicLoadTexture("Texture//Crystal.tga");

		clawLeft2ID = GraphicLoadTexture("Texture//Claw_Left.tga");
		clawRight2ID = GraphicLoadTexture("Texture//Claw_Right.tga");
		clawLeft1ID = GraphicLoadTexture("Texture//Claw_ArmLR.tga");
		clawRight1ID = GraphicLoadTexture("Texture//Claw_ArmLR.tga");

		wingLeft2ID = GraphicLoadTexture("Texture//Angel_Wing.tga");
		wingRight2ID = GraphicLoadTexture("Texture//Angel_Wing.tga");
		wingLeft1ID = GraphicLoadTexture("Texture//Angel_Arm.tga");
		wingRight1ID = GraphicLoadTexture("Texture//Angel_Arm.tga");

		catFoot1ID = GraphicLoadTexture("Texture//Cat_Leg.tga");
		catFoot2ID = GraphicLoadTexture("Texture//Cat_Foot.tga");
		duckFoot1ID = GraphicLoadTexture("Texture//Duck_Leg.tga");
		duckFoot2ID = GraphicLoadTexture("Texture//Duck_Foot.tga");
		angelWing1ID = GraphicLoadTexture("Texture//Angel_Arm.tga");
		angelWing2ID = GraphicLoadTexture("Texture//Angel_Wing.tga");

	
		shopBoardID = JE::GraphicLoadTexture("Texture//shop.tga");

		rectangleID = JE::GraphicLoadTexture("Texture//EmptyRec.tga");

		angelWing_PDA = JE::GraphicLoadTexture("Texture//PDA_Angel.tga");
		claw_PDA = JE::GraphicLoadTexture("Texture//PDA_Claw.tga");
		Lantern_PDA = JE::GraphicLoadTexture("Texture//PDA_Lighter.tga");
		Radio_PDA = JE::GraphicLoadTexture("Texture//PDA_Radio.tga");
		good_PDA = JE::GraphicLoadTexture("Texture//PDA_Good.tga");
		bear_PDA = JE::GraphicLoadTexture("Texture//PDA_Bear.tga");
		cat_PDA = JE::GraphicLoadTexture("Texture//PDA_Cat.tga");
		duck_PDA = JE::GraphicLoadTexture("Texture//PDA_Duck.tga");

		quesMark = JE::GraphicLoadTexture("Texture//QustionMark.tga");


	}
	/*******************************************************************************
	Function: ShopInit

	Description: Initiliaze the vairalbe.

	Inputs: None.

	Outputs: None.
	*******************************************************************************/
	void ShopInit()
	{
		shop_Head_1 = shop_Body_1 = shop_Arm_1 = shop_Foot_1 =
			shop_Head_2 = shop_Body_2 = shop_Arm_2 = shop_Foot_2 = 0/* = quesMark*/;

		resourceID = ResourceSystem::GetResourceID();
		resourceID.mineral = crystalID;
		for (CHARACTER_ID id = PLAYER_1ST; id < PLAYER_END;
			id = static_cast<CHARACTER_ID>(static_cast<int>(id)+1))
		{
			
			player[id] = &CharacterManager::GetPlayerByID(id);
			player_bone[id] = player[id]->GetBone();
		}

		// This is for mouse collision(interacting)
		selectBox[PART_HEAD] = JEPoint2D(4, 4.5);
		selectBox[PART_BODY] = JEPoint2D(6.5, -2);
		selectBox[PART_ARM] = JEPoint2D(0.86, 1.35);
		selectBox[PART_LEG] = JEPoint2D(1.7, -2);


		// This is for drawing
		selectBoxImage[PART_HEAD] = JEPoint2D(20, 22.45);
		selectBoxImage[PART_BODY] = JEPoint2D(4.06, 6.96);
		selectBoxImage[PART_ARM] = JEPoint2D(35.8, 6.96);
		selectBoxImage[PART_LEG] = JEPoint2D(8.8, -9);


		for (int i = 0; i<4; i++)
			for (int j = 0; j<2; j++)
			{
				//0 1
				//2 3
				//4 5
				//6 7
				partsMenuBox[i * 2 + j].x = 10.6 + j*2.8;
				partsMenuBox[i * 2 + j].y = 5 - 2.5*i;

				partsMenuBoxImage[i * 2 + j].x = 53 + j * 14;
				partsMenuBoxImage[i * 2 + j].y = 26 - 12 * i;
			}
	}



	/*******************************************************************************
	Function: ShopUpdate

	Description: Check the shop using and add the part to the character.

	Inputs: mousePosition - get mouse position from state.
	cameraPosition - get camera position from state.

	Outputs: None.
	*******************************************************************************/
	bool ShopUpdate(const JEPoint2D& mousePosition, JEPoint2D& padCursorPosition)
	{
		auto pBody = player[PLAYER_1ST]->GetBody();
		auto pBody_2 = player[PLAYER_2ND]->GetBody();

		orthoMouse = mousePosition;
		padOrthoMouse = padCursorPosition;
		if (gd.pause)
			return shopOn;
		if (g_Input->IsTriggered(JE_ESCAPE) || MyGamepad
			->GetButtonDown(XButtons.Back))
		{
			shopOn = false;
		}

		PartIndex = GetRect(mousePosition, PartIndex);
		PadPartIndex = GetRect(padCursorPosition, PadPartIndex);

		 if(g_Input->IsPressed(JE_CONTROL) && g_Input->IsTriggered(JE_S))
			{
				if (shopOn)
				{
					shopOn = false;
					SOUNDMANAGER()->Play("shop");
				}
				else
				{
					shopOn = true;
					SOUNDMANAGER()->Play("shop");
					for (CHARACTER_ID id = PLAYER_1ST; id < PLAYER_END;
						id = static_cast<CHARACTER_ID>(static_cast<int>(id)+1))
					{
						if (player[id]->GetProperty().flip == true)
						{
							player[id]->GetProperty().flip = false;
							BoneReverse(player_bone[id]);
						}
					}// Character loop
				}
			}
	

		if (shopOn)
		{

			MyGamepad->Rumble(0.f, 0.f);
			//Revive the first dead player
			if (!(player[PLAYER_1ST]->live1) && player[PLAYER_2ND]->live2)
			{
				if (MyGamepad->GetButtonPressed(XButtons.R_Shoulder))
				{
					player[PLAYER_1ST]->live1 = true;
					player[PLAYER_1ST]->GetResource().bread = 50;
					player[PLAYER_1ST]->SetPosition(player[PLAYER_2ND]->GetPosition() + JEPoint2D(0, 5));
				}
			}
			//Revive the second dead player
			else if (player[PLAYER_1ST]->live1 && !(player[PLAYER_2ND]->live2))
			{
				if (g_Input->IsTriggered(JE_F))
				{
					player[PLAYER_2ND]->live2 = true;
					player[PLAYER_2ND]->GetResource().bread = 50;
					player[PLAYER_2ND]->SetPosition(player[PLAYER_1ST]->GetPosition() + JEPoint2D(0, 5));
				}
			}


			for (CHARACTER_ID id = PLAYER_1ST; id < PLAYER_END;
				id = static_cast<CHARACTER_ID>(static_cast<int>(id)+1))
			{
				if (player[id]->GetProperty().flip == true)
				{
					player[id]->GetProperty().flip = false;
					BoneReverse(player_bone[id]);
				}
			}// Character loop

			if (shop_Head_1)
			{
				//////////player 1////////////////////
				GraphicSetTexture(shop_Head_1);
				GraphicSetFilpTex(JE::FLIP_IDLE, 1);
				GraphicSetAnimaTex(1, 0);
				GraphicSetOrthogTransform(-0.72, 0.62, 0, // x,y,z coordinate
					0.2, 0.2,//scale x,y
					0);//rotation
			}

			if (shop_Arm_1)
			{
				GraphicSetTexture(shop_Arm_1);
				GraphicSetFilpTex(JE::FLIP_IDLE, 1);
				GraphicSetAnimaTex(1, 0);
				GraphicSetOrthogTransform(-0.085, 0.62, 0, // x,y,z coordinate
					0.2, 0.2,//scale x,y
					0);//rotation
			}

			if (shop_Body_1)
			{
				GraphicSetTexture(shop_Body_1);
				GraphicSetFilpTex(JE::FLIP_IDLE, 1);
				GraphicSetAnimaTex(1, 0);
				GraphicSetOrthogTransform(-0.72, 0.06, 0, // x,y,z coordinate
					0.2, 0.2,//scale x,y
					0);//rotation
			}

			if (shop_Foot_1)
			{
				GraphicSetTexture(shop_Foot_1);
				GraphicSetFilpTex(JE::FLIP_IDLE, 1);
				GraphicSetAnimaTex(1, 0);
				GraphicSetOrthogTransform(-0.085, 0.05, 0, // x,y,z coordinate
					0.2, 0.2,//scale x,y
					0);//rotation
			}

				///////////player 2////////////////////////
			if (shop_Head_2)
			{
				GraphicSetTexture(shop_Head_2);
				GraphicSetFilpTex(JE::FLIP_IDLE, 1);
				GraphicSetAnimaTex(1, 0);
				GraphicSetOrthogTransform(-0.72, -0.24, 0, // x,y,z coordinate
					0.2, 0.2,//scale x,y
					0);//rotation
			}

			if (shop_Arm_2)
			{
				GraphicSetTexture(shop_Arm_2);
				GraphicSetFilpTex(JE::FLIP_IDLE, 1);
				GraphicSetAnimaTex(1, 0);
				GraphicSetOrthogTransform(-0.085, -0.24, 0, // x,y,z coordinate
					0.2, 0.2,//scale x,y
					0);//rotation
			}

			if (shop_Body_2)
			{
				GraphicSetTexture(shop_Body_2);
				GraphicSetFilpTex(JE::FLIP_IDLE, 1);
				GraphicSetAnimaTex(1, 0);
				GraphicSetOrthogTransform(-0.72, -0.81 , 0, // x,y,z coordinate
					0.2, 0.2,//scale x,y
					0);//rotation
			}

			if (shop_Foot_2)
			{
				GraphicSetTexture(shop_Foot_2);
				GraphicSetFilpTex(JE::FLIP_IDLE, 1);
				GraphicSetAnimaTex(1, 0);
				GraphicSetOrthogTransform(-0.085, -0.81, 0, // x,y,z coordinate
					0.2, 0.2,//scale x,y
					0);//rotation
			}

			/////////////////////////player1////////////////////////////////
			if ((orthoMouse.x > -0.79 && orthoMouse.x < -0.57) && (orthoMouse.y > 0.46 && orthoMouse.y < 0.68))
			{
				if ((g_Input->IsTriggered(JE_MOUSE_LEFT)) && ShopChk == false)
				{
					SOUNDMANAGER()->Play("click");
					//partID = RADIO_HEAD;
					ShopCheck(pID);

					if (pID == radioHeadPartID || pID == lightHeadPartID)
					{
						int radioprice = RadioHead().GetPrice();
						int lightprice = LanternHead().GetPrice();

						if (pBody->GetResource().meat >= radioprice || pBody->GetResource().meat >= lightprice)
						{
							player[PLAYER_1ST]->ChangeParts(PART_HEAD, partID);
							BoneSetTextureID(player_bone[PLAYER_1ST], BODY_NAME::word[PN_HEAD], pID);
							if (pID == radioHeadPartID)
							{
								pBody->AddMeat(-1 * radioprice);
								shop_Head_1 = radioHeadID;
							}
							else if (pID == lightHeadPartID)
							{
								shop_Head_1 = lightHeadID;
								pBody->AddMeat(-1 * lightprice);
							}
						}
						else
							SOUNDMANAGER()->Play("denied");
					}
					else
						SOUNDMANAGER()->Play("denied");
				}
			}

			if ((orthoMouse.x > -0.79 && orthoMouse.x < -0.57) && (orthoMouse.y > -0.1 && orthoMouse.y < 0.12))
			{
				if ((g_Input->IsTriggered(JE_MOUSE_LEFT)) && ShopChk == false)
				{
					SOUNDMANAGER()->Play("click");
					//partID = RADIO_HEAD;
					ShopCheck(pID);

					if (pID == goodBodyPartID || pID == bearBodyPartID)
					{
						int goodprice = GoodBody().GetPrice();
						int bearprice = BearBody().GetPrice();

						
						if (pID == goodBodyPartID)
						{
							if (pBody->GetResource().meat >= goodprice)
							{
								shop_Body_1 = goodBodyID;
								pBody->AddMeat(-1 * goodprice);
								player[PLAYER_1ST]->ChangeParts(PART_BODY, partID);
								BoneSetTextureID(player_bone[PLAYER_1ST], BODY_NAME::word[PN_BODY], pID);
							}
							else
								SOUNDMANAGER()->Play("denied");
						}
						else if (pID == bearBodyPartID)
						{
							if (pBody->GetResource().meat >= bearprice)
							{
								shop_Body_1 = bearBodyID;
								pBody->AddMeat(-1 * bearprice);
								player[PLAYER_1ST]->ChangeParts(PART_BODY, partID);
								BoneSetTextureID(player_bone[PLAYER_1ST], BODY_NAME::word[PN_BODY], pID);
							}
							else
								SOUNDMANAGER()->Play("denied");
						}
					}

					else
						SOUNDMANAGER()->Play("denied");
				}
			}

			if ((orthoMouse.x > -0.15 && orthoMouse.x < 0.06) && (orthoMouse.y > 0.46 && orthoMouse.y < 0.68))
			{
				if ((g_Input->IsTriggered(JE_MOUSE_LEFT)) && ShopChk == false)
				{
					SOUNDMANAGER()->Play("click");
					//partID = RADIO_HEAD;
					ShopCheck(pID);

					if (pID == clawLeft1ID || pID == wingLeft1ID)
					{
						int clawprice = ClawArm().GetPrice();
						int wingprice = FeatherWing().GetPrice();

						if (pBody->GetResource().meat >= clawprice || pBody->GetResource().meat >= wingprice)
						{
							player[PLAYER_1ST]->ChangeParts(PART_ARM, partID);
							BoneSetTextureID(player_bone[PLAYER_1ST], BODY_NAME::word[PN_LEFTARM2], pID2);
							BoneSetTextureID(player_bone[PLAYER_1ST], BODY_NAME::word[PN_RIGHTARM2], pID4);
							BoneSetTextureID(player_bone[PLAYER_1ST], BODY_NAME::word[PN_LEFTARM1], pID);
							BoneSetTextureID(player_bone[PLAYER_1ST], BODY_NAME::word[PN_RIGHTARM1], pID3);
							if (pID == clawLeft1ID)
							{
								shop_Arm_1 = clawID;
								pBody->AddMeat(-1 * clawprice);
							}
							else if (pID == wingLeft1ID)
							{
								shop_Arm_1 = wingID;
								pBody->AddMeat(-1 * wingprice);
							}
						}
						else
							SOUNDMANAGER()->Play("denied");
					}

					else
						SOUNDMANAGER()->Play("denied");
				}
			}


			if ((orthoMouse.x > -0.15 && orthoMouse.x < 0.06) && (orthoMouse.y > -0.11 && orthoMouse.y < 0.11))
			{
				if ((g_Input->IsTriggered(JE_MOUSE_LEFT)) && ShopChk == false)
				{
					SOUNDMANAGER()->Play("click");
					//partID = RADIO_HEAD;
					ShopCheck(pID);

					if (pID == catFoot1ID || pID == duckFoot1ID)
					{
						int catprice = DashLeg().GetPrice();
						int duckprice = JumpLeg().GetPrice();

						if (pBody->GetResource().meat >= catprice || pBody->GetResource().meat >= duckprice)
						{
							player[PLAYER_1ST]->ChangeParts(PART_LEG, partID);

							BoneSetTextureID(player_bone[PLAYER_1ST], BODY_NAME::word[PN_RIGHTLEG1], pID);
							BoneSetTextureID(player_bone[PLAYER_1ST], BODY_NAME::word[PN_LEFTLEG1], pID);
							BoneSetTextureID(player_bone[PLAYER_1ST], BODY_NAME::word[PN_RIGHTLEG2], pID2);
							BoneSetTextureID(player_bone[PLAYER_1ST], BODY_NAME::word[PN_LEFTLEG2], pID2);
							if (pID == catFoot1ID)
							{
								shop_Foot_1 = catLegID;
								pBody->AddMeat(-1 * catprice);
							}
							else if (pID == duckFoot1ID)
							{
								shop_Foot_1 = duckLegID;
								pBody->AddMeat(-1 * duckprice);
							}
						}
						else
							SOUNDMANAGER()->Play("denied");
					}

					else
						SOUNDMANAGER()->Play("denied");
				}
			}

			/////////////////////////player2////////////////////////////////
			if ((padOrthoMouse.x > -0.79 && padOrthoMouse.x < -0.57) && (padOrthoMouse.y > -0.4 && padOrthoMouse.y < -0.18))
			{
				if (MyGamepad->GetButtonDown(XButtons.R_Shoulder) && PadShopChk == false)
				{
					SOUNDMANAGER()->Play("click");
					//partID = RADIO_HEAD;
					PadShopCheck(pID);

					if (pID == radioHeadPartID || pID == lightHeadPartID)
					{
						int radioprice = RadioHead().GetPrice();
						int lightprice = LanternHead().GetPrice();

						if (pBody_2->GetResource().meat >= radioprice || pBody_2->GetResource().meat >= lightprice)
						{
							player[PLAYER_2ND]->ChangeParts(PART_HEAD, partID);
							BoneSetTextureID(player_bone[PLAYER_2ND], BODY_NAME::word[PN_HEAD], pID);
							if (pID == radioHeadPartID)
							{
								shop_Head_2 = radioHeadID;
								pBody_2->AddMeat(-1 * radioprice);
							}
							else if (pID == lightHeadPartID)
							{
								shop_Head_2 = lightHeadID;
								pBody_2->AddMeat(-1 * lightprice);
							}
						}
						else
							SOUNDMANAGER()->Play("denied");
					}

					else
						SOUNDMANAGER()->Play("denied");
				}
			}

			if ((padOrthoMouse.x > -0.79 && padOrthoMouse.x < -0.57) && (padOrthoMouse.y > -0.97 && padOrthoMouse.y < -0.75))
			{
				if (MyGamepad->GetButtonDown(XButtons.R_Shoulder) && PadShopChk == false)
				{
					SOUNDMANAGER()->Play("click");
					//partID = RADIO_HEAD;
					PadShopCheck(pID);

					if (pID == goodBodyPartID || pID == bearBodyPartID)
					{
						int goodprice = GoodBody().GetPrice();
						int bearprice = BearBody().GetPrice();

						
						if (pID == goodBodyPartID)
						{
							if (pBody->GetResource().meat >= goodprice)
							{
								shop_Body_2 = goodBodyID;
								pBody_2->AddMeat(-1 * goodprice);
								player[PLAYER_2ND]->ChangeParts(PART_BODY, partID);
								BoneSetTextureID(player_bone[PLAYER_2ND], BODY_NAME::word[PN_BODY], pID);
							}
							else
								SOUNDMANAGER()->Play("denied");
						}
						else if (pID == bearBodyPartID)
						{
							if (pBody->GetResource().meat >= goodprice)
							{
								shop_Body_2 = bearBodyID;
								pBody_2->AddMeat(-1 * bearprice);
								player[PLAYER_2ND]->ChangeParts(PART_BODY, partID);
								BoneSetTextureID(player_bone[PLAYER_2ND], BODY_NAME::word[PN_BODY], pID);
							}
							else
								SOUNDMANAGER()->Play("denied");
						}
					}

					else
						SOUNDMANAGER()->Play("denied");
				}
			}

			if ((padOrthoMouse.x > -0.15 && padOrthoMouse.x < 0.06) && (padOrthoMouse.y > -0.4 && padOrthoMouse.y < -0.18))
			{
				if (MyGamepad->GetButtonDown(XButtons.R_Shoulder) && PadShopChk == false)
				{
					SOUNDMANAGER()->Play("click");
					//partID = RADIO_HEAD;
					PadShopCheck(pID);

					if (pID == clawLeft1ID || pID == wingLeft1ID)
					{
						int clawprice = ClawArm().GetPrice();
						int wingprice = FeatherWing().GetPrice();

						if (pBody_2->GetResource().meat >= clawprice || pBody_2->GetResource().meat >= wingprice)
						{
							player[PLAYER_2ND]->ChangeParts(PART_ARM, partID);
							BoneSetTextureID(player_bone[PLAYER_2ND], BODY_NAME::word[PN_LEFTARM2], pID2);
							BoneSetTextureID(player_bone[PLAYER_2ND], BODY_NAME::word[PN_RIGHTARM2], pID4);
							BoneSetTextureID(player_bone[PLAYER_2ND], BODY_NAME::word[PN_LEFTARM1], pID);
							BoneSetTextureID(player_bone[PLAYER_2ND], BODY_NAME::word[PN_RIGHTARM1], pID3);
							if (pID == clawLeft1ID)
							{
								shop_Arm_2 = clawID;
								pBody_2->AddMeat(-1 * clawprice);
							}
							else if (pID == wingLeft1ID)
							{
								shop_Arm_2 = wingID;
								pBody_2->AddMeat(-1 * wingprice);
							}
						}
						else
							SOUNDMANAGER()->Play("denied");
					}

					else
						SOUNDMANAGER()->Play("denied");
				}
			}


			if ((padOrthoMouse.x > -0.15 && padOrthoMouse.x < 0.06) && (padOrthoMouse.y > -0.97 && padOrthoMouse.y < -0.75))
				{
					if (MyGamepad->GetButtonDown(XButtons.R_Shoulder) && PadShopChk == false)
					{
						SOUNDMANAGER()->Play("click");
						//partID = RADIO_HEAD;
						PadShopCheck(pID);

						if (pID == catFoot1ID || pID == duckFoot1ID)
						{
							int catprice = DashLeg().GetPrice();
							int duckprice = JumpLeg().GetPrice();

							if (pBody_2->GetResource().meat >= catprice || pBody_2->GetResource().meat >= duckprice)
							{
								player[PLAYER_2ND]->ChangeParts(PART_LEG, partID);

								BoneSetTextureID(player_bone[PLAYER_2ND], BODY_NAME::word[PN_RIGHTLEG1], pID);
								BoneSetTextureID(player_bone[PLAYER_2ND], BODY_NAME::word[PN_LEFTLEG1], pID);
								BoneSetTextureID(player_bone[PLAYER_2ND], BODY_NAME::word[PN_RIGHTLEG2], pID2);
								BoneSetTextureID(player_bone[PLAYER_2ND], BODY_NAME::word[PN_LEFTLEG2], pID2);
								if (pID == catFoot1ID)
								{
									shop_Foot_2 = catLegID;
									pBody_2->AddMeat(-1 * catprice);
								}
								else if (pID == duckFoot1ID)
								{
									shop_Foot_2 = duckLegID;
									pBody_2->AddMeat(-1 * duckprice);
								}
							}
							else
								SOUNDMANAGER()->Play("denied");
						}
						else
							SOUNDMANAGER()->Play("denied");
					}
				}

			///////////////////////1P SHOPPART//////////////////////////////////
			if ((orthoMouse.x > 0.25 && orthoMouse.x < 0.45) && (orthoMouse.y > 0.25 && orthoMouse.y < 0.55))
			{
				GraphicSetTexture(Radio_PDA);
				GraphicSetFilpTex(JE::FLIP_IDLE, 1);
				GraphicSetAnimaTex(1, 0);
				GraphicSetOrthogTransform(orthoMouse.x - 0.28, orthoMouse.y + 0.15, 0, // x,y,z coordinate
					0.6, 0.3,//scale x,y
					0);//rotation

				if ((g_Input->IsTriggered(JE_MOUSE_LEFT)))
				{
					SOUNDMANAGER()->Play("click");
					partID = RADIO_HEAD;
					pID = radioHeadPartID;
					ShopCheck(radioHeadID);
				}
			}

			if ((orthoMouse.x > 0.54 && orthoMouse.x < 0.73) && (orthoMouse.y > 0.25 && orthoMouse.y < 0.55))
			{
				GraphicSetTexture(Lantern_PDA);
				GraphicSetFilpTex(JE::FLIP_IDLE, 1);
				GraphicSetAnimaTex(1, 0);
				GraphicSetOrthogTransform(orthoMouse.x - 0.28, orthoMouse.y + 0.15, 0, // x,y,z coordinate
					0.6, 0.3,//scale x,y
					0);//rotation

				if ((g_Input->IsTriggered(JE_MOUSE_LEFT)))
				{
					SOUNDMANAGER()->Play("click");
					partID = LANTERN_HEAD;
					pID = lightHeadPartID;
					ShopCheck(lightHeadID);
				}
			}

			if ((orthoMouse.x > 0.25 && orthoMouse.x < 0.45) && (orthoMouse.y > -0.14 && orthoMouse.y < 0.16))
			{
				GraphicSetTexture(good_PDA);
				GraphicSetFilpTex(JE::FLIP_IDLE, 1);
				GraphicSetAnimaTex(1, 0);
				GraphicSetOrthogTransform(orthoMouse.x - 0.28, orthoMouse.y + 0.15, 0, // x,y,z coordinate
					0.6, 0.3,//scale x,y
					0);//rotation

				if ((g_Input->IsTriggered(JE_MOUSE_LEFT)))
				{
					SOUNDMANAGER()->Play("click");
					partID = GOOD_BODY;
					pID = goodBodyPartID;
					ShopCheck(goodBodyID);
				}
			}

			if ((orthoMouse.x > 0.54 && orthoMouse.x < 0.73) && (orthoMouse.y > -0.14 && orthoMouse.y < 0.16))
			{
				GraphicSetTexture(bear_PDA);
				GraphicSetFilpTex(JE::FLIP_IDLE, 1);
				GraphicSetAnimaTex(1, 0);
				GraphicSetOrthogTransform(orthoMouse.x - 0.28, orthoMouse.y + 0.15, 0, // x,y,z coordinate
					0.6, 0.3,//scale x,y
					0);//rotation

				if ((g_Input->IsTriggered(JE_MOUSE_LEFT)))
				{
					SOUNDMANAGER()->Play("click");
					partID = BEAR_BODY;
					pID = bearBodyPartID;
					ShopCheck(bearBodyID);
				}
			}

			if ((orthoMouse.x > 0.25 && orthoMouse.x < 0.45) && (orthoMouse.y > -0.49 && orthoMouse.y < -0.23))
			{
				GraphicSetTexture(claw_PDA);
				GraphicSetFilpTex(JE::FLIP_IDLE, 1);
				GraphicSetAnimaTex(1, 0);
				GraphicSetOrthogTransform(orthoMouse.x - 0.28, orthoMouse.y + 0.15, 0, // x,y,z coordinate
					0.6, 0.3,//scale x,y
					0);//rotation

				if ((g_Input->IsTriggered(JE_MOUSE_LEFT)))
				{
					SOUNDMANAGER()->Play("click");
					partID = CLAW_ARM;
					pID = clawLeft1ID;
					pID2 = clawLeft2ID;
					pID3 = clawRight1ID;
					pID4 = clawRight2ID;
					ShopCheck(clawID);
				}
			}

			if ((orthoMouse.x > 0.54 && orthoMouse.x < 0.73) && (orthoMouse.y > -0.49 && orthoMouse.y < -0.23))
			{
				GraphicSetTexture(angelWing_PDA);
				GraphicSetFilpTex(JE::FLIP_IDLE, 1);
				GraphicSetAnimaTex(1, 0);
				GraphicSetOrthogTransform(orthoMouse.x - 0.28, orthoMouse.y + 0.15, 0, // x,y,z coordinate
					0.6, 0.3,//scale x,y
					0);//rotation

				if ((g_Input->IsTriggered(JE_MOUSE_LEFT)))
				{
					SOUNDMANAGER()->Play("click");
					partID = FEATHER_WING;
					pID = wingLeft1ID;
					pID2 = wingLeft2ID;
					pID3 = wingRight1ID;
					pID4 = wingRight2ID;
					ShopCheck(wingID);
				}
			}

			if ((orthoMouse.x > 0.25 && orthoMouse.x < 0.45) && (orthoMouse.y > -0.87 && orthoMouse.y < -0.55))
			{
				GraphicSetTexture(cat_PDA);
				GraphicSetFilpTex(JE::FLIP_IDLE, 1);
				GraphicSetAnimaTex(1, 0);
				GraphicSetOrthogTransform(orthoMouse.x - 0.28, orthoMouse.y + 0.15, 0, // x,y,z coordinate
					0.6, 0.3,//scale x,y
					0);//rotation

				if ((g_Input->IsTriggered(JE_MOUSE_LEFT)))
				{
					SOUNDMANAGER()->Play("click");
					partID = DASH_LEG/*JUMP_LEG*/;
					pID = catFoot1ID;
					pID2 = catFoot2ID;
					ShopCheck(catLegID);
				}
			}

			if ((orthoMouse.x > 0.54 && orthoMouse.x < 0.73) && (orthoMouse.y > -0.87 && orthoMouse.y < -0.55))
			{
				GraphicSetTexture(duck_PDA);
				GraphicSetFilpTex(JE::FLIP_IDLE, 1);
				GraphicSetAnimaTex(1, 0);
				GraphicSetOrthogTransform(orthoMouse.x - 0.28, orthoMouse.y + 0.15, 0, // x,y,z coordinate
					0.6, 0.3,//scale x,y
					0);//rotation

				if ((g_Input->IsTriggered(JE_MOUSE_LEFT)))
				{
					SOUNDMANAGER()->Play("click");
					partID = JUMP_LEG/*DASH_LEG*/;
					pID = duckFoot1ID;
					pID2 = duckFoot2ID;
					ShopCheck(duckLegID);
				}
			}

			///////////////////////2P SHOPPART//////////////////////////////////
			if ((padOrthoMouse.x > 0.25 && padOrthoMouse.x < 0.45) && (padOrthoMouse.y > 0.25 && padOrthoMouse.y < 0.55))
			{
				GraphicSetTexture(Radio_PDA);
				GraphicSetFilpTex(JE::FLIP_IDLE, 1);
				GraphicSetAnimaTex(1, 0);
				GraphicSetOrthogTransform(padOrthoMouse.x - 0.28, padOrthoMouse.y + 0.15, 0, // x,y,z coordinate
					0.6, 0.3,//scale x,y
					0);//rotation

				if (MyGamepad->GetButtonDown(XButtons.R_Shoulder))
				{
					SOUNDMANAGER()->Play("click");
					partID = RADIO_HEAD;
					pID = radioHeadPartID;
					PadShopCheck(radioHeadID);
				}
			}

			if ((padOrthoMouse.x > 0.54 && padOrthoMouse.x < 0.73) && (padOrthoMouse.y > 0.25 && padOrthoMouse.y < 0.55))
			{
				GraphicSetTexture(Lantern_PDA);
				GraphicSetFilpTex(JE::FLIP_IDLE, 1);
				GraphicSetAnimaTex(1, 0);
				GraphicSetOrthogTransform(padOrthoMouse.x - 0.28, padOrthoMouse.y + 0.15, 0, // x,y,z coordinate
					0.6, 0.3,//scale x,y
					0);//rotation

				if (MyGamepad->GetButtonDown(XButtons.R_Shoulder))
				{
					SOUNDMANAGER()->Play("click");
					partID = LANTERN_HEAD;
					pID = lightHeadPartID;
					PadShopCheck(lightHeadID);
				}
			}

			if ((padOrthoMouse.x > 0.25 && padOrthoMouse.x < 0.45) && (padOrthoMouse.y > -0.14 && padOrthoMouse.y < 0.16))
			{
				GraphicSetTexture(good_PDA);
				GraphicSetFilpTex(JE::FLIP_IDLE, 1);
				GraphicSetAnimaTex(1, 0);
				GraphicSetOrthogTransform(padOrthoMouse.x - 0.28, padOrthoMouse.y + 0.15, 0, // x,y,z coordinate
					0.6, 0.3,//scale x,y
					0);//rotation

				if (MyGamepad->GetButtonDown(XButtons.R_Shoulder))
				{
					SOUNDMANAGER()->Play("click");
					partID = GOOD_BODY;
					pID = goodBodyPartID;
					PadShopCheck(goodBodyID);
				}
			}

			if ((padOrthoMouse.x > 0.54 && padOrthoMouse.x < 0.73) && (padOrthoMouse.y > -0.14 && padOrthoMouse.y < 0.16))
			{
				GraphicSetTexture(bear_PDA);
				GraphicSetFilpTex(JE::FLIP_IDLE, 1);
				GraphicSetAnimaTex(1, 0);
				GraphicSetOrthogTransform(padOrthoMouse.x - 0.28, padOrthoMouse.y + 0.15, 0, // x,y,z coordinate
					0.6, 0.3,//scale x,y
					0);//rotation

				if (MyGamepad->GetButtonDown(XButtons.R_Shoulder))
				{
					SOUNDMANAGER()->Play("click");
					partID = BEAR_BODY;
					pID = bearBodyPartID;
					PadShopCheck(bearBodyID);
				}
			}

			if ((padOrthoMouse.x > 0.25 && padOrthoMouse.x < 0.45) && (padOrthoMouse.y > -0.49 && padOrthoMouse.y < -0.23))
			{
				GraphicSetTexture(claw_PDA);
				GraphicSetFilpTex(JE::FLIP_IDLE, 1);
				GraphicSetAnimaTex(1, 0);
				GraphicSetOrthogTransform(padOrthoMouse.x - 0.28, padOrthoMouse.y + 0.15, 0, // x,y,z coordinate
					0.6, 0.3,//scale x,y
					0);//rotation

				if (MyGamepad->GetButtonDown(XButtons.R_Shoulder))
				{
					SOUNDMANAGER()->Play("click");
					partID = CLAW_ARM;
					pID = clawLeft1ID;
					pID2 = clawLeft2ID;
					pID3 = clawRight1ID;
					pID4 = clawRight2ID;
					PadShopCheck(clawID);
				}
			}

			if ((padOrthoMouse.x > 0.54 && padOrthoMouse.x < 0.73) && (padOrthoMouse.y > -0.49 && padOrthoMouse.y < -0.23))
			{
				GraphicSetTexture(angelWing_PDA);
				GraphicSetFilpTex(JE::FLIP_IDLE, 1);
				GraphicSetAnimaTex(1, 0);
				GraphicSetOrthogTransform(padOrthoMouse.x - 0.28, padOrthoMouse.y + 0.15, 0, // x,y,z coordinate
					0.6, 0.3,//scale x,y
					0);//rotation

				if (MyGamepad->GetButtonDown(XButtons.R_Shoulder))
				{
					SOUNDMANAGER()->Play("click");
					partID = FEATHER_WING;
					pID = wingLeft1ID;
					pID2 = wingLeft2ID;
					pID3 = wingRight1ID;
					pID4 = wingRight2ID;
					PadShopCheck(wingID);
				}
			}

			if ((padOrthoMouse.x > 0.25 && padOrthoMouse.x < 0.45) && (padOrthoMouse.y > -0.87 && padOrthoMouse.y < -0.55))
			{
				GraphicSetTexture(cat_PDA);
				GraphicSetFilpTex(JE::FLIP_IDLE, 1);
				GraphicSetAnimaTex(1, 0);
				GraphicSetOrthogTransform(padOrthoMouse.x - 0.28, padOrthoMouse.y + 0.15, 0, // x,y,z coordinate
					0.6, 0.3,//scale x,y
					0);//rotation

				if (MyGamepad->GetButtonDown(XButtons.R_Shoulder))
				{
					SOUNDMANAGER()->Play("click");
					partID = DASH_LEG/*JUMP_LEG*/;
					pID = catFoot1ID;
					pID2 = catFoot2ID;
					PadShopCheck(catLegID);
				}
			}

			if ((padOrthoMouse.x > 0.54 && padOrthoMouse.x < 0.73) && (padOrthoMouse.y > -0.87 && padOrthoMouse.y < -0.55))
			{
				GraphicSetTexture(duck_PDA);
				GraphicSetFilpTex(JE::FLIP_IDLE, 1);
				GraphicSetAnimaTex(1, 0);
				GraphicSetOrthogTransform(padOrthoMouse.x - 0.28, padOrthoMouse.y + 0.15, 0, // x,y,z coordinate
					0.6, 0.3,//scale x,y
					0);//rotation

				if (MyGamepad->GetButtonDown(XButtons.R_Shoulder))
				{
					SOUNDMANAGER()->Play("click");
					partID = JUMP_LEG/*DASH_LEG*/;
					pID = duckFoot1ID;
					pID2 = duckFoot2ID;
					PadShopCheck(duckLegID);
				}
			}
		}

		else if (!shopOn && (!ShopChk || !PadShopChk))
		{
			ShopCheck(tempID);
			PadShopCheck(tempID);
		}


		return shopOn;
	}

	void ShopDraw()
	{
		

		for (int i = 0; i < 2; i++)
		{
			const double gaugePosition = GAUGE_POSITION.x - 0.06 + (i * GAUGE_PLAYERS_SPACE);
			Resource resource = player[i]->GetResource();

			GraphicSetTexture(resourceID.bread);
			GraphicSetFilpTex(JE::FLIP_IDLE, 1);
			GraphicSetAnimaTex(1, 0);
			GraphicSetOrthogTransform(gaugePosition, GAUGE_POSITION.y, 0, // x,y,z coordinate
				0.05, 0.05,//scale x,y
				0);//rotation

			GraphicSetTexture(resourceID.mineral);
			GraphicSetFilpTex(JE::FLIP_IDLE, 1);
			GraphicSetAnimaTex(1, 0);
			GraphicSetOrthogTransform(gaugePosition, GAUGE_POSITION.y - GAUGE_Y_SPACE, 0, // x,y,z coordinate
				0.05, 0.05,//scale x,y
				0);//rotation

			GraphicSetTexture(resourceID.meat);
			GraphicSetFilpTex(JE::FLIP_IDLE, 1);
			GraphicSetAnimaTex(1, 0);
			GraphicSetOrthogTransform(gaugePosition, GAUGE_POSITION.y - (GAUGE_Y_SPACE * 2), 0, // x,y,z coordinate
				0.05, 0.05,//scale x,y
				0);//rotation
			
		}

		if (shopOn)
		{
			///////////////// SHOP BOARD ///////////////////////////
			GraphicSetTexture(shopBoardID);
			GraphicSetFilpTex(JE::FLIP_IDLE, 1);
			GraphicSetAnimaTex(1, 0);
			GraphicSetOrthogTransform(0, -0.1, 0, // x,y,z coordinate
				1.8, 1.8,//scale x,y
				0);//rotation

			BoneDrawHUD(player_bone[PLAYER_1ST], -0.4, 0.3, 0);
			BoneDrawHUD(player_bone[PLAYER_2ND], -0.4, -0.55, 0);

			/////////////////////////// Parts menus below ///////////////////////////////////////
			GraphicSetTexture(rectangleID);
			GraphicSetAnimaTex(1, 0);
			GraphicSetOrthogTransform(selectBoxImage[PartIndex].x, selectBoxImage[PartIndex].y, 0,//x,y coordinate z order
				CUSTOM_BOX_SCALE / 2, CUSTOM_BOX_SCALE / 2,//scale x,y
				0);//rotation

			GraphicSetTexture(rectangleID);
			GraphicSetAnimaTex(1, 0);
			GraphicSetOrthogTransform(selectBoxImage[PadPartIndex].x, selectBoxImage[PadPartIndex].y, 0,//x,y coordinate z order
				CUSTOM_BOX_SCALE / 2, CUSTOM_BOX_SCALE / 2,//scale x,y
				0);//rotation
			
		}
	}
	void DrawMouse(bool pause, JEPoint2D* mousePosition, JEPoint2D* padCursorPosition, const double dt, int cursorID)
	{
		if (shopOn || !pause)
		{
			// 1P
			GraphicSetTexture(cursorID);
			GraphicSetFilpTex(JE::FLIP_IDLE, 1);
			GraphicSetAnimaTex(1, 0);
			if (ShopChk)
				GraphicSetOrthogTransform(mousePosition->x, mousePosition->y, 0.f, // x,y,z coordinate
					0.1f, 0.1f,//scale x,y
					0);
			else
				GraphicSetOrthogTransform(mousePosition->x, mousePosition->y, 0.f, // x,y,z coordinate
					0.2f, 0.2f,//scale x,y
					0.f);


			if (gd.padAlpha > 0.f)
			{
				// 2P
				GraphicSetTexture(padPointID);
				GraphicSetFilpTex(JE::FLIP_IDLE, 1);
				GraphicSetAnimaTex(1, 0);
				if (PadShopChk)
					GraphicSetOrthoParticleTransform(padCursorPosition->x, padCursorPosition->y, 0.f, // x,y,z coordinate
					0.1f, 0.1f,//scale x,y
					0.f, gd.padAlpha);
				else
					GraphicSetOrthoParticleTransform(padCursorPosition->x, padCursorPosition->y, 0.f, // x,y,z coordinate
					0.2f, 0.2f,//scale x,y
					0.f, gd.padAlpha);
			}
		}

		if (gd.padConnected == false && MyGamepad->Connected())
		{
			gd.padTime += dt;
			if (gd.padTime > 1.f)
				gd.padConnected = true;

			if (gd.padAlpha < 1.f)
				gd.padAlpha += static_cast<float>(dt);

			GraphicSetTexture(connectedID);
			GraphicSetFilpTex(JE::FLIP_IDLE, 1);
			GraphicSetAnimaTex(1, 0);
			GraphicSetOrthogTransform(0.f, 0.f, 0.f, // x,y,z coordinate
				0.4f, 0.15f,//scale x,y
				0.f);
		}
		else if (gd.padConnected == true && !MyGamepad->Connected())
		{
			gd.padTime -= dt;
			if (gd.padTime < 0.f)
				gd.padConnected = false;

			if (gd.padAlpha > 0.f)
				gd.padAlpha -= static_cast<float>(dt);

			GraphicSetTexture(disConnectedID);
			GraphicSetFilpTex(JE::FLIP_IDLE, 1);
			GraphicSetAnimaTex(1, 0);
			GraphicSetOrthogTransform(0.f, 0.f, 0.f, // x,y,z coordinate
				0.4f, 0.15f,//scale x,y
				0.f);
		}
	}
	/*******************************************************************************
	Function: ShopShutdown

	Description: Reset the variables and setting. Empty function yet.

	Inputs: None.

	Outputs: None.
	*******************************************************************************/
	void ShopShutdown()
	{
	}
	/*******************************************************************************
	Function: ShopUnload

	Description: Release the resources. Empty function yet.

	Inputs: None.

	Outputs: None.
	*******************************************************************************/
	void ShopUnload()
	{
		GraphicUnloadTexture(connectedID);
		GraphicUnloadTexture(disConnectedID);
		GraphicUnloadTexture(crystalID);

		GraphicUnloadTexture(clawLeft2ID);
		GraphicUnloadTexture(clawRight2ID);
		GraphicUnloadTexture(clawLeft1ID);
		GraphicUnloadTexture(clawRight1ID);

		GraphicUnloadTexture(wingLeft2ID); 
		GraphicUnloadTexture(wingRight2ID);
		GraphicUnloadTexture(wingLeft1ID);
		GraphicUnloadTexture(wingRight1ID);

		GraphicUnloadTexture(catFoot1ID);
		GraphicUnloadTexture(catFoot2ID);
		GraphicUnloadTexture(duckFoot1ID);
		GraphicUnloadTexture(duckFoot2ID);
		GraphicUnloadTexture(angelWing1ID);
		GraphicUnloadTexture(angelWing2ID);

		GraphicUnloadTexture(shopBoardID);
		GraphicUnloadTexture(rectangleID);

		GraphicUnloadTexture(mousePointID); 
		GraphicUnloadTexture(padPointID);
		GraphicUnloadTexture(radioHeadPartID);
		GraphicUnloadTexture(radioHeadID); 
		GraphicUnloadTexture(lightHeadPartID);
		GraphicUnloadTexture(lightHeadID); 
		GraphicUnloadTexture(bearBodyPartID);
		GraphicUnloadTexture(bearBodyID);
		GraphicUnloadTexture(goodBodyPartID); 
		GraphicUnloadTexture(goodBodyID);
		GraphicUnloadTexture(clawID);
		GraphicUnloadTexture(wingID);
		GraphicUnloadTexture(catLegID);
		GraphicUnloadTexture(duckLegID);

		GraphicUnloadTexture(angelWing_PDA);
		GraphicUnloadTexture(claw_PDA);
		GraphicUnloadTexture(Lantern_PDA);
		GraphicUnloadTexture(Radio_PDA);
		GraphicUnloadTexture(good_PDA);
		GraphicUnloadTexture(bear_PDA);
		GraphicUnloadTexture(cat_PDA);
		GraphicUnloadTexture(duck_PDA);
	}

	/*******************************************************************************
	Function: GetRect

	Description: Get the rectangle array index by checking mouse position.

	Inputs: mouse - mouse position from the state.
	part - the current part box.

	Outputs: PARTS type variable.
	*******************************************************************************/
	BODY_PART GetRect(const JEPoint2D& mouse, BODY_PART part)
	{
		if (mouse.y >= selectBox[0].y - HALF_SIDE_LENGTH && mouse.y < selectBox[0].y + HALF_SIDE_LENGTH)
		{
			if ((mouse.x >= selectBox[0].x - HALF_SIDE_LENGTH && mouse.x < selectBox[0].x + HALF_SIDE_LENGTH)
				&& g_Input->IsTriggered(JE_MOUSE_LEFT))
			{
				SOUNDMANAGER()->Play("shop");
				return PART_HEAD;
			}
		}
		else if (mouse.y >= selectBox[1].y - HALF_SIDE_LENGTH && mouse.y < selectBox[1].y + HALF_SIDE_LENGTH)
		{
			if ((mouse.x >= selectBox[1].x - HALF_SIDE_LENGTH && mouse.x < selectBox[1].x + HALF_SIDE_LENGTH)
				&& g_Input->IsTriggered(JE_MOUSE_LEFT))
			{
				SOUNDMANAGER()->Play("shop");
				return PART_ARM;
			}

		}
		else if (mouse.y >= selectBox[3].y - HALF_SIDE_LENGTH && mouse.y < selectBox[3].y + HALF_SIDE_LENGTH)
		{
			if ((mouse.x >= selectBox[3].x - HALF_SIDE_LENGTH && mouse.x < selectBox[3].x + HALF_SIDE_LENGTH)
				&& g_Input->IsTriggered(JE_MOUSE_LEFT))
			{
				SOUNDMANAGER()->Play("shop");
				return PART_LEG;
			}
		}
		else if (mouse.y >= selectBox[3].y - HALF_SIDE_LENGTH && mouse.y < selectBox[3].y + HALF_SIDE_LENGTH)
		{
			if ((mouse.x >= selectBox[3].x - HALF_SIDE_LENGTH && mouse.x < selectBox[3].x + HALF_SIDE_LENGTH)
				&& g_Input->IsTriggered(JE_MOUSE_LEFT))
			{
				SOUNDMANAGER()->Play("shop");
				return PART_BODY;
			}
		}
		return part;
	}
}