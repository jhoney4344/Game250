/*******************************************************************************
Copyright (C) 2015 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
filename    State_OpenWorld.cpp
author      Jaeheon Lee
			Minsu Kang
			Seho.Joo
email       jhoney4344@gmail.com
			minsu9486@gmail.com
			jsh2958@gamil.com
course      GAM250

*******************************************************************************/
#include "JEStates.h"
#include "JEInput.h"
#include "JEDebug.h"
#include "JELoadTextureNew.h"
#include "JEGraphicsNew.h"
#include "JEStateManager.h"
#include "JEHUD.h"
#include "JEShop.h"
#include "JECharacter.h"
#include "JETileMap.h"
#include "JEResourceGauge.h"
#include "JEOptionMenu.h"
#include "JEGameData.h"
#include "JEDebug.h"
#include "JECharacterManager.h"
#include "JECharacter.h"
#include "JEApplication.h"
#include "JEDayAndNight.h"
#include "JEOldParticle.h"

//#include "JECamera.h"

extern JE::GameData gd;
struct ButtonObject
{
	ButtonObject() {};
	ButtonObject(int id, double x, double y, double w, double h, float a = 0.5f)
		: ID(id), centerPosition(x, y), width(w), height(h), alpha(a) {}

	int ID;

	JEPoint2D centerPosition;
	double width;
	double height;
	float alpha;
};

namespace{
	double save_dt;
	JE::ResourceID resourceID;
	int mousePointID;
	int loadingID;
	bool restartState;
	bool showLoading;
	JEPoint2D cameraPosition;
	JEPoint2D mousePosition;
	JEPoint2D padCursorPosition;
	double stateTotalTime;

	JE::Character*player[2];
	JEPoint2D originCapsule;
	double capsuleLandY;
	double capsuleSize;
	double capsuleAangle;

	int backgroundID;
	int bugID;
	TileMapManager* pTileMapManager;
	ResourseGaugeTexture resourceGauge;
	JE::DayNight* pDayNight;
	int redDotID;

	JEPoint2D background[3];
	float backgroundWidth;
	float backgroundHeight;

	int rightBG;
	int leftBG;
	bool openShop;
	bool openMenu;
	int  mainMenuToPressID;
	int  restartToPressID;
	int  startToPressID;
	bool startToPress;
	JE::Tomb*pTomb;

	bool mainMenuReasking;
	ButtonObject mainMenuAsking;
	ButtonObject mainMenuYesButton;
	ButtonObject mainMenuNoButton;

	particleSystem* pParticleSys;
	int dirtID;
	int smokeID;
	int bloodID;
	int rainBloodID;
	int flyID;

	int lastMessageID;
	int surviveMessageID;
	const double ANIMATION_TIME = 0.15f;
	double survieAniTime = 0.f;
	double aniRateInc = .5f;
	double currAniRate = 0;
	int cloudID;
	JEPoint2D cloudBG[3];
	static int endcon;
	static int bombcon;
}

namespace JE
{
	/*******************************************************************************
	Function: State_Lab::Load

	Description: Load the resources.

	Inputs: None.

	Outputs: None.
	*******************************************************************************/
	void State_OpenWorld::Load()
	{
		// Resource Textures
		ResourceSystem::Load();
		ShopLoad();
		HUD::Load();
		CharacterManager::Load();
		OptionLoad();
		ObjectManager::Load();

		// this system should be excute after ResourceSystem::LOAD
		resourceGauge.TextureLoad();
		mousePointID = JE::GraphicLoadTexture("Texture//mouse.tga");
		loadingID = JE::GraphicLoadTexture("Texture//LoadingMSG.tga");
		mainMenuToPressID = JE::GraphicLoadTexture("Texture//HUD_PressToMainMenu.tga");
		restartToPressID = GraphicLoadTexture("Texture//HUD_PressToRestart.tga");
		startToPressID = GraphicLoadTexture("Texture//HUD_PressToStart.tga");
		redDotID = GraphicLoadTexture("Texture//Dot.tga");
		backgroundID = GraphicLoadTexture("Texture//Background.tga");
		bugID = GraphicLoadTexture("Texture//bug.tga");
		cloudID = GraphicLoadTexture("Texture//Cloud.tga");
		dirtID = GraphicLoadTexture("Texture//TY_Dirt.tga");
		smokeID = GraphicLoadTexture("Texture//particleSmoke.tga");
		bloodID = GraphicLoadTexture("Texture//particleBlood.tga");
		rainBloodID = GraphicLoadTexture("Texture//rainParticle.tga");
		flyID = GraphicLoadTexture("Texture//ParticleFly.tga");

		mainMenuAsking.ID = JE::GraphicLoadTexture("Texture//HUD_MainMenu_Ask.tga");
		mainMenuYesButton.ID = JE::GraphicLoadTexture("Texture//Button_Exit_Ask_Yes.tga");
		mainMenuNoButton.ID = JE::GraphicLoadTexture("Texture//Button_Exit_Ask_No.tga");

		lastMessageID = GraphicLoadTexture("Texture//OpenWorldLast.tga");
		surviveMessageID = GraphicLoadTexture("Texture//openWorldSurvive.tga");
		SOUNDMANAGER()->LoadSound();
	}

	/*******************************************************************************
	Function: State_Lab::Init

	Description: Initializing function. Empty.

	Inputs: None.

	Outputs: None.
	*******************************************************************************/
	void State_OpenWorld::Init()
	{
		JEPRINTF("State_OpenWorld Init\n");		
		stateTotalTime = 0;
		restartState = false;
		showLoading = false;
		SOUNDMANAGER()->Play("bgmday");
		SOUNDMANAGER()->Play("bgmnight");
		SOUNDMANAGER()->BGMNightMute();
		for (int i = 0; i < 10; i++)
			SOUNDMANAGER()->BGMVolumeDown();
		if (gd.muteBGM)
		{
			SOUNDMANAGER()->BGMNightMute();
			SOUNDMANAGER()->BGMDayMute();
		}
		if (gd.muteEFT)
			SOUNDMANAGER()->EFTMute();
		CharacterManager::Init();
		ShopInit(); // Shop should init after character
		HUD::Init();
		
		OptionInit();
		
		GraphicInitCamera();
		cameraPosition = JEPoint2D(0, 0);
		JE::GraphicCameraZoom(false, 50.f);
		padCursorPosition = JEPoint2D();

		pTileMapManager = &GetTileMapManager();
		pTileMapManager->InitTiles_OpenWorld(cameraPosition);
		resourceID = ResourceSystem::GetResourceID();

		// Set the players' position just on the ground
		CharacterManager::GetPlayerByID(PLAYER_1ST).GetProperty().position
			= ConvertTileToWorldPosition(TilePosition(0, GetLandHeight(0))) + JEPoint2D(0, 5);
		CharacterManager::GetPlayerByID(PLAYER_2ND).GetProperty().position
			= ConvertTileToWorldPosition(TilePosition(1, GetLandHeight(1))) + JEPoint2D(0, 5);

		// The Original Capsule
		originCapsule = JEPoint2D(CharacterManager::GetPlayerByID(PLAYER_2ND).GetProperty().position.x, 1750.f);
		SOUNDMANAGER()->Play("drop");
		if (originCapsule.x > -10.f)
		{
			originCapsule.x += TILE_SPACE;
			originCapsule.x -= TILE_SPACE;
		}
		originCapsule.x -= TILE_SPACE / 2;
		capsuleLandY = CharacterManager::GetPlayerByID(PLAYER_2ND).GetProperty().position.y + 1.2f;
		capsuleSize = 5.f;
		capsuleAangle = 40.f;

		backgroundWidth = 2048.f;
		backgroundHeight = 1024.f;

		gd.background[0] = JEPoint2D((double)-backgroundWidth, backgroundHeight*2/5);
		gd.background[1] = JEPoint2D(0, backgroundHeight * 2 / 5);
		gd.background[2] = JEPoint2D((double)backgroundWidth, backgroundHeight * 2 / 5);

		cloudBG[0] = JEPoint2D((double)-backgroundWidth, 150);
		cloudBG[1] = JEPoint2D(0, 150);
		cloudBG[2] = JEPoint2D((double)backgroundWidth, 150);

		rightBG = 2;
		leftBG = 0;
		
		JE::JEInitLight();
		SetCharacters_Obj();
	
		pDayNight = &(getDayNight());
		pDayNight->InitDayNight();

		for (CHARACTER_ID id = PLAYER_1ST; id < PLAYER_END;
			id = static_cast<CHARACTER_ID>(static_cast<int>(id)+1))
		{
			player[id] = &CharacterManager::GetPlayerByID(id);
			JE::Vector3f characterPosition(float(player[id]->GetPosition().x),
				float(player[id]->GetPosition().y),
				-30.f);
			int point = reinterpret_cast<int>(player[id]);
			pDayNight->InitSpotLight(id, point, characterPosition, 0, 60);
		}

		//for particle test
		pParticleSys = &getParticleSystem();
		pParticleSys->loadParticleTexture(DIRT, dirtID);
		pParticleSys->loadParticleTexture(SMOKE, smokeID);
		pParticleSys->loadParticleTexture(BLOOD, bloodID);
		pParticleSys->loadParticleTexture(FIREFLY, smokeID);
		pParticleSys->loadParticleTexture(PJUMP, smokeID);
		pParticleSys->loadParticleTexture(PRAIN, rainBloodID); 
		pParticleSys->loadParticleTexture(PFLY, flyID);

		//pParticleSys->loadParticleTexture

		ShowCursor(false);
		save_dt = 0.016;
		gd.pause = true;
		openShop = false;
		openMenu = false;
		startToPress = false;

		mainMenuReasking = false;
		mainMenuAsking = ButtonObject(mainMenuAsking.ID, 0.f, 0.f, 1.f, 0.6f, 1.f);
		mainMenuYesButton = ButtonObject(mainMenuYesButton.ID, -0.25f, -0.1f, 0.38f, 0.225f);
		mainMenuNoButton = ButtonObject(mainMenuNoButton.ID, 0.25f, -0.1f, 0.38f, 0.225f);

		endcon = 0;
	}
	/*******************************************************************************
	Function: State_Lab::Update

	Description: Excute this function every frame until exit the state.

	Inputs: dt - the frame time. At least 1/60sec

	Outputs: None.
	*******************************************************************************/
	void State_OpenWorld::Update(double dt, bool& fullScreen)
	{
		SOUNDMANAGER()->Update();
		gd.DayNight = pDayNight->GetCurrState();
		JE_UNUSED_PARAM(fullScreen);
		stateTotalTime += dt;
		
		// Mouse cursor update
		mousePosition.x = g_Input->GetMouse_x();
		mousePosition.y = g_Input->GetMouse_y();

		// Pad cursor update
		if (MyGamepad->Connected())
		{
			if (std::abs(MyGamepad->LeftStick_X()) > 0.5f)
			{
				if (-1.f > padCursorPosition.x)
					padCursorPosition.x = -1.f;

				if (padCursorPosition.x > 1.f)
					padCursorPosition.x = 1.f;

				padCursorPosition.x += MyGamepad->LeftStick_X() / 100.f;
			}
			else
			{
				if (MyGamepad->GetButtonPressed(XButtons.DPad_Left)
					&& -1.f < padCursorPosition.x)
					padCursorPosition.x -= 0.025f;

				if (MyGamepad->GetButtonPressed(XButtons.DPad_Right)
					&& padCursorPosition.x < 1.f)
					padCursorPosition.x += 0.025f;
			}

			if (std::abs(MyGamepad->LeftStick_Y()) > 0.5f)
			{
				if (-1.f > padCursorPosition.y)
					padCursorPosition.y = -1.f;

				if (padCursorPosition.y > 1.f)
					padCursorPosition.y = 1.f;

				padCursorPosition.y += MyGamepad->LeftStick_Y() / 75.f;
			}
			else
			{
				if (MyGamepad->GetButtonPressed(XButtons.DPad_Down)
					&& -1.f < padCursorPosition.y)
					padCursorPosition.y -= 0.03f;

				if (MyGamepad->GetButtonPressed(XButtons.DPad_Up)
					&& padCursorPosition.y < 1.f)
					padCursorPosition.y += 0.03f;
			}
		}
		//JEPRINTF("%f, %f\n", padCursorPosition.x, padCursorPosition.y);


		GraphicConvertWorldToScreen(mousePosition.x, mousePosition.y);

		// For the first time
		if (!startToPress)
		{
			// Camera Update
			cameraPosition = originCapsule;

			// Capcule Update
			if (capsuleLandY < originCapsule.y)
				originCapsule.y -= (dt * 250.f);
			else
			{

				while (bombcon == 0)
				{
					bombcon++;
					SOUNDMANAGER()->Play("grb");
					if (bombcon == 1)
						break;
				}

				originCapsule.y = capsuleLandY;
				if (g_Input->IsAnyTriggered() || MyGamepad->AnyButtonPressed())
				{
					JE::ObjectManager::AddObject(JE::OBJECTS::OBJ_SHOP,
					new JE::Shop(JEPoint2D(CharacterManager::GetPlayerByID(PLAYER_2ND).GetProperty().position)));

					gd.pause = !gd.pause;
					startToPress = !startToPress;
				}
			
			}

			if (capsuleSize < 18.f)
				capsuleSize += (dt * 2.f);
			else
				capsuleSize = 18.f;

			if (capsuleAangle > 0.f)
				capsuleAangle -= (dt * 7.5f);
			else
				capsuleAangle = 0.f;

		}
		// During the game playing
		else
		{
			// Cheat code for free camera
			if (g_Input->IsPressed(JE_CONTROL))
			{
				if (g_Input->IsPressed(JE_NUMPAD8))
					cameraPosition.y += 5;
				if (g_Input->IsPressed(JE_NUMPAD2))
					cameraPosition.y -= 5;
				if (g_Input->IsPressed(JE_NUMPAD6))
					cameraPosition.x += 5;
				if (g_Input->IsPressed(JE_NUMPAD4))
					cameraPosition.x -= 5;

				if (g_Input->IsPressed(JE_NUMPAD5))
					cameraPosition = pTileMapManager->GetMolePosition();
			}
			// Camera on a player
			else
			{
				const JEPoint2D player1Position = CharacterManager::GetPlayerByID(PLAYER_1ST).GetProperty().position;
				const JEPoint2D player2Position = CharacterManager::GetPlayerByID(PLAYER_2ND).GetProperty().position;

				// Mid Position
				if (Character::live1 && Character::live2)
				{
					const JEPoint2D subPosition = player1Position - player2Position;
					const JEPoint2D distance = JEPoint2D(subPosition.x / 2.f, subPosition.y / 2.f);

					const double d = std::sqrt(distance.x * distance.x
						+ distance.y + distance.y) - 20.f;

					// Zoom in/out (10 = CAMERA_Adjustment)
					if (50.f - 10 < d && d < 75.f)
					{
						Vector3f cameraPos = GraphicGetCameraPos() - Vector3f(0.f, 20, 0.f);
						JE::GraphicCameraZoom(false, float(d) + 10 + cameraPos.m_z);
					}

					cameraPosition = player1Position - distance;
				}
				// Player 1
				else if (Character::live1 && !Character::live2)
				{
					cameraPosition = player1Position;
					GraphicSetCameraPos(Vector3f(static_cast<float>(cameraPosition.x), static_cast<float>(cameraPosition.y), -50.f));
				}
				// Player 2
				else if (!Character::live1 && Character::live2)
				{
					cameraPosition = player2Position;
					GraphicSetCameraPos(Vector3f(static_cast<float>(cameraPosition.x), static_cast<float>(cameraPosition.y), -50.f));
				}
			}

			// For emergency escape
			if (gd.pause && !openMenu)
				openMenu = !openMenu;

			if ((g_Input->IsTriggered(JE_ESCAPE) || MyGamepad->GetButtonDown(XButtons.Back)) && openShop == false)
			{
				gd.pause = !gd.pause;
				openMenu = !openMenu;
				OptionClose();
			}
		}

		GraphicCameraPosition(float(cameraPosition.x), float(cameraPosition.y));
		
		pTileMapManager->UpdateOpenWorldTiles(cameraPosition, stateTotalTime, pDayNight->GetCurrState(), dt);

		GraphicStartDraw();

		GraphicSetBackgroundColor(0, 0, 0); // Black = Space
		JEUseObjShader();

		//BACKGROUND WRAPPING
		if (rightBG < 0)
			rightBG = 2;
		if (leftBG > 2)
			leftBG = 0;

		if (cameraPosition.x < gd.background[leftBG].x)
		{
			gd.background[rightBG].x -= backgroundWidth;
			leftBG = rightBG;
			rightBG--;
		}
		if (cameraPosition.x > gd.background[rightBG].x)
		{
			gd.background[leftBG].x += backgroundWidth;
			rightBG = leftBG;
			leftBG++;
		}

		// back ground
		for (int i = 0; i < 3; i++)
		{
			GraphicSetTextureUnit(0);
			GraphicSetTexture(backgroundID);
			GraphicSetFilpTex(FLIP_IDLE, 1);
			GraphicSetAnimaTex(1, 0);
			GraphicSetPersTransform(gd.background[i].x, gd.background[i].y, 0.f,	//x,y coordinate z order
				backgroundWidth, backgroundHeight,		//scale x,y
				0.f);
			GraphicSetTextureUnit(0);
			GraphicSetTexture(cloudID);
			GraphicSetFilpTex(FLIP_IDLE, 1);
			GraphicSetAnimaTex(1, 0);
			GraphicSetPersTransform(gd.background[i].x, cloudBG[i].y, 0.f,	//x,y coordinate z order
				backgroundWidth, backgroundHeight,		//scale x,y
				0.f);
		}

		pTileMapManager->DrawTiles(cameraPosition);
		if (!gd.pause)
		{
			if (!gd.muteBGM)
			{
				SOUNDMANAGER()->BGMVolumeUp();
			}
			if (!gd.muteEFT)
				SOUNDMANAGER()->EFTVolumeUp();

			if (!openShop)
			{
				bool live[2] = { player[0]->live1, player[1]->live2};

				CharacterManager::Update(dt);
				ObjectManager::Update(dt, pDayNight->GetCurrState(), Vector3f(float(cameraPosition.x), float(cameraPosition.y), 0.f));
				AniSystem::Update();

				pParticleSys->updateParticleSystem(float(dt));

				pDayNight->UpdateDayNight(dt, JE::Vector3f(float(cameraPosition.x), float(cameraPosition.y), 0.0f));

				for (CHARACTER_ID id = PLAYER_1ST; id < PLAYER_END;
					id = static_cast<CHARACTER_ID>(static_cast<int>(id)+1))
				{
					Vector3f characterPosition(float(player[id]->GetPosition().x),
						float(player[id]->GetPosition().y),
						-10.f);

					pDayNight->ChangeSpotLight(id, live[id], characterPosition);
				}

			}
		}
		else
		{
			if (gd.windowInactive || startToPress)
			{
				SOUNDMANAGER()->BGMVolumeDown();
				SOUNDMANAGER()->EFTVolumeDown();
			}
			MyGamepad->Rumble(0.f, 0.f);
		}

		ObjectManager::Draw(dt);

		// The Epic screen
		if (!startToPress)
		{	
			GraphicSetTextureUnit(0);
			GraphicSetTexture(JE::ObjectManager::GetObjID()[OBJ_SHOP]);
			GraphicSetFilpTex(FLIP_IDLE, 1);
			GraphicSetAnimaTex(1, 0);
			GraphicSetPersTransform(originCapsule.x, originCapsule.y, 0.f,	//x,y coordinate z order
				capsuleSize, capsuleSize,		//scale x,y
				capsuleAangle);

			if (originCapsule.y == capsuleLandY)
			{
				survieAniTime += dt;
				if (survieAniTime > ANIMATION_TIME)
				{
					currAniRate += aniRateInc;
					survieAniTime = 0;
				}

				GraphicSetTextureUnit(0);
				GraphicSetTexture(startToPressID);
				GraphicSetFilpTex(FLIP_IDLE, 1);
				GraphicSetAnimaTex(1, 0);
				GraphicSetOrthogTransform(0.f, -0.65f, 0.f,	//x,y coordinate z order
					0.8f, 0.3f,		//scale x,y
					0.f);

				GraphicSetTextureUnit(0);
				GraphicSetTexture(lastMessageID);
				GraphicSetFilpTex(FLIP_IDLE, 1);
				GraphicSetAnimaTex(1, 0);
				GraphicSetPersTransform(cameraPosition.x + 25, cameraPosition.y + 30, 0.f,	//x,y coordinate z order
					60, 30,		//scale x,y
					0.f);

				GraphicSetTextureUnit(0);
				GraphicSetTexture(surviveMessageID);
				GraphicSetFilpTex(FLIP_IDLE, 1);
				GraphicSetAnimaTex(2, float(currAniRate));
				GraphicSetPersTransform(cameraPosition.x - 30, cameraPosition.y + 30, 0.f,	//x,y coordinate z order
					60, 30,		//scale x,y
					0.f);
			}
		}
		// Game Play
		else
			CharacterManager::Draw();

		resourceGauge.DrawResourceGauge(pDayNight->GetCurrState());
		ShopDraw();
		openShop = ShopUpdate(mousePosition, padCursorPosition);

		// Dead condition screen
		if (!player[CHARACTER_ID::PLAYER_1ST]->live1 && !player[CHARACTER_ID::PLAYER_2ND]->live2)
		{
			if (restartState)
				StateManager::RestartState();

			while (endcon == 0)
			{
				endcon++;
				SOUNDMANAGER()->Play("gameover");
				if (endcon == 1)
					break;
			}

			// Restart
			GraphicSetTextureUnit(0);
			GraphicSetTexture(restartToPressID);
			GraphicSetFilpTex(FLIP_IDLE, 1);
			GraphicSetAnimaTex(1, 0);
			GraphicSetOrthogTransform(0.f, -0.65f, 0.f,	//x,y coordinate z order
				0.8f, 0.3f,		//scale x,y
				0.f);

			// Main Menu
			GraphicSetTextureUnit(0);
			GraphicSetTexture(mainMenuToPressID);
			GraphicSetFilpTex(FLIP_IDLE, 1);
			GraphicSetAnimaTex(1, 0);
			GraphicSetOrthogTransform(-0.75f, -0.85f, 0.f,	//x,y coordinate z order
				0.5f, 0.3f,		//scale x,y
				0.f);

			// Main Menu
			if (g_Input->IsTriggered(JE_BACKSPACE) || MyGamepad->GetButtonDown(XButtons.Back))
				mainMenuReasking = true;

			// mainMenuReasking == true
			if (mainMenuReasking)
			{
				// Exit_Yes Button
				if (g_Collision->IntersectionPointRect(mousePosition, mainMenuYesButton.centerPosition, mainMenuYesButton.width, mainMenuYesButton.height)
					|| g_Collision->IntersectionPointRect(padCursorPosition, mainMenuYesButton.centerPosition, mainMenuYesButton.width, mainMenuYesButton.height))
				{
					if (mainMenuYesButton.alpha < 3.f)
						mainMenuYesButton.alpha += static_cast<float>(dt * 2.f);

					if (g_Input->IsTriggered(JE_MOUSE_LEFT) || MyGamepad->GetButtonDown(XButtons.R_Shoulder))
						StateManager::SetNextState(JE::MENU);
				}
				else
				{
					if (mainMenuYesButton.alpha > 0.5f)
						mainMenuYesButton.alpha -= static_cast<float>(dt * 4.f);
				}

				// Exit_No Button
				if (g_Collision->IntersectionPointRect(mousePosition, mainMenuNoButton.centerPosition, mainMenuNoButton.width, mainMenuNoButton.height)
					|| g_Collision->IntersectionPointRect(padCursorPosition, mainMenuNoButton.centerPosition, mainMenuNoButton.width, mainMenuNoButton.height))
				{
					if (mainMenuNoButton.alpha < 3.f)
						mainMenuNoButton.alpha += static_cast<float>(dt * 2.f);

					if (g_Input->IsTriggered(JE_MOUSE_LEFT) || MyGamepad->GetButtonDown(XButtons.R_Shoulder))
						mainMenuReasking = false;
				}
				else
				{
					if (mainMenuNoButton.alpha > 0.5f)
						mainMenuNoButton.alpha -= static_cast<float>(dt * 4.f);
				}

				GraphicSetTexture(mainMenuAsking.ID);
				GraphicSetFilpTex(FLIP_IDLE, 1);
				GraphicSetAnimaTex(1, 0);
				GraphicSetOrthoParticleTransform(mainMenuAsking.centerPosition.x, //x,y coordinate z order
					mainMenuAsking.centerPosition.y, 0.f,
					mainMenuAsking.width, //scale x,y
					mainMenuAsking.height,
					0.f, mainMenuAsking.alpha);	//Rotation

				GraphicSetTexture(mainMenuYesButton.ID);
				GraphicSetFilpTex(FLIP_IDLE, 1);
				GraphicSetAnimaTex(1, 0);
				GraphicSetOrthoParticleTransform(mainMenuYesButton.centerPosition.x, //x,y coordinate z order
					mainMenuYesButton.centerPosition.y, 0.f,
					mainMenuYesButton.width, //scale x,y
					mainMenuYesButton.height,
					0.f, mainMenuYesButton.alpha);	//Rotation

				GraphicSetTexture(mainMenuNoButton.ID);
				GraphicSetFilpTex(FLIP_IDLE, 1);
				GraphicSetAnimaTex(1, 0);
				GraphicSetOrthoParticleTransform(mainMenuNoButton.centerPosition.x, //x,y coordinate z order
					mainMenuNoButton.centerPosition.y, 0.f,
					mainMenuNoButton.width, //scale x,y
					mainMenuNoButton.height,
					0.f, mainMenuNoButton.alpha);	//Rotation

				DrawMouse(false, &mousePosition, &padCursorPosition, dt, mousePointID);
			}
			else // mainMenuReasking == false
			{
				// Restart
				if (g_Input->IsTriggered(JE_RETURN) || MyGamepad->GetButtonDown(XButtons.Start))
				{
					GraphicSetTexture(loadingID);
					GraphicSetFilpTex(FLIP_IDLE, 1);
					GraphicSetAnimaTex(1, 0);
					GraphicSetOrthogTransform(0.6, //x,y coordinate z order
						-0.9, 0.f,
						0.8, //scale x,y
						0.5,
						0.f);

					if (g_Input->IsTriggered(JE_RETURN) || MyGamepad->GetButtonDown(XButtons.Start))
						restartState = true;
				}
			}
		}

		// In game menu
		if (openMenu)
		{
			OptionUpdate(mousePosition.x, mousePosition.y, padCursorPosition.x, padCursorPosition.y, dt);

			if (gd.pause)
				OptionDraw();
		}

		if (startToPress)
			DrawMouse(!gd.pause, &mousePosition, &padCursorPosition, dt, mousePointID);

		pParticleSys->drawParticleSystem();

		GraphicEndDraw();
	}
	/*******************************************************************************
	Function: State_Lab::ShutDown

	Description: Release the resources.

	Inputs: None.

	Outputs: None.
	*******************************************************************************/
	void State_OpenWorld::ShutDown()
	{
		AniSystem::ShutDown();
		ObjectManager::ClearMap();
		pTileMapManager->ClearTiles();
		ShopShutdown();
		HUD::Shutdown();
		OptionShutdown();
		CharacterManager::Unload();
		JEShutdownLight();
		pParticleSys->shutDownParticle();
	}
	/*******************************************************************************
	Function: State_Lab::Unload

	Description: Unload the resources.

	Inputs: None.

	Outputs: None.
	*******************************************************************************/
	void State_OpenWorld::Unload()
	{
		// Resource Textures
		ObjectManager::Unload();
		ResourceSystem::Unload();

		GraphicUnloadTexture(mousePointID);
		GraphicUnloadTexture(restartToPressID);
		GraphicUnloadTexture(startToPressID);
		GraphicUnloadTexture(bugID);
		GraphicUnloadTexture(redDotID);
		GraphicUnloadTexture(backgroundID);
		GraphicUnloadTexture(loadingID);
		GraphicUnloadTexture(dirtID);
		GraphicUnloadTexture(smokeID);
		GraphicUnloadTexture(bloodID);
		GraphicUnloadTexture(rainBloodID);
		GraphicUnloadTexture(flyID);

		GraphicUnloadTexture(mainMenuAsking.ID);
		GraphicUnloadTexture(mainMenuNoButton.ID);
		GraphicUnloadTexture(mainMenuYesButton.ID);

		GraphicUnloadTexture(cloudID);
		GraphicUnloadTexture(lastMessageID);
		GraphicUnloadTexture(surviveMessageID);

		SOUNDMANAGER()->DeleteSound();
		resourceGauge.TextureUnload();
		ShopUnload();
		HUD::Unload();
		OptionUnload();
	}
}//JE