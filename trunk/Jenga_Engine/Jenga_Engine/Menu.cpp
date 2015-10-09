/*******************************************************************************
Copyright (C) 2015 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior 
written consent of DigiPen Institute of Technology is prohibited.
filename    Menu.cpp
author      Jaeheon Lee
            Seho.joo - lighting, particle
			Minsu Kang
email       jhoney4344@gmail.com
	        jsh2958@gmail.com
			minsu9486@gmail.com
course      GAM200

Brief Description:
  This is materialization of menu state class.
  Show the start menu screen to the user.

*******************************************************************************/
#include "JEStates.h"
#include "JEInput.h"
#include "JEDebug.h"
#include "JELoadTextureNew.h"
#include "JEGraphicsNew.h"
#include "JEStateManager.h"
#include "JEObjectManager.h"
#include "JECollisionManager.h"
#include "JEApplication.h"
#include "JEShop.h"
#include "JEOldParticle.h"
#include "JEGameData.h"

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

struct MenuObject
{
	MenuObject() {}
	MenuObject(double x, double y, double w, double h, double r = 0)
		: centerPosition(x, y), width(w), height(h), rotate(r) {}

	JEPoint2D centerPosition;
	JEPoint2D velocity;
	double width;
	double height;
	double rotate;
};

namespace{
	int mousePointID;
	int meteorID;
	int extraSpaceShipID;
	int capsuleID;

	int controllerID;
	int dgrbLogoID;
	int digiLogoID;
	int fmodID;
	int teamLogoID;
	int mainMenuID;
	int mainMenuBackgroundID;
	int loadingID;
	int  pauseBoxID;
	int  pauseButton[2];
	int titleID;

	JEPoint2D cameraPosition;
	JEPoint2D mousePosition;
	JEPoint2D padCursorPosition;
	double titleCenterPositionY;

	bool start;
	bool showLoading;
	bool quitReasking;

	ButtonObject startButton;
	ButtonObject exitButton;
	ButtonObject exitAsking;
	ButtonObject exitYesButton;
	ButtonObject exitNoButton;

	MenuObject extraSpaceship[6];
	MenuObject capsule[4];
	JEPoint2D meteorPosition;
	JEPoint2D meteorSpeed;
	double meteroSize;

	int fireID;
	int fireMiddleID;
	int particleMeteorID;

	particleSystem* pParticleSys;
	emitter* pMainEmitter[9] = {0};
	emitter* pExtraShipEmit[6] = {0};
	emitter* pMeteorEmit = 0;
	JE::Vector3f mainEmitterPos[3];
	JE::Vector3f extraEmitterPos[6];

	JE::Vector3f meteorEmitPos;

	float ambientPointIntensity;
	JE::Vector3f ambientPointColor;
	JE::Vector3f standardPoint;
	JE::Vector3f standardBoombColor;

	int mainWhiteMenuBackgroundID;
	bool checkMeteoColl;
	float whiteAmbientIntensity;
}

namespace JE
{
	void Menu::Load()
	{
		dgrbLogoID = JE::GraphicLoadTexture("Texture//DGRB_Logo.tga");
		loadingID = JE::GraphicLoadTexture("Texture//LoadingMSG.tga");
		titleID = JE::GraphicLoadTexture("Texture//MainMenu_Title.tga");
		pauseBoxID = JE::GraphicLoadTexture("Texture//Continue_back.tga");
		pauseButton[0] = JE::GraphicLoadTexture("Texture//Continue_n.tga");
		pauseButton[1] = JE::GraphicLoadTexture("Texture//Continue_y.tga");
		mousePointID = JE::GraphicLoadTexture("Texture//mouse.tga");
		extraSpaceShipID = JE::GraphicLoadTexture("Texture//MainMenu_ExtraSpaceship.tga");
		meteorID = JE::GraphicLoadTexture("Texture//MainMenu_Meteor.tga");
		capsuleID = JE::GraphicLoadTexture("Texture//Capsule.tga");

		controllerID = JE::GraphicLoadTexture("Texture//Controller.tga");
		digiLogoID = JE::GraphicLoadTexture("Texture//DigiLogo.tga");
		fmodID = JE::GraphicLoadTexture("Texture//FMOD.tga");
		teamLogoID = JE::GraphicLoadTexture("Texture//TeamLogo.tga");

		mainMenuID = JE::GraphicLoadTexture("Texture//MainMenu.tga");
		mainMenuBackgroundID = JE::GraphicLoadTexture("Texture//menuBackground.tga");
		mainWhiteMenuBackgroundID = JE::GraphicLoadTexture("Texture//menuWhiteBackground.tga");

		startButton.ID = JE::GraphicLoadTexture("Texture//Button_Start.tga");
		exitButton.ID = JE::GraphicLoadTexture("Texture//Button_Exit.tga");
		exitAsking.ID = JE::GraphicLoadTexture("Texture//Button_Exit_Ask.tga");
		exitYesButton.ID = JE::GraphicLoadTexture("Texture//Button_Exit_Ask_Yes.tga");
		exitNoButton.ID = JE::GraphicLoadTexture("Texture//Button_Exit_Ask_No.tga");

		fireID = GraphicLoadTexture("Texture//fireParticle.tga");
		fireMiddleID = GraphicLoadTexture("Texture//fireMiddleParticle.tga");

		particleMeteorID = GraphicLoadTexture("Texture//fireParticle.tga");
	}
/*******************************************************************************
   Function: Menu::Init

Description: Initialize variable and load the menu state resource.

     Inputs: None.

    Outputs: None.
*******************************************************************************/
void Menu::Init()
{
	GraphicInitCamera();
	SOUNDMANAGER()->LoadSound();
	SOUNDMANAGER()->Play("main");
	showLoading = false;
	JE::GraphicCameraZoom(false, 40.f);
	ShowCursor(false);
	JE::JEInitLight();

	start = false;
	quitReasking = false;
	titleCenterPositionY = 35.f;
	padCursorPosition = JEPoint2D();

	startButton = ButtonObject(startButton.ID, 0.375f, -0.23f, 0.38f, 0.225f);
	exitButton = ButtonObject(exitButton.ID, - 0.86f, -0.875f, 0.38f, 0.225f);
	exitAsking = ButtonObject(exitAsking.ID, 0.f, 0.f, 1.f, 0.6f, 1.f);
	exitYesButton = ButtonObject(exitYesButton.ID, -0.25f, -0.1f, 0.38f, 0.225f);
	exitNoButton = ButtonObject(exitNoButton.ID, 0.25f, -0.1f, 0.38f, 0.225f);

	capsule[0] = MenuObject(-24.5f + (0 * 4.05f), -3.5f, 5.f, 5.f, 40.f);
	capsule[1] = MenuObject(-24.5f + (1 * 4.05f), -3.5f, 5.f, 5.f, 40.f);
	capsule[2] = MenuObject(-24.5f + (3 * 4.05f), -3.5f, 5.f, 5.f, 40.f);
	capsule[3] = MenuObject(-24.5f + (4 * 4.05f), -3.5f, 5.f, 5.f, 40.f);

	extraSpaceship[0] = MenuObject(-20, 12.5f, 12.f, 3.f);
	extraSpaceship[1] = MenuObject(25, -20.f, 12.f, 3.f);
	extraSpaceship[2] = MenuObject(-60.f, 20.f, 16.f, 4.f);
	extraSpaceship[3] = MenuObject(-55.f, -15.f, 26.f, 6.5f);
	extraSpaceship[4] = MenuObject(50.f, 15.f, 26.f, 6.5f);
	extraSpaceship[5] = MenuObject(10, -25.f, 32.f, 8.f);

	float startY = -3.f;
	for (int i = 0; i < 3; ++i)
	{
		mainEmitterPos[i] = JE::Vector3f(28, startY, 0.f);
		startY += 4.f;
	}

	for (int i = 0; i < 6; ++i)
	{
		extraEmitterPos[i] = JE::Vector3f(float(extraSpaceship[i].centerPosition.x + (extraSpaceship[i].width/3)),
										  float(extraSpaceship[i].centerPosition.y),
			                              0.f);
	}


	meteorPosition = JEPoint2D(60.f, 40.f);
	meteorSpeed = JEPoint2D(0.f, 0.f);
	meteroSize = 5.f;

	capsule[0].velocity = JEPoint2D(0.f, 0.f);
	capsule[1].velocity = JEPoint2D(0.f, 0.f);
	capsule[2].velocity = JEPoint2D(0.f, 0.f);
	capsule[3].velocity = JEPoint2D(0.f, 0.f);

	pParticleSys = &getParticleSystem();
	pParticleSys->loadParticleTexture(PFIREUP, fireID);
	pParticleSys->loadParticleTexture(PFIREDOWN, fireID);
	pParticleSys->loadParticleTexture(PFIREMIDDLE, fireMiddleID);
	pParticleSys->loadParticleTexture(PMETEOR, particleMeteorID);

	JE::JEInitLight();

	ambientPointIntensity = JEGetDirectionalAmbient();
	ambientPointColor = JEGetDirectionalColor();

	JE::JEAddPointLight(int(&(meteorPosition)), JE::Vector3f(float(meteorPosition.x), float(meteorPosition.y), 0.f),
		JE::Vector3f(120.f, 40.f, 0.f), 0.5f, 0.f);

	JE::JEAddPointLight(int(&(standardPoint)), standardPoint,
		standardBoombColor, 1.f, 0.f);

	standardPoint = { 0, 10, 0 };
	standardBoombColor = { 0, 0, 0 };

	checkMeteoColl = false;
	whiteAmbientIntensity = 0.f;

	std::cout << "Menu Init" << std::endl;
}

/*******************************************************************************
   Function: Menu::Update

Description: Update menu every frame. Select next state and set option or exit.

     Inputs: dt - The frame time.

    Outputs: None.
*******************************************************************************/
void Menu::Update(double dt,bool& fullScreen)
{
	JE_UNUSED_PARAM(fullScreen);
	SOUNDMANAGER()->Update();
	static double time = 0.f;
	static double rotateTime = 0.f;
	static double buttonAniTime = 0.f;
	static double aniTime = 0.f;

	static float  checkHowTo =  500.f;
	static float  checkGame  = -500.f;

	static float ambientLight = 1.f;
	static Vector3f ambientColor = { 1.f, 1.f, 1.f };
	static float linear = 1.f;

	// Mouse cursor update
	mousePosition.x = g_Input->GetMouse_x();
	mousePosition.y = g_Input->GetMouse_y();

	// Force to move to next state
	if (g_Input->IsPressed(JE_CONTROL) && g_Input->IsPressed(JE_RETURN))
		StateManager::SetNextState(STATE_OPENWORLD);

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

	GraphicConvertWorldToScreen(mousePosition.x, mousePosition.y);


	time += dt;


	if (!gd.windowInactive)
	{
		if (!gd.muteBGM)
			SOUNDMANAGER()->BGMVolumeUp();
		
		if (!gd.muteEFT)
			SOUNDMANAGER()->EFTVolumeUp();
	}
	else
	{
		SOUNDMANAGER()->EFTVolumeDown();
		SOUNDMANAGER()->BGMVolumeDown();
		//SOUNDMANAGER()->EFTVolumeDown();
		MyGamepad->Rumble(0.f, 0.f);
	}


	// Logos
	if(time < 6.f)
	{
		// init
		for (unsigned i = 0; i < 6; ++i)
			extraSpaceship[i].velocity.x = dt * (extraSpaceship[i].height * extraSpaceship[i].height) / 5.f;

		// Skip intro
		if (g_Input->IsAnyTriggered() || MyGamepad->AnyButtonPressed())
			time += 6.f;
	}
	// Main Menu
	else
	{
		if (ambientLight >= 0.9f)
			ambientLight -= 0.001f;

		JEControlLight(ambientColor, ambientLight);

		meteorEmitPos = JE::Vector3f(float(meteorPosition.x), float(meteorPosition.y), 0.f);

		if (pMeteorEmit == 0)
		{
			pMeteorEmit = pParticleSys->addParticleEmit(PMETEOR, meteorEmitPos, 1000, EMPTY_EMITTER_ID, JE::Vector3f(float(meteorSpeed.x), float(meteorSpeed.y), 0.f),
				::PERSPECTIVE, float(meteroSize));
		}

		if (pMeteorEmit != 0)
		{
			pMeteorEmit->changeEmitterPos(meteorEmitPos);
			pMeteorEmit->changeEmitterRange(float(meteroSize));
			if (pMeteorEmit->getNumberOfParticle() <= 0)
			{
				pMeteorEmit = 0;
			}
		}

		// Game in Cue..
		if (start == false)
		{
			if (!quitReasking) // quitReasking == false
			{
				// Start button
				if (g_Collision->IntersectionPointRect(mousePosition, startButton.centerPosition, startButton.width, startButton.height)
					|| g_Collision->IntersectionPointRect(padCursorPosition, startButton.centerPosition, startButton.width, startButton.height))
				{
					if (startButton.alpha < 3.f)
						startButton.alpha += static_cast<float>(dt * 2.f);

					if (g_Input->IsTriggered(JE_MOUSE_LEFT) || MyGamepad->GetButtonDown(XButtons.R_Shoulder))
						start = true;
				}
				else
				{
					if (startButton.alpha > 0.5f)
						startButton.alpha -= static_cast<float>(dt * 4.f);
				}

				// Exit button
				if (g_Collision->IntersectionPointRect(mousePosition, exitButton.centerPosition, exitButton.width, exitButton.height)
					|| g_Collision->IntersectionPointRect(padCursorPosition, exitButton.centerPosition, exitButton.width, exitButton.height))
				{
					if (exitButton.alpha < 3.f)
						exitButton.alpha += static_cast<float>(dt * 2.f);

					if (g_Input->IsTriggered(JE_MOUSE_LEFT) || MyGamepad->GetButtonDown(XButtons.R_Shoulder))
						quitReasking = true;
				}
				else
				{
					if (exitButton.alpha > 0.5f)
						exitButton.alpha -= static_cast<float>(dt * 4.f);
				}
			}
			else // quitReasking == true
			{
				// Exit_Yes Button
				if (g_Collision->IntersectionPointRect(mousePosition, exitYesButton.centerPosition, exitYesButton.width, exitYesButton.height)
					|| g_Collision->IntersectionPointRect(padCursorPosition, exitYesButton.centerPosition, exitYesButton.width, exitYesButton.height))
				{
					if (exitYesButton.alpha < 3.f)
						exitYesButton.alpha += static_cast<float>(dt * 2.f);

					if (g_Input->IsTriggered(JE_MOUSE_LEFT) || MyGamepad->GetButtonDown(XButtons.R_Shoulder))
						StateManager::SetNextState(END_STATE);
				}
				else
				{
					if (exitYesButton.alpha > 0.5f)
						exitYesButton.alpha -= static_cast<float>(dt * 4.f);
				}

				// Exit_No Button
				if (g_Collision->IntersectionPointRect(mousePosition, exitNoButton.centerPosition, exitNoButton.width, exitNoButton.height)
					|| g_Collision->IntersectionPointRect(padCursorPosition, exitNoButton.centerPosition, exitNoButton.width, exitNoButton.height))
				{
					if (exitNoButton.alpha < 3.f)
						exitNoButton.alpha += static_cast<float>(dt * 2.f);

					if (g_Input->IsTriggered(JE_MOUSE_LEFT) || MyGamepad->GetButtonDown(XButtons.R_Shoulder))
						quitReasking = false;
				}
				else
				{
					if (exitNoButton.alpha > 0.5f)
						exitNoButton.alpha -= static_cast<float>(dt * 4.f);
				}
			}

			for (unsigned i = 0; i < 6; ++i)
			{
				extraSpaceship[i].centerPosition.x -= extraSpaceship[i].velocity.x;
				extraEmitterPos[i] = JE::Vector3f(float(extraSpaceship[i].centerPosition.x + (extraSpaceship[i].width / 3)), float(extraSpaceship[i].centerPosition.y), 0.f);
			}
		}
		// Start!
		else
		{
			// Remove All Text
			if (startButton.centerPosition.x < 1.15f)
			{
				titleCenterPositionY             += 0.5f;
				startButton.centerPosition.x     += 0.013f;
				exitButton.centerPosition.x      -= 0.01f;

				for (unsigned i = 0; i < 6; ++i)
				{
					extraSpaceship[i].centerPosition.x -= extraSpaceship[i].velocity.x;
					extraEmitterPos[i] = JE::Vector3f(float(extraSpaceship[i].centerPosition.x + (extraSpaceship[i].width / 3)), float(extraSpaceship[i].centerPosition.y), 0.f);
				}
			}
			// Episode Start!
			else
			{
				const double velocity = dt * 20.f;
				const double distance = (meteorPosition.x * meteorPosition.x) + (meteorPosition.y * meteorPosition.y);

				if (distance < 3500.f)
				{
					capsule[0].velocity.x =  velocity;
					capsule[0].velocity.y = -velocity;
				}

				if (distance < 2000.f)
				{
					
					capsule[2].velocity.x =  velocity;
					capsule[2].velocity.y = -velocity;
				}

				if (distance < 500.f)
				{
					capsule[3].velocity.x =  velocity;
					capsule[3].velocity.y = -velocity;
				}
				if (distance < 150.f)
				{
					//showLoading = true;
					meteorSpeed.x = 0.f;
					meteorSpeed.y = 0.f;

					ambientPointColor.m_x += 100.f;
					ambientPointColor.m_y += 40.f;
					ambientPointColor.m_z += 10.f;
				

					JE::JEChangePointLight(int(&(meteorPosition)), JE::Vector3f(float(meteorPosition.x), float(meteorPosition.y), 0.f),
						JE::Vector3f(0.f, 0.f, 0.f), 0.f, 0.f);

					JE::JEUpdatePointLight();

					checkMeteoColl = true;

					if (whiteAmbientIntensity == 0.0f)
						SOUNDMANAGER()->Play("bomb");
					if (!gd.windowInactive)
						MyGamepad->Rumble(1.0f, 1.0f);

					whiteAmbientIntensity += float(10.f * dt);
				}
				else
				{
					// Meteor Update
					const double meteorVelocity = dt * meteroSize;
					meteorSpeed = JEPoint2D(-1.1f * meteorVelocity, -0.5f * meteorVelocity);
					
					meteroSize += (dt * 2.f);
					rotateTime += dt * 500.f;
				}

				meteorPosition.x += meteorSpeed.x;
				meteorPosition.y += meteorSpeed.y;


				// Extra Spaceshipes Update
				for (unsigned i = 0; i < 6; ++i)
				{
					extraSpaceship[i].centerPosition.x -= (extraSpaceship[i].velocity.x * 2.f);
					extraEmitterPos[i] = JE::Vector3f(float(extraSpaceship[i].centerPosition.x + (extraSpaceship[i].width / 3)), float(extraSpaceship[i].centerPosition.y), 0.f);
				}

				// Capsules Update
				for (unsigned i = 0; i < 4; ++i)
				{
					capsule[i].centerPosition.x += capsule[i].velocity.x;
					capsule[i].centerPosition.y += capsule[i].velocity.y;
				}

			}

		}

		const double spaceEdge = 90.f;
		for (unsigned i = 0; i < 6; ++i)
		{
			if (extraSpaceship[i].centerPosition.x < -spaceEdge)
			{
				extraSpaceship[i].centerPosition.x = spaceEdge;
				extraSpaceship[i].centerPosition.y += RandValue(-5, 5);
			}
		}
	}

	pParticleSys->updateParticleSystem(float(dt));

	if (time >= 6.f)
	{
		for (int countOfBooster = 0; countOfBooster < 9; countOfBooster += 3)
		{
			if ((pMainEmitter[countOfBooster] == 0 && pMainEmitter[countOfBooster + 1] == 0 && pMainEmitter[countOfBooster + 2] == 0)
				&& whiteAmbientIntensity <= 40.f)
			{
				unsigned boosterPosIndex = countOfBooster / 3;
				pMainEmitter[countOfBooster] = pParticleSys->addParticleEmit(PFIREUP, mainEmitterPos[boosterPosIndex], 1000);
				pMainEmitter[countOfBooster + 1] = pParticleSys->addParticleEmit(PFIREMIDDLE, mainEmitterPos[boosterPosIndex], 1000);
				pMainEmitter[countOfBooster + 2] = pParticleSys->addParticleEmit(PFIREDOWN, mainEmitterPos[boosterPosIndex], 1000);
			}

			if (pMainEmitter[countOfBooster] != 0 && pMainEmitter[countOfBooster+1] != 0 && pMainEmitter[countOfBooster+2] != 0
				&& pMainEmitter[countOfBooster]->getNumberOfParticle() <= 0 
				&& pMainEmitter[countOfBooster + 1]->getNumberOfParticle() <= 0
				&& pMainEmitter[countOfBooster + 2]->getNumberOfParticle() <= 0)
			{
				pMainEmitter[countOfBooster] = 0;
				pMainEmitter[countOfBooster + 1] = 0;
				pMainEmitter[countOfBooster + 2] = 0;
			}
		}

		for (int numberOfShip = 0; numberOfShip < 6; numberOfShip += 1)
		{
			if ((pExtraShipEmit[numberOfShip] == 0)
				&& whiteAmbientIntensity <= 40.f)
			{
				pExtraShipEmit[numberOfShip] = pParticleSys->addParticleEmit(PFIREMIDDLE, extraEmitterPos[numberOfShip], 1000,
																				EMPTY_EMITTER_ID, JE::Vector3f(0.f, 0.f, 0.f),
																				::PERSPECTIVE, float(extraSpaceship[numberOfShip].height));
			}
			

			if (pExtraShipEmit[numberOfShip] != 0)
			{
				pExtraShipEmit[numberOfShip]->changeEmitterPos(extraEmitterPos[numberOfShip]);

				if (pExtraShipEmit[numberOfShip]->getNumberOfParticle() <= 0)
				{
					pExtraShipEmit[numberOfShip] = 0;
				}
			}
		}

		if (whiteAmbientIntensity > 40.f)
			pParticleSys->shutDownParticle();
	}

	GraphicStartDraw();

	JEUseObjShader();

	if (time < 6.f)
	{
		// Controller Logo
		GraphicSetTexture(controllerID);
		GraphicSetFilpTex(FLIP_IDLE, 1);
		GraphicSetAnimaTex(1, 0);
		GraphicSetOrthogTransform(-0.25f, //x,y coordinate z order
			0.8, 0.f,
			1.5, //scale x,y
			0.3,
			0.f);	//Rotation

		// DGRB Logo
		GraphicSetTexture(dgrbLogoID);
		GraphicSetFilpTex(FLIP_IDLE, 1);
		GraphicSetAnimaTex(1, 0);
		GraphicSetOrthogTransform(0.9f, //x,y coordinate z order
			-0.8, 0.f,
			0.2, //scale x,y
			0.3,
			0.f);	//Rotation
	}

	// Digipen Logo
	if(time < 2.f)
	{
		
		GraphicSetTexture(digiLogoID);
		GraphicSetFilpTex(FLIP_IDLE, 1);
		GraphicSetAnimaTex(1,0);
		GraphicSetPersTransform(0.f,0.f,0.f,		//x,y coordinate z order
					 150,150,			//scale x,y
					 0);
	}
	// Team Logo
	else if(time < 4.f)
	{
		
		GraphicSetTexture(teamLogoID);
		GraphicSetFilpTex(FLIP_IDLE, 1);
		GraphicSetAnimaTex(1,0);
		GraphicSetPersTransform(0.f,0.f,0.f,		//x,y coordinate z order
					 64.f,32.f,			//scale x,y
					 0.f);
	}
	// FMOD Logo
	else if(time < 6.f)
	{
		
		GraphicSetTexture(fmodID);
		GraphicSetFilpTex(FLIP_IDLE, 1);
		GraphicSetAnimaTex(1,0);
		GraphicSetPersTransform(0.f, 0.f, 0.f,		//x,y coordinate z order
					 64.f, 24.f,			//scale x,y
					 0.f);
	}
	// Main Menu
	else
	{
		//GraphicSetBackgroundColor(0, 0, 0); // Black = Space

		rotateTime += dt * 500.f;

		if(checkGame == -1000 && g_Input->IsTriggered(JE_RETURN))
		{
			checkGame = -500;
			JE::GraphicCameraPosition(0,0);
		}

		if(checkHowTo == 1000 && g_Input->IsTriggered(JE_RETURN))
		{
			checkGame = -1000;
			checkHowTo = 500;
			JE::GraphicCameraPosition(-1000,0);
		}
		
		GraphicSetTexture(mainMenuBackgroundID);
		GraphicSetFilpTex(FLIP_IDLE, 1);
		GraphicSetAnimaTex(1,0);
		GraphicSetPersTransform(0.f, 0.f, 0.f,		//x,y coordinate z order
					 154.f, 100.f,		//scale x,y
					 0.f);		//Rotation

		if (whiteAmbientIntensity <= 40.f)
		{
			// Spaceshipes
			GraphicSetTexture(extraSpaceShipID);
			GraphicSetFilpTex(FLIP_IDLE, 1);
			GraphicSetAnimaTex(1, 0);
			for (unsigned i = 0; i < 6; ++i)
			{
				GraphicSetPersTransform(extraSpaceship[i].centerPosition.x, //x,y coordinate z order
					extraSpaceship[i].centerPosition.y, 0.f,
					extraSpaceship[i].width, //scale x,y
					extraSpaceship[i].height,
					0.f);
			}

			// Spaceships with the Title
			GraphicSetTexture(mainMenuID);
			GraphicSetFilpTex(FLIP_IDLE, 1);
			GraphicSetAnimaTex(1, 0);
			GraphicSetPersTransform(0.f, 0.f, 0.f, //x,y coordinate z order
				200.f, 100.f,		       //scale x,y
				0.f);				       //Rotation
			
			// Capsules
			GraphicSetTexture(capsuleID);
			GraphicSetFilpTex(FLIP_IDLE, 1);
			GraphicSetAnimaTex(1, 0);
			for (unsigned i = 0; i < 4; ++i)
				GraphicSetPersTransform(capsule[i].centerPosition.x,
				capsule[i].centerPosition.y, 0.f, //x,y coordinate z order
				capsule[i].width,
				capsule[i].height, //scale x,y
				capsule[i].rotate); //Rotation
			GraphicSetPersTransform(-24.5f + (2 * 4.05f), -3.5f, 0.f, //x,y coordinate z order
				5.f, 5.f,		                        //scale x,y
				40.f);				                    //Rotation

			// Meteor
			GraphicSetTexture(meteorID);
			GraphicSetFilpTex(FLIP_IDLE, 1);
			GraphicSetAnimaTex(1, 0);
			GraphicSetPersTransform(meteorPosition.x, meteorPosition.y, 0.f, //x,y coordinate z order
				meteroSize, meteroSize,		                             //scale x,y
				rotateTime);
		}

		float pointAmbient = 60.f;
		if (meteorPosition.x >= 0)
			pointAmbient  = float(60.f / meteorPosition.x);

		JE::JEChangePointLight(int(&meteorPosition), JE::Vector3f(float(meteorPosition.x), float(meteorPosition.y), 0.f), 
			JE::Vector3f(50.f, 10.f, 0.f), pointAmbient, 0.f);
		
		pParticleSys->drawParticleSystem();


		/******************************************************************************************/
		/* Buttons */
		if (startButton.centerPosition.x < 1.15f)
		{
			GraphicSetTexture(titleID);
			GraphicSetFilpTex(FLIP_IDLE, 1);
			GraphicSetAnimaTex(1, 0);
			GraphicSetPersTransform(0.f, titleCenterPositionY, 0.f, //x,y coordinate z order
				100.f, 25.f, //scale x,y
				0.f);	    //Rotation

			GraphicSetTexture(startButton.ID);
			GraphicSetFilpTex(FLIP_IDLE, 1);
			GraphicSetAnimaTex(1, 0);
			GraphicSetOrthoParticleTransform(startButton.centerPosition.x, //x,y coordinate z order
				startButton.centerPosition.y, 0.f,
				startButton.width, //scale x,y
				startButton.height,
				0.f, startButton.alpha);	//Rotation

			GraphicSetTexture(exitButton.ID);
			GraphicSetFilpTex(FLIP_IDLE, 1);
			GraphicSetAnimaTex(1, 0);
			GraphicSetOrthoParticleTransform(exitButton.centerPosition.x, //x,y coordinate z order
				exitButton.centerPosition.y, 0.f,
				exitButton.width, //scale x,y
				exitButton.height,
				0.f, exitButton.alpha);	//Rotation
		}

		if (quitReasking)
		{
			GraphicSetTexture(exitAsking.ID);
			GraphicSetFilpTex(FLIP_IDLE, 1);
			GraphicSetAnimaTex(1, 0);
			GraphicSetOrthoParticleTransform(exitAsking.centerPosition.x, //x,y coordinate z order
				exitAsking.centerPosition.y, 0.f,
				exitAsking.width, //scale x,y
				exitAsking.height,
				0.f, exitAsking.alpha);	//Rotation

			GraphicSetTexture(exitYesButton.ID);
			GraphicSetFilpTex(FLIP_IDLE, 1);
			GraphicSetAnimaTex(1, 0);
			GraphicSetOrthoParticleTransform(exitYesButton.centerPosition.x, //x,y coordinate z order
				exitYesButton.centerPosition.y, 0.f,
				exitYesButton.width, //scale x,y
				exitYesButton.height,
				0.f, exitYesButton.alpha);	//Rotation

			GraphicSetTexture(exitNoButton.ID);
			GraphicSetFilpTex(FLIP_IDLE, 1);
			GraphicSetAnimaTex(1, 0);
			GraphicSetOrthoParticleTransform(exitNoButton.centerPosition.x, //x,y coordinate z order
				exitNoButton.centerPosition.y, 0.f,
				exitNoButton.width, //scale x,y
				exitNoButton.height,
				0.f, exitNoButton.alpha);	//Rotation
		}

		/******************************************************************************************/

		DrawMouse(start, &mousePosition, &padCursorPosition, dt, mousePointID);
		GraphicSetOrthogTransform(0.f, 0.f, 0.f, 0.f, 0.f, 0.f);
	}

	if (checkMeteoColl)
	{
		linear -= 1.f;
		JE::JEChangePointLight(int(&(standardPoint)), standardPoint,
			ambientPointColor, whiteAmbientIntensity, linear);

		JE::JEUpdatePointLight();
		if (whiteAmbientIntensity >= 43.f)
			showLoading = true;

		if (whiteAmbientIntensity >= 45.f)
			StateManager::SetNextState(STATE_OPENWORLD);
	}
	if (showLoading)
	{
		GraphicSetTexture(loadingID);
		GraphicSetFilpTex(FLIP_IDLE, 1);
		GraphicSetAnimaTex(1, 0);
		GraphicSetOrthogTransform(0.6, //x,y coordinate z order
			-0.9, 0.f,
			0.8, //scale x,y
			0.55,
			0.f);	//Rotation
	}
	GraphicEndDraw();

	//Debug::ClearConsole();
}
/*******************************************************************************
   Function: Menu::ShutDown

Description: Release the resources at the initilize function.

     Inputs: None.

    Outputs: None.
*******************************************************************************/
void Menu::ShutDown()
{
	pParticleSys->shutDownParticle();
	JEShutdownLight();

	SOUNDMANAGER()->DeleteSound();

	std::cout << "Menu ShutDown" << std::endl;
}
void Menu::Unload()
{
	JE::GraphicUnloadTexture(dgrbLogoID);
	JE::GraphicUnloadTexture(capsuleID);
	JE::GraphicUnloadTexture(extraSpaceShipID);
	JE::GraphicUnloadTexture(titleID);
	JE::GraphicUnloadTexture(mousePointID);
	JE::GraphicUnloadTexture(meteorID);
	JE::GraphicUnloadTexture(digiLogoID);
	JE::GraphicUnloadTexture(fmodID);
	JE::GraphicUnloadTexture(teamLogoID);
	JE::GraphicUnloadTexture(mainMenuID);
	JE::GraphicUnloadTexture(mainMenuBackgroundID);
	JE::GraphicUnloadTexture(pauseBoxID);
	JE::GraphicUnloadTexture(pauseButton[0]);
	JE::GraphicUnloadTexture(pauseButton[1]);
	JE::GraphicUnloadTexture(startButton.ID);
	JE::GraphicUnloadTexture(exitButton.ID);
	JE::GraphicUnloadTexture(loadingID);
	JE::GraphicUnloadTexture(controllerID);
	GraphicUnloadTexture(fireID);
	GraphicUnloadTexture(fireMiddleID);
	GraphicUnloadTexture(particleMeteorID);

	GraphicUnloadTexture(mainWhiteMenuBackgroundID);
}
}//Jenga Engine