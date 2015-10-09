/*******************************************************************************
Copyright (C) 2015 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
filename    JEObjectShader.cpp
author      Seho.Joo
email       jsh2958@gmail.com
course      GAM250
assignment  Final milestone
due date    6/12/2015

Brief Description:
This file controls object shader

*******************************************************************************/

#include "JEObjectShader.h"
#include "JEGraphicsNew.h"


void ObejctShader::SetWolrdViewProjec(const JE::Matrix4f& worldViewProjec)
{
	glUniformMatrix4fv(m_worldViewProjecLocation, 1, GL_TRUE, (const GLfloat*)worldViewProjec.m);
}

void ObejctShader::SetTextureUnit(unsigned int textureUnit)
{
	glUniform1i(m_gSamplerLocation, textureUnit);
}

void ObejctShader::SetAniTex(JE::Vector2f scaleTex_X, JE::Vector2f transTex_X)
{
	glUniform2fv(m_scaleTexLocation, 1, (const GLfloat*)&scaleTex_X);
	glUniform2fv(m_transTexLocation, 1, (const GLfloat*)&transTex_X);
}

void ObejctShader::SetFlipTex(JE::Vector2f flipScale, JE::Vector2f flipTrans)
{
	glUniform2fv(m_flipTexScaleLocation, 1, (const GLfloat*)&flipScale);
	glUniform2fv(m_flipTexTransLocation, 1, (const GLfloat*)&flipTrans);
}

void ObejctShader::SetDirectionalLight(const DirectionalLight& light)
{
	glUniform3f(m_directionLightLocation.m_color, light.m_color.m_x, light.m_color.m_y, light.m_color.m_z);
	glUniform1f(m_directionLightLocation.m_ambientIntensity, light.m_ambientIntensity);
	JE::Vector3f direction = light.m_direction;
	direction.Normalize();
	glUniform3f(m_directionLightLocation.m_direction, direction.m_x, direction.m_y, direction.m_z);
	glUniform1f(m_directionLightLocation.m_diffuseIntensity, light.m_diffuseIntensity);
}

void ObejctShader::SetWorldMatrix(const JE::Matrix4f& world)
{
	glUniformMatrix4fv(m_worldMatrixLocation, 1, GL_TRUE, (const GLfloat*)world.m);

}


void ObejctShader::SetPointLights(unsigned int numLights, PointLight* pLights)
{
	glUniform1i(m_numPointLightsLocation, numLights);

	//PointLight::pointIter it_Point = pLights->GetBeginItor();
	int numberOfLocation = 0;

	//for (unsigned int i = 0; i < numLights; ++i)
	for (PointLight::pointIter it_Point = pLights->GetBeginItor(); it_Point != pLights->GetendItor(); ++it_Point)
	{
		glUniform3f(m_pointLightsLocation[numberOfLocation].m_color, it_Point->m_color.m_x, it_Point->m_color.m_y, it_Point->m_color.m_z);
		glUniform1f(m_pointLightsLocation[numberOfLocation].m_ambientIntensity, it_Point->m_ambientIntensity);
		glUniform1f(m_pointLightsLocation[numberOfLocation].m_diffuseIntensity, it_Point->m_diffuseIntensity);
		glUniform3f(m_pointLightsLocation[numberOfLocation].m_position, it_Point->m_position.m_x, it_Point->m_position.m_y, it_Point->m_position.m_z);
		glUniform1f(m_pointLightsLocation[numberOfLocation].m_constant, it_Point->m_constant);
		glUniform1f(m_pointLightsLocation[numberOfLocation].m_linear, it_Point->m_linear);
		glUniform1f(m_pointLightsLocation[numberOfLocation].m_exp, it_Point->m_exp);

		++numberOfLocation;
	}
}

void ObejctShader::SetSpotLights(unsigned int numLights, SpotLight* pLights)
{
	glUniform1i(m_numSpotLightsLocation, numLights);

	//SpotLight::spotIter it_Spot = pLights->GetBeginItor();
	int numberOfLocation = 0;

	//for (unsigned int i = 0; i < numLights; ++i)
	for (SpotLight::spotIter it_Spot = pLights->GetBeginItor(); it_Spot != pLights->GetendItor(); ++it_Spot)
	{
		glUniform3f(m_spotLightsLocation[numberOfLocation].m_color, it_Spot->m_color.m_x, it_Spot->m_color.m_y, it_Spot->m_color.m_z);
		glUniform1f(m_spotLightsLocation[numberOfLocation].m_ambientIntensity, it_Spot->m_ambientIntensity);
		glUniform1f(m_spotLightsLocation[numberOfLocation].m_diffuseIntensity, it_Spot->m_diffuseIntensity);
		glUniform3f(m_spotLightsLocation[numberOfLocation].m_position, it_Spot->m_position.m_x, it_Spot->m_position.m_y, it_Spot->m_position.m_z);
		JE::Vector3f direction = it_Spot->m_direction;
		direction.Normalize();
		glUniform3f(m_spotLightsLocation[numberOfLocation].m_direction, direction.m_x, direction.m_y, direction.m_z);
		glUniform1f(m_spotLightsLocation[numberOfLocation].m_cutOff, cosf(DEG2RAD(it_Spot->m_cutOff)));
		glUniform1f(m_spotLightsLocation[numberOfLocation].m_constant, it_Spot->m_constant);
		glUniform1f(m_spotLightsLocation[numberOfLocation].m_linear, it_Spot->m_linear);
		glUniform1f(m_spotLightsLocation[numberOfLocation].m_exp, it_Spot->m_exp);

		++numberOfLocation;
		/*glUniform3f(m_spotLightsLocation[i].m_color, pLights[i].m_color.m_x, pLights[i].m_color.m_y, pLights[i].m_color.m_z);
		glUniform1f(m_spotLightsLocation[i].m_ambientIntensity, pLights[i].m_ambientIntensity);
		glUniform1f(m_spotLightsLocation[i].m_diffuseIntensity, pLights[i].m_diffuseIntensity);
		glUniform3f(m_spotLightsLocation[i].m_position, pLights[i].m_position.m_x, pLights[i].m_position.m_y, pLights[i].m_position.m_z);
		JE::Vector3f direction = pLights[i].m_direction;
		direction.Normalize();
		glUniform3f(m_spotLightsLocation[i].m_direction, direction.m_x, direction.m_y, direction.m_z);
		glUniform1f(m_spotLightsLocation[i].m_cutOff, cosf(DEG2RAD(pLights[i].m_cutOff)));
		glUniform1f(m_spotLightsLocation[i].m_constant, pLights[i].m_constant);
		glUniform1f(m_spotLightsLocation[i].m_linear, pLights[i].m_linear);
		glUniform1f(m_spotLightsLocation[i].m_exp, pLights[i].m_exp);*/
	}

}


bool ObejctShader::Init()
{
	if (!Shader::Init())
		return false;

	Shader::AddShader("vertexShader/objShader.vert", GL_VERTEX_SHADER);

	Shader::AddShader("fragmentShader/objShader.frag", GL_FRAGMENT_SHADER);

	Shader::ExecuteShader();

	//Get locations of shader variable
	m_worldViewProjecLocation = GetUniformLocation("gWorldViewProjec");
	m_worldMatrixLocation = GetUniformLocation("gWorld");

	m_gSamplerLocation = GetUniformLocation("gSampler");

	//for testing animation
	m_scaleTexLocation = GetUniformLocation("scaleTex");
	m_transTexLocation = GetUniformLocation("transTex");

	//m_directionLightLocation.
	m_flipTexScaleLocation = GetUniformLocation("flipScale");
	m_flipTexTransLocation = GetUniformLocation("flipTrans");

	m_directionLightLocation.m_color = GetUniformLocation("gDirectionalLight.Base.Color");
	m_directionLightLocation.m_ambientIntensity = GetUniformLocation("gDirectionalLight.Base.AmbientIntensity");

	m_directionLightLocation.m_direction = GetUniformLocation("gDirectionalLight.Direction");
	m_directionLightLocation.m_diffuseIntensity = GetUniformLocation("gDirectionalLight.Base.DiffuseIntensity");

	m_numPointLightsLocation = GetUniformLocation("gNumPointLights");
	m_numSpotLightsLocation = GetUniformLocation("gNumSpotLights");


	for (unsigned int i = 0; i < ARRAY_SIZE_IN_ELEMENTS(m_pointLightsLocation); ++i)
	{
		std::string stdName = "gPointLights[" + JE::IntToString(i) + "].Base.Color";
		LPCSTR name = stdName.c_str();
		m_pointLightsLocation[i].m_color = GetUniformLocation(name);

		stdName = "gPointLights[" + JE::IntToString(i) + "].Base.AmbientIntensity";
		name = stdName.c_str();
		m_pointLightsLocation[i].m_ambientIntensity = GetUniformLocation(name);

		stdName = "gPointLights[" + JE::IntToString(i) + "].Base.DiffuseIntensity";
		name = stdName.c_str();
		m_pointLightsLocation[i].m_diffuseIntensity = GetUniformLocation(name);

		stdName = "gPointLights[" + JE::IntToString(i) + "].Position";
		name = stdName.c_str();
		m_pointLightsLocation[i].m_position = GetUniformLocation(name);

		stdName = "gPointLights[" + JE::IntToString(i) + "].Constant";
		name = stdName.c_str();
		m_pointLightsLocation[i].m_constant = GetUniformLocation(name);

		stdName = "gPointLights[" + JE::IntToString(i) + "].Linear";
		name = stdName.c_str();
		m_pointLightsLocation[i].m_linear = GetUniformLocation(name);

		stdName = "gPointLights[" + JE::IntToString(i) + "].Exp";
		name = stdName.c_str();
		m_pointLightsLocation[i].m_exp = GetUniformLocation(name);
	}

	for (unsigned int i = 0; i < ARRAY_SIZE_IN_ELEMENTS(m_spotLightsLocation); ++i)
	{
		std::string stdName = "gSpotLights[" + JE::IntToString(i) + "].Base.Base.Color";
		LPCSTR name = stdName.c_str();
		m_spotLightsLocation[i].m_color = GetUniformLocation(name);

		stdName = "gSpotLights[" + JE::IntToString(i) + "].Base.Base.AmbientIntensity";
		name = stdName.c_str();
		m_spotLightsLocation[i].m_ambientIntensity = GetUniformLocation(name);

		stdName = "gSpotLights[" + JE::IntToString(i) + "].Base.Base.DiffuseIntensity";
		name = stdName.c_str();
		m_spotLightsLocation[i].m_diffuseIntensity = GetUniformLocation(name);

		stdName = "gSpotLights[" + JE::IntToString(i) + "].Base.Position";
		name = stdName.c_str();
		m_spotLightsLocation[i].m_position = GetUniformLocation(name);

		stdName = "gSpotLights[" + JE::IntToString(i) + "].Base.Constant";
		name = stdName.c_str();
		m_spotLightsLocation[i].m_constant = GetUniformLocation(name);

		stdName = "gSpotLights[" + JE::IntToString(i) + "].Base.Linear";
		name = stdName.c_str();
		m_spotLightsLocation[i].m_linear = GetUniformLocation(name);

		stdName = "gSpotLights[" + JE::IntToString(i) + "].Base.Exp";
		name = stdName.c_str();
		m_spotLightsLocation[i].m_exp = GetUniformLocation(name);

		stdName = "gSpotLights[" + JE::IntToString(i) + "].Direction";
		name = stdName.c_str();
		m_spotLightsLocation[i].m_direction = GetUniformLocation(name);

		stdName = "gSpotLights[" + JE::IntToString(i) + "].CutOff";
		name = stdName.c_str();
		m_spotLightsLocation[i].m_cutOff = GetUniformLocation(name);
	}

	return true;
}