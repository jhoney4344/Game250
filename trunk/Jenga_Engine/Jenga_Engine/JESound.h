/*****************************************************************************/
/*!
Copyright (C) 2015 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.

\file JESound.h
\author Sunghak Jin
\par email: shjin920\@gmail.com
\brief 
header file for JESound.cpp
*/
/*****************************************************************************/
#ifndef JENGA_ENGINE_SOUND_H
#define JENGA_ENGINE_SOUND_H
#include <fmod.h>
#include <fmod.hpp>
#include <string>
#include <windows.h>

using namespace std;

//set maximum number of sounds and channels
#define MAX_SOUND 26
//#define MAX_CHANNEL 24

/*****************************************************************************/
/*!
\class SoundManager
\brief 
class for managing all the sound effects
*/
/*****************************************************************************/
class SoundManager
{
private:
	FMOD::Sound *pSound[MAX_SOUND];
	FMOD::Channel *pChannel[MAX_SOUND];
	static SoundManager singleton;
	float b_volume;
	float s_volume;

public:
	FMOD::System *pSystem;
	static SoundManager *instance(){return &singleton;}

	SoundManager(void);

	void LoadSound();
	void Update();
	bool CreateSound(char* _filename);
	void Play(const std::string& filename);
	void DeleteSound();
	void BGMVolumeUp();
	void BGMVolumeDown();
	void BGMDayMute();
	void BGMNightMute();
	void EFTVolumeUp();
	void EFTVolumeDown();
	void EFTMute();


	//inline function for fmod error checking
	inline void ErrCheck(FMOD_RESULT result)
	{
		if(result != FMOD_OK)
		{
			MessageBox(NULL, TEXT("FMOD ERROR!"), TEXT("CHECK YOUR FMOD ERROR!"), MB_OK);
			exit(-1);
		}
	}

};
//singleton for soundmanager
#define SOUNDMANAGER() SoundManager::instance()

#endif