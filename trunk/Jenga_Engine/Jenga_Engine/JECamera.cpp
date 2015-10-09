/*******************************************************************************
Copyright (C) 2015 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
filename    JECamera.cpp
author      Seho.Joo
email       jsh2958@gmail.com
course      GAM250
assignment  Final milestone
due date    6/12/2015

Brief Description:
This file provides the controller of camera position and zoom.

*******************************************************************************/


#include "JECamera.h"

namespace JE
{
/*******************************************************************************
   Function: Camera

Description: Constructor for initializing variable in camera class.

     Inputs: windowWidth  - Size of screen width 
	         windowHeight - Size of screen height

    Outputs: None.
*******************************************************************************/


	Camera::Camera(int windowWidth, int windowHeight)
	{
		m_windowWidth  = windowWidth;
		m_windowHeight = windowHeight;
		m_pos          = Vector3f(0.0f, 0.0f, 0.0f);
		m_target       = Vector3f(0.0f, 0.0f, 1.0f);
		m_target.Normalize();
		m_up           = Vector3f(0.0f, 1.0f, 0.0f);
	}

/*******************************************************************************
   Function: Camera

Description: Constructor for initializing variable in camera class.

     Inputs: windowWidth  - Size of screen width 
	         windowHeight - Size of screen height
			 pos          - Camera position
			 target       - Look at vector
			 up           - Up vector for camera

    Outputs: None.
*******************************************************************************/
	Camera::Camera(int windowWidth, int windowHeight, const Vector3f& pos, const Vector3f& target, const Vector3f& up)
	{
		m_windowWidth  = windowWidth;
		m_windowHeight = windowHeight;
		m_pos = pos;

		m_target = target;
		m_target.Normalize();

		m_up = up;
		m_up.Normalize();
	}

/*******************************************************************************
   Function: SetCameraPosition

Description: This function decides the camera position.

     Inputs: x - Camera X position that is wanted by user.
	         y - Camera Y position that is wanted by user.

    Outputs: None.
*******************************************************************************/
	void Camera::SetCameraPosition(float x, float y)
	{
		m_pos.m_x = x;
		m_pos.m_y = y;
	}

/*******************************************************************************
   Function: SetCameraZoom

Description: This function controls the camera zoom.

     Inputs: zoomIn        - This variable decides whether the order is zoom in 
	                         or zoom out.
	         magnification - Magnification of zoom in or zoom out.

    Outputs: None.
*******************************************************************************/
	void Camera::SetCameraZoom(bool zoomIn, float magnification)
	{
		if(zoomIn == true)
			//m_target is target z-axis. So this statement zoom in
			m_pos += (m_target * magnification);
		else if(zoomIn == false)
			//m_target is target z-axis. So this statement zoom out
			m_pos -= (m_target * magnification);
	}
}
