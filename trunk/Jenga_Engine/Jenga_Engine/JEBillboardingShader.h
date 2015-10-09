/*******************************************************************************
Copyright (C) 2015 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
filename    JEBillboardingShader.h
author      Seho.Joo
email       jsh2958@gmail.com
course      GAM250
assignment  Final milestone
due date    6/12/2015

Brief Description:
This file is header file that executes billboarding shader.

*******************************************************************************/

#ifndef JEBILLBOARDINGSHADER_H
#define JEBILLBOARDINGSHADER_H

#include "JEShader.h"

class Billboarding : public Shader
{
public:

	Billboarding();

	virtual bool Init();

	void SetViewPerspective(const JE::Matrix4f& viewPerspective);
	void SetCameraPosition(const JE::Vector3f& pos);
	void SetColorTextureUnit(unsigned int textureUint);
	void SetBillboardSize(float billboardSize);

private:
	GLuint m_viewPerspectiveLocation;
	GLuint m_cameraPosLocation;
	GLuint m_colorTexLocation;
	GLuint m_billboardSize;
};

#endif