/*******************************************************************************
Copyright (C) 2015 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
filename    JEGraphicsNew.h
author      Seho.Joo
email       jsh2958@gmail.com
course      GAM250
assignment  Final milestone
due date    6/12/2015

Brief Description:
This header includes entire graphic program such as rendering, animating and
etc

*******************************************************************************/
#ifndef JEGRAPHICS_NEW_H
#define JEGRAPHICS_NEW_H

//For window application
#include <Windows.h>

//For openGL
#include "glew.h"
#include <gl\GL.h>
#include <gl\GLU.h>

//For graphic helper 
#include "JEOGLMathHelper.h"
#include "JEPipeLine.h"

//For camera 
#include "JECamera.h"

//For bone
#include <sstream>
#include <cmath>

#include "JEObjectShader.h"
#include "JELighting.h"

#include "JEParticle.h"


//I think this liblary doesn't work 
//#include <iostream>
//#include <fstream>
//#include <string>
//#include <map>
//#include "SDL/SDL.h"
//#include <stdint.h>

//For animation 
#define STANDARD_TEX_X 1

//Max number of children
#define MAX_CHCOUNT				8      

//Max number of bone
#define MAX_BONECOUNT			20      

//Max number of frame
#define MAX_KFCOUNT             20    

//Max number of animation
#define MAX_ATCOUNT				20

//Max number of vertex
#define MAX_VXCOUNT				20

#define PI						3.14159f

//For matching length rate of graphic engine and length rate of the animation tool
#define LENGTH_DECREASE_RATE	30

//Entire animation speed that uses bone system.
#define ANIMATION_SPEED         4

//Degree to radian
#define DEG2RAD(a) (((a) * PI) / 180) 

#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))

#define CHANGE_DELTATIME_INT 100

#define MAX_PARTICLE 40

namespace JE
{
	enum FLIP_STANDARD_AXIS
	{
		FLIP_IDLE,
		FLIP_XY,
		FLIP_X,
		FLIP_Y
	};

	enum PROJECTION_TYPE
	{
		PERSPECTIVE,
		ORTHOGONAL
	};

	//Structor for controlling vertex data
	struct JEVertex
	{
		Vector3f m_pos;
		Vector2f m_tex;
		Vector3f m_normal;

		JEVertex(){}

		JEVertex(Vector3f pos, Vector2f tex)
		{
			m_pos = pos;
			m_tex = tex;
			m_normal = Vector3f(0.0f, 0.0f, 0.0f);
		}
	};

	//Window data class
	struct JEWindowData
	{
		int windowWidth;
		int windowHeight;

		HWND windowHandle;
		HDC  deviceContext;
		//open GL rendering context
		HGLRC oglContext;
	};

	//Animation frame class for bone system
	class Keyframe
	{
		public:
			unsigned int time;
			float angle, length;
	};

	class CopyFrameTime
	{
		public:
			unsigned int time;
	};

	//Vertex class
	class Vertex
	{
		public:
			//Coordinate
			float x, y;
			//Color
			float r, g, b;
	};

	//
	class Bone
	{
		public:
			char depthStr[20];  //Distance of children from root
			char name[20];		//Variable for finding a bone 
			float x;			//Starting point x
			float y;			//Starting point y
			float a;			//Angle
			float l;			//Length of the bone

			float offA;			//Offsets measures for angle 
			float offL;			//Offsets measures for length

			float initA;		//Initial angle
			float initL;		//Initial langth

			int   part;			//For distinguish position of part



			unsigned int flags;		    //Number of children that has parent
			unsigned int childCount;    //Number of whole children

			Bone *child[MAX_CHCOUNT];	//Pointers to children
			Bone *parent;				//Parent bone

			unsigned int actionTypeCount;				//Number of animation
			unsigned int NumberOfAction;
			unsigned int keyframeCount[MAX_ATCOUNT];	//Number of frame
			Keyframe keyframe[MAX_ATCOUNT][MAX_KFCOUNT];//For saving animation data
			CopyFrameTime copyTime[MAX_ATCOUNT][MAX_KFCOUNT]; //For copying time

			Vertex vertex[MAX_VXCOUNT];					//Vertex array

			int textureID;
			FLIP_STANDARD_AXIS flip;
	};
	
	static void CreateVertexBuffer(const unsigned int* pIndices, unsigned int indexCount);
	static void CreateIndexBuffer(const unsigned int* pIndices, unsigned int sizeInBytes);
	static void GraphicCreatingOpenGLContext(void);
	void GraphicSetBackgroundColor(float red, float green, float blue);
	void GraphicInit(HWND window, int left, int right, int top, int bottom);
	void GraphicStartDraw(void);
	void GraphicEndDraw(void);
	void GraphicSetTexture(int textureID);
	//for random texture for particle
	void GraphicSetRandomTexture(int textureID);

	//Perspective trans
	void GraphicSetPersTransform(double transX, double transY, double zOrder,
							     double scaleX, double scaleY, 
								 double rotateZ);

	void GraphicDrawing(void);
	void GraphicsShutdown(void);


	void GraphicSetTextureUnit(int unit);
	//
	void GraphicSetParticleTransform(double transX, double transY, double zOrder,
		double scaleX, double scaleY,
		double rotateZ,
		float ambientIntensity);

	void GraphicSetOrthoParticleTransform(double transX, double transY, double zOrder,
		double scaleX, double scaleY,
		double rotateZ,
		float ambientIntensity);

	//Othogonal trans
	void GraphicSetOrthogTransform(double transX, double transY, double zOrder,
		double scaleX, double scaleY,
		double rotateZ);

	//For mouse position
	void GraphicConvertWorldToScreen(double& mouseX, double& mouseY);
	void GraphicConvertScreenToWorld(double& mouseX, double& mouseY);

	//HUD transform function
	void GraphicSetHUDTransform(double transX, double transY, double zOrder,
		double scaleX, double scaleY,
		double rotateZ);

	//For camera
	void GraphicInitCamera(void);
	void GraphicCameraPosition(float x, float y);
	void GraphicCameraZoom(bool zoomIn, float magnification);
	Vector3f GraphicGetCameraPos();
	void GraphicSetCameraPos(Vector3f pos);

	//Animation texture
	void GraphicSetAnimaTex(float numberOfAnimation, float rateOfChange);

	//Flip texture
	void GraphicSetFilpTex(FLIP_STANDARD_AXIS axis, float numberOfFrame);

	inline std::string IntToString(int number);
	Bone* BoneAddChild(Bone *root,
		float x, float y,
		float a, float l,
		unsigned int flags,
		char *depthStr,
		char *name,
		int part);

	Bone *BoneAddAniChild(Bone* pRoot, Keyframe** ppKeyframe, int numberOfAction, int numbeOfFrame);
	Bone *BoneAddTextureChild(Bone* pRoot, int textureID, FLIP_STANDARD_AXIS flip);

	Bone* BoneFreeTree(Bone *root);
	Bone* BoneFindByName(Bone *root, char *name);//, char* depth);
	void BoneSetTextureID(Bone *root, char *partName, int textureID, FLIP_STANDARD_AXIS flip = FLIP_IDLE);

	void  BoneListNames(Bone *root, char names[MAX_BONECOUNT][20]);
	Bone* BoneLoadStructure(char *path);

	void  BoneDraw(Bone *root, double transX, double transY, double transZ);
	void  BoneDrawHUD(Bone *root, double transX, double transY, double transZ);

	//For bone state
	Bone* BoneInit(Bone *root, char* path);
	void  BoneUpdate(Bone *root);


	void BoneSetEmptyTextrueID(Bone *root, int bodyID, int emptyID);

	//For bone animation 
	void  getMeshStartingPoint(Bone *root, Pipeline* pipeline);
	void  getHUDMeshStartingPoint(Bone *root, Pipeline* pipeline);
	void  BoneAnimate(Bone* root, bool& initFrame, int time, int actionType);
	bool  BonePressAnimate(Bone *root, int frameNubmer, int actionType);
	bool  BoneTriggerAnimate(Bone *root, int time, int actionType, bool& checkTrigger);
	bool  BoneGrabAnimate(Bone *root, int time, int actionType, bool& checkTrigger, bool& checkPressed);
	void  BoneReverse(Bone *root);
	void  BoneEndAnimate(Bone *root);
	//For bone animation(dt)
	void BoneUpdateTime(Bone* root, double deltaTime);

	//For Light
	void JEControlLight(JE::Vector3f color, float ambientIntensity);
	void JECalculateNormals(const unsigned int* pIndices, unsigned int indexCount, JEVertex* pVertices, unsigned int VertexCount);
	
	void JEInitLight(void);
	//point light
	void JEAddPointLight(int pointID, Vector3f position, Vector3f color, float ambientIntensity, float weakness);
	void JEUpdatePointLight(void);
	void JEChangePointLight(int pointID, Vector3f position, Vector3f color, float ambientIntensity, float weakness);
	void JEDeletePointLight(int pointID);

	//spot light
	void JEAddSpotLight(int spotID, Vector3f position, Vector3f color, float angle);
	void JEChangeSpotLight(int spotID, Vector3f position, Vector3f color, float ambientIntensity, float angle);
	float JEGetAmbientSpotLight(int spotID);

	void JEUpdateSpotLight(void);
	void JEDeleteSpotLight(int spotID);
	
	void JEShutdownLight(void);
	float JEGetDirectionalAmbient(void);
	JE::Vector3f JEGetDirectionalColor(void);

	//for objShader
	void JEUseObjShader(void);

	//for particle
	void JEParticleInit(void);
	void JEParticleUpdate(double dt);

	namespace
	{
		GLuint	vertexBufferObject;
		GLuint   indicesBufferObject;
		JEWindowData windowData;
		PersProjInfo persProjInfo;
		OthoProjInfo othoProjInfo;

		//For shader
		ObejctShader objShaderClass;

		//For cameara
		Camera* pGameCamera = NULL;

		//For Light
		DirectionalLight directionalPersLight;
		DirectionalLight directionalOrthoLight;
		DirectionalLight directionalParticleLight;
		DirectionalLight directionalHUDParticleLight;
		PointLight       pointLight;
		SpotLight        spotLight;

		//For Particle
		ParticleSystem m_particleSystem[MAX_PARTICLE];
	}
}

#endif 