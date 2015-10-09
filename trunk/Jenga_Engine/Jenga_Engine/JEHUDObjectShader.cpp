#include "JEHUDObjectShader.h"
#include "JEGraphicsNew.h"


void HUDShader::SetWolrdViewProjec(const JE::Matrix4f& worldViewProjec)
{
	glUniformMatrix4fv(m_worldViewProjecLocation, 1, GL_TRUE, (const GLfloat*)worldViewProjec.m);
}

void HUDShader::SetTextureUnit(unsigned int textureUnit)
{
	glUniform1i(m_gSamplerLocation, textureUnit);
}

void HUDShader::SetAniTex(JE::Vector2f scaleTex_X, JE::Vector2f transTex_X)
{
	glUniform2fv(m_scaleTexLocation, 1, (const GLfloat*)&scaleTex_X);
	glUniform2fv(m_transTexLocation, 1, (const GLfloat*)&transTex_X);
}

void HUDShader::SetFlipTex(JE::Vector2f flipScale, JE::Vector2f flipTrans)
{
	glUniform2fv(m_flipTexScaleLocation, 1, (const GLfloat*)&flipScale);
	glUniform2fv(m_flipTexTransLocation, 1, (const GLfloat*)&flipTrans);
}


bool HUDShader::Init()
{
	if (!Shader::Init())
		return false;

	Shader::AddShader("vertexShader/HUDShader.vert", GL_VERTEX_SHADER);

	Shader::AddShader("fragmentShader/HUDShader.frag", GL_FRAGMENT_SHADER);

	Shader::ExecuteShader();

	//Get locations of shader variable
	m_worldViewProjecLocation = GetUniformLocation("gWorldViewProjec");

	m_gSamplerLocation = GetUniformLocation("gSampler");

	//for testing animation
	m_scaleTexLocation = GetUniformLocation("scaleTex");
	m_transTexLocation = GetUniformLocation("transTex");

	//m_directionLightLocation.
	m_flipTexScaleLocation = GetUniformLocation("flipScale");
	m_flipTexTransLocation = GetUniformLocation("flipTrans");

	return true;
}