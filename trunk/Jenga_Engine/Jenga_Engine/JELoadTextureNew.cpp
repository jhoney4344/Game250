/*******************************************************************************
Copyright (C) 2015 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
filename    JELoadTextureNew.cpp
author      Seho.Joo
email       jsh2958@gmail.com
course      GAM250
assignment  Final milestone
due date    6/12/2015

Brief Description:
This file includes entire load texture program.

*******************************************************************************/

#include "JELoadTextureNew.h"

namespace JE
{
/*******************************************************************************
   Function: GraphicLoadFail

Description: If Loading file is failed, this function delete texture data.
     Inputs: pTexture - texture object 
			 ppFile   - file name
    
	Outputs: None
*******************************************************************************/
static void GraphicLoadFail(JETexture* pTexture, FILE** ppFile)
{
	if(pTexture)
	{
		delete pTexture -> imageData;
		pTexture = 0;
	}

	fclose(*ppFile);
	*ppFile = 0;
}

/*******************************************************************************
   Function: GraphicUpdateTexture

Description: Updates the texture data from the file.
     
	 Inputs: pTexture - texture object 
			 pHeader  - tga header
    
	Outputs: return whether it's valid file or not
*******************************************************************************/
static bool GraphicUpdateTexture(JETexture* pTexture, const JETGAHeader* pHeader)
{
	//24 bits per pixel, 32 bits per pixel
	const GLuint RGB_BITS  = 24;
	const GLuint RGBA_BITS = 32;

	
	pTexture -> width = pHeader -> headerType2[1] * 256 +
		pHeader -> headerType2[0];

	pTexture -> height = pHeader -> headerType2[3] * 256 +
		pHeader -> headerType2[2];

	pTexture -> bitsPerPixel = pHeader -> headerType2[4];

	pTexture -> bytesPerPixel = pTexture -> bitsPerPixel / 8;

	pTexture -> imageSize = pTexture -> bytesPerPixel * pTexture ->width *
		pTexture->height;

	//Check out bit
	if(pTexture->bitsPerPixel == RGB_BITS)
		pTexture -> format = GL_RGB;
	else
		pTexture -> format = GL_RGBA;

	//Must have a valid width and height and must be 24 or 32 bits
	if( (pTexture->width <= 0) || (pTexture->height <= 0) ||
		(pTexture->bitsPerPixel != RGB_BITS &&
		 pTexture->bitsPerPixel != RGBA_BITS))
	{
		return false;
	}

	pTexture -> imageData = new GLubyte[pTexture -> imageSize];

	if(!pTexture->imageData)
	{
		return false;
	}

	return true;
}

/*******************************************************************************
   Function: JEGraphicsLoadUncompressedTGA

Description: This function loads an uncompressed TGA file. 

     Inputs: pTexture - texture object
			 pFile    - file name 

    Outputs: return whether it's success or not
*******************************************************************************/

static bool GraphicLoadUncompressedTGA(JETexture* pTexture, FILE* pFile)
{
	if(fread(pTexture->imageData, pTexture->imageSize, 1, pFile) == 0)
	{
		GraphicLoadFail(pTexture, &pFile);
		return false;
	}

	for(GLuint cswap = 0; cswap < (int)pTexture -> imageSize; cswap += pTexture -> bytesPerPixel)
	{
		pTexture->imageData[cswap] ^= pTexture->imageData[cswap+2] ^=
        pTexture->imageData[cswap] ^= pTexture->imageData[cswap+2];
	}

	return true;
}

/*******************************************************************************
   Function: GraphicLoadCompressedTGA

Description: This function loads an compressed TGA file. 

     Inputs: pTexture - texture object
			 pFile    - file name 

    Outputs: return whether it's success or not
*******************************************************************************/

static bool GraphicLoadCompressedTGA(JETexture* pTexture, FILE* pFile)
{
	//The number of pixels in the image
	GLuint pixelCount = pTexture->height * pTexture->width;
	//The current pixel being read
	GLuint currentPixel = 0;
	GLuint currentByte = 0;
	GLubyte * colorBuffer = new GLubyte[pTexture->bytesPerPixel];

	do
	{
		GLubyte chunkHeader = 0;

		if(fread(&chunkHeader, sizeof(chunkHeader), 1, pFile) == 0)
		{
			GraphicLoadFail(pTexture, &pFile);
			return false;
		}

		if(chunkHeader < 128)
		{
			//Add 1 to get the number of color packets
			++chunkHeader;

			//Make sure we won't overwrite our buffer
			if((currentPixel + chunkHeader) > pixelCount)
			{
				GraphicLoadFail(pTexture, &pFile);
				return false;
			}

			//Now read in the color packets
			for(GLubyte counter = 0; counter < chunkHeader; ++counter)
			{
				//Read one pixel
				if (fread(colorBuffer, 1, pTexture->bytesPerPixel, pFile) != 
					pTexture->bytesPerPixel)
				{
					GraphicLoadFail(pTexture, &pFile);
					return false;
				}

				//Copy 3 or 4 bytes depending on the bytes per pixel
				memcpy(&pTexture->imageData[currentByte], colorBuffer, 
					pTexture->bytesPerPixel);

				//Swap r and b
				pTexture->imageData[currentByte]     = colorBuffer[2];
				pTexture->imageData[currentByte + 2] = colorBuffer[0];

				//Increase current byte
				currentByte += pTexture->bytesPerPixel;
				++currentPixel;
			}
		}
		else
		{
			chunkHeader -= 127;
			//Attempt to read the color
			if(fread(colorBuffer, 1, pTexture->bytesPerPixel, pFile) != 
				pTexture->bytesPerPixel)
			{
				GraphicLoadFail(pTexture, &pFile);
				return false;
			}

			//Make sure we won't overwrite our buffer
			if((currentPixel + chunkHeader) > pixelCount)
			{
				GraphicLoadFail(pTexture, &pFile);
				return false;
			}

			//Copy the color, chunkHeader number of times
			for(GLubyte counter = 0; counter < chunkHeader; ++counter)
			{
				//Copy 3 or 4 bytes depending on the bytes per pixel
				memcpy(&pTexture->imageData[currentByte], colorBuffer, 
					pTexture->bytesPerPixel);		

				//Swap r and b
				pTexture->imageData[currentByte]     = colorBuffer[2];
				pTexture->imageData[currentByte + 2] = colorBuffer[0];

				//Increase current byte
				currentByte += pTexture->bytesPerPixel;
				++currentPixel;
			}
		}
	}while(currentPixel < pixelCount);

	return true;
}

/*******************************************************************************
   Function: GraphicLoadTGA

Description: This function loads an TGA file. 

     Inputs: pTexture  - texture object
			 pFileName - file name 

    Outputs: return whether it's success or not
*******************************************************************************/

static bool GraphicLoadTGA(JETexture* pTexture, const char* pFileName)
{
	//Uncompressed TGA Header
	const GLubyte UNCOMPRESSED_TGA[12] = {0,0,2, 0,0,0,0,0,0,0,0,0};	
	//Compressed TGA Header
	const GLubyte COMPRESSED_TGA[12]   = {0,0,10,0,0,0,0,0,0,0,0,0};

	JETGAHeader tgaHeader;

	FILE* pFile;

	fopen_s(&pFile, pFileName, "rb");
	if(!pFile)
	{
		GraphicLoadFail(0, &pFile);
		return false;
	}
	
	fread(&tgaHeader, sizeof(tgaHeader), 1, pFile);

	//Fill in the texture data
	if(!GraphicUpdateTexture(pTexture, &tgaHeader))
	{
		GraphicLoadFail(0, &pFile);
		return false;
	}

	//Make sure the file is a power of two
	if(!((pTexture->width > 0) && !(pTexture->width & (pTexture->width - 1)))
		 || !((pTexture->height > 0) && !(pTexture->height & (pTexture->height - 1))))
	{
		GraphicLoadFail(pTexture, &pFile);
		return false;
	}

	//Compare with UMCOMPRESSED Header format
	if(!memcmp(UNCOMPRESSED_TGA, tgaHeader.headerType1, sizeof(UNCOMPRESSED_TGA)))
	{
		//If the 3rd byte is value 2, it is uncompressed, load that format
		GraphicLoadUncompressedTGA(pTexture, pFile);
	}
	else if(!memcmp(COMPRESSED_TGA, tgaHeader.headerType1, sizeof(COMPRESSED_TGA)))
	{
		//If the 3rd byte is 10, it is compressed, load that format
		GraphicLoadCompressedTGA(pTexture, pFile);
	}
	else //The texture didn't match our types
	{
		GraphicLoadFail(pTexture, &pFile);
		return false;
	}

	fclose(pFile);
	pFile = 0;
	return true;
}

/*******************************************************************************
   Function: GraphicLoadTexture

Description: This function generate texture in GPU.

     Inputs: pfileName - file name 

    Outputs: return buffer object.
*******************************************************************************/
int GraphicLoadTexture(const char* pfileName)
{
	JETexture texture;
	int textureID = FAIL;

	if(!GraphicLoadTGA(&texture, pfileName))
    return textureID;

	//Return currently unused names for texture objects in the array texture
	glGenTextures(NUM_TEXTURE, (GLuint*)&textureID);
	//Initialize my texture
	glBindTexture(GL_TEXTURE_2D, textureID);

	//I don't know this two things
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//Provide initial image data for a 2D texture
	glTexImage2D(GL_TEXTURE_2D, 0,
				 texture.format,
				 texture.width,
				 texture.height,
				 0,
				 texture.format,
				 GL_UNSIGNED_BYTE,
				 texture.imageData);

	glBindTexture(GL_TEXTURE_2D, 0);

	delete texture.imageData;


	return textureID;
}

/*******************************************************************************
   Function: GraphicUnloadTexture

Description: This function delete the texture.

     Inputs: textureID - buffer object

    Outputs: None
*******************************************************************************/
void GraphicUnloadTexture(int textureID)
{
	glDeleteTextures(NUM_TEXTURE, (GLuint*)&textureID);
}

}