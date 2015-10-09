/*******************************************************************************
Copyright (C) 2015 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
filename    JEOptionMenu.cpp
author      Jaeheon Lee
email       jhoney4344@gmail.com
course      GAM250
assignment  Final project submission
due date    12/6/2015

Brief Description:
This file is interface file for option menu.

*******************************************************************************/

#include "JEMath.h"
#include "JEOptionMenu.h"
#include "JEGraphicsNew.h"
#include "JELoadTextureNew.h"
#include "JEApplication.h"
#include "JEGameData.h"
#include "JEDayAndNight.h"

extern JE::GameData gd;

namespace{

	struct Button
	{
		Button()
			: aniNumber(1),aniRate(0),aniCurr(0),reAsk(false), active(false){}
		int ID;
		JEPoint2D position;
		float aniNumber;
		float aniRate;
		float aniCurr;
		float scaleX;
		float scaleY;
		double aniTime;
		bool reAsk;
		bool active;
	};

	struct PauseMenu
	{	
		Button Box;
		Button AskOptionBox;
		Button AskRestartBox;
		Button AskQuitBox;
		Button resume[2];
		Button restart[2];
		Button quit[2];
		Button option[2];
		Button Credit[2];
		Button howToPlay[2];
		Button check[2];
	};
	int loadingID;
	bool restartState;
	bool showLoading;
	JEPoint2D mousePosition;
	JEPoint2D padPosition;
	PauseMenu pauseMenu;
	int howToIndex;
	int howToScrID[2];
	int translucentLayerID;
	int creditScrID;
	int AskOptionToggleID;
	JE::DayNight* pDayNight;
	JEPoint2D muteAll;
	JEPoint2D muteBGM;
	JEPoint2D fullScr;
	JEPoint2D Exit;
}

namespace JE{

	void OptionLoad(void)
	{
		// load textuer files
		loadingID = JE::GraphicLoadTexture("Texture//LoadingMSG.tga");
		howToScrID[0] = GraphicLoadTexture("Texture//HowToPlayControlKey.tga");
		howToScrID[1] = GraphicLoadTexture("Texture//HowToPlayGamePlay.tga");
		::pauseMenu.check[0].ID = ::pauseMenu.check[1].ID = GraphicLoadTexture("Texture//Check.tga");
		creditScrID = GraphicLoadTexture("Texture//Credit.tga");
		::pauseMenu.AskOptionBox.ID = GraphicLoadTexture("Texture//AskOptionBox.tga");
		AskOptionToggleID = GraphicLoadTexture("Texture//AskOptionToggle.tga");
		::pauseMenu.Box.ID = GraphicLoadTexture("Texture//MenuScreen2.tga");

		// Buttons
		::pauseMenu.resume[0].ID = ::pauseMenu.resume[1].ID = GraphicLoadTexture("Texture//Continue.tga");
		::pauseMenu.restart[0].ID = ::pauseMenu.restart[1].ID = GraphicLoadTexture("Texture//Restart.tga");
		::pauseMenu.option[0].ID = ::pauseMenu.option[1].ID = GraphicLoadTexture("Texture//OptionButton.tga");
		::pauseMenu.howToPlay[0].ID = ::pauseMenu.howToPlay[1].ID = GraphicLoadTexture("Texture//HowToPlayButton.tga");
		::pauseMenu.Credit[0].ID = ::pauseMenu.Credit[1].ID = GraphicLoadTexture("Texture//CreditsButton.tga");
		::pauseMenu.quit[0].ID = ::pauseMenu.quit[1].ID = GraphicLoadTexture("Texture//QuitGame.tga");

		::pauseMenu.AskRestartBox.ID = GraphicLoadTexture("Texture//AskRestart.tga");
		::pauseMenu.AskQuitBox.ID = GraphicLoadTexture("Texture//AskQuit.tga");
		translucentLayerID = GraphicLoadTexture("Texture//translucentLayer.tga");
	}

	void OptionInit(void)
	{
		restartState = false;
		showLoading = false;
		pDayNight = &(getDayNight());
		// initialize animation part

		::pauseMenu.check[1].aniNumber = 4;
		::pauseMenu.check[1].aniRate = ::pauseMenu.check[1].aniCurr = 1.f / ::pauseMenu.check[1].aniNumber;
		::pauseMenu.check[0].aniNumber = 4;
		::pauseMenu.check[0].aniRate = ::pauseMenu.check[0].aniCurr = 1.f / ::pauseMenu.check[0].aniNumber;

		for (int i = 0; i < 2; i++)
		{
			::pauseMenu.Credit[i].aniNumber = 4;
			::pauseMenu.Credit[i].aniRate = ::pauseMenu.Credit[i].aniCurr = 1.f / ::pauseMenu.Credit[i].aniNumber;
			::pauseMenu.option[i].aniNumber = 4;
			::pauseMenu.option[i].aniRate = ::pauseMenu.option[i].aniCurr = 1.f / ::pauseMenu.option[i].aniNumber;
			::pauseMenu.resume[i].aniNumber = 4;
			::pauseMenu.resume[i].aniRate = ::pauseMenu.resume[i].aniCurr = 1.f / ::pauseMenu.resume[i].aniNumber;
			::pauseMenu.restart[i].aniNumber = 4;
			::pauseMenu.restart[i].aniRate = ::pauseMenu.restart[i].aniCurr = 1.f / ::pauseMenu.restart[i].aniNumber;
			::pauseMenu.howToPlay[i].aniNumber = 4;
			::pauseMenu.howToPlay[i].aniRate = ::pauseMenu.howToPlay[i].aniCurr = 1.f / ::pauseMenu.howToPlay[i].aniNumber;

			::pauseMenu.quit[i].aniNumber = 4;
			::pauseMenu.quit[i].aniRate = ::pauseMenu.quit[i].aniCurr = 1.f / ::pauseMenu.quit[i].aniNumber;

			::pauseMenu.resume[i].position.y = 0.7;
			::pauseMenu.restart[i].position.y = 0.45;
			::pauseMenu.option[i].position.y = 0.15;

			::pauseMenu.howToPlay[i].position.y = -0.15;
			::pauseMenu.Credit[i].position.y = -0.4;
			::pauseMenu.quit[i].position.y = -0.7;

			::pauseMenu.Credit[i].scaleX = 0.6f; ::pauseMenu.Credit[i].scaleY = 0.2f;
			::pauseMenu.option[i].scaleX = 0.8f; ::pauseMenu.option[i].scaleY = 0.2f;
			
			::pauseMenu.resume[i].scaleX = 1.f; ::pauseMenu.resume[i].scaleY = 0.3f;
			::pauseMenu.restart[i].scaleX = 1.f; ::pauseMenu.restart[i].scaleY = 0.3f;
			::pauseMenu.howToPlay[i].scaleX = 1.f; ::pauseMenu.howToPlay[i].scaleY = .3f;
			::pauseMenu.quit[i].scaleX = 1.f; ::pauseMenu.quit[i].scaleY = .3f;
		}
		
		howToIndex = 0;
		::pauseMenu.Box.scaleX = 1.5f; ::pauseMenu.Box.scaleY = 2.0f;
		::pauseMenu.check[0].position.x = -.15;
		::pauseMenu.check[0].scaleX = ::pauseMenu.check[0].scaleY = .3f;
		::pauseMenu.check[1].position.x = -.15;
		::pauseMenu.check[1].scaleX = ::pauseMenu.check[1].scaleY = .3f;
		::pauseMenu.AskQuitBox.scaleX = 1.f; ::pauseMenu.AskQuitBox.scaleY = .4f;
		::pauseMenu.AskOptionBox.scaleX = 1.5f; ::pauseMenu.AskOptionBox.scaleY = 1.5f;
		muteAll.x = muteBGM.x = fullScr.x = 0.5;
		muteAll.y = 0.2; muteBGM.y = 0.4; fullScr.y = 0.05;
		Exit.y = -0.35;
	}

	void OptionUpdate(double mouseX, double mouseY, double padX, double padY, double dt)
	{
		::pauseMenu.resume[PLAYER_1ST].active = false;
		::pauseMenu.restart[PLAYER_1ST].active = false;
		::pauseMenu.quit[PLAYER_1ST].active = false;
		::pauseMenu.howToPlay[PLAYER_1ST].active = false;
		::pauseMenu.Credit[PLAYER_1ST].active = false;
		::pauseMenu.option[PLAYER_1ST].active = false;

		const float STRING_HEIGHT = .1f;
		const float UNIT_FRAME_TIME = 0.15f;

		mousePosition = JEPoint2D(mouseX,mouseY);
		padPosition.x = padX;
		padPosition.y = padY;

		if (restartState)
			StateManager::RestartState();
		for (int i = 0; i < 2; ++i)
		{
			if (::pauseMenu.check[i].reAsk)
			{
				::pauseMenu.check[i].aniTime += dt;
				if (::pauseMenu.check[i].aniTime > UNIT_FRAME_TIME)
				{
					::pauseMenu.check[i].aniCurr += ::pauseMenu.check[i].aniRate;
					::pauseMenu.check[i].aniTime = 0;
				}
			}
			::pauseMenu.check[i].reAsk = false;
		}
		if (::pauseMenu.howToPlay[PLAYER_1ST].reAsk)
		{
			if (g_Input->IsTriggered(JE_MOUSE_LEFT))
			{
				if (howToIndex == 0)
					howToIndex = 1;
				else if (howToIndex == 1)
				{
					howToIndex = 0;
					::pauseMenu.howToPlay[PLAYER_1ST].reAsk = false;
				}
			}
		}
		/////////////////////Secondary Asking/////////////////////////////
		else if (::pauseMenu.quit[PLAYER_1ST].reAsk)
		{
			if (mousePosition.y > -STRING_HEIGHT && mousePosition.y < STRING_HEIGHT)
			{
				::pauseMenu.check[PLAYER_1ST].reAsk = true;
				double STRING_LENGTH = 0.3;
				::pauseMenu.check[PLAYER_1ST].position.y = .0;
				if (mousePosition.x > ::pauseMenu.AskQuitBox.position.x - 2 * STRING_LENGTH
					&& mousePosition.x < ::pauseMenu.AskQuitBox.position.x)
				{
					::pauseMenu.check[PLAYER_1ST].position.x = -.15;
					if (g_Input->IsTriggered(JE_MOUSE_LEFT))
						StateManager::SetNextState(END_STATE);
				}
				else if (mousePosition.x > ::pauseMenu.AskQuitBox.position.x
					&& mousePosition.x < ::pauseMenu.AskQuitBox.position.x + 2 * STRING_LENGTH)
				{
					::pauseMenu.check[PLAYER_1ST].position.x = .25;
					if (g_Input->IsTriggered(JE_MOUSE_LEFT))
						::pauseMenu.quit[PLAYER_1ST].reAsk = false;
				}
			}
			
		}
		else if (::pauseMenu.restart[PLAYER_1ST].reAsk)
		{
			if (mousePosition.y > -STRING_HEIGHT && mousePosition.y < STRING_HEIGHT)
			{
				::pauseMenu.check[PLAYER_1ST].reAsk = true;
				::pauseMenu.check[PLAYER_1ST].position.y = .0;
				double STRING_LENGTH = 0.3;
				if (mousePosition.x > ::pauseMenu.AskQuitBox.position.x - 2 * STRING_LENGTH
					&& mousePosition.x < ::pauseMenu.AskQuitBox.position.x)
				{
					::pauseMenu.check[PLAYER_1ST].position.x = -.15;
					if (g_Input->IsTriggered(JE_MOUSE_LEFT))
					{
						gd.pause = false;
						::pauseMenu.restart[PLAYER_1ST].reAsk = false;
						showLoading = true;
						restartState = true;
						GraphicSetTexture(loadingID);
						GraphicSetFilpTex(FLIP_IDLE, 1);
						GraphicSetAnimaTex(1, 0);
						GraphicSetOrthogTransform(0.6, //x,y coordinate z order
							-0.8, 0.f,
							0.8, //scale x,y
							0.4,
							0.f);
					}
				}
				else if (mousePosition.x > ::pauseMenu.AskQuitBox.position.x
					&& mousePosition.x < ::pauseMenu.AskQuitBox.position.x + 2 * STRING_LENGTH)
				{
					::pauseMenu.check[PLAYER_1ST].position.x = .25;
					if (g_Input->IsTriggered(JE_MOUSE_LEFT))
						::pauseMenu.restart[PLAYER_1ST].reAsk = false;
				}
			}
		}
		else if (::pauseMenu.Credit[PLAYER_1ST].reAsk)
		{
			if (g_Input->IsTriggered(JE_MOUSE_LEFT))
				::pauseMenu.Credit[PLAYER_1ST].reAsk = false;
		}
		else if (::pauseMenu.option[PLAYER_1ST].reAsk)
		{
			::pauseMenu.check[PLAYER_1ST].reAsk = true;

			// Exit
			if (mousePosition.y > Exit.y - STRING_HEIGHT && mousePosition.y < Exit.y + STRING_HEIGHT
				&& mousePosition.x > -.2 && mousePosition.x < .2)
			{
				::pauseMenu.check[PLAYER_1ST].position.x = Exit.x;
				::pauseMenu.check[PLAYER_1ST].position.y = Exit.y + 0.05;
				
				if (g_Input->IsTriggered(JE_MOUSE_LEFT))
					::pauseMenu.option[PLAYER_1ST].reAsk = false;
			}
			//mute bgm
			else if(mousePosition.y > muteBGM.y - STRING_HEIGHT && mousePosition.y < muteBGM.y + STRING_HEIGHT
				&& mousePosition.x > muteBGM.x - 2 * 0.3 && mousePosition.x < muteBGM.x)
			{
				::pauseMenu.check[PLAYER_1ST].position.y = muteBGM.y + 0.2;
				::pauseMenu.check[PLAYER_1ST].position.x = muteBGM.x - .1;
				if (g_Input->IsTriggered(JE_MOUSE_LEFT))
				{
					if (gd.muteBGM == false)
					{
						for (int i = 0; i < 10; i++)
							SOUNDMANAGER()->BGMVolumeDown();
						gd.muteBGM = true;
					}
					::pauseMenu.option[PLAYER_1ST].reAsk = false;
				}
			}
			else if (mousePosition.y > muteBGM.y - STRING_HEIGHT && mousePosition.y < muteBGM.y + STRING_HEIGHT
				&& mousePosition.x >muteBGM.x && mousePosition.x < muteBGM.x + 2 * 0.3)
			{
				::pauseMenu.check[PLAYER_1ST].position.y = muteBGM.y + 0.2;
				::pauseMenu.check[PLAYER_1ST].position.x = muteBGM.x + .1;
				if (g_Input->IsTriggered(JE_MOUSE_LEFT))
				{
					for (int i = 0; i < 10; i++)
					{
						SOUNDMANAGER()->BGMVolumeUp();
					}
					gd.muteBGM = false;
					::pauseMenu.option[PLAYER_1ST].reAsk = false;
				}
			}
			// mute all
			else if (mousePosition.y > muteAll.y - STRING_HEIGHT && mousePosition.y < muteAll.y + STRING_HEIGHT
				&& mousePosition.x > muteAll.x - 2 * 0.3 && mousePosition.x < muteAll.x)
			{
				::pauseMenu.check[PLAYER_1ST].position.y = muteAll.y + 0.13;
				::pauseMenu.check[PLAYER_1ST].position.x = muteAll.x - .1;
				if (g_Input->IsTriggered(JE_MOUSE_LEFT))
				{
					if (gd.muteBGM == false)
					{
						for (int i = 0; i < 10; i++)
							SOUNDMANAGER()->BGMVolumeDown();
								
						gd.muteBGM = true;
					}
					if (gd.muteEFT == false)
					{
						SOUNDMANAGER()->EFTMute();
						gd.muteEFT = true;
					}
					::pauseMenu.option[PLAYER_1ST].reAsk = false;
				}

			}
			else if (mousePosition.y > muteAll.y - STRING_HEIGHT && mousePosition.y < muteAll.y + STRING_HEIGHT
				&& mousePosition.x > muteAll.x - 2 * 0.3 && mousePosition.x < muteAll.x)
			{
				::pauseMenu.check[PLAYER_1ST].position.y = muteAll.y + 0.13;
				::pauseMenu.check[PLAYER_1ST].position.x = muteAll.x - .1;
				if (g_Input->IsTriggered(JE_MOUSE_LEFT))
				{
					if (gd.muteBGM == false)
					{
						for (int i = 0; i < 10; i++)
							SOUNDMANAGER()->BGMVolumeDown();
								
						gd.muteBGM = true;
					}
					if (gd.muteEFT == false)
					{
						SOUNDMANAGER()->EFTMute();
						gd.muteEFT = true;
					}
					::pauseMenu.option[PLAYER_1ST].reAsk = false;
				}
			}
			else if (mousePosition.y > muteAll.y - STRING_HEIGHT && mousePosition.y < muteAll.y + STRING_HEIGHT
				&& mousePosition.x >  muteAll.x && mousePosition.x <  muteAll.x + 2 * 0.3)
			{
				::pauseMenu.check[PLAYER_1ST].position.y = muteAll.y + 0.13;
				::pauseMenu.check[PLAYER_1ST].position.x = muteAll.x + .1;
				if (g_Input->IsTriggered(JE_MOUSE_LEFT))
				{
					for (int i = 0; i < 10; i++)
					{
						SOUNDMANAGER()->BGMVolumeUp();
					}
					SOUNDMANAGER()->EFTVolumeUp();
					gd.muteBGM = false;
					gd.muteEFT = false;
					::pauseMenu.option[PLAYER_1ST].reAsk = false;
				}

			}
			// full screen
			else if (mousePosition.y > fullScr.y - STRING_HEIGHT && mousePosition.y < fullScr.y + STRING_HEIGHT
				&& mousePosition.x > fullScr.x - 2 * 0.3 && mousePosition.x < fullScr.x)
			{
				::pauseMenu.check[PLAYER_1ST].position.y = fullScr.y;
				::pauseMenu.check[PLAYER_1ST].position.x = fullScr.x - .1;
				if (g_Input->IsTriggered(JE_MOUSE_LEFT))
				{
					APP::FullScreen(true);
					::pauseMenu.option[PLAYER_1ST].reAsk = false;
				}
			}
			else if (mousePosition.y > fullScr.y - STRING_HEIGHT && mousePosition.y < fullScr.y + STRING_HEIGHT
				&& mousePosition.x > fullScr.x - 2 && mousePosition.x < fullScr.x + 2 * 0.3)
			{
				::pauseMenu.check[PLAYER_1ST].position.y = fullScr.y;
				::pauseMenu.check[PLAYER_1ST].position.x = fullScr.x + .1;
				if (g_Input->IsTriggered(JE_MOUSE_LEFT))
				{
					APP::FullScreen(false);
					::pauseMenu.option[PLAYER_1ST].reAsk = false;
				}
			}
			else
				::pauseMenu.check[PLAYER_1ST].reAsk = false;
		}

		/////////////////////Basic Menu/////////////////////////////////////////////
		else if (mousePosition.x > -.5 && mousePosition.x < .5)
		{
			if (mousePosition.y > ::pauseMenu.resume[PLAYER_1ST].position.y - STRING_HEIGHT
				&& mousePosition.y < ::pauseMenu.resume[PLAYER_1ST].position.y + STRING_HEIGHT)
			{
				::pauseMenu.resume[PLAYER_1ST].active = true;
				if (g_Input->IsTriggered(JE_MOUSE_LEFT))
					gd.pause = false;
			}
			else if (mousePosition.y > ::pauseMenu.restart[PLAYER_1ST].position.y - STRING_HEIGHT
				&& mousePosition.y < ::pauseMenu.restart[PLAYER_1ST].position.y + STRING_HEIGHT)
			{
				::pauseMenu.restart[PLAYER_1ST].active = true;
				if (g_Input->IsTriggered(JE_MOUSE_LEFT))
				{
					::pauseMenu.restart[PLAYER_1ST].reAsk = true;
				}
			}
			else if (mousePosition.y > ::pauseMenu.option[PLAYER_1ST].position.y - STRING_HEIGHT
				&& mousePosition.y < ::pauseMenu.option[PLAYER_1ST].position.y + STRING_HEIGHT)
			{
				::pauseMenu.option[PLAYER_1ST].active = true;
				if (g_Input->IsTriggered(JE_MOUSE_LEFT))
				{
					::pauseMenu.option[PLAYER_1ST].reAsk = true;
				}
			}
			else if (mousePosition.y > ::pauseMenu.Credit[PLAYER_1ST].position.y - STRING_HEIGHT
				&& mousePosition.y < ::pauseMenu.Credit[PLAYER_1ST].position.y + STRING_HEIGHT)
			{
				::pauseMenu.Credit[PLAYER_1ST].active = true;
				if (g_Input->IsTriggered(JE_MOUSE_LEFT))
				{
					::pauseMenu.Credit[PLAYER_1ST].reAsk = true;
				}
			}
			else if (mousePosition.y > ::pauseMenu.howToPlay[PLAYER_1ST].position.y - STRING_HEIGHT
				&& mousePosition.y < ::pauseMenu.howToPlay[PLAYER_1ST].position.y + STRING_HEIGHT)
			{
				::pauseMenu.howToPlay[PLAYER_1ST].active = true;
				if (g_Input->IsTriggered(JE_MOUSE_LEFT))
				{
					::pauseMenu.howToPlay[PLAYER_1ST].reAsk = true;
				}
			}
			else if (mousePosition.y > ::pauseMenu.quit[PLAYER_1ST].position.y - STRING_HEIGHT
				&& mousePosition.y < ::pauseMenu.quit[PLAYER_1ST].position.y + STRING_HEIGHT)
			{
				::pauseMenu.quit[PLAYER_1ST].active = true;
				if (g_Input->IsTriggered(JE_MOUSE_LEFT))
					::pauseMenu.quit[PLAYER_1ST].reAsk = true;
			}
		}
		
////////////////////////////// PLAYER 2ND ///////////////////////////////////////////////////////

		if (::pauseMenu.howToPlay[PLAYER_1ST].reAsk)
		{
			if (MyGamepad->GetButtonDown(XButtons.R_Shoulder))
			{
				if (howToIndex == 0)
					howToIndex = 1;
				else if (howToIndex == 1)
				{
					howToIndex = 0;
					::pauseMenu.howToPlay[PLAYER_1ST].reAsk = false;
				}
			}
		}
/////////////////////Secondary Asking/////////////////////////////
		else if (::pauseMenu.quit[PLAYER_1ST].reAsk)
		{
			if (padPosition.y > -STRING_HEIGHT && padPosition.y < STRING_HEIGHT)
			{
				::pauseMenu.check[PLAYER_2ND].reAsk = true;
				double STRING_LENGTH = 0.3;
				::pauseMenu.check[PLAYER_2ND].position.y = .0;
				if 	(padPosition.x > ::pauseMenu.AskQuitBox.position.x - 2 * STRING_LENGTH
					&& padPosition.x < ::pauseMenu.AskQuitBox.position.x)
				{
					::pauseMenu.check[PLAYER_2ND].position.x = -.15;
					if (MyGamepad->GetButtonDown(XButtons.R_Shoulder))
						StateManager::SetNextState(END_STATE);
				}
				else if (padPosition.x > ::pauseMenu.AskQuitBox.position.x
					  && padPosition.x < ::pauseMenu.AskQuitBox.position.x + 2 * STRING_LENGTH)
				{
					::pauseMenu.check[PLAYER_2ND].position.x = .25;
					if (MyGamepad->GetButtonDown(XButtons.R_Shoulder))
						::pauseMenu.quit[PLAYER_1ST].reAsk = false;
				}
			}

		}
		else if (::pauseMenu.restart[PLAYER_1ST].reAsk)
		{
			if (padPosition.y > -STRING_HEIGHT && padPosition.y < STRING_HEIGHT)
			{
				::pauseMenu.check[PLAYER_2ND].reAsk = true;
				::pauseMenu.check[PLAYER_2ND].position.y = .0;
				double STRING_LENGTH = 0.3;
				if (padPosition.x > ::pauseMenu.AskQuitBox.position.x - 2 * STRING_LENGTH
				 && padPosition.x < ::pauseMenu.AskQuitBox.position.x)
				{
					::pauseMenu.check[PLAYER_2ND].position.x = -.15;
					if (MyGamepad->GetButtonDown(XButtons.R_Shoulder))
					{
						gd.pause = false;
						::pauseMenu.restart[PLAYER_1ST].reAsk = false;
						showLoading = true;
						restartState = true;
						GraphicSetTexture(loadingID);
						GraphicSetFilpTex(FLIP_IDLE, 1);
						GraphicSetAnimaTex(1, 0);
						GraphicSetOrthogTransform(0.6, //x,y coordinate z order
							-0.8, 0.f,
							0.8, //scale x,y
							0.4,
							0.f);
					}
				}
				else if (padPosition.x > ::pauseMenu.AskQuitBox.position.x
					&& padPosition.x < ::pauseMenu.AskQuitBox.position.x + 2 * STRING_LENGTH)
				{
					::pauseMenu.check[PLAYER_2ND].position.x = .25;
					if (MyGamepad->GetButtonDown(XButtons.R_Shoulder))
						::pauseMenu.restart[PLAYER_1ST].reAsk = false;
				}
			}
		}
		else if (::pauseMenu.Credit[PLAYER_1ST].reAsk)
		{
			if (MyGamepad->GetButtonDown(XButtons.R_Shoulder))
				::pauseMenu.Credit[PLAYER_1ST].reAsk = false;
		}
		else if (::pauseMenu.option[PLAYER_1ST].reAsk)
		{
			::pauseMenu.check[PLAYER_2ND].reAsk = true;

			// Exit
			if (padPosition.y > Exit.y - STRING_HEIGHT && padPosition.y < Exit.y + STRING_HEIGHT
				&& padPosition.x > -.2 && padPosition.x < .2)
			{
				::pauseMenu.check[PLAYER_2ND].position.x = Exit.x;
				::pauseMenu.check[PLAYER_2ND].position.y = Exit.y + 0.05;

				if (MyGamepad->GetButtonDown(XButtons.R_Shoulder))
					::pauseMenu.option[PLAYER_1ST].reAsk = false;
			}
			//mute bgm
			else if (padPosition.y > muteBGM.y - STRING_HEIGHT && padPosition.y < muteBGM.y + STRING_HEIGHT
				&& padPosition.x > muteBGM.x - 2 * 0.3 && padPosition.x < muteBGM.x)
			{
				::pauseMenu.check[PLAYER_2ND].position.y = muteBGM.y + 0.2;
				::pauseMenu.check[PLAYER_2ND].position.x = muteBGM.x - .1;
				if (MyGamepad->GetButtonDown(XButtons.R_Shoulder))
				{
					if (gd.muteBGM == false)
					{
						for (int i = 0; i < 10; i++)
							SOUNDMANAGER()->BGMVolumeDown();
						gd.muteBGM = true;
					}
					::pauseMenu.option[PLAYER_1ST].reAsk = false;
				}
			}
			else if (padPosition.y > muteBGM.y - STRING_HEIGHT && padPosition.y < muteBGM.y + STRING_HEIGHT
				&& padPosition.x >muteBGM.x && padPosition.x < muteBGM.x + 2 * 0.3)
			{
				::pauseMenu.check[PLAYER_2ND].position.y = muteBGM.y + 0.2;
				::pauseMenu.check[PLAYER_2ND].position.x = muteBGM.x + .1;
				if (MyGamepad->GetButtonDown(XButtons.R_Shoulder))
				{
					for (int i = 0; i < 10; i++)
					{
						SOUNDMANAGER()->BGMVolumeUp();
					}
					gd.muteBGM = false;
					::pauseMenu.option[PLAYER_1ST].reAsk = false;
				}
			}
			// mute all
			else if (padPosition.y > muteAll.y - STRING_HEIGHT && padPosition.y < muteAll.y + STRING_HEIGHT
				&& padPosition.x > muteAll.x - 2 * 0.3 && padPosition.x < muteAll.x)
			{
				::pauseMenu.check[PLAYER_2ND].position.y = muteAll.y + 0.13;
				::pauseMenu.check[PLAYER_2ND].position.x = muteAll.x - .1;
				if (MyGamepad->GetButtonDown(XButtons.R_Shoulder))
				{
					if (gd.muteBGM == false)
					{
						for (int i = 0; i < 10; i++)
							SOUNDMANAGER()->BGMVolumeDown();

						gd.muteBGM = true;
					}
					if (gd.muteEFT == false)
					{
						SOUNDMANAGER()->EFTMute();
						gd.muteEFT = true;
					}
					::pauseMenu.option[PLAYER_1ST].reAsk = false;
				}

			}
			else if (padPosition.y > muteAll.y - STRING_HEIGHT && padPosition.y < muteAll.y + STRING_HEIGHT
				&& padPosition.x > muteAll.x - 2 * 0.3 && padPosition.x < muteAll.x)
			{
				::pauseMenu.check[PLAYER_2ND].position.y = muteAll.y + 0.13;
				::pauseMenu.check[PLAYER_2ND].position.x = muteAll.x - .1;
				if (MyGamepad->GetButtonDown(XButtons.R_Shoulder))
				{
					if (gd.muteBGM == false)
					{
						for (int i = 0; i < 10; i++)
							SOUNDMANAGER()->BGMVolumeDown();

						gd.muteBGM = true;
					}
					if (gd.muteEFT == false)
					{
						SOUNDMANAGER()->EFTMute();
						gd.muteEFT = true;
					}
					::pauseMenu.option[PLAYER_1ST].reAsk = false;
				}
			}
			else if (padPosition.y > muteAll.y - STRING_HEIGHT && padPosition.y < muteAll.y + STRING_HEIGHT
				&& padPosition.x >  muteAll.x && padPosition.x <  muteAll.x + 2 * 0.3)
			{
				::pauseMenu.check[PLAYER_2ND].position.y = muteAll.y + 0.13;
				::pauseMenu.check[PLAYER_2ND].position.x = muteAll.x + .1;
				if (MyGamepad->GetButtonDown(XButtons.R_Shoulder))
				{
					for (int i = 0; i < 10; i++)
					{
						SOUNDMANAGER()->BGMVolumeUp();
					}
					SOUNDMANAGER()->EFTVolumeUp();
					gd.muteBGM = false;
					gd.muteEFT = false;
					::pauseMenu.option[PLAYER_1ST].reAsk = false;
				}

			}
			// full screen
			else if (padPosition.y > fullScr.y - STRING_HEIGHT && padPosition.y < fullScr.y + STRING_HEIGHT
				   && padPosition.x > fullScr.x - 2 * 0.3 && padPosition.x < fullScr.x)
			{
				::pauseMenu.check[PLAYER_2ND].position.y = fullScr.y;
				::pauseMenu.check[PLAYER_2ND].position.x = fullScr.x - .1;
				if (MyGamepad->GetButtonDown(XButtons.R_Shoulder))
				{
					APP::FullScreen(true);
					::pauseMenu.option[PLAYER_1ST].reAsk = false;
				}
			}
			else if (padPosition.y > fullScr.y - STRING_HEIGHT && padPosition.y < fullScr.y + STRING_HEIGHT
				&& padPosition.x > fullScr.x - 2 && padPosition.x < fullScr.x + 2 * 0.3)
			{
				::pauseMenu.check[PLAYER_2ND].position.y = fullScr.y;
				::pauseMenu.check[PLAYER_2ND].position.x = fullScr.x + .1;
				if (MyGamepad->GetButtonDown(XButtons.R_Shoulder))
				{
					APP::FullScreen(false);
					::pauseMenu.option[PLAYER_1ST].reAsk = false;
				}
			}
			else
				::pauseMenu.check[PLAYER_2ND].reAsk = false;
		}


		/////////////////////Basic Menu/////////////////////////////////////////////
		else
		 if (padPosition.x > -0.5 && padPosition.x < 0.5)
		{
			if (padPosition.y > ::pauseMenu.resume[PLAYER_1ST].position.y - STRING_HEIGHT
				&& padPosition.y < ::pauseMenu.resume[PLAYER_1ST].position.y + STRING_HEIGHT)
			{  
				
				::pauseMenu.resume[PLAYER_1ST].active = true;

				if (MyGamepad->GetButtonDown(XButtons.R_Shoulder))
					gd.pause = false;
			}
		 else
			 if (padPosition.y > ::pauseMenu.restart[PLAYER_2ND].position.y - STRING_HEIGHT
				&& padPosition.y < ::pauseMenu.restart[PLAYER_2ND].position.y + STRING_HEIGHT)
			{
				::pauseMenu.restart[PLAYER_1ST].active = true;
				if (MyGamepad->GetButtonDown(XButtons.R_Shoulder))
				{
					::pauseMenu.restart[PLAYER_1ST].reAsk = true;
				}
			}
			else if (padPosition.y > ::pauseMenu.option[PLAYER_2ND].position.y - STRING_HEIGHT
				&& padPosition.y < ::pauseMenu.option[PLAYER_2ND].position.y + STRING_HEIGHT)
			{
				::pauseMenu.option[PLAYER_1ST].active = true;
				if (MyGamepad->GetButtonDown(XButtons.R_Shoulder))
				{
					::pauseMenu.option[PLAYER_1ST].reAsk = true;
				}
			}
			else if (padPosition.y > ::pauseMenu.Credit[PLAYER_2ND].position.y - STRING_HEIGHT
				&& padPosition.y < ::pauseMenu.Credit[PLAYER_2ND].position.y + STRING_HEIGHT)
			{
				::pauseMenu.Credit[PLAYER_1ST].active = true;
				if (MyGamepad->GetButtonDown(XButtons.R_Shoulder))
				{
					::pauseMenu.Credit[PLAYER_1ST].reAsk = true;
					//::pauseMenu.Credit[PLAYER_2ND].reAsk = true;
				}
			}
			else if (padPosition.y > ::pauseMenu.howToPlay[PLAYER_2ND].position.y - STRING_HEIGHT
				&& padPosition.y < ::pauseMenu.howToPlay[PLAYER_2ND].position.y + STRING_HEIGHT)
			{
				::pauseMenu.howToPlay[PLAYER_1ST].active = true;
				if (MyGamepad->GetButtonDown(XButtons.R_Shoulder))
				{
					::pauseMenu.howToPlay[PLAYER_1ST].reAsk = true;
					//::pauseMenu.howToPlay[PLAYER_2ND].reAsk = true;
				}
			}
			else if (padPosition.y > ::pauseMenu.quit[PLAYER_2ND].position.y - STRING_HEIGHT
				&& padPosition.y < ::pauseMenu.quit[PLAYER_2ND].position.y + STRING_HEIGHT)
			{
				::pauseMenu.quit[PLAYER_1ST].active = true;
				if (MyGamepad->GetButtonDown(XButtons.R_Shoulder))
				{
					::pauseMenu.quit[PLAYER_1ST].reAsk = true;
					//::pauseMenu.quit[PLAYER_2ND].reAsk = true;
				}
			}
		}
		 if (::pauseMenu.resume[PLAYER_1ST].active)
		 {

		 }
		 if (::pauseMenu.restart[PLAYER_1ST].active)
		 {
			 ::pauseMenu.restart[PLAYER_1ST].aniTime += 2*dt;
			 if (::pauseMenu.restart[PLAYER_1ST].aniTime > UNIT_FRAME_TIME)
			 {
				 ::pauseMenu.restart[PLAYER_1ST].aniCurr += ::pauseMenu.restart[PLAYER_1ST].aniRate;
				 ::pauseMenu.restart[PLAYER_1ST].aniTime = 0;
			 }
		 }
		 else ::pauseMenu.restart[PLAYER_1ST].aniCurr = 0;
		 if (::pauseMenu.quit[PLAYER_1ST].active)
		 {
			 ::pauseMenu.quit[PLAYER_1ST].aniTime += 2*dt;
			 if (::pauseMenu.quit[PLAYER_1ST].aniTime > UNIT_FRAME_TIME)
			 {
				 ::pauseMenu.quit[PLAYER_1ST].aniCurr += ::pauseMenu.quit[PLAYER_1ST].aniRate;
				 ::pauseMenu.quit[PLAYER_1ST].aniTime = 0;
			 }
		 }
		 else  ::pauseMenu.quit[PLAYER_1ST].aniCurr = 0.25;
		 if (::pauseMenu.howToPlay[PLAYER_1ST].active)
		 {
			 ::pauseMenu.howToPlay[PLAYER_1ST].aniTime += 2*dt;
			 if (::pauseMenu.howToPlay[PLAYER_1ST].aniTime > UNIT_FRAME_TIME)
			 {
				 ::pauseMenu.howToPlay[PLAYER_1ST].aniCurr += ::pauseMenu.howToPlay[PLAYER_1ST].aniRate;
				 ::pauseMenu.howToPlay[PLAYER_1ST].aniTime = 0;
			 }
		 }
		 else ::pauseMenu.howToPlay[PLAYER_1ST].aniCurr = 0;
		 if (::pauseMenu.Credit[PLAYER_1ST].active)
		 {
			 ::pauseMenu.Credit[PLAYER_1ST].aniTime += 2*dt;
			 if (::pauseMenu.Credit[PLAYER_1ST].aniTime > UNIT_FRAME_TIME)
			 {
				 ::pauseMenu.Credit[PLAYER_1ST].aniCurr += ::pauseMenu.Credit[PLAYER_1ST].aniRate;
				 ::pauseMenu.Credit[PLAYER_1ST].aniTime = 0;
			 }
		 }
		 else  ::pauseMenu.Credit[PLAYER_1ST].aniCurr = 0;
		 if (::pauseMenu.option[PLAYER_1ST].active)
		 {
			 ::pauseMenu.option[PLAYER_1ST].aniTime += 2*dt;
			 if (::pauseMenu.option[PLAYER_1ST].aniTime > UNIT_FRAME_TIME)
			 {
				 ::pauseMenu.option[PLAYER_1ST].aniCurr += ::pauseMenu.option[PLAYER_1ST].aniRate;
				 ::pauseMenu.option[PLAYER_1ST].aniTime = 0;
			 }
		 }
		 else ::pauseMenu.option[PLAYER_1ST].aniCurr = 0;
		 if (::pauseMenu.resume[PLAYER_1ST].active)
		 {
			 ::pauseMenu.resume[PLAYER_1ST].aniTime += 2*dt;
			 if (::pauseMenu.resume[PLAYER_1ST].aniTime > UNIT_FRAME_TIME)
			 {
				 ::pauseMenu.resume[PLAYER_1ST].aniCurr += ::pauseMenu.resume[PLAYER_1ST].aniRate;
				 ::pauseMenu.resume[PLAYER_1ST].aniTime = 0;
			 }
		 }
		 else ::pauseMenu.resume[PLAYER_1ST].aniCurr = 0;
	}
	void OptionClose()
	{
		::pauseMenu.Box.reAsk = false;
		::pauseMenu.AskOptionBox.reAsk = false;
		::pauseMenu.AskRestartBox.reAsk = false;
		::pauseMenu.AskQuitBox.reAsk = false;
		for (int i = 0; i < 2; i++)
		{
			::pauseMenu.resume[i].reAsk = false;
			::pauseMenu.restart[i].reAsk = false;
			::pauseMenu.quit[i].reAsk = false;
			::pauseMenu.option[i].reAsk = false;
			::pauseMenu.Credit[i].reAsk = false;
			::pauseMenu.howToPlay[i].reAsk = false;
		}
	}

	void OptionDraw(void)
	{
		// translucentLayer
		GraphicSetTexture(translucentLayerID);
		GraphicSetFilpTex(FLIP_IDLE, 1);
		GraphicSetAnimaTex(1, 0);
		GraphicSetOrthogTransform(0, 0, 0.f, //x,y coordinate z order
			10.f, 10.f,		//scale x,y
			0.f);

		// 
		GraphicSetTexture(::pauseMenu.Box.ID);
		GraphicSetFilpTex(FLIP_IDLE, 1);
		if (pDayNight->GetCurrState() == DAY)
			GraphicSetAnimaTex(2, 0.5);
		else
			GraphicSetAnimaTex(2, 1);
		
		GraphicSetOrthogTransform(0, 0, 0.f, //x,y coordinate z order
			::pauseMenu.Box.scaleX, ::pauseMenu.Box.scaleY,//scale x,y	
			0.f);

		
		GraphicSetTexture(::pauseMenu.resume[PLAYER_1ST].ID);
		GraphicSetFilpTex(FLIP_IDLE, 1);
		GraphicSetAnimaTex(::pauseMenu.resume[PLAYER_1ST].aniNumber, ::pauseMenu.resume[PLAYER_1ST].aniCurr);
		GraphicSetOrthogTransform(::pauseMenu.resume[PLAYER_1ST].position.x,
			::pauseMenu.resume[PLAYER_1ST].position.y, 0.f, //x,y coordinate z order
			::pauseMenu.resume[PLAYER_1ST].scaleX, ::pauseMenu.resume[PLAYER_1ST].scaleY,//scale x,y
			0.f);

		
		GraphicSetTexture(::pauseMenu.restart[PLAYER_1ST].ID);
		GraphicSetFilpTex(FLIP_IDLE, 1);
		GraphicSetAnimaTex(::pauseMenu.restart[PLAYER_1ST].aniNumber, ::pauseMenu.restart[PLAYER_1ST].aniCurr);
		GraphicSetOrthogTransform(::pauseMenu.restart[PLAYER_1ST].position.x,
			::pauseMenu.restart[PLAYER_1ST].position.y, 0.f, //x,y coordinate z order
			::pauseMenu.restart[PLAYER_1ST].scaleX, ::pauseMenu.restart[PLAYER_1ST].scaleY,//1.f, 0.3f,		//scale x,y
			0.f);

		GraphicSetTexture(::pauseMenu.howToPlay[PLAYER_1ST].ID);
		GraphicSetFilpTex(FLIP_IDLE, 1);
		GraphicSetAnimaTex(::pauseMenu.howToPlay[PLAYER_1ST].aniNumber, ::pauseMenu.howToPlay[PLAYER_1ST].aniCurr);
		GraphicSetOrthogTransform(::pauseMenu.howToPlay[PLAYER_1ST].position.x,
			::pauseMenu.howToPlay[PLAYER_1ST].position.y, 0.f, //x,y coordinate z order
			::pauseMenu.howToPlay[PLAYER_1ST].scaleX, ::pauseMenu.howToPlay[PLAYER_1ST].scaleY,// 1.f, 0.3f,		//scale x,y
			0.f);

		GraphicSetTexture(::pauseMenu.quit[PLAYER_1ST].ID);
		GraphicSetFilpTex(FLIP_IDLE, 1);
		GraphicSetAnimaTex(::pauseMenu.quit[PLAYER_1ST].aniNumber, ::pauseMenu.quit[PLAYER_1ST].aniCurr);
		GraphicSetOrthogTransform(::pauseMenu.quit[PLAYER_1ST].position.x,
			::pauseMenu.quit[PLAYER_1ST].position.y, 0.f, //x,y coordinate z order
			::pauseMenu.quit[PLAYER_1ST].scaleX, ::pauseMenu.quit[PLAYER_1ST].scaleY,//1.f, 0.3f,		//scale x,y
			0.f);

		GraphicSetTexture(::pauseMenu.Credit[PLAYER_1ST].ID);
		GraphicSetFilpTex(FLIP_IDLE, 1);
		GraphicSetAnimaTex(::pauseMenu.Credit[PLAYER_1ST].aniNumber, ::pauseMenu.Credit[PLAYER_1ST].aniCurr);
		GraphicSetOrthogTransform(::pauseMenu.Credit[PLAYER_1ST].position.x,
			::pauseMenu.Credit[PLAYER_1ST].position.y, 0.f, //x,y coordinate z order
			::pauseMenu.Credit[PLAYER_1ST].scaleX, ::pauseMenu.Credit[PLAYER_1ST].scaleY,//1.f, 0.3f,		//scale x,y
			0.f);
		GraphicSetTexture(::pauseMenu.option[PLAYER_1ST].ID);
		GraphicSetFilpTex(FLIP_IDLE, 1);
		GraphicSetAnimaTex(::pauseMenu.option[PLAYER_1ST].aniNumber, ::pauseMenu.option[PLAYER_1ST].aniCurr);
		GraphicSetOrthogTransform(::pauseMenu.option[PLAYER_1ST].position.x,
			::pauseMenu.option[PLAYER_1ST].position.y, 0.f, //x,y coordinate z order
			::pauseMenu.option[PLAYER_1ST].scaleX, ::pauseMenu.option[PLAYER_1ST].scaleY,//1.f, 0.3f,		//scale x,y
			0.f);
		if (::pauseMenu.quit[PLAYER_1ST].reAsk)
		{
			GraphicSetTexture(::pauseMenu.AskQuitBox.ID);
			GraphicSetFilpTex(FLIP_IDLE, 1);
			//GraphicSetAnimaTex(::pauseMenu.quit.aniNumber, ::pauseMenu.quit.aniCurr);
			GraphicSetAnimaTex(1, 0);
			GraphicSetOrthogTransform(::pauseMenu.AskQuitBox.position.x,
				::pauseMenu.AskQuitBox.position.y, 0.f, //x,y coordinate z order
				::pauseMenu.AskQuitBox.scaleX, ::pauseMenu.AskQuitBox.scaleY,//1.f, 0.3f,		//scale x,y
				0.f);
		}
		else if (::pauseMenu.restart[PLAYER_1ST].reAsk)
		{
			GraphicSetTexture(::pauseMenu.AskRestartBox.ID);
			GraphicSetFilpTex(FLIP_IDLE, 1);
			//GraphicSetAnimaTex(::pauseMenu.quit.aniNumber, ::pauseMenu.quit.aniCurr);
			GraphicSetAnimaTex(1, 0);
			GraphicSetOrthogTransform(::pauseMenu.AskQuitBox.position.x,
				::pauseMenu.AskQuitBox.position.y, 0.f, //x,y coordinate z order
				::pauseMenu.AskQuitBox.scaleX, ::pauseMenu.AskQuitBox.scaleY,//1.f, 0.3f,		//scale x,y
				0.f);
		}
		else if (::pauseMenu.Credit[PLAYER_1ST].reAsk)
		{
			GraphicSetTexture(creditScrID);
			GraphicSetFilpTex(FLIP_IDLE, 1);
			//GraphicSetAnimaTex(::pauseMenu.quit.aniNumber, ::pauseMenu.quit.aniCurr);
			GraphicSetAnimaTex(1, 0);
			GraphicSetOrthogTransform(0.f, 0.f, 0.f, //x,y coordinate z order
				1.25f, 1.5f,//1.f, 0.3f,		//scale x,y
				0.f);
		}
		else if (::pauseMenu.option[PLAYER_1ST].reAsk)
		{
			GraphicSetTexture(::pauseMenu.AskOptionBox.ID);
			GraphicSetFilpTex(FLIP_IDLE, 1);
			//GraphicSetAnimaTex(::pauseMenu.quit.aniNumber, ::pauseMenu.quit.aniCurr);
			GraphicSetAnimaTex(1, 0);
			GraphicSetOrthogTransform(0.f, 0.f, 0.f, //x,y coordinate z order
				::pauseMenu.AskOptionBox.scaleX, ::pauseMenu.AskOptionBox.scaleY,//1.f, 0.3f,		//scale x,y
				0.f);
			GraphicSetTexture(AskOptionToggleID);
			GraphicSetFilpTex(FLIP_IDLE, 1);
			//GraphicSetAnimaTex(::pauseMenu.quit.aniNumber, ::pauseMenu.quit.aniCurr);
			GraphicSetAnimaTex(1, 0);
			GraphicSetOrthogTransform(0.f, 0.f, 0.f, //x,y coordinate z order
				::pauseMenu.AskOptionBox.scaleX, ::pauseMenu.AskOptionBox.scaleY,//1.f, 0.3f,		//scale x,y
				0.f);
		}
		else if (::pauseMenu.howToPlay[PLAYER_1ST].reAsk)
		{
			GraphicSetTexture(howToScrID[howToIndex]);
			GraphicSetFilpTex(FLIP_IDLE, 1);
			//GraphicSetAnimaTex(::pauseMenu.quit.aniNumber, ::pauseMenu.quit.aniCurr);
			GraphicSetAnimaTex(1, 0);
			GraphicSetOrthogTransform(0.f, 0.f, 0.f, //x,y coordinate z order
				1.5f, 1.f,//1.f, 0.3f,		//scale x,y
				0.f);
		}
		for (int i = 0; i < 2; ++i)
			if (::pauseMenu.check[i].reAsk)
			{
			GraphicSetTexture(::pauseMenu.check[i].ID);
			GraphicSetFilpTex(FLIP_IDLE, 1);
			//GraphicSetAnimaTex(::pauseMenu.quit.aniNumber, ::pauseMenu.quit.aniCurr);
			GraphicSetAnimaTex(::pauseMenu.check[i].aniNumber, ::pauseMenu.check[i].aniCurr);
			GraphicSetOrthogTransform(::pauseMenu.check[i].position.x,
				::pauseMenu.check[i].position.y, 0.f, //x,y coordinate z order
				::pauseMenu.check[i].scaleX, ::pauseMenu.check[i].scaleY,//1.f, 0.3f,		//scale x,y
				0.f);
			}
		if (showLoading)
		{
			GraphicSetTexture(loadingID);
			GraphicSetFilpTex(FLIP_IDLE, 1);
			GraphicSetAnimaTex(1, 0);
			GraphicSetOrthogTransform(0.6, //x,y coordinate z order
				-0.8, 0.f,
				0.8, //scale x,y
				0.4,
				0.f);	//Rotation
			
		}
	}
	void OptionShutdown(void)
	{
		pDayNight = 0;
	}

	void OptionUnload(void)
	{
		GraphicUnloadTexture(loadingID);
		GraphicUnloadTexture(howToScrID[1]);
		GraphicUnloadTexture(howToScrID[0]);
		GraphicUnloadTexture(::pauseMenu.check[0].ID);
		GraphicUnloadTexture(AskOptionToggleID);
		GraphicUnloadTexture(::pauseMenu.howToPlay[PLAYER_1ST].ID);
		GraphicUnloadTexture(::pauseMenu.AskOptionBox.ID);
		GraphicUnloadTexture(creditScrID);
		GraphicUnloadTexture(::pauseMenu.Credit[PLAYER_1ST].ID);
		GraphicUnloadTexture(::pauseMenu.option[PLAYER_1ST].ID);
		GraphicUnloadTexture(::pauseMenu.Box.ID);
		GraphicUnloadTexture(::pauseMenu.resume[PLAYER_1ST].ID);
		GraphicUnloadTexture(::pauseMenu.restart[PLAYER_1ST].ID);
		GraphicUnloadTexture(::pauseMenu.quit[PLAYER_1ST].ID);
		GraphicUnloadTexture(::pauseMenu.AskQuitBox.ID);
		GraphicUnloadTexture(::pauseMenu.AskQuitBox.ID);
		GraphicUnloadTexture(translucentLayerID);
	}
}