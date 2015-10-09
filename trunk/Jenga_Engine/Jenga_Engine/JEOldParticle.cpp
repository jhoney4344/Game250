/*******************************************************************************
Copyright (C) 2015 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
filename    JEOldParticle.cpp
author      Seho.Joo
email       jsh2958@gmail.com
course      GAM250
assignment  Final milestone
due date    6/12/2015

Brief Description:
This file executes old style particle that uses CPU.

*******************************************************************************/

#include "JEOldParticle.h"
#include "JEGraphicsNew.h"
#include <cmath>


#define EMPTY_ID      -1
#define CAMERA_GAP_X 10
#define CAMERA_GAP_Y 10

namespace
{
	particleSystem particleSys;
}

particleSystem& getParticleSystem(void)
{
	return particleSys;
}

particleSystem::particleSystem()
{
}

void particleSystem::loadParticleTexture(PARTICLE_TYPE type, GLuint textureID)
{
	m_texture[type] = textureID;
}

emitter* particleSystem::addParticleEmit(PARTICLE_TYPE type, JE::Vector3f position, int numberOfParticle, EMITTER_ID id, JE::Vector3f objDirection, PROJECTION_TYPE projection
										 , float Range)
{
	position.m_z = 0;
	return m_emitter.addEmitter(id, type, m_texture[type], position, numberOfParticle, objDirection, projection, Range);
}

//void particleSystem::deleteParticleEmit(EMITTER_ID id)
//{
//	m_emitter.deleteEmitter(id);
//}

void particleSystem::updateParticleSystem(float deltaTime)
{
	m_emitter.updateEmitter(deltaTime);
}

void particleSystem::drawParticleSystem()
{
	m_emitter.drawEmitter();
}

void particleSystem::shutDownParticle(void)
{
	m_emitter.freeEmitter();
}

emitter::emitter()
{

}

emitter* emitter::addEmitter(EMITTER_ID id,
						 PARTICLE_TYPE type,
	                     GLuint textureID, 
						 JE::Vector3f position, 
						 int numberOfParticle,
						 JE::Vector3f objDirection,
						 PROJECTION_TYPE projection,
						 float Range)
{
	emitter emitterProper;
		
	emitterProper.m_position = position;
	emitterProper.m_numberOfParticle = numberOfParticle;
	emitterProper.m_type = type;
	emitterProper.m_textureID = textureID;
	emitterProper.m_emitterID = id;
	emitterProper.m_timer = 0;
	emitterProper.m_objDirection = objDirection;
	emitterProper.m_projection = projection;
	emitterProper.m_range = Range;

	switch (type)
	{
		case DIRT:
			emitterProper.m_emitterTime = 0.05f;
			emitterProper.m_numberOfEmittion = 1.f;
			break;
	
		case SMOKE:
			emitterProper.m_emitterTime = 0.25f;
			emitterProper.m_numberOfEmittion = 10.f;
			break;
		case BLOOD:
			emitterProper.m_emitterTime = 0.25f;
			emitterProper.m_numberOfEmittion = 1.f;
			break;
		case FIREFLY:
			emitterProper.m_emitterTime = .5f;
			emitterProper.m_numberOfEmittion = 2.f;
			break;
		case PJUMP:
			emitterProper.m_emitterTime = .0f;
			emitterProper.m_numberOfEmittion = 10.f;
			break;
		case PRAIN:
			emitterProper.m_emitterTime = .5f;
			emitterProper.m_numberOfEmittion = 2.f;
			break;
		case PFIREUP:
			emitterProper.m_emitterTime = 0.001f;
			emitterProper.m_numberOfEmittion = 1;
			break;
		case PFIREDOWN:
			emitterProper.m_emitterTime = 0.001f;//getRandomNum(10, 1) * 0.1f;
			emitterProper.m_numberOfEmittion = 1;
			break;
		case PFIREMIDDLE:
			emitterProper.m_emitterTime = 0.001f;//getRandomNum(10, 1) * 0.1f;
			emitterProper.m_numberOfEmittion = 1;
			break;
		case PMETEOR:
			emitterProper.m_emitterTime = 0.f;
			emitterProper.m_numberOfEmittion = 1;
			break;
		case PFLY:
			emitterProper.m_emitterTime = 1.5f;
			emitterProper.m_numberOfEmittion = 2;
			break;
	}

	m_emitterList.push_back(emitterProper);
	return &m_emitterList.back();
}

void emitter::setNumberOfParticle(int numberOfParticle)
{
	m_numberOfParticle = numberOfParticle;
}

int emitter::getNumberOfParticle(void)
{
	return m_numberOfParticle;
}


//void emitter::deleteEmitter(EMITTER_ID id)
//{
//	emitterItor it_emitter = findEmitter(id);
//	
//	if (it_emitter != m_emitterList.end())
//		it_emitter->m_numberOfParticle = 0;
//}

void emitter::changeEmitterPos(JE::Vector3f position)
{
	m_position = position;//result;
}

void emitter::changeEmitterRange(float range)
{
	m_range = range;
}

emitter::emitterItor emitter::findEmitter(EMITTER_ID id)
{
	if (m_emitterList.begin() != m_emitterList.end())
	{
		for (emitterItor m_it_emitter = m_emitterList.begin(); m_it_emitter != m_emitterList.end(); ++m_it_emitter)
		{
			if (m_it_emitter->m_emitterID == id)
			{
				return m_it_emitter;
			}
		}
	}
	return m_emitterList.end();
}

void emitter::updateEmitter(float deltaTime)
{
	emitterItor m_it_emitter = m_emitterList.begin();
	int count;
	while (m_it_emitter != m_emitterList.end())
	{
		if (m_it_emitter->m_timer >= m_it_emitter->m_emitterTime 
			&& m_it_emitter->m_numberOfParticle > 0)
		{
			for (int count = 0; count < m_it_emitter->m_numberOfEmittion; ++count)
			{
				m_it_emitter->m_particle.initParticle(m_it_emitter->m_type,
													  m_it_emitter->m_textureID,
													  m_it_emitter->m_position,
													  m_it_emitter->m_objDirection,
													  m_it_emitter->m_range);

				--m_it_emitter->m_numberOfParticle;
			}
			m_it_emitter->m_timer = 0;
		}

		m_it_emitter->m_particle.updateParticle(deltaTime, m_it_emitter->getType());

		m_it_emitter->m_timer += deltaTime;

		count = m_it_emitter->m_numberOfParticle;

		if (m_it_emitter->m_numberOfParticle <= 0)
		{
			if (!m_it_emitter->m_particle.checkAliveParticle())
			{
				m_it_emitter = m_emitterList.erase(m_it_emitter);
			}
			else
			{
				++m_it_emitter;
			}
		}
		else
		{
			++m_it_emitter;
		}
	}
}

void emitter::drawEmitter()
{
	for (emitterItor m_it_emitter = m_emitterList.begin(); m_it_emitter != m_emitterList.end(); ++m_it_emitter)
	{
		if (m_it_emitter->m_projection == PERSPECTIVE)
			m_it_emitter->m_particle.drawPersParticle();
		else if (m_it_emitter->m_projection == ORTHOGONAL)
			m_it_emitter->m_particle.drawOrthoParticle();
	}
}

PARTICLE_TYPE emitter::getType(void)
{
	return m_type;
}

JE::Vector3f emitter::getPosition(void)
{
	return m_position;
}

void emitter::freeEmitter(void)
{
	if (m_emitterList.begin() != m_emitterList.end())
	{
		for (emitterItor m_it_emitter = m_emitterList.begin(); m_it_emitter != m_emitterList.end(); ++m_it_emitter)
		{
			m_it_emitter->m_particle.freeParticle();
		}
		m_emitterList.clear();
	}
}

particle::particle()
{

}

void particle::initParticle(PARTICLE_TYPE type, GLuint textureID, JE::Vector3f position, JE::Vector3f objDirection, float range)
{
	particle particleProper;

	particleProper.m_position = position;
	particleProper.m_textureID = textureID;
	particleProper.m_type = type;

	switch (type)
	{
		case DIRT:
		{
			particleProper.m_lifeTime = .5f;
			particleProper.m_velocity = JE::Vector3f(getRandomNum(10, 1), getRandomNum(10, 1), 0.f);
			particleProper.m_size = JE::Vector2f(2.f, 2.f);
			particleProper.m_accel = JE::Vector3f(getRandomNum(15, 10), getRandomNum(15, 10), 0.f);
			particleProper.m_rotate = 0;
			particleProper.m_rotateVelociy = 500.f;
			particleProper.m_direction = JE::Vector3f(getRandomNum(3, -1), 1.f, 0.f);
			particleProper.m_ambientIntensity = 1.f;

			m_particleList.push_back(particleProper);
			break;
		}
		case SMOKE:
		{
			particleProper.m_position += JE::Vector3f(getRandomNum(2, -2), getRandomNum(2, -2), 0.f);
			particleProper.m_lifeTime = 1.2f;
			particleProper.m_velocity = JE::Vector3f(getRandomNum(3, -2), getRandomNum(3, -2), 0.f);
			particleProper.m_size = JE::Vector2f(3.f, 3.f);
			particleProper.m_accel = JE::Vector3f(0.f, 0.f, 0.f);
			particleProper.m_rotate = 0;
			particleProper.m_rotateVelociy = 0.f;
			particleProper.m_direction = JE::Vector3f(getRandomNum(2, -1), getRandomNum(2, -1), 0.f);
			particleProper.m_ambientIntensity = 0.7f;
			particleProper.m_ambientVelo = 0.01f;

			m_particleList.push_back(particleProper);
			break;
		}
		case BLOOD:
		{
			particleProper.m_lifeTime = .5f;
			particleProper.m_velocity = JE::Vector3f(getRandomNum(10, 1), getRandomNum(10, 1), 0.f);
			particleProper.m_size = JE::Vector2f(2.f, 2.f);
			particleProper.m_accel = JE::Vector3f(getRandomNum(15, 10), getRandomNum(15, 10), 0.f);
			particleProper.m_rotate = 0;
			particleProper.m_rotateVelociy = 500.f;
			particleProper.m_direction = JE::Vector3f(getRandomNum(2, -1), 1.f, 0.f);
			particleProper.m_ambientIntensity = 1.f;

			m_particleList.push_back(particleProper);
			break;
		}
		case FIREFLY:
		{
			particleProper.m_lifeTime = 2.f;
			particleProper.m_lightLifeTime = particleProper.m_lifeTime;
			particleProper.m_lightAmbientIntensity = 0.f;
			particleProper.m_position += JE::Vector3f(getRandomNum(160, -160), getRandomNum(80, -80), 0.f);
			particleProper.m_velocity = JE::Vector3f(getRandomNum(5, 1), getRandomNum(5, 1), 0.f);

			particleProper.m_size = JE::Vector2f(10.f, 10.f);
			
			particleProper.m_accel = JE::Vector3f(getRandomNum(8, 2), getRandomNum(8, 2), 0.f);
			
			particleProper.m_rotate = 0;
			particleProper.m_rotateVelociy = 0.f;
			particleProper.m_direction = JE::Vector3f(getRandomNum(2, -1), getRandomNum(2, -1), 0.f);
			particleProper.m_ambientIntensity = 0.f;
			particleProper.m_ambientVelo = 1.2f;
			
			particleProper.m_lightColor.m_x = 0;//getRandomNum(250, 1);
			particleProper.m_lightColor.m_y = getRandomNum(250, 1);
			particleProper.m_lightColor.m_z = getRandomNum(250, 1);


			m_particleList.push_back(particleProper);
			JE::JEAddPointLight(int(&m_particleList.back()), particleProper.m_position, particleProper.m_lightColor, particleProper.m_lightAmbientIntensity, 0.f);

			break;
		}

		case PJUMP:
		{
			JE::Vector3f objRandomDirection;

			if (objDirection.m_x < 0)
			{
				objRandomDirection = JE::Vector3f(getRandomNum(10, -2), -2.f, 0.f);
			}
			else if (objDirection.m_x > 0)
			{
				objRandomDirection = JE::Vector3f(getRandomNum(2, -10), -2.f, 0.f);
			}
			else
			{
				objRandomDirection = JE::Vector3f(getRandomNum(5, -5), 0.f, 0.f);
			}
			particleProper.m_position += JE::Vector3f(getRandomNum(5, -5), getRandomNum(-2, -4), 0.f);//JE::Vector3f(getRandomNum(2, -2), -4.f, 0.f);
			particleProper.m_lifeTime = 1.2f;
			particleProper.m_velocity = objRandomDirection;//JE::Vector3f(getRandomNum(5, -5), 0.f, 0.f);
			particleProper.m_size = JE::Vector2f(3.f, 3.f);
			particleProper.m_accel = JE::Vector3f(getRandomNum(10, 2), 0.f, 0.f);//JE::Vector3f(0.f, 0.f, 0.f);
			particleProper.m_rotate = 0;
			particleProper.m_rotateVelociy = 0.f;
			particleProper.m_direction = JE::Vector3f(-objDirection.m_x, 0.f, 0.f);//JE::Vector3f(-objDirection.m_x, getRandomNum(-1, -2), 0.f);
			particleProper.m_ambientIntensity = 0.7f;
			particleProper.m_ambientVelo = 0.4f;

			m_particleList.push_back(particleProper);
			break;
		}
		case PRAIN:
		{
			float range = getRandomNum(3, -2) / 10.f;
			
			particleProper.m_position += JE::Vector3f(range, .0f, 0.f);
			particleProper.m_lifeTime = 1.f;
			particleProper.m_lightLifeTime = particleProper.m_lifeTime;
			particleProper.m_velocity = JE::Vector3f(0, -.2f, 0.f);
			particleProper.m_size = JE::Vector2f(0.035f, 0.035f);
			particleProper.m_accel = JE::Vector3f(0.f, 0.f, 0.f);
			particleProper.m_rotate = 0;
			particleProper.m_rotateVelociy = 0.f;
			particleProper.m_direction = JE::Vector3f(0.f, -1.f, 0.f);
			particleProper.m_ambientIntensity = 0.f;
			particleProper.m_ambientVelo = 4.f;

			m_particleList.push_back(particleProper);
			break;
		}

		case PFIREUP:
		{
			if (range == 0)
			{
				particleProper.m_position += JE::Vector3f(0, getRandomNum(100, -100) * 0.01f, 0.f);
			}
			else
			{
				int intRange = int(range) * 10;
				particleProper.m_position += JE::Vector3f(0, getRandomNum(intRange, -intRange) * 0.01f, 0.f);
			}
			particleProper.m_lifeTime = 1.45f;
			particleProper.m_lightLifeTime = particleProper.m_lifeTime;

			particleProper.m_velocity = JE::Vector3f(15.f, 0.f, 0.f);
			particleProper.m_size = JE::Vector2f(1., 1.f);
			particleProper.m_accel = JE::Vector3f(5.f, 0.f, 0.f);
			particleProper.m_rotate = 0;
			particleProper.m_rotateVelociy = 0.f;
			particleProper.m_direction = JE::Vector3f(1.f,0.f, 0.f);
			particleProper.m_ambientIntensity = 1.f;
			particleProper.m_ambientVelo = getRandomNum(1000, 800) * .001f;

			m_particleList.push_back(particleProper);
			break;
		}

		case PFIREDOWN:
		{
			if (range == 0)
			{
				particleProper.m_position += JE::Vector3f(0, getRandomNum(100, -100) * 0.01f, 0.f);
			}
			else
			{
				int intRange = int(range) * 10;
				particleProper.m_position += JE::Vector3f(0, getRandomNum(intRange, -intRange) * 0.01f, 0.f);
			}
			particleProper.m_lifeTime = 1.45f;
			particleProper.m_lightLifeTime = particleProper.m_lifeTime;
			
			particleProper.m_velocity = JE::Vector3f(15.f, 0.f, 0.f);
			particleProper.m_size = JE::Vector2f(1., 1.f);
			particleProper.m_accel = JE::Vector3f(5.f, 0.f, 0.f);
			particleProper.m_rotate = 0;
			particleProper.m_rotateVelociy = 0.f;
			particleProper.m_direction = JE::Vector3f(1.f, 0.f, 0.f);
			particleProper.m_ambientIntensity = 1.f;
			particleProper.m_ambientVelo = getRandomNum(1000, 800) * .001f;

			m_particleList.push_back(particleProper);
			
			break;
		}

		case PFIREMIDDLE:
		{
			if (range == 0)
			{
				particleProper.m_position += JE::Vector3f(0, getRandomNum(100, -100) * 0.01f, 0.f);
			}
			else
			{
				int intRange = int(range) * 10;
				particleProper.m_position += JE::Vector3f(0, getRandomNum(intRange, -intRange) * 0.01f, 0.f);

			}
			particleProper.m_lifeTime = 1.f;
			particleProper.m_lightLifeTime = particleProper.m_lifeTime;
			particleProper.m_velocity = JE::Vector3f(30.f, 0.f, 0.f);
			particleProper.m_size = JE::Vector2f(1.f, 1.f);
			particleProper.m_accel = JE::Vector3f(4.f, 0.f, 0.f);
			particleProper.m_rotate = 0;
			particleProper.m_rotateVelociy = 0.f;
			particleProper.m_direction = JE::Vector3f(-1.f, 0.f, 0.f);
			particleProper.m_ambientIntensity = 1.f;
			particleProper.m_ambientVelo = getRandomNum(1000, 800) * .001f;

			m_particleList.push_back(particleProper);
			break;
		}

		case PMETEOR:
		{
			int intRange = int(range) * 30;
			particleProper.m_position += JE::Vector3f(getRandomNum(intRange, -intRange) * 0.01f, getRandomNum(intRange, -intRange) * 0.01f, 0.f);
		
			int maxSize = int(range) * 100;
			int minSize = (int(range) - 3) * 100;

			particleProper.m_lifeTime = 1.f;
			particleProper.m_lightLifeTime = particleProper.m_lifeTime;
			particleProper.m_velocity = JE::Vector3f(10.f, 10.f, 0.f);
			particleProper.m_size = JE::Vector2f(getRandomNum(maxSize, minSize) * 0.01f,
												 getRandomNum(maxSize, minSize) * 0.01f);
			particleProper.m_accel = JE::Vector3f(getRandomNum(10, 8), getRandomNum(10, 8), 0.f);
			particleProper.m_rotate = 0;
			particleProper.m_rotateVelociy = 0.f;
			particleProper.m_direction = JE::Vector3f(-objDirection.m_x, -objDirection.m_y, 0.f);
			particleProper.m_ambientIntensity = 1.f;
			particleProper.m_ambientVelo = getRandomNum(1000, 800) * .001f;

			m_particleList.push_back(particleProper);
			break;
		}

		case PFLY:
		{
			particleProper.m_position += JE::Vector3f(getRandomNum(200, -200) * 0.01f, getRandomNum(0, -200) * 0.01f, 0.f);
			particleProper.m_lifeTime = 1.f;
			particleProper.m_lightLifeTime = particleProper.m_lifeTime;
			particleProper.m_velocity = JE::Vector3f(0.f, 10.f, 0.f);
			particleProper.m_size = JE::Vector2f(1.f, 1.f);
			particleProper.m_accel = JE::Vector3f(0.f, getRandomNum(10, 8), 0.f);
			particleProper.m_rotate = 0;
			particleProper.m_rotateVelociy = 0.f;
			particleProper.m_direction = JE::Vector3f(0.f, getRandomNum(100, -100) * 0.01f, 0.f);
			particleProper.m_ambientIntensity = 1.f;
			particleProper.m_ambientVelo = getRandomNum(1000, 800) * .001f;

			m_particleList.push_back(particleProper);
			break;
		}
	}
}

void particle::updateParticle(float deltaTime, PARTICLE_TYPE type)
{
	switch (type)
	{
		case DIRT:
		{
			particleItor m_it_particle = m_particleList.begin();
			while (m_it_particle != m_particleList.end())
			{
				m_it_particle->m_velocity = m_it_particle->m_velocity + (m_it_particle->m_accel * deltaTime) * m_it_particle->m_direction * JE::Vector3f(1.0f, -.98f, 0.0f);
				m_it_particle->m_position = m_it_particle->m_position + (m_it_particle->m_velocity * deltaTime);
				m_it_particle->m_lifeTime -= deltaTime;
				m_it_particle->m_rotate = m_it_particle->m_rotate + (m_it_particle->m_rotateVelociy * deltaTime);

				if (m_it_particle->m_ambientIntensity >= 0)
					m_it_particle->m_ambientIntensity -= deltaTime;

				if (m_it_particle->m_lifeTime <= 0)
				{
					m_it_particle = m_particleList.erase(m_it_particle);
				}
				else
				{
					++m_it_particle;
				}
			}
			break;
		}
		case SMOKE:
		{
			particleItor m_it_particle = m_particleList.begin();
			while (m_it_particle != m_particleList.end())
			{
				m_it_particle->m_velocity = m_it_particle->m_velocity + (m_it_particle->m_accel * deltaTime) * m_it_particle->m_direction;
				m_it_particle->m_position = m_it_particle->m_position + (m_it_particle->m_velocity * deltaTime);
				m_it_particle->m_lifeTime -= deltaTime;
				m_it_particle->m_rotate = m_it_particle->m_rotate + (m_it_particle->m_rotateVelociy * deltaTime);

				if (m_it_particle->m_ambientIntensity >= 0)
					m_it_particle->m_ambientIntensity -= (m_it_particle->m_ambientVelo * deltaTime);

				if (m_it_particle->m_lifeTime <= 0)
				{
					m_it_particle = m_particleList.erase(m_it_particle);
				}
				else
				{
					++m_it_particle;
				}
			}
			break;
		}
		case BLOOD:
		{
			particleItor m_it_particle = m_particleList.begin();
			while (m_it_particle != m_particleList.end())
			{
				m_it_particle->m_velocity = m_it_particle->m_velocity + (m_it_particle->m_accel * deltaTime) * m_it_particle->m_direction * JE::Vector3f(1.0f, -.98f, 0.0f);
				m_it_particle->m_position = m_it_particle->m_position + (m_it_particle->m_velocity * deltaTime);
				m_it_particle->m_lifeTime -= deltaTime;
				m_it_particle->m_rotate = m_it_particle->m_rotate + (m_it_particle->m_rotateVelociy * deltaTime);

				if (m_it_particle->m_ambientIntensity >= 0)
					m_it_particle->m_ambientIntensity -= deltaTime;

				if (m_it_particle->m_lifeTime <= 0)
				{
					m_it_particle = m_particleList.erase(m_it_particle);
				}
				else
				{
					++m_it_particle;
				}
			}
			break;
		}
		case FIREFLY:
		{
			particleItor m_it_particle = m_particleList.begin();
			while (m_it_particle != m_particleList.end())
			{
				m_it_particle->m_velocity = m_it_particle->m_velocity + (m_it_particle->m_accel * deltaTime) * m_it_particle->m_direction;
				m_it_particle->m_position = m_it_particle->m_position + (m_it_particle->m_velocity * deltaTime);
				m_it_particle->m_lifeTime -= deltaTime;
				m_it_particle->m_rotate = m_it_particle->m_rotate + (m_it_particle->m_rotateVelociy * deltaTime);

				if (m_it_particle->m_lightLifeTime / 2 <= m_it_particle->m_lifeTime)
				{
					m_it_particle->m_lightAmbientIntensity += 0.0005f;
				}
				else if (m_it_particle->m_lightLifeTime / 2 > m_it_particle->m_lifeTime)
				{
					m_it_particle->m_lightAmbientIntensity -= 0.0005f;
				}

				JE::JEChangePointLight(int(&(*m_it_particle)), m_it_particle->m_position, m_it_particle->m_lightColor, m_it_particle->m_lightAmbientIntensity, 0.f);

				if (m_it_particle->m_lifeTime <= 0)
				{
					JE::JEDeletePointLight(int(&(*m_it_particle)));
					m_it_particle = m_particleList.erase(m_it_particle);
				}
				else
				{
					++m_it_particle;
				}
			}
			break;
		}

		case PJUMP:
		{
			particleItor m_it_particle = m_particleList.begin();
			while (m_it_particle != m_particleList.end())
			{
				m_it_particle->m_velocity = m_it_particle->m_velocity + (m_it_particle->m_accel * deltaTime) * m_it_particle->m_direction;
				m_it_particle->m_position = m_it_particle->m_position + (m_it_particle->m_velocity * deltaTime);
				m_it_particle->m_lifeTime -= deltaTime;
				m_it_particle->m_rotate = m_it_particle->m_rotate + (m_it_particle->m_rotateVelociy * deltaTime);

				if (m_it_particle->m_ambientIntensity >= 0)
					m_it_particle->m_ambientIntensity -= (m_it_particle->m_ambientVelo * deltaTime);

				if (m_it_particle->m_lifeTime <= 0)
				{
					m_it_particle = m_particleList.erase(m_it_particle);
				}
				else
				{
					++m_it_particle;
				}
			}
			break;
		}

		case PRAIN:
		{
			particleItor m_it_particle = m_particleList.begin();
			while (m_it_particle != m_particleList.end())
			{
				m_it_particle->m_velocity = m_it_particle->m_velocity + (m_it_particle->m_accel * deltaTime) * m_it_particle->m_direction;
				m_it_particle->m_position = m_it_particle->m_position + (m_it_particle->m_velocity * deltaTime);
				m_it_particle->m_lifeTime -= deltaTime;
				m_it_particle->m_rotate = m_it_particle->m_rotate + (m_it_particle->m_rotateVelociy * deltaTime);


				if (m_it_particle->m_lightLifeTime / 2 <= m_it_particle->m_lifeTime)
				{
					m_it_particle->m_ambientIntensity += (m_it_particle->m_ambientVelo * deltaTime);
				}
				else if (m_it_particle->m_lightLifeTime / 2 > m_it_particle->m_lifeTime)
				{
					m_it_particle->m_ambientIntensity -= (m_it_particle->m_ambientVelo * deltaTime);
				}

				if (m_it_particle->m_lifeTime <= 0)
				{
					m_it_particle = m_particleList.erase(m_it_particle);
				}
				else
				{
					++m_it_particle;
				}
			}
			break;
		}

		case PFIREUP:
		{
			particleItor m_it_particle = m_particleList.begin();
			while (m_it_particle != m_particleList.end())
			{
				m_it_particle->m_velocity.m_y = m_it_particle->m_velocity.m_y + (m_it_particle->m_accel.m_y * deltaTime) * m_it_particle->m_direction.m_y;
				m_it_particle->m_velocity.m_x = m_it_particle->m_velocity.m_x + (m_it_particle->m_accel.m_x * deltaTime) * m_it_particle->m_direction.m_x;

				
				m_it_particle->m_velocity.m_y = getRandomNum(10, 1)* cosf(m_it_particle->m_velocity.m_x);
			
				//m_it_particle->m_velocity.m_x = m_it_particle->m_velocity.m_y * m_it_particle->m_velocity.m_y;

				m_it_particle->m_position = m_it_particle->m_position + (m_it_particle->m_velocity * deltaTime);
				m_it_particle->m_lifeTime -= deltaTime;
				m_it_particle->m_rotate = m_it_particle->m_rotate + (m_it_particle->m_rotateVelociy * deltaTime);


				if (m_it_particle->m_ambientIntensity >= 0)
					m_it_particle->m_ambientIntensity -= (m_it_particle->m_ambientVelo * deltaTime);

				if (m_it_particle->m_lifeTime <= 0 && m_it_particle->m_ambientIntensity <= 0)
				{
					m_it_particle = m_particleList.erase(m_it_particle);
				}
				else
				{
					++m_it_particle;
				}
			}
			break;
		}
		case PFIREDOWN:
		{
			particleItor m_it_particle = m_particleList.begin();
			while (m_it_particle != m_particleList.end())
			{
				m_it_particle->m_velocity.m_y = m_it_particle->m_velocity.m_y + (m_it_particle->m_accel.m_y * deltaTime) * m_it_particle->m_direction.m_y;
				m_it_particle->m_velocity.m_x = m_it_particle->m_velocity.m_x + (m_it_particle->m_accel.m_x * deltaTime) * m_it_particle->m_direction.m_x;

				m_it_particle->m_velocity.m_y = -getRandomNum(10, 1) * cosf(m_it_particle->m_velocity.m_x);
				

				//m_it_particle->m_velocity.m_x = m_it_particle->m_velocity.m_y * m_it_particle->m_velocity.m_y;

				m_it_particle->m_position = m_it_particle->m_position + (m_it_particle->m_velocity * deltaTime);
				m_it_particle->m_lifeTime -= deltaTime;
				m_it_particle->m_rotate = m_it_particle->m_rotate + (m_it_particle->m_rotateVelociy * deltaTime);


				if (m_it_particle->m_ambientIntensity >= 0)
					m_it_particle->m_ambientIntensity -= (m_it_particle->m_ambientVelo * deltaTime);

				if (m_it_particle->m_lifeTime <= 0 && m_it_particle->m_ambientIntensity <= 0)
				{
					m_it_particle = m_particleList.erase(m_it_particle);
				}
				else
				{
					++m_it_particle;
				}
			}
			break;
		}

		case PFIREMIDDLE:
		{
			particleItor m_it_particle = m_particleList.begin();
			while (m_it_particle != m_particleList.end())
			{
				m_it_particle->m_velocity.m_y = m_it_particle->m_velocity.m_y + (m_it_particle->m_accel.m_y * deltaTime) * m_it_particle->m_direction.m_y;
				m_it_particle->m_velocity.m_x = m_it_particle->m_velocity.m_x + (m_it_particle->m_accel.m_x * deltaTime) * m_it_particle->m_direction.m_x;


				m_it_particle->m_position = m_it_particle->m_position + (m_it_particle->m_velocity * deltaTime);
				m_it_particle->m_lifeTime -= deltaTime;
				m_it_particle->m_rotate = m_it_particle->m_rotate + (m_it_particle->m_rotateVelociy * deltaTime);


				if (m_it_particle->m_ambientIntensity >= 0)
					m_it_particle->m_ambientIntensity -= (m_it_particle->m_ambientVelo * deltaTime);

				if (m_it_particle->m_lifeTime <= 0 && m_it_particle->m_ambientIntensity <= 0)
				{
					m_it_particle = m_particleList.erase(m_it_particle);
				}
				else
				{
					++m_it_particle;
				}
			}
			break;
		}

		case PMETEOR:
		{
			particleItor m_it_particle = m_particleList.begin();
			while (m_it_particle != m_particleList.end())
			{
				m_it_particle->m_velocity.m_y = m_it_particle->m_velocity.m_y + (m_it_particle->m_accel.m_y * deltaTime) * m_it_particle->m_direction.m_y;
				m_it_particle->m_velocity.m_x = m_it_particle->m_velocity.m_x + (m_it_particle->m_accel.m_x * deltaTime) * m_it_particle->m_direction.m_x;


				m_it_particle->m_position = m_it_particle->m_position + (m_it_particle->m_velocity * deltaTime);
				m_it_particle->m_lifeTime -= deltaTime;
				m_it_particle->m_rotate = m_it_particle->m_rotate + (m_it_particle->m_rotateVelociy * deltaTime);


				if (m_it_particle->m_ambientIntensity >= 0)
					m_it_particle->m_ambientIntensity -= (m_it_particle->m_ambientVelo * deltaTime);

				if (m_it_particle->m_lifeTime <= 0 && m_it_particle->m_ambientIntensity <= 0)
				{
					m_it_particle = m_particleList.erase(m_it_particle);
				}
				else
				{
					++m_it_particle;
				}
			}
			break;
		}

		case PFLY:
		{
			particleItor m_it_particle = m_particleList.begin();
			while (m_it_particle != m_particleList.end())
			{
				m_it_particle->m_velocity.m_y = m_it_particle->m_velocity.m_y + (m_it_particle->m_accel.m_y * deltaTime) * m_it_particle->m_direction.m_y;
				//m_it_particle->m_velocity.m_x = m_it_particle->m_velocity.m_x + (m_it_particle->m_accel.m_x * deltaTime) * m_it_particle->m_direction.m_x;

				//m_it_particle->m_velocity.m_y = sinf(m_it_particle->m_velocity.m_y);
				//m_it_particle->m_velocity.m_x = cosf(m_it_particle->m_velocity.m_x);
				m_it_particle->m_velocity.m_x = -(getRandomNum(1000, 100) * 0.01f) * sinf(m_it_particle->m_velocity.m_y);

				m_it_particle->m_position = m_it_particle->m_position + (m_it_particle->m_velocity * deltaTime);
				m_it_particle->m_lifeTime -= deltaTime;
				m_it_particle->m_rotate = m_it_particle->m_rotate + (m_it_particle->m_rotateVelociy * deltaTime);


				if (m_it_particle->m_ambientIntensity >= 0)
					m_it_particle->m_ambientIntensity -= (m_it_particle->m_ambientVelo * deltaTime);

				if (m_it_particle->m_lifeTime <= 0 && m_it_particle->m_ambientIntensity <= 0)
				{
					m_it_particle = m_particleList.erase(m_it_particle);
				}
				else
				{
					++m_it_particle;
				}
			}
			break;
		}
	}
}

void particle::drawPersParticle(void)
{
	for (particleItor m_it_particle = m_particleList.begin(); m_it_particle != m_particleList.end(); ++m_it_particle)
	{
		JE::GraphicSetTextureUnit(0);
		JE::GraphicSetFilpTex(JE::FLIP_IDLE, 1);
		JE::GraphicSetAnimaTex(1, 0);
		JE::GraphicSetTexture(m_it_particle->m_textureID);

		JE::GraphicSetParticleTransform(m_it_particle->m_position.m_x, m_it_particle->m_position.m_y, 0.f,
			m_it_particle->m_size.m_x, m_it_particle->m_size.m_y,
			m_it_particle->m_rotate,
			m_it_particle->m_ambientIntensity);
	}
}

void particle::drawOrthoParticle(void)
{
	for (particleItor m_it_particle = m_particleList.begin(); m_it_particle != m_particleList.end(); ++m_it_particle)
	{
		JE::GraphicSetTextureUnit(0);
		JE::GraphicSetFilpTex(JE::FLIP_IDLE, 1);
		JE::GraphicSetAnimaTex(1, 0);
		JE::GraphicSetTexture(m_it_particle->m_textureID);

		JE::GraphicSetOrthoParticleTransform(m_it_particle->m_position.m_x, m_it_particle->m_position.m_y, 0.f,
			m_it_particle->m_size.m_x, m_it_particle->m_size.m_y,
			m_it_particle->m_rotate,
			m_it_particle->m_ambientIntensity);
	}
}


float getRandomNum(int maxNum, int minNum)
{
	float num = float(rand() % (maxNum - minNum) + minNum);

	if (num == 0)
	{
		int check = rand() % 99 + 1;
		if (check >= 51)
			num = 1.f;
		else
			num = -1.f;
	}
	/*if (num == 0)
		num = 1;*/
	return num;
}

//test for direction for flip
JE::Vector3f particle::getObjectDirection(JE::Vector3f speed)
{
	return speed;
}

bool particle::checkAliveParticle(void)
{
	bool result = false;

	if (m_particleList.begin() != m_particleList.end())
	{
		particle* pParticle = &m_particleList.back();
		if (pParticle->m_lifeTime >= 0)
		{
			result = true;
			return result;
		}
	}
	return result;
}



void particle::freeParticle(void)
{
	if (m_particleList.begin() != m_particleList.end())
	{
		particleItor m_it_particle = m_particleList.begin();

		if (m_it_particle->m_type == FIREFLY)
		{
			while(m_it_particle != m_particleList.end())
			{
				JE::JEDeletePointLight(int(&(*m_it_particle)));
				++m_it_particle;
			}
			JE::JEUpdatePointLight();
		}
		m_particleList.clear();
	}
}


