/*******************************************************************************
Copyright (C) 2015 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
filename    JERandomTexture.h
author      Seho.Joo
email       jsh2958@gmail.com
course      GAM250
assignment  Final milestone
due date    6/12/2015

Brief Description:
This header file is for JERandomTexture.cpp.

*******************************************************************************/

#ifndef JERANDOMTEXTURE_H
#define	JERANDOMTEXTURE_H

#include "glew.h"
#include <iostream>

class RandomTexture
{
public:
	RandomTexture();

	~RandomTexture();

	bool InitRandomTexture(unsigned int Size);

	void SetTexture(GLenum TextureUint);

	bool glCheckErrors();

private:
	GLuint m_textureObj;
};


#endif