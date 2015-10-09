/*******************************************************************************
Copyright (C) 2015 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
filename    JELighting.cpp
author      Seho.Joo
email       jsh2958@gmail.com
course      GAM250
assignment  Final milestone
due date    6/12/2015

Brief Description:
This file handle light function.

*******************************************************************************/

#include "JELighting.h"

void DirectionalLight::ChangeLightAmbientIntensity(float ambientIntensity)
{
	m_ambientIntensity = ambientIntensity;
}

void DirectionalLight::ChangeLightColor(JE::Vector3f color)
{
	m_color = color;
}

void DirectionalLight::ChangeLightDirection(JE::Vector3f direction)
{
	m_direction = direction;
}
void DirectionalLight::ChangeLightDiffuseIntensity(float diffuseIntensity)
{
	m_diffuseIntensity = diffuseIntensity;
}

void PointLight::AddPointLight(int pointID,JE::Vector3f position, JE::Vector3f color, float ambientIntensity, float weakness)
{
	PointLight pointLight;
	
	pointLight.m_pointID = pointID;
	//Position
	pointLight.m_position = position;
	//Color
	pointLight.m_color = color;
	//AmbientIntensity
	pointLight.m_ambientIntensity = ambientIntensity;
	//weakness
	pointLight.m_linear = weakness;

	m_pointLight.push_back(pointLight);
}

SpotLight::pointIter PointLight::FindPointLight(int pointID)
{
	for (m_it_pointLight = m_pointLight.begin(); m_it_pointLight != m_pointLight.end(); ++m_it_pointLight)
	{
		if (m_it_pointLight->m_pointID == pointID)
		{
			return m_it_pointLight;
		}
	}
	return m_pointLight.end();
}

SpotLight::pointIter PointLight::GetBeginItor(void)
{
	return m_pointLight.begin();
}

SpotLight::pointIter PointLight::GetendItor(void)
{
	return m_pointLight.end();
}

void PointLight::DeletePointLight(int pointID)
{
	pointIter it_pointLight = FindPointLight(pointID);

	if (it_pointLight != m_pointLight.end())
		m_pointLight.erase(it_pointLight);
}

void PointLight::FreePointLight(void)
{
	if (m_pointLight.begin() != m_pointLight.end())
	{
		m_pointLight.clear();
	}
}

void SpotLight::AddSpotLight(int spotID,JE::Vector3f position, JE::Vector3f color, float angle , JE::Vector3f direction)
{
	SpotLight spotLight;

	//Position
	spotLight.m_position = position;

	spotLight.m_cutOff = angle;

	spotLight.m_direction = direction;

	spotLight.m_diffuseIntensity = 0.1f;
	//Color
	spotLight.m_color = color;
	//AmbientIntensity
	spotLight.m_ambientIntensity = 1.f;
	//weakness
	spotLight.m_linear = 0.0f;

	spotLight.m_spotID = spotID;

	m_spotLight.push_back(spotLight);
}

SpotLight::spotIter SpotLight::FindSpotLight(int spotID)
{
	for (m_it_spotLight = m_spotLight.begin(); m_it_spotLight != m_spotLight.end(); ++m_it_spotLight)
	{
		if (m_it_spotLight->m_spotID == spotID)
		{
			return m_it_spotLight;
		}
	}
	return m_spotLight.end();
}

SpotLight::spotIter SpotLight::GetBeginItor(void)
{
	return m_spotLight.begin();
}

SpotLight::spotIter SpotLight::GetendItor(void)
{
	return m_spotLight.end();
}

void SpotLight::DeleteSpotLight(int spotID)
{
	spotIter it_spotLight = FindSpotLight(spotID);

	if (it_spotLight != m_spotLight.end())
		m_spotLight.erase(it_spotLight);
}

void SpotLight::FreeSpotLight(void)
{
	if (m_spotLight.begin() != m_spotLight.end())
	{
		m_spotLight.clear();
	}
}
