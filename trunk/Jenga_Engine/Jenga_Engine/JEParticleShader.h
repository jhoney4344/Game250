/*******************************************************************************
Copyright (C) 2015 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
filename    JEParticleShader.h
author      Seho.Joo
email       jsh2958@gmail.com
course      GAM250
assignment  Final milestone
due date    6/12/2015

Brief Description:
This file is header file for JEParticleShader.cpp

*******************************************************************************/

#ifndef  JEPARTICLESHADER_H
#define  JEPARTICLESHADER_H

#include "JEShader.h"


class ParticleShader : public Shader
{
public:
	virtual bool Init();

	void SetDeltaTimeMillis(unsigned int deltaTimeMillis);

	void SetTime(int time);

	void SetRandomTextureUnit(unsigned int textureUnit);

	void SetLauncherLifeTime(float lifeTime);

	void SetShellLifeTime(float lifeTime);

	void SetSecondaryShellLifetime(float lifeTime);

private:
	GLuint m_deltaTimeMillisLocation;
	GLuint m_randomTextureLocation;
	GLuint m_timeLocation;
	GLuint m_launcherLifetimeLocation;
	GLuint m_shellLifetimeLocation;
	GLuint m_secondaryShellLifetimeLocation;
};


#endif