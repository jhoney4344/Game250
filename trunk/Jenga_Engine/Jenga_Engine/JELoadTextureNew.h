/*******************************************************************************
Copyright (C) 2015 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
filename    JELoadTextureNew.h
author      Seho.Joo
email       jsh2958@gmail.com
course      GAM250
assignment  Final milestone
due date    6/12/2015

Brief Description:
This header includes entire load texture program.

*******************************************************************************/

#ifndef JELOAD_TEXTURE_NEW_H
#define JELOAD_TEXTURE_NEW_H

#include <iostream>
#include "glew.h"
#include <gl\GL.h>
#include <gl\GLU.h>

#define NUM_TEXTURE  1
#define FAIL        -1

namespace JE
{
	//Texture Data
	class JETexture
	{
		public:
			GLubyte* imageData;
			GLuint   bitsPerPixel;
			GLuint   bytesPerPixel;
			GLuint   width;
			GLuint   height;
			GLuint   textureID;
			GLuint   format;
			GLuint   imageSize;
	};

	class JETGAHeader 
	{
		public:
			GLubyte headerType1[12];
			GLubyte headerType2[6];
	};

	static void GraphicLoadFail(JETexture* pTexture, FILE** ppFile);
	int GraphicLoadTexture(const char* pfileName);
	static bool GraphicUpdateTexture(JETexture* pTexture, const JETGAHeader* pHeader);
	static bool GraphicLoadUncompressedTGA(JETexture* pTexture, FILE* pFile);
	static bool GraphicLoadCompressedTGA(JETexture* pTexture, FILE* pFile);
	static bool GraphicLoadTGA(JETexture* pTexture, const char* pFileName);
	void GraphicUnloadTexture(int textureID);

}
#endif