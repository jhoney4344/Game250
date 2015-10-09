#include "JELoadTexture.h"


/*******************************************************************************
   Function: JEGraphicsLoadFail

Description: If Loading tga file is failed by something, this function delete 
             texture date.
     Inputs: pTexture - texture class 
			 ppFile   - tga file name
    Outputs: None
*******************************************************************************/
static void JEGraphicsLoadFail(JETexture* pTexture, FILE** ppFile)
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
   Function: JEGraphicsUpdateTexture

Description: Fills in the texture pointer with data from the file.
     
	 Inputs: pTexture - texture class 
			 pHeader  - tga header
    
	Outputs: true if we have a valid file, false otherwise.
*******************************************************************************/
static bool JEGraphicsUpdateTexture(JETexture* pTexture, const JETGAHeader* pHeader)
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

     Inputs: pTexture - texture class
			 pFile    - tga file name 

    Outputs: True if the load was successful, flase otherwise
*******************************************************************************/

static bool JEGraphicsLoadUncompressedTGA(JETexture* pTexture, FILE* pFile)
{
	if(fread(pTexture->imageData, pTexture->imageSize, 1, pFile) == 0)
	{
		JEGraphicsLoadFail(pTexture, &pFile);
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
   Function: JEGraphicsLoadCompressedTGA

Description: This function loads an compressed TGA file. 

     Inputs: pTexture - texture class
			 pFile    - tga file name 

    Outputs: True if the load was successful, flase otherwise
*******************************************************************************/

static bool JEGraphicsLoadCompressedTGA(JETexture* pTexture, FILE* pFile)
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
			JEGraphicsLoadFail(pTexture, &pFile);
			return false;
		}

		if(chunkHeader < 128)
		{
			//Add 1 to get the number of color packets
			++chunkHeader;

			//make sure we won't overwrite our buffer
			if((currentPixel + chunkHeader) > pixelCount)
			{
				JEGraphicsLoadFail(pTexture, &pFile);
				return false;
			}

			//now read in the color packets
			for(GLubyte counter = 0; counter < chunkHeader; ++counter)
			{
				//read one pixel
				if (fread(colorBuffer, 1, pTexture->bytesPerPixel, pFile) != 
					pTexture->bytesPerPixel)
				{
					JEGraphicsLoadFail(pTexture, &pFile);
					return false;
				}

				//copy 3 or 4 bytes depending on the bytes per pixel
				memcpy(&pTexture->imageData[currentByte], colorBuffer, 
					pTexture->bytesPerPixel);

				//Swap r and b
				pTexture->imageData[currentByte]     = colorBuffer[2];
				pTexture->imageData[currentByte + 2] = colorBuffer[0];

				//increase current byte
				currentByte += pTexture->bytesPerPixel;
				++currentPixel;
			}
		}
		else
		{
			/*if chunkHeader > 128 it means RLE data.  
			The next color is repeated (chunkHeader - 127) times*/
			chunkHeader -= 127;
			//Attempt to read the color
			if(fread(colorBuffer, 1, pTexture->bytesPerPixel, pFile) != 
				pTexture->bytesPerPixel)
			{
				JEGraphicsLoadFail(pTexture, &pFile);
				return false;
			}

			//make sure we won't overwrite our buffer
			if((currentPixel + chunkHeader) > pixelCount)
			{
				JEGraphicsLoadFail(pTexture, &pFile);
				return false;
			}

			//copy the color, chunkHeader number of times
			for(GLubyte counter = 0; counter < chunkHeader; ++counter)
			{
				//copy 3 or 4 bytes depending on the bytes per pixel
				memcpy(&pTexture->imageData[currentByte], colorBuffer, 
					pTexture->bytesPerPixel);

				//Swap r and b
				pTexture->imageData[currentByte]     = colorBuffer[2];
				pTexture->imageData[currentByte + 2] = colorBuffer[0];

				//increase current byte
				currentByte += pTexture->bytesPerPixel;
				++currentPixel;
			}
		}
	}while(currentPixel < pixelCount);

	return true;
}

static bool JEGraphicsLoadTGA(JETexture* pTexture, const char* pFileName)
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
		//we need WEDEBUG!
		//Make sure the file is opened
		return false;
	}
	
	fread(&tgaHeader, sizeof(tgaHeader), 1, pFile);

	//Fill in the texture data
	if(!JEGraphicsUpdateTexture(pTexture, &tgaHeader))
	{
		JEGraphicsLoadFail(0, &pFile);
		return false;
	}

	//Make sure the file is a power of two
	if(!((pTexture->width > 0) && !(pTexture->width & (pTexture->width - 1)))
		 || !((pTexture->height > 0) && !(pTexture->height & (pTexture->height - 1))))
	{
		JEGraphicsLoadFail(pTexture, &pFile);
		return false;
	}

	//Compare with UMCOMPRESSED Header format
	if(!memcmp(UNCOMPRESSED_TGA, tgaHeader.headerType1, sizeof(UNCOMPRESSED_TGA)))
	{
		//If the 3rd byte is value 2, it is uncompressed, load that format
		JEGraphicsLoadUncompressedTGA(pTexture, pFile);
	}
	else if(!memcmp(COMPRESSED_TGA, tgaHeader.headerType1, sizeof(COMPRESSED_TGA)))
	{
		//If the 3rd byte is 10, it is compressed, load that format
		JEGraphicsLoadCompressedTGA(pTexture, pFile);
	}
	else //The texture didn't match our types
	{
		/*Free texture data*/
		JEGraphicsLoadFail(pTexture, &pFile);
		return false;
	}

	fclose(pFile);
	pFile = 0;
	return true;
}

/*******************************************************************************
   Function: JEGraphicsLoadTexture

Description: This function generate texture in graphic engine.

     Inputs: pfileName - tga file name 

    Outputs: return texture id.
*******************************************************************************/
int JEGraphicLoadTexture(const char* pfileName)
{
	JETexture texture;
	int textureID = FAIL;

	if(!JEGraphicsLoadTGA(&texture, pfileName))
    return textureID;

	//Return currently unused names for texture objects in the array texture
	glGenTextures(NUM_TEXTURE, (GLuint*)&textureID);
	//initialize my texture
	glBindTexture(GL_TEXTURE_2D, textureID);

	//I don't know this two things
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//provide initial image data for a 2D texture
	glTexImage2D(GL_TEXTURE_2D, 0,
				 texture.format,
				 texture.width,
				 texture.height,
				 0,
				 texture.format,
				 GL_UNSIGNED_BYTE,
				 texture.imageData);

	delete texture.imageData;

	++s_textureCount;

	return textureID;
}

/*******************************************************************************
   Function: JEGraphicsUnloadTexture

Description: This function unload the texture.

     Inputs: textureID - texture id to distinguish

    Outputs: None
*******************************************************************************/
void JEGraphicsUnloadTexture(int textureID)
{
	glDeleteTextures(NUM_TEXTURE, (GLuint*)&textureID);
	--s_textureCount;
}

/*******************************************************************************
   Function: JEGraphicsGetTextureCount

Description: This function count the number of texture.

     Inputs: None

    Outputs: the number of all texture in the program.
*******************************************************************************/
int JEGraphicsGetTextureCount(void)
{
	return s_textureCount;
}



