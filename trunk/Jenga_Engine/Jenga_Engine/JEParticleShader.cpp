/*******************************************************************************
Copyright (C) 2015 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
filename    JEParticleShader.cpp
author      Seho.Joo
email       jsh2958@gmail.com
course      GAM250
assignment  Final milestone
due date    6/12/2015

Brief Description:
This file is particle shader that executes the new style particle. 

*******************************************************************************/

#include "JEParticleShader.h"

bool ParticleShader::Init()
{
	if (!Shader::Init())
		return false;

	Shader::AddShader("vertexShader/particleShader.vert", GL_VERTEX_SHADER);

	Shader::AddShader("geometryShader/particleShader.geo", GL_GEOMETRY_SHADER);

	Shader::AddShader("fragmentShader/particleShader.frag", GL_FRAGMENT_SHADER);

	Shader::ExecuteShader();

	const GLchar* varyings[4];
	varyings[0] = "Type1";
	varyings[1] = "Position1";
	varyings[2] = "Velocity1";
	varyings[3] = "Age1";

	glTransformFeedbackVaryings(m_shaderProgram, 4, varyings, GL_INTERLEAVED_ATTRIBS);

	Shader::ExecuteShader();

	m_deltaTimeMillisLocation			= GetUniformLocation("gDeltaTimeMillis");
	m_randomTextureLocation				= GetUniformLocation("gRandomTexture");
	m_timeLocation						= GetUniformLocation("gTime");
	m_launcherLifetimeLocation			= GetUniformLocation("gLauncherLifetime");
	m_shellLifetimeLocation				= GetUniformLocation("gShellLifetime");
	m_secondaryShellLifetimeLocation	= GetUniformLocation("gSecondaryShellLifetime");

	return true;
}

void ParticleShader::SetDeltaTimeMillis(unsigned int deltaTimeMillis)
{
	glUniform1f(m_deltaTimeMillisLocation, (float)deltaTimeMillis);
}

void ParticleShader::SetTime(int time)
{
	glUniform1f(m_timeLocation, (float)time);
}

void ParticleShader::SetRandomTextureUnit(unsigned int textureUnit)
{
	glUniform1i(m_randomTextureLocation, textureUnit);
}

void ParticleShader::SetLauncherLifeTime(float lifeTime)
{
	glUniform1f(m_launcherLifetimeLocation, lifeTime);
}

void ParticleShader::SetShellLifeTime(float lifeTime)
{
	glUniform1f(m_shellLifetimeLocation, lifeTime);
}

void ParticleShader::SetSecondaryShellLifetime(float lifeTime)
{
	glUniform1f(m_secondaryShellLifetimeLocation, lifeTime);
}