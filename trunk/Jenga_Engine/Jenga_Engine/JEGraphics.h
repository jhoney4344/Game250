
#ifndef JEGRAPHICS_H
#define JEGRAPHICS_H

#include <iostream>
#include <Windows.h>

#include "glew.h"
#include <gl\GL.h>
#include <gl\GLU.h>


#define ROWS 4
#define COLS 4


class JEVertexList
{
	public:
		unsigned vertexCount;
		GLuint   vboID;
};

class JEVertex
{
	public:
		float x;
		float y;
		float z;

		float textureX;
		float textureY;
};

//vec helper
struct Vec2
{
  float x;
  float y;
};


class JEGraphicsData
{
	public:

		GLdouble cameraZ;
		GLdouble cameraX;
		GLdouble cameraY;
		GLdouble cameraRot;

		GLdouble nearClip;
		GLdouble farClip;
		GLdouble aspectRatio;   
		GLdouble fov;


		HGLRC renderContext;
		HDC   deviceContext;
		HWND  window;

		//client area of the screen
		int width;
		int height;

		//fix it code
		float    worldMinX;     /*The minimum x extent of the world on screen*/
		float    worldMaxX;     /*The Maximum x extent of the world on screen*/
		float    worldMinY;     /*The minimum y extent of the world on screen*/
		float    worldMaxY;     /*The maximum y extent of the world on screen*/

		Vec2   worldTopLeft;  /*The top left point on the screen.*/
		Vec2   worldTopRight; /*The top right point on the screen.*/
		Vec2   worldBotLeft;  /*The bottom Left point on the screen.*/
		Vec2   worldBotRight; /*The bottom right point on the screen.*/

};

class Mtx44
{
	public:
		float m[ROWS][COLS];
};

static void JEGraphicsRenderContextInit(void);
static void JEGraphicsCreateVertexBuffer(void);
void JEGraphicsInit(HWND window, int width, int height);
void GraphicsShutdown(void);
void JEGraphicsSetTexture(int textureID);
void JEGraphicsStartDraw(void);
void JEGraphicsEndDraw(void);
void JEGraphicsSetTextureCoords(float scaleX, float scaleY, float radians, float transX, float transY);
void JEGraphicsDraw(const Mtx44* pWorldMatrix);
void JEGraphicsSetBackGroundColor(float red, float green, float blue);
void JEGraphicsSetAttributeColor(unsigned color);





//helper function
void WEMtx44MakeTransform(Mtx44* pResult, double scaleX, double scaleY, 
                          double radians,
                          double transX, double transY, 
                          double zOrder);

static JEGraphicsData jgpData;
static JEVertexList   vboList;


//copy code fix it
static const GLdouble MIN_CAM_DISTANCE = 2.0f; /*Min camera distance*/
static const GLdouble MAX_CAM_DISTANCE = 999.0f;/*Max camera distance*/



void JEGraphicsSetToPerspective(void);
static void JEGraphicsSetPerspective(GLdouble fov, GLdouble aspectRatio, 
                                     GLdouble nearClip, GLdouble farClip);
void JEGraphicsSetCamera(float cameraX, float cameraY,
                                float cameraZ, float cameraRot);

static void JEGraphicsCalulateWorldExtents(void);

///*Default values for my projection and view matrices*/
static const GLdouble WIN_FOV       = 90.0;   /*Field of view*/
static const GLdouble CAM_DISTANCE  = 60.0;   /*Camera distance in the Z*/
static const GLdouble CAM_NEAR_CLIP = 1.0;    /*Near clip plane in the Z*/
static const GLdouble CAM_FAR_CLIP  = 1000.0; /*Far Clip plane in the Z*/


void WEMtx44MakeRotateZ( Mtx44* pResult, float radians );
float WEMathDegreeToRadian( float degrees );

float WEMathClamp( float x, float low, float high );



#endif


