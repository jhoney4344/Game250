#include "JEGraphics.h"


static void JEGraphicsRenderContextInit(void)
{
	//A struct the specifies information about the back buffer
	PIXELFORMATDESCRIPTOR pfd;
	
	//The result value after choosing my pixel format
	int pixelFormat;

	//Used to check for errors
	BOOL result;

	//set the values.
	memset(&pfd, 0, sizeof(pfd));

	//Set up information about pixels for buffer and back buffer
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags    = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 32;

	//choose back buffer
	pixelFormat = ChoosePixelFormat(jgpData.deviceContext, &pfd);

	//set back buffer information
	result = SetPixelFormat(jgpData.deviceContext, pixelFormat, &pfd);

	//Create openGL render context
	jgpData.renderContext = wglCreateContext(jgpData.deviceContext);

	//Set the openGL render context
	result = wglMakeCurrent(jgpData.deviceContext, jgpData.renderContext);

}
static void JEGraphicsCreateVertexBuffer(void)
{
	const int VERT_COUNT = 6;
	JEVertex vertArray[6] = {
		{ .5f,  .5f, 0.f, 1.f, 1.f},  /*1*/
		{-.5f,  .5f, 0.f, 0.f, 1.f},  /*2*/
		{-.5f, -.5f, 0.f, 0.f, 0.f},  /*3*/

		{-.5f, -.5f, 0.f, 0.f, 0.f},  /*4*/
		{ .5f, -.5f, 0.f, 1.f, 0.f},  /*5*/
		{ .5f,  .5f, 0.f, 1.f, 1.f}}; /*6*/ 

	GLsizei dataSize = sizeof(JEVertex) * VERT_COUNT;

    //Generate my vbo
    glGenBuffers((GLsizei)1, &vboList.vboID);
    vboList.vertexCount = VERT_COUNT;

    //Set my buffer to the current
    glBindBuffer(GL_ARRAY_BUFFER, vboList.vboID);
    //Fill in data into my vbo
    glBufferData(GL_ARRAY_BUFFER, dataSize, vertArray, GL_STATIC_DRAW);
}


void JEGraphicsInit(HWND window, int width, int height)
{
	jgpData.window = window;
	jgpData.width  = width;
	jgpData.height = height;

	//fix it copy code
	jgpData.fov         = WIN_FOV; 
	jgpData.nearClip    = CAM_NEAR_CLIP;
	jgpData.farClip     = CAM_FAR_CLIP;
	jgpData.aspectRatio = (GLdouble)width/height;

	jgpData.cameraZ     = CAM_DISTANCE;
	jgpData.cameraX     = 0.0;
	jgpData.cameraY     = 0.0;
	jgpData.cameraRot   = 0.f;


	 /*Get Device context for window*/
	jgpData.deviceContext = GetDC(jgpData.window);

	/*This will assert if it fails otherwise gfxData.renderContext is valid*/
	JEGraphicsRenderContextInit();

	/*Give back the resource until we need it again.*/
	ReleaseDC(jgpData.window, jgpData.deviceContext);
	jgpData.deviceContext = 0;
	
	// Black Background
	glClearColor(0.f, 0.f, 0.f, 1.f);

	glPolygonMode(GL_FRONT, GL_FILL);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);/*Font face is Counter clock wise*/
	glCullFace(GL_BACK);/*cull back face triangles*/

	//Enable Texture Mapping 
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Set fill mode
	glPolygonMode(GL_FRONT, GL_FILL);

	//Enable Smooth Shading
    glShadeModel(GL_SMOOTH);

	//Enable Depth setup
	glEnable(GL_DEPTH_TEST);
    glClearDepth(1.0f);
    glDepthFunc(GL_LEQUAL);

	//Set up for position and color
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    
	//Set my viewport
	glViewport(0, 0, jgpData.width, jgpData.height);

	JEGraphicsSetPerspective(jgpData.fov, 
	jgpData.aspectRatio, 
    jgpData.nearClip, 
    jgpData.farClip);

	/*Set my camera*/
	JEGraphicsSetCamera((float)jgpData.cameraX, 
                      (float)jgpData.cameraY,
                      (float)jgpData.cameraZ,
                      (float)jgpData.cameraRot);



	glewInit();
	JEGraphicsCreateVertexBuffer();
}

/*******************************************************************************
   Function: GraphicsShutdown

Description: Close the graphic engine.
     Inputs: None
    Outputs: None
*******************************************************************************/
void GraphicsShutdown(void)
{
	glDeleteBuffers((GLsizei)1, &vboList.vboID);
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(jgpData.renderContext);
	jgpData.renderContext = 0;
}

/*******************************************************************************
   Function: JEGraphicsStartDraw

Description: Sets the starting of drawing.
     Inputs: None
    Outputs: None
*******************************************************************************/

void JEGraphicsStartDraw(void)
{
	jgpData.deviceContext = GetDC(jgpData.window);
	wglMakeCurrent(jgpData.deviceContext, jgpData.renderContext);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

/*******************************************************************************
   Function: JEGraphicsStartDraw

Description: Sets the ending of drawing.
     Inputs: None
    Outputs: None
*******************************************************************************/
void JEGraphicsEndDraw(void)
{
	SwapBuffers(jgpData.deviceContext);
	ReleaseDC(jgpData.window, jgpData.deviceContext);
	jgpData.deviceContext = 0;
}

void JEGraphicsSetTextureCoords(float scaleX, float scaleY, float radians,
                                float transX, float transY)
{
	Mtx44 textureTransForm;
	WEMtx44MakeTransform(&textureTransForm, scaleX, scaleY, radians, transX, transY, 0.f);
	glMatrixMode(GL_TEXTURE);
	glLoadMatrixf(&textureTransForm.m[0][0]);
	glMatrixMode(GL_MODELVIEW);
}


void JEGraphicsDraw(const Mtx44* pWorldMatrix)
{
	//Set the matrix mode
	glMatrixMode(GL_MODELVIEW);
	//Add this matrix to the view
	glPushMatrix();
	//Right now my matrix is the transpose of open gl
	glMultMatrixf((GLfloat*)&pWorldMatrix->m[0][0]);

	//Set the start of my position data
	glVertexPointer(3, GL_FLOAT, sizeof(JEVertex), 0);

	//Set  my texture positions
	glTexCoordPointer(2, GL_FLOAT, sizeof(JEVertex),
		((char*)0 + sizeof(float) * 3));

	//Draw my object
	glDrawArrays(GL_TRIANGLES, 0, vboList.vertexCount);

	//Remove this matrix
	glPopMatrix();
}

void JEGraphicsSetTexture(int textureID)
{
	glBindTexture(GL_TEXTURE_2D, textureID);
}

void JEGraphicsSetBackGroundColor(float red, float green, float blue)
{
	glClearColor(red, green, blue, 1.0);
}

void JEGraphicsSetAttributeColor(unsigned color)
{
	glColor4ubv((const GLubyte*)&color);
}

//fix this function(my style to use shader)
void WEMtx44MakeTransform(Mtx44* pResult, double scaleX, double scaleY, 
                          double radians,
                          double transX, double transY, 
                          double zOrder)
{
  float sinAngle = sinf((float)radians);
  float cosAngle = cosf((float)radians);

  /*Set the first ROW*/
  pResult->m[0][0] = float(scaleX * cosAngle);
  pResult->m[0][1] = float(scaleX * sinAngle);
  pResult->m[0][2] = 0.f;
  pResult->m[0][3] = 0.f;

  /*Set the second ROW*/
  pResult->m[1][0] = float(scaleY * -sinAngle);
  pResult->m[1][1] = float(scaleY * cosAngle);
  pResult->m[1][2] = 0.f;
  pResult->m[1][3] = 0.f;

  /*Set the third ROW*/
  pResult->m[2][0] = 0.f;
  pResult->m[2][1] = 0.f;
  pResult->m[2][2] = 1.0f;
  pResult->m[2][3] = 0.f;
  
  /*Set the Fourth ROW*/
  pResult->m[3][0] = (float)transX;
  pResult->m[3][1] = (float)transY;
  pResult->m[3][2] = (float)zOrder;
  pResult->m[3][3] = 1.0f;
}
// fix your code. this code is just for testing
void JEGraphicsSetToPerspective(void)
{
	JEGraphicsSetPerspective(jgpData.fov, jgpData.aspectRatio,
    jgpData.nearClip,jgpData.farClip);
	JEGraphicsSetCamera((float)jgpData.cameraX, 
    (float)jgpData.cameraY, 
    (float)jgpData.cameraZ,
    (float)jgpData.cameraRot);
}

static void JEGraphicsSetPerspective(GLdouble fov, GLdouble aspectRatio, 
                                     GLdouble nearClip, GLdouble farClip)
{
	/*Set our current view mode*/
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	/*Set up Perspective Matrix*/
	gluPerspective(fov, aspectRatio, nearClip, farClip);
	/*Switch back to Model view*/
	glMatrixMode(GL_MODELVIEW);
}

void JEGraphicsSetCamera(float cameraX, float cameraY,
                                float cameraZ, float cameraRot)
{
  Mtx44 rotMatrix;

  cameraZ = WEMathClamp(cameraZ, (float)MIN_CAM_DISTANCE, (float)MAX_CAM_DISTANCE);

  jgpData.cameraX   = cameraX;
  jgpData.cameraY   = cameraY;
  jgpData.cameraZ   = cameraZ;
  jgpData.cameraRot = cameraRot;


  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(cameraX, cameraY, cameraZ,/*Camera position*/
    cameraX, cameraY, 0.0,    /*Target*/
    0.0, 1.0, 0.0);           /*UpVector*/


  WEMtx44MakeRotateZ(&rotMatrix, cameraRot);
  glMultMatrixf((GLfloat*)&rotMatrix.m[0][0]);


  /*Re calculate world extents*/
  JEGraphicsCalulateWorldExtents();
}

static void JEGraphicsCalulateWorldExtents(void)
{
	  float worldMaxY;
	  float worldMaxX;
	  float worldMinY;
	  float worldMinX;
	  Mtx44 rotMatrix;
	  WEMtx44MakeRotateZ(&rotMatrix, (float)-jgpData.cameraRot);

	  /*Calculate world corners based on camera z*/
	  worldMaxY = (tanf( WEMathDegreeToRadian(.5f * (float)jgpData.fov) ) * 
		(float)jgpData.cameraZ);

	  worldMaxX = worldMaxY * (float)jgpData.aspectRatio;
	  worldMinY = -worldMaxY;
	  worldMinX = -worldMaxX;
  
	  /*Shift the corners based on the camera x and y*/
	  worldMaxY += (float)jgpData.cameraY;
	  worldMinY += (float)jgpData.cameraY;
	  worldMaxX += (float)jgpData.cameraX;
	  worldMinX += (float)jgpData.cameraX;

	  /*then rotate the corner points*/
	  jgpData.worldTopLeft.x = worldMinX*rotMatrix.m[0][0] + worldMaxY*rotMatrix.m[1][0];
	  jgpData.worldTopLeft.y = worldMinX*rotMatrix.m[0][1] + worldMaxY*rotMatrix.m[1][1];

	  jgpData.worldTopRight.x = worldMaxX*rotMatrix.m[0][0] + worldMaxY*rotMatrix.m[1][0];
	  jgpData.worldTopRight.y = worldMaxX*rotMatrix.m[0][1] + worldMaxY*rotMatrix.m[1][1];

	  jgpData.worldBotLeft.x = worldMinX*rotMatrix.m[0][0] + worldMinY*rotMatrix.m[1][0];
	  jgpData.worldBotLeft.y = worldMinX*rotMatrix.m[0][1] + worldMinY*rotMatrix.m[1][1];

	  jgpData.worldBotRight.x = worldMaxX*rotMatrix.m[0][0] + worldMinY*rotMatrix.m[1][0];
	  jgpData.worldBotRight.y = worldMaxX*rotMatrix.m[0][1] + worldMinY*rotMatrix.m[1][1];
}

void WEMtx44MakeRotateZ( Mtx44* pResult, float radians )
{
  float sinAngle = sinf(radians);
  float cosAngle = cosf(radians);

  ZeroMemory(pResult, sizeof(*pResult));
  pResult->m[0][0] = pResult->m[1][1] = cosAngle;
  pResult->m[1][0] = -sinAngle;
  pResult->m[0][1] =  sinAngle;
  pResult->m[2][2] = pResult->m[3][3] = 1.f;
}
float WEMathDegreeToRadian( float degrees )
{
  return (3.1415926535897932384626433832795f / 180.f) * degrees;
}

float WEMathClamp( float x, float low, float high )
{
  if(x < low)
    x = low;
  else if(x > high)
    x = high;

  return x;
}