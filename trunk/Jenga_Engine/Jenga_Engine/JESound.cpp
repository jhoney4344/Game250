/*****************************************************************************/
/*!
Copyright (C) 2015 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.

\file JESound.cpp
\author Sunghak Jin
\par email: shjin920\@gmail.com
\brief 
this is file of all functions for FMOD sounds
*/
/*****************************************************************************/
#include "JESound.h"
#include "JEGameData.h"
//volume
//float tmp_volume_b = 0.0f;
float tmp_volume_s = 0.0f;
bool daybool = false;
bool nightbool = false;;
extern JE::GameData gd;
//sounds
enum SOUNDS
{
	S_Menu = 0,
	S_BGMDay,
	S_BGMNight,
	S_Morning,
	S_Night,
	S_Jump1,
	S_Jump2,
	S_Cat,
	S_Punch,
	S_EnePunch,
	S_PowerPunch,
	S_Pop,
	S_Eat,
	S_Shop,
	S_Mineral,
	S_Grb,
	S_Shovel,
	S_Dirt,
	S_Tick,
	S_Wood,
	S_WoodCut,
	S_Denied,
	S_Teleport,
	S_Gameover,
	S_Drop,
	S_Bomb
};

//singleton for sound
SoundManager SoundManager::singleton;

/*****************************************************************************/
/*!
constructor
\return 
Nothing
\brief 
basic constructor. set all channels and sounds to NULL and set volumes
*/
/*****************************************************************************/
SoundManager::SoundManager(void)
{
	for (int index = 0; index < MAX_SOUND; ++index)
	{
		pChannel[index] = NULL;
	}

	for(int index = 0; index < MAX_SOUND; ++index)
	{
		pSound[index] = NULL;
	}

	b_volume = 0.5f;
	s_volume = 0.5f;
}

/*****************************************************************************/
/*!
Load all sound files
\return 
Nothing
\brief 
Load all sound files and separate them into backgroundmusics and effectsounds
*/
/*****************************************************************************/
void SoundManager::LoadSound(void)
{
	ErrCheck(FMOD::System_Create(&pSystem));

	ErrCheck(pSystem->init(32, FMOD_INIT_NORMAL, 0));

	ErrCheck(pSystem->createSound("Sound/Main.wav", FMOD_LOOP_NORMAL, 0, &pSound[S_Menu]));
	ErrCheck(pSystem->createSound("Sound/DayStage.wav", FMOD_LOOP_NORMAL, 0, &pSound[S_BGMDay]));
	ErrCheck(pSystem->createSound("Sound/NightStage.wav", FMOD_LOOP_NORMAL, 0, &pSound[S_BGMNight]));
	ErrCheck(pSystem->createSound("Sound/Morning.wav", FMOD_DEFAULT, 0, &pSound[S_Morning]));
	ErrCheck(pSystem->createSound("Sound/Night.wav", FMOD_DEFAULT, 0, &pSound[S_Night]));
	ErrCheck(pSystem->createSound("Sound/Jump.wav", FMOD_DEFAULT, 0, &pSound[S_Jump1]));
	ErrCheck(pSystem->createSound("Sound/Jump2.wav", FMOD_DEFAULT, 0, &pSound[S_Jump2]));
	ErrCheck(pSystem->createSound("Sound/CatDash.wav", FMOD_DEFAULT, 0, &pSound[S_Cat]));
	ErrCheck(pSystem->createSound("Sound/Punch.wav", FMOD_DEFAULT, 0, &pSound[S_Punch]));
	ErrCheck(pSystem->createSound("Sound/EnemyPunch.wav", FMOD_DEFAULT, 0, &pSound[S_EnePunch]));
	ErrCheck(pSystem->createSound("Sound/StrongPunch.wav", FMOD_DEFAULT, 0, &pSound[S_PowerPunch]));
	ErrCheck(pSystem->createSound("Sound/Pop.wav", FMOD_DEFAULT, 0, &pSound[S_Pop]));
	ErrCheck(pSystem->createSound("Sound/Eat.wav", FMOD_DEFAULT, 0, &pSound[S_Eat]));
	ErrCheck(pSystem->createSound("Sound/Shop.wav", FMOD_DEFAULT, 0, &pSound[S_Shop]));
	ErrCheck(pSystem->createSound("Sound/Mineral.wav", FMOD_DEFAULT, 0, &pSound[S_Mineral]));
	ErrCheck(pSystem->createSound("Sound/Zip.wav", FMOD_DEFAULT, 0, &pSound[S_Grb]));
	ErrCheck(pSystem->createSound("Sound/Shovel.wav", FMOD_DEFAULT, 0, &pSound[S_Shovel]));
	ErrCheck(pSystem->createSound("Sound/Dirt.wav", FMOD_DEFAULT, 0, &pSound[S_Dirt]));
	ErrCheck(pSystem->createSound("Sound/Tick.wav", FMOD_DEFAULT, 0, &pSound[S_Tick]));
	ErrCheck(pSystem->createSound("Sound/Wood.wav", FMOD_DEFAULT, 0, &pSound[S_Wood]));
	ErrCheck(pSystem->createSound("Sound/WoodCutting.wav", FMOD_DEFAULT, 0, &pSound[S_WoodCut]));
	ErrCheck(pSystem->createSound("Sound/Denied.wav", FMOD_DEFAULT, 0, &pSound[S_Denied]));
	ErrCheck(pSystem->createSound("Sound/Teleport.wav", FMOD_DEFAULT, 0, &pSound[S_Teleport]));
	ErrCheck(pSystem->createSound("Sound/Gameover.wav", FMOD_DEFAULT, 0, &pSound[S_Gameover]));
	ErrCheck(pSystem->createSound("Sound/Drop.wav", FMOD_DEFAULT, 0, &pSound[S_Drop]));
	ErrCheck(pSystem->createSound("Sound/Bomb.wav", FMOD_DEFAULT, 0, &pSound[S_Bomb]));
}

/*****************************************************************************/
/*!
Update SoundManager
\return 
Nothing
\brief 
Update SoundManager
*/
/*****************************************************************************/
void SoundManager::Update()
{
	pSystem->update();
}

/*****************************************************************************/
/*!
play soundfiles
\param name
standard for soundfile separation
\return 
Nothing
\brief 
play different sounds for different conditoins
*/
/*****************************************************************************/
void SoundManager::Play(const std::string& name)
{
	if (gd.muteBGM&&gd.muteEFT)
		return;

	string soundname = name;

	if (!gd.muteBGM)
	{
		if (name == "main")
		{
			ErrCheck(pSystem->playSound( pSound[S_Menu], 0, FALSE, &pChannel[S_Menu]));
		}

		else if (name == "bgmday")
		{

			ErrCheck(pSystem->playSound( pSound[S_BGMDay], 0, FALSE, &pChannel[S_BGMDay]));
		}

		else if (name == "bgmnight")
		{

			ErrCheck(pSystem->playSound(pSound[S_BGMNight], 0, FALSE, &pChannel[S_BGMNight]));
		}
	}
	if (!gd.muteEFT)
	{
		if (name == "morning")
			ErrCheck(pSystem->playSound( pSound[S_Morning], 0, FALSE, &pChannel[S_Morning]));

		else if (name == "night")
			ErrCheck(pSystem->playSound( pSound[S_Night], 0, FALSE, &pChannel[S_Night]));

		else if (name == "jump1")
			ErrCheck(pSystem->playSound( pSound[S_Jump1], 0, FALSE, &pChannel[S_Jump1]));

		else if (name == "jump2")
			ErrCheck(pSystem->playSound( pSound[S_Jump2], 0, FALSE, &pChannel[S_Jump2]));

		else if (name == "cat")
			ErrCheck(pSystem->playSound( pSound[S_Cat], 0, FALSE, &pChannel[S_Cat]));

		else if (name == "punch")
			ErrCheck(pSystem->playSound( pSound[S_Punch], 0, FALSE, &pChannel[S_Punch]));

		else if (name == "enepunch")
			ErrCheck(pSystem->playSound( pSound[S_EnePunch], 0, FALSE, &pChannel[S_EnePunch]));

		else if (name == "powerpunch")
			ErrCheck(pSystem->playSound( pSound[S_PowerPunch], 0, FALSE, &pChannel[S_PowerPunch]));

		else if (name == "click")
			ErrCheck(pSystem->playSound( pSound[S_Pop], 0, FALSE, &pChannel[S_Pop]));

		else if (name == "acq")
			ErrCheck(pSystem->playSound( pSound[S_Eat], 0, FALSE, &pChannel[S_Eat]));

		else if (name == "shop")
			ErrCheck(pSystem->playSound( pSound[S_Shop], 0, FALSE, &pChannel[S_Shop]));

		else if (name == "mineral")
			ErrCheck(pSystem->playSound( pSound[S_Mineral], 0, FALSE, &pChannel[S_Mineral]));

		else if (name == "grb")
			ErrCheck(pSystem->playSound( pSound[S_Grb], 0, FALSE, &pChannel[S_Grb]));

		else if (name == "shovel")
			ErrCheck(pSystem->playSound( pSound[S_Shovel], 0, FALSE, &pChannel[S_Shovel]));

		else if (name == "dirt")
			ErrCheck(pSystem->playSound( pSound[S_Dirt], 0, FALSE, &pChannel[S_Dirt]));

		else if (name == "tick")
			ErrCheck(pSystem->playSound( pSound[S_Tick], 0, FALSE, &pChannel[S_Tick]));

		else if (name == "wood")
			ErrCheck(pSystem->playSound( pSound[S_Wood], 0, FALSE, &pChannel[S_Wood]));

		else if (name == "woodcut")
			ErrCheck(pSystem->playSound( pSound[S_WoodCut], 0, FALSE, &pChannel[S_WoodCut]));

		else if (name == "denied")
			ErrCheck(pSystem->playSound( pSound[S_Denied], 0, FALSE, &pChannel[S_Denied]));

		else if (name == "teleport")
			ErrCheck(pSystem->playSound( pSound[S_Teleport], 0, FALSE, &pChannel[S_Teleport]));

		else if (name == "gameover")
			ErrCheck(pSystem->playSound( pSound[S_Gameover], 0, FALSE, &pChannel[S_Gameover]));

		else if (name == "drop")
			ErrCheck(pSystem->playSound( pSound[S_Drop], 0, FALSE, &pChannel[S_Drop]));

		else if (name == "bomb")
			ErrCheck(pSystem->playSound( pSound[S_Bomb], 0, FALSE, &pChannel[S_Bomb]));
	}
}

/*****************************************************************************/
/*!
release sounds
\return 
Nothing
\brief 
close and release channels and sounds
*/
/*****************************************************************************/
void SoundManager::DeleteSound()
{
	//stop all channels
	for (int index = 0; index < MAX_SOUND; ++index)
	{
		pChannel[index]->stop();
	}
	//release all sounds
	for(int index = 0; index < MAX_SOUND; ++index)
	{
		pSound[index]->release();
		pSound[index] = NULL;
	}
	//and then release/close system
	ErrCheck(pSystem->close());
	ErrCheck(pSystem->release());
}

/*****************************************************************************/
/*!
turn up the BGM volume
\return 
Nothing
\brief 
turn up all the backgroundmusic volumes
*/
/*****************************************************************************/
void SoundManager::BGMVolumeUp()
{
	b_volume += 0.1f;
	//in case of volume is more than 1
	if(b_volume >= 1.0f)
	{
		b_volume = 1.0f;
	}
	for (int index = 0; index < 5; ++index)
		pChannel[index]->setVolume(b_volume);
}

/*****************************************************************************/
/*!
turn down the BGM volume
\return 
Nothing
\brief 
turn down all the backgroundmusic volumes
*/
/*****************************************************************************/
void SoundManager::BGMVolumeDown()
{
	b_volume -= 0.1f;
	//in case of volume is less than 0
	if(b_volume <= 0.0f)
	{
		b_volume = 0.0f;
	}
	for (int index = 0; index < 5; ++index)
		pChannel[index]->setVolume(b_volume);
}

/*****************************************************************************/
/*!
mute the BGMDay volume
\return 
Nothing
\brief 
mute BGMDay volumes
*/
/*****************************************************************************/
void SoundManager::BGMDayMute()
{
	if (daybool == false)
	{
		daybool = true;
		pChannel[S_BGMDay]->setPaused(daybool);
	}

	else
	{
		daybool = false;
		pChannel[S_BGMDay]->setPaused(daybool);
	}
	//if(b_volume > 0.0f)
	//{
	//	tmp_volume_b = b_volume;
	//	b_volume = 0.0f;
	//	pChannel[C_BGMDay]->setVolume(b_volume);
	//	pChannel[C_BGMNight]->setVolume(b_volume);
	//}
	//else
	//{
	//	b_volume = tmp_volume_b;
	//	pChannel[C_BGMDay]->setVolume(b_volume);
	//	pChannel[C_BGMNight]->setVolume(b_volume);
	//}
}

/*****************************************************************************/
/*!
mute the BGMNight volume
\return
Nothing
\brief
mute BGMNight volumes
*/
/*****************************************************************************/
void SoundManager::BGMNightMute()
{
	if (nightbool == false)
	{
		nightbool = true;
		pChannel[S_BGMNight]->setPaused(nightbool);
	}

	else
	{
		nightbool = false;
		pChannel[S_BGMNight]->setPaused(nightbool);
	}
	//if (b_volume > 0.0f)
	//{
	//	tmp_volume_b = b_volume;
	//	b_volume = 0.0f;
	//	pChannel[C_BGMDay]->setVolume(b_volume);
	//	pChannel[C_BGMNight]->setVolume(b_volume);
	//	pChannel[C_BGMDay]->setPaused(true);
	//}
	//else
	//{
	//	b_volume = tmp_volume_b;
	//	pChannel[C_BGMDay]->setVolume(b_volume);
	//	pChannel[C_BGMNight]->setVolume(b_volume);
	//}
}

/*****************************************************************************/
/*!
turn up the effect sound volume
\return 
Nothing
\brief 
turn up all the effect sound volume
*/
/*****************************************************************************/
void SoundManager::EFTVolumeUp()
{
	s_volume += 0.1f;
	//in case of volume is more than 1
	if(s_volume >= 1.0f)
	{
		s_volume = 1.0f;
	}
	for (int index = 5; index < MAX_SOUND; ++index)
		pChannel[index]->setVolume(s_volume);
}

/*****************************************************************************/
/*!
turn down the effect sound volume
\return 
Nothing
\brief 
turn down all the effect sound volume
*/
/*****************************************************************************/
void SoundManager::EFTVolumeDown()
{
	//in case of volume is less than 0
	if(s_volume >= 0.0f)
	{
		s_volume -= 0.1f;
	}
	for (int index = 5; index < MAX_SOUND; ++index)
		pChannel[index]->setVolume(s_volume);
}

/*****************************************************************************/
/*!
mute the effectsound volume
\return 
Nothing
\brief 
mute all the effectsound volumes
*/
/*****************************************************************************/
void SoundManager::EFTMute()
{

	/*if(s_volume > 0.0f)
	{
		tmp_volume_s = s_volume;
		s_volume = 0.0f;
		for (int index = 2; index < MAX_CHANNEL; ++index)
			pChannel[index]->setVolume(s_volume);
	}
	else
	{
		s_volume = tmp_volume_s;
		for (int index = 2; index < MAX_CHANNEL; ++index)
			pChannel[index]->setVolume(s_volume);
	}*/

	for (int index = 5; index < MAX_SOUND; ++index)
		pChannel[index]->setPaused(true);
}
