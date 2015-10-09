/*******************************************************************************
Copyright (C) 2015 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
filename    JEObjectShader.h
author      Seho.Joo
email       jsh2958@gmail.com
course      GAM250
assignment  Final milestone
due date    6/12/2015

Brief Description:
This header is for JEObjectShader.cpp.

*******************************************************************************/

#ifndef  JEOBJECTSHADER_H
#define  JEOBJECTSHADER_H

#include "JEShader.h"


class ObejctShader : public Shader
{
public:
	virtual bool Init();

	void SetWolrdViewProjec(const JE::Matrix4f& worldViewProjec);
	void SetTextureUnit(unsigned int textureUnit);
	void SetAniTex(JE::Vector2f scaleTex, JE::Vector2f transTex);
	void SetFlipTex(JE::Vector2f flipScale, JE::Vector2f flipTrans);
	void SetDirectionalLight(const DirectionalLight& light);
	void SetWorldMatrix(const JE::Matrix4f& world);
	//For point light
	void SetPointLights(unsigned int numLights, PointLight* pLights);
	//For spot light
	void SetSpotLights(unsigned int numLights, SpotLight* pLights);

private:
	GLuint m_worldViewProjecLocation;
	GLuint m_worldMatrixLocation;
	GLuint m_gSamplerLocation;

	//For animation
	GLuint m_scaleTexLocation;
	GLuint m_transTexLocation;

	//For flip
	GLuint m_flipTexScaleLocation;
	GLuint m_flipTexTransLocation;

	//For pointLight
	GLuint m_numPointLightsLocation;

	//For spotLight
	GLuint m_numSpotLightsLocation;

	//For directional lighting
	struct
	{
		GLuint m_ambientIntensity;
		GLuint m_color;
		GLuint m_direction;
		GLuint m_diffuseIntensity;
	}m_directionLightLocation;

	//For point lighting
	struct
	{
		GLuint m_ambientIntensity;
		GLuint m_color;
		GLuint m_diffuseIntensity;
		GLuint m_position;

		GLuint m_constant;
		GLuint m_linear;
		GLuint m_exp;
	}m_pointLightsLocation[MAX_POINT_LIGHTS];

	//For spot lighting
	struct
	{
		GLuint m_ambientIntensity;
		GLuint m_color;
		GLuint m_diffuseIntensity;
		GLuint m_position;

		GLuint m_constant;
		GLuint m_linear;
		GLuint m_exp;
		GLuint m_direction;
		GLuint m_cutOff;
	}m_spotLightsLocation[MAX_SPOT_LIGHTS];

};

#endif