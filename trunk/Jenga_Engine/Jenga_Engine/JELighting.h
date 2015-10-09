/*******************************************************************************
Copyright (C) 2015 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
filename    JELighting.h
author      Seho.Joo
email       jsh2958@gmail.com
course      GAM250
assignment  Final milestone
due date    6/12/2015

Brief Description:
This header is for JELighting.cpp.

*******************************************************************************/
#ifndef JELIGHT_H
#define JELIGHT_H

#include "JEOGLMathHelper.h"

#include <list>

struct BaseLight
{
	JE::Vector3f m_color;
	float m_ambientIntensity;
	float m_diffuseIntensity;

	BaseLight()
	{
		m_color = JE::Vector3f(0.0f, 0.0f, 0.0f);
		m_ambientIntensity = 0.0f;
		m_diffuseIntensity = 0.0f;
	}
};

struct DirectionalLight : public BaseLight
{
	JE::Vector3f m_direction;

	DirectionalLight()
	{
		m_direction = JE::Vector3f(0.0f, 0.0f, 0.0f);
	}
	
	void ChangeLightDirection(JE::Vector3f direction);
	void ChangeLightAmbientIntensity(float ambientIntensity);
	void ChangeLightColor(JE::Vector3f color);
	void ChangeLightDiffuseIntensity(float diffuseIntensity);
};

struct PointLight : public BaseLight
{
public:
	typedef std::list<PointLight>::iterator pointIter;

	JE::Vector3f m_position;
	float m_constant;
	float m_linear;
	float m_exp;
	int   m_countOfPoint;
	int   m_pointID;

	PointLight()
	{
		m_position = JE::Vector3f(0.0f, 0.0f, 0.0f);
		m_constant = 1.0f;
		m_linear   = 0.0f;
		m_exp      = 0.0f; 
	}

	void AddPointLight(int pointID, JE::Vector3f position, JE::Vector3f color, float ambientIntensity, float weakness);
	pointIter FindPointLight(int pointID);
	pointIter GetBeginItor(void);
	pointIter GetendItor(void);
	void DeletePointLight(int pointID);
	void FreePointLight(void);

private:
	//std::vector<PointLight> m_sourceLight;
	//std::vector<PointLight>::iterator m_it_sourceLight;
	std::list<PointLight> m_pointLight;
	pointIter m_it_pointLight;
};

struct SpotLight : public PointLight
{
public:
	typedef std::list<SpotLight>::iterator spotIter;

	JE::Vector3f m_direction;
	float m_cutOff;
	int   m_countOfSpot;
	int   m_spotID;

	SpotLight()
	{
		m_direction = JE::Vector3f(0.0f, 0.0f, 0.0f);
		m_cutOff = 0.0f;
	}

	void AddSpotLight(int spotID, JE::Vector3f position, JE::Vector3f color, float angle, JE::Vector3f direction);
	spotIter FindSpotLight(int spotID);
	spotIter GetBeginItor(void);
	spotIter GetendItor(void);
	void DeleteSpotLight(int spotID);
	void FreeSpotLight(void);

private:
	//std::vector<SpotLight> m_spotLight;
	//std::vector<SpotLight>::iterator m_it_spotLight;
	std::list<SpotLight> m_spotLight;
	spotIter m_it_spotLight;
};

#endif