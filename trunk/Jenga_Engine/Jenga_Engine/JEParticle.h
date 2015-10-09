/*******************************************************************************
Copyright (C) 2015 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
filename    JEParticle.h
author      Seho.Joo
email       jsh2958@gmail.com
course      GAM250
assignment  Final milestone
due date    6/12/2015

Brief Description:
This file is header file for JEParticle.cpp.

*******************************************************************************/

#ifndef  JEPARTICLE_H
#define  JEPARTICLE_H

//For openGL
#include "glew.h"
#include <gl\GL.h>
#include <gl\GLU.h>

#include "JEOGLMathHelper.h"
#include "JEParticleShader.h"
#include "JEBillboardingShader.h"
#include "JERandomTexture.h"

class ParticleSystem
{
public:
	ParticleSystem();

	~ParticleSystem();

	bool InitParticleSystem(const char* pfileName, const JE::Vector3f& pos);

	void Render(int deltaTimeMillis, const JE::Matrix4f& viewPerspective, const JE::Vector3f& cameraPos);

	bool glCheckErrors();

private:
	void UpdateParticles(int deltaTimeMillis);
	void RenderParticles(const JE::Matrix4f& viewPerspective, const JE::Vector3f& cameraPos);


	bool m_isFirst;
	unsigned int m_curVertexBuffer;
	unsigned int m_curTransFeedBack;
	GLuint m_particleBuffer[2];
	GLuint m_transformFeedback[2];

	ParticleShader m_particleShader;
	Billboarding   m_billboardingShader;

	RandomTexture m_randomTexture;

	GLuint m_textureID;
	int m_time;
};

struct Particle
{
	float type;
	JE::Vector3f pos;
	JE::Vector3f vel;
	float lifeTime;
};

#endif