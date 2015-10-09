/*******************************************************************************
Copyright (C) 2015 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
filename    JEParticle.cpp
author      Seho.Joo
email       jsh2958@gmail.com
course      GAM250
assignment  Final milestone
due date    6/12/2015

Brief Description:
This file executes new style particle that uses GPU. But this file doesn't work

*******************************************************************************/

#include "JEParticle.h"
#include "JELoadTextureNew.h"
#include "JEGraphicsNew.h"
#include "JEDebug.h"
#define MAX_PARTICLES 1000


#define PARTICLE_TYPE_LAUNCHER 0.0f
#define PARTICLE_TYPE_SHELL 1.0f
#define PARTICLE_TYPE_SECONDARY_SHELL 2.0f

ParticleSystem::ParticleSystem()
{
	m_isFirst = true;
	m_curVertexBuffer  = 0;
	m_curTransFeedBack = 1;
	m_time = 0;
	m_textureID = NULL;

	memset(m_particleBuffer, 0, sizeof(m_particleBuffer));
	memset(m_transformFeedback, 0, sizeof(m_transformFeedback));
}

ParticleSystem::~ParticleSystem()
{
	JE::GraphicUnloadTexture(m_textureID);

	if (m_transformFeedback[0] != 0)
		glDeleteTransformFeedbacks(2, m_transformFeedback);

	if (m_particleBuffer[0] != 0)
		glDeleteBuffers(2, m_particleBuffer);
}



bool ParticleSystem::InitParticleSystem(const char* pfileName, const JE::Vector3f& pos)
{
	Particle Particles[MAX_PARTICLES];
	memset(Particles, 0, sizeof(Particles));

	Particles[0].type     = PARTICLE_TYPE_LAUNCHER;
	Particles[0].pos      = pos;
	Particles[0].vel      = JE::Vector3f(0.0f, 0.0001f, 0.0f);
	Particles[0].lifeTime = 0.0f;

	//For drawing into one while using the other as input.
	//So, it need two buffer
	glGenTransformFeedbacks(2, m_transformFeedback);
	//Create two buffer objects for transform feedback buffer
	glGenBuffers(2, m_particleBuffer);

	for (unsigned int i = 0; i < 2; i++)
	{
		//Bind buffer
		glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, m_transformFeedback[i]);
		glBindBuffer(GL_ARRAY_BUFFER, m_particleBuffer[i]);
		//Allocates size bytes of storage for the buffer object bound to target.
		//Put the data into buffer
		glBufferData(GL_ARRAY_BUFFER, sizeof(Particles), Particles, GL_DYNAMIC_DRAW);
		//Bind the particle buffer and feedback buffer
		glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, m_particleBuffer[i]);
	}

	if (!m_particleShader.Init())
		return false;

	m_particleShader.UseProgram();

	m_particleShader.SetRandomTextureUnit(3);
	m_particleShader.SetLauncherLifeTime(100.0f);
	m_particleShader.SetShellLifeTime(10000.0f);
	m_particleShader.SetSecondaryShellLifetime(2500.0f);

	if (!m_randomTexture.InitRandomTexture(1000))
		return false;

	m_randomTexture.SetTexture(GL_TEXTURE3);

	if (!m_billboardingShader.Init())
		return false;

	m_billboardingShader.UseProgram();

	m_billboardingShader.SetBillboardSize(0.01f);

	m_billboardingShader.SetColorTextureUnit(0);

	m_textureID = JE::GraphicLoadTexture(pfileName);

	return glCheckErrors();
}

bool ParticleSystem::glCheckErrors()
{
	GLenum errNum = glGetError();
	if (errNum != GL_NO_ERROR)
	{
		JEPRINTF("OpenGL error: %s\n", gluErrorString(errNum));
		
		return false;
	}
	return true;
}

void ParticleSystem::Render(int deltaTimeMillis, const JE::Matrix4f& viewPerspective, const JE::Vector3f& cameraPos)
{
	m_time += deltaTimeMillis;

	UpdateParticles(deltaTimeMillis);
	RenderParticles(viewPerspective, cameraPos);

	m_curVertexBuffer = m_curTransFeedBack;
	m_curTransFeedBack = (m_curTransFeedBack + 1) & 0x1;
}

void ParticleSystem::UpdateParticles(int deltaTimeMillis)
{
	m_particleShader.UseProgram();
	m_particleShader.SetTime(m_time);
	m_particleShader.SetDeltaTimeMillis(deltaTimeMillis);

	m_randomTexture.SetTexture(GL_TEXTURE3);

	glEnable(GL_RASTERIZER_DISCARD);

	glBindBuffer(GL_ARRAY_BUFFER, m_particleBuffer[m_curVertexBuffer]);
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, m_transformFeedback[m_curTransFeedBack]);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);

	glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), 0);                          // type
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (const GLvoid*)4);         // position
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (const GLvoid*)16);        // velocity
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (const GLvoid*)28);          // lifetime

	glBeginTransformFeedback(GL_POINTS);

	if (m_isFirst) 
	{
		glDrawArrays(GL_POINTS, 0, 1);

		m_isFirst = false;
	}
	else 
		glDrawTransformFeedback(GL_POINTS, m_transformFeedback[m_curVertexBuffer]);

	glEndTransformFeedback();

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
}

void ParticleSystem::RenderParticles(const JE::Matrix4f& viewPerspective, const JE::Vector3f& cameraPos)
{
	m_billboardingShader.UseProgram();
	m_billboardingShader.SetCameraPosition(cameraPos);
	m_billboardingShader.SetViewPerspective(viewPerspective);
	JE::GraphicSetTexture(m_textureID);

	glDisable(GL_RASTERIZER_DISCARD);

	glBindBuffer(GL_ARRAY_BUFFER, m_particleBuffer[m_curTransFeedBack]);

	glEnableVertexAttribArray(0);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (const GLvoid*)4); 

	glDrawTransformFeedback(GL_POINTS, m_transformFeedback[m_curTransFeedBack]);

	glDisableVertexAttribArray(0);
}