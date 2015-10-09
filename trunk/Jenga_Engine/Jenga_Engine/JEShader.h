/*******************************************************************************
Copyright (C) 2015 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
filename    JEShader.h
author      Seho.Joo
email       jsh2958@gmail.com
course      GAM250
assignment  Final milestone
due date    6/12/2015

Brief Description:
This header file includes JEShader.cpp that controls the entire shader

*******************************************************************************/

#ifndef  JESHADER_H
#define  JESHADER_H

#include "glew.h"

#include <sstream>
#include <fstream>

#include <list>

#include "JEOGLMathHelper.h"
#include "JELighting.h"

#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))
//Degree to radian
#define DEG2RAD(a) (((a) * PI) / 180) 
namespace
{
	//Name of shader
	const unsigned int MAX_POINT_LIGHTS = 60;
	const unsigned int MAX_SPOT_LIGHTS = 5;
}

class Shader
{
public:
	Shader();

	virtual ~Shader();

	virtual bool Init();

	bool ReadFile(const char* pFileName, std::string& outFile);

	void AddShader(const char *pFilename, GLenum shaderType);
	void ExecuteShader(void);

	GLint GetUniformLocation(const char* pUniformName);

	void UseProgram(void);

	GLuint m_shaderProgram;
private:
	typedef std::list<GLuint> ShaderObjList;
	ShaderObjList m_shaderObjList;
};

#endif 