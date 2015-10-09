/*******************************************************************************
Copyright (C) 2015 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
filename    JERandomTexture.cpp
author      Seho.Joo
email       jsh2958@gmail.com
course      GAM250
assignment  Final milestone
due date    6/12/2015

Brief Description:
This file gives us random texture direction.

*******************************************************************************/

#include "JERandomTexture.h"
#include "JEOGLMathHelper.h"

#define GLCheckError() (glGetError() == GL_NO_ERROR)

RandomTexture::RandomTexture()
{
	m_textureObj = 0;
}

RandomTexture::~RandomTexture()
{
	if (m_textureObj != 0)
		glDeleteTextures(1, &m_textureObj);
}

bool RandomTexture::InitRandomTexture(unsigned int size)
{
	JE::Vector3f* pRandomData = new JE::Vector3f[size];
	for (unsigned int i = 0; i < size; ++i)
	{
		pRandomData[i].m_x = JE::RandomFloat();
		pRandomData[i].m_y = JE::RandomFloat();
		pRandomData[i].m_z = JE::RandomFloat();
	}

	glGenTextures(1, &m_textureObj);
	glBindTexture(GL_TEXTURE_1D, m_textureObj);
	glTexImage1D(GL_TEXTURE_1D, 0, GL_RGB, size, 0, GL_RGB, GL_FLOAT, pRandomData);
	glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);

	delete[] pRandomData;

	return GLCheckError();
}

void RandomTexture::SetTexture(GLenum TextureUint)
{
	glActiveTexture(TextureUint);
	glBindTexture(GL_TEXTURE_1D, m_textureObj);
}