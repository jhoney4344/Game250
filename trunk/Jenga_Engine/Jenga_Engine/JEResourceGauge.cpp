/*******************************************************************************
Copyright (C) 2015 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
filename    JEResourceGauge.cpp
author      Minsu Kang
email       minsu9486@gmail.com
course      GAM250

*******************************************************************************/

#include "JEResourceGauge.h"
#include "JEBasePart.h"
#include "JEGraphicsNew.h"
#include "JELoadTextureNew.h"
#include "JECharacterManager.h"
#include "JEOldParticle.h"

namespace
{
	particleSystem* particleSys = &getParticleSystem();
	emitter* pEmitter[2] = { 0, 0 };
}


void ResourseGaugeTexture::TextureLoad(void)
{
	deadBlackID = JE::GraphicLoadTexture("Texture//HUD_Dead_Black.tga");
	deadRedID = JE::GraphicLoadTexture("Texture//HUD_Dead_Red.tga");

	breadID = JE::GraphicLoadTexture("Texture//Dot.tga");
	meatID = JE::GraphicLoadTexture("Texture//Dot_Yellow.tga");
	mineralID = JE::GraphicLoadTexture("Texture//Dot_Blue.tga");

	breadBackID = JE::GraphicLoadTexture("Texture//Dot_Trans_Red.tga");
	mineralBackID = JE::GraphicLoadTexture("Texture//Dot_Trans_Blue.tga");
	meatBackID = JE::GraphicLoadTexture("Texture//Dot_Trans_Yellow.tga");
}

void ResourseGaugeTexture::TextureUnload(void)
{
	JE::GraphicUnloadTexture(deadBlackID);
	JE::GraphicUnloadTexture(deadRedID);
	JE::GraphicUnloadTexture(breadID);
	JE::GraphicUnloadTexture(meatID);
	JE::GraphicUnloadTexture(mineralID);
	JE::GraphicUnloadTexture(breadBackID);
	JE::GraphicUnloadTexture(mineralBackID);
	JE::GraphicUnloadTexture(meatBackID);
}

void ResourseGaugeTexture::DrawResourceGauge(const JE::DAY_NIGHT dayNight)
{
	for (unsigned int i = 0; i < 2; ++i)
	{
		const JE::Resource resource = JE::CharacterManager::GetPlayerByID((JE::CHARACTER_ID)i).GetResource();
		const JE::Resource* MAX_RESOURCE = &JE::CharacterManager::GetPlayerByID((JE::CHARACTER_ID)i).GetBody()->GetMaxResource();
		const double gaugePosition = GAUGE_POSITION.x + (i * GAUGE_PLAYERS_SPACE);

		const double healthSize = GAUGE_SIZE * 1.5f;
		double value = 0;

		// For bread
		value = (resource.bread / MAX_RESOURCE->bread) * MAX_LENGTH;

		JE::GraphicSetTexture(breadBackID);
		GraphicSetFilpTex(JE::FLIP_IDLE, 1);
		JE::GraphicSetAnimaTex(1, 0);
		JE::GraphicSetOrthogTransform(gaugePosition + (MAX_LENGTH / 2), GAUGE_POSITION.y, 0, // x,y,z coordinate
			GAUGE_SIZE + MAX_LENGTH, healthSize,//scale x,y
			0);//rotation

		JE::GraphicSetTexture(breadID);
		GraphicSetFilpTex(JE::FLIP_IDLE, 1);
		JE::GraphicSetAnimaTex(1, 0);
		JE::GraphicSetOrthogTransform(gaugePosition + (value / 2), GAUGE_POSITION.y, 0, // x,y,z coordinate
			GAUGE_SIZE + value, healthSize,//scale x,y
			0);//rotation

		// For mineral
		value = ((double)resource.mineral / MAX_RESOURCE->mineral) * MAX_LENGTH;

		JE::GraphicSetTexture(mineralBackID);
		GraphicSetFilpTex(JE::FLIP_IDLE, 1);
		JE::GraphicSetAnimaTex(1, 0);
		JE::GraphicSetOrthogTransform(gaugePosition + (MAX_LENGTH / 2), GAUGE_POSITION.y - GAUGE_Y_SPACE, 0, // x,y,z coordinate
			GAUGE_SIZE + MAX_LENGTH, GAUGE_SIZE,//scale x,y
			0);//rotation

		JE::GraphicSetTexture(mineralID);
		GraphicSetFilpTex(JE::FLIP_IDLE, 1);
		JE::GraphicSetAnimaTex(1, 0);
		JE::GraphicSetOrthogTransform(gaugePosition + (value / 2), GAUGE_POSITION.y - GAUGE_Y_SPACE, 0, // x,y,z coordinate
			GAUGE_SIZE + value, GAUGE_SIZE,//scale x,y
			0);//rotation

		// For meat
		value = ((double)resource.meat / MAX_RESOURCE->meat) * MAX_LENGTH;

		JE::GraphicSetTexture(meatBackID);
		GraphicSetFilpTex(JE::FLIP_IDLE, 1);
		JE::GraphicSetAnimaTex(1, 0);
		JE::GraphicSetOrthogTransform(gaugePosition + (MAX_LENGTH / 2), GAUGE_POSITION.y - (GAUGE_Y_SPACE * 2), 0, // x,y,z coordinate
			GAUGE_SIZE + MAX_LENGTH, GAUGE_SIZE,//scale x,y
			0);//rotation

		JE::GraphicSetTexture(meatID);
		GraphicSetFilpTex(JE::FLIP_IDLE, 1);
		JE::GraphicSetAnimaTex(1, 0);
		JE::GraphicSetOrthogTransform(gaugePosition + (value / 2), GAUGE_POSITION.y - (GAUGE_Y_SPACE * 2), 0, // x,y,z coordinate
			GAUGE_SIZE + value, GAUGE_SIZE,//scale x,y
			0);//rotation
	}

	// Day(Black) & Night(Red)
	int textureID;
	if (dayNight == JE::DAY)
		textureID = deadBlackID;
	else
		textureID = deadRedID;

	if (!JE::CharacterManager::GetPlayerByID(JE::CHARACTER_ID::PLAYER_1ST).live1)
	{

		JE::GraphicSetTexture(textureID);
		GraphicSetFilpTex(JE::FLIP_IDLE, 1);
		JE::GraphicSetAnimaTex(1, 0);
		JE::GraphicSetOrthogTransform(CONDITION_POSITION.x, CONDITION_POSITION.y, 0, // x,y,z coordinate
			CONDITION_SIZE_X, CONDITION_SIZE_Y,//scale x,y
			0);//rotation

		if (pEmitter[0] == 0 && textureID == deadRedID)
		{
			JE::Vector3f rainPosition = JE::Vector3f(float(CONDITION_POSITION.x), float(CONDITION_POSITION.y), 0.f);
			pEmitter[0] = particleSys->addParticleEmit(PRAIN, rainPosition, 100, EMPTY_EMITTER_ID, JE::Vector3f(0.f, 0.f, 0.f), ORTHOGONAL);
		}
		else if (pEmitter[0] != 0 && textureID == deadBlackID)
		{
			//pParticleSys->deleteParticleEmit(NIGHT_FIREFLY);
			pEmitter[0]->setNumberOfParticle(0);
			pEmitter[0] = 0;
		}
	}

	if (!JE::CharacterManager::GetPlayerByID(JE::CHARACTER_ID::PLAYER_2ND).live2)
	{
		JE::GraphicSetTexture(textureID);
		GraphicSetFilpTex(JE::FLIP_IDLE, 1);
		JE::GraphicSetAnimaTex(1, 0);
		JE::GraphicSetOrthogTransform(CONDITION_POSITION.x + GAUGE_PLAYERS_SPACE, CONDITION_POSITION.y, 0, // x,y,z coordinate
			CONDITION_SIZE_X, CONDITION_SIZE_Y,//scale x,y
			0);//rotation

		if (pEmitter[1] == 0 && textureID == deadRedID)
		{
			JE::Vector3f rainPosition = JE::Vector3f(float(CONDITION_POSITION.x + GAUGE_PLAYERS_SPACE), float(CONDITION_POSITION.y), 0.f);
			pEmitter[1] = particleSys->addParticleEmit(PRAIN, rainPosition, 100, EMPTY_EMITTER_ID, JE::Vector3f(0.f, 0.f, 0.f), ORTHOGONAL);
		}
		else if (pEmitter[1] != 0 && textureID == deadBlackID)
		{
			//pParticleSys->deleteParticleEmit(NIGHT_FIREFLY);
			pEmitter[1]->setNumberOfParticle(0);
			pEmitter[1] = 0;
		}
	}
}