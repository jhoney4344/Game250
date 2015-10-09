/*******************************************************************************
Copyright (C) 2015 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
filename    JEGraphicsNew.cpp
author      Seho.Joo
email       jsh2958@gmail.com
course      GAM250
assignment  Final milestone
due date    6/12/2015

Brief Description:
This file includes entire graphic program such as rendering, animating and
etc

*******************************************************************************/

#include "JEGraphicsNew.h"
#include "JEIniFileManager.h"
#include "JEDayAndNight.h"

namespace{
	JE::DayNight* pDayNight;
}

namespace JE
{
/*******************************************************************************
   Function: CreateVertexBuffer

Description: This function creates vertex buffer for rendering.

     Inputs: None.

    Outputs: None.
*******************************************************************************/
	static void CreateVertexBuffer(const unsigned int* pIndices, unsigned int indexCount)
	{
		//Number of vertex
		const int numberOfVertex = 4;
		//Initialize vertex structor array that has information of vectices.
		JE::JEVertex vertices[numberOfVertex] = { JE::JEVertex(Vector3f(-.5f, -.5f, 0.f) , Vector2f(0.f, 0.f)),
												  JE::JEVertex(Vector3f(.5f , -.5f, 0.f) , Vector2f(1.f, 0.f)),
												  JE::JEVertex(Vector3f(-.5f, .5f , 0.f) , Vector2f(0.f, 1.f)),
												  JE::JEVertex(Vector3f(.5f , .5f , 0.f) , Vector2f(1.f, 1.f))};
		
		unsigned int vertexCount = ARRAY_SIZE_IN_ELEMENTS(vertices);


		JECalculateNormals(pIndices, indexCount, vertices, vertexCount);
												  							  
		//Return currently unused names for texture objects in GPU.		
		glGenBuffers(1, &JE::vertexBufferObject);
		//Bind vertex buffer and set the data of vertex buffer	
		glBindBuffer(GL_ARRAY_BUFFER, JE::vertexBufferObject);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	}
/*******************************************************************************
   Function: CreateIndexBuffer

Description: This function decides the drawing order of vertex.

     Inputs: None.

    Outputs: None.
*******************************************************************************/
	static void CreateIndexBuffer(const unsigned int* pIndices, unsigned int sizeInBytes)
	{
		//Return currently unused names for texture objects in GPU.			
		glGenBuffers(1, &indicesBufferObject);
		//Bind index buffer and set the data of index buffer	
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesBufferObject);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeInBytes, pIndices, GL_STATIC_DRAW);
	}
/*******************************************************************************
   Function: GraphicsCreatingOpenGLContext

Description: This function creates open GL rendering context.

     Inputs: None.

    Outputs: None.
*******************************************************************************/
	static void GraphicCreatingOpenGLContext(void)
	{
		//the properties of the default framebuffer that the OpenGL context
		PIXELFORMATDESCRIPTOR pfd =
		{
			sizeof(PIXELFORMATDESCRIPTOR),
			1,
			PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER, //Flags
			PFD_TYPE_RGBA,		//The kind of framebuffer. RGBA or palette.
			32,					//Colordepth of the framebuffer.
			0, 0, 0, 0, 0, 0,
			0,
			0,
			0,
			0, 0, 0, 0,
			24,					//Number of bits for the depthbuffer
			8,					//Number of bits for the stencilbuffer
			0,                  //Number of Aux buffers in the framebuffer.
			PFD_MAIN_PLANE,
			0,
			0, 0, 0
		};
	
		//The result value after choosing my pixel format
		int pixelFormat;

		memset(&pfd, 0, sizeof(pfd));

		//choose back buffer
		pixelFormat = ChoosePixelFormat(windowData.deviceContext, &pfd);

		//set the pixel format
		SetPixelFormat(windowData.deviceContext, pixelFormat, &pfd);

		//create the opengl context
		windowData.oglContext = wglCreateContext(windowData.deviceContext);

		//set the opengl context
		wglMakeCurrent(windowData.deviceContext, windowData.oglContext);

	}

/*******************************************************************************
   Function: GraphicsInit

Description: This function initializes mostly graphic options.

     Inputs: window - window handler.
	         width  - width of window screen
			 height - height of window screen

    Outputs: None.
*******************************************************************************/

	void GraphicInit(HWND window, int left, int right, int top, int bottom)
	{
		//Get the projection information
		int width  = right - left;
		int height = bottom - top;

		persProjInfo.m_width   = (float)width;
		persProjInfo.m_height  = (float)height;
		persProjInfo.m_FOV     = 100.f;
		persProjInfo.m_zFar    = 1000.f;
		persProjInfo.m_zNear   = 1.f;

		othoProjInfo.m_nearVal = 1.f;
		othoProjInfo.m_farVal  = 1000.f;
		othoProjInfo.m_bottom  = 0.f;
		othoProjInfo.m_top     = (float)height;
		othoProjInfo.m_left    = 0.f;
		othoProjInfo.m_right   = (float)width;


		//Get the window data
		windowData.windowWidth  = width;
		windowData.windowHeight = height;
		windowData.windowHandle = window;

		//Get device context for creating opengl rendering context 
		windowData.deviceContext = GetDC(windowData.windowHandle);

		//Make opengl rendering context
		GraphicCreatingOpenGLContext();

		//Release divice context
		ReleaseDC(windowData.windowHandle, windowData.deviceContext);
		windowData.deviceContext = 0;

		//Set background color
		glClearColor(1.f, 1.f, 1.f, 1.f);

		//Fill the front side
		glPolygonMode(GL_FRONT, GL_FILL);

		//enable texture 2D
		glEnable(GL_TEXTURE_2D);
		//enable smooth shading
		glShadeModel(GL_SMOOTH);

		//Blending function for translucency based on source alpha value
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//Enable depth buffer setup
		glEnable(GL_DEPTH_TEST);
		glClearDepth(1.0f);
		glDepthFunc(GL_LEQUAL);

		glEnable(GL_CULL_FACE);
		//Decide direction that draws the vertex(counter clockwise)
		glFrontFace(GL_CCW);
		//Draw only front side 
		glCullFace(GL_BACK);

		glewInit();
		
		//the drawing order of vertex
		unsigned int Indices[] = { 3, 2, 0,
								   1, 3, 0 };

		CreateIndexBuffer(Indices, sizeof(Indices));
		CreateVertexBuffer(Indices, ARRAY_SIZE_IN_ELEMENTS(Indices));

		//Compile the shader
		objShaderClass.Init();
	}



/*******************************************************************************
   Function: GraphicStartDraw

Description: This function set the opengl rendering context and clear 
             the buffer.
     Inputs: None.

    Outputs: None.
*******************************************************************************/
	void GraphicStartDraw(void)
	{
		//Set the open GL redering contex
		windowData.deviceContext = GetDC(windowData.windowHandle);
		wglMakeCurrent(windowData.deviceContext, windowData.oglContext);
		//Clear the buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

/*******************************************************************************
   Function: GraphicEndDraw

Description: This function release device context and swap the buffer for 
             eliminating flickering.
     Inputs: None.

    Outputs: None.
*******************************************************************************/
	void GraphicEndDraw(void)
	{
		//Swap the buffer
		SwapBuffers(windowData.deviceContext);
		//Release device context
		ReleaseDC(windowData.windowHandle, windowData.deviceContext);
		windowData.deviceContext = 0;
	}
/*******************************************************************************
   Function: GraphicsSetBackgroundColor

Description: This function set back ground color.
     Inputs: red   - red
	         green - green
			 blue  - blue

    Outputs: None.
*******************************************************************************/
	void GraphicSetBackgroundColor(float red, float green, float blue)
	{
		//Set the back ground color
		glClearColor(red, green, blue, 1.f);
	}

/*******************************************************************************
   Function: GraphicsSetTexture

Description: This function set texture ID that is wanted by user.
     Inputs: textureID - buffer object

    Outputs: None.
*******************************************************************************/
	void GraphicSetTexture(int textureID)
	{
		//Select active texture unit
		glActiveTexture(GL_TEXTURE0);
		//Make active buffer array
		glBindTexture(GL_TEXTURE_2D, textureID);
	}

	void GraphicSetRandomTexture(int textureID)
	{
		//Select active texture unit
		glActiveTexture(GL_TEXTURE3);
		//Make active buffer array
		glBindTexture(GL_TEXTURE_2D, textureID);
	}
/*******************************************************************************
   Function: GraphicSetPersTransform

Description: This function makes translation matrix and applies it in shader 
             program.
     Inputs: transX  - X coordinate of buffer object
			 transY  - Y coordinate of buffer object
			 zOrder  - zOrder
			 scaleX  - scale X of buffer object
			 scaleY  - scale Y of buffer object
			 rotateZ - rotation of buffer object(z-axis)
    Outputs: None.
*******************************************************************************/
	void GraphicSetPersTransform(double transX, double transY, double zOrder,
							 double scaleX, double scaleY,
							 double rotateZ)
	{
		//Initialize pipeLine for making translation matrix
		Pipeline p;
		int characterNum = 0;
	
		//Set translation matrix in pipeline
		p.WorldPos((float)transX, (float)transY, (float)zOrder);
		p.Scale((float)scaleX, (float)scaleY, 0);
		p.Rotate(0, 0, (float)rotateZ);

		const Matrix4f& worldTransformation = p.GetWorldTrans();

		//Set perspective information in pipeline
		p.SetPerspective(persProjInfo);
		//Set camera data in pipeline.
		p.SetCamera(*pGameCamera);

		//Set the uniform variable that is located in worlViewProjecLocation
		objShaderClass.SetWolrdViewProjec(p.GetWorldViewPerspectTrans());
		objShaderClass.SetDirectionalLight(directionalPersLight);
		objShaderClass.SetWorldMatrix(worldTransformation);

		SpotLight::spotIter spotSave = spotLight.GetBeginItor();
		if (spotSave != spotLight.GetendItor())
		{
			if (spotSave->m_cutOff == 0)
			{
				while (spotSave != spotLight.GetendItor())
				{
					spotSave->m_cutOff = pDayNight->GetSpotAngle(characterNum);
					spotSave->m_ambientIntensity = pDayNight->GetSpotAmbient(characterNum);
					++characterNum;
					++spotSave;
				}
				JEUpdateSpotLight();
			}
		}

		PointLight::pointIter pointSave = pointLight.GetBeginItor();
		if (pointSave != pointLight.GetendItor())
		{
			if (pointSave->m_constant == 0)
			{
				while (pointSave != pointLight.GetendItor())
				{
					pointSave->m_constant = 1.f;
					pointSave->m_linear = 10.f;
					++pointSave;
				}
				JEUpdatePointLight();
			}
		}

		GraphicDrawing();
	}
/*******************************************************************************
Function: GraphicSetParticleTransform

Description: This function makes translation matrix and applies it in shader
program for particle.
Inputs: transX  - X coordinate of buffer object
transY  - Y coordinate of buffer object
zOrder  - zOrder
scaleX  - scale X of buffer object
scaleY  - scale Y of buffer object
rotateZ - rotation of buffer object(z-axis)
ambientIntensity - ambient intensity for each particle
Outputs: None.
*******************************************************************************/

	void GraphicSetParticleTransform(double transX, double transY, double zOrder,
		double scaleX, double scaleY,
		double rotateZ,
		float ambientIntensity)
	{
		//Initialize pipeLine for making translation matrix
		Pipeline p;

		//Set translation matrix in pipeline
		p.WorldPos((float)transX, (float)transY, (float)zOrder);
		p.Scale((float)scaleX, (float)scaleY, 0);
		p.Rotate(0, 0, (float)rotateZ);

		directionalParticleLight.m_ambientIntensity = ambientIntensity;

		const Matrix4f& worldTransformation = p.GetWorldTrans();

		//Set perspective information in pipeline
		p.SetPerspective(persProjInfo);
		//Set camera data in pipeline.
		p.SetCamera(*pGameCamera);

		//Set the uniform variable that is located in worlViewProjecLocation
		objShaderClass.SetWolrdViewProjec(p.GetWorldViewPerspectTrans());
		objShaderClass.SetDirectionalLight(directionalParticleLight);
		objShaderClass.SetWorldMatrix(worldTransformation);

		SpotLight::spotIter spotSave = spotLight.GetBeginItor();
		if (spotSave != spotLight.GetendItor())
		{
			if (spotSave->m_cutOff != 0)
			{
				while (spotSave != spotLight.GetendItor())
				{
					spotSave->m_cutOff = 0;
					spotSave->m_ambientIntensity = 0;
					++spotSave;
				}
				JEUpdateSpotLight();
			}
		}

		PointLight::pointIter pointSave = pointLight.GetBeginItor();
		if (pointSave != pointLight.GetendItor())
		{
			if (pointSave->m_constant == 0)
			{
				while (pointSave != pointLight.GetendItor())
				{
					pointSave->m_constant = 1.f;
					pointSave->m_linear = 10.f;
					++pointSave;
				}
				JEUpdatePointLight();
			}
		}

		GraphicDrawing();
	}
	void GraphicSetOrthoParticleTransform(double transX, double transY, double zOrder,
										  double scaleX, double scaleY,
										  double rotateZ,
										  float ambientIntensity)
	{
		//Initialize pipeLine for making translation matrix
		Pipeline p;

		//Set translation matrix in pipeline
		p.WorldPos((float)transX, (float)transY, (float)zOrder);
		p.Scale((float)scaleX, (float)scaleY, 0);
		p.Rotate(0, 0, (float)rotateZ);

		directionalHUDParticleLight.m_ambientIntensity = ambientIntensity;


		//Set perspective information in pipeline
		p.SetOthogonal(othoProjInfo);
		//Set camera data in pipeline.
		p.SetCamera(*pGameCamera);

		//Set the uniform variable that is located in worlViewProjecLocation
		objShaderClass.SetWolrdViewProjec(p.GetWorldViewOthogonalTrans());
		objShaderClass.SetDirectionalLight(directionalHUDParticleLight);

		SpotLight::spotIter spotSave = spotLight.GetBeginItor();
		if (spotSave != spotLight.GetendItor())
		{
			if (spotSave->m_cutOff != 0)
			{
				while (spotSave != spotLight.GetendItor())
				{
					spotSave->m_cutOff = 0;
					spotSave->m_ambientIntensity = 0;
					++spotSave;
				}
				JEUpdateSpotLight();
			}
		}

		PointLight::pointIter pointSave = pointLight.GetBeginItor();
		if (pointSave != pointLight.GetendItor())
		{
			if (pointSave->m_constant == 0)
			{
				while (pointSave != pointLight.GetendItor())
				{
					pointSave->m_constant = 0.f;
					pointSave->m_linear = 0.f;
					++pointSave;
				}
				JEUpdatePointLight();
			}
		}

		GraphicDrawing();
	}


	void GraphicSetOrthogTransform(double transX, double transY, double zOrder,
		double scaleX, double scaleY,
		double rotateZ)
	{
		//Initialize pipeLine for making translation matrix
		Pipeline p;

		//Set translation matrix in pipeline
		p.WorldPos((float)transX, (float)transY, (float)zOrder);
		p.Scale((float)scaleX, (float)scaleY, 0);
		p.Rotate(0, 0, (float)rotateZ);
		//Set perspective information in pipeline
		p.SetOthogonal(othoProjInfo);
		//Set camera data in pipeline.
		p.SetCamera(*pGameCamera);


		//Set the uniform variable that is located in worlViewProjecLocation
		objShaderClass.SetWolrdViewProjec(p.GetWorldViewOthogonalTrans());
		objShaderClass.SetDirectionalLight(directionalOrthoLight);

		SpotLight::spotIter spotSave = spotLight.GetBeginItor();
		if (spotSave != spotLight.GetendItor())
		{
			if (spotSave->m_cutOff != 0)
			{
				while (spotSave != spotLight.GetendItor())
				{
					spotSave->m_cutOff = 0;
					spotSave->m_ambientIntensity = 0;
					++spotSave;
				}
				JEUpdateSpotLight();
			}
		}

		PointLight::pointIter pointSave = pointLight.GetBeginItor();
		if (pointSave != pointLight.GetendItor())
		{
			if (pointSave->m_constant != 0)
			{
				while (pointSave != pointLight.GetendItor())
				{
					pointSave->m_constant = 0.f;
					pointSave->m_linear = 0.f;
					++pointSave;
				}
				JEUpdatePointLight();
			}
		}

		/*JEUpdateSpotLight();
		JEUpdatePointLight();*/

		GraphicDrawing();
	}

	void GraphicSetHUDTransform(double transX, double transY, double zOrder,
		double scaleX, double scaleY,
		double rotateZ)
	{
		//Initialize pipeLine for making translation matrix
		Pipeline p;

		//Set translation matrix in pipeline
		p.WorldPos((float)transX, (float)transY, (float)zOrder);
		p.Scale((float)scaleX, (float)scaleY, 0);
		p.Rotate(0, 0, (float)rotateZ);
		//Set perspective information in pipeline
		p.SetPerspective(persProjInfo);

		/*SpotLight* spotSave = spotLight.FindSpotLight(0);
		spotSave->m_cutOff = 0;
		SpotLight* spotSave2 = spotLight.FindSpotLight(1);
		spotSave2->m_cutOff = 0;

		objShaderClass.SetWolrdViewProjec(p.GetWorldPerspectTrans());
		objShaderClass.SetDirectionalLight(directionalOrthoLight);
		objShaderClass.SetSpotLights(spotLight.m_countOfSpot, spotSave);*/

		GraphicDrawing();
	}

	void GraphicDrawing(void)
	{
		//Enable location 0 in shader.vert
		glEnableVertexAttribArray(0);
		//Enable location 1 in shader.vert
		glEnableVertexAttribArray(1);

		glEnableVertexAttribArray(2);

		//Active vertex buffer
		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);

		//connect 'in' variable in the shader.vert
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(JEVertex), 0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(JEVertex), (const GLvoid*)12);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(JEVertex), (const GLvoid*)20);

		//Active indices buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesBufferObject);

		//Draw the vertices by drawing order
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		//Disable location 0 in shader.vert
		glDisableVertexAttribArray(0);
		//Disable location 1 in shader.vert
		glDisableVertexAttribArray(1);

		glDisableVertexAttribArray(2);
	}

	void GraphicConvertWorldToScreen(double& mouseX, double& mouseY)
	{
		GLint viewPort[4]; //= { 0, 0, windowData.windowWidth, windowData.windowHeight};
		GLdouble modelView[16];
		GLdouble projection[16];
		GLdouble winX, winY; 
		GLdouble objX, objY, objZ;
		
		glGetDoublev(GL_MODELVIEW_MATRIX, modelView);
		glGetDoublev(GL_PROJECTION_MATRIX, projection);
		glGetIntegerv(GL_VIEWPORT, viewPort);

		winX = (float)mouseX;
		winY = (float)mouseY;

		gluUnProject(winX, winY, 0, modelView, projection, viewPort, &objX, &objY, &objZ);

		mouseX = objX;
		mouseY = objY;
	}

	void GraphicConvertScreenToWorld(double& mouseX, double& mouseY)
	{
		Pipeline p;

		GLint viewPort[4];
		GLdouble modelView[16];
		GLdouble projection[16];
		GLdouble winX, winY, winZ;
		GLdouble objX, objY, objZ;

		//p.WorldPos((float)mouseX, (float)mouseY, 0);
		p.SetPerspective(persProjInfo);

		Matrix4f projectMatrix = p.GetWorldPerspectTrans();

		glGetDoublev(GL_MODELVIEW_MATRIX, modelView);
		
		for (unsigned int j = 0; j < 4; ++j)
			for (unsigned int i = 0; i < 4; ++i)
				projection[j * 4 + i] = projectMatrix.m[j][i];
		//glGetDoublev(GL_PROJECTION_MATRIX, projection);

		glGetIntegerv(GL_VIEWPORT, viewPort);

		winX = (float)mouseX;
		winY = (float)mouseY;

		//gluProject(0, 0, 0, modelView, projection, viewPort, &winX, &winY, &winZ);
		glReadPixels((GLint)winX, (GLint)winY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);

		gluUnProject(winX, winY, winZ, modelView, projection, viewPort, &objX, &objY, &objZ);

		mouseX = objX;
		mouseY = objY;
	}


	void GraphicSetTextureUnit(int unit)
	{
		objShaderClass.SetTextureUnit(unit);
	}

/*******************************************************************************
   Function: GraphicsShutdown

Description: This function shut down all graphic programs.
     
	 Inputs: None.
    
	Outputs: None.
*******************************************************************************/
	void GraphicsShutdown(void)
	{
		//Delete buffers
		glDeleteBuffers((GLsizei)1, &vertexBufferObject);
		glDeleteBuffers((GLsizei)1, &indicesBufferObject);

		//Delete open GL rendering context
		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(windowData.oglContext);
		windowData.oglContext = 0;
	}

/*******************************************************************************
   Function: GraphicInitCamera

Description: This function initilize camera class
     
	 Inputs: None.
    
	Outputs: None.
*******************************************************************************/
	void GraphicInitCamera(void)
	{
		//Initialize camera class
		pGameCamera = new Camera(windowData.windowWidth, windowData.windowHeight);
	}

/*******************************************************************************
   Function: GraphicCameraPosition

Description: This function set the camera position
     
	 Inputs: x - coordinate X of camera
	         y - coordinate Y of camera
    
	Outputs: None.
*******************************************************************************/
	void GraphicCameraPosition(float x, float y)
	{
		//Set the camera position
		pGameCamera->SetCameraPosition(x,y);
	}

/*******************************************************************************
   Function: GraphicCameraZoom

Description: This function set the camera zoom
     
	 Inputs: zoomIn        - This variable decides whether the order is zoom in 
	                         or zoom out.
	         magnification - Magnification of zoom in or zoom out.
    
	Outputs: None.
*******************************************************************************/
	void GraphicCameraZoom(bool zoomIn, float magnification)
	{
		pGameCamera->SetCameraZoom(zoomIn, magnification);
	}

	Vector3f GraphicGetCameraPos()
	{
		return pGameCamera->getPos();
	}

	void GraphicSetCameraPos(Vector3f pos)
	{
		pGameCamera->setPos(pos);
	}

/*******************************************************************************
   Function: GraphicSetAnimaTex

Description: This function devides textrue coordinate for making animation

	 Inputs: numberOfFrame - number of frame in animation
	         rateOfChange  - rate of change about each frame
    
	Outputs: None.
*******************************************************************************/
	void GraphicSetAnimaTex(float numberOfFrame, float rateOfChange) 
	{
		float changedTexX = STANDARD_TEX_X/numberOfFrame;
		Vector2f scaleTex_X(changedTexX, 1);
		Vector2f transTex_X(rateOfChange,0);

		objShaderClass.SetAniTex(scaleTex_X, transTex_X);
	}

/*******************************************************************************
   Function: GraphicSetFilpTex

Description: This function decides whether the texture flip or not

	 Inputs: axis          - standard axis for flipping
	         numberOfFrame - number of frame in animation
    
	Outputs: None.
*******************************************************************************/

	void GraphicSetFilpTex(FLIP_STANDARD_AXIS axis, float numberOfFrame)
	{
		Vector2f flipScale;
		Vector2f flipTrans;

		float frameRate = 1 / numberOfFrame;

		switch (axis)
		{
			case FLIP_X:
				flipTrans = { frameRate, 0 };
				flipScale = { -1, 1 };
				break;

			case FLIP_Y:
				flipTrans = { 0, frameRate };
				flipScale = { 1, -1 };
				break;

			case FLIP_XY:
				flipTrans = { frameRate, frameRate };
				flipScale = { -1, -1 };
				break;

			case FLIP_IDLE:
				flipTrans = { 0, 0 };
				flipScale = { 1, 1 };
				break;
		}
	
		objShaderClass.SetFlipTex(flipScale, flipTrans);
	}

	//////////////////////////////For bone(function)/////////////////////////

	/*******************************************************************************
	Function: IntToString

	Description: This function change int to string.

	Inputs: number - The variable that change to string.

	Outputs: return string that be changed from int
	*******************************************************************************/
	//helper function int to string
	inline std::string IntToString(int number)
	{
		std::ostringstream convStream;

		convStream << number;

		return convStream.str();
	}
	/*******************************************************************************
	Function: boneAddChild

	Description: This function makes each bone

	Inputs: root   - Bone pointer
	x     - x of bone
	y     - y of bone
	a     - angle of bone
	l     - length of bone
	flags - number of hanged parts from the current bone

	Outputs: return bone pointer that saves data
	*******************************************************************************/
	Bone *BoneAddChild(Bone *root,
		float x, float y,
		float a, float l,
		unsigned int flags,
		char *depthStr,
		char *name,
		int part)
	{
		Bone *pSave = 0;

		//if there is no root, create one
		if (!root)
		{
			root = new Bone;

			root->parent = 0;
		}
		//if there is space for another child
		else if (root->childCount < MAX_CHCOUNT)
		{
			pSave = new Bone;

			//Set it's parent
			pSave->parent = root;
			//Increment the childCounter
			root->child[root->childCount++] = pSave;
			root = pSave;
		}
		else
		{
			return 0;
		}

		//Set the bone data
		root->x = x;
		root->y = y;
		root->a = a;
		root->l = l;
		root->flags = flags;
		root->childCount = 0;
		root->actionTypeCount = 0;
		root->offA = 0;
		root->offL = 0;
		root->initA = a;
		root->initL = l;
		root->part = part;


		for (int index = 0; index < MAX_ATCOUNT; ++index)
			root->keyframeCount[index] = 0;

		//decide bone's name
		if (name)
			strcpy_s(root->name, name);
		else
			strcpy_s(root->name, "Bone");

		if (depthStr)
			strcpy_s(root->depthStr, depthStr);

		for (int i = 0; i < MAX_CHCOUNT; ++i)
			root->child[i] = NULL;

		return root;
	}


	//Add bone ani child
	Bone *BoneAddAniChild(Bone* pRoot, Keyframe** ppKeyframe, int numberOfAction, int numberOfFrame)
	{
		if (!pRoot)
			return 0;

		for (int i = 0; i < numberOfAction; ++i)
		{
			for (int j = 0; j < numberOfFrame; ++j)
			{
				 pRoot->keyframe[i][j]= ppKeyframe[i][j];
			}
		}
		return pRoot;
	}

	Bone *BoneAddTextureChild(Bone* pRoot, int textureID, FLIP_STANDARD_AXIS flip)
	{
		if (!pRoot)
			return 0;

		pRoot->textureID = textureID;
		pRoot->flip = flip;

		return pRoot;
	}
	/*******************************************************************************
	Function: BoneFreeTree

	Description: This function deletes bones

	Inputs: root - Bone pointer

	Outputs: return null
	*******************************************************************************/
	Bone* BoneFreeTree(Bone *root)
	{
		if (!root)
			return 0;

		for (unsigned int i = 0; i < root->childCount; ++i)
			BoneFreeTree(root->child[i]);

		//delete bone
		delete root;

		return 0;
	}

	/*******************************************************************************
	Function: BoneFindByName

	Description: This function finds unique bone

	Inputs: root  - Bone pointer
	name  - Bone name for finding unique bone

	Outputs: return bone pointer that has same name
	*******************************************************************************/
	Bone* BoneFindByName(Bone *root, char *name)//, char* depth)
	{
		Bone *p;

		if (!root)
			return NULL;
		
		//if we find the bone, return bone
		if (!strcmp(root->name, name))// && !strcmp(root->depthStr, depth) )//&& root->part == part)
			return root;

		//if we can't the bone, then search their children
		for (unsigned int i = 0; i < root->childCount; ++i)
		{
			p = BoneFindByName(root->child[i], name);//, depth);// , part);

			if (p)
				return p;
		}
		return NULL;
	}

	void BoneSetTextureID(Bone *root, char *partName, int textureID, FLIP_STANDARD_AXIS flip)
	{
		Bone *pSaveBone = BoneFindByName(root, partName);
		pSaveBone->textureID = textureID;
		pSaveBone->flip = flip;
	}
	/*******************************************************************************
	Function: BoneListNames

	Description: This function fills a NULL terminated array of strings.

	Inputs: root  - Bone pointer
	name  - Bone name

	Outputs: None.
	*******************************************************************************/
	void BoneListNames(Bone *root, char names[MAX_BONECOUNT][20])
	{
		unsigned int i;
		int present;

		if (!root)
			return;

		present = 0;
		for (i = 0; (i < MAX_BONECOUNT) && (names[i][0] != '\0'); ++i)
		{
			if (!strcmp(names[i], root->name))
			{
				present = 1;
				break;
			}
		}
		if (!present && (i < MAX_BONECOUNT))
		{
			strcpy_s(names[i], root->name);

			if (i + 1 < MAX_BONECOUNT)
			{
				names[i + 1][0] = '\0';
			}
		}

		for (i = 0; i < root->childCount; ++i)
			BoneListNames(root->child[i], names);
	}
	/*******************************************************************************
	Function: BoneLoadStructure

	Description: This function load ini file that has information of bones

	Inputs: path - Path that has ini file

	Outputs: return bone pointer
	*******************************************************************************/
	Bone* BoneLoadStructure(char *path)
	{
		//The root of the tree to Load
		Bone *root;
		//A temporary root
		Bone *temp;

		float x,
			y,
			length;
		int   angle;
		int  part;

		int depth,		//Depth retrieved from file
			actualLevel = 0,//Actual depth level
			flags,		//Bone flags
			numberOfLine = 0,
			numberOfAction = 0,
			numberOfFrame = 0;

		char name[20],
			depthStr[20];

		unsigned int time;

		Keyframe *k;
		CopyFrameTime *copyTime;

		root = NULL;
		temp = NULL;


		numberOfLine = GetPrivateProfileInt("Animation", "NumberOfLines", 0, path);

		
		for (int partIndex = 0; partIndex < numberOfLine; ++partIndex)
		{
			std::string s_part = "Part" + IntToString(partIndex);

			LPCSTR s_part_conversion = s_part.c_str();

			GetPrivateProfileString(s_part_conversion, "Depth", 0, depthStr, 20, path);
			x = (float)GetPrivateProfileInt(s_part_conversion, "X", 0, path);
			y = (float)GetPrivateProfileInt(s_part_conversion, "Y", 0, path);

			angle = GetPrivateProfileInt(s_part_conversion, "Angle", 0, path);
			length = (float)GetPrivateProfileInt(s_part_conversion, "Length", 0, path) / LENGTH_DECREASE_RATE;
			flags = GetPrivateProfileInt(s_part_conversion, "Flag", 0, path);

			GetPrivateProfileString(s_part_conversion, "PartName", 0, name, 20, path);

			part = GetPrivateProfileInt(s_part_conversion, "Part", 0, path);

			if (!part)
			{
				part = 1;
			}

			depth = strlen(depthStr) - 1;
			if (depth < 0 || depth > MAX_CHCOUNT)
			{
				return NULL;
			}

			for (; actualLevel > depth; --actualLevel)
				temp = temp->parent;

			if (!root && !depth)
			{
				root = BoneAddChild(NULL, x, y, (float)angle, length, flags, depthStr, name, part);
				temp = root;
			}
			else
				temp = BoneAddChild(temp, x, y, (float)angle, length, flags, depthStr, name, part);

			//about animation

			numberOfAction = GetPrivateProfileInt(s_part_conversion, "NumberOfActions", 0, path);

			temp->NumberOfAction = numberOfAction;

			for (int actionIndex = 0; actionIndex < numberOfAction; ++actionIndex)
			{
				std::string s_action = "Action-" + IntToString(actionIndex) + "/part" + IntToString(partIndex);
				LPCSTR s_action_conversion = s_action.c_str();

				//read number of animation
				numberOfFrame = GetPrivateProfileInt(s_action_conversion, "NumberOfFrames", 0, path);

				for (int frameIndex = 0; frameIndex < numberOfFrame; ++frameIndex)
				{
					std::string s_time = "Time" + IntToString(frameIndex);
					std::string s_length = "Length" + IntToString(frameIndex);
					std::string s_angle = "Angle" + IntToString(frameIndex);

					LPCSTR s_time_conversion = s_time.c_str();
					LPCSTR s_length_conversion = s_length.c_str();
					LPCSTR s_angle_conversion = s_angle.c_str();

					time = GetPrivateProfileInt(s_action_conversion, s_time_conversion, 0, path);
					length = (float)GetPrivateProfileInt(s_action_conversion, s_length_conversion, 0, path) / LENGTH_DECREASE_RATE;
					angle = GetPrivateProfileInt(s_action_conversion, s_angle_conversion, 0, path);

					if (temp->keyframeCount[actionIndex] >= MAX_KFCOUNT)
					{
						continue;
					}

					k = &(temp->keyframe[temp->actionTypeCount][temp->keyframeCount[actionIndex]]); 
					copyTime = &(temp->copyTime[temp->actionTypeCount][temp->keyframeCount[actionIndex]]);

					//control
					k->time = time * ANIMATION_SPEED;
					k->length = length;
					k->angle = (float)angle;

					copyTime->time = k->time;

					temp->keyframeCount[actionIndex]++;
				}
				temp->actionTypeCount++;
			}
			temp->actionTypeCount = 0;
			++actualLevel;
		}
		return root;
	}

	void getMeshStartingPoint(Bone *root, Pipeline* pipeline)
	{
		if (!root)
			return;

		if (root->parent)
		{
			float startTransX = (cos(DEG2RAD(root->parent->a)) * root->parent->l);
			float startTransY = (sin(DEG2RAD(root->parent->a)) * root->parent->l);
			float startTransZ;

			Vector3f preWorldPos = pipeline->getWorldPos();

			startTransX += preWorldPos.m_x;
			startTransY += preWorldPos.m_y;
			startTransZ = preWorldPos.m_z;

			pipeline->WorldPos(startTransX, startTransY, startTransZ);
			
			getMeshStartingPoint(root->parent, pipeline); 
		}
	}

	void getHUDMeshStartingPoint(Bone *root, Pipeline* pipeline)
	{
		if (!root)
			return;

		if (root->parent)
		{
			float startTransX = (cos(DEG2RAD(root->parent->a)) * root->parent->l * 0.05f);
			float startTransY = (sin(DEG2RAD(root->parent->a)) * root->parent->l * 0.05f);
			float startTransZ;

			Vector3f preWorldPos = pipeline->getWorldPos();

			startTransX += preWorldPos.m_x;
			startTransY += preWorldPos.m_y;
			startTransZ = preWorldPos.m_z;

			pipeline->WorldPos(startTransX, startTransY, startTransZ);

			getHUDMeshStartingPoint(root->parent, pipeline);
		}
	}

	/*******************************************************************************
	Function: BoneDraw

	Description: This function draws bones that has each information

	Inputs: root - Bone pointer

	Outputs: None.
	*******************************************************************************/
	void BoneDraw(Bone *root, double transX, double transY, double transZ)
	{
		//Initialize pipeLine for making translation matrix
		Pipeline p;
		int characterNum = 0;

		float midTransX = (cos(DEG2RAD(root->a)) * root->l) / 2;
		float midTransY = (sin(DEG2RAD(root->a)) * root->l) / 2;

		
		p.WorldPos((float)midTransX + (float)transX, (float)midTransY + (float)transY, (float)transZ);

		//p.Scale(0.6f, 0.6f, 0);
		if (!strcmp(root->name, "body"))
		{
			p.Scale(root->l * 1.4f, 5.f, 0);
		}
		else if (!strcmp(root->name,"leftleg1")|| 
			     !strcmp(root->name,"leftleg2")||
				 !strcmp(root->name,"rightleg1")||
				 !strcmp(root->name,"rightleg2"))
		{
			p.Scale(root->l * 1.4f, 5.f, 0);
		}
		else if (!strcmp(root->name, "head"))
		{
			p.Scale(root->l * 1.4f, 3.8f, 0);
		}
		else
		{
			p.Scale(root->l * 1.4f, 3.f, 0);
		}
		
		p.Rotate(0, 0, root->a);

		//Set perspective information in pipeline
		p.SetPerspective(persProjInfo);
		
		//Set camera data in pipeline.
		p.SetCamera(*pGameCamera);

		getMeshStartingPoint(root, &p);

		GraphicSetTextureUnit(0);
		GraphicSetTexture(root->textureID);
		GraphicSetAnimaTex(1, 0.f);
		GraphicSetFilpTex(root->flip, 1);

		//Set the uniform variable that is located in worlViewProjecLocation
		//glUniformMatrix4fv(worldViewProjecLocation, 1, GL_TRUE, (const GLfloat*)p.GetWorldPerspectTrans());

		//Set the uniform variable that is located in worlViewProjecLocation
		objShaderClass.SetWolrdViewProjec(p.GetWorldViewPerspectTrans());
		objShaderClass.SetDirectionalLight(directionalPersLight);
		objShaderClass.SetWorldMatrix(p.GetWorldTrans());


		SpotLight::spotIter spotSave = spotLight.GetBeginItor();
		if (spotSave != spotLight.GetendItor())
		{
			if (spotSave->m_cutOff == 0)
			{
				while (spotSave != spotLight.GetendItor())
				{
					spotSave->m_cutOff = pDayNight->GetSpotAngle(characterNum);
					spotSave->m_ambientIntensity = pDayNight->GetSpotAmbient(characterNum);
					++characterNum;
					++spotSave;
				}
				JEUpdateSpotLight();
			}
		}

		PointLight::pointIter pointSave = pointLight.GetBeginItor();
		if (pointSave != pointLight.GetendItor())
		{
			if (pointSave->m_constant == 0)
			{
				while (pointSave != pointLight.GetendItor())
				{
					pointSave->m_constant = 1.f;
					pointSave->m_linear = 10.f;
					++pointSave;
				}
				JEUpdatePointLight();
			}
		}

		//Enable location 0 in shader.vert
		glEnableVertexAttribArray(0);
		//Enable location 1 in shader.vert
		glEnableVertexAttribArray(1);

		glEnableVertexAttribArray(2);

		//Active vertex buffer
		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);

		//connect 'in' variable in the shader.vert
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(JEVertex), 0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(JEVertex), (const GLvoid*)12);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(JEVertex), (const GLvoid*)20);

		//Active indices buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesBufferObject);

		//Draw the vertices by drawing order
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		//Disable location 0 in shader.vert
		glDisableVertexAttribArray(0);
		//Disable location 1 in shader.vert
		glDisableVertexAttribArray(1);

		glDisableVertexAttribArray(2);
	
		for (unsigned int i = 0; i <root->childCount; i++)
			BoneDraw(root->child[i], transX, transY, transZ);
	
	}

	void BoneDrawHUD(Bone *root, double transX, double transY, double transZ)
	{
		//Initialize pipeLine for making translation matrix
		Pipeline p;

		float midTransX = (cos(DEG2RAD(root->a)) * root->l * 0.05f) / 2;
		float midTransY = (sin(DEG2RAD(root->a)) * root->l * 0.05f) / 2;


		p.WorldPos((float)midTransX + (float)transX, (float)midTransY + (float)transY, (float)transZ);

		//p.Scale(0.6f, 0.6f, 0);
		if (!strcmp(root->name, "body"))
		{
			p.Scale(root->l * .07f, .25f, 0);
		}
		else if (!strcmp(root->name, "leftleg1") ||
			!strcmp(root->name, "leftleg2") ||
			!strcmp(root->name, "rightleg1") ||
			!strcmp(root->name, "rightleg2"))
		{
			p.Scale(root->l * .07f, .25f, 0);
		}
		else if (!strcmp(root->name, "head"))
		{
			p.Scale(root->l * .07f, 0.19f, 0);
		}
		else
		{
			p.Scale(root->l * 0.07f, 0.15f, 0);
		}

		p.Rotate(0, 0, root->a);

		//Set othogonal information in pipeline
		p.SetOthogonal(othoProjInfo);

		//Set camera data in pipeline.
		p.SetCamera(*pGameCamera);

		getHUDMeshStartingPoint(root, &p);

		GraphicSetTextureUnit(0);
		GraphicSetTexture(root->textureID);
		GraphicSetAnimaTex(1, 0.f);
		GraphicSetFilpTex(root->flip, 1);

		//Set the uniform variable that is located in worlViewProjecLocation
		//glUniformMatrix4fv(worldViewProjecLocation, 1, GL_TRUE, (const GLfloat*)p.GetWorldPerspectTrans());

		//Set the uniform variable that is located in worlViewProjecLocation
		objShaderClass.SetWolrdViewProjec(p.GetWorldViewOthogonalTrans());
		objShaderClass.SetDirectionalLight(directionalOrthoLight);


		SpotLight::spotIter spotSave = spotLight.GetBeginItor();
		if (spotSave != spotLight.GetendItor())
		{
			if (spotSave->m_cutOff != 0)
			{
				while (spotSave != spotLight.GetendItor())
				{
					spotSave->m_cutOff = 0;
					spotSave->m_ambientIntensity = 0;
					++spotSave;
				}
				JEUpdateSpotLight();
			}
		}

		PointLight::pointIter pointSave = pointLight.GetBeginItor();
		if (pointSave != pointLight.GetendItor())
		{
			if (pointSave->m_constant != 0)
			{
				while (pointSave != pointLight.GetendItor())
				{
					pointSave->m_constant = 0.f;
					pointSave->m_linear = 0.f;
					++pointSave;
				}
				JEUpdatePointLight();
			}
		}

		//Enable location 0 in shader.vert
		glEnableVertexAttribArray(0);
		//Enable location 1 in shader.vert
		glEnableVertexAttribArray(1);

		glEnableVertexAttribArray(2);

		//Active vertex buffer
		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);

		//connect 'in' variable in the shader.vert
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(JEVertex), 0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(JEVertex), (const GLvoid*)12);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(JEVertex), (const GLvoid*)20);

		//Active indices buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesBufferObject);

		//Draw the vertices by drawing order
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		//Disable location 0 in shader.vert
		glDisableVertexAttribArray(0);
		//Disable location 1 in shader.vert
		glDisableVertexAttribArray(1);

		glDisableVertexAttribArray(2);

		for (unsigned int i = 0; i <root->childCount; i++)
			BoneDrawHUD(root->child[i], transX, transY, transZ);

	}

	void BoneUpdateTime(Bone* root, double deltaTime)
	{
		deltaTime *= CHANGE_DELTATIME_INT;
		for (unsigned int i = 0; i < root->NumberOfAction; i++)
		{
			for (unsigned int j = 0; j < root->keyframeCount[i]; j++)
			{
			  root->keyframe[i][j].time = unsigned int(root->copyTime[i][j].time * deltaTime);
			}

		}
	}

	/*******************************************************************************
	Function: BoneAnimate

	Description: This function makes animation for bones

	Inputs: root - Bone pointer
	time - Frame time
	type - Aciton time

	Outputs: None.
	*******************************************************************************/
	void BoneAnimate(Bone* root,
		bool& initFrame,
		int time,
		int actionType)
	{
		unsigned int i;

		float ang,
			len,
			tim;

		for (i = 0; i < root->keyframeCount[actionType]; i++)
		{
			if (root->keyframe[actionType][i].time == (unsigned)time)
			{
				if (i != root->keyframeCount[actionType] - 1)
				{
					tim = (float)(root->keyframe[actionType][i + 1].time - root->keyframe[actionType][i].time);
					ang = (float)(root->keyframe[actionType][i + 1].angle - root->keyframe[actionType][i].angle);
					len = (float)(root->keyframe[actionType][i + 1].length - root->keyframe[actionType][i].length);

					if (ang >= 180 || ang <= -180)
					{
						if (ang >= 0)
						{
							ang = 360 - ang;
							ang *= -1;
						}
						else if (ang < 0)
						{
							ang = 360 + ang;
						}
					}

					root->offA = ang / tim;
					root->offL = len / tim;
				}
				else
				{
					root->offA = 0;
					root->offL = 0;
				}
			}
			else if (root->keyframe[actionType][i].time > (unsigned)time)
				initFrame = true;
		}
	}

	bool BonePressAnimate(Bone *root, int time, int actionType)
	{
		unsigned int i;
		bool initFrame = 0;

		BoneAnimate(root, initFrame, time, actionType);

		root->a += root->offA;
		root->l += root->offL;

		for (i = 0; i < root->childCount; i++)
			if (BonePressAnimate(root->child[i], time, actionType))
				initFrame = true;

		return initFrame;
	}

	bool BoneTriggerAnimate(Bone *root, int time, int actionType, bool& checkTrigger)
	{
		unsigned int i;
		bool initFrame = 0;

		BoneAnimate(root, initFrame, time, actionType);

		root->a += root->offA;
		root->l += root->offL;

		if ((unsigned)time == root->keyframe[actionType][root->keyframeCount[actionType] - 1].time && time != 0)
		{
			checkTrigger = false;
		}

		for (i = 0; i < root->childCount; i++)
		{
			if (BoneTriggerAnimate(root->child[i], time, actionType, checkTrigger))
			{
				initFrame = true;
			}
		}
		return initFrame;
	}

	bool BoneGrabAnimate(Bone *root, int time, int actionType, bool& checkTrigger, bool& checkPressed)
	{
		unsigned int i;
		bool initFrame = 0;

		BoneAnimate(root, initFrame, time, actionType);

		if ((unsigned)time > root->keyframe[actionType][root->keyframeCount[actionType] - root->keyframeCount[actionType]/2].time)
		{
			checkPressed = true;
		}

		if ((unsigned)time == root->keyframe[actionType][root->keyframeCount[actionType] - 1].time)
		{
			checkTrigger = false;
		}

		root->a += root->offA;
		root->l += root->offL;

		for (i = 0; i < root->childCount; i++)
		{
			if (BoneGrabAnimate(root->child[i], time, actionType, checkTrigger, checkPressed))
			{
				initFrame = true;
			}
		}
		return initFrame;
	}
	/*******************************************************************************
	Function: BoneEndAnimate

	Description: This function ends bone animation

	Inputs: root - Bone pointer

	Outputs: None.
	*******************************************************************************/
	void BoneEndAnimate(Bone *root)
	{
		root->a = root->initA;
		root->l = root->initL;

		for (unsigned int i = 0; i < root->childCount; ++i)
		{
			BoneEndAnimate(root->child[i]);
		}
	}
	/*******************************************************************************
	Function: BoneReverse

	Description: This function reverses every bones.

	Inputs: root - Bone pointer

	Outputs: None.
	*******************************************************************************/
	void BoneReverse(Bone *root)
	{
		unsigned int i;
		unsigned int action;

		if (!root)
			return;

		if (root->flip == FLIP_IDLE)
			root->flip = FLIP_Y;
		else
			root->flip = FLIP_IDLE;

		if (root->l)
		{
			float gapAngle = root->a - root->initA;

			if (root->initA >= 0 && root->initA < 90)
			{
				root->initA = root->initA + ((90 - root->initA) * 2);
			}
			else if (root->initA > 90 
				  && root->initA <= 180)
			{
				root->initA = root->initA - ((root->initA - 90) * 2);
			}
			else if (root->initA > 180 
				  && root->initA < 270)
			{
				root->initA = root->initA + ((270 - root->initA) * 2);
			}
			else if (root->initA > 270 
				  && root->initA <= 360)
			{
				root->initA = root->initA - ((root->initA - 270) * 2);
			}

			root->a = root->initA - gapAngle;
			root->offA = -(root->offA);
		}
		
		for (action = 0; action < root->NumberOfAction; action++)
		{
			for (i = 0; i < root->keyframeCount[action]; i++)
			{
				if (root->keyframe[action][i].angle >= 0
					&& root->keyframe[action][i].angle < 90)
				{
					root->keyframe[action][i].angle = root->keyframe[action][i].angle + ((90 - root->keyframe[action][i].angle) * 2);
				}
				else if (root->keyframe[action][i].angle > 90
					&& root->keyframe[action][i].angle <= 180)
				{
					root->keyframe[action][i].angle = root->keyframe[action][i].angle - ((root->keyframe[action][i].angle - 90) * 2);
				}
				else if (root->keyframe[action][i].angle > 180
					&& root->keyframe[action][i].angle < 270)
				{
					root->keyframe[action][i].angle = root->keyframe[action][i].angle + ((270 - root->keyframe[action][i].angle) * 2);
				}
				else if (root->keyframe[action][i].angle > 270
					&& root->keyframe[action][i].angle <= 360)
				{
					root->keyframe[action][i].angle = root->keyframe[action][i].angle - ((root->keyframe[action][i].angle - 270) * 2);
				}
			}
		}
	

		for (unsigned int i = 0; i < root->childCount; ++i)
		{
			BoneReverse(root->child[i]);
		}
	}
	/*******************************************************************************
	Function: BoneInit

	Description: This function initialize bone ID

	Inputs: root - Bone pointer
	path - Path that has ini file


	Outputs: return bone pointer
	*******************************************************************************/
	Bone* BoneInit(Bone *root, char* path)
	{
		char names[MAX_BONECOUNT][20];
		names[0][0] = '\0';
		
		root = BoneLoadStructure(path);

		BoneListNames(root, names);

		return root;
	}
	/*******************************************************************************
	Function: boneUpdate

	Description: This function update bones

	Inputs: root - Bone pointer

	Outputs: None.
	*******************************************************************************/
	void BoneUpdate(Bone *root)
	{
		GraphicStartDraw();
		root;
		//modifying
		//BoneDraw(root);

		GraphicEndDraw();

		return;
	}

	void BoneSetEmptyTextrueID(Bone *root,int bodyID, int emptyID)
	{
		if(!strcmp(root->name, "part1"))
			root->textureID = bodyID;
		/*else if (!strcmp(root->name, "part5"))
			root->textureID = bodyID;*/
		else
			root->textureID = emptyID;

		root->flip = FLIP_IDLE;

		for (unsigned int i = 0; i < root->childCount; ++i)
		{
			//++funCallCounter;
			BoneSetEmptyTextrueID(root->child[i], bodyID, emptyID);
		}
	}

	void JEControlLight(JE::Vector3f color, float ambientIntensity)
	{
		directionalPersLight.ChangeLightAmbientIntensity(ambientIntensity);
		directionalPersLight.ChangeLightColor(color);
	}

	void JECalculateNormals(const unsigned int* pIndices, unsigned int indexCount, JEVertex* pVertices, unsigned int VertexCount)
	{
		for (unsigned int i = 0; i < indexCount; i += 3)
		{
			unsigned int index0 = pIndices[i];
			unsigned int index1 = pIndices[i + 1];
			unsigned int index2 = pIndices[i + 2];

			Vector3f v1 = pVertices[index1].m_pos - pVertices[index0].m_pos;
			Vector3f v2 = pVertices[index2].m_pos - pVertices[index0].m_pos;
			Vector3f Normal = v1.Cross(v2);
			Normal.Normalize();

			pVertices[index0].m_normal += Normal;
			pVertices[index1].m_normal += Normal;
			pVertices[index2].m_normal += Normal;
		}

		for (unsigned int i = 0; i < VertexCount; i++) 
		{
			pVertices[i].m_normal.Normalize();
		}
	}

	void JEInitLight(void)
	{
		directionalPersLight.m_color = Vector3f(1.f, 1.f, 1.f);
		directionalPersLight.m_ambientIntensity = 1.0f;
		directionalPersLight.m_diffuseIntensity = 0.01f;
		directionalPersLight.m_direction = Vector3f(1.0f, -1.0, 0.0);

		directionalOrthoLight.m_color = Vector3f(1.0f, 1.0f, 1.0f);
		directionalOrthoLight.m_ambientIntensity = 1.0f;
		directionalOrthoLight.m_diffuseIntensity = 0.75f;
		directionalOrthoLight.m_direction = Vector3f(1.0f, -1.0, 0.0);

		directionalParticleLight.m_color = Vector3f(1.0f, 1.0f, 1.0f);
		directionalParticleLight.m_ambientIntensity = 1.0f;
		directionalParticleLight.m_diffuseIntensity = 0.75f;
		directionalParticleLight.m_direction = Vector3f(1.0f, -1.0, 0.0);

		directionalHUDParticleLight.m_color = Vector3f(1.0f, 1.0f, 1.0f);
		directionalHUDParticleLight.m_ambientIntensity = 1.0f;
		directionalHUDParticleLight.m_diffuseIntensity = 0.75f;
		directionalHUDParticleLight.m_direction = Vector3f(1.0f, -1.0, 0.0);

		pointLight.m_countOfPoint = 0;
		
		spotLight.m_countOfSpot = 0;

		pDayNight = &(getDayNight());
	}

	void JEUpdatePointLight(void)
	{
		objShaderClass.SetPointLights(pointLight.m_countOfPoint, &pointLight);	
	}

	void JEAddPointLight(int pointID,Vector3f position, Vector3f color, float ambientIntensity, float weakness)
	{
		pointLight.AddPointLight(pointID, position, color, ambientIntensity, weakness);
		++pointLight.m_countOfPoint;
	}

	void JEChangePointLight(int pointID, Vector3f position, Vector3f color, float ambientIntensity, float weakness)
	{
		PointLight::pointIter pSave = pointLight.FindPointLight(pointID);
		pSave->m_position = position;
		pSave->m_color    = color;
		pSave->m_ambientIntensity = ambientIntensity;
		pSave->m_linear           = weakness;
	}

	void JEDeletePointLight(int pointID)
	{
		pointLight.DeletePointLight(pointID);
		--pointLight.m_countOfPoint;
	}

	void JEAddSpotLight(int spotID,Vector3f position, Vector3f color, float angle)
	{
		spotLight.AddSpotLight(spotID, position, color, angle, pGameCamera->getTarget());
		++spotLight.m_countOfSpot;
	}
	void JEUpdateSpotLight(void)
	{
		if (spotLight.m_countOfSpot != 0)
		{
			objShaderClass.SetSpotLights(spotLight.m_countOfSpot, &spotLight);
		}
	}

	void JEChangeSpotLight(int spotID, Vector3f position, Vector3f color, float ambientIntensity, float angle)
	{
		SpotLight::spotIter pSave = spotLight.FindSpotLight(spotID);
		pSave->m_linear = 0.1f;
		pSave->m_position = position;
		pSave->m_cutOff = angle;
		pSave->m_color = color;
		pSave->m_ambientIntensity = ambientIntensity;
	}

	float JEGetAmbientSpotLight(int spotID)
	{
		SpotLight::spotIter pSave = spotLight.FindSpotLight(spotID);
		return pSave->m_ambientIntensity;
	}

	void JEDeleteSpotLight(int spotID)
	{
		spotLight.DeleteSpotLight(spotID);
		--spotLight.m_countOfSpot;
	}

	void JEShutdownLight(void)
	{
		pointLight.FreePointLight();
		spotLight.FreeSpotLight();
		pointLight.m_countOfPoint = 0;
		spotLight.m_countOfSpot = 0;
		JEUpdatePointLight();
		JEUpdateSpotLight();
	}

	float JEGetDirectionalAmbient(void)
	{
		return directionalPersLight.m_ambientIntensity;
	}

	JE::Vector3f JEGetDirectionalColor(void)
	{
		return directionalPersLight.m_color;
	}
	void JEUseObjShader(void)
	{
		objShaderClass.UseProgram();
	}

	void JEParticleInit(void)
	{
		JE::Vector3f particleSystemPos = Vector3f(0.0f, 0.0f, 0.0f);
		m_particleSystem[0].InitParticleSystem("Texture//Meat.tga", particleSystemPos);
	}

	void JEParticleUpdate(double dt)
	{
		Pipeline p;

		p.SetPerspective(persProjInfo);
		p.SetCamera(*pGameCamera);

		//dt *= CHANGE_DELTATIME_INT;
		//dt *= 10;

		m_particleSystem[0].Render(int(dt), p.GetViewPerspectTrans(), pGameCamera->getPos());
	}

}

