/*******************************************************************************
Copyright (C) 2015 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
filename    JEShader.cpp
author      Seho.Joo
email       jsh2958@gmail.com
course      GAM250
assignment  Final milestone
due date    6/12/2015

Brief Description:
This file controls the entire shader 

*******************************************************************************/


#include "JEShader.h"
#include "JEGraphicsNew.h"

/*******************************************************************************
Function: ReadFile

Description: This function reads file.

Inputs: pFileName - File name
outFile   - String varialbe for saving line that is distinguished
by new line

Outputs: return whether it's success or not about reading file.
*******************************************************************************/
bool Shader::ReadFile(const char* pFileName, std::string& outFile)
{
	//Read file
	std::ifstream file(pFileName);

	bool success = false;

	//If read file is success
	if (file.is_open())
	{
		std::string line;

		while (getline(file, line))
		{
			//save the line that is distinguished by new line
			outFile.append(line);
			outFile.append("\n");
		}

		file.close();
		success = true;
	}

	return success;
}

Shader::Shader()
{
	m_shaderProgram = 0;
}

Shader::~Shader()
{
	for (ShaderObjList::iterator it = m_shaderObjList.begin(); it != m_shaderObjList.end(); ++it)
	{
		glDeleteShader(*it);
	}

	if (m_shaderProgram != 0)
	{
		glDeleteShader(m_shaderProgram);
		m_shaderProgram = 0;
	}
}

bool Shader::Init()
{
	//Create shader program
	m_shaderProgram = glCreateProgram();

	if (m_shaderProgram == 0)
	{
		fprintf(stderr, "Error creating shader program\n");
		return false;
	}
	return true;
}


void Shader::AddShader(const char *pFilename, GLenum shaderType)
{
	std::string shader;

	if (!ReadFile(pFilename, shader))
	{
		fprintf(stderr, "Error reading shader program\n");
		exit(1);
	}

	//Create shader object
	GLuint shaderObj = glCreateShader(shaderType);

	if (shaderObj == 0)
	{
		fprintf(stderr, "Error creating shader %d\n", shaderType);
		exit(0);
	}

	m_shaderObjList.push_back(shaderObj);

	//Made a variabe to point shader name.
	const GLchar *pSaveString[1];
	pSaveString[0] = shader.c_str();

	//Calculate a length of shader name.
	GLint lengthOfText[1];
	lengthOfText[0] = shader.size();

	glShaderSource(shaderObj, 1, pSaveString, lengthOfText);
	glCompileShader(shaderObj);
	//For checking to compile shader
	GLint success;
	glGetShaderiv(shaderObj, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		GLchar InfoLog[1024];
		glGetShaderInfoLog(shaderObj, 1024, NULL, InfoLog);
		fprintf(stderr, "Error compiling shader %d: '%s'\n", shaderType, InfoLog);
		exit(1);
	}

	//Attach the shader in shader program
	glAttachShader(m_shaderProgram, shaderObj);
}

void Shader::ExecuteShader(void)
{
	//For checking to link 
	GLint success = false;
	GLchar errorLog[1024] = { 0 };

	//Link shader
	glLinkProgram(m_shaderProgram);
	glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(m_shaderProgram, sizeof(errorLog), NULL, errorLog);
		fprintf(stderr, "Error linking shader: '%s'\n", errorLog);
		exit(1);
	}

	glValidateProgram(m_shaderProgram);
	glGetProgramiv(m_shaderProgram, GL_VALIDATE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(m_shaderProgram, sizeof(errorLog), NULL, errorLog);
		fprintf(stderr, "Invalid shader program: '%s'\n", errorLog);
		exit(1);
	}

	for (ShaderObjList::iterator it = m_shaderObjList.begin(); it != m_shaderObjList.end(); ++it)
	{
		glDeleteShader(*it);
	}

	m_shaderObjList.clear();
}

void Shader::UseProgram(void)
{
	//Installs a program object as part of current rendering state
	glUseProgram(m_shaderProgram);
}

GLint Shader::GetUniformLocation(const char* pUniformName)
{
	GLuint Location = glGetUniformLocation(m_shaderProgram, pUniformName);

	return Location;
}

