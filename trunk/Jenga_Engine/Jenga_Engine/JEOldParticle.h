/*******************************************************************************
Copyright (C) 2015 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
filename    JEOldParticle.h
author      Seho.Joo
email       jsh2958@gmail.com
course      GAM250
assignment  Final milestone
due date    6/12/2015

Brief Description:
This file is header file for JEOldParticle.cpp

*******************************************************************************/

#ifndef  JEOLDPARTICLE_H
#define  JEOLDPARTICLE_H

#include "JEOGLMathHelper.h"
#include <list>
#include "JEGraphicsNew.h"

#define NUMBER_OF_TEXTURE 20

enum PARTICLE_TYPE
{
	DIRT,
	SMOKE,
	BLOOD,
	FIREFLY,
	PJUMP,
	PRAIN,
	PFIREUP,
	PFIREDOWN,
	PFIREMIDDLE,
	PMETEOR,
	PFLY
};

enum EMITTER_ID
{
	EMPTY_EMITTER_ID,
	NIGHT_FIREFLY
};

enum PROJECTION_TYPE
{
	PERSPECTIVE,
	ORTHOGONAL
};

class particle
{
public:
	typedef std::list<particle>::iterator particleItor;

	particle();

	void initParticle(PARTICLE_TYPE type, GLuint textureID, JE::Vector3f position, JE::Vector3f objDirection, float range);
	void updateParticle(float deltaTime, PARTICLE_TYPE type);
	void drawPersParticle(void);
	void drawOrthoParticle(void);

	bool checkAliveParticle(void);
	void freeParticle(void);

	JE::Vector3f getObjectDirection(JE::Vector3f speed);

private:
	std::list<particle> m_particleList;

	GLuint m_textureID;
	JE::Vector3f m_velocity;
	JE::Vector3f m_accel;
	JE::Vector3f m_position;
	JE::Vector2f m_size;
	JE::Vector3f m_direction;
	float m_rotateVelociy;
	float m_rotate;
	float m_lifeTime;
	float m_lightLifeTime;
	float m_lightAmbientIntensity;
	float m_ambientIntensity;
	float m_ambientVelo;
	JE::Vector3f m_lightColor;
	PARTICLE_TYPE m_type;
};

class emitter 
{
public:
	typedef std::list<emitter>::iterator emitterItor;

	emitter();

	emitter* addEmitter(EMITTER_ID id, PARTICLE_TYPE type, GLuint textureID, JE::Vector3f position, int numberOfParticle, JE::Vector3f objDirection, 
						PROJECTION_TYPE projection, float Range);

	void updateEmitter(float deltaTime);
	void drawEmitter();

	float getLifeTime(void);
	JE::Vector3f getVelocity(void);
	JE::Vector3f getAcceleration(void);
	JE::Vector3f getPosition(void);
	PARTICLE_TYPE getType(void);

	void setNumberOfParticle(int numberOfParticle);
	int  getNumberOfParticle(void);
	void changeEmitterRange(float range);
	void changeEmitterPos(JE::Vector3f position);
	emitterItor findEmitter(EMITTER_ID id);
	void freeEmitter(void);

private:
	std::list<emitter> m_emitterList;

	GLuint m_textureID;
	EMITTER_ID m_emitterID;
	PARTICLE_TYPE m_type;
	JE::Vector3f m_position;
	int m_numberOfParticle;
	particle m_particle;
	float m_emitterTime;
	float m_timer;
	float m_numberOfEmittion;
	JE::Vector3f m_objDirection;
	PROJECTION_TYPE m_projection;
	float m_range;
};

class particleSystem
{
public:
	particleSystem();

	void loadParticleTexture(PARTICLE_TYPE type, GLuint textureID);
	emitter* addParticleEmit(PARTICLE_TYPE type, 
		                     JE::Vector3f position, 
							 int numberOfParticle, 
							 EMITTER_ID id = EMPTY_EMITTER_ID,
							 JE::Vector3f objDirection = JE::Vector3f(0.f, 0.f, 0.f),
							 PROJECTION_TYPE projection = PERSPECTIVE,
							 float Range = 0.f);
	void updateParticleSystem(float deltaTime);
	void drawParticleSystem();
	void shutDownParticle(void);

private:
	emitter m_emitter;
	GLuint m_texture[NUMBER_OF_TEXTURE];
};

float getRandomNum(int maxNum, int minNum);
particleSystem& getParticleSystem(void);

#endif