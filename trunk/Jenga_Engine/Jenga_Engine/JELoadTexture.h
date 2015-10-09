#ifndef JELOADTEXTURE_H
#define JELOADTEXTURE_H

#include <iostream>
#include "glew.h"
#include <gl\GL.h>
#include <gl\GLU.h>

#define NUM_TEXTURE  1
#define FAIL        -1

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

static void JEGraphicsLoadFail(JETexture* pTexture, FILE** ppFile);
int JEGraphicLoadTexture(const char* pfileName);
static bool JEGraphicsUpdateTexture(JETexture* pTexture, const JETGAHeader* pHeader);
static bool JEGraphicsLoadUncompressedTGA(JETexture* pTexture, FILE* pFile);
static bool JEGraphicsLoadCompressedTGA(JETexture* pTexture, FILE* pFile);
static bool WEGraphicsLoadTGA(JETexture* pTexture, const char* pFileName);
void JEGraphicsUnloadTexture(int textureID);
int  JEGraphicsGetTextureCount(void);


static int s_textureCount = 0;

#endif