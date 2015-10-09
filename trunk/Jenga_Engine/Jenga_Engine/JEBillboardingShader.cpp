/*******************************************************************************
Copyright (C) 2015 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
filename    JEBillboardingShader.cpp
author      Seho.Joo
email       jsh2958@gmail.com
course      GAM250
assignment  Final milestone
due date    6/12/2015

Brief Description:
This file executes billboarding shader.

*******************************************************************************/

#include "JEBillboardingShader.h"

/*******************************************************************************
Function: Billboarding

Description: This function is constructor for billboarding shading

Inputs: None.

Outputs: None.
*******************************************************************************/
Billboarding::Billboarding()
{

}

/*******************************************************************************
Function: Init

Description: This function initializes the billboarding shade.

Inputs: None.

Outputs: return checking whether billboarding shader works or not.
*******************************************************************************/
bool Billboarding::Init()
{
	if (!Shader::Init())
		return false;

	Shader::AddShader("vertexShader/billboardShader.vert", GL_VERTEX_SHADER);
	
	Shader::AddShader("geometryShader/billboardShader.geo", GL_GEOMETRY_SHADER);
	
	Shader::AddShader("fragmentShader/billboardShader.frag", GL_FRAGMENT_SHADER);

	Shader::ExecuteShader();


	m_viewPerspectiveLocation = GetUniformLocation("gViewPerspective");
	
	m_cameraPosLocation = GetUniformLocation("gCameraPos");
	
	m_colorTexLocation = GetUniformLocation("gColorMap");

	m_billboardSize = GetUniformLocation("gBillboardSize");

	return true;
}

/*******************************************************************************
Function: SetViewPerspective

Description: This function put the matrix that calculates view space 
             to some location in shader.

Inputs: Matrix for calculating view space.

Outputs: None.
*******************************************************************************/
void Billboarding::SetViewPerspective(const JE::Matrix4f& viewPerspective)
{
	glUniformMatrix4fv(m_viewPerspectiveLocation, 1, GL_TRUE, (const GLfloat*)viewPerspective.m);
}

/*******************************************************************************
Function: SetCameraPosition

Description: This function put the camera position to some location in shader.

Inputs: Matrix for calculating view space.

Outputs: None.
*******************************************************************************/
void Billboarding::SetCameraPosition(const JE::Vector3f& pos)
{
	glUniform3f(m_cameraPosLocation, pos.m_x, pos.m_y, pos.m_z);
}

/*******************************************************************************
Function: SetColorTextureUnit

Description: This function put the texture to some location in shader.

Inputs: texture ID.

Outputs: None.
*******************************************************************************/
void Billboarding::SetColorTextureUnit(unsigned int textureUint)
{
	glUniform1i(m_colorTexLocation, textureUint);
}

/*******************************************************************************
Function: SetBillboardSize

Description: This function put the billboard size to some location in shader.

Inputs: The size of billbaord

Outputs: None.
*******************************************************************************/
void Billboarding::SetBillboardSize(float billboardSize)
{
	glUniform1f(m_billboardSize, billboardSize);
}