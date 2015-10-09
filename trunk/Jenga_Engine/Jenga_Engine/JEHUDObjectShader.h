#ifndef  JEHUDOBJECTSHADER_H
#define  JEHUDOBJECTSHADER_H

#include "JEShader.h"

class HUDShader : public Shader
{
public:
	virtual bool Init();

	void SetWolrdViewProjec(const JE::Matrix4f& worldViewProjec);
	void SetTextureUnit(unsigned int textureUnit);
	void SetAniTex(JE::Vector2f scaleTex, JE::Vector2f transTex);
	void SetFlipTex(JE::Vector2f flipScale, JE::Vector2f flipTrans);

private:
	GLuint m_worldViewProjecLocation;
	GLuint m_gSamplerLocation;

	//For animation
	GLuint m_scaleTexLocation;
	GLuint m_transTexLocation;

	//For flip
	GLuint m_flipTexScaleLocation;
	GLuint m_flipTexTransLocation;
};

#endif